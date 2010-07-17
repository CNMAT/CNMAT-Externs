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
  VERSION 0.5: music notation display
  VERSION 0.5.1: grid lines
  VERSION 0.5.2: quarter tones in notation display
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
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

#define MAX_NUM_FUNCTIONS 64

#define XMARGIN 5
#define YMARGIN 5

typedef struct _point{ 
 	t_pt coords;
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
	t_jrgba gridColor;
	double xmin, xmax, ymin, ymax;
	t_pt *pos;
	long drawpos, drawlabels;
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
	t_symbol *displaymode;
	double major_x_grid_width, major_y_grid_height, num_minor_x_grid_divisions, num_minor_y_grid_divisions;
	long show_x_grid, show_y_grid;
	//float *major_x_tics, *major_y_tics, *minor_x_tics, *minor_y_tics;
	float major_x_tics[1024], major_y_tics[1024], minor_x_tics[1024], minor_y_tics[1024];
	long num_major_x_tics, num_major_y_tics, num_minor_x_tics, num_minor_y_tics;
	long snaptogrid;
} t_bpf; 

void *bpf_class; 

void bpf_paint(t_bpf *x, t_object *patcherview); 
void bpf_paint_bpf(t_bpf *x, t_object *patcherview, t_rect r);
void bpf_paint_bpfPosition(t_bpf *x, t_object *patcherview, t_rect r);
void bpf_paint_notes(t_bpf *x, t_object *patcherview, t_rect r);
void bpf_paintGrid(t_bpf *x, t_object *patcherview, t_rect r);
void bpf_drawMajorXGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx);
void bpf_drawMajorYGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx);
void bpf_drawMinorXGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx);
void bpf_drawMinorYGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx);
void bpf_major_x_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv);
void bpf_major_y_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv);
void bpf_minor_x_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv);
void bpf_minor_y_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv);
void bpf_findNearestGridPoint(t_bpf *x, t_pt pt_sc, t_pt *pt_out_sc);
void bpf_reorderPoint(t_bpf *x, t_point *p);
void bpf_swapPoints(t_point *p1, t_point *p2);
double bpf_computeNotePos(double y, t_rect r);
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
t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum);
void bpf_removeSelected(t_bpf *x);
void bpf_removePoint(t_bpf *x, t_point *point, int functionNum);
void bpf_clear(t_bpf *x);
double bpf_scale(double f, double min_in, double max_in, double min_out, double max_out);
void bpf_xminmax(t_bpf *x, double min, double max);
void bpf_yminmax(t_bpf *x, double min, double max);
void bpf_snapAllPointsToGrid(t_bpf *);
void bpf_snapCurrentFunctionToGrid(t_bpf *x);
void bpf_snapFunctionToGrid(t_bpf *x, int function);
void bpf_snapSelectionToGrid(t_bpf *x);
void bpf_resizeRectWithMargins(t_bpf *x, t_rect *r);

//void bpf_read(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_doread(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_write(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
//void bpf_dowrite(t_bpf *x, t_symbol *msg, short argc, t_atom *argv);
int main(void); 
void *bpf_new(t_symbol *s, long argc, t_atom *argv); 
t_max_err bpf_points_get(t_bpf *x, t_object *attr, long *argc, t_atom **argv);
t_max_err bpf_points_set(t_bpf *x, t_object *attr, long argc, t_atom *argv);
/*
t_max_err bpf_majorXTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_majorYTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_minorXTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv);
t_max_err bpf_minorYTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv);
*/
void myobject_write(t_bpf *x, t_symbol *s);
void myobject_dowrite(t_bpf *x, t_symbol *s);
void myobject_writefile(t_bpf *x, char *filename, short path);

t_symbol *l_background, *l_points, *l_pos, *ps_int, *l_grid;
t_symbol *ps_bpf, *ps_notes;

//const char *notenames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
int notestep[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6};
int sharps[] = {0, 93, 103, 113, 0, 93, 103, 113, 0, 93, 0, 93, 103, 113, 0, 93, 103, 113, 0, 93, 103, 113, 0, 93};

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

	bpf_resizeRectWithMargins(x, &r);
	bpf_paintGrid(x, patcherview, r);

	if(x->displaymode == ps_bpf){
		bpf_paint_bpf(x, patcherview, r);
	}else if(x->displaymode == ps_notes){
		bpf_paint_notes(x, patcherview, r);
	}
	bpf_paint_bpfPosition(x, patcherview, r);
	g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);
	jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
	jgraphics_stroke(g);
}

void bpf_paint_bpf(t_bpf *x, t_object *patcherview, t_rect r){
	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_points, r.width, r.height);
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
				sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);

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
				sc.x = bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width);
				sc.y = bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y);
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
		critical_exit(x->lock);
		jbox_end_layer((t_object *)x, patcherview, l_points);
 	} 
	jbox_paint_layer((t_object *)x, patcherview, l_points, r.x, r.y);
}

