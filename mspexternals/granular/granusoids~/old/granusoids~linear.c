/*--
 
 
--*/


#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include <math.h>

#undef PI
#define PI 3.14159265358979323846264338327950288419716939937510

#undef E
#define E  2.71828182845904523536028747135266249775724709369995

#define MAXOSCILLATORS 1024

#define TPOW 16
#define STABSZ (1l<<TPOW)

typedef struct _osc
{

    long        next_phase_inc;
    uint32_t    phase_current;
    uint32_t    prev_phase;
    
    double      phase_inc;
    
    uint32_t    window_phase_curent;
    uint32_t    prev_window_phase;
    double      window_phase_inc;
    
    int         offset;
//    int         window_type; //<< future
  
    double      window_exp;
    double      window_slope;
    
//    double      amp;
//    double      next_amp;
//    double      amp_inc;
    
    double      dur_samps;
    double      dur_inc;
    double      next_dur_inc;
    

} t_osc;


typedef struct _grans {
	t_pxobject		ob;
    t_osc           *base;
        
    int             nosc;
    int             next_nosc;
    double          pk;
    double          pkw;
        
    double          samplerate;
    double          sampleinterval;
    
    double          *sinetab;

    double          *wind_costab;
    
    double          prev_in1;
    
    Boolean         verbose;
} t_grans;


