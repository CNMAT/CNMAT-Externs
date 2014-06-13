#define NAME "localpeak~"
#define DESCRIPTION "Report local maximum sample within given time period, similar to peakamp~ but sample accurate"
#define AUTHORS "Rama Gottfried"
#define COPYRIGHT_YEARS "2013"

#ifdef PD_VERION
#include "m_pd.h"
#else
#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#endif

#include "version.h"



static t_class *localpeak_tilde_class;

typedef struct _localpeak_tilde {
#ifdef PD_VERSION
    t_object    x_obj;
#else
    t_pxobject  x_obj;
#endif
    
    t_sample     ms;
    t_sample     ms2samps;
    t_sample     samps;
    
    t_sample     peak;
    t_sample     block_peak;
    t_sample     phase;
    
    t_sample     sr;
    
} t_localpeak_tilde;

void localpeak_tilde_float(t_localpeak_tilde *x, t_sample f)
{
    x->ms = f ? f : 1000;
    x->samps = (x->ms * x->ms2samps);
    
}

t_int *localpeak_tilde_perform(t_int *w)
{
    t_localpeak_tilde *x = (t_localpeak_tilde *)(w[1]);
    t_sample *in1 = (t_sample *)(w[2]);
    t_sample *out1 = (t_sample *)(w[3]);
    t_sample *out2 = (t_sample *)(w[4]);
    int nsamps = (int)(w[5]);

    t_float pc = x->phase;
    t_float realtime_peak = x->peak;
    t_float samps = x->samps;
    t_float block_peak = x->block_peak;
    
    t_sample s;
    
    while (nsamps--)
    {
        s = *in1++;

        if (pc >= samps)
        {
            block_peak = realtime_peak;
            realtime_peak = 0;
            pc = 0;
        }
        
        realtime_peak = (realtime_peak < s) ? s : realtime_peak;

        *out1++ = realtime_peak;
        *out2++ = block_peak;

        pc++;
    }
    
    x->phase = pc;
    x->block_peak = block_peak;
    x->peak = realtime_peak;
    return (w+6);
}

void localpeak_tilde_dsp(t_localpeak_tilde *x, t_signal **sp)
{
    if(sys_getsr() != x->sr)
    {
        x->sr = sys_getsr();
        x->ms2samps = x->sr * 0.001;
//        error("sample rate changed!");
    }
    
    dsp_add(localpeak_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
    
}

void localpeak_tilde_perform64(t_localpeak_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    
    t_sample *in = ins[0];
    t_sample *out1 = outs[0];
    t_sample *out2 = outs[1];
    long nsamps = sampleframes;
    
    t_sample pc = x->phase;
    t_sample realtime_peak = x->peak;
    t_sample samps = x->samps;
    t_sample block_peak = x->block_peak;
    
    t_sample s;
    
    while (nsamps--)
    {
        s = *in++;
        
        if (pc >= samps)
        {
            block_peak = realtime_peak;
            realtime_peak = 0;
            pc = 0;
        }
        
        realtime_peak = (realtime_peak < s) ? s : realtime_peak;
        
        *out1++ = realtime_peak;
        *out2++ = block_peak;
        
        pc++;
    }
    
    x->phase = pc;
    x->block_peak = block_peak;
    x->peak = realtime_peak;
}

void localpeak_tilde_perform64_2(t_localpeak_tilde *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    
    t_sample *in = ins[0];
    t_sample *in2 = ins[1];
    t_sample *out1 = outs[0];
    t_sample *out2 = outs[1];
    long nsamps = sampleframes;
    t_sample ms2samps = x->ms2samps;
    
    t_sample pc = x->phase;
    t_sample realtime_peak = x->peak;
    t_sample samps = x->samps;
    t_sample block_peak = x->block_peak;
    
    t_sample s, ms, prev_ms;
    
    while (nsamps--)
    {
        s = *in++;
        ms = *in2++;
        
        if(ms != prev_ms)
        {
            samps = ms * ms2samps;
            prev_ms = ms;

        }
        
        if (pc >= samps)
        {
            block_peak = realtime_peak;
            realtime_peak = 0;
            pc = 0;
        }
        
        realtime_peak = (realtime_peak < s) ? s : realtime_peak;
        
        *out1++ = realtime_peak;
        *out2++ = block_peak;
        
        pc++;
    }
    
    x->phase = pc;
    x->block_peak = block_peak;
    x->peak = realtime_peak;
    x->ms = ms;
    x->samps = samps;
}

void localpeak_tilde_dsp64(t_localpeak_tilde *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{

    if(samplerate != x->sr)
    {
        x->sr = samplerate;
        x->ms2samps = x->sr * 0.001;
        //        error("sample rate changed!");
    }
    
    if(count[0] && !count[1])
        object_method(dsp64, gensym("dsp_add64"), x, localpeak_tilde_perform64, 0, NULL);
    else if(count[0] && count[1])
        object_method(dsp64, gensym("dsp_add64"), x, localpeak_tilde_perform64_2, 0, NULL);
    
}


#ifndef PD_VERSION
void *localpeak_tilde_new(t_symbol *s, long argc, t_atom *argv)
{
	t_localpeak_tilde *x = (t_localpeak_tilde *)object_alloc(localpeak_tilde_class);
    
	if (x)
    {
        double f = 0;
        if(argc > 0 && atom_gettype(argv) == A_FLOAT)
            f = atom_getfloat(argv);
        
        x->ms = f ? f : 1000;
        x->sr = sys_getsr();
        x->ms2samps = x->sr * 0.001;
        x->samps =(x->ms * x->ms2samps);
        x->phase = 0;
        x->peak = 0;
        x->block_peak = 0;
        
        // post("%f %f", x->samps, x->ms);
        
        dsp_setup((t_pxobject *)x, 2);
        
        outlet_new(x, "signal");
        outlet_new(x, "signal");
        
    
    }
    return (x);
}

int main(void)
{
	t_class *c = class_new(NAME, (method)localpeak_tilde_new, (method)dsp_free, (long)sizeof(t_localpeak_tilde), NULL,   A_GIMME);
	
	class_addmethod(c, (method)localpeak_tilde_dsp64,   "dsp64",	A_CANT, 0);
    class_addmethod(c, (method)localpeak_tilde_float,   "float",    A_FLOAT, 0);
    
    class_dspinit(c);
	class_register(CLASS_BOX, c);
	localpeak_tilde_class = c;
    
	version_post_copyright();
    
	return 0;

}


#else
void *localpeak_tilde_new(t_floatarg f)
{
    t_localpeak_tilde *x = (t_localpeak_tilde *)pd_new(localpeak_tilde_class);
    
    if(x)
    {
        x->ms = f ? f : 1000;
        x->sr = sys_getsr();
        x->ms2samps = x->sr * 0.001;
        x->samps =(x->ms * x->ms2samps);
        x->phase = 0;
        x->peak = 0;
        x->block_peak = 0;
        
       // post("%f %f", x->samps, x->ms);
        
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, &s_float);
        
        outlet_new(&x->x_obj, &s_signal);
        outlet_new(&x->x_obj, &s_signal);
        
    }
    return (void *)x;
}

void localpeak_tilde_setup(void) {

    localpeak_tilde_class = class_new(gensym("localpeak~"), (t_newmethod)localpeak_tilde_new, 0, sizeof(t_localpeak_tilde), CLASS_NOINLET, A_DEFFLOAT, 0);

    class_addmethod(localpeak_tilde_class, (t_method)localpeak_tilde_dsp, gensym("dsp"), 0);

    class_addfloat(localpeak_tilde_class, localpeak_tilde_float);
    
}
#endif