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
  NAME: tempo_editor
  DESCRIPTION: GUI for the tempocurver
  AUTHORS: John MacCallum 
  COPYRIGHT_YEARS: 2009 
  SVN_REVISION: $LastChangedRevision: 587 $ 
  VERSION 0.0: First try 
  VERSION 1.0.1: Fixed a crash that would occur due to the plan not being initialized
  VERSION 1.1: lots of updates leading up to ICMC 2010
  VERSION 1.2: enhancements to the grid system, better legend, muting, locking, background functions are now drawn lighter
  VERSION 1.2.1: fixed a bug in doClearFunction() which was clearing the current function, not the one passed to it.
  VERSION 1.2.2: mouse selection and dragging has been totally rewritten to allow multiple points to be selected at once
  VERSION 1.3: lots of improvements including cut/copy/paste and beat/control point numbers
  VERSION 1.3.1: flip and reverse selected points and signal outlets that output the control point numbers and beat numbers
  VERSION 1.3.2: dumpbeats now takes a list of subdivisions 
  VERSION 1.3.3: dumpbeats now outputs monotonic phase without resets at control points
  VERSION 1.3.4: a few minor bugfixes
  VERSION 1.3.5: function colors are now setable
  VERSION 1.3.6: fixed a bug with things not getting deselected, cut now no longer leaves the first point behind, and implemented pasteattime and pasteatcoords
  VERSION 1.3.7: colors implemented better
  VERSION 1.3.8: fixed a bug in the way that the unwrapped monotonic phase was being computed.
  VERSION 1.3.9: fixed a bug in the dumpbeats function
  VERSION 1.4: new selection code--boxes work better now.  beat highlighting can be reset at control points
  VERSION 1.4.1: more efficient drawing of the position marker and fixed a beat highlighting bug.  also new points can now be added at the boundaries of a selection box
  VERSION 1.4.2: more efficient drawing for real this time
  VERSION 1.5: better selection code, dumpbeats code rewritten to take an unordered list of key val sets, selectregion
  VERSION 1.5.1: mapx and mapy
  VERSION 1.5.2: duration column for cellblock and durations between the selected point and neighboring ones are drawn
  VERSION 1.6: the perform routine is now parallelized
  VERSION 1.6.1: fixed a bug with the cellblock communication code
  VERSION 1.6.2: fixed a bug in the dumpbeats function that was incorrectly computing beats around the first and last controlpoints
  VERSION 1.6.3: option/option-shift to move only along the x or y axes
  VERSION 1.6.4: fixed a bug in te_computeCorrectedPhase that would put a beat at a controlpoint even when the phase was not 0 at that point
  VERSION 1.6.5: fixed a bug in te_computeCorrectedMonotonicPhase() affecting the value of the phase at the control points
  VERSION 1.6.6: superdivs are now represented by negative numbers rather than values between 0 and 1
  VERSION 1.6.7: OSC bundles for each point are now output when changes are made to the interface
  VERSION 1.6.8: fixed a bug that would cause max to crash when timewarp~ was instantiated, and fixed another bug that would prevent the perform routine from being notified that the tempo map had changed.
  VERSION 1.6.9: scaletofit now takes function numbers to operate on and defaults to operating on all of them.  
  VERSION 1.6.10: /function/* can be used to address the parameters of all functions
  VERSION 1.6.11: minor bug fixes and outlets are now labeled
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
*/

/*
  todo:

  if the arrival and departure points don't overlap, the phase triangle for the departure point should be inside the circle

  there is a drawing bug with the beat at the arrival frequency point when it is different from the dep. freq.  sometimes it draws and sometimes not.

  should be able to click inside the departure freq circle

  look into a non-normalized incomplete beta function.  maybe it makes sense to rewrite the gsl one

  a list in the left inlet needs to be handled in a clever way to allow for any subset of the parameters

*/

//#include "version.h"
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_obex_util.h" 
#include "ext_critical.h"
#include "ext_sysparallel.h"
#include "ext_hashtab.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "z_dsp.h"
//#include "version.c"
//#include "gsl/gsl_sf.h"
//#include "gsl/gsl_errno.h"
#undef msg
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/beta.hpp>
#include <boost/math/tools/stats.hpp>
#include <boost/math/constants/constants.hpp>
//#include "cycle.h"

#include "osc.h"
#include "osc_bundle_s.h"
#include "osc_bundle_u.h"
#include "osc_message_s.h"
#include "osc_message_u.h"
#include "osc_byteorder.h"
//#define __OSC_PROFILE__
#include "osc_profile.h"
#include "osc_mem.h"
#include "omax_util.h"

#define MAX_NUM_FUNCTIONS 8
#define POINT_WIDTH 14
#define CONTROL_POINT_WIDTH 10

#define DEFAULT_ERROR_SPAN .8
#define DEFAULT_ERROR_OFFSET .1

#define BEFORE_FIRST_POINT 1
#define AFTER_LAST_POINT 2

#ifndef FLT_MIN
#define FLT_MIN 1.17549435e-38F
#endif

// to select the selection box
#define LEFT_EDGE 1
#define RIGHT_EDGE 2
#define TOP_EDGE 4
#define BOTTOM_EDGE 8
#define TOP_LEFT_CORNER LEFT_EDGE | TOP_EDGE
#define TOP_RIGHT_CORNER RIGHT_EDGE | TOP_EDGE
#define BOTTOM_LEFT_CORNER LEFT_EDGE | BOTTOM_EDGE
#define BOTTOM_RIGHT_CORNER RIGHT_EDGE | BOTTOM_EDGE
#define BOX_SELECTED 16

#define RANGE_SELECT 1
#define BOX_SELECT 2

#define DOWNSAMPLE 1

#define POSITION_UPDATE_RATE_MS 100

typedef struct _osc_point{
	char header[16];
	
	char function_size[4];
	char function_address[20];
	char function_typetags[4];
	char function_data[4];
	
	char time_size[4];
	char time_address[16];
	char time_typetags[4];
	char time_data[4];

	char atempo_size[4];
	char atempo_address[24];
	char atempo_typetags[4];
	char atempo_data[4];

	char dtempo_size[4];
	char dtempo_address[28];
	char dtempo_typetags[4];
	char dtempo_data[4];

	char aphase_size[4];
	char aphase_address[24];
	char aphase_typetags[4];
	char aphase_data[4];

	char dphase_size[4];
	char dphase_address[28];
	char dphase_typetags[4];
	char dphase_data[4];

	char alpha_size[4];
	char alpha_address[16];
	char alpha_typetags[4];
	char alpha_data[4];

	char beta_size[4];
	char beta_address[16];
	char beta_typetags[4];
	char beta_data[4];

	char ealpha_size[4];
	char ealpha_address[24];
	char ealpha_typetags[4];
	char ealpha_data[4];

	char ebeta_size[4];
	char ebeta_address[24];
	char ebeta_typetags[4];
	char ebeta_data[4];
} t_osc_point;

typedef struct _point{ 
 	t_pt coords; 
	double d_freq; // departure freq screen y
	double a_phase, d_phase; // arrival and departure phase
	double aux_points[2];
	// shape parameters for the beta distribution
	double alpha, beta;
	double error_alpha, error_beta;
	int whichPoint;  // 0=this point, >0=aux_points
	// if we are dragging a point and it is close to the y-coord of another in the same function,
	// we will put the other points coords here and then draw a line between them.
	struct _point *closest_point_with_same_ycoord;
	int draw_snapline;
 	struct _point *next; 
 	struct _point *prev; 
 	struct _point *next_selected; 
 	struct _point *prev_selected; 
	int selected;
} t_point; 

typedef struct _plan{ 
	int state;
	double startTime, endTime;
	double startFreq, endFreq;
	double startPhase, endPhase;
	double phaseError, phaseError_start;
	double correctionStart, correctionEnd;
	//double m, b;
	double alpha, beta;
	double error_alpha, error_beta;  // shape parameters for the beta distribution.  
	//unfortunately, they need to be here as well as with the point above
	double beta_ab, error_beta_ab; // we don't need to compute this every time if the values of a and b haven't changed.
	int pointnum_left, pointnum_right;
	int valid;
 	struct _plan *next; 
 	struct _plan *prev; 
} t_plan; 

typedef struct _te{ 
 	t_pxjbox box; 
 	void *out_osc, *out_cellblock;
	void *proxy;
	long in;
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
	float *last_y;  // in the perform routine, it's too expensive to compute the tempo so we'll do it manually
	float last_x;
 	int currentFunction; 
 	int numFunctions; 
	int hideFunctions[MAX_NUM_FUNCTIONS];
	int muteFunctions[MAX_NUM_FUNCTIONS];
	int lockFunctions[MAX_NUM_FUNCTIONS];
	t_jrgba functionColors[MAX_NUM_FUNCTIONS];
	t_plan *plans;  // used in the perform routine only
	int dirty; // set this to non-zero to indicate that a point has been added
	double time_min, time_max;
	double freq_min, freq_max;
	double error_span, error_offset; // these are the defaults that will be used when a new point is created
	int show_beat_correction;
	int show_tempo_correction;
	int show_x_axis, show_y_axis;
	int show_major_x_grid, show_minor_x_grid, show_major_y_grid, show_minor_y_grid;
	int show_beats;
	long show_beat_numbers;
	float major_grid_width_sec, major_grid_height_bps;
	float num_minor_x_grid_divisions, num_minor_y_grid_divisions;
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba phaseColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
	t_jrgba correctionColor;
	t_jrgba correctionColor_box;
	t_jrgba major_grid_line_color, minor_grid_line_color;
	float major_grid_line_width, minor_grid_line_width;
	t_symbol *name;
	double **ptrs;
	t_pt last_mouse;
	int mousestate;
	t_pt mousemove;
	int snap_to_grid;
	t_jsurface *surface;
	t_object *pv;
	int draw_mouse, draw_voice_legend;
	float fs;
	t_rect sel_box;
	unsigned long sel_box_selected;
	int sel_type;
	int *beatnumbuf; // temp array for storing indexes where beats are drawn
	int beatnumbuflen;
	int beatnumbufpos;
	int highlight_every_n;
	int reset_highlight_at_controlpoints;
	int apply_selection_box_to_all_functions_by_default;
	void *update_position_clock;
	float position_update_rate_ms;
	long drawposition;
	t_sysparallel_task *parallel_task;
	long parallel;
	t_int *w;
} t_te; 

t_class *te_class; 

t_symbol *ps_cellblock, *ps_pointNum, *ps_time, *ps_duration, *ps_dFreq, *ps_aFreq, *ps_dPhase, *ps_aPhase, *ps_alpha, *ps_beta, *ps_errorAlpha, *ps_errorBeta, *ps_error, *ps_FullPacket;

t_symbol *te_clipboard;

t_symbol *l_background, *l_xgrid, *l_ygrid, *l_xycoords, *l_legend, *l_xaxis, *l_yaxis, *l_position;
t_symbol *l_function_layers[MAX_NUM_FUNCTIONS];

t_critical te_clipboard_lock;

const char *params[] = {"/time", "/tempo/arrival", "/tempo/departure", "/phase/arrival", 
		  "/phase/departure", "/alpha", "/beta", "/error/alpha", "/error/beta"};
long offsets[] = {calcoffset(t_point, coords.x), calcoffset(t_point, coords.y), calcoffset(t_point, d_freq),
		  calcoffset(t_point, a_phase), calcoffset(t_point, d_phase), calcoffset(t_point, alpha),
		  calcoffset(t_point, beta), calcoffset(t_point, error_alpha), calcoffset(t_point, error_beta)};
double defaults[] = {0., 0., 0., 0., 0., 1., 1., 2., 2.};

t_osc_point _osc_point = (t_osc_point){
		"#bundle\0\0\0\0\0\0\0\0",
	
		"\0\0\0",
		"/selected/function\0",
		",i\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/time\0",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/tempo/arrival",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/tempo/departure\0\0",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/phase/arrival",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/phase/departure\0\0",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/alpha",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/beta\0",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/error/alpha\0\0",
		",f\0",
		"\0\0\0",

		"\0\0\0",
		"/selected/error/beta\0\0\0",
		",f\0",
		"\0\0\0"
	};

