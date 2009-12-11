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
  VERSION 0.11: added critical sections and fixed a memory leak
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

typedef struct _point{
	double x, y;
	t_symbol *label;
	t_jrgba color;
	struct _point *next;
	struct _point *prev;
} t_point;

typedef struct _xydisplay{
	t_jbox ob;
	void *selectedOutlet, *listOutlet;
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
} t_xydisplay;

static t_class *xydisplay_class;

void xydisplay_paint(t_xydisplay *x, t_object *patcherview);
void xydisplay_bang(t_xydisplay *x);
void xydisplay_list(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv);
void xydisplay_anything(t_xydisplay *x, t_symbol *msg, short argc, t_atom *argv);
void xydisplay_mousedown(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers);
void xydisplay_mousedrag(t_xydisplay *x, t_object *patcherview, t_pt pt, long modifiers);
t_point *xydisplay_selectPoint(t_xydisplay *x, t_pt pt);
void xydisplay_removePoint(t_xydisplay *x, t_point *p);
void xydisplay_outputPoints(t_xydisplay *x);
void xydisplay_rename(t_xydisplay *x, t_symbol *msg, int argc, t_atom *argv);
void xydisplay_clear(t_xydisplay *x);
void xydisplay_free(t_xydisplay *x);
void xydisplay_assist(t_xydisplay *x, void *b, long m, long a, char *s);
double xydisplay_scale(double f, double min_in, double max_in, double min_out, double max_out);
double xydisplay_clip(double f, double min, double max);
t_max_err xydisplay_getvalueof(t_xydisplay *x, long *ac, t_atom **av);
t_max_err xydisplay_setvalueof(t_xydisplay *x, long ac, t_atom *av);
void *xydisplay_new(t_symbol *msg, int argc, t_atom *argv);


void xydisplay_paint(t_xydisplay *x, t_object *patcherview){
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
		t_point *p = x->points;
		critical_enter(x->lock);
		while(p){
			if(p == x->selected){
				jgraphics_set_source_jrgba(g, &(x->selectedcolor));
			}else{
				jgraphics_set_source_jrgba(g, &(x->pointcolor));
			}
			// small center circle
			jgraphics_ellipse(g, p->x - (x->pointdiameter / 2), p->y - (x->pointdiameter / 2), x->pointdiameter, x->pointdiameter);
			if(x->draw_circle){
				jgraphics_ellipse(g, p->x - .5, p->y - .5, 1, 1);
			}
			jgraphics_stroke(g);

			if(x->draw_label && p->label){
				jgraphics_set_source_jrgba(g, &(p->color));
				jgraphics_set_font_size(g, 10);
				double w, h;
				jgraphics_text_measure(g, p->label->s_name, &w, &h);
				jgraphics_move_to(g, p->x - (w / 2.), p->y - (h / 2.));
				jgraphics_show_text(g, p->label->s_name);
			}
			jgraphics_stroke(g);

			p = p->next;
		}
		critical_exit(x->lock);
	}
}

void xydisplay_bang(t_xydisplay *x){
	xydisplay_outputPoints(x);
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
				x->selected = p;
				x->npoints++;
				char buf[16];
				sprintf(buf, "%ld", x->monotonic_point_counter++);
				p->label = gensym(buf);
				p->color = x->labelcolor;
				critical_exit(x->lock);
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
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	xydisplay_outputPoints(x);
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
			critical_exit(x->lock);
		}
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	}

	object_notify(x, _sym_modified, NULL);
	jbox_redraw(&(x->ob));
	xydisplay_outputPoints(x);
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
			t_atom *outptr = out;
			critical_enter(x->lock);
			while(p){
				atom_setfloat(outptr++, xydisplay_scale(p->x, 0, r.width, x->xmin, x->xmax));
				atom_setfloat(outptr++, xydisplay_scale(p->y, r.height, 0, x->ymin, x->ymax));
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
			t_atom *ptr = out;
			critical_enter(x->lock);
			while(p){
				atom_setsym(ptr++, p->label);
				atom_setfloat(ptr++, xydisplay_scale(p->x, 0, r.width, x->xmin, x->xmax));
				atom_setfloat(ptr++, xydisplay_scale(p->y, r.height, 0, x->ymin, x->ymax));
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
		x->listOutlet = outlet_new(x, NULL);
		x->npoints = 0;
		x->points = NULL;
		x->monotonic_point_counter = 0;

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
	class_addmethod(c, (method)xydisplay_clear, "clear", 0);
	class_addmethod(c, (method)xydisplay_rename, "rename", A_GIMME, 0);
    
	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 
    
 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_xydisplay, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
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
    
    
	class_register(CLASS_BOX, c);
	xydisplay_class = c;

	common_symbols_init();

	version(0);

	return 0;
}
