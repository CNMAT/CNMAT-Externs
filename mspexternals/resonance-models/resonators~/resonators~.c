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
	©1988,1989,2005,2006 Adrian Freed
	©1999, 2005 UC Regents, All Rights Reserved. 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: resonators~
DESCRIPTION: MSP resonator Bank
AUTHORS: Adrian Freed
COPYRIGHT_YEARS: 1996,1997,1998,1999,2000,2001,2002,2004,2005,2006
VERSION 1.6: Compiles under 7/02 Max SDK and CW 7.0 
VERSION 1.7: Doesn't get smooth/unsmooth backwards
VERSION 1.7a: first windows compile
VERSION 1.8: double precision mode, second outlet for filter state, ping completed and tested, repaired amplitude interpretation
VERSION 1.9: added output amplitude vector 
VERSION 1.95: added output amplitude vector interpolation, found unfixed bug in double stuff
VERSION 1.96: Doesn't crash when making a new object (filterstate array taken out of t_resonators)
VERSION 1.97: fixed  double precision version  strange high frequency sound by turning off unrolling (Compiler now seems to do a good enough job on PowerPC anyway, fixed coefficient interpolation bug in case where smoothing is used and resonance models of different sizes are loaded
VERSION 1.98: MW+AF re-fixed coefficient interpolation bug to zero state variables and interpolate a1 aka a0.
VERSION 1.99: AF: fixed typo in above changed NewPtr to getbytes and increased number of resonances to 512
VERSION 1.995: AF: changed getbytes back to NewPtr and increased resonances to 1024
SVN_REVISION: $LastChangedRevision$
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


	To-Do
	
		After careful benchmarking on relevant processors/platforms some of the multiplicity of versions can be chucked away
		perhaps all of them in favor of the double precision version
		
		set the maximum number of resonances dynamically with an integer argument
		fix the assistance strings 
		test on failure of resonance allocation
		protect against filter update parallelism bugs a la peqbank
		partial filter set updates
	
	Make smooth mode settable by a message.
	fold all four perform procedures into a single version (with compiled-out conditionals and perhaps the loop unrolling using "duffs device")
			ouch this is hard
	
*/


#include "version.h"


#include "ext.h"
#include "z_dsp.h"
#include <math.h>
#define OGAIN
#define xUNROLL

void *resonators_class;
#define MAXRESONANCES 1024
typedef  struct resdesc
{
	float out1, out2;   // state
	float a1,a1prime, b1, b2;
	float o_a1, o_b1, o_b2;
	float fastr; // a value of r to accelerate decay
#ifdef OGAIN
	float og;
	float o_og;
#endif
} resdesc;
typedef  struct dresdesc
{
	double out1, out2;   // state
	double a1,a1prime, b1, b2;
	double o_a1, o_b1, o_b2;
	double fastr; // a value of r to accelerate decay
#ifdef OGAIN
	double og;
	double o_og;
#endif
} dresdesc;

/* bank of filters */
typedef struct 
{
	t_pxobject b_obj;
	short b_connected;
	resdesc *base;
	dresdesc *dbase;
	int nres; 
	int nmax;	/* maximum number of filters*/
	int ping; /* index of filter that will be pinged at the next opportunity */
	float pingsize; /* size of pulse */
	double samplerate;
	double sampleinterval;
#define  UNDERFLOWCHECK
#ifdef UNDERFLOWCHECK
	int underflowcheck;
#endif
	Boolean interpolating;
	Boolean doubling;
	void *outlet1;
} resbank;
typedef resbank t_resonators;

t_int *resonators_perform(t_int *w);
t_int *resonators2_perform(t_int *w);
void resonators_dsp(t_resonators *x, t_signal **sp, short *connect);
void resonators_float(t_resonators *x, double f);
void resonators_int(t_resonators *x, long n);
void resonators_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv);
void resonators_clear(t_resonators *x);
void resonators_assist(t_resonators *x, void *b, long m, long a, char *s);
void *resonators_new(t_symbol *s, short argc, t_atom *argv);
void resonators_tellmeeverything(t_resonators *x);


