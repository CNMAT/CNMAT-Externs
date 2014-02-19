/*--
 granufm~ v.0
 
 evelope type mod example:
 inlets:
    0 trigger
    1 car hz
    2 harm      << realtime adjust all grains?
    3 mod index << realtime adjust all grains?
    4 amp dur
    5 x amp slope
    6 y amp slope
    7 x mod slope
    8 y mode slope
    9 amp enve type
    10 mod env type
 
 single evelope version:
    0 trigger
    1 car hz
    2 harm (for all)
    3 mod index (for all)
    4 amp dur
    5 x amp slope
    6 y amp slope
    7 amp enve type

 //maybe make switch to lock harm and index to grain
 
 window types:
    0: cos
    1: fof
    2: dampedsine
    3: sinc
 
    10: linear chirp
    11: exp chirp
 
 chirp are based on trigger value for incremental amount from hz to 20 or 20k hz.

 
--*/


#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#include <math.h>

#undef PI
#define PI 3.14159265358979323846264338327950288419716939937510

#undef E
#define E  2.71828182845904523536028747135266249775724709369995

#define DEFAULTMAXOSCILLATORS 256

#define TPOW 16
#define STABSZ (1l<<TPOW)


typedef struct _osc
{


    double      phase_current;
    double      phase_inc;
    
    double      chirp_rate;
    double      chirp_direction;
    
    int         window_type;
    
    double      window_phase_curent;
    double      window_phase_inc;
    
    double      window_attack;
    double      window_slope;
       
    long        tex;
    int         offset;

    int         outlet;
    

} t_osc;


typedef struct _grans {
	t_pxobject		ob;
    
    int             numoutlets;
    int             outletiter;

    int             maxoscillators;
    
    double          maxhz;
    t_osc           *base;
    t_osc           *modbase;
    
    int             nosc;
    int             next_nosc;
    double          pk;
    double          pkw;
        
    double          samplerate;
    double          sampleinterval;
    
    double          *sinetab;
    double          *wind_costab;
    double          *expdecaytab;
    double          *dampedsinetab;

    double          *sincwindow;
    int             sincripples;
    
    double          prev_in1;
    
    Boolean         always_on;
} t_grans;


void *grans_new(t_symbol *s, long argc, t_atom *argv);
void grans_free(t_grans *x);
void grans_assist(t_grans *x, void *b, long m, long a, char *s);

void grans_dsp64(t_grans *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void grans_perform64(t_grans *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

static void grans_clear(t_grans *x);

void grans_releaseVoice(t_grans *x, t_osc *osc);
void grans_setNewGrain(t_grans *x, int offset, double chirp, double freq, double duration, double attack, double slope, int type);



void SineFunction(int n, double *stab, int stride, double from, double to);
void Makeoscsinetable(t_grans *x);
void CosWindowFunction(int n, double *tab, int stride, double from, double to);
void MakeCosWindow(t_grans *x);
void ExpDecayFunction(int n, double *stab, int stride, double from, double to);
void MakeExpDecaytable(t_grans *x);
void DampedSineFunction(int n, double *tab, int stride, double from, double to, double k);
void MakeDampedSineWindow(t_grans *x);

void SincFunction(int n, double *tab, int stride, double from, double to);
void MakeSincWindow(t_grans *x);

double wrapPhase(double p_current, double tablesize);
double linear_interp( double v0, double v1, double t);

double FM(double *sinetab, long tablesize, double car_pc, double harm, double index  );


// global class pointer variable
static t_class *grans_class = NULL;


//***********************************************************************************************

int main(void)
{	
	t_class *c = class_new("granufm~", (method)grans_new, (method)grans_free, (long)sizeof(t_grans), NULL,   A_GIMME);
	
    class_addmethod(c, (method)grans_clear,		"clear",     0);    
	class_addmethod(c, (method)grans_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)grans_assist,	"assist",	A_CANT, 0);
	
    CLASS_ATTR_LONG(c, "alwayson", 0, t_grans, always_on);
    
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	grans_class = c;

	return 0;
}


void *grans_new(t_symbol *s, long argc, t_atom *argv)
{
    int n;
	t_grans *x = (t_grans *)object_alloc(grans_class);

	if (x) {
		dsp_setup((t_pxobject *)x, 8);

        x->maxoscillators = DEFAULTMAXOSCILLATORS;
        
        if( argc > 0)
        {
            int i;
            t_atom *ap;
            for (i = 0, ap = argv; i < argc; i++, ap++) {
                if(atom_gettype(ap) == A_LONG)
                {
                    switch(i){
                        case 0:
                            n = atom_getlong(ap);
                            x->maxoscillators = n;
                           // object_post((t_object *)x, "%d oscillators initialized", n);
                            break;
                        case 1:
                            n = atom_getlong(ap);
                            x->numoutlets = n;
                           // object_post((t_object *)x, "%d outlets?", n);

                            while(n--){
                                outlet_new(x, "signal");
                            }
                        default:
                        break;       
                    }
                }
            }
            
        }
        else
        {
            x->numoutlets = 1;
            outlet_new(x, "signal");
           // outlet_new(x, "signal");
            
        }
		
        x->ob.z_misc |= Z_NO_INPLACE;

        
		x->base = (t_osc *)calloc(x->maxoscillators, sizeof(t_osc));
//		x->modbase = (t_osc *)calloc(x->maxoscillators, sizeof(t_osc));

        x->sinetab = (double *)calloc(STABSZ, sizeof(double));
        x->wind_costab = (double *)calloc(STABSZ, sizeof(double));
        x->expdecaytab = (double *)calloc(STABSZ, sizeof(double));
        x->dampedsinetab = (double *)calloc(STABSZ, sizeof(double));
        x->sincwindow = (double *)calloc(STABSZ, sizeof(double));

        
        x->samplerate =  sys_getsr();
        if(x->samplerate<=0)
            x->samplerate = 44100;
      
        
        x->sampleinterval = 1.0 / x->samplerate;
        x->pkw = ( STABSZ * x->sampleinterval ) ;

        x->maxhz = (x->samplerate / 2) * x->pkw;
        x->nosc = x->next_nosc = 0;
        
        x->prev_in1 = 0.0;
        
        x->sincripples = 5; //could make this an attribute, or maybe compute sinc in realtime...
        
        Makeoscsinetable(x);
        MakeCosWindow(x);
        MakeExpDecaytable(x);
        MakeDampedSineWindow(x);
        MakeSincWindow(x);
        
        grans_clear(x);
        
        x->always_on = 0;
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d);
            object_free(d);
        }
        

        
    } else {
        object_post((t_object *)x, "this is potentially bad!");
    }
    
	return (x);
}


