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

#include "version.h" 
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_obex_util.h" 
#include "ext_critical.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "z_dsp.h"
#include "version.c" 
#include "gsl/gsl_sf.h"
#include "gsl/gsl_errno.h"
#undef msg
#include "boost/math/special_functions/gamma.hpp"
#include <boost/math/special_functions/beta.hpp>
#include <boost/math/tools/stats.hpp>
//#include <boost/math/tools/test.hpp>
#include <boost/math/constants/constants.hpp>
//#include <boost/type_traits/is_floating_point.hpp>

#define MAX_NUM_FUNCTIONS 8
#define POINT_WIDTH 14
#define CONTROL_POINT_WIDTH 10

#define DEFAULT_ERROR_SPAN .8
#define DEFAULT_ERROR_OFFSET .1

#define BEFORE_FIRST_POINT 1
#define AFTER_LAST_POINT 2

typedef struct _point{ 
 	t_pt screen_coords; 
	double d_freq_sc; // departure freq in screen coords
	double a_phase, d_phase; // arrival and departure phase
	double aux_points[2];
	// shape parameters for the beta distribution
	double alpha, beta;
	double error_alpha, error_beta;
	int whichPoint;  // 0=this point, >0=aux_points
 	struct _point *next; 
 	struct _point *prev; 
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
 	struct _plan *next; 
 	struct _plan *prev; 
} t_plan; 

typedef struct _te{ 
 	t_pxjbox box; 
 	void *out_info;
	void *proxy;
	long in;
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
	float *last_y;  // in the perform routine, it's too expensive to compute the tempo so we'll do it manually
 	int currentFunction; 
 	int numFunctions; 
	t_plan plan;
	int dirty; // set this to non-zero to indicate that a point has been added
	double time_min, time_max;
	double freq_min, freq_max;
	double error_span, error_offset; // these are the defaults that will be used when a new point is created
	int mode;
	int show_beat_correction;
	int show_tempo_correction;
	int show_x_axis, show_y_axis;
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba phaseColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
	t_jrgba correctionColor;
	t_jrgba correctionColor_box;
	t_symbol *name;
	t_float **ptrs;
	t_pt last_mouse;
} t_te; 

t_class *te_class; 

t_symbol *ps_cellblock, *ps_pointNum, *ps_time, *ps_dFreq, *ps_aFreq, *ps_dPhase, *ps_aPhase, *ps_alpha, *ps_beta, *ps_errorAlpha, *ps_errorBeta, *ps_error;

