/*
Copyright (c) 2003.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Adrian Freed, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
*/



/*
	harmonics~.c
	
	MSP harmonic oscillator Bank
	
	©1996,1997,1998,1999,2000,01,02,03 UC Regents, All Rights Reserved. 

	
*/


	/* harmonic oscillator bank */
	/* sums a bunch of sinusoidal oscillators which are frequency locked to integer harmonics: great for glottal simulations, periodic pulse trains, etc */
	/* a list of floats sets the amplitudes (and the length of the list sets the number harmonic partials) */
	/* a single float sets the frequency of the first partial */
	/* TODO: method to set the first harmonic amplitude, and a method to set the amplitude roll off in db/Octave */
	/* TODO: more efficient implementation that locks the phase by using a single phasor */
	
	
/* Full set of features:

	Just first (or nth or nth through mth) amplitude (without changing # oscillators)
	Spectral slope (and corner) to scale amplitudes  (put in outer object like res-transform)
	
		
*/

#include "ext.h"
#include "z_dsp.h"
#include <math.h>
#undef PI	
#define PI 3.14159265358979323f
#define MAXOSCILLATORS 256

#define TPOW 14
#define STABSZ (1l<<TPOW)
#define LOGBASE2OFTABLEELEMENT 2

void *sinusoids_class;
float Sinetab[STABSZ];

typedef  unsigned long ulong;

typedef  struct oscdesc
{
	float next_amplitude;
	float amplitude;		/* amplitude */
	ulong phase_current;
//	ulong next_phaseadd;
//	ulong phaseadd;			/* phase */
} oscdesc;


/* bank of oscillators */
typedef struct 
{
	t_pxobject b_obj;
	oscdesc base[MAXOSCILLATORS];
	long next_phase_inc;
	long phase_inc;			/* frequency */
	int nosc; 
	int cleared;
	float  pk;
	float samplerate;
	float sampleinterval;
	
	// For logging
	int numTimesPerformCalled;
	int numTimesParamsChangedDuringPerform;
} oscbank;
typedef oscbank t_sinusoids;

t_int *sinusoids2_perform(t_int *w);
static void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect);
static void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv);
static void sinusoids_clear(t_sinusoids *x);
static void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s);
static void *sinusoids_new(t_symbol *s, short argc, t_atom *argv);


static void ResetInterruptStats(t_sinusoids *x) {
	x->numTimesPerformCalled = x->numTimesParamsChangedDuringPerform = 0;
}

static void ReportInterruptStats(t_sinusoids *x) {
	post("%ld calls to perform(); parameters changed %ld times during perform().",
		 x->numTimesPerformCalled, x->numTimesParamsChangedDuringPerform);
	ResetInterruptStats(x);
}


static t_int *sinusoids2_perform(t_int *w)
{
	t_sinusoids *x = (t_sinusoids *)(w[1]);
		t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	int nosc = x->nosc;
	int i,j;
	oscdesc *o = x->base;
	const char *st = (const char *)Sinetab;
	float rate ;
	long pi_fundamental;


	/* Store local copy of F0 outside the loop in case the user changes it in the middle of DSP. */
	pi_fundamental = x->phase_inc;

//	if(op->b_obj.z_disabled)
//		goto out;
	rate = 1.0f/n;

	for(j=0;j<n;++j)
		out[j] = 0.0f;

	
	for(i=0;i<nosc && i<x->nosc;++i)   // In case user changes nosc, we synthesize the smaller number of oscillators.
	{
		register float a = o->amplitude;
		register long pi = pi_fundamental * (i+1);   // This partial's freq is proportional to harmonic number
		register ulong pc = o->phase_current;
		register long pstep = ((x->next_phase_inc * (i+1)) - pi) * rate ;		// Also prop. to harmonic #
		register float astep = (o->next_amplitude - o->amplitude)*rate;
//		register ulong pa  = o->phaseadd;
//		register  long phaseadd_inc = (o->next_phaseadd - o->phaseadd)*rate;
		
		for(j=0;j<n;++j)
		{

			out[j] +=  a  * 
					   *((float *)(st + (((pc) >> (32-TPOW-LOGBASE2OFTABLEELEMENT))
					     & ((STABSZ-1)*sizeof(*Sinetab)))));
			pc +=  pi;
			pi += pstep;
			a += astep;
//			out[j] +=  a  * Sinetab[pc%STABSZ];
//			pa +=  phaseadd_inc;
		}
		o->amplitude = o->next_amplitude;
		o->phase_current = pc;
		++o;
	}
	x->phase_inc = x->next_phase_inc;

out:
	if(x->cleared)
	{
		x->nosc = 0;
		x->cleared = 0;
	}
	
	++(x->numTimesPerformCalled);
	if (pi_fundamental != x->phase_inc) {
		++(x->numTimesParamsChangedDuringPerform);
	}
	return (w+4);
}

