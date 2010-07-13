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
  NAME: dev_bpf 
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

#ifndef FLT_MAX
#define FLT_MAX 999999999.f
#endif

#ifdef WIN
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

#define MAX_NUM_FUNCTIONS 256 

#define XMARGIN 5
#define YMARGIN 5

#define MAX_HISTORY 100;

typedef struct _point{ 
 	t_pt coords;
	int id;
	int selected;
 	struct _point *next; 
 	struct _point *prev; 
 	struct _point *next_selected; 
 	struct _point *prev_selected; 
} t_point;

typedef struct	_logNode{
	t_symbol *sym;
	long ac;
	t_atom *av;
/*	
	t_point *insertedPoint;
	function *undoFuction;
	function *redoFunction;
*/	
	struct _logNode *next;
	struct _logNode *prev;
} t_logNode;

typedef struct _function_attributes{
	t_jrgba line_color, point_color;
	double dash[8];
	long ndash;
	char name[64];
} t_function_attributes, t_funcattr;

typedef struct _dev_bpf{ //object struct
 	t_jbox box; 
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
	t_symbol *x_res, *y_res;
	t_pt *pos;
	long drawpos, drawlabels;
	int *monotonic_point_counter;
	int *npoints;

	t_pt drag;
	
	long mdmods;
	
	t_rect sel_box;
	int sel_box_mouseover;
	t_jrgba sel_box_lineColor;
	t_pt sel_box_tempPts[256];
	int nTempPoints;
	
	t_logNode *history;
	t_logNode *topNode;
	t_logNode *currentNode;
	bool freshNode;
	long *hsteps;
	} t_dev_bpf; 

void *dev_bpf_class; 

//>
void dev_bpf_paint(t_dev_bpf *x, t_object *patcherview); 
void dev_bpf_list(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);
void dev_bpf_listInsert(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);

//<

void dev_bpf_float(t_dev_bpf *x, double f);
void dev_bpf_setPosition(t_dev_bpf *x, double f);
void dev_bpf_find_btn(t_dev_bpf *x, t_point *function, double xx, t_point **left, t_point **right);
t_point *dev_bpf_select(t_dev_bpf *x, t_pt p);
void dev_bpf_draw_bounds(t_dev_bpf *x, t_jgraphics *g, t_rect *rect);
void dev_bpf_dump(t_dev_bpf *x);
void dev_bpf_invalidateAllPos(t_dev_bpf *x);
void dev_bpf_assist(t_dev_bpf *x, void *b, long m, long a, char *s); 

//------>
void dev_bpf_free(t_dev_bpf *x); 
//<------

t_max_err dev_bpf_notify(t_dev_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void dev_bpf_clearSelectedAndRedraw(t_dev_bpf *x);
void dev_bpf_clearSelected(t_dev_bpf *x);

//------>
void dev_bpf_mousedown(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
void dev_bpf_mousedrag(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers);
void dev_bpf_mousemove(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers);
void dev_bpf_mouseup(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers); 
//<------

void dev_bpf_outputSelection(t_dev_bpf *x);
void dev_bpf_addFunction(t_dev_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void dev_bpf_functionList(t_dev_bpf *x, t_symbol *msg, int argc, t_atom *argv); 
void dev_bpf_setFunction(t_dev_bpf *x, long f); 
void dev_bpf_setFunctionName(t_dev_bpf *x, t_symbol *name);
//void dev_bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
//void dev_bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
//void dev_bpf_renumber(t_dev_bpf *x);
void dev_bpf_zoomToFit(t_dev_bpf *x);

//-->?
t_point *dev_bpf_insertPoint(t_dev_bpf *x, t_pt coords, int functionNum, int id);
//<

void dev_bpf_removeSelected(t_dev_bpf *x);
void dev_bpf_removePoint(t_dev_bpf *x, t_point *point, int functionNum);
void dev_bpf_clear(t_dev_bpf *x);
double dev_bpf_scale(double f, double min_in, double max_in, double min_out, double max_out);
void dev_bpf_xminmax(t_dev_bpf *x, double min, double max);
void dev_bpf_yminmax(t_dev_bpf *x, double min, double max);
void dev_bpf_resizeRectWithMargins(t_rect *r);
//void dev_bpf_read(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void dev_bpf_doread(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void dev_bpf_write(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void dev_bpf_dowrite(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);
int main(void); 

//>
void *dev_bpf_new(t_symbol *s, long argc, t_atom *argv); 
//<

t_max_err dev_bpf_points_get(t_dev_bpf *x, t_object *attr, long *argc, t_atom **argv);

//>
t_max_err dev_bpf_points_set(t_dev_bpf *x, t_object *attr, long argc, t_atom *argv);
//<

//------
bool lineSegmentIntersection(t_dev_bpf *x, double Ax, double Ay,double Bx, double By,double Cx, double Cy,double Dx, double Dy,double *X, double *Y);
void dev_bpf_keyInput(t_dev_bpf *x, long k);
void dev_bpf_historyLog(t_dev_bpf *x, t_symbol *s, long argc, t_atom *argv);
void dev_bpf_clearHistory(t_dev_bpf *x);
t_logNode *dev_bpf_getUndo(t_dev_bpf *x);
t_logNode *dev_bpf_getRedo(t_dev_bpf *x);
void dev_bpf_doUndoRedo(t_dev_bpf *x, t_logNode *n);
void dev_bpf_listRemove(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv);


void dev_bpf_anythingPipe(t_dev_bpf *x, t_symbol *s, long argc, t_atom *argv);

//-----


void myobject_write(t_dev_bpf *x, t_symbol *s);
void myobject_dowrite(t_dev_bpf *x, t_symbol *s);
void myobject_writefile(t_dev_bpf *x, char *filename, short path);

t_symbol *l_background, *l_points, **l_pos, *ps_int; //why are these global variables?


/////// main
int main(void){ 
 	t_class *c = class_new("dev_bpf", (method)dev_bpf_new, (method)dev_bpf_free, sizeof(t_dev_bpf), 0L, A_GIMME, 0); 

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

 	class_addmethod(c, (method)dev_bpf_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)dev_bpf_assist, "assist", A_CANT, 0); 
	class_addmethod(c, (method)dev_bpf_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)dev_bpf_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)dev_bpf_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)dev_bpf_mouseup, "mouseup", A_CANT, 0); 
	class_addmethod(c, (method)dev_bpf_mousemove, "mousemove", A_CANT, 0); 

	class_addmethod(c, (method)dev_bpf_anythingPipe, "anything", A_GIMME, 0);

	//list and float methodes redirect to pipe:
	class_addmethod(c, (method)dev_bpf_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)dev_bpf_float, "float", A_FLOAT, 0);
