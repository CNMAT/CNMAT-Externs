/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009, The Regents of
the University of California (Regents). 
Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: SDIF-editor
DESCRIPTION: Editor/viewer for SDIF files
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "cmmjl/cmmjl_sdif.h"
#include "version.c"
#include <math.h>

typedef struct _sel{
	double click, drag;
} t_sel;

typedef struct _sdifed{
	t_jbox box;
	void *listout;
	t_cmmjl_sdif_buffer sdif_buf;
	t_jrgba bgcolor;
	t_jrgba selcolor;
	t_jrgba linecolor;
	t_jrgb datacolor;
	t_jrgb hilite;

	float freqmin, freqmax;
	float timemin, timemax;
	float contrast;
	//int vertline1, vertline2, horizline1, horizline2;
	// selections along the x and y axes
	t_sel sel_x, sel_y;
	// bitfield:
	// 	first byte = modifiers
	//	second byte = which lines to draw (vertline{1,2}, horizline{1,2})
	unsigned long long state; 
} t_sdifed;

void *sdifed_class;

int main(void);
void *sdifed_new(t_symbol *s, long argc, t_atom *argv);
void sdifed_set(t_sdifed *x, t_symbol *bufname);
void sdifed_paint(t_sdifed *x, t_object *patcherview);
void sdifed_draw_sdif(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_draw_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_assist(t_sdifed *x, void *b, long m, long a, char *s);
void sdifed_free(t_sdifed *x);
void sdifed_error(SDIFresult r, const char *st);
t_max_err sdifed_notify(t_sdifed *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void sdifed_mousedown(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
void sdifed_mousedrag(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
void sdifed_mouseup(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
double sdifed_get_sel_min(t_sel *s);
double sdifed_get_sel_max(t_sel *s);
void sdifed_get_intersection(t_sel *sel_x, t_sel *sel_y, t_rect *r);
void sdifed_post_sel(t_sel *sel_x, t_sel *sel_y);
double sdifed_scale(float f, float min_in, float max_in, float min_out, float max_out);

int main(void){
	t_class *c = class_new("SDIF-editor", (method)sdifed_new, (method)sdifed_free, sizeof(t_sdifed), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR);

	class_addmethod(c, (method)sdifed_paint, "paint", A_CANT, 0);
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)sdifed_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)sdifed_set, "set", A_SYM, 0);
	class_addmethod(c, (method)sdifed_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mouseup, "mouseup", A_CANT, 0);

	CLASS_ATTR_FLOAT(c, "freqmin", 0, t_sdifed, freqmin);
	CLASS_ATTR_FLOAT(c, "freqmax", 0, t_sdifed, freqmax);
	CLASS_ATTR_FLOAT(c, "timemin", 0, t_sdifed, timemin);
	CLASS_ATTR_FLOAT(c, "timemax", 0, t_sdifed, timemax);
	CLASS_ATTR_FLOAT(c, "contrast", 0, t_sdifed, contrast);

	CLASS_STICKY_ATTR(c, "category", 0, "Color");

	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_sdifed, bgcolor);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1.");
	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color");

	CLASS_ATTR_RGBA(c, "selcolor", 0, t_sdifed, selcolor);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selcolor", 0, "0. 0. 0. .25");
	CLASS_ATTR_STYLE_LABEL(c, "selcolor", 0, "rgba", "Selection Color");

	CLASS_ATTR_RGBA(c, "linecolor", 0, t_sdifed, linecolor);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "linecolor", 0, "0. 0. 0. 1.");
	CLASS_ATTR_STYLE_LABEL(c, "linecolor", 0, "rgba", "Line Color");

	CLASS_ATTR_RGBA(c, "datacolor", 0, t_sdifed, datacolor);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "datacolor", 0, "0. 0. 0.");
	CLASS_ATTR_STYLE_LABEL(c, "datacolor", 0, "rgb", "Data Color");

	CLASS_ATTR_RGBA(c, "hilite", 0, t_sdifed, hilite);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "hilite", 0, "1. 0. 0.");
	CLASS_ATTR_STYLE_LABEL(c, "hilite", 0, "rgb", "Highlight Color");

	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "color", 0, "0. 0. 0. 1."); 
    	CLASS_ATTR_STYLE_LABEL(c,"color",0,"rgba","Check Color");

	CLASS_STICKY_ATTR_CLEAR(c, "category");

	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100.");

	class_register(CLASS_BOX, c);
	sdifed_class = c;

	version(0);
	
	return 0;
}