void te_paint(t_te *x, t_object *patcherview); 
t_int *te_perform(t_int *w);
void te_makePlan(t_te *x, float f, int function, t_plan *plan);
int te_isPlanValid(t_te *x, double time, t_plan *plan, int function);
void te_computePhaseError(t_te *x, t_plan *plan);
double te_betaPDF(double z, double a, double b);
double te_betaCDF(double z, double a, double b);
double te_betaCDFInt(double z, double a, double b);
double te_scaledBetaCDFInt(double z, double a, double b, double scale, double offset);
double te_computeCorrectedTempo(double t, t_plan *p);
double te_computeTempo(double t, t_plan *p);
double te_computePhase(double t, t_plan *p);
double te_computeCorrectedPhase(double t, t_plan *p);
void te_editSel(t_te *x, double xx, double yy, double zz);
void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_float(t_te *x, double f);
void te_find_btn(t_point *function, double x, t_point **left, t_point **right);
void te_reorderPoint(t_point *p);
void te_initReorderedPoint(t_te *x, t_point *p);
void te_swapPoints(t_point *p1, t_point *p2);
t_point *te_select(t_te *x, t_pt p);
void te_draw_bounds(t_te *x, t_jgraphics *g, t_rect *rect); 
void te_assist(t_te *x, void *b, long m, long a, char *s); 
void te_free(t_te *x); 
t_point *te_selectControlPoint(t_te *x, t_pt p);
t_max_err te_notify(t_te *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void te_mousedown(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mousedrag(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mouseup(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_outputSelection(t_te *x);
void te_addFunction(t_te *x); 
void te_setFunction(t_te *x, long f); 
double te_clip(double f, double min, double max);
double te_scale(double f, double min_in, double max_in, double min_out, double max_out);
void te_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
void te_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
t_point *te_insertPoint(t_te *x, t_pt screen_coords, int functionNum);
void te_removePoint(t_te *x, t_point *point, int functionNum);
void te_dump(t_te *x);
void te_dumpBeats(t_te *x);
void te_dumpCellblock(t_te *x);
void te_time_min(t_te *x, double f);
void te_time_max(t_te *x, double f);
void te_time_minmax(t_te *x, double min, double max);
void te_freq_min(t_te *x, double f);
void te_freq_max(t_te *x, double f);
void te_freq_minmax(t_te *x, double min, double max);
void te_postPlan(t_plan *plan);
void te_clear(t_te *x);
void te_clearFunction(t_te *x, int f);
void te_clearCurrent(t_te *x);
void te_doClearFunction(t_te *x, int f);
//void te_read(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_doread(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_write(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_dowrite(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_dsp(t_te *x, t_signal **sp, short *count);
t_symbol *te_mangleName(t_symbol *name, int i, int fnum);
void te_errorHandler(const char * reason, const char * file, int line, int gsl_errno);
int main(void); 
void *te_new(t_symbol *s, long argc, t_atom *argv); 

void te_paint(t_te *x, t_object *patcherview){ 
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

	critical_enter(x->lock);

	// draw the beat lines
	{
		int i, j;
		double prev_t = te_scale(-1, 0, rect.width, x->time_min, x->time_max);
		t_plan plan;
		for(j = 0; j < x->numFunctions; j++){
			if(x->functions[j] == NULL){
				continue;
			}

			//post("1. makePlan, %f", prev_t);
			te_makePlan(x, prev_t, j, &plan);
			double prev_correctedPhase = te_computeCorrectedPhase(prev_t, &plan);
			double prev_phase = te_computePhase(prev_t, &plan);
			for(i = 0; i < rect.width * 4; i++){
				double idx = i / 4.;
				double t = te_scale(idx, 0, rect.width, x->time_min, x->time_max);
				double p;
				if(!te_isPlanValid(x, t, &plan, j)){
					te_makePlan(x, t, j, &plan);
				}

				if(x->show_beat_correction && j == x->currentFunction){//&& t >= plan.startTime + plan.segmentDuration_sec){
					jgraphics_set_source_jrgba(g, &(x->lineColor));
					p = te_computePhase(t, &plan);
					if((p - floor(p)) < (prev_phase - floor(prev_phase))){
						jgraphics_move_to(g, idx, rect.height);
						jgraphics_line_to(g, idx, te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
						jgraphics_stroke(g);
					}
					prev_phase = p;
					jgraphics_set_source_jrgba(g, &(x->correctionColor));
				}else{
					if(j == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->lineColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}

				p = te_computeCorrectedPhase(t, &plan);
				if(p - floor(p) < prev_correctedPhase - floor(prev_correctedPhase)){
									jgraphics_move_to(g, idx, rect.height);
									//if(x->show_tempo_correction){
						jgraphics_line_to(g, idx, te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
						//}else{
						//jgraphics_line_to(g, idx, te_scale(te_computeCorrectedTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
						//}
					jgraphics_stroke(g);
				}

				prev_t = t;
				prev_correctedPhase = p;
			}
		}
	}

	// draw the tempo lines
	{
		jgraphics_set_source_jrgba(g, &(x->lineColor));
		int i;
		for(i = 0; i < x->numFunctions; i++){
			t_point *p = x->functions[i];
			while(p){
				if(!(p->next)){
					break;
				}
				int j;
				t_plan plan;
				te_makePlan(x, te_scale(p->screen_coords.x + 1, 0, rect.width, x->time_min, x->time_max), i, &plan);
				double freq = te_computeTempo(plan.startTime, &plan);
				double freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
				jgraphics_move_to(g, p->screen_coords.x, freq_sc);
				double endTime_sc = te_scale(plan.endTime, x->time_min, x->time_max, 0., rect.width);
				for(j = p->screen_coords.x; j < endTime_sc; j++){

					// if we will show the correction, we should draw the uncorrected line here
					//if(x->show_tempo_correction){
						freq = te_computeTempo(te_scale(j, 0, rect.width, x->time_min, x->time_max), &plan);
						//}else{
						//freq = te_computeCorrectedTempo(te_scale(j, 0, rect.width, x->time_min, x->time_max), &plan);
						//}
					freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
					jgraphics_line_to(g, j, freq_sc);
					jgraphics_stroke(g);
					jgraphics_move_to(g, j, freq_sc);
				}
				//jgraphics_stroke(g);
				p = p->next;
			}
		}
	}

	// draw points 
 	{ 
		int i;
		double ps = POINT_WIDTH; // point size
		double ps2 = ps / 2.; // point size divided by 2
 		//jgraphics_set_line_width(g, 0.25); 
		for(i = 0; i < x->numFunctions; i++){
			t_point *p = x->functions[i];
			double xx, yy;
			while(p){
				// departure circle
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}

				jgraphics_ellipse(g, p->screen_coords.x - ps2 - 2, p->d_freq_sc - ps2 - 2, ps + 4, ps + 4);
				jgraphics_stroke(g);

				// departure phase tick
				jgraphics_set_source_jrgba(g, &(x->phaseColor));

				xx = (ps2 + 4) * sin(2 * M_PI * p->d_phase);
				yy = (ps2 + 4) * cos(2 * M_PI * p->d_phase);
				jgraphics_move_to(g, p->screen_coords.x + xx, p->d_freq_sc - yy);

				xx = (6 + ps2 + 4) * sin(2 * M_PI * (p->d_phase - .03));
				yy = (6 + ps2 + 4) * cos(2 * M_PI * (p->d_phase - .03));
				jgraphics_line_to(g, p->screen_coords.x + xx, p->d_freq_sc - yy);
				xx = (6 + ps2 + 4) * sin(2 * M_PI * (p->d_phase + .03));
				yy = (6 + ps2 + 4) * cos(2 * M_PI * (p->d_phase + .03));
				jgraphics_line_to(g, p->screen_coords.x + xx, p->d_freq_sc - yy);

				jgraphics_close_path(g);
				jgraphics_fill(g);

				// arrival circle
				if(p == x->selected){
					jgraphics_set_source_jrgba(g, &(x->selectionColor));
				}else{
					if(i == x->currentFunction){
						jgraphics_set_source_jrgba(g, &(x->pointColor));
					}else{
						jgraphics_set_source_jrgba(g, &(x->bgFuncColor));
					}
				}

				jgraphics_ellipse(g, p->screen_coords.x - ps2, p->screen_coords.y - ps2, ps, ps);
				jgraphics_fill(g);

				// arrival phase tick
				jgraphics_set_source_jrgba(g, &(x->phaseColor));

				xx = (ps2) * sin(2 * M_PI * p->a_phase);
				yy = (ps2) * cos(2 * M_PI * p->a_phase);
				jgraphics_move_to(g, p->screen_coords.x + xx, p->screen_coords.y - yy);

				xx = (ps2 - 4) * sin(2 * M_PI * (p->a_phase - .25));
				yy = (ps2 - 4) * cos(2 * M_PI * (p->a_phase - .25));
				jgraphics_line_to(g, p->screen_coords.x + xx, p->screen_coords.y - yy);
				xx = (ps2 - 4) * sin(2 * M_PI * (p->a_phase + .25));
				yy = (ps2 - 4) * cos(2 * M_PI * (p->a_phase + .25));
				jgraphics_line_to(g, p->screen_coords.x + xx, p->screen_coords.y - yy);

				jgraphics_close_path(g);
				jgraphics_fill(g);

				p = p->next;
			}
		}
 	} 

	// draw the tempo correction line and control points
	{
		if(x->show_tempo_correction){
			jgraphics_set_source_jrgba(g, &(x->correctionColor));
			int i;
			for(i = 0; i < x->numFunctions; i++){
				if(i != x->currentFunction){
					continue;
				}
				t_point *p = x->functions[i];
				while(p){
					if(!(p->next)){
						break;
					}
					double correctionStart_sc, correctionEnd_sc;
					int j;
					t_plan plan;
					//post("%f %f", correctionStart_sc, correctionEnd_sc);
					te_makePlan(x, te_scale(p->screen_coords.x + 1, 0, rect.width, x->time_min, x->time_max), i, &plan);
					correctionStart_sc = te_scale(p->aux_points[0], 0., 1., p->screen_coords.x, p->next->screen_coords.x);
					correctionEnd_sc = te_scale(p->aux_points[1], 0., 1., p->screen_coords.x, p->next->screen_coords.x);
					double freq = te_computeTempo(plan.correctionStart, &plan);
					double freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
					double freq_sc1 = freq_sc;
					//jgraphics_ellipse(g, correctionStart_sc - CONTROL_POINT_WIDTH / 2, freq_sc - CONTROL_POINT_WIDTH / 2, CONTROL_POINT_WIDTH, CONTROL_POINT_WIDTH);
					jgraphics_set_source_jrgba(g, &(x->correctionColor));
					jgraphics_move_to(g, correctionStart_sc, freq_sc);
					for(j = correctionStart_sc; j < correctionEnd_sc; j++){
						freq = te_computeCorrectedTempo(te_scale(j, 0, rect.width, x->time_min, x->time_max), &plan);
						freq_sc = te_scale(freq, x->freq_min, x->freq_max, rect.height, 0);
						jgraphics_line_to(g, j, freq_sc);
						jgraphics_move_to(g, j, freq_sc);

						//jgraphics_stroke(g);
					}
					//jgraphics_ellipse(g, j - CONTROL_POINT_WIDTH / 2, freq_sc - CONTROL_POINT_WIDTH / 2, CONTROL_POINT_WIDTH, CONTROL_POINT_WIDTH);
					jgraphics_stroke(g);

					jgraphics_set_source_jrgba(g, &(x->correctionColor_box));
					jgraphics_move_to(g, correctionStart_sc, rect.height);
					jgraphics_line_to(g, correctionStart_sc, freq_sc1);
					jgraphics_line_to(g, correctionEnd_sc, freq_sc);
					jgraphics_line_to(g, correctionEnd_sc, rect.height);

					jgraphics_fill(g);
					p = p->next;
				}
			}
		}
	}

	// draw the function number in the top left corner
	{
 		jgraphics_set_line_width(g, 0.5); 
		jgraphics_set_source_jrgba(g, &(x->lineColor));
		char buf[32];
		double w, h;
		sprintf(buf, "%d/%d", x->currentFunction + 1, x->numFunctions);
		jgraphics_set_font_size(g, 10);
		jgraphics_text_measure(g, buf, &w, &h);
		//post("%f %f", w, h);
		jgraphics_rectangle(g, rect.width - w - 4, 0, w + 4, h + 2);
		jgraphics_stroke(g);
		jgraphics_move_to(g, rect.width - w - 2, h - 2);
		jgraphics_show_text(g, buf);
		//jgraphics_stroke(g);
	}

	// draw the y axis
	{
		if(x->show_y_axis){
			jgraphics_set_line_width(g, 0.5); 
			char buf1[32], buf2[32], buf3[32];
			double w1, w2, w3, h1, h2, h3;
			double maxw;
			sprintf(buf1, "%.1f", x->freq_min);
			sprintf(buf2, "%.1f", x->freq_max - x->freq_min);
			sprintf(buf3, "%.1f", x->freq_max);
			jgraphics_set_font_size(g, 10);
			jgraphics_text_measure(g, buf1, &w1, &h1);
			jgraphics_text_measure(g, buf2, &w2, &h2);
			jgraphics_text_measure(g, buf3, &w3, &h3);
			if(w1 >= w2){
				maxw = w1;
			}else{
				maxw = w2;
			}
			if(w3 > maxw){
				maxw = w3;
			}

			double p1, p2, p3;
			p1 = rect.height / 8;
			p2 = rect.height / 2;
			p3 = (rect.height / 8) * 7;
			//jgraphics_rectangle(g, 0, p1, maxw + 4, p3);
			//t_jrgba box = x->bgcolor;
			//box.alpha = .25;
			//jgraphics_set_source_jrgba(g, &(box));
			//jgraphics_fill(g);

			jgraphics_set_source_jrgba(g, &(x->lineColor));
			jgraphics_move_to(g, 2, p1 + (h1 / 2));
			jgraphics_show_text(g, buf1);

			jgraphics_move_to(g, 2, p2 + (h2 / 2));
			jgraphics_show_text(g, buf2);

			jgraphics_move_to(g, 2, (p3 + (h3 / 2)));
			jgraphics_show_text(g, buf3);

			jgraphics_move_to(g, 8, p1 + h1);
			jgraphics_line_to(g, 4, p1 + h1 + 4);
			jgraphics_line_to(g, 12, p1 + h1 + 4);
			jgraphics_close_path(g);
			jgraphics_fill(g);

			jgraphics_move_to(g, 8, p1 + h1);
			jgraphics_line_to(g, 8, p2 - h2);
			jgraphics_stroke(g);

			jgraphics_move_to(g, 8, p2 + h2);
			jgraphics_line_to(g, 8, p3 - h3);
			jgraphics_stroke(g);

			jgraphics_move_to(g, 8, p3 - h3);
			jgraphics_line_to(g, 4, p3 - h3 - 4);
			jgraphics_line_to(g, 12, p3 - h3 - 4);
			jgraphics_close_path(g);
			jgraphics_fill(g);
		}
	}

	// draw the x axis
	{
		if(x->show_x_axis){
			jgraphics_set_line_width(g, 0.5); 
			char buf1[32], buf2[32], buf3[32];
			double w1, w2, w3, h1, h2, h3;
			sprintf(buf1, "%.1f", x->time_min);
			sprintf(buf2, "%.1f", x->time_max - x->time_min);
			sprintf(buf3, "%.1f", x->time_max);
			jgraphics_set_font_size(g, 10);
			jgraphics_text_measure(g, buf1, &w1, &h1);
			jgraphics_text_measure(g, buf2, &w2, &h2);
			jgraphics_text_measure(g, buf3, &w3, &h3);

			double p1, p2, p3;
			p1 = rect.width / 8;
			p2 = rect.width / 2;
			p3 = (rect.width / 8) * 7;
			//jgraphics_rectangle(g, p1 - 2, rect.height - h1, p3 + 2, rect.height);
			//t_jrgba box = x->bgcolor;
			//box.alpha = .5;
			//jgraphics_set_source_jrgba(g, &(box));
			//jgraphics_fill(g);

			jgraphics_set_source_jrgba(g, &(x->lineColor));
			jgraphics_move_to(g, p1, rect.height - 4);
			jgraphics_show_text(g, buf1);

			jgraphics_move_to(g, p2 - (w2 / 2), rect.height - 4);
			jgraphics_show_text(g, buf2);

			jgraphics_move_to(g, p3 - w3, rect.height - 4);
			jgraphics_show_text(g, buf3);

			jgraphics_move_to(g, p1 + w1 + 4, rect.height - 8);
			jgraphics_line_to(g, p1 + w1 + 8, rect.height - 4);
			jgraphics_line_to(g, p1 + w1 + 8, rect.height - 12);
			jgraphics_close_path(g);
			jgraphics_fill(g);

			jgraphics_move_to(g, p1 + w1 + 4, rect.height - 8);
			jgraphics_line_to(g, p2 - w2, rect.height - 8);
			jgraphics_stroke(g);

			jgraphics_move_to(g, p2 + w2, rect.height - 8);
			jgraphics_line_to(g, p3 - w3 - 4, rect.height - 8);
			jgraphics_stroke(g);

			jgraphics_move_to(g, p3 - w3 - 4, rect.height - 8);
			jgraphics_line_to(g, p3 - w3 - 8, rect.height - 4);
			jgraphics_line_to(g, p3 - w3 - 8, rect.height - 12);
			jgraphics_close_path(g);
			jgraphics_fill(g);
		}
	}


	critical_exit(x->lock);
} 

t_int *te_perform(t_int *w){
	t_te *x = (t_te *)w[1];
	int n = (int)w[2];
	t_float *in = (t_float *)w[3];
	t_float *out_phase_wrapped = (t_float *)w[4];
	t_float *out_phase = (t_float *)w[5];
	t_float *out_bps = (t_float *)w[6];
	int i, j;
	t_plan *plan = &(x->plan);
	t_symbol *name1, *name2, *name3;
	//post("x = %p, %d, plan = %p, %p %p %p %p", x, n, plan, in, out_phase_wrapped, out_phase, out_bps);

	for(j = 0; j < x->numFunctions; j++){
		if(name1 = te_mangleName(x->name, 1, j)){
			name1->s_thing = (t_object *)(x->ptrs[j * 3]);
		}
		if(name2 = te_mangleName(x->name, 2, j)){
			name2->s_thing = (t_object *)(x->ptrs[(j * 3) + 1]);
		}
		if(name3 = te_mangleName(x->name, 3, j)){
			name3->s_thing = (t_object *)(x->ptrs[(j * 3) + 2]);
		}

		if(x->functions[j] == NULL){
			memset(x->ptrs[(j * 3)], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 3) + 1], 0, n * sizeof(t_float));
			memset(x->ptrs[(j * 3) + 2], 0, n * sizeof(t_float));
		}else{

			te_makePlan(x, in[0], j, plan);
			for(i = 0; i < n; i++){
				if(te_isPlanValid(x, in[i], plan, j) == 0){
					te_makePlan(x, in[i], j, plan);
				}

				x->ptrs[(j * 3) + 1][i] = te_computeCorrectedPhase(in[i], plan);
				x->ptrs[(j * 3)][i] = x->ptrs[(j * 3) + 1][i] - floor((x->ptrs[(j * 3) + 1][i]));
				//x->ptrs[(j * 3) + 2][i] = m * in[i] + b;
				//x->ptrs[(j * 3) + 2][i] = 0.; //te_computeTempo(in[i], plan);
				x->ptrs[(j * 3) + 2][i] = (x->ptrs[(j * 3) + 1][i] - x->last_y[j]) * 44100.;
				x->last_y[j] = x->ptrs[(j * 3) + 1][i];
			}
		}
	}
	memcpy(out_phase_wrapped, x->ptrs[x->currentFunction * 3], n * sizeof(t_float));
	memcpy(out_phase, x->ptrs[x->currentFunction * 3 + 1], n * sizeof(t_float));
	memcpy(out_bps, x->ptrs[x->currentFunction * 3 + 2], n * sizeof(t_float));

	for(j = x->numFunctions; j < MAX_NUM_FUNCTIONS; j++){
		if(name1 = te_mangleName(x->name, 1, j)){
			name1->s_thing = NULL;
		}
		if(name2 = te_mangleName(x->name, 2, j)){
			name2->s_thing = NULL;
		}
		if(name3 = te_mangleName(x->name, 3, j)){
			name3->s_thing = NULL;
		}
	}

	return w + 7;
}

void te_makePlan(t_te *x, float f, int function, t_plan *plan){
	if(!plan){
		return;
	}
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double f_sc = te_scale(f, x->time_min, x->time_max, 0, r.width);
	t_point *p = x->functions[function];
	t_point *next = NULL;
	if(p){
		next = p->next;
	}else{
		memset(plan, 0, sizeof(plan));
		return;
	}

	// we are somewhere between the minimum time (probably 0) and the first
	// point.  so we'll make a plan with a freq of 0.
	if(f_sc < p->screen_coords.x){
		plan->alpha = p->alpha;
		plan->beta = p->beta;
		plan->error_alpha = p->error_alpha;
		plan->error_beta = p->error_beta;
		plan->state = BEFORE_FIRST_POINT;
		plan->startTime = x->time_min;
		plan->endTime = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
		plan->startFreq = 0.;
		plan->endFreq = 0.;
		plan->startPhase = p->a_phase;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.
		plan->endPhase = p->a_phase; 
		te_computePhaseError(x, plan);
		//plan->correctionStart = plan->startTime + ((plan->endTime - plan->startTime) / 3.);
		//plan->correctionEnd = plan->correctionStart + ((plan->endTime - plan->startTime) / 3.);
		plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
		plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
		//te_postPlan(plan);
		return;
	}

	while(p && next){
		if(f_sc >= p->screen_coords.x && f_sc < next->screen_coords.x){
			plan->alpha = p->alpha;
			plan->beta = p->beta;
			plan->error_alpha = p->error_alpha;
			plan->error_beta = p->error_beta;
			plan->state = 0;
			plan->startTime = te_scale(p->screen_coords.x, 0., r.width, x->time_min, x->time_max);
			plan->endTime = te_scale(next->screen_coords.x, 0., r.width, x->time_min, x->time_max);
			plan->startFreq = te_scale(p->d_freq_sc, r.height, 0., x->freq_min, x->freq_max);
			plan->endFreq = te_scale(next->screen_coords.y, r.height, 0., x->freq_min, x->freq_max);
			plan->startPhase = p->d_phase;
			plan->endPhase = next->a_phase;
			te_computePhaseError(x, plan);
			//plan->correctionStart = plan->startTime + ((plan->endTime - plan->startTime) / 3.);
			//plan->correctionEnd = plan->correctionStart + ((plan->endTime - plan->startTime) / 3.);
			plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
			plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
			//te_postPlan(plan);
			return;
		}
		p = next;
		next = next->next;
	}

	// if we made it here, we're somewhere between the last point and time_max
	if(f_sc >= p->screen_coords.x){
		plan->alpha = p->alpha;
		plan->beta = p->beta;
		plan->error_alpha = p->error_alpha;
		plan->error_beta = p->error_beta;
		plan->state = AFTER_LAST_POINT;
		plan->startTime = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
		plan->endTime = x->time_max;
		plan->startFreq = te_scale(p->screen_coords.y, r.height, 0., x->freq_min, x->freq_max);
		plan->endFreq = te_scale(p->screen_coords.y, r.height, 0., x->freq_min, x->freq_max);
		plan->startPhase = p->d_phase;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.  this will effectively be a jump to phase and freq 0.
		plan->endPhase = p->d_phase;
		te_computePhaseError(x, plan);
		//plan->correctionStart = plan->startTime + ((plan->endTime - plan->startTime) / 3.);
		//plan->correctionEnd = plan->correctionStart + ((plan->endTime - plan->startTime) / 3.);
		plan->correctionStart = te_scale(p->aux_points[0], 0, 1., plan->startTime, plan->endTime);
		plan->correctionEnd = te_scale(p->aux_points[1], 0, 1., plan->startTime, plan->endTime);
		//te_postPlan(plan);
		return;

	}
}

int te_isPlanValid(t_te *x, double time, t_plan *plan, int function){
	if(!plan){
		return 0;
	}
	if(time < plan->startTime || time > plan->endTime){
		return 0;
	}
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double stsc, etsc;
	stsc = te_scale(plan->startTime, x->time_min, x->time_max, 0, r.width);
	etsc = te_scale(plan->endTime, x->time_min, x->time_max, 0, r.width);
	t_point *p = x->functions[function];
	if(!p){
		return 0;
	}
	if(p->screen_coords.x - etsc < .001 && plan->startTime == 0.){
		return 1;
	}
	while(p){
		if(fabs(p->screen_coords.x - stsc) < .001){
			//post("%f == %f", p->screen_coords.x, stsc);
			if(p->next){
				if(fabs(p->next->screen_coords.x - etsc) < .001){
					//post("there is p->next and %f == %f", p->next->screen_coords.x, etsc);
					return 1;
				}else{
					//post("there is p->next but %f != %f", p->next->screen_coords.x, etsc);
					return 0;
				}
			}else if(fabs(etsc - r.width) < 0.001){
				//post("there is no p->next but %f == %f", etsc, r.width)
				return 1;
			}else{
				//post("man, this plan is fucked");
				return 0;
			}
		}else{
			//post("%f != %f", p->screen_coords.x, stsc);
		}
		p = p->next;
	}
	return 0;
}

void te_computePhaseError(t_te *x, t_plan *plan){
	double endingPhase = te_scaledBetaCDFInt(1., plan->alpha, plan->beta, plan->endFreq - plan->startFreq, plan->startFreq);
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
		//phaseError = 1. - phaseError;
	}
	//post("phase error = %f", phaseError);
	plan->phaseError = phaseError;
}

/*
  void te_computePhaseError(t_te *x, t_plan *plan){
  double m = (plan->endFreq - plan->startFreq) / (plan->endTime - plan->startTime);
  double b = plan->startFreq - (m * plan->startTime);
  plan->m = m;
  plan->b = b;

  // this is the offset that we need to subtract in order to get this part of the plan to start at 0.
  plan->phaseError_start = ((plan->startTime * ((2. * b) + (m * plan->startTime))) / 2.) - plan->startPhase;

  // evaluate the integral at the endpoint and translate by the offset computed above.
  plan->phaseError = ((plan->endTime * ((2. * b) + (m * plan->endTime))) / 2.) - plan->phaseError_start;

  // wrap to [0,1)
  plan->phaseError = plan->phaseError - ((int)(plan->phaseError));

  // compute the actual error
  plan->phaseError = plan->endPhase - plan->phaseError;

  // add 1 to the phase if necessary
  if(x->mode < 0){
  }else if(x->mode > 0){
  plan->phaseError += 1.;
  }else{
  if(fabs(plan->phaseError) > 0.5){
  if(plan->phaseError < 0.){
  plan->phaseError += 1.;
  }else{
  plan->phaseError -= 1.;
  }
  }
  }

  // check to see if the phase accumulation function ever has a negative derivitave and add 1 to the phase error if so
  double i;
  double st, et, inc;
  st = plan->correctionStart;
  et = plan->correctionEnd;
  inc = (et - st) / 10.;
  //post("%f %f %f", st, et, inc);
  double prev_phase = te_computeCorrectedPhase(st, plan);
  double prev_time = st;
  for(i = st + inc; i < et; i+= inc){
  double ph = te_computeCorrectedPhase(i, plan);
  //post("%f %f %f %f %f", i, ph, prev_time, prev_phase, ((ph - prev_phase) / (i - prev_time)));
  if(((ph - prev_phase) / (i - prev_time)) < 0){
  //post("***");
  plan->phaseError += 1;
  return;
  }
  prev_phase = ph;
  prev_time = i;
  }

  }
*/

double te_betaPDF(double z, double a, double b){
	//post("te_betaPDF: z = %f", z);
	//return ((pow(z, a - 1) * (pow(1 - z, b - 1))) / gsl_sf_beta(a, b));
	return ((pow(z, a - 1) * (pow(1 - z, b - 1))) / boost::math::beta(a, b));
}

double te_betaCDF(double z, double a, double b){
	//post("te_betaCDF: z = %f", z);
	z = te_clip(z, 0., 1.);
	//return gsl_sf_beta_inc(a, b, z);
	return boost::math::ibeta(a, b, z);
}

double te_betaCDFInt(double z, double a, double b){
	post("te_betaCDFInt: z = %f", z);
	z = te_clip(z, 0., 1.);
	//double beta_ab = gsl_sf_beta(a, b);
	//return (z * (gsl_sf_beta_inc(a, b, z) * beta_ab) - (gsl_sf_beta_inc(a + 1, b, z) * gsl_sf_beta(a + 1, b))) / beta_ab;
	return (z * (boost::math::beta(a, b, z) - boost::math::beta(a + 1, b, z))) / boost::math::beta(a, b);
}

double te_scaledBetaCDFInt(double z, double a, double b, double scale, double offset){
	//post("te_scaledBetaCDFInt: z = %f", z);
	//post("%f %f %f %f %f", z, a, b, scale, offset);
	z = te_clip(z, 0., 1.);
	//double beta_ab = gsl_sf_beta(a, b);
	//return (offset * z + (scale * z * ((gsl_sf_beta_inc(a, b, z) * beta_ab) - (gsl_sf_beta_inc(a + 1, b, z) * gsl_sf_beta(a + 1, b)) / z)) / beta_ab);
	return (offset * z + (scale * z * (boost::math::beta(a, b, z) - (boost::math::beta(a + 1, b, z)) / z)) / boost::math::beta(a, b));
}

double te_computeCorrectedTempo(double t, t_plan *p){
	if(t >= p->correctionStart && t <= p->correctionEnd){
		return (te_computeCorrectedPhase(t, p) - te_computeCorrectedPhase(t - (1. / 44100.), p)) * 44100.;
	}else{
		return te_computeTempo(t, p);
	}
	return 0.;
}

double te_computeTempo(double t, t_plan *p){
	double norm_t = te_scale(t, p->startTime, p->endTime, 0., 1.);
	norm_t = te_clip(norm_t, 0., 1.);
	//post("te_computeTempo: norm_t = %f", norm_t);
	//return gsl_sf_beta_inc(p->alpha, p->beta, norm_t) * (p->endFreq - p->startFreq) + p->startFreq;
	return boost::math::ibeta(p->alpha, p->beta, norm_t) * (p->endFreq - p->startFreq) + p->startFreq;
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
	//post("te_computePhase: norm_t = %f", norm_t);
	return te_scaledBetaCDFInt(norm_t, p->alpha, p->beta, p->endFreq - p->startFreq, p->startFreq) * (p->endTime - p->startTime) + p->startPhase;
}

double te_computeCorrectedPhase(double t, t_plan *p){
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
			double error = p->startPhase;
			double sr = 44100.;
			if(t >= p->startTime && t < p->correctionStart){
			}else if(t >= p->correctionStart && t < p->correctionEnd){
				//double segdur = p->correctionEnd - p->correctionStart;
				//error = ((p->phaseError / (segdur * sr)) * ((t - (p->startTime + segdur)) * sr));
				//double norm_t = (t - p->correctionStart) / (p->correctionEnd - p->correctionStart);
				double norm_t = te_scale(t, p->correctionStart, p->correctionEnd, 0., 1.);
				norm_t = te_clip(norm_t, 0., 1.);
				error += p->phaseError * (te_betaCDF(norm_t, p->error_alpha, p->error_beta));
				//post("te_correctedPhase: norm_t %f, error %f", norm_t, error);
				//post("%f %f %f %f", norm_t, p->phaseError, te_betaCDF(p->error_alpha, p->error_beta, norm_t), error);
			}else if(t >= p->correctionEnd && t <= p->endTime){
				error += p->phaseError;
			}

			double norm_t = te_scale(t, p->startTime, p->endTime, 0., 1.);
			norm_t = te_clip(norm_t, 0., 1.);
			//post("te_correctedPhase: norm_t = %f", norm_t);
			//return ((p->startPhase + te_scaledBetaCDFInt(norm_t, p->alpha, p->beta, p->endFreq - p->startFreq, p->startFreq)) * (p->endTime - p->startTime)) + error;
			return ((te_scaledBetaCDFInt(norm_t, p->alpha, p->beta, p->endFreq - p->startFreq, p->startFreq)) * (p->endTime - p->startTime)) + error;
		}
	}
}

void te_editSel(t_te *x, double xx, double yy, double zz){
	/*
	  t_pt screen_coords;
	  t_rect r;
	  jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	  //te_getScreenCoords(r, norm_coords, &screen_coords);
	  screen_coords.x = te_scale(xx, x->time_min, x->time_max, 0, r.width);
	  screen_coords.y = te_scale(yy, x->freq_min, x->freq_max, r.height, 0);
	  //post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
	  te_removePoint(x, x->selected, x->currentFunction);
	  x->selected = te_insertPoint(x, screen_coords, x->currentFunction);
	  x->selected->phase = zz;
	  jbox_redraw((t_jbox *)x);
	  te_dumpCellblock(x);
	*/
}

void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom *a = argv;
	int functionNum = x->currentFunction;
	t_pt screen_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);

	switch(proxy_getinlet((t_object *)x)){
	case 0:
		if(argc == 4){
			functionNum = atom_getlong(a++);
			if(functionNum > x->numFunctions - 1){
				x->numFunctions = functionNum + 1;
			}
		}

		//norm_coords.x = atom_getfloat(a++);
		//norm_coords.y = atom_getfloat(a++);
		//te_getScreenCoords(r, norm_coords, &screen_coords);
		screen_coords.x = te_scale(atom_getfloat(a++), x->time_min, x->time_max, 0, r.width);
		screen_coords.y = te_scale(atom_getfloat(a++), x->freq_min, x->freq_max, r.height, 0);
		//post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
		x->selected = te_insertPoint(x, screen_coords, functionNum);
		x->selected->a_phase = atom_getfloat(a);
		break;
	case 1:
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
					p->screen_coords.x = te_scale(val, x->time_min, x->time_max, 0, r.width);
					break;
				case 2:
					// freq
					p->screen_coords.y = te_scale(val, x->freq_min, x->freq_max, r.height, 0);
					break;
				case 3:
					// departure freq
					p->d_freq_sc = te_scale(val, x->freq_min, x->freq_max, r.height, 0);
					break;
				case 4:
					// arrival phase
					p->a_phase = val;
					break;
				case 5:
					// departure phase
					p->d_phase = val;
					break;
				case 6:
					// alpha
					p->alpha = val;
					break;
				case 7:
					// beta
					p->beta = val;
					break;
				case 8:
					// error_alpha
					p->error_alpha = val;
					break;
				case 9:
					// error_beta
					p->error_beta = val;
					break;
				}
				if(col == 1){
					/*
					  double ph = p->phase;
					  double a, b, ae, be;
					  a = p->alpha;
					  b = p->beta;
					  ae = p->error_alpha;
					  be = p->error_beta;
					  t_pt sc = p->screen_coords;
					  te_removePoint(x, p, x->currentFunction);
					  x->selected = te_insertPoint(x, sc, x->currentFunction);
					  x->selected->phase = ph;
					  x->selected->alpha = a;
					  x->selected->beta = b;
					  x->selected->error_alpha = ae;
					  x->selected->error_beta = be;
					*/
					x->selected = p;
					//x->selected->screen_coords = sc;
					te_reorderPoint(x->selected);
					te_initReorderedPoint(x, x->selected);
				}
			}else{
				t_pt sc = {0., r.height};
				/*
				  double ph = 0.;
				  double a, b, ae, be;
				  a = p->alpha;
				  b = p->beta;
				  ae = p->error_alpha;
				  be = p->error_beta;
				*/
				switch(col){
				case 0:
					sc.x = te_scale(val, x->time_min, x->time_max, 0, r.width);
					break;
				case 1:
					sc.y = te_scale(val, x->freq_min, x->freq_max, r.height, 0);
					break;
					/*
					  case 2:
					  ph = val;
					  break;
					*/
				}
				x->selected = te_insertPoint(x, sc, x->currentFunction);
				/*
				  x->selected->phase = ph;
				  x->selected->alpha = a;
				  x->selected->beta = b;
				  x->selected->error_alpha = ae;
				  x->selected->error_beta = be;
				*/
			}
			if(p->d_phase < p->a_phase){
				p->d_phase += 1;
			}
		}
		break;
	}
	jbox_redraw((t_jbox *)x);
	te_dumpCellblock(x);
}

