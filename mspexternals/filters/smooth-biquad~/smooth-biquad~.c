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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: smooth-biquad~
DESCRIPTION: smooth-biquad~ is just like biquad~ except that new coefficient updates are "smoothed" to avoid clicks and other undesirable artifacts: the object linearly interpolates the coefficients from the old value to the new value over one MSP signal processing vector.
AUTHORS: Tristan Jehan, Matt Wright
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
DRUPAL_NODE: /patch/4007
SVN_REVISION: $LastChangedRevision$
VERSION 1.3alpha: Adrian's initial version
VERSION 1.4: Rough windows compile by mzed, 20 April 2004
VERSION 1.4.1: Better compile 27 April 2004, mzed
VERSION 1.5: CFM/MachO compile, proper version info.
VERSION 1.5.1: Force Package Info Generation
VERSION 1.5.2: Denormals squashed
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/


#include "ext.h"
#include "z_dsp.h"
// #include <math.h>

#include "version.h"
#include "version.c"

#define SQUASH_DENORMALS
#ifdef WINDOWS
#include <xmmintrin.h>
#else
#include <fenv.h>
#pragma STDC FENV_ACCESS ON
#endif

void *biquad_class;

enum {
	FF0 = 0,
	FF1,
	FF2,
	FB1,
	FB2,
	NUM_COEFFICIENTS = 5
};

typedef struct _biquad
{
	t_pxobject b_obj;
	float b_ym1;
	float b_ym2;
	float b_xm1;
	float b_xm2;
	float a0,a1,a2,b1,b2; // desired values
	float b_a0;		// gain
	float b_a1;		// feedforward 1
	float b_a2;		// feedforward 2
	float b_b1;		// feedback 1
	float b_b2;		// feedback 2
	short b_connected[NUM_COEFFICIENTS];  // are coef signals connected?
} t_biquad;

t_int *biquad_perform(t_int *w);
t_int *biquad2_perform(t_int *w);
void biquad_dsp(t_biquad *x, t_signal **sp, short *connect);
void biquad_float(t_biquad *x, double f);
void biquad_int(t_biquad *x, long n);
void biquad_list(t_biquad *x, t_symbol *s, short argc, t_atom *argv);
void biquad_clear(t_biquad *x);
void biquad_assist(t_biquad *x, void *b, long m, long a, char *s);
void *biquad_new(t_symbol *s, short argc, t_atom *argv);

void main(void)
{
	setup((t_messlist **)&biquad_class, (method)biquad_new, (method)dsp_free, 
		  (short)sizeof(t_biquad), 0L, A_GIMME, 0);
	post("smooth-biquad~ 1.3Alpha- Adrian Freed");
	post("Copyright ©1999 Regents of the University of California.");
	post("Never expires");

	addmess((method)biquad_dsp, "dsp", A_CANT, 0);

	addmess((method)biquad_list, "list", A_GIMME, 0);
	addmess((method)biquad_clear, "clear", 0);
	addfloat((method)biquad_float);
	addint((method)biquad_int);
	addmess((method)biquad_assist, "assist", A_CANT, 0);
	addmess((method)version, "version", 0);
	dsp_initclass();
	version(0);
}

t_int *biquad_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_biquad *x = (t_biquad *)(w[3]);
	int n = ((int)(w[4]));
	int i;
	float rate = 1.0f/n;
	float a0 = x->b_a0, a1 = x->b_a1, a2 = x->b_a2, b1 = x->b_b1, b2 = x->b_b2;
	float y1 = x->b_ym1, y0 = x->b_ym2, i3 = x->b_xm1, i2 = x->b_xm2;
	float a0inc = (x->a0-x->b_a0) *  rate;
	float a2inc = (x->a2-x->b_a2) *  rate;
	float a1inc = (x->a1-x->b_a1) *  rate;
	float b1inc = (x->b1-x->b_b1) *  rate;
	float b2inc = (x->b2-x->b_b2) *  rate;
	  float o0, o1, o2, o3;
	  float i0,i1;


