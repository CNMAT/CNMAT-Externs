#include "ext.h"
#include "z_dsp.h"
#include <math.h>

// the lores object. A low pass controllable with freq and res

void *lores_class;

typedef struct _lores
{
	t_pxobject l_obj;
	float l_freq;			// stored cutoff frequency in Hz
	float l_r;				// stored resonance (0-1)
	float l_a1;				// computed coefficient
	float l_a2;				// computer coefficient
	float l_ym1;			// previous output sample
	float l_ym2;			// previous to previous output sample
	float l_fqterm;			// computed frequency term
	float l_resterm;		// computer resonance term
	float l_2pidsr;			// stored value of 2 pi over the sampling rate
	short l_rcon;			// is a signal connected to the resonance inlet 
	short l_fcon;			// is a signal connected to the frequency inlet
} t_lores;

t_int *lores_perform(t_int *w);
void lores_dsp(t_lores *x, t_signal **sp, short *count);
void lores_int(t_lores *x, long n);
void lores_float(t_lores *x, double f);
void lores_calc(t_lores *x);
void lores_clear(t_lores *x);
void *lores_new(double freq, double reso);
void lores_assist(t_lores *x, void *b, long m, long a, char *s);

int errno;

void main(void)
{
	setup((t_messlist **)&lores_class,(method)lores_new, (method)dsp_free, 
		(short)sizeof(t_lores), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);
	addmess((method)lores_dsp, "dsp", A_CANT, 0);
	addmess((method)lores_assist, "assist", A_CANT, 0);
	addmess((method)lores_clear, "clear", 0);
	addint((method)lores_int);
	addfloat((method)lores_float);
	dsp_initclass();
	rescopy(FOUR_CHAR_CODE('STR#'),3325);
}

void lores_assist(t_lores *x, void *b, long m, long a, char *s)
{
	assist_string(3325,m,a,1,4,s);
}

t_int *lores_perform(t_int *w)
{
	// assign from parameters
    t_float *in = (t_float *)(w[1]);
    t_float *out = (t_float *)(w[2]);
    t_lores *x = (t_lores *)(w[3]);
    t_float freq = x->l_fcon? *(float *)(w[4]) : x->l_freq;
    t_float resonance = x->l_rcon? *(float *)(w[5]) : x->l_r;
    int n = (int)(w[6]);
    float a1 = x->l_a1,a2 = x->l_a2, ym1 = x->l_ym1, ym2 = x->l_ym2;
    float val,scale,r,temp,resterm;
    
    if (x->l_obj.z_disabled)
    	goto out;
    	
    // check signal value with resonance
    
   	if (x->l_rcon) {
   		if (resonance >= 1.)
    		resonance = 1. - 1E-20;
   		else if (resonance < 0.)
    		resonance = 0.;
    }
    
    // do we need to recompute coefficients?
    
    if (freq != x->l_freq || resonance != x->l_r) {
    	if (resonance != x->l_r)
    		resterm = x->l_resterm = exp(resonance * 0.125) * .882497;
    	else
    		resterm = x->l_resterm;
    	if (freq != x->l_freq)
    		x->l_fqterm = cos(x->l_2pidsr * freq);
    	x->l_a1 = a1 = -2. * resterm * x->l_fqterm;
    	x->l_a2 = a2 = resterm * resterm;
    	x->l_r = resonance;
    	x->l_freq = freq;
    }
    
    scale = 1. + a1 + a2;
    
    // DSP loop
    
    while (--n)
    {
    	val = *++in;
    	temp = ym1;
    	ym1 = scale * val - a1 * ym1 - a2 * ym2;
    	ym2 = temp;
    	*++out = ym1;
    }
    x->l_ym1 = ym1;
    x->l_ym2 = ym2;
out:
    return (w+7);
}

void lores_dsp(t_lores *x, t_signal **sp, short *count)
{
	x->l_2pidsr = (2. * PI) / sp[0]->s_sr;
	lores_calc(x);
	x->l_fcon = count[1];		// signal connected to the frequency inlet?
	x->l_rcon = count[2];		// signal connected to the resonance inlet?
	lores_clear(x);

	dsp_add(lores_perform, 6, sp[0]->s_vec-1, sp[3]->s_vec-1, x, 
		sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n+1);
}

void lores_int(t_lores *x, long n)
{
	lores_float(x,(double)n);
}

void lores_float(t_lores *x, double f)
{
	
	if (x->l_obj.z_in == 1) {
		x->l_freq = f;
		lores_calc(x);
	} else if (x->l_obj.z_in == 2) {
		x->l_r = f >= 1.0 ? 1 - 1E-20 : f;
		lores_calc(x);
	}
}

void lores_clear(t_lores *x)
{
	x->l_ym1 = x->l_ym2 = 0.;		// clear sample memory to recover from blowup
}

void lores_calc(t_lores *x)
{
	float bl,resterm;
	
	// calculate filter coefficients from frequency and resonance
	
	resterm = exp(x->l_r * 0.125) * 0.882497;
	x->l_fqterm = cos(x->l_2pidsr * x->l_freq);
	x->l_a1 = -2. * resterm * x->l_fqterm;
	x->l_a2 = resterm * resterm;
	x->l_resterm = resterm;
}

void *lores_new(double val, double reso)
{
    t_lores *x = (t_lores *)newobject(lores_class);
    dsp_setup((t_pxobject *)x,3);

	// three signal inlets
	
    x->l_freq = val;
    x->l_r = reso >= 1.0 ? 1. - 1E-20 : reso;
    x->l_2pidsr = (2. * PI) / sys_getsr();
    lores_calc(x);
    
    // one signal outlet
    
    outlet_new((t_object *)x, "signal");
    
    return (x);
}
