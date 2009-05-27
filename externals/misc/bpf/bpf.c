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
#include "ext_hashtab.h"
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
 	void *out_main, *out_dump, *out_sel, *out_bang; 
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
 	int currentFunction; 
 	int numFunctions; 
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
} t_bpf; 

void *bpf_class; 

void bpf_paint(t_bpf *x, t_object *patcherview); 
void bpf_editSel(t_bpf *x, double xx, double yy);
void bpf_list(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
void bpf_float(t_bpf *x, double f);
void bpf_find_btn(t_point *function, double x, t_point **left, t_point **right);
t_point *bpf_select(t_bpf *x, t_pt p);
void bpf_draw_bounds(t_bpf *x, t_jgraphics *g, t_rect *rect); 
void bpf_assist(t_bpf *x, void *b, long m, long a, char *s); 
void bpf_free(t_bpf *x); 
t_max_err bpf_notify(t_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void bpf_mousedown(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mousedrag(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_outputSelection(t_bpf *x);
void bpf_addFunction(t_bpf *x); 
void bpf_setFunction(t_bpf *x, long f); 
void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
void bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
t_point *bpf_insertPoint(t_bpf *x, t_pt screen_coords, int functionNum);
void bpf_removePoint(t_bpf *x, t_point *point, int functionNum);
void bpf_clear(t_bpf *x);
int main(void); 
void *bpf_new(t_symbol *s, long argc, t_atom *argv); 

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

 	{ 
		critical_enter(x->lock);
		int i;
		for(i = 0; i < x->numFunctions; i++){
			t_point *p = x->functions[i];
			if(p){
				jgraphics_move_to(g, p->screen_coords.x, p->screen_coords.y);
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}
				jgraphics_ellipse(g, p->screen_coords.x - 3., p->screen_coords.y - 3., 6., 6.);
				jgraphics_fill(g);
				p = p->next;
			}
			while(p){
				//post("drawing point %f %f", p->screen_coords.x, p->screen_coords.y);
				t_pt norm_coords;
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}
				jgraphics_ellipse(g, p->screen_coords.x - 3., p->screen_coords.y - 3., 6., 6.);
				jgraphics_fill(g);
				if(i == x->currentFunction){
					jgraphics_set_source_jrgba(g, &(x->lineColor));
				}else{
					jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
				}
				jgraphics_move_to(g, p->prev->screen_coords.x, p->prev->screen_coords.y);
				jgraphics_line_to(g, p->screen_coords.x, p->screen_coords.y);
				jgraphics_stroke(g);
				p = p->next;
			}
		}
		critical_exit(x->lock);
 	} 
} 

void bpf_editSel(t_bpf *x, double xx, double yy){
	t_pt screen_coords, norm_coords = {xx, yy};
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	bpf_getScreenCoords(r, norm_coords, &screen_coords);
	post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
	bpf_removePoint(x, x->selected, x->currentFunction);
	x->selected = bpf_insertPoint(x, screen_coords, x->currentFunction);
	jbox_redraw(&(x->box));
}

void bpf_list(t_bpf *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom *a = argv;
	int functionNum = x->currentFunction;
	if(argc == 3){
		functionNum = atom_getlong(a++);
		if(functionNum > x->numFunctions - 1){
			x->numFunctions = functionNum + 1;
		}
	}
	t_pt screen_coords, norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	norm_coords.x = atom_getfloat(a++);
	norm_coords.y = atom_getfloat(a++);
	bpf_getScreenCoords(r, norm_coords, &screen_coords);
	//post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
	x->selected = bpf_insertPoint(x, screen_coords, functionNum);
	jbox_redraw(&(x->box));
}

void bpf_float(t_bpf *x, double f){
	if(f < 0.) f = 0.;
	if(f > 1.) f = 1.;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	double screenx = f * r.width;
	t_atom out[3]; // function number, x, y
	atom_setfloat(&(out[1]), f);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		t_point *left = NULL, *right = NULL;
		bpf_find_btn(p, screenx, &left, &right);
		//return;
		if(!left || !right){
			atom_setfloat(&(out[2]), 0.);
			outlet_list(x->out_main, NULL, 3, out);
		}else{
			double m = (right->screen_coords.y - left->screen_coords.y) / (right->screen_coords.x - left->screen_coords.x);
			double b = left->screen_coords.y - (m * left->screen_coords.x);
			double y = (m * screenx) + b;
			atom_setfloat(&(out[2]), abs(y - r.height) / r.height);
			outlet_list(x->out_main, NULL, 3, out);
		}
	}
}

void bpf_find_btn(t_point *function, double x, t_point **left, t_point **right){
	//post("function = %p", function);
	if(!function){
		return;
	}
	t_point *ptr = function;
	//post("%f %f %f", x, function->screen_coords.x, function->screen_coords.y);
	//return;
	if(x < function->screen_coords.x){
		*left = NULL;
		*right = function;
		return;
	}
	while(ptr->next){
		if(x >= ptr->screen_coords.x && x <= ptr->next->screen_coords.x){
			*left = ptr;
			*right = ptr->next;
			return;
		}
		ptr = ptr->next;
	}
	*left = ptr;
	*right = NULL;
}

