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
  NAME: xydisplay
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

#define OUTPUT_CONCATENATE 0
#define OUTPUT_LIST 1

#define XY_INNER 1
#define XY_OUTER 2

/*
typedef struct _m22{
	double a, b, c, d;
} t_m22;

typedef struct _basis{
	t_m22 cov_mat;
	t_m22 icov_mat;
	double det;
	double mscale, gscale;
} t_basis;
*/

typedef struct _point{
	double x, y;
	t_symbol *label;
	t_jrgba color;
	long double exponent;
	long double weight;
	double inner_radius, outer_radius;
	int dragging;
	struct _point *next;
	struct _point *prev;
} t_point;

typedef struct _xydisplay{
	t_jbox ob;
	void *selectedOutlet, *listOutlet, *distanceOutlet;
	t_critical lock;
	t_jrgba bgcolor, pointcolor, bordercolor, selectedcolor, labelcolor;
	float pointdiameter;
	t_point *points;
	long npoints;
	t_point *selected;
	double xmin, xmax, ymin, ymax;
	long draw_circle, draw_label;
	long monotonic_point_counter;
	long outputmode;
	long locked;
	long rbf;
	long mouse_state;
	float line_width;
} t_xydisplay;

static t_class *xydisplay_class;

void xydisplay_paint(t_xydisplay *x, t_object *patcherview);
void xydisplay_bang(t_xydisplay *x);
void xydisplay_list(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv);
void xydisplay_anything(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv);
//void xydisplay_initCovarianceMatrix(t_basis *cm, t_m22 m, double scale);
void xydisplay_mousedown(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers);
void xydisplay_mousedrag(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers);
void xydisplay_mouseup(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers);
t_point *xydisplay_insertPoint(t_xydisplay *x, t_pt pt);
long double xydisplay_computeWeightFromDistances(double d1, double d2);
long double xydisplay_computeExponentFromDistances(double d1, double d2);
void xydisplay_exponent(t_xydisplay *x, double f);
void xydisplay_weight(t_xydisplay *x, double f);
t_point *xydisplay_selectPoint(t_xydisplay *x, t_pt pt);
void xydisplay_removePoint(t_xydisplay *x, t_point *p);
void xydisplay_outputPoints(t_xydisplay *x);
void xydisplay_outputDistance(t_xydisplay *x);
void xydisplay_rename(t_xydisplay *x, t_symbol *msg, int argc, t_atom *argv);
void xydisplay_clear(t_xydisplay *x);
void xydisplay_free(t_xydisplay *x);
void xydisplay_assist(t_xydisplay *x, void *b, long m, long a, char *s);
//double xydisplay_gaussian(t_xydisplay *x, t_point *p, double xx, double yy);
double xydisplay_scale(double f, double min_in, double max_in, double min_out, double max_out);
double xydisplay_clip(double f, double min, double max);
t_max_err xydisplay_getvalueof(t_xydisplay *x, long *ac, t_atom **av);
t_max_err xydisplay_setvalueof(t_xydisplay *x, long ac, t_atom *av);
void *xydisplay_new(t_symbol *msg, int argc, t_atom *argv);


