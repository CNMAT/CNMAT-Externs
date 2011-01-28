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
  NAME: bdist
  DESCRIPTION: The beta distribution
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2009
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 0.0.1: Added version info
  VERSION 0.1: Better help file and prevents a and b from being <= 0
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

#include "gsl/gsl_sf.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_rng.h"

#define PDF 0
#define CDF 1

typedef struct _bdist{
	t_jbox ob;
	void *outlet, *info_outlet;
	void *proxies[2];
	long inlet;
	t_critical lock;
	t_jrgba bgcolor, linecolor, bordercolor;
	double x, a, b;
	long displaymode;
	gsl_rng *rng;
	double *hist;
	long histpos, histlen, histbufsize;
	long drawhist;
	t_pt mean, mode;
	double variance, skewness, kurtosis;
	long showmean, showmode, showvariance;
} t_bdist;

static t_class *bdist_class;

double bdist_beta(t_bdist *x, double xx, double a, double b);
void bdist_paint(t_bdist *x, t_object *patcherview);
t_pt bdist_computeMode(double a, double b);
void bdist_computeMoments(t_bdist *x);
void bdist_outputMoments(t_bdist *x);
void bdist_bang(t_bdist *x);
void bdist_distlist(t_bdist *x, t_symbol *msg, int argc, t_atom *argv);
void bdist_int(t_bdist *x, long n);
void bdist_float(t_bdist *x, double xx);
void bdist_list(t_bdist *x, t_symbol *msg, short argc, t_atom *argv);
void bdist_free(t_bdist *x);
void bdist_assist(t_bdist *x, void *b, long m, long a, char *s);
double bdist_scale(double f, double min_in, double max_in, double min_out, double max_out);
double bdist_clip(double f, double min, double max);
void bdist_mousedown(t_bdist *x, t_object *patcherview, t_pt pt, long modifiers);
void bdist_mousedrag(t_bdist *x, t_object *patcherview, t_pt pt, long modifiers);
void *bdist_new(t_symbol *msg, int argc, t_atom *argv);
t_max_err bdist_notify(t_bdist *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void bdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

t_symbol *l_hist, *l_func, *l_xhairs;

double bdist_beta(t_bdist *x, double xx, double a, double b){
	xx = bdist_clip(xx, 0., 1.);
	if(a <= 0){
		a = DBL_MIN;
	}
	if(b <= 0){
		b = DBL_MIN;
	}
	if(x->displaymode == PDF){
  		return gsl_ran_beta_pdf(xx, a, b);
	}else{
  		//return gsl_sf_beta_inc(a, b, bdist_clip(xx, 0., 1.));
		return gsl_cdf_beta_P(xx, a, b);
	}
}

void bdist_paint(t_bdist *x, t_object *patcherview){
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

	t_pt invar[(long)rect.width];
	long invarpos = 0;

	t_jgraphics *glayer = jbox_start_layer((t_object *)x, patcherview, l_hist, rect.width, rect.height);
	if(glayer && x->drawhist){
		t_jsurface *s = jgraphics_image_surface_create(JGRAPHICS_FORMAT_ARGB32, rect.width, rect.height);
		t_rect thisrect = rect;
		thisrect.x = thisrect.y = 0.;
		int i, n = 128;
		for(i = 0; i < n; i++){
			if(i >= x->histlen){
				break;
			}
			double row = i * (rect.height / n);
			double r = x->hist[((x->histpos - i) + x->histbufsize) % x->histbufsize];
			double col = r * rect.width;
			jgraphics_image_surface_set_pixel(s, col, row, x->linecolor);
		}
		jgraphics_image_surface_draw(glayer, s, thisrect, thisrect);
		if(s){
			jgraphics_surface_destroy(s);
		}
	}
	jbox_end_layer((t_object *)x, patcherview, l_hist);
	jbox_paint_layer((t_object *)x, patcherview, l_hist, 0, 0);

#define MARGIN 5
	double a = x->a, b = x->b;
	double width = rect.width, height = rect.height;
	double ymin = 0, ymax = 1;
	double varleft = x->mean.x - x->variance, varright = x->mean.x + x->variance;
	if(x->displaymode == PDF){
		ymax = x->mode.y;
	}
	glayer = jbox_start_layer((t_object *)x, patcherview, l_func, rect.width, rect.height);
	if(glayer){
		// draw the curve
		{
			jgraphics_set_source_jrgba(glayer, &(x->linecolor));
			int i;
			double xx1, xx2;
			for(i = 1; i < rect.width; i++){
				xx1 = bdist_beta(x, (i - 1) / width, a, b);
				if((i / rect.width) >= varleft && (i / rect.width) <= varright){
					invar[invarpos].x = i - 1;
					invar[invarpos].y = xx1 = bdist_scale(xx1, ymin, ymax, height, MARGIN);
					invarpos++;
				}else{
					xx1 = bdist_scale(xx1, ymin, ymax, height, MARGIN);
				}
				xx2 = bdist_beta(x, i / width, a, b);
				xx2 = bdist_scale(xx2, ymin, ymax, height, MARGIN);
				jgraphics_move_to(glayer, i - 1, xx1);
				jgraphics_line_to(glayer, i, xx2);
				jgraphics_stroke(glayer);
			}
		}
		// draw the first and second moments
		{
			if(x->showmean){
				if(x->displaymode == PDF){
					jgraphics_move_to(glayer, x->mean.x * rect.width, bdist_scale(x->mean.y, ymin, ymax, rect.height, MARGIN));
				}else{
					jgraphics_move_to(glayer, x->mean.x * rect.width, bdist_scale(bdist_beta(x, x->mean.x, x->a, x->b), ymin, ymax, rect.height, MARGIN));
				}
				jgraphics_line_to(glayer, x->mean.x * rect.width, rect.height);
				jgraphics_stroke(glayer);
			}

			if(x->showmode){
				jgraphics_set_dash(glayer, (double[2]){4., 4.}, 2, 0);
				if(x->displaymode == PDF){
					jgraphics_move_to(glayer, x->mode.x * rect.width, bdist_scale(x->mode.y, ymin, ymax, rect.height, MARGIN));
				}else{
					jgraphics_move_to(glayer, x->mode.x * rect.width, bdist_scale(bdist_beta(x, x->mode.x, x->a, x->b), ymin, ymax, rect.height, MARGIN));
				}
				jgraphics_line_to(glayer, x->mode.x * rect.width, rect.height);
				jgraphics_stroke(glayer);
			}

			if(x->showvariance){
				int i;
				jgraphics_move_to(glayer, invar[0].x, rect.height);
				jgraphics_line_to(glayer, invar[0].x, invar[0].y);
				for(i = 1; i < invarpos; i++){
					jgraphics_line_to(glayer, invar[i].x, invar[i].y);
				}
				jgraphics_line_to(glayer, invar[i - 1].x, rect.height);
				jgraphics_line_to(glayer, invar[0].x, rect.height);
				jgraphics_close_path(glayer);
				t_jrgba c = x->linecolor;
				c.alpha = .3;
				jgraphics_set_source_jrgba(glayer, &c);
				jgraphics_fill(glayer);
			}
		}
	}
	jbox_end_layer((t_object *)x, patcherview, l_func);
	jbox_paint_layer((t_object *)x, patcherview, l_func, 0, 0);

	glayer = jbox_start_layer((t_object *)x, patcherview, l_xhairs, rect.width, rect.height);
	if(glayer){
		double xx = x->x;
		double f = bdist_beta(x, xx, a, b);
		f = bdist_scale(f, ymin, ymax, height, MARGIN);
		jgraphics_set_source_jrgba(glayer, &(x->linecolor));
		//jgraphics_move_to(g, 0, f);
		//jgraphics_line_to(g, rect.width, f);
		jgraphics_ellipse(glayer, bdist_scale(xx, 0., 1., 0., width) - 2, f - 2, 4, 4);
		jgraphics_fill(glayer);
		//f = rect.width * x->x;
		//jgraphics_move_to(g, f, 0);
		//jgraphics_line_to(g, f, rect.height);
		//jgraphics_stroke(g);
	}
	jbox_end_layer((t_object *)x, patcherview, l_xhairs);
	jbox_paint_layer((t_object *)x, patcherview, l_xhairs, 0, 0);
}

t_pt bdist_computeMode(double a, double b){
	t_pt mode;
	if(a > 1 && b > 1){
		mode.x = (a - 1) / (a + b - 2);
		mode.y = gsl_ran_beta_pdf(mode.x, a, b);
	}else if(a == 1 && b == 1){
		mode.x = .5;
		mode.y = 1.;
	}else{
		double left, right;
		left = gsl_ran_beta_pdf(.0001, a, b);
		right = gsl_ran_beta_pdf(.9999, a, b);
		if(right >= left){
			mode.x = .0001;
			mode.y = right;
		}else{
			mode.x = .9999;
			mode.y = left;
		}
	}
	return mode;
}

void bdist_computeMoments(t_bdist *x){
	double a = x->a, b = x->b;
	x->mean.x = a / (a + b);
	x->mean.y = gsl_ran_beta_pdf(x->mean.x, a, b);
	x->mode = bdist_computeMode(a, b);
	x->variance = (a * b) / (pow(a + b, 2.) * (a + b + 1));
	x->skewness = (2 * (b - a) * sqrt(a + b + 1)) / ((a + b + 2) * sqrt(a * b));
	x->kurtosis = (3*(1 + a + b)*(a*b*(-6 + a + b) + 2*pow(a + b,2))) / (a*b*(2 + a + b)*(3 + a + b));
}

void bdist_outputMoments(t_bdist *x){
	t_atom out[2];
	atom_setfloat(out, x->mean.x);
	atom_setfloat(out + 1, x->mean.y);
	outlet_anything(x->info_outlet, gensym("mean"), 2, out);

	atom_setfloat(out, x->mode.x);
	atom_setfloat(out + 1, x->mode.y);
	outlet_anything(x->info_outlet, gensym("mode"), 2, out);

	atom_setfloat(out, x->variance);
	outlet_anything(x->info_outlet, gensym("variance"), 1, out);

	atom_setfloat(out, x->skewness);
	outlet_anything(x->info_outlet, gensym("skewness"), 1, out);

	atom_setfloat(out, x->kurtosis);
	outlet_anything(x->info_outlet, gensym("kurtosis"), 1, out);
}

void bdist_bang(t_bdist *x){
	bdist_outputMoments(x);
	double r = gsl_ran_beta(x->rng, x->a, x->b);
	x->hist[x->histpos] = r;
	x->histpos = (x->histpos + 1) % x->histbufsize;
	x->histlen++;
	outlet_float(x->outlet, r);
	if(x->drawhist){
		jbox_invalidate_layer((t_object *)x, NULL, l_hist);
		jbox_redraw(&(x->ob));
	}
}

void bdist_distlist(t_bdist *x, t_symbol *msg, int argc, t_atom *argv){
	long n;
	if(argc == 1){
		n = atom_getlong(argv);
	}else if(!(argc % 2)){
		n = argc / 2;
	}else{
		object_error((t_object *)x, "bad number of arguments (%d)", argc);
		return;
	}
	bdist_outputMoments(x);
	x->histlen += n;
	int i;
	double r[n];
	t_atom out[n];
	if(argc == 1){
		for(i = 0; i < n; i++){
			r[i] = gsl_ran_beta(x->rng, x->a, x->b);
			x->hist[x->histpos] = r[i];
			x->histpos = (x->histpos + 1) % x->histbufsize;
			atom_setfloat(out + i, r[i]);
		}
	}else{
		t_atom *ptr = argv;
		double a, b;
		for(i = 0; i < n; i++){
			a = atom_getfloat(ptr++);
			b = atom_getfloat(ptr++);
			r[i] = gsl_ran_beta(x->rng, a, b);
			x->hist[x->histpos] = r[i];
			x->histpos = (x->histpos + 1) % x->histbufsize;
			atom_setfloat(out + i, r[i]);
		}
	}
	outlet_list(x->outlet, NULL, n, out);
	if(x->drawhist){
		jbox_invalidate_layer((t_object *)x, NULL, l_hist);
		jbox_redraw(&(x->ob));
	}
}

void bdist_int(t_bdist *x, long n){
}

void bdist_float(t_bdist *x, double f){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		x->x = bdist_clip(f, 0., 1.);
		bdist_outputMoments(x);
		outlet_float(x->outlet, bdist_beta(x, f, x->a, x->b));
		break;
	case 1:
		x->a = bdist_clip(f, FLT_MIN, FLT_MAX);
		bdist_computeMoments(x);
		bdist_outputMoments(x);
		jbox_invalidate_layer((t_object *)x, NULL, l_func);
		break;
	case 2:
		x->b = bdist_clip(f, FLT_MIN, FLT_MAX);
		bdist_computeMoments(x);
		bdist_outputMoments(x);
		jbox_invalidate_layer((t_object *)x, NULL, l_func);
		break;
	}
	jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
	jbox_redraw(&(x->ob));
}