void te_float(t_te *x, double f){
	if(f < 0.) f = 0.;
	if(f > 1.) f = 1.;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	double screenx = f * r.width;
	t_atom out[3]; // function number, x, y
	atom_setfloat(&(out[1]), f);
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		t_point *left = NULL, *right = NULL;
		te_find_btn(p, screenx, &left, &right);
		//return;
		if(!left || !right){
			atom_setfloat(&(out[2]), 0.);
			//outlet_list(x->out_main, NULL, 3, out);
		}else{
			double m = (right->screen_coords.y - left->screen_coords.y) / (right->screen_coords.x - left->screen_coords.x);
			double b = left->screen_coords.y - (m * left->screen_coords.x);
			double y = (m * screenx) + b;
			atom_setfloat(&(out[2]), fabs(y - r.height) / r.height);
			//outlet_list(x->out_main, NULL, 3, out);
		}
	}
}

void te_find_btn(t_point *function, double x, t_point **left, t_point **right){
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

void te_reorderPoint(t_point *p){
	int i = 0;
	while(p->prev){
		if(p->screen_coords.x < p->prev->screen_coords.x){
			post("first, before %p %p %p", p, p->prev, p->next);
			te_swapPoints(p->prev, p);
			post("first, after %p %p %p", p, p->prev, p->next);
			break;
		}else{
			break;
		}
	}
	i = 0;
	while(p->next){
		if(p->screen_coords.x > p->next->screen_coords.x){
			post("second, before %p %p %p", p, p->prev, p->next);
			te_swapPoints(p, p->next);
			post("second, after %p %p %p", p, p->prev, p->next);
			break;
		}else{
			break;
		}
	}
}

void te_initReorderedPoint(t_te *x, t_point *p){
	while(p->prev){
		p = p->prev;
	}
	x->functions[x->currentFunction] = p;
	if(p->next){
		if(p->aux_points[0] == 0 && p->aux_points[1] == 0){
			p->aux_points[0] = x->error_offset;
			p->aux_points[1] = x->error_span + x->error_offset;
		}
	}else{
		p->aux_points[0] = p->aux_points[1] = 0.;
	}

}

void te_swapPoints(t_point *p1, t_point *p2){
	post("%p %p %p %p %p %p", p1, p1->prev, p1->next, p2, p2->prev, p2->next);
	//return;

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

t_point *te_select(t_te *x, t_pt p){
	double min = 1000000000.;
	t_point *min_ptr = NULL;
	t_point *ptr = x->functions[x->currentFunction];
	double xdif, ydif;
	while(ptr){
		if((xdif = fabs(p.x - ptr->screen_coords.x)) < POINT_WIDTH / 2. && (ydif = fabs(p.y - ptr->screen_coords.y)) < POINT_WIDTH / 2.){
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
	t_point *ptr = x->functions[x->currentFunction];
	while(ptr){
		if(ptr->next){
			if(p.x >= ptr->screen_coords.x && p.x < ptr->next->screen_coords.x){
				break;
			}
		}
		ptr = ptr->next;
	}

	if(ptr){
		if(ptr->next){
			t_rect r;
			jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
			t_plan plan;
			te_makePlan(x, te_scale(ptr->screen_coords.x + 1, 0, r.width, x->time_min, x->time_max), x->currentFunction, &plan);
			double correctedTempo = te_computeCorrectedTempo(te_scale(ptr->aux_points[0], 0., 1., plan.startTime, plan.endTime), &plan);
			double y_sc = te_scale(correctedTempo, x->freq_min, x->freq_max, r.height, 0.);
			if(fabs(p.x - te_scale(ptr->aux_points[0], 0., 1., ptr->screen_coords.x, ptr->next->screen_coords.x)) <= CONTROL_POINT_WIDTH / 2. && p.y >= y_sc){
				ptr->whichPoint = 1;
				return ptr;
			}

			correctedTempo = te_computeCorrectedTempo(te_scale(ptr->aux_points[1], 0., 1., plan.startTime, plan.endTime), &plan);
			y_sc = te_scale(correctedTempo, x->freq_min, x->freq_max, r.height, 0);
			if(fabs(p.x - te_scale(ptr->aux_points[1], 0., 1., ptr->screen_coords.x, ptr->next->screen_coords.x)) <= CONTROL_POINT_WIDTH / 2 && p.y >= y_sc){
				ptr->whichPoint = 2;
				return ptr;
			}
		}
	}
	return NULL;
}

t_max_err te_notify(t_te *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	//t_symbol *attrname; 
 	if (msg == gensym("attr_modified")){ 
 		//attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
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
 	//post("0x%X", modifiers); 
 	t_rect r; 
	//t_pt norm_coords;
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	//te_getNormCoords(r, pt, &norm_coords);
	x->last_mouse = pt;
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		//post("inserting %f %f", pt.x, pt.y);
		if(x->selected = te_select(x, pt)){
			break;
		}else if(x->selected = te_selectControlPoint(x, pt)){
			break;
		}else{
			x->selected = te_insertPoint(x, pt, x->currentFunction);
			x->selected->d_freq_sc = pt.y;
			x->selected->a_phase = x->selected->d_phase = 0.;
			x->selected->alpha = x->selected->beta = 1.;
			x->selected->error_alpha = x->selected->error_beta = 2.;
			x->selected->whichPoint = 0;
			x->selected->aux_points[0] = x->error_offset;
			x->selected->aux_points[1] = x->error_span + x->error_offset;
			//post("%f %f", x->selected->aux_points[0], x->selected->aux_points[1]);
		}
		break;
	case 0x12:
		// shift.  
		if(x->selected = te_select(x, pt)){
			te_removePoint(x, x->selected, x->currentFunction);
			x->selected = NULL;
			break;
		}
		break;
	case 0x11:
		x->selected = te_select(x, pt);
		break;
	default:
		x->selected = NULL;
	}
	te_outputSelection(x);
	jbox_redraw((t_jbox *)x);
	te_dumpCellblock(x);
}

void te_mousedrag(t_te *x, t_object *patcherview, t_pt pt, long modifiers){
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
	if(modifiers == 0x13){
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
		//te_freq_minmax(x, min.y, max.y);

		x->last_mouse = pt;
	}else{
		if(x->selected){
			if(x->selected->whichPoint && x->selected->next){
				x->selected->aux_points[x->selected->whichPoint - 1] = te_scale(pt.x, x->selected->screen_coords.x, x->selected->next->screen_coords.x, 0., 1.);
				x->selected->aux_points[x->selected->whichPoint - 1] = te_clip(x->selected->aux_points[x->selected->whichPoint - 1], 0., 1.);

				if(x->selected->aux_points[0] > x->selected->aux_points[1]){
					x->selected->aux_points[0] = x->selected->aux_points[1];
				}
			}else{
				if(x->selected->screen_coords.y == x->selected->d_freq_sc && modifiers ^ 0x11){
					x->selected->d_freq_sc = pt.y;
				}
				x->selected->screen_coords = pt;
				te_reorderPoint(x->selected);
				te_initReorderedPoint(x, x->selected);
			}
		}
	}

	te_outputSelection(x);
	jbox_redraw((t_jbox *)x);
	te_dumpCellblock(x);
}

void te_mouseup(t_te *x, t_object *patcherview, t_pt pt, long modifiers){
}

void te_outputSelection(t_te *x){
	if(!(x->selected)){
		return;
	}
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_pt norm_coords;
	t_atom out[3];
	te_getNormCoords(r, x->selected->screen_coords, &norm_coords);
	atom_setlong(&(out[0]), x->currentFunction);
	atom_setfloat(&(out[1]), norm_coords.x);
	atom_setfloat(&(out[2]), norm_coords.y);
	//outlet_list(x->out_sel, NULL, 3, out);
}

void te_addFunction(t_te *x){
	critical_enter(x->lock);
	if(x->numFunctions + 1 > MAX_NUM_FUNCTIONS){
		error("te: maximum number of functions: %d", MAX_NUM_FUNCTIONS);
		return;
	}
	x->numFunctions++;
	critical_exit(x->lock);
	te_setFunction(x, x->numFunctions - 1);
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
	critical_enter(x->lock);
	x->currentFunction = f;
	x->selected = NULL;
	critical_exit(x->lock);
	te_dumpCellblock(x);
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

void te_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = fabs(r.height - screen_coords.y) / r.height;
}

void te_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords){
	screen_coords->x = norm_coords.x * r.width;
	screen_coords->y = fabs((norm_coords.y * r.height) - r.height);
}

t_point *te_insertPoint(t_te *x, t_pt screen_coords, int functionNum){
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
		p->aux_points[0] = x->error_offset;
		p->aux_points[1] = x->error_span + x->error_offset;
	}else{
		int i = 1;
		t_point *current, *next;
		current = (*function);
		next = current->next;
		while(next){
			if(p->screen_coords.x >= current->screen_coords.x && p->screen_coords.x <= next->screen_coords.x){
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
	critical_exit(x->lock);
	return p;
}

void te_removePoint(t_te *x, t_point *point, int functionNum){
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
	critical_exit(x->lock);
}

void te_dump(t_te *x){
	int i;
	t_atom out[9];
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		while(p){
			atom_setfloat(&(out[0]), te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max));
			atom_setfloat(&(out[1]), te_scale(p->screen_coords.y, r.height, 0, x->freq_min, x->freq_max));
			atom_setfloat(&(out[2]), te_scale(p->d_freq_sc, r.height, 0, x->freq_min, x->freq_max));
			atom_setfloat(&(out[3]), p->a_phase);
			atom_setfloat(&(out[4]), p->d_phase);
			atom_setfloat(&(out[5]), p->alpha);
			atom_setfloat(&(out[6]), p->beta);
			atom_setfloat(&(out[7]), p->error_alpha);
			atom_setfloat(&(out[8]), p->error_beta);
			outlet_anything(x->out_info, _sym_dump, 9, out);
			p = p->next;
		}
	}
	atom_setsym(&(out[0]), _sym_done);
	outlet_anything(x->out_info, _sym_dump, 1, out);
}

void te_dumpBeats(t_te *x){
	int function;
	double t;
	t_atom out[4];
	critical_enter(x->lock);
	for(function = 0; function < x->numFunctions; function++){
		if(x->functions[function] == NULL){
			continue;
		}
		t_plan plan;
		te_makePlan(x, x->time_min, function, &plan);
		double prev_phase = .9999;
		double p, wp;
		for(t = x->time_min; t <= x->time_max; t += (1. / 500.)){
			if(!te_isPlanValid(x, t, &plan, function)){
				te_makePlan(x, t, function, &plan);
			}
			p = te_computeCorrectedPhase(t, &plan);
			wp = p - floor(p);

			if(wp < prev_phase){
				atom_setlong(&(out[0]), function);
				atom_setfloat(&(out[1]), t);
				atom_setfloat(&(out[2]), te_computeTempo(t, &plan));
				atom_setfloat(&(out[3]), wp);
				critical_exit(x->lock);
				outlet_list(x->out_info, NULL, 4, out);
				critical_enter(x->lock);
			}
			prev_phase = wp;
		}
	}
	critical_exit(x->lock);
}

void te_dumpCellblock(t_te *x){
	t_point *p = x->functions[x->currentFunction];
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	t_plan plan;
	t_atom out[4];
	atom_setsym(&(out[0]), _sym_clear);
	atom_setsym(&(out[1]), gensym("all"));
	outlet_anything(x->out_info, ps_cellblock, 2, out);
	atom_setsym(&(out[0]), _sym_set);

	// output col header
	int c = 0;
	atom_setlong(&(out[2]), 0);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_pointNum);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_time);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_aFreq);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_dFreq);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_aPhase);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_dPhase);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_alpha);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_beta);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_errorAlpha);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_errorBeta);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	atom_setlong(&(out[1]), c++);
	atom_setsym(&(out[3]), ps_error);
	outlet_anything(x->out_info, ps_cellblock, 4, out);

	int i = 1; // first row is the header
	while(p){
		te_makePlan(x, te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max), x->currentFunction, &plan);
		c = 0;
		atom_setlong(&(out[2]), i);

		// point number
		atom_setlong(&(out[1]), c++);
		atom_setlong(&(out[3]), i);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// time
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max));
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// freq
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), te_scale(p->screen_coords.y, r.height, 0, x->freq_min, x->freq_max));
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// departure freq
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), te_scale(p->d_freq_sc, r.height, 0, x->freq_min, x->freq_max));
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// arrival phase
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->a_phase);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// departure phase
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->d_phase);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// alpha
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->alpha);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// beta
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->beta);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// error_alpha
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->error_alpha);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// error_beta
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), p->error_beta);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		// error
		atom_setlong(&(out[1]), c++);
		atom_setfloat(&(out[3]), plan.phaseError);
		outlet_anything(x->out_info, ps_cellblock, 4, out);

		p = p->next;
		i++;
	}
}

