/*
DESCRIPTION: Dudas' hack MSP port of IRCAM's copyrighted FOF implementation
*/

/*
 * FOF implementation based on IIR representation
 * by Gerhard Eckel and Francisco Iovino, Feb 1994
 * (c) 1994 IRCAM
 */
 
 // MSP version Dudas Hack 
 //
 // Autumn 98
 // Switched the amp and bw inlets
 //   it just makes more sense that way
 // made a no-sig-inputs perform method
 //
 // Summer 1998
 // changed max number of fofs to 256 just to give us some growing room
 //   (originally 40 - the max that an ispw processor could handle)
 //   this definition is found in foflib.h
 // commented out fof_error() in foflib.c - 
 //   we don't want to do unnecessary printing at audio interrupt!
 //   also commented out error() in fof_error just in case!!!
 // removed old int and float messages (old shortcuts for maxfofs and start)
 

#define VERSION "fof~: version 0.2 for MSP (eckel/iovino/dudas)"
#define RES_ID 19991

#include "ext.h"
#include "z_dsp.h"

#include "foflib.c"

/*
// in foflib.h 
typedef struct _sigfofs {
    t_pxobject x_obj;
    t_fof *ctlp;
    t_fof ctl;
    t_sample cf, bw, amp, tex, debatt, atten;
    short b_connected[NUM_INLETS];
} t_sigfofs;*/

static void fof_error(void);
void sigfofs_start(t_sigfofs *x, float p);
void sigfofs_phase(t_sigfofs *x, t_sample v);
void sigfofs_bang(t_sigfofs *x);
void sigfofs_maxfofs(t_sigfofs *x, long v);
void sigfofs_f0_phase(t_sigfofs *x, t_sample v);

void sigfofs_float(t_sigfofs *x, t_sample v);
void sigfofs_int(t_sigfofs *x, long v);
void sigfofs_f0(t_sigfofs *x, t_sample v);
void sigfofs_cf(t_sigfofs *x, t_sample v);
void sigfofs_bw(t_sigfofs *x, t_sample v);
void sigfofs_amp(t_sigfofs *x, t_sample v);
void sigfofs_tex(t_sigfofs *x, t_sample v);
void sigfofs_debatt(t_sigfofs *x, t_sample v);
void sigfofs_atten(t_sigfofs *x, t_sample v);
void sigfofs_phase(t_sigfofs *x, t_sample v);


void sigfofs_input(t_int *w, long m);
t_int *sigfofs_perform(t_int *w);
void sigfofs_dsp(t_sigfofs *x, t_signal **sp, short *count);
float sigfofs_get_value(Atom *a);
void sigfofs_assist(t_sigfofs *x, void *b, long m, long a, char *s);
void sigfofs_init_args(t_sigfofs *x, int ac, Atom *av);
void *sigfofs_new(Symbol *s, int ac, Atom *av);

void *sigfofs_class;

void main(void)
{
    setup(&sigfofs_class, sigfofs_new, (method)dsp_free, (short)sizeof(t_sigfofs), 0, A_GIMME, 0);
    addmess((method)sigfofs_dsp, "dsp", A_CANT, 0);
    addbang((method)sigfofs_bang);
    addfloat((method)sigfofs_float);
    addint((method)sigfofs_int);
	
    addmess((method)sigfofs_maxfofs,"maxfofs", A_LONG, 0);
    addmess((method)sigfofs_start, "start", A_FLOAT, 0);
    addmess((method)sigfofs_phase, "fof-phase", A_FLOAT, 0);
    addmess((method)sigfofs_f0_phase, "f0-phase", A_FLOAT, 0);

	addmess((method)sigfofs_assist,"assist",A_CANT,0);
	dsp_initclass();
	
	rescopy('STR#',RES_ID);
    post(VERSION);

}

// fof_error is called when we run out of fofs, to avoid too much printing
// we sample the output down

static int time = 0;
static count = 0;
static void fof_error(void)
{
    count++;
    if (gettime() - time > 100) {
	//post("fof~: error! out of fofs (%d @ %d)", count, gettime());
	count = 0;
	time = gettime();
    }
}

//sigfofs_start triggers a fof at p seconds after next block begin
void sigfofs_start(t_sigfofs *x, float p)
{
    long offset;
    t_fof_real delta;
    
    p *= 0.001;

    offset = p * x->ctlp->params.sr;

    delta = p - (offset * x->ctlp->params.sp);
    fof_trigger(x->ctlp, offset, delta);
}

void sigfofs_phase(t_sigfofs *x, t_sample v)
{
    v = v - (int)v;
    if (v < 0)
        v += 1;
    x->ctlp->inputs.phase = v * x->ctlp->params.twopi;
    x->ctlp->inputs.changed = 1;
}

