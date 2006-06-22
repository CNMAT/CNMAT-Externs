/*
DESCRIPTION: finite impulse response (all zeros) filter, implemented in the time domain (i.e., inefficient and minimal latency).  Probably obsoleted by MSP's buffir~ object.
*/


#include "ext.h"
#include "ext_common.h"
#include "z_dsp.h"

#define MAXSIZE 2000
#define RES_ID 6668

typedef struct _fir
{
    t_pxobject f_obj;

    int f_length;
    t_float f_coefs[MAXSIZE]; // filter coefficients
    t_float f_ff[MAXSIZE]; // feed forward (delay line of incoming samples)
} t_fir;

void fir_bang(t_fir *x);
void fir_clear(t_fir *x);
void fir_set(t_fir *x, Symbol *s, int ac, Atom *av);
void fir_assist(t_fir *x, void *b, long m, long a, char *s);
void *fir_new(long n);

t_int *fir_perform(t_int *w);
void fir_dsp(t_fir *x, t_signal **sp);

void *fir_class;

void main(void)
{
	setup(&fir_class, fir_new, (method)dsp_free, (short)sizeof(t_fir), 0L, A_LONG, 0);
	addmess((method)fir_dsp, "dsp", A_CANT, 0);
	addbang((method)fir_bang);
	addmess((method)fir_set, "set", A_GIMME, 0);
	addmess((method)fir_clear, "clear", 0);
	addmess((method)fir_assist, "assist", A_CANT, 0);
	dsp_initclass();
	rescopy('STR#',RES_ID);
}

void fir_bang(t_fir *x)
{
	//nothing
}

void fir_clear(t_fir *x)
{
	t_float *ff = x->f_ff;
	set_zero(ff, MAXSIZE);
}

void fir_set(t_fir *x, Symbol *s, int ac, Atom *av)
{
	int i, j;
	int m = MIN(MAXSIZE, ac);
	t_float *coefs = x->f_coefs;
	set_zero(coefs, MAXSIZE);
	for (i=j=0; i < m; i++) {
		if (av[i].a_type == A_FLOAT) {
			*coefs++ = av[i].a_w.w_float;
			j++;
		}
		else if (av[i].a_type == A_LONG) {
			*coefs++ = (float)av[i].a_w.w_long;
			j++;
		}
	}
	//x->f_length = j;
}

void fir_assist(t_fir *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

void *fir_new(long n)
{
	int i;
	
	t_fir *x = (t_fir *)newobject(fir_class);
	t_float *coefs = x->f_coefs;
	t_float *ff = x->f_ff;
	
	dsp_setup((t_pxobject *)x, 1);
	outlet_new((t_object *)x, "signal");
	
	x->f_length = n + 1;
	set_zero(coefs, MAXSIZE);
	set_zero(ff, MAXSIZE);
	return (x);
}

void fir_dsp(t_fir *x, t_signal **sp)
{
    dsp_add(fir_perform, 4, x, sp[0]->s_vec-1, sp[1]->s_vec-1, sp[0]->s_n+1);
}

t_int *fir_perform(t_int *w)
{
    t_fir *x = (t_fir *)(w[1]);
    t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
    int n = (int)(w[4]);
    
    int mm = x->f_length+1;
    t_float *coefs = x->f_coefs-1;
    t_float *ff = x->f_ff-1;
    float *xc;
    float *xf;
    float xn, a, temp;
	int m;
	
	if (x->f_obj.z_disabled)
		goto out;
		
	while (--n) {
		xc = coefs;
		xf = ff;
		xn = *++in;
		*++out = 0.;
		m = mm;
		while (--m) {
			temp = *++xf;
			a = *++xc;
			*out += xn * a;
			*xf = xn;
			xn = temp;
		}
	}

out:
	return (w+5);
}
