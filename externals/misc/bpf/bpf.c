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
  VERSION 0.1: bug fixes, scaling, function names, dotted background functions, dump out now includes index number for coll, displays point coordinates in the upper right corner
  VERSION 0.1.1: added functionNames message
  VERSION 0.2: rescaling now works properly
  VERSION 0.2.1: clear now resets the number of functions properly
  VERSION 0.2.2: assist function is now implemented and the second outlet behaves better when a list is sent into the obj
  VERSION 0.2.3: x coordinate from the leftmost outlet is now correct when a float is less than xmin or greater than xmax.
  VERSION 0.2.4: x coordinates are no longer output when a float is received in the leftmost inlet
  VERSION 0.2.5: the bang outlet (3rd) is now gone and the bang message that follows a dump now comes out the dump outlet
  VERSION 0.2.6: snap to int
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
*/

/*
todo:
extend to 0 and 1 function
functions array needs to be a linked hashtab to properly support the function attributes and the function name
carefully review use of locks
display mouse coords in upper left even when not dragging a point
needs to take signals
shift-drag while dragging a point should snap it to the y-value of the point with the closest y-value
 */

#include "version.h" 
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_critical.h"
#include "ext_hashtab.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "version.c" 

#ifdef WIN
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

#define MAX_NUM_FUNCTIONS 256 

typedef struct _point{ 
 	t_pt coords;
 	struct _point *next; 
 	struct _point *prev; 
} t_point; 

typedef struct _function_attributes{
	t_jrgba line_color, point_color;
	double dash[8];
	long ndash;
	char name[64];
} t_function_attributes, t_funcattr;

typedef struct _bpf{ 
 	t_jbox box; 
 	void *out_main, *out_dump, *out_sel; 
	t_critical lock;
 	t_point **functions; 
	t_funcattr **funcattr;
	t_point *selected;
 	int currentFunction; 
 	int numFunctions; 
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
	t_jrgba textColor;
	double xmin, xmax, ymin, ymax;
	t_symbol *x_res, *y_res;
	//double scalex, scaley, offsetx, offsety;
} t_bpf; 

void *bpf_class; 

void bpf_paint(t_bpf *x, t_object *patcherview); 
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
void bpf_addFunction(t_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void bpf_functionList(t_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void bpf_setFunction(t_bpf *x, long f); 
void bpf_setFunctionName(t_bpf *x, t_symbol *name);
//void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
//void bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum);
void bpf_removePoint(t_bpf *x, t_point *point, int functionNum);
void bpf_clear(t_bpf *x);
double bpf_scale(double f, double min_in, double max_in, double min_out, double max_out);
void bpf_xminmax(t_bpf *x, double min, double max);
void bpf_yminmax(t_bpf *x, double min, double max);
//void bpf_read(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_doread(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_write(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_dowrite(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
int main(void); 
void *bpf_new(t_symbol *s, long argc, t_atom *argv); 

t_max_err bpf_xmin_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_xmax_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_ymin_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_ymax_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);


void myobject_write(t_bpf *x, t_symbol *s);
void myobject_dowrite(t_bpf *x, t_symbol *s);
void myobject_writefile(t_bpf *x, char *filename, short path);

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
				t_pt sc;
				sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, 0, rect.width);
				sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, rect.height, 0);
				jgraphics_move_to(g, sc.x, sc.y);
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_dash(g, NULL, 0, 0);
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_dash(g, (double[2]){3., 3.}, 2, 0);
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}
				jgraphics_ellipse(g, sc.x - 3., sc.y - 3., 6., 6.);
				jgraphics_fill(g);
				p = p->next;
			}
			while(p){
				t_pt sc;
				sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, 0, rect.width);
				sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, rect.height, 0);
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}
				jgraphics_ellipse(g, sc.x - 3., sc.y - 3., 6., 6.);
				jgraphics_fill(g);
				if(i == x->currentFunction){
					jgraphics_set_dash(g, NULL, 0, 0);
					jgraphics_set_source_jrgba(g, &(x->lineColor));
				}else{
					jgraphics_set_dash(g, (double[2]){3., 3.}, 2, 0);
					jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
				}
				jgraphics_move_to(g, bpf_scale(p->prev->coords.x, x->xmin, x->xmax, 0, rect.width), bpf_scale(p->prev->coords.y, x->ymin, x->ymax, rect.height, 0));
				jgraphics_line_to(g, sc.x, sc.y);
				jgraphics_stroke(g);
				p = p->next;
			}
		}

		jgraphics_set_source_jrgba(g, &(x->textColor));
		jgraphics_set_font_size(g, 10);
		char buf[128];
		if(x->selected){	
			sprintf(buf, "%s:(%f, %f)", x->funcattr[x->currentFunction]->name, x->selected->coords.x, x->selected->coords.y);
		}else{
			sprintf(buf, "%s", x->funcattr[x->currentFunction]->name);
		}
		jgraphics_move_to(g, 1, 10);
		jgraphics_show_text(g, buf);

		critical_exit(x->lock);
 	} 
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
	bpf_outputSelection(x);
	t_pt pt = (t_pt){atom_getfloat(a++), atom_getfloat(a++)};
	if(x->x_res == gensym("int")){
		pt.x = round(pt.x);
	}
	if(x->y_res == gensym("int")){
		pt.y = round(pt.y);
	}
	x->selected = bpf_insertPoint(x, pt, functionNum);
	jbox_redraw(&(x->box));
}

