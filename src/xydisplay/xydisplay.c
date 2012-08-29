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
  COPYRIGHT_YEARS: 2010
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 0.0.1: labels now work
  VERSION 0.0.2: resize in any direction
  VERSION 0.0.3: lock flag added
  VERSION 0.0.4: pattr compatibility
  VERSION 0.0.5: pattr bugfix and lock now just prevents the creation of new points
  VERSION 0.0.6: selected outlet now has proper id number
  VERSION 0.0.7: nearest point is now chosen when locked
  VERSION 0.0.8: outputs points after a pattr update
  VERSION 0.1: paths to connect points, renumber points, interp between points, save with patcher
  VERSION 0.1.1: points are now in the right order saved and reopened
  VERSION 0.1.2: fixed capslock bug
  VERSION 0.1.3: new points are now numbered to fill in the available slots.
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

#ifndef DBL_MAX
#define DBL_MAX 999999999.
#endif

#ifdef WIN32
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#else
#include <mach/mach_time.h>
#endif

/*
  list
  set
  movePoint
  setPoint
  moveSelected
  setSelected
  select
*/

#define HASH_LEN 65536
typedef struct _point{
	t_pt pt;
	unsigned long id;
	struct _point *next;
	struct _point *prev;
} t_point;

typedef struct _xy{
	t_jbox ob;
	t_object *patcherview;
	void *selectedOutlet, *listOutlet, *interpOutlet, *dumpOutlet;//, *distanceOutlet;
	t_critical lock;
	t_jrgba bgcolor, pointcolor, bordercolor, selectedcolor, labelcolor;
	float pointdiameter;
	t_point *points;
	t_point **point_hash;
	long point_hash_len;
	long npoints;
	t_point *selected;
	double xmin, xmax, ymin, ymax;
	long drawlabels;
	long connect_points;
	long monotonic_point_counter;
	long locked;
	t_pt interp;
	int draw_stupid_little_point;
	int highlight_line;
	char *slots;
	int nslots;
} t_xy;

static t_class *xy_class;