static void clear(t_sinusoids *x)
{

	
	oscdesc *p = x->base;
	int i;
	
	x->phase_inc = 0;
	x->next_phase_inc = 0;
	
	for(i=0;i<MAXOSCILLATORS;++i, p++)
	{
		p->next_amplitude = 0.0f;
		p->amplitude = 0.0f;
		p->phase_current = 0;
//		p->phaseadd = 0;
//		p->next_phaseadd = 0;
	}
}

static void sinusoids_clear(t_sinusoids *x)
{
	oscdesc *p = x->base;
	int i;
	for(i=0;i<MAXOSCILLATORS;++i, p++)
	{
		p->next_amplitude = 0.0f;
	}
	x->cleared = TRUE;
}


static void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect)
{
	dsp_add(sinusoids2_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

static void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv)
{
	int i;


	{
		oscdesc *fp = x->base;
		int nosc;
		nosc = argc;
		if(nosc>MAXOSCILLATORS)
			nosc = MAXOSCILLATORS;
//		
// mess with ampl
//for(i=x->nres;i<nres;++i)
//		{
//			fp[i].out1 = fp[i].out2 = 0.0f;
//		}
		x->nosc = nosc;
		for(i=0;i<nosc;++i)
		{
			float a = 	atom_getfloatarg(i,argc,argv);
			fp[i].next_amplitude = a;
//	post("%d %d %d %f %f %f", i, nosc, fp[i].next_amplitude, f, a);
		}
//		post("nosc %d x-nosc %d", nosc, x->nosc);
	}
}

static void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s)
{
	assist_string(3214,m,a,1,2,s);
}

static void frequency_float(t_sinusoids *x, double ff)
{
	float f = ff;
	
	if(f>-0.0f && ff<x->samplerate/2.0)
	{
		x->next_phase_inc = x->pk*f;	/* frequency	*/

	}
}

static void *sinusoids_new(t_symbol *s, short argc, t_atom *argv)
{
    t_sinusoids *x = (t_sinusoids *)newobject(sinusoids_class);
    dsp_setup((t_pxobject *)x,0);
    outlet_new((t_object *)x, "signal");
	x->samplerate =  sys_getsr();
	if(x->samplerate<=0.0f)
		x->samplerate = 44100.0f;
	x->sampleinterval = 1.0f/x->samplerate;
	x->pk = (STABSZ*x->sampleinterval)*(1l<<(32-TPOW)) ;
     x->nosc = 0;
     x->cleared = 0;
     clear(x);

    sinusoids_list(x,s,argc,argv);
    frequency_float(x,440.0f);
//post("%d %f %f %f", x->nosc, x->pk, x->sampleinterval, x->samplerate);

	ResetInterruptStats(x);
    return (x);
}

static void SineFunction(int n, float *stab, int stride, float from, float to);
static void SineFunction(int n, float *stab, int stride, float from, float to)
{
	int j;
	float f = (to-from)/n;

	for(j=0;j<n;++j)
	{
		stab[j*stride] = sinf(from +j*f);
//		if(j%64==0)
//			post("%f", stab[j*stride]);
	}
}

static void Makeoscsinetable();
static void Makeoscsinetable()
{
		SineFunction(STABSZ, Sinetab, 1, 0.0f, 2.0f*(float)PI);
}

#define EXPIRE

void main(void)
{
	setup((t_messlist **)&sinusoids_class, (method)sinusoids_new, (method)dsp_free, 
		  (short)sizeof(t_sinusoids), 0L, A_GIMME, 0);
	post("harmonics~ 1.0 - Adrian Freed");
	post("Copyright © 1996,1997,1998,1999,2000,01,02,03 Regents of the University of California. All Rights Reserved");
	post("Maximum Oscillators: %d", MAXOSCILLATORS);
#ifndef EXPIRE
    post("Never expires");
    
#endif
	Makeoscsinetable();

#ifdef EXPIRE
#define YEAR 2004
	post("Expires 2004");
	{
		DateTimeRec date;
		GetTime(&date);
		if(date.year>=YEAR)
		{
			post("Expired");
		}
		else

#else
{
#endif

		addmess((method)sinusoids_dsp, "dsp", A_CANT, 0);
	}
	addmess((method)sinusoids_list, "list", A_GIMME, 0);		// amplitudes
	addmess((method)sinusoids_clear, "clear", 0);
	addmess((method)sinusoids_assist, "assist", A_CANT, 0);
	addfloat((method)frequency_float);							// F0
	addmess((method)ReportInterruptStats, "report-interrupt-stats", 0);

	dsp_initclass();
	rescopy('STR#',3214);
}