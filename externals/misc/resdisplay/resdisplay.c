/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2010, The Regents of
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
  NAME: resdisplay
  DESCRIPTION: Display/edit resonance models
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2010
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 0.1: bug fix in the log display mode and much faster drawing
  VERSION 0.2: Info about the range and selection is now drawn at the top of the display
  VERSION 0.2.1: Bang now outputs model and model comes out when loaded
  VERSION 0.2.2: added displayrange message for compatibility with resonance-display.js
  VERSION 0.2.3: more reasonable default size
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "jpatcher_api.h" 
#include "jgraphics.h"
#include "ext_critical.h"
#include "version.c"
#include "math.h"

typedef struct _res{
	double f, a, d;
} t_res;

typedef struct _sin{
	double f, a;
} t_sin;

typedef struct _range{
	double min, max;
} t_range;

typedef struct _rd{
	t_jbox ob;
	void *outlet;
	t_critical lock;
	t_jrgba bgcolor, datacolor, selectioncolor, bordercolor;
	double freqmin, freqmax, ampmin, ampmax;
	double ampmin_log, ampmax_log;
	double *buffer;
	long n, buffer_size;
	int sinusoids;
	long mode;
	t_range selection;
	long log;
	int num_partials_selected;
} t_rd;

static t_class *rd_class;