void te_paint(t_te *x, t_object *patcherview); 
t_int *te_perform(t_int *w);
int te_makePlan(t_te *x, float f, int function, t_plan *plan);
int te_isPlanValid(t_te *x, double time, t_plan *plan, int function);
void te_computePhaseError(t_te *x, t_plan *plan);
double te_betaPDF(double z, double a, double b, double beta_ab);
double te_betaCDF(double z, double a, double b, double beta_ab);
double te_betaCDFInt(double z, double a, double b, double beta_ab);
double te_scaledBetaCDFInt(double z, double a, double b, double scale, double offset, double beta_ab);
double te_computeCorrectedTempo(double t, t_plan *p);
double te_computeTempo(double t, t_plan *p);
double te_computePhase(double t, t_plan *p);
double te_computeCorrectedPhase(double t, t_plan *p);
double te_computeCorrectedUnwrappedMonotonicPhase(t_te *x, double t, int function, t_plan *plan);
void te_editSel(t_te *x, double xx, double yy, double zz);
void te_fullPacket(t_te *x, long len, long ptr);
void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_float(t_te *x, double f);
void te_find_btn(t_point *function, double x, t_point **left, t_point **right);
void te_reorderPoint(t_point *p);
void te_initReorderedPoint(t_te *x, t_point *p);
void te_swapPoints(t_point *p1, t_point *p2);
void te_removePoint(t_te *x, t_point *point, int functionNum);
void te_clearSelected(t_te *x);
void te_clearSelectionBox(t_te *x);
void te_applySelectionToAllFunctions(t_te *x);
void te_addPointsAtSelectionBoundariesForAllFunctions(t_te *x);
void te_addPointsAtSelectionBoundaries(t_te *x);
void te_addPointsAtSelectionBoundariesForFunction(t_te *x, int function);
void te_snapSelectionToGrid(t_te *x);
unsigned long te_selectBox(t_te *x, t_pt pt);
void te_selectPointsInBox(t_te *x, t_rect drawing_rect, t_rect sel_box, int function);
t_point *te_select(t_te *x, t_pt p);
void te_draw_bounds(t_te *x, t_jgraphics *g, t_rect *rect); 
void te_hideFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_muteFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_lockFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv);
double te_add(double a, double b);
double te_subtract(double a, double b);
double te_multiply(double a, double b);
double te_divide(double a, double b);
double (*te_getFunctionForSymbol(t_symbol *sym))(double, double);
void te_mapx(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_mapy(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_domap(t_te *x, t_symbol *msg, int argc, t_atom *argv, int xory, double *rect_coord_to_move, double *rect_dim);
void te_scaleToFit(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_scaleToFitX(t_te *x, int numfunctions, int *functions);
void te_scaleToFitY(t_te *x, int numfunctions, int *functions);
void te_addToFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_addToAllFunctions(t_te *x, double xx, double yy);
void te_getFunctionColor(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_setFunctionColor(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_invertSelected(t_te *x);
void te_reverseSelected(t_te *x);
void te_makeColorForInt(int i, t_jrgb *c);
void te_assist(t_te *x, void *b, long m, long a, char *s); 
void te_free(t_te *x); 
t_point *te_selectControlPoint(t_te *x, t_pt p);
t_max_err te_notify(t_te *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void te_mousedown(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mousedrag(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mousemove(t_te *x, t_object *patcherview, t_pt pt, long modifiers);
void te_repeatSelection(t_te *x);
void te_cut(t_te *x);
void te_copy(t_te *x);
void te_paste(t_te *x);
void te_pasteAtTime(t_te *x, double time);
void te_pasteAtCoords(t_te *x, double xx, double yy);
void te_findNearestGridPoint(t_te *x, t_pt pt_sc, t_pt *pt_out_sc);
void te_showgrid(t_te *x, long b);
void te_mouseup(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_addFunction(t_te *x); 
void te_setFunction(t_te *x, long f); 
double te_clip(double f, double min, double max);
double te_scale(double f, double min_in, double max_in, double min_out, double max_out);
void te_s2w(t_te *x, t_rect r, t_pt screen_coords, t_pt *world_coords); 
void te_w2s(t_te *x, t_rect r, t_pt world_coords, t_pt *screen_coords);
t_point *te_allocAndInsertPoint(t_te *x, t_pt coords, int functionNum);
void te_insertPoint(t_te *x, t_point *p, int functionNum);
void te_initPoint(t_te *x, t_point *p);
void te_dump(t_te *x);
void te_processArgs_r(t_te *x,
		      t_symbol *key,
		      long argc, 
		      t_atom *argv, 
		      long *numfunctions, 
		      long *functions, 
		      long *rangelen, 
		      double *range, 
		      long *numsubdivs, 
		      double *subdivs, 
		      long *numcontrolpoints, 
		      long *controlpoints);
void te_dumpBeats(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_dumpBeatsForFunction(t_te *x, 
			     long function, 
			     long numsubdivs, 
			     double *subdivs_atoms, 
			     double time_min, 
			     double time_max);
void te_dumpSelectedOSC(t_te *x);
void te_dumpCellblock(t_te *x);
void te_time_minmax(t_te *x, double min, double max);
void te_freq_minmax(t_te *x, double min, double max);
void te_getTimeMinMax(t_te *x);
void te_getFreqMinMax(t_te *x);
void te_clear(t_te *x);
void te_clearFunction(t_te *x, int f);
void te_clearRange(t_te *x, t_symbol *msg, int argc, t_atom *argv);
void te_clearRangeForFunction(t_te *x, int function, float timemin, float timemax);
void te_clearRangeForAllFunctions(t_te *x, float timemin, float timemax);
void te_clearCurrent(t_te *x);
void te_doClearFunction(t_te *x, int f);
void te_resetFunctionColors(t_te *x);
void te_invalidateAllFunctions(t_te *x);
void te_invalidateAll(t_te *x);
void te_run(t_te *x);
void te_dsp(t_te *x, t_signal **sp, short *count);
void te_postplan(t_plan *p, int (*print)(const char *, ...));
void te_postpoint(t_point *p, int(print)(const char *, ...));
t_symbol *te_mangleName(t_symbol *name, int i, int fnum);
void te_critical_enter(t_critical lock, int line);
void te_critical_exit(t_critical lock, int line);
int main(void); 
void *te_new(t_symbol *s, long argc, t_atom *argv); 
void te_dumpCellblockCallback(t_te *x, t_symbol *msg, int argc, t_atom *argv);

t_max_err te_functions_get(t_te *x, t_object *attr, long *argc, t_atom **argv);
t_max_err te_functions_set(t_te *x, t_object *attr, long argc, t_atom *argv);

t_max_err te_functionColorsGet(t_te *x, t_object *attr, long *argc, t_atom **argv);
t_max_err te_functionColorsSet(t_te *x, t_object *attr, long argc, t_atom *argv);

void te_paint(t_te *x, t_object *patcherview){ 
	x->pv = patcherview;
 	t_rect rect; 
 	t_jrgba c; 
 	c.red = c.green = c.blue = 0; 
 	c.alpha = 1.; 

 	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &rect); 

	t_jrgba pointcolor, phasecolor, tempocolor, beatcolor;
	struct dashstruct {double dash[2]; int len;} tempodash, beatdash, dash = {{4., 4.}, 2}, nodash = {NULL, 0};

	t_jrgba gray = (t_jrgba){0.5, 0.5, 0.5, 0.5};

	// draw background
	{
		t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_background, rect.width, rect.height);
		if(gg){

			// draw the outline of the box 


			jgraphics_set_source_jrgba(gg, &c); 
			jgraphics_set_line_width(gg, 1); 
			jgraphics_rectangle(gg, 0., 0., rect.width, rect.height); 
			jgraphics_stroke(gg); 

			jgraphics_set_source_jrgba(gg, &(x->bgcolor)); 
			jgraphics_rectangle(gg, 0., 0., rect.width, rect.height); 
			jgraphics_fill(gg); 
		}
		jbox_end_layer((t_object *)x, patcherview, l_background);
		jbox_paint_layer((t_object *)x, patcherview, l_background, 0, 0);
	}

	te_critical_enter(x->lock, __LINE__);


	// draw the x grid
	{
		if(/*x->show_major_x_grid && */x->major_grid_width_sec > 0. && x->major_grid_width_sec > ((x->time_max - x->time_min) / rect.width) / 2.){
			t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_xgrid, rect.width, rect.height);
			if(gg){
				double mgws = x->major_grid_width_sec;

				double xx = x->time_min + mgws;
				double x_sc;
				double minor_grid_width_sec = mgws / x->num_minor_x_grid_divisions;
				double minor_grid_width_sc = te_scale(minor_grid_width_sec, 0, x->time_max - x->time_min, 0., rect.width);
				while(xx < x->time_max){
					//xx = ceil(xx);
					x_sc = te_scale(xx, x->time_min, x->time_max, 0, rect.width);
					jgraphics_set_source_jrgba(gg, &(x->major_grid_line_color)); 
					if(x->show_major_x_grid){
						jgraphics_set_line_width(gg, x->major_grid_line_width);
						jgraphics_move_to(gg, x_sc, 0);
						jgraphics_line_to(gg, x_sc, rect.height);
						jgraphics_stroke(gg);
					}
					if(x->show_x_axis){
						if(!(x->show_major_x_grid)){
							jgraphics_set_line_width(gg, x->major_grid_line_width);
							jgraphics_move_to(gg, x_sc, 0);
							jgraphics_line_to(gg, x_sc, 10);
							jgraphics_stroke(gg);
						}
						char buf[128];
						sprintf(buf, "%.1f", xx);
						double w, h;
						jgraphics_text_measure(gg, buf, &w, &h);
						jgraphics_move_to(gg, x_sc + 2, h);
						jgraphics_show_text(gg, buf);
					}
					if(x->show_minor_x_grid){
						jgraphics_set_source_jrgba(gg, &(x->minor_grid_line_color)); 
						jgraphics_set_line_width(gg, x->minor_grid_line_width);

						int i;
						for(i = 0; i < x->num_minor_x_grid_divisions; i++){
							jgraphics_move_to(gg, x_sc + i * minor_grid_width_sc, 0); 
							jgraphics_line_to(gg, x_sc + i * minor_grid_width_sc, rect.height); 
							jgraphics_stroke(gg);
						}
					}
					xx = xx + mgws;
				}
			}
			jbox_end_layer((t_object *)x, patcherview, l_xgrid);
			jbox_paint_layer((t_object *)x, patcherview, l_xgrid, 0, 0);
		}
	}

	// draw the y grid
	{
		if(/*x->show_major_y_grid && */x->major_grid_height_bps > 0.){
			t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_ygrid, rect.width, rect.height);
			if(gg){
				double yy = x->freq_min;
				double y_sc;
				double minor_grid_height_bps = x->major_grid_height_bps / x->num_minor_y_grid_divisions;
				double minor_grid_height_sc = te_scale(minor_grid_height_bps, x->freq_min, x->freq_max, 0., rect.height);
				while(yy < x->freq_max){
					y_sc = te_scale(yy, x->freq_min, x->freq_max, rect.height, 0.);
					jgraphics_set_source_jrgba(gg, &(x->major_grid_line_color)); 
					if(x->show_major_y_grid){
						jgraphics_set_line_width(gg, x->major_grid_line_width);
						jgraphics_move_to(gg, 0., y_sc);
						jgraphics_line_to(gg, rect.width, y_sc);
						jgraphics_stroke(gg);
					}
					if(x->show_y_axis){
						if(!(x->show_major_y_grid)){
							jgraphics_set_line_width(gg, x->major_grid_line_width);
							jgraphics_move_to(gg, 0, y_sc);
							jgraphics_line_to(gg, 10, y_sc);
							jgraphics_stroke(gg);
						}
						char buf[128];
						sprintf(buf, "%.1f", yy);
						jgraphics_move_to(gg, 2, y_sc - 2);
						jgraphics_show_text(gg, buf);
					}
					if(x->show_minor_y_grid){
						jgraphics_set_source_jrgba(gg, &(x->minor_grid_line_color)); 
						jgraphics_set_line_width(gg, x->minor_grid_line_width);

						int i;
						for(i = 0; i < x->num_minor_y_grid_divisions; i++){
							jgraphics_move_to(gg, 0, y_sc - i * minor_grid_height_sc);
							jgraphics_line_to(gg, rect.width, y_sc - i * minor_grid_height_sc);
							jgraphics_stroke(gg);
						}
					}
					yy = yy + x->major_grid_height_bps;
				}
			}
			jbox_end_layer((t_object *)x, patcherview, l_ygrid);
			jbox_paint_layer((t_object *)x, patcherview, l_ygrid, 0, 0);
		}
	}

	int function;
	for(function = 0; function < x->numFunctions; function++){
		// set up colors
		beatcolor = x->functionColors[function];
		//te_makeColorForInt(function, (t_jrgb *)(&beatcolor));
		beatcolor.alpha = 1.;
		if(x->currentFunction == function){
			pointcolor = x->pointColor;
			phasecolor = x->phaseColor;
			tempocolor = x->lineColor;
			tempodash = nodash;
			beatdash = nodash;
		}else{
			pointcolor = gray;
			phasecolor = gray;
			tempocolor = gray;
			beatcolor.alpha = 0.5;
			tempodash = dash;
			beatdash = dash;
		}
		t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_function_layers[function], rect.width, rect.height);
		if(gg){
			jgraphics_set_line_width(gg, 1.);
			// draw the beat lines
			if(x->show_beats){
				jgraphics_set_source_jrgba(gg, &beatcolor);
				jgraphics_set_dash(gg, beatdash.dash, beatdash.len, 0);
				int i;
				double prev_t = te_scale(-1, 0, rect.width, x->time_min, x->time_max);
				t_plan plan;
				if(x->functions[function] == NULL || x->hideFunctions[function]){
					continue;
				}

				te_makePlan(x, prev_t, function, &plan);
				double prev_correctedPhase = te_computeCorrectedPhase(prev_t, &plan);
				//double prev_phase = te_computePhase(prev_t, &plan);
				//int beatnum = 1;
				//int newplan = 0;
				int lbnx = -100, lbny = -100;
				int highlight = 0;
				for(i = 0; i < rect.width * 1; i++){
					double idx = i / 1;
					double t = te_scale(idx, 0, rect.width, x->time_min, x->time_max);
					double p;
					// the display width really isn't of good enough resolution to get 
					// the beats accurately, so we're adding a small amount to the
					// time to make sure that the correct beat number shows up.
					// it might be interesting to compute more than the rect width...
					if(!te_isPlanValid(x, t + ((x->time_max - x->time_min) / rect.width), &plan, function)){
						te_makePlan(x, t + ((x->time_max - x->time_min) / rect.width), function, &plan);
					}
					/*
					  if(x->show_beat_correction && function == x->currentFunction){//&& t >= plan.startTime + plan.segmentDuration_sec){
					  jgraphics_set_source_jrgba(gg, &c);
					  p = te_computePhase(t, &plan);
					  if((p - floor(p)) < (prev_phase - floor(prev_phase))){
					  jgraphics_move_to(gg, idx, rect.height);
					  jgraphics_line_to(gg, idx, te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
					  jgraphics_stroke(gg);
					  }
					  prev_phase = p;
					  jgraphics_set_source_jrgba(gg, &(x->correctionColor));
					  }else{
					  if(function == x->currentFunction){
					  jgraphics_set_source_jrgba(gg, &c);
					  }else{
					  jgraphics_set_source_jrgba(gg, &c);
					  }
					  }
					*/

					p = te_computeCorrectedPhase(t, &plan);
					if(floor(p) > floor(prev_correctedPhase) || p - floor(p) < prev_correctedPhase - floor(prev_correctedPhase)){
						//if((highlight % x->highlight_every_n) == 0 && function == x->currentFunction){
						if(x->reset_highlight_at_controlpoints){
							if((lrintf(p) % x->highlight_every_n) == 0 && function == x->currentFunction){
								jgraphics_set_line_width(gg, 3.);
							}
						}else{
							double mp = te_computeCorrectedUnwrappedMonotonicPhase(x, t + ((x->time_max - x->time_min) / rect.width), function, &plan);
							if((lrintf(mp) % x->highlight_every_n) == 0 && function == x->currentFunction){
								jgraphics_set_line_width(gg, 3.);
							}
						}
						highlight++;
						jgraphics_move_to(gg, idx, rect.height);
						double beatheight = te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0);
						jgraphics_line_to(gg, idx, beatheight);
						jgraphics_stroke(gg);
						jgraphics_set_line_width(gg, 1.);
						if(function == x->currentFunction){	
							// draw the beat number over the beat
							char buf[8];
							double w, h;
							long beatnum = lrintf(p); // cast to round down
							beatnum++;
							if(beatnum == 1){
								if(t >= plan.endTime){
									sprintf(buf, "%d", plan.pointnum_right);
								}else{
									sprintf(buf, "%d", plan.pointnum_left);
								}
								jgraphics_text_measure(gg, buf, &w, &h);
								jgraphics_move_to(gg, idx - (w / 2), beatheight - 25);
								jgraphics_show_text(gg, buf);
								jgraphics_rectangle(gg, idx - w, beatheight - 25 - h, w * 2, h * 1.3);
								jgraphics_stroke(gg);
								lbnx = idx + (POINT_WIDTH / 2);
							}else{
								sprintf(buf, "%ld", beatnum);
								jgraphics_text_measure(gg, buf, &w, &h);
								if((idx - (w / 2)) - lbnx > 2 || (beatheight - 4) - lbny > (h / 2) + 2){
									jgraphics_move_to(gg, idx - (w / 2), beatheight - 4);
									jgraphics_show_text(gg, buf);
									jgraphics_ellipse(gg, idx - 2, beatheight - 2, 4, 4);
									jgraphics_fill(gg);
									lbnx = idx + (w / 2);
									lbny = beatheight - 4;
								}
							}
							//beatnum++;
						}
					}

					prev_t = t;
					prev_correctedPhase = p;
				}
			}

			// draw the tempo lines
			{
				jgraphics_set_source_jrgba(gg, &tempocolor);
				jgraphics_set_dash(gg, tempodash.dash, tempodash.len, 0);
				if(x->hideFunctions[function]){
					continue;
				}

				float kamt = 2.;
				if(x->currentFunction != function){
					kamt = 0.25;
				}
				t_point *p = x->functions[function];
				while(p){
					if(!(p->next)){
						break;
					}
					int j;
					float k;
					t_plan plan;
					t_pt sc;
					te_w2s(x, rect, p->coords, &sc);
					te_makePlan(x, p->coords.x + ((x->time_max - x->time_min) / rect.width), function, &plan);
					double freq = te_computeTempo(plan.startTime, &plan);
					double freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
					jgraphics_move_to(gg, sc.x, freq_sc);
					double endTime_sc = te_scale(plan.endTime, x->time_min, x->time_max, 0., rect.width);
					for(j = sc.x, k = 0; j < endTime_sc; j++, k += kamt){
						// if we will show the correction, we should draw the uncorrected line here
						freq = te_computeTempo(te_scale(j, 0, rect.width, x->time_min, x->time_max), &plan);
						freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
						if(!((int)floorf(k) % 2)){
							jgraphics_line_to(gg, j, freq_sc);
							jgraphics_stroke(gg);
						}
						jgraphics_move_to(gg, j, freq_sc);
					}
					p = p->next;
				}
			}

			// draw points 
			{ 
				jgraphics_set_dash(gg, NULL, 0, 0);
				double ps = POINT_WIDTH; // point size
				double ps2 = ps / 2.; // point size divided by 2
				if(x->hideFunctions[function]){
					continue;
				}
				t_point *p = x->functions[function];
				double xx, yy;
				while(p){
					//if(p == x->selected){
					if(p->selected){
						jgraphics_set_source_jrgba(gg, &(x->selectionColor));
					}else{
						jgraphics_set_source_jrgba(gg, &pointcolor);
					}
					t_pt sc;
					te_w2s(x, rect, p->coords, &sc);

					// departure circle
					double d_freq_sc = te_scale(p->d_freq, x->freq_min, x->freq_max, rect.height, 0);
					jgraphics_ellipse(gg, sc.x - ps2 - 2, d_freq_sc - ps2 - 2, ps + 4, ps + 4);
					jgraphics_stroke(gg);

					// arrival circle
					jgraphics_ellipse(gg, sc.x - ps2, sc.y - ps2, ps, ps);
					jgraphics_fill(gg);

					// departure phase tick
					jgraphics_set_source_jrgba(gg, &phasecolor);

					xx = (ps2 + 4) * sin(2 * M_PI * p->d_phase);
					yy = (ps2 + 4) * cos(2 * M_PI * p->d_phase);
					jgraphics_move_to(gg, sc.x + xx, d_freq_sc - yy);

					xx = (6 + ps2 + 4) * sin(2 * M_PI * (p->d_phase - .03));
					yy = (6 + ps2 + 4) * cos(2 * M_PI * (p->d_phase - .03));
					jgraphics_line_to(gg, sc.x + xx, d_freq_sc - yy);
					xx = (6 + ps2 + 4) * sin(2 * M_PI * (p->d_phase + .03));
					yy = (6 + ps2 + 4) * cos(2 * M_PI * (p->d_phase + .03));
					jgraphics_line_to(gg, sc.x + xx, d_freq_sc - yy);

					jgraphics_close_path(gg);
					jgraphics_fill(gg);


					// arrival phase tick
					xx = (ps2) * sin(2 * M_PI * p->a_phase);
					yy = (ps2) * cos(2 * M_PI * p->a_phase);
					jgraphics_move_to(gg, sc.x + xx, sc.y - yy);

					xx = (ps2 - 4) * sin(2 * M_PI * (p->a_phase - .25));
					yy = (ps2 - 4) * cos(2 * M_PI * (p->a_phase - .25));
					jgraphics_line_to(gg, sc.x + xx, sc.y - yy);
					xx = (ps2 - 4) * sin(2 * M_PI * (p->a_phase + .25));
					yy = (ps2 - 4) * cos(2 * M_PI * (p->a_phase + .25));
					jgraphics_line_to(gg, sc.x + xx, sc.y - yy);

					jgraphics_close_path(gg);
					jgraphics_fill(gg);

					p = p->next;
				}

				// coordinates above point being moved
				//if(x->mousestate && x->selected){
				if((x->mousestate == 0x110 || x->mousestate == 0x10) && x->currentFunction == function){
					t_point *p = x->selected;
					while(p){
						if(p->whichPoint){
							p = p->next_selected;
							continue;
						}
						char buf[128];
						t_pt pt = p->coords;
						t_pt sc;
						te_w2s(x, rect, pt, &sc);
						sprintf(buf, "%.2f %.2f", pt.x, pt.y);
						double w, h;
						jgraphics_text_measure(gg, buf, &w, &h);
						t_pt topleft = (t_pt){0., 0.};
						double pad = 2;
						topleft = (t_pt){sc.x - (w / 2), sc.y - 45};

						jgraphics_set_source_jrgba(gg, &(x->bgcolor));
						jgraphics_rectangle(gg, topleft.x, topleft.y - h + pad, w, h);
						jgraphics_fill(gg);
						t_jrgba black = (t_jrgba){0., 0., 0., 1.};
						jgraphics_set_source_jrgba(gg, &black);
						jgraphics_move_to(gg, topleft.x, topleft.y);
						jgraphics_show_text(gg, buf);

						p = p->next_selected;
					}
				}
			} 

			// draw the tempo correction line and control points
			{
				if(x->show_tempo_correction){
					jgraphics_set_source_jrgba(gg, &(x->correctionColor));
					if(function != x->currentFunction || x->hideFunctions[function]){
						continue;
					}
					t_point *p = x->functions[function];
					while(p){
						if(!(p->next)){
							break;
						}
						double correctionStart_sc, correctionEnd_sc;
						int j;
						t_plan plan;
						te_makePlan(x, p->coords.x + ((x->time_max - x->time_min) / rect.width), function, &plan);
						double scx, nscx;
						//printf("%f %f \n", p->coords.x, p->next->coords.x);
						scx = te_scale(p->coords.x, x->time_min, x->time_max, 0, rect.width);
						nscx = te_scale(p->next->coords.x, x->time_min, x->time_max, 0, rect.width);

						//printf("%f %f %f %f\n", p->aux_points[0], p->aux_points[1], scx, nscx);
						correctionStart_sc = te_scale(p->aux_points[0], 0., 1., scx, nscx);
						correctionEnd_sc = te_scale(p->aux_points[1], 0., 1., scx, nscx);
						if(correctionStart_sc < 0){
							correctionStart_sc = 0.;
						}
						if(correctionEnd_sc > rect.width){
							correctionEnd_sc = rect.width;
						}
						double freq = te_computeTempo(plan.correctionStart, &plan);
						double freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
						double freq_sc1 = freq_sc;

						jgraphics_set_source_jrgba(gg, &(x->correctionColor));
						jgraphics_move_to(gg, correctionStart_sc, freq_sc);
						//printf("%f %f\n", correctionStart_sc, correctionEnd_sc);
						for(j = correctionStart_sc; j < correctionEnd_sc; j++){
							freq = te_computeCorrectedTempo(te_scale(j, 0, rect.width, x->time_min, x->time_max), &plan);
							freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
							jgraphics_line_to(gg, j, freq_sc);
							jgraphics_move_to(gg, j, freq_sc);
						}
						jgraphics_stroke(gg);
						jgraphics_ellipse(gg, correctionStart_sc - 2, freq_sc1 - 2, 4, 4);
						jgraphics_ellipse(gg, correctionEnd_sc - 2, freq_sc - 2, 4, 4);
						jgraphics_fill(gg);
						/*
						  jgraphics_set_source_jrgba(gg, &(x->correctionColor_box));
						  jgraphics_move_to(gg, correctionStart_sc, rect.height);
						  jgraphics_line_to(gg, correctionStart_sc, freq_sc1);
						  jgraphics_line_to(gg, correctionEnd_sc, freq_sc);
						  jgraphics_line_to(gg, correctionEnd_sc, rect.height);

						  jgraphics_fill(gg);
						*/
						p = p->next;
					}
				}
			}
		}
		jbox_end_layer((t_object *)x, patcherview, l_function_layers[function]);
	}

	for(function = 0; function < x->numFunctions; function++){
		jbox_paint_layer((t_object *)x, patcherview, l_function_layers[function], 0, 0);
	}

	// draw lines from the selected points to any visible point that has the same y coord
	{
		if(x->selected){
			t_point *p = x->selected;
			while(p){
				if(p->draw_snapline && p->closest_point_with_same_ycoord){
					t_jrgba blue = (t_jrgba){0., 0., 1., 1.};
					jgraphics_set_source_jrgba(g, &blue);
					jgraphics_set_line_width(g, .5);
					t_pt sc;
					te_w2s(x, rect, p->coords, &sc);
					if(p->coords.x < p->closest_point_with_same_ycoord->coords.x){
						jgraphics_move_to(g, sc.x + POINT_WIDTH - 4, sc.y);
						te_w2s(x, rect, p->closest_point_with_same_ycoord->coords, &sc);
						jgraphics_line_to(g, sc.x - POINT_WIDTH + 4, sc.y);
					}else{
						jgraphics_move_to(g, sc.x - POINT_WIDTH + 4, sc.y);
						te_w2s(x, rect, p->closest_point_with_same_ycoord->coords, &sc);
						jgraphics_line_to(g, sc.x + POINT_WIDTH - 4, sc.y);
					}
					jgraphics_stroke(g);
				}
				p = p->next_selected;
			}
		}
	}

#define XY_COORDS_Y_OFFSET 10
	/*
	// draw the x,y coords of the mouse
	if(x->draw_mouse){
	char buf[32];
	double w, h;
	sprintf(buf, "%.2f sec, %.2f bps", te_scale(x->mousemove.x, 0, rect.width, x->time_min, x->time_max), te_scale(x->mousemove.y, rect.height, 0, x->freq_min, x->freq_max));
	jgraphics_set_font_size(g, 10);
	jgraphics_text_measure(g, buf, &w, &h);
	t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_xycoords, w, h);
	if(gg){
	jgraphics_set_source_jrgba(gg, &(x->bgcolor));
	jgraphics_rectangle(gg, 0, 0, w, h);
	jgraphics_fill(gg);
	jgraphics_set_source_jrgba(gg, &(x->lineColor));
	jgraphics_move_to(gg, 0, 8);
	jgraphics_set_font_size(gg, 10);
	jgraphics_show_text(gg, buf);
	}
	jbox_end_layer((t_object *)x, patcherview, l_xycoords);
	jbox_paint_layer((t_object *)x, patcherview, l_xycoords, rect.width - w - 4, XY_COORDS_Y_OFFSET);
	}
	*/
	// draw legend
#define COL_HEIGHT 15
	if(x->draw_voice_legend){
		t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_legend, 100, (x->numFunctions - 1) * COL_HEIGHT + 20);
		if(gg){
			int i;
			t_jrgba c;
			jgraphics_set_source_jrgba(gg, &(x->bgcolor));
			jgraphics_rectangle(gg, 0, 0, 100, (x->numFunctions - 1) * COL_HEIGHT + 20);
			jgraphics_fill(gg);
			for(i = 0; i < x->numFunctions; i++){
				//te_makeColorForInt(i, (t_jrgb *)(&c));
				c = x->functionColors[i];
				c.alpha = 1.;
				jgraphics_set_source_jrgba(gg, &c);
				jgraphics_set_line_width(gg, 1.); 
				if(x->currentFunction == i){
					jgraphics_set_dash(gg, NULL, 0, 0);
				}else{
					jgraphics_set_dash(gg, (double[2]){4., 4.}, 2, 0);
				}
				jgraphics_move_to(gg, 0, i * COL_HEIGHT + 12);
				jgraphics_line_to(gg, 25, i * COL_HEIGHT + 12);
				jgraphics_stroke(gg);
				if(x->currentFunction == i){
					jgraphics_set_source_jrgba(gg, &(x->lineColor));
				}else{
					t_jrgba gray = (t_jrgba){0.5, 0.5, 0.5, 0.5};
					jgraphics_set_source_jrgba(gg, &gray);
				}
				char buf[32];
				double w, h;
				sprintf(buf, "f%d", i);
				jgraphics_set_font_size(gg, 12);
				jgraphics_text_measure(gg, buf, &w, &h);
				jgraphics_move_to(gg, 30, i * COL_HEIGHT + 16);
				jgraphics_show_text(gg, buf);
				jgraphics_set_dash(gg, NULL, 0, 0);
				/*
				  if(x->currentFunction == i){
				  t_jrgba red = (t_jrgba){1., 0., 0., .75};
				  jgraphics_set_source_jrgba(gg, &red);
				  jgraphics_ellipse(gg, 40, i * COL_HEIGHT + COL_HEIGHT, 4, 4);
				  jgraphics_fill(gg);
				  jgraphics_set_source_jrgba(gg, &gray);
				  jgraphics_ellipse(gg, 40, i * COL_HEIGHT + COL_HEIGHT, 4, 4);
				  jgraphics_stroke(gg);
				  }else{
				  jgraphics_set_source_jrgba(gg, &gray);
				  jgraphics_ellipse(gg, 40, i * COL_HEIGHT + COL_HEIGHT, 4, 4);
				  jgraphics_stroke(gg);
				  }
				*/
				// lock
				gray.alpha = 1.;
				jgraphics_set_source_jrgba(gg, &gray);
				jgraphics_rectangle(gg, 50, i * COL_HEIGHT + 10, 7, 6);
				jgraphics_fill(gg);
				if(x->lockFunctions[i]){
					jgraphics_arc(gg, 53.5, i * COL_HEIGHT + 10, 2, M_PI, 0);
				}else{
					jgraphics_arc(gg, 56, i * COL_HEIGHT + 10, 2, M_PI, 0);
				}
				jgraphics_stroke(gg);

				// mute
				int speaker = 10;
				jgraphics_move_to(gg, 70, i * COL_HEIGHT + speaker);
				jgraphics_line_to(gg, 74, i * COL_HEIGHT + speaker);
				jgraphics_line_to(gg, 78, i * COL_HEIGHT + speaker - 4);
				jgraphics_line_to(gg, 78, i * COL_HEIGHT + speaker + 8);
				jgraphics_line_to(gg, 74, i * COL_HEIGHT + speaker + 4);
				jgraphics_line_to(gg, 70, i * COL_HEIGHT + speaker + 4);
				jgraphics_close_path(gg);
				jgraphics_fill(gg);

				jgraphics_fill(gg);
				if(!(x->muteFunctions[i])){
					jgraphics_arc(gg, 78, i * COL_HEIGHT + speaker + 2, 2, 1.5 * M_PI, .5 * M_PI);
					jgraphics_arc(gg, 78, i * COL_HEIGHT + speaker + 2, 4, 1.55 * M_PI, .45 * M_PI);
					jgraphics_arc(gg, 78, i * COL_HEIGHT + speaker + 2, 6, 1.6 * M_PI, .4 * M_PI);
					jgraphics_stroke(gg);
				}
			}
		}
		jbox_end_layer((t_object *)x, patcherview, l_legend);
		jbox_paint_layer((t_object *)x, patcherview, l_legend, rect.width - 100, XY_COORDS_Y_OFFSET + COL_HEIGHT);
	}

	// draw the selection box
	{
		t_jrgba c = x->selectionColor;
		jgraphics_set_line_width(g, 3.);
		c.alpha = 1.;
		jgraphics_set_source_jrgba(g, &c);
		double sbx = te_scale(x->sel_box.x, x->time_min, x->time_max, 0, rect.width);
		double sby = te_scale(x->sel_box.y, x->freq_min, x->freq_max, 0, rect.height);
		double sbw = te_scale(x->sel_box.width, x->time_min, x->time_max, 0, rect.width);
		double sbh = te_scale(x->sel_box.height, x->freq_min, x->freq_max, 0, rect.height);
		jgraphics_rectangle(g, sbx, sby, sbw, sbh);
		jgraphics_stroke(g);
		jgraphics_set_line_width(g, 1.);
		c.alpha = .3;
		jgraphics_set_source_jrgba(g, &c);
		jgraphics_rectangle(g, sbx, sby, sbw, sbh);
		jgraphics_fill(g);
	}

	// draw the position
	{
		float xx = te_scale(x->last_x, x->time_min, x->time_max, 0, rect.width);
		t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_position, 10, rect.height);
		if(gg){
			if(x->last_x >= x->time_min && x->last_x <= x->time_max && x->drawposition){
				t_jrgba black = (t_jrgba){0., 0., 0., 1.};
				jgraphics_set_source_jrgba(gg, &black);
				jgraphics_set_line_width(gg, 2.);
				jgraphics_move_to(gg, 5, 0);
				jgraphics_line_to(gg, 5, rect.height);
				jgraphics_stroke(gg);
			}
			jbox_end_layer((t_object *)x, patcherview, l_position);
		}
		jbox_paint_layer((t_object *)x, patcherview, l_position, xx - 5, 0);
	}

	// draw the distance from the first selected point to its neighboring ones
	{
		t_point *p = x->selected;
		if(p && x->mousestate){
			t_jrgba c = x->bgcolor;
			t_jrgba black = (t_jrgba){0., 0., 0., 1.};
			t_jrgba blue = (t_jrgba){0., 0., 1., 1.};
			char buf[64];
			double w, h;
			t_pt sc;
			te_w2s(x, rect, p->coords, &sc);
			int drawmiddle = 0;
			if(p->next){
				t_pt nsc;
				te_w2s(x, rect, p->next->coords, &nsc);

				jgraphics_set_source_jrgba(g, &c);
				jgraphics_rectangle(g, sc.x + 4, rect.height - 22, nsc.x - sc.x - 8, 22);
				jgraphics_fill(g);

				jgraphics_set_source_jrgba(g, &blue);
				jgraphics_move_to(g, sc.x + 5, rect.height - 10);
				jgraphics_line_to(g, nsc.x - 5, rect.height - 10);
				jgraphics_stroke(g);
				jgraphics_move_to(g, nsc.x - 5, rect.height - 10);
				jgraphics_line_to(g, nsc.x - 10, rect.height - 15);
				jgraphics_line_to(g, nsc.x - 10, rect.height - 5);
				jgraphics_close_path(g);
				jgraphics_fill(g);

				jgraphics_move_to(g, sc.x + 5, rect.height - 10);
				jgraphics_line_to(g, sc.x + 10, rect.height - 15);
				jgraphics_line_to(g, sc.x + 10, rect.height - 5);
				jgraphics_close_path(g);
				jgraphics_fill(g);

				jgraphics_set_source_jrgba(g, &black);
				sprintf(buf, "%.2f", (p->next->coords.x - p->coords.x));
				jgraphics_text_measure(g, buf, &w, &h);
				jgraphics_move_to(g, ((nsc.x - sc.x) / 2) + sc.x - (w / 2), rect.height - 12);
				jgraphics_show_text(g, buf);

				jgraphics_set_line_width(g, 4);
				jgraphics_set_source_jrgba(g, &blue);
				jgraphics_move_to(g, nsc.x, rect.height);
				jgraphics_line_to(g, nsc.x, nsc.y + (POINT_WIDTH / 2));
				jgraphics_stroke(g);

				drawmiddle = 1;

			}
			jgraphics_set_line_width(g, 1.);
			if(p->prev){
				t_pt nsc;
				te_w2s(x, rect, p->prev->coords, &nsc);

				jgraphics_set_source_jrgba(g, &c);
				jgraphics_rectangle(g, sc.x - 4, rect.height - 22, nsc.x - sc.x + 4, 22);
				jgraphics_fill(g);

				jgraphics_set_source_jrgba(g, &blue);
				jgraphics_move_to(g, sc.x - 5, rect.height - 10);
				jgraphics_line_to(g, nsc.x + 5, rect.height - 10);
				jgraphics_stroke(g);
				jgraphics_move_to(g, nsc.x + 5, rect.height - 10);
				jgraphics_line_to(g, nsc.x + 10, rect.height - 15);
				jgraphics_line_to(g, nsc.x + 10, rect.height - 5);
				jgraphics_close_path(g);
				jgraphics_fill(g);

				jgraphics_move_to(g, sc.x - 5, rect.height - 10);
				jgraphics_line_to(g, sc.x - 10, rect.height - 15);
				jgraphics_line_to(g, sc.x - 10, rect.height - 5);
				jgraphics_close_path(g);
				jgraphics_fill(g);

				jgraphics_set_source_jrgba(g, &c);
				jgraphics_rectangle(g, ((nsc.x - sc.x) / 2.) + sc.x - (w / 2), rect.height - 12 - h, w, h);
				jgraphics_fill(g);

				jgraphics_set_source_jrgba(g, &black);
				sprintf(buf, "%.2f", (p->coords.x - p->prev->coords.x));
				jgraphics_text_measure(g, buf, &w, &h);
				jgraphics_move_to(g, ((nsc.x - sc.x) / 2) + sc.x - (w / 2), rect.height - 12);
				jgraphics_show_text(g, buf);

				jgraphics_set_line_width(g, 4);
				jgraphics_set_source_jrgba(g, &blue);
				jgraphics_move_to(g, nsc.x, rect.height);
				jgraphics_line_to(g, nsc.x, nsc.y + (POINT_WIDTH / 2));
				jgraphics_stroke(g);

				drawmiddle = 1;
			}
			jgraphics_set_line_width(g, 4);
			jgraphics_set_source_jrgba(g, &blue);
			jgraphics_move_to(g, sc.x, rect.height);
			jgraphics_line_to(g, sc.x, sc.y + (POINT_WIDTH / 2));
			jgraphics_stroke(g);
		}
	}

	te_critical_exit(x->lock, __LINE__);
}

void te_updatePositionCallback(t_te *x){
	if(x->drawposition){
		jbox_invalidate_layer((t_object *)x, x->pv, l_position);
		jbox_redraw((t_jbox *)&(x->box));
	}
	clock_fdelay(x->update_position_clock, x->position_update_rate_ms);   
}

void te_perform64(t_te *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long vectorsize, long flags, void *userparam)
{
	//t_te *x = (t_te *)w[1];
	if(!(x->w)){
		x->w = (t_int *)calloc(9, sizeof(t_int));
	}
	x->w[1] = (t_int)x;
	x->w[2] = (t_int)vectorsize;
	x->w[3] = (t_int)ins[0];
	x->w[4] = (t_int)outs[0];
	x->w[5] = (t_int)outs[1];
	x->w[6] = (t_int)outs[2];
	x->w[7] = (t_int)outs[3];
	x->w[8] = (t_int)outs[4];
	//x->w = w;
	te_run(x);

	double *out_phase_wrapped = outs[0];//(t_float *)w[4];
	double *out_phase = outs[1];//(t_float *)w[5];
	double *out_bps = outs[2];//(t_float *)w[6];
	double *out_pointnum = outs[3];//(t_float *)w[7];
	double *out_beatnum = outs[4];//(t_float *)w[8];

	int n = vectorsize;//(int)w[2];

	memcpy(out_phase_wrapped, x->ptrs[x->currentFunction * 5], n * sizeof(double));
	memcpy(out_phase, x->ptrs[x->currentFunction * 5 + 1], n * sizeof(double));
	memcpy(out_bps, x->ptrs[x->currentFunction * 5 + 2], n * sizeof(double));
	memcpy(out_pointnum, x->ptrs[x->currentFunction * 5 + 3], n * sizeof(double));
	memcpy(out_beatnum, x->ptrs[x->currentFunction * 5 + 4], n * sizeof(double));

	//return w + 9;
}

void te_dsp64(t_te *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	int j;
	t_symbol *name1, *name2, *name3, *name4, *name5;
	x->fs = samplerate;
	if(count[0]){
		for(j = 0; j < x->numFunctions; j++){
			if(name1 = te_mangleName(x->name, 0, j)){
				name1->s_thing = (t_object *)(x->ptrs[j * 5]);
			}
			if(name2 = te_mangleName(x->name, 1, j)){
				name2->s_thing = (t_object *)(x->ptrs[(j * 5) + 1]);
			}
			if(name3 = te_mangleName(x->name, 2, j)){
				name3->s_thing = (t_object *)(x->ptrs[(j * 5) + 2]);
			}
			if(name4 = te_mangleName(x->name, 3, j)){
				name4->s_thing = (t_object *)(x->ptrs[(j * 5) + 3]);
			}
			if(name5 = te_mangleName(x->name, 4, j)){
				name5->s_thing = (t_object *)(x->ptrs[(j * 5) + 4]);
			}
		}
		for(j = x->numFunctions; j < MAX_NUM_FUNCTIONS; j++){
			if(name1 = te_mangleName(x->name, 0, j)){
				name1->s_thing = NULL;
			}
			if(name2 = te_mangleName(x->name, 1, j)){
				name2->s_thing = NULL;
			}
			if(name3 = te_mangleName(x->name, 2, j)){
				name3->s_thing = NULL;
			}
			if(name4 = te_mangleName(x->name, 3, j)){
				name4->s_thing = NULL;
			}
			if(name5 = te_mangleName(x->name, 4, j)){
				name5->s_thing = NULL;
			}
		}
		object_method(dsp64, gensym("dsp_add64"), x, te_perform64, 0, NULL);
	}
}

void te_dsp(t_te *x, t_signal **sp, short *count){
	int j;
	t_symbol *name1, *name2, *name3, *name4, *name5;
	x->fs = sp[0]->s_sr;
	if(count[0]){
		for(j = 0; j < x->numFunctions; j++){
			if(name1 = te_mangleName(x->name, 0, j)){
				name1->s_thing = (t_object *)(x->ptrs[j * 5]);
			}
			if(name2 = te_mangleName(x->name, 1, j)){
				name2->s_thing = (t_object *)(x->ptrs[(j * 5) + 1]);
			}
			if(name3 = te_mangleName(x->name, 2, j)){
				name3->s_thing = (t_object *)(x->ptrs[(j * 5) + 2]);
			}
			if(name4 = te_mangleName(x->name, 3, j)){
				name4->s_thing = (t_object *)(x->ptrs[(j * 5) + 3]);
			}
			if(name5 = te_mangleName(x->name, 4, j)){
				name5->s_thing = (t_object *)(x->ptrs[(j * 5) + 4]);
			}
		}
		for(j = x->numFunctions; j < MAX_NUM_FUNCTIONS; j++){
			if(name1 = te_mangleName(x->name, 0, j)){
				name1->s_thing = NULL;
			}
			if(name2 = te_mangleName(x->name, 1, j)){
				name2->s_thing = NULL;
			}
			if(name3 = te_mangleName(x->name, 2, j)){
				name3->s_thing = NULL;
			}
			if(name4 = te_mangleName(x->name, 3, j)){
				name4->s_thing = NULL;
			}
			if(name5 = te_mangleName(x->name, 4, j)){
				name5->s_thing = NULL;
			}
		}
		dsp_add (te_perform, 8, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec);
	}
}

void te_dspstate(t_te *x, long state){
	if(state){
		clock_fdelay(x->update_position_clock, x->position_update_rate_ms);
	}else{
		clock_unset(x->update_position_clock);
	}
}

void te_workerproc(t_sysparallel_worker *worker){
	t_te *x = *((t_te **)(worker->data));
	t_int *w = x->w;
	int n = (int)w[2];
	//t_float *in = (t_float *)w[3];
	double *in = (double *)w[3];

	int numfunctions = x->numFunctions;
	int id = worker->id;
	int i;
	int muted[MAX_NUM_FUNCTIONS];
	memcpy(muted, x->muteFunctions, MAX_NUM_FUNCTIONS * sizeof(int));

	x->last_x = in[0];
	if(x->functions[id] == NULL || muted[id]){
		// memset(x->ptrs[(id * 5)], 0, n * sizeof(t_float));
		// memset(x->ptrs[(id * 5) + 1], 0, n * sizeof(t_float));
		// memset(x->ptrs[(id * 5) + 2], 0, n * sizeof(t_float));
		// memset(x->ptrs[(id * 5) + 3], 0, n * sizeof(t_float));
		// memset(x->ptrs[(id * 5) + 4], 0, n * sizeof(t_float));
		memset(x->ptrs[(id * 5)], 0, n * sizeof(double));
		memset(x->ptrs[(id * 5) + 1], 0, n * sizeof(double));
		memset(x->ptrs[(id * 5) + 2], 0, n * sizeof(double));
		memset(x->ptrs[(id * 5) + 3], 0, n * sizeof(double));
		memset(x->ptrs[(id * 5) + 4], 0, n * sizeof(double));
	}
	t_plan plan;
	//te_critical_enter(x->lock, __LINE__);
	plan = x->plans[id];
	//te_critical_exit(x->lock, __LINE__);
	int ret = 0;
	int j;
	for(i = 0; i < n; i += DOWNSAMPLE){
		if(te_isPlanValid(x, in[i], &plan, id) == 0 || plan.valid == 0){
			ret = te_makePlan(x, in[i], id, &plan);
		}
		if(ret || plan.valid == 0){
			x->ptrs[(id * 5)][i] = 0.;
			x->ptrs[(id * 5) + 1][i] = 0.;
			x->ptrs[(id * 5) + 2][i] = 0.;
			x->ptrs[(id * 5) + 3][i] = 0.;
			x->ptrs[(id * 5) + 4][i] = 0.;
		}else{
			x->ptrs[(id * 5) + 1][i] = te_computeCorrectedPhase(in[i], &plan);
			x->ptrs[(id * 5)][i] = x->ptrs[(id * 5) + 1][i] - floor(x->ptrs[(id * 5) + 1][i]);
			x->ptrs[(id * 5) + 2][i] = (x->ptrs[(id * 5) + 1][i] - x->last_y[id]) * x->fs;
			//x->ptrs[(id * 5) + 2][i] = te_computeCorrectedTempo(in[i], &plan);
			x->last_y[id] = x->ptrs[(id * 5) + 1][i];
			x->ptrs[(id * 5) + 3][i] = plan.pointnum_left;
			x->ptrs[(id * 5) + 4][i] = floorf(x->ptrs[(id * 5) + 1][i]);
		}

		for(j = 1; j < DOWNSAMPLE; j++){
			x->ptrs[(id * 5) + j][i + 1] = x->ptrs[(id * 5) + 1][i];
			x->ptrs[(id * 5)][i + j] = x->ptrs[(id * 5)][i];
			x->ptrs[(id * 5) + 2][i + j] = x->ptrs[(id * 5) + 2][i];
			x->ptrs[(id * 5) + 3][i + j] = plan.pointnum_left;
			x->ptrs[(id * 5) + 4][i + j] = x->ptrs[(id * 5) + 4][i];
		}
	}
	//te_critical_enter(x->lock, __LINE__);
	x->plans[id] = plan;
	//te_critical_exit(x->lock, __LINE__);
}

void te_run(t_te *x){
	if(!x->parallel_task){
		x->parallel_task = sysparallel_task_new(&x, (method)te_workerproc, x->numFunctions);
	}
	x->parallel_task->priority = SYSPARALLEL_PRIORITY_HIGH;
	sysparallel_task_data(x->parallel_task, &x);
	if(sysparallel_task_execute(x->parallel_task)){
		object_error((t_object *)x, "oh crap");
		return;
	}
}

t_int *te_perform(t_int *w){
	t_te *x = (t_te *)w[1];
	x->w = w;
	te_run(x);

	t_float *out_phase_wrapped = (t_float *)w[4];
	t_float *out_phase = (t_float *)w[5];
	t_float *out_bps = (t_float *)w[6];
	t_float *out_pointnum = (t_float *)w[7];
	t_float *out_beatnum = (t_float *)w[8];

	int n = (int)w[2];

	memcpy(out_phase_wrapped, x->ptrs[x->currentFunction * 5], n * sizeof(t_float));
	memcpy(out_phase, x->ptrs[x->currentFunction * 5 + 1], n * sizeof(t_float));
	memcpy(out_bps, x->ptrs[x->currentFunction * 5 + 2], n * sizeof(t_float));
	memcpy(out_pointnum, x->ptrs[x->currentFunction * 5 + 3], n * sizeof(t_float));
	memcpy(out_beatnum, x->ptrs[x->currentFunction * 5 + 4], n * sizeof(t_float));

	return w + 9;
}


/*
t_int *te_perform(t_int *w){
	t_te *x = (t_te *)w[1];
	int n = (int)w[2];
	t_float *in = (t_float *)w[3];
	t_float *out_phase_wrapped = (t_float *)w[4];
	t_float *out_phase = (t_float *)w[5];
	t_float *out_bps = (t_float *)w[6];
	t_float *out_pointnum = (t_float *)w[7];
	t_float *out_beatnum = (t_float *)w[8];

	int i, j;
	t_symbol *name1, *name2, *name3, *name4, *name5;

	int muted[MAX_NUM_FUNCTIONS];
	memcpy(muted, x->muteFunctions, MAX_NUM_FUNCTIONS * sizeof(int));
	t_plan plan;

	for(j = 0; j < x->numFunctions; j++){
		if(x->functions[j] == NULL){
			memset(x->ptrs[(j * 5)], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 1], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 2], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 3], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 4], 0, n * sizeof(t_float));
		}
	}

	x->last_x = in[0];

	for(j = 0; j < x->numFunctions; j++){
		if(name1 = te_mangleName(x->name, 0, j)){
			name1->s_thing = (t_object *)(x->ptrs[j * 5]);
		}
		if(name2 = te_mangleName(x->name, 1, j)){
			name2->s_thing = (t_object *)(x->ptrs[(j * 5) + 1]);
		}
		if(name3 = te_mangleName(x->name, 2, j)){
			name3->s_thing = (t_object *)(x->ptrs[(j * 5) + 2]);
		}
		if(name4 = te_mangleName(x->name, 3, j)){
			name4->s_thing = (t_object *)(x->ptrs[(j * 5) + 3]);
		}
		if(name5 = te_mangleName(x->name, 4, j)){
			name5->s_thing = (t_object *)(x->ptrs[(j * 5) + 4]);
		}

		if(x->functions[j] == NULL){
			memset(x->ptrs[(j * 5)], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 1], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 2], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 3], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 5) + 4], 0, n * sizeof(t_float));
		}else{
			if(muted[j]){
				memset(x->ptrs[(j * 5)], 0, n * sizeof(t_float));
				memset(x->ptrs[(j * 5) + 1], 0, n * sizeof(t_float));
				memset(x->ptrs[(j * 5) + 2], 0, n * sizeof(t_float));
				memset(x->ptrs[(j * 5) + 3], 0, n * sizeof(t_float));
				memset(x->ptrs[(j * 5) + 4], 0, n * sizeof(t_float));
				continue;
			}
			//t_plan *plan = &(x->plans[j]);
			//te_critical_enter(x->lock, __LINE__);
			plan = x->plans[j];
			//te_critical_exit(x->lock, __LINE__);
			for(i = 0; i < n; i++){
				if(te_isPlanValid(x, in[i], &plan, j) == 0){
					te_makePlan(x, in[i], j, &plan);
				}

				x->ptrs[(j * 5) + 1][i] = te_computeCorrectedPhase(in[i], &plan);
				x->ptrs[(j * 5)][i] = x->ptrs[(j * 5) + 1][i] - floor(x->ptrs[(j * 5) + 1][i]);
				x->ptrs[(j * 5) + 2][i] = (x->ptrs[(j * 5) + 1][i] - x->last_y[j]) * x->fs;
				x->last_y[j] = x->ptrs[(j * 5) + 1][i];
				x->ptrs[(j * 5) + 3][i] = plan.pointnum_left;
				x->ptrs[(j * 5) + 4][i] = floorf(x->ptrs[(j * 5) + 1][i]);
			}
			//te_critical_enter(x->lock, __LINE__);
			x->plans[j] = plan;
			//te_critical_exit(x->lock, __LINE__);
		}
	}

	memcpy(out_phase_wrapped, x->ptrs[x->currentFunction * 5], n * sizeof(t_float));
	memcpy(out_phase, x->ptrs[x->currentFunction * 5 + 1], n * sizeof(t_float));
	memcpy(out_bps, x->ptrs[x->currentFunction * 5 + 2], n * sizeof(t_float));
	memcpy(out_pointnum, x->ptrs[x->currentFunction * 5 + 3], n * sizeof(t_float));
	memcpy(out_beatnum, x->ptrs[x->currentFunction * 5 + 4], n * sizeof(t_float));

	for(j = x->numFunctions; j < MAX_NUM_FUNCTIONS; j++){
		if(name1 = te_mangleName(x->name, 0, j)){
			name1->s_thing = NULL;
		}
		if(name2 = te_mangleName(x->name, 1, j)){
			name2->s_thing = NULL;
		}
		if(name3 = te_mangleName(x->name, 2, j)){
			name3->s_thing = NULL;
		}
		if(name4 = te_mangleName(x->name, 3, j)){
			name4->s_thing = NULL;
		}
		if(name5 = te_mangleName(x->name, 4, j)){
			name5->s_thing = NULL;
		}
	}

	return w + 9;
}

*/

int te_makePlan(t_te *x, float f, int function, t_plan *plan){
	if(!plan || !(x->functions[function])){
		return 1;
	}
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); 
	double f_sc = te_scale(f, x->time_min, x->time_max, 0, r.width);
	t_point *p = x->functions[function];
	t_point *next = NULL;
	if(p){
		next = p->next;
	}else{
		memset(plan, 0, sizeof(plan));
		return 1;
	}

	// we are somewhere between the minimum time (probably 0) and the first
	// point.  so we'll make a plan with a freq of 0.
	double scx = te_scale(p->coords.x, x->time_min, x->time_max, 0, r.width);
	if(f_sc < scx){
		plan->alpha = p->alpha;
		plan->beta = p->beta;
		plan->beta_ab = boost::math::beta(plan->alpha, plan->beta);
		plan->error_alpha = p->error_alpha;
		plan->error_beta = p->error_beta;
		plan->error_beta_ab = boost::math::beta(plan->error_alpha, plan->error_beta);
		plan->state = BEFORE_FIRST_POINT;
		plan->startTime = x->time_min;
		plan->endTime = p->coords.x;
		plan->startFreq = 0.;
		plan->endFreq = 0.;
		plan->startPhase = p->a_phase;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.
		plan->endPhase = p->a_phase; 
		te_computePhaseError(x, plan);
		plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
		plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
		//te_postPlan(plan);
		plan->pointnum_left = 0;
		plan->pointnum_right = 1;
		plan->valid = 1;
		return 0;
	}

	int counter = 1;
	while(p && next){
		double scx = te_scale(p->coords.x, x->time_min, x->time_max, 0, r.width);
		double nscx = te_scale(next->coords.x, x->time_min, x->time_max, 0, r.width);
		if(f_sc >= scx && f_sc < nscx){
			//printf("f_sc = %f, scx = %f, nscx = %f\n", f_sc, scx, nscx);
			plan->alpha = p->alpha;
			plan->beta = p->beta;
			plan->beta_ab = boost::math::beta(plan->alpha, plan->beta);
			plan->error_alpha = p->error_alpha;
			plan->error_beta = p->error_beta;
			plan->error_beta_ab = boost::math::beta(plan->error_alpha, plan->error_beta);
			plan->state = 0;
			plan->startTime = p->coords.x;
			//printf("%s: time = %f %f\n", __PRETTY_FUNCTION__, p->coords.x, next->coords.x);
			plan->endTime = next->coords.x;
			plan->startFreq = p->d_freq;
			plan->endFreq = next->coords.y;
			plan->startPhase = p->d_phase;
			plan->endPhase = next->a_phase;
			te_computePhaseError(x, plan);
			plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
			plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
			//te_postPlan(plan);
			plan->pointnum_left = counter;
			plan->pointnum_right = counter + 1;
			plan->valid = 1;
			return 0;
		}
		counter++;
		p = next;
		next = next->next;
	}

	// if we made it here, we're somewhere between the last point and time_max
	scx = te_scale(p->coords.x, x->time_min, x->time_max, 0., r.width);
	if(f_sc >= scx){
		plan->alpha = p->alpha;
		plan->beta = p->beta;
		plan->beta_ab = boost::math::beta(plan->alpha, plan->beta);
		plan->error_alpha = p->error_alpha;
		plan->error_beta = p->error_beta;
		plan->error_beta_ab = boost::math::beta(plan->error_alpha, plan->error_beta);
		plan->state = AFTER_LAST_POINT;
		plan->startTime = p->coords.x;
		plan->endTime = plan->startTime;
		plan->startFreq = p->coords.y;
		plan->endFreq = p->coords.y;
		plan->startPhase = p->d_phase;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.  this will effectively be a jump to phase and freq 0.
		plan->endPhase = p->d_phase;
		plan->phaseError = 0.;
		plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
		plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
		//te_postPlan(plan);
		plan->pointnum_left = counter;
		plan->pointnum_right = counter;
		plan->valid = 1;
		return 0;

	}
	return 1;
}

int te_isPlanValid(t_te *x, double time, t_plan *plan, int function){
	if(!plan){
		return 0;
	}
	if(time < plan->startTime || time > plan->endTime || (plan->startTime == 0 && plan->endTime == 0)){
		return 0;
	}

	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double stsc, etsc;
	stsc = te_scale(plan->startTime, x->time_min, x->time_max, 0, r.width);
	etsc = te_scale(plan->endTime, x->time_min, x->time_max, 0, r.width);
	t_point *p = x->functions[function];
	if(!p){
		return 0;
	}
	/***************************************************
	 // this could be bad...
	 ***************************************************/
	//if(fabs(p->coords.x - plan->endTime) < .001 && plan->startTime == 0.){
	if(p->coords.x == plan->endTime && plan->startTime == 0.){
		// special case where the start time is zero, but the first point is some time after that
		return 1;
	}
	while(p){
		if(p->next){
			if(time >= p->coords.x && time <= p->next->coords.x){
				if(p->coords.x != plan->startTime || p->next->coords.x != plan->endTime || 
				   p->coords.y != plan->startFreq || p->next->coords.y != plan->endFreq){
					return 0;
				}
			}
		}
		//if(fabs(p->screen_coords.x - stsc) < .001){
		/*
		if(fabs(p->coords.x - plan->startTime) < .001){
			if(p->next){
				//if(fabs(p->next->screen_coords.x - etsc) < .001){
				if(fabs(p->next->coords.x - plan->endTime) < .001){
					return 1;
				}else{
					return 0;
				}
			}else if(fabs(etsc - r.width) < 0.001){
				return 1;
			}else{
				return 0;
			}
		}else{

		}
		*/
		p = p->next;
	}
	return 0;
}

void te_computePhaseError(t_te *x, t_plan *plan){
	double endingPhase = te_scaledBetaCDFInt(1., plan->alpha, plan->beta, plan->endFreq - plan->startFreq, plan->startFreq, plan->beta_ab);
	endingPhase *= (plan->endTime - plan->startTime);
	endingPhase += plan->startPhase;
	double endingPhase_wrapped = (endingPhase - floor(endingPhase));
        double phaseError = plan->endPhase - endingPhase_wrapped;
	if(fabs(phaseError) > 0.5){
		if(phaseError < 0){
			phaseError += 1;
		}else{
			phaseError -= 1;
		}
	}
	plan->phaseError = phaseError;
}

double te_betaPDF(double z, double a, double b, double beta_ab){
	//return ((pow(z, a - 1) * (pow(1 - z, b - 1))) / gsl_sf_beta(a, b));
	return ((pow(z, a - 1) * (pow(1 - z, b - 1))) / beta_ab);
}

double te_betaCDF(double z, double a, double b, double beta_ab){
	z = te_clip(z, 0., 1.);
	//return gsl_sf_beta_inc(a, b, z);
	double r;
	if(a == 1. && b == 1.){
		r = z;
	}else if(a == 1.){
		r = 1. - (1. * pow((1. - z), b));
	}else if(b == 1.){
		if(a == 2){
			r = z * z;
		}else{
			r = pow(z, a);
		}
	}else{
		r = boost::math::beta(a, b, z) / beta_ab;
	}
	return r;
}

double te_betaCDFInt(double z, double a, double b, double beta_ab){
	z = te_clip(z, 0., 1.);
	//double beta_ab = gsl_sf_beta(a, b);
	//return (z * (gsl_sf_beta_inc(a, b, z) * beta_ab) - (gsl_sf_beta_inc(a + 1, b, z) * gsl_sf_beta(a + 1, b))) / beta_ab;
	double r;
	if(a == 1 && b == 1){
		r = (z * z) / 2;
	}else if(a == 1){
		r = (1 /(1. + b)) * pow(1. - z, b + 1) + z;
	}else if(b == 1.){
		if(a == 2){
			r = (z * z * z) / 3;
		}else{
			r = pow(z, b + 1) / (b + 1);
		}
	}else{
		r = (z * (boost::math::beta(a, b, z) - boost::math::beta(a + 1, b, z))) / beta_ab;
	}
	return r;
}

double te_scaledBetaCDFInt(double z, double a, double b, double scale, double offset, double beta_ab){
	if(a <= 0 || b <= 0){
		error("tempo_editor: a (%f) and b (%f) must both be greater than 0", a, b);
		return 0;
	}
	z = te_clip(z, 0., 1.);
	if(z == 0){
		return 0;
	}
	//double beta_ab = gsl_sf_beta(a, b);
	//return (offset * z + (scale * z * ((gsl_sf_beta_inc(a, b, z) * beta_ab) - (gsl_sf_beta_inc(a + 1, b, z) * gsl_sf_beta(a + 1, b)) / z)) / beta_ab);

	if(a <= 0 || b <= 0){
		error("a and b must be greater than 0 (%f %f)", a, b);
		return 0.;
	}
	double r;
	if(a == 1 && b == 1){
		r = offset * z + ((scale * (z * z)) / 2.);
	}else if(a == 1.){
		// not sure why this doesn't work...
		//r = (1 / (b + 1)) * scale * pow(1. - 1. * z, (b + 1)) + offset * z + scale * (-1. + 1. * z);
		r = (offset * z + (scale * z * (boost::math::beta(a, b, z) - (boost::math::beta(a + 1, b, z)) / z)) / beta_ab);
	}else if(b == 1.){
		r = offset * z + ((scale * pow(z, a + 1)) / (a + 1));
	}else{
		r = (offset * z + (scale * z * (boost::math::beta(a, b, z) - (boost::math::beta(a + 1, b, z)) / z)) / beta_ab);
	}
	return r;
}

double te_computeCorrectedTempo(double t, t_plan *p){
	//post("t = %f, startTime = %f, endTime = %f", t, p->startTime, p->endTime);
	if(t == p->startTime){
		return p->startFreq;
	}else if(t == p->endTime){
		return p->endFreq;
	}else if(t >= p->correctionStart && t <= p->correctionEnd){
		//return (te_computeCorrectedPhase(t, p) - te_computeCorrectedPhase(t - (1. / 44100.), p)) * 44100.;
		return (te_computeCorrectedPhase(t, p) - te_computeCorrectedPhase(t - .000000001, p)) * (1. / .000000001);
	}else{
		return te_computeTempo(t, p);
	}
	return 0.;
}

double te_computeTempo(double t, t_plan *p){
	//te_postplan(p, printf);
	//printf("**************************************************\n");
	double norm_t = te_scale(t, p->startTime, p->endTime, 0., 1.);
	norm_t = te_clip(norm_t, 0., 1.);
	//return gsl_sf_beta_inc(p->alpha, p->beta, norm_t) * (p->endFreq - p->startFreq) + p->startFreq;
	return (boost::math::ibeta(p->alpha, p->beta, norm_t)) * (p->endFreq - p->startFreq) + p->startFreq;
	//return t * p->m + p->b;
}

double te_computePhase(double t, t_plan *p){
	switch(p->state){
	case BEFORE_FIRST_POINT:
		return 0.999;
	case AFTER_LAST_POINT:
		return 0.;
	default:
		break;
		//return (((t * ((2. * p->b) + (p->m * t))) / 2.) - p->phaseError_start);
	}

	double norm_t = te_scale(t, p->startTime, p->endTime, 0., 1.);
	norm_t = te_clip(norm_t, 0., 1.);
	return te_scaledBetaCDFInt(norm_t, p->alpha, p->beta, p->endFreq - p->startFreq, p->startFreq, p->beta_ab) * (p->endTime - p->startTime) + p->startPhase;
}

double te_computeCorrectedPhase(double t, t_plan *p){
	//printf("%s: time = %f %f, freq = %f %f\n", __PRETTY_FUNCTION__, p->startTime, p->endTime, p->startFreq, p->endFreq);
	switch(p->state){
	case BEFORE_FIRST_POINT:
		if(p->endPhase == 0.){
			return 0.999;
		}else{
			return p->endPhase;
		}
	case AFTER_LAST_POINT:
		return 0.;
	default:
		{
			if(t == p->startTime){
				return p->startPhase;
			}
			double error = p->startPhase;
			if(t >= p->startTime && t < p->correctionStart){
			}else if(t >= p->correctionStart && t < p->correctionEnd){
				double norm_t = te_scale(t, p->correctionStart, p->correctionEnd, 0., 1.);
				norm_t = te_clip(norm_t, 0., 1.);
				error += p->phaseError * (te_betaCDF(norm_t, p->error_alpha, p->error_beta, p->error_beta_ab));
			}else if(t >= p->correctionEnd && t <= p->endTime){
				error += p->phaseError;
			}

			double norm_t = te_scale(t, p->startTime, p->endTime, 0., 1.);
			norm_t = te_clip(norm_t, 0., 1.);
			return ((te_scaledBetaCDFInt(norm_t, p->alpha, p->beta, p->endFreq - 
				p->startFreq, p->startFreq, p->beta_ab)) * (p->endTime - p->startTime)) + error;
		}
	}
}

double te_computeCorrectedUnwrappedMonotonicPhase(t_te *x, double t, int function, t_plan *plan){
	// first, compute the phase at time t, then compute the phase at each control point
	// adding in any additional phase incurred by a different departure phase
	double phase = te_computeCorrectedPhase(t, plan);
	//printf("corrected phase = %f\n", phase);
	t_point *p = x->functions[function];
	if(p){
		p = p->next;
	}
	int i = 0;
	while(p){
		if(p->coords.x > (t + (1. / 44100.)) && fabs(p->coords.x - (t + (1. / 44100.))) > .0001){
			//post("%f > %f", p->coords.x, (t + (1. / 44100.)));
		//if(fabs(p->coords.x - (t + (1. / 44100.))) < .0001){
			//post("DONE: %f %f", p->coords.x, t + (1. / 44100.));
			break;
		}else if(!(p->next)){
			//post("DONE: no next point");
			//break;
		}
		t_plan thisplan;
		te_makePlan(x, p->coords.x - (2. / 44100.), function, &thisplan);
		//post("%f %f", p->coords.x, thisplan.startTime
		//printf("%d: adding %f to %f (%d)\n", thisplan.pointnum_left, 
		//te_computeCorrectedPhase(p->coords.x - (2. / 44100.), &thisplan), phase, i++);
		phase += te_computeCorrectedPhase(p->coords.x, &thisplan);

		double dphase = p->d_phase;
		if(dphase >= 1.){
			while(dphase >= 1.){
				dphase -= 1;
			}
		}
		phase -= dphase;

		p = p->next;
	}
	//post("PHASE: %f", phase);
	return phase;
}

void te_editSel(t_te *x, double xx, double yy, double zz){
}

void te_processOSC(t_te *x, char *prefix, t_hashtab *ht, int functionNum){
	t_osc_msg_s *time = NULL;
	char buf[64];
	//sprintf(buf, "/function/%02d/time", i);
	if(prefix){
		sprintf(buf, "%s/time", prefix);
	}else{
		strncpy(buf, "/time\0", 6);
	}
	hashtab_lookup(ht, gensym(buf), (t_object **)(&time));
	if(!time){
		return;
	}
	long argc_time = omax_util_getNumAtomsInOSCMsg(time);
	t_atom argv_time[argc_time];
	omax_util_oscMsg2MaxAtoms(time, argv_time);
	int npoints = osc_message_s_getArgCount(time);
	t_point *head = NULL, *tail = NULL;
	int j;
	for(j = 0; j < npoints; j++){
		t_point *point = (t_point *)calloc(sizeof(t_point), 1);
		if(!head){
			head = point;
		}
		if(tail){
			tail->next = point;
		}
		tail = point;
		point->coords.x = (double)atom_getfloat(argv_time + j + 1);
	}
	for(j = 1; j < (int)(sizeof(params) / sizeof(char*)); j++){
		const char *param = params[j];
		//sprintf(buf, "/function/%02d%s", i, param);
		if(prefix){
			sprintf(buf, "%s%s", prefix, param);
		}else{
			strcpy(buf, param);
		}
		t_osc_msg_s *mm = NULL;
		hashtab_lookup(ht, gensym(buf), (t_object **)(&mm));
		if(!mm){
			continue;
		}
		long argc = omax_util_getNumAtomsInOSCMsg(mm);
		t_atom argv[argc];
		omax_util_oscMsg2MaxAtoms(mm, argv);
		int k = 0;
		t_point *p = head;
		while(p){
			if(k < argc){
				*((double *)(((char *)p) + offsets[j])) = (double)atom_getfloat(argv + k + 1);
			}else{
				*((double *)(((char *)p) + offsets[j])) = defaults[j];
			}
			p = p->next;
			k++;
		}
	}
	t_point *p = head, *next = NULL;
	j = 0;
	while(p){
		// te_insertPoint will change the next link, so we have to 
		// get it first before calling that function.
		next = p->next;
		te_insertPoint(x, p, functionNum);
		p->whichPoint = 0;
		p->aux_points[0] = x->error_offset;
		p->aux_points[1] = x->error_span + x->error_offset;
		p = next;
	}
}

void te_fullPacket(t_te *x, long len, long ptr){
	// if no /function prefix, check to see if anything is selected and replace that,
	// otherwise just jam a new point in there
	t_hashtab *ht = hashtab_new(91);
	hashtab_flags(ht, OBJ_FLAG_DATA);

	int i;
	for(i = 0; i < (int)(sizeof(params) / sizeof(char*)); i++){
		t_osc_msg_ar_s *m = NULL;
		//osc_bundle_s_lookupAddress(len, (char *)ptr, params[i], &m, 1);
        m = osc_bundle_s_lookupAddress(len, (char *)ptr, params[i], 1);

		int j = 0;
		for(j = 0; j < osc_message_array_s_getLen(m); j++){
			t_osc_msg_s *mm = (t_osc_msg_s *)osc_message_array_s_get(m, j);
			hashtab_store(ht, gensym(osc_message_s_getAddress(mm)), (t_object *)mm);
		}
		if(m){
			osc_message_array_s_free(m);
		}
	}
	if(hashtab_getsize(ht)){
		te_processOSC(x, NULL, ht, x->currentFunction);
		t_symbol **keys = NULL;
		long nkeys = 0;
		hashtab_getkeys(ht, &nkeys, &keys);
		for(i = 0; i < nkeys; i++){
			t_osc_msg_s *m = NULL;
			hashtab_lookup(ht, keys[i], (t_object **)(&m));
			osc_message_s_free(m);
		}
		hashtab_clear(ht);
	}

	t_osc_msg_ar_s *m = NULL;
	//osc_bundle_lookupAddress_s(len, (char *)ptr, "/function/??/", &m, 0);	
	//osc_bundle_s_lookupAddress(len, (char *)ptr, "/function/", &m, 0);
    m = osc_bundle_s_lookupAddress(len, (char *)ptr, "/function/", 0);
	if(m){
		int fplen = 10;
		char fn[99];
		memset(fn, '\0', 99);
		unsigned int i;
		for(i = 0; i < osc_message_array_s_getLen(m); i++){
			char *param = NULL;
			t_osc_msg_s *mm = (t_osc_msg_s *)osc_message_array_s_get(m, i);
			char *mm_address = osc_message_s_getAddress(mm);
			if(*(mm_address + fplen) == '*'){
				int j;
				for(j = 0; j < x->numFunctions; j++){
					fn[j] = 1;
					char address[128];
					sprintf(address, "/function/%02d/%s", j, mm_address + fplen + 2);
					t_osc_msg_s *mm_copy = osc_message_s_alloc();
					osc_message_s_copy(&mm_copy, mm);
					hashtab_store(ht, gensym(address), (t_object *)mm_copy);
				}
			}else{
				unsigned int functionNum = strtol(mm_address + fplen, &param, 0);
				if(functionNum < sizeof(fn)){
					fn[functionNum] = 1;
					t_osc_msg_s *mm_copy = osc_message_s_alloc();
					osc_message_s_copy(&mm_copy, mm);
					hashtab_store(ht, gensym(mm_address), (t_object *)mm_copy);
				}
			}
		}

		for(i = 0; i < sizeof(fn); i++){
			if(fn[i] == 0){
				continue;
			}
			char prefix[64];
			sprintf(prefix, "/function/%02d", i);
			te_processOSC(x, prefix, ht, i);
		}
	}
	t_symbol **keys = NULL;
	long nkeys = 0;
	hashtab_getkeys(ht, &nkeys, &keys);
	for(i = 0; i < nkeys; i++){
		t_osc_msg_s *m = NULL;
		hashtab_lookup(ht, keys[i], (t_object **)(&m));
		osc_message_s_free(m);
	}
	hashtab_chuck(ht);

	memset(x->plans, '\0', MAX_NUM_FUNCTIONS * sizeof(t_plan));
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
	te_dumpSelectedOSC(x);
	te_dumpCellblock(x);
}

void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);

	switch(proxy_getinlet((t_object *)x)){
	case 0: // add points
		{
			if(argc < 3){
				object_error((t_object *)x, "you must supply at least the time and tempo of the point you want to add");
				return;
			}
			long functionnum = atom_getlong(argv);
			if(functionnum > MAX_NUM_FUNCTIONS){
				object_error((t_object *)x, "max number of functions is currently %d", MAX_NUM_FUNCTIONS);
				return;
			}
			t_atom *ptr = argv;
			x->selected = te_allocAndInsertPoint(x, (t_pt){atom_getfloat(argv + 1), atom_getfloat(argv + 2)}, atom_getlong(argv));
			ptr += 3;
			x->selected->d_freq = x->selected->coords.y;
			x->selected->a_phase = x->selected->d_phase = 0.;
			x->selected->alpha = x->selected->beta = 1.;
			x->selected->error_alpha = x->selected->error_beta = 2.;
			x->selected->whichPoint = 0;
			x->selected->aux_points[0] = x->error_offset;
			x->selected->aux_points[1] = x->error_span + x->error_offset;

			if(ptr - argv >= argc){
				break;
			}
			x->selected->d_freq = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->a_phase = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->d_phase = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->alpha = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->beta = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->error_alpha = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
			x->selected->error_beta = atom_getfloat(ptr++);
			if(ptr - argv >= argc){
				break;
			}
		}
		break;
	case 1: // cellblock
		{
			if(argc == 2){
				return;
			}
			t_point *p = x->functions[x->currentFunction];
			int i;
			int row = atom_getlong(&(argv[1]));
			if(row == 0){ // header
				return;
			}
			row--; // decrement the row--the 0th point is on row 1 due to the header
			int col = atom_getlong(&(argv[0]));
			double val = atom_getfloat(&(argv[2]));
			for(i = 0; p; i++){
				if(i == row){
					break;
				}
				p = p->next;
			}
			if(p){
				x->selected = p;
				switch(col){
				case 1:
					// time
					p->coords.x = val;
					break;
				case 2:
					// duration
					break;
				case 3:
					// freq
					p->coords.y = val;
					break;
				case 4:
					// departure freq
					p->d_freq = val;
					break;
				case 5:
					// arrival phase
					p->a_phase = val;
					break;
				case 6:
					// departure phase
					p->d_phase = val;
					break;
				case 7:
					// alpha
					p->alpha = val;
					break;
				case 8:
					// beta
					p->beta = val;
					break;
				case 9:
					// error_alpha
					p->error_alpha = val;
					break;
				case 10:
					// error_beta
					p->error_beta = val;
					break;
				}
				if(col == 1){
					x->selected = p;
					te_reorderPoint(x->selected);
					te_initReorderedPoint(x, x->selected);
				}
			}else{
				t_pt sc = {0., r.height};
				switch(col){
				case 0:
					sc.x = te_scale(val, x->time_min, x->time_max, 0, r.width);
					break;
				case 1:
					sc.y = te_scale(val, x->freq_min, x->freq_max, r.height, 0);
					break;
				}
				x->selected = te_allocAndInsertPoint(x, sc, x->currentFunction);
			}
			if(p->d_phase < p->a_phase){
				p->d_phase += 1;
			}
		}
		break;
	}
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
}