/*	
	class_addmethod(c, (method)dev_bpf_addFunction, "addFunction", A_GIMME, 0); 
 	class_addmethod(c, (method)dev_bpf_functionList, "functionList", A_GIMME, 0); 
 	class_addmethod(c, (method)dev_bpf_setFunction, "setFunction", A_LONG, 0); 
	class_addmethod(c, (method)dev_bpf_clear, "clear", 0);
	class_addmethod(c, (method)dev_bpf_dump, "dump", 0);
	class_addmethod(c, (method)dev_bpf_setFunctionName, "setFunctionName", A_SYM, 0);
	class_addmethod(c, (method)dev_bpf_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)dev_bpf_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);
	//class_addmethod(c, (method)dev_bpf_renumber, "renumber", 0);
	class_addmethod(c, (method)dev_bpf_zoomToFit, "zoomtofit", 0);
	class_addmethod(c, (method)dev_bpf_clearSelectedAndRedraw, "unselect", 0);
	class_addmethod(c, (method)dev_bpf_removeSelected, "deleteselected", 0);

	class_addmethod(c, (method)dev_bpf_keyInput, "key", A_LONG, 0);
*/
	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_dev_bpf, xmin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

	CLASS_ATTR_DOUBLE(c, "xmax", 0, t_dev_bpf, xmax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "ymin", 0, t_dev_bpf, ymin);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

	CLASS_ATTR_DOUBLE(c, "ymax", 0, t_dev_bpf, ymax);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_SYM(c, "x_res", 0, t_dev_bpf, x_res);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "x_res", 0, "float");

	CLASS_ATTR_SYM(c, "y_res", 0, t_dev_bpf, y_res);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "y_res", 0, "float");

	CLASS_ATTR_LONG(c, "drawpos", 0, t_dev_bpf, drawpos);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawpos", 0, "1");

	CLASS_ATTR_LONG(c, "drawlabels", 0, t_dev_bpf, drawlabels);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "drawlabels", 0, "1");

	CLASS_ATTR_ATOM_VARSIZE(c, "points", 0, t_dev_bpf, functions, numFunctions, 1024);
	CLASS_ATTR_ACCESSORS(c, "points", dev_bpf_points_get, dev_bpf_points_set);
	CLASS_ATTR_SAVE(c, "points", 0);

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_dev_bpf, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointColor", 0, t_dev_bpf, pointColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointColor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "lineColor", 0, t_dev_bpf, lineColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lineColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "lineColor", 0, "rgba", "Line Color"); 

 	CLASS_ATTR_RGBA(c, "bgFuncColor", 0, t_dev_bpf, bgFuncColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgFuncColor", 0, "0. 0. 0. 0.5"); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgFuncColor", 0, "rgba", "Background Function Color"); 

 	CLASS_ATTR_RGBA(c, "selectionColor", 0, t_dev_bpf, selectionColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectionColor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectionColor", 0, "rgba", "Selection Color"); 

 	CLASS_ATTR_RGBA(c, "textColor", 0, t_dev_bpf, textColor); 
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
 	dev_bpf_class = c; 

 	version(0); 
	
 	return 0; 
} 

void dev_bpf_anythingPipe(t_dev_bpf *x, t_symbol *s, long argc, t_atom *argv)
{
	x->freshNode = TRUE;
	t_atom *ap = argv;
	if(s == gensym("addFunction"))
		dev_bpf_addFunction(x, s, argc, argv);
	if(s == gensym("functionList"))
		dev_bpf_functionList(x, s, argc, argv);
	if(s == gensym("setFunction"))
		dev_bpf_setFunction(x, atom_getlong(ap));
	if(s == gensym("list"))
		dev_bpf_listInsert(x, s, argc, argv);
	if(s == gensym("float"))
		dev_bpf_setPosition(x, atom_getfloat(ap));
	if(s == gensym("clear"))
		dev_bpf_clear(x);
	if(s == gensym("dump"))
		dev_bpf_dump(x);
	if(s == gensym("setFunctionName"))
		dev_bpf_setFunctionName(x, atom_getsym(ap));
	if(s == gensym("xminmax"))
		dev_bpf_xminmax(x, atom_getfloat(ap), atom_getfloat(ap+1));
	if(s == gensym("yminmax"))
		dev_bpf_yminmax(x, atom_getfloat(ap), atom_getfloat(ap+1));
/*	if(s == gensym("renumber"))
		dev_bpf_renumber(x); */
	if(s == gensym("zoomtofit"))
		dev_bpf_zoomToFit(x);
	if(s == gensym("unselect"))
		dev_bpf_clearSelectedAndRedraw(x);
	if(s == gensym("deleteselected"))
		dev_bpf_removeSelected(x);
	if(s == gensym("key"))
		dev_bpf_keyInput(x, atom_getlong(ap));
	if(s == gensym("clearhistory"))
		dev_bpf_clearHistory(x);
	
	
}

