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
  NAME: bpf 
  DESCRIPTION: Breakpoint function editor 
  AUTHORS: John MacCallum 
  COPYRIGHT_YEARS: 2009 
  SVN_REVISION: $LastChangedRevision: 587 $ 
  VERSION 0.0: First try 
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
*/

#include "version.h" 
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_critical.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "version.c" 

#define MAX_NUM_FUNCTIONS 256 

typedef struct _point{ 
 	t_pt screen_coords; 
 	struct _point *next; 
 	struct _point *prev; 
} t_point; 


typedef struct _bpf{ 
 	t_jbox box; 
 	void *out_data, *out_info; 
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
 	int currentFunction; 
 	int numFunctions; 
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
} t_bpf; 

void *bpf_class; 

int main(void); 
void *bpf_new(t_symbol *s, long argc, t_atom *argv); 
void bpf_paint(t_bpf *x, t_object *patcherview); 
void bpf_draw_bounds(t_bpf *x, t_jgraphics *g, t_rect *rect); 
void bpf_assist(t_bpf *x, void *b, long m, long a, char *s); 
void bpf_free(t_bpf *x); 
t_max_err bpf_notify(t_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void bpf_mousedown(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mousedrag(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_addFunction(t_bpf *x); 
void bpf_setFunction(t_bpf *x, long f); 
void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
t_point *bpf_insertPoint(t_bpf *x, t_pt screen_coords);
void bpf_removePoint(t_bpf *x, t_point *point);

int main(void){ 
 	t_class *c = class_new("bpf", (method)bpf_new, (method)bpf_free, sizeof(t_bpf), 0L, A_GIMME, 0); 

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

 	class_addmethod(c, (method)bpf_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)bpf_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mouseup, "mouseup", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_addFunction, "addFunction", 0); 
 	class_addmethod(c, (method)bpf_setFunction, "setFunction", A_LONG, 0); 

 	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_bpf, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointColor", 0, t_bpf, pointColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointColor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "lineColor", 0, t_bpf, lineColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lineColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "lineColor", 0, "rgba", "Line Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

 	class_register(CLASS_BOX, c); 
 	bpf_class = c; 

 	version(0); 
	
 	return 0; 
} 

void *bpf_new(t_symbol *s, long argc, t_atom *argv){ 
 	t_bpf *x = NULL; 
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

 	if(x = (t_bpf *)object_alloc(bpf_class)){ 

 		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->box.b_firstin = (void *)x; 

 		//x->out_info = outlet_new((t_object *)x, NULL); 
 		//x->out_data = listout((t_object *)x); 

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *)); 
 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		critical_new(&(x->lock));

 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 

 		return x; 
 	} 
 	return NULL; 
} 

void bpf_paint(t_bpf *x, t_object *patcherview){ 
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

	//jgraphics_set_source_jrgba(g, &(x->pointColor));
	t_jrgba bgroundLine;
	bgroundLine.red = bgroundLine.green = bgroundLine.blue = 0.;
	bgroundLine.alpha = 0.75;

 	{ 
		critical_enter(x->lock);
		int i;
		for(i = 0; i < x->numFunctions; i++){
			t_point *p = x->functions[i];
			if(i == x->currentFunction){
				jgraphics_set_source_jrgba(g, &(x->pointColor));
				jgraphics_set_line_width(g, 1.5);
			}else{
				jgraphics_set_source_jrgba(g, &bgroundLine);
				jgraphics_set_line_width(g, .75);
			}
			if(p){
				jgraphics_move_to(g, p->screen_coords.x, p->screen_coords.y);
				jgraphics_ellipse(g, p->screen_coords.x - 2, p->screen_coords.y - 2, 4., 4.);
				jgraphics_fill(g);
				p = p->next;
			}
			while(p){
				//post("drawing point %f %f", p->screen_coords.x, p->screen_coords.y);
				t_pt norm_coords;
				//bpf_getNormCoords(rect, p->screen_coords, &norm_coords);
				jgraphics_ellipse(g, p->screen_coords.x - 2, p->screen_coords.y - 2, 4., 4.);
				jgraphics_fill(g);
				jgraphics_move_to(g, p->prev->screen_coords.x, p->prev->screen_coords.y);
				jgraphics_line_to(g, p->screen_coords.x, p->screen_coords.y);
				jgraphics_stroke(g);
				p = p->next;
			}
		}
		critical_exit(x->lock);
 	} 
} 