//sigfofs_bang triggers a fof at next block begin
void sigfofs_bang(t_sigfofs *x)
{
    sigfofs_start(x, 0.);
}

//sigfofs_maxfofs sets the maximum number of fofs
void sigfofs_maxfofs(t_sigfofs *x, long v)
{
    if (v > MAX_FOFS) {
	post("fof~: warning!maxfofs out of range (%d), set to %d", v, MAX_FOFS);
	v = MAX_FOFS;
    }
    if (v < 1)
	v = 1;
    x->ctlp->list.maxfofs = v;
    fof_init_list(&(x->ctlp->list));
}

//sigfofs_f0_phase sets the initial phase of the fof (0 <= v < 1)
void sigfofs_f0_phase(t_sigfofs *x, t_sample v)
{
    if (v < 0)
        v = -v;
    x->ctlp->f0_phase = x->ctlp->params.sr - v;
}

// sigfofs_cf, sigfofs_bw, sigfofs_amp, sigfofs_tex, sigfofs_debatt,
// and sigfofs_atten set the fof parameters

void sigfofs_float(t_sigfofs *x, t_sample v)
{    
	long in = x->x_obj.z_in;

	//if (!in)
	//	return;
	switch (in) {
		//case 0: sigfofs_start(x,(float)v); break;
		case 0: sigfofs_f0(x,v); break;
		case 1: sigfofs_cf(x,v); break;
		case 2: sigfofs_amp(x,v); break;
		case 3: sigfofs_bw(x,v); break;
		case 4: sigfofs_tex(x,v); break;
		case 5: sigfofs_debatt(x,v); break;
		case 6: sigfofs_atten(x,v); break;
	}
}

void sigfofs_int(t_sigfofs *x, long v)
{    
	sigfofs_float(x, (t_sample)v);
}

