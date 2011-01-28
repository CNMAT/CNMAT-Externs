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
  NAME: rbfi
  DESCRIPTION: A 2-D graphical display/editor like pictctrl but supporting multiple points.
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2009
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 0.1: Added labels, and the ability to lock the display so that no new points can be added
  VERSION 0.1.1: added critical sections and fixed a memory leak
  VERSION 0.2: points are now output in the order in which they were entered
  VERSION 0.3: Added a middle outlet to output the distances from the current point to all others
  VERSION 0.4: New algorithm, ability to reshape it by dragging concentric circles
  VERSION 0.5: Points are now stored in a linked hashtable so that we can operate on them by name
  VERSION 0.6: Lots of bugfixes and UI features:  dump, add_point, etc.
  VERSION 0.6.1: Now rescales properly and fixes a problem where a black line would appear at the bottom of the window when resizing
  VERSION 0.7: Reworked the mouse interaction to be closer to Ali's js version and more sane than prev versions of this obj
  VERSION 0.7.1: added add_point message and implemented a more sane scheme for choosing colors when none are supplied by the user
  VERSION 0.7.2: got rid of index stuff
  VERSION 0.7.3: uses layers to redraw the background only when needed
  VERSION 0.7.4: mouse stuff works for real this time
  VERSION 0.7.5: preset parameters can be adjusted by name
  VERSION 0.7.6: dump now outputs key value pairs
  VERSION 0.7.7: circle radii are now specified in [0-1]
  VERSION 0.7.8: locked presets are now grayed out and the mouse position outlet works
  VERSION 0.7.9: fixed a bug that would cause a crash in the anything routine
  VERSION 0.7.10: fixed a bug in the way points were being cleared
  VERSION 0.7.11: fixed the coords message which was upside down with respect to the y-axis
  VERSION 0.8: points now saved with patcher
  VERSION 0.8.1: added patchercoords argument to add_point message for faking drag and drop
  VERSION 0.8.2: added getnumpoints message, and fixed capslog bug
  VERSION 0.9: nested spaces--each point can now have it's own space
  VERSION 0.9.1: works properly in pres mode
  VERSION 0.9.2: mouse coords are correct when mouse_active_beyond_rect is off.
  VERSION 0.9.3: got rid of a nipple that would occur when the innner_radius > outer_radius
  VERSION 0.9.4: changed the format of the dump message
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h"
#include "jpatcher_api.h" 
#include "jgraphics.h"
#include "ext_critical.h"
#include "version.c"
#include "math.h"
#include <sys/time.h>

#ifdef WIN32
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#else
#include <mach/mach_time.h>
#endif

#define OUTPUT_CONCATENATE 0
#define OUTPUT_LIST 1

#define XY_INNER 1
#define XY_OUTER 2

/*
  #define MOUSEUP 0x10
  #define MOUSEDOWN 0x20
*/

#define INSIDE_INNER_CIRCLE 0x1
#define ON_INNER_CIRCLE 0x2
//#define INSIDE_OUTER_CIRCLE 0x4
#define ON_OUTER_CIRCLE 0x4

#define NUM_FIELDS_IN_POINT 12
typedef struct _point{
	//double x, y;
	t_pt pt;
	t_symbol *label;
	t_jrgba color;
	double exponent;
	double weight;
	double inner_radius, outer_radius;
	unsigned long mousestate;
	int locked;
	struct _space *space;
	struct _point *next;
	struct _point *prev;
} t_point;

typedef struct _space{
	t_point *points;
	int npoints;
	t_point *selected;
	struct _space *next;
} t_space;

typedef struct _rbfi{
	t_jbox ob;
	void *selectedOutlet, *listOutlet, *distanceOutlet, *mouseposOutlet, *dumpOutlet;
	t_critical lock;
	t_jrgba bgcolor, pointcolor, bordercolor, selectedcolor, labelcolor, xhaircolor;
	float pointdiameter;
	t_space *spaces;
	long nspaces;
	t_hashtab *ht;
	t_hashtab *master_hash;
	double xmin, xmax, ymin, ymax;
	int always_draw_circles, always_draw_labels;
	long monotonic_point_counter;
	long outputmode;
	long rbf;
	float line_width;
	long modifiers;
	t_pt xhairs;
	long mouse_active_beyond_rect;
	int font_size;
	struct timeval lastclick;
	t_object *pv;
	int compat_mode;
} t_rbfi;

static t_symbol *rbfi_ps_coords, *rbfi_ps_patchercoords, *rbfi_ps_name, *rbfi_ps_rgb, *rbfi_ps_hsv, /**rbfi_ps_weight, *rbfi_ps_exponent, */*rbfi_ps_inner_radius, *rbfi_ps_outer_radius, *rbfi_ps_locked;

static t_symbol *ps_top, *ps_done, *ps_clear, *ps_space, *ps_numpoints, *ps_mouse_down, *ps_mouse_up;

static t_symbol *l_color, *l_points, *l_xhairs;
static t_class *rbfi_class;