void grans_free(t_grans *x) 
{
    dsp_free((t_pxobject *)x);

    free(x->sincwindow);
    free(x->dampedsinetab);
    free(x->expdecaytab);
    free(x->wind_costab);
	free(x->sinetab);
    free(x->base);
  //  free(x->modbase);

  //  x->modbase = NULL;
    x->sincwindow = NULL;
    x->dampedsinetab = NULL;
    x->expdecaytab = NULL;
    x->wind_costab = NULL;
    x->base = NULL;
    x->sinetab = NULL;
}


//***********************************************************************************************

static void grans_clear(t_grans *x)
{
	t_osc *p = x->base;
	int i;
	for( i=0; i<x->maxoscillators; ++i, p++)
	{
//		p->next_phase_inc = 0.0;
		p->phase_inc = 0.0;
		p->phase_current = 0.0;
        p->window_phase_curent = 0.0;
        p->window_attack = 1.0;
        p->window_slope = 1.0;
        p->window_type  = 0;
        p->tex = 0.0;
	}
}


void grans_assist(t_grans *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0:
                sprintf(s, "(signal) trigger != 0 ");
                break;
            case 1:
                sprintf(s, "(signal) hz ");
                break;
            case 2:
                sprintf(s, "(signal) grain ms ");
                break;
            case 3:
                sprintf(s, "(signal) attack ");
                break;
            case 4:
                sprintf(s, "(signal) curve exp ");
                break;
            case 5:
                sprintf(s, "(signal) window type ");
                break;
            default:
                sprintf(s, "I am inlet %ld", a);
                break;
        }
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
/*
 single evelope version:
 0 trigger
 1 car hz
 2 harm (for all)
 3 mod index (for all)
 4 amp dur
 5 x amp slope
 6 y amp slope
 7 amp enve type
*/ 
	double *in1 = ins[0];
    double *in2 = ins[1];
	double *harm = ins[2];
	double *modindex = ins[3];
	double *in5 = ins[4];
    double *in6 = ins[5];
    double *in7 = ins[6];
    double *in8 = ins[7];

    
    double **outlets = outs;

    register double  prev_trig = x->prev_in1;
    register double  trigger;
    register double  type;
    register double  hz;
    register double  dur;
    register double  w_attack;
    register double  w_slope;
	
    
    int j, i;
    double *sinetab = x->sinetab; //not sure if the const is helping me a lot?
    const double *cos_wind = x->wind_costab;
    const double *decaytab = x->expdecaytab;
    const double *dampedsinetab = x->dampedsinetab;
    const double *sinc_wind = x->sincwindow;
    
    const Boolean alwayson = x->always_on;
    
    //zero out samples, and check for new triggers
    for ( j = 0; j < sampleframes; j++)
    {
        
        for(i = 0; i < numouts; i++){ //might be able to move this into the main loop, and do the grain allocation there too... if that was the case the j offset could be removed also
            outlets[i][j] = 0.0;
        }

        trigger = *in1++; //trigger also used for chirp rate
        hz = *in2++;
        dur = *in5++;
        w_attack = *in6++;
        w_slope = *in7++;
        type = *in8++;
        
        if ( trigger != 0.0 && hz > 0.0 && dur > 0.0 && w_attack > 0.0 && w_slope > 0.0)
        {
            grans_setNewGrain( x, j, trigger, hz, dur,  w_attack, w_slope, (int)type );

        }
        prev_trig = trigger;

    }

    x->prev_in1 = prev_trig;

    
    t_osc *o = x->base;
//    t_osc *mod = x->modbase; //in case of individual grain envelopes

    const int    maxoscillators = x->maxoscillators;
    const double tabSamp = 1.0 / STABSZ;
    const double halftab = 0.5 * STABSZ;

    register long       tex_pc;
    register double     pc, wpc, chirpdir;
    register double     pi, wpi, att, slop, amp, tex, chirprate;
    register int        w_type, outletnum;
    
    for( i = 0; i < maxoscillators; i++, o++)
    {
        
        pi      = o->phase_inc;
        if( pi == 0.0 && !alwayson) continue; //<< if not always on, it's a little cheaper, but spiky
        //actually this is only one part of cpu cost, the other part is writing the windows, i'm not sure if I can reduced that...
        
        pc      = o->phase_current;
        
        wpc     = o->window_phase_curent;
        wpi     = o->window_phase_inc;

        att     = o->window_attack;
        slop    = o->window_slope;

        w_type  = o->window_type;
        
        outletnum = o->outlet;
        
        j = o->offset;
        
        if( w_type == 0 ) //cos window
        {
            while(j < sampleframes && wpc < STABSZ)
            {
                
                amp = pow( cos_wind[ (uint32_t)(pow( wpc * tabSamp, att ) * STABSZ) ], slop);
                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = fmod((pc + pi), STABSZ);
                wpc += wpi;
                
                j++;
            }
        }
        else if( w_type == 1 ) //fof window
        {
            tex = o->tex;
            while(j < sampleframes && wpc < STABSZ)
            {
                
                tex_pc = (uint32_t)wrapPhase(wpc * tex, STABSZ);
                if( tex_pc <= halftab )
                {
                    amp = pow( cos_wind[ tex_pc ], slop) * decaytab[ (uint32_t)wpc ];
                }
                else
                {
                    amp = decaytab[ (uint32_t)wpc ];
                }
                                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = fmod((pc + pi), STABSZ);
                wpc += wpi;
                
                j++;
                
            }
        }
        else if( w_type == 2 ) //dampedsine window
        {
            while(j < sampleframes && wpc < STABSZ)
            {
                amp = pow( dampedsinetab[(uint32_t)( pow( wpc * tabSamp, att ) * STABSZ )], slop);
                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = fmod((pc + pi), STABSZ);
                wpc += wpi;
                
                j++;
            }
        }
        else if( w_type == 3 ) //sinc window
        {
            while(j < sampleframes && wpc < STABSZ)
            {
                amp = pow( sinc_wind[(uint32_t)( pow( wpc * tabSamp, att ) * STABSZ )], slop);
                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = fmod((pc + pi), STABSZ);
                wpc += wpi;
                
                j++;
            }
        }
        else if( w_type == 10 )
        {
            chirprate = o->chirp_rate;
            chirpdir  = o->chirp_direction;
            while(j < sampleframes && wpc < STABSZ)
            {
                
                amp = pow( cos_wind[ (uint32_t)(pow( wpc * tabSamp, att ) * STABSZ) ], slop);
                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = wrapPhase(pc + pi + (wpc * tabSamp * chirprate * chirpdir), STABSZ);
                wpc += wpi;

                j++;
            }
            
        } 
        else if( w_type == 11 )
        {
            chirprate = o->chirp_rate;
            chirpdir  = o->chirp_direction;
            while(j < sampleframes && wpc < STABSZ)
            {
                amp = pow( cos_wind[ (uint32_t)(pow( wpc * tabSamp, att ) * STABSZ) ], slop);
                
                outlets[outletnum][j] += amp * FM(sinetab, STABSZ, pc, harm[j], modindex[j] );
                pc = wrapPhase(pc + pi + (chirpdir * pow(chirprate, wpc * tabSamp)), STABSZ);
                wpc += wpi;
                
                j++;
            }
            
        } else {
            goto release;
        }
        
        o->offset = 0;
        
        if( wpc < STABSZ )
        {
            o->window_phase_curent = wpc;
            o->phase_current = pc;
            
        }
        else
        {
release:
            grans_releaseVoice(x, o); //this could probably be moved out of the osc loop, but seems ok

            if(x->nosc == 0 && !alwayson)
                break;
        }

    }
}

double FM(double *sinetab, long tablesize, double car_pc, double harm, double index  )
{
    double mod_pc = sinetab[(uint32_t)wrapPhase(car_pc * harm, tablesize)] * tablesize;
    return sinetab[ (uint32_t)(wrapPhase(car_pc + (index * mod_pc), tablesize) )];
}


void grans_setNewGrain(t_grans *x, int offset, double chirprate, double freq, double duration, double attack, double slope, int type)
{
    int i, maxoscillators = x->maxoscillators;
    t_osc *o = x->base;
    double pkw = x->pkw;
    //  double sr = x->samplerate; //could pass this from perform in case of change?
    
        
    for ( i = 0; i < maxoscillators; ++i )
    {
        if( o->phase_inc == 0.0 )
        {
            o->window_type = type;
            
            // o->phase_inc = freq * pk; //bit shift version
            o->phase_inc = freq * pkw; //modulo version
            
            o->chirp_direction = (chirprate > 0) ? 1 : -1;
            o->chirp_rate = fabs(chirprate) * pkw;
            
            double dur_hz = 1000.0 / duration;
            o->window_phase_inc = dur_hz * pkw;
            
            
           // o->tex = 1.0 / ((attack > 1.0) ? 1.0 : attack);
            o->tex = 1.0 / attack;
            
            o->offset = offset;
            
            o->window_phase_curent = 0.0; //offset for cos
            o->window_attack = attack;
            o->window_slope = slope;
            
            x->outletiter++;
            o->outlet =  x->outletiter % x->numoutlets;
            
            x->nosc++;
            break;
        }
        o++;
    }
    
}

void grans_releaseVoice(t_grans *x, t_osc *o)
{
    //with the window verison most of this clearing is probably unecessary as long as things are initialized correctly  ... i.e. just need to set phase_inc to 0.0
    o->phase_inc = 0.0;
    o->phase_current = 0.0;

    o->window_phase_inc = 0.0;
    o->window_phase_curent = 0.0;

    --x->nosc; //I think not used now
}


//*************************************************************************************************



double wrapPhase(double p_current, double tablesize)
{
    if( p_current < 0)
        return fmod(p_current, tablesize) + tablesize;
    else return fmod(p_current, tablesize);
}


double linear_interp( double v0, double v1, double t)
{
    return v0+(v1-v0)*t;
}


void MakeSincWindow(t_grans *x)
{
    SincFunction(STABSZ, x->sincwindow, 1, (double)-(x->sincripples), (double)x->sincripples);
}


void SincFunction(int n, double *tab, int stride, double from, double to)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        if (t == 0.0)
        {
            tab[ j * stride ] = 1.0; //avoid divide by zero!
        }
        else
        {
            tab[ j * stride ] = fabs( sin(PI * t) / (PI * t) ); // ABS version
        }
	}
};


void MakeDampedSineWindow(t_grans *x)
{
    DampedSineFunction(STABSZ, x->dampedsinetab, 1, 0.0, 5.0, 1.0);
}

void DampedSineFunction(int n, double *tab, int stride, double from, double to, double k)
{
    int j;
    double f = (to-from) / n;
    double t;
    
	for( j=0; j<n; ++j )
	{
        t = (from + (j * f));
        
		tab[ j * stride ] = fabs( pow( E, -k * t ) * sin( 2.0 * PI * t ) ); // ABS version
        
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

void MakeExpDecaytable(t_grans *x)
{
    ExpDecayFunction(STABSZ, x->expdecaytab, 1, 0.0, 5.0);
    
}

void ExpDecayFunction(int n, double *stab, int stride, double from, double to)
{
    int j;
	double f = (to-from) / n;
    
	for( j=0; j<n; ++j )
	{
		stab[ j * stride ] = exp( -(from + j * f) );
        
	}

}