void te_time_min(t_te *x, double f){
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.x = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
			p->screen_coords.x = te_scale(p->screen_coords.x, f, x->time_max, 0, r.width);
			p = p->next;
		}
	}
	x->time_min = f;
	jbox_redraw((t_jbox *)x);
}

void te_time_max(t_te *x, double f){
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.x = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
			p->screen_coords.x = te_scale(p->screen_coords.x, x->time_min, f, 0, r.width);
			p = p->next;
		}
	}
	x->time_max = f;
	jbox_redraw((t_jbox *)x);
}

void te_time_minmax(t_te *x, double min, double max){
	double mmin = min;
	double mmax = max;
	if(min > max){
		mmin = max;
		mmax = min;
	}
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.x = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
			p->screen_coords.x = te_scale(p->screen_coords.x, mmin, mmax, 0, r.width);
			p = p->next;
		}
	}
	x->time_min = mmin;
	x->time_max = mmax;
	jbox_redraw((t_jbox *)x);
}

void te_freq_min(t_te *x, double f){
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.y = te_scale(p->screen_coords.y, r.height, 0, x->freq_min, x->freq_max);
			p->screen_coords.y = te_scale(p->screen_coords.y, f, x->freq_max, r.height, 0);
			p = p->next;
		}
	}
	x->freq_min = f;
	jbox_redraw((t_jbox *)x);
}