void rbfi_paint(t_rbfi *x, t_object *patcherview);
void rbfi_bang(t_rbfi *x);
void rbfi_list(t_rbfi *x, t_symbol *msg, short argc, t_atom *argv);
void rbfi_deletePoint(t_rbfi *x, t_symbol *name);
void rbfi_anything(t_rbfi *x, t_symbol *msg, short argc, t_atom *argv);
void rbfi_computeWeights(t_rbfi *x, t_pt coords, t_rect r, t_point *points, int nweights, double *weights);
void rbfi_move(t_rbfi *x, double xx, double yy);
void rbfi_pushPointName(t_rbfi *x, t_symbol *name);
void rbfi_push(t_rbfi *x, t_point *p);
void rbfi_pop(t_rbfi *x, t_symbol *top);
void rbfi_outputMousePos(t_rbfi *x, t_pt pt);
int rbfi_doubleclick(struct timeval t1, struct timeval t2);
void rbfi_mousedown(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers);
void rbfi_mousedrag(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers);
void rbfi_mouseup(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers);
void rbfi_mousemove(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers);
void rbfi_mouseleave(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers);
void rbfi_addPoint(t_rbfi *x, t_symbol *msg, int argc, t_atom *argv);
int rbfi_parseAddPointArgs(t_rbfi *x, t_point *p, int argc, t_atom *argv);
t_point *rbfi_addPointToList(t_rbfi *x, t_point point);
t_point *rbfi_newPoint(void);
void rbfi_initPoint(t_rbfi *x, t_point *p);
double rbfi_computeWeightFromDistances(double d1, double d2);
double rbfi_computeExponentFromDistances(double d1, double d2);
void rbfi_inner_radius(t_rbfi *x, double f);
void rbfi_outer_radius(t_rbfi *x, double f);
void rbfi_exponent(t_rbfi *x, double f);
void rbfi_weight(t_rbfi *x, double f);
t_point *rbfi_selectPoint(t_rbfi *x, t_pt pt);
void rbfi_removePoint(t_rbfi *x, t_point *p);
void rbfi_outputDistance(t_rbfi *x);
void rbfi_dump(t_rbfi *x);
int rbfi_getArraySize(t_rbfi *x);
void rbfi_toArray(t_space *s, t_atom *ar);
t_atom *rbfi_doToArray(t_space *s, t_atom *ar);
t_space *rbfi_fromArray(t_hashtab *ht, long n, t_atom *ar);
t_atom *rbfi_doFromArray(t_hashtab *ht, long n, t_atom *ar, t_space *space);
void rbfi_clear(t_rbfi *x);
void rbfi_free(t_rbfi *x);
void rbfi_assist(t_rbfi *x, void *b, long m, long a, char *s);
double rbfi_scale(double f, double min_in, double max_in, double min_out, double max_out);
double rbfi_clip(double f, double min, double max);
void rbfi_getnumpoints(t_rbfi *x);
t_max_err rbfi_getvalueof(t_rbfi *x, long *ac, t_atom **av);
t_max_err rbfi_setvalueof(t_rbfi *x, long ac, t_atom *av);
void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v );
void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v );
void *rbfi_new(t_symbol *msg, int argc, t_atom *argv);
void rbfi_postPoint(t_point *p);
t_max_err rbfi_notify(t_rbfi *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_max_err rbfi_spaces_get(t_rbfi *x, t_object *attr, long *argc, t_atom **argv);
t_max_err rbfi_spaces_set(t_rbfi *x, t_object *attr, long argc, t_atom *argv);
void rbfi_getRect(t_rbfi *x, t_rect *rect);

void rbfi_paint(t_rbfi *x, t_object *patcherview){
	x->pv = patcherview;
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

	{
		t_jgraphics *glayer = jbox_start_layer((t_object *)x, patcherview, l_color, rect.width, rect.height);

		if(glayer){
			t_jsurface *s = jgraphics_image_surface_create(JGRAPHICS_FORMAT_ARGB32, rect.width, rect.height);
			t_rect srect = rect;
			t_rect drect = rect;
			srect.x = srect.y = drect.x = drect.y = 0.;
			if(1){
				double s = rect.width / 128.;
				srect.width = 128.;
				srect.height = ceil(rect.height / s);
			}else{
			}
			// color
			{
				int i, j, k;
				int w = srect.width;
				int h = srect.height;
				double sum;
				double weights[x->spaces->npoints];
				for(i = 0; i < w; i++){
					for(j = 0; j < h; j++){
						t_point *p = x->spaces->points;
						sum = 0;
						t_jrgba color = {0., 0., 0., 1.};
						double ii = rbfi_scale(i, 0, srect.width, 0, drect.width);
						double jj = rbfi_scale(j, 0, srect.width, 0, drect.width);
						rbfi_computeWeights(x, (t_pt){ii, jj}, rect, x->spaces->points, x->spaces->npoints, weights);

						k = 0;
						while(p){
							color.red += weights[k] * p->color.red;
							color.green += weights[k] * p->color.green;
							color.blue += weights[k] * p->color.blue;
							sum += weights[k];
							p = p->next;
							k++;
						}
						color.red /= sum;
						color.green /= sum;
						color.blue /= sum;
						jgraphics_image_surface_set_pixel(s, i, j, color);
					}
				}
			}
			jgraphics_image_surface_draw(glayer, s, srect, drect);
			if(s){
				jgraphics_surface_destroy(s);
			}
		}
		jbox_end_layer((t_object *)x, patcherview, l_color);
		jbox_paint_layer((t_object *)x, patcherview, l_color, 0, 0);
	}
	// draw points
	{
		jgraphics_set_line_width(g, 1);
		t_point *p = x->spaces->points;
		critical_enter(x->lock);
		t_jrgba white = {1., 1., 1., 1.};
		t_jrgba gray = {.75, .75, .75, 1.};
		while(p){
			if(p == x->spaces->selected){
				jgraphics_set_source_jrgba(g, &(white));
			}else{
				jgraphics_set_source_jrgba(g, &(gray));
			}
			t_jrgba color;
			t_pt pt = p->pt;
			pt.x = rbfi_scale(pt.x, x->xmin, x->xmax, 0, rect.width);
			pt.y = rbfi_scale(pt.y, x->ymin, x->ymax, rect.height, 0);
			{
				if(x->always_draw_circles == 1 && (x->modifiers ^ 0x2) && (x->modifiers ^ 0x12)){
					if(p->locked){
						color = gray;
					}else{
						color = white;
					}
					jgraphics_set_dash(g, NULL, 0, 0);
					jgraphics_set_line_width(g, 2.);
					jgraphics_set_source_jrgba(g, &color);
					jgraphics_ellipse(g, pt.x - p->inner_radius * rect.width, pt.y - p->inner_radius * rect.width, p->inner_radius * rect.width * 2, p->inner_radius * rect.width * 2);
					jgraphics_stroke(g);
				}else if(x->modifiers & 0x2){
					if(p == x->spaces->selected){
						if((p->mousestate == INSIDE_INNER_CIRCLE) || (p->mousestate == ON_INNER_CIRCLE)){
							color = white;
						}else{
							color = gray;
						}
					}else{
						color = gray;
					}
					if(p->locked){
						color = gray;
					}

					jgraphics_set_dash(g, NULL, 0, 0);
					jgraphics_set_line_width(g, 2.);
					jgraphics_set_source_jrgba(g, &color);
					jgraphics_ellipse(g, pt.x - p->inner_radius * rect.width, pt.y - p->inner_radius * rect.width, p->inner_radius * rect.width * 2, p->inner_radius * rect.width * 2);
					jgraphics_stroke(g);
				}

				if(x->always_draw_circles == 1 && (x->modifiers ^ 0x2) && (x->modifiers ^ 0x12)){
					if(p->locked){
						color = gray;
					}else{
						color = white;
					}
					jgraphics_set_dash(g, (double[2]){3., 3.}, 2, 0);
					jgraphics_set_line_width(g, 1.);
					jgraphics_set_source_jrgba(g, &color);
					jgraphics_ellipse(g, pt.x - p->outer_radius * rect.width, pt.y - p->outer_radius * rect.width, p->outer_radius * rect.width * 2, p->outer_radius * rect.width * 2);
					jgraphics_stroke(g);
				}else if(x->modifiers & 0x2){
					if(p == x->spaces->selected){
						//post("%s is selected, comparing 0x%x to 0x%x and 0x%x", p->label->s_name, p->mousestate, (MOUSEUP | INSIDE_INNER_CIRCLE), (MOUSEUP | ON_OUTER_CIRCLE));
						if((p->mousestate == INSIDE_INNER_CIRCLE) || (p->mousestate == ON_OUTER_CIRCLE)){
							color = white;
						}else{
							color = gray;
						}
					}else{
						color = gray;
					}
					if(p->locked){
						color = gray;
					}
					jgraphics_set_dash(g, (double[2]){3., 3.}, 2, 0);
					jgraphics_set_line_width(g, 1.);
					jgraphics_set_source_jrgba(g, &color);
					jgraphics_ellipse(g, pt.x - p->outer_radius * rect.width, pt.y - p->outer_radius * rect.width, p->outer_radius * rect.width * 2, p->outer_radius * rect.width * 2);
					jgraphics_stroke(g);
				}
			}

			//if((x->draw_labels == gensym("edit") && (x->modifiers & 0x10)) || x->draw_labels == gensym("always")){
			{
				if(x->always_draw_labels && (x->modifiers ^ 0x2) && (x->modifiers ^ 0x12)){
					if(p->locked){
						color = gray;
					}else{
						color = white;
					}

					if(p->locked){
						jgraphics_select_font_face(g, "Arial", JGRAPHICS_FONT_SLANT_ITALIC, JGRAPHICS_FONT_WEIGHT_NORMAL);
					}else{
						jgraphics_select_font_face(g, "Arial", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
					}
					jgraphics_set_source_jrgba(g, &(color));
					jgraphics_set_font_size(g, x->font_size);
					double w, h;
					jgraphics_text_measure(g, p->label->s_name, &w, &h);
					jgraphics_move_to(g, pt.x - (w / 2.), pt.y + (h / 2.) - 1);
					jgraphics_show_text(g, p->label->s_name);
				}else if(x->modifiers & 0x2){
					if(p == x->spaces->selected){
						if(p->mousestate & 0xful){
							color = white;
						}else{
							color = gray;
						}
					}else{
						color = gray;
					}

					if(p->locked){
						jgraphics_select_font_face(g, "Arial", JGRAPHICS_FONT_SLANT_ITALIC, JGRAPHICS_FONT_WEIGHT_NORMAL);
						color = gray;
					}else{
						jgraphics_select_font_face(g, "Arial", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
					}

					jgraphics_set_source_jrgba(g, &(color));
					jgraphics_set_font_size(g, x->font_size);
					double w, h;
					jgraphics_text_measure(g, p->label->s_name, &w, &h);
					jgraphics_move_to(g, pt.x - (w / 2.), pt.y + (h / 2.) - 1);
					jgraphics_show_text(g, p->label->s_name);
				}
			}
			jgraphics_stroke(g);

			p = p->next;
		}
		critical_exit(x->lock);
	}

	jgraphics_set_dash(g, NULL, 0, 0);

	// draw mouse cursor
	{
		jgraphics_set_source_jrgba(g, &(x->xhaircolor));
		jgraphics_move_to(g, x->xhairs.x - 4, x->xhairs.y - 4);
		jgraphics_line_to(g, x->xhairs.x + 4, x->xhairs.y + 4);
		jgraphics_move_to(g, x->xhairs.x + 4, x->xhairs.y - 4);
		jgraphics_line_to(g, x->xhairs.x - 4, x->xhairs.y + 4);
		jgraphics_stroke(g);
	}
}

void rbfi_bang(t_rbfi *x){
}

void rbfi_list(t_rbfi *x, t_symbol *msg, short argc, t_atom *argv){

}

void rbfi_deletePoint(t_rbfi *x, t_symbol *name){
	t_point *p;
	critical_enter(x->lock);
	hashtab_lookup(x->ht, name, (t_object **)&p);
	if(p){
		rbfi_removePoint(x, p);
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_anything(t_rbfi *x, t_symbol *msg, short argc, t_atom *argv){
	t_symbol *name = msg;
	if(!name){
		object_error((t_object *)x, "bad arguments");
		return;
	}
	t_point *p = NULL;
	critical_enter(x->lock);
	hashtab_lookup(x->ht, name, (t_object **)&p);
	if(p){
		if(p->locked){
			critical_exit(x->lock);
			return;
		}
		rbfi_parseAddPointArgs(x, p, argc, argv);
		t_rect r;
		rbfi_getRect(x, &r);
		p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
		p->exponent = rbfi_computeExponentFromDistances(p->inner_radius  * r.width, p->outer_radius * r.width);
	}else{
		object_error((t_object *)x, "no point with name %s", name->s_name);
		critical_exit(x->lock);
		return;
	}		
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_computeWeights(t_rbfi *x, t_pt coords, t_rect r, t_point *points, int nweights, double *weights){
	critical_enter(x->lock);
	t_point *p = points;
	int i = 0;
	double sum = 0;
	while(p){
		if((p->inner_radius == 0.0) && (p->outer_radius == 0.0)){
			weights[i] = 0;
			p = p->next;
			continue;
		}
		t_pt pt = p->pt;
		pt.x = rbfi_scale(pt.x, x->xmin, x->xmax, 0, r.width);
		pt.y = rbfi_scale(pt.y, x->ymin, x->ymax, r.height, 0);
		if(coords.x == pt.x && coords.y == pt.y){
			if(p->inner_radius > p->outer_radius){
				if(p->outer_radius == 0){
					weights[i] = 1. / p->inner_radius;
				}else{
					weights[i] = 0.0;
				}
			}else if(p->inner_radius == 0){
				weights[i] = 1. / p->outer_radius;
			}else{
				memset(weights, 0, nweights * sizeof(double));
				weights[i] = 1.;
			}
			critical_exit(x->lock);
			return;
		}else{
			
			if(p->outer_radius == 0){
				weights[i] = 1. / p->inner_radius;
			}else if(p->inner_radius == 0){
				weights[i] = 1. / p->outer_radius;
			}else{
				double d = sqrt(pow(coords.x - pt.x, 2.) + pow(coords.y - pt.y, 2.));
				//post("d = %f, ir = %f, or = %f", d, p->inner_radius, p->outer_radius);
			
				//if(d > (p->inner_radius < p->outer_radius ? p->outer_radius : p->inner_radius) * r.width * x->boobs){
				//weights[i] = 0;
				//}else{
				//	weights[i] = pow(1. / d, p->exponent) * p->weight;
				//}
				//weights[i] = sin(weights[i] * d) / (weights[i]* d);
				weights[i] = pow(1. / d, p->exponent) * p->weight;
				if(isinf(weights[i])){
					memset(weights, 0, nweights * sizeof(double));
					weights[i] = 1.;
					critical_exit(x->lock);
					return;
				}
			}
		}
		sum += weights[i];
		p = p->next;
		i++;
	}
	critical_exit(x->lock);
	for(i = 0; i < nweights; i++){
		//post("weights[%d]: %f %f, sum = %f", i, weights[i], weights[i] / sum, sum);
		weights[i] /= sum;
	}
}

void rbfi_move(t_rbfi *x, double xx, double yy){
	t_rect r;
	rbfi_getRect(x, &r);

	double weights[x->spaces->npoints];
	double xx_screen = rbfi_scale(xx, x->xmin, x->xmax, 0, r.width);
	double yy_screen = rbfi_scale(yy, x->ymin, x->ymax, r.height, 0);
	rbfi_computeWeights(x, (t_pt){xx_screen,yy_screen}, r, x->spaces->points, x->spaces->npoints, weights);
	t_atom out[2];
	int i = 0;
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	while(p){
		atom_setsym(out, p->label);
		atom_setfloat(out + 1, weights[i]);
		outlet_list(x->listOutlet, NULL, 2, out);
		i++;
		p = p->next;
	}
	x->xhairs = (t_pt){xx_screen,yy_screen};
	critical_exit(x->lock);
	outlet_anything(x->listOutlet, ps_done, 0, NULL);
	jbox_redraw(&(x->ob));
}

void rbfi_pushPointName(t_rbfi *x, t_symbol *name){
	if(!name){
		return;
	}
	t_point *p = NULL;
	critical_enter(x->lock);
	hashtab_lookup(x->ht, name, (t_object **)&p);
	if(p){
		rbfi_push(x, p);
	}else{
		object_error((t_object *)x, "no point found with name %s", name->s_name);
	}
	critical_exit(x->lock);
}

void rbfi_push(t_rbfi *x, t_point *p){
	critical_enter(x->lock);
	if(p->space){
		p->space->next = x->spaces;
		x->spaces = p->space;
	}else{
		p->space = (t_space *)sysmem_newptr(sizeof(t_space));
		t_space *s = p->space;
		s->selected = NULL;
		s->points = NULL;
		s->npoints = 0;
		p->space->next = x->spaces;
		x->spaces = p->space;
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_pop(t_rbfi *x, t_symbol *top){
	critical_enter(x->lock);
	if(top == ps_top){
		t_space *s = x->spaces;
		while(s->next){
			s = s->next;
		}
		x->spaces = s;
	}else if(x->spaces->next){
		x->spaces = x->spaces->next;
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

int rbfi_doubleclick(struct timeval t1, struct timeval t2){
	double elapsed = (t2.tv_sec - t1.tv_sec) * 1000.;
	elapsed += (t2.tv_sec - t1.tv_sec) / 1000.;
	if(elapsed > 200){
		return 0;
	}
	return 1;
}

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 

#define CIRCLE_HIT 3 // num pixels on either side of a circle that will register a hit

void rbfi_outputMousePos(t_rbfi *x, t_pt pt){
	t_rect r;
	rbfi_getRect(x, &r);
	t_atom pos[2];
	atom_setfloat(pos, rbfi_scale(pt.x, 0., r.width, x->xmin, x->xmax));
	atom_setfloat(pos + 1, rbfi_scale(pt.y, r.height, 0., x->ymin, x->ymax));
	outlet_list(x->mouseposOutlet, NULL, 2, pos);
}

void rbfi_mousedown(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int doubleclick = rbfi_doubleclick(x->lastclick, tv);
	x->lastclick = tv;
	x->modifiers = modifiers;
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	outlet_anything(x->mouseposOutlet, ps_mouse_down, 0, NULL);
	rbfi_outputMousePos(x, pt);

	switch(modifiers){
	case 0x110:
	case 0x10:
		{
			t_atom out[2];
			int i = 0;
			critical_enter(x->lock);
			double weights[x->spaces->npoints];
			rbfi_computeWeights(x, pt, r, x->spaces->points, x->spaces->npoints, weights);
			t_point *p = x->spaces->points;
			while(p){
				atom_setsym(out, p->label);
				atom_setfloat(out + 1, weights[i]);
				outlet_list(x->listOutlet, NULL, 2, out);
				i++;
				p = p->next;
			}
			critical_exit(x->lock);
			x->xhairs = pt;
			outlet_anything(x->listOutlet, ps_done, 0, NULL);
		}
		break;
	case 0x112:
	case 0x12:
		{
			critical_enter(x->lock);
			if(doubleclick && x->spaces->selected){
				// push into a new space
				rbfi_push(x, x->spaces->selected);
			}
			critical_exit(x->lock);
			//x->spaces->selected = rbfi_selectPoint(x, pt);
		}
		break;
	case (0x112 | 0x111):
	case (0x12 | 0x11):
		// command-shift -> delete a point 
		{
			critical_enter(x->lock);
			if(x->spaces->selected){
				if(x->spaces->selected->locked){
					break;
				}
				if(x->spaces->selected->mousestate == INSIDE_INNER_CIRCLE){
					rbfi_removePoint(x, x->spaces->selected);
					x->spaces->selected = NULL;
					jbox_invalidate_layer((t_object *)x, NULL, l_color);
				}
			}
			critical_exit(x->lock);
		}
		break;
	case 0x11a:
	case 0x1a:
		// option-shift -> make a new point
		{
			t_atom a[3];
			atom_setsym(a, rbfi_ps_coords);
			atom_setfloat(a + 1, rbfi_scale(pt.x, 0., r.width, x->xmin, x->xmax));
			atom_setfloat(a + 2, rbfi_scale(pt.y, r.height, 0., x->ymin, x->ymax));
			rbfi_addPoint(x, NULL, 3, a);
			jbox_invalidate_layer((t_object *)x, NULL, l_color);
		}
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	rbfi_outputDistance(x);
}

void rbfi_mousedrag(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x110:
	case 0x10:
		{
			critical_enter(x->lock);
			if(!(x->mouse_active_beyond_rect)){
				pt = (t_pt){
					rbfi_clip(pt.x, 0, r.width),
					rbfi_clip(pt.y, 0, r.height)
				};
			}
			double weights[x->spaces->npoints];
			rbfi_computeWeights(x, pt, r, x->spaces->points, x->spaces->npoints, weights);
			t_atom out[2];
			int i = 0;
			t_point *p = x->spaces->points;
			while(p){
				atom_setsym(out, p->label);
				atom_setfloat(out + 1, weights[i]);
				outlet_list(x->listOutlet, NULL, 2, out);
				i++;
				p = p->next;
			}
			x->xhairs = pt;
			critical_exit(x->lock);
			outlet_anything(x->listOutlet, ps_done, 0, NULL);
		}
		break;
	case 0x112:
	case 0x12:
		// shift -> edit point
		critical_enter(x->lock);
		if(x->spaces->selected){
			if(x->spaces->selected->locked){
				break;
			}
			switch(x->spaces->selected->mousestate){
			case INSIDE_INNER_CIRCLE:
				x->spaces->selected->pt = (t_pt){
					rbfi_scale(pt.x, 0., r.width, x->xmin, x->xmax),
					rbfi_scale(pt.y, r.height, 0., x->ymin, x->ymax)
				};
				break;
			case ON_INNER_CIRCLE:
				{
					t_point *p = x->spaces->selected;
					t_pt sc = p->pt;
					//sc.x *= r.width;
					//sc.y = r.height - sc.y * r.height;
					sc.x = rbfi_scale(p->pt.x, x->xmin, x->xmax, 0., r.width);
					sc.y = rbfi_scale(p->pt.y, x->ymin, x->ymax, r.height, 0.);
					p->inner_radius = sqrt(pow(pt.x - sc.x, 2.) + pow(pt.y - sc.y, 2.)) / r.width;
					p->exponent = rbfi_computeExponentFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
					p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
				}
				break;
			case ON_OUTER_CIRCLE:
				{
					t_point *p = x->spaces->selected;
					t_pt sc = p->pt;
					sc.x = rbfi_scale(p->pt.x, x->xmin, x->xmax, 0., r.width);
					sc.y = rbfi_scale(p->pt.y, x->ymin, x->ymax, r.height, 0.);
					p->outer_radius = sqrt(pow(pt.x - sc.x, 2.) + pow(pt.y - sc.y, 2.)) / r.width;
					p->exponent = rbfi_computeExponentFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
					p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
				}
				break;
			}
		        jbox_invalidate_layer((t_object *)x, NULL, l_color);
		}
		critical_exit(x->lock);
		break;
	}
	rbfi_outputMousePos(x, pt);
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	rbfi_outputDistance(x);
}

void rbfi_mouseup(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	outlet_anything(x->mouseposOutlet, ps_mouse_up, 0, NULL);
}

void rbfi_mouseleave(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers){
	x->modifiers = 0;
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	critical_enter(x->lock);
	x->spaces->selected = NULL;
	critical_exit(x->lock);
	jbox_redraw(&(x->ob));
}

void rbfi_mousemove(t_rbfi *x, t_object *patcherview, t_pt pt, long modifiers){
	x->modifiers = modifiers;
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x2:
		{
			critical_enter(x->lock);
			// shift
			t_point *p = x->spaces->points;
			int i = 0;
			while(p){
				if(p->locked){
					p = p->next;
					continue;
				}
				t_pt sc;
				sc.x = rbfi_scale(p->pt.x, x->xmin, x->xmax, 0., r.width);
				sc.y = rbfi_scale(p->pt.y, x->ymin, x->ymax, r.height, 0.);
				i++;
				double pos = sqrt(pow(pt.x - sc.x, 2.) + pow(pt.y - sc.y, 2.));
				p->mousestate = 0;
				if(fabs(pos - p->inner_radius * r.width) <= CIRCLE_HIT){
					p->mousestate |= ON_INNER_CIRCLE;
					if(x->spaces->selected){
						if(fabs(pos - p->inner_radius * r.width) < fabs(pos - x->spaces->selected->inner_radius * r.width)){
							x->spaces->selected = p;
						}
					}else{
						x->spaces->selected = p;
					}
				}else if(fabs(pos - p->outer_radius * r.width) <= CIRCLE_HIT){
					p->mousestate |= ON_OUTER_CIRCLE;
					if(x->spaces->selected){
						if(fabs(pos - p->outer_radius * r.width) < fabs(pos - x->spaces->selected->outer_radius * r.width)){
							x->spaces->selected = p;
						}
					}else{
						x->spaces->selected = p;
					}
				}else if(p->inner_radius * r.width - pos >= 0){
					p->mousestate |= INSIDE_INNER_CIRCLE;
					if(x->spaces->selected){
						if((p->inner_radius * r.width - pos) < (x->spaces->selected->inner_radius * r.width - pos)){
							x->spaces->selected = p;
						}
					}else{
						x->spaces->selected = p;
					}
				}else if(p == x->spaces->selected){
					x->spaces->selected = NULL;
				}
				p = p->next;
			}
		}
		critical_exit(x->lock);
		break;
	}
	jbox_redraw(&(x->ob));
}

// this is called in response to the "add_point" message from Max
void rbfi_addPoint(t_rbfi *x, t_symbol *msg, int argc, t_atom *argv){
	t_point p;
	rbfi_initPoint(x, &p);
	rbfi_parseAddPointArgs(x, &p, argc, argv);

	t_rect r;
	rbfi_getRect(x, &r);
	p.weight = rbfi_computeWeightFromDistances(p.inner_radius * r.width, p.outer_radius * r.width);
	p.exponent = rbfi_computeExponentFromDistances(p.inner_radius  * r.width, p.outer_radius * r.width);
	critical_enter(x->lock);
	x->spaces->selected = rbfi_addPointToList(x, p);
	critical_exit(x->lock);
	//rbfi_postPoint(x->spaces->selected);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

int rbfi_parseAddPointArgs(t_rbfi *x, t_point *p, int argc, t_atom *argv){
	if(argc == 0){
		return 0;
	}
	t_symbol *s;
	t_atom *ptr = argv;
	if(atom_gettype(ptr) != A_SYM){
		error("rbfi: error parsing arguments to add_point.  expected a symbol and found something else.");
		return 1;
	}
	s = atom_getsym(ptr++);
	if(s == rbfi_ps_coords){
		if(argc - (ptr - argv) < 2){
			goto bail;
		}
		p->pt = (t_pt){atom_getfloat(ptr++), atom_getfloat(ptr++)};
	}else if(s == rbfi_ps_patchercoords){
		if(argc - (ptr - argv) < 2){
			goto bail;
		}
		t_rect r;
		rbfi_getRect(x, &r);
		float xx = atom_getfloat(ptr++);
		float yy = atom_getfloat(ptr++);
		xx -= r.x;
		yy -= r.y;
		//post("%s: xx = %f yy = %f", __PRETTY_FUNCTION__, xx, yy);
		p->pt = (t_pt){rbfi_scale(xx, 0, r.width, x->xmin, x->xmax), rbfi_scale(yy, r.height, 0, x->ymin, x->ymax)};
	}else if(s == rbfi_ps_name){
		if(argc - (ptr - argv) < 1){
			goto bail;
		}
		p->label = atom_getsym(ptr++);
	}else if(s == rbfi_ps_rgb){
		if(argc - (ptr - argv) == 0){
			goto bail;
		}
		if(atom_gettype(ptr) == A_FLOAT){
			// this is probably a set of floats
			if(argc - (ptr - argv) < 3){
				goto bail;
			}
			p->color = (t_jrgba){atom_getfloat(ptr++), atom_getfloat(ptr++), atom_getfloat(ptr++), 1.};
		}else if(atom_gettype(ptr) == A_SYM){
			t_symbol *hex = atom_getsym(ptr++);
			if(*(hex->s_name) == '0' && (*(hex->s_name + 1) == 'x' || *(hex->s_name + 1) == 'X')){
				// hex
				long h = strtol(hex->s_name, NULL, 0);
				p->color.red = (double)((h & 0xff0000) / 0xff);
				p->color.green = (double)((h & 0xff00) / 0xff);
				p->color.blue = (double)((h & 0xff) / 0xff);
			}else{
				error("rbfi: unrecognized argument for key rgb");
				return 1;
			}
		}
	}else if(s == rbfi_ps_hsv){
		if(argc - (ptr - argv) == 0){
			goto bail;
		}
		if(atom_gettype(ptr) == A_FLOAT){
			// this is probably a set of floats
			if(argc - (ptr - argv) < 3){
				goto bail;
			}
			p->color = (t_jrgba){atom_getfloat(ptr++), atom_getfloat(ptr++), atom_getfloat(ptr++), 1.};
		}else if(atom_gettype(ptr) == A_SYM){
			t_symbol *hex = atom_getsym(ptr++);
			if(*(hex->s_name) == '0' && (*(hex->s_name + 1) == 'x' || *(hex->s_name + 1) == 'X')){
				// hex
				long h = strtol(hex->s_name, NULL, 0);
				p->color.red = (double)((h & 0xff0000) / 0xff);
				p->color.green = (double)((h & 0xff00) / 0xff);
				p->color.blue = (double)((h & 0xff) / 0xff);
			}else{
				error("rbfi: unrecognized argument for key rgb");
				return 1;
			}
		}
		/*
		  }else if(s == rbfi_ps_weight){
		  if(argc - (ptr - argv) < 1){
		  goto bail;
		  }
		  p->weight = atom_getfloat(ptr++);
		  }else if(s == rbfi_ps_exponent){
		  if(argc - (ptr - argv) < 1){
		  goto bail;
		  }
		  p->exponent = atom_getfloat(ptr++);
		*/
	}else if(s == rbfi_ps_inner_radius){
		if(argc - (ptr - argv) < 1){
			goto bail;
		}
		p->inner_radius = atom_getfloat(ptr++);
	}else if(s == rbfi_ps_outer_radius){
		if(argc - (ptr - argv) < 1){
			goto bail;
		}
		p->outer_radius = atom_getfloat(ptr++);
	}else if(s == rbfi_ps_locked){
		if(argc - (ptr - argv) < 1){
			goto bail;
		}
		p->locked = atom_getlong(ptr++);
	}else{
		error("rbfi: unrecognized key %s", s->s_name);
	}
	return rbfi_parseAddPointArgs(x, p, argc - (ptr - argv), ptr);

 bail:
	error("rbfi: not enough arguments for key %s", s->s_name);
	return 1;
}

t_point *rbfi_addPointToList(t_rbfi *x, t_point point){
	critical_enter(x->lock);
	t_hashtab *ht = x->ht;
	t_point *p = NULL;
	if(point.label){
		hashtab_lookup(ht, point.label, (t_object **)&p);
		if(p){
			p->pt = point.pt;
			p->color = point.color;
			p->exponent = point.exponent;
			p->weight = point.weight;
			p->inner_radius = point.inner_radius;
			p->outer_radius = point.outer_radius;
			p->mousestate = 0;
			critical_exit(x->lock);
			return p;
		}
	}
	if(!p){
		p = rbfi_newPoint();
	}
	if(!p){
		critical_exit(x->lock);
		return NULL;
	}
	memcpy(p, &point, sizeof(t_point));

	p->next = x->spaces->points;
	p->prev = NULL;
	if(x->spaces->points){
		x->spaces->points->prev = p;
	}
	x->spaces->points = p;
	x->spaces->npoints++;

	hashtab_store(ht, p->label, (t_object *)p);

	critical_exit(x->lock);
	return p;
}

t_point *rbfi_newPoint(void){
	t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
	if(p){
		return p;
	}else{
		error("rbfi: out of memory!  failed to allocate a new point");
		return NULL;
	}
}

void rbfi_initPoint(t_rbfi *x, t_point *p){
	t_rect r;
	rbfi_getRect(x, &r);
	p->pt = (t_pt){(double)rand() / RAND_MAX, (double)rand() / RAND_MAX};
	char buf[128];
	critical_enter(x->lock);
	if(x->spaces->next){
		if(x->spaces->next->selected){
			sprintf(buf, "%s/p%ld", x->spaces->next->selected->label->s_name, x->monotonic_point_counter++);
		}else{
			sprintf(buf, "/NULL/p%ld", x->monotonic_point_counter++);
		}
	}else{
		sprintf(buf, "/p%ld", x->monotonic_point_counter++);
	}
	critical_exit(x->lock);
	p->label = gensym(buf);
	HSVtoRGB(&(p->color.red), &(p->color.green), &(p->color.blue), (x->monotonic_point_counter * 20) % 360, 0.75, 0.75);
	p->color.alpha = 0;
	p->inner_radius = .05;
	p->outer_radius = .2;
	p->exponent = rbfi_computeExponentFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
	p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
	p->mousestate = 0;
	p->locked = 0;
	p->space = NULL;
}

#define THRESH .9
double rbfi_computeWeightFromDistances(double d1, double d2){
	if(d1 == 0){
		return 1. / d2;
	}else if(d2 == 0){
		return 1. / d1;
	}
	double dd1 = d1;
	if(dd1 == 0){
		dd1 = .00000001;
	}
	double dd2 = d2;
	if((dd1 / dd2) <= 1 && (dd1 / dd2) >= THRESH){
		dd1 = THRESH * dd2;
	}else if((dd1 / dd2) >= 1 && (dd1 / dd2) <= (1. / THRESH)){
		dd1 = dd2 / THRESH;
	}

	double w = exp(-((log(2.) + log(5.)) * (log(dd1) + log(dd2))) / (log(dd1) - log(dd2)));
	return w;
}

double rbfi_computeExponentFromDistances(double d1, double d2){
	if(d1 == 0 || d2 == 0){
		return 1;
	}
	double dd1 = d1;
	if(dd1 == 0){
		dd1 = .00000001;
	}
	double dd2 = d2;
	if((dd1 / dd2) <= 1 && (dd1 / dd2) >= THRESH){
		dd1 = THRESH * dd2;
	}else if((dd1 / dd2) >= 1 && (dd1 / dd2) <= (1. / THRESH)){
		dd1 = dd2 / THRESH;
	}

	double d = (2 * (log(2.) + log(5.))) / (-log(dd1) + log(dd2));
	return d;
}

void rbfi_inner_radius(t_rbfi *x, double f){
	t_rect r;
	rbfi_getRect(x, &r);
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	while(p){
		p->inner_radius = f;
		p->exponent = rbfi_computeExponentFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
		p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
		p = p->next;
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_outer_radius(t_rbfi *x, double f){
	t_rect r;
	rbfi_getRect(x, &r);
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	while(p){
		p->outer_radius = f;
		p->exponent = rbfi_computeExponentFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
		p->weight = rbfi_computeWeightFromDistances(p->inner_radius * r.width, p->outer_radius * r.width);
		p = p->next;
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_exponent(t_rbfi *x, double f){
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	while(p){
		p->exponent = f;
		p = p->next;
	}
	critical_exit(x->lock);
	jbox_redraw(&(x->ob));
}

void rbfi_weight(t_rbfi *x, double f){
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	while(p){
		p->weight = f;
		p = p->next;
	}
	critical_exit(x->lock);
	jbox_redraw(&(x->ob));
}

t_point *rbfi_selectPoint(t_rbfi *x, t_pt pt){
	critical_enter(x->lock);
	t_point *p = x->spaces->points;
	t_rect r;
	rbfi_getRect(x, &r);
	while(p){
		t_pt sc = p->pt;
		sc.x = rbfi_scale(p->pt.x, x->xmin, x->xmax, 0., r.width);
		sc.y = rbfi_scale(p->pt.y, x->ymin, x->ymax, r.height, 0.);
		if(pt.x >= (sc.x - x->pointdiameter) && pt.x <= (sc.x + x->pointdiameter) &&
		   pt.y >= (sc.y - x->pointdiameter) && pt.y <= (sc.y + x->pointdiameter)){
			goto out;
		}
		p = p->next;
	}
 out:
	critical_exit(x->lock);
	return p;
}

void rbfi_removePoint(t_rbfi *x, t_point *p){
	critical_enter(x->lock);
	if(p->prev){
		p->prev->next = p->next;
	}else{
		x->spaces->points = p->next;
	}
	if(p->next){
		p->next->prev = p->prev;
	}
	if(p->label){
		hashtab_chuckkey(x->ht, p->label);
	}
	sysmem_freeptr((void *)p);
	x->spaces->npoints--;
	critical_exit(x->lock);
}

void rbfi_outputDistance(t_rbfi *x){
	t_rect r;
	rbfi_getRect(x, &r);
	t_point *p = x->spaces->points;
	t_atom out[x->spaces->npoints * 2];
	t_atom *outptr = out;
	critical_enter(x->lock);
	while(p){
		atom_setsym(outptr++, p->label);
		atom_setfloat(outptr++, sqrt(pow(rbfi_scale(x->xhairs.x, 0, r.width, x->xmin, x->xmax) - p->pt.x, 2.) + pow(rbfi_scale(x->xhairs.y, r.height, 0, x->ymin, x->ymax) - p->pt.y, 2.)));
		p = p->next;
	}
	critical_exit(x->lock);
	outlet_list(x->distanceOutlet, NULL, x->spaces->npoints * 2, out);
}

void rbfi_dump(t_rbfi *x){
	t_rect r;
	rbfi_getRect(x, &r);

	long argc;
	t_atom *argv = NULL;
	critical_enter(x->lock);
	t_space *s = x->spaces;
	while(s->next){
		s = s->next;
	}

	if(x->compat_mode){
		argc = rbfi_getArraySize(x);
		argv = (t_atom *)sysmem_newptr(argc * sizeof(t_atom));
		rbfi_toArray(s, argv);

		outlet_list(x->dumpOutlet, NULL, argc, argv);
	}else{
		t_point *p = x->spaces->points;
		while(p){
			t_atom out[16];
			t_atom *ptr = out;
			atom_setsym(ptr++, rbfi_ps_name);
			atom_setsym(ptr++, p->label);
			atom_setsym(ptr++, rbfi_ps_coords);
			atom_setfloat(ptr++, p->pt.x);
			atom_setfloat(ptr++, p->pt.y);
			atom_setsym(ptr++, rbfi_ps_rgb);
			atom_setfloat(ptr++, p->color.red);
			atom_setfloat(ptr++, p->color.green);
			atom_setfloat(ptr++, p->color.blue);
			atom_setsym(ptr++, rbfi_ps_inner_radius);
			atom_setfloat(ptr++, p->inner_radius);
			atom_setsym(ptr++, rbfi_ps_outer_radius);
			atom_setfloat(ptr++, p->outer_radius);
			outlet_anything(x->dumpOutlet, p->label, ptr - out, out);
			p = p->next;
		}
	}
	critical_exit(x->lock);

	if(argv){
		sysmem_freeptr(argv);
	}
}

int rbfi_getArraySize(t_rbfi *x){
	long n = 0, i, nkeys;
	t_symbol **keys = NULL;;
	critical_enter(x->lock);
	hashtab_getkeys(x->ht, &nkeys, &keys);
	for(i = 0; i < nkeys; i++){
		t_point *p;
		hashtab_lookup(x->ht, keys[i], (t_object **)(&p));
		if(p){
			n += NUM_FIELDS_IN_POINT;
			if(p->space){
				n += 2;
			}
		}
	}
	critical_exit(x->lock);
	return n + 2;
}

void rbfi_toArray(t_space *s, t_atom *ar){
	rbfi_doToArray(s, ar);
}

t_atom *rbfi_doToArray(t_space *s, t_atom *ar){
	t_atom *ptr = ar;
	atom_setsym(ptr++, ps_space);
	atom_setlong(ptr++, s->npoints);
	t_point *p = s->points;
	if(!p){
		return ptr;
	}
	while(p->next){
		p = p->next;
	}
	while(p){
		atom_setfloat(ptr++, p->pt.x);
		atom_setfloat(ptr++, p->pt.y);
		atom_setsym(ptr++, p->label);
		atom_setfloat(ptr++, p->color.red);
		atom_setfloat(ptr++, p->color.green);
		atom_setfloat(ptr++, p->color.blue);
		atom_setfloat(ptr++, p->color.alpha);
		atom_setfloat(ptr++, p->exponent);
		atom_setfloat(ptr++, p->weight);
		atom_setfloat(ptr++, p->inner_radius);
		atom_setfloat(ptr++, p->outer_radius);
		atom_setlong(ptr++, p->locked);
		if(p->space){
			ptr = rbfi_doToArray(p->space, ptr);
		}
		p = p->prev;
	}
	return ptr;
}

t_space *rbfi_fromArray(t_hashtab *ht, long n, t_atom *ar){
	//t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
	t_space *s = (t_space *)sysmem_newptr(sizeof(t_space));
	s->selected = NULL;
	s->points = NULL;
	s->npoints = 0;
	s->next = NULL;
	rbfi_doFromArray(ht, n, ar, s);
	return s;
}

t_atom *rbfi_doFromArray(t_hashtab *ht, long n, t_atom *ar, t_space *space){
	t_atom *ptr = ar;
	if(atom_gettype(ptr) == A_SYM){
		if(atom_getsym(ptr) == gensym("space")){
			ptr++;
		}
	}
	long npoints = atom_getlong(ptr++);
	int i;
	for(i = 0; i < npoints; i++){
		t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
		p->pt.x = atom_getfloat(ptr++);
		p->pt.y = atom_getfloat(ptr++);
		p->label = atom_getsym(ptr++);
		p->color = (t_jrgba){atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++)};
		p->exponent = atom_getfloat(ptr++);
		p->weight = atom_getfloat(ptr++);
		p->inner_radius = atom_getfloat(ptr++);
		p->outer_radius = atom_getfloat(ptr++);
		p->locked = atom_getlong(ptr++);
		p->space = NULL;
		if(atom_gettype(ptr) == A_SYM){
			if(atom_getsym(ptr) == gensym("space")){
				t_space *s = (t_space *)sysmem_newptr(sizeof(t_space));
				s->selected = NULL;
				s->npoints = atom_getlong(ptr + 1);;
				s->points = NULL;
				s->next = NULL;
				p->space = s;
				ptr = rbfi_doFromArray(ht, n - (ptr - ar), ptr, s);
			}
		}
		p->next = space->points;
		p->prev = NULL;
		if(space->points){
			space->points->prev = p;
		}
		space->points = p;
		space->npoints = npoints;
		hashtab_store(ht, p->label, (t_object *)p);
	}
	return ptr;
}

void rbfi_clear(t_rbfi *x){
	critical_enter(x->lock);
	while(x->spaces->next){
		x->spaces = x->spaces->next;
	}
	long nkeys;
	t_symbol **keys = NULL;
	hashtab_getkeys(x->ht, &nkeys, &keys);
	int i;
	for(i = 0; i < nkeys; i++){
		t_point *p = NULL;
		hashtab_lookup(x->ht, keys[i], (t_object **)&p);
		if(p){
			if(p->space){
				sysmem_freeptr(p->space);
			}
			sysmem_freeptr(p);
		}
	}
	if(!x->spaces){
		// this shouldn't have to happen
		x->spaces = (t_space *)sysmem_newptr(sizeof(t_space));
	}
	x->spaces->points = NULL;
	x->spaces->npoints = 0;
	x->spaces->next = NULL;
	x->spaces->selected = NULL;
	x->monotonic_point_counter = 0; // not really monotonic i guess...
	hashtab_clear(x->ht);
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, NULL, l_color);
	jbox_redraw(&(x->ob));
}

void rbfi_free(t_rbfi *x){
	jbox_free(&(x->ob));
	rbfi_clear(x);
	if(x->spaces){
		sysmem_freeptr(x->spaces);
	}
	critical_free(x->lock);
}

void rbfi_assist(t_rbfi *x, void *b, long io, long index, char *s){
	switch(io){
	case 2:
		switch(index){
		case 0:
			sprintf(s, "Weights (list)");
			break;
		case 1:
			sprintf(s, "Distance from the cursor to the centers of each preset (list)");
			break;
		case 2:
			sprintf(s, "name and x,y coordinates of the preset being moved (list)");
			break;
		case 3:
			sprintf(s, "Mouse position (list)");
			break;
		case 4:
			sprintf(s, "Dump (list)");
			break;
		}
		break;
	case 1:
		switch(index){
		case 0:
			sprintf(s, "All point coordinates (list)");
			break;
		case 1:
			sprintf(s, "Selected point coordinates (list)");
			break;
		}
		break;
	}
}

double rbfi_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double rbfi_clip(double f, double min, double max){
	double ff = f;
	if(ff < min){
		ff = min;
	}
	if(ff > max){
		ff = max;
	}
	return ff;
}

void rbfi_getnumpoints(t_rbfi *x){
	t_atom a;
	critical_enter(x->lock);
	atom_setlong(&a, x->spaces->npoints);
	critical_exit(x->lock);
	outlet_anything(x->dumpOutlet, ps_numpoints, 1, &a);
}

// for pattr
t_max_err rbfi_getvalueof(t_rbfi *x, long *ac, t_atom **av){
	t_rect r;
	rbfi_getRect(x, &r);
	critical_enter(x->lock);
	*ac = rbfi_getArraySize(x);
	*av = (t_atom *)sysmem_newptr(*ac * sizeof(t_atom));
	t_space *s = x->spaces;
	while(s->next){
		s = s->next;
	}
	rbfi_toArray(s, *av);
	critical_exit(x->lock);
	return MAX_ERR_NONE;
}

t_max_err rbfi_setvalueof(t_rbfi *x, long ac, t_atom *av){
	rbfi_clear(x);
	critical_enter(x->lock);
	x->spaces = rbfi_fromArray(x->ht, ac, av);
	critical_exit(x->lock);
	return MAX_ERR_NONE;
}

#define MIN3(x,y,z)  ((y) <= (z) ?		\
		      ((x) <= (y) ? (x) : (y))	\
		      :				\
		      ((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ?		\
		      ((x) >= (y) ? (x) : (y))	\
		      :				\
		      ((x) >= (z) ? (x) : (z)))

// http://www.cs.rit.edu/~ncs/color/t_convert.html
void RGBtoHSV( double r, double g, double b, double *h, double *s, double *v ){ 
	double min, max, delta;
	min = MIN3( r, g, b );
	max = MAX3( r, g, b );
	*v = max;				// v
	delta = max - min;
	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;
}
void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v )
{
	int i;
	double f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:		// case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}

/**************************************************
Object and instance creation functions.
**************************************************/

void *rbfi_new(t_symbol *msg, int argc, t_atom *argv){
	t_rbfi *x;
    
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
		| JBOX_GROWY
		//| JBOX_GROWBOTH
		//      | JBOX_HILITE
		| JBOX_BACKGROUND
		| JBOX_DRAWBACKGROUND
		//      | JBOX_NOFLOATINSPECTOR
		//      | JBOX_MOUSEDRAGDELTA
		//      | JBOX_TEXTFIELD
		;

	if(x = (t_rbfi *)object_alloc(rbfi_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 

		x->dumpOutlet = outlet_new(x, NULL);
		x->mouseposOutlet = outlet_new(x, NULL);
		x->selectedOutlet = outlet_new(x, NULL);
		x->distanceOutlet = outlet_new(x, NULL);
		x->listOutlet = outlet_new(x, NULL);

		x->spaces = (t_space *)sysmem_newptr(sizeof(t_space));
		x->spaces->points = NULL;
		x->spaces->selected = NULL;
		x->ht = hashtab_new(0);
		hashtab_flags(x->ht, OBJ_FLAG_DATA);
		x->spaces->npoints = 0;
		x->spaces->next = NULL;

		x->monotonic_point_counter = 0;
		x->xhairs = (t_pt){-10., -10.};
		gettimeofday(&(x->lastclick), NULL);

		x->pv = NULL;

		x->compat_mode = 0;

		critical_new(&(x->lock));

		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
        
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("rbfi", (method)rbfi_new, (method)rbfi_free, sizeof(t_rbfi), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)rbfi_paint, "paint", A_CANT, 0); 
	class_addmethod(c, (method)rbfi_bang, "bang", 0);
	//class_addmethod(c, (method)rbfi_int, "int", A_LONG, 0);
	//class_addmethod(c, (method)rbfi_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)rbfi_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)rbfi_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rbfi_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)rbfi_getvalueof, "getvalueof", A_CANT, 0);
	class_addmethod(c, (method)rbfi_setvalueof, "setvalueof", A_CANT, 0);
	class_addmethod(c, (method)rbfi_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)rbfi_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)rbfi_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method)rbfi_mousemove, "mousemove", A_CANT, 0);
	class_addmethod(c, (method)rbfi_mouseleave, "mouseleave", A_CANT, 0);
	class_addmethod(c, (method)rbfi_clear, "clear", 0);
	class_addmethod(c, (method)rbfi_inner_radius, "inner_radius", A_FLOAT, 0);
	class_addmethod(c, (method)rbfi_outer_radius, "outer_radius", A_FLOAT, 0);
	class_addmethod(c, (method)rbfi_move, "move", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)rbfi_dump, "dump", 0);
	class_addmethod(c, (method)rbfi_addPoint, "add_point", A_GIMME, 0);
	class_addmethod(c, (method)rbfi_deletePoint, "delete_point", A_SYM, 0);
	class_addmethod(c, (method)rbfi_notify, "notify", A_CANT, 0); 
	class_addmethod(c, (method)rbfi_getnumpoints, "getnumpoints", 0);
	class_addmethod(c, (method)rbfi_pushPointName, "push", A_SYM, 0);
	class_addmethod(c, (method)rbfi_pop, "pop", A_DEFSYM, 0);

	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_rbfi, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointcolor", 0, t_rbfi, pointcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointcolor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_rbfi, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 

 	CLASS_ATTR_RGBA(c, "selectedcolor", 0, t_rbfi, selectedcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedcolor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedcolor", 0, "rgba", "Selected Color"); 

 	CLASS_ATTR_RGBA(c, "labelcolor", 0, t_rbfi, labelcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "labelcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "labelcolor", 0, "rgba", "Label Color"); 

 	CLASS_ATTR_RGBA(c, "xhaircolor", 0, t_rbfi, xhaircolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xhaircolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "xhaircolor", 0, "rgba", "Crosshair Color"); 
    
	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

	CLASS_ATTR_FLOAT(c, "pointdiameter", 0, t_rbfi, pointdiameter);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "pointdiameter", 0, "10.0");

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_rbfi, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_rbfi, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_rbfi, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_rbfi, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_LONG(c, "always_draw_circles", 0, t_rbfi, always_draw_circles);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "always_draw_circles", 0, "0");
	CLASS_ATTR_LONG(c, "always_draw_labels", 0, t_rbfi, always_draw_labels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "always_draw_labels", 0, "0");

	CLASS_ATTR_LONG(c, "mouse_active_beyond_rect", 0, t_rbfi, mouse_active_beyond_rect);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "mouse_active_beyond_rect", 0, "1");

	CLASS_ATTR_LONG(c, "font_size", 0, t_rbfi, font_size);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "font_size", 0, "11");

	/*
	CLASS_ATTR_SYM(c, "draw_circles", 0, t_rbfi, draw_circles);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "draw_circles", 0, "edit");
	CLASS_ATTR_SYM(c, "draw_labels", 0, t_rbfi, draw_labels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "draw_labels", 0, "edit");
	*/

	CLASS_ATTR_ATOM_VARSIZE(c, "spaces", 0, t_rbfi, spaces, nspaces, 1024);
	CLASS_ATTR_ACCESSORS(c, "spaces", rbfi_spaces_get, rbfi_spaces_set);
	CLASS_ATTR_SAVE(c, "spaces", 0);

	CLASS_ATTR_LONG(c, "outputmode", 0, t_rbfi, outputmode);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "outputmode", 0, "0");

	CLASS_ATTR_LONG(c, "rbf", 0, t_rbfi, rbf);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "rbf", 0, 0);

	CLASS_ATTR_DOUBLE(c, "line_width", 0, t_rbfi, line_width);
    	CLASS_ATTR_DEFAULTNAME_SAVE(c, "line_width", 0, "1.0");

	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 200. 200.");

	CLASS_ATTR_LONG(c, "compatmode", 0, t_rbfi, compat_mode);

	class_register(CLASS_BOX, c);
	rbfi_class = c;

	rbfi_ps_coords = gensym("coords");
	rbfi_ps_patchercoords = gensym("patchercoords");
	rbfi_ps_name = gensym("name");
	rbfi_ps_rgb = gensym("rgb");
	rbfi_ps_hsv = gensym("hsv");
	/*
	rbfi_ps_weight = gensym("weight");
	rbfi_ps_exponent = gensym("exponent");
	*/
	rbfi_ps_inner_radius = gensym("inner_radius");
	rbfi_ps_outer_radius = gensym("outer_radius");
	rbfi_ps_locked = gensym("locked");

	l_color = gensym("l_color");
	l_points = gensym("l_points");
	l_xhairs = gensym("l_xhairs");

	common_symbols_init();

#ifdef WIN32
#else
	srand(mach_absolute_time());
#endif

	ps_top = gensym("top");
	ps_done = gensym("done");
	ps_mouse_down = gensym("mousedown");
	ps_mouse_up = gensym("mouseup");
	ps_clear = gensym("clear");
	ps_space = gensym("space");
	ps_numpoints = gensym("numpoints");

	version(0);

	error("rbfi: the output of the dump message has changed, and will change again soon!");
	error("rbfi: If you need the old behavior, set the \"compatmode\" attribute to 1.");
	error("rbfi: In the next version of rbfi (early 2011), all output will be done with OSC bundles");
	
	return 0;
}

void rbfi_postPoint(t_point *p){
	if(!p){
		post("p == NULL!!");
		return;
	}
	post("point %p:", p);
	post("coords: %f %f", p->pt.x, p->pt.y);
	if(p->label){
		post("label: %s", p->label->s_name);
	}else{
		post("no label");
	}
	post("color: %f %f %f", p->color.red, p->color.blue, p->color.green);
	post("exponent: %f, weight: %f", p->exponent, p->weight);
	post("inner radius: %f, outer radius: %f", p->inner_radius, p->outer_radius);
	post("mousestate: 0x%X", p->mousestate);

	/*
	post("data count: %d", p->datac);
	post("data:");
	int i;
	for(i = 0; i < p->datac; i++){
		postatom(p->data + i);
	}
	*/
}

t_max_err rbfi_notify(t_rbfi *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	if(msg == gensym("attr_modified")){
		jbox_invalidate_layer((t_object *)x, NULL, l_color);
		jbox_invalidate_layer((t_object *)x, NULL, l_points);
		jbox_invalidate_layer((t_object *)x, NULL, l_xhairs);
		jbox_redraw(&(x->ob));
	}
	return 0;
}

t_max_err rbfi_spaces_get(t_rbfi *x, t_object *attr, long *argc, t_atom **argv){
	critical_enter(x->lock);
	t_space *s = x->spaces;
	while(s->next){
		s = s->next;
	}

	long n = rbfi_getArraySize(x);
	t_atom *ar = (t_atom *)sysmem_newptr(n * sizeof(t_atom));
	rbfi_toArray(s, ar);
	critical_exit(x->lock);
	*argc = n;
	*argv = ar;
	return MAX_ERR_NONE;
}

t_max_err rbfi_spaces_set(t_rbfi *x, t_object *attr, long argc, t_atom *argv){
	critical_enter(x->lock);
	x->spaces = rbfi_fromArray(x->ht, argc, argv);
	critical_exit(x->lock);
	return MAX_ERR_NONE;
}

void rbfi_getRect(t_rbfi *x, t_rect *rect){
	jbox_get_rect_for_view((t_object *)&(x->ob), x->pv, rect);
}
