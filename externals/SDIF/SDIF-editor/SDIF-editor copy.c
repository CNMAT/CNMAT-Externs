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
	double min, max;
} t_sel;

typedef struct _sdifed{
	t_jbox box;
	void *listout;
	t_cmmjl_sdif_buffer sdif_buf;
	t_jrgba outline;
	t_jrgba background;
	t_jrgba linecolor;
	float freqmin, freqmax;
	float timemin, timemax;
	float contrast;
	//int vertline1, vertline2, horizline1, horizline2;
	t_sel sel_h, sel_v;
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
void sdifed_set_sel(t_sel *s, double v1, double v2);
void sdifed_get_intersection(t_sel *sel_x, t_sel *sel_y, t_range *r);

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

	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "color", 0, "0. 0. 0. 1."); 
	CLASS_ATTR_STYLE_LABEL(c, "color", 0, "rgba", "Check Color");

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
	//attr_dictionary_process(x, d);

	jbox_ready((t_jbox *)x);

	// variables
	x->freqmin = 0;
	x->freqmax = 22050;
	x->timemin = -1;
	x->timemax = -1;
	x->contrast = 100;
	x->vertline1 = x->vertline2 = x->horizline1 = x->horizline2 = -1;

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
	//sdifed_process(x);
}

void sdifed_paint(t_sdifed *x, t_object *patcherview){
	t_rect rect;

	// get graphics context
	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);

	// get our box's rectangle
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	// draw the outline of the box
	jgraphics_set_source_jrgba(g, &x->outline);
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
	double freq_ar[SDIFED_BUFSIZE];
	double prevtime;
	int i, r, c;
	double freqmin = x->freqmin;
	double freqmax = x->freqmax;
	double timemin = x->timemin;
	double timemax = x->timemax;
	double contrast = x->contrast;
	double freq, amp, time;
	int index;
	double freqtohilite_min, freqtohilite_max, timetohilite_min, timetohilite_max;

	if(!(x->sdif_buf.t_buf)){
		return;
	}

	if(timemin == -1){
		SDIFbuf_GetMinTime(x->sdif_buf.t_buf, &timemin);
	}
	if(timemax == -1){
		SDIFbuf_GetMaxTime(x->sdif_buf.t_buf, &timemax);
	}
	post("timemin = %f, timemax = %f", timemin, timemax);

	freqtohilite_min = freqtohilite_max = timetohilite_min = timetohilite_max = -1;
	if(x->horizline1 >= 0 && x->horizline2 >= 0){
		// this looks weird because frequencies will go from the bottom of the window
		// increasing towards the top, but the window coords start at 0 at the top
		// and decrease towards the bottom.
		freqtohilite_min = x->horizline1 > x->horizline2 ? x->horizline1 : x->horizline2;
		freqtohilite_max = x->horizline1 < x->horizline2 ? x->horizline1 : x->horizline2;
		freqtohilite_min = fabsf((freqtohilite_min / rect->height) - 1) * (x->freqmax - x->freqmin) + x->freqmin;
		freqtohilite_max = fabsf((freqtohilite_max / rect->height) - 1) * (x->freqmax - x->freqmin) + x->freqmin;
		post("%f %f", freqtohilite_min, freqtohilite_max);
	}

	if(x->vertline1 >= 0 && x->vertline2 >= 0){
		timetohilite_min = x->vertline1 < x->vertline2 ? x->vertline1 : x->vertline2;
		timetohilite_max = x->vertline1 > x->vertline2 ? x->vertline1 : x->vertline2;
		timetohilite_min = (timetohilite_min / rect->width) * (timemax - timemin) + timemin;
		timetohilite_max = (timetohilite_max / rect->width) * (timemax - timemin) + timemin;
		post("timetohilite_min = %f timetohilite_max = %f", timetohilite_min, timetohilite_max);
	}

	cmmjl_sdif_getMainMatrixType(&(x->sdif_buf), matrixType);

	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, timemin, 1);
	if(!(m = GetMatrix(&(x->sdif_buf), matrixType, f->header.time, 1))){
		error("SDIF-editor: error!!");
		return;
	}

	if(m->header.matrixDataType == SDIF_FLOAT64){
		error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
		return;
	}

	data = (float *)(m->data);
	//post("%f %f %f %f", data[0], data[1], data[2], data[3]);
	r = m->header.rowCount;
	c = m->header.columnCount;
	time = f->header.time;

	t_jrgba hilite;
	hilite.red = 1.;
	hilite.green = 0.;
	hilite.blue = 0.;
	hilite.alpha = 1.;
	jgraphics_set_source_jrgba(g, &hilite);
	jgraphics_set_line_width(g, 1.);

	//post("c = %d, r = %d", c, r);
	//post("timemin = %f, timemax = %f, time = %f", timemin, timemax, time);

	if(time <= timemax && time >= timemin){
		for(i = 0; i < r; i++){
			index = (int)(data[(i * c)]);
			freq = data[(i * c) + 1];
			amp = data[(i * c) + 2];
			//post("%d %f %f %f %f", index, freq, amp, freqmin, freqmax);
			if(freq <= freqmax && freq >= freqmin){
				hilite.red = hilite.green = hilite.blue = powf(fabsf(amp - 1), contrast);
				if((freq <= freqtohilite_max && freq >= freqtohilite_min) || 
				   (time <= timetohilite_max && time >= timetohilite_min)){
					//post("freq = %f", freq);
					hilite.green = hilite.blue = 0;
				}
				
				jgraphics_set_source_jrgba(g, &hilite);
				//freq_ar[index] = ((freqmax - freq) / (freq - freqmin)) / 2.;
				freq_ar[index] = (freq - freqmin) / (freqmax - freqmin);
				//post("freq_ar[%d] = %f %f", index, freq_ar[index], (freq - freqmin) / (freqmax - freqmin));

				//post("stupid: %f", roundf(fabsf(freq_ar[index] - 1) * rect->height));
				jgraphics_rectangle(g, 0, roundf(fabsf(freq_ar[index] - 1) * rect->height), 1, 1);
				jgraphics_stroke(g);
			}
		}
	}

	prevtime = time;

	while(f->next){
		f = f->next;
		time = f->header.time;
		r = m->header.rowCount;
		c = m->header.columnCount;
		//post("time = %f", f->header.time);
		if(!(m = GetMatrix(&(x->sdif_buf), matrixType, f->header.time, 1))){
			error("SDIF-editor: error!!");
			return;
		}
		data = (float *)(m->data);
		//post("%f %f %f %f", data[0], data[1], data[2], data[3]);
		if(time <= timemax && time >= timemin){
			for(i = 0; i < r; i++){
				index = (int)(data[(i * c)]);
				freq = data[(i * c) + 1];
				amp = data[(i * c) + 2];
				//post("%d %f %f %f %f %f %f", index, freq, amp, freqmin, freqmax, freqtohilite_min, freqtohilite_max);
				if(freq <= freqmax && freq >= freqmin){
					hilite.red = hilite.green = hilite.blue = powf(fabsf(amp - 1), contrast);
					if(freq <= freqtohilite_max && freq >= freqtohilite_min || 
					   (time <= timetohilite_max && time >= timetohilite_min)){
						//post("freq = %f", freq);
						hilite.green = hilite.blue = 0;
					}
					jgraphics_set_source_jrgba(g, &hilite);
					//freq_ar[index] = ((freqmax - freq) / (freq - freqmin)) / 2.;
					freq_ar[index] = (freq - freqmin) / (freqmax - freqmin);
					//post("freq_ar[%d] = %f %f", index, freq_ar[index], (freq - freqmin) / (freqmax - freqmin));

					//post("stupid: %f", roundf(fabsf(freq_ar[index] - 1) * rect->height));
					jgraphics_rectangle(g, rect->width * ((time - timemin) / (timemax - timemin)), roundf(fabsf(freq_ar[index] - 1) * rect->height), 1, 1);
					//post("%f %f", rect->width * ((time - timemin) / (timemax - timemin)), roundf(fabsf(freq_ar[index] - 1) * rect->height));
					jgraphics_stroke(g);
				}
			}
		}
	}
}

void sdifed_draw_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	t_jrgba c;
	c.red = 0.;
	c.green = 0.;
	c.blue = 0.;
	c.alpha = 1.;
	jgraphics_set_source_jrgba(g, &c);
	jgraphics_set_line_width(g, 1.);

	jgraphics_move_to(g, x->vertline1, 0);
	jgraphics_line_to(g, x->vertline1, rect->height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, x->vertline2, 0);
	jgraphics_line_to(g, x->vertline2, rect->height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, 0, x->horizline1);
	jgraphics_line_to(g, rect->width, x->horizline1);
	jgraphics_stroke(g);

	jgraphics_move_to(g, 0, x->horizline2);
	jgraphics_line_to(g, rect->width, x->horizline2);
	jgraphics_stroke(g);

	c.alpha = .25;
	jgraphics_set_source_jrgba(g, &c);
	if(x->vertline1 >= 0 && x->vertline2 >= 0 && (x->horizline1 < 0 || x->horizline2 < 0)){
		jgraphics_rectangle(g, x->vertline1, 0, x->vertline2 - x->vertline1, rect->height);
		jgraphics_fill(g);
	}else if(x->horizline1 >= 0 && x->horizline2 >= 0 && (x->vertline1 < 0 || x->vertline2 < 0)){
		jgraphics_rectangle(g, 0, x->horizline1, rect->width, x->horizline2 - x->horizline1);
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
	t_symbol *attrname;
        
	if (msg == gensym("attr_modified")){
		//attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		x->vertline1 = x->vertline2 = x->horizline1 = x->horizline2 = -1;
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
	//t_rect rect;
        //jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
	switch(modifiers){
	case 0x10:
		// single vertical line at mouse position
		x->vertline1 = (int)(pt.x);
		x->vertline2 = -1;
		x->horizline1 = -1;
		x->horizline2 = -1;
		break;
	case 0x12:
		// two vertical lines
		x->vertline1 = (int)(pt.x);
		x->vertline2 = (int)(pt.x);
		x->horizline1 = -1;
		x->horizline2 = -1;
		break;
	case 0x18:
		// one horizontal line
		x->horizline1 = (int)(pt.y);
		x->horizline2 = -1;
		x->vertline1 = -1;
		x->vertline2 = -1;
		break;
	case 0x1A:
		x->horizline1 = (int)(pt.y);
		x->horizline2 = (int)(pt.y);
		x->vertline1 = -1;
		x->vertline2 = -1;
		break;
	case 0x1B:
		x->vertline1 = (int)(pt.x);
		x->vertline2 = (int)(pt.x);
		x->horizline1 = (int)(pt.y);
		x->horizline2 = (int)(pt.y);
		break;
	}

	jbox_redraw(&(x->box));
	//post("0x%X", modifiers);
}

void sdifed_mousedrag(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	switch(modifiers){
	case 0x10:
		// single vertical line at mouse position
		x->vertline1 = (int)(pt.x);
		x->vertline2 = -1;
		break;
	case 0x12:
		// two vertical lines--only the second one follows the mouse
		//x->vertline1 = (int)(pt.x);
		x->vertline2 = (int)(pt.x);
		break;
	case 0x18:
		// one horizontal line
		x->horizline1 = (int)(pt.y);
		x->horizline2 = -1;
		break;
	case 0x1A:
		// two horizontal lines--only the second one follows the mouse
		//x->horizline1 = (int)(pt.y);
		x->horizline2 = (int)(pt.y);
		break;
	case 0x1B:
		// a box--only the second lines follow the mouse
		//x->vertline1 = (int)(pt.x);
		x->vertline2 = (int)(pt.x);
		//x->horizline1 = (int)(pt.y);
		x->horizline2 = (int)(pt.y);
		break;
	}

	jbox_redraw(&(x->box));
}

void sdifed_mouseup(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect rect;
        jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
}

void sdifed_set_sel(t_sel *s, double v1, double v2){
	if(v1 < v2){
		s->min = v1;
		s->max = v2;
	}else{
		s->max = v1;
		s->min = v2;
	}
}

void sdifed_get_intersection(t_sel *sel_x, t_sel *sel_y, t_range *r)
	r->x = sel_x->min;
	r->y = sel_y->min;
	r->width = sel_x->max - sel_x->min;
	r->height = sel_y->max - sel_y->min;
}