void *sdifed_new(t_symbol *s, long argc, t_atom *argv){
	t_sdifed *x = NULL;
	t_dictionary *d = NULL;
	long boxflags;

	x = (t_sdifed *)object_alloc(sdifed_class);

	x->listout = listout(x);

	// box setup
	if(!(d = object_dictionaryarg(argc, argv))){
		return NULL;
	}

	boxflags = 0
            | JBOX_DRAWFIRSTIN 
		//| JBOX_NODRAWBOX
            | JBOX_DRAWINLAST
		//| JBOX_TRANSPARENT  
    //      | JBOX_NOGROW
		//| JBOX_GROWY
    	    | JBOX_GROWBOTH
    //      | JBOX_HILITE
    	    | JBOX_BACKGROUND
            | JBOX_DRAWBACKGROUND
    //      | JBOX_NOFLOATINSPECTOR
    //      | JBOX_MOUSEDRAGDELTA
    //      | JBOX_TEXTFIELD
            ;

	jbox_new((t_jbox *)x, boxflags, argc, argv);
	x->box.b_firstin = (void *)x;

	// uncomment if we add attributes
	attr_dictionary_process(x, d);

	jbox_ready((t_jbox *)x);

	// variables
	x->freqmin = 0;
	x->freqmax = 22050;
	x->timemin = -1;
	x->timemax = -1;
	x->contrast = 1.;
	x->sel_x.click = x->sel_x.drag = -1;
	x->sel_y.click = x->sel_y.drag = -1;

	// SDIF setup
	cmmjl_sdif_init(sdifed_error);

	return x;
}

void sdifed_set(t_sdifed *x, t_symbol *bufname){
	x->sdif_buf.t_bufferSym = bufname;
	LookupMyBuffer(&(x->sdif_buf));
	if(x->sdif_buf.t_buffer == 0){
		error("SDIF-editor: there is no SDIF-buffer called %s", x->sdif_buf.t_bufferSym->s_name);
		return;
	}
	jbox_redraw(&(x->box));
}

void sdifed_paint(t_sdifed *x, t_object *patcherview){
	t_rect rect;
	t_jrgba c;
	c.red = c.green = c.blue = 0;
	c.alpha = 1.;

	// get graphics context
	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);

	// get our box's rectangle
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	// draw the outline of the box
	jgraphics_set_source_jrgba(g, &c);
	jgraphics_set_line_width(g, 1);
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);

	sdifed_draw_sdif(x, g, &rect);
	sdifed_draw_lines(x, g, &rect);

	jgraphics_stroke(g);

	// make a rectangle
	/*
	t_jrgba hilite;
	hilite.red = 1.;
	hilite.green = 0.;
	hilite.blue = 0.;
	hilite.alpha = 1.;
	jgraphics_set_source_jrgba(g, &hilite);
	jgraphics_set_line_width(g, 1.);
	jgraphics_ellipse(g, 50, 50, 2, 2);
	jgraphics_fill(g);
	jgraphics_stroke(g);
	*/
}