// unsmoothed with input
t_int *resonators_perform(t_int *w)
{
const 	t_float *in = (t_float *)(w[2]);
	t_float *out = (t_float *)(w[3]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[4]);
	int nfilters = op->nres;
	  float o0, o1, o2, o3;
	  float i0,i1,i2,i3,i4,i5;
		float yn,yo;
	int i, j;
	int ping = op->ping;
	
	if(op->b_obj.z_disabled)
		goto out;

	if(ping>=0 && ping<nfilters )
	{
		op->base[ping].out2 += op->pingsize*op->base[ping].a1prime;
		op->ping = -1;
	}

	for(j=0;j<n;j+=4)
	{
		resdesc *f = op->base;
		i0 = in[0];
		i1 = in[1];
		i2 = in[2];
		i3 = in[3];
		o0 = o1 = o2 = o3  = 0;	

		for(i=0;i< nfilters ;++i)
		{
			yn = f->out1;
			yo = f->out2;
	
			yo = f->b1*yn + f->b2*yo + f->a1*i0;	
			yn = f->b1*yo + f->b2*yn + f->a1*i1;	
#ifdef OGAIN
#define GMUL f->og*
#else
#define GMUL
#endif
			o0 += GMUL yo;
			o1 += GMUL yn;
	
			yo = f->b1*yn + f->b2*yo + f->a1*i2;	
			yn = f->b1*yo + f->b2*yn + f->a1*i3;	
			f->out2 = yo;
			f->out1 = yn;	
			o2 += GMUL  yo;
			o3 += GMUL yn;
			++f;
		}
		out[0] = o0;
		out[1] = o1;
		out[2] = o2;
		out[3] = o3;
		out += 4;
		in += 4;
	}

#ifdef UNDERFLOWCHECK
#define RESEPS 1.e-20f
#define MINUSRESEPS -1.e-20f
	/* underflow check */
	if(op->nres>0)
	{
		resdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f   = &op->base[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0f;
		}
	}
#endif
out:
	return (w+5);
}
// unsmoothed without input
t_int *resonators2_perform(t_int *w)
{
	t_float *out = (t_float *)(w[2]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[3]);
	
	int nfilters = op->nres;
	  float o0, o1, o2, o3;
	  float i0,i1,i2,i3,i4,i5;
		float yn,yo;
	int i, j;
	int ping = op->ping;
	if(op->b_obj.z_disabled)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->base[ping].out2 += op->pingsize*op->base[ping].a1prime;
		op->ping = -1;
	}

	for(j=0;j<n;j+=4)
	{
		resdesc *f = op->base;
		o0 = o1 = o2 = o3  = 0;	

		for(i=0;i< nfilters ;++i)
		{
			yn = f->out1;
			yo = f->out2;
	
			yo = f->b1*yn + f->b2*yo ;	
			yn = f->b1*yo + f->b2*yn ;	
			o0 += GMUL yo;
			o1 += GMUL yn;
	
			yo = f->b1*yn + f->b2*yo ;	
			yn = f->b1*yo + f->b2*yn;	
			f->out2 = yo;
			f->out1 = yn;	
			o2 += GMUL yo;
			o3 += GMUL yn;
			++f;
		}
		out[0] = o0;
		out[1] = o1;
		out[2] = o2;
		out[3] = o3;
		out += 4;
	}
#ifdef UNDERFLOWCHECK
	/* underflow check */
	if(op->nres>0)
	{
		resdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  &op->base[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0f;
		}
	}
#endif
out:

	return (w+4);
}