void te_float(t_te *x, double f){
	//printf("%s: t = %f\n", __PRETTY_FUNCTION__, f);
	uint32_t j;
	//t_atom out[6], *ptr;
	struct {
		char header[16];
		char function_size[4];
		char function_address[12]; // /function\0\0\0
		char function_tt[4];
		char function_data[4];

		char point_size[4];
		char point_address[8]; // /point\0\0
		char point_tt[4];
		char point_data[4];

		char beat_size[4];
		char beat_address[8]; // /beat\0\0\0
		char beat_tt[4];
		char beat_data[4];

		char time_size[4];
		char time_address[8]; // /time\0\0\0
		char time_tt[4];
		char time_data[4];

		char tempo_size[4];
		char tempo_address[8]; // /tempo\0\0
		char tempo_tt[4];
		char tempo_data[4];

		char phase_size[4];
		char phase_address[8]; // /phase\0\0
		char phase_tt[4];
		char phase_data[4];
	} bndl = {
		"#bundle\0\0\0\0\0\0\0\0",
		{0, 0, 0, 20},
		"/function\0\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 16},
		"/point\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 16},
		"/beat\0\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 16},
		"/time\0\0",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 16},
		"/tempo\0",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 16},
		"/phase\0",
		",f\0",
		"\0\0\0"
	};

	for(j = 0; j < (uint32_t)x->numFunctions; j++){
		t_plan plan;

		te_makePlan(x, f, j, &plan);
		*((uint32_t *)bndl.function_data) = hton32(j);

		uint32_t l = plan.pointnum_left;
		*((uint32_t *)bndl.point_data) = hton32(l);

		float ff = (float)te_computeCorrectedPhase(f, &plan);
		*((uint32_t *)bndl.beat_data) = hton32((uint32_t)ff);

		ff = (float)f;
		*((uint32_t *)bndl.time_data) = hton32(*((uint32_t *)(&ff)));

		ff = (float)te_computeCorrectedTempo(f, &plan);
		*((uint32_t *)bndl.tempo_data) = hton32(*((uint32_t *)(&ff)));

		ff = (float)te_computeCorrectedUnwrappedMonotonicPhase(x, f, j, &plan);
		*((uint32_t *)bndl.phase_data) = hton32(*((uint32_t *)(&ff)));

		t_atom out[2];
		atom_setlong(out, sizeof(bndl));
		atom_setlong(out + 1, (long)(&bndl));
		outlet_anything(x->out_osc, ps_FullPacket, 2, out);

	}
}

