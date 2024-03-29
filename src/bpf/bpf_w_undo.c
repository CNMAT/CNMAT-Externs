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
  VERSION 0.3: points track location on all functions, save with patcher
  VERSION 0.3.1: added margins and point numbers that monotonically increase from left to right
  VERSION 0.3.2: multiple selections are possible
  VERSION 0.3.3: fixed capslock bug
  VERSION 0.3.4: fixed a bug with the way that the background was being drawn
  VERSION 0.3.5: added locks to the x and y dimensions and a step mode which turns the function into a step function
  VERSION 0.4: takes a signal as input and outputs via te_breakout~
  VERSION 0.4.1: fixed a bug in the perform routine
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
#include "z_dsp.h"
#include "version.c" 

#ifndef FLT_MAX
#define FLT_MAX 999999999.f
#endif

#ifdef WIN
#include "../../../max-sdk-base/c74support/max-includes/common/commonsyms.c"
#endif

#define MAX_NUM_FUNCTIONS 64

#define XMARGIN 5
#define YMARGIN 5

typedef struct _point{ 
 	t_pt coords;
	int id;
	int selected;
 	struct _point *next; 
 	struct _point *prev; 
 	struct _point *next_selected; 
 	struct _point *prev_selected;
} t_point; 

typedef struct _function_attributes{
	t_jrgba line_color, point_color;
	double dash[8];
	long ndash;
	char name[64];
} t_function_attributes, t_funcattr;


//------->
typedef void (*lfptr)(void *, void *, void *, void *); //log function pointer type

typedef struct	_logNode{	
	//not using delta for now... but could be a fast way of undoing in the future
	//	void *delta;	//change in value
	void *data1; 
	void *data2;
	void *data3;
	//	void *pt_ptr; //reference (t_point if in terms of points - or as andother void * data slot)
	lfptr unF; 	//pointer to undo function (reverse sign of delta)
//	lfptr doF;  //poiter to do function
	struct _logNode *next;
	struct _logNode *prev;
} t_logNode;
//<---------

typedef struct _bpf{ 
 	t_pxjbox box; 
 	void *out_main, *out_dump, *out_sel; 
	t_object *pv;
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
	t_funcattr **funcattr;
 	int currentFunction; 
 	int numFunctions; 
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
	t_jrgba textColor;
	double xmin, xmax, ymin, ymax;
	t_symbol *xres, *yres;
	t_pt *pos;
	long drawpos, drawlabels;
	int *monotonic_point_counter;
	int *npoints;
	t_rect sel_box;
	t_pt drag;
	int lockx, locky, lockinput;
	int step;
	double xmargin, ymargin;
	int labelstart;
	int *hideFunctions;
	t_symbol *name;
	t_float **ptrs;

	//----->
	t_logNode *history;
	t_logNode *topNode;
	t_logNode *logCursor;
	t_point *origin;
	bool mdrag;
	//<------
} t_bpf; 

void *bpf_class; 

//----->

int bpf_historyLog(t_bpf *x, void *data1, void *data2, void *data3, lfptr unF);
void bpf_printLog(t_bpf *x);
void bpf_clearHistory(t_bpf *x);
void bpf_undo(t_bpf *x);
t_logNode *bpf_getUndo(t_bpf *x);
t_point *bpf_replacePoint(t_bpf *x, t_point *point, int functionNum);
t_point *bpf_separateSelectedGroup(t_bpf *x);

t_point *bpf_getOrigin(t_bpf *x);

void bpf_undoPointCMD(void *b, void *point, void *functionNum, void *nothing);
void bpf_clearCMD(void *b, void *nothing, void *morenothing,void *evenmorenothing);
void bpf_reDragCMD(void *bpf, void *origin, void *newPt, void *functionNum);
void bpf_unDragCMD(void *bpf, void *origin, void *newPt, void *functionNum);
//<------