void sdifed_draw_sdif(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	char matrixType[4];
	SDIFmem_Matrix m;
	SDIFmem_Frame f;
	float *data;
	int i, r, c;
	double freqmin = x->freqmin;
	double freqmax = x->freqmax;
	double timemin = x->timemin;
	double timemax = x->timemax;
	double contrast = x->contrast;
	double freq, amp, time;
	int index;
	double freqtohilite_min, freqtohilite_max, timetohilite_min, timetohilite_max;
	t_sel sel_x, sel_y;
	sel_x = x->sel_x;
	sel_y = x->sel_y;

	if(!(x->sdif_buf.t_buf)){
		return;
	}

	if(timemin == -1){
		SDIFbuf_GetMinTime(x->sdif_buf.t_buf, &timemin);
	}
	if(timemax == -1){
		SDIFbuf_GetMaxTime(x->sdif_buf.t_buf, &timemax);
	}

	freqtohilite_min = freqtohilite_max = timetohilite_min = timetohilite_max = -1;
	if(sel_y.click >= 0 && sel_y.drag >= 0){
		// this looks weird because frequencies will go from the bottom of the window
		// increasing towards the top, but the window coords start at 0 at the top
		// and decrease towards the bottom.
		freqtohilite_min = sel_y.click > sel_y.drag ? sel_y.click : sel_y.drag;
		freqtohilite_max = sel_y.click < sel_y.drag ? sel_y.click : sel_y.drag;
		freqtohilite_min = fabsf((freqtohilite_min / rect->height) - 1) * (x->freqmax - x->freqmin) + x->freqmin;
		freqtohilite_max = fabsf((freqtohilite_max / rect->height) - 1) * (x->freqmax - x->freqmin) + x->freqmin;
		//post("%f %f", freqtohilite_min, freqtohilite_max);
	}

	if(sel_x.click >= 0 && sel_x.drag >= 0){
		timetohilite_min = sel_x.click < sel_x.drag ? sel_x.click : sel_x.drag;
		timetohilite_max = sel_x.click > sel_x.drag ? sel_x.click : sel_x.drag;
		timetohilite_min = (timetohilite_min / rect->width) * (timemax - timemin) + timemin;
		timetohilite_max = (timetohilite_max / rect->width) * (timemax - timemin) + timemin;
		//post("timetohilite_min = %f timetohilite_max = %f", timetohilite_min, timetohilite_max);
	}

	cmmjl_sdif_getMainMatrixType(&(x->sdif_buf), matrixType);

	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, timemin, 1);

	t_jrgba datacolor;
	datacolor.red = x->datacolor.red;
	datacolor.green = x->datacolor.green;
	datacolor.blue = x->datacolor.blue;
	datacolor.alpha = 1.;

	t_jrgba hilite;
	hilite.red = x->hilite.red;
	hilite.green = x->hilite.green;
	hilite.blue = x->hilite.blue;
	hilite.alpha = 1.;

	jgraphics_set_line_width(g, 1.);

	while(f){
		if(!(m = GetMatrix(&(x->sdif_buf), matrixType, f->header.time, 1))){
			error("SDIF-editor: error!!");
			return;
		}

		if(m->header.matrixDataType == SDIF_FLOAT64){
			error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
			return;
		}

		time = f->header.time;
		r = m->header.rowCount;
		c = m->header.columnCount;
		data = (float *)(m->data);
		if(time <= timemax && time >= timemin){
			for(i = 0; i < r; i++){
				index = (int)(data[(i * c)]);
				freq = data[(i * c) + 1];
				amp = data[(i * c) + 2];

				if(freq <= freqmax && freq >= freqmin){
					if((freq <= freqtohilite_max && freq >= freqtohilite_min) || 
					   (time <= timetohilite_max && time >= timetohilite_min)){
						jgraphics_set_source_jrgba(g, &hilite);
					}else{
						datacolor.alpha = amp * contrast;
						jgraphics_set_source_jrgba(g, &datacolor);
					}
					freq = (freq - freqmin) / (freqmax - freqmin);
					jgraphics_rectangle(g, rect->width * ((time - timemin) / (timemax - timemin)), roundf(fabsf(freq - 1) * rect->height), 1, 1);
					jgraphics_stroke(g);
				}
			}
		}
		f = f->next;
	}
}

