/*
Copyright (c) 1999.  The Regents of the University of California (Regents).
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
	sinusoids.c
	
	MSP resonator Bank
	
	©1988,1989 Adrian Freed
	©1999 UC Regents, All Rights Reserved. 

	
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
	long next_phase_inc;
	long phase_inc;			/* frequency */
//	ulong next_phaseadd;
//	ulong phaseadd;			/* phase */
} oscdesc;


/* bank of filters */
typedef struct 
{
	t_pxobject b_obj;
	oscdesc base[MAXOSCILLATORS];
	int nosc; 
	int cleared;
	float  pk;
	float samplerate;
	float sampleinterval;
} oscbank;
typedef oscbank t_sinusoids;

t_int *sinusoids2_perform(t_int *w);
void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect);
 void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv);
 void sinusoids_clear(t_sinusoids *x);
 void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s);
 void *sinusoids_new(t_symbol *s, short argc, t_atom *argv);

t_int *sinusoids2_perform(t_int *w)
{
	t_sinusoids *op = (t_sinusoids *)(w[1]);
		t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	int nosc = op->nosc;
	 int i,j;
	 oscdesc *o = op->base;
	 const char *st = (const char *)Sinetab;
	 float rate ;
//	if(op->b_obj.z_disabled)
//		goto out;
	rate = 1.0f/n;
	for(j=0;j<n;++j)
		out[j] = 0.0f;

	for(i=0;i<nosc;++i)
	{
		register float a = o->amplitude;
		register long pi = o->phase_inc;
		register ulong pc = o->phase_current;
		register long pstep = (o->next_phase_inc - o->phase_inc)*rate;
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
		o->phase_inc = o->next_phase_inc;
		o->phase_current = pc;
		++o;
	}
out:
	if(op->cleared)
	{
		op->nosc = 0;
		op->cleared = 0;
	}
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
	x->cleared = TRUE;
}


void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect)
{
	int i;

	dsp_add(sinusoids2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
}

void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv)
{
	int i;
	if(argc%2!=0)
	{
		post("multiple of 2 floats (Frequency, amplitude) required");
	}
	else
	{
		oscdesc *fp = x->base;
		int nosc;
		nosc = argc/2;
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
			float f = atom_getfloatarg(i*2,argc,argv);
			float a = 	atom_getfloatarg(i*2+1,argc,argv);
			if((f<=0.0f) || (f>=(x->samplerate*0.5f)))
			{
				fp[i].next_phase_inc = 0;
				fp[i].next_amplitude = 0.0f;
				fp[i].amplitude = 0.0f;
				fp[i].phase_inc = 0;
				fp[i].phase_current = 0;
//				fp[i].phaseadd = 0;
//				fp[i].next_phaseadd = 0;
			}
			else
			{
				fp[i].next_phase_inc = x->pk*f;	/* frequency	*/
				fp[i].next_amplitude = a;		/* amplitude	*/
			}
//	post("%d %d %d %f %f %f", i, nosc,fp[i].next_phase_inc, fp[i].next_amplitude, f, a);
		}
//		post("nosc %d x-nosc %d", nosc, x->nosc);
	}
}

void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s)
{
	assist_string(3214,m,a,1,2,s);
}

void *sinusoids_new(t_symbol *s, short argc, t_atom *argv)
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

void main(void)
{
	setup((t_messlist **)&sinusoids_class, (method)sinusoids_new, (method)dsp_free, 
		  (short)sizeof(t_sinusoids), 0L, A_GIMME, 0);
	post("sinusoids~ 1.6 - Adrian Freed");
	post("Copyright © 1996,1997,1998,1999,2000,01,02 Regents of the University of California.");
	post("Maximum Oscillators: %d", MAXOSCILLATORS);
    post("Never expires");
    
	Makeoscsinetable();

#ifdef EXPIRE
#define YEAR 1999
#define MONTH 1
#define MONTH1 12
	post("Expires Dec 1999");
	{
		DateTimeRec date;
		GetTime(&date);
		if(date.year!=YEAR || date.month <MONTH || date.month >MONTH1)
		{
			post("Expired");
		}
		else

#else
{
#endif

			addmess((method)sinusoids_dsp, "dsp", A_CANT, 0);

	}
	addmess((method)sinusoids_list, "list", A_GIMME, 0);
	addmess((method)sinusoids_clear, "clear", 0);
	addmess((method)sinusoids_assist, "assist", A_CANT, 0);
	dsp_initclass();
	rescopy('STR#',3214);
}


