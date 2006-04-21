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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: harmonics~ 
AUTHORS: Adrian Freed
DESCRIPTION: MSP harmonic oscillator Bank
COPYRIGHT_YEARS: 1996,97,98,99,2000,2001,2002,2003,2004,2005,2006
SVN_REVISION: $LastChangedRevision: 406$
VERSION 1.1: Adrian Freed - NB: still working on amplutide normalisation of thw wave outputs
VERSION 1.2: Doesn't expire, uses new versioning system
VERSION 1.3: Implements "tellmeeverything"
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/



/*
	harmonics~.c
	
	©1996,1997,1998,1999,2000,01,02,03 UC Regents, All Rights Reserved. 

	
*/


	/* harmonic oscillator bank */
	/* sums a bunch of sinusoidal oscillators which are frequency locked to integer harmonics: great for glottal simulations, periodic pulse trains, etc */
	/* a list of floats sets the amplitudes (and the length of the list sets the number harmonic partials) */
	/* a single float sets the frequency of the first partial */
	/* TODO: method to set the first harmonic amplitude, and a method to set the amplitude roll off in db/Octave */
	/* TODO: more efficient implementation that locks the phase by using a single phasor */
	
	
/*
	Left inlet float = ffreq;  list = freq + amp&*
	Right inlet list is just amplitudes

*/	
	
/* Full set of features:

	Just first (or nth or nth through mth) amplitude (without changing # oscillators)
	Spectral slope (and corner) to scale amplitudes  (put in outer object like res-transform)
	
		
*/
#include "version.h"

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#include "noise-table.h"
#undef PI	
#define PI 3.14159265358979323f
#define MAXOSCILLATORS 1024

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
	void *proxy;
	long which_inlet;
	
	oscdesc base[MAXOSCILLATORS];
	long next_phase_inc;
	long phase_inc;			/* frequency */
	int nosc; 
	int nyqmaxosc;
	int cleared;
	float  pk;
	float noisiness;
	float next_noisiness;
	float samplerate;
	float sampleinterval;
	int is_bwe;		// Boolean for whether this object is bandwidth-enhanced
	float *noisep;  // Points into the global noise table
	
	// For logging
	int numTimesPerformCalled;
	int numTimesParamsChangedDuringPerform;
} oscbank;
typedef oscbank t_sinusoids;

static void ResetInterruptStats(t_sinusoids *x);
static void ReportInterruptStats(t_sinusoids *x);
t_int *sinusoids2_perform(t_int *w);
static void clear(t_sinusoids *x);
static void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect);
static void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv);
static void sinusoids_clear(t_sinusoids *x);
static void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s);
static void *sinusoids_new(t_symbol *s, short argc, t_atom *argv);
static void frequency_float(t_sinusoids *x, double ff);
void harmonics_free(t_sinusoids *x);
void version(t_sinusoids *x);
static void SineFunction(int n, float *stab, int stride, float from, float to);
static void Makeoscsinetable();
void tellmeeverything(t_sinusoids *x);



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
	long pi_fundamental, pi_nextfundamental;
	static float *local_noisep; 
	float na=sqrtf(1.0f - x->noisiness), nb=sqrtf(1.0f - x->next_noisiness);
		float nna=sqrtf(2.0f * x->noisiness), nnb=sqrtf(2.0f * x->next_noisiness);


	/* Store local copy of F0 outside the loop in case the user changes it in the middle of DSP. */
	pi_fundamental = x->phase_inc;
	pi_nextfundamental = x->next_phase_inc;