void xydisplay_paint(t_xydisplay *x, t_object *patcherview){
	t_rect rect;

	t_jgraphics *g_onscreen = (t_jgraphics *)patcherview_get_jgraphics(patcherview);    
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	t_jsurface *s = jgraphics_image_surface_create(JGRAPHICS_FORMAT_ARGB32, rect.width, rect.height);
	t_jgraphics *g = jgraphics_create(s);

	jgraphics_set_source_jrgba(g_onscreen, &(x->bordercolor));
	jgraphics_set_line_width(g_onscreen, 1);
	jgraphics_rectangle(g_onscreen, 0., 0., rect.width, rect.height);
	jgraphics_stroke(g_onscreen);
    
	jgraphics_set_source_jrgba(g_onscreen, &(x->bgcolor));
	jgraphics_rectangle(g_onscreen, 0., 0., rect.width, rect.height);
	jgraphics_fill(g_onscreen);

	t_rect srect = rect;
	t_rect drect = rect;
	srect.x = srect.y = drect.x = drect.y = 0.;
	if(1){
		srect.width = srect.height = 128.;
	}else{
	}


	{
		int i, j;
		int w = srect.width;
		int h = srect.height;
		double sum;
		for(i = 0; i < w; i++){
			for(j = 0; j < h; j++){
				t_point *p = x->points;
				sum = 0;
				t_jrgba color = {0., 0., 0., 1.};
				double weight;
				double ii = xydisplay_scale(i, 0, srect.width, 0, drect.width);
				double jj = xydisplay_scale(j, 0, srect.width, 0, drect.width);
				while(p){
					if(ii == p->x && jj == p->y){
						//weight = 1.;
						color = p->color;
						sum = 1;
						break;
					}else{
						weight = powl(1. / sqrtl(pow(ii - p->x, 2.) + powl(jj - p->y, 2.)), p->exponent) * p->weight;
						if(isinf(weight)){
							color = p->color;
							sum = 1.;
							break;
						}
					}
					color.red += weight * p->color.red;
					color.green += weight * p->color.green;
					color.blue += weight * p->color.blue;
					sum += weight;
					p = p->next;
				}
				color.red /= sum;
				color.green /= sum;
				color.blue /= sum;
				jgraphics_image_surface_set_pixel(s, i, j, color);
				/*
				jgraphics_set_source_jrgba(g, &color);
				jgraphics_rectangle(g, i, j, 1, 1);
				jgraphics_fill(g);
				*/
			}
		}
	}

	jgraphics_destroy(g);
	jgraphics_image_surface_draw(g_onscreen, s, srect, drect);

	g = g_onscreen;
	// draw points
	{
		jgraphics_set_line_width(g, 1);
		t_point *p = x->points;
		critical_enter(x->lock);
		t_jrgba white = {1., 1., 1., 1.};
		t_jrgba gray = {.5, .5, .5, 1.};
		t_jrgba black = {0., 0., 0., 1.};
		while(p){
			if(p == x->selected){
				jgraphics_set_source_jrgba(g, &(white));
			}else{
				jgraphics_set_source_jrgba(g, &(gray));
			}
			// small center circle
			//jgraphics_ellipse(g, p->x - (x->pointdiameter / 2), p->y - (x->pointdiameter / 2), x->pointdiameter, x->pointdiameter);
			/*
			if(x->draw_circle){
				jgraphics_ellipse(g, p->x - .5, p->y - .5, 1, 1);
			}
			*/

			double inner_p = .4, outer_p = 1.25;
			t_jrgba inner_color = (t_jrgba){pow(p->color.red, inner_p), pow(p->color.green, inner_p), pow(p->color.blue, inner_p), 0.6};
			t_jrgba outer_color = (t_jrgba){pow(p->color.red, outer_p), pow(p->color.green, outer_p), pow(p->color.blue, outer_p), 0.6};


			jgraphics_set_dash(g, NULL, 0, 0);
			jgraphics_set_line_width(g, 2.);
			jgraphics_set_source_jrgba(g, &inner_color);
			jgraphics_ellipse(g, p->x - p->inner_radius, p->y - p->inner_radius, p->inner_radius * 2, p->inner_radius * 2);
			jgraphics_stroke(g);

			jgraphics_set_dash(g, (double[2]){3., 3.}, 2, 0);
			jgraphics_set_line_width(g, 1.);
			jgraphics_set_source_jrgba(g, &inner_color);
			jgraphics_ellipse(g, p->x - p->outer_radius, p->y - p->outer_radius, p->outer_radius * 2, p->outer_radius * 2);
			jgraphics_stroke(g);

			if(x->draw_label && p->label){
				jgraphics_set_source_jrgba(g, &(white));
				jgraphics_set_font_size(g, 10);
				double w, h;
				jgraphics_text_measure(g, p->label->s_name, &w, &h);
				jgraphics_move_to(g, p->x - (w / 2.), p->y + (w / 2.));
				jgraphics_show_text(g, p->label->s_name);
			}
			jgraphics_stroke(g);

			p = p->next;
		}
		critical_exit(x->lock);
	}

	{


		/*
		double sums[w * h];
		double weights[w * h * x->npoints];
		double *wp = weights;
		memset(sums, 0, (w * h) * sizeof(double));
		memset(weights, 0, (w * h * x->npoints) * sizeof(double));
		for(i = 0; i < w; i++){
			for(j = 0; j < h; j++){
				t_point *p = x->points;
				while(p){
					if(i == p->x && j == p->y){
						*wp = 1.;
					}else{
						*wp = pow(1. / sqrt(pow(i - p->x, 2.) + pow(j - p->y, 2.)), p->exponent);
					}
					//printf("%ld: %d %d %f %f %f %f %f %f %f\n", wp, i, j, p->x, p->y, pow(i - p->x, 2.), pow(j - p->y, 2.), sqrt(pow(i - p->x, 2.) + pow(j - p->y, 2.)), 1. / sqrt(pow(i - p->x, 2.) + pow(j - p->y, 2.)), pow(1. / sqrt(pow(i - p->x, 2.) + pow(j - p->y, 2.)), p->exponent));
					sums[i * h + j] += *wp;
					wp++;
					p = p->next;
				}
				if(sums[i * h + j]){
					sums[i * h + j] = .95 / sums[i * h + j];
				}else{
					sums[i * h + j] = 1.;
				}
			}
		}

		wp = weights;
		for(i = 0; i < w; i++){
			for(j = 0; j < h; j++){
				t_jrgba color = {1., 1., 1., 1.};
				t_point *p = x->points;
				while(p){
					color.red += (p->color.red * *wp);
					color.green += (p->color.green * *wp);
					color.blue += (p->color.blue * *wp);
					wp++;
					p = p->next;
					//color.alpha += *wp++;
				}
				color.red /= sums[i * h + j];
				color.green /= sums[i * h + j];
				color.blue /= sums[i * h + j];
				color.alpha /= sums[i * h + j];
				//printf("%ld: %d %d, alpha = %f\n", i * h + (j * x->npoints) + k, i, j, color.alpha);
				jgraphics_set_source_jrgba(g, &color);
				jgraphics_rectangle(g, i, j, 1, 1);
				jgraphics_fill(g);
			}
		}
		*/
	}

	/*
	{
		int i, j;
		critical_enter(x->lock);
		int m = 20;
		int n = 20;
		double gms[m * (n + 1)];
		double sgms[m * (n + 1)];
		double aspect = (rect.width / rect.height);
		double dx = 2.0 * aspect / (m - 1);
		double dy = 2.0 / (n - 1);
		double xx, yy;
		//for(i = 0; i < rect.width; i++){
		//for(j = 0; j < rect.height; j++){
		for(i = 0, xx = -aspect; i < m; i++, xx += dx){
			for(j = 0, yy = -1; j < n + 1; j++, yy += dy){
				double sum = 0;
				t_point *p = x->points;
				//double xx = xydisplay_scale(i, 0, rect.width, -1., 1.);
				//double yy = xydisplay_scale(j, rect.height, 0, -1., 1.);
				while(p){
					double g = xydisplay_gaussian(x, p, xx, yy);
					sum += g;
					p = p->next;
				}
				if(sum == 0.){
					sgms[i * n + j] = 1.;
				}else{
					sgms[i * n + j] = 0.95 / sum;
				}
			}
		}
		t_point *p = x->points;
		while(p){
			double red = p->color.red;
			double green = p->color.green;
			double blue = p->color.blue;
			//for(i = 0; i < rect.width; i++){
			//for(j = 0; j < rect.height; j++){
			for(i = 0, xx = -aspect; i < m; i++, xx += dx){
				for(j = 0, yy = -1; j < n + 1; j++, yy += dy){
					//double xx = xydisplay_scale(i, 0, rect.width, -1., 1.);
					//double yy = xydisplay_scale(j, rect.height, 0, -1., 1.);
					double gauss = xydisplay_gaussian(x, p, xx, yy);
					gms[i * n + j] = gauss * 0.75;
				}
			}
			//for(i = 0; i < rect.width; i++){
			//for(j = 0; j < rect.height; j++){
			for(i = 0, xx = -aspect; i < m; i++, xx += dx){
				//yy = -1. + dy;
				for(j = 0, yy = 1.; j < n; j++, yy -= dy){
					t_jrgba color = (t_jrgba){red, green, blue, gms[i * n + j] * sgms[i * n + j]};
					jgraphics_set_source_jrgba(g, &color);
					jgraphics_rectangle(g, xydisplay_scale(xx, -1., 1., 0., rect.width), xydisplay_scale(yy, 1., -1., 0., rect.height), xydisplay_scale(dx, 0., aspect * 2., 0., rect.width), xydisplay_scale(dy, 0., 2., 0., rect.height));
					post("alpha = %f, %f %f %d %d, %f %f, %f %f", gms[i * n + j] * sgms[i * n + j], xydisplay_scale(xx, -aspect, aspect, 0., rect.width), xydisplay_scale(yy, 1., -1., 0., rect.height), m, n, dx, dy, xydisplay_scale(dx, 0., aspect * 2, 0., rect.width), (aspect * (m - 1)) / 2.);
					jgraphics_fill(g);
				}

			}
			p = p->next;
		}
		critical_exit(x->lock);
	}
	*/
}