void bpf_assist(t_bpf *x, void *b, long m, long a, char *s){ 
 	if (m == ASSIST_OUTLET){ 
 	}else{ 
 		switch (a) {	 
 		case 0: 
 			break; 
 		} 
 	} 
} 

void bpf_free(t_bpf *x){ 
 	jbox_free((t_jbox *)x); 
} 

t_max_err bpf_notify(t_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	//t_symbol *attrname; 
 	if (msg == gensym("attr_modified")){ 
 		//attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
 		jbox_redraw(&(x->box)); 
	} 
	return 0; 
} 

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void bpf_mousedown(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){ 
 	//post("0x%X", modifiers); 
 	t_rect r; 
	//t_pt norm_coords;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	//bpf_getNormCoords(r, pt, &norm_coords);
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		//post("inserting %f %f", pt.x, pt.y);
		x->selected = bpf_insertPoint(x, pt);
		break;
	case 0x12:
		// shift.  
		break;
	case 0x13:
		break;
	}
	jbox_redraw(&(x->box));
}

void bpf_mousedrag(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
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

	switch(modifiers){
	case 0x10:
		bpf_removePoint(x, x->selected);
		x->selected = bpf_insertPoint(x, pt);
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	}
	jbox_redraw(&(x->box));
}

void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
}

void bpf_addFunction(t_bpf *x){
	critical_enter(x->lock);
	x->numFunctions++;
	critical_exit(x->lock);
	bpf_setFunction(x, x->numFunctions - 1);
	jbox_redraw(&(x->box));
}

void bpf_setFunction(t_bpf *x, long f){
	critical_enter(x->lock);
	x->currentFunction = f;
	critical_exit(x->lock);
	jbox_redraw(&(x->box));
}

void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = abs(r.height - screen_coords.y) / r.height;
}

t_point *bpf_insertPoint(t_bpf *x, t_pt screen_coords){
	critical_enter(x->lock);
	t_point **function = &(x->functions[x->currentFunction]);
	t_point *p = (t_point *)calloc(1, sizeof(t_point));
	p->screen_coords.x = screen_coords.x;
	p->screen_coords.y = screen_coords.y;
	if(*function == NULL){
		p->prev = NULL;
		p->next = NULL;
		*function = p;
	}else if(p->screen_coords.x < (*function)->screen_coords.x){
		p->prev = NULL;
		p->next = (*function);
		(*function)->prev = p;
		x->functions[x->currentFunction] = p;
	}else{
		int i = 1;
		t_point *current, *next;
		current = (*function);
		next = current->next;
		while(next){
			if(p->screen_coords.x >= current->screen_coords.x && p->screen_coords.x <= next->screen_coords.x){
				current->next = p;
				next->prev = p;
				p->next = next;
				p->prev = current;
				goto out;
			}
			current = next;
			next = next->next;
			i++;
		}
		p->prev = current;
		p->next = NULL;
		current->next = p;
	}
 out:
	//post("inserted point %p %f %f", p, screen_coords.x, screen_coords.y);
	critical_exit(x->lock);
	return p;
}

void bpf_removePoint(t_bpf *x, t_point *point){
	if(!point){
		return;
	}
	critical_enter(x->lock);
	//post("removing point %p %f %f", point, point->screen_coords.x, point->screen_coords.y);
	t_point **function = &(x->functions[x->currentFunction]);
	t_point *p = *function;
	int i = 0;
	while(p){
		if(p == point){
			if(p->prev){
				if(p->next){
					p->prev->next = p->next;
				}else{
					p->prev->next = NULL;
				}
			}
			if(p->next){
				if(p->prev){
					p->next->prev = p->prev;
				}else{
					p->next->prev = NULL;
				}
			}
			if(i == 0){
				x->functions[x->currentFunction] = p->next;
			}

			if(p){
				free(p);
			}
			goto out;
		}
		i++;
		p = p->next;
	}
 out:
	critical_exit(x->lock);
}