void bpf_paint_bpfPosition(t_bpf *x, t_object *patcherview, t_rect r){
	if(x->displaymode != ps_notes){
		int i;
		for(i = 0; i < x->numFunctions; i++){
			t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_pos, 4, r.height);
			if(g){
				if(x->currentFunction == i){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					jgraphics_set_source_jrgba(g, &(x->lineColor));
				}
				jgraphics_ellipse(g, 0, 0, 4, 4);
				jgraphics_fill(g);
				jbox_end_layer((t_object *)x, patcherview, l_pos);
			}
			jbox_paint_layer((t_object *)x, patcherview, l_pos, x->pos[i].x - 2, x->pos[i].y - 2);
		}
	}else{
		t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_pos, 2, r.height);
		if(g){
			jgraphics_move_to(g, 2, 0);
			jgraphics_line_to(g, 2, r.height);
			jgraphics_stroke(g);
			jbox_end_layer((t_object *)x, patcherview, l_pos);
		}
		jbox_paint_layer((t_object *)x, patcherview, l_pos, x->pos[x->currentFunction].x - 2, 0);
	}
} 

#define DIV 24
#define STEP 4
#define OCTAVE (7 * STEP)
#define SPACE 8

void bpf_paint_notes(t_bpf *x, t_object *patcherview, t_rect r){
	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_points, r.width, r.height);
 	if(g){ 
		double xpos = 0, ypos = 8;
		double mid = r.height / 2;
		int i;
		//jgraphics_move_to(g, 0, r.height / 2);
		//jgraphics_line_to(g, r.width, r.height / 2);
		for(i = 0; i < 5; i++){
			jgraphics_move_to(g, 0, ypos + mid);
			jgraphics_line_to(g, r.width, ypos + mid);
			jgraphics_move_to(g, 0, mid - ypos);
			jgraphics_line_to(g, r.width, mid - ypos);
			ypos += SPACE;
		}
		jgraphics_stroke(g);
		jgraphics_select_font_face(g, "Sonora", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
		jgraphics_set_font_size(g, 54);
		char ch = 38;
		jgraphics_move_to(g, 0, mid - 16);
		jgraphics_show_text(g, &ch);
		ch = 63;
		jgraphics_move_to(g, 0, mid + 16);
		jgraphics_show_text(g, &ch);

		// middle c is at r.height / 2
		int function;
		critical_enter(x->lock);
		for(function = 0; function < x->numFunctions; function++){
			if(x->hideFunctions[function]){
				continue;
			}
			t_point *p = x->functions[function];
			while(p){
				if(function == x->currentFunction){
					jgraphics_set_source_jrgba(g, &(x->pointColor));
				}else{
					jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
				}
				int pc = (int)round(p->coords.y * 2) % DIV;
				ypos = bpf_computeNotePos(p->coords.y, r);
				double xsc = bpf_scale(p->coords.x, x->xmin, x->xmax, 0, r.width);
				// ledger lines
				if(p->coords.y > 80){
					double ll = (r.height / 2.) - (SPACE * 6);
					ch = 95;
					while(ll > ypos - SPACE){
						jgraphics_move_to(g, xsc - 2, ll);
						jgraphics_show_text(g, &ch);
						ll -= SPACE;
					}
				}else if(p->coords.y < 40){
					double ll = (r.height / 2.) + (SPACE * 6);
					ch = 95;
					while(ll < ypos){
						jgraphics_move_to(g, xsc - 2, ll);
						jgraphics_show_text(g, &ch);
						ll += SPACE;
					}
				}
				if(sharps[pc]){
					jgraphics_select_font_face(g, "Tempera", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
					jgraphics_set_font_size(g, 36);
					//ch = 35;
					ch = sharps[pc];
					jgraphics_move_to(g, xsc - 10, ypos - STEP);
					jgraphics_show_text(g, &ch);
				}
				ch = 81;
				if(p->coords.y < 50 || (p->coords.y > 59 && p->coords.y < 71)){
					ch = 113;
				}
				jgraphics_select_font_face(g, "Sonora", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
				jgraphics_set_font_size(g, 54);
				if(p->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}
				jgraphics_move_to(g, xsc, ypos);
				jgraphics_show_text(g, &ch);
				p = p->next;
			}
		}
		critical_exit(x->lock);
		jbox_end_layer((t_object *)x, patcherview, l_points);
 	} 
	jbox_paint_layer((t_object *)x, patcherview, l_points, r.x, r.y);
}

void bpf_paintGrid(t_bpf *x, t_object *patcherview, t_rect r){
	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_grid, r.width, r.height);
 	if(g){ 
		double xgw = (x->major_x_grid_width / (x->xmax - x->xmin)) * r.width;
		double ygw = (x->major_y_grid_height / (x->ymax - x->ymin)) * r.height;
		double pos = 0;
		int i, j;
		if(x->num_major_x_tics){
			for(i = 0; i < x->num_major_x_tics; i++){
				double tic = bpf_scale(x->major_x_tics[i], x->xmin, x->xmax, 0, r.width);
				bpf_drawMajorXGridLine(x, g, r, tic);
				if(!x->num_minor_x_tics){
					if(i < x->num_major_x_tics - 1){
						for(j = 0; j < x->num_minor_x_grid_divisions; j++){
							bpf_drawMinorXGridLine(x, g, r, tic + (((bpf_scale(x->major_x_tics[i + 1], x->xmin, x->xmax, 0, r.width) - tic) / (x->num_minor_x_grid_divisions)) * j));
						}
					}
				}
			}
		}else{
			while(pos < r.width){
				bpf_drawMajorXGridLine(x, g, r, pos);
				if(!x->num_minor_x_tics){
					for(j = 1; j < x->num_minor_x_grid_divisions; j++){
						bpf_drawMinorXGridLine(x, g, r, pos + (j * (xgw / (x->num_minor_x_grid_divisions))));
					}
				}
				pos += xgw;
			}
		}
		if(x->num_minor_x_tics){
			for(i = 0; i < x->num_minor_x_tics; i++){
				double tic = bpf_scale(x->minor_x_tics[i], x->xmin, x->xmax, 0, r.width);
				bpf_drawMinorXGridLine(x, g, r, tic);
			}
		}

		if(x->displaymode != ps_notes){
			pos = r.height;
			if(x->num_major_y_tics){
				for(i = 0; i < x->num_major_y_tics; i++){
					double tic = bpf_scale(x->major_y_tics[i], x->ymin, x->ymax, r.height, 0);
					bpf_drawMajorYGridLine(x, g, r, tic);
					if(!x->num_minor_y_tics){
						if(i < x->num_major_y_tics - 1){
							for(j = 0; j < x->num_minor_y_grid_divisions; j++){
								bpf_drawMinorYGridLine(x, g, r, tic + (((bpf_scale(x->major_y_tics[i + 1], x->ymin, x->ymax, r.height, 0) - tic) / (x->num_minor_y_grid_divisions)) * j));
							}
						}
					}
				}
			}else{
				while(pos > 0){
					bpf_drawMajorYGridLine(x, g, r, pos);
					if(!x->num_minor_y_tics){
						for(j = 1; j < x->num_minor_y_grid_divisions; j++){
							bpf_drawMinorYGridLine(x, g, r, pos - (j * (ygw / (x->num_minor_y_grid_divisions))));
						}
					}
					pos -= ygw;
				}
			}
			if(x->num_minor_y_tics){
				for(i = 0; i < x->num_minor_y_tics; i++){
					double tic = bpf_scale(x->minor_y_tics[i], x->ymin, x->ymax, r.height, 0);
					bpf_drawMinorYGridLine(x, g, r, tic);
				}
			}
		}
		critical_enter(x->lock);

		critical_exit(x->lock);
		jbox_end_layer((t_object *)x, patcherview, l_points);
 	} 
	jbox_paint_layer((t_object *)x, patcherview, l_grid, r.x, r.y);
}

void bpf_drawMajorXGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx){
	char buf[16];
	jgraphics_set_source_jrgba(g, &(x->gridColor));
	jgraphics_set_line_width(g, 1.5);
	jgraphics_move_to(g, xx + 2, 10);
	sprintf(buf, "%.02f", bpf_scale(xx, 0., r.width, x->xmin, x->xmax));
	jgraphics_show_text(g, buf);
	jgraphics_move_to(g, xx, 0);
	if(x->show_x_grid){
		jgraphics_line_to(g, xx, r.height);
	}else{
		jgraphics_line_to(g, xx, 10);
	}
	jgraphics_stroke(g);
}