void sdifed_draw_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	double xmin, xmax, ymin, ymax;
	xmin = sdifed_get_sel_min(&(x->sel_x));
	xmax = sdifed_get_sel_max(&(x->sel_x));
	ymin = sdifed_get_sel_min(&(x->sel_y));
	ymax = sdifed_get_sel_max(&(x->sel_y));

	jgraphics_set_source_jrgba(g, &(x->linecolor));
	jgraphics_set_line_width(g, 0.5);

	jgraphics_move_to(g, xmin, 0);
	jgraphics_line_to(g, xmin, rect->height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, xmax, 0);
	jgraphics_line_to(g, xmax, rect->height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, 0, ymin);
	jgraphics_line_to(g, rect->width, ymin);
	jgraphics_stroke(g);

	jgraphics_move_to(g, 0, ymax);
	jgraphics_line_to(g, rect->width, ymax);
	jgraphics_stroke(g);

	jgraphics_set_source_jrgba(g, &(x->selcolor));
	if(xmin >= 0 && xmax >= 0 && (ymin < 0 || ymax < 0)){
		jgraphics_rectangle(g, xmin, 0, xmax - xmin, rect->height);
		jgraphics_fill(g);
	}else if(ymin >= 0 && ymax >= 0 && (xmin < 0 || xmax < 0)){
		jgraphics_rectangle(g, 0, ymin, rect->width, ymax - ymin);
		jgraphics_fill(g);
	}
}

void sdifed_assist(t_sdifed *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}else{
		switch (a) {	
		case 0:
			break;
		}
	}
}

void sdifed_free(t_sdifed *x){
	jbox_free((t_jbox *)x);
}

void sdifed_error(SDIFresult r, const char *st){
	if(r) error("SDIF-editor: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIF-editor: %s", st);
}

t_max_err sdifed_notify(t_sdifed *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	//t_symbol *attrname;
	if (msg == gensym("attr_modified")){
		//attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1;
		jbox_redraw(&(x->box));

        }
        return 0;
}

void sdifed_output_matrix_for_time(t_sdifed *x, double time){

}

// 0x10 = no modifiers
// 0x11 = command
// 0x12 = shift
// 0x94 = control
// 0x18 = option
void sdifed_mousedown(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	//post("0x%X", modifiers);
	switch(modifiers){
	case 0x10:
		post("0");
		// no modifiers.  
		x->sel_x.click = pt.x;
		x->sel_x.drag = -1;
		x->sel_y.click = -1;
		x->sel_y.drag = -1;
		break;
	case 0x12:
		post("shift");
		// shift.  
		x->sel_x.click = -1;
		x->sel_x.drag = -1;
		x->sel_y.click = pt.y;
		x->sel_y.drag = -1;
		break;
	case 0x13:
		x->sel_x.click = pt.x;
		x->sel_x.drag = -1;
		x->sel_y.click = pt.y;
		x->sel_y.drag = -1;
		break;
	}
	jbox_redraw(&(x->box));
	sdifed_post_sel(&(x->sel_x), &(x->sel_y));
}

void sdifed_mousedrag(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	switch(modifiers){
	case 0x10:
		x->sel_x.drag = pt.x;
		x->sel_y.click = x->sel_y.drag = -1;
		break;
	case 0x12:
		x->sel_x.click = x->sel_x.drag = -1;
		x->sel_y.drag = pt.y;
		break;
	case 0x13:
		x->sel_x.drag = pt.x;
		x->sel_y.drag = pt.y;
		break;
	}

	jbox_redraw(&(x->box));
}

void sdifed_mouseup(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect rect;
        jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
}

double sdifed_get_sel_min(t_sel *s){
	if(s->click < s->drag){
		return s->click;
	}else{
		return s->drag;
	}
}

double sdifed_get_sel_max(t_sel *s){
	if(s->click < s->drag){
		return s->drag;
	}else{
		return s->click;
	}
}

void sdifed_get_intersection(t_sel *sel_x, t_sel *sel_y, t_rect *r){
	double xmin, ymin;
	xmin = sdifed_get_sel_min(sel_x);
	ymin = sdifed_get_sel_min(sel_y);
	r->x = xmin;
	r->y = ymin;
	r->width = sdifed_get_sel_max(sel_x) - xmin;
	r->height = sdifed_get_sel_max(sel_y) - ymin;
}

void sdifed_post_sel(t_sel *sel_x, t_sel *sel_y){
	post("%f %f %f %f", sel_x->click, sel_x->drag, sel_y->click, sel_y->drag);
}

double sdifed_scale(float f, float min_in, float max_in, float min_out, float max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}