void sigfofs_f0(t_sigfofs *x, t_sample v)
{    
    if (v < 0)
    	v = 0;
    if (v > x->ctlp->params.sro2)
    	v = x->ctlp->params.sro2;
    x->ctlp->inputs.f0 = v;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_cf(t_sigfofs *x, t_sample v)
{    
    if (v < 0)
    	v = 0;
    if (v > x->ctlp->params.sro2)
    	v = x->ctlp->params.sro2;
    x->ctlp->inputs.cf = v;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_bw(t_sigfofs *x, t_sample v)
{
    if (v < 0)
    	v = 0;
    if (v > x->ctlp->params.sro2)
    	v = x->ctlp->params.sro2;
    x->ctlp->inputs.bw = v;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_amp(t_sigfofs *x, t_sample v)
{
    x->ctlp->inputs.amp = v;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_tex(t_sigfofs *x, t_sample v)
{
    if (v < 0)
    	v = 0;
    x->ctlp->inputs.tex = v * 0.001;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_debatt(t_sigfofs *x, t_sample v)
{
    if (v < 0)
    	v = 0;
    x->ctlp->inputs.debatt = v * 0.001;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_atten(t_sigfofs *x, t_sample v)
{
    if (v < 0)
    	v = 0;
    x->ctlp->inputs.atten = v * 0.001;
    x->ctlp->inputs.changed = 1;
}

void sigfofs_input(t_int *w, long m)
{
    t_sigfofs *x = (t_sigfofs *)(w[1]);
    t_sample *in = (t_float *)(w[2]);
	t_sample *cf = (t_float *)(w[3]);
	t_sample *amp = (t_float *)(w[4]);
	t_sample *bw = (t_float *)(w[5]);
	t_sample *tex = (t_float *)(w[6]);
	t_sample *debatt = (t_float *)(w[7]);
	t_sample *atten = (t_float *)(w[8]);
    
    t_sample v;
	
    if (x->b_connected[0] && cf[m] != x->cf) {
	v = cf[m];
	if (v < 0)
	    v = -v;
	if (v > x->ctlp->params.sro2)
	    v = x->ctlp->params.sro2;
	x->cf = x->ctlp->inputs.cf = v;
	x->ctlp->inputs.changed = 1;
    }
    if (x->b_connected[1] && amp[m] != x->amp) {
	x->amp = x->ctlp->inputs.amp = amp[m];
	x->ctlp->inputs.changed = 1;
    }
    if (x->b_connected[2] && bw[m] != x->bw) {
	v = bw[m];
	if (v < 0)
	    v = -v;
	if (v > x->ctlp->params.sro2)
	    v = x->ctlp->params.sro2;
	x->bw = x->ctlp->inputs.bw = v;
	x->ctlp->inputs.changed = 1;
    }
    if (x->b_connected[3] && tex[m] != x->tex) {
	v = tex[m];
	if (v < 0)
	    v = 0;
	x->tex = v;
	x->ctlp->inputs.tex = v * 0.001;
	x->ctlp->inputs.changed = 1;
    }
    if (x->b_connected[4] && debatt[m] != x->debatt) {
	v = debatt[m];
	if (v < 0)
	    v = 0;
	x->debatt = v;
	x->ctlp->inputs.debatt = v * 0.001;
	x->ctlp->inputs.changed = 1;
    }
    if (x->b_connected[5] && atten[m] != x->atten) {
	v = atten[m];
	if (v < 0)
	    v = 0;
	x->atten = v;
	x->ctlp->inputs.atten = v * 0.001;
	x->ctlp->inputs.changed = 1;
    }
}

t_int *sigfofs_perform(t_int *w)
//(t_sigfofs *x, t_sample *in, t_sample *out, long n)
{
    t_sigfofs *x = (t_sigfofs *)(w[1]);
    t_sample *in = (t_float *)(w[2]);
    t_sample *out = (t_float *)(w[9]);
    t_sample *o = out;
    int n = (int)(w[10]);
    int i = n, m;
		
    t_fof_real sr = x->ctlp->params.sr;
    t_fof_real p = x->ctlp->f0_phase;
    
    if (x->x_obj.z_disabled)
		goto out;
		
    while (i--) {
	p += *in++;
	if (p >= sr) {
	    p -= sr;
	    m = n - i - 1;
	    sigfofs_input(w, m);
	    fof_trigger(x->ctlp, m, p / (*(in - 1) * sr));
	}
	*o++ = 0;
    }
    x->ctlp->f0_phase = p;
    fof_run_all(x, out, n);

out:    
    return (w+11);
}

//the dsp put method, updates sampling rate related values
void sigfofs_dsp(t_sigfofs *x, t_signal **sp, short *connect)
{
	short i;
	Boolean sigversion = FALSE;
    
    x->ctlp->params.sr = sys_getsr();
    x->ctlp->params.sro2 = x->ctlp->params.sr * 0.5;
    x->ctlp->params.sp = 1 / x->ctlp->params.sr;
    x->ctlp->params.piosr = x->ctlp->params.pi * x->ctlp->params.sp;

	for (i = 0; i < 6; i++) // 6 control inlets
		if (x->b_connected[i] = connect[i+1])
			sigversion = TRUE;
    
	if (sigversion) {
		dsp_add(sigfofs_perform, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, 
		 sp[3]->s_vec, sp[4]->s_vec, sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[0]->s_n);
    }
    else if (connect[0]) {
    	dsp_add(fof_perform, 4, x, sp[0]->s_vec, sp[7]->s_vec, sp[0]->s_n);
    }
    else {
		dsp_add(fof_perform_nosig, 3, x, sp[7]->s_vec, sp[0]->s_n);
    }
    
}

// initialization helper functions

float sigfofs_get_value(Atom *a)
{
    switch (a->a_type) {
	case A_FLOAT: return a->a_w.w_float;
	case A_LONG:  return a->a_w.w_long;
	case A_SYM:   if (*a->a_w.w_sym->s_name == '#') break;
	default:      post("fof~: illegal init argument -ignored");
    }
    return 0;
}

void sigfofs_assist(t_sigfofs *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,8,s);
}

void sigfofs_init_args(t_sigfofs *x, int ac, Atom *av)
{

    long maxfofs = ac > 0 ? sigfofs_get_value(av) : 5;
    
    sigfofs_maxfofs(x, maxfofs);
    if (ac > 1) sigfofs_cf(x, sigfofs_get_value(av + 1));
    if (ac > 2) sigfofs_amp(x, sigfofs_get_value(av + 2));
    if (ac > 3) sigfofs_bw(x, sigfofs_get_value(av + 3));
    if (ac > 4) sigfofs_tex(x, sigfofs_get_value(av + 4));
    if (ac > 5) sigfofs_debatt(x, sigfofs_get_value(av + 5));
    if (ac > 6) sigfofs_atten(x, sigfofs_get_value(av + 6));
    if (ac > 7) sigfofs_phase(x, sigfofs_get_value(av + 7));
    if (ac > 8) post("fofm~: too many init args - ignoring");
}


void *sigfofs_new(Symbol *s, int ac, Atom *av)
{
    t_sigfofs *x = (t_sigfofs *)newobject(sigfofs_class);
    dsp_setup((t_pxobject *)x,7);
    outlet_new((t_object *)x, "signal");
    x->ctlp = &(x->ctl);
    fof_init(x->ctlp);
    
    sigfofs_init_args(x, ac, av);

    return (x);
}


