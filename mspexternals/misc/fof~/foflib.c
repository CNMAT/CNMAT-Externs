/*
 * File: foflib.c
 * FOF implementation based on IIR representation
 * by Gerhard Eckel and Francisco Iovino, Feb 1994
 * (c) 1994 IRCAM
 */
 
 // MSP version Dudas Hack Summer 1998
 // changed max number of fofs to 256 just to give us some growing room
 //   (originally 40 - the max that an ispw processor could handle)
 //   this definition is found in foflib.h
 // commented out fof_error() in foflib.c - 
 //   we don't want to do unnecessary printing at audio interrupt!


#include <math.h>
#include "foflib.h"


// fof_init sets up the inputs and params structure with default values
// and initializes the f0_phase

static void fof_init(t_fof *x)
{   
	x->inputs.f0 = 0;
    x->inputs.cf = DEF_CF;
    x->inputs.bw = DEF_BW;
    x->inputs.amp = DEF_AMP;
    x->inputs.tex = DEF_TEX;
    x->inputs.debatt = DEF_DEBATT;
    x->inputs.atten = DEF_ATTEN;
    x->inputs.phase = 0.;
    x->inputs.changed = 1;
    x->params.sr = DEFAULT_SRATE;
    x->params.sro2 = x->params.sr * 0.5;
    x->params.sp = 1 / x->params.sr;
    x->params.pi = 4 * atan(1.);
    x->params.twopi = x->params.pi * 2;
    x->params.piosr = x->params.pi * x->params.sp;
    x->f0_phase = 0;
}

// fof_init_list initializes the free list to contain all (maxfofs) fofs
static void fof_init_list(t_fof_list *x)
{
    t_fof_entry *p = x->fofs;
    int i;
    
    x->free.next = p;
    x->used.next = LIST_END;
    for (i = 1; i < x->maxfofs; i++)
	p[i - 1].next = &p[i];
    p[--i].next = LIST_END;
}

/* fof_update calculates some fof parameters and coefficients from the
   fof inputs, it is called when any of the parameters changed
   [2x exp(), 3x cos()] */
   
static void fof_update(t_fof_entry *entry, t_fof *x)
{
    t_fof_real sp = x->params.sp, pi = x->params.pi, bwc;
    
    x->params.cf2pi = x->inputs.cf * x->params.twopi;
    x->params.bwpi = x->inputs.bw * pi;
    x->params.piotex = pi / x->inputs.tex;
    x->params.pioatten = pi / x->inputs.atten;
    x->params.fpc = x->params.cf2pi * sp;
    x->params.bpc = exp(x->params.bwpi * sp);
    x->params.tpc = x->inputs.tex > 0 ? x->params.piotex * sp : 0;
    x->params.apc = x->inputs.atten > 0 ? x->params.pioatten * sp : 0;

    bwc = exp(- x->params.bwpi * sp);
    x->coefs_cache.ec1 = 2 * bwc * cos(x->params.fpc);
    x->coefs_cache.ec2 = - (bwc * bwc);
    x->coefs_cache.tc = 2 * cos(x->params.tpc);
    x->coefs_cache.ac = 2 * cos(x->params.apc);
    entry->coefs = x->coefs_cache;
    x->inputs.changed = 0;
}

/* fof_onset calculates the initial values for the three fof states
   based on the parameters and coefficients calculated by fof_update */
   
static void fof_onset(t_fof_entry *e, t_fof *x, long start, t_fof_real d1)
{
    t_fof_real pi = x->params.pi, sr = x->params.sr, sp = x->params.sp;
    t_fof_real dexp = d1 * x->params.cf2pi + x->inputs.phase;
    t_fof_real aexp = exp(-x->params.bwpi*d1) * x->inputs.amp * x->params.bpc;
    t_fof_real dmd = x->inputs.debatt - d1;
    
    if (x->inputs.tex >= sp) {
	t_fof_real dtex = x->params.piotex * d1;
	e->tex.count = (x->inputs.tex - d1) * sr + 1;
	e->tex.y1 = cos(pi - x->params.tpc + dtex) * 0.5;
	e->tex.y2 = cos(pi - x->params.tpc * 2 + dtex) * 0.5;
    } else
	e->tex.count = 0;
	
    e->start = start;
    e->exp.count = dmd * sr + 1;
    e->exp.y1 = sin(- x->params.fpc + dexp) * aexp;
    e->exp.y2 = sin(- x->params.fpc * 2 + dexp) * x->params.bpc * aexp;
    
    if (x->inputs.atten >= sp) {
	t_fof_real d2 = e->exp.count * sp - dmd;
	t_fof_real datt = x->params.pioatten * d2;
	e->atten.count = (x->inputs.atten - d2) * sr + 1;
	e->atten.y1 = cos(- x->params.apc + datt) * 0.5;
	e->atten.y2 = cos(- x->params.apc * 2 + datt) * 0.5;
    } else
	e->atten.count = 0;

    e->exp.count -= e->tex.count;
    if (e->exp.count < 0)
	e->exp.count = 0;
}

/* fof_trigger allocates a new fof, calls fof_update if necessary
   and starts the fof at a certain start time and start phase */
 
static void fof_trigger(t_fof *x, long start, t_fof_real delta)
{
    t_fof_entry *new = x->list.free.next;

    if (new == LIST_END) {
	// fof_error(); //in msp we dont want to waste time printing -rd
	return;
    }
    x->list.free.next = new->next;
    new->next = x->list.used.next;
    x->list.used.next = new;
    if (x->inputs.changed)
	fof_update(new, x);
    new->coefs = x->coefs_cache;
    fof_onset(new, x, start, delta);
}