#ifdef SQUASH_DENORMALS
#ifdef WINDOWS
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON)
#else
	fenv_t oldEnv;
	//Read the old environment and set the new environment using default flags and denormals off
	fegetenv( &oldEnv );
	fesetenv( FE_DFL_DISABLE_SSE_DENORMS_ENV );
#endif
#endif
	for(i=0;i<n;i+=4)
	{
		out[i] = y0 = (a0 * (i0 = in[i])) + (a1 * i3) + (a2 * i2) - (b1 * y1) - (b2 * y0);
		a1 += a1inc;
		a2 += a2inc;
		a0 += a0inc;
		b1 += b1inc;
		b2 += b2inc;
		out[i+1] = y1 = (a0 * (i1 = in[i+1])) + (a1 * i0) + (a2 * i3) - (b1 * y0) - (b2 * y1);
		a1 += a1inc;
		a2 += a2inc;
		a0 += a0inc;
		b1 += b1inc;
		b2 += b2inc;
		out[i+2] = y0 = (a0 * (i2 = in[i+2])) + (a1 * i1) + (a2 * i0) - (b1 * y1) - (b2 * y0);
		a1 += a1inc;
		a2 += a2inc;
		a0 += a0inc;
		b1 += b1inc;
		b2 += b2inc;
		out[i+3] = y1 = (a0 *  (i3 = in[i+3])) + (a1 * i2) + (a2 * i1) - (b1 * y0) - (b2 * y1);
		a1 += a1inc;
		a2 += a2inc;
		a0 += a0inc;
		b1 += b1inc;
		b2 += b2inc;
	}

#ifdef SQUASH_DENORMALS
#ifdef WINDOWS
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF)
#else
	fesetenv( &oldEnv );
#endif
#endif
	
	x->b_ym1 = y1;
	x->b_ym2 = y0;
	x->b_xm1 = i3;
	x->b_xm2 = i2;
	x->b_a0 = x->a0;
	x->b_a1 = x->a1;
	x->b_a2 = x->a2;
	x->b_b1 = x->b1;
	x->b_b2 = x->b2;
	return (w+5);
}

t_int *biquad2_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_biquad *x = (t_biquad *)(w[3]);
	int n = (int)(w[4]);
	int i;
	float rate = 1.0f/n;
	float a0 = x->b_a0, a1 = x->b_a1, a2 = x->b_a2, b1 = x->b_b1, b2 = x->b_b2;
	float yn,xn,ym1 = x->b_ym1, ym2 = x->b_ym2, xm1 = x->b_xm1, xm2 = x->b_xm2;
	float a0inc, a1inc,a2inc, b1inc,b2inc;
	if (x->b_obj.z_disabled)
		return (w+10);
	if(x->b_connected[FF0])
		x->a0= *(float *)(w[5]) ;
	if(x->b_connected[FF2])
		x->a2= *(float *)(w[7]) ;
	if(x->b_connected[FF1])
		x->a1= *(float *)(w[6]) ;
	if(x->b_connected[FB1])
		x->b1= *(float *)(w[8]) ;
	if(x->b_connected[FB2])
		x->b2= *(float *)(w[9]) ;
	a0inc = (x->a0-x->b_a0) *  rate;
	a2inc = ( x->a2-x->b_a2) *  rate;
	a1inc = (x->a1-x->b_a1) *  rate;
	b1inc = (x->b1-x->b_b1) *  rate;
	b2inc = ( x->b2-x->b_b2) *  rate;

#ifdef SQUASH_DENORMALS	
#ifdef WINDOWS
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON)
#else
	fenv_t oldEnv;
	//Read the old environment and set the new environment using default flags and denormals off
	fegetenv( &oldEnv );
	fesetenv( FE_DFL_DISABLE_SSE_DENORMS_ENV );