void bpf_paint(t_bpf *x, t_object *patcherview); 
void bpf_dsp(t_bpf *x, t_signal **sp, short *count);
t_int *bpf_perform(t_int *w);
t_symbol *bpf_mangleName(t_symbol *name, int i, int fnum);
void bpf_list(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
void bpf_float(t_bpf *x, double f);
double bpf_compute(t_bpf *x, int function, double f);
void bpf_find_btn(t_bpf *x, t_point *function, double xx, t_point **left, t_point **right);
t_point *bpf_select(t_bpf *x, t_pt p);
void bpf_draw_bounds(t_bpf *x, t_jgraphics *g, t_rect *rect); 
void bpf_invalidateAllPos(t_bpf *x);
void bpf_assist(t_bpf *x, void *b, long m, long a, char *s); 
void bpf_free(t_bpf *x); 
t_max_err bpf_notify(t_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void bpf_clearSelectedAndRedraw(t_bpf *x);
void bpf_clearSelected(t_bpf *x);
void bpf_mousedown(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mousedrag(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void bpf_outputSelection(t_bpf *x);
void bpf_addFunction(t_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void bpf_hideFunction(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
void bpf_functionList(t_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void bpf_setFunction(t_bpf *x, long f); 
void bpf_setFunctionName(t_bpf *x, t_symbol *name);
//void bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
//void bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
//void bpf_renumber(t_bpf *x);
void bpf_zoomToFit(t_bpf *x);
t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum, int id);
void bpf_removeSelected(t_bpf *x);
void bpf_removePoint(t_bpf *x, t_point *point, int functionNum);
void bpf_clear(t_bpf *x);
double bpf_scale(double f, double min_in, double max_in, double min_out, double max_out);
void bpf_xminmax(t_bpf *x, double min, double max);
void bpf_yminmax(t_bpf *x, double min, double max);
void bpf_resizeRectWithMargins(t_bpf *x, t_rect *r);
//void bpf_read(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_doread(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_write(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_dowrite(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
int main(void); 
void *bpf_new(t_symbol *s, long argc, t_atom *argv); 
t_max_err bpf_points_get(t_bpf *x, t_object *attr, long *argc, t_atom **argv);
t_max_err bpf_points_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);

void myobject_write(t_bpf *x, t_symbol *s);
void myobject_dowrite(t_bpf *x, t_symbol *s);
void myobject_writefile(t_bpf *x, char *filename, short path);

t_symbol *l_background, *l_points, **l_pos, *ps_int, *l_axes;

void bpf_paint(t_bpf *x, t_object *patcherview){ 
	x->pv = patcherview;
 	t_rect r; 
 	t_jrgba c = (t_jrgba){0., 0., 0., 1.};;

 	// get graphics context 
 	//t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 

 	// get our box's rectangle 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &r); 

	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_background, r.width, r.height);
	if(g){
		// draw the outline of the box 
		jgraphics_set_source_jrgba(g, &c); 
		jgraphics_set_line_width(g, 1); 
		jgraphics_rectangle(g, 0., 0., r.width, r.height); 
		jgraphics_stroke(g); 

		jgraphics_set_source_jrgba(g, &(x->bgcolor)); 
		jgraphics_rectangle(g, 0., 0., r.width, r.height); 
		jgraphics_fill(g); 
		jbox_end_layer((t_object *)x, patcherview, l_background);
	}
	jbox_paint_layer((t_object *)x, patcherview, l_background, 0, 0);

	// points
	g = jbox_start_layer((t_object *)x, patcherview, l_points, r.width, r.height);
	bpf_resizeRectWithMargins(x, &r);

 	if(g){ 
		critical_enter(x->lock);
		int i, j;
		for(i = 0; i < x->numFunctions; i++){
			if(x->hideFunctions[i]){
				continue;
			}
			j = x->labelstart;
			t_point *p = x->functions[i];
			if(p){
				t_pt sc;
				if(x->xres == ps_int){
					sc.x = bpf_scale(round(p->coords.x), x->xmin, x->xmax, r.x, r.width);
				}else{
					sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				}
				if(x->yres == ps_int){
					sc.y = bpf_scale(round(p->coords.y), x->ymin, x->ymax, r.height, r.y);
				}else{
					sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
				}
				jgraphics_move_to(g, sc.x, sc.y);
				//if(p == x->selected){
				if(p->selected){
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
				if(x->drawlabels){
					char buf[16];
					sprintf(buf, "%d", j++);
					double w, h;
					jgraphics_text_measure(g, buf, &w, &h);
					if(sc.y < 20){
						jgraphics_move_to(g, sc.x - w / 2., sc.y + h);
					}else{
						jgraphics_move_to(g, sc.x - w / 2., sc.y - h / 2.);
					}
					jgraphics_show_text(g, buf);
				}
				p = p->next;
			}
			while(p){
				t_pt sc;
				if(x->xres == ps_int){
					sc.x = bpf_scale(round(p->coords.x), x->xmin, x->xmax, r.x, r.width);
				}else{
					sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				}
				if(x->yres == ps_int){
					sc.y = bpf_scale(round(p->coords.y), x->ymin, x->ymax, r.height, r.y);
				}else{
					sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
				}
				//if(p == x->selected){
				if(p->selected){
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
				double px = p->prev->coords.x, py = p->prev->coords.y;
				if(x->xres == ps_int){
					px = round(px);
				}
				if(x->yres == ps_int){
					py = round(py);
				}
				double pysc = bpf_scale(py, x->ymin, x->ymax, r.height, r.y);
				jgraphics_move_to(g, bpf_scale(px, x->xmin, x->xmax, r.x, r.width), pysc);
				if(x->step && p->prev->coords.y != p->coords.y){
					jgraphics_line_to(g, sc.x, pysc);
					jgraphics_line_to(g, sc.x, sc.y);
					jgraphics_ellipse(g, sc.x - 2, pysc - 2, 4, 4);
				}else{
					jgraphics_line_to(g, sc.x, sc.y);
				}
				jgraphics_stroke(g);
				if(x->drawlabels){
					char buf[16];
					sprintf(buf, "%d", j++);
					double w, h;
					jgraphics_text_measure(g, buf, &w, &h);
					if(sc.y < 20){
						jgraphics_move_to(g, sc.x - w / 2., sc.y + h);
					}else{
						jgraphics_move_to(g, sc.x - w / 2., sc.y - h / 2.);
					}
					jgraphics_show_text(g, buf);
				}
				p = p->next;
			}
		}

		jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
		jgraphics_stroke(g);

		critical_exit(x->lock);
		jbox_end_layer((t_object *)x, patcherview, l_points);
 	} 
	jbox_paint_layer((t_object *)x, patcherview, l_points, r.x, r.y);
	if(x->drawpos){
		int i;
		for(i = 0; i < x->numFunctions; i++){
			g = jbox_start_layer((t_object *)x, patcherview, l_pos[i], 4, 4);
			if(g){
				if(x->currentFunction == i){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					jgraphics_set_source_jrgba(g, &(x->lineColor));
				}
				jgraphics_ellipse(g, 0, 0, 4, 4);
				jgraphics_fill(g);
				jbox_end_layer((t_object *)x, patcherview, l_pos[i]);
			}
			jbox_paint_layer((t_object *)x, patcherview, l_pos[i], x->pos[i].x - 2, x->pos[i].y - 2);
		}
	}
} 

void bpf_dsp(t_bpf *x, t_signal **sp, short *count){
	if(count[0]){
		dsp_add64(bpf_perform, 3, x, sp[0]->s_n, sp[0]->s_vec);
	}
}

t_int *bpf_perform(t_int *w){
	t_bpf *x = (t_bpf *)w[1];
	int n = (int)w[2];
	t_float *in = (t_float *)w[3];
	int i, j;
	if(x->name){
		for(i = 0; i < x->numFunctions; i++){
			if(x->functions[i] == NULL){
				memset(x->ptrs[i], 0, n * sizeof(t_float));
			}
			t_symbol *name;
			if(name = bpf_mangleName(x->name, 0, i)){
				name->s_thing = (t_object *)(x->ptrs[i]);
			}
		}
		for(i = 0; i < n; i++){
			for(j = 0; j < x->numFunctions; j++){
				x->ptrs[j][i] = bpf_compute(x, j, in[i]);
			}
		}
	}

	return w + 4;
}

t_symbol *bpf_mangleName(t_symbol *name, int i, int fnum){
	if(!name){
		return NULL;
	}
	char buf[256];
	sprintf(buf, "bkout_%s_%d_%d", name->s_name, i, fnum);
	return gensym(buf);
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
	//bpf_outputSelection(x);
	t_pt pt = (t_pt){atom_getfloat(a++), atom_getfloat(a++)};

	//x->selected = bpf_insertPoint(x, pt, functionNum, x->monotonic_point_counter[functionNum]++);
	t_point *p = bpf_insertPoint(x, pt, functionNum, x->monotonic_point_counter[functionNum]++);
	/*
	if(x->selected){
		x->selected->prev = p;
	}
	p->next_selected = x->selected;
	p->prev = NULL;
	x->selected = p;
	*/
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_float(t_bpf *x, double f){
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);
	bpf_resizeRectWithMargins(x, &r);
	t_atom out[2]; // function number, y
	int i;
	for(i = 0; i < x->numFunctions; i++){
		atom_setlong(&(out[0]), i);
		double y = bpf_compute(x, i, f);
		x->pos[i] = (t_pt){bpf_scale(f, x->xmin, x->xmax, r.x, r.width) + x->xmargin, bpf_scale(y, x->ymin, x->ymax, r.height, r.y) + x->ymargin};

		atom_setfloat(&(out[1]), y);
		if(x->drawpos){
			jbox_invalidate_layer((t_object *)x, x->pv, l_pos[i]);
		}
		outlet_list(x->out_main, NULL, 2, out);
	}
	if(x->drawpos){
		jbox_redraw((t_jbox *)(t_jbox *)(&x->box));
	}
}

double bpf_compute(t_bpf *x, int function, double f){
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);
	bpf_resizeRectWithMargins(x, &r);
	x->pos[function] = (t_pt){-1., -1.};
	t_point *p = x->functions[function];
	t_point *left = NULL, *right = NULL;
	bpf_find_btn(x, p, f, &left, &right);

	if(!left || !right){
		return 0;
	}else{
		double lx = left->coords.x, ly = left->coords.y;
		double rx = right->coords.x, ry = right->coords.y;
		if(x->xres == ps_int){
			lx = round(lx);
			rx = round(rx);
		}
		if(x->yres == ps_int){
			ly = round(ly);
			ry = round(ry);
		}
		if(x->step){
			return ly;
		}else{
			double m = (ry - ly) / (rx - lx);
			double b = ly - (m * lx);
			double y = (m * f) + b;
			return y;
		}
	}
}

void bpf_find_btn(t_bpf *x, t_point *function, double xx, t_point **left, t_point **right){
	//post("function = %p", function);
	if(!function){
		return;
	}
	t_point *ptr = function;
	if(x->xres == ps_int){
		if(xx < round(function->coords.x)){
			*left = NULL;
			*right = function;
			return;
		}
		while(ptr->next){
			if(xx >= round(ptr->coords.x) && xx <= round(ptr->next->coords.x)){
				*left = ptr;
				*right = ptr->next;
				return;
			}
			ptr = ptr->next;
		}
	}else{
		if(xx < function->coords.x){
			*left = NULL;
			*right = function;
			return;
		}
		while(ptr->next){
			if(xx >= ptr->coords.x && xx <= ptr->next->coords.x){
				*left = ptr;
				*right = ptr->next;
				return;
			}
			ptr = ptr->next;
		}
	}
	*left = ptr;
	*right = NULL;
}

t_point *bpf_select(t_bpf *x, t_pt p_sc){
	double min = 1000000000.;
	t_point *min_ptr = NULL;
	t_point *ptr = x->functions[x->currentFunction];
	double xdif, ydif;
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);
	bpf_resizeRectWithMargins(x, &r);
	//r.x = r.y = 0;

	//t_pt p_sc;
	//p_sc.x = bpf_scale(p.x, x->xmin, x->xmax, r.x, r.width);
	//p_sc.y = bpf_scale(p.y, x->ymin, x->ymax, r.height, r.y);
	/*
	if(x->xres == ps_int){
		p_sc.x = round(p_sc.x);
	}
	if(x->yres == ps_int){
		p_sc.y = round(p_sc.y);
	}
	*/
	while(ptr){
		t_pt sc = ptr->coords;
		if(x->xres == ps_int){
			sc.x = round(sc.x);
		}
		if(x->yres == ps_int){
			sc.y = round(sc.y);
		}

		sc.x = bpf_scale(sc.x, x->xmin, x->xmax, r.x, r.width);
		sc.y = bpf_scale(sc.y, x->ymin, x->ymax, r.height, r.y);

		//post("click: %f %f, point: %f %f", p_sc.x, p_sc.y, sc.x, sc.y);
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
		/*
		if(attrname == gensym("xres") || attrname == gensym("yres")){
			int i;
			for(i = 0; i < x->numFunctions; i++){
				t_point *p = x->functions[i];
				while(p){
					if(x->xres == gensym("int")){
						p->coords.x = round(p->coords.x);
					}
					if(x->yres == gensym("int")){
						p->coords.y = round(p->coords.y);
					}
					p = p->next;
				}
			}
		}
		*/
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
		jbox_invalidate_layer((t_object *)x, x->pv, l_background);
		jbox_invalidate_layer((t_object *)x, x->pv, l_points);
 		jbox_redraw((t_jbox *)&(x->box)); 
	} 
	return 0; 
} 

void bpf_clearSelectedAndRedraw(t_bpf *x){
	bpf_clearSelected(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void bpf_clearSelected(t_bpf *x){
	if(x->selected){
		t_point *p = x->selected, *next;
		while(p){
			p->selected = 0;
			next = p->next_selected;
			p->prev_selected = p->next_selected = NULL;
			p = next;
		}
	}
	x->selected = NULL;
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
	bpf_resizeRectWithMargins(x, &r);
	pt.x -= x->xmargin;
	pt.y -= x->ymargin;
	x->drag = pt;
	//r.x = r.y = 0;
	t_pt coords;
	coords.x = bpf_scale(pt.x, r.x, r.width, x->xmin, x->xmax);
	coords.y = bpf_scale(pt.y, r.height, r.y, x->ymin, x->ymax);
	switch(modifiers){
	case 0x110:
	case 0x10:{
		// no modifiers.  
		t_point *p;
		t_point *s = x->selected;
		//---->
		if(s){
			post("mousedown s %p selected, at {%f, %f}",x->selected, x->selected->coords.x,x->selected->coords.y);
			x->origin = s;
//			x->origin = bpf_getOrigin(x);
		}
		//<----
		while(s){
			double sx = s->coords.x, sy = s->coords.y;
			if(x->xres == ps_int){
				sx = round(sx);
				pt.x = round(pt.x);
			}
			if(x->yres == ps_int){
				sy = round(sy);
				pt.y = round(pt.y);
			}
			if(abs(bpf_scale(sx, x->xmin, x->xmax, r.x, r.width) - pt.x) < 3 && abs(bpf_scale(sy, x->ymin, x->ymax, r.height, r.y) - pt.y) < 3){
				break;
			}
			s = s->next_selected;
		}
		if(s){
			break;
		}else{
			bpf_clearSelected(x);
			if(p = bpf_select(x, pt)){
				p->selected = 1;
				x->selected = p;
				//---->
				post("mousedown else %p selected, at {%f, %f}",x->selected, x->selected->coords.x,x->selected->coords.y);
				x->origin = x->selected;
//				x->origin = bpf_getOrigin(x);
				//<----
			}
			if(p){
				break;
			}
		}

		if(!x->lockinput){
			p = bpf_insertPoint(x, coords, x->currentFunction, x->monotonic_point_counter[x->currentFunction]++);
			p->selected = 1;
			if(x->selected){
				x->selected->prev_selected = p;
			}
			p->next_selected = x->selected;
			p->prev_selected = NULL;
			x->selected = p;
			//----->
//			x->origin = bpf_getOrigin(x);
			x->origin = p;
			post("mousedown new %p selected, at {%f, %f}",x->selected, x->selected->coords.x,x->selected->coords.y);
			int *fnum = (int *)calloc(1, sizeof(int));
			*fnum = x->currentFunction;
			bpf_historyLog(x, p, fnum, NULL, (lfptr)&bpf_undoPointCMD);
			//<-------
		}
	}break;
	case 0x112:
	case 0x12:{
		// shift.  
		t_point *p;
		if(p = bpf_select(x, pt)){
			bpf_removePoint(x, p, x->currentFunction);
			break;
		}
	}break;
	case 0x113:
	case 0x13:
		x->sel_box.x = pt.x;
		x->sel_box.y = pt.y;
		x->sel_box.width = 0;
		x->sel_box.height = 0;
		break;
	}
	bpf_outputSelection(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_mousedrag(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
	x->mdrag = TRUE;
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	bpf_resizeRectWithMargins(x, &r);
	pt.x -= x->xmargin;
	pt.y -= x->ymargin;
	//r.x = r.y = 0;
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
	sc.x = bpf_scale(pt.x, r.x, r.width, x->xmin, x->xmax);
	sc.y = bpf_scale(pt.y, r.height, r.y, x->ymin, x->ymax);

	switch(modifiers){
	case 0x110:
	case 0x10:{
		t_point *p = x->selected;		
		t_point *next, *prev;
		while(p){
			next = p->next_selected;
			prev = p->prev_selected;
			int id = p->id;
			int head = 0;
			head = (p == x->selected);
			t_pt pp = (t_pt){bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
			if(!x->lockx){
				pp.x += (pt.x - x->drag.x);
			}
			if(!x->locky){
				pp.y += (pt.y - x->drag.y);
			}
			pp.x = bpf_scale(pp.x, r.x, r.width, x->xmin, x->xmax);
			pp.y = bpf_scale(pp.y, r.height, r.y, x->ymin, x->ymax);
//			post("rem %p", p);
			bpf_removePoint(x, p, x->currentFunction);
			p = bpf_insertPoint(x, pp, x->currentFunction, id);
//			post("in %p", p);
			p->selected = 1;
			if(head){
				x->selected = p;
			}
			p->next_selected = next;
			p->prev_selected = prev;
			if(next){
				next->prev_selected = p;
			}
			if(prev){
				prev->next_selected = p;
			}
			p = p->next_selected;
		}
		
	}
		break;
	case 0x112:
	case 0x12:
		break;
	case 0x113:
	case 0x13:{
		x->sel_box.width = pt.x - x->sel_box.x;
		x->sel_box.height = pt.y - x->sel_box.y;
		bpf_clearSelected(x);
		t_point *p = x->functions[x->currentFunction];
		while(p){
			t_pt coords;
			coords.x = bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
			coords.y = bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
			double left = x->sel_box.width > 0 ? x->sel_box.x : x->sel_box.x + x->sel_box.width;
			double right = x->sel_box.width > 0 ? x->sel_box.x + x->sel_box.width : x->sel_box.x;
			double top = x->sel_box.height > 0 ? x->sel_box.y : x->sel_box.y + x->sel_box.height;
			double bottom = x->sel_box.height > 0 ? x->sel_box.y + x->sel_box.height : x->sel_box.y;
			if(coords.x > left && coords.x < right && coords.y < bottom && coords.y > top){
				p->selected = 1;
				p->next_selected = x->selected;
				if(x->selected){
					x->selected->prev_selected = p;
				}
				x->selected = p;
			}
			p = p->next;
		}
	}break;
	}
	
//	post("dragged, x->origin = %p {%f, %f}", x->origin, x->origin->coords.x, x->origin->coords.y);
	
	x->drag = pt;
	bpf_outputSelection(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
	//------->
	if(x->mdrag){
		int *fnum = (int *)calloc(1, sizeof(int));
		*fnum = x->currentFunction;
		t_point *o = x->origin;		
		t_point *p = x->selected;
		post("mouseup o %p {%f, %f} p %p {%f, %f}", o, o->coords.x, o->coords.y, p, p->coords.x, p->coords.y); // if removePoint free's points this doesn't work - find which points need to get stored (copied)
		if((o != p) && (!p->next_selected)){
			o->next = p;
			bpf_historyLog(x, o, p, fnum, (lfptr)&bpf_unDragCMD);//lfptr = log function pointer typedef, see above
			x->origin = x->selected;

		} else if(x->sel_box.width && x->sel_box.height && p->next_selected){
			x->origin = p;
			
			while(p){
				post("mouseup selected %p",p);
				p = p->next_selected;
			}
			
			//bpf_printLog(x);
	
		} else if(p->next_selected && o->next_selected){
			while(o && p){
				o->next = p;
				//post("origin select list %p o->next %p (new point = %p)  -- next_selected: %p", o, o->next, p, o->next_selected);
				o = o->next_selected;
				p = p->next_selected;
			}
			bpf_historyLog(x, x->origin, x->selected, fnum, (lfptr)&bpf_unDragCMD);
			x->origin = x->selected;

		}
	}
	x->sel_box = (t_rect){0., 0., 0., 0.};	
	x->mdrag = FALSE;//	post("mouse drag = %f %f", x->drag.x, x->drag.y);
	//<-------
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

//---->
void bpf_reDragCMD(void *bpf, void *origin, void *newPt, void *functionNum){
	//return from origin to new point
	t_bpf *x = (t_bpf *)bpf;
	t_point *o = (t_point *)origin;
	t_point *n = (t_point *)newPt;
	int fnum = *(int *)functionNum;
	
	bpf_removePoint(x, o, fnum);
	bpf_insertPoint(x, n->coords, fnum, o->id);
	//add multiple points to do
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
	
}

void bpf_unDragCMD(void *bpf, void *origin, void *newPt, void *functionNum)
{
	//return to origin
	t_bpf *x = (t_bpf *)bpf;
	t_point *o = (t_point *)origin;
	t_point *n = (t_point *)newPt;
	int fnum = *(int *)functionNum;
	
	if(o->next_selected){
		//post("orig %p new %p o->next_selected %p n->next_selected %p", o, n, o->next_selected, n->next_selected);
		while(o && n){
			//post("list removing {%p} inserting {%p}", n, o);
			bpf_removePoint(x, n, fnum); //...need to protect next selected bit
			bpf_replacePoint(x, o, fnum);
			o = o->next_selected;
			n = n->next_selected;
			//post("next removing {%p} inserting {%p}", n, o);
		}
	} else { 
 
		//post("removing {%p} inserting {%p}", n, o);
		bpf_removePoint(x, n, fnum);
		bpf_replacePoint(x, o, fnum);
	}

	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));

}
//<----

void bpf_outputSelection(t_bpf *x){
	if(!(x->selected)){
		return;
	}
	t_point *p = x->selected;
	t_atom out[3];
	while(p){
		atom_setlong(&(out[0]), x->currentFunction);
		if(x->xres == ps_int){
			atom_setfloat(&(out[1]), round(x->selected->coords.x));
		}else{
			atom_setfloat(&(out[1]), x->selected->coords.x);
		}
		if(x->yres == ps_int){
			atom_setfloat(&(out[2]), round(x->selected->coords.y));
		}else{
			atom_setfloat(&(out[2]), x->selected->coords.y);
		}
		outlet_list(x->out_sel, NULL, 3, out);
		p = p->next_selected;
	}

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
        jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
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
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_hideFunction(t_bpf *x, t_symbol *msg, short argc, t_atom *argv){
        long function;
        long b;
        function = atom_getlong(argv);
        if(argc == 3){ // from matrixctrl
                b = atom_getlong(argv + 2);
        }else if(argc == 2){
                b = atom_getlong(argv + 1);
        }else{
                object_error((t_object *)x, "two arguments required: function number, on/off");
                return;
	}
        x->hideFunctions[function] = b;
        jbox_invalidate_layer((t_object *)x, x->pv, l_points);
        jbox_redraw((t_jbox *)(t_jbox *)x);
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
	bpf_clearSelected(x);
	critical_exit(x->lock);
        jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_setFunctionName(t_bpf *x, t_symbol *name){
	strncpy(x->funcattr[x->currentFunction]->name, name->s_name, 64);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
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
/*
void bpf_renumber(t_bpf *x){
	int i;
	critical_enter(x->lock);
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i], *prev = NULL;
		x->monotonic_point_counter[i] = 0;
		while(p){
			x->monotonic_point_counter[i]++;
			prev = p;
			p = p->next;
		}
		p = prev;
		int mpc = x->monotonic_point_counter[i];
		while(p){
			p->id = --mpc;
			p = p->prev;
		}
	}
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)(t_jbox *)&x->box);
}
*/

void bpf_zoomToFit(t_bpf *x){
	int i;
	x->xmin = x->ymin = FLT_MAX;
	x->xmax = x->ymax = 0;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		while(p){
			if(p->coords.x < x->xmin){
				x->xmin = p->coords.x;
			}
			if(p->coords.x > x->xmax){
				x->xmax = p->coords.x;
			}
			if(p->coords.y < x->ymin){
				x->ymin = p->coords.y;
			}
			if(p->coords.y > x->ymax){
				x->ymax = p->coords.y;
			}
			p = p->next;
		}
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)(t_jbox *)&(x->box));
}

//-------->
t_point *bpf_replacePoint(t_bpf *x, t_point *p, int functionNum){
	//same as insertPoint, but only places point back into next<->prev list
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
//	t_point *p = (t_point *)calloc(1, sizeof(t_point));	
//	p->coords = coords;
//	p->id = id;
//	x->npoints[functionNum]++;
//	p->next_selected = p->prev_selected = NULL;
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
//<---------


t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum, int id){
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = (t_point *)calloc(1, sizeof(t_point));
	p->coords = coords;
	p->id = id;
	x->npoints[functionNum]++;
	p->next_selected = p->prev_selected = NULL;
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

void bpf_removeSelected(t_bpf *x){
	t_point *p = x->selected, *next;
	while(p){
		next = p->next_selected;
		bpf_removePoint(x, p, x->currentFunction);
		p = next;
	}
	x->selected = NULL;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}


//----->

t_point *bpf_getOrigin(t_bpf *x){
	t_point *s = x->selected;
	if(s->next_selected){

		if(s->coords.x < s->next_selected->coords.x) //left to right selection
			return s;
		if(s->coords.x > s->next_selected->coords.x){ //right to left selection
			t_point *l = s;
			while(s->next_selected){
				s = s->next_selected;
				l->prev_selected = s;
			}
			return l;
		}
	}
	return s;
}


t_point *bpf_separateSelectedGroup(t_bpf *x){
	t_point *s = x->selected, *n, *first, *last, *right, *left;
	t_point *r = s;
	
	//determine selection direction
	if(s->coords.x < s->next_selected->coords.x){ //left to right selection
		first = s;
		left = s->prev;
		n = left;
		while(s){
			if(s->next != s->next_selected){
				n->next = s->next;
				s->next->prev = n;
				n = n->next;
			}
			s = s->next;
		}
		r = first;
	}
	if(s->coords.x > s->next_selected->coords.x){ //right to left selection
		last = s;
		right = s->next;
		n = right;
		while(s){
			if(s->prev != s->next_selected){
				n->prev = s->prev;
				s->prev->next = n;
				n = n->prev;
			}
			s = s->prev;
		}
		r = last;
	}
	
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
	return r; //returns first point of next_selected group
}


void bpf_undoPointCMD(void *b, void *point, void *functionNum, void *nothing){
	int fnum = *(int *)functionNum;
	t_point *p = (t_point *)point;
	t_bpf *x = (t_bpf *)b;
	if(x && p){
		post("removePoint %p {%f, %f}, functionNum = %d", p, p->coords.x, p->coords.y, fnum);
//		bpf_removePoint((t_bpf *)b, (t_point *)point, *(int *)functionNum); //fast cast
		bpf_removePoint(x, p, fnum); //debugging
		jbox_invalidate_layer((t_object *)x, x->pv, l_points);
		jbox_redraw((t_jbox *)&(x->box));
		
	} else {
		post("problem in undoPointCMD");
	}
}


//<-------

void bpf_removePoint(t_bpf *x, t_point *point, int functionNum){
	if(!point){
		return;
	}
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = *function;
	int i = 0;
	x->npoints[functionNum]--;
	
	t_point *o = x->origin;
	while(p){
		if(p == point){
			if(p->next){
				p->next->prev = p->prev;
			}
			if(p->prev){
				p->prev->next = p->next;
			}
			if(i == 0){
				x->functions[functionNum] = p->next;
			}
			
			if(p == o){
				goto out;
			} else {
				while(o){
					if((p->next_selected == o->next_selected) || (p->prev_selected == o->prev_selected)){
						post("removepoint o %p o->next_selected %p o->prev_selected %p", o, o->next_selected, o->prev_selected);
						goto out;
					} else {
						o = o->next_selected;
					}
				}
				
				if(p->next_selected){
					post("removepoint p %p p->next_selected %p", p, p->next_selected);
					p->next_selected->prev_selected = p->prev_selected;
				}
				if(p->prev_selected){
					post("removepoint p %p p->prev_selected %p", p, p->prev_selected);
					p->prev_selected->next_selected = p->next_selected;
				}
				if(p){
					free(p);
				}
					
					
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
		atom_setlong(&(out[1]), x->labelstart + i);
		while(p){
			atom_setlong(&(out[0]), point_num++);
			if(x->xres == ps_int){
				atom_setfloat(&(out[2]), round(p->coords.x));
			}else{
				atom_setfloat(&(out[2]), p->coords.x);
			}
			if(x->yres == ps_int){
				atom_setfloat(&(out[3]), round(p->coords.y));
			}else{
				atom_setfloat(&(out[3]), p->coords.y);
			}

			outlet_list(x->out_dump, NULL, 4, out);
			p = p->next;
		}
	}
	outlet_bang(x->out_dump);
}

void bpf_xminmax(t_bpf *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	bpf_invalidateAllPos(x);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_yminmax(t_bpf *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	bpf_invalidateAllPos(x);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_resizeRectWithMargins(t_bpf *x, t_rect *r){
	r->x = x->xmargin;
	r->width -= (x->xmargin * 2);
	r->y = x->ymargin;
	r->height -= (x->ymargin * 2);
}

void bpf_clearCMD(void *x, void *nothing, void *morenothing, void *evenmorenothing)
{
	bpf_clear((t_bpf *)x);

}

void bpf_clear(t_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		if(p){
			free(p);
		}
		x->functions[i] = NULL;
		x->monotonic_point_counter[i] = 0;
		x->npoints[i] = 0;
	}
	critical_enter(x->lock);
	x->numFunctions = 1;
	x->currentFunction = 0;
	critical_exit(x->lock);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	bpf_invalidateAllPos(x);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_invalidateAllPos(t_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		jbox_invalidate_layer((t_object *)x, x->pv, l_pos[i]);
	}
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

//--------->

void bpf_undo(t_bpf *x)
{
	t_logNode *u = bpf_getUndo(x);
	if(u){
		lfptr undoer = u->unF; //lfptr = log function pointer typedef, see above
		undoer(x, u->data1, u->data2, u->data3);
	} else {
		post("issue in getUndo");
	}
}

t_logNode *bpf_getUndo(t_bpf *x)
{
	t_logNode *c = x->logCursor;
	if(c->prev)
		x->logCursor = c->prev;
	
	if(c){
		post("undoing %p", c);
		return c;
	} else {
		post("nothing more to undo");
		return NULL;
	}
}

int bpf_historyLog(t_bpf *x, void *data1, void *data2, void *data3, lfptr unF)
{
	t_logNode *hq = x->history;
	t_logNode *c = x->logCursor;
	t_logNode *t = x->topNode;
	t_logNode *n = (t_logNode *)malloc(sizeof(t_logNode));
	n->data1 = data1;
	n->data2 = data2;
	n->data3 = data3;
	n->next = NULL;
	n->unF = unF;
	//*x->hsteps++; //add max history steps control
	
	//post("n->data %d", *(int *)n->data);
	
	if(hq->next == NULL){
		hq->next = n;
		hq->prev = hq;//hmm
		n->prev = hq;
		x->history = hq;
		x->topNode = n;
		x->logCursor = n;
//		post("1 {%p} n:%p next:%p prev:%p", n->pt_ptr, n, n->next, n->prev);	
	} else if(c == t){
		t->next = n;
		n->prev = t;
		x->topNode = n;
		x->logCursor = n;
//		post("2 {%p} n:%p next:%p prev:%p", n->pt_ptr, n, n->next, n->prev);
	} else if(c != t){
		
		t_logNode *temp;
		t_logNode *f = x->logCursor->next;
		while(f->next){
			temp = f;
//			post("freeing %p\n", f);
			if(f->data1)
				free(f->data1);
			if(f->data2)
				free(f->data2);
			if(f->data3)
				free(f->data3);
			if(f)
				free(f);
			f = temp->next;
		}
		
		c->next = n;
		n->prev = c;
		x->topNode = n;
		x->logCursor = n;
//		post("3 {%p} n:%p next:%p prev:%p", n->pt_ptr, n, n->next, n->prev);	
	}

	//bpf_printLog(x);
	
	return 1;
}

void bpf_printLog(t_bpf *x)
{
	t_logNode *h = x->history;
	if(h->next){
		while(h){
			post("t_point %p\n \t%p \t prev: \t%p \tnext: \t%p\n", h->data1, h, h->prev, h->next);
			h = h->next;
		}
		
		//print points
		int i;
		for(i = 0; i < x->numFunctions; i++){
			t_point *p = x->functions[i];
			t_point *next;
			while(p){
				next = p->next;
				post("t_point %p", p);
				p = next;
				next = next->next;
			}
		}
		
	} else {
		post("history clear\n");
	}
}

void bpf_clearHistory(t_bpf *x)
{
	t_logNode *h = x->history;
	t_logNode *next;
	if(h->next){
		h = h->next; //save x->history pointer
		while(h->next){
			next = h->next;
			if(h->data1)
				free(h->data1);
			if(h->data2)
				free(h->data2);
			if(h->data3)
				free(h->data3);

			free(h);
			h = next;
		}
		h->next = NULL;
		x->history = h;
	} else {
		post("history clear\n");
	}
}

//<---------



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
	
	//--------->
	t_logNode *l = x->history;
	t_logNode *nextL;

	while(l){
		nextL = l->next;
		if(l->data1)
			free(l->data1);
		if(l->data2)
			free(l->data2);
		if(l->data3)
			free(l->data3);
		//important that this is after above point freeing
		free(l);
		l = nextL;
	}

	//<---------
	
	for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
		if(x->ptrs[i]){
			sysmem_freeptr(x->ptrs[i]);
		}
	}
	if(x->ptrs){
		sysmem_freeptr(x->ptrs);
	}
	
} 

int main(void){ 
 	t_class *c = class_new("bpf", (method)bpf_new, (method)bpf_free, sizeof(t_bpf), 0L, A_GIMME, 0); 
	class_dspinitjbox(c);

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

	//---->
	class_addmethod(c, (method)bpf_undo, "undo", 0);
	class_addmethod(c, (method)bpf_printLog, "printLog", 0);
	class_addmethod(c, (method)bpf_clearHistory, "clearHistory", 0);	
	//<----
	
	class_addmethod(c, (method)bpf_dsp, "dsp", A_CANT, 0);
 	class_addmethod(c, (method)bpf_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)bpf_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_mouseup, "mouseup", A_CANT, 0); 
 	class_addmethod(c, (method)bpf_addFunction, "addfunction", A_GIMME, 0); 
 	class_addmethod(c, (method)bpf_functionList, "functionlist", A_GIMME, 0); 
 	class_addmethod(c, (method)bpf_setFunction, "setfunction", A_LONG, 0); 
	class_addmethod(c, (method)bpf_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)bpf_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)bpf_clear, "clear", 0);
	class_addmethod(c, (method)bpf_dump, "dump", 0);
	class_addmethod(c, (method)bpf_setFunctionName, "setfunctionname", A_SYM, 0);
	class_addmethod(c, (method)bpf_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)bpf_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);
	//class_addmethod(c, (method)bpf_renumber, "renumber", 0);
	class_addmethod(c, (method)bpf_zoomToFit, "zoomtofit", 0);
	class_addmethod(c, (method)bpf_clearSelectedAndRedraw, "unselect", 0);
	class_addmethod(c, (method)bpf_removeSelected, "deleteselected", 0);
	class_addmethod(c, (method)bpf_hideFunction, "hidefunction", A_GIMME, 0);

	CLASS_ATTR_SYM(c, "name", 0, t_bpf, name);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "name", 0, "0");

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_bpf, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_bpf, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_bpf, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_bpf, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "xmargin", 0, t_bpf, xmargin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmargin", 0, "5.0");

        CLASS_ATTR_DOUBLE(c, "ymargin", 0, t_bpf, ymargin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymargin", 0, "5.0");

	CLASS_ATTR_SYM(c, "xres", 0, t_bpf, xres);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xres", 0, "float");

	CLASS_ATTR_SYM(c, "yres", 0, t_bpf, yres);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "yres", 0, "float");

	CLASS_ATTR_LONG(c, "drawpos", 0, t_bpf, drawpos);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawpos", 0, "1");

	CLASS_ATTR_LONG(c, "drawlabels", 0, t_bpf, drawlabels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawlabels", 0, "1");

	CLASS_ATTR_LONG(c, "labelstart", 0, t_bpf, labelstart);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "labelstart", 0, "0");

	CLASS_ATTR_LONG(c, "lockx", 0, t_bpf, lockx);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lockx", 0, "0");

	CLASS_ATTR_LONG(c, "locky", 0, t_bpf, locky);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "locky", 0, "0");

	CLASS_ATTR_LONG(c, "lockinput", 0, t_bpf, lockinput);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lockinput", 0, "0");

	CLASS_ATTR_LONG(c, "step", 0, t_bpf, step);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "step", 0, "0");

	CLASS_ATTR_ATOM_VARSIZE(c, "points", 0, t_bpf, functions, numFunctions, 1024);
	CLASS_ATTR_ACCESSORS(c, "points", bpf_points_get, bpf_points_set);
	CLASS_ATTR_SAVE(c, "points", 0);

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

	l_background = gensym("l_background");
	l_points = gensym("l_points");
	l_pos = (t_symbol **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_symbol *));
	int i;
	for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
		char buf[16];
		sprintf(buf, "l_pos_%d", i);
		l_pos[i] = gensym(buf);
	}

	ps_int = gensym("int");
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
 		x->box.z_box.b_firstin = (void *)x; 
		dsp_setupjbox((t_pxjbox *)x, 1);

 		x->out_dump = listout((t_object *)x); 
		x->out_sel = listout((t_object *)x);
 		x->out_main = listout((t_object *)x); 

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *));
 		x->funcattr = (t_funcattr **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_funcattr *));
		x->pos = (t_pt *)calloc(MAX_NUM_FUNCTIONS, sizeof(t_pt));
		x->hideFunctions = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));

		x->monotonic_point_counter = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
		memset(x->monotonic_point_counter, '\0', MAX_NUM_FUNCTIONS * sizeof(int));

		x->npoints = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
		memset(x->npoints, '\0', MAX_NUM_FUNCTIONS * sizeof(int));

		x->selected = NULL;
		x->sel_box = (t_rect){-1, -1, 0, 0};

		x->xmargin = XMARGIN;
		x->ymargin = YMARGIN;

 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		
		//----->
		t_logNode *hq = (t_logNode *)calloc(1, sizeof(t_logNode));
		hq->data1 = NULL;
		hq->data2 = NULL;
		hq->data3 = NULL;
		hq->unF = &bpf_clearCMD; //storing base log point as clear method, always kept as node 0, new nodes are added above *hq