// interpolating (smooth) with input
t_int *iresonators_perform(t_int *w);
t_int *iresonators_perform(t_int *w)
{
	const t_float *in = (t_float *)(w[2]);
	t_float *out = (t_float *)(w[3]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[4]);
	int nfilters = op->nres;
	register	float yn,yo;
	int i, j;
	int ping = op->ping;
	float rate = 1.0f/n;
	
	if(op->b_obj.z_disabled)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->base[ping].out2 += op->pingsize*op->base[ping].a1prime;
		op->ping = -1;
	}

	{
		resdesc *f = op->base;
		for(j=0;j<n;++j)
			out[j] = 0.0f;
		for(i=0;i< nfilters ;++i)
		{
			register float b1=f[i].o_b1,b2=f[i].o_b2,a1=f[i].o_a1;
#ifdef OGAIN	
			register float			og=f[i].o_og;
#endif

			float a1inc = (f[i].a1-f[i].o_a1) *  rate;
			float b1inc = (f[i].b1-f[i].o_b1) *  rate;
			float b2inc = (f[i].b2-f[i].o_b2) *  rate;
#ifdef OGAIN
			float oginc = (f[i].og-f[i].o_og) *  rate;
#endif
			yo= f[i].out1;
			yn =f[i].out2;
#ifdef UNROLL
#ifdef OGAIN
if(f[i].og!=0.0f)
#endif
			for(j=0;j<n;j+=4) //unroll 4 times
			{
				yn = b1*yo + b2*yn + a1*in[j];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
			
				 out[j] += og* yn;
				 og += oginc;
#else
				 out[j] += yn;
#endif
				yo = b1*yn + b2*yo + a1*in[j+1];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;

#ifdef OGAIN
			
				 out[j+1] += og* yo;
				 og += oginc;
#else
				 out[j+1] += yo;
#endif
			yn = b1*yo + b2*yn + a1*in[j+2];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
	#ifdef OGAIN
			
				 out[j+2] += og* yn;
				 og += oginc;
#else
				 out[j+2] += yn;
#endif
			yo = b1*yn + b2*yo + a1*in[j+3];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
			
				 out[j+3] += og* yo;
				 og += oginc;
#else
				 out[j+3] += yo;
#endif
		
			}
#else
			for(j=0;j<n;++j) 
			{
				float x = yo;
				yo = b1*yo + b2*yn + a1*in[j];	
#ifdef OGAIN
			
				 out[j] += og* yn;
				 og += oginc;
#else
				 out[j] += yn;
#endif
				yn = x;
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
			}

#endif

			f[i].o_a1 = f[i].a1;
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
#ifdef OGAIN	
		f[i].o_og = f[i].og;
#endif


			f[i].out1= yo;
			f[i].out2 = yn;
		
		}
	}
#ifdef UNDERFLOWCHECK
		/* underflow check */
	if(op->nres>0)
	{
		resdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  &op->base[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0f;
		}
	}
#endif
out:
	return (w+5);
}

// smoothed without input
t_int *iresonators2_perform(t_int *w);
t_int *iresonators2_perform(t_int *w)
{
	t_float *out = (t_float *)(w[2]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[3]);
	
	float rate = 1.0f/n;
	int nfilters = op->nres;
		float yn,yo;
	int i, j;
	int ping = op->ping;
	if(op->b_obj.z_disabled)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->base[ping].out2 += op->pingsize*op->base[ping].a1prime;
		op->ping = -1;
	}

	{
		resdesc *f = op->base;
		for(j=0;j<n;++j)
			out[j] = 0.0f;

	
for(i=0;i< nfilters ;++i)
		{
			register float b1=f[i].o_b1,b2=f[i].o_b2;
				float b1inc = (f[i].b1-f[i].o_b1) *  rate;
			float b2inc = (f[i].b2-f[i].o_b2) *  rate;
#ifdef OGAIN	
			register float			og=f[i].o_og;
			float oginc = (f[i].og-f[i].o_og) *  rate;
#endif

#ifdef OGAIN
if(f[i].og==0.0f)
	continue;
#endif	
			yo= f[i].out1;
			yn =f[i].out2;
			for(j=0;j<n;j+=4) //unroll 4 times
			{
				yn = b1*yo + b2*yn ;	
			b1 += b1inc;
			b2 += b2inc;

#ifdef OGAIN
			
				 out[j] += og* yn;
				 og += oginc;
#else
				 out[j] += yn;
#endif				yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
			
				 out[j+1] += og* yo;
				 og += oginc;
#else
				 out[j+1] += yn;
#endif
				yn = b1*yo + b2*yo;	
				b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
			
				 out[j+2] += og* yn;
				 og += oginc;
#else
				 out[j+2] += yn;
#endif			
			yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
			
				 out[j+3] += og* yo;
				 og += oginc;
#else
				 out[j+3] += yo;
#endif		
			}
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
			f[i].out1= yo;
			f[i].out2 = yn;

#ifdef OGAIN	
		f[i].o_og = f[i].og;
#endif

		
		}
	}