void bpf_drawMajorYGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx){
	char buf[16];
	jgraphics_set_source_jrgba(g, &(x->gridColor));
	jgraphics_set_line_width(g, 1.5);
	jgraphics_move_to(g, 0, xx - 2);
	sprintf(buf, "%.02f", bpf_scale(xx, r.height, 0., x->ymin, x->ymax));
	jgraphics_show_text(g, buf);
	jgraphics_move_to(g, 0, xx);
	if(x->show_y_grid){
		jgraphics_line_to(g, r.width, xx);
	}else{
		jgraphics_line_to(g, 25, xx);
	}
	jgraphics_stroke(g);
}

void bpf_drawMinorXGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx){
	if(!x->show_x_grid){
		return;
	}
	jgraphics_set_line_width(g, .5);
	jgraphics_set_source_jrgba(g, &(x->gridColor));
	jgraphics_move_to(g, xx, 0);
	jgraphics_line_to(g, xx, r.height);
	jgraphics_stroke(g);
}

void bpf_drawMinorYGridLine(t_bpf *x, t_jgraphics *g, t_rect r, double xx){
	if(!x->show_y_grid){
		return;
	}
	jgraphics_set_line_width(g, .5);
	jgraphics_set_source_jrgba(g, &(x->gridColor));
	jgraphics_move_to(g, 0, xx);
	jgraphics_line_to(g, r.width, xx);
	jgraphics_stroke(g);
}

void bpf_major_x_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	sysmem_freeptr(x->major_x_tics);
	//x->major_x_tics = NULL;
	x->num_major_x_tics = 0;
	if(argc){
		//x->major_x_tics = (float *)sysmem_newptr(argc * sizeof(float));
		int i;
		for(i = 0; i < argc; i++){
			x->major_x_tics[i] = atom_getfloat(argv + i);
		}
		x->num_major_x_tics = argc;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_major_y_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	sysmem_freeptr(x->major_y_tics);
	//x->major_y_tics = NULL;
	x->num_major_y_tics = 0;
	if(argc){
		//x->major_y_tics = (float *)sysmem_newptr(argc * sizeof(float));
		int i;
		for(i = 0; i < argc; i++){
			x->major_y_tics[i] = atom_getfloat(argv + i);
		}
		x->num_major_y_tics = argc;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_minor_x_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	sysmem_freeptr(x->minor_x_tics);
	//x->minor_x_tics = NULL;
	x->num_minor_x_tics = 0;
	if(argc){
		//x->minor_x_tics = (float *)sysmem_newptr(argc * sizeof(float));
		int i;
		for(i = 0; i < argc; i++){
			x->minor_x_tics[i] = atom_getfloat(argv + i);
		}
		x->num_minor_x_tics = argc;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_minor_y_tics(t_bpf *x, t_symbol *msg, int argc, t_atom *argv){
	sysmem_freeptr(x->minor_y_tics);
	//x->minor_y_tics = NULL;
	x->num_minor_y_tics = 0;
	if(argc){
		//x->minor_y_tics = (float *)sysmem_newptr(argc * sizeof(float));
		int i;
		for(i = 0; i < argc; i++){
			x->minor_y_tics[i] = atom_getfloat(argv + i);
		}
		x->num_minor_y_tics = argc;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_findNearestGridPoint(t_bpf *x, t_pt pt_sc, t_pt *pt_out_sc){
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	bpf_resizeRectWithMargins(x, &r);
	t_pt pt;
	pt.x = bpf_scale(pt_sc.x, 0, r.width, x->xmin, x->xmax);
	pt.y = bpf_scale(pt_sc.y, r.height, 0, x->ymin, x->ymax);
	double min = 0, mindiff = DBL_MAX, diff = 0;
	int i, j;

	if(x->num_major_x_tics){
		for(i = 0; i < x->num_major_x_tics; i++){
			diff = fabs(pt.x - x->major_x_tics[i]);
			if(diff < mindiff){
				mindiff = diff;
				min = x->major_x_tics[i];
			}
			if(!x->num_minor_x_tics){
				if(i < x->num_major_x_tics - 1){
					for(j = 0; j < x->num_minor_x_grid_divisions; j++){
						diff = fabs(pt.x - (x->major_x_tics[i] + (((x->major_x_tics[i + 1] - x->major_x_tics[i]) / (x->num_minor_x_grid_divisions)) * j)));
						if(diff < mindiff){
							mindiff = diff;
							min = (x->major_x_tics[i] + (((x->major_x_tics[i + 1] - x->major_x_tics[i]) / (x->num_minor_x_grid_divisions)) * j));
						}
					}
				}
			}
		}
		if(x->num_minor_x_tics){
			for(i = 0; i < x->num_minor_x_tics; i++){
				diff = fabs(pt.x - x->minor_x_tics[i]);
				if(diff < mindiff){
					mindiff = diff;
					min = x->minor_x_tics[i];
				}
			}
		}
	}else{
		double pos = x->xmin;
		while(pos < x->xmax){
			diff = fabs(pt.x - pos);
			if(diff < mindiff){
				mindiff = diff;
				min = pos;
			}
			if(!x->num_minor_x_tics){
				for(i = 0; i < x->num_minor_x_grid_divisions; i++){
					diff = fabs(pt.x - (pos + ((x->major_x_grid_width / (x->num_minor_x_grid_divisions)) * i)));
					if(diff < mindiff){
						mindiff = diff;
						min = (pos + ((x->major_x_grid_width / x->num_minor_x_grid_divisions) * i));
					}
				}
			}
			pos += x->major_x_grid_width;
		}
	}
	pt_out_sc->x = bpf_scale(min, x->xmin, x->xmax, r.x, r.width);

	min = 0;
	mindiff = DBL_MAX;
	if(x->num_major_y_tics){
		for(i = 0; i < x->num_major_y_tics; i++){
			diff = fabs(pt.y - x->major_y_tics[i]);
			if(diff < mindiff){
				mindiff = diff;
				min = x->major_y_tics[i];
			}
			if(!x->num_minor_y_tics){
				if(i < x->num_major_y_tics - 1){
					for(j = 0; j < x->num_minor_y_grid_divisions; j++){
						diff = fabs(pt.y - (x->major_y_tics[i] + (((x->major_y_tics[i + 1] - x->major_y_tics[i]) / (x->num_minor_y_grid_divisions)) * j)));
						if(diff < mindiff){
							mindiff = diff;
							min = (x->major_y_tics[i] + (((x->major_y_tics[i + 1] - x->major_y_tics[i]) / (x->num_minor_y_grid_divisions)) * j));
						}
					}
				}
			}
		}
		if(x->num_minor_y_tics){
			for(i = 0; i < x->num_minor_y_tics; i++){
				diff = fabs(pt.y - x->minor_y_tics[i]);
				if(diff < mindiff){
					mindiff = diff;
					min = x->minor_y_tics[i];
				}
			}
		}
	}else{
		double pos = x->ymin;
		while(pos < x->ymax){
			diff = fabs(pt.y - pos);
			if(diff < mindiff){
				mindiff = diff;
				min = pos;
			}
			if(!x->num_minor_y_tics){
				for(i = 0; i < x->num_minor_y_grid_divisions; i++){
					diff = fabs(pt.y - (pos + ((x->major_y_grid_height / (x->num_minor_y_grid_divisions)) * i)));
					if(diff < mindiff){
						mindiff = diff;
						min = (pos + ((x->major_y_grid_height / x->num_minor_y_grid_divisions) * i));
					}
				}
			}
			pos += x->major_y_grid_height;
		}
	}

	pt_out_sc->y = bpf_scale(min, x->ymin, x->ymax, r.height, r.y);
}

void bpf_reorderPoint(t_bpf *x, t_point *p){
	if(!p){
		return;
	}
	while(p->prev){
		if(p->coords.x < p->prev->coords.x){
			bpf_swapPoints(p->prev, p);
		}else{
			break;
		}
	}
	while(p->next){
		if(p->coords.x > p->next->coords.x){
			bpf_swapPoints(p, p->next);
		}else{
			break;
		}
	}
	while(p->prev){
		p = p->prev;
	}
	x->functions[x->currentFunction] = p;
}

void bpf_swapPoints(t_point *p1, t_point *p2){
	if(!p1 || !p2){
		return;
	}
	if(p1->prev){
		p1->prev->next = p2;
	}
	if(p2->next){
		p2->next->prev = p1;
	}

	p1->next = p2->next;
	p2->prev = p1->prev;
	p1->prev = p2;
	p2->next = p1;

}

double bpf_computeNotePos(double y, t_rect r){
	int pc = (int)round(y * 2) % DIV;
	int step = (notestep[pc] - 1) * STEP;
	int oct = (int)floor(((y * 2) / DIV) - 5);
	
	if(!pc && round(y * 2) > y * 2){
		oct++;
	}
	//post("pc = %d, step = %d, oct = %d, %f", pc, step, oct, ((r.height / 2) - (oct * OCTAVE)) - step);
	return ((r.height / 2) - (oct * OCTAVE)) - step;
}

double bpf_uncomputeNotePos(double y, t_rect r){
	double diff = DBL_MAX;
	double closest = 0;
	double i;
	for(i = 0; i < 128; i += .5){
		double d = fabs(bpf_computeNotePos(i, r) - y);
		//post("y = %f, i = %f, diff = %f, notepos = %f", y, i, diff, bpf_computeNotePos(i, r));
		if(d < diff){
			diff = d;
			closest = i;
		}
	}
	//post("closest = %f", closest);
	double notepos = bpf_computeNotePos(closest, r);
	//post("notepos = %f, middle = %f", notepos, r.height / 2);
	int cl = closest;
	if(y < notepos - (SPACE * (2 / 3))){
		if(sharps[(cl + 1)] % DIV){
			cl += .5;
		}
		//cl += (sharps[(cl + 1) % DIV]);
	}else if(y > notepos + (SPACE * (2 / 3))){
		if((cl % DIV) > 0){
			if(sharps[(cl - 1) % DIV] == 0){
				cl -= .5;
			}
			//cl -= sharps[(cl - 1) % DIV];
		}
	}
	return cl;
	/*
	double nspaces = r.height / SPACE;
	double space = (nspaces * (y / (r.height));

	post("space = %f", space);
	*/
}

void bpf_dsp(t_bpf *x, t_signal **sp, short *count){
	if(count[0]){
		dsp_add(bpf_perform, 3, x, sp[0]->s_n, sp[0]->s_vec);
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

	t_point *p = bpf_insertPoint(x, pt, functionNum);
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
			jbox_invalidate_layer((t_object *)x, x->pv, l_pos);
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

	while(ptr){
		t_pt sc = ptr->coords;
		sc.x = bpf_scale(sc.x, x->xmin, x->xmax, r.x, r.width);
		if(x->displaymode == ps_bpf){
			sc.y = bpf_scale(sc.y, x->ymin, x->ymax, r.height, r.y);
			if((xdif = fabs(sc.x - p_sc.x)) < 3 && (ydif = fabs(sc.y - p_sc.y)) < 3){
				if(xdif + ydif < min){
					min = xdif + ydif;
					min_ptr = ptr;
				}
			}
		}else{
			sc.y = bpf_computeNotePos(sc.y, r);
			if((xdif = fabs((sc.x + 6) - p_sc.x)) < 8 && (ydif = fabs(sc.y - p_sc.y)) < SPACE){
				if(xdif + ydif < min){
					min = xdif + ydif;
					min_ptr = ptr;
				}
			}
		}

		ptr = ptr->next;
	}
	return min_ptr;
}

t_max_err bpf_notify(t_bpf *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		/*
		if(attrname == gensym("major_x_grid_width")){
			x->num_major_x_tics = 0;
		}
		if(attrname == gensym("major_y_grid_width")){
			x->num_major_y_tics = 0;
		}
		if(attrname == gensym("num_minor_x_grid_divisions")){
			x->num_minor_x_tics = 0;
		}
		if(attrname == gensym("num_minor_y_grid_divisions")){
			x->num_minor_y_tics = 0;
		}
		*/
		jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
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
		t_point *s = bpf_select(x, pt);
		if(s){
			if(s->selected){
				if(s != x->selected){
					if(s->next_selected){
						s->next_selected->prev_selected = s->prev_selected;
					}
					if(s->prev_selected){
						s->prev_selected->next_selected = s->next_selected;
					}
					s->next_selected = x->selected;
					x->selected = s;
				}
			}else{
				bpf_clearSelected(x);
				s->selected = 1;
				x->selected = s;
				s->next_selected = s->prev_selected = NULL;
			}
		}else{
			if(!x->lockinput){
				bpf_clearSelected(x);
				if(x->displaymode == ps_notes){
					coords.y = bpf_uncomputeNotePos(pt.y + x->ymargin, r);
				}
				if(x->snaptogrid){
					t_pt snapped = (t_pt){bpf_scale(coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(coords.y, x->ymin, x->ymax, r.height, r.y)};
					bpf_findNearestGridPoint(x, snapped, &snapped);
					coords = (t_pt){bpf_scale(snapped.x, r.x, r.width, x->xmin, x->xmax), bpf_scale(snapped.y, r.height, r.y, x->ymin, x->ymax)};
				}
				p = bpf_insertPoint(x, coords, x->currentFunction);
				p->selected = 1;
				if(x->selected){
					x->selected->prev_selected = p;
				}
				p->next_selected = x->selected;
				p->prev_selected = NULL;
				x->selected = p;
			}
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
		if(!(x->selected)){
			break;
		}
		t_point *p = x->selected;
		t_point *next, *prev;
		t_pt psc = (t_pt){bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
		t_pt delta;
		if(x->snaptogrid){
			t_pt snapped;
			bpf_findNearestGridPoint(x, pt, &snapped);
			pt = snapped;
		}
		p->coords = (t_pt){bpf_scale(pt.x, r.x, r.width, x->xmin, x->xmax), bpf_scale(pt.y, r.height, r.y, x->ymin, x->ymax)};
		delta.x = pt.x - psc.x;
		delta.y = pt.y - psc.y;
		bpf_reorderPoint(x, p);
		p = p->next_selected;
		while(p){
			psc = (t_pt){bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
			p->coords = (t_pt){bpf_scale(psc.x + delta.x, r.x, r.width, x->xmin, x->xmax), bpf_scale(psc.y + delta.y, r.height, r.y, x->ymin, x->ymax)};
			bpf_reorderPoint(x, p);
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
	x->drag = pt;
	bpf_outputSelection(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_mouseup(t_bpf *x, t_object *patcherview, t_pt pt, long modifiers){
	x->sel_box = (t_rect){0., 0., 0., 0.};
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_outputSelection(t_bpf *x){
	if(!(x->selected)){
		return;
	}
	t_point *p = x->selected;
	t_atom out[3];
	while(p){
		atom_setlong(&(out[0]), x->currentFunction);
		atom_setfloat(&(out[1]), x->selected->coords.x);
		atom_setfloat(&(out[2]), x->selected->coords.y);
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

t_point *bpf_insertPoint(t_bpf *x, t_pt coords, int functionNum){
	critical_enter(x->lock);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = (t_point *)calloc(1, sizeof(t_point));
	p->coords = coords;
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

void bpf_removePoint(t_bpf *x, t_point *point, int functionNum){
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
			if(p->next){
				p->next->prev = p->prev;
			}
			if(p->prev){
				p->prev->next = p->next;
			}
			if(p->next_selected){
				p->next_selected->prev_selected = p->prev_selected;
			}
			if(p->prev_selected){
				p->prev_selected->next_selected = p->next_selected;
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
		atom_setlong(&(out[1]), x->labelstart + i);
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
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	bpf_invalidateAllPos(x);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_yminmax(t_bpf *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_invalidate_layer((t_object *)x, x->pv, l_grid);
	bpf_invalidateAllPos(x);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_snapAllPointsToGrid(t_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		bpf_snapFunctionToGrid(x, i);
	}
}

void bpf_snapCurrentFunctionToGrid(t_bpf *x){
	bpf_snapFunctionToGrid(x, x->currentFunction);
}

void bpf_snapFunctionToGrid(t_bpf *x, int function){
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);
	bpf_resizeRectWithMargins(x, &r);
	t_point *p = x->functions[function];
	while(p){
		t_pt sc = (t_pt){bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
		bpf_findNearestGridPoint(x, sc, &sc);
		p->coords = (t_pt){bpf_scale(sc.x, r.x, r.width, x->xmin, x->xmax), bpf_scale(sc.y, r.height, r.y, x->ymin, x->ymax)};
		p = p->next;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_snapSelectionToGrid(t_bpf *x){
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);
	bpf_resizeRectWithMargins(x, &r);
	t_point *p = x->selected;
	while(p){
		t_pt sc = (t_pt){bpf_scale(p->coords.x, x->xmin, x->xmax, r.x, r.width), bpf_scale(p->coords.y, x->ymin, x->ymax, r.height, r.y)};
		bpf_findNearestGridPoint(x, sc, &sc);
		p->coords = (t_pt){bpf_scale(sc.x, r.x, r.width, x->xmin, x->xmax), bpf_scale(sc.y, r.height, r.y, x->ymin, x->ymax)};
		p = p->next_selected;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_points);
	jbox_redraw((t_jbox *)&(x->box));
}

void bpf_resizeRectWithMargins(t_bpf *x, t_rect *r){
	r->x = x->xmargin;
	r->width -= (x->xmargin * 2);
	r->y = x->ymargin;
	r->height -= (x->ymargin * 2);
}

void bpf_clear(t_bpf *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		if(p){
			free(p);
		}
		x->functions[i] = NULL;
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
	jbox_invalidate_layer((t_object *)x, x->pv, l_pos);
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
	class_addmethod(c, (method)bpf_major_x_tics, "major_x_tics", A_GIMME, 0);
	class_addmethod(c, (method)bpf_major_y_tics, "major_y_tics", A_GIMME, 0);
	class_addmethod(c, (method)bpf_minor_x_tics, "minor_x_tics", A_GIMME, 0);
	class_addmethod(c, (method)bpf_minor_y_tics, "minor_y_tics", A_GIMME, 0);
	class_addmethod(c, (method)bpf_snapAllPointsToGrid, "snap_all_points_to_grid", 0);
	class_addmethod(c, (method)bpf_snapCurrentFunctionToGrid, "snap_current_function_to_grid", 0);
	class_addmethod(c, (method)bpf_snapCurrentFunctionToGrid, "snap_function_to_grid", A_LONG, 0);
	class_addmethod(c, (method)bpf_snapSelectionToGrid, "snap_selection_to_grid", 0);

	CLASS_ATTR_SYM(c, "displaymode", 0, t_bpf, displaymode);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "displaymode", 0, "bpf");

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

	CLASS_ATTR_DOUBLE(c, "major_x_grid_width", 0, t_bpf, major_x_grid_width);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "major_x_grid_width", 0, "0.25");
	CLASS_ATTR_LABEL(c, "major_x_grid_width", 0, "Major X Grid Width");

	CLASS_ATTR_DOUBLE(c, "major_y_grid_height", 0, t_bpf, major_y_grid_height);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "major_y_grid_height", 0, "0.25");
	CLASS_ATTR_LABEL(c, "major_y_grid_height", 0, "Major Y Grid Height");

	CLASS_ATTR_DOUBLE(c, "num_minor_x_grid_divisions", 0, t_bpf, num_minor_x_grid_divisions);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "num_minor_x_grid_divisions", 0, "1");
	CLASS_ATTR_LABEL(c, "num_minor_x_grid_divisions", 0, "Number of X Grid Divisions");

	CLASS_ATTR_DOUBLE(c, "num_minor_y_grid_divisions", 0, t_bpf, num_minor_y_grid_divisions);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "num_minor_y_grid_divisions", 0, "1");
	CLASS_ATTR_LABEL(c, "num_minor_y_grid_divisions", 0, "Number of Y Grid Divisions");

	CLASS_ATTR_LONG(c, "show_x_grid", 0, t_bpf, show_x_grid);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "show_x_grid", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "show_x_grid", 0, "onoff", "Show X Grid");

	CLASS_ATTR_LONG(c, "show_y_grid", 0, t_bpf, show_y_grid);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "show_y_grid", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "show_y_grid", 0, "onoff", "Show Y Grid");

	CLASS_ATTR_FLOAT_VARSIZE(c, "major_x_tics", 0, t_bpf, major_x_tics, num_major_x_tics, 1024);
	//CLASS_ATTR_ACCESSORS(c, "major_x_tics", NULL, bpf_majorXTicsSet);
	CLASS_ATTR_SAVE(c, "major_x_tics", 0);

	CLASS_ATTR_FLOAT_VARSIZE(c, "major_y_tics", 0, t_bpf, major_y_tics, num_major_y_tics, 1024);
	//CLASS_ATTR_ACCESSORS(c, "major_y_tics", NULL, bpf_majorYTicsSet);
	CLASS_ATTR_SAVE(c, "major_y_tics", 0);

	CLASS_ATTR_FLOAT_VARSIZE(c, "minor_x_tics", 0, t_bpf, minor_x_tics, num_minor_x_tics, 1024);
	//CLASS_ATTR_ACCESSORS(c, "minor_x_tics", NULL, bpf_minorXTicsSet);
	CLASS_ATTR_SAVE(c, "minor_x_tics", 0);

	CLASS_ATTR_FLOAT_VARSIZE(c, "minor_y_tics", 0, t_bpf, minor_y_tics, num_minor_y_tics, 1024);
	//CLASS_ATTR_ACCESSORS(c, "minor_y_tics", NULL, bpf_minorYTicsSet);
	CLASS_ATTR_SAVE(c, "minor_y_tics", 0);

	CLASS_ATTR_LONG(c, "snaptogrid", 0, t_bpf, snaptogrid);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "snaptogrid", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "snaptogrid", 0, "onoff", "Snap to Grid");

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

 	CLASS_ATTR_RGBA(c, "gridColor", 0, t_bpf, gridColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "gridColor", 0, "0. 0. 1. .75"); 
 	CLASS_ATTR_STYLE_LABEL(c, "gridColor", 0, "rgba", "Grid Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

	l_background = gensym("l_background");
	l_points = gensym("l_points");
	l_grid = gensym("l_grid");
	l_pos = gensym("l_pos");

	ps_int = gensym("int");
	ps_bpf = gensym("bpf");
	ps_notes = gensym("notes");

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

		x->npoints = (int *)calloc(MAX_NUM_FUNCTIONS, sizeof(int));
		memset(x->npoints, '\0', MAX_NUM_FUNCTIONS * sizeof(int));

		x->selected = NULL;
		x->sel_box = (t_rect){-1, -1, 0, 0};

		x->xmargin = XMARGIN;
		x->ymargin = YMARGIN;
		/*
		x->major_x_tics = (float *)sysmem_newptr(1024 * sizeof(float));
		x->major_y_tics = (float *)sysmem_newptr(1024 * sizeof(float));
		x->minor_x_tics = (float *)sysmem_newptr(1024 * sizeof(float));
		x->minor_y_tics = (float *)sysmem_newptr(1024 * sizeof(float));
		*/
 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
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
		post("num tics %d %d %d %d", x->num_major_x_tics, x->num_major_y_tics, x->num_minor_x_tics, x->num_minor_y_tics);
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
			bpf_insertPoint(x, pt, cf);
		}
	}
	return MAX_ERR_NONE;
}
/*
t_max_err bpf_majorXTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	post("%s %d", __PRETTY_FUNCTION__, argc);
	int i;
	x->num_major_x_tics = argc;
	for(i = 0; i < argc; i++){
		x->major_x_tics[i] = atom_getfloat(argv + i);
	}
	return MAX_ERR_NONE;
}

t_max_err bpf_majorYTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	post("%s %d", __PRETTY_FUNCTION__, argc);
	int i;
	x->num_major_y_tics = argc;
	for(i = 0; i < argc; i++){
		x->major_y_tics[i] = atom_getfloat(argv + i);
	}
	return MAX_ERR_NONE;
}


t_max_err bpf_minorXTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	post("%s %d", __PRETTY_FUNCTION__, argc);
	int i;
	x->num_minor_x_tics = argc;
	for(i = 0; i < argc; i++){
		x->minor_x_tics[i] = atom_getfloat(argv + i);
	}
	return MAX_ERR_NONE;
}

t_max_err bpf_minorYTicsSet(t_bpf *x, t_object *attr, long argc, t_atom *argv){
	post("%s %d", __PRETTY_FUNCTION__, argc);
	int i;
	x->num_minor_y_tics = argc;
	for(i = 0; i < argc; i++){
		x->minor_y_tics[i] = atom_getfloat(argv + i);
	}
	return MAX_ERR_NONE;
}
*/