void te_freq_max(t_te *x, double f){
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.y = te_scale(p->screen_coords.y, r.height, 0, x->freq_min, x->freq_max);
			p->screen_coords.y = te_scale(p->screen_coords.y, x->freq_min, f, r.height, 0);
			p = p->next;
		}
	}

	x->freq_max = f;
	jbox_redraw((t_jbox *)x);
}

void te_freq_minmax(t_te *x, double min, double max){
	int i;
	t_point *p;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		p = x->functions[i];
		while(p){
			p->screen_coords.y = te_scale(p->screen_coords.y, r.height, 0, x->freq_min, x->freq_max);
			p->screen_coords.y = te_scale(p->screen_coords.y, min, max, r.height, 0);
			p->d_freq_sc = te_scale(p->d_freq_sc, r.height, 0, x->freq_min, x->freq_max);
			p->d_freq_sc = te_scale(p->d_freq_sc, min, max, r.height, 0);
			p = p->next;
		}
	}

	x->freq_min = min;
	x->freq_max = max;
	jbox_redraw((t_jbox *)x);
}

void te_postPlan(t_plan *plan){
	post("start time = %f, end time = %f", plan->startTime, plan->endTime);
	post("start tempo = %f, end tempo = %f", plan->startFreq, plan->endFreq);
	post("start phase = %f, end phase = %f", plan->startPhase, plan->endPhase);
	post("phase error = %f %f", plan->phaseError, plan->phaseError_start);
	//post("segment duration = %f seconds", plan->segmentDuration_sec);
	post("correction starts at %f and ends at %f (%f)", plan->correctionStart, plan->correctionEnd, (plan->correctionEnd - plan->correctionStart));
	//post("m = %f, b = %f", plan->m, plan->b);
	post("state = %d", plan->state);
}