void te_find_btn(t_point *function, double x, t_point **left, t_point **right){
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
		//if(x >= ptr->screen_coords.x && x <= ptr->next->screen_coords.x){
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

void te_reorderPoint(t_point *p){
	int i = 0;
	while(p->prev){
		if(p->coords.x < p->prev->coords.x){
			te_swapPoints(p->prev, p);
		}else{
			break;
		}
	}
	i = 0;
	while(p->next){
		if(p->coords.x > p->next->coords.x){
			te_swapPoints(p, p->next);
		}else{
			break;
		}
	}
}

void te_initReorderedPoint(t_te *x, t_point *p){
	int i, function = 0;
	te_critical_enter(x->lock, __LINE__);
	while(p->prev){
		p = p->prev;
	}
	for(i = 0; i < x->numFunctions; i++){
		if(p == x->functions[i] || p->next == x->functions[i]){
			function = i;
			break;
		}
	}
	x->functions[function] = p;
	if(p->next){
		if(p->aux_points[0] == 0 && p->aux_points[1] == 0){
			p->aux_points[0] = x->error_offset;
			p->aux_points[1] = x->error_span + x->error_offset;
		}
	}else{
		p->aux_points[0] = p->aux_points[1] = 0.;
	}
	te_critical_exit(x->lock, __LINE__);
}

void te_swapPoints(t_point *p1, t_point *p2){
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

void te_selectRegion(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	// do this with a recursive function to parse:
	// time region
	// control points
	// functions
	// if noncontiguous control points, select from min to max
	long numfunctions = 0;
	long functions[MAX_NUM_FUNCTIONS];
	long rangelen = 0;
	double range[2];
	// these shouldn't actually be present--we could throw an error if anything gets put in here.
	long numsubdivs = 0;
	double subdivs[argc];
	long numcontrolpoints = 0;
	long controlpoints[argc];
	te_processArgs_r(x, NULL, argc, argv, &numfunctions, functions, &rangelen, range, &numsubdivs, subdivs, &numcontrolpoints, controlpoints);

	int i;

	if(!numfunctions){
		numfunctions = 1;
		functions[0] = x->currentFunction;
	}
	if(!rangelen && !numcontrolpoints){
		rangelen = 2;
		range[0] = x->time_min;
		range[1] = x->time_max;
	}

	// if there's a range and a list of control points, do the range and bail
	if(rangelen){
		if(range[0] > range[1]){
			double tmp = range[0];
			range[0] = range[1];
			range[1] = tmp;
		}
	}else if(numcontrolpoints){
		// get the min and max control points and then use their x coords for the range
		double mintime = DBL_MAX, maxtime = 0;
		t_point *p;
		int j, pointnum;
		for(i = 0; i < numfunctions; i++){
			p = x->functions[functions[i]];
			pointnum = 1;
			while(p){
				for(j = 0; j < numcontrolpoints; j++){
					if(pointnum == controlpoints[j]){
						if(p->next){
							if(p->coords.x < mintime){
								mintime = p->coords.x;
							}
							if(p->next->coords.x > maxtime){
								maxtime = p->next->coords.x;
							}
						}
					}
				}
				pointnum++;
				p = p->next;
			}
		}
		range[0] = mintime - (1. / 44100.);
		range[1] = maxtime + (1. / 44100.);
		rangelen = 2;
	}else{
		// select x->time_min to x->time_max for all functions listed
		range[0] = x->time_min;
		range[1] = x->time_max;
		rangelen = 2;
	}

	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	x->sel_box.x = range[0];
	x->sel_box.width = range[1];
	x->sel_box.y = x->freq_max;
	x->sel_box.height = x->freq_min;
	/*
	x->sel_box.x = te_scale(range[0], x->time_min, x->time_max, 0., r.width);
	x->sel_box.width = te_scale(range[1], x->time_min, x->time_max, 0, r.width);
	x->sel_box.width -= x->sel_box.x;
	x->sel_box.y = 0.;
	x->sel_box.height = r.height;
	*/
	te_clearSelected(x);
	for(i = 0; i < numfunctions; i++){
		te_selectPointsInBox(x, r, x->sel_box, functions[i]);
	}

	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_clearSelected(t_te *x){
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
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_clearSelectionBox(t_te *x){
	x->sel_box = (t_rect){0., 0., 0., 0.};
	x->sel_box_selected = 0;
}

void te_applySelectionToAllFunctions(t_te *x){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	te_clearSelected(x);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		te_selectPointsInBox(x, r, x->sel_box, i);
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[i]);
	}
	jbox_redraw((t_jbox *)x);
}

void te_addPointsAtSelectionBoundariesForAllFunctions(t_te *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		te_addPointsAtSelectionBoundariesForFunction(x, i);
	}
}

void te_addPointsAtSelectionBoundaries(t_te *x){
	if(x->apply_selection_box_to_all_functions_by_default){
		int i;
		for(i = 0; i < x->numFunctions; i++){
			te_addPointsAtSelectionBoundariesForFunction(x, i);
		}
	}else{
		te_addPointsAtSelectionBoundariesForFunction(x, x->currentFunction);
	}
}

void te_addPointsAtSelectionBoundariesForFunction(t_te *x, int function){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	if(x->sel_box.width == 0 || x->sel_box.height == 0.){
		return;
	}
	//double left = te_scale(x->sel_box.x, 0., r.width, x->time_min, x->time_max);
	//double right = te_scale(x->sel_box.x + x->sel_box.width, 0., r.width, x->time_min, x->time_max);
	double left = x->sel_box.x;
	double right = x->sel_box.x + x->sel_box.width;
	double y;
	t_plan plan;
	t_point *p;
	te_makePlan(x, left, function, &plan);
	y = te_computeTempo(left, &plan);
	p = te_allocAndInsertPoint(x, (t_pt){left, y}, function);
	te_initPoint(x, p);
	p->selected = 1;
	p->next_selected = x->selected;
	p->prev_selected = NULL;
	if(x->selected){
		x->selected->prev_selected = p;
	}
	x->selected = p;

	te_makePlan(x, right, function, &plan);
	y = te_computeTempo(right, &plan);
	p = te_allocAndInsertPoint(x, (t_pt){right, y}, function);
	te_initPoint(x, p);
	p->selected = 1;
	p->next_selected = x->selected;
	p->prev_selected = NULL;
	if(x->selected){
		x->selected->prev_selected = p;
	}
	x->selected = p;

	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[function]);
	jbox_redraw((t_jbox *)x);
}

void te_snapSelectionToGrid(t_te *x){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_point *p;
	t_pt sc, snapped;
	double dfreq;
	te_critical_enter(x->lock, __LINE__);
	p = x->selected;
	while(p){
		sc.x = te_scale(p->coords.x, x->time_min, x->time_max, 0, r.width);
		sc.y = te_scale(p->coords.y, x->freq_min, x->freq_max, r.height, 0);
		dfreq = te_scale(p->d_freq, x->freq_min, x->freq_max, r.height, 0);
		te_findNearestGridPoint(x, sc, &snapped);
		dfreq += snapped.y - sc.y;
		sc = snapped;
		p->coords.x = te_scale(sc.x, 0, r.width, x->time_min, x->time_max);
		p->coords.y = te_scale(sc.y, r.height, 0, x->freq_min, x->freq_max);
		p->d_freq = te_scale(dfreq, r.height, 0, x->freq_min, x->freq_max);
		p = p->next_selected;
	}
	te_critical_exit(x->lock, __LINE__);
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
}

unsigned long te_selectBox(t_te *x, t_pt pt){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r);
	t_rect sel_box = (t_rect){
		te_scale(x->sel_box.x, x->time_min, x->time_max, 0, r.width),
		te_scale(x->sel_box.y, x->freq_min, x->freq_max, 0, r.height),
		te_scale(x->sel_box.width, x->time_min, x->time_max, 0, r.width),
		te_scale(x->sel_box.height, x->freq_min, x->freq_max, 0, r.height)
	};
	unsigned long sel_box_selected = 0;
	// the width could be 0--we don't want to select both the right and left edges, so do the
	// right edge first
	if(fabs(pt.x - (sel_box.x + sel_box.width)) < 4. && pt.y >= sel_box.y && pt.y <= (sel_box.y + sel_box.height)){
		sel_box_selected |= RIGHT_EDGE;
	}else if(fabs(pt.x - sel_box.x) < 4. && pt.y >= sel_box.y && pt.y <= (sel_box.y + sel_box.height)){
		sel_box_selected |= LEFT_EDGE;
	}
	if(fabs(pt.y - (sel_box.y + sel_box.height)) < 4. && pt.x >= sel_box.x && pt.x <= (sel_box.x + sel_box.width)){
		sel_box_selected |= BOTTOM_EDGE;
	}else if(fabs(pt.y - sel_box.y) < 4. && pt.x >= sel_box.x && pt.x <= (sel_box.x + sel_box.width)){
		sel_box_selected |= TOP_EDGE;
	}
	if(sel_box_selected == 0){
		if(pt.x < (sel_box.x + sel_box.width) && pt.x > sel_box.x && pt.y > sel_box.y && pt.y < (sel_box.y + sel_box.height)){
			sel_box_selected = BOX_SELECTED;
		}
	}
	return sel_box_selected;
}

void te_selectPointsInBox(t_te *x, t_rect drawing_rect, t_rect sel_box, int function){
	t_point *p = x->functions[function];
	while(p){
		//t_pt sc;
		//te_w2s(x, drawing_rect, p->coords, &sc);
		double left = sel_box.width > 0 ? sel_box.x : sel_box.x + sel_box.width;
		double right = sel_box.width > 0 ? sel_box.x + sel_box.width : sel_box.x;
		double bottom = x->freq_max - (sel_box.height > 0 ? sel_box.y : sel_box.y + sel_box.height);
		double top = x->freq_max - (sel_box.height > 0 ? sel_box.y + sel_box.height : sel_box.y);
		if(p->coords.x >= left && p->coords.x <= right && p->coords.y <= bottom && p->coords.y >= top){
			p->selected = 1;
			p->whichPoint = 0;
			p->next_selected = x->selected;
			if(x->selected){
				x->selected->prev_selected = p;
			}
			x->selected = p;
		}
		p = p->next;
	}
}

t_point *te_select(t_te *x, t_pt p){
	t_rect r;
	//jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); 
	double min = 1000000000.;
	t_point *min_ptr = NULL;
	t_point *ptr = x->functions[x->currentFunction];
	double xdif, ydif;
	t_pt sc;
	while(ptr){
		te_w2s(x, r, ptr->coords, &sc);
		if((xdif = fabs(p.x - sc.x)) < POINT_WIDTH / 2. && (ydif = fabs(p.y - sc.y)) < POINT_WIDTH / 2.){
			if(xdif + ydif < min){
				min = xdif + ydif;
				min_ptr = ptr;
			}
		}
		ptr = ptr->next;
	}
	if(min_ptr){
		min_ptr->whichPoint = 0;
	}
	return min_ptr;
}

t_point *te_selectControlPoint(t_te *x, t_pt p){
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_point *ptr = x->functions[x->currentFunction];
	t_pt sc, nsc;
	while(ptr){
		if(ptr->next){
			te_w2s(x, r, ptr->coords, &sc);
			te_w2s(x, r, ptr->next->coords, &nsc);
			if(p.x >= sc.x && p.x < nsc.x){
				break;
			}
		}
		ptr = ptr->next;
	}

	if(ptr){
		if(ptr->next){
			t_plan plan;
			te_makePlan(x, ptr->coords.x, x->currentFunction, &plan);
			double xx = te_scale(ptr->aux_points[0], 0., 1., plan.startTime, plan.endTime);
			double correctedTempo = te_computeCorrectedTempo(xx, &plan);
			xx = te_scale(xx, x->time_min, x->time_max, 0, r.width);
			correctedTempo = te_scale(correctedTempo, x->freq_min, x->freq_max, r.height, 0);
			if(fabs(p.x - xx) <= CONTROL_POINT_WIDTH / 2. && fabs(p.y - correctedTempo) <= CONTROL_POINT_WIDTH / 2.){
				ptr->whichPoint = 1;
				return ptr;
			}
			xx = te_scale(ptr->aux_points[1], 0., 1., plan.startTime, plan.endTime);
			correctedTempo = te_computeCorrectedTempo(xx, &plan);
			xx = te_scale(xx, x->time_min, x->time_max, 0, r.width);
			correctedTempo = te_scale(correctedTempo, x->freq_min, x->freq_max, r.height, 0);
			if(fabs(p.x - xx) <= CONTROL_POINT_WIDTH / 2 && fabs(p.y - correctedTempo) <= CONTROL_POINT_WIDTH / 2.){
				ptr->whichPoint = 2;
				return ptr;
			}
		}
	}
	return NULL;
}

