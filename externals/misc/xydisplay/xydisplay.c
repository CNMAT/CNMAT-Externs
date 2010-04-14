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

typedef struct _point{
	t_pt pt;
	struct _point *next;
	struct _point *prev;
} t_point;

typedef struct _xy{
	t_jbox ob;
	void *selectedOutlet, *listOutlet;//, *distanceOutlet;
	t_critical lock;
	t_jrgba bgcolor, pointcolor, bordercolor, selectedcolor, labelcolor;
	float pointdiameter;
	t_point *points;
	long npoints;
	t_point *selected;
	double xmin, xmax, ymin, ymax;
	int always_draw_circles, always_draw_labels;
	long monotonic_point_counter;
} t_xy;

static t_class *xy_class;

void xy_paint(t_xy *x, t_object *patcherview);
void xy_bang(t_xy *x);
void xy_set(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_list(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_processList(t_xy *x, t_symbol *msg, short argc, t_atom *argv);
void xy_movePoint(t_xy *x, long n, double xx, double yy);
void xy_setPoint(t_xy *x, long n, double xx, double yy);
void xy_doMovePoint(t_xy *x, t_point *p, double xx, double yy);
void xy_moveSelected(t_xy *x, double xx, double yy);
void xy_setSelected(t_xy *x, double xx, double yy);
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
t_point *xy_newPoint(void);
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

void xy_paint(t_xy *x, t_object *patcherview){
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

	// draw points
	{
		jgraphics_set_line_width(g, 1);
		// this should be 
		// critical_enter(x->lock);
		// make copy
		// critical_exit(x->lock);
		t_point *p = x->points;
		while(p){
			if(p == x->selected){
				jgraphics_set_source_jrgba(g, &(x->selectedcolor));
			}else{
				jgraphics_set_source_jrgba(g, &(x->pointcolor));
			}
			t_pt pt = p->pt;
			pt.x = xy_scale(pt.x, x->xmin, x->xmax, 0, rect.width);
			pt.y = xy_scale(pt.y, x->ymin, x->ymax, rect.height, 0);
			jgraphics_ellipse(g, pt.x - x->pointdiameter / 2., pt.y - x->pointdiameter / 2., x->pointdiameter, x->pointdiameter);
			jgraphics_fill(g);

			p = p->next;
		}
	}
}

void xy_bang(t_xy *x){
	// output everything
	xy_outputAll(x);
}

void xy_set(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
	xy_processList(x, msg, argc, argv);
	x->selected = NULL;
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xy_list(t_xy *x, t_symbol *msg, short argc, t_atom *argv){
	xy_processList(x, msg, argc, argv);
	x->selected = NULL;
	xy_outputPoints(x);
	object_notify(x, _sym_modified, NULL);
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
		t_point *p = xy_newPoint();
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
	t_point *p = xy_findPointByPosition(x, n);
	if(!p){
		object_error((t_object *)p, "couldn't find point");
		return;
	}
	x->selected = p;
	xy_doMovePoint(x, p, xx, yy);
	critical_exit(x->lock);
	xy_outputAll(x);
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xy_setPoint(t_xy *x, long n, double xx, double yy){
	critical_enter(x->lock);
	t_point *p = xy_findPointByPosition(x, n);
	if(!p){
		object_error((t_object *)p, "couldn't find point");
		return;
	}
	x->selected = p;
	xy_doMovePoint(x, p, xx, yy);
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
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
	jbox_redraw(&(x->ob));
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
	x->selected = xy_findPointByPosition(x, n);
	critical_exit(x->lock);
	if(x->selected){
		xy_outputSelected(x);
	}
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xy_setSelection(t_xy *x, long n){
	critical_enter(x->lock);
	x->selected = xy_findPointByPosition(x, n);
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xy_unselect(t_xy *x){
	critical_enter(x->lock);
	x->selected = NULL;
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
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
	case 0x10:
	case 0x12:
		{
			critical_enter(x->lock);
			x->selected = xy_selectPoint(x, pt);
			critical_exit(x->lock);
			if(!(x->selected)){
				t_point *p = xy_newPoint();
				if(!p){
					return;
				}
				p->pt.x = xy_scale(pt.x, 0., r.width, x->xmin, x->xmax);
				p->pt.y = xy_scale(pt.y, r.height, 0., x->ymin, x->ymax);
				xy_addPoint(x, p);
				x->selected = p;
			}
			if(modifiers == 0x10){
				xy_outputAll(x);
			}
		}
		break;
	case 0x11:
	case (0x11 | 0x12):
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
	jbox_redraw(&(x->ob));
}

void xy_mousedrag(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x10:
	case 0x12:
		critical_enter(x->lock);
		if(x->selected){
			x->selected->pt.x = xy_scale(pt.x, 0., r.width, x->xmin, x->xmax);
			x->selected->pt.y = xy_scale(pt.y, r.height, 0., x->ymin, x->ymax);
		}
		critical_exit(x->lock);
		if(modifiers == 0x12){
			xy_outputAll(x);
		}
		break;
	}
	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
}

void xy_mouseup(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

void xy_mouseleave(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

void xy_mousemove(t_xy *x, t_object *patcherview, t_pt pt, long modifiers){
}

t_point *xy_newPoint(void){
	t_point *p = (t_point *)sysmem_newptr(sizeof(t_point));
	if(p){
		p->next = NULL;
		p->prev = NULL;
		p->pt = (t_pt){0., 0.};
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
	critical_exit(x->lock);
}

void xy_removePoint(t_xy *x, t_point *p){
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

void xy_outputPoints(t_xy *x){
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);
	{
		t_point *p = x->points;
		long outlen = x->npoints * 2;
		t_atom out[outlen];
		t_atom *outptr = out + outlen - 1;
		critical_enter(x->lock);
		// since the points are pushed onto the front of the linked list, we should work backwards
		while(p){
			atom_setfloat(outptr--, p->pt.y);
			atom_setfloat(outptr--, p->pt.x);
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
	if(!(x->selected)){
		return;
	}
	t_atom out[3];
	int i = 0;
	critical_enter(x->lock);
	t_point *p = x->selected;
	while(p){
		i++;
		p = p->prev;
	}
	atom_setlong(out, i);
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
	t_rect r;
	jbox_get_patching_rect(&((x->ob.b_ob)), &r);

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
	x->selected = NULL;
	x->monotonic_point_counter = 0; // not really monotonic i guess...
	critical_exit(x->lock);
	object_notify(x, _sym_modified, NULL);
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
	xy_clear(x);
	critical_free(x->lock);
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
	atom_alloc_array(x->npoints * 2, ac, av, &alloc);

	critical_enter(x->lock);
	t_point *p = x->points;
	t_atom *ptr = *av;
	while(p){
		atom_setfloat(ptr++, p->pt.x);
		atom_setfloat(ptr++, p->pt.y);
		p = p->next;
	}	
	critical_exit(x->lock);
	return MAX_ERR_NONE;
}

t_max_err xy_setvalueof(t_xy *x, long ac, t_atom *av){
	//xy_list(x, NULL, ac, av);
	int i;
	for(i = 0; i < ac / 12; i++){
		xy_list(x, NULL, ac, av);
	}
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
		| JBOX_GROWY
		//| JBOX_GROWBOTH
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

		x->selectedOutlet = outlet_new(x, NULL);
		//x->distanceOutlet = outlet_new(x, NULL);
		x->listOutlet = outlet_new(x, NULL);
		x->npoints = 0;
		x->points = NULL;
		x->monotonic_point_counter = 0;
		x->pointdiameter = 3.0;

		//x->ht = hashtab_new(0);
		//hashtab_flags(x->ht, OBJ_FLAG_DATA);

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
	//class_addmethod(c, (method)xy_float, "float", A_FLOAT, 0);
	//class_addmethod(c, (method)xy_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)xy_set, "set", A_GIMME, 0);
	class_addmethod(c, (method)xy_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)xy_movePoint, "move_point", A_LONG, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_setPoint, "set_point", A_LONG, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_moveSelected, "move_selected", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_setSelected, "set_selected", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_select, "select", A_LONG, 0);
	class_addmethod(c, (method)xy_setSelection, "set_selection", A_LONG, 0);
	class_addmethod(c, (method)xy_unselect, "unselect", A_LONG, 0);
	class_addmethod(c, (method)xy_findNearest, "find_nearest", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)xy_findNearestX, "find_nearest_x", A_FLOAT, 0);
	class_addmethod(c, (method)xy_findNearestY, "find_nearest_y", A_FLOAT, 0);
	class_addmethod(c, (method)xy_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)xy_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)xy_mousedrag, "mousedrag", A_CANT, 0);
	class_addmethod(c, (method)xy_mouseup, "mouseup", A_CANT, 0);
	class_addmethod(c, (method)xy_mousemove, "mousemove", A_CANT, 0);
	class_addmethod(c, (method)xy_mouseleave, "mouseleave", A_CANT, 0);
	class_addmethod(c, (method)xy_clear, "clear", 0);
	class_addmethod(c, (method)xy_dump, "dump", 0);
	class_addmethod(c, (method)xy_notify, "notify", A_CANT, 0); 
    
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

	CLASS_ATTR_FLOAT(c, "pointdiameter", 0, t_xy, pointdiameter);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "pointdiameter", 0, "10.0");

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_xy, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_xy, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_xy, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");
	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_xy, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_LONG(c, "always_draw_circles", 0, t_xy, always_draw_circles);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "always_draw_circles", 0, "0");
	CLASS_ATTR_LONG(c, "always_draw_labels", 0, t_xy, always_draw_labels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "always_draw_labels", 0, "0");

	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 200. 200."); 

	class_register(CLASS_BOX, c);
	xy_class = c;

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
		if(attrname == gensym("xmin") || attrname == gensym("xmax") || attrname == gensym("ymin") || attrname == gensym("ymax")){
			jbox_redraw(&(x->ob));
		}
	}
	return 0;
}