void rd_paint(t_rd *x, t_object *patcherview);
void rd_clear(t_rd *x);
void rd_bang(t_rd *x);
void rd_int(t_rd *x, long n);
void rd_float(t_rd *x, double xx);
void rd_list(t_rd *x, t_symbol *msg, short argc, t_atom *argv);
void rd_copy_data(t_rd *x, short argc, t_atom *argv);
void rd_displayrange(t_rd *x, double min, double max);
double rd_scale(double f, double min_in, double max_in, double min_out, double max_out);
void rd_mousedown(t_rd *x, t_object *patcherview, t_pt pt, long modifiers);
void rd_mousedrag(t_rd *x, t_object *patcherview, t_pt pt, long modifiers);
void rd_output_all(t_rd *x);
void rd_output_sel(t_rd *x);
double rd_ftom(double f);
void rd_free(t_rd *x);
void rd_assist(t_rd *x, void *b, long m, long a, char *s);
void *rd_new(t_symbol *msg, int argc, t_atom *argv);
t_max_err rd_notify(t_rd *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void rd_paint(t_rd *x, t_object *patcherview){
	t_rect rect;
    
	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
        
	jgraphics_set_source_jrgba(g, &(x->bordercolor));
	jgraphics_set_line_width(g, 1);
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_stroke(g);
    
	jgraphics_set_source_jrgba(g, &(x->bgcolor));
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_fill(g);

	// data
	{
		jgraphics_set_source_jrgba(g, &(x->datacolor));
		int i;
		if(x->sinusoids){
			t_sin *s = (t_sin *)(x->buffer);
			for(i = 0; i < x->n; i++){
				double xx, yy;
				xx = rd_scale(s[i].f, x->freqmin, x->freqmax, 0, rect.width);
				if(x->log){
					yy = rd_scale((20. * log(s[i].a)) / log(10), x->ampmin_log, x->ampmax_log, rect.height, 0.);
				}else{
					yy = rd_scale(s[i].a, x->ampmin, x->ampmax, rect.height, 0);
				}

				jgraphics_move_to(g, xx, yy);
				jgraphics_line_to(g, xx, rect.height);
				jgraphics_stroke(g);
			}

		}else if(x->mode){
			t_jsurface *s = jgraphics_image_surface_create(JGRAPHICS_FORMAT_ARGB32, rect.width, rect.height);
			//t_jgraphics *gg = jgraphics_create(s);
			t_res *r = (t_res *)(x->buffer);
			int j;
			t_jrgba c;
			for(j = 0; j < rect.width; j++){
				for(i = 0; i < x->n; i++){
					double amp = exp((-j / (rect.width * .1)) * r[i].d);
					c = (t_jrgba){x->datacolor.red * amp, x->datacolor.green * amp, x->datacolor.blue * amp, 1.};
					jgraphics_image_surface_set_pixel(s, j, rd_scale(r[i].f, x->freqmin, x->freqmax, rect.height, 0), c);

					//jgraphics_set_source_jrgba(g, &c);
					//jgraphics_ellipse(g, j, rd_scale(r[i].f, 0., 22050., rect.height, 0), 1, 1);
					//jgraphics_fill(g);
					//jgraphics_move_to(g, 0, yy1);
					//jgraphics_line_to(g, xx, yy2);
				}
			}
			jgraphics_image_surface_draw(g, s, (t_rect){0., 0., rect.width, rect.height}, (t_rect){0., 0., rect.width, rect.height});
		}else{
			t_res *r = (t_res *)(x->buffer);
			for(i = 0; i < x->n; i++){
				double xx, yy1, yy2;
				xx = rd_scale(r[i].f, x->freqmin, x->freqmax, 0, rect.width);
				if(x->log){
					//yy1 = rd_scale((100+20.0*log(r[i].a))/100./log(10), -1., 1., rect.height, 0.);
					yy1 = rd_scale((20.0*log(r[i].a))/log(10), x->ampmin_log, x->ampmax_log, rect.height, 0.);
				}else{
					yy1 = rd_scale(r[i].a, x->ampmin, x->ampmax, rect.height, 0);
				}
				yy2 = fabs((rect.height - yy1) * (.4 / sqrt(r[i].d))) + yy1;

				jgraphics_move_to(g, xx, yy1);
				jgraphics_line_to(g, xx, yy2);
				jgraphics_stroke(g);
			}
		}
	}

	// selection lines
	{
		jgraphics_set_source_jrgba(g, &(x->selectioncolor));
		if(x->mode){
			jgraphics_move_to(g, 0, rd_scale(x->selection.min, x->freqmin, x->freqmax, rect.height, 0));
			jgraphics_line_to(g, rect.width, rd_scale(x->selection.min, x->freqmin, x->freqmax, rect.height, 0));
			jgraphics_move_to(g, 0, rd_scale(x->selection.max, x->freqmin, x->freqmax, rect.height, 0));
			jgraphics_line_to(g, rect.width, rd_scale(x->selection.max, x->freqmin, x->freqmax, rect.height, 0));
		}else{
			jgraphics_move_to(g, rd_scale(x->selection.min, x->freqmin, x->freqmax, 0, rect.width), 0);
			jgraphics_line_to(g, rd_scale(x->selection.min, x->freqmin, x->freqmax, 0, rect.width), rect.height);
			jgraphics_move_to(g, rd_scale(x->selection.max, x->freqmin, x->freqmax, 0, rect.width), 0);
			jgraphics_line_to(g, rd_scale(x->selection.max, x->freqmin, x->freqmax, 0, rect.width), rect.height);
		}
		jgraphics_stroke(g);
	}

	// info
	{
		char buf[128];
		double w, h;
		jgraphics_set_source_jrgba(g, &x->datacolor);
		sprintf(buf, "%0.2f", x->selection.min);
		jgraphics_text_measure(g, buf, &w, &h);
		jgraphics_move_to(g, 0, h);
		jgraphics_show_text(g, buf);

		sprintf(buf, "%0.2f", x->selection.max);
		jgraphics_text_measure(g, buf, &w, &h);
		jgraphics_move_to(g, rect.width - w, h);
		jgraphics_show_text(g, buf);

		sprintf(buf, "%0.2fHz (%0.2fMc) %d/%d", x->selection.max - x->selection.min, rd_ftom(x->selection.max) - rd_ftom(x->selection.min), x->num_partials_selected, x->n);
		jgraphics_text_measure(g, buf, &w, &h);
		jgraphics_move_to(g, (rect.width / 2) - (w / 2), h);
		jgraphics_show_text(g, buf);
	}
}

void rd_clear(t_rd *x){
	x->n = 0;
	jbox_redraw(&(x->ob));
}

void rd_bang(t_rd *x){
	rd_output_all(x);
}

void rd_int(t_rd *x, long n){
}

void rd_float(t_rd *x, double f){
}

void rd_sinusoids(t_rd *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc & 1){
		object_error((t_object *)x, "multiple of 2 arguments required (freq, amp)");
		return;
	}
	x->sinusoids = 1;
	x->n = argc / 2;
	rd_copy_data(x, argc, argv);
}

void rd_list(t_rd *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc % 3){
		object_error((t_object *)x, "multiple of 3 arguments required (freq, amp, decayrate)");
		return;
	}
	x->sinusoids = 0;
	x->n = argc / 3;
	rd_copy_data(x, argc, argv);
	rd_output_all(x);
}

void rd_copy_data(t_rd *x, short argc, t_atom *argv){
	if(argc > x->buffer_size){
		double *tmp = realloc(x->buffer, argc * sizeof(double));
		if(tmp){
			x->buffer = tmp;
			x->buffer_size = argc;
		}else{
			object_error((t_object *)x, "out of memory!");
			return;
		}
	}
	int i;
	for(i = 0; i < argc; i++){
		x->buffer[i] = atom_getfloat(argv + i);
		//post("%d: %f", i, x->buffer[i]);
	}
	jbox_redraw(&(x->ob));
}

