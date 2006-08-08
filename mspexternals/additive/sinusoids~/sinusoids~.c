/*
Copyright (c) 1988-2005.  The Regents of the University of California (Regents).
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

	sinusoids.c
	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: sinusoids~
DESCRIPTION: Bank of (optionally bandwidth-enhanced) sinusoidal oscillators
AUTHORS: Adrian Freed
COPYRIGHT_YEARS: 1988,89,90-99,2000,01,02,03,04,05
SVN_REVISION: $LastChangedRevision$
VERSION 1.7: 030219, bandwidth-enhanced
VERSION 1.7.1: 041228, ability to toggle verbosity	
VERSION 1.7.2: 050606, merged Michael Zbyszynski's 27 April 2004 Windows changes
VERSION 1.7.3: Changed name (in version system) to have the tilde.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

#include "version.h"
#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#ifdef WIN_VERSION
#define sqrtf sqrt
#define sinf sin
#endif

#include "noise-table.h"

#undef PI	
#define PI 3.14159265358979323f
#define MAXOSCILLATORS 256

#define TPOW 14
#define STABSZ (1l<<TPOW)
#define LOGBASE2OFTABLEELEMENT 2

//#define NTABSZ 129088

void *sinusoids_class;
float Sinetab[STABSZ];

static Symbol *ps_bwe;

typedef  unsigned long ulong;

typedef  struct oscdesc
{
	float next_amplitude;
	float amplitude;		/* amplitude */
	ulong phase_current;
	long next_phase_inc;
	long phase_inc;			/* frequency */
//	ulong next_phaseadd;
//	ulong phaseadd;			/* phase */
	float noisiness;
	float next_noisiness;
} oscdesc;


/* bank of oscillators */
typedef struct 
{
	t_pxobject b_obj;
	oscdesc base[MAXOSCILLATORS];
	int nosc; 
	int next_nosc;
	float  pk;
	float samplerate;
	float sampleinterval;
	int is_bwe;		// Boolean for whether this object is bandwidth-enhanced
	float *noisep;  // Points into the global noise table
	
	int debugPrintsRemaining;
	Boolean verbose;
} oscbank;
typedef oscbank t_sinusoids;

t_int *sinusoids_perform(t_int *w);
t_int *sinusoids_bwe_perform(t_int *w);
void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect);
void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv);
void sinusoids_clear(t_sinusoids *x);
void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s);
void *sinusoids_new(t_symbol *s, short argc, t_atom *argv);
void tellmeeverything(t_sinusoids *x);
void sinusoids_verbose(t_sinusoids *x, long v);