void te_clear(t_te *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		te_doClearFunction(x, i);
	}
	x->currentFunction = 0;
	x->numFunctions = 1;
	te_dumpCellblock(x);
	jbox_redraw((t_jbox *)x);
}

void te_clearFunction(t_te *x, int f){
	te_doClearFunction(x, f);
	te_dumpCellblock(x);
	jbox_redraw((t_jbox *)x);
}

void te_clearCurrent(t_te *x){
	te_doClearFunction(x, x->currentFunction);
	te_dumpCellblock(x);
	jbox_redraw((t_jbox *)x);
}

void te_doClearFunction(t_te *x, int f){
	critical_enter(x->lock);
	t_point *p = x->functions[x->currentFunction];
	t_point *next = p->next;
	while(p){
		if(p){
			next = p->next;
			free(p);
			p = next;
		}
	}
	x->functions[f] = NULL;
	critical_exit(x->lock);
}

void te_assist(t_te *x, void *b, long m, long a, char *s){ 
 	if (m == ASSIST_OUTLET){ 
 	}else{ 
 		switch (a) {	 
 		case 0: 
 			break; 
 		} 
 	} 
} 

void te_free(t_te *x){ 
	dsp_freejbox((t_pxjbox *)x);
	jbox_free((t_jbox *)x);

	object_free(x->proxy);

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
} 

