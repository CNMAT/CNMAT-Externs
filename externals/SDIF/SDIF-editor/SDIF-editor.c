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

#define SDIFED_BUF_SIZE 2048

#define SDIFED_LAYER_BACKGROUND 0
#define SDIFED_LAYER_SDIF 1
#define SDIFED_LAYER_SEL_LINES 2
#define SDIFED_LAYER_SEL 3

#ifndef VPOST()
#define VPOST(vv, str, args...) (((vv) > 0) ? post("%s:%d: " str, NAME, __LINE__, ##args) : 0)
#endif

typedef struct _sel{
	double click, drag;
} t_sel;

typedef struct _trackstats{
	float amp_min;
	float amp_max;
	float freq_min;
	float freq_max;
} t_trackstats;

typedef struct _sdifed{
	t_jbox box;
	void *out_data, *out_info;
	t_cmmjl_sdif_buffer sdif_buf;
	char matrix_type[4];
	// if the matrix is 1TRC or something else that has track numbers
	// we can store the max number here.
	int numTracks; 
	double delta_avg;
	int framecount;
	t_jrgba bgcolor;
	t_jrgba selcolor;
	t_jrgba linecolor;
	t_jrgb datacolor;
	t_jrgb hilite;
	double single_frame_width;
	double freqmin_disp, freqmax_disp;
	double freqmin_sdif, freqmax_sdif;
	double timemin_disp, timemax_disp;
	double timemin_sdif, timemax_sdif;
	float contrast;
	t_sel sel_x, sel_y;
	t_atom *buf;
	int v; // verbose
	unsigned long long state;
	t_trackstats *trackstats;
	// these should be marked non-zero to indicate that a layer should be redrawn
	int layerstate[4]; 
} t_sdifed;

void *sdifed_class;

int main(void);
void *sdifed_new(t_symbol *s, long argc, t_atom *argv);
void sdifed_set(t_sdifed *x, t_symbol *bufname);
void sdifed_paint(t_sdifed *x, t_object *patcherview);
void sdifed_draw_sdif(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_draw_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_draw_track_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_draw_single_frame(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_draw_bounds(t_sdifed *x, t_jgraphics *g, t_rect *rect);
void sdifed_assist(t_sdifed *x, void *b, long m, long a, char *s);
void sdifed_free(t_sdifed *x);
void sdifed_error(SDIFresult r, const char *st);
t_max_err sdifed_notify(t_sdifed *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void sdifed_output_current_frame(t_sdifed *x);
void sdifed_mousedown(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
void sdifed_mousedrag(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
void sdifed_mouseup(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers);
double sdifed_get_sel_min(t_sel *s);
double sdifed_get_sel_max(t_sel *s);
void sdifed_get_intersection(t_sel *sel_x, t_sel *sel_y, t_rect *r);
void sdifed_post_sel(t_sel *sel_x, t_sel *sel_y);
void sdifed_output_sel(t_sdifed *x);
double sdifed_scale(double f, double min_in, double max_in, double min_out, double max_out);
void sdifed_zoom_in(t_sdifed *x);
void sdifed_zoom_out(t_sdifed *x);
void sdifed_zero_sel(t_sel *sel);
void sdifed_output_bounds(t_sdifed *x);

t_symbol *ps_sdifLayer;

int main(void){
	t_class *c = class_new("SDIF-editor", (method)sdifed_new, (method)sdifed_free, sizeof(t_sdifed), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR);

	class_addmethod(c, (method)sdifed_paint, "paint", A_CANT, 0);
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)sdifed_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)sdifed_set, "set", A_SYM, 0);
	class_addmethod(c, (method)sdifed_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)sdifed_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method)sdifed_zoom_in, "zoom_in", 0);
	class_addmethod(c, (method)sdifed_zoom_out, "zoom_out", 0);

	CLASS_ATTR_DOUBLE(c, "freqmin", 0, t_sdifed, freqmin_disp);
	CLASS_ATTR_DOUBLE(c, "freqmax", 0, t_sdifed, freqmax_disp);
	CLASS_ATTR_DOUBLE(c, "timemin", 0, t_sdifed, timemin_disp);
	CLASS_ATTR_DOUBLE(c, "timemax", 0, t_sdifed, timemax_disp);
	CLASS_ATTR_FLOAT(c, "contrast", 0, t_sdifed, contrast);
	CLASS_ATTR_DOUBLE(c, "single_frame_width", 0, t_sdifed, single_frame_width);
	CLASS_ATTR_LONG(c, "verbose", 0, t_sdifed, v);

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

	ps_sdifLayer = gensym("sdifLayer");

	version(0);
	
	return 0;
}

