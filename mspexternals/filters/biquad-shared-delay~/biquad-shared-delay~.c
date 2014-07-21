/* 
SVN_REVISION: $LastChangedRevision$
DESCRIPTION: this is the "new" biquad test version using shared delays ym1, ym2. still mathematically equivalent to direct form.  it seems slightly more efficient (rounds/truncates differently after the fifth decimal place)
*/


// Windows compile by Michael Zbyszynski 20 April 2004

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

//#define RES_ID 13251

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
	setup((t_messlist **)&biquad_class,biquad_new, (method)dsp_free, (short)sizeof(t_biquad),
		0L, A_GIMME, 0);
	addmess((method)biquad_dsp, "dsp", A_CANT, 0);
	addmess((method)biquad_list, "list", A_GIMME, 0);
	addmess((method)biquad_clear, "clear", 0);
	addfloat((method)biquad_float);
	addint((method)biquad_int);
	addmess((method)biquad_assist, "assist", A_CANT, 0);
	dsp_initclass();
	//rescopy('STR#',RES_ID);
	// assist isn't defined yet
}

t_int *biquad_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_biquad *x = (t_biquad *)(w[3]);
	int n = (int)(w[4]);
	float a0 = x->b_a0, a1 = x->b_a1, a2 = x->b_a2, b1 = x->b_b1, b2 = x->b_b2;
	float yn,xn,ym1 = x->b_ym1, ym2 = x->b_ym2;

	while (--n) {
		xn = *++in;
		yn = xn - (b1 * ym1) - (b2 * ym2);
		*++out = (a0 * yn) + (a1 * ym1) + (a2 * ym2);
		ym2 = ym1;
		ym1 = yn;
	}
	x->b_ym1 = ym1;
	x->b_ym2 = ym2;
	return (w+5);
}

t_int *biquad2_perform(t_int *w)
{
	float a0,a1,a2,b1,b2,yn,xn,ym1,ym2,xm1,xm2;
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_biquad *x = (t_biquad *)(w[3]);
	int n = (int)(w[4]);

	if (x->b_obj.z_disabled)
		return (w+10);
	a0 = x->b_connected[FF0]? *(float *)(w[5]) : x->b_a0;
	a1 = x->b_connected[FF1]? *(float *)(w[6]) : x->b_a1;
	a2 = x->b_connected[FF2]? *(float *)(w[7]) : x->b_a2;
	b1 = x->b_connected[FB1]? *(float *)(w[8]) : x->b_b1;
	b2 = x->b_connected[FB2]? *(float *)(w[9]) : x->b_b2;
	ym1 = x->b_ym1;
	ym2 = x->b_ym2;


	while (--n) {
		xn = *++in;
		yn = xn - (b1 * ym1) - (b2 * ym2);
		*++out = (a0 * yn) + (a1 * ym1) + (a2 * ym2);
		ym2 = ym1;
		ym1 = yn;
	}
	x->b_ym1 = ym1;
	x->b_ym2 = ym2;
	return (w+10);
}

// this is done every time the filter is restarted, in case you blow it up

void biquad_clear(t_biquad *x)
{
	x->b_ym1 = 0.;
	x->b_ym2 = 0.;
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
		dsp_add(biquad2_perform, 9, sp[0]->s_vec-1, sp[6]->s_vec-1, x, sp[0]->s_n + 1,
			sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec);
	else {
		dsp_add(biquad_perform, 4, sp[0]->s_vec-1, sp[6]->s_vec-1, x, sp[0]->s_n + 1);
	}
}

void biquad_float(t_biquad *x, double f)
{
	long in = x->b_obj.z_in;

	if (!in)
		return;
	in--;
	switch (in) {
		case FF0: x->b_a0 = f; break;
		case FF1: x->b_a1 = f; break;
		case FF2: x->b_a2 = f; break;
		case FB1: x->b_b1 = f; break;
		case FB2: x->b_b2 = f; break;
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
			case FF0: x->b_a0 = atom_getfloatarg(i,argc,argv); break;
			case FF1: x->b_a1 = atom_getfloatarg(i,argc,argv); break;
			case FF2: x->b_a2 = atom_getfloatarg(i,argc,argv); break;
			case FB1: x->b_b1 = atom_getfloatarg(i,argc,argv); break;
			case FB2: x->b_b2 = atom_getfloatarg(i,argc,argv); break;
		}
	}
}

void biquad_assist(t_biquad *x, void *b, long m, long a, char *s)
/*{
	assist_string(RES_ID,m,a,1,7,s);
}*/

void *biquad_new(t_symbol *s, short argc, t_atom *argv)
{
    t_biquad *x = (t_biquad *)newobject(biquad_class);
    dsp_setup((t_pxobject *)x,6);
    outlet_new((t_object *)x, "signal");
    x->b_a0 = x->b_a1 = x->b_a2 = x->b_b1 = x->b_b2 = 0.;
    biquad_clear(x);
    biquad_list(x,s,argc,argv);
    return (x);
}