t_max_err te_notify(t_te *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		//t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
		te_invalidateAll(x);
 		jbox_redraw((t_jbox *)x); 
	} 
	return 0; 
} 

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void te_mousedown(t_te *x, t_object *patcherview, t_pt pt, long modifiers){ 
	if(x->lockFunctions[x->currentFunction]){
		return;
	}
	x->mousestate = modifiers;
	if(x->hideFunctions[x->currentFunction]){
		return;
	}
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	t_pt wc;
	te_s2w(x, r, pt, &wc);
	x->last_mouse = pt;
	switch(modifiers){
	case 0x118:
	case 0x18:
	case 0x1A:
	case 0x11A:
	case 0x110:
	case 0x10:{
		// no modifiers.  
		// select a point if the click was on top of one, or
		// add a new point if the function isn't locked
		unsigned long box_selected = te_selectBox(x, pt);
		if(box_selected){
			x->sel_box_selected = box_selected;
			break;
		}
		t_point *tmp = NULL;
		if(x->show_tempo_correction){
			tmp = te_selectControlPoint(x, pt);
			if(tmp){
				te_clearSelected(x);
				te_clearSelectionBox(x);
				x->selected = tmp;
				tmp->selected = 1;
				break;
			}
		}
		tmp = te_select(x, pt);
		if(tmp){
			if(tmp == x->selected){
				break;
			}else if(tmp->selected){
				// find the point that the user clicked on and make it the first element of the
				// selected linked list.
				if(tmp->prev_selected){
					tmp->prev_selected->next_selected = tmp->next_selected;
				}
				if(tmp->next_selected){
					tmp->next_selected->prev_selected = tmp->prev_selected;
				}
				tmp->next_selected = x->selected;
				x->selected = tmp;
			}else{
				te_clearSelected(x);
				x->selected = tmp;
				x->selected->selected = 1;
			}
			break;
		}else{
			te_clearSelected(x);
			te_clearSelectionBox(x);
		}
	}
		break;
	case 0x111:
	case 0x11:{
		// command.
		// add a point
			te_clearSelected(x);
			x->selected = te_allocAndInsertPoint(x, wc, x->currentFunction);
			x->selected->selected = 1;
			x->selected->d_freq = wc.y;
			x->selected->a_phase = x->selected->d_phase = 0.;
			x->selected->alpha = x->selected->beta = 1.;
			x->selected->error_alpha = x->selected->error_beta = 2.;
			x->selected->whichPoint = 0;
			x->selected->aux_points[0] = x->error_offset;
			x->selected->aux_points[1] = x->error_span + x->error_offset;
			x->selected->draw_snapline = 0;
		}
		break;
	case 0x194:
	case 0x94:{
		// control.  
		// delete a point if one was clicked
		if(x->selected = te_select(x, pt)){
			te_removePoint(x, x->selected, x->currentFunction);
			x->selected = NULL;
			break;
		}
	}
		break;
	case 0x112:
	case 0x12:{
		// shift
		// add a single point to the selection list, 
		// or draw a box to select a region
		t_point *tmp = te_select(x, pt);
		if(tmp){
			if(tmp->selected){
				tmp->selected = 0;
				if(tmp->prev_selected){
					tmp->prev_selected->next_selected = tmp->next_selected;
				}else{
					x->selected = tmp->next_selected;
				}
				if(tmp->next_selected){
					tmp->next_selected->prev_selected = tmp->prev_selected;
				}
				tmp->next_selected = tmp->prev_selected = NULL;
			}else{
				tmp->next_selected = x->selected;
				tmp->prev_selected = NULL;
				if(x->selected){
					x->selected->prev_selected = tmp;
				}
				x->selected = tmp;
				tmp->selected = 1;
			}
			break;
		}
		te_clearSelected(x);
		/*
		x->sel_box.x = pt.x;
		x->sel_box.y = 0;
		x->sel_box.width = 0;
		x->sel_box.height = r.height;
		*/
		x->sel_box.x = te_scale(pt.x, 0, r.width, x->time_min, x->time_max);
		x->sel_box.y = x->freq_min;
		x->sel_box.width = 0.;
		x->sel_box.height = x->freq_max - x->freq_min;
		x->sel_type = RANGE_SELECT;
	}
		break;
	case 0x13:
	case 0x113:
		// command-shift -> selection box
		/*
		x->sel_box.x = pt.x;
		x->sel_box.y = pt.y;
		x->sel_box.width = 0;
		x->sel_box.height = 0;
		*/
		x->sel_box.x = te_scale(pt.x, 0, r.width, x->time_min, x->time_max);
		x->sel_box.y = te_scale(pt.y, 0, r.height, x->freq_min, x->freq_max);
		x->sel_box.width = 0.;
		x->sel_box.height = 0;
		x->sel_type = BOX_SELECT;
		break;
	default:
		x->selected = NULL;
	}
	//jbox_invalidate_layer((t_object *)x, patcherview, l_function_layers[x->currentFunction]);
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
}

void te_mousedrag(t_te *x, t_object *patcherview, t_pt pt, long modifiers){
	if(x->lockFunctions[x->currentFunction]){
		return;
	}
	if(x->hideFunctions[x->currentFunction]){
		return;
	}
	x->mousemove = pt;

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

	t_pt min = {x->time_min, x->freq_min};
	t_pt max = {x->time_max, x->freq_max};
	switch(modifiers){
	case (0x11 | 0x12 | 0x18):
	case (0x111 | 0x112 | 0x118):{
		// using the mouse to change xmin and xmax
		//if(modifiers == 0x13 || modifiers == 0x113){
		t_pt scale;
		scale.x = fabs(pt.x - x->last_mouse.x);
		scale.y = fabs(pt.y - x->last_mouse.y);
		if(x->last_mouse.x < pt.x){
			min.x = x->time_min - (((x->time_max - x->time_min) / r.width) * scale.x);
			max.x = x->time_max - (((x->time_max - x->time_min) / r.width) * scale.x);
		}else if(x->last_mouse.x > pt.x){
			min.x = x->time_min + (((x->time_max - x->time_min) / r.width) * scale.x);
			max.x = x->time_max + (((x->time_max - x->time_min) / r.width) * scale.x);
		}

		if(x->last_mouse.y < pt.y){
			min.y = x->freq_min + (((x->freq_max - x->freq_min) / r.width) * scale.y);
			max.y = x->freq_max + (((x->freq_max - x->freq_min) / r.width) * scale.y);
		}else if(x->last_mouse.y > pt.y){
			min.y = x->freq_min - (((x->freq_max - x->freq_min) / r.width) * scale.y);
			max.y = x->freq_max - (((x->freq_max - x->freq_min) / r.width) * scale.y);
		}
		te_time_minmax(x, min.x, max.x);
		t_atom out[2];
		atom_setfloat(out, min.x);
		atom_setfloat(out + 1, max.x);
		//outlet_anything(x->out_osc, gensym("timeminmax"), 2, out);
		te_getTimeMinMax(x);
		break;
	}
	case 0x12:
	case 0x112:{
		// shift
		// change dimensions of the region selection box
		//printf("%s: %f %f %f\n", __func__, pt.x, te_scale(pt.x, 0, r.width, x->time_min, x->time_max), x->sel_box.x);
		x->sel_box.width = te_scale(pt.x, 0, r.width, x->time_min, x->time_max) - x->sel_box.x;
		te_clearSelected(x);
		if(x->apply_selection_box_to_all_functions_by_default){
			int function;
			for(function = 0; function < x->numFunctions; function++){
				te_selectPointsInBox(x, r, x->sel_box, function);
			}
		}else{
				te_selectPointsInBox(x, r, x->sel_box, x->currentFunction);
		}
		break;
	}
	case 0x13:
	case 0x113:{
		// command shift
		// change dimensions of the selection box
		x->sel_box.width = te_scale(pt.x, 0, r.width, x->time_min, x->time_max) - x->sel_box.x;
		x->sel_box.height = te_scale(pt.y, 0, r.height, x->freq_min, x->freq_max) - x->sel_box.y;
		te_clearSelected(x);
		if(x->apply_selection_box_to_all_functions_by_default){
			int function;
			for(function = 0; function < x->numFunctions; function++){
				te_selectPointsInBox(x, r, x->sel_box, function);
			}
		}else{
				te_selectPointsInBox(x, r, x->sel_box, x->currentFunction);
		}
		break;
	}
	default:{
		if(x->sel_box_selected){
			unsigned long box_selected = x->sel_box_selected;
			t_rect sel_box = (t_rect){
				te_scale(x->sel_box.x, x->time_min, x->time_max, 0, r.width),
				te_scale(x->sel_box.y, x->freq_min, x->freq_max, 0, r.height),
				te_scale(x->sel_box.width, x->time_min, x->time_max, 0, r.width),
				te_scale(x->sel_box.height, x->freq_min, x->freq_max, 0, r.height)
			};
			if(box_selected == BOX_SELECTED){
				t_pt delta;
				if(modifiers == 0x18){
					delta = (t_pt){pt.x - x->last_mouse.x, 0.};
				}else if(modifiers == 0x1A){
					delta = (t_pt){0., pt.y - x->last_mouse.y};
				}else{
					delta = (t_pt){pt.x - x->last_mouse.x, pt.y - x->last_mouse.y};
				}
				int firstone = 1;
				t_point *p = x->selected;
				while(p){
					t_pt sc = p->coords;
					double dfreq;
					dfreq = te_scale(p->d_freq, x->freq_min, x->freq_max, r.height, 0.);
					sc.x = te_scale(p->coords.x, x->time_min, x->time_max, 0., r.width);
					sc.x += delta.x;
					if(x->sel_type == BOX_SELECT){
						sc.y = te_scale(p->coords.y, x->freq_min, x->freq_max, r.height, 0.);
						sc.y += delta.y;
						dfreq += delta.y;
					}
					if(firstone){
						t_pt grid;
						te_findNearestGridPoint(x, sc, &grid);
						double diff = fabs(sc.x - grid.x);
						if(diff < 2.){
							delta.x += (grid.x - sc.x);
							sc.x = grid.x;
						}
						firstone = 0;
					}
					p->coords.x = te_scale(sc.x, 0., r.width, x->time_min, x->time_max);
					if(x->sel_type == BOX_SELECT){
						p->coords.y = te_scale(sc.y, r.height, 0., x->freq_min, x->freq_max);
						p->d_freq = te_scale(dfreq, r.height, 0., x->freq_min, x->freq_max);
					}

					te_reorderPoint(p);
					te_initReorderedPoint(x, p);

					p = p->next_selected;
				}
				sel_box.x += delta.x;
				if(x->sel_type == BOX_SELECT){
					sel_box.y += delta.y;
				}
			}else{
				if(box_selected & LEFT_EDGE){
					sel_box.x = pt.x;
					sel_box.width += (x->last_mouse.x - pt.x);
				}
				if(box_selected & RIGHT_EDGE){
					sel_box.width = pt.x - sel_box.x;
				}
				if(x->sel_type == BOX_SELECT){
					if(box_selected & TOP_EDGE){
						sel_box.y = pt.y;
						sel_box.height += x->last_mouse.y - pt.y;
					}
					if(box_selected & BOTTOM_EDGE){
						sel_box.height = pt.y - sel_box.y;
					}
				}
				te_clearSelected(x);
				te_selectPointsInBox(x, r, (t_rect){
						te_scale(sel_box.x, 0, r.width, x->time_min, x->time_max),
							te_scale(sel_box.y, 0, r.height, x->freq_min, x->freq_max),
							te_scale(sel_box.width, 0, r.width, x->time_min, x->time_max),
							te_scale(sel_box.height, 0, r.height, x->freq_min, x->freq_max)
							}, x->currentFunction);
			}
			x->sel_box = (t_rect){
				te_scale(sel_box.x, 0, r.width, x->time_min, x->time_max),
				te_scale(sel_box.y, 0, r.height, x->freq_min, x->freq_max),
				te_scale(sel_box.width, 0, r.width, x->time_min, x->time_max),
				te_scale(sel_box.height, 0, r.height, x->freq_min, x->freq_max)
			};
			goto out;
			break;
		}
		if(!x->selected){
			break;
		}
		if(x->selected->whichPoint && x->selected->next){
			x->selected->aux_points[x->selected->whichPoint - 1] = te_scale(pt.x, 0, r.width, x->time_min, x->time_max);
			x->selected->aux_points[x->selected->whichPoint - 1] = te_scale(x->selected->aux_points[x->selected->whichPoint - 1], x->selected->coords.x, x->selected->next->coords.x, 0., 1.);
			x->selected->aux_points[x->selected->whichPoint - 1] = te_clip(x->selected->aux_points[x->selected->whichPoint - 1], 0., 1.);

			if(x->selected->aux_points[0] > x->selected->aux_points[1]){
				x->selected->aux_points[0] = x->selected->aux_points[1];
			}
			break;
		}

		t_pt delta;
		te_w2s(x, r, x->selected->coords, &delta);
		if(x->snap_to_grid){
			te_findNearestGridPoint(x, pt, &pt);
		}else{
			t_point *p = x->selected->next;
			t_point *closest = NULL;
			while(p){
				if(!(p->selected) && p != x->selected && p->coords.x < x->freq_max){
					t_pt sc;
					te_w2s(x, r, p->coords, &sc);
					if(abs(sc.y - pt.y) <= 4){
						if(closest){
							if(abs(p->coords.y - closest->coords.y) > abs(pt.y - p->coords.y)){
								closest = p;
							}
						}else{
							closest = p;
						}
					}
				}
				p = p->next;
			}
			p = x->selected->prev;
			while(p){
				if(!(p->selected) && p != x->selected && p->coords.x > x->freq_min){
					t_pt sc;
					te_w2s(x, r, p->coords, &sc);
					if(abs(sc.y - pt.y) <= 4){
						if(closest){
							if(abs(p->coords.y - closest->coords.y) > abs(pt.y - p->coords.y)){
								closest = p;
							}
						}else{
							closest = p;
						}
					}
				}
				p = p->prev;
			}
			if(closest){
				pt.y = te_scale(closest->coords.y, x->freq_min, x->freq_max, r.height, 0);
				x->selected->closest_point_with_same_ycoord = closest;
				x->selected->draw_snapline = 1;
			}else{
				x->selected->closest_point_with_same_ycoord = NULL;
				x->selected->draw_snapline = 0;
			}
		}

		if(modifiers == 0x18 || modifiers == 0x118){
			delta.x -= pt.x;
			delta.y = 0;
			x->selected->coords.x = te_scale(pt.x, 0., r.width, x->time_min, x->time_max);
		}else if(modifiers == 0x1A || modifiers == 0x11A){
			delta.x = 0;
			delta.y -= pt.y;
			if(x->selected->d_freq == x->selected->coords.y){
				x->selected->d_freq = te_scale(pt.y, r.height, 0, x->freq_min, x->freq_max);
			}
			x->selected->coords.y = te_scale(pt.y, r.height, 0., x->freq_min, x->freq_max);
		}else{
			delta.x -= pt.x;
			delta.y -= pt.y;
			if(x->selected->d_freq == x->selected->coords.y){
				x->selected->d_freq = te_scale(pt.y, r.height, 0, x->freq_min, x->freq_max);
			}
			te_s2w(x, r, pt, &(x->selected->coords));
		}

		te_reorderPoint(x->selected);
		te_initReorderedPoint(x, x->selected);

		t_point *p = x->selected->next_selected;
		while(p){
			int same = 0;
			if(p->coords.y == p->d_freq){
				same = 1;
			}
			te_w2s(x, r, p->coords, &(p->coords));
			p->coords.x -= delta.x;
			p->coords.y -= delta.y;
			te_s2w(x, r, p->coords, &(p->coords));
			if(same){
				p->d_freq = p->coords.y;
			}
			te_reorderPoint(p);
			te_initReorderedPoint(x, p);
			p = p->next_selected;
		}
	}
	}

 out:
	x->last_mouse = pt;
	//jbox_invalidate_layer((t_object *)x, patcherview, l_function_layers[x->currentFunction]);
	te_invalidateAllFunctions(x);
	jbox_invalidate_layer((t_object *)x, patcherview, l_xycoords);
	jbox_redraw((t_jbox *)x);
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
}

void te_mousemove(t_te *x, t_object *patcherview, t_pt pt, long modifiers){
	//x->mousemove = pt;
	//jbox_invalidate_layer((t_object *)x, patcherview, l_xycoords);
	//jbox_redraw((t_jbox *)x);
}

void te_repeatSelection(t_te *x){
	t_point *p, *start = NULL, *end = NULL;
	double starttime = DBL_MAX, endtime = 0;
	te_critical_enter(x->lock, __LINE__);
	p = x->selected;
	while(p){
		if(p->coords.x < starttime){
			starttime = p->coords.x;
			start = p;
		}
		if(p->coords.x > endtime){
			endtime = p->coords.x;
			end = p;
		}
		p = p->next_selected;
	}
	p = x->selected;
	double delta = endtime - starttime;
	t_point *newpoint = NULL;
	while(p){
		if(p != start){
			newpoint = te_allocAndInsertPoint(x, (t_pt){p->coords.x + delta, p->coords.y}, x->currentFunction);
			te_initPoint(x, newpoint);
			newpoint->alpha = p->alpha;
			newpoint->beta = p->beta;
			newpoint->d_freq = p->d_freq;
		}
		if(p == end){
			end->alpha = start->alpha;
			end->beta = start->beta;
			end->d_freq = start->coords.y;
		}
		p = p->next_selected;
	}
	te_critical_exit(x->lock, __LINE__);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[x->currentFunction]);
	jbox_redraw((t_jbox *)x);
}

void te_cut(t_te *x){
	if(!x->selected){
		object_error((t_object *)x, "you must select something first");
		return;
	}
	te_critical_enter(x->lock, __LINE__);
	t_point *selected = x->selected;
	x->selected = NULL;
	t_point *p = selected;
	while(p){
		if(p->next){
			p->next->prev = p->prev;
		}
		if(p->prev){
			p->prev->next = p->next;
		}
		if(p == x->functions[x->currentFunction]){
			x->functions[x->currentFunction] = NULL;
		}
		p = p->next_selected;
	}
	te_critical_exit(x->lock, __LINE__);
	critical_enter(te_clipboard_lock);
	if(te_clipboard->s_thing){
		p = (t_point *)(te_clipboard->s_thing);
		t_point *next = NULL;
		while(p){
			next = p->next_selected;
			free(p);
			p = next;
		}
	}
	te_clipboard->s_thing = (t_object *)selected;
	critical_exit(te_clipboard_lock);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[x->currentFunction]);
	jbox_redraw((t_jbox *)x);
}

void te_copy(t_te *x){
	if(!x->selected){
		object_error((t_object *)x, "you must select something first");
		return;
	}
	t_point *copy = NULL, *sel = NULL;
	te_critical_enter(x->lock, __LINE__);
	sel = x->selected;
	while(sel->next_selected){
		sel = sel->next_selected;
	}
	while(sel){
		t_point *p = (t_point *)calloc(1, sizeof(t_point));
		*p = *sel;
		p->next = NULL;
		p->prev = NULL;
		if(copy){
			copy->prev_selected = p;
		}
		p->next_selected = copy;
		p->prev_selected = NULL;
		copy = p;
		sel = sel->prev_selected;
	}
	te_critical_exit(x->lock, __LINE__);
	critical_enter(te_clipboard_lock);
	if(te_clipboard->s_thing){
		t_point *p = (t_point *)(te_clipboard->s_thing);
		t_point *next = NULL;
		while(p){
			next = p->next_selected;
			free(p);
			p = next;
		}
	}
	te_clipboard->s_thing = (t_object *)copy;
	critical_exit(te_clipboard_lock);
}

void te_paste(t_te *x){
	t_point *cb = NULL;
	critical_enter(te_clipboard_lock);
	cb = (t_point *)(te_clipboard->s_thing);
	if(!cb){
		critical_exit(te_clipboard_lock);
		return;
	}
	double minx = DBL_MAX, miny = 0;
	while(cb){
		if(cb->coords.x < minx){
			minx = cb->coords.x;
			miny = cb->coords.y;
		}
		cb = cb->next_selected;
	}
	critical_exit(te_clipboard_lock);
	te_pasteAtCoords(x, minx, miny);
}

void te_pasteAtTime(t_te *x, double time){
	t_point *cb = NULL;
	critical_enter(te_clipboard_lock);
	cb = (t_point *)(te_clipboard->s_thing);
	if(!cb){
		critical_exit(te_clipboard_lock);
		return;
	}
	double minx = DBL_MAX, miny = 0;
	while(cb){
		if(cb->coords.x < minx){
			minx = cb->coords.x;
			miny = cb->coords.y;
		}
		cb = cb->next_selected;
	}
	critical_exit(te_clipboard_lock);
	te_pasteAtCoords(x, time, miny);
}

void te_pasteAtCoords(t_te *x, double xx, double yy){
	t_point *copy = NULL;
	t_point *cb = NULL;
	critical_enter(te_clipboard_lock);
	cb = (t_point *)(te_clipboard->s_thing);
	double minx = DBL_MAX, miny = 0;
	while(cb){
		if(cb->coords.x < minx){
			minx = cb->coords.x;
			miny = cb->coords.y;
		}
		t_point *p = (t_point *)calloc(1, sizeof(t_point));
		*p = *cb;
		p->next = copy;
		copy = p;
		cb = cb->next_selected;
	}
	t_pt delta = (t_pt){xx - minx, yy - miny};
	critical_exit(te_clipboard_lock);
	te_critical_enter(x->lock, __LINE__);
	t_point *left = NULL, *right = NULL;
	te_clearSelected(x);
	while(copy){
		t_point *next = copy->next;
		if(x->selected){
			x->selected->prev_selected = copy;
		}
		copy->prev_selected = NULL;
		copy->next_selected = x->selected;
		copy->coords.x += delta.x;
		copy->coords.y += delta.y;
		copy->d_freq += delta.y;
		x->selected = copy;
		te_find_btn(x->functions[x->currentFunction], copy->coords.x, &left, &right);
		copy->prev = left;
		copy->next = right;
		if(left){
			left->next = copy;
		}else{
			x->functions[x->currentFunction] = copy;
		}
		if(right){
			right->prev = copy;
		}
		copy = next;
	}
	te_critical_exit(x->lock, __LINE__);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[x->currentFunction]);
	jbox_redraw((t_jbox *)x);
}

void te_findNearestGridPoint(t_te *x, t_pt pt_sc, t_pt *pt_out_sc){
	double step;
	if(x->show_minor_x_grid){
		step = x->major_grid_width_sec / x->num_minor_x_grid_divisions;
	}else{
		step = x->major_grid_width_sec;
	}
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_pt pt;
	pt.x = te_scale(pt_sc.x, 0, r.width, x->time_min, x->time_max);
	pt.y = te_scale(pt_sc.y, r.height, 0., x->freq_min, x->freq_max);
	pt_out_sc->x = x->time_min;
	pt_out_sc->y = x->freq_min;

	while(pt_out_sc->x <= pt.x){
		pt_out_sc->x += step;
	}
	if(fabs(pt_out_sc->x - pt.x) > fabs((pt_out_sc->x - step) - pt.x)){
		pt_out_sc->x -= step;
	}
	pt_out_sc->x = te_scale(pt_out_sc->x, x->time_min, x->time_max, 0, r.width);

	if(x->show_minor_y_grid){
		step = x->major_grid_height_bps / x->num_minor_y_grid_divisions;
	}else{
		step = x->major_grid_height_bps;
	}
	while(pt_out_sc->y <= pt.y){
		pt_out_sc->y += step;
	}
	if(fabs(pt_out_sc->y - pt.y) > fabs((pt_out_sc->y - step) - pt.y)){
		pt_out_sc->y -= step;
	}
	pt_out_sc->y = te_scale(pt_out_sc->y, x->freq_min, x->freq_max, r.height, 0);
}

void te_showgrid(t_te *x, long b){
	x->show_major_x_grid = b;
	x->show_minor_x_grid = b;
	x->show_major_y_grid = b;
	x->show_minor_y_grid = b;
	jbox_invalidate_layer((t_object *)x, x->pv, l_xgrid);
	jbox_invalidate_layer((t_object *)x, x->pv, l_ygrid);
	jbox_redraw((t_jbox *)x);
}

void te_mouseup(t_te *x, t_object *patcherview, t_pt pt, long modifiers){
	x->mousestate = 0;
	if(x->selected){
		x->selected->draw_snapline = 0;
	}
	//x->sel_box = (t_rect){0., 0., 0., 0.};
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[x->currentFunction]);
	jbox_redraw((t_jbox *)x);

	struct _mouseup{
		char header[16];
		char size[4];
		char address[12];
		char tt[4];
	};
	struct _mouseup mouseup = {
		"#bundle\0\0\0\0\0\0\0\0",
		{0, 0, 0, 16},
		"/mouse/up\0\0",
		",\0\0"
	};
	t_atom out[2];
	atom_setlong(out, sizeof(mouseup));
	atom_setlong(out + 1, (long)&mouseup);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
}