#endif
#endif
	
	for(i=0;i<n;++i)
	{
		xn = in[i];
		out[i] = yn = (a0 * xn) + (a1 * xm1) + (a2 * xm2) - (b1 * ym1) - (b2 * ym2);
		xm2 = xm1;
		xm1 = xn;
		ym2 = ym1;
		ym1 = yn;
		a1 += a1inc;
		a2 += a2inc;
		a0 += a0inc;
		b1 += b1inc;
		b2 += b2inc;
	}

#ifdef SQUASH_DENORMALS
#ifdef WINDOWS
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF)
#else
	fesetenv( &oldEnv );
#endif
#endif
	
	x->b_ym1 = ym1;
	x->b_ym2 = ym2;
	x->b_xm1 = xm1;
	x->b_xm2 = xm2;
	x->b_a0 = x->a0;
	x->b_a1 = x->a1;
	x->b_a2 = x->a2;
	x->b_b1 = x->b1;
	x->b_b2 = x->b2;
	return (w+10);
}

// this is done every time the filter is restarted, in case you blow it up

void biquad_clear(t_biquad *x)
{
	x->b_ym1 = 0.f;
	x->b_ym2 = 0.f;
	x->b_xm1 = 0.f;
	x->b_xm2 = 0.f;
}

void biquad_dsp(t_biquad *x, t_signal **sp, short *connect)
{
	short i;
	Boolean sigversion = FALSE;

	biquad_clear(x);
	for (i = 0; i < 5; i++)
		if (x->b_connected[i] = connect[i+1])
			sigversion = TRUE;

	if (sigversion)
		dsp_add(biquad2_perform, 9, sp[0]->s_vec, sp[6]->s_vec, x, sp[0]->s_n,
			sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec);
	else {
		dsp_add(biquad_perform, 4, sp[0]->s_vec, sp[6]->s_vec, x, sp[0]->s_n);
	}
}

void biquad_float(t_biquad *x, double f)
{
	long in = x->b_obj.z_in;

	if (!in)
		return;
	in--;
	switch (in) {
		case FF0: x->a0 = f; break;
		case FF1: x->a1 = f; break;
		case FF2: x->a2 = f; break;
		case FB1: x->b1 = f; break;
		case FB2: x->b2 = f; break;
	}
}

void biquad_int(t_biquad *x, long n)
{
	biquad_float(x,(double)n);
}

// ignores the inlet, uses order to specify the coefficients

void biquad_list(t_biquad *x, t_symbol *s, short argc, t_atom *argv)
{
	short i;

	for (i=0; i < argc; i++) {
		switch (i) {
			case FF0: x->a0 = atom_getfloatarg(i,argc,argv); break;
			case FF1: x->a1 = atom_getfloatarg(i,argc,argv); break;
			case FF2: x->a2 = atom_getfloatarg(i,argc,argv); break;
			case FB1: x->b1 = atom_getfloatarg(i,argc,argv); break;
			case FB2: x->b2 = atom_getfloatarg(i,argc,argv); break;
		}
	}
}

void biquad_assist(t_biquad *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"(signal)output");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(signal) input");
			break;
		case 1:
			sprintf(s,"(signal/float) input gain (FF coeffficient 0");
			break;
		case 2:
			sprintf(s,"(signal/float) FF coeffficient 1");
			break;
		case 3:
			sprintf(s,"(signal/float) FF coeffficient 2");
			break;
		case 4:
			sprintf(s,"(signal/float) FB coeffficient 1");
			break;
		case 5:
			sprintf(s,"(signal/float) FB coeffficient 2");
			break;
		}
	}
}

void *biquad_new(t_symbol *s, short argc, t_atom *argv)
{
    t_biquad *x = (t_biquad *)newobject(biquad_class);
    dsp_setup((t_pxobject *)x,6);
    outlet_new((t_object *)x, "signal");
    x->a0 = x->a1 = x->a2 = x->b1 = x->b2 = 0.0f;
    biquad_clear(x);
    biquad_list(x,s,argc,argv);
  	x->b_a0 = x->a0;
	x->b_a1 = x->a1;
	x->b_a2 = x->a2;
	x->b_b1 = x->b1;
	x->b_b2 = x->b2;  
    return (x);
}