void xydisplay_bang(t_xydisplay *x){
	xydisplay_outputPoints(x);
	xydisplay_outputDistance(x);
}

void xydisplay_list(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc % 2){
		object_error((t_object *)x, "list must be a multiple of 2 elements long");
		return;
	}
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	x->npoints = argc / 2;

	int i;

	critical_enter(x->lock);
	for(i = 0; i < argc / 2; i++){
		t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
		p->next = x->points;
		x->points = p;
		p->prev = NULL;

		p->x = atom_getfloat(argv + (i * 2));
		p->x = xydisplay_scale(p->x, x->xmin, x->xmax, 0, r.width);
		p->y = atom_getfloat(argv + ((i * 2) + 1));
		p->y = xydisplay_scale(p->y, x->ymin, x->ymax, r.height, 0);
		char buf[16];
		sprintf(buf, "%ld", x->monotonic_point_counter++);
		p->label = gensym(buf);
		p->color = x->labelcolor;
	}
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xydisplay_anything(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv){

}

/*
void xydisplay_initCovarianceMatrix(t_basis *cm, t_m22 m, double scale){
	cm->det = m.a * m.d - m.b * m.c;
	if(cm->det != 0.0){
		cm->cov_mat = m;
		double one_over_det = 1. / cm->det;
		cm->icov_mat.a = one_over_det * m.d;
		cm->icov_mat.b = one_over_det * -m.b;
		cm->icov_mat.c = one_over_det * -m.c;
		cm->icov_mat.d = one_over_det * m.a;
		cm->mscale = scale;
		cm->gscale = scale / (2.0 * M_PI * sqrt(cm->det));
	}
}
*/

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void xydisplay_mousedown(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers){ 
 	//post("0x%X", modifiers); 
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	x->selected = NULL;
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		{
			x->selected = xydisplay_selectPoint(x, pt);
			if(!(x->selected) && !(x->locked)){
				x->selected = xydisplay_insertPoint(x, pt);
			}
		}
		break;
	case 0x12:
		// shift.  
		{
			t_point *p;
			if(p = xydisplay_selectPoint(x, pt)){
				xydisplay_removePoint(x, p);
				critical_enter(x->lock);
				x->selected = NULL;
				critical_exit(x->lock);
			}
		}
		break;
	case 0x13:
		{
			t_point *p = x->points;
			int i = 0;
			while(p){
				post("inner: %f %f %f", pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.), pow(p->inner_radius, 2.), fabs(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.) - pow(p->inner_radius, 2.)));
				post("outer: %f %f %f", pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.), pow(p->outer_radius, 2.), fabs(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.) - pow(p->outer_radius, 2.)));
				i++;
				if(fabs(sqrt(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.)) - p->inner_radius) <= 5){
					p->dragging = XY_INNER;
					break;
				}else if(fabs(sqrt(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.)) - p->outer_radius) <= 5.){
					p->dragging = XY_OUTER;
					break;
				}else{
					p->dragging = 0;
				}


				p = p->next;
			}
		}
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	xydisplay_outputPoints(x);
	xydisplay_outputDistance(x);
}