t_point *bpf_select(t_bpf *x, t_pt p){
	double min = 1000000000.;
	t_point *min_ptr = NULL;
	t_point *ptr = x->functions[x->currentFunction];
	double xdif, ydif;
	while(ptr){
		if((xdif = abs(p.x - ptr->screen_coords.x)) < 3 && (ydif = abs(p.y - ptr->screen_coords.y)) < 3){
			if(xdif + ydif < min){
				min = xdif + ydif;
				min_ptr = ptr;
			}
		}
		ptr = ptr->next;
	}
	return min_ptr;
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
		if(x->selected = bpf_select(x, pt)){
			break;
		}else{
			x->selected = bpf_insertPoint(x, pt, x->currentFunction);
		}
		break;
	case 0x12:
		// shift.  
		if(x->selected = bpf_select(x, pt)){
			bpf_removePoint(x, x->selected, x->currentFunction);
			break;
		}
		break;
	case 0x13:
		break;
	}
	bpf_outputSelection(x);
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
		bpf_removePoint(x, x->selected, x->currentFunction);
		x->selected = bpf_insertPoint(x, pt, x->currentFunction);
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	}
	bpf_outputSelection(x);
	jbox_redraw(&(x->box));
}

void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
}

void bpf_outputSelection(t_bpf *x){
	if(!(x->selected)){
		return;
	}
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	t_pt norm_coords;
	t_atom out[3];
	bpf_getNormCoords(r, x->selected->screen_coords, &norm_coords);
	atom_setlong(&(out[0]), x->currentFunction);
	atom_setfloat(&(out[1]), norm_coords.x);
	atom_setfloat(&(out[2]), norm_coords.y);
	outlet_list(x->out_sel, NULL, 3, out);
}

void bpf_addFunction(t_bpf *x){
	critical_enter(x->lock);
	if(x->numFunctions + 1 > MAX_NUM_FUNCTIONS){
		error("bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	x->numFunctions++;
	critical_exit(x->lock);
	bpf_setFunction(x, x->numFunctions - 1);
	jbox_redraw(&(x->box));
}

void bpf_setFunction(t_bpf *x, long f){
	if(f > MAX_NUM_FUNCTIONS){
		error("bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	if(f > x->numFunctions - 1){
		while(f > x->numFunctions - 1){
			x->numFunctions++;
		}
	}
	critical_enter(x->lock);
	x->currentFunction = f;
	x->selected = NULL;
	critical_exit(x->lock);
	jbox_redraw(&(x->box));
}

void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = abs(r.height - screen_coords.y) / r.height;
}

void bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords){
	screen_coords->x = norm_coords.x * r.width;
	screen_coords->y = abs((norm_coords.y * r.height) - r.height);
}

t_point *bpf_insertPoint(t_bpf *x, t_pt screen_coords, int functionNum){
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
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
		x->functions[functionNum] = p;
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

void bpf_removePoint(t_bpf *x, t_point *point, int functionNum){
	if(!point){
		return;
	}
	critical_enter(x->lock);
	//post("removing point %p %f %f", point, point->screen_coords.x, point->screen_coords.y);
	t_point **function = &(x->functions[functionNum]);
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
				x->functions[functionNum] = p->next;
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

void bpf_dump(t_bpf *x){
	int i;
	t_atom out[3];
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		while(p){
			bpf_getNormCoords(r, p->screen_coords, &norm_coords);
			atom_setfloat(&(out[1]), norm_coords.x);
			atom_setfloat(&(out[2]), norm_coords.y);
			outlet_list(x->out_dump, NULL, 3, out);
			p = p->next;
		}
	}
	outlet_bang(x->out_bang);
}

void bpf_clear(t_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		if(p){
			free(p);
		}
		x->functions[i] = NULL;
	}
	jbox_redraw(&(x->box));
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
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			if(p){
				free(p);
			}
			p = p->next;
		}
	}
} 

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
	class_addmethod(c, (method)bpf_editSel, "editSelection", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)bpf_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)bpf_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)bpf_clear, "clear", 0);
	class_addmethod(c, (method)bpf_dump, "dump", 0);

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

 	CLASS_ATTR_RGBA(c, "bgFuncColor", 0, t_bpf, bgFuncColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgFuncColor", 0, "0. 0. 0. 0.5"); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgFuncColor", 0, "rgba", "Background Function Color"); 

 	CLASS_ATTR_RGBA(c, "selectionColor", 0, t_bpf, selectionColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectionColor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectionColor", 0, "rgba", "Selection Color"); 

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

 		x->out_dump = listout((t_object *)x); 
		x->out_bang = bangout((t_object *)x);
		x->out_sel = listout((t_object *)x);
 		x->out_main = listout((t_object *)x); 

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

