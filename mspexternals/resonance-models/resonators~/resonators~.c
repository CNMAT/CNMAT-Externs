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
	resonators.c
	
	MSP resonator Bank
	
	©1988,1989 Adrian Freed
	©1999 UC Regents, All Rights Reserved. 

	
*/


/*
 Version 1.6: Compiles under 7/02 Max SDK and CW 7.0 
*/

#define VERSION	"resonators~ 1.6Beta- Adrian Freed"


#include "ext.h"
#include "z_dsp.h"
#include <math.h>

void *resonators_class;
#define MAXRESONANCES 256
typedef  struct resdesc
{
	float out1, out2;   // state
	float a1,a1prime, b1, b2;
	float o_a1, o_b1, o_b2;
	float fastr; // a value of r to accelerate decay
} resdesc;

/* bank of filters */
typedef struct 
{
	t_pxobject b_obj;
	short b_connected;
	resdesc base[MAXRESONANCES];
	int nres; 
	int nmax;/* maximum number of filters*/
	int ping; /* index of filter that will be pinged at the next opportunity */
	float pingsize; /* size of pulse */
	float samplerate;
	float sampleinterval;
#define  UNDERFLOWCHECK
#ifdef UNDERFLOWCHECK
	int underflowcheck;
#endif
	Boolean interpolating;
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

	if(ping>=0 && ping<nfilters &&(op->base[ping].b1 > 0.0f))
	{
		op->base[ping].out1 += op->pingsize/op->base[ping].b1;
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
			o0 += yo;
			o1 += yn;
	
			yo = f->b1*yn + f->b2*yo + f->a1*i2;	
			yn = f->b1*yo + f->b2*yn + f->a1*i3;	
			f->out2 = yo;
			f->out1 = yn;	
			o2 += yo;
			o3 += yn;
			++f;
		}
		out[0] = o0;
		out[1] = o1;
		out[2] = o2;
		out[3] = o3;
		out += 4;
		in += 4;
	}