#ifdef UNDERFLOWCHECK	
	/* underflow check */
	if(op->nres>0)
	{
		resdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  &op->base[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0f;
		}
	}
#endif

out:

	return (w+4);
}

#define MAXMAXVECTOR 4096
// double precision interpolating (smooth) with input
t_int *diresonators_perform(t_int *w);
t_int *diresonators_perform(t_int *w)
{
	const t_float *in = (t_float *)(w[2]);
	t_float *fout = (t_float *)(w[3]);
	t_resonators *op = (t_resonators *)(w[1]);
	double out[MAXMAXVECTOR];
	int n = (int)(w[4]);
	int nfilters = op->nres;
	register	double yn,yo;
	int i, j;
	int ping = op->ping;
	double rate = 1.0/n;
	
	if(op->b_obj.z_disabled || n>MAXMAXVECTOR)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->dbase[ping].out2 += op->pingsize*op->dbase[ping].a1prime;
		op->ping = -1;
	}

	{
		dresdesc *f = op->dbase;
		for(j=0;j<n;++j)
			out[j] = 0.0;

	
		for(i=0;i< nfilters ;++i)
		{
			register double b1=f[i].o_b1, b2=f[i].o_b2, a1=f[i].o_a1;
			double a1inc = (f[i].a1-f[i].o_a1) *  rate;
			double b1inc = (f[i].b1-f[i].o_b1) *  rate;
			double b2inc = (f[i].b2-f[i].o_b2) *  rate;
#ifdef OGAIN	
			register double			og=f[i].o_og;

			double oginc = (f[i].og-f[i].o_og) *  rate;
#endif		
			yo= f[i].out1;
			yn =f[i].out2;
#ifdef UNROLL
#ifdef OGAIN
if(f[i].og==0.0)
	continue;
	
#endif			
	for(j=0;j<n;j+=4) //unroll 4 times
			{
			yn = b1*yo + b2*yn + a1*in[j];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;

#ifdef OGAIN			
				 out[j] += og* yn;
				 og += oginc;
#else
				 out[j] += yn;
#endif	
			yo = b1*yn + b2*yo ;	
				yo = b1*yn + b2*yo + a1*in[j+1];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
			
#ifdef OGAIN
				 out[j+1] += og* yo;
				 og += oginc;
#else
				 out[j+1] += yo;
#endif		
		yo = b1*yn + b2*yo ;	

				yn = b1*yo + b2*yn + a1*in[j+2];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN			
				 out[j+2] += og* yn;
				 og += oginc;
#else
				 out[j+2] += yn;
#endif	
				yo = b1*yn + b2*yo + a1*in[j+3];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
				 out[j+3] += og* yo;
				 og += oginc;
#else
				 out[j+3] += yo;
#endif		
		
			}
#else
			for(j=0;j<n;++j)
			{
				double x = yo;
				yo = b1*yo + b2*yn + a1*in[j];	
#ifdef OGAIN
				 out[j] += og* yo;
				 og += oginc;
#else
				 out[j] += yo;
#endif		
				yn = x;
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
			}

#endif

			f[i].o_a1 = f[i].a1;
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
			f[i].out1= yo;
			f[i].out2 = yn;
#ifdef OGAIN	
		f[i].o_og = f[i].og;
#endif		
		}
	}
	for(j=0;j<n;++j)
	{
		fout[j] = out[j]; // when will max have double signals?
	}

#ifdef UNDERFLOWCHECK
		/* underflow check */
	if(op->nres>0)
	{
		dresdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  &op->dbase[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0;
		}
	}
#endif

out:
	return (w+5);
}