void te_addFunction(t_te *x){
	te_critical_enter(x->lock, __LINE__);
	if(x->numFunctions + 1 > MAX_NUM_FUNCTIONS){
		error("te: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	x->numFunctions++;
	te_critical_exit(x->lock, __LINE__);
	te_setFunction(x, x->numFunctions - 1);
	te_invalidateAllFunctions(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_legend);
	jbox_redraw((t_jbox *)x);
}

void te_setFunction(t_te *x, long f){
	if(f > MAX_NUM_FUNCTIONS){
		error("te: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	if(f > x->numFunctions - 1){
		while(f > x->numFunctions - 1){
			x->numFunctions++;
		}
	}
	if(f < 0){
		error("te: function must be a positive number");
		return;
	}
	te_clearSelected(x);
	te_critical_enter(x->lock, __LINE__);
	x->currentFunction = f;
	te_critical_exit(x->lock, __LINE__);
	te_dumpCellblock(x);
	te_invalidateAllFunctions(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_legend);
	jbox_redraw((t_jbox *)x);
}

double te_clip(double f, double min, double max){
	double ff = f;
	if(ff < min){
		ff = min;
	}else if(ff > max){
		ff = max;
	}
	return ff;
}

double te_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

void te_s2w(t_te *x, t_rect r, t_pt screen_coords, t_pt *world_coords){
	world_coords->x = te_scale(screen_coords.x, 0, r.width, x->time_min, x->time_max);
	world_coords->y = te_scale(screen_coords.y, r.height, 0, x->freq_min, x->freq_max);
}

void te_w2s(t_te *x, t_rect r, t_pt world_coords, t_pt *screen_coords){
	screen_coords->x = te_scale(world_coords.x, x->time_min, x->time_max, 0, r.width);
	screen_coords->y = te_scale(world_coords.y, x->freq_min, x->freq_max, r.height, 0);
}

void te_initPoint(t_te *x, t_point *p){
	if(!p){
		return;
	}
	p->d_freq = p->coords.y;
	p->a_phase = p->d_phase = 0;
	p->aux_points[0] = x->error_offset;
	p->aux_points[1] = x->error_span + x->error_offset;
	p->alpha = p->beta = 1.;
	p->error_alpha = p->error_beta = 2.;
}

t_point *te_allocAndInsertPoint(t_te *x, t_pt coords, int functionNum){
	t_point *p = (t_point *)calloc(1, sizeof(t_point));
	p->next_selected = NULL;
	p->prev_selected = NULL;
	p->selected = 0;
	p->coords = coords;
	te_insertPoint(x, p, functionNum);
	return p;
}

void te_insertPoint(t_te *x, t_point *p, int functionNum){
	te_critical_enter(x->lock, __LINE__);
	t_point **function = &(x->functions[functionNum]);
	if(*function == NULL){
		p->prev = NULL;
		p->next = NULL;
		*function = p;
		if(x->numFunctions <= functionNum){
			x->numFunctions = functionNum + 1;
		}
	}else if(p->coords.x < (*function)->coords.x){
		p->prev = NULL;
		p->next = (*function);
		(*function)->prev = p;
		x->functions[functionNum] = p;
		p->aux_points[0] = x->error_offset;
		p->aux_points[1] = x->error_span + x->error_offset;
	}else{
		int i = 1;
		t_point *current, *next;
		current = (*function);
		next = current->next;
		while(next){
			if(p->coords.x >= current->coords.x && p->coords.x <= next->coords.x){
				p->aux_points[0] = x->error_offset;
				p->aux_points[1] = x->error_span + x->error_offset;

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

		if(p->prev->aux_points[0] == 0 && p->prev->aux_points[1] == 0){
			current->aux_points[0] = x->error_offset;
			current->aux_points[1] = x->error_span + x->error_offset;
		}
	}
 out:
	te_critical_exit(x->lock, __LINE__);
}

void te_removePoint(t_te *x, t_point *point, int functionNum){
	if(!point){
		return;
	}
	te_critical_enter(x->lock, __LINE__);
	t_point **function = &(x->functions[functionNum]);
	t_point *p = *function;
	int i = 0;
	while(p){
		if(p == point){
			if(p->prev){
				p->prev->next = p->next;
			}
			if(p->next){
				p->next->prev = p->prev;
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
	te_critical_exit(x->lock, __LINE__);
}

void te_dump(t_te *x){
	//double rdtsc = RDTSC_CYCLES_PER_SECOND;
	//TIMER_START(bar, rdtsc);
	t_osc_bndl_u *b = osc_bundle_u_alloc();
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		if(!p){
			continue;
		}
		t_osc_msg_u *m = osc_message_u_alloc();
		char address[128];
		sprintf(address, "/dump/function/%d", i);
		osc_message_u_setAddress(m, address);

		t_osc_bndl_u *fb = osc_bundle_u_alloc();

		t_osc_msg_u *function_num = osc_message_u_alloc();
		osc_message_u_setAddress(function_num, "/function");
		osc_message_u_appendInt32(function_num, i);
		osc_bundle_u_addMsg(fb, function_num);

		int p_num = 0;
		while(p){
			t_osc_msg_u *pm = osc_message_u_alloc();
			char pm_address[128];
			sprintf(pm_address, "/point/%d", p_num);
			osc_message_u_setAddress(pm, pm_address);

			t_osc_bndl_u *pmb = osc_bundle_u_alloc();

			//osc_bundle_u_addMsg(pmb, function_num);
			t_osc_msg_u *pmb_function_num = osc_message_u_alloc();
			osc_message_u_setAddress(pmb_function_num, "/function");
			osc_message_u_appendInt32(pmb_function_num, i);
			osc_bundle_u_addMsg(pmb, pmb_function_num);

			t_osc_msg_u *point = osc_message_u_alloc();
			osc_message_u_setAddress(point, "/point");
			osc_message_u_appendInt32(point, p_num);
			osc_bundle_u_addMsg(pmb, point);

			t_osc_msg_u *time = osc_message_u_alloc();
			osc_message_u_setAddress(time, "/time");
			osc_message_u_appendFloat(time, p->coords.x);
			osc_bundle_u_addMsg(pmb, time);

			t_osc_msg_u *tempo_arrival = osc_message_u_alloc();
			osc_message_u_setAddress(tempo_arrival, "/tempo/arrival");
			osc_message_u_appendFloat(tempo_arrival, p->coords.y);
			osc_bundle_u_addMsg(pmb, tempo_arrival);

			t_osc_msg_u *tempo_departure = osc_message_u_alloc();
			osc_message_u_setAddress(tempo_departure, "/tempo/departure");
			osc_message_u_appendFloat(tempo_departure, p->d_freq);
			osc_bundle_u_addMsg(pmb, tempo_departure);

			t_osc_msg_u *phase_arrival = osc_message_u_alloc();
			osc_message_u_setAddress(phase_arrival, "/phase/arrival");
			osc_message_u_appendFloat(phase_arrival, p->a_phase);
			osc_bundle_u_addMsg(pmb, phase_arrival);

			t_osc_msg_u *phase_departure = osc_message_u_alloc();
			osc_message_u_setAddress(phase_departure, "/phase/departure");
			osc_message_u_appendFloat(phase_departure, p->d_phase);
			osc_bundle_u_addMsg(pmb, phase_departure);

			t_osc_msg_u *alpha = osc_message_u_alloc();
			osc_message_u_setAddress(alpha, "/alpha");
			osc_message_u_appendFloat(alpha, p->alpha);
			osc_bundle_u_addMsg(pmb, alpha);

			t_osc_msg_u *beta = osc_message_u_alloc();
			osc_message_u_setAddress(beta, "/beta");
			osc_message_u_appendFloat(beta, p->beta);
			osc_bundle_u_addMsg(pmb, beta);

			t_osc_msg_u *error_alpha = osc_message_u_alloc();
			osc_message_u_setAddress(error_alpha, "/error/alpha");
			osc_message_u_appendFloat(error_alpha, p->error_alpha);
			osc_bundle_u_addMsg(pmb, error_alpha);

			t_osc_msg_u *error_beta = osc_message_u_alloc();
			osc_message_u_setAddress(error_beta, "/error/beta");
			osc_message_u_appendFloat(error_beta, p->error_beta);
			osc_bundle_u_addMsg(pmb, error_beta);

			//long mb_s_len = 0;
			//char *mb_s = NULL;
			//osc_bundle_u_serialize(pmb, &mb_s_len, &mb_s);
			//osc_message_u_appendBndl(pm, mb_s_len, mb_s);
            osc_message_u_appendBndl_u(pm, pmb);
			osc_bundle_u_addMsg(fb, pm);

			//if(mb_s){
			//	osc_mem_free(mb_s);
			//}
			p = p->next;
			p_num++;
		}
		//long fb_s_len = 0;
		//char *fb_s = NULL;
		//osc_bundle_u_serialize(fb, &fb_s_len, &fb_s);
        t_osc_bndl_s *fb_s = osc_bundle_u_serialize(fb);
		osc_message_u_appendBndl(m, osc_bundle_s_getLen(fb_s), osc_bundle_s_getPtr(fb_s));
		if(fb_s){
			//osc_mem_free(fb_s);
            osc_bundle_s_deepFree(fb_s);
		}
		osc_bundle_u_addMsg(b, m);
	}
	//long len = 0;
	//char *bndl_s = NULL;
	//osc_bundle_u_serialize(b, &len, &bndl_s);
    t_osc_bndl_s *sb = osc_bundle_u_serialize(b);
    long len = osc_bundle_s_getLen(sb);
    char *bndl_s = osc_bundle_s_getPtr(sb);

	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)bndl_s);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);

	//osc_mem_free(bndl_s);
    osc_bundle_s_deepFree(sb);
	osc_bundle_u_free(b);

	//TIMER_STOP(bar, rdtsc);
	//TIMER_PRINTF(bar);
	return;
	//////////////////////////////////////////////////
	// OLD
	//////////////////////////////////////////////////
	{
	//TIMER_START(foo, rdtsc);
	t_atom out[2];

	char function_address[] = "/dump/function/";
	int function_address_size = sizeof(function_address) - 1; // don't want the null byte
	char time_address[] = "/time\0";
	char atempo_address[] = "/tempo/arrival";
	char dtempo_address[] = "/tempo/departure\0\0";
	char aphase_address[] = "/phase/arrival";
	char dphase_address[] = "/phase/departure\0\0";
	char alpha_address[] = "/alpha";
	char beta_address[] = "/beta\0";
	char ealpha_address[] = "/error/alpha\0\0";
	char ebeta_address[] = "/error/beta\0\0\0";

	char *buf = (char *)calloc(1024, sizeof(char));
	int bufsize = 1024, bufpos = 0;

	// header
	strncpy(buf + bufpos, "#bundle\0\0\0\0\0\0\0\0\0", 16);
	bufpos += 16;
	
	int i;
	for(i = 0; i < x->numFunctions; i++){
		int npoints = 0;
		te_critical_enter(x->lock, __LINE__);
		t_point *p = x->functions[i];
		if(!p){
			continue;
		}
		while(p){
			npoints++;
			p = p->next;
		}
		
		uint32_t time[npoints];
		uint32_t atempo[npoints];
		uint32_t dtempo[npoints];
		uint32_t aphase[npoints];
		uint32_t dphase[npoints];
		uint32_t alpha[npoints];
		uint32_t beta[npoints];
		uint32_t ealpha[npoints];
		uint32_t ebeta[npoints];
		int j = 0;
		p = x->functions[i];
		while(p){
			float f = (float)(p->coords.x);
			time[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->coords.y);
			atempo[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->d_freq);
			dtempo[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->a_phase);
			aphase[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->d_phase);
			dphase[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->alpha);
			alpha[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->beta);
			beta[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->error_alpha);
			ealpha[j] = hton32(*((uint32_t *)(&f)));
			f = (float)(p->error_beta);
			ebeta[j] = hton32(*((uint32_t *)(&f)));
			
			p = p->next;
			j++;
		}

		int padded = (npoints + 1) + (4 - ((npoints + 1) % 4));
		char tt[padded];
		memset(tt, '\0', padded);
		int stt = sizeof(tt);
		memset(tt, 'f', npoints + 1);
		tt[0] = ',';

		int bndlsize = 
			(9 * function_address_size) + // /function/
			(9 * 2) + 
			(9 * 4) + // sizes
			sizeof(time_address) +
			sizeof(atempo_address) +
			sizeof(dtempo_address) +
			sizeof(aphase_address) +
			sizeof(dphase_address) +
			sizeof(alpha_address) +
			sizeof(beta_address) +
			sizeof(ealpha_address) +
			sizeof(ebeta_address) +
			(9 * sizeof(tt)) + // typetags
			sizeof(time) +
			sizeof(atempo) +
			sizeof(dtempo) +
			sizeof(aphase) +
			sizeof(dphase) +
			sizeof(alpha) +
			sizeof(beta) +
			sizeof(ealpha) +
			sizeof(ebeta);

		if((bufpos + bndlsize) > bufsize){
			buf = (char *)realloc(buf, bufsize + bndlsize);
			memset(buf + bufpos, '\0', bndlsize);
			bufsize += bndlsize;
		}


		int npoints4 = npoints * 4;
		int rest = function_address_size + 2 + stt + (npoints * 4);

		char fn[3];
		sprintf(fn, "%02d", i);
		
		// time
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(time_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), time_address, sizeof(time_address));
		bufpos += sizeof(time_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), time, npoints4);
		bufpos += npoints4;

		// atempo
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(atempo_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), atempo_address, sizeof(atempo_address));
		bufpos += sizeof(atempo_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), atempo, npoints4);
		bufpos += npoints4;

		// dtempo
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(dtempo_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), dtempo_address, sizeof(dtempo_address));
		bufpos += sizeof(dtempo_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), dtempo, npoints4);
		bufpos += npoints4;

		// aphase
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(aphase_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), aphase_address, sizeof(aphase_address));
		bufpos += sizeof(aphase_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), aphase, npoints4);
		bufpos += npoints4;

		// dphase
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(dphase_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), dphase_address, sizeof(dphase_address));
		bufpos += sizeof(dphase_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), dphase, npoints4);
		bufpos += npoints4;

		// alpha
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(alpha_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), alpha_address, sizeof(alpha_address));
		bufpos += sizeof(alpha_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), alpha, npoints4);
		bufpos += npoints4;

		// beta
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(beta_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), beta_address, sizeof(beta_address));
		bufpos += sizeof(beta_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), beta, npoints4);
		bufpos += npoints4;

		// ealpha
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(ealpha_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), ealpha_address, sizeof(ealpha_address));
		bufpos += sizeof(ealpha_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), ealpha, npoints4);
		bufpos += npoints4;

		// ebeta
		*((uint32_t *)(buf + bufpos)) = hton32(sizeof(ebeta_address) + rest);
		bufpos += 4;
		memcpy((buf + bufpos), function_address, function_address_size);
		bufpos += function_address_size;
		memcpy((buf + bufpos), fn, 2);
		bufpos += 2;
		memcpy((buf + bufpos), ebeta_address, sizeof(ebeta_address));
		bufpos += sizeof(ebeta_address);
		memcpy((buf + bufpos), tt, stt);
		bufpos += stt;
		memcpy((buf + bufpos), ebeta, npoints4);
		bufpos += npoints4;

		te_critical_exit(x->lock, __LINE__);
		
	}
	atom_setlong(out, bufpos);
	atom_setlong(out + 1, (long)buf);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
	if(buf){
		free(buf);
	}

	//TIMER_STOP(foo, rdtsc);
	//TIMER_PRINTF(foo);
	}
}

void te_processArgs_r(t_te *x,
			     t_symbol *key,
			     long argc, 
			     t_atom *argv, 
			     long *numfunctions, 
			     long *functions, 
			     long *rangelen, 
			     double *range, 
			     long *numsubdivs, 
			     double *subdivs, 
			     long *numcontrolpoints, 
			     long *controlpoints)
{
	if(!argc){
		return;
	}
	long i;
	t_symbol *k = NULL;

	if(key){
		k = key;
		if(atom_gettype(argv) == A_SYM){
			// this is either a new key or a * to signify 
			// all functions.
			if(key == gensym("functions") && atom_getsym(argv) == gensym("*")){
				// put all the functions in
				for(i = 0; i < x->numFunctions; i++){
					functions[(*numfunctions)++] = i;
				}
			}else{
				// new key
				k = atom_getsym(argv);
			}
		}else{
			if(k == gensym("functions")){
				functions[(*numfunctions)++] = atom_getlong(argv);
			}else if(k == gensym("timerange")){
				range[(*rangelen)++] = atom_getfloat(argv);
			}else if(k == gensym("subdivs")){
				subdivs[(*numsubdivs)++] = atom_getfloat(argv);
			}else if(k == gensym("controlpoints")){
				controlpoints[(*numcontrolpoints)++] = atom_getlong(argv);
			}else{
				key = NULL;
			}
		}
	}else{
		if(atom_gettype(argv) == A_SYM){
			k = atom_getsym(argv);
		}
	}
	te_processArgs_r(x, k, argc - 1, argv + 1, numfunctions, functions, rangelen, range, numsubdivs, subdivs, numcontrolpoints, controlpoints);
	return;
}

void te_dumpBeats(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	long numfunctions = 0;
	long functions[MAX_NUM_FUNCTIONS];
	long rangelen = 0;
	double range[2];
	long numsubdivs = 0;
	double subdivs[argc];
	long numcontrolpoints = 0;
	long controlpoints[argc];
	te_processArgs_r(x, NULL, argc, argv, &numfunctions, functions, &rangelen, range, &numsubdivs, subdivs, &numcontrolpoints, controlpoints);

	if(!numfunctions){
		numfunctions = 1;
		functions[0] = x->currentFunction;
	}
	if(!rangelen && !numcontrolpoints){
		range[0] = x->time_min;
		range[1] = x->time_max;
		rangelen = 2;
	}

	int i, j;
	if(rangelen){
		for(i = 0; i < numfunctions; i++){
			te_dumpBeatsForFunction(x, functions[i], numsubdivs, subdivs, range[0], range[1]);
		}
	}

	if(numcontrolpoints){
		t_point *p = NULL;
		long pointnum;
		for(i = 0; i < numfunctions; i++){
			p = x->functions[functions[i]];
			pointnum = 1;
			for(j = 0; j < numcontrolpoints; j++){
				while(p && pointnum < controlpoints[j]){
					pointnum++;
					p = p->next;
				}
				if(pointnum == controlpoints[j] && p){
					if(p->next){
						te_dumpBeatsForFunction(x, functions[i], numsubdivs, subdivs, p->coords.x, p->next->coords.x);
					}
				}
			}
		}
	}
	//t_atom out;
	//atom_setsym(&out, gensym("done"));
	//outlet_anything(x->out_osc, gensym("dumpbeats"), 1, &out);
}

int te_compare(const void *a, const void *b){
	if(*((double *)a) < *((double *)b)){
		return -1;
	}else if(*((double *)a) > *((double *)b)){
		return 1;
	}else{
		return 0;
	}
}

void te_outputBeatOSC(t_te *x, long function, long point, long beat, float time, float tempo, float phase){
	t_osc_bndl_u *bndl = osc_bundle_u_alloc();
	t_osc_msg_u *msg = osc_message_u_alloc();
	char address[32];
	sprintf(address, "/function/%ld/beat", function);
	osc_message_u_setAddress(msg, address);
	t_osc_bndl_u *nested_bndl = osc_bundle_u_alloc();
	t_osc_msg_u *function_msg = osc_message_u_alloc();
	osc_message_u_setAddress(function_msg, "/function");
	osc_message_u_appendInt32(function_msg, function);
	osc_bundle_u_addMsg(nested_bndl, function_msg);

	t_osc_msg_u *point_msg = osc_message_u_alloc();
	osc_message_u_setAddress(point_msg, "/point");
	osc_message_u_appendInt32(point_msg, point);
	osc_bundle_u_addMsg(nested_bndl, point_msg);

	t_osc_msg_u *beat_msg = osc_message_u_alloc();
	osc_message_u_setAddress(beat_msg, "/beat");
	osc_message_u_appendInt32(beat_msg, beat);
	osc_bundle_u_addMsg(nested_bndl, beat_msg);

	t_osc_msg_u *time_msg = osc_message_u_alloc();
	osc_message_u_setAddress(time_msg, "/time");
	osc_message_u_appendFloat(time_msg, time);
	osc_bundle_u_addMsg(nested_bndl, time_msg);

	t_osc_msg_u *tempo_msg = osc_message_u_alloc();
	osc_message_u_setAddress(tempo_msg, "/tempo");
	osc_message_u_appendFloat(tempo_msg, tempo);
	osc_bundle_u_addMsg(nested_bndl, tempo_msg);

	t_osc_msg_u *phase_msg = osc_message_u_alloc();
	osc_message_u_setAddress(phase_msg, "/phase");
	osc_message_u_appendFloat(phase_msg, phase);
	osc_bundle_u_addMsg(nested_bndl, phase_msg);

	//char *nested_bndl_s = NULL;
	//long nested_bndl_len_s = 0;
	//osc_bundle_u_serialize(nested_bndl, &nested_bndl_len_s, &nested_bndl_s);
	//osc_message_u_appendBndl(msg, nested_bndl_len_s, nested_bndl_s);
    osc_message_u_appendBndl_u(msg, nested_bndl);
	osc_bundle_u_addMsg(bndl, msg);

	//char *bndl_s = NULL;
	//long bndl_len_s = 0;

	//osc_bundle_u_serialize(bndl, &bndl_len_s, &bndl_s);
    t_osc_bndl_s *sb = osc_bundle_u_serialize(bndl);
    char *bndl_s = osc_bundle_s_getPtr(sb);
    long bndl_len_s = osc_bundle_s_getLen(sb);

	t_atom foo[2];
	atom_setlong(foo, bndl_len_s);
	atom_setlong(foo + 1, (long)bndl_s);
	outlet_anything(x->out_osc, ps_FullPacket, 2, foo);

	osc_bundle_u_free(bndl);
	//osc_mem_free(nested_bndl_s);
	//osc_mem_free(bndl_s);
    osc_bundle_s_deepFree(sb);

	return;

	struct {
		char header[16];
		char function_size[4];
		char function_address[16]; // /beat/function\0\0
		char function_tt[4];
		char function_data[4];

		char point_size[4];
		char point_address[12]; // /beat/point\0
		char point_tt[4];
		char point_data[4];

		char beat_size[4];
		char beat_address[12]; // /beat/beat\0\0
		char beat_tt[4];
		char beat_data[4];

		char time_size[4];
		char time_address[12]; // /beat/time\0\0
		char time_tt[4];
		char time_data[4];

		char tempo_size[4];
		char tempo_address[12]; // /beat/tempo\0
		char tempo_tt[4];
		char tempo_data[4];

		char phase_size[4];
		char phase_address[12]; // /beat/phase\0
		char phase_tt[4];
		char phase_data[4];
	} bt = {
		"#bundle\0\0\0\0\0\0\0\0",
		{0, 0, 0, 24},
		"/beat/function\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"/beat/point",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"/beat/beat\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"/beat/time\0",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"/beat/tempo",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"/beat/phase",
		",f\0",
		"\0\0\0"
	};
	*((uint32_t *)bt.function_data) = hton32(function);
	*((uint32_t *)bt.point_data) = hton32(point);
	*((uint32_t *)bt.beat_data) = hton32(beat);
	*((uint32_t *)bt.time_data) = hton32(*((uint32_t *)(&time)));
	*((uint32_t *)bt.tempo_data) = hton32(*((uint32_t *)(&tempo)));
	*((uint32_t *)bt.phase_data) = hton32(*((uint32_t *)(&phase)));
	t_atom out[2];
	atom_setlong(out, sizeof(bt));
	atom_setlong(out + 1, (long)&bt);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
}

void te_outputSubdivOSC(t_te *x, long function, long point, long beat, long subdivtype, long subdiv, float time, float tempo, float phase){
	t_osc_bndl_u *bndl = osc_bundle_u_alloc();
	t_osc_msg_u *msg = osc_message_u_alloc();
	char address[32];
	sprintf(address, "/function/%ld/subdiv/%ld", function, subdivtype);
	osc_message_u_setAddress(msg, address);
	t_osc_bndl_u *nested_bndl = osc_bundle_u_alloc();
	t_osc_msg_u *function_msg = osc_message_u_alloc();

	osc_message_u_setAddress(function_msg, "/function");
	osc_message_u_appendInt32(function_msg, function);
	osc_bundle_u_addMsg(nested_bndl, function_msg);

	t_osc_msg_u *point_msg = osc_message_u_alloc();
	osc_message_u_setAddress(point_msg, "/point");
	osc_message_u_appendInt32(point_msg, point);
	osc_bundle_u_addMsg(nested_bndl, point_msg);

	t_osc_msg_u *beat_msg = osc_message_u_alloc();
	osc_message_u_setAddress(beat_msg, "/beat");
	osc_message_u_appendInt32(beat_msg, beat);
	osc_bundle_u_addMsg(nested_bndl, beat_msg);

	t_osc_msg_u *subdivtype_msg = osc_message_u_alloc();
	osc_message_u_setAddress(subdivtype_msg, "/subdivtype");
	osc_message_u_appendInt32(subdivtype_msg, subdivtype);
	osc_bundle_u_addMsg(nested_bndl, subdivtype_msg);

	t_osc_msg_u *subdiv_msg = osc_message_u_alloc();
	osc_message_u_setAddress(subdiv_msg, "/subdiv");
	osc_message_u_appendInt32(subdiv_msg, subdiv);
	osc_bundle_u_addMsg(nested_bndl, subdiv_msg);

	t_osc_msg_u *time_msg = osc_message_u_alloc();
	osc_message_u_setAddress(time_msg, "/time");
	osc_message_u_appendFloat(time_msg, time);
	osc_bundle_u_addMsg(nested_bndl, time_msg);

	t_osc_msg_u *tempo_msg = osc_message_u_alloc();
	osc_message_u_setAddress(tempo_msg, "/tempo");
	osc_message_u_appendFloat(tempo_msg, tempo);
	osc_bundle_u_addMsg(nested_bndl, tempo_msg);

	t_osc_msg_u *phase_msg = osc_message_u_alloc();
	osc_message_u_setAddress(phase_msg, "/phase");
	osc_message_u_appendFloat(phase_msg, phase);
	osc_bundle_u_addMsg(nested_bndl, phase_msg);
    
    //char *nested_bndl_s = NULL;
    //long nested_bndl_len_s = 0;
    //osc_bundle_u_serialize(nested_bndl, &nested_bndl_len_s, &nested_bndl_s);
    //osc_message_u_appendBndl(msg, nested_bndl_len_s, nested_bndl_s);
    osc_message_u_appendBndl_u(msg, nested_bndl);
    osc_bundle_u_addMsg(bndl, msg);
    
    //char *bndl_s = NULL;
    //long bndl_len_s = 0;
    
    //osc_bundle_u_serialize(bndl, &bndl_len_s, &bndl_s);
    t_osc_bndl_s *sb = osc_bundle_u_serialize(bndl);
    char *bndl_s = osc_bundle_s_getPtr(sb);
    long bndl_len_s = osc_bundle_s_getLen(sb);
    
    t_atom foo[2];
    atom_setlong(foo, bndl_len_s);
    atom_setlong(foo + 1, (long)bndl_s);
    outlet_anything(x->out_osc, ps_FullPacket, 2, foo);
    
    osc_bundle_u_free(bndl);
    //osc_mem_free(nested_bndl_s);
    //osc_mem_free(bndl_s);
    osc_bundle_s_deepFree(sb);

	return;
	struct {
		char header[16];
		char function_size[4];
		char function_address[24]; // /subdiv/xxx/function\0\0\0\0
		char function_tt[4];
		char function_data[4];

		char point_size[4];
		char point_address[20]; // /subdiv/xxx/point\0\0\0
		char point_tt[4];
		char point_data[4];

		char beat_size[4];
		char beat_address[20]; // /subdiv/xxx/beat\0\0\0\0
		char beat_tt[4];
		char beat_data[4];

		char subdiv_size[4];
		char subdiv_address[20]; // /subdiv/xxx/subdiv\0\0
		char subdiv_tt[4];
		char subdiv_data[4];

		char time_size[4];
		char time_address[20]; // /subdiv/xxx/time\0\0\0\0
		char time_tt[4];
		char time_data[4];

		char tempo_size[4];
		char tempo_address[20]; // /subdiv/xxx/tempo\0\0\0
		char tempo_tt[4];
		char tempo_data[4];

		char phase_size[4];
		char phase_address[20]; // /subdiv/xxx/phase\0\0\0
		char phase_tt[4];
		char phase_data[4];
	} sd = {
		"#bundle\0\0\0\0\0\0\0\0",
		{0, 0, 0, 32},
		"/subdiv/xxx/function\0\0\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/point\0\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/beat\0\0\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/subdiv\0",
		",i\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/time\0\0\0",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/tempo\0\0",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 28},
		"/subdiv/xxx/phase\0\0",
		",f\0",
		"\0\0\0"
	};
	sprintf(sd.function_address, "/subdiv/%03ld/function", subdivtype);
	sprintf(sd.point_address, "/subdiv/%03ld/point", subdivtype);
	sprintf(sd.beat_address, "/subdiv/%03ld/beat", subdivtype);
	sprintf(sd.subdiv_address, "/subdiv/%03ld/subdiv", subdivtype);
	sprintf(sd.time_address, "/subdiv/%03ld/time", subdivtype);
	sprintf(sd.tempo_address, "/subdiv/%03ld/tempo", subdivtype);
	sprintf(sd.phase_address, "/subdiv/%03ld/phase", subdivtype);

	*((uint32_t *)sd.function_data) = hton32(function);
	*((uint32_t *)sd.point_data) = hton32(point);
	*((uint32_t *)sd.beat_data) = hton32(beat);
	*((uint32_t *)sd.subdiv_data) = hton32(subdiv);
	*((uint32_t *)sd.time_data) = hton32(*((uint32_t *)(&time)));
	*((uint32_t *)sd.tempo_data) = hton32(*((uint32_t *)(&tempo)));
	*((uint32_t *)sd.phase_data) = hton32(*((uint32_t *)(&phase)));
	t_atom out[2];
	atom_setlong(out, sizeof(sd));
	atom_setlong(out + 1, (long)&sd);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
}