//////////////////////////////////functions
void dev_bpf_paint(t_dev_bpf *x, t_object *patcherview){ 
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

	g = jbox_start_layer((t_object *)x, patcherview, l_points, r.width, r.height);
	dev_bpf_resizeRectWithMargins(&r);

 	if(g){ 
		critical_enter(x->lock);
		int i, j;
		for(i = 0; i < x->numFunctions; i++){
			j = 0;
			t_point *p = x->functions[i];
			if(p){
//				post("j = %d, p->cords.x = %f -- p->cords.y = %f (first)\n", j, p->coords.x, p->coords.y);
				t_pt sc;
				if(x->x_res == ps_int){
					sc.x = dev_bpf_scale(round(p->coords.x), x->xmin, x->xmax, r.x, r.width);
				}else{
					sc.x = dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				}
				if(x->y_res == ps_int){
					sc.y = dev_bpf_scale(round(p->coords.y), x->ymin, x->ymax, r.height, r.y);
				}else{
					sc.y = dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
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
				
				p = p->next; //increment to next t_point (each t_point has a next t_point (last is NULL?))
			}
			while(p){
//				post("j = %d, p->cords.x = %f -- p->cords.y = %f\n", j, p->coords.x, p->coords.y);

				t_pt sc;
				if(x->x_res == ps_int){
					sc.x = dev_bpf_scale(round(p->coords.x), x->xmin, x->xmax, r.x, r.width);
				}else{
					sc.x = dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				}
				if(x->y_res == ps_int){
					sc.y = dev_bpf_scale(round(p->coords.y), x->ymin, x->ymax, r.height, r.y);
				}else{
					sc.y = dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
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
				if(x->x_res == ps_int){
					px = round(px);
				}
				if(x->y_res == ps_int){
					py = round(py);
				}
				jgraphics_move_to(g, dev_bpf_scale(px, x->xmin, x->xmax, r.x, r.width), dev_bpf_scale(py, x->ymin, x->ymax, r.height, r.y));
				jgraphics_line_to(g, sc.x, sc.y);
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
		/*
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
		*/
		

/*
		jgraphics_set_source_rgba(g, 0.5, 0.5, 0.5, 0.1);
		jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
		jgraphics_fill(g);
		
		jgraphics_set_source_jrgba(g, &(x->sel_box_lineColor));
		jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
		jgraphics_stroke(g);
*/	
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
	//-----
	//sel_box paint
	if(fabs(x->sel_box.width)>0){
		g = jbox_start_layer((t_object *)x, patcherview, gensym("sel_box"), r.width, r.height);

		if(g){
			jgraphics_set_source_rgba(g, 0.5, 0.5, 0.5, 0.1);
			jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
			jgraphics_fill(g);
			
			jgraphics_set_source_jrgba(g, &(x->sel_box_lineColor));
			jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
			jgraphics_stroke(g);
			
			if(x->nTempPoints && x->sel_box_mouseover){
				jgraphics_set_source_rgba(g, 1., 0., 0., 0.8);
				int i;
				for(i=0; i< x->nTempPoints; i++){
					jgraphics_ellipse(g, x->sel_box_tempPts[i].x - 2, x->sel_box_tempPts[i].y - 2, 4, 4);
					jgraphics_fill(g);
				}

			}
			jbox_end_layer((t_object *)x, patcherview, gensym("sel_box"));
			jbox_paint_layer((t_object *)x, patcherview, gensym("sel_box"), r.x, r.y);
		}
	}

} 

void dev_bpf_float(t_dev_bpf *x, double f){
	t_atom fa;
	atom_setfloat(&fa, f);
	dev_bpf_anythingPipe(x, gensym("float"), 1, &fa);
}

void dev_bpf_setPosition(t_dev_bpf *x, double f){
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	dev_bpf_resizeRectWithMargins(&r);
	//r.x = r.y = 0;
	t_atom out[2]; // function number, y
	int i;
	for(i = 0; i < x->numFunctions; i++){
		x->pos[i] = (t_pt){-1., -1.};
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		t_point *left = NULL, *right = NULL;
		dev_bpf_find_btn(x, p, f, &left, &right);
		if(!left || !right){
			atom_setfloat(&(out[1]), 0.);
			outlet_list(x->out_main, NULL, 2, out);
		}else{
			double lx = left->coords.x, ly = left->coords.y;
			double rx = right->coords.x, ry = right->coords.y;
			if(x->x_res == ps_int){
				lx = round(lx);
				rx = round(rx);
			}
			if(x->y_res == ps_int){
				ly = round(ly);
				ry = round(ry);
			}
			double m = (ry - ly) / (rx - lx); //slope
			double b = ly - (m * lx); //line
			double y = (m * f) + b; //y point on line
			atom_setfloat(&(out[1]), y);
			//x->pos[function number] = {scaled(x float input), scaled(y point on function)}
			x->pos[i] = (t_pt){dev_bpf_scale(f, x->xmin, x->xmax, r.x, r.width) + XMARGIN, dev_bpf_scale(y, x->ymin, x->ymax, r.height, r.y) + YMARGIN};
			if(x->drawpos){
				jbox_invalidate_layer((t_object *)x, x->pv, l_pos[i]);
			}
			outlet_list(x->out_main, NULL, 2, out);
		}
	}
	if(x->drawpos){
		jbox_redraw((t_jbox *)(&x->box));
	}
}

void dev_bpf_find_btn(t_dev_bpf *x, t_point *function, double xx, t_point **left, t_point **right){
	//post("function = %p", function);
	if(!function){
		return;
	}
	t_point *ptr = function;
	if(x->x_res == ps_int){
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

t_point *dev_bpf_select(t_dev_bpf *x, t_pt p_sc){
	double min = 1000000000.;
	t_point *min_ptr = NULL;
	t_point *ptr = x->functions[x->currentFunction];
	double xdif, ydif;
	t_rect r;
	jbox_get_patching_rect(&(x->box.b_ob), &r);
	dev_bpf_resizeRectWithMargins(&r);
	//r.x = r.y = 0;

	//t_pt p_sc;
	//p_sc.x = dev_bpf_scale(p.x, x->xmin, x->xmax, r.x, r.width);
	//p_sc.y = dev_bpf_scale(p.y, x->ymin, x->ymax, r.height, r.y);
	/*
	if(x->x_res == ps_int){
		p_sc.x = round(p_sc.x);
	}
	if(x->y_res == ps_int){
		p_sc.y = round(p_sc.y);
	}
	*/
	while(ptr){
		t_pt sc = ptr->coords;
		if(x->x_res == ps_int){
			sc.x = round(sc.x);
		}
		if(x->y_res == ps_int){
			sc.y = round(sc.y);
		}

		sc.x = dev_bpf_scale(sc.x, x->xmin, x->xmax, r.x, r.width);
		sc.y = dev_bpf_scale(sc.y, x->ymin, x->ymax, r.height, r.y);

//		post("click: %f %f, point: %f %f", p_sc.x, p_sc.y, sc.x, sc.y);
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

t_max_err dev_bpf_notify(t_dev_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	//t_symbol *attrname; 
 	if (msg == gensym("attr_modified")){ 
 		t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		/*
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
		*/
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
		jbox_invalidate_layer((t_object *)x, x->pv, l_background);
		jbox_invalidate_layer((t_object *)x, x->pv, l_points);
 		jbox_redraw(&(x->box)); 
	} 
	return 0; 
} 

void dev_bpf_clearSelectedAndRedraw(t_dev_bpf *x){
	dev_bpf_clearSelected(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box)); 
}

void dev_bpf_clearSelected(t_dev_bpf *x){
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
void dev_bpf_mousedown(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers)
{ 
	x->mdmods = modifiers;
 //	post("0x%X", modifiers);
	x->freshNode = TRUE;
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	dev_bpf_resizeRectWithMargins(&r);
	pt.x -= XMARGIN; //mouse position
	pt.y -= YMARGIN;
	x->drag = pt; //set drag start point for mouse delta
	//r.x = r.y = 0;
	t_pt coords;
	coords.x = dev_bpf_scale(pt.x, r.x, r.width, x->xmin, x->xmax);
	coords.y = dev_bpf_scale(pt.y, r.height, r.y, x->ymin, x->ymax);

//	t_modifiers mod = jkeyboard_getcurrentmodifiers();

	switch(modifiers){
		case 0x110:
		case 0x10:
		{
			// no modifiers.  
			t_point *p;
			t_point *s = x->selected;
			while(s){									//while a point is selected, update it's coordinates (moved mouse? or reselecting selected)
				double sx = s->coords.x, sy = s->coords.y;
				if(x->x_res == ps_int){
					sx = round(sx);
					pt.x = round(pt.x);
				}
				if(x->y_res == ps_int){
					sy = round(sy);
					pt.y = round(pt.y);
				}
				if(abs(dev_bpf_scale(sx, x->xmin, x->xmax, r.x, r.width) - pt.x) < 3 && abs(dev_bpf_scale(sy, x->ymin, x->ymax, r.height, r.y) - pt.y) < 3){
					break;
				}
				s = s->next_selected;
			}
			if(s){
				break;
			}else{									//if none selected
				dev_bpf_clearSelected(x);			//clear all selections?
				if(p = dev_bpf_select(x, pt)){		//make p current mouse point
					p->selected = 1;				//set point as selected
					x->selected = p;				//set struct as selected
				}
				if(p){			//if _select returns a point, it exsists
					break;		//break switch
				}
			}
								//otherwise, make a new point:
			//NOTE: insert point uses coords while remove point uses pt... maybe make these the same for history cmds?
			p = dev_bpf_insertPoint(x, coords, x->currentFunction, x->monotonic_point_counter[x->currentFunction]++);
	//		post("%f %f", coords.x, coords.y);
			p->selected = 1;
			if(x->selected){
				x->selected->prev_selected = p;
			}
			p->next_selected = x->selected;
			p->prev_selected = NULL;
			x->selected = p;
			
			//history log new point
			t_atom logPt[3];
			atom_setlong(&logPt[0], x->currentFunction);
			atom_setfloat(&logPt[1], pt.x);
			atom_setfloat(&logPt[2], pt.y);
			dev_bpf_historyLog(x, gensym("insertPoint"), 3, logPt);
//			post("coords %f %f", x->hsteps, atom_getfloat(&logPt[0]), atom_getfloat(&logPt[1]));
//			post("pt %f %f", pt.x, pt.y);
			
		}
			break;
		case 0x112:
		case 0x12:
		{ // shift
			t_point *p;
			if(p = dev_bpf_select(x, pt)){
				dev_bpf_removePoint(x, p, x->currentFunction);
				post("%f %f", pt.x, pt.y);
				break;
			}
		}
			break;
		case 0x113:
		case 0x13:
		{ //0x13 = shift + cmd
			//need to clean this up maybe---->
			t_pt tempPoints[256];
			critical_enter(x->lock);
			memcpy(x->sel_box_tempPts, tempPoints, 256 * sizeof(t_pt));
			critical_exit(x->lock);
			x->nTempPoints = 0;
			//<-------
			x->sel_box.x = pt.x;
			x->sel_box.y = pt.y;
			x->sel_box.width = 0;
			x->sel_box.height = 0;
		}
			break;
	}
	dev_bpf_outputSelection(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box));
}

void dev_bpf_mousedrag(t_dev_bpf *x, t_object *patcherview, t_pt pt, long ignoreDragMods){

	t_rect r; //r =  patcher rect
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	dev_bpf_resizeRectWithMargins(&r);
	pt.x -= XMARGIN;
	pt.y -= YMARGIN;
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
	sc.x = dev_bpf_scale(pt.x, r.x, r.width, x->xmin, x->xmax);
	sc.y = dev_bpf_scale(pt.y, r.height, r.y, x->ymin, x->ymax);

	long mods = x->mdmods;
	switch(mods){
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
				t_pt pp = (t_pt){dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
				pp.x += (pt.x - x->drag.x);
				pp.y += (pt.y - x->drag.y);
				pp.x = dev_bpf_scale(pp.x, r.x, r.width, x->xmin, x->xmax);
				pp.y = dev_bpf_scale(pp.y, r.height, r.y, x->ymin, x->ymax);

				dev_bpf_removePoint(x, p, x->currentFunction);
				p = dev_bpf_insertPoint(x, pp, x->currentFunction, id);
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
		case 0x11: //cmd click
		{
			t_point *p = x->selected;
			t_point *next, *prev;
			if(x->nTempPoints && x->sel_box_mouseover){ //if there are temp points on box parimeter, create new points and select them
				int i;
				for(i=0; i<x->nTempPoints; i++){
					t_pt newp;
					newp.x = dev_bpf_scale(x->sel_box_tempPts[i].x, r.x, r.width, x->xmin, x->xmax);
					newp.y = dev_bpf_scale(x->sel_box_tempPts[i].y, r.height, r.y, x->ymin, x->ymax);
//					post("%f %f", newp.x, newp.y);
					p = dev_bpf_insertPoint(x, newp, x->currentFunction, x->monotonic_point_counter[x->currentFunction]++);
					p->selected = 1;
					p->next_selected = x->selected;
					if(x->selected){
						x->selected->prev_selected = p;
					}
					x->selected = p;
				}
				x->nTempPoints = 0;
			}
			
			//translate selected points per mouse delta
			double mdeltax = pt.x - x->drag.x;
			double mdeltay = pt.y - x->drag.y;
			
			t_pt pp = (t_pt){dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
			pp.x += mdeltax; //(mouse delta)
			pp.y += mdeltay;
			pp.x = dev_bpf_scale(pp.x, r.x, r.width, x->xmin, x->xmax);
			pp.y = dev_bpf_scale(pp.y, r.height, r.y, x->ymin, x->ymax);
			
			x->sel_box.x += mdeltax;
			x->sel_box.y += mdeltay;

			
			while(p){
				next = p->next_selected;
				prev = p->prev_selected;
				int id = p->id;
				int head = 0;
				head = (p == x->selected);
				t_pt pp = (t_pt){dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
				pp.x += (pt.x - x->drag.x);
				pp.y += (pt.y - x->drag.y);
				pp.x = dev_bpf_scale(pp.x, r.x, r.width, x->xmin, x->xmax);
				pp.y = dev_bpf_scale(pp.y, r.height, r.y, x->ymin, x->ymax);
				
				dev_bpf_removePoint(x, p, x->currentFunction);
				p = dev_bpf_insertPoint(x, pp, x->currentFunction, id);
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
		case 0x12: //shift
			break;
		case 0x113:
		case 0x13: //shift+cmd
			x->sel_box.width = pt.x - x->sel_box.x;
			x->sel_box.height = pt.y - x->sel_box.y;
			//post("box width = %f, box height = %f", x->sel_box.width, x->sel_box.height);
			t_pt rPt[4];
			rPt[0] = (t_pt){x->sel_box.x, x->sel_box.y};
			rPt[1] = (t_pt){x->sel_box.x, x->sel_box.y+x->sel_box.height};
			rPt[2] = (t_pt){x->sel_box.x+x->sel_box.width, x->sel_box.y+x->sel_box.height};
			rPt[3] = (t_pt){x->sel_box.x+x->sel_box.width, x->sel_box.y};	
			
			//int i;
			/*
			for(i=0; i<4; i++)
				post("rPt[%d] = {%f,%f}", i, rPt[i].x, rPt[i].y);
			*/
			double left = x->sel_box.width > 0 ? x->sel_box.x : x->sel_box.x + x->sel_box.width;
			double right = x->sel_box.width > 0 ? x->sel_box.x + x->sel_box.width : x->sel_box.x;
			double top = x->sel_box.height > 0 ? x->sel_box.y : x->sel_box.y + x->sel_box.height;
			double bottom = x->sel_box.height > 0 ? x->sel_box.y + x->sel_box.height : x->sel_box.y;
			
			dev_bpf_clearSelected(x);
			int tpCount = 0;
			t_pt tpArray[256];
			t_point *p = x->functions[x->currentFunction];
			while(p){
				t_pt coords;
				coords.x = dev_bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				coords.y = dev_bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
				if(coords.x > left && coords.x < right && coords.y < bottom && coords.y > top){ //if p->coords are within box select them
					p->selected = 1;
					p->next_selected = x->selected;
					if(x->selected){
						x->selected->prev_selected = p;
					}
					x->selected = p;
				}

				if(p->next){
					t_pt nextCoords;
					nextCoords.x = dev_bpf_scale(p->next->coords.x, x->xmin, x->xmax, r.x, r.width);
					nextCoords.y = dev_bpf_scale(p->next->coords.y, x->ymin, x->ymax, r.height, r.y);
					t_pt tp;
					long ii;
					for(ii=0; ii<4; ii++){
						/*post("ax %f, ay %f, bx %f, by %f, cx 5f, cy %f, dx %f, dy %f", 
						 coords.x, coords.y, nextCoords.x, nextCoords.y, rPt[ii].x, rPt[ii].y, rPt[(ii+1)%4].x, rPt[(ii+1)%4].y);
						 */
						bool cPt = lineSegmentIntersection(x, coords.x, coords.y, nextCoords.x, nextCoords.y,
															   rPt[ii].x, rPt[ii].y, rPt[(ii+1)%4].x, rPt[(ii+1)%4].y, &tp.x, &tp.y); 
						if(cPt == TRUE){
							tpArray[tpCount] = tp;
							//post("intersection {%2.f, %2.f}", tpArray[tpCount].x, tpArray[tpCount].y, tpCount);
							tpCount++;
						}
							 
					}
				}
				p = p->next;
			}
			if(tpArray){
				critical_enter(x->lock);
				memcpy(x->sel_box_tempPts, tpArray, tpCount * sizeof(t_pt)); //tempPts are prescaled
				
				x->nTempPoints = tpCount;
				critical_exit(x->lock);
			}
			//post(" point count = %d", x->nTempPoints);
			//post("sel_box_tempPts[0].coords.x = %f", x->sel_box_tempPts[0].x);
			break;
	}
	x->drag = pt;
	dev_bpf_outputSelection(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_invalidate_layer((t_object *)x, x->pv, gensym("sel_box"));
//	dev_bpf_invalidateAllPos(x);
	jbox_redraw(&(x->box));
}

void dev_bpf_mousemove(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers)
{
	if(fabs(x->sel_box.width) > 0){
		double left = x->sel_box.width > 0 ? x->sel_box.x : x->sel_box.x + x->sel_box.width;
		double right = x->sel_box.width > 0 ? x->sel_box.x + x->sel_box.width : x->sel_box.x;
		double top = x->sel_box.height > 0 ? x->sel_box.y : x->sel_box.y + x->sel_box.height;
		double bottom = x->sel_box.height > 0 ? x->sel_box.y + x->sel_box.height : x->sel_box.y;
		
		if(pt.x > left + XMARGIN && pt.x < right + XMARGIN && pt.y < bottom + YMARGIN && pt.y > top + YMARGIN){
			//post("over sel_box");
			x->sel_box_mouseover = 1;
			x->sel_box_lineColor = (t_jrgba){1., 0., 0., 1.};
//			jrgba_set(&(x->sel_box_lineColor), 1., 0., 0., 0.2);
		} else {
			//post("not over sel_box");
			x->sel_box_mouseover = 0;
			x->sel_box_lineColor = (t_jrgba){0.5, 0.5, 0.5, 0.2};
		}
		jbox_invalidate_layer((t_object *)x, x->pv, gensym("sel_box"));
		dev_bpf_invalidateAllPos(x);
		jbox_redraw(&(x->box));
	}

							  
}


bool lineSegmentIntersection(t_dev_bpf *x, double Ax, double Ay,double Bx, double By,double Cx, double Cy,double Dx, double Dy,double *X, double *Y) {
	//  public domain function by Darel Rex Finley, 2006
	//need to fix rounding errors
	
	double  distAB, theCos, theSin, newX, ABpos ;
	
	//  Fail if either line segment is zero-length.
	if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return FALSE;
	
	//  Fail if the segments share an end-point.
	if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
		||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
    return FALSE; }
	
	//  (1) Translate the system so that point A is on the origin.
	Bx-=Ax; By-=Ay;
	Cx-=Ax; Cy-=Ay;
	Dx-=Ax; Dy-=Ay;
	
	//  Discover the length of segment A-B.
	distAB=sqrt(Bx*Bx+By*By);
	
	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos=Bx/distAB;
	theSin=By/distAB;
	newX=Cx*theCos+Cy*theSin;
	Cy  =Cy*theCos-Cx*theSin; Cx=newX;
	newX=Dx*theCos+Dy*theSin;
	Dy  =Dy*theCos-Dx*theSin; Dx=newX;
	
	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.) return FALSE;
	
	//  (3) Discover the position of the intersection point along line A-B.
	ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
	
	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos<0. || ABpos>distAB) return FALSE;
	
	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	*X=Ax+ABpos*theCos;
	*Y=Ay+ABpos*theSin;
	
	//  Success.
	return TRUE;
} 


void dev_bpf_mouseup(t_dev_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_invalidate_layer((t_object *)x, x->pv, gensym("sel_box"));
	jbox_redraw(&(x->box));
}

void dev_bpf_outputSelection(t_dev_bpf *x){
	if(!(x->selected)){
		return;
	}
	t_point *p = x->selected;
	t_atom out[3];
	while(p){
		atom_setlong(&(out[0]), x->currentFunction);
		if(x->x_res == ps_int){
			atom_setfloat(&(out[1]), round(x->selected->coords.x));
		}else{
			atom_setfloat(&(out[1]), x->selected->coords.x);
		}
		if(x->y_res == ps_int){
			atom_setfloat(&(out[2]), round(x->selected->coords.y));
		}else{
			atom_setfloat(&(out[2]), x->selected->coords.y);
		}
		outlet_list(x->out_sel, NULL, 3, out);
		p = p->next_selected;
	}
}

void dev_bpf_addFunction(t_dev_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	critical_enter(x->lock);
	if(x->numFunctions + 1 > MAX_NUM_FUNCTIONS){
		object_error((t_object *)x, "dev_bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	x->numFunctions++;
	critical_exit(x->lock);
	dev_bpf_setFunction(x, x->numFunctions - 1);
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
	jbox_redraw(&(x->box));
}

void dev_bpf_functionList(t_dev_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc > MAX_NUM_FUNCTIONS){
		object_error((t_object *)x, "dev_bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
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
	jbox_redraw(&(x->box));
}

void dev_bpf_setFunction(t_dev_bpf *x, long f){
	if(f > MAX_NUM_FUNCTIONS){
		error("dev_bpf: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	if(f < 0){
		error("dev_bpf: function number must be positive");
		return;
	}
	if(f > x->numFunctions - 1){
		while(f > x->numFunctions - 1){
			x->numFunctions++;
		}
	}
	critical_enter(x->lock);
	x->currentFunction = f;
	dev_bpf_clearSelected(x);
	critical_exit(x->lock);
        jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box));
}

void dev_bpf_setFunctionName(t_dev_bpf *x, t_symbol *name){
	strncpy(x->funcattr[x->currentFunction]->name, name->s_name, 64);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box));
}


/*
void dev_bpf_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = fabs(r.height - screen_coords.y) / r.height;
}

void dev_bpf_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords){
	screen_coords->x = norm_coords.x * r.width;
	screen_coords->y = fabs((norm_coords.y * r.height) - r.height);
}
*/
/*
void dev_bpf_renumber(t_dev_bpf *x){
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
	jbox_redraw((t_jbox *)&x->box);
}
*/

void dev_bpf_zoomToFit(t_dev_bpf *x){
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
	jbox_redraw((t_jbox *)&(x->box));
}

t_point *dev_bpf_insertPoint(t_dev_bpf *x, t_pt coords, int functionNum, int id){
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
	}else if(p->coords.x < (*function)->coords.x){ //if the new point is left of the first point, it becomes the first
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
			if(p->coords.x >= current->coords.x && p->coords.x <= next->coords.x){ //determine point placement (if it is between this one and the next one)
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

void dev_bpf_removeSelected(t_dev_bpf *x){
	t_point *p = x->selected, *next;
	while(p){
		next = p->next_selected;
		dev_bpf_removePoint(x, p, x->currentFunction);
		p = next;
	}
	x->selected = NULL;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box));
}

void dev_bpf_removePoint(t_dev_bpf *x, t_point *point, int functionNum){
	if(!point){
		return;
	}
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = *function;
	int i = 0;
	x->npoints[functionNum]--;
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


void dev_bpf_keyInput(t_dev_bpf *x, long k){
	
	if(k == 'c'){
	post("c:%p next:%p prev:%p", x->currentNode, x->currentNode->next, x->currentNode->prev);
	post("t:%p next:%p prev:%p", x->topNode, x->topNode->next, x->topNode->prev);	
	}
	//bool store = TRUE;
	long mod = jkeyboard_getcurrentmodifiers();
	//	post("mod 0x%X key %c", mod, k);
	//0x1 cmd
	//0x2 shift
	//0x3 cmd+shift
	//0x8 alt
	//0xA alt+shift
	//0x84 ctrl
	//0x86 ctrl+shift
	
	switch (mod) {
		case 0xA:
			post("hello alt key and %d", k);
			break;
		case 0x1: //cmd
			if(k == 'z'){
				t_logNode *n = dev_bpf_getUndo(x);
				dev_bpf_doUndoRedo(x, n);
			}
			if(k == 'y')
				;
			if(k == 'p'){
//				t_logNode *n = x->topNode;
				t_logNode *h = x->history;
				if(h->next){
					//int i;
					while(h){
						post("%p \t prev: \t%p \tnext: \t%p", h, h->prev, h->next);
						/*if(n->ac){
							for(i=0; i < n->ac; i++){
								if(atom_gettype(&n->av[i]) == A_FLOAT)
									post("%d %f",i,atom_getfloat(&n->av[i]));
								if(atom_gettype(&n->av[i]) == A_LONG)
									post("%d %d",i,atom_getfloat(&n->av[i]));
							}
						}*/
						h = h->next;
					}
				} else {
					post("history clear");
				}
				//store = FALSE;
			}
			
			break;
		case 0x3://cmd+shift
			if(k == 'z'){
				t_logNode *n = dev_bpf_getRedo(x);
				dev_bpf_doUndoRedo(x, n);
			}
			break;
		default:
			break;
	}
	
	if(k == -7)
		dev_bpf_removeSelected(x);
	if(k == -3){
		post("esc button");
		x->sel_box = (t_rect){0., 0., 0., 0.};
		dev_bpf_clearSelected(x);
		jbox_invalidate_layer((t_object *)x, x->pv, l_points);
		jbox_invalidate_layer((t_object *)x, x->pv, gensym("sel_box"));
		jbox_redraw(&(x->box)); 
	}
	
	
	/*
	 t_atom keyNum;
	 atom_setlong(&keyNum, k);
	 if(store)
	 dev_bpf_historyLog(x, "keyInput", 1, &keyNum);
	 */
	//double *keyNum = (double *)k;
	//if(store)
	//	dev_bpf_historyLog(x, "keyInput", 1, keyNum);
	
}
/////////////////////

void dev_bpf_clearHistory(t_dev_bpf *x)
{
	critical_enter(x->lock);
	t_logNode *t = x->topNode;
	t_logNode *temp;
	while(t->prev){
		temp = t;
//		post("freeing %p", t);
		free(t->av);
		free(t);
		t = temp->prev;
	}
	x->topNode = x->history;
	x->currentNode  = x->history;
//	post("cleared history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);
	critical_exit(x->lock);
}

void dev_bpf_historyLog(t_dev_bpf *x, t_symbol *s, long argc, t_atom *argv)
{
	critical_enter(x->lock);
	t_logNode *hq = x->history;
	t_logNode *n = (t_logNode *)calloc(1, sizeof(t_logNode));
	n->av = (t_atom *)malloc(argc * sizeof(t_atom));
	memcpy(n->av, argv, argc * sizeof(t_atom));
		
	n->sym = s;
	n->ac = argc;
	n->next = NULL;
	
	//*x->hsteps++; //add max history steps option maybe

	if(hq->next == NULL){
		x->history->next = n;
		n->prev = hq;
		x->topNode = n;
		x->currentNode = n;
		post("1 n:%p next:%p prev:%p", n, n->next, n->prev);
		post("1 c:%p next:%p prev:%p", x->currentNode, x->currentNode->next, x->currentNode->prev);
		post("1 t:%p next:%p prev:%p", x->topNode, x->topNode->next, x->topNode->prev);
	} else if(x->currentNode == x->topNode){
		x->topNode->next = n;
		n->prev = x->topNode;
		x->topNode = n;
		x->currentNode = n;
		post("3 n:%p next:%p prev:%p", n, n->next, n->prev);
		post("3 c:%p next:%p prev:%p", x->currentNode, x->currentNode->next, x->currentNode->prev);
		post("3 t:%p next:%p prev:%p", x->topNode, x->topNode->next, x->topNode->prev);
	} else if(x->currentNode != x->topNode){

		
		t_logNode *temp;
		t_logNode *f = x->currentNode->next;
		while(f->next){
			temp = f;
			post("freeing %p", f);
			free(f->av);
			free(f);
			f = temp->next;
		}

		x->currentNode->next = n;
		n->prev = x->currentNode;
		x->topNode = n;
		x->currentNode = n;
		post("2 n:%p next:%p prev:%p", n, n->next, n->prev);
		post("2 c:%p next:%p prev:%p", x->currentNode, x->currentNode->next, x->currentNode->prev);
		post("2 t:%p next:%p prev:%p", x->topNode, x->topNode->next, x->topNode->prev);
		
	}
	
//	if(x->freshNode == TRUE)
//		post("history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);
	if(n->sym == gensym("insertPoint"))
		post("%p %s :: function %d at {%f, %f}",n,n->sym->s_name, atom_getlong(&n->av[0]), atom_getfloat(&n->av[1]), atom_getfloat(&n->av[2]));
	
	
	critical_exit(x->lock);
}

t_logNode *dev_bpf_getUndo(t_dev_bpf *x){
	x->freshNode = FALSE;
	t_logNode *c = x->currentNode;
	t_logNode *r = c;
	if(c->prev != NULL){
		x->currentNode = c->prev;
		//post("undo history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);
	} else {
		object_post((t_object *)x, "nothing to undo");
		post("undo history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);

	}
	return r;
}

t_logNode *dev_bpf_getRedo(t_dev_bpf *x){
	x->freshNode = FALSE;
	t_logNode *c = x->currentNode;
	if(c->next != NULL){
		c = c->next;
		x->currentNode = c;
		//post("redo history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);
	} else {
		object_post((t_object *)x, "nothing to redo");
		//post("undo history(%p) topNode(%p) currentNode(%p)", x->history, x->topNode, x->currentNode);

	}
	return c;
}


void dev_bpf_doUndoRedo(t_dev_bpf *x, t_logNode *n)
{
	x->freshNode = FALSE;
	t_symbol *s = n->sym;
	if(s == gensym("insertPoint")){ //--> remove point
		post("%p undo node function %d pt:{%f, %f}",n, atom_getlong(&n->av[0]), atom_getfloat(&n->av[1]), atom_getfloat(&n->av[2]));
		dev_bpf_listRemove(x, n->sym, n->ac, n->av);
	}
	if(s == gensym("removePoint")){ //--> insert point
		post("%p redo node function %d pt:{%f, %f}",n, atom_getlong(&n->av[0]), atom_getfloat(&n->av[1]), atom_getfloat(&n->av[2]));
		dev_bpf_listInsert(x, n->sym, n->ac, n->av);

	}
	if(s == gensym("startingPoint")){
		dev_bpf_clear(x);		
	}
	
}


void dev_bpf_list(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv){
	dev_bpf_anythingPipe(x, gensym("list"), argc, argv);
}

void dev_bpf_listInsert(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom *a = argv;
	int i = 0, functionNum = x->currentFunction;
	while(i < argc){
		if((argc % 3) == 0){		
			functionNum = atom_getlong(&a[i++]);
			if(functionNum > x->numFunctions - 1){
				x->numFunctions = functionNum + 1;
			}
		}
		t_pt pt = (t_pt){atom_getfloat(&a[i++]), atom_getfloat(&a[i++])};
		post("function %d pt{%f, %f}", functionNum, pt.x, pt.y);
		dev_bpf_insertPoint(x, pt, functionNum, x->monotonic_point_counter[functionNum]++);	
		
	
	}
	
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw(&(x->box));
}

void dev_bpf_listRemove(t_dev_bpf *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom *a = argv;
	int functionNum = x->currentFunction;
	int i = 0;
	if((argc % 3) == 0){
		while(i < argc){
			functionNum = atom_getlong(&a[i++]);

			t_point *p;
			t_pt pt = (t_pt){atom_getfloat(&a[i++]), atom_getfloat(&a[i++])};
			if(p = dev_bpf_select(x, pt)){
				dev_bpf_removePoint(x, p, functionNum);
				x->monotonic_point_counter[functionNum]--;
				jbox_invalidate_layer((t_object *)x, x->pv, l_points);
				jbox_redraw(&(x->box));
			} else {
				post("failed pt {%f, %f}", pt.x, pt.y);
			}
		}
		
//		dev_bpf_historyLog(x, "removePoint", (long)argc, argv);
		jbox_invalidate_layer((t_object *)x, x->pv, l_points);
		jbox_redraw(&(x->box));
	}
}


void dev_bpf_dump(t_dev_bpf *x){
	int i, point_num = 0;
	t_atom out[4];
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[1]), i);
		while(p){
			atom_setlong(&(out[0]), point_num++);
			if(x->x_res == ps_int){
				atom_setfloat(&(out[2]), round(p->coords.x));
			}else{
				atom_setfloat(&(out[2]), p->coords.x);
			}
			if(x->y_res == ps_int){
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

double dev_bpf_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

void dev_bpf_xminmax(t_dev_bpf *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	dev_bpf_invalidateAllPos(x);
	jbox_redraw(&(x->box));
}

void dev_bpf_yminmax(t_dev_bpf *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	dev_bpf_invalidateAllPos(x);
	jbox_redraw(&(x->box));
}

void dev_bpf_resizeRectWithMargins(t_rect *r){
	r->x = XMARGIN;
	r->width -= (XMARGIN * 2);
	r->y = YMARGIN;
	r->height -= (YMARGIN * 2);
}

void dev_bpf_clear(t_dev_bpf *x){
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
	dev_bpf_invalidateAllPos(x);
	jbox_redraw(&(x->box));
}
	   
	   
void dev_bpf_invalidateAllPos(t_dev_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		jbox_invalidate_layer((t_object *)x, x->pv, l_pos[i]);
	}
}

t_max_err dev_bpf_points_get(t_dev_bpf *x, t_object *attr, long *argc, t_atom **argv){
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

t_max_err dev_bpf_points_set(t_dev_bpf *x, t_object *attr, long argc, t_atom *argv){
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
			dev_bpf_insertPoint(x, pt, cf, x->monotonic_point_counter[cf]++);
		/*	
			//history log new point
			t_atom logPt[3];
			atom_setlong(&logPt[0], cf);
			atom_setfloat(&logPt[1], pt.x);
			atom_setfloat(&logPt[2], pt.y);
			dev_bpf_historyLog(x, "insertPoint", 3, logPt);
		*/	
		}
	}
	return MAX_ERR_NONE;
}

void dev_bpf_assist(t_dev_bpf *x, void *b, long io, long num, char *s){ 
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

void dev_bpf_free(t_dev_bpf *x){ 
 	jbox_free((t_jbox *)x); 
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		t_point *next;
		while(p){
			next = p->next; 
			free(p);
			p = next;
			next = next->next; //is this this same as next = p->next?
		}
	}
	
	t_logNode *l = x->history;
	t_logNode *nextL;
	while(l){
		nextL = l->next;
		free(l->av);
		free(l);
		l = nextL;
	}
} 


void *dev_bpf_new(t_symbol *s, long argc, t_atom *argv){ 
 	t_dev_bpf *x = NULL; 
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

 	if(x = (t_dev_bpf *)object_alloc(dev_bpf_class)){ 

 		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->box.b_firstin = (void *)x; 

 		x->out_dump = listout((t_object *)x); 
		x->out_sel = listout((t_object *)x);
 		x->out_main = listout((t_object *)x); 

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *));
 		x->funcattr = (t_funcattr **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_funcattr *));
		x->pos = (t_pt *)calloc(MAX_NUM_FUNCTIONS, sizeof(t_pt));

		x->monotonic_point_counter = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
		memset(x->monotonic_point_counter, '\0', MAX_NUM_FUNCTIONS * sizeof(int));

		x->npoints = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
		memset(x->npoints, '\0', MAX_NUM_FUNCTIONS * sizeof(int));


		x->history = (t_logNode *)calloc(1, sizeof(t_logNode));
		x->history->sym = gensym("startingPoint");
		x->history->ac = 0;
		x->history->av = NULL;
		x->history->prev = NULL;
		x->history->next = NULL;
	
//		memset(x->history, '\0', sizeof(t_logNode));
		
		x->topNode = (t_logNode *)calloc(1, sizeof(t_logNode));
		x->currentNode = (t_logNode *)calloc(1, sizeof(t_logNode));

		memcpy(x->topNode, x->history, sizeof(t_logNode));
		memcpy(x->currentNode, x->history, sizeof(t_logNode));
		
		x->freshNode = FALSE;
		x->hsteps = 0;

		
		//changed to plain int
		x->nTempPoints = 0;//(int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
//		memset(x->nTempPoints, '\0', MAX_NUM_FUNCTIONS * sizeof(int));
		//
		
		x->selected = NULL;
		x->sel_box = (t_rect){-1, -1, 0, 0};

		x->sel_box_mouseover = 0;
		x->sel_box_lineColor = (t_jrgba){0.5, 0.5, 0.5, 0.8};
		//x->sel_box_tempPts = (t_pt **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_pt *));

 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		critical_new(&(x->lock));

		int i;
		for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
			x->funcattr[i] = (t_funcattr *)calloc(1, sizeof(t_funcattr));
			x->funcattr[i]->line_color = x->lineColor;
			x->funcattr[i]->point_color = x->pointColor;
			memset(x->funcattr[i]->dash, 0, 8);
			x->funcattr[i]->ndash = 0;
			sprintf(x->funcattr[i]->name, "%d", i);
		}
 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 

 		return x; 
 	} 
 	return NULL; 
} 