t_symbol *te_mangleName(t_symbol *name, int i, int fnum){
	if(!name){
		return NULL;
	}
	char buf[256];
	sprintf(buf, "tempo_editor_%s_%d_%d", name->s_name, i, fnum);
	return gensym(buf);
}

void te_dsp(t_te *x, t_signal **sp, short *count){
	if(count[0]){
		dsp_add(te_perform, 6, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
	}
}

void te_errorHandler(const char * reason, const char * file, int line, int gsl_errno){

}

int main(void){ 
 	t_class *c = class_new("tempo_editor", (method)te_new, (method)te_free, sizeof(t_te), 0L, A_GIMME, 0); 
	class_dspinitjbox(c);

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

	class_addmethod(c, (method)te_dsp, "dsp", A_CANT, 0);
 	class_addmethod(c, (method)te_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)te_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)te_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)te_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)te_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)te_mouseup, "mouseup", A_CANT, 0); 
 	class_addmethod(c, (method)te_addFunction, "addFunction", 0); 
 	class_addmethod(c, (method)te_setFunction, "setFunction", A_LONG, 0); 
	class_addmethod(c, (method)te_editSel, "editSelection", A_FLOAT, A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)te_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)te_clear, "clear", 0);
	class_addmethod(c, (method)te_clearCurrent, "clear_current", 0);
	class_addmethod(c, (method)te_clearFunction, "clear_function", A_LONG, 0);
	class_addmethod(c, (method)te_dump, "dump", 0);
	class_addmethod(c, (method)te_time_min, "time_min", A_FLOAT, 0);
	class_addmethod(c, (method)te_time_max, "time_max", A_FLOAT, 0);
	class_addmethod(c, (method)te_time_minmax, "time_minmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_min, "freq_min", A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_max, "freq_max", A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_minmax, "freq_minmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_dumpBeats, "dump_beats", 0);

	//class_addmethod(c, (method)te_read, "read", A_GIMME, 0);
	//class_addmethod(c, (method)te_write, "write", A_GIMME, 0);
	//class_addmethod(c, (method)myobject_write, "test", A_DEFSYM, 0);

	CLASS_ATTR_LONG(c, "mode", 0, t_te, mode);
	CLASS_ATTR_SAVE(c, "mode", 0);
	CLASS_ATTR_SYM(c, "name", 0, t_te, name);
	CLASS_ATTR_SAVE(c, "name", 0);
	CLASS_ATTR_LONG(c, "show_tempo_correction", 0, t_te, show_tempo_correction);
	CLASS_ATTR_SAVE(c, "show_tempo_correction", 0);
	CLASS_ATTR_LONG(c, "show_beat_correction", 0, t_te, show_beat_correction);
	CLASS_ATTR_SAVE(c, "show_beat_correction", 0);
	CLASS_ATTR_LONG(c, "show_x_axis", 0, t_te, show_x_axis);
	CLASS_ATTR_SAVE(c, "show_x_axis", 0);
	CLASS_ATTR_LONG(c, "show_y_axis", 0, t_te, show_y_axis);
	CLASS_ATTR_SAVE(c, "show_y_axis", 0);

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

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

 	class_register(CLASS_BOX, c); 
 	te_class = c; 

	common_symbols_init();
	ps_cellblock = gensym("cellblock");
	ps_pointNum = gensym("Point #");
	ps_time = gensym("Time");
	ps_dFreq = gensym("D Tempo");
	ps_aFreq = gensym("A Tempo");
	ps_dPhase = gensym("D Phase");
	ps_aPhase = gensym("A Phase");
	ps_alpha = gensym("Alpha");
	ps_beta = gensym("Beta");
	ps_errorAlpha = gensym("E Alpha");
	ps_errorBeta = gensym("E Beta");
	ps_error = gensym("Error");

 	version(0); 
	
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

		dsp_setupjbox((t_pxjbox *)x, 1);

		x->proxy = proxy_new((t_object *)x, 1, &(x->in));

		x->out_info = outlet_new((t_object *)x, NULL);
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 

		x->time_min = 0.;
		x->time_max = 10.;
		x->freq_min = 0.;
		x->freq_max = 10.;
		x->mode = 0;

 		x->functions = (t_point **)calloc(MAX_NUM_FUNCTIONS, sizeof(t_point *));
		x->last_y = (float *)calloc(MAX_NUM_FUNCTIONS, sizeof(float));
 		x->numFunctions = 1; 
 		x->currentFunction = 0; 
		critical_new(&(x->lock));

		x->ptrs = (t_float **)malloc((MAX_NUM_FUNCTIONS * 3) * sizeof(t_float *));
		int i;
		for(i = 0; i < MAX_NUM_FUNCTIONS * 3; i++){
			x->ptrs[i] = (t_float *)malloc(2048 * sizeof(t_float));
		}

		// this is really fucking important.  if there's an error and the gsl's 
		// default handler gets called, it aborts the program!
		gsl_set_error_handler(te_errorHandler);  

		x->name = NULL;

		x->error_span = DEFAULT_ERROR_SPAN;
		x->error_offset = DEFAULT_ERROR_OFFSET;

		x->show_x_axis = x->show_y_axis = 1;

 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 

		x->box.z_misc = Z_PUT_FIRST;

 		return x; 
 	} 
 	return NULL; 
} 