//		hq->doF = NULL;
		hq->next = NULL;
		hq->prev = NULL;
		
		x->history = hq;
		x->logCursor = hq;
		x->topNode = hq;
		
		x->origin = (t_point *)calloc(1, sizeof(t_point));
		x->mdrag = FALSE;
		//<-----
		
		
		critical_new(&(x->lock));

		int i;
		x->ptrs = (t_float **)sysmem_newptr(MAX_NUM_FUNCTIONS * sizeof(t_float *));
		for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
			x->funcattr[i] = (t_funcattr *)calloc(1, sizeof(t_funcattr));
			x->funcattr[i]->line_color = x->lineColor;
			x->funcattr[i]->point_color = x->pointColor;
			memset(x->funcattr[i]->dash, 0, 8);
			x->funcattr[i]->ndash = 0;
			sprintf(x->funcattr[i]->name, "%d", i);
			x->ptrs[i] = (t_float *)sysmem_newptr(2048 * sizeof(t_float));
		}

		x->name = NULL;
 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
		x->box.z_misc = Z_PUT_FIRST;

		
 		return x; 
 	} 
 	return NULL; 
} 


t_max_err bpf_points_get(t_bpf *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	int n = 0, i;
	long npoints[x->numFunctions];
	memset(npoints, '\0', x->numFunctions * sizeof(long));
	for(i = 0; i < x->numFunctions; i++){
		n += 19;
		t_point *p = x->functions[i];
		while(p){
			n += 2;
			npoints[i]++;
			p = p->next;
		}
	}
	atom_alloc_array(n, argc, argv, &alloc);
	t_atom *ptr = *argv;
	for(i = 0; i < x->numFunctions; i++){
		atom_setfloat(ptr++, x->funcattr[i]->line_color.red);
		atom_setfloat(ptr++, x->funcattr[i]->line_color.green);
		atom_setfloat(ptr++, x->funcattr[i]->line_color.blue);
		atom_setfloat(ptr++, x->funcattr[i]->line_color.alpha);

		atom_setfloat(ptr++, x->funcattr[i]->point_color.red);
		atom_setfloat(ptr++, x->funcattr[i]->point_color.green);
		atom_setfloat(ptr++, x->funcattr[i]->point_color.blue);
		atom_setfloat(ptr++, x->funcattr[i]->point_color.alpha);

		int j;
		for(j = 0; j < 8; j++){
			atom_setfloat(ptr++, x->funcattr[i]->dash[j]);
		}
		atom_setlong(ptr++, x->funcattr[i]->ndash);
		atom_setsym(ptr++, gensym(x->funcattr[i]->name));

		atom_setlong(ptr++, npoints[i]);
		t_point *p = x->functions[i];
		while(p){
			atom_setfloat(ptr++, p->coords.x);
			atom_setfloat(ptr++, p->coords.y);
			p = p->next;
		}
	}
	return MAX_ERR_NONE;
}

t_max_err bpf_points_set(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	t_atom *ptr = argv;

	x->numFunctions = 0;
	int cf = 0;
	while(ptr - argv < argc){
		x->numFunctions++;
		cf = x->numFunctions - 1;
		x->funcattr[cf]->line_color = (t_jrgba){atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++)};
		x->funcattr[cf]->point_color = (t_jrgba){atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++),atom_getfloat(ptr++)};
		int j;
		for(j = 0; j < 8; j++){
			x->funcattr[cf]->dash[j] = atom_getfloat(ptr++);
		}
		x->funcattr[cf]->ndash = atom_getlong(ptr++);
		strcpy(x->funcattr[cf]->name, atom_getsym(ptr++)->s_name);
		int npoints = atom_getlong(ptr++) + 1;
		x->npoints[cf] = npoints;
		while(--npoints){
			t_pt pt = (t_pt){atom_getfloat(ptr++), atom_getfloat(ptr++)};
			bpf_insertPoint(x, pt, cf, x->monotonic_point_counter[cf]++);
		}
	}
	return MAX_ERR_NONE;
}
