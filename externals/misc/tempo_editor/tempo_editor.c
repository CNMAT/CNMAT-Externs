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
extend to 0 and 1 function
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

#define MAX_NUM_FUNCTIONS 256 

typedef struct _point{ 
 	t_pt screen_coords; 
	double phase;
 	struct _point *next; 
 	struct _point *prev; 
} t_point; 

typedef struct _plan{ 
	double startTime, endTime;
	double startFreq, endFreq;
	double startPhase, endPhase;
	double phaseError, phaseError_start;
	double segmentDuration_sec;
	double m, b;
 	struct _plan *next; 
 	struct _plan *prev; 
} t_plan; 

typedef struct _te{ 
 	t_pxjbox box; 
 	void *out_info;
	t_critical lock;
 	t_point **functions; 
	t_point *selected;
 	int currentFunction; 
 	int numFunctions; 
	t_plan plan;
	int dirty; // set this to non-zero to indicate that a point has been added
	double time_min, time_max;
	double freq_min, freq_max;
 	t_jrgba bgcolor; 
	t_jrgba pointColor;
	t_jrgba lineColor;
	t_jrgba bgFuncColor;
	t_jrgba selectionColor;
} t_te; 

void *te_class; 

void te_paint(t_te *x, t_object *patcherview); 
t_int *te_perform(t_int *w);
void te_makePlan(t_te *x, float f, int function, t_plan *plan);
int te_isPlanValid(t_te *x, double time, t_plan *plan);
void te_computePhaseError(t_plan *plan);
double te_computeTempo(double t, t_plan *p);
double te_computePhase(double t, t_plan *p);
void te_editSel(t_te *x, double xx, double yy);
void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_float(t_te *x, double f);
void te_find_btn(t_point *function, double x, t_point **left, t_point **right);
t_point *te_select(t_te *x, t_pt p);
void te_draw_bounds(t_te *x, t_jgraphics *g, t_rect *rect); 
void te_assist(t_te *x, void *b, long m, long a, char *s); 
void te_free(t_te *x); 
t_max_err te_notify(t_te *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void te_mousedown(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mousedrag(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_mouseup(t_te *x, t_object *patcherview, t_pt pt, long modifiers); 
void te_outputSelection(t_te *x);
void te_addFunction(t_te *x); 
void te_setFunction(t_te *x, long f); 
double te_scale(double f, double min_in, double max_in, double min_out, double max_out);
void te_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords); 
void te_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords);
t_point *te_insertPoint(t_te *x, t_pt screen_coords, int functionNum);
void te_removePoint(t_te *x, t_point *point, int functionNum);
void te_time_min(t_te *x, double f);
void te_time_max(t_te *x, double f);
void te_time_minmax(t_te *x, double min, double max);
void te_freq_min(t_te *x, double f);
void te_freq_max(t_te *x, double f);
void te_freq_minmax(t_te *x, double min, double max);
void te_clear(t_te *x);
//void te_read(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_doread(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_write(t_te *x, t_symbol *msg, short argc, t_atom *argv);
//void te_dowrite(t_te *x, t_symbol *msg, short argc, t_atom *argv);
void te_dsp(t_te *x, t_signal **sp, short *count);
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

	// draw points and lines between them
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

	{
		critical_enter(x->lock);
		jgraphics_set_source_jrgba(g, &(x->lineColor));
		int i;
		double prev_t = te_scale(0, 0, rect.width, x->time_min, x->time_max);
		t_plan plan;
		te_makePlan(x, prev_t, x->currentFunction, &plan);
		double prev_phase = te_computePhase(prev_t, &plan);
		for(i = 1; i < rect.width * 100; i++){
			double idx = i / 100.;
			double t = te_scale(idx, 0, rect.width, x->time_min, x->time_max);
			te_makePlan(x, t, x->currentFunction, &plan);
			double p = te_computePhase(t, &plan);
			//post("%d %f %f %f %f", idx, prev_t, prev_phase, t, p);
			if((p - ((int)p)) < (prev_phase - ((int)prev_phase))){
				jgraphics_move_to(g, idx, rect.height);
				jgraphics_line_to(g, idx, te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
				jgraphics_stroke(g);
				//post("moveto %f %f", (float)i, rect.height);
				//post("lineto %f %f", (float)i, te_scale(te_computeTempo(t, &plan), x->freq_min, x->freq_max, rect.height, 0));
			}
			prev_t = t;
			prev_phase = p;
		}
		critical_exit(x->lock);
	}
} 

t_int *te_perform(t_int *w){
	t_te *x = (t_te *)w[1];
	int n = (int)w[2];
	float sr = 44100.;
	t_float *in = (t_float *)w[3];
	t_float *out_phase_wrapped = (t_float *)w[4];
	t_float *out_phase = (t_float *)w[5];
	t_float *out_bps = (t_float *)w[6];
	int i;
	t_plan *plan = &(x->plan);

	//post("x = %p, %d, plan = %p, %p %p %p %p", x, n, plan, in, out_phase_wrapped, out_phase, out_bps);

	int seg = 0;
	int oldseg = -1;
	double m, b;
	int outputphase = 0;
	for(i = 0; i < n; i++){
		if(te_isPlanValid(x, in[i], plan) == 0){
			te_makePlan(x, in[i], x->currentFunction, plan);
			if(0){//in[i] != 10.0){
				post("time = %f", in[i]);
				post("plan: %f %f %f %f %f %f", plan->startTime, plan->startFreq, plan->startPhase, plan->endTime, plan->endFreq, plan->endPhase);
				post("phase_error = %f, m = %f, b = %f", plan->phaseError, plan->m, plan->b);
				post("seglen = %f", plan->segmentDuration_sec);
				post("i = %d", i);
				if(i != 0){
					post("**********************%f", out_phase[i - 1]);
					outputphase = 1;
				}
			}
		}

		if(0){
			post("plan: %f %f %f %f %f %f", plan->startTime, plan->startFreq, plan->startPhase, plan->endTime, plan->endFreq, plan->endPhase);
			post("phase_error = %f, m = %f, b = %f", plan->phaseError, plan->m, plan->b);
			post("seglen = %f", plan->segmentDuration_sec);
		}
		m = plan->m;
		b = plan->b;

		out_phase[i] = ((in[i] * ((2. * b) + (m * in[i]))) / 2.) - plan->phaseError_start;
		if(in[i] < plan->startTime + plan->segmentDuration_sec){
			seg = 1;
			// segment 1--do nothing
		}else if(in[i] < plan->startTime + (2. * plan->segmentDuration_sec)){
			seg = 2;
			// segment 2--incrementally add the error
			out_phase[i] = out_phase[i] + ((plan->phaseError / (plan->segmentDuration_sec * sr)) * ((in[i] - (plan->startTime + plan->segmentDuration_sec)) * sr));
		}else{
			seg = 3;
			// segment 3--add the error
			out_phase[i] = out_phase[i] + plan->phaseError;
		}
		/*
		if(seg != oldseg){
			post("segment %d", seg);
			oldseg = seg;
		}
		*/
		out_phase_wrapped[i] = out_phase[i] - ((int)(out_phase[i]));
		out_bps[i] = m * in[i] + b;

		if(outputphase){
			post("++++++++++++++++++%f", out_phase[i]);
			outputphase = 0;
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
		plan->startTime = x->time_min;
		plan->endTime = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
		plan->startFreq = 0.;
		plan->endFreq = 0.;
		plan->startPhase = 0.;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.
		plan->endPhase = 0.; 
		te_computePhaseError(plan);
		plan->segmentDuration_sec = (plan->endTime - plan->startTime) / 3.;
		return;
	}

	while(p && next){
		if(f_sc >= p->screen_coords.x && f_sc <= next->screen_coords.x){
			plan->startTime = te_scale(p->screen_coords.x, 0., r.width, x->time_min, x->time_max);
			plan->endTime = te_scale(next->screen_coords.x, 0., r.width, x->time_min, x->time_max);
			plan->startFreq = te_scale(p->screen_coords.y, r.height, 0., x->freq_min, x->freq_max);
			plan->endFreq = te_scale(next->screen_coords.y, r.height, 0., x->freq_min, x->freq_max);
			plan->startPhase = p->phase;
			plan->endPhase = next->phase;
			te_computePhaseError(plan);
			plan->segmentDuration_sec = (plan->endTime - plan->startTime) / 3.;
			/*
			post("startTime = %f", plan->startTime);
			post("endTime = %f", plan->endTime);
			post("startFreq = %f", plan->startFreq);
			post("endFreq = %f", plan->endFreq);
			post("startPhase = %f", plan->startPhase);
			post("endPhase = %f", plan->endPhase);

			post("phaseError = %f, %f", plan->phaseError, plan->phaseError_start);
			post("segdur = %f", plan->segmentDuration_sec);
			*/
			return;
		}
		p = next;
		next = next->next;
	}

	// if we made it here, we're somewhere between the last point and time_max
	if(f_sc >= p->screen_coords.x){
		plan->startTime = te_scale(p->screen_coords.x, 0, r.width, x->time_min, x->time_max);
		plan->endTime = x->time_max;
		plan->startFreq = 0.;
		plan->endFreq = 0.;
		plan->startPhase = 0.;
		// we don't want to introduce any phase error here that would cause the 
		// algorithm to try to compensate for it.  this will effectively be a jump to phase and freq 0.
		plan->endPhase = 0.; 
		te_computePhaseError(plan);
		plan->segmentDuration_sec = (plan->endTime - plan->startTime) / 3.;
		return;

	}
}

int te_isPlanValid(t_te *x, double time, t_plan *plan){
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
	t_point *p = x->functions[x->currentFunction];
	while(p){
		if(abs(p->screen_coords.x - stsc) < .001){
			//post("%f == %f", p->screen_coords.x, stsc);
			if(p->next){
				if(abs(p->next->screen_coords.x - etsc) < .001){
					//post("there is p->next and %f == %f", p->next->screen_coords.x, etsc);
					return 1;
				}else{
					//post("there is p->next but %f != %f", p->next->screen_coords.x, etsc);
					return 0;
				}
			}else if(etsc == x->time_max){
				//post("there is no p->next but %f == %f", etsc, x->time_max);
				return 1;
			}else{
				//post("this plan is fucked");
				return 0;
			}
		}else{
			//post("%f != %f", p->screen_coords.x, stsc);
		}
		p = p->next;
	}
	return 0;
}

void te_computePhaseError(t_plan *plan){
	double m = (plan->endFreq - plan->startFreq) / (plan->endTime - plan->startTime);
	double b = plan->startFreq - (m * plan->startTime);
	//post("*****");
	plan->m = m;
	plan->b = b;
	//post("%f %f", m * plan->startTime + b, m * plan->endTime + b);
	//post("b = %f", b);
	//post("this should be 0. -> %f", ((plan->startTime * ((2. * b) + (m * plan->startTime))) / 2.));
	plan->phaseError_start = ((plan->startTime * ((2. * b) + (m * plan->startTime))) / 2.);
	plan->phaseError = ((plan->endTime * ((2. * b) + (m * plan->endTime))) / 2.) - plan->phaseError_start;
	//post("%f", plan->phaseError);
	plan->phaseError = plan->phaseError - ((int)(plan->phaseError));
	//post("%f", plan->phaseError);
	plan->phaseError = plan->endPhase - plan->phaseError;
	//post("%f", plan->phaseError);
	//post("*****");
}

double te_computeTempo(double t, t_plan *p){
	return t * p->m + p->b;
}

double te_computePhase(double t, t_plan *p){
	double error = 0;
	double sr = 44100.;
	if(t >= p->startTime && t < p->startTime + p->segmentDuration_sec){
	}else if(t >= p->startTime + p->segmentDuration_sec && t < p->startTime + (2. * p->segmentDuration_sec)){
		error = ((p->phaseError / (p->segmentDuration_sec * sr)) * ((t - (p->startTime + p->segmentDuration_sec)) * sr));
	}else if(t >= p->startTime + (2. * p->segmentDuration_sec) && t <= p->startTime + (3. * p->segmentDuration_sec)){
		error = p->phaseError;
	}

	return (((t * ((2. * p->b) + (p->m * t))) / 2.) - p->phaseError_start) + error;
}

void te_editSel(t_te *x, double xx, double yy){
	t_pt screen_coords, norm_coords = {xx, yy};
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	te_getScreenCoords(r, norm_coords, &screen_coords);
	//post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
	te_removePoint(x, x->selected, x->currentFunction);
	x->selected = te_insertPoint(x, screen_coords, x->currentFunction);
	jbox_redraw((t_jbox *)x);
}

void te_list(t_te *x, t_symbol *msg, short argc, t_atom *argv){
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
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	norm_coords.x = atom_getfloat(a++);
	norm_coords.y = atom_getfloat(a++);
	te_getScreenCoords(r, norm_coords, &screen_coords);
	//post("%f %f %f %f", norm_coords.x, norm_coords.y, screen_coords.x, screen_coords.y);
	x->selected = te_insertPoint(x, screen_coords, functionNum);
	jbox_redraw((t_jbox *)x);
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
			atom_setfloat(&(out[2]), abs(y - r.height) / r.height);
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

t_point *te_select(t_te *x, t_pt p){
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
	switch(modifiers){
	case 0x10:
		// no modifiers.  
		//post("inserting %f %f", pt.x, pt.y);
		if(x->selected = te_select(x, pt)){
			break;
		}else{
			x->selected = te_insertPoint(x, pt, x->currentFunction);
		}
		break;
	case 0x12:
		// shift.  
		if(x->selected = te_select(x, pt)){
			te_removePoint(x, x->selected, x->currentFunction);
			break;
		}
		break;
	case 0x13:
		break;
	}
	te_outputSelection(x);
	jbox_redraw((t_jbox *)x);
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

	switch(modifiers){
	case 0x10:
		te_removePoint(x, x->selected, x->currentFunction);
		x->selected = te_insertPoint(x, pt, x->currentFunction);
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	}
	te_outputSelection(x);
	jbox_redraw((t_jbox *)x);
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
	critical_enter(x->lock);
	x->currentFunction = f;
	x->selected = NULL;
	critical_exit(x->lock);
	jbox_redraw((t_jbox *)x);
}

double te_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

void te_getNormCoords(t_rect r, t_pt screen_coords, t_pt *norm_coords){
	norm_coords->x = screen_coords.x / r.width;
	norm_coords->y = abs(r.height - screen_coords.y) / r.height;
}

void te_getScreenCoords(t_rect r, t_pt norm_coords, t_pt *screen_coords){
	screen_coords->x = norm_coords.x * r.width;
	screen_coords->y = abs((norm_coords.y * r.height) - r.height);
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

void te_removePoint(t_te *x, t_point *point, int functionNum){
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

void te_dump(t_te *x){
	int i;
	t_atom out[3];
	t_pt norm_coords;
	t_rect r;
	jbox_get_patching_rect(&(x->box.z_box.b_ob), &r);
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		atom_setlong(&(out[0]), i);
		while(p){
			te_getNormCoords(r, p->screen_coords, &norm_coords);
			atom_setfloat(&(out[1]), norm_coords.x);
			atom_setfloat(&(out[2]), norm_coords.y);
			//outlet_list(x->out_dump, NULL, 3, out);
			p = p->next;
		}
	}
	//outlet_bang(x->out_bang);
}

void te_time_min(t_te *x, double f){
	x->time_min = f;
	jbox_redraw((t_jbox *)x);
}

void te_time_max(t_te *x, double f){
	x->time_max = f;
	jbox_redraw((t_jbox *)x);
}

void te_time_minmax(t_te *x, double min, double max){
	x->time_min = min;
	x->time_max = max;
	jbox_redraw((t_jbox *)x);
}

void te_freq_min(t_te *x, double f){
	x->freq_min = f;
	jbox_redraw((t_jbox *)x);
}

void te_freq_max(t_te *x, double f){
	x->freq_max = f;
	jbox_redraw((t_jbox *)x);
}

void te_freq_minmax(t_te *x, double min, double max){
	x->freq_min = min;
	x->freq_max = max;
	jbox_redraw((t_jbox *)x);
}

void te_clear(t_te *x){
	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		if(p){
			free(p);
		}
		x->functions[i] = NULL;
	}
	jbox_redraw((t_jbox *)x);
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

	int i;
	for(i = 0; i < x->numFunctions; i++){
		t_point *p = x->functions[i];
		t_point *next;
		while(p){
			next = p->next;
			free(p);
			p = next;
		}
	}
} 

void te_dsp(t_te *x, t_signal **sp, short *count){
	if(count[0]){
		dsp_add(te_perform, 6, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
	}
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
	class_addmethod(c, (method)te_editSel, "editSelection", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)te_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)te_clear, "clear", 0);
	class_addmethod(c, (method)te_dump, "dump", 0);
	class_addmethod(c, (method)te_time_min, "time_min", A_FLOAT, 0);
	class_addmethod(c, (method)te_time_max, "time_max", A_FLOAT, 0);
	class_addmethod(c, (method)te_time_minmax, "time_minmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_min, "freq_min", A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_max, "freq_max", A_FLOAT, 0);
	class_addmethod(c, (method)te_freq_minmax, "freq_minmax", A_FLOAT, A_FLOAT, 0);

	//class_addmethod(c, (method)te_read, "read", A_GIMME, 0);
	//class_addmethod(c, (method)te_write, "write", A_GIMME, 0);
	//class_addmethod(c, (method)myobject_write, "test", A_DEFSYM, 0);

 	CLASS_STICKY_ATTR(c, "category", 0, "Color"); 

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_te, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "pointColor", 0, t_te, pointColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "pointColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "pointColor", 0, "rgba", "Point Color"); 

 	CLASS_ATTR_RGBA(c, "lineColor", 0, t_te, lineColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "lineColor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "lineColor", 0, "rgba", "Line Color"); 

 	CLASS_ATTR_RGBA(c, "bgFuncColor", 0, t_te, bgFuncColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgFuncColor", 0, "0. 0. 0. 0.5"); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgFuncColor", 0, "rgba", "Background Function Color"); 

 	CLASS_ATTR_RGBA(c, "selectionColor", 0, t_te, selectionColor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectionColor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectionColor", 0, "rgba", "Selection Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

 	class_register(CLASS_BOX, c); 
 	te_class = c; 

	common_symbols_init();

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
 		x->box.z_box.b_firstin = (void *)x; 

		dsp_setupjbox((t_pxjbox *)x, 1);

		x->out_info = outlet_new((t_object *)x, NULL);
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 
 		outlet_new((t_object *)x, "signal"); 

		x->time_min = 0.;
		x->time_max = 10.;
		x->freq_min = 0.;
		x->freq_max = 10.;

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