void xy_paint(t_xy *x, t_object *patcherview);
void xy_bang(t_xy *x);
void xy_float(t_xy *x, double f);
void xy_renumber(t_xy *x);
void xy_set(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_list(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_processList(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_movePoint(t_xy *x, long n, double xx, double yy);
void xy_setPoint(t_xy *x, long n, double xx, double yy);
void xy_doMovePoint(t_xy *x, t_point *p, double xx, double yy);
void xy_moveSelected(t_xy *x, double xx, double yy);
void xy_setSelected(t_xy *x, double xx, double yy);
int xy_get_slot(t_xy *x);
t_point *xy_findPointByPosition(t_xy *x, long n);
void xy_select(t_xy *x, long n);
void xy_setSelection(t_xy *x, long n);
void xy_unselect(t_xy *x);
void xy_findNearest(t_xy *x, double xx, double yy);
void xy_findNearestX(t_xy *x, double xx);
void xy_findNearestY(t_xy *x, double yy);
void xy_anything(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_mousedown(t_xy *x, t_object *patcherview, t_pt pt, long modifiers);
void xy_mousedrag(t_xy *x, t_object *patcherview, t_pt pt, long modifiers);
void xy_mouseup(t_xy *x, t_object *patcherview, t_pt pt, long modifiers);
void xy_mousemove(t_xy *x, t_object *patcherview, t_pt pt, long modifiers);
void xy_mouseleave(t_xy *x, t_object *patcherview, t_pt pt, long modifiers);
t_point *xy_newPoint(t_xy *x);
t_point *xy_selectPoint(t_xy *x, t_pt pt);
void xy_addPoint(t_xy *x, t_point *p);
void xy_removePoint(t_xy *x, t_point *p);
void xy_outputPoints(t_xy *x);
//void xy_outputDistance(t_xy *x);
void xy_outputSelected(t_xy *x);
void xy_outputAll(t_xy *x);
void xy_dump(t_xy *x);
void xy_clear(t_xy *x);
void xy_free(t_xy *x);
void xy_assist(t_xy *x, void *b, long m, long a, char *s);
double xy_scale(double f, double min_in, double max_in, double min_out, double max_out);
double xy_clip(double f, double min, double max);
double xy_edist(t_pt p1, t_pt p2);
t_max_err xy_getvalueof(t_xy *x, long *ac, t_atom **av);
t_max_err xy_setvalueof(t_xy *x, long ac, t_atom *av);
void *xy_new(t_symbol *msg, int argc, t_atom *argv);
void xy_postPoint(t_point *p);
t_max_err xy_notify(t_xy *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_max_err xy_points_get(t_xy *x, t_object *attr, long *argc, t_atom **argv);
t_max_err xy_points_set(t_xy *x, t_object *attr, long argc, t_atom *argv);

t_symbol *l_points, *l_stupid_little_point, *l_background;



void xy_paint(t_xy *x, t_object *patcherview){
	x->patcherview = patcherview;
	t_rect rect;

	t_jgraphics *gg = (t_jgraphics *)patcherview_get_jgraphics(patcherview);    
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_background, rect.width, rect.height);
	if(g){
		jgraphics_set_source_jrgba(g, &(x->bordercolor));
		jgraphics_set_line_width(g, 1);
		jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
		jgraphics_stroke(g);
    
		jgraphics_set_source_jrgba(g, &(x->bgcolor));
		jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
		jgraphics_fill(g);
		jbox_end_layer((t_object *)x, patcherview, l_background);
	}
	jbox_paint_layer((t_object *)x, patcherview, l_background, 0, 0);
	// draw points
	g = jbox_start_layer((t_object *)x, patcherview, l_points, rect.width, rect.height);
	if(g){
		jgraphics_set_line_width(g, 1);
		// this should be 
		// critical_enter(x->lock);
		// make copy
		// critical_exit(x->lock);
		t_point *p = x->points;
		t_pt prev_pt = (t_pt){0., 0.};
		if(p){
			prev_pt = (t_pt){xy_scale(p->pt.x, x->xmin, x->xmax, 0, rect.width), xy_scale(p->pt.y, x->ymin, x->ymax, rect.height, 0)};
		}
		int i = 0;
		while(p){
			t_jrgba color;
			if(p == x->selected){
				color = x->selectedcolor;
			}else{
				color = x->pointcolor;
			}
			jgraphics_set_source_jrgba(g, &(color));
			t_pt pt = p->pt;
			pt.x = xy_scale(pt.x, x->xmin, x->xmax, 0, rect.width);
			pt.y = xy_scale(pt.y, x->ymin, x->ymax, rect.height, 0);
			jgraphics_ellipse(g, pt.x - x->pointdiameter / 2., pt.y - x->pointdiameter / 2., x->pointdiameter, x->pointdiameter);
			jgraphics_fill(g);
			if(x->connect_points){
				if(x->highlight_line == i){
					color = x->selectedcolor;
				}else{
					color = x->pointcolor;
				}
				jgraphics_set_source_jrgba(g, &(color));
				jgraphics_move_to(g, prev_pt.x, prev_pt.y);
				jgraphics_line_to(g, pt.x, pt.y);
				jgraphics_stroke(g);
				prev_pt = pt;
			}

			// draw labels
			if(x->drawlabels){
				jgraphics_select_font_face(g, "Arial", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
				jgraphics_set_font_size(g, 12);  
				if(p == x->selected){
					color = x->selectedcolor;
				}else{
					color = x->pointcolor;
				}
				jgraphics_set_source_jrgba(g, &(color));
				double w, h;                              
				char buf[32];
				sprintf(buf, "%lu", p->id);                                      
				jgraphics_text_measure(g, buf, &w, &h);
				jgraphics_move_to(g, pt.x - (w / 2.), pt.y - (h / 2));                     
				jgraphics_show_text(g, buf);
			}
			
			p = p->next;
			i++;
		}
		jbox_end_layer((t_object *)x, patcherview, l_points);
	}
	jbox_paint_layer((t_object *)x, patcherview, l_points, 0, 0);
	if(x->draw_stupid_little_point){
		g = jbox_start_layer((t_object *)x, patcherview, l_stupid_little_point, 4, 4);
		if(g){
			jgraphics_set_source_jrgba(g, &(x->selectedcolor));
			jgraphics_ellipse(g, 0, 0, 4, 4);
			jgraphics_fill(g);
			jbox_end_layer((t_object *)x, patcherview, l_stupid_little_point);
		}
		jbox_paint_layer((t_object *)x, patcherview, l_stupid_little_point, x->interp.x - 2, x->interp.y - 2);
	}
}

void xy_bang(t_xy *x){
	// output everything
	xy_outputAll(x);
}

void xy_float(t_xy *x, double f){
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	t_point *p = x->points;
	x->interp = (t_pt){-1., -1};
	int i = 0;
	while(p){
		if(!p->next){
			break;
		}
		t_point *next = p->next;
		if((p->id <= f && f <= next->id) || (p->id >= f && f >= next->id)){
			if(p->id == f){
				x->interp.x = xy_scale(p->pt.x, x->xmin, x->xmax, 0, r.width);
				x->interp.y = xy_scale(p->pt.y, x->ymin, x->ymax, r.height, 0);
			}else if(next->id == f){
				x->interp.x = xy_scale(next->pt.x, x->xmin, x->xmax, 0, r.width);
				x->interp.y = xy_scale(next->pt.y, x->ymin, x->ymax, r.height, 0);
			}else{
				double x1, x2, y1, y2;
				if(p->id < next->id){
					x1 = xy_scale(p->pt.x, x->xmin, x->xmax, 0, r.width);
					x2 = xy_scale(next->pt.x, x->xmin, x->xmax, 0, r.width);
					y1 = xy_scale(p->pt.y, x->ymin, x->ymax, r.height, 0);
					y2 = xy_scale(next->pt.y, x->ymin, x->ymax, r.height, 0);
				}else{
					x2 = xy_scale(p->pt.x, x->xmin, x->xmax, 0, r.width);
					x1 = xy_scale(next->pt.x, x->xmin, x->xmax, 0, r.width);
					y2 = xy_scale(p->pt.y, x->ymin, x->ymax, r.height, 0);
					y1 = xy_scale(next->pt.y, x->ymin, x->ymax, r.height, 0);
				}
				x->interp.x = ((x2 - x1) * (f - floor(f))) + x1;
				x->interp.y = (((y2 - y1) / (x2 - x1)) * (x->interp.x - x1)) + y1;
				x->highlight_line = i + 1;
			}
			t_atom out[2];
			atom_setfloat(out, xy_scale(x->interp.x, 0, r.width, x->xmin, x->xmax));
			atom_setfloat(out + 1, xy_scale(x->interp.y, r.width, 0, x->ymin, x->ymax));
			outlet_list(x->interpOutlet, NULL, 2, out);
			if(x->draw_stupid_little_point){
				jbox_invalidate_layer((t_object *)x, x->patcherview, l_stupid_little_point);
				jbox_redraw(&(x->ob));
			}
			break;
		}
		p = p->next;
		i++;
	}
}

void xy_renumber(t_xy *x){
	//int n = x->npoints - 1;
	t_point *p = x->points;
	//x->monotonic_point_counter = n;
	int i = 0;
	memset(x->slots, '\0', x->nslots);
	while(p){
		p->id = i;
		x->slots[i] = 1;
		i++;
		p = p->next;
	}
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_set(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
	xy_processList(x, msg, argc, argv);
	x->selected = NULL;
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_list(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
	xy_processList(x, msg, argc, argv);
	x->selected = NULL;
	xy_outputPoints(x);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_processList(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc % 2){
		object_error((t_object *)x, "list must contain a multiple of 2 elements");
		return;
	}
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	int i;
	for(i = 0; i < argc / 2; i++){
		t_point *p = xy_newPoint(x);
		if(!p){
			return;
		}
		p->pt.x = atom_getfloat(argv + (i * 2));
		p->pt.y = atom_getfloat(argv + (i * 2 + 1));
		xy_addPoint(x, p);
	}
}

void xy_movePoint(t_xy *x, long n, double xx, double yy){
	critical_enter(x->lock);
	//t_point *p = xy_findPointByPosition(x, n);
	if(n > x->point_hash_len){
		object_error((t_object *)x, "no point %d", n);
		return;
	}
	t_point *p = x->point_hash[n];
	if(!p){
		object_error((t_object *)p, "couldn't find point");
		return;
	}
	x->selected = p;
	xy_doMovePoint(x, p, xx, yy);
	critical_exit(x->lock);
	xy_outputAll(x);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_setPoint(t_xy *x, long n, double xx, double yy){
	critical_enter(x->lock);
	//t_point *p = xy_findPointByPosition(x, n);
	if(n > x->point_hash_len){
		object_error((t_object *)x, "no point %d", n);
		return;
	}
	t_point *p = x->point_hash[n];
	if(!p){
		object_error((t_object *)p, "couldn't find point");
		return;
	}
	x->selected = p;
	xy_doMovePoint(x, p, xx, yy);
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_doMovePoint(t_xy *x, t_point *p, double xx, double yy){
	if(p){
		p->pt = (t_pt){xx, yy};
	}
}

void xy_moveSelected(t_xy *x, double xx, double yy){
	critical_enter(x->lock);
	if(!(x->selected)){
		object_error((t_object *)x, "no point is selected");
		return;
	}
	xy_doMovePoint(x, x->selected, xx, yy);
	critical_exit(x->lock);
	xy_outputAll(x);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_setSelected(t_xy *x, double xx, double yy){
	critical_enter(x->lock);
	if(!(x->selected)){
		object_error((t_object *)x, "no point is selected");
		return;
	}
	xy_doMovePoint(x, x->selected, xx, yy);
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

int xy_get_slot(t_xy *x){
	int i;
	for(i = 0; i < x->nslots; i++){
		if(x->slots[i] == 0){
			x->slots[i] = 1;
			return i;
		}
	}
	x->slots = (char *)sysmem_resizeptr(x->slots, x->nslots * 2);
	x->nslots *= 2;
	return (x->nslots / 2);
}

t_point *xy_findPointByPosition(t_xy *x, long n){
	if(n > x->npoints){
		object_error((t_object *)x, "can't move point number %d--there are only %d points!", x->npoints);
		return NULL;
	}
	int i = x->npoints - 1;
	critical_enter(x->lock);
	t_point *p = x->points;
	while(p){
		if(i == n){
			break;
		}
		i--;
		p = p->next;
	}
	critical_exit(x->lock);
	return p;
}

void xy_select(t_xy *x, long n){
	critical_enter(x->lock);
	//x->selected = xy_findPointByPosition(x, n);
	if(n > x->point_hash_len){
		object_error((t_object *)x, "no point %d", n);
		return;
	}
	x->selected = x->point_hash[n];
	critical_exit(x->lock);
	if(x->selected){
		xy_outputSelected(x);
	}
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_setSelection(t_xy *x, long n){
	critical_enter(x->lock);
	//x->selected = xy_findPointByPosition(x, n);
	if(n > x->point_hash_len){
		object_error((t_object *)x, "no point %d", n);
		return;
	}
	x->selected = x->point_hash[n];
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_unselect(t_xy *x){
	critical_enter(x->lock);
	x->selected = NULL;
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_findNearest(t_xy *x, double xx, double yy){
	double dist = DBL_MAX;
	t_point *nearest = NULL;
	critical_enter(x->lock);
	t_point *p = x->points;
	while(p){
		double d = xy_edist((t_pt){xx, yy}, p->pt);
		if(d < dist){
			dist = d;
			nearest = p;
		}
		p = p->next;
	}
	x->selected = nearest;
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_findNearestX(t_xy *x, double xx){
	double dist = DBL_MAX;
	t_point *nearest = NULL;
	critical_enter(x->lock);
	t_point *p = x->points;
	while(p){
		double d = fabs(xx - p->pt.x);
		if(d < dist){
			dist = d;
			nearest = p;
		}
		p = p->next;
	}
	x->selected = nearest;
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_findNearestY(t_xy *x, double yy){
	double dist = DBL_MAX;
	t_point *nearest = NULL;
	critical_enter(x->lock);
	t_point *p = x->points;
	while(p){
		double d = fabs(yy - p->pt.y);
		if(d < dist){
			dist = d;
			nearest = p;
		}
		p = p->next;
	}
	x->selected = nearest;
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_anything(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
}

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 

#define CIRCLE_HIT 3 // num pixels on either side of a circle that will register a hit

void xy_mousedown(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x110:
	case 0x10:
		//case 0x12:
		{
			if(x->locked){
				xy_findNearest(x, xy_scale(pt.x, 0., r.width, x->xmin, x->xmax), xy_scale(pt.y, r.height, 0., x->ymin, x->ymax));
			}else{
				critical_enter(x->lock);
				x->selected = xy_selectPoint(x, pt);
				critical_exit(x->lock);
				if(!(x->selected)){
					t_point *p = xy_newPoint(x);
					if(!p){
						return;
					}
					p->pt.x = xy_scale(pt.x, 0., r.width, x->xmin, x->xmax);
					p->pt.y = xy_scale(pt.y, r.height, 0., x->ymin, x->ymax);
					xy_addPoint(x, p);
					x->selected = p;
				}
			}
			if(modifiers == 0x10){
				xy_outputAll(x);
			}
		}
		break;
		//case 0x11:
		//case (0x11 | 0x12):
	case 0x112:
	case 0x12:
		{
			critical_enter(x->lock);
			x->selected = xy_selectPoint(x, pt);
			critical_exit(x->lock);
			if(x->selected){
				xy_removePoint(x, x->selected);
				x->selected = NULL;
			}
			if(modifiers == 0x11){
				xy_outputAll(x);
			}
		}
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_mousedrag(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x110:
	case 0x10:
		//case 0x12:
		critical_enter(x->lock);
		if(x->selected){
			x->selected->pt.x = xy_scale(pt.x, 0., r.width, x->xmin, x->xmax);
			x->selected->pt.y = xy_scale(pt.y, r.height, 0., x->ymin, x->ymax);
		}
		critical_exit(x->lock);
		if(modifiers == 0x10){
			xy_outputAll(x);
		}
		break;
	}
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_mouseup(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

void xy_mouseleave(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

void xy_mousemove(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

t_point *xy_newPoint(t_xy *x){
	t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
	if(p){
		p->next = NULL;
		p->prev = NULL;
		p->pt = (t_pt){0., 0.};
		p->id = xy_get_slot(x);
		return p;
	}else{
		error("xy: out of memory!  failed to allocate a new point");
		return NULL;
	}
}

t_point *xy_selectPoint(t_xy *x, t_pt pt){
	critical_enter(x->lock);
	t_point *p = x->points;
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	while(p){
		t_pt sc = p->pt;
		sc.x = xy_scale(p->pt.x, x->xmin, x->xmax, 0., r.width);
		sc.y = xy_scale(p->pt.y, x->ymin, x->ymax, r.height, 0.);
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

void xy_addPoint(t_xy *x, t_point *p){
	critical_enter(x->lock);
	if(x->points){
		x->points->prev = p;
	}
	p->prev = NULL;
	p->next = x->points;
	x->points = p;
	x->npoints++;
	if(p->id > x->point_hash_len){
		x->point_hash = realloc(x->point_hash, (x->point_hash_len * sizeof(t_point *)) * 2);
		if(!(x->point_hash)){
			object_error((t_object *)x, "out of memory!  you should probably restart max...");
			critical_exit(x->lock);
			return;
		}
	}
	x->point_hash[p->id] = p;
	critical_exit(x->lock);
}

void xy_removePoint(t_xy *x, t_point *p){
	critical_enter(x->lock);
	x->slots[p->id] = 0;
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

void xy_outputPoints(t_xy *x){
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	{
		t_point *p = x->points;
		long outlen = x->npoints * 3;
		t_atom out[outlen];
		t_atom *outptr = out + outlen - 1;
		critical_enter(x->lock);
		// since the points are pushed onto the front of the linked list, we should work backwards
		while(p){
			atom_setfloat(outptr--, p->pt.y);
			atom_setfloat(outptr--, p->pt.x);
			atom_setlong(outptr--, p->id);
			p = p->next;
		}
		critical_exit(x->lock);
		outlet_list(x->listOutlet, NULL, outlen, out);
	}
}

/*
  void xy_outputDistance(t_xy *x){
  if(!(x->selected)){
  return;
  }
  t_rect r;
  jbox_get_patching_rect(&((x->ob.b_ob)), &r);

  t_point *p = x->points;
  t_atom out[x->npoints * 2];
  t_atom *outptr = out;
  critical_enter(x->lock);
  while(p){

  p = p->next;
  }
  critical_exit(x->lock);
  outlet_list(x->distanceOutlet, NULL, x->npoints * 2, out);
  }
*/

void xy_outputSelected(t_xy *x){
	critical_enter(x->lock);
	if(!(x->selected)){
		critical_exit(x->lock);
		return;
	}
	t_atom out[3];

	atom_setlong(out, x->selected->id);
	atom_setfloat(out + 1, x->selected->pt.x);
	atom_setfloat(out + 2, x->selected->pt.y);
	critical_exit(x->lock);
	outlet_list(x->selectedOutlet, NULL, 3, out);
}

void xy_outputAll(t_xy *x){
	xy_outputSelected(x);
	//xy_outputDistance(x);
	xy_outputPoints(x);
}

void xy_dump(t_xy *x){
	//t_rect r;
	//jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	if(!(x->points)){
		goto out;
	}
	t_point *p = x->points;
	t_atom out[3];
	while(p->next){
		p = p->next;
	}
	while(p){
		atom_setlong(out, p->id);
		atom_setfloat(out + 1, p->pt.x);
		atom_setfloat(out + 2, p->pt.y);
		outlet_list(x->dumpOutlet, NULL, 3, out);
		p = p->prev;
	}
 out:
	outlet_anything(x->dumpOutlet, gensym("done"), 0, NULL);
}

void xy_clear(t_xy *x){
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
	memset(x->point_hash, '\0', x->point_hash_len * sizeof(t_point *));
	x->selected = NULL;
	//x->monotonic_point_counter = 0; // not really monotonic i guess...
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
	jbox_redraw(&(x->ob));
}

void xy_free(t_xy *x){
	jbox_free(&(x->ob));
	t_point *p = x->points;
	t_point *next = NULL;
	while(p){
		next = p->next;
		sysmem_freeptr(p);
		p = next;
	}
	if(x->point_hash){
		free(x->point_hash);
	}
	critical_free(x->lock);
	if(x->slots){
		sysmem_freeptr(x->slots);
	}
}

void xy_assist(t_xy *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "Point coordinates (list)");
			break;
		case 1:
			sprintf(s, "x,y coordinates of the selected point (list)");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "All point coordinates (list)");
			break;
		}
		break;
	}
}

double xy_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double xy_clip(double f, double min, double max){
	double ff = f;
	if(ff < min){
		ff = min;
	}
	if(ff > max){
		ff = max;
	}
	return ff;
}

double xy_edist(t_pt p1, t_pt p2){
	return sqrt(pow(p1.x - p2.x, 2.) + pow(p1.y - p2.y, 2.));
}

// for pattr
t_max_err xy_getvalueof(t_xy *x, long *ac, t_atom **av){
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);

	char alloc;
	atom_alloc_array(x->npoints * 3, ac, av, &alloc);

	critical_enter(x->lock);
	t_point *p = x->points;
	t_atom *ptr = *av + ((x->npoints * 3) - 1);
	while(p){
		atom_setfloat(ptr--, p->pt.y);
		atom_setfloat(ptr--, p->pt.x);
		atom_setlong(ptr--, p->id);
		p = p->next;
	}	
	critical_exit(x->lock);
	return MAX_ERR_NONE;
}

t_max_err xy_setvalueof(t_xy *x, long ac, t_atom *av){
	xy_clear(x);
	int i;
	t_atom *ptr = av;
	for(i = 0; i < ac / 3; i++){
		t_point *p = xy_newPoint(x);
		p->id = atom_getlong(ptr++);
		p->pt.x = atom_getfloat(ptr++);
		p->pt.y = atom_getfloat(ptr++);
		xy_addPoint(x, p);
	}
	xy_outputAll(x);
	return MAX_ERR_NONE;
}

/**************************************************
Object and instance creation functions.
**************************************************/

void *xy_new(t_symbol *msg, int argc, t_atom *argv){
	t_xy *x;
    
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

	if(x = (t_xy *)object_alloc(xy_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 

		x->dumpOutlet = outlet_new(x, NULL);
		x->interpOutlet = outlet_new(x, NULL);
		x->selectedOutlet = outlet_new(x, NULL);
		//x->distanceOutlet = outlet_new(x, NULL);
		x->listOutlet = outlet_new(x, NULL);
		x->npoints = 0;
		x->points = NULL;
		//x->monotonic_point_counter = 0;
		x->nslots = 256;
		x->slots = (char *)sysmem_newptr(x->nslots);
		memset(x->slots, '\0', x->nslots);
		x->pointdiameter = 3.0;

		//x->ht = hashtab_new(0);
		//hashtab_flags(x->ht, OBJ_FLAG_DATA);
		x->point_hash = (t_point **)calloc(HASH_LEN, sizeof(t_point *));
		x->point_hash_len = HASH_LEN;
		critical_new(&(x->lock));

		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
        
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("xydisplay", (method)xy_new, (method)xy_free, sizeof(t_xy), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)xy_paint, "paint", A_CANT, 0); 
	class_addmethod(c, (method)xy_bang, "bang", 0);
	//class_addmethod(c, (method)xy_int, "int", A_LONG, 0);
	class_addmethod(c, (method)xy_float, "float", A_FLOAT, 0);
	//class_addmethod(c, (method)xy_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)xy_set, "set", A_GIMME, 0);
	class_addmethod(c, (method)xy_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)xy_movePoint, "movepoint", A_LONG, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_setPoint, "setpoint", A_LONG, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_moveSelected, "moveselected", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_setSelected, "setselected", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_select, "select", A_LONG, 0);
	class_addmethod(c, (method)xy_setSelection, "setselection", A_LONG, 0);
	class_addmethod(c, (method)xy_unselect, "unselect", 0);
	class_addmethod(c, (method)xy_findNearest, "findnearest", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_findNearestX, "findnearestx", A_FLOAT, 0);
	class_addmethod(c, (method)xy_findNearestY, "findnearesty", A_FLOAT, 0);
	class_addmethod(c, (method)xy_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)xy_getvalueof, "getvalueof", A_CANT, 0);
	class_addmethod(c, (method)xy_setvalueof, "setvalueof", A_CANT, 0);
	class_addmethod(c, (method)xy_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)xy_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)xy_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method)xy_mousemove, "mousemove", A_CANT, 0);
	class_addmethod(c, (method)xy_mouseleave, "mouseleave", A_CANT, 0);
	class_addmethod(c, (method)xy_clear, "clear", 0);
	class_addmethod(c, (method)xy_dump, "dump", 0);
	class_addmethod(c, (method)xy_notify, "notify", A_CANT, 0); 
	class_addmethod(c, (method)xy_renumber, "renumber", 0);

	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_xy, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointcolor", 0, t_xy, pointcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointcolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointcolor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_xy, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 

 	CLASS_ATTR_RGBA(c, "selectedcolor", 0, t_xy, selectedcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedcolor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedcolor", 0, "rgba", "Selected Color"); 

	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

	CLASS_ATTR_ATOM_VARSIZE(c, "points", 0, t_xy, points, npoints, 1024);
	CLASS_ATTR_ACCESSORS(c, "points", xy_points_get, xy_points_set);
	CLASS_ATTR_SAVE(c, "points", 0);

	CLASS_ATTR_FLOAT(c, "pointdiameter", 0, t_xy, pointdiameter);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointdiameter", 0, "10.0");

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_xy, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_xy, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_xy, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_xy, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_LONG(c, "drawlabels", 0, t_xy, drawlabels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawlabels", 0, "0");

	CLASS_ATTR_LONG(c, "drawstupidlittlepoint", 0, t_xy, draw_stupid_little_point);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawstupidlittlepoint", 0, "1");

	CLASS_ATTR_LONG(c, "connect_points", 0, t_xy, connect_points);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "connect_points", 0, "0");

	CLASS_ATTR_LONG(c, "locked", 0, t_xy, locked);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "locked", 0, "0");

	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 200. 200."); 

	class_register(CLASS_BOX, c);
	xy_class = c;

	l_points = gensym("l_points");
	l_background = gensym("l_background");
	l_stupid_little_point = gensym("l_stupid_little_point");

	common_symbols_init();

#ifdef WIN32
#else
	srand(mach_absolute_time());
#endif

	version(0);

	return 0;
}

void xy_postPoint(t_point *p){
	if(!p){
		post("p == NULL!!");
		return;
	}
	post("point %p:", p);
	post("coords: %f %f", p->pt.x, p->pt.y);
	post("%p<--%p-->%p", p->prev, p, p->next);
}

t_max_err xy_notify(t_xy *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	if(msg == gensym("attr_modified")){
		t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		if(attrname == gensym("xmin") || attrname == gensym("xmax") || attrname == gensym("ymin") || attrname == gensym("ymax") || attrname == gensym("connect_points") || attrname == gensym("drawlabels")){
			jbox_invalidate_layer((t_object *)x, x->patcherview, l_points);
			jbox_redraw(&(x->ob));
		}
	}
	return 0;
}

t_max_err xy_points_get(t_xy *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc_array(x->npoints * 3, argc, argv, &alloc);
	t_atom *ptr = *argv;
	t_point *p = x->points;
	int i = 0;
	while(p){
		atom_setfloat(ptr + i++, p->pt.x);
		atom_setfloat(ptr + i++, p->pt.y);
		atom_setlong(ptr + i++, p->id);
		p = p->next;
	}
	return MAX_ERR_NONE;
}

t_max_err xy_points_set(t_xy *x, t_object *attr, long argc, t_atom *argv){
	int i;
	t_atom *ptr = argv + (argc - 1);
	for(i = 0; i < argc / 3; i++){
		t_point *p = xy_newPoint(x);
		if(!p){
			return MAX_ERR_NONE;
		}
		p->id = atom_getlong(ptr--);
		p->pt.y = atom_getfloat(ptr--);
		p->pt.x = atom_getfloat(ptr--);
		xy_addPoint(x, p);
	}
	return MAX_ERR_NONE;
}