void bdist_list(t_bdist *x, t_symbol *msg, short argc, t_atom *argv){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		if(argc){
			x->x = bdist_clip(atom_getfloat(argv), 0., 1.);
		}
		if(argc > 1){
			x->a = bdist_clip(atom_getfloat(argv + 1), FLT_MIN, FLT_MAX);
		}
		if(argc > 2){
			x->b = bdist_clip(atom_getfloat(argv + 2), FLT_MIN, FLT_MAX);
		}
		bdist_outputMoments(x);
		break;
	case 1:
		if(argc){
			x->a = bdist_clip(atom_getfloat(argv), FLT_MIN, FLT_MAX);
		}
		if(argc > 1){
			x->b = bdist_clip(atom_getfloat(argv + 1), FLT_MIN, FLT_MAX);
		}
		bdist_computeMoments(x);
		bdist_outputMoments(x);
		break;
	case 2:
		if(argc){
			x->b = bdist_clip(atom_getfloat(argv), FLT_MIN, FLT_MAX);
		}
		bdist_computeMoments(x);
		bdist_outputMoments(x);
		break;
	}
	jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
	jbox_invalidate_layer((t_object *)x, NULL, l_func);
	jbox_redraw(&(x->ob));
}

void bdist_anything(t_bdist *x, t_symbol *msg, short argc, t_atom *argv){

}

