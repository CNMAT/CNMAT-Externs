/*
Copyright (c) 1999,2000,01,02,03,04,05,06.  The Regents of the
University of California (Regents).  All Rights Reserved.

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
NAME: oscillators~
DESCRIPTION: Oscillator bank that can read waveform from a buffer~
AUTHORS: Adrian Freed
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
VERSION 1.0: Adrian's initial version 
VERSION 1.1: (Matt) initializes no-argument buffer null pointer correctly 
VERSION 1.2: (Matt) noglissbirthmode
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
	
	©1988,1989 Adrian Freed
	©1999 UC Regents, All Rights Reserved. 
*/

#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"
t_symbol *ps_buffer;
#include <math.h>
#undef PI  // of math.h
#define PI 3.14159265358979323f
#define MAXOSCILLATORS 256

#define TPOW 16
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
    t_symbol *l_sym;
    t_buffer *l_buf;
	int noglissbirthmode;
	
} oscbank;
typedef oscbank t_sinusoids;

t_int *sinusoids2_perform(t_int *w);
void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect);
 void sinusoids_list(t_sinusoids *x, t_symbol *s, short argc, t_atom *argv);
 void sinusoids_clear(t_sinusoids *x);
 void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s);
 void *oscillators_new(t_symbol *s, short argc, t_atom *argv);
void oscillators_noglissbirthmode(t_sinusoids *x, long i);


t_int *sinusoids2_perform(t_int *w)
{
	t_sinusoids *op = (t_sinusoids *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	int n = (int)(w[3]);
	int nosc = op->nosc;
	int i,j;
	oscdesc *o = op->base;
    t_buffer *b = op->l_buf;
	const char *st;
	float rate ;
	 
	if(op->b_obj.z_disabled) 
		goto out;
	if((b==0) || !b->b_valid || (b->b_frames!=(1l<<16)))
		goto zero;
	rate = 1.0f/n;
	st  = (const char *)b->b_samples;
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
	goto out;
zero:
	for(i=0;i<n;++i)
		out[i] = 0.0f;
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

void oscillators_set(t_sinusoids *x, t_symbol *s);
void oscillators_set(t_sinusoids *x, t_symbol *s)
{
	t_buffer *b;
	
	if (s==0) {
		error("oscillators: no buffer specified");
		return;
	}
	
	x->l_sym = s;
	if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer) {
		x->l_buf = b;
	} else {
		error("oscillators: %s is not a buffer~", s->s_name);
		x->l_buf = 0;
	}
}
void sinusoids_dsp(t_sinusoids *x, t_signal **sp, short *connect)
{
	int i;

	/* Now that DSP is starting, assume that the buffer~ we're reading from has been
	   loaded */
    oscillators_set(x,x->l_sym);

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
		int noglissbirthmode = x->noglissbirthmode;
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
				if (noglissbirthmode) {
					if (fp[i].amplitude == 0.0) {
						/* No need to interpolate frequency */
						fp[i].phase_inc = x->pk*f;
					}
				}				
			}
//	post("%d %d %d %f %f %f", i, nosc,fp[i].next_phase_inc, fp[i].next_amplitude, f, a);
		}
//		post("nosc %d x-nosc %d", nosc, x->nosc);
	}
}

void oscillators_noglissbirthmode(t_sinusoids *x, long i) {
	x->noglissbirthmode = i;
}


void sinusoids_assist(t_sinusoids *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET){
		sprintf(s,"(signal) oscillator bank output");
	}
	else {
		sprintf(s,"(list)frequency amplitude pairs");	
	}
}

void *oscillators_new(t_symbol *s, short argc, t_atom *argv)
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
    x->l_buf = 0;
	x->noglissbirthmode = 0;
		
		if(argc>0 && argv->a_type==A_SYM) {
		    	x->l_sym = argv[0].a_w.w_sym;
		    	/* Remember the symbol, but don't try to look up the buffer yet,
		    	   since the buffer~ object in question might not be loaded yet.
		    	   Instead we'll call oscillators_set() from the dsp() routine. */
		} else {
			post("no buffer~.");
			x->l_sym = 0;
		}

    sinusoids_list(x,s,argc-1,argv+1);

//post("%d %f %f %f", x->nosc, x->pk, x->sampleinterval, x->samplerate);
    return (x);
}


void main(void)
{
	setup((struct messlist **)&sinusoids_class,(method)oscillators_new, (method)dsp_free, (short)sizeof(t_sinusoids),
		0L, A_GIMME, 0);
	post("oscillators~ 1.3 - Adrian Freed and Matt Wright");
	post("Copyright © 1996,1997,1998,1999,2000,2001,02,03,04 Regents of the University of California.");
	post("Maximum Oscillators: %d", MAXOSCILLATORS);
    post("Never expires.");
    

	addmess((method)sinusoids_dsp, "dsp", A_CANT, 0);
	addmess((method)sinusoids_list, "list", A_GIMME, 0);
	addmess((method)sinusoids_clear, "clear", 0);
	addmess((method)sinusoids_assist, "assist", A_CANT, 0);
	addmess((method)oscillators_set, "set", A_SYM, 0);
	addmess((method)oscillators_noglissbirthmode, "noglissbirthmode", A_LONG, 0);
	dsp_initclass();
	
	ps_buffer = gensym("buffer~");
}