#define UNDERFLOWCHECK
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
// ping stuff is undocumented and untested
	if(ping>=0 && ping<nfilters &&(op->base[ping].b1 > 0.0f))
	{
		op->base[ping].out1 += op->pingsize/op->base[ping].b1;
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
			o0 += yo;
			o1 += yn;
	
			yo = f->b1*yn + f->b2*yo ;	
			yn = f->b1*yo + f->b2*yn;	
			f->out2 = yo;
			f->out1 = yn;	
			o2 += yo;
			o3 += yn;
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

	if(ping>=0 && ping<nfilters &&(op->base[ping].b1 > 0.0f))
	{
		op->base[ping].out1 += op->pingsize/op->base[ping].b1;
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


// interpolating (smooth)
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

	if(ping>=0 && ping<nfilters &&(op->base[ping].b1 > 0.0f))
	{
		op->base[ping].out1 += op->pingsize/op->base[ping].b1;
		op->ping = -1;
	}

	{
		resdesc *f = op->base;
		for(j=0;j<n;++j)
			out[j] = 0.0f;
		for(i=0;i< nfilters ;++i)
		{
			register float b1=f[i].o_b1,b2=f[i].o_b2,a1=f[i].o_a1;
			float a1inc = (f[i].a1-f[i].o_a1) *  rate;
			float b1inc = (f[i].b1-f[i].o_b1) *  rate;
			float b2inc = (f[i].b2-f[i].o_b2) *  rate;
			yo= f[i].out1;
			yn =f[i].out2;
#define UNROLL
#ifdef UNROLL
			for(j=0;j<n;j+=4) //unroll 4 times
			{
				yn = b1*yo + b2*yn + a1*in[j];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;

				 out[j] += yn;
				yo = b1*yn + b2*yo + a1*in[j+1];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
				 out[j+1] += yo;
				yn = b1*yo + b2*yn + a1*in[j+2];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
				 out[j+2] += yn;
				yo = b1*yn + b2*yo + a1*in[j+3];	
			a1 += a1inc;
			b1 += b1inc;
			b2 += b2inc;
				 out[j+3] += yo;
		
			}
#else
			for(j=0;j<n;++j) //unroll 4 times
			{
				float x = yo;
				yo = b1*yo + b2*yn + a1*in[j];	
				 out[j] += yo;
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

	if(ping>=0 && ping<nfilters &&(op->base[ping].b1 > 0.0f))
	{
		op->base[ping].out1 += op->pingsize/op->base[ping].b1;
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
			yo= f[i].out1;
			yn =f[i].out2;
			for(j=0;j<n;j+=4) //unroll 4 times
			{
				yn = b1*yo + b2*yn ;	
			b1 += b1inc;
			b2 += b2inc;

				 out[j] += yn;
				yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
				 out[j+1] += yo;
				yn = b1*yo + b2*yn;	
				b1 += b1inc;
			b2 += b2inc;
				 out[j+2] += yn;
				yo = b1*yn + b2*yo ;	
			b1 += b1inc;
			b2 += b2inc;
				 out[j+3] += yo;
		
			}
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
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

	return (w+4);
}

// this is done every time the filter is restarted, in case you blow it up

void resonators_clear(t_resonators *x)
{
	resdesc *f = x->base;
	int i;
		for(i=0;i<x->nres;++i)
		{
			f[i].out1 = f[i].out2 = 0.0f;

		}

}

void resonators_dsp(t_resonators *x, t_signal **sp, short *connect)
{
	short i;
	resonators_clear(x);

	if (x->b_connected = connect[1])
	{
		if(x->interpolating)
			dsp_add(iresonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
		else
			dsp_add(resonators_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec,  sp[0]->s_n);
	}
	else {
		if(x->interpolating)
			dsp_add(resonators2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
		else
			dsp_add(iresonators2_perform, 3, x,sp[0]->s_vec,  sp[0]->s_n);
		
	}
}

void resonators_float(t_resonators *x, double ff)
{
	float f = ff;
	resdesc *fp = x->base;
	int i;
		for(i=0;i<x->nres;++i)
		{
			fp[i].out1 += fp[i].a1prime*f;
		
		}

}

void resonators_squelch(t_resonators *x);
void resonators_squelch(t_resonators *x)
{
	resdesc *fp = x->base;
	int i;
		for(i=0;i<x->nres;++i)
		{
			fp[i].b1 *= fp[i].fastr;
			fp[i].b2 *= fp[i].fastr*fp[i].fastr;
		
		}

}


void resonators_int(t_resonators *x, long n)
{
	resonators_float(x,(double)n);
}

// ignores the inlet, uses order to specify the coefficients

void resonators_list(t_resonators *x, t_symbol *s, short argc, t_atom *argv)
{
	int i;
	
#define NOOVERLAP
#ifdef NOOVERLAP
	float ta1[MAXRESONANCES];
	float tb1[MAXRESONANCES];
	float tb2[MAXRESONANCES];
#endif
	int nres;
	float srbar = x->sampleinterval;
	resdesc *fp = x->base;

	if (argc%3!=0) {
		post("multiple of 3 floats required");
		return;
	}
			
	for(i=0; i*3<argc; ++i) {
		if (i >= MAXRESONANCES) {
			post("¥ resonators~: warning: list has more than %ld resonances; dropping extras",
				 MAXRESONANCES);
			break;
		} else {
		
			float f = atom_getfloatarg(i*3,argc,argv);
			float g = 	atom_getfloatarg(i*3+1,argc,argv);
			float rate = atom_getfloatarg(i*3+2,argc,argv);
			float r;
	//	expf(g*0.1151292546497f)



			r =  expf(-rate*srbar);
			if((f<=0.0f) || (f>=(0.995f*x->samplerate*0.5f)) || (r<=0.0f) || (r>1.0f))
			{
	//				post("Warning parameters out of range");
	#ifdef NOOVERLAP
				ta1[i] = 0.0f;
				tb1[i] = 0.0f;
				tb2[i] = 0.0f;
	#else
				fp[i].b1 = fp[i].b1 = 0.0f;
	#endif
				fp[i].a1prime = 0.0f;
				fp[i].fastr = 0.0f;
			}
			else
			{
				f *= 2.0f*3.14159265358979323f*srbar;
				tb1[i] = r*cosf(f)*2.0f;
				fp[i].a1prime = g*sinf(f)/tb1[i];
				ta1[i] = g*  sinf(f)*  (1.0f-r);
				tb2[i] =  -r*r;
				fp[i].fastr = expf(-rate*100.0f*srbar)/r;
			}
		}
	}
	/* Now we know how many "good" resonances (freq > 0) were in the list */
	nres = i;

	/* If there are now fewer resonances than there were: */ 
	for(i=x->nres; i<nres; ++i) {
		fp[i].out1 = fp[i].out2 = 0.0f;
	}

	x->nres = nres;


	for(i=0;i<x->nres;++i)
	{
			fp[i].b1 =tb1[i];
			fp[i].a1 = ta1[i];
			fp[i].b2 =  tb2[i];
	}
//		post("nres %d x->nres %d", nres, x->nres);
}

void resonators_assist(t_resonators *x, void *b, long m, long a, char *s)
{
	assist_string(3216,m,a,1,2,s);
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

void *resonators_new(t_symbol *s, short argc, t_atom *argv)
{
    t_resonators *x = (t_resonators *)newobject(resonators_class);
		x->samplerate =  sys_getsr();
		if(x->samplerate<=0.0f)
			x->samplerate = 44100.0f;
		x->sampleinterval = 1.0f/x->samplerate;

    x->nres = MAXRESONANCES;
    resonators_clear(x); // clears state
    x->nres = 0;

    {
    	x->interpolating = false;
    	
	    if(argc>=1)
	    {
	    	if(isthesymbol("smooth", argv))
	    	{
	    		argc--; argv++;
	    		x->interpolating = true;
	    	}
	    }
    }

    resonators_list(x,s,argc,argv);
    {
    	resdesc *f = x->base;
		int i;
		for(i=0;i<x->nres;++i)
		{
			f[i].o_a1 = f[i].a1;
			f[i].o_b1 = f[i].b1;
			f[i].o_b2 = f[i].b2;
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
		
    outlet_new((t_object *)x, "signal");
    return (x);
}


void main(void)
{
	setup((t_messlist **)&resonators_class, (method) resonators_new, (method)dsp_free, (short)sizeof(t_resonators),
		0L, A_GIMME, 0);

    post(VERSION);
	post("Copyright © 1986,1987 Adrian Freed");
	post("Copyright © 1996,1997,1998,1999,2000,2001,2002 Regents of the University of California.");
	post("Maximum number of resonances: %d", MAXRESONANCES);
	post("Never expires");
	
	addmess((method)resonators_dsp, "dsp", A_CANT, 0);
	addmess((method)resonators_list, "list", A_GIMME, 0);
	addmess((method)resonators_clear, "clear", 0);
	addmess((method)resonators_squelch, "squelch", 0);
	addfloat((method)resonators_float);
	addint((method)resonators_int);
	addmess((method)resonators_assist, "assist", A_CANT, 0);
	addmess((method)resonators_tellmeeverything, "tellmeeverything", 0);
	dsp_initclass();
	rescopy('STR#',3216);
}

void resonators_tellmeeverything(t_resonators *x) {
	int i;
	
	post(VERSION);
	post("  Compiled " __DATE__ " " __TIME__);
/*	if (x->interpolating) {
		post("  Smooth mode: parameter changes interpolated over time");
	} else {
		post("  Fast mode: no interpolation, more efficient");
	} */
	post("  Max resonances: %d, currently computing %d", MAXRESONANCES, x->nres);
}