void te_dumpBeatsForFunction(t_te *x, 
			     long function, 
			     long numsubdivs, 
			     double *subdivs_atoms, 
			     double time_min, 
			     double time_max)
{
	//int function;
	double t;
	//t_atom out[9];
	te_critical_enter(x->lock, __LINE__);

	if(x->functions[function] == NULL){
		return;
	}
	t_plan plan;
	te_makePlan(x, x->time_min, function, &plan);
	//double prev_phase = .9999;
	double prev_phase[numsubdivs + 1];
	double subdivs[numsubdivs + 1];
	int i;
	prev_phase[0] = .9999;
	subdivs[0] = 1;
	for(i = 1; i < numsubdivs + 1; i++){
		prev_phase[i] = .9999;
		//subdivs[i] = atom_getfloat(subdivs_atoms + (i - 1));
		if(subdivs_atoms[i - 1] < 1.0 && subdivs_atoms[i - 1] > 0.){
			object_error((t_object *)x, "the use of floating point numbers to specify superdivisions has been deprecated");
			object_error((t_object *)x, "%f should be expressed as %d", subdivs_atoms[i - 1],
				     (int)(-1. / subdivs_atoms[i - 1]));
		}
		if(subdivs_atoms[i - 1] < 0){
			subdivs[i] = 1 / fabs(subdivs_atoms[i - 1]);
		}else{
			subdivs[i] = subdivs_atoms[i - 1];
		}
	}
	//qsort(subdivs, numsubdivs + 1, sizeof(double), te_compare);
	double p, wp;
	time_min = ceil(time_min * 44100.) / 44100.;
	time_max = floor(time_max * 44100.) / 44100.;
	for(t = time_min; t <= time_max; t += (1. / (44100. / 64.))){
		if(!te_isPlanValid(x, t, &plan, function)){
			te_makePlan(x, t, function, &plan);
		}
		for(i = 0; i < numsubdivs + 1; i++){
			p = te_computeCorrectedPhase(t, &plan) * subdivs[i];
			wp = p - floor(p);
			if(subdivs[i] > 0){
				// t_atom *ptr = out;
				// if(i > 0){
				// 	atom_setsym(ptr++, gensym("subdiv"));
				// }
				if(wp < prev_phase[i]){
					// we're going through fairly coarsely (1/689") when we find a beat, step backwards by 1/44100" until we get a better estimate of where the beat is
					double tt = t - (1. / 44100.);
					double pm1 = te_computeCorrectedPhase(tt, &plan) * subdivs[i];
					double wpm1 = pm1 - floor(pm1);
					if(plan.state == AFTER_LAST_POINT){
						wp = plan.startPhase;
						tt = plan.startTime;
					}else{
						while(wp > wpm1){
							p = pm1;
							wp = wpm1;
							tt -= (1. / 44100.);
							pm1 = te_computeCorrectedPhase(tt, &plan) * subdivs[i];
							wpm1 = pm1 - floor(pm1);
						}
						tt += (1. / 44100.);
					}
					if(fabs((plan.startTime * 44100.) - (tt * 44100.)) <= 1){
						tt = plan.startTime;
						wp = plan.startPhase;
					}else if(fabs((plan.endTime * 44100.) - (tt * 44100.)) <= 1){
						tt = plan.endTime;
						wp = plan.endPhase;
					}
					if(plan.pointnum_left > 0){
						long point, beat;
						float time, tempo, phase;
						point = plan.pointnum_left;
						beat = (long)(p / subdivs[i]);
						time = tt;
						tempo = te_computeCorrectedTempo(tt, &plan) * subdivs[i];
						phase = te_computeCorrectedUnwrappedMonotonicPhase(x, tt, function, &plan);
						if(i > 0){
							// subdiv
							long subdivtype, subdiv;
							if(subdivs[i] < 1){
								subdivtype = (long)(-1.0 / subdivs[i]);
							}else{
								subdivtype = (long)subdivs[i];
							}
							if((long)subdivs[i] == 0){
								subdiv = 0;
							}else{
								subdiv = (long)p % (long)subdivs[i];
							}
							te_outputSubdivOSC(x, function, point, beat, subdivtype, subdiv, time, tempo, phase);
						}else{
							// beat
							te_outputBeatOSC(x, function, point, beat, time, tempo, phase);
						}
						/*
						atom_setlong(ptr++, function);
						atom_setlong(ptr++, plan.pointnum_left);
						atom_setlong(ptr++, (long)(p / subdivs[i]));
						if(i > 0){
							if(subdivs[i] < 1){
								atom_setlong(ptr++, (long)(-1.0 / subdivs[i]));
							}else{
								atom_setlong(ptr++, (long)subdivs[i]);
							}
							if((long)subdivs[i] == 0){
								atom_setlong(ptr++, 0);
							}else{
								atom_setlong(ptr++, (long)p % (long)subdivs[i]);
							}
						}
						atom_setfloat(ptr++, tt);
						atom_setfloat(ptr++, te_computeCorrectedTempo(tt, &plan) * subdivs[i]);
						// phase:
						atom_setfloat(ptr++, te_computeCorrectedUnwrappedMonotonicPhase(x, tt, function, &plan));
						te_critical_exit(x->lock, __LINE__);
						if(tt >= time_min && tt <= time_max){
							// even though we are looping from t = time_min to t <= time_max we identify the first beat at the beginning of that
							// time span and work backwards to find it's actual location at the nearest 44.1kHz sample which may be before time_min
							outlet_anything(x->out_osc, gensym("dumpbeats"), ptr - out, out);
						}
						te_critical_enter(x->lock, __LINE__);
						*/
					}
				}
			}
			prev_phase[i] = wp;
		}
	}
	te_critical_exit(x->lock, __LINE__);
}

void te_dumpSelectedOSC(t_te *x){
	t_osc_point osc_point = _osc_point;
	te_critical_enter(x->lock, __LINE__);
	t_point *p = x->selected;
	if(!p){
		return;
	}
	*((uint32_t *)osc_point.function_data) = hton32(x->currentFunction);
	float f;
	
	f = p->coords.x;
	*((uint32_t *)osc_point.time_data) = hton32(*((uint32_t *)(&f)));

	f = p->coords.y;
	*((uint32_t *)osc_point.atempo_data) = hton32(*((uint32_t *)(&f)));

	f = p->d_freq;
	*((uint32_t *)osc_point.dtempo_data) = hton32(*((uint32_t *)(&f)));

	f = p->a_phase;
	*((uint32_t *)osc_point.aphase_data) = hton32(*((uint32_t *)(&f)));

	f = p->d_phase;
	*((uint32_t *)osc_point.dphase_data) = hton32(*((uint32_t *)(&f)));

	f = p->alpha;
	*((uint32_t *)osc_point.alpha_data) = hton32(*((uint32_t *)(&f)));

	f = p->beta;
	*((uint32_t *)osc_point.beta_data) = hton32(*((uint32_t *)(&f)));

	f = p->error_alpha;
	*((uint32_t *)osc_point.ealpha_data) = hton32(*((uint32_t *)(&f)));

	f = p->error_beta;
	*((uint32_t *)osc_point.ebeta_data) = hton32(*((uint32_t *)(&f)));

	te_critical_exit(x->lock, __LINE__);

	t_atom out[2];
	atom_setlong(out, sizeof(osc_point));
	atom_setlong(out + 1, (long)(&osc_point));
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
}

void te_dumpCellblock(t_te *x){
	t_point *p = x->functions[x->currentFunction];
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_plan plan;
	t_atom out[4];
	atom_setsym(&(out[0]), _sym_clear);
	atom_setsym(&(out[1]), gensym("all"));
	outlet_anything(x->out_cellblock, ps_cellblock, 2, out);
	atom_setsym(&(out[0]), _sym_set);

	// output col header
	int c = 0;
	atom_setlong(&(out[2]), 0);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_pointNum);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_time);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_duration);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_aFreq);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_dFreq);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_aPhase);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_dPhase);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_alpha);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_beta);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_errorAlpha);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_errorBeta);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_error);
	outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

	int i = 1; // first row is the header
	while(p){
		te_makePlan(x, p->coords.x, x->currentFunction, &plan);
		c = 0;
		atom_setlong(&(out[2]), i);

		// point number
		atom_setlong(&(out[1]), c++);
		atom_setlong(&(out[3]), i);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// time
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->coords.x);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// duration
		atom_setlong(&(out[1]), c++);
		if(p->next){
			atom_setfloat(&(out[3]), p->next->coords.x - p->coords.x);
		}else{
			atom_setfloat(&(out[3]), 0.);
		}
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// freq
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->coords.y);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// departure freq
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->d_freq);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// arrival phase
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->a_phase);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// departure phase
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->d_phase);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// alpha
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->alpha);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// beta
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->beta);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// error_alpha
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->error_alpha);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// error_beta
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->error_beta);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		// error
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), plan.phaseError);
		outlet_anything(x->out_cellblock, ps_cellblock, 4, out);

		p = p->next;
		i++;
	}
}

void te_time_minmax(t_te *x, double min, double max){
	if(min == max){
		object_error((t_object *)x, "min can't equal max");
		return;
	}
	x->time_min = min;
	x->time_max = max;
	if(min > max){
		x->time_min = max;
		x->time_max = min;
	}
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_freq_minmax(t_te *x, double min, double max){
	if(min == max){
		object_error((t_object *)x, "min can't equal max");
		return;
	}
	x->freq_min = min;
	x->freq_max = max;
	if(min > max){
		x->freq_min = max;
		x->freq_max = min;
	}
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_outputMinMax(t_te *x, const char *minstring, const char *maxstring, float minval, float maxval){
	struct {
		char header[16];
		char min_size[4];
		char min_address[12];
		char min_tt[4];
		char min_data[4];

		char max_size[4];
		char max_address[12];
		char max_tt[4];
		char max_data[4];
	} s = {
		"#bundle\0\0\0\0\0\0\0\0", 
		{0, 0, 0, 20},
		"",
		",f\0",
		"\0\0\0",

		{0, 0, 0, 20},
		"", 
		",f\0",
		"\0\0\0"
	};
	strncpy(s.min_address, minstring, 9);
	strncpy(s.max_address, maxstring, 9);
	float f = minval;
	*((uint32_t *)(s.min_data)) = hton32(*((uint32_t *)&f));
	f = maxval;
	*((uint32_t *)(s.max_data)) = hton32(*((uint32_t *)&f));
	t_atom out[2];
	atom_setlong(out, sizeof(s));
	atom_setlong(out + 1, (long)&s);
	outlet_anything(x->out_osc, ps_FullPacket, 2, out);
}

void te_getTimeMinMax(t_te *x){
	te_outputMinMax(x, "/time/min", "/time/max", x->time_min, x->time_max);
}

void te_getFreqMinMax(t_te *x){
	te_outputMinMax(x, "/freq/min", "/freq/max", x->freq_min, x->freq_max);
}

void te_clear(t_te *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		te_doClearFunction(x, i);
	}
	x->currentFunction = 0;
	x->numFunctions = 1;
	te_dumpCellblock(x);
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_clearFunction(t_te *x, int f){
	te_doClearFunction(x, f);
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[f]);
	jbox_redraw((t_jbox *)x);
}

void te_clearRange(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	int function = x->currentFunction;
	t_atom *ptr = argv;
	if(argc > 2){
		if(atom_gettype(ptr) == A_LONG){
			function = atom_getlong(ptr++);
		}else if(atom_getsym(ptr) == gensym("*")){
			te_clearRangeForAllFunctions(x, atom_getfloat(ptr + 1), atom_getfloat(ptr + 2));
			return;
		}
	}
	te_clearRangeForFunction(x, function, atom_getfloat(ptr), atom_getfloat(ptr + 1));
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[function]);
	jbox_redraw((t_jbox *)x);
}

void te_clearRangeForFunction(t_te *x, int function, float timemin, float timemax){
	te_critical_enter(x->lock, __LINE__);
	t_point *p = x->functions[function];
	while(p){
		if(p->coords.x >= timemin && p->coords.x <= timemax){
			if(p == x->selected){
				x->selected = NULL;
			}
			te_removePoint(x, p, function);
		}
		p = p->next;
	}
	te_critical_exit(x->lock, __LINE__);
}

void te_clearRangeForAllFunctions(t_te *x, float timemin, float timemax){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		te_clearRangeForFunction(x, i, timemin, timemax);
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[i]);
	}
	jbox_redraw((t_jbox *)x);
}

void te_clearCurrent(t_te *x){
	te_doClearFunction(x, x->currentFunction);
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[x->currentFunction]);
	jbox_redraw((t_jbox *)x);
}

void te_doClearFunction(t_te *x, int f){
	te_critical_enter(x->lock, __LINE__);
	memset(x->plans + f, '\0', sizeof(t_plan));
	t_point *p = x->functions[f];
	t_point *next = p->next;
	while(p){
		if(x->selected == p){
			x->selected = NULL;
		}
		next = p->next;
		free(p);
		p = next;
	}
	x->functions[f] = NULL;
	te_critical_exit(x->lock, __LINE__);
}

void te_resetFunctionColors(t_te *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		x->functionColors[i] = (t_jrgba){0., 0., 0., 1.};
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[i]);
	}
	t_atom a;
	atom_setlong(&a, x->currentFunction);
	//te_getFunctionColor(x, NULL, 1, &a);
	jbox_redraw((t_jbox *)x);
}

void te_hideFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	long function;
	long b;
	function = atom_getlong(argv);
	if(argc == 3){ // from matrixctrl
		b = atom_getlong(argv + 2);
	}else if(argc == 2){
		b = atom_getlong(argv + 1);
	}else{
		error("tempo_editor: two arguments required: function number, on/off");
		return;
	}
	x->hideFunctions[function] = b;
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[function]);
	jbox_redraw((t_jbox *)x);
}

void te_muteFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	long function;
	long b;
	function = atom_getlong(argv);
	if(argc == 3){ // from matrixctrl
		b = atom_getlong(argv + 2);
	}else if(argc == 2){
		b = atom_getlong(argv + 1);
	}else{
		error("tempo_editor: two arguments required: function number, on/off");
		return;
	}
	x->muteFunctions[function] = b;
	jbox_invalidate_layer((t_object *)x, x->pv, l_legend);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[function]);
	jbox_redraw((t_jbox *)x);
}

void te_lockFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	long function;
	long b;
	function = atom_getlong(argv);
	if(argc == 3){ // from matrixctrl
		b = atom_getlong(argv + 2);
	}else if(argc == 2){
		b = atom_getlong(argv + 1);
	}else{
		error("tempo_editor: two arguments required: function number, on/off");
		return;
	}
	x->lockFunctions[function] = b;
	jbox_invalidate_layer((t_object *)x, x->pv, l_legend);
	jbox_redraw((t_jbox *)x);
}


// there is some nasty function pointer madness below...
double te_add(double a, double b){return a + b;}
double te_subtract(double a, double b){return a - b;}
double te_multiply(double a, double b){return a * b;}
double te_divide(double a, double b){return a / b;}

double (*te_getFunctionForSymbol(t_symbol *sym))(double, double){
	if(sym == gensym("+")){
		return te_add;
	}else if(sym == gensym("-")){
		return te_subtract;
	}else if(sym == gensym("*")){
		return te_multiply;
	}else if(sym == gensym("/")){
		return te_divide;
	}else{
		return NULL;
	}
}

void te_mapx(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc < 2){
		object_error((t_object *)x, "mapx requires two arguments: operator (+, -, *, /) and an argument");
		return;
	}
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double sel_box_x = x->sel_box.x;//te_scale(x->sel_box.x, 0., r.width, x->time_min, x->time_max);
	double sel_box_width = x->sel_box.width;
	te_domap(x, msg, argc, argv, 0, &sel_box_x, &sel_box_width);
	//x->sel_box.x = te_scale(sel_box_x, x->time_min, x->time_max, 0., r.width);
	//x->sel_box.width = sel_box_width;
	memset(x->plans, '\0', sizeof(t_plan) * x->numFunctions);
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_mapy(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc < 2){
		object_error((t_object *)x, "mapx requires two arguments: operator (+, -, *, /) and an argument");
		return;
	}
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double sel_box_y = x->sel_box.y;//te_scale(x->sel_box.y, r.height, 0., x->freq_min, x->freq_max);
	double sel_box_height = x->sel_box.height;
	te_domap(x, msg, argc, argv, 1, &sel_box_y, &sel_box_height);
	//x->sel_box.y = te_scale(sel_box_y, x->freq_min, x->freq_max, r.height, 0.);
	//x->sel_box.height = sel_box_height;
	memset(x->plans, '\0', sizeof(t_plan) * x->numFunctions);
	te_invalidateAll(x);
	jbox_redraw((t_jbox *)x);
}

void te_domap(t_te *x, t_symbol *msg, int argc, t_atom *argv, int xory, double *rect_coord_to_move, double *rect_dim){
	if(atom_gettype(argv) != A_SYM){
		object_error((t_object *)x, "first argument must be +, -, *, or /");
		return;
	}
	t_symbol *op = atom_getsym(argv);
	double (*f)(double, double) = te_getFunctionForSymbol(op);
	if(!f){
		object_error((t_object *)x, "unrecognized operator %s", op->s_name);
		return;
	}

	te_critical_enter(x->lock, __LINE__);
	t_point *p = x->selected;
	double *coords;
	double arg = atom_getfloat(argv + 1);
	while(p){
		coords = (double*)(&(p->coords));

		coords[xory] = f(coords[xory], arg);
		if(xory){
			p->d_freq = f(p->d_freq, arg);
		}
		te_reorderPoint(p);
		te_initReorderedPoint(x, p);
		p = p->next_selected;
	}
	*rect_coord_to_move = f(*rect_coord_to_move, arg);
	if(op == gensym("*") || op == gensym("/")){
		*rect_dim = f(*rect_dim, arg);
	}
	te_critical_exit(x->lock, __LINE__);
	te_clearSelectionBox(x);
}

void te_scaleToFit(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	int functions[MAX_NUM_FUNCTIONS] = {x->currentFunction};
	int numfunctions = 1;
	int xx = 0, yy = 0;
	int i;
	int havestar = 0;
	for(i = 0; i < argc; i++){
		switch(atom_gettype(argv + i)){
		case A_LONG:
			if(havestar){
				//weird...
			}else{
				functions[numfunctions++] = atom_getlong(argv + i);
			}
			break;
		case A_SYM:
			if(atom_getsym(argv + i) == gensym("x")){
				xx++;
			}else if(atom_getsym(argv + i) == gensym("y")){
				yy++;
			}else if(atom_getsym(argv + i) == gensym("*")){
				int j;
				for(j = 0; j < x->numFunctions; j++){
					functions[j] = j;
				}
				numfunctions = x->numFunctions;
				havestar = 1;
			}else{
				object_error((t_object *)x, "unrecognized argument: %s\n", atom_getsym(argv + i)->s_name);
				return;
			}
			break;
		default:
			object_error((t_object *)x, "unrecognized argument number %d", i);
		}
	}
	if(!xx && !yy){
		te_scaleToFitX(x, numfunctions, functions);
		te_scaleToFitY(x, numfunctions, functions);
	}else{
		if(xx){
			te_scaleToFitX(x, numfunctions, functions);
		}
		if(yy){
			te_scaleToFitY(x, numfunctions, functions);
		}
	}
}

void te_scaleToFitX(t_te *x, int numfunctions, int *functions){
	int i;
	for(i = 0; i < numfunctions; i++){
		if(!(x->functions[functions[i]])){
			continue;
		}
		double minx, maxx;
		t_point *p = x->functions[functions[i]];
		minx = p->coords.x;
		while(p->next){
			p = p->next;
		}
		maxx = p->coords.x;
		p = x->functions[functions[i]];
		while(p){
			p->coords.x = te_scale(p->coords.x, minx, maxx, x->time_min, x->time_max);
			p = p->next;
		}
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[functions[i]]);
	}
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
	jbox_redraw((t_jbox *)x);
}

void te_scaleToFitY(t_te *x, int numfunctions, int *functions){
	int i;
	for(i = 0; i < numfunctions; i++){
		if(!(x->functions[functions[i]])){
			continue;
		}
		double miny = FLT_MAX, maxy = 0;
		t_point *p = x->functions[functions[i]];
		while(p){
			double yy = (p->d_freq < p->coords.y ? p->d_freq : p->coords.y);
			if(yy < miny){
				miny = yy;
			}
			yy = (p->d_freq > p->coords.y ? p->d_freq : p->coords.y);
			if(yy > maxy){
				maxy = yy;
			}
			p = p->next;
		}
		p = x->functions[functions[i]];
		while(p){
			p->coords.y = te_scale(p->coords.y, miny, maxy, x->freq_min, x->freq_max);
			p->d_freq = te_scale(p->d_freq, miny, maxy, x->freq_min, x->freq_max);
			p = p->next;
		}
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[functions[i]]);
	}
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
	jbox_redraw((t_jbox *)x);
}

void te_addToFunction(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	int offset = 0;
	int func = x->currentFunction;
	if(argc == 3){
		offset = 1;
		func = atom_getlong(argv);
	}
	float xx = atom_getfloat(argv + offset++);
	float yy = atom_getfloat(argv + offset++);
	t_point *p = x->functions[func];
	t_rect r;
	jbox_get_rect_for_view((t_object *)x, x->pv, &r); //jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	while(p){
		p->coords.x += xx;
		p->coords.y += yy;
		p->d_freq += yy;
		p = p->next;
	}
	te_dumpSelectedOSC(x);
	
	te_dumpCellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[func]);
	jbox_redraw((t_jbox *)x);
}

void te_addToAllFunctions(t_te *x, double xx, double yy){
	int i;
	t_atom a[3];
	atom_setfloat(&(a[1]), xx);
	atom_setfloat(&(a[2]), yy);
	for(i = 0; i < x->numFunctions; i++){
		atom_setfloat(&(a[0]), i);
		te_addToFunction(x, NULL, 3, a);
	}
}
/*
void te_getFunctionColor(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	t_atom c[4];
	int i;
	t_symbol *functioncolor = gensym("functioncolor");
	if(argc){
		for(i = 0; i < argc; i++){
			atom_setlong(c, atom_getlong(argv + i));
			atom_setfloat(c + 1, x->functionColors[atom_getlong(argv + i)].red);
			atom_setfloat(c + 2, x->functionColors[atom_getlong(argv + i)].green);
			atom_setfloat(c + 3, x->functionColors[atom_getlong(argv + i)].blue);
			outlet_anything(x->out_osc, functioncolor, 4, c);
		}
	}else{
		for(i = 0; i < x->numFunctions; i++){
			atom_setlong(c, i);
			atom_setfloat(c + 1, x->functionColors[i].red);
			atom_setfloat(c + 2, x->functionColors[i].green);
			atom_setfloat(c + 3, x->functionColors[i].blue);
			outlet_anything(x->out_osc, functioncolor, 4, c);
		}
	}
}
*/
void te_setFunctionColor(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc < 3 || argc > 5){
		object_error((t_object *)x, "setfunctioncolor requires between 3 and 5 arguments:\n[function number (int)] <red> <green> <blue> [alpha]");
		return;
	}
	int function = x->currentFunction;
	t_atom *ptr = argv;
	if(atom_gettype(ptr) == A_LONG){
		function = atom_getlong(ptr++);
	}
	t_jrgba color;
	color.red = atom_getfloat(ptr++);
	color.green = atom_getfloat(ptr++);
	color.blue = atom_getfloat(ptr++);
	color.alpha = 1.;
	if(ptr - argv < argc){
		color.alpha = atom_getfloat(ptr);
	}
	x->functionColors[function] = color;
	jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[function]);
	jbox_redraw((t_jbox *)x);
}

void te_invertSelected(t_te *x){
	if(!(x->selected)){
		return;
	}
	double *y = (double *)sysmem_newptr(128 * sizeof(double));
	double *dy = (double *)sysmem_newptr(128 * sizeof(double));
	int len = 128;
	te_critical_enter(x->lock, __LINE__);
	t_point *p = x->functions[x->currentFunction];
	t_point *prev = NULL;
	int i = 0;
	while(p){
		if(p->selected){
			y[i] = p->coords.y;
			dy[i] = p->d_freq;
			i++;
			if(i >= len){
				y = (double *)sysmem_resizeptr(y, (len * 2) * sizeof(double));
				if(!y){
					object_error((t_object *)x, "out of memory");
					goto out;
				}
				dy = (double *)sysmem_resizeptr(dy, (len * 2) * sizeof(double));
				if(!dy){
					object_error((t_object *)x, "out of memory");
					goto out;
				}
				len *= 2;
			}
		}
		p = p->next;
	}
	p = x->functions[x->currentFunction];
	i = 0;
	while(p){
		if(p->selected){
			if(i > 0){
				p->coords.y = prev->coords.y - (y[i] - y[i - 1]);
				p->d_freq = prev->d_freq - (dy[i] - dy[i - 1]);
			}
			prev = p;
			i++;
		}
		p = p->next;
	}
 out:
	sysmem_freeptr(y);
	sysmem_freeptr(dy);

	te_critical_exit(x->lock, __LINE__);
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
}

void te_reverseSelected(t_te *x){
	if(!(x->selected)){
		return;
	}
	double *y = (double *)sysmem_newptr(128 * sizeof(double));
	double *dy = (double *)sysmem_newptr(128 * sizeof(double));
	double *xdelta = (double *)sysmem_newptr(128 * sizeof(double));
	int len = 128;
	te_critical_enter(x->lock, __LINE__);
	t_point *p = x->functions[x->currentFunction], *prev = NULL;
	int i = 0;
	double prevx = 0;
	while(p){
		// the next_selected and prev_selected linked list is not ordered by 
		// x coords like the main linked list is.  so we have to 
		// go through every point and check to see if it's selected
		if(p->selected){
			dy[i] = p->d_freq;
			y[i] = p->coords.y;
			if(prev){
				xdelta[i] = p->coords.x - prev->coords.x;
			}else{
				xdelta[i] = 0;
			}
			i++;
			if(i >= len){
				y = (double *)sysmem_resizeptr(y, len * 2);
				if(!y){
					object_error((t_object *)x, "out of memory!");
					goto out;
				}
				dy = (double *)sysmem_resizeptr(y, (len * 2) * sizeof(double));
				if(!dy){
					object_error((t_object *)x, "out of memory!");
					goto out;
				}
				xdelta = (double *)sysmem_resizeptr(y, (len * 2) * sizeof(double));
				if(!xdelta){
					object_error((t_object *)x, "out of memory!");
					goto out;
				}
				len *= 2;
			}
			prev = p;
		}
		p = p->next;
	}
	p = prev;
	i = 0;
	while(p){
		if(p->selected){
			p->coords.y = y[i];
			p->d_freq = dy[i];
			if(i > 0){
				p->coords.x = prevx - xdelta[i];
			}
			prevx = p->coords.x;
			i++;
		}
		p = p->prev;
	}
 out:
	sysmem_freeptr(y);
	sysmem_freeptr(dy);
	sysmem_freeptr(xdelta);

	te_critical_exit(x->lock, __LINE__);
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
}

void te_makeColorForInt(int i, t_jrgb *c){
	int j = i;//i + 1;
	if(j == 7){
		c->red = .5;
		c->green = .25;
		c->blue = 0.;
	}else{
		c->red = j & 1;
		c->green = (j & 2) >> 1;
		c->blue = (j & 4) >> 2;
	}
}

void te_invalidateAllFunctions(t_te *x){
	int i;
	for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
		jbox_invalidate_layer((t_object *)x, x->pv, l_function_layers[i]);
	}
}

