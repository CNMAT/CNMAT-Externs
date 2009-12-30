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
#include "gsl/gsl_errno.h"

#define DEF_BUF_LEN 128

typedef struct _bdist{
	t_jbox ob;
	void *outlet;
	void *proxies[2];
	long inlet;
	t_critical lock;
	t_jrgba bgcolor, linecolor, bordercolor;
	double x, a, b;
	double xmin, xmax, ymin, ymax;
} t_bdist;

static t_class *bdist_class;

double bdist_beta(double x, double a, double b);
void bdist_paint(t_bdist *x, t_object *patcherview);
void bdist_bang(t_bdist *x);
void bdist_int(t_bdist *x, long n);
void bdist_float(t_bdist *x, double xx);
void bdist_list(t_bdist *x, t_symbol *msg, short argc, t_atom *argv);
void bdist_free(t_bdist *x);
void bdist_assist(t_bdist *x, void *b, long m, long a, char *s);
double bdist_scale(double f, double min_in, double max_in, double min_out, double max_out);
double bdist_clip(double f, double min, double max);
void *bdist_new(t_symbol *msg, int argc, t_atom *argv);
void bdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

double bdist_beta(double x, double a, double b){
	return gsl_sf_beta_inc(a, b, bdist_clip(x, 0., 1.));
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

	// draw the curve
	{
		jgraphics_set_source_jrgba(g, &(x->linecolor));
		int i;
		jgraphics_move_to(g, 0, rect.height);
		for(i = 1; i < rect.width; i++){
			jgraphics_line_to(g, i, rect.height * (1. - bdist_beta(i / rect.width, x->a, x->b)));
		}
		jgraphics_stroke(g);
	}

	// draw the crosshairs
	{
		double f = rect.height * (1. - bdist_beta(x->x, x->a, x->b));
		jgraphics_set_source_jrgba(g, &(x->linecolor));
		jgraphics_move_to(g, 0, f);
		jgraphics_line_to(g, rect.width, f);
		f = rect.width * x->x;
		jgraphics_move_to(g, f, 0);
		jgraphics_line_to(g, f, rect.height);
		jgraphics_stroke(g);
	}
}

void bdist_bang(t_bdist *x){
}

void bdist_int(t_bdist *x, long n){
}

void bdist_float(t_bdist *x, double f){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		x->x = bdist_scale(f, x->xmin, x->xmax, 0., 1.);
		x->x = bdist_clip(x->x, 0., 1.);
		outlet_float(x->outlet, bdist_scale(bdist_beta(x->x, x->a, x->b), 0., 1., x->ymin, x->ymax));
		break;
	case 1:
		x->a = bdist_clip(f, FLT_MIN, FLT_MAX);
		break;
	case 2:
		x->b = bdist_clip(f, FLT_MIN, FLT_MAX);
		break;
	}
	jbox_redraw(&(x->ob));
}

void bdist_list(t_bdist *x, t_symbol *msg, short argc, t_atom *argv){
}

void bdist_anything(t_bdist *x, t_symbol *msg, short argc, t_atom *argv){

}

void bdist_free(t_bdist *x){
	jbox_free(&(x->ob));
	critical_free(x->lock);
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

		x->outlet = outlet_new(x, NULL);
		x->proxies[1] = proxy_new((t_object *)x, 2, &(x->inlet));
		x->proxies[0] = proxy_new((t_object *)x, 1, &(x->inlet));

		// this is really fucking important.  if there's an error and the gsl's 
		// default handler gets called, it aborts the program!
		gsl_set_error_handler(bdist_errorHandler);  

		critical_new(&(x->lock));
        
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
	CLASS_ATTR_FILTER_CLIP(c, "a", FLT_MIN, FLT_MAX);
	CLASS_ATTR_DOUBLE(c, "b", 0, t_bdist, b);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "b", 0, "1.0");
	CLASS_ATTR_FILTER_CLIP(c, "b", FLT_MIN, FLT_MAX);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_bdist, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "xmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_bdist, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_bdist, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ymin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_bdist, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ymax", 0, "1.0");
    
    
	class_register(CLASS_BOX, c);
	bdist_class = c;

	common_symbols_init();

	version(0);

	return 0;
}

void bdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno){
	error("bdist: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}