void *sdifed_new(t_symbol *s, long argc, t_atom *argv){
	t_sdifed *x = NULL;
	t_dictionary *d = NULL;
	long boxflags;

	x = (t_sdifed *)object_alloc(sdifed_class);

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

	x->out_info = outlet_new((t_object *)x, NULL);
	x->out_data = listout((t_object *)x);


	// uncomment if we add attributes
	attr_dictionary_process(x, d);

	jbox_ready((t_jbox *)x);

	// variables
	x->freqmin_disp = 0;
	x->freqmax_disp = 22050;
	x->freqmin_sdif = 0;
	x->freqmax_sdif = 22050;
	x->timemin_disp = 0;
	x->timemax_disp = 1;
	x->contrast = 10.;
	x->sel_x.click = x->sel_x.drag = -1;
	x->sel_y.click = x->sel_y.drag = -1;
	x->single_frame_width = 100;
	x->buf = (t_atom *)malloc(SDIFED_BUF_SIZE * sizeof(t_atom));
	x->v = 1;
	/*
	x->sel[SDIFED_LAYER_BACKGROUND] = 1;
	x->sel[SDIFED_LAYER_SDIF] = 1;
	x->sel[SDIFED_LAYER_SEL_LINES] = 1;
	x->sel[SDIFED_LAYER_SEL] = 1;
	*/
	// SDIF setup
	cmmjl_sdif_init(sdifed_error);

	return x;
}