void xydisplay_mousedrag(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x10:
		if(x->selected){
			critical_enter(x->lock);
			x->selected->x = pt.x;
			x->selected->y = pt.y;
			x->mouse_state = 1;
			critical_exit(x->lock);
		}
		break;
	case 0x12:
		break;
	case 0x13:
		{
			t_point *p = x->points;
			while(p){
				if(p->dragging == XY_INNER){
					p->inner_radius = sqrt(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.));
					p->exponent = xydisplay_computeExponentFromDistances(p->inner_radius, p->outer_radius);
					p->weight = xydisplay_computeWeightFromDistances(p->inner_radius, p->outer_radius);
					break;
				}else if(p->dragging == XY_OUTER){
					p->outer_radius = sqrt(pow(pt.x - p->x, 2.) + pow(pt.y - p->y, 2.));
					p->exponent = xydisplay_computeExponentFromDistances(p->inner_radius, p->outer_radius);
					p->weight = xydisplay_computeWeightFromDistances(p->inner_radius, p->outer_radius);
					break;
				}
				p = p->next;
			}
		}
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	xydisplay_outputPoints(x);
	xydisplay_outputDistance(x);
}

void xydisplay_mouseup(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers){
	if(x->mouse_state){
		x->mouse_state = 0;
		jbox_redraw(&(x->ob));
	}
}

