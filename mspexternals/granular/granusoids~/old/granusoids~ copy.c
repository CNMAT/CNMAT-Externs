/*--
 
 I'm pretty sure I have to make this a linked list since we don't know when and which envelopes will finish
 should probably still allocate the whole set of grains to avoid realtime memory stuff
 
--*/


#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include <math.h>

#undef PI
#define PI 3.14159265358979323846264338327950288419716939937510
#define MAXOSCILLATORS 1024

#define TPOW 16
#define STABSZ (1l<<TPOW)


//each osc grain has a freq/phase-incr and a window envelope
//phase could be in terms of lookup table...

typedef unsigned long ulong;

typedef struct _osc
{

    uint32_t   phase_current;
    long    next_phase_inc;
    long    phase_inc;
    
    double  amp;
    double  next_amp;
    double  amp_inc;
    
    double  dur_samps;
    double  dur_inc;
    double  next_dur_inc;
    

} t_osc;


typedef struct _grans {
	t_pxobject		ob;			// the object itself (t_pxobject in MSP instead of t_object)
    t_osc           *base;
    t_osc           **stack;
    
    t_osc           emptyosc;
    
    int             nosc;
    int             next_nosc;
    double          pk; //what does this mean exactly?
    
    double          samplerate;
    double          sampleinterval;
    
    double			offset; 	// the value of a property of our object
    
    double          *sinetab;
    
    double          prev_in1;
    
    Boolean         verbose;
} t_grans;


void *grans_new(t_symbol *s, long argc, t_atom *argv);
void grans_free(t_grans *x);
void grans_assist(t_grans *x, void *b, long m, long a, char *s);
void grans_float(t_grans *x, double f);
void grans_dsp64(t_grans *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void grans_perform64(t_grans *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void grans_makeStack(t_grans *x);
int debug_grans_makeStack(t_grans *x);

void grans_printstack(t_grans *x);
void grans_testStack(t_grans *x);


static void grans_clear(t_grans *x);
int grans_setNewVoice(t_grans *x, double freq, double duration);
int grans_releaseVoice(t_grans *x, t_osc *osc);


void SineFunction(int n, double *stab, int stride, double from, double to);
void Makeoscsinetable(t_grans *x);

t_osc *debug_grans_setNewVoice(t_grans *x, double freq, double duration);


// global class pointer variable
static t_class *grans_class = NULL;


//***********************************************************************************************

int main(void)
{	
	t_class *c = class_new("granusoids~", (method)grans_new, (method)grans_free, (long)sizeof(t_grans), NULL,   0);
	
    class_addmethod(c, (method)grans_clear,		"clear",     0);
    class_addmethod(c, (method)grans_printstack,"printstack",     0);
    class_addmethod(c, (method)grans_testStack, "teststack",     0);
    
	class_addmethod(c, (method)grans_dsp64,		"dsp64",	A_CANT, 0);
	
    class_addmethod(c, (method)grans_assist,	"assist",	A_CANT, 0);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	grans_class = c;

	return 0;
}


void *grans_new(t_symbol *s, long argc, t_atom *argv)
{
	t_grans *x = (t_grans *)object_alloc(grans_class);

	if (x) {
		dsp_setup((t_pxobject *)x, 3);
								
		outlet_new(x, "signal");
		outlet_new(x, "signal");
		outlet_new(x, "signal");
        
        x->ob.z_misc |= Z_NO_INPLACE;

		x->base = (t_osc *)calloc(MAXOSCILLATORS, sizeof(t_osc));
        x->stack = (t_osc **)calloc(MAXOSCILLATORS, sizeof(t_osc *));
        
        x->sinetab = (double *)calloc(STABSZ, sizeof(double));
        
        x->offset = 0.0;

        x->samplerate =  sys_getsr();
        if(x->samplerate<=0)
            x->samplerate = 44100;
      
        
        x->sampleinterval = 1.0 / x->samplerate;
        x->pk = ( STABSZ * x->sampleinterval ) * ( 1l << ( 32 - TPOW )) ;

        x->nosc = x->next_nosc = 0;
        
        x->verbose = 0;
        x->prev_in1 = 0.0;
        
        Makeoscsinetable(x);
        
        grans_clear(x);

    } else {
        object_post((t_object *)x, "could not make object!");
        
    }
    
	return (x);
}


void grans_free(t_grans *x) 
{
    dsp_free((t_pxobject *)x);
    
	free(x->sinetab);
    free(x->base);
    free(x->stack);
    
    x->base = NULL;
    x->stack = NULL;
    x->sinetab = NULL;
}


//***********************************************************************************************

void grans_printstack(t_grans *x)
{
    t_osc **stack = x->stack;
    t_osc *o = *stack;
    t_osc *empty = &x->emptyosc;
    
    while (o != empty) {
        post("o->phase_inc %d",o->phase_inc);
        o = *stack++;
    }
    
    post("end of stack");
    
}

void grans_testStack(t_grans *x)
{
    
    t_osc *o = debug_grans_setNewVoice(x, 100, 200.);
    post("returned object phase %dl", o->phase_inc);
    int count = debug_grans_makeStack(x);
    post("stack count = %i", count);

}


static void grans_clear(t_grans *x)
{
	t_osc *p = x->base;
    t_osc **stack = x->stack;
	int i;
	for( i=0; i<MAXOSCILLATORS; ++i, p++)
	{
		p->next_phase_inc = 0.0;
		p->next_amp = 0.0;
		p->amp = 0.0;
		p->phase_inc = 0.0;
		p->phase_current = 0.0;
     
        *stack = &x->emptyosc;
        
	}
}


void grans_assist(t_grans *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "I am inlet %ld", a);
	} 
	else {	// outlet
		sprintf(s, "I am outlet %ld", a); 			
	}
}