//	if(op->b_obj.z_disabled)
//		goto out;
	rate = 1.0f/n;

	for(j=0;j<n;++j)
		out[j] = 0.0f;

		local_noisep = x->noisep;

	if(nna==0.0f && nnb==0.0f)
	for(i=0;i<nosc && i<x->nosc&&i<x->nyqmaxosc;++i)   // In case user changes nosc, we synthesize the smaller number of oscillators.
	{
		register float a = o->amplitude;
		register long pi = pi_fundamental * (i+1);   // This partial's freq is proportional to harmonic number
		register ulong pc = o->phase_current;
		register long pstep = ((x->next_phase_inc * (i+1)) - pi) *rate ;		// Also prop. to harmonic #
		register float astep = (o->next_amplitude - o->amplitude)*rate;
	
	
	//		register ulong pa  = o->phaseadd;
//		register  long phaseadd_inc = (o->next_phaseadd - o->phaseadd)*rate;

		
		for(j=0;j<n;++j)
		{

		out[j] +=  a  * 
					   *((float *)(st + ((pc >> (32-TPOW-LOGBASE2OFTABLEELEMENT))
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
		else
	for(i=0;i<nosc && i<x->nosc&&i<x->nyqmaxosc;++i)   // In case user changes nosc, we synthesize the smaller number of oscillators.
	{
		register float a = o->amplitude;
		register long pi = pi_fundamental * (i+1);   // This partial's freq is proportional to harmonic number
		register ulong pc = o->phase_current;
		register long pstep = ((x->next_phase_inc * (i+1)) - pi) *rate ;		// Also prop. to harmonic #
//		register float astep = (o->next_amplitude - o->amplitude)*rate;
		register float carrier_amp, carrier_amp_inc;
		register float mod_amp, mod_amp_inc;
		float carrier_amp_final, mod_amp_final;

	
			// Make sure we're not going to run out of noise:
		if ((local_noisep + n) >= &(NoiseTable[NTABSZ])) {
			local_noisep = &(NoiseTable[0]);
			// Could start at a random location within the noise table...
		}	
		// These formulae are from Loris / Kelly Fitz:
        //      carrier amp: sqrt( 1. - noisiness ) * amp
        //      modulation index: sqrt( 2. * noisiness ) * amp
		
		carrier_amp = na * o->amplitude;
		carrier_amp_inc = (nb * o->next_amplitude - carrier_amp) *rate;
	mod_amp =  nna   * o->amplitude;
		mod_amp_inc = ( nnb * o->next_amplitude -mod_amp) * rate;
//		register ulong pa  = o->phaseadd;
//		register  long phaseadd_inc = (o->next_phaseadd - o->phaseadd)*rate;

		// Make sure we're not going to run out of noise:
		if ((local_noisep + n) >= &(NoiseTable[NTABSZ])) {
			local_noisep = &(NoiseTable[0]);
			// Could start at a random location within the noise table...
		}

		
		for(j=0;j<n;++j)
		{

		float a = carrier_amp + mod_amp * (*local_noisep++);
		out[j] +=  a  * 
					   *((float *)(st + ((pc >> (32-TPOW-LOGBASE2OFTABLEELEMENT))
					     & ((STABSZ-1)*sizeof(*Sinetab)))));
			pc +=  pi;
			pi += pstep;
					carrier_amp += carrier_amp_inc;
			mod_amp += mod_amp_inc;
	
			//a += astep;
//			out[j] +=  a  * Sinetab[pc%STABSZ];
//			pa +=  phaseadd_inc;
		}
		o->amplitude = o->next_amplitude;
		o->phase_current = pc;
		++o;
	}
	x->noisiness = x->next_noisiness;
	x->noisep = local_noisep;
	x->phase_inc = x->next_phase_inc;	

out:
	if(x->cleared)
	{
		x->nosc = 0;
		x->cleared = 0;
	}
	
/*	++(x->numTimesPerformCalled);
	if (pi_nextfundamental != x->x->next_phase_inc) {
		++(x->numTimesParamsChangedDuringPerform);
	} */
	
	return (w+4);
}

static void clear(t_sinusoids *x) {
	
	oscdesc *p = x->base;
	int i;
	
	x->phase_inc = 0;
	x->next_phase_inc = 0;
	x->noisiness =0.0f;
	x->next_noisiness =0.0f;
	
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
	x->noisiness =0.0;
	x->next_noisiness =0.0;
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
long strcmp(const char *s1, const char *s2);
long strcmp(const char *s1, const char *s2)
{
	char c1, c2, dif;
	
	for (;;) {
		if (!(c1 = *s1++))
			return *s2 ? -1 : 0;
		if (!(c2 = *s2++))
			return 1;
		if (!(dif = (c1 - c2)))
			continue;
		if (dif < 0)
			return -1;
		else
			return 1;
	}

	return 0;
}

Boolean isthesymbol(char *name, t_atom *t);
Boolean isthesymbol(char *name, t_atom *t)
{
		if(t->a_type==A_SYM && (strcmp(t->a_w.w_sym->s_name,
						name)==0))
		{
			return true;
		}
return false;
}

static void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv)
{


			
	
	{
		int i;
		
	for (i=0; i< argc; ++i) {
		if (argv[i].a_type == A_SYM) {
			error("harmonics~: all input lists must contain only numbers");
			return;
		}
	}	

	if (x->which_inlet == 0) {
		/* Left inlet: first element of list is freq */
		frequency_float(x, atom_getfloatarg(0, argc, argv));
		--argc;
		++argv;
	}

	{
		oscdesc *fp = x->base;
		int i, nosc;
		nosc = argc;
		if (nosc>MAXOSCILLATORS) nosc = MAXOSCILLATORS;

		x->nosc = nosc;

		for(i=0;i<nosc;++i)	{
			float a = atom_getfloatarg(i,argc,argv);
			fp[i].next_amplitude = a;
		}
	}
	}
}	


static void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s)
{
	assist_string(3214,m,a,1,2,s);
}

static void frequency_float(t_sinusoids *x, double ff)
{
	float f = ff;
	
	if(f>-0.0f && ff<x->samplerate/2.0f)
	{
		x->next_phase_inc = x->pk*f;	/* frequency	*/
		x->nyqmaxosc = x->samplerate/2.0f/f;

	}
}

static void wave_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv)
{
	if(argc>=1)
	{
		if(argv[0].a_type == A_SYM)
		{
			float sq2=sqrt(2.0)/2.0f;
			oscdesc *fp = x->base;
			int j;
	    	if(isthesymbol("sawtooth", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude = sq2 * 0.5f/(j+1);
				x->nosc=512;
			}
    	if(isthesymbol("sine", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude = 0.0f;
				fp[0].next_amplitude = 1.0f;
			x->nosc=512;
			}
			if(isthesymbol("square", argv))
			{
		
				for(j=0;j<MAXOSCILLATORS;j +=2)
				{
					fp[j].next_amplitude = 2.0f/PI/(j+1);
					fp[j+1].next_amplitude = 0.0f;
					x->nosc=512;
				}
			}
			if(isthesymbol("triangle", argv))
			{
	
					for(j=0;j<MAXOSCILLATORS;j +=2)
				{
					fp[j].next_amplitude = 8.0f/PI/PI/(j+1)/(j+1);
					fp[j+1].next_amplitude = 0.0f;
					x->nosc=512;
				}
				x->nosc=512;
			}
			else
   			if(isthesymbol("pulse-train", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude= 0.005f;
				x->nosc=512;
			}
if(x->nosc!=512)
			{
				error("harmonics~: unknown wave shape");
				return ;
			}
			   argc--; argv++;
			if ( argc>1) {
				if (argv[0].a_type == A_SYM) {
					error("harmonics~: frequency expected");
					return ;
			}
			if(argc==1)
			{
				int n= atom_getintarg(0, argc, argv);
				if( n>=0 && n<MAXOSCILLATORS)
					x->nosc = n;
				--argc;
				++argv;
				}
			}
			}
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
	x->is_bwe = 0;
	x->noisep = &(NoiseTable[0]);


	x->proxy = proxy_new(x, 1L, &x->which_inlet);
	
	if(argc>=1)
	{
		if(argv[0].a_type == A_SYM)
		{
			float sq2=sqrt(2.0)/2.0f;
			oscdesc *fp = x->base;
			int j;
	    	if(isthesymbol("sawtooth", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude = sq2 * 0.5f/(j+1); // I got the scaling wrong here xxx
				x->nosc=512;
			}
    	if(isthesymbol("sine", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude = 0.0f;
				fp[0].next_amplitude = 1.0f;
			x->nosc=512;
			}
			if(isthesymbol("square", argv))
			{
		
				for(j=0;j<MAXOSCILLATORS;j +=2)
				{
				fp[j].next_amplitude = 2.0f/PI/(j+1);
						fp[j+1].next_amplitude = 0.0f;
					x->nosc=512;
				}
			}
			if(isthesymbol("triangle", argv))
			{
	
					for(j=0;j<MAXOSCILLATORS;j +=2)
				{
						fp[j].next_amplitude = 8.0f/PI/PI/(j+1)/(j+1);
										fp[j+1].next_amplitude = 0.0f;
					x->nosc=512;
				}
				x->nosc=512;
			}
			else
   			if(isthesymbol("pulse-train", argv))
			{
	
				for(j=0;j<MAXOSCILLATORS;++j)
					fp[j].next_amplitude= 0.01f; // gotta scale this right xxx
				x->nosc=512;
			}
			if(x->nosc!=512)
			{
				error("harmonics~: unknown wave shape");
				return x;
			}
			   argc--; argv++;
			if ( argc>1) {
				if (argv[0].a_type == A_SYM) {
					error("harmonics~: frequency expected");
					return x;
			}
			if(argc==1)
			{
				int n= atom_getintarg(0, argc, argv);
				if( n>=0 && n<MAXOSCILLATORS)
					x->nosc = n;
				--argc;
				++argv;
				}
			}
			
			frequency_float(x,440.0f);

		}
		else
		{
			x->which_inlet = 0;
	  	  sinusoids_list(x,s,argc,argv);
		}
	
	}
	
//post("%d %f %f %f", x->nosc, x->pk, x->sampleinterval, x->samplerate);

   return (x);
}


void harmonics_free(t_sinusoids *x) {
	freeobject(x->proxy);
	dsp_free(&(x->b_obj));
}

static void SineFunction(int n, float *stab, int stride, float from, float to)
{
	int j;
	double f = (to-from)/n;

	for(j=0;j<n;++j)
	{
		stab[j*stride] = sin(from +j*f);
//		if(j%64==0)
//			post("%f", stab[j*stride]);
	}
}

static void Makeoscsinetable()
{
		SineFunction(STABSZ, Sinetab, 1, 0.0f, 2.0f*(float)PI);
}

static void first_amplitude(t_sinusoids *x, double ff);
static void first_amplitude(t_sinusoids *x, double ff)
{
	float f = ff;
	
	if (x->nosc>0) x->base[0].next_amplitude = f;
}
static void noisiness(t_sinusoids *x, double ff);
static void noisiness(t_sinusoids *x, double ff)
{
	float f = ff;
	
	if (f>=0.0f && f<=1.0f) x->next_noisiness = f;
	else
		post("noisiness must be between 0.0 and 1.0: %d", f);
}

static void version(t_sinusoids *x) {

	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}

void tellmeeverything(t_sinusoids *x) {
	int i;
	float f0 = x->next_phase_inc / x->pk;

	post(NAME " object with %ld oscillators:", x->nosc);
	
	for (i = 0; i < x->nosc; ++i) {
		oscdesc *o = x->base+i;
		post("  freq %ld*%f = %f, amp (%.3f -> %.3f), ",
			 i+1, f0, (i+1)*f0, o->amplitude, o->next_amplitude);
	}
}


void main(void)
{
	setup((t_messlist **)&sinusoids_class, (method)sinusoids_new, (method)harmonics_free, 
		  (short)sizeof(t_sinusoids), 0L, A_GIMME, 0);
		  
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("NB: still working on amplutide normalisation of thw wave outputs");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
	post("Maximum Oscillators: %d", MAXOSCILLATORS);
#ifndef EXPIRE
    post("Never expires");
    
#endif
	Makeoscsinetable();

#ifdef EXPIRE
#define YEAR 2005
	post("Expires 2005");
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
	addmess((method)wave_list, "wave", A_GIMME, 0);		// amplitudes
	addmess((method)sinusoids_clear, "clear", 0);
	addmess((method)sinusoids_assist, "assist", A_CANT, 0);
	addmess((method)version, "version", 0);
	addfloat((method)frequency_float);							// F0
	    addmess((method)first_amplitude, 	"first-amplitude", 		A_FLOAT, 0);
	    addmess((method)noisiness, 	"noisiness", 		A_FLOAT, 0);
   	addmess((method)tellmeeverything, "tellmeeverything", 0);

	dsp_initclass();
}