void *grans_new(t_symbol *s, long argc, t_atom *argv);
void grans_free(t_grans *x);
void grans_assist(t_grans *x, void *b, long m, long a, char *s);
void grans_float(t_grans *x, double f);
void grans_dsp64(t_grans *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void grans_perform64(t_grans *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

static void grans_clear(t_grans *x);

int grans_setNewVoice(t_grans *x, double freq, double duration);

void grans_setNewCosVoice(t_grans *x, int offset, double freq, double duration, double slope, double exponent);
void grans_releaseVoice(t_grans *x, t_osc *osc);


void SineFunction(int n, double *stab, int stride, double from, double to);
void Makeoscsinetable(t_grans *x);
void CosWindowFunction(int n, double *tab, int stride, double from, double to);
void MakeCosWindow(t_grans *x);


t_osc *debug_grans_setNewVoice(t_grans *x, double freq, double duration);


// global class pointer variable
static t_class *grans_class = NULL;


//***********************************************************************************************

int main(void)
{	
	t_class *c = class_new("granusoids~", (method)grans_new, (method)grans_free, (long)sizeof(t_grans), NULL,   0);
	
    class_addmethod(c, (method)grans_clear,		"clear",     0);    
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
		dsp_setup((t_pxobject *)x, 5);
								
		outlet_new(x, "signal");
		outlet_new(x, "signal");
	//	outlet_new(x, "signal");
        
        x->ob.z_misc |= Z_NO_INPLACE;

		x->base = (t_osc *)calloc(MAXOSCILLATORS, sizeof(t_osc));
        x->sinetab = (double *)calloc(STABSZ, sizeof(double));
        x->wind_costab = (double *)calloc(STABSZ, sizeof(double));
        
        x->samplerate =  sys_getsr();
        if(x->samplerate<=0)
            x->samplerate = 44100;
      
        
        x->sampleinterval = 1.0 / x->samplerate;
        x->pk = ( STABSZ * x->sampleinterval ) * ( 1l << ( 32 - TPOW )) ;
        x->pkw = ( STABSZ * x->sampleinterval ) ;

        x->nosc = x->next_nosc = 0;
        
        x->verbose = 0;
        x->prev_in1 = 0.0;
        
        Makeoscsinetable(x);
        MakeCosWindow(x);
        
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
    
    x->base = NULL;
    x->sinetab = NULL;
}


//***********************************************************************************************

static void grans_clear(t_grans *x)
{
	t_osc *p = x->base;
	int i;
	for( i=0; i<MAXOSCILLATORS; ++i, p++)
	{
		p->next_phase_inc = 0.0;
//		p->next_amp = 0.0;
//		p->amp = 0.0;
		p->phase_inc = 0.0;
		p->phase_current = 0.0;
        p->window_exp = 1.0;
        p->window_slope = 1.0;
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

double linear_interp( double v0, double v1, double t);
double linear_interp( double v0, double v1, double t)
{
    return v0+(v1-v0)*t;
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
	double *in4 = ins[3];
	double *in5 = ins[4];
    double *out = outs[0];
    double *out2 = outs[1];
 //   double *out3 = outs[2];

    double prev_trig = x->prev_in1;
    double trigger;
    double hz;
    double dur;
    double w_exp;
    double w_slope;
	
    double *sinetab = x->sinetab;
//    double rate = 1.0 / sampleframes;
    int j, i;

    t_osc *o = x->base;
    double *window = x->wind_costab;
    
    
    //zero out samples, and check for new triggers
    for ( j = 0; j < sampleframes; j++)
    {
        out[j] = 0.0;
        out2[j] = 0.0;
     //   out3[j] = 0.0;

        trigger = *in1++;
        hz = *in2++;
        dur = *in3++;
        w_slope = *in4++;
        w_exp = *in5++;

     //   *out3++ = w_exp;
        
        //made for sharp click triggers, but try to avoid creating new voices on every sample..
        if ( trigger  > 0.9 && prev_trig == 0.0 && hz > 0.0 && dur > 0.0 && w_slope > 0.0 && w_exp > 0.0)
        {
            grans_setNewCosVoice( x, j, hz, dur, w_slope, w_exp );

        }
        prev_trig = trigger;

    }

    x->prev_in1 = prev_trig;

//    int nosc = x->nosc;
    double tabSamp = 1.0 / STABSZ;
    
    for( i = 0; i < MAXOSCILLATORS; i++, o++)
    {
        
        register long       pi      = o->phase_inc;
        if( pi == 0.0) continue;
        
        register uint32_t   pc      = o->phase_current;
        //register uint32_t   prev_pc = o->prev_phase;

        
        register uint32_t   wpc     = o->window_phase_curent;
        register uint32_t   prev_wpc     = o->prev_window_phase;
        register double       wpi     = o->window_phase_inc;
//        register uint32_t   widx;

        register double     ex      = o->window_exp;
        register double     slop    = o->window_slope;
        
        register double     amp;
        
        register double     t;
        register double     freqA;
        register long     test_idx;
        
        j = o->offset;
        
        while(j < sampleframes)
        {
            //removing the bit shift just adds a tiny bit, might switch back just for simplicity
//            widx = wpc % STABSZ;
//            widx = wpc >> shift;
//            wtest = pow( window[ (uint32_t)(widx * pow(widx * tabSamp, slop ))], ex);
//            wtest = window[ widx * (uint32_t)pow(widx * tabSamp, slop ) ];
            
            t = (wpc - floor(wpc)) * tabSamp;
            test_idx = (wpc - 1);
            test_idx = test_idx >= 0 ? test_idx : 0;
            amp = linear_interp(window[ test_idx ], window[ wpc ], t);
            
            t = (pc - floor(pc)) * tabSamp;
            test_idx = (pc - 1);
            test_idx = test_idx >= 0 ? test_idx : 0;
            freqA = linear_interp(sinetab[ test_idx ], sinetab[ wpc ], t);
            
            out[j] += amp * freqA;

            //          out[j] +=  sinetab[ pc >> shift ];

//           out[j] += norm * window[ wpc % STABSZ ] * sinetab[ pc % STABSZ ];
            pc = (pc + pi) % STABSZ;

            prev_wpc = wpc;
            wpc += wpi;
            
            //i think this break bit might not be working properly
            if( wpc >= STABSZ )
            {
                break; //window == 1 table read
            }

            j++;
        }

        o->offset = 0;
        
        if( wpc < STABSZ )
        {
            o->window_phase_curent = wpc;
            o->prev_window_phase = prev_wpc;
            o->phase_current = pc;
            
        } else {

            grans_releaseVoice(x, o); //this could be moved out of the osc loop
            if(x->nosc == 0)
                break;
        }

    }
}

void grans_setNewCosVoice(t_grans *x, int offset, double freq, double duration, double slope, double exponent)
{
    int i;//, status = 0;
    t_osc *o = x->base;
    double pk = x->pk;
    double pkw = x->pkw;
    double sr = x->samplerate; //could pass this from perform
    
    
    for ( i = 0; i < MAXOSCILLATORS; ++i )
    {
        if( o->phase_inc == 0.0 )
        {
           // o->phase_inc = freq * pk; //bit shift version
            o->phase_inc = freq * pkw; //modulo version
            
            o->next_phase_inc = o->phase_inc; //this might be useful in case of frequency change
            
            double dur_hz = 1000.0 / duration;
            o->dur_samps = sr / dur_hz;
            o->window_phase_inc = dur_hz * pkw;
            
            o->offset = offset;
            
            o->window_phase_curent = 0.0; //offset for cos
            o->prev_window_phase = 0.0;
            o->window_exp = exponent;
            o->window_slope = slope;
            //o->next_dur_inc = sr / o->dur_samps;
            
           // status =  1;
            
            x->nosc++;
            break;
        }
        o++;
    }
   // return status;
    
}


int grans_setNewVoice(t_grans *x, double freq, double duration)
{
    int i, status = 0;
    t_osc *o = x->base;
    double sr = x->samplerate; //could pass this from perform

    for ( i = 0; i < MAXOSCILLATORS; ++i )
    {
        if( o->phase_inc == 0.0 )
        {
            o->phase_inc = freq * x->pk;
            o->next_phase_inc = o->phase_inc; //this might be useful in case of frequency change
            
            o->dur_samps = sr / (1000.0 / duration);

            o->next_dur_inc = sr / o->dur_samps;
            
//            o->amp_inc = -1.0 / o->dur_samps; //decaying linearly over the duration (rough envelope for now)
 //           o->amp = 1.0;

            status =  1;

            x->nosc++;
            break;
        }
        o++;
    }
    return status;
}

void grans_releaseVoice(t_grans *x, t_osc *o)
{
    //with the window verison most of this clearing is probably unecessary as long as things are initialized correctly
    o->phase_inc = 0.0;
    o->phase_current = 0.0;
   // o->amp_inc = 0.0;
//    o->amp = 0.0;
    o->window_phase_inc = 0.0;
    o->window_phase_curent = 0.0;
    o->prev_window_phase = 0.0;

  //  o->dur_samps = 0.0;
    --x->nosc;
}

void DampedSineFunction(int n, double *tab, int stride, double from, double to, double k)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        
		tab[ j * stride ] = 1 - pow( E, -k * t ) * cos( 2.0 * PI * t );
        
	}
};

void MakeCosWindow(t_grans *x)
{
    CosWindowFunction(STABSZ, x->wind_costab, 1, 0.0, 2.0 * PI);
}

void CosWindowFunction(int n, double *tab, int stride, double from, double to)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        
		tab[ j * stride ] = 1 - (( cos( t ) * 0.5) + 0.5);
        
	}
}

void Makeoscsinetable(t_grans *x)
{
    SineFunction(STABSZ, x->sinetab, 1, 0.0, 2.0 * PI);
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