//***********************************************************************************************

void grans_dsp64(t_grans *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{

	object_method(dsp64, gensym("dsp_add64"), x, grans_perform64, 0, NULL);
}


void grans_perform64(t_grans *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{

	double *in1 = ins[0];
    double *in2 = ins[1];
	double *in3 = ins[2];
    
    double *out = outs[0];
    double *out2 = outs[1];
    double *out3 = outs[2];

    double prev_trig = x->prev_in1;
    double trigger;
    double hz;
    double dur;
	
    double *sinetab = x->sinetab;
//    double rate = (double)1 / n;
    int j, i;
    
//    Boolean makestack = false;
        
    //zero out samples, and check for new triggers
    for ( j = 0; j < sampleframes; j++)
    {
        out[j] = 0.0;
        out2[j] = 0.0;
        out3[j] = 0.0;

        trigger = *in1++;
        hz = *in2++;
        dur = *in3++;
        
        //made for sharp click triggers, but try to avoid creating new voices on every sample..
        if ( trigger  > 0.9 && prev_trig == 0.0 && hz > 0.0 && dur > 0.0)
        {
            grans_setNewVoice( x, hz, dur );

        }
        prev_trig = trigger;

    }

    x->prev_in1 = prev_trig;

   // t_osc **stack = x->stack;
   // t_osc *o = *stack;
    //t_osc *empty = &x->emptyosc;

    int nosc = x->nosc;
    t_osc *o = x->base;
    
    //while ( o != empty)
    for( i = 0; i < MAXOSCILLATORS && nosc > 0; i++, o++)
    {
        register double     a       = o->amp;

        if( a == 0.0) continue;
        
        register uint32_t   pc      = o->phase_current;
        register long       pi      = o->phase_inc;
        
        
        register double     astep   = o->amp_inc;
        
        double norm = 1.0 / sqrt(nosc); //equal power norm

        
        for (j = 0; j < sampleframes; j++)
        {
            
            out3[j] = nosc;
            out2[j] += a;//debugging
            
            out[j] += norm * a * sinetab[ pc >> (32 - TPOW) ];

            //out[j] = sinetab[ (pc % TPOW) ]; //slient?

            pc += pi;

            a += astep;
            
            if( a <= 0.0){
                a = 0.0;
            }

        }
/*
        o->amp = a;
        o->phase_current = pc;
*/
        if( a > 0.0 )
        {
            o->amp = a;
            o->phase_current = pc;
            
        } else {
           // makestack = true;
            nosc = grans_releaseVoice(x, o);
            if(nosc == 0)
                break;
        }

       // o = *stack++;


    }
/*
    if(makestack == true)
        grans_makeStack(x);
  */  

 //   x->nosc = nosc;

}


void grans_makeStack(t_grans *x)
{
    int i, count = 0;
    t_osc *o = x->base;
    t_osc **stack = x->stack;
    t_osc *empty  = &x->emptyosc;
    
    for ( i = 0; i < MAXOSCILLATORS; ++i, o++ )
    {
        if( o->phase_inc != 0.0 )
        {
            *stack++ = o;
            count++;
        }
        
    }
    
    while (count < MAXOSCILLATORS) {
        *stack++ = empty;
        count++;
    }
    
}

int grans_setNewVoice(t_grans *x, double freq, double duration)
{
    int i, status = 0;
    t_osc *o = x->base;

    for ( i = 0; i < MAXOSCILLATORS; ++i )
    {
        if( o->phase_inc == 0.0 )
        {
            o->next_phase_inc = freq * x->pk;
            o->phase_inc = o->next_phase_inc; //this might be useful in case of frequency change
            
            double sr = x->samplerate;
            o->dur_samps = sr / (1000.0 / duration);

            o->next_dur_inc = sr / o->dur_samps;
            
            o->amp_inc = -1.0 / o->dur_samps; //decaying linearly over the duration (rough envelope for now)
            o->amp = 1.0;

            status =  1;

            x->nosc++;
            break;
        }
        o++;
    }
   /*
    if(status == 1)
        grans_makeStack(x);
*/
    return status;
}

int grans_releaseVoice(t_grans *x, t_osc *o)
{
    o->phase_inc = 0.0;
    o->phase_current = 0.0;
    o->amp_inc = 0.0;
    o->amp = 0.0;
    return --x->nosc;
}


t_osc *debug_grans_setNewVoice(t_grans *x, double freq, double duration)
{
    int i, status = 0;
    t_osc *o = x->base;
    
    
    for ( i = 0; i < MAXOSCILLATORS; ++i, o++ )
    {
        if( o->phase_inc == 0 )
        {
            o->next_phase_inc = freq * x->pk;
            o->phase_inc = freq * x->pk;
            
            double sr = x->samplerate;
            o->dur_samps = sr / (1000. / duration);
            o->next_dur_inc = sr / o->dur_samps;
            
            o->amp_inc = -1 / o->dur_samps; //decaying linearly over the duration (rough envelope for now)
            o->next_amp = 1;
            
            status =  1;
            break;
        }
    }
    
    if(status == 1)
        grans_makeStack(x);
    
    return o;
}


int debug_grans_makeStack(t_grans *x)
{
    int i, count = 0;
    t_osc *o = x->base;
    t_osc **stack = x->stack;
    t_osc *empty  = &x->emptyosc;
    
    for ( i = 0; i < MAXOSCILLATORS; ++i, o++ )
    {
        if( o->phase_inc != 0 )
        {
            *stack++ = o;
            count++;
        }
        
    }
    int nonzero = count--;
    while (count < MAXOSCILLATORS) {
        *stack++ = empty;
        count++;
    }
    
    return nonzero;
}

void SineFunction(int n, double *stab, int stride, double from, double to)
{
	int j;
	double f = (to-from) / n;
    
	for( j=0; j<n; ++j )
	{
		stab[ j * stride ] = sin( from + j * f );

	}
}

void Makeoscsinetable(t_grans *x)
{
    SineFunction(STABSZ, x->sinetab, 1, (double)0, 2 * PI);
}