// double precision smoothed without input
t_int *diresonators2_perform(t_int *w);
t_int *diresonators2_perform(t_int *w)
{
	t_float *fout = (t_float *)(w[2]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[3]);
	double out[MAXMAXVECTOR];
	
	double rate = 1.0/n;
	int nfilters = op->nres;
		double yn,yo;
	int i, j;
	int ping = op->ping;
	if(op->b_obj.z_disabled||n>MAXMAXVECTOR)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->dbase[ping].out2 += op->pingsize*op->dbase[ping].a1prime;
		op->ping = -1;
	}

	{
		dresdesc *f = op->dbase;
		for(j=0;j<n;++j)
			out[j] = 0.0f;



	for(i=0;i< nfilters ;++i)
		{
			register double b1=f[i].o_b1,b2=f[i].o_b2;
				double b1inc = (f[i].b1-f[i].o_b1) *  rate;
			double b2inc = (f[i].b2-f[i].o_b2) *  rate;
#ifdef OGAIN	
			register double			og=f[i].o_og;

			double oginc = (f[i].og-f[i].o_og) *  rate;
#endif
#ifdef OGAIN
if(f[i].og==0.0)
	continue;
#endif	
			yo= f[i].out1;
			yn =f[i].out2;
			for(j=0;j<n;j+=4) //unroll 4 times
			{
				yn = b1*yo + b2*yn ;	
			b1 += b1inc;
			b2 += b2inc;

#ifdef OGAIN
				 out[j] += og* yn;
				 og += oginc;
#else
				 out[j] += yn;
#endif		
				yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
				 out[j+1] += og* yo;
				 og += oginc;
#else
				 out[j+1] += yo;
#endif		
				yn = b1*yo + b2*yn;	
				b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
				 out[j+2] += og* yn;
				 og += oginc;
#else
				 out[j+2] += yn;
#endif		
				yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
#ifdef OGAIN
				 out[j+3] += og* yo;
				 og += oginc;
#else
				 out[j+3] += yo;
#endif		
		
			}
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
			f[i].out1= yo;
			f[i].out2 = yn;
#ifdef OGAIN	
		f[i].o_og = f[i].og;
#endif
		
		}
	}
	
	for(j=0;j<n;++j)
	{
		fout[j] = out[j]; // when will max have floating point signals?
	}

#ifdef UNDERFLOWCHECK	
	/* underflow check */
	if(op->nres>0)
	{
		dresdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  &op->dbase[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0;
		}
	}
#endif

out:

	return (w+4);
}
// this is done every time the filter is restarted, in case you blow it up

void resonators_clear(t_resonators *x)
{
	resdesc *f = x->base;
	dresdesc *df = x->dbase;
	int i;
		for(i=0;i<x->nres;++i)
		{
			f[i].out1 = f[i].out2 = 0.0f;
			df[i].out1 = df[i].out2 = 0.0f;

		}

}