t_int *sinusoids_perform(t_int *w) {
	t_sinusoids *op = (t_sinusoids *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	int nosc = op->nosc;
	int i,j;
	oscdesc *o = op->base;
	const char *st = (const char *)Sinetab;
	float rate = 1.0f/n;
		
	for(j=0;j<n;++j)
		out[j] = 0.0f;
	
	for(i=0; i<nosc; ++i) {
		register float a = o->amplitude;
		register long pi = o->phase_inc;
		register ulong pc = o->phase_current;
		register long pstep = (o->next_phase_inc - o->phase_inc)*rate;
		register float astep = (o->next_amplitude - o->amplitude)*rate;
//		register ulong pa  = o->phaseadd;
//		register  long phaseadd_inc = (o->next_phaseadd - o->phaseadd)*rate;
		
		for(j=0; j<n; ++j) {
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
		o->phase_inc = o->next_phase_inc;
		o->phase_current = pc;
		++o;
	}

	op->nosc = op->next_nosc;

	return (w+4);
}


t_int *sinusoids_bwe_perform(t_int *w) {
	t_sinusoids *op = (t_sinusoids *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	int nosc = op->nosc;
	int i,j;
	oscdesc *o = op->base;
	const char *st = (const char *)Sinetab;
	float rate = 1.0f/n;
	float *local_noisep; 
	
	for(j=0;j<n;++j)
		out[j] = 0.0f;

	local_noisep = op->noisep;

	for(i=0;i<nosc;++i)
	{
		register long pi = o->phase_inc;
		register ulong pc = o->phase_current;
		register long pstep = (o->next_phase_inc - o->phase_inc)*rate;
//		register ulong pa  = o->phaseadd;
//		register  long phaseadd_inc = (o->next_phaseadd - o->phaseadd)*rate;
		
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
		
		carrier_amp = sqrtf(1.0f - o->noisiness) * o->amplitude ;
		carrier_amp_final = sqrtf(1.0f - o->next_noisiness) * o->next_amplitude;
		carrier_amp_inc = (carrier_amp_final - carrier_amp) / n;
		
		mod_amp =       sqrtf(2.0f * o->noisiness) * o->amplitude ;
		mod_amp_final = sqrtf(2.0f * o->next_noisiness) * o->next_amplitude ;
		mod_amp_inc = (mod_amp_final - mod_amp) / n;
		
		// post("carrier %f to %f, mod %f to %f", carrier_amp, carrier_amp_final, mod_amp, mod_amp_final);
		
		
		
		for (j=0; j<n; ++j) {
		
			float a = (carrier_amp + (mod_amp * (*local_noisep++)));
			
			/* if (op->debugPrintsRemaining) {
				--(op->debugPrintsRemaining);
				post("a %f", a);
			} */
	
			out[j] +=  a *
				*((float *)(st + (((pc) >> (32-TPOW-LOGBASE2OFTABLEELEMENT))
				& ((STABSZ-1)*sizeof(*Sinetab)))));

			carrier_amp += carrier_amp_inc;
			mod_amp += mod_amp_inc;
			pc +=  pi;
			pi += pstep;
//			out[j] +=  a  * Sinetab[pc%STABSZ];
//			pa +=  phaseadd_inc;
		}
		
		o->amplitude = o->next_amplitude;
		o->phase_inc = o->next_phase_inc;
		o->noisiness = o->next_noisiness;
		o->phase_current = pc;
		++o;
	}

	op->noisep = local_noisep;
	op->nosc = op->next_nosc;

out:	
	return (w+4);
}


static void clear(t_sinusoids *x)
{
	oscdesc *p = x->base;
	int i;
	for(i=0;i<MAXOSCILLATORS;++i, p++)
	{
	
		p->next_phase_inc = 0;
		p->next_amplitude = 0.0f;
		p->amplitude = 0.0f;
		p->phase_inc = 0;
		p->phase_current = 0;
		p->noisiness = p->next_noisiness = 0.0;
//		p->phaseadd = 0;
//		p->next_phaseadd = 0;
	}
}
void sinusoids_clear(t_sinusoids *x)
{
	oscdesc *p = x->base;
	int i;
	for(i=0;i<MAXOSCILLATORS;++i, p++)
	{
		p->next_amplitude = 0.0f;
	}
	x->next_nosc = 0;
}


void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect)
{
	int i;
	
	
	if (sp[0]->s_n > NTABSZ) {
		error("sinusoids~: %ld-size Noise table is too small for sigvs %ld",
			   NTABSZ, sp[0]->s_n);
		return;
	}
	
	if (x->is_bwe) {
		dsp_add(sinusoids_bwe_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
	} else {
		dsp_add(sinusoids_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
	}
}

void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv) {
	int i;
	int is_bwe = x->is_bwe;
	oscdesc *fp = x->base;
	int old_nosc, nosc;


	if (is_bwe) {
		if (argc % 3 != 0) {
			post("multiple of 3 floats (Frequency, amplitude, noisiness) required");
			return;
		}
		nosc = argc/3;		
	} else {
		if (argc % 2 != 0) {
			post("multiple of 2 floats (Frequency, amplitude) required");
			return;
		}
		nosc = argc/2;
	}	
		
	if (nosc>MAXOSCILLATORS)
		nosc = MAXOSCILLATORS;

	x->next_nosc = nosc;
	old_nosc = x->nosc;
	
	if (old_nosc > nosc) {
		/* Some partials are dying. */
	
		for (i=nosc; i < old_nosc; ++i) {
			/* Death */
			fp[i].next_amplitude = 0.0f;
		}
		/* Leave x->nosc at the current value; after the dying partials reach zero amplitude,
		   then x->nosc will be set to x->next_nosc (at the end of the next perform()). */
	} else {
		/* We have the same number or more partials than before; start synthesizing the new
		   ones immediately */
		x->nosc = nosc;
	}

	for(i=0; i<nosc; ++i) {
		float f, a, b;
		
		if (is_bwe) {
			f = atom_getfloatarg(i*3,argc,argv);
			a = atom_getfloatarg(i*3+1,argc,argv);
			b = atom_getfloatarg(i*3+2,argc,argv);
		} else {
			f = atom_getfloatarg(i*2,argc,argv);
			a = atom_getfloatarg(i*2+1,argc,argv);
			b = 0.0f;
		}
		
		if((f<=0.0f) || (f>=(x->samplerate*0.5f))) {
			// MW commented these out; fade out this partial but leave freq+noisiness alone
			// fp[i].next_phase_inc = 0;
			// fp[i].amplitude = 0.0f;
			// fp[i].phase_inc = 0;
			// fp[i].phase_current = 0;
			// fp[i].next_noisiness = 0.0f;
			fp[i].next_amplitude = 0.0f;
			if (x->verbose) {
				error("sinusoids~: bad frequency %f for partial %ld (killing partial)", f, i+1);
			}
		} else {
			fp[i].next_phase_inc = x->pk*f;	/* frequency	*/
			fp[i].next_amplitude = a;		/* amplitude	*/
			fp[i].next_noisiness = b;
		}
		
		if (b < 0.0f || b > 1.0f) {
			if (x->verbose) {
				error("sinusoids~: bad noisiness %f for partial %ld (setting to 0)", f, i+1);
			}
			fp[i].next_noisiness = 0.0f;
		}
		
		if (i >= old_nosc) {
			/* Birth: fade amplitude up from zero, but keep freq+noisiness constant */
			fp[i].amplitude = 0.0f;
			fp[i].phase_inc = fp[i].next_phase_inc;
			fp[i].noisiness = fp[i].next_noisiness;
		}
		//	post("%d %d %d %f %f %f", i, nosc,fp[i].next_phase_inc, fp[i].next_amplitude, f, a);
	}
//		post("nosc %d x-nosc %d", nosc, x->nosc);
}

void sinusoids_assist(t_sinusoids *x, void *box, long msg, long arg, char *dstString) {
       if (msg == ASSIST_INLET) {
               sprintf(dstString, "%s", "(List) freq, amp, (bandwidth) tuples");
       } else if (msg = ASSIST_OUTLET) {
               sprintf(dstString, "%s", "(Signal) Oscillator bank output");
       } else {
               error("sinusoids_assist: bad msg %ld", msg);
       }       
}


void *sinusoids_new(t_symbol *s, short argc, t_atom *argv) {
    t_sinusoids *x = (t_sinusoids *)newobject(sinusoids_class);
    int i;
    
    dsp_setup((t_pxobject *)x,0);
    outlet_new((t_object *)x, "signal");
    
	x->samplerate =  sys_getsr();
	if(x->samplerate<=0.0f)
		x->samplerate = 44100.0f;
	x->sampleinterval = 1.0f/x->samplerate;
	x->pk = (STABSZ*x->sampleinterval)*(1l<<(32-TPOW)) ;
    x->nosc = x->next_nosc = 0;
	x->debugPrintsRemaining = 80;
	x->is_bwe = 0;
	x->verbose = 0;
	x->noisep = &(NoiseTable[0]);
	
	if (argc > 0 && argv[0].a_type == A_SYM && argv[0].a_w.w_sym == ps_bwe) {
		x->is_bwe = 1;
		--argc;
		++argv;
		post("Bandwidth enhanced");
	}
	
    clear(x);

    sinusoids_list(x,s,argc,argv);
    
    /* Don't ramp initial frequencies up from zero: */
    for (i = 0; i < x->nosc; ++i) {
    	x->base[i].phase_inc = x->base[i].next_phase_inc;
    }
//post("%d %f %f %f", x->nosc, x->pk, x->sampleinterval, x->samplerate);
    return (x);
}

void SineFunction(int n, float *stab, int stride, float from, float to);
void SineFunction(int n, float *stab, int stride, float from, float to)
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
void Makeoscsinetable();
void Makeoscsinetable()
{
		SineFunction(STABSZ, Sinetab, 1, 0.0f, 2.0f*(float)PI);
}

int NTS(void);


void main(void)
{
	setup((t_messlist **)&sinusoids_class, (method)sinusoids_new, (method)dsp_free, 
		  (short)sizeof(t_sinusoids), 0L, A_GIMME, 0);
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
	post("Maximum Oscillators: %d", MAXOSCILLATORS);
    post("Never expires");
    
    // post("sizeof(NoiseTable) %ld", NTS());
	Makeoscsinetable();



	addmess((method)sinusoids_dsp, "dsp", A_CANT, 0);
	addmess((method)sinusoids_list, "list", A_GIMME, 0);
	addmess((method)sinusoids_clear, "clear", 0);
	addmess((method)sinusoids_assist, "assist", A_CANT, 0);
	addmess((method)tellmeeverything, "tellmeeverything", 0);
	addmess((method)sinusoids_verbose, "verbose", A_LONG, 0);
	dsp_initclass();
	
	ps_bwe = gensym("bwe");
}

void tellmeeverything(t_sinusoids *x) {
	int i;
	post("%ssinusoids~ object with %ld oscillators:", x->is_bwe ? "bandwidth-enhanced " : "", x->nosc);
	
	for (i = 0; i < x->nosc; ++i) {
		oscdesc *o = x->base+i;
		post("  amp (%.3f -> %.3f), freq (%4.2f -> %4.2f), noisiness (%.3f -> %.3f)",
			 o->amplitude, o->next_amplitude,
			 ((float) o->phase_inc) / x->pk, ((float) o->next_phase_inc) / x->pk,
			 o->noisiness, o->next_noisiness);
	}
}

void sinusoids_verbose(t_sinusoids *x, long v) {
	x->verbose = v;
	if (x->verbose) {
   		post("sinusoids~: turned verbose mode on");
	} else {
   		post("sinusoids~: turned verbose mode off");
	}
}