void rd_anything(t_rd *x, t_symbol *msg, short argc, t_atom *argv){

}

void rd_displayrange(t_rd *x, double min, double max){
	if(min > max){
		x->freqmin = max;
		x->freqmax = min;
	}else{
		x->freqmin = min;
		x->freqmax = max;
	}
	jbox_redraw(&(x->ob));
}

double rd_scale(double f, double min_in, double max_in, double min_out, double max_out){
        float m = (max_out - min_out) / (max_in - min_in);
        float b = (min_out - (m * min_in));
        return m * f + b;
}

void rd_mousedown(t_rd *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect rect;
    	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	double f;
	if(x->mode){
		f = rd_scale(pt.y, rect.height, 0, x->freqmin, x->freqmax);
	}else{
		f = rd_scale(pt.x, 0, rect.width, x->freqmin, x->freqmax);
	}
	x->selection.min = x->selection.max = f;

	jbox_redraw(&(x->ob));
}

void rd_mousedrag(t_rd *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect rect;
    	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	double f;
	if(x->mode){
		f = rd_scale(pt.y, rect.height, 0, x->freqmin, x->freqmax);
	}else{
		f = rd_scale(pt.x, 0, rect.width, x->freqmin, x->freqmax);
	}
	if((f > x->selection.min)&&(f >= x->selection.max)){
		x->selection.max = f;
	}else if ((f <= x->selection.min)&&(f < x->selection.max)){
		x->selection.min = f;
				
	}else{
		//if its in between both, move the closest one. 
		if (abs(f - x->selection.min) < abs(f - x->selection.max)){
			x->selection.min = f;
		} else {
			x->selection.max = f;
		}
	}
	rd_output_sel(x);
	jbox_redraw(&(x->ob));
}

void rd_select_decayrates(t_rd *x, t_symbol *key, double f){
	if(x->sinusoids){
		return;
	}
	t_rect rect;
        jbox_get_patching_rect(&((x->ob.b_ob)), &rect);
	int i;
	t_atom buf[x->buffer_size];
	int selpos = 0, nselpos = x->buffer_size - 1;
	t_res *r = (t_res *)x->buffer;

	for(i = 0; i < x->n; i++){
		if(r[i].d >= f && r[i].d <= f){
			atom_setfloat(buf + selpos++, r[i].f);
			atom_setfloat(buf + selpos++, r[i].a);
			atom_setfloat(buf + selpos++, r[i].d);
		}else{
			//post("%d: %f %f %f", nselpos, r[i].f, r[i].a, r[i].d);
			atom_setfloat(buf + nselpos--, r[i].d);
			atom_setfloat(buf + nselpos--, r[i].a);
			atom_setfloat(buf + nselpos--, r[i].f);
		}
	}
	x->num_partials_selected = selpos / (x->sinusoids ? 2 : 3);
	outlet_anything(x->outlet, gensym("selected"), selpos, buf);
	outlet_anything(x->outlet, gensym("unselected"), x->buffer_size - nselpos - 1, buf + nselpos + 1);
}

void rd_output_all(t_rd *x){
	int i;
	int n = (x->sinusoids ? (x->n * 2) : (x->n * 3));
	t_atom buf[x->buffer_size];
	for(i = 0; i < n; i++){
		atom_setfloat(buf + i, x->buffer[i]);
	}
	outlet_list(x->outlet, NULL, n, buf);
}

void rd_output_sel(t_rd *x){
	t_rect rect;
        jbox_get_patching_rect(&((x->ob.b_ob)), &rect);
	int i;
	t_atom buf[x->buffer_size];
	int selpos = 0, nselpos = x->buffer_size - 1;
	if(x->sinusoids){
		t_sin *s = (t_sin *)x->buffer;
		for(i = 0; i < x->n; i++){
			if(s[i].f >= x->selection.min && s[i].f <= x->selection.max){
				atom_setfloat(buf + selpos++, s[i].f);
				atom_setfloat(buf + selpos++, s[i].a);
			}else{
				atom_setfloat(buf + nselpos--, s[i].f);
				atom_setfloat(buf + nselpos--, s[i].a);
			}
		}
	}else{
		t_res *r = (t_res *)x->buffer;
		for(i = 0; i < x->n; i++){
			if(r[i].f >= x->selection.min && r[i].f <= x->selection.max){
				atom_setfloat(buf + selpos++, r[i].f);
				atom_setfloat(buf + selpos++, r[i].a);
				atom_setfloat(buf + selpos++, r[i].d);
			}else{
				//post("%d: %f %f %f", nselpos, r[i].f, r[i].a, r[i].d);
				atom_setfloat(buf + nselpos--, r[i].d);
				atom_setfloat(buf + nselpos--, r[i].a);
				atom_setfloat(buf + nselpos--, r[i].f);
			}
		}
	}
	x->num_partials_selected = selpos / (x->sinusoids ? 2 : 3);
	outlet_anything(x->outlet, gensym("selected"), selpos, buf);
	outlet_anything(x->outlet, gensym("unselected"), x->buffer_size - nselpos - 1, buf + nselpos + 1);
}