void resonators_dsp(t_resonators *x, t_signal **sp, short *connect)
{
	short i;
	resonators_clear(x);
	
	if(x->doubling)
	{
		if (x->b_connected = connect[1])
		{
				dsp_add(diresonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
			}
		else {
				dsp_add(diresonators2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
		}
	}
	else
	{

		if (x->b_connected = connect[1])
		{
			if(x->interpolating)
				dsp_add(iresonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
			else
				dsp_add(resonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
		}
		else {
			if(x->interpolating)
				dsp_add(iresonators2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
			else
				dsp_add(resonators2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
		}
	}
}

// note that this assumes we can never be interrupted by perform routine
// we need buffering here as we did with the ping functions
void resonators_float(t_resonators *x, double ff)
{
	int i;
	if(x->doubling)
	{
		dresdesc *dp = x->dbase;
		for(i=0;i<x->nres;++i)
		{
			dp[i].out2 += dp[i].a1prime*ff;
		
		}
	}
	else
	{
		resdesc *fp = x->base;
		float f = ff;
		for(i=0;i<x->nres;++i)
		{
			fp[i].out2 += fp[i].a1prime*f;
		
		}
	}

}
// again we shouldn't jam the variables
void resonators_squelch(t_resonators *x);
void resonators_squelch(t_resonators *x)
{
	resdesc *fp = x->base;
	dresdesc *dp = x->dbase;
	int i;
		for(i=0;i<x->nres;++i)
		{
			if(x->doubling)
			{
				dp[i].b1 *= dp[i].fastr;
				dp[i].b2 *= dp[i].fastr*dp[i].fastr;
			}
			else
			{
				fp[i].b1 *= fp[i].fastr;
				fp[i].b2 *= fp[i].fastr*fp[i].fastr;
			}
		
		}

}


void resonators_int(t_resonators *x, long n)
{
	resonators_float(x,(double)n);
}
void resonators_bang(t_resonators *x);
void resonators_bang(t_resonators *x)
{
	int i;
	t_atom filterstate[MAXRESONANCES*5+1];

//	output filter state and coefficients to the second outlet
// should we output the sample rate or normalize the coefficients?
		SETFLOAT(&filterstate[0], x->samplerate);

	for(i=0;i<x->nres;++i)
	{
		SETFLOAT(&filterstate[1+i*5+0], x->doubling?x->dbase[i].out1: x->base[i].out1);
		SETFLOAT(&filterstate[1+i*5+1], x->doubling?x->dbase[i].out2:x->base[i].out2);
		SETFLOAT(&filterstate[1+i*5+2], x->doubling?x->dbase[i].a1:x->base[i].a1);
		SETFLOAT(&filterstate[1+i*5+3], x->doubling?x->dbase[i].b1:x->base[i].b1);
		SETFLOAT(&filterstate[1+i*5+4], x->doubling?x->dbase[i].b2:x->base[i].b2);
	}
	   outlet_list(x->outlet1, 0L, 1+i*5, filterstate);

}
// ignores the inlet, uses order to specify the coefficients
static void outputgain_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv);
void outputgain_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv)
{
	int i;
	resdesc *fp = x->base;
	dresdesc *dp = x->dbase;
		for(i=0; i<argc; ++i) {
		if (i >= MAXRESONANCES) {
			post("¥ resonators~: warning: output gain list has more than %ld resonances; dropping extras",
				 MAXRESONANCES);
			break;
		} else {
#ifdef OGAIN
			
			 dp[i].og =fp[i].og = atom_getfloatarg(i,argc,argv);
#endif
	}
	}
	}
void resonators_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv)
{
	int i;
	// does this overlap stuff work? why dont we buffer a1prime and fastr? 
	
#define NOOVERLAP
#ifdef NOOVERLAP
	double ta1[MAXRESONANCES];
	double tb1[MAXRESONANCES];
	double tb2[MAXRESONANCES];
#endif
	int nres;
	double srbar = x->sampleinterval;
	resdesc *fp = x->base;
	dresdesc *dp = x->dbase;
	
	if(argc==2)
	{
		 x->ping = atom_getintarg(0,argc,argv);
		if(x->ping>x->nres)
			x->ping = -1;
		x->pingsize = atom_getfloatarg(1,argc,argv);
		return;
	}

	if (argc%3!=0) {
		post("multiple of 3 floats required (frequency amplitude decayRate");
		return;
	}
			
	for(i=0; (i*3)<argc; ++i) {
		if (i >= MAXRESONANCES) {
			post("¥ resonators~: warning: list has more than %ld resonances; dropping extras",
				 MAXRESONANCES);
			break;
		} else {
		
			double f = atom_getfloatarg(i*3,argc,argv);
			double g = 	atom_getfloatarg(i*3+1,argc,argv);
			double rate = atom_getfloatarg(i*3+2,argc,argv);
			double r;
			r =  exp(-rate*srbar);
			if((f<=0.0) || (f>=(0.995*x->samplerate*0.5)) || (r<=0.0) || (r>1.0))
			{
	//				post("Warning parameters out of range");
	#ifdef NOOVERLAP
				ta1[i] = 0.0;
				tb1[i] = 0.0;
				tb2[i] = 0.0;
	#else
				fp[i].b1 = fp[i].b1 = 0.0f;
				dp[i].b1 = dp[i].b1 = 0.0;
	#endif
				fp[i].a1prime = 0.0f;
				fp[i].fastr = 0.0f;
				dp[i].a1prime = 0.0;
				dp[i].fastr = 0.0;
			}
			else
			{
				double ts;
				f *= 2.0*3.14159265358979323*srbar;
				ts = g;
					ts *= sin(f);
				tb1[i] = r*cos(f)*2.0;
					ta1[i] = ts *  (1.0-r);   //this is one of the relavent L norms
				tb2[i] =  -r*r;
				fp[i].a1prime = dp[i].a1prime = ts/tb2[i];   //this is the other norm that establishes the impulse response of the right amplitude (scaled 
													// so that it can be summed into the state variable outside the perform routine 
													// If patents were cheaper..........
			fp[i].fastr = dp[i].fastr= exp(-rate*100.0*srbar)/r; //to decay fast
			}
		}
	}
	/* Now we know how many "good" resonances (freq > 0) were in the list */
	nres = i;
//This is where we should be double buffering




	for(i=0;i<nres;++i)
	{
			fp[i].b1 = dp[i].b1 = tb1[i];
			fp[i].a1 = dp[i].a1 = ta1[i];
			fp[i].b2 = dp[i].b2 =  tb2[i];
			if(i>=x->nres) 	/* If there are now more resonances than there were: */ 

			{
			    // Set old a1 to zero so that the input to the new resonators will ramp up over the first signal vector.
				fp[i].o_a1 = 0.0f;
				fp[i].o_b1 = fp[i].b1;
				fp[i].o_b2 = fp[i].b2;
				dp[i].o_a1 = 0.0;
				dp[i].o_b1 = dp[i].b1;
				dp[i].o_b2 = dp[i].b2;
				// Clear out state variables for these totally new resonances
				fp[i].out1 = fp[i].out2 = 0.0f;
				dp[i].out1 = dp[i].out2 = 0.0f;

#ifdef OGAIN
				dp[i].o_og = dp[i].og = 1.0;
				fp[i].o_og = fp[i].og = 1.0f;
#endif
			}
	}
	x->nres = nres;
	
// end of double buffering
//		post("nres %d x->nres %d", nres, x->nres);
}

void resonators_assist(t_resonators *x, void *b, long m, long a, char *s)
{
       if (m == ASSIST_OUTLET)
               sprintf(s,"(Signal) Filter bank output");
       else {
             sprintf(s,"(Signal) Filter bank input and control messages");
      }
}
/*

I commented this whole thing out to get resonators~ to compile with Xcode.  A bad idea?  -mzed

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
*/
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

void *resonators_new(t_symbol *s, short argc, t_atom *argv)
{
    t_resonators *x = (t_resonators *)newobject(resonators_class);
		x->samplerate =  sys_getsr();
		if(x->samplerate<=0.0)
			x->samplerate = 44100.0;
		x->sampleinterval = 1.0/x->samplerate;


    {
    	x->interpolating = false;
    	
    	x->doubling = false;
    	
	    if(argc>=1)
	    {
	    	if(isthesymbol("smooth", argv))
	    	{
	    		argc--; argv++;
	    		x->interpolating = true;
	    	}
		    if(isthesymbol("double", argv))
	    	{
	    		argc--; argv++;
	    		x->interpolating = true;
	    		x->doubling = true;
	    	}
	    }
    }
 	x->dbase = (dresdesc *) NewPtr(MAXRESONANCES*sizeof(dresdesc));
    	x->base = (resdesc *) NewPtr(MAXRESONANCES*sizeof(resdesc));
  	    if(x->base==NIL || x->dbase==NIL)
	    {			post("¥ resonators~: warning: not enough memory.  Expect to crash soon.");
	    	return 0;
	    }

   x->nres = MAXRESONANCES;
    resonators_clear(x); // clears state
    x->nres = 0;
    resonators_list(x,s,argc,argv);
    {
    	resdesc *f = x->base;
    	dresdesc *df = x->dbase;
		int i;
		for(i=0;i<x->nres;++i)
		{
			f[i].o_a1 = f[i].a1;
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
			df[i].o_a1 = df[i].a1;
			df[i].o_b1 = df[i].b1;
			df[i].o_b2 = df[i].b2;
#ifdef OGAIN
			df[i].o_og = df[i].og = 1.0;
			f[i].o_og = f[i].og = 1.0f;
#endif
		}
	}
    	x->ping = -1;
		x->pingsize = 1.0f;
#ifdef UNDERFLOWCHECK
		x->underflowcheck = 0;
#endif
		
   x->b_obj.z_misc = Z_NO_INPLACE;
    dsp_setup((t_pxobject *)x,1);
    x->b_obj.z_misc = Z_NO_INPLACE;
 
    x->outlet1 = listout(x);
		
    outlet_new((t_object *)x, "signal");
    return (x);
}


void main(void)
{
	setup((t_messlist **)&resonators_class, (method) resonators_new, (method)dsp_free, (short)sizeof(t_resonators),
		0L, A_GIMME, 0);

	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © 1986, 1987 Adrian Freed");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
	post("Maximum number of resonances: %d", MAXRESONANCES);
	post("Never expires");
	
	addmess((method)resonators_dsp, "dsp", A_CANT, 0);
	addmess((method)resonators_list, "list", A_GIMME, 0);
	addmess((method)outputgain_list, "outputgain", A_GIMME, 0);
	addmess((method)resonators_clear, "clear", 0);
	addmess((method)resonators_squelch, "squelch", 0);
	addbang((method)resonators_bang);
	addfloat((method)resonators_float);
	addint((method)resonators_int);
	addmess((method)resonators_assist, "assist", A_CANT, 0);
	addmess((method)resonators_tellmeeverything, "tellmeeverything", 0);
	dsp_initclass();
//	rescopy('STR#',3216);
}

void resonators_tellmeeverything(t_resonators *x) {
	int i;
	
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © 1986, 1987 Adrian Freed");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
	post("  Compiled " __DATE__ " " __TIME__);
/*	if (x->interpolating) {
		post("  Smooth mode: parameter changes interpolated over time");
	} else {
		post("  Fast mode: no interpolation, more efficient");
	} */
	post("%s\n%s\n Max resonances: %d, currently computing  %d",
	x->interpolating?"  Smooth mode: parameter changes interpolated over time":"  Fast mode: no interpolation, more efficient",
	x->doubling?"  Double precision mode":"",
	 MAXRESONANCES, x->nres);
}


#ifdef SHIT
t_int *iresonators_perform(t_int *w);
t_int *iresonators_perform(t_int *w)
{
const	t_float *in = (t_float *)(w[2]);
	t_float *out = (t_float *)(w[3]);
	t_resonators *op = (t_resonators *)(w[1]);
	int n = (int)(w[4]);
	int nfilters = op->nres;
	  float o0, o1, o2, o3;
	  float i0,i1,i2,i3,i4,i5;
		float yn,yo;
	int i, j;
	int ping = op->ping;
	
	if(op->b_obj.z_disabled)
		goto out;

	if(ping>=0 && ping<nfilters)
	{
		op->base[ping].out2 += op->pingsize*op->base[ping].a1prime;
		op->ping = -1;
	}

	for(j=0;j<n;++j)
	{
		resdesc *f = op->base;
		out[j] = 0.0f;
		for(i=0;i< nfilters ;++i,++f)
		{
			yo = f->b1*f->out1 + f->b2*f->out2 + f->a1*in[j];	
			 out[j] += yo;
	
			f->out2 = f->out1;
			f->out1 = yo;	
		}
	}
#ifdef UNDERFLOWCHECK
	/* underflow check */
	if(op->nres>0)
	{
		resdesc *f;
		(op->underflowcheck)++;
		op->underflowcheck %= op->nres;
		f =  = &op->base[op->underflowcheck];
		if((f->out2<RESEPS) && (f->out2>MINUSRESEPS) && (f->out1<RESEPS) && (f->out1>MINUSRESEPS))
		{
			f->out1 = f->out2 = 0.0f;
		}
	}
#endif
out:
	return (w+5);
}
#endif