void sdifed_set(t_sdifed *x, t_symbol *bufname){
	SDIFmem_Matrix m;
	SDIFmem_Frame f;
	float *data;
	double time;
	int i, r, c, framecount, numTracks;

	x->sdif_buf.t_bufferSym = bufname;
	LookupMyBuffer(&(x->sdif_buf));
	if(x->sdif_buf.t_buffer == 0){
		error("SDIF-editor: there is no SDIF-buffer called %s", x->sdif_buf.t_bufferSym->s_name);
		return;
	}

	// get the max and min times in the SDIF file
	SDIFbuf_GetMinTime(x->sdif_buf.t_buf, &(x->timemin_sdif));
	SDIFbuf_GetMaxTime(x->sdif_buf.t_buf, &(x->timemax_sdif));
	VPOST(x->v, "min time = %f\nmax time = %f", x->timemin_sdif, x->timemax_sdif);
	x->timemin_disp = x->timemin_sdif;
	//x->timemax_disp = x->timemax_sdif;

	if(x->timemax_sdif > 10.){
		x->timemax_disp = 10.;
	}else{
		x->timemax_disp = x->timemax_sdif;
	}


	// figure out what the main matrix type is
	cmmjl_sdif_getMainMatrixType(&(x->sdif_buf), x->matrix_type);
	if(strncmp(x->matrix_type, "1TRC", 4)){
		error("SDIF-editor: matrix type %c%c%c%c is not supported at this time (sorry)", 
		      x->matrix_type[0],
		      x->matrix_type[1],
		      x->matrix_type[2],
		      x->matrix_type[3]);
		return;
	}
	VPOST(x->v, "matrix type = %c%c%c%c", 
	      x->matrix_type[0], 
	      x->matrix_type[1], 
	      x->matrix_type[2], 
	      x->matrix_type[3]);

	// get the first frame
	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, x->timemin_disp, ESDIF_SEARCH_FORWARDS);

	if(!strncmp(x->matrix_type, "1TRC", 4)){
		x->numTracks = 0;
	}

	double prevdelta = 0;
	double delta_avg = 0;
	framecount = 0;
	time = 0;

	// compute the average time between frames
	while(f){
		delta_avg += f->header.time - time;
		time = f->header.time;

		if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
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

		f = f->next;
		framecount++;
	}

	x->delta_avg = delta_avg / ((double)framecount);
	x->framecount = framecount;

	VPOST(x->v, "framecount = %d, numTracks = %d", x->framecount, x->numTracks);
	//post("nFrames = %f", (x->timemax_sdif - x->timemin_sdif) / (delta_avg / (double)i));


	// go through each frame and make sure the data is reasonable and that
	// our buffer (x->buf) is big enough to handle it.
	/*
	while(f){
		if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
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

		if(r * c + 1 > SDIFED_BUF_SIZE){
			x->buf = (t_atom *)realloc(x->buf, (r * c + 1) * sizeof(t_atom));
			if(x->buf == NULL){
				error("SDIF-editor: out of memory!");
				return;
			}
		}

		if(!strncmp(x->matrix_type, "1TRC", 4)){
			for(i = 0; i < r; i++){
				if(data[(r * i)] > x->numTracks){
					x->numTracks = (int)(data[(r * i)]);
				}
			}
		}

		//post("time = %f", f->header.time);
		f = f->next;
	}
	*/

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
	jgraphics_stroke(g);

	jgraphics_set_source_jrgba(g, &(x->bgcolor));
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_fill(g);

	sdifed_draw_track_lines(x, g, &rect);
	/*
	if(x->layerstate[SDIFED_LAYER_SDIF]){
		jbox_invalidate_layer((t_object *)x, patcherview, ps_sdifLayer);
		x->layerstate[SDIFED_LAYER_SDIF] = 0;
	}
	g = jbox_start_layer((t_object *)x, patcherview, ps_sdifLayer, 0, 0);
	*/
	sdifed_draw_sdif(x, g, &rect);
	//jbox_end_layer((t_object *)x, patcherview, ps_sdifLayer);

	sdifed_draw_lines(x, g, &rect);
	//sdifed_draw_single_frame(x, g, &rect);
	sdifed_output_bounds(x);
	//sdifed_draw_bounds(x, g, &rect);

}

void sdifed_draw_sdif(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	SDIFmem_Matrix m;
	SDIFmem_Frame f;
	float *data;
	int i, r, c;
	double freqmin_disp = x->freqmin_disp;
	double freqmax_disp = x->freqmax_disp;
	double timemin_disp = x->timemin_disp;
	double timemax_disp = x->timemax_disp;
	double contrast = x->contrast;
	double freq, amp, time;
	int index;
	int numFramesToSkip;
	double freqtohilite_min, freqtohilite_max, timetohilite_min, timetohilite_max;
	t_sel sel_x, sel_y;
	sel_x = x->sel_x;
	sel_y = x->sel_y;

	if(!(x->sdif_buf.t_buf)){
		return;
	}

	if(timemin_disp == -1){
		timemin_disp = x->timemin_sdif;
	}
	if(timemax_disp == -1){
		timemax_disp = x->timemax_sdif;
	}

	freqtohilite_min = freqtohilite_max = timetohilite_min = timetohilite_max = -1;
	if(sel_y.click >= 0 && sel_y.drag >= 0){
		// this looks weird because frequencies will go from the bottom of the window
		// increasing towards the top, but the window coords start at 0 at the top
		// and decrease towards the bottom.
		freqtohilite_min = sel_y.click > sel_y.drag ? sel_y.click : sel_y.drag;
		freqtohilite_max = sel_y.click < sel_y.drag ? sel_y.click : sel_y.drag;
		freqtohilite_min = fabsf((freqtohilite_min / rect->height) - 1) * (x->freqmax_disp - x->freqmin_disp) + x->freqmin_disp;
		freqtohilite_max = fabsf((freqtohilite_max / rect->height) - 1) * (x->freqmax_disp - x->freqmin_disp) + x->freqmin_disp;
		//post("%f %f", freqtohilite_min, freqtohilite_max);
	}

	if(sel_x.click >= 0 && sel_x.drag >= 0){
		timetohilite_min = sel_x.click < sel_x.drag ? sel_x.click : sel_x.drag;
		timetohilite_max = sel_x.click > sel_x.drag ? sel_x.click : sel_x.drag;
		timetohilite_min = (timetohilite_min / rect->width) * (timemax_disp - timemin_disp) + timemin_disp;
		timetohilite_max = (timetohilite_max / rect->width) * (timemax_disp - timemin_disp) + timemin_disp;
		//post("timetohilite_min = %f timetohilite_max = %f", timetohilite_min, timetohilite_max);
	}

	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, timemin_disp, ESDIF_SEARCH_FORWARDS);

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

	numFramesToSkip = (int)floor(((x->timemax_disp - x->timemin_disp) / x->delta_avg) / rect->width);

	while(f && (f->header.time <= x->timemax_disp)){
		if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
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
		if(time <= timemax_disp && time >= timemin_disp){
			for(i = 0; i < r; i++){
				index = (int)(data[(i * c)]);
				freq = data[(i * c) + 1];
				amp = data[(i * c) + 2];

				if(freq <= freqmax_disp && freq >= freqmin_disp){
					if(x->state == 0x10 || x->state == 0x12){
						if((freq <= freqtohilite_max && freq >= freqtohilite_min) || 
						   (time <= timetohilite_max && time >= timetohilite_min)){
							jgraphics_set_source_jrgba(g, &hilite);
						}else{
							datacolor.alpha = amp * contrast;
							jgraphics_set_source_jrgba(g, &datacolor);
						}
					}else if(x->state == 0x13){
						if((freq <= freqtohilite_max && freq >= freqtohilite_min) && 
						   (time <= timetohilite_max && time >= timetohilite_min)){
							jgraphics_set_source_jrgba(g, &hilite);
						}else{
							datacolor.alpha = amp * contrast;
							jgraphics_set_source_jrgba(g, &datacolor);
						}
					}else{
						datacolor.alpha = amp * contrast;
						jgraphics_set_source_jrgba(g, &datacolor);
					}
					freq = (freq - freqmin_disp) / (freqmax_disp - freqmin_disp);
					jgraphics_rectangle(g, rect->width * ((time - timemin_disp) / (timemax_disp - timemin_disp)), roundf(fabsf(freq - 1) * rect->height), 2, 2);
					jgraphics_fill(g);
				}
			}
		}
		for(i = 0; i < numFramesToSkip + 1; i++){
			f = f->next;
			if(!f){
				break;
			}
		}
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

// if we can assume that a track number will only be used once in the course
// of a file, this function can be greatly simplified.
void sdifed_draw_track_lines(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	SDIFmem_Matrix m;
	SDIFmem_Frame f, ff;
	float *data;
	int r, c, i, j;
	int index;
	float freq, amp;
	double time, prev_time;
	float prev_freq[SDIFED_BUF_SIZE];
	int prev_ind[SDIFED_BUF_SIZE];
	int num_prev_indices;
	double timemin_disp, timemax_disp;
	t_jrgba datacolor;
	double numFramesToSkip = ((x->timemax_disp - x->timemin_disp) / x->delta_avg) / rect->width;
	//post("%f %f %f %f %f", numFramesToSkip, x->timemax_disp, x->timemin_disp, x->delta_avg, rect->width);
	if(numFramesToSkip > .25){
		post("numFramesToSkip = %f, __NOT__ drawing lines");
		return;
	}
	post("numFramesToSkip = %f, drawing lines");

	datacolor.red = x->datacolor.red;
	datacolor.green = x->datacolor.green;
	datacolor.blue = x->datacolor.blue;
	datacolor.alpha = 1.;

	jgraphics_set_line_width(g, .2);
	jgraphics_set_source_jrgba(g, &datacolor);

	if(!(x->sdif_buf.t_buf)){
		return;
	}

	timemin_disp = x->timemin_disp;
	timemax_disp = x->timemax_disp;
	if(timemin_disp == -1){
		timemin_disp = x->timemin_sdif;
	}
	if(timemax_disp == -1){
		timemax_disp = x->timemax_sdif;
	}

	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, x->timemin_sdif, ESDIF_SEARCH_FORWARDS);
	ff = f;

	int counter;
	// check to see how many frequency components we're supposed to display.
	// if it's ever more than the number of pixels we have, bail out.
	// this needs to be smarter--it could be that it's fine for the most part
	// but bad in only one small region.  we should try to deal with this properly
	/*
	while(ff && ff->header.time < x->timemin_disp){
		if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
			error("SDIF-editor: error!!");
			return;
		}

		if(m->header.matrixDataType == SDIF_FLOAT64){
			error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
			return;
		}
		r = m->header.rowCount;
		c = m->header.columnCount;
		data = (float *)(m->data);
		counter = 0;
		for(i = 0; i < r; i++){
			freq = data[(i * c) + 1];
			if(freq >= x->freqmin_disp && freq <= x->freqmax_disp){
				if(++counter > rect->height){
					return;
				}
			}
		}
		ff = ff->next;
	}
	*/

	if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
		error("SDIF-editor: error!!");
		return;
	}

	if(m->header.matrixDataType == SDIF_FLOAT64){
		error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
		return;
	}

	r = m->header.rowCount;
	c = m->header.columnCount;
	data = (float *)(m->data);
	prev_time = f->header.time;
	prev_time = sdifed_scale(prev_time, x->timemin_disp, x->timemax_disp, 0, rect->width);

	num_prev_indices = 0;
	for(i = 0; i < r; i++){
		index = data[i * c];
		freq = data[(i * c) + 1];
		freq = sdifed_scale(freq, x->freqmin_disp, x->freqmax_disp, rect->height, 0);
		prev_freq[num_prev_indices] = freq;
		prev_ind[num_prev_indices] = index;
		num_prev_indices++;
	}

	while(f && f->header.time <= x->timemax_disp){
		if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
			error("SDIF-editor: error!!");
			return;
		}

		if(m->header.matrixDataType == SDIF_FLOAT64){
			error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
			return;
		}

		r = m->header.rowCount;
		c = m->header.columnCount;
		data = (float *)(m->data);
		time = f->header.time;
		//post("**********	TIME = %f", time);
		time = sdifed_scale(time, x->timemin_disp, x->timemax_disp, 0, rect->width);

		for(i = 0; i < r; i++){
	       		index = data[i * c];
			freq = data[(i * c) + 1];
			if(freq >= x->freqmin_disp && freq <= x->freqmax_disp){
				freq = sdifed_scale(freq, x->freqmin_disp, x->freqmax_disp, rect->height, 0);
				amp = data[(i * c) + 2];
				for(j = 0; j < num_prev_indices; j++){
					if(prev_ind[j] == index){
						//post("prev index = %d, index = %d, prev freq = %f, freq = %f", prev_ind[j], index, prev_freq[j], freq);
						datacolor.alpha = amp * x->contrast;
						jgraphics_set_source_jrgba(g, &datacolor);     
						jgraphics_move_to(g, prev_time, prev_freq[j]);
						jgraphics_line_to(g, time, freq);
						jgraphics_stroke(g);
					}
				}
			}
		}
		prev_time = time;
		num_prev_indices = 0;
		for(i = 0; i < r; i++){
			index = data[i * c];
			freq = data[(i * c) + 1];
			if(freq >= x->freqmin_disp && freq <= x->freqmax_disp){
				freq = sdifed_scale(freq, x->freqmin_disp, x->freqmax_disp, rect->height, 0);
				prev_freq[num_prev_indices] = freq;
				prev_ind[num_prev_indices] = index;
				num_prev_indices++;
			}
		}
		for(i = 0; i < (int)floor(numFramesToSkip) + 1; i++){
			f = f->next;
			if(!f){
				break;
			}
		}
	}
}

void sdifed_draw_single_frame(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	/*
	double xx = x->sel_x.drag;
	double timemin, timemax;
	double time;
	char matrixType[4];
	SDIFmem_Matrix m;
	SDIFmem_Frame f;
	float *data;
	int r, c, i;
	int index;
	float freq, amp;

	timemin = x->timemin_disp;
	timemax = x->timemax_disp;
	if(timemin == -1){
		timemin = x->timemin_sdif;
	}
	if(timemax == -1){
		timemax = x->timemax_sdif;
	}

	time = sdifed_scale(xx, 0., rect->width, timemin, timemax);

	if(!(x->sdif_buf.t_buf)){
		return;
	}
	cmmjl_sdif_getMainMatrixType(&(x->sdif_buf), matrixType);
	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, time, 1);

	if(!(m = GetMatrix(&(x->sdif_buf), matrixType, f->header.time, 1))){
		error("SDIF-editor: error!!");
		return;
	}

	if(m->header.matrixDataType == SDIF_FLOAT64){
		error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
		return;
	}

	r = m->header.rowCount;
	c = m->header.columnCount;
	data = (float *)(m->data);

	t_jrgba color;
	color.red = color.green = 0;
	color.blue = 1.;
	color.alpha = .6;
	jgraphics_set_line_width(g, 1.);
	jgraphics_set_source_jrgba(g, &color);

	//post("**************************************************");
	//post("freqmin_disp = %f, freqmax_disp = %f", x->freqmin_disp, x->freqmax_disp);
	post("sfw: %f", x->single_frame_width);
	for(i = 0; i < r; i++){
		index = (int)(data[(i * c)]);
		freq = data[(i * c) + 1];
		//post("freq = %f", freq);
		freq = sdifed_scale(freq, x->freqmin_disp, x->freqmax_disp, rect->height, 0.);
		//post("freq adj = %f", freq);
		amp = data[(i * c) + 2];
		//post("amp = %f", amp);
		amp = amp * x->single_frame_width;
		//post("amp adj = %f", amp);
		jgraphics_move_to(g, xx, freq);
		jgraphics_line_to(g, xx + amp, freq);
		jgraphics_stroke(g);
	}
	*/
}

void sdifed_draw_bounds(t_sdifed *x, t_jgraphics *g, t_rect *rect){
	t_jrgba textcolor;
	t_jfont *jf;
	t_jtextlayout *jtl;
	char text[16];
	
	// draw counter
	jf = jfont_create(jbox_get_fontname((t_object *)x)->s_name, jbox_get_font_slant((t_object *)x), jbox_get_font_weight((t_object *)x), jbox_get_fontsize((t_object *)x));
	jtl = jtextlayout_create();
	textcolor.red = textcolor.green = textcolor.blue = 0;
	textcolor.alpha = 1;
	jtextlayout_settextcolor(jtl, &textcolor); 

	sprintf(text, "%.01f", x->freqmax_disp);	
	jtextlayout_set(jtl, text, jf, 0, 0, rect->width - 10, rect->height- 10, JGRAPHICS_TEXT_JUSTIFICATION_LEFT, JGRAPHICS_TEXTLAYOUT_NOWRAP);
	jtextlayout_draw(jtl, g);

	sprintf(text, "%.01f", x->freqmin_disp);
	jtextlayout_set(jtl, text, jf, 0, rect->height - 10, rect->width - 10, rect->height- 10, JGRAPHICS_TEXT_JUSTIFICATION_LEFT, JGRAPHICS_TEXTLAYOUT_NOWRAP);
	jtextlayout_draw(jtl, g);

	/*
	sprintf(text, "%.02f", x->timemin_disp);	
	jtextlayout_set(jtl, text, jf, 0, 0, rect->width - 10, rect->height- 10, JGRAPHICS_TEXT_JUSTIFICATION_LEFT, JGRAPHICS_TEXTLAYOUT_NOWRAP);
	jtextlayout_draw(jtl, g);

	sprintf(text, "%.02f", x->freqmin_disp);	
	jtextlayout_set(jtl, text, jf, 0, 0, rect->width - 10, rect->height- 10, JGRAPHICS_TEXT_JUSTIFICATION_LEFT, JGRAPHICS_TEXTLAYOUT_NOWRAP);
	jtextlayout_draw(jtl, g);
	*/
	jtextlayout_destroy(jtl);
	jfont_destroy(jf);
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
		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1;
		jbox_redraw(&(x->box));

        }
        return 0;
}

void sdifed_output_current_frame(t_sdifed *x){
	double xx = x->sel_x.drag;
	if(xx == -1){
		xx = x->sel_x.click;
	}
	double timemin, timemax;
	double time;
	SDIFmem_Matrix m;
	SDIFmem_Frame f;
	float *data;
	int r, c, i;
	int n;
	t_atom *buf = x->buf;
	t_rect rect;

	jbox_get_patching_rect(&(x->box.b_ob), &rect);

	timemin = x->timemin_disp;
	timemax = x->timemax_disp;
	if(timemin == -1){
		timemin = x->timemin_sdif;
	}
	if(timemax == -1){
		timemax = x->timemax_sdif;
	}

	time = sdifed_scale(xx, 0., rect.width, timemin, timemax);

	if(!(x->sdif_buf.t_buf)){
		return;
	}
	f = SDIFbuf_GetFrame(x->sdif_buf.t_buf, time, ESDIF_SEARCH_FORWARDS);

	if(!(m = GetMatrix(&(x->sdif_buf), x->matrix_type, f->header.time, 1))){
		error("SDIF-editor: error!!");
		return;
	}

	if(m->header.matrixDataType == SDIF_FLOAT64){
		error("SDIF-editor: SDIF matrix contains doubles!!  I wasn't expecting that...");
		return;
	}

	r = m->header.rowCount;
	c = m->header.columnCount;
	n = r * c + 1;

	data = (float *)(m->data);

	atom_setfloat(buf++, time);
	for(i = 0; i < r * c; i++){
		atom_setfloat(buf++, *((float *)(data++)));
	}
	outlet_list(x->out_data, NULL, n, x->buf);
}

void sdifed_output_matrix_for_time(t_sdifed *x, double time){

}

// 0x10 = no modifiers
// 0x11 = command
// 0x12 = shift
// 0x94 = control
// 0x18 = option
void sdifed_mousedown(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	x->state = (unsigned long long)modifiers;
	//post("0x%X", modifiers);
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		x->sel_x.click = pt.x;
		x->sel_x.drag = -1;
		x->sel_y.click = -1;
		x->sel_y.drag = -1;
		break;
	case 0x12:
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
	//sdifed_post_sel(&(x->sel_x), &(x->sel_y));
	sdifed_output_sel(x);
	//sdifed_output_current_frame(x);
}