t_point *xydisplay_insertPoint(t_xydisplay *x, t_pt pt){
	critical_enter(x->lock);
	t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
	p->x = pt.x;
	p->y = pt.y;
	p->next = x->points;
	p->prev = NULL;
	if(x->points){
		x->points->prev = p;
	}
	x->points = p;
	x->npoints++;
	char buf[16];
	sprintf(buf, "%ld", x->monotonic_point_counter++);
	p->label = gensym(buf);
	p->color = (t_jrgba){(double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX, 1.};
	//xydisplay_initCovarianceMatrix(&(p->cov_mat), (t_m22){.125, 0., 0., .125}, .125);
	//p->exponent = 2.;
	//p->weight = 1.;
	p->inner_radius = 10.;
	p->outer_radius = 100.;
	p->exponent = xydisplay_computeExponentFromDistances(p->inner_radius, p->outer_radius);
	p->weight = xydisplay_computeWeightFromDistances(p->inner_radius, p->outer_radius);
	critical_exit(x->lock);
	return p;
}

#define THRESH .9
long double xydisplay_computeWeightFromDistances(double d1, double d2){
	double dd1 = d1;
	double dd2 = d2;
	if((dd1 / dd2) <= 1 && (dd1 / dd2) >= THRESH){
		post("%s: a", __PRETTY_FUNCTION__);
		dd1 = THRESH * dd2;
	}else if((dd1 / dd2) >= 1 && (dd1 / dd2) <= (1. / THRESH)){
		post("%s: b", __PRETTY_FUNCTION__);
		dd1 = dd2 / THRESH;
	}

	long double w = expl(-((log(2.) + log(5.)) * (log(dd1) + log(dd2))) / (log(dd1) - log(dd2)));
	post("w = %f", (double)w);
	return w;
}

long double xydisplay_computeExponentFromDistances(double d1, double d2){
	double dd1 = d1;
	double dd2 = d2;
	if((dd1 / dd2) <= 1 && (dd1 / dd2) >= THRESH){
		dd1 = THRESH * dd2;
	}else if((dd1 / dd2) >= 1 && (dd1 / dd2) <= (1. / THRESH)){
		dd1 = dd2 / THRESH;
	}

	long double d = (2 * (log(2.) + log(5.))) / (-log(dd1) + log(dd2));
	post("p = %f", (double)d);
	return d;
}

void xydisplay_exponent(t_xydisplay *x, double f){
	t_point *p = x->points;
	while(p){
		p->exponent = f;
		p = p->next;
	}
	/*
	if(x->selected){
		x->selected->exponent = f;
	}
	*/
	jbox_redraw(&(x->ob));
}

void xydisplay_weight(t_xydisplay *x, double f){
	if(x->selected){
		x->selected->weight = f;
	}
	jbox_redraw(&(x->ob));
}

t_point *xydisplay_selectPoint(t_xydisplay *x, t_pt pt){
	critical_enter(x->lock);
	t_point *p = x->points;
	while(p){
		if(pt.x >= (p->x - x->pointdiameter) && pt.x <= (p->x + x->pointdiameter) &&
		   pt.y >= (p->y - x->pointdiameter) && pt.y <= (p->y + x->pointdiameter)){
			goto out;
		}
		p = p->next;
	}
 out:
	critical_exit(x->lock);
	return p;
}

void xydisplay_removePoint(t_xydisplay *x, t_point *p){
	critical_enter(x->lock);
	if(p->prev){
		p->prev->next = p->next;
	}else{
		x->points = p->next;
	}
	if(p->next){
		p->next->prev = p->prev;
	}
	sysmem_freeptr((void *)p);
	x->npoints--;
	critical_exit(x->lock);
}

void xydisplay_outputPoints(t_xydisplay *x){
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	if(x->selected){
		t_atom out[2];
		critical_enter(x->lock);
		atom_setfloat(out, xydisplay_scale(x->selected->x, 0, r.width, x->xmin, x->xmax));
		atom_setfloat(out + 1, xydisplay_scale(x->selected->y, r.height, 0, x->ymin, x->ymax));
		critical_exit(x->lock);
		outlet_list(x->selectedOutlet, NULL, 2, out);
	}
	t_point *p = x->points;
	switch(x->outputmode){
	case OUTPUT_CONCATENATE:
		{
			long outlen = x->npoints * 2;
			t_atom out[outlen];
			t_atom *outptr = out + outlen - 1;
			critical_enter(x->lock);
			// since the points are pushed onto the front of the linked list, we should work backwards
			while(p){
				atom_setfloat(outptr--, xydisplay_scale(p->y, r.height, 0, x->ymin, x->ymax));
				atom_setfloat(outptr--, xydisplay_scale(p->x, 0, r.width, x->xmin, x->xmax));
				p = p->next;
			}
			critical_exit(x->lock);
			outlet_list(x->listOutlet, NULL, outlen, out);
		}
		break;
	case OUTPUT_LIST:
		{
			// we could go through and call the outlet for each point, but we would have to exit the critical
			// section and then re-enter at the beginning of the while loop.  since the outlet function can 
			// take a long time to return, we should make a copy of everything quickly and safely, and then 
			// output our copy.
			int npoints = x->npoints;
			t_atom out[3 * npoints];
			t_atom *ptr = out + (3 * npoints) - 1;
			critical_enter(x->lock);
			while(p){
				atom_setfloat(ptr--, xydisplay_scale(p->y, r.height, 0, x->ymin, x->ymax));
				atom_setfloat(ptr--, xydisplay_scale(p->x, 0, r.width, x->xmin, x->xmax));
				atom_setsym(ptr--, p->label);
				p = p->next;
			}
			critical_exit(x->lock);
			int i;
			outlet_anything(x->listOutlet, gensym("clear"), 0, NULL);
			ptr = out;
			for(i = 0; i < npoints; i++){
				t_symbol *sym = atom_getsym(ptr++);
				outlet_anything(x->listOutlet, sym, 2, ptr);
				ptr += 2;
			}
		}
		break;
	}
}

void xydisplay_outputDistance(t_xydisplay *x){
	if(!x->selected){
		return;
	}
	critical_enter(x->lock);
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	int npoints = x->npoints;
	t_atom out[npoints * 2];
	t_atom *ptr = out + (npoints * 2) - 1;
	t_point *p = x->points;
	double selx = xydisplay_scale(x->selected->x, 0, r.width, 0., 1.);
	double sely = xydisplay_scale(x->selected->y, r.height, 0, 0., 1.);

	while(p){
		atom_setfloat(ptr--, sqrt(pow(selx - xydisplay_scale(p->x, 0., r.width, 0., 1.), 2.) + pow(sely - xydisplay_scale(p->y, r.height, 0., 0., 1.), 2.)));
		atom_setsym(ptr--, p->label);
		p = p->next;
	}
	critical_exit(x->lock);
	outlet_list(x->distanceOutlet, NULL, npoints * 2, out);
}

void xydisplay_clear(t_xydisplay *x){
	critical_enter(x->lock);
	t_point *p = x->points;
	t_point *next;
	while(p){
		next = p->next;
		sysmem_freeptr(p);
		x->npoints--;
		p = next;
	}
	x->points = NULL;
	x->selected = NULL;
	x->monotonic_point_counter = 0; // not really monotonic i guess...
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xydisplay_rename(t_xydisplay *x, t_symbol *msg, int argc, t_atom *argv){
	critical_enter(x->lock);
	if(argc == 1){
		if(x->selected){
			x->selected->label = atom_getsym(argv);
		}
	}else{
		t_point *p = x->points;
		t_symbol *oldname = atom_getsym(argv);
		while(p){
			if(p->label = oldname){
				p->label = atom_getsym(argv + 1);
				break;
			}
			p = p->next;
		}
	}
	critical_exit(x->lock);
	jbox_redraw(&(x->ob));
}

void xydisplay_free(t_xydisplay *x){
	jbox_free(&(x->ob));
	t_point *p = x->points;
	t_point *next = NULL;
	while(p){
		next = p->next;
		sysmem_freeptr(p);
		p = next;
	}
	xydisplay_clear(x);
	critical_free(x->lock);
}

void xydisplay_assist(t_xydisplay *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "Point coordinates (list)");
			break;
		}
		break;
	case 2:
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

/*
double xydisplay_gaussian(t_xydisplay *x, t_point *p, double xx, double yy){
	t_basis cm = p->cov_mat;
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	xx -= xydisplay_scale(p->x, 0., r.width, -1., 1.);
	yy -= xydisplay_scale(p->y, 0., r.height, -1., 1.);
	double g = cm.gscale * exp(-0.5 * (xx * (cm.icov_mat.a * xx + cm.icov_mat.c * yy) + yy * (cm.icov_mat.b * xx + cm.icov_mat.d * yy)));
	//post("cm.gscale = %f x = %f y = %f g = %f\n", cm.gscale, xx, yy, g);
	return g;
}
*/

double xydisplay_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double xydisplay_clip(double f, double min, double max){
	double ff = f;
	if(ff < min){
		ff = min;
	}
	if(ff > max){
		ff = max;
	}
	return ff;
}

// for pattr
t_max_err xydisplay_getvalueof(t_xydisplay *x, long *ac, t_atom **av){
	//post("getvalueof");
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	int npoints = x->npoints;
	if(*ac && *av){
	}else{
		*ac = npoints * 2;
		*av = (t_atom *)sysmem_newptr(*ac * sizeof(t_atom));
	}
	critical_enter(x->lock);
	t_point *p = x->points;
	t_atom *outptr = *av;
	while(p){
		atom_setfloat(outptr++, xydisplay_scale(p->x, 0, r.width, x->xmin, x->xmax));
		atom_setfloat(outptr++, xydisplay_scale(p->y, r.height, 0, x->ymin, x->ymax));
		p = p->next;
	}
	critical_exit(x->lock);

	return MAX_ERR_NONE;
}

t_max_err xydisplay_setvalueof(t_xydisplay *x, long ac, t_atom *av){
	xydisplay_list(x, NULL, ac, av);
	return MAX_ERR_NONE;
}

/**************************************************
Object and instance creation functions.
**************************************************/

void *xydisplay_new(t_symbol *msg, int argc, t_atom *argv){
	t_xydisplay *x;
    
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

	if(x = (t_xydisplay *)object_alloc(xydisplay_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 

		x->selectedOutlet = outlet_new(x, NULL);
		x->distanceOutlet = outlet_new(x, NULL);
		x->listOutlet = outlet_new(x, NULL);
		x->npoints = 0;
		x->points = NULL;
		x->monotonic_point_counter = 0;
		x->mouse_state = 0;

		critical_new(&(x->lock));
        
		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
        
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("xydisplay", (method)xydisplay_new, (method)xydisplay_free, sizeof(t_xydisplay), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)xydisplay_paint, "paint", A_CANT, 0); 
	class_addmethod(c, (method)xydisplay_bang, "bang", 0);
	//class_addmethod(c, (method)xydisplay_int, "int", A_LONG, 0);
	//class_addmethod(c, (method)xydisplay_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)xydisplay_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)xydisplay_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_getvalueof, "getvalueof", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_setvalueof, "setvalueof", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method)xydisplay_clear, "clear", 0);
	class_addmethod(c, (method)xydisplay_rename, "rename", A_GIMME, 0);
	class_addmethod(c, (method)xydisplay_exponent, "exponent", A_FLOAT, 0);
	class_addmethod(c, (method)xydisplay_weight, "weight", A_FLOAT, 0);
    
	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_xydisplay, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointcolor", 0, t_xydisplay, pointcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointcolor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_xydisplay, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 

 	CLASS_ATTR_RGBA(c, "selectedcolor", 0, t_xydisplay, selectedcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedcolor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedcolor", 0, "rgba", "Selected Color"); 

 	CLASS_ATTR_RGBA(c, "labelcolor", 0, t_xydisplay, labelcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "labelcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "labelcolor", 0, "rgba", "Label Color"); 
    
	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

	CLASS_ATTR_FLOAT(c, "pointdiameter", 0, t_xydisplay, pointdiameter);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "pointdiameter", 0, "10.0");

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_xydisplay, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "xmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_xydisplay, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_xydisplay, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ymin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_xydisplay, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "ymax", 0, "1.0");

	CLASS_ATTR_LONG(c, "draw_circle", 0, t_xydisplay, draw_circle);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "draw_circle", 0, "1");
	CLASS_ATTR_LONG(c, "draw_label", 0, t_xydisplay, draw_label);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "draw_label", 0, "1");

	CLASS_ATTR_LONG(c, "outputmode", 0, t_xydisplay, outputmode);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "outputmode", 0, "0");

	CLASS_ATTR_LONG(c, "locked", 0, t_xydisplay, locked);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "locked", 0, "0");

	CLASS_ATTR_LONG(c, "rbf", 0, t_xydisplay, rbf);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "rbf", 0, 0);

	CLASS_ATTR_DOUBLE(c, "line_width", 0, t_xydisplay, line_width);
    	CLASS_ATTR_DEFAULTNAME_SAVE(c, "line_width", 0, "1.0");

	class_register(CLASS_BOX, c);
	xydisplay_class = c;

	common_symbols_init();

	version(0);

	return 0;
}