void te_invalidateAll(t_te *x){
	te_invalidateAllFunctions(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_background);
	jbox_invalidate_layer((t_object *)x, x->pv, l_xgrid);
	jbox_invalidate_layer((t_object *)x, x->pv, l_ygrid);
	jbox_invalidate_layer((t_object *)x, x->pv, l_xycoords);
	jbox_invalidate_layer((t_object *)x, x->pv, l_legend);
	jbox_invalidate_layer((t_object *)x, x->pv, l_xaxis);
	jbox_invalidate_layer((t_object *)x, x->pv, l_yaxis);
	jbox_invalidate_layer((t_object *)x, x->pv, l_position);
}

void te_assist(t_te *x, void *b, long m, long a, char *s){ 
 	if (m == ASSIST_OUTLET){ 
		switch(a){
		case 0:
			sprintf(s, "Wrapped phase (function %d)", x->currentFunction);
			break;
		case 1:
			sprintf(s, "Unwrapped phase (function %d)", x->currentFunction);
			break;
		case 2:
			sprintf(s, "Instantaneous tempo in beats/second (function %d)", x->currentFunction);
			break;
		case 3:
			sprintf(s, "Control point number (function %d)", x->currentFunction);
			break;
		case 4:
			sprintf(s, "Beat Number (function %d)", x->currentFunction);
			break;
		case 5:
			sprintf(s, "OSC outlet");
			break;
		case 6:
			sprintf(s, "Info outlet");
			break;
		}
 	}else{ 
 		switch (a) {	 
 		case 0: 
			sprintf(s, "Time (signal/float)");
 			break; 
 		} 
 	} 
} 

void te_free(t_te *x){ 
	dsp_freejbox((t_pxjbox *)x);
	jbox_free((t_jbox *)x);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		t_point *next;
		while(p){
			next = p->next;
			free(p);
			p = next;
		}
		if(x->ptrs[i * 3]){
			free(x->ptrs[i * 3]);
		}
		if(x->ptrs[i * 3 + 1]){
			free(x->ptrs[i * 3 + 1]);
		}
		if(x->ptrs[i * 3 + 2]){
			free(x->ptrs[i * 3 + 2]);
		}
	}
	if(x->ptrs){
		free(x->ptrs);
	}
	if(x->last_y){
		free(x->last_y);
	}
	if(x->plans){
		free(x->plans);
	}
	if(x->update_position_clock){
		clock_unset(x->update_position_clock);
		object_free(x->update_position_clock);
	}
} 

void te_postplan(t_plan *p, int (*print)(const char *, ...)){
	print("state = %d\n", p->state);
	print("startTime = %f, endTime = %f\n", p->startTime, p->endTime);
	print("startPhase = %f, endPhase = %f\n", p->startPhase, p->endPhase);
	print("phaseError = %f\n", p->phaseError);
	print("correctionStart = %f, correctionEnd = %f\n", p->correctionStart, p->correctionEnd);
	print("alpha = %f, beta = %f\n", p->alpha, p->beta);
	print("error_alpha = %f, error_beta = %f\n", p->error_alpha, p->error_beta);
	print("beta_ab = %f, error_beta_ab = %f\n", p->beta_ab, p->error_beta_ab);
}

void te_postpoint(t_point *p, int (print)(const char *, ...)){
	print("coords = (%f %f)\n", p->coords.x, p->coords.y);
	print("d_freq = %f\n", p->d_freq);
	print("a_phase = %f, d_phase = %f\n", p->a_phase, p->d_phase);
	print("aux_points = (%f, %f)\n", p->aux_points[0], p->aux_points[1]);
	print("alpha = %f, beta = %f\n", p->alpha, p->beta);
	print("error_alpha = %f, error_beta = %f\n", p->error_alpha, p->error_beta);
	print("whichPoint = %f\n", p->whichPoint);
	print("closest_point_with_same_ycoord = %p\n", p->closest_point_with_same_ycoord);
	print("draw_snapline = %d\n", p->draw_snapline);
	print("%p<--%p-->%p\n", p->prev, p, p->next);
	print("selected: %p<--%p-->%p\n", p->prev_selected, p, p->next_selected);
}

t_symbol *te_mangleName(t_symbol *name, int i, int fnum){
	if(!name){
		return NULL;
	}
	char buf[256];
	sprintf(buf, "bkout_%s_%d_%d", name->s_name, i, fnum);
	return gensym(buf);
}

void te_critical_enter(t_critical lock, int line){
	//printf("critical enter: %d\n", line);
	critical_enter(lock);
}

void te_critical_exit(t_critical lock, int line){
	//printf("critical exit: %d\n", line);
	critical_exit(lock);
}

int main(void){ 
	char buf[32];
 	t_class *c = class_new("timewarp~", (method)te_new, (method)te_free, sizeof(t_te), 0L, A_GIMME, 0); 
	class_dspinitjbox(c);

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

	class_addmethod(c, (method)te_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)te_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)te_dspstate, "dspstate", A_CANT, 0);
 	class_addmethod(c, (method)te_paint, "paint", A_CANT, 0); 
 	//class_addmethod(c, (method)version, "version", 0);
 	class_addmethod(c, (method)te_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)te_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)te_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)te_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)te_mousemove, "mousemove", A_CANT, 0); 
 	class_addmethod(c, (method)te_mouseup, "mouseup", A_CANT, 0); 
 	class_addmethod(c, (method)te_addFunction, "addfunction", 0); 
 	class_addmethod(c, (method)te_setFunction, "setfunction", A_LONG, 0); 
	class_addmethod(c, (method)te_selectRegion, "selectregion", A_GIMME, 0);
	class_addmethod(c, (method)te_editSel, "editselection", A_FLOAT, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)te_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)te_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)te_clear, "clear", 0);
	class_addmethod(c, (method)te_resetFunctionColors, "resetfunctioncolors", 0);
	class_addmethod(c, (method)te_clearCurrent, "clearcurrent", 0);
	class_addmethod(c, (method)te_clearFunction, "clearfunction", A_LONG, 0);
	class_addmethod(c, (method)te_clearRange, "clearrange", A_GIMME, 0);
	class_addmethod(c, (method)te_dump, "dump", 0);
	class_addmethod(c, (method)te_time_minmax, "timeminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_minmax, "freqminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_getTimeMinMax, "gettimeminmax", 0);
	class_addmethod(c, (method)te_getFreqMinMax, "getfreqminmax", 0);
	class_addmethod(c, (method)te_dumpBeats, "dumpbeats", A_GIMME, 0);
	//class_addmethod(c, (method)te_dumpBeatsForRange, "dumpbeatsforrange", A_GIMME, 0);
	class_addmethod(c, (method)te_addToFunction, "addToFunction", A_GIMME, 0);
	class_addmethod(c, (method)te_addToAllFunctions, "addToAllFunctions", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_hideFunction, "hidefunction", A_GIMME, 0);
	class_addmethod(c, (method)te_muteFunction, "mutefunction", A_GIMME, 0);
	class_addmethod(c, (method)te_lockFunction, "lockfunction", A_GIMME, 0);
	class_addmethod(c, (method)te_showgrid, "showgrid", A_LONG, 0);
	class_addmethod(c, (method)te_cut, "cut", 0);
	class_addmethod(c, (method)te_copy, "copy", 0);
	class_addmethod(c, (method)te_paste, "paste", 0);
	class_addmethod(c, (method)te_pasteAtTime, "pasteattime", A_FLOAT, 0);
	class_addmethod(c, (method)te_pasteAtCoords, "pasteatcoords", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_invertSelected, "invertselected", 0);
	class_addmethod(c, (method)te_reverseSelected, "reverseselected", 0);
	class_addmethod(c, (method)te_setFunctionColor, "setfunctioncolor", A_GIMME, 0);
	//class_addmethod(c, (method)te_getFunctionColor, "getfunctioncolor", A_GIMME, 0);
	class_addmethod(c, (method)te_applySelectionToAllFunctions, "apply_selection_box_to_all_functions", 0);
	class_addmethod(c, (method)te_snapSelectionToGrid, "snapselectiontogrid", 0);
	class_addmethod(c, (method)te_addPointsAtSelectionBoundaries, "add_points_at_selection_boundaries", 0);
	class_addmethod(c, (method)te_addPointsAtSelectionBoundariesForAllFunctions, "add_points_at_selection_boundaries_for_all_functions", 0);
	class_addmethod(c, (method)te_mapx, "mapx", A_GIMME, 0);
	class_addmethod(c, (method)te_mapy, "mapy", A_GIMME, 0);
	class_addmethod(c, (method)te_repeatSelection, "repeatselection", 0);
	class_addmethod(c, (method)te_scaleToFit, "scaletofit", A_GIMME, 0);
	class_addmethod(c, (method)te_clearSelected, "clearselection", 0);

	CLASS_ATTR_SYM(c, "name", 0, t_te, name);
	CLASS_ATTR_SAVE(c, "name", 0);
	CLASS_ATTR_LONG(c, "showtempocorrection", 0, t_te, show_tempo_correction);
	//CLASS_ATTR_SAVE(c, "showtempocorrection", 0);
	CLASS_ATTR_PAINT(c, "showtempocorrection", 0);
	CLASS_ATTR_LONG(c, "showbeatcorrection", 0, t_te, show_beat_correction);
	//CLASS_ATTR_SAVE(c, "showbeatcorrection", 0);
	CLASS_ATTR_PAINT(c, "showbeatcorrection", 0);
	CLASS_ATTR_LONG(c, "show_xaxis", 0, t_te, show_x_axis);
	CLASS_ATTR_SAVE(c, "show_xaxis", 0);
	CLASS_ATTR_LONG(c, "show_yaxis", 0, t_te, show_y_axis);
	CLASS_ATTR_SAVE(c, "show_yaxis", 0);

	CLASS_ATTR_DOUBLE(c, "timemin", 0, t_te, time_min);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "timemin", 0, "0");
	CLASS_ATTR_DOUBLE(c, "timemax", 0, t_te, time_max);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "timemax", 0, "10");
	CLASS_ATTR_DOUBLE(c, "freqmin", 0, t_te, freq_min);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "freqmin", 0, "0");
	CLASS_ATTR_DOUBLE(c, "freqmax", 0, t_te, freq_max);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "freqmax", 0, "10");

	CLASS_ATTR_LONG(c, "apply_selection_box_to_all_functions_by_default", 0, t_te, apply_selection_box_to_all_functions_by_default);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "apply_selection_box_to_all_functions_by_default", 0, "0");
 	CLASS_ATTR_STYLE_LABEL(c, "apply_selection_box_to_all_functions_by_default", 0, "onoff", "Select All Functions When Making A Selection"); 

	CLASS_ATTR_LONG(c, "reset_highlight_at_controlpoints", 0, t_te, reset_highlight_at_controlpoints);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "reset_highlight_at_controlpoints", 0, "0");
 	CLASS_ATTR_STYLE_LABEL(c, "reset_highlight_at_controlpoints", 0, "onoff", "Reset Highlighted Beat At Controlpoints"); 

	CLASS_ATTR_DOUBLE_ARRAY(c, "colors", 0, t_te, functionColors, MAX_NUM_FUNCTIONS * 4);
	CLASS_ATTR_ACCESSORS(c, "colors", te_functionColorsGet, te_functionColorsSet);
	CLASS_ATTR_SAVE(c, "colors", 0);

	CLASS_ATTR_ATOM_VARSIZE(c, "functions", 0, t_te, functions, numFunctions, 1024);
	CLASS_ATTR_ACCESSORS(c, "functions", te_functions_get, te_functions_set);
	CLASS_ATTR_SAVE(c, "functions", 0);

	CLASS_ATTR_LONG(c, "snaptogrid", 0, t_te, snap_to_grid);
	CLASS_ATTR_DEFAULT_SAVE(c, "snaptogrid", 0, "0");

	CLASS_ATTR_LONG(c, "drawposition", 0, t_te, drawposition);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "drawposition", 0, "1");
	CLASS_ATTR_STYLE_LABEL(c, "drawposition", 0, "onoff", "Draw Position When DSP is On");

	CLASS_ATTR_FLOAT(c, "position_update_rate_ms", 0, t_te, position_update_rate_ms);
	sprintf(buf, "%f", (double)POSITION_UPDATE_RATE_MS);
	CLASS_ATTR_DEFAULT_SAVE(c, "position_update_rate_ms", 0, buf);
	CLASS_ATTR_LABEL(c, "position_update_rate_ms", 0, "Position Update Rate (ms)");

	CLASS_ATTR_LONG(c, "show_major_xgrid", 0, t_te, show_major_x_grid);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_major_xgrid", 0, "1");
	CLASS_ATTR_LONG(c, "show_minor_xgrid", 0, t_te, show_minor_x_grid);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_minor_xgrid", 0, "1");

	CLASS_ATTR_LONG(c, "show_major_ygrid", 0, t_te, show_major_y_grid);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_major_ygrid", 0, "1");
	CLASS_ATTR_LONG(c, "show_minor_ygrid", 0, t_te, show_minor_y_grid);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_minor_ygrid", 0, "1");

	CLASS_ATTR_LONG(c, "show_beats", 0, t_te, show_beats);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_beats", 0, "1");

	CLASS_ATTR_LONG(c, "show_beat_numbers", 0, t_te, show_beat_numbers);
	CLASS_ATTR_DEFAULT_SAVE(c, "show_beat_numbers", 0, "1");

	CLASS_ATTR_LONG(c, "highlight_every_n", 0, t_te, highlight_every_n);
	CLASS_ATTR_DEFAULT_SAVE(c, "highlight_every_n", 0, "4");

	CLASS_ATTR_LONG(c, "draw_mouse", 0, t_te, draw_mouse);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "draw_mouse", 0, "1");
	CLASS_ATTR_LONG(c, "draw_voice_legend", 0, t_te, draw_voice_legend);
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "draw_voice_legend", 0, "1");

	CLASS_ATTR_FLOAT(c, "major_grid_width_sec", 0, t_te, major_grid_width_sec);
	CLASS_ATTR_DEFAULT_SAVE(c, "major_grid_width_sec", 0, "1.");
	CLASS_ATTR_FLOAT(c, "num_minor_x_grid_divisions", 0, t_te, num_minor_x_grid_divisions);
	CLASS_ATTR_DEFAULT_SAVE(c, "num_minor_x_grid_divisions", 0, "2"); 

	CLASS_ATTR_FLOAT(c, "major_grid_height_bps", 0, t_te, major_grid_height_bps);
	CLASS_ATTR_DEFAULT_SAVE(c, "major_grid_height_bps", 0, "1.");
	CLASS_ATTR_FLOAT(c, "num_minor_y_grid_divisions", 0, t_te, num_minor_y_grid_divisions);
	CLASS_ATTR_DEFAULT_SAVE(c, "num_minor_y_grid_divisions", 0, "2"); 

	CLASS_ATTR_FLOAT(c, "major_grid_line_width", 0, t_te, major_grid_line_width);
	CLASS_ATTR_DEFAULT_SAVE(c, "major_grid_line_width", 0, "1.");
	CLASS_ATTR_FLOAT(c, "minor_grid_line_width", 0, t_te, minor_grid_line_width);
	CLASS_ATTR_DEFAULT_SAVE(c, "minor_grid_line_width", 0, ".5");

 	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_te, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointColor", 0, t_te, pointColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointColor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "phaseColor", 0, t_te, phaseColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "phaseColor", 0, "0. 1. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "phaseColor", 0, "rgba", "Phase Color"); 

 	CLASS_ATTR_RGBA(c, "lineColor", 0, t_te, lineColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lineColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "lineColor", 0, "rgba", "Line Color"); 

 	CLASS_ATTR_RGBA(c, "bgFuncColor", 0, t_te, bgFuncColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgFuncColor", 0, "0. 0. 0. 0.5"); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgFuncColor", 0, "rgba", "Background Function Color"); 

 	CLASS_ATTR_RGBA(c, "selectionColor", 0, t_te, selectionColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectionColor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectionColor", 0, "rgba", "Selection Color"); 

 	CLASS_ATTR_RGBA(c, "correctionColor", 0, t_te, correctionColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "correctionColor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "correctionColor", 0, "rgba", "Correction Color"); 

 	CLASS_ATTR_RGBA(c, "correctionColor_box", 0, t_te, correctionColor_box); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "correctionColor_box", 0, "1. 0. 0. .15"); 
 	CLASS_ATTR_STYLE_LABEL(c, "correctionColor_box", 0, "rgba", "Correction Color Box"); 

 	CLASS_ATTR_RGBA(c, "major_grid_line_color", 0, t_te, major_grid_line_color); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "major_grid_line_color", 0, "0. 0. 1. .5"); 
 	CLASS_ATTR_STYLE_LABEL(c, "major_grid_line_color", 0, "rgba", "Major Grid Line Color"); 

 	CLASS_ATTR_RGBA(c, "minor_grid_line_color", 0, t_te, minor_grid_line_color); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "minor_grid_line_color", 0, "0. 0. 1. .25");
 	CLASS_ATTR_STYLE_LABEL(c, "minor_grid_line_color", 0, "rgba", "Minor Grid Line Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

 	class_register(CLASS_BOX, c); 
 	te_class = c; 

	critical_new(&te_clipboard_lock);

	common_symbols_init();
	ps_cellblock = gensym("cellblock");
	ps_pointNum = gensym("Point #");
	ps_time = gensym("Time");
	ps_duration = gensym("Duration");
	ps_dFreq = gensym("D Tempo");
	ps_aFreq = gensym("A Tempo");
	ps_dPhase = gensym("D Phase");
	ps_aPhase = gensym("A Phase");
	ps_alpha = gensym("Alpha");
	ps_beta = gensym("Beta");
	ps_errorAlpha = gensym("E Alpha");
	ps_errorBeta = gensym("E Beta");
	ps_error = gensym("Error");

	ps_FullPacket = gensym("FullPacket");

	te_clipboard = gensym("te_clipboard");

	l_background = gensym("l_background");
	l_xgrid = gensym("l_xgrid");
	l_ygrid = gensym("l_ygrid");
	l_xycoords = gensym("l_xycoords");
	l_legend = gensym("l_legend");
	l_xaxis = gensym("l_xaxis");
	l_yaxis = gensym("l_yaxis");
	l_position = gensym("l_position");

	int i;
	for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
		sprintf(buf, "l_function_layer_%d", i);
		l_function_layers[i] = gensym(buf);
	}

 	//version(0);
	
 	return 0; 
} 

void *te_new(t_symbol *s, long argc, t_atom *argv){
 	t_te *x = NULL; 
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

 	if(x = (t_te *)object_alloc(te_class)){ 

 		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->box.z_box.b_firstin = (t_object *)x; 

		x->proxy = proxy_new((t_object *)x, 1, &(x->in));

		x->out_cellblock = outlet_new((t_object *)x, NULL);
		x->out_osc = outlet_new((t_object *)x, NULL);
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 

		x->time_min = 0.;
		x->time_max = 10.;
		x->freq_min = 0.;
		x->freq_max = 10.;

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *));
		memset(x->hideFunctions, 0, MAX_NUM_FUNCTIONS * sizeof(int));
		memset(x->muteFunctions, 0, MAX_NUM_FUNCTIONS * sizeof(int));
		memset(x->lockFunctions, 0, MAX_NUM_FUNCTIONS * sizeof(int));
		x->last_y = (float *)calloc(MAX_NUM_FUNCTIONS, sizeof(float));
		x->plans = (t_plan *)calloc(MAX_NUM_FUNCTIONS, sizeof(t_plan));
 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		critical_new(&(x->lock));

		//x->ptrs = (t_float **)malloc((MAX_NUM_FUNCTIONS * 5) * sizeof(t_float *));
		x->ptrs = (double **)malloc((MAX_NUM_FUNCTIONS * 5) * sizeof(double *));
		int i;
		t_jrgba black = (t_jrgba){0., 0., 0., 1.};
		for(i = 0; i < MAX_NUM_FUNCTIONS * 5; i++){
			//x->ptrs[i] = (t_float *)malloc(2048 * sizeof(t_float));
			x->ptrs[i] = (double *)malloc(2048 * sizeof(double));
		}

		for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
			x->functionColors[i] = black;
		}

		x->name = NULL;

		x->error_span = DEFAULT_ERROR_SPAN;
		x->error_offset = DEFAULT_ERROR_OFFSET;

		x->show_x_axis = x->show_y_axis = 1;

 		attr_dictionary_process(x, d); 
		if(x->numFunctions == 0){
			x->numFunctions = 1;
		}

		x->position_update_rate_ms = POSITION_UPDATE_RATE_MS;
		x->update_position_clock = clock_new((t_object *)x, (method)te_updatePositionCallback);

		x->box.z_misc = Z_PUT_FIRST;

		*((uint32_t *)_osc_point.function_size) = hton32(sizeof(_osc_point.function_address) +
						 sizeof(_osc_point.function_typetags) +
						 sizeof(_osc_point.function_data));

		*((uint32_t *)_osc_point.time_size) = hton32(sizeof(_osc_point.time_address) +
						 sizeof(_osc_point.time_typetags) +
						 sizeof(_osc_point.time_data));

		*((uint32_t *)_osc_point.atempo_size) = hton32(sizeof(_osc_point.atempo_address) +
						 sizeof(_osc_point.atempo_typetags) +
						 sizeof(_osc_point.atempo_data));

		*((uint32_t *)_osc_point.dtempo_size) = hton32(sizeof(_osc_point.dtempo_address) +
						 sizeof(_osc_point.dtempo_typetags) +
						 sizeof(_osc_point.dtempo_data));

		*((uint32_t *)_osc_point.aphase_size) = hton32(sizeof(_osc_point.aphase_address) +
						 sizeof(_osc_point.aphase_typetags) +
						 sizeof(_osc_point.aphase_data));

		*((uint32_t *)_osc_point.dphase_size) = hton32(sizeof(_osc_point.dphase_address) +
						 sizeof(_osc_point.dphase_typetags) +
						 sizeof(_osc_point.dphase_data));

		*((uint32_t *)_osc_point.alpha_size) = hton32(sizeof(_osc_point.alpha_address) +
						 sizeof(_osc_point.alpha_typetags) +
						 sizeof(_osc_point.alpha_data));

		*((uint32_t *)_osc_point.beta_size) = hton32(sizeof(_osc_point.beta_address) +
						 sizeof(_osc_point.beta_typetags) +
						 sizeof(_osc_point.beta_data));

		*((uint32_t *)_osc_point.ealpha_size) = hton32(sizeof(_osc_point.ealpha_address) +
						 sizeof(_osc_point.ealpha_typetags) +
						 sizeof(_osc_point.ealpha_data));

		*((uint32_t *)_osc_point.ebeta_size) = hton32(sizeof(_osc_point.ebeta_address) +
						 sizeof(_osc_point.ebeta_typetags) +
						 sizeof(_osc_point.ebeta_data));

		
 		jbox_ready((t_jbox *)x); 
		dsp_setupjbox((t_pxjbox *)x, 1);

		schedule_delay(x, (method)te_dumpCellblockCallback, 100, NULL, 0, NULL);
 		return x; 
 	} 
 	return NULL; 
} 

void te_dumpCellblockCallback(t_te *x, t_symbol *msg, int argc, t_atom *argv){
	te_dumpCellblock(x);
}

t_max_err te_functions_get(t_te *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	int n = 0, i;
	long npoints[x->numFunctions];
	memset(npoints, '\0', x->numFunctions * sizeof(long));
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		n++;
		while(p){
			n += 12;
			npoints[i]++;
			p = p->next;
		}
	}
	atom_alloc_array(n, argc, argv, &alloc);
	t_atom *ptr = *argv;
	for(i = 0; i < x->numFunctions; i++){
		atom_setlong(ptr++, npoints[i]);
		t_point *p = x->functions[i];
		while(p){
			atom_setfloat(ptr++, p->coords.x);
			atom_setfloat(ptr++, p->coords.y);
			atom_setfloat(ptr++, p->d_freq);
			atom_setfloat(ptr++, p->a_phase);
			atom_setfloat(ptr++, p->d_phase);
			atom_setfloat(ptr++, p->aux_points[0]);
			atom_setfloat(ptr++, p->aux_points[1]);
			atom_setfloat(ptr++, p->alpha);
			atom_setfloat(ptr++, p->beta);
			atom_setfloat(ptr++, p->error_alpha);
			atom_setfloat(ptr++, p->error_beta);
			atom_setlong(ptr++, p->whichPoint);
			p = p->next;
		}
	}
	return MAX_ERR_NONE;
}

t_max_err te_functions_set(t_te *x, t_object *attr, long argc, t_atom *argv){
	t_atom *ptr = argv;
	x->numFunctions = 0;
	while(ptr - argv < argc){
		int npoints = atom_getlong(ptr++);
		//printf("npoints = %d\n", npoints);
		if(npoints == 0){
			continue;
		}
		te_addFunction(x);
		while(npoints--){
			x->selected = te_allocAndInsertPoint(x, (t_pt){atom_getfloat(ptr++), atom_getfloat(ptr++)}, x->currentFunction);

			x->selected->d_freq = atom_getfloat(ptr++);
			x->selected->a_phase = atom_getfloat(ptr++);
			x->selected->d_phase = atom_getfloat(ptr++);
			x->selected->aux_points[0] = atom_getfloat(ptr++);
			x->selected->aux_points[1] = atom_getfloat(ptr++);
			x->selected->alpha = atom_getfloat(ptr++);
			x->selected->beta = atom_getfloat(ptr++);
			x->selected->error_alpha = atom_getfloat(ptr++);
			x->selected->error_beta = atom_getfloat(ptr++);
			x->selected->whichPoint = atom_getlong(ptr++);
			//te_postpoint(x->selected, printf);
		}
	}
	x->currentFunction = 0;
	te_invalidateAllFunctions(x);
	jbox_redraw((t_jbox *)x);
	te_dumpCellblock(x);
	return MAX_ERR_NONE;
}

t_max_err te_functionColorsGet(t_te *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	t_max_err ret;
	if(ret = atom_alloc_array(MAX_NUM_FUNCTIONS * 4, argc, argv, &alloc)){
		return ret;
	}
	int i;
	for(i = 0; i < MAX_NUM_FUNCTIONS; i++){
		atom_setfloat(*argv + (i * 4), x->functionColors[i].red);
		atom_setfloat(*argv + (i * 4 + 1), x->functionColors[i].green);
		atom_setfloat(*argv + (i * 4 + 2), x->functionColors[i].blue);
		atom_setfloat(*argv + (i * 4 + 3), x->functionColors[i].alpha);
	}

	return MAX_ERR_NONE;
}

t_max_err te_functionColorsSet(t_te *x, t_object *attr, long argc, t_atom *argv){
	int i;
	for(i = 0; i < argc / 4; i++){
		x->functionColors[i].red = atom_getfloat(argv + (i * 4));
		x->functionColors[i].green = atom_getfloat(argv + (i * 4 + 1));
		x->functionColors[i].blue = atom_getfloat(argv + (i * 4 + 2));
		x->functionColors[i].alpha = atom_getfloat(argv + (i * 4 + 3));
	}
	return MAX_ERR_NONE;
}