void bdist_clearhist(t_bdist *x){
	x->histlen = 0;
	x->histpos = 0;
	jbox_invalidate_layer((t_object *)x, NULL, l_hist);
	jbox_redraw(&(x->ob));
}

void bdist_free(t_bdist *x){
	jbox_free(&(x->ob));
	critical_free(x->lock);
	if(x->hist){
		sysmem_freeptr(x->hist);
	}
}

void bdist_assist(t_bdist *x, void *b, long io, long index, char *s){
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

double bdist_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double bdist_clip(double f, double min, double max){
	double ff = f;
	if(ff < min){
		ff = min;
	}
	if(ff > max){
		ff = max;
	}
	return ff;
}

void bdist_mousedown(t_bdist *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	x->x = pt.x / r.width;
	x->x = bdist_clip(x->x, 0., 1.);
	bdist_outputMoments(x);
	outlet_float(x->outlet, bdist_beta(x, x->x, x->a, x->b));
	jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
	jbox_redraw(&(x->ob));
}

void bdist_mousedrag(t_bdist *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	double xx = pt.x / r.width;
	x->x = bdist_clip(xx, 0., 1.);
	bdist_outputMoments(x);
	outlet_float(x->outlet, bdist_beta(x, x->x, x->a, x->b));
	jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
	jbox_redraw(&(x->ob));
}


/**************************************************
Object and instance creation functions.
**************************************************/

void *bdist_new(t_symbol *msg, int argc, t_atom *argv){
	t_bdist *x;
    
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

	if(x = (t_bdist *)object_alloc(bdist_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 

		x->info_outlet = outlet_new(x, NULL);
		x->outlet = outlet_new(x, NULL);
		x->proxies[1] = proxy_new((t_object *)x, 2, &(x->inlet));
		x->proxies[0] = proxy_new((t_object *)x, 1, &(x->inlet));

		// set up the random number generator	
		gsl_rng_env_setup();

		// waterman14 was the fastest according to my tests
		x->rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_waterman14);
	
		// seed it by reading from /dev/random on mac os x and 
		// something similar on windows
		unsigned int r;
		FILE *f;
#ifdef Windows
#else
		f = fopen("/dev/random", "r");
#endif
		fread(&r, sizeof(r), 1, f);
		gsl_rng_set(x->rng, r);

		// this is really fucking important.  if there's an error and the gsl's 
		// default handler gets called, it aborts the program!
		gsl_set_error_handler(bdist_errorHandler);  

		critical_new(&(x->lock));

		x->histbufsize = 2048;
		x->hist = (double *)sysmem_newptr(x->histbufsize * sizeof(double));
		memset(x->hist, '\0', x->histbufsize * sizeof(double));
		x->histpos = 0;
		x->histlen = 0;
        
		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
        
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("bdist", (method)bdist_new, (method)bdist_free, sizeof(t_bdist), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)bdist_paint, "paint", A_CANT, 0); 
	class_addmethod(c, (method)bdist_bang, "bang", 0);
	class_addmethod(c, (method)bdist_int, "int", A_LONG, 0);
	class_addmethod(c, (method)bdist_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)bdist_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)bdist_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)bdist_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)bdist_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)bdist_distlist, "distlist", A_GIMME, 0);
	class_addmethod(c, (method)bdist_notify, "notify", A_GIMME, 0);
    
	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_bdist, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "linecolor", 0, t_bdist, linecolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "linecolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "linecolor", 0, "rgba", "Line Color"); 

 	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_bdist, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 
    
	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

	CLASS_ATTR_DOUBLE(c, "a", 0, t_bdist, a);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "a", 0, "1.0");
	CLASS_ATTR_FILTER_CLIP(c, "a", .000001, FLT_MAX);
	CLASS_ATTR_DOUBLE(c, "b", 0, t_bdist, b);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "b", 0, "1.0");
	CLASS_ATTR_FILTER_CLIP(c, "b", .000001, FLT_MAX);

	CLASS_ATTR_LONG(c, "displaymode", 0, t_bdist, displaymode);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "displaymode", 0, "0");
	CLASS_ATTR_ENUMINDEX(c, "displaymode", 0, "PDF CDF");
	CLASS_ATTR_LABEL(c, "displaymode", 0, "Display Type");

	CLASS_ATTR_LONG(c, "showmean", 0, t_bdist, showmean);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "showmean", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "showmean", 0, "onoff", "Show Mean");

	CLASS_ATTR_LONG(c, "showmode", 0, t_bdist, showmode);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "showmode", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "showmode", 0, "onoff", "Show Mode");

	CLASS_ATTR_LONG(c, "showvariance", 0, t_bdist, showvariance);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "showvariance", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "showvariance", 0, "onoff", "Show Variance");

	CLASS_ATTR_LONG(c, "drawhist", 0, t_bdist, drawhist);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "drawhist", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "drawhist", 0, "onoff", "Draw History");
    
	class_register(CLASS_BOX, c);
	bdist_class = c;

	common_symbols_init();

	l_hist = gensym("hist");
	l_func = gensym("func");
	l_xhairs = gensym("xhairs");

	version(0);

	return 0;
}

t_max_err bdist_notify(t_bdist *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	if(msg == gensym("attr_modified")){
		if(x->drawhist){
			jbox_invalidate_layer((t_object *)x, NULL, l_hist);
		}
		jbox_invalidate_layer((t_object *)x, NULL, l_func);
		jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
		jbox_redraw(&(x->ob));
	}
	return 0;
}

void bdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno){
	error("bdist: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}