void bpf_float(t_bpf *x, double f){
	//double f = (ff - x->xmin) / (x->xmax - x->xmin);;
	//if(f < x->xmin) f = 0.;
	//if(f > x->xmax) f = 1.;
	t_atom out[2]; // function number, y
	//atom_setfloat(&(out[1]), f);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		t_point *left = NULL, *right = NULL;
		bpf_find_btn(p, f, &left, &right);
		if(!left || !right){
			atom_setfloat(&(out[1]), 0.);
			outlet_list(x->out_main, NULL, 2, out);
		}else{
			double m = (right->coords.y - left->coords.y) / (right->coords.x - left->coords.x);
			double b = left->coords.y - (m * left->coords.x);
			double y = (m * f) + b;
			atom_setfloat(&(out[1]), y);
			outlet_list(x->out_main, NULL, 2, out);
		}
	}
}

void bpf_find_btn(t_point *function, double x, t_point **left, t_point **right){
	//post("function = %p", function);
	if(!function){
		return;
	}
	t_point *ptr = function;
	if(x < function->coords.x){
		*left = NULL;
		*right = function;
		return;
	}
	while(ptr->next){
		if(x >= ptr->coords.x && x <= ptr->next->coords.x){
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
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);

	t_pt p_sc;
	p_sc.x = bpf_scale(p.x, x->xmin, x->xmax, 0, r.width);
	p_sc.y = bpf_scale(p.y, x->ymin, x->ymax, r.height, 0);

	while(ptr){
		t_pt sc;
		sc.x = bpf_scale(ptr->coords.x, x->xmin, x->xmax, 0, r.width);
		sc.y = bpf_scale(ptr->coords.y, x->ymin, x->ymax, r.height, 0);
		//post("%f %f %f %f", p_sc.x, p_sc.y, sc.x, sc.y);
		if((xdif = fabs(sc.x - p_sc.x)) < 3 && (ydif = fabs(sc.y - p_sc.y)) < 3){
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
 		t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		if(attrname == gensym("x_res") || attrname == gensym("y_res")){
			int i;
			for(i = 0; i < x->numFunctions; i++){
				t_point *p = x->functions[i];
				while(p){
					if(x->x_res == gensym("int")){
						p->coords.x = round(p->coords.x);
					}
					if(x->y_res == gensym("int")){
						p->coords.y = round(p->coords.y);
					}
					p = p->next;
				}
			}
		}
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
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	t_pt coords;
	coords.x = bpf_scale(pt.x, 0, r.width, x->xmin, x->xmax);
	coords.y = bpf_scale(pt.y, r.height, 0, x->ymin, x->ymax);
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		if(x->selected = bpf_select(x, coords)){
			break;
		}else{
			if(x->x_res == gensym("int")){
				coords.x = round(coords.x);
			}
			if(x->y_res == gensym("int")){
				coords.y = round(coords.y);
			}
			x->selected = bpf_insertPoint(x, coords, x->currentFunction);
		}
		break;
	case 0x12:
		// shift.  
		if(x->selected = bpf_select(x, coords)){
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

	t_pt sc;
	sc.x = bpf_scale(pt.x, 0, r.width, x->xmin, x->xmax);
	sc.y = bpf_scale(pt.y, r.height, 0, x->ymin, x->ymax);
	if(x->x_res == gensym("int")){
		sc.x = round(sc.x);
	}
	if(x->y_res == gensym("int")){
		sc.y = round(sc.y);
	}

	switch(modifiers){
	case 0x10:
		bpf_removePoint(x, x->selected, x->currentFunction);
		x->selected = bpf_insertPoint(x, sc, x->currentFunction);
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
	t_atom out[3];
	atom_setlong(&(out[0]), x->currentFunction);
	atom_setfloat(&(out[1]), x->selected->coords.x);
	atom_setfloat(&(out[2]), x->selected->coords.y);
	outlet_list(x->out_sel, NULL, 3, out);
}

void bpf_addFunction(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	critical_enter(x->lock);
	if(x->numFunctions + 1 > MAX_NUM_FUNCTIONS){
		object_error((t_object *)x, "bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	x->numFunctions++;
	critical_exit(x->lock);
	bpf_setFunction(x, x->numFunctions - 1);
	if(argc){
		switch(atom_gettype(argv)){
		case A_FLOAT:
			sprintf(x->funcattr[x->currentFunction]->name, "%f", atom_getfloat(argv));
			break;
		case A_LONG:
			sprintf(x->funcattr[x->currentFunction]->name, "%ld", atom_getlong(argv));
			break;
		case A_SYM:
			strncpy(x->funcattr[x->currentFunction]->name, atom_getsym(argv)->s_name, 64);
			break;
		}
	}
	jbox_redraw(&(x->box));
}

void bpf_functionList(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc > MAX_NUM_FUNCTIONS){
		object_error((t_object *)x, "bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	critical_enter(x->lock);
	int i;
	for(i = 0; i < argc; i++){
		strncpy(x->funcattr[i]->name, atom_getsym(argv + i)->s_name, 64);
	}
	x->numFunctions = argc;
	x->currentFunction = 0;
	critical_exit(x->lock);
	jbox_redraw(&(x->box));
}

void bpf_setFunction(t_bpf *x, long f){
	if(f > MAX_NUM_FUNCTIONS){
		error("bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	if(f < 0){
		error("bpf: function number must be positive");
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

void bpf_setFunctionName(t_bpf *x, t_symbol *name){
	strncpy(x->funcattr[x->currentFunction]->name, name->s_name, 64);
	jbox_redraw(&(x->box));
}

/*
void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = fabs(r.height - screen_coords.y) / r.height;
}

void bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords){
	screen_coords->x = norm_coords.x * r.width;
	screen_coords->y = fabs((norm_coords.y * r.height) - r.height);
}
*/

t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum){
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = (t_point *)calloc(1, sizeof(t_point));
	p->coords = coords;
	if(*function == NULL){
		p->prev = NULL;
		p->next = NULL;
		*function = p;
	}else if(p->coords.x < (*function)->coords.x){
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
			if(p->coords.x >= current->coords.x && p->coords.x <= next->coords.x){
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
	critical_exit(x->lock);
	return p;
}

void bpf_removePoint(t_bpf *x, t_point *point, int functionNum){
	if(!point){
		return;
	}
	critical_enter(x->lock);
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


double bpf_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

void bpf_dump(t_bpf *x){
	int i, point_num = 0;
	t_atom out[4];
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[1]), i);
		while(p){
			atom_setlong(&(out[0]), point_num++);
			atom_setfloat(&(out[2]), p->coords.x);
			atom_setfloat(&(out[3]), p->coords.y);
			outlet_list(x->out_dump, NULL, 4, out);
			p = p->next;
		}
	}
	outlet_bang(x->out_dump);
}

void bpf_xminmax(t_bpf *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
	jbox_redraw(&(x->box));
}

void bpf_yminmax(t_bpf *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_redraw(&(x->box));
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
	critical_enter(x->lock);
	x->numFunctions = 1;
	x->currentFunction = 0;
	critical_exit(x->lock);
	jbox_redraw(&(x->box));
}

void bpf_assist(t_bpf *x, void *b, long io, long num, char *s){ 
 	if(io == ASSIST_OUTLET){ 
		switch(num){
		case 0:
			sprintf(s, "Functions evaluated for a given x value. (list)");
			break;
		case 1:
			sprintf(s, "The (x,y) coordinates for the selected point. (list)");
			break;
		case 2:
			sprintf(s, "Bang when finished dumping.");
			break;
		case 3:
			sprintf(s, "Dump outlet (list)");
			break;
		}
 	}else{ 
 		switch (num) {	 
 		case 0: 
			sprintf(s, "Input a float to evaluate the functions at that x value.");
 			break; 
 		} 
 	} 
} 

void bpf_free(t_bpf *x){ 
 	jbox_free((t_jbox *)x); 
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		t_point *next;
		while(p){
			next = p->next;
			free(p);
			p = next;
			next = next->next;
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
 	class_addmethod(c, (method)bpf_addFunction, "addFunction", A_GIMME, 0); 
 	class_addmethod(c, (method)bpf_functionList, "functionList", A_GIMME, 0); 
 	class_addmethod(c, (method)bpf_setFunction, "setFunction", A_LONG, 0); 
	class_addmethod(c, (method)bpf_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)bpf_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)bpf_clear, "clear", 0);
	class_addmethod(c, (method)bpf_dump, "dump", 0);
	class_addmethod(c, (method)bpf_setFunctionName, "setFunctionName", A_SYM, 0);
	class_addmethod(c, (method)bpf_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)bpf_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_bpf, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_bpf, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_bpf, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_bpf, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_SYM(c, "x_res", 0, t_bpf, x_res);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "x_res", 0, "float");

	CLASS_ATTR_SYM(c, "y_res", 0, t_bpf, y_res);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "y_res", 0, "float");

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

 	CLASS_ATTR_RGBA(c, "textColor", 0, t_bpf, textColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "textColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "textColor", 0, "rgba", "Text Color"); 

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
		x->out_sel = listout((t_object *)x);
 		x->out_main = listout((t_object *)x); 

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *));
 		x->funcattr = (t_funcattr **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_funcattr *));
 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		critical_new(&(x->lock));

 		attr_dictionary_process(x, d); 

		int i;
		for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
			x->funcattr[i] = (t_funcattr *)calloc(1, sizeof(t_funcattr));
			x->funcattr[i]->line_color = x->lineColor;
			x->funcattr[i]->point_color = x->pointColor;
			memset(x->funcattr[i]->dash, 0, 8);
			x->funcattr[i]->ndash = 0;
			sprintf(x->funcattr[i]->name, "%d", i);
		}
 		jbox_ready((t_jbox *)x); 

 		return x; 
 	} 
 	return NULL; 
} 

/*
t_max_err bpf_xmin_set(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	float f;
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}

	f = atom_getfloat(argv);
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			//p->screen_coords.x *= ((x->xmax - x->xmin) / (x->xmax - f));
			bpf_getNormCoords(r, p->screen_coords, &norm_coords);
			double scaledx = norm_coords.x * (x->xmax - x->xmin) + x->xmin;
			scaledx = (scaledx - f) / (x->xmax - f);
			p->screen_coords.x = scaledx * r.width;
			p = p->next;
		}
	}
	x->xmin = f;

	return 0;
}

t_max_err bpf_xmax_set(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	float f;
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}

	f = atom_getfloat(argv);
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			//p->screen_coords.x *= ((x->xmax - x->xmin) / (f - x->xmin));
			bpf_getNormCoords(r, p->screen_coords, &norm_coords);
			double scaledx = norm_coords.x * (x->xmax - x->xmin) + x->xmin;
			scaledx = (scaledx - x->xmin) / (f - x->xmin);
			p->screen_coords.x = scaledx * r.width;
			p = p->next;
		}
	}
	x->xmax = f;

	return 0;
}

t_max_err bpf_ymin_set(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	float f;
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}

	f = atom_getfloat(argv);
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			double normy = (r.height - p->screen_coords.y) / r.height;
			double scaledy = normy * (x->ymax - x->ymin) + x->ymin;
			scaledy = (scaledy - f) / (x->ymax - f);
			p->screen_coords.y = ((1 - scaledy) * r.height);
			p = p->next;
		}
	}
	x->ymin = f;

	return 0;
}

t_max_err bpf_ymax_set(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	float f;
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}

	f = atom_getfloat(argv);
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			double normy = (r.height - p->screen_coords.y) / r.height;
			double scaledy = normy * (x->ymax - x->ymin) + x->ymin;
			scaledy = (scaledy - x->ymin) / (f - x->ymin);
			p->screen_coords.y = ((1 - scaledy) * r.height);
			p = p->next;
		}
	}
	x->ymax = f;

	return 0;
}
*/