#define BASE 27.5
double rd_ftom(double f){
	if(!f){
		f = FLT_MIN;
	}
	return (log(f / BASE) / log(pow(2., 1. / 12.))) + 33;
}

void rd_free(t_rd *x){
	jbox_free(&(x->ob));
	critical_free(x->lock);
	if(x->buffer){
		free(x->buffer);
	}
}

void rd_assist(t_rd *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	}
}

/**************************************************
Object and instance creation functions.
**************************************************/

void *rd_new(t_symbol *msg, int argc, t_atom *argv){
	t_rd *x;
    
 	t_dictionary *d = NULL; 
 	long boxflags; 
    
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

	if(x = (t_rd *)object_alloc(rd_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 

		x->outlet = outlet_new(x, NULL);

		critical_new(&(x->lock));

		x->buffer_size = 1024 * 3;
		x->n = 0;
		x->buffer = (double *)calloc(x->buffer_size, sizeof(double));
		x->num_partials_selected = 0;
        
		attr_dictionary_process(x, d); 
		x->selection = (t_range){x->freqmin, x->freqmax};
 		jbox_ready((t_jbox *)x); 
        
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("resdisplay", (method)rd_new, (method)rd_free, sizeof(t_rd), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)rd_paint, "paint", A_CANT, 0); 
	class_addmethod(c, (method)rd_bang, "bang", 0);
	class_addmethod(c, (method)rd_int, "int", A_LONG, 0);
	class_addmethod(c, (method)rd_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)rd_sinusoids, "sinusoids", A_GIMME, 0);
	class_addmethod(c, (method)rd_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rd_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)rd_clear, "clear", 0);
	class_addmethod(c, (method)rd_displayrange, "displayrange", A_FLOAT, A_FLOAT, 0);

	class_addmethod(c, (method)rd_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)rd_mousedrag, "mousedrag", A_CANT, 0);

	class_addmethod(c, (method)rd_notify, "notify", A_CANT, 0);
    
	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_rd, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "datacolor", 0, t_rd, datacolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "datacolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "datacolor", 0, "rgba", "Data Color"); 

 	CLASS_ATTR_RGBA(c, "selectioncolor", 0, t_rd, selectioncolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectioncolor", 0, "0. 0. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectioncolor", 0, "rgba", "Selection Color"); 

 	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_rd, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 
    
	CLASS_STICKY_ATTR_CLEAR(c, "category");

	CLASS_ATTR_DOUBLE(c, "freqmin", 0, t_rd, freqmin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "freqmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "freqmax", 0, t_rd, freqmax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "freqmax", 0, "22050.");

	CLASS_ATTR_DOUBLE(c, "ampmin", 0, t_rd, ampmin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ampmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ampmax", 0, t_rd, ampmax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ampmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ampmin_log", 0, t_rd, ampmin_log);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ampmin_log", 0, "-100.");
	CLASS_ATTR_DOUBLE(c, "ampmax_log", 0, t_rd, ampmax_log);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ampmax_log", 0, "0.0");

	CLASS_ATTR_LONG(c, "mode", 0, t_rd, mode);
	CLASS_ATTR_PAINT(c, "mode", 0);
    
	CLASS_ATTR_LONG(c, "log", 0, t_rd, log);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "log", 0, "1");
	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 
    
	class_register(CLASS_BOX, c);
	rd_class = c;

	common_symbols_init();

	version(0);

	return 0;
}

t_max_err rd_notify(t_rd *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_rect rect;
        jbox_get_patching_rect(&((x->ob.b_ob)), &rect);
        t_symbol *attrname;
        if (msg == gensym("attr_modified")){
                attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		if(attrname == gensym("mode")){
			/*
			if(x->mode){
				x->selection.min = rect.height - ((x->selection.min / rect.width) * rect.height);
				x->selection.max = rect.height - ((x->selection.max / rect.width) * rect.height);
			}else{
				x->selection.min = (1. - (x->selection.min / rect.height)) * rect.width;
				x->selection.max = (1. - (x->selection.max / rect.height)) * rect.width;
			}
			*/
		}
	}
	jbox_redraw(&(x->ob));
        return 0;
}
