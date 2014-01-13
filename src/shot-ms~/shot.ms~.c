
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"


static t_class *shotms_class;

typedef struct _shotms {
    t_pxobject  ob;
    
    t_double    ms;
    t_double    ms2samps;
    t_double    samps;
    
    t_sample    inc;
    t_double    phase;
    
    t_double    sr;

    t_int       active;
    
    
} t_shotms;


t_double shotms_setMs(t_shotms *x, t_sample f)
{
    x->ms = f ? f : 1000;
    x->samps =(x->ms * x->ms2samps);
    x->inc = 1 / x->samps;
    return x->inc;
}

// this is 64-bit perform method for Max 6
void shotms_perform64(t_shotms *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	t_double *in1 = ins[0];
	t_double *out1 = outs[0];
    t_double *out2 = outs[1];
    t_double *out3 = outs[2];

	long n = sampleframes;
    
    t_int active = x->active;
    t_double pc = x->phase;
    t_double inc = x->inc;
    t_double samps = x->samps;
    
    //check in1 for != 0 if not active, if there is something, then start counting
    
    t_sample setms;
    t_sample delegate = active;
    
    while (n--)
    {
        setms = *in1++;
        
        if (!active && setms) {
            
            inc = shotms_setMs(x, setms);
            
            pc = 0;
            active = 1;
            //set delegation after this sample output
        }
        
        if (active) {
            
            if (pc >= samps) {
                active = 0;
                pc = 0;
                delegate = 0;
            } else {
                pc++;
            }
            
        }
        
        *out1++ = pc * inc;
        *out2++ = active;
        *out3++ = setms * delegate;
        
        if(active && setms)
        {
            delegate = 1;
        }
    }
    
    x->phase = pc;
    x->active = active;
}

void shotms_dsp64(t_shotms *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(samplerate != x->sr)
    {
        x->sr = samplerate;
        x->ms2samps = x->sr * 0.001;
    }
	object_method(dsp64, gensym("dsp_add64"), x, shotms_perform64, 0, NULL);
}

t_int *shotms_perform(t_int *w)
{
    t_shotms *x = (t_shotms *)(w[1]);
    t_sample *in1 = (t_sample *)(w[2]);
    t_sample *out1 = (t_sample *)(w[3]);
    t_sample *out2 = (t_sample *)(w[4]);
    t_sample *out3 = (t_sample *)(w[5]);
    int nsamps = (int)(w[6]);
    t_int active = x->active;

    t_double pc = x->phase;
    t_double inc = x->inc;
    t_double samps = x->samps;
    
    //check in1 for != 0 if not active, if there is something, then start counting

    t_sample setms;
    t_sample delegate = active;
    
    while (nsamps--)
    {
        setms = *in1++;
        
        if (!active && setms) {
            
            inc = shotms_setMs(x, setms);
            
            pc = 0;
            active = 1;
            //set delegation after this sample output
        }
        
        if (active) {
            
            if (pc >= samps) {
                active = 0;
                pc = 0;
                delegate = 0;
            } else {
                pc++;
            }

        }
        
        *out1++ = pc * inc;
        *out2++ = active;
        *out3++ = setms * delegate;
        
        if(active && setms)
        {
            delegate = 1;
        }
    }
    
    x->phase = pc;
    x->active = active;
    return (w+7);
}


void shotms_dsp(t_shotms *x, t_signal **sp)
{
    if(sys_getsr() != x->sr)
    {
        x->sr = sys_getsr();
        x->ms2samps = x->sr * 0.001;
//        error("sample rate changed!");
    }
    
    dsp_add(shotms_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);
}

void shotms_assist(t_shotms *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "I am inlet %ld", a);
	}
	else {	// outlet
		sprintf(s, "I am outlet %ld", a);
	}
}

void *shotms_new(t_symbol* s, short argc, t_atom* argv)
{
    t_shotms *x = (t_shotms *)object_alloc(shotms_class);
    if(x)
    {
        x->active = 0;
        x->ms = 10;
        x->sr = sys_getsr();
        x->ms2samps = x->sr * 0.001;
        x->samps =(x->ms * x->ms2samps);
        x->inc = 1 / x->samps;
        
//        post("ms %f samps %f inc %f", x->ms, x->samps, x->inc);
        x->phase = 0;
        
        dsp_setup((t_pxobject *)x, 1);
        
        outlet_new(x, "signal");
        outlet_new(x, "signal");
        outlet_new(x, "signal");
        
    }
    return (void *)x;
}

int C74_EXPORT main(void)
{

    t_class *c = class_new("shot-ms~", (method)shotms_new, (method)dsp_free, sizeof(t_shotms), 0L, A_GIMME, 0);

    class_addmethod(c, (method)shotms_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)shotms_dsp64, "dsp64", A_CANT,0);
	class_addmethod(c, (method)shotms_assist,	"assist",	A_CANT, 0);

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    shotms_class = c;
}