void sdifed_mousedrag(t_sdifed *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	if(pt.x < 0){
		pt.x = 0;
	}else if(pt.x > r.width){
		pt.x = r.width;
	}
	if(pt.y < 0){
		pt.y = 0;
	}else if(pt.y > r.height){
		pt.y = r.height;
	}
	x->state = (unsigned long long)modifiers;
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
	sdifed_output_sel(x);
	//sdifed_output_current_frame(x);
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

void sdifed_output_sel(t_sdifed *x){
	t_rect r;
	t_atom out[4];
	double xmin, xmax, ymin, ymax;
	double timemin_disp, timemax_disp;

	xmin = sdifed_get_sel_min(&(x->sel_x));
	xmax = sdifed_get_sel_max(&(x->sel_x));

	// these are swapped intentionally since the top of the view is 0.
	ymin = sdifed_get_sel_max(&(x->sel_y));
	ymax = sdifed_get_sel_min(&(x->sel_y));

	jbox_get_patching_rect(&(x->box.b_ob), &r);

	if(xmin == -1){
		xmin = 0.;
	}
	if(xmax == -1){
		xmax = r.width;
	}
	if(ymin == -1){
		ymin = r.height;
	}
	if(ymax == -1){
		ymax = 0.;
	}

	timemin_disp = x->timemin_disp;
	timemax_disp = x->timemax_disp;
	if(timemin_disp == -1){
		timemin_disp = x->timemin_sdif;
	}
	if(timemax_disp == -1){
		timemax_disp = x->timemax_sdif;
	}

	atom_setfloat(&(out[0]), sdifed_scale(xmin, 0., r.width, timemin_disp, timemax_disp));
	atom_setfloat(&(out[1]), sdifed_scale(xmax, 0., r.width, timemin_disp, timemax_disp));
	atom_setfloat(&(out[2]), sdifed_scale(ymin, r.height, 0., x->freqmin_disp, x->freqmax_disp));
	atom_setfloat(&(out[3]), sdifed_scale(ymax, r.height, 0., x->freqmin_disp, x->freqmax_disp));

	outlet_anything(x->out_info, gensym("selection"), 4, out);
}

double sdifed_scale(double f, double min_in, double max_in, double min_out, double max_out){
	double m = (max_out - min_out) / (max_in - min_in);
	double b = (min_out - (m * min_in));
	return m * f + b;
}

void sdifed_zoom_in(t_sdifed *x){
	t_rect r;
	double xmin, xmax, ymin, ymax;

	xmin = sdifed_get_sel_min(&(x->sel_x));
	xmax = sdifed_get_sel_max(&(x->sel_x));

	// these are swapped intentionally since the top of the view is 0.
	ymin = sdifed_get_sel_max(&(x->sel_y));
	ymax = sdifed_get_sel_min(&(x->sel_y));

	jbox_get_patching_rect(&(x->box.b_ob), &r);

	if(xmin >= 0){
		xmin = sdifed_scale(xmin, 0., r.width, x->timemin_disp, x->timemax_disp);
	}else{
		xmin = x->timemin_disp;
	}
	if(xmax >= 0){
		xmax = sdifed_scale(xmax, 0., r.width, x->timemin_disp, x->timemax_disp);
	}else{
		xmax = x->timemax_disp;
	}
	if(ymin >= 0){
		ymin = sdifed_scale(ymin, r.height, 0., x->freqmin_disp, x->freqmax_disp);
	}else{
		ymin = x->freqmin_disp;
	}
	if(ymax >= 0){
		ymax = sdifed_scale(ymax, r.height, 0., x->freqmin_disp, x->freqmax_disp);
	}else{
		ymax = x->freqmax_disp;
	}

	x->timemin_disp = xmin;
	x->timemax_disp = xmax;
	x->freqmin_disp = ymin;
	x->freqmax_disp = ymax;

	sdifed_zero_sel(&(x->sel_x));
	sdifed_zero_sel(&(x->sel_y));

	jbox_redraw(&(x->box));
}

void sdifed_zoom_out(t_sdifed *x){
	x->freqmin_disp = x->freqmin_sdif;
	x->freqmax_disp = x->freqmax_sdif;
	x->timemin_disp = x->timemin_sdif;
	x->timemax_disp = x->timemax_sdif;

	sdifed_zero_sel(&(x->sel_x));
	sdifed_zero_sel(&(x->sel_y));

	jbox_redraw(&(x->box));
}

void sdifed_zero_sel(t_sel *sel){
	sel->click = -1;
	sel->drag = -1;
}

void sdifed_output_bounds(t_sdifed *x){
	t_atom a[2];
	atom_setfloat(a, (float)(x->timemin_disp));
	atom_setfloat(a + 1, (float)(x->timemax_disp));
	outlet_anything(x->out_info, gensym("domain"), 2, a);

	atom_setfloat(a, (float)(x->freqmax_disp));
	atom_setfloat(a + 1, (float)(x->freqmin_disp));
	outlet_anything(x->out_info, gensym("range"), 2, a);

	//post("bounds: %f %f %f %f", x->timemin_disp, x->timemax_disp, x->freqmin_disp, x->freqmax_disp);
}

void sdifed_output_selection(t_sdifed *x){
}