/* fof_run_phasor accumulates the current phase by the incoming
   frequency signal and triggers a fof whenever the phase runs over
   (it also clears the output buffer at the same time) */

static void fof_run_phasor(t_sigfofs *x, t_sample *in, t_sample *out, long n)
{
	t_fof *xx = x->ctlp;
    t_fof_real sr = xx->params.sr;
    t_fof_real p = xx->f0_phase;
    int i = n;
    
    while (i--) {
	p += *in++;
	if (p >= sr) {
	    p -= sr;
	    fof_trigger(xx, n - i - 1, p / (*(in - 1) * sr));
	}
	*out++ = 0;
    }
    xx->f0_phase = p;
}

static void fof_run_phasor_nosig(t_sigfofs *x, t_sample in, t_sample *out, long n)
{
	t_fof *xx = x->ctlp;
    t_fof_real sr = xx->params.sr;
    t_fof_real p = xx->f0_phase;
    int i = n;
    
    while (i--) {
	p += in;
	if (p >= sr) {
	    p -= sr;
	    fof_trigger(xx, n - i - 1, p / (in * sr));
	}
	*out++ = 0;
    }
    xx->f0_phase = p;
}

/* fof_run_one runs one fof and returns 0 if the fof can be freed and 1
   if it is still needed, this is the heart of the implementation,
   it contains the calculation of the three fof parts
   (exp + tex, exp, exp + atten). */
   
static int fof_run_one(t_fof_entry *x, t_sample *out, long n)
{
    t_fof_real y, a = x->coefs.ec1, b = x->coefs.ec2;
    t_fof_real y1 = x->exp.y1, y2 = x->exp.y2;
    
    if (x->start > 0) {
	int m = MIN(n, x->start);
	n -= m;
	out += m;
	x->start -= m;
    }
    if (x->tex.count > 0) {
	t_fof_real ty, ta = x->coefs.tc, ty1 = x->tex.y1, ty2 = x->tex.y2;
	int m = MIN(n, x->tex.count);
	n -= m;
	x->tex.count -= m;
	while (m--) { /* [4x *, 3x +, 1x -] */
	    y = a * y1 + b * y2;
	    y2 = y1;
	    y1 = y;
	    ty = ta * ty1 - ty2;
	    *out++ += y * (ty + 0.5);
	    ty2 = ty1;
	    ty1 = ty;
	}
	x->tex.y1 = ty1;
	x->tex.y2 = ty2;
    }
    if (x->exp.count > 0) {
		int m = MIN(n, x->exp.count);
		n -= m;
		x->exp.count -= m;

		while (m--) { /* [2x *, 2x +] */
		    y = a * y1 + b * y2;
		    *out++ += y;
		    y2 = y1;
		    y1 = y;
		}
	}
    if (x->atten.count > 0) {
	t_fof_real ay, aa = x->coefs.ac, ay1 = x->atten.y1, ay2 = x->atten.y2;
	int m = MIN(n, x->atten.count);
	n -= m;
	x->atten.count -= m;
	while (m--) { /* [4x *, 3x +, 1x -] */
	    y = a * y1 + b * y2;
	    y2 = y1;
	    y1 = y;
	    ay = aa * ay1 - ay2;
	    ay2 = ay1;
	    ay1 = ay;
	    *out++ += y * (ay + 0.5);
	}
	x->atten.y1 = ay1;
	x->atten.y2 = ay2;
    }
    x->exp.y1 = y1;
    x->exp.y2 = y2;
    if (x->atten.count > 0 || x->exp.count > 0 || x->tex.count > 0)
	return 1;
    else
	return 0;
}

//fof_run_all runs all fofs and removes the finished fofs
// from the used list and puts them into the free list
static void fof_run_all(t_sigfofs *x, t_sample *out, long n)
{
	t_fof *xx = x->ctlp;
    t_fof_entry *prev = (t_fof_entry*)&(xx->list.used);
    t_fof_entry *curr = prev->next;

    while (curr != LIST_END) {
	if (fof_run_one(curr, out, n) == 0) {
	    prev->next = curr->next;
	    curr->next = xx->list.free.next;
	    xx->list.free.next = curr;
	    curr = prev->next;
	} else {
	    prev = curr;
	    curr = curr->next;
	}
    }
}

// fof_run calculates a block of the fof's output
//static void fof_run(t_fof *x, t_sample *in, t_sample *out, long n)
static t_int *fof_perform(t_int *w)
{
	t_sigfofs *x = (t_sigfofs *)(w[1]);

	t_float *in = (t_float *)(w[2]);
	t_float *out = (t_float *)(w[3]);
	int n = (int)(w[4]);
	
	if (x->x_obj.z_disabled)
		goto out;
		
    fof_run_phasor(x, in, out, n);
    fof_run_all(x, out, n);
    
out:    
    return (w+5);
}

static t_int *fof_perform_nosig(t_int *w)
{
	t_sigfofs *x = (t_sigfofs *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	
	t_float in = x->ctlp->inputs.f0;
    
	if (x->x_obj.z_disabled)
		goto out;
		
    fof_run_phasor_nosig(x, in, out, n);
    fof_run_all(x, out, n);
    
out:   
    return (w+4);
}