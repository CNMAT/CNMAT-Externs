/*--
 granubuf~ v.0

 inlets
    0: trigger
    1: sample location
    2: dur
    3: rate
    4: x shape
    5: y shape
    6: env type
    7: outlet number: -1 for cycle
    8: buffer index
    9: amp
 
 window types:
    0: cos
    1: fof
    2: dampedsine
    3: sinc
 
 to do: 
    - linear interp (probably cubic also, and make attribute)
    - correct phase reading for buffer's sampling rate?

    - move buffer ref into grains to allow for corpus based file selection with polybuffer, or add "setbuffer <index> <name>"
        or, accept list of buffers in order, counting down from -1, -2, etc.
 
    - fix chirp

    - check custom envelope situation, it seems like it is longer than the default tables, and cuts out as if it had run out of voices earlier, is that becuase of the attack / rev-attack shape?  i.e. not smooth overlap?
 
 
 *****
 - new version, window buffers should ALWAYS be declared, if none are declared use cos or hanning at index 0
 - because of this change, perform routine needs to always look up window index
 
--- TODO
 : change all phases to be in terms of user buffer rather than scaling every time from STABSZ
 : same thing for window and buffer phases, i.e. pkw for a given buffer should be ( frames[index] * x->sampleinterval ) used in the calculation of the phase increment based on rate
 : add interpolation for all lookups
 : add attributes to set individual parameters (that may or may not have inlets)
 : figure out how to deal with fact that we are now assigning buffers explictly, so some window "buffers" may actually be built in, and so need not be checked like external buffers
 : I like having some built in windows, which avoids needing to have extra buffers if not needed

 : special is the fof routinue which REQUIRES the tex shaping function... so fof cannot be added after instantiation if there are no shaping function inlets, or rather, fof tex can not be adjusted at sample rate if there is no inlet, so not a show stopper
 
 
--*/


#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#include "buffer.h"

#include "granu.h"

#define GRANUBUF_NUMINLET_TYPES 11

typedef enum _granubuf_in
{
    UNUSED = -1,
    TRIGGER,
    LOCATION,
    RATE,
    DURATION,
    SHAPEX,
    SHAPEY,
    WIN_INDEX,
    OUTLET,
    BUF_INDEX,
    AMP,
    TEX, //only used with fof window
    CHIRP //only used with chirp window (chirp window used to specify log or linear)
} e_granubuf_in;


typedef struct _sample_grain
{
    long        startpoint;
    
    double      phase_current;
    double      phase_inc;
    
    double      chirp_rate;
    double      chirp_direction;
    
    int         window_index;
    
    double      window_phase_curent;
    double      window_phase_inc;
    
    double      window_shapex;
    double      window_shapey;
       
    double      tex;
    int         offset;

    int         outlet;
    int         buf_index;
    double      amp;
    

} t_sample_grain;


typedef struct _grans {
	t_pxobject		ob;
    
    //float input memory
    double          location;
    double          rate;
    double          dur;
    double          w_shapex;
    double          w_shapey;
    long            window_index; //change to window_index
    long            outlet;
    long            buf_index;
    double          amp;
    
    t_symbol        *current_window;
    t_symbol        *current_buffer;
    
    t_symbol        *inlet_name[GRANUBUF_NUMINLET_TYPES];
    e_granubuf_in   inlet_type[GRANUBUF_NUMINLET_TYPES];
    long            numinlets;
    
//    e_granubuf_in   *inlet_type; //variable inlet order
    
    //sample buffer array, 
    t_buffer_ref    *buf[GRANU_MAX_BUFFERS];
	t_symbol        *buf_name[GRANU_MAX_BUFFERS];
	t_bool          buffer_modified[GRANU_MAX_BUFFERS];
    long            buf_frames[GRANU_MAX_BUFFERS];
    long            buf_nchans[GRANU_MAX_BUFFERS];
    long            numbufs;

    //sample buffer array for external buffers
    t_buffer_ref    *envbuf[GRANU_MAX_BUFFERS];
	t_symbol        *envbuf_name[GRANU_MAX_BUFFERS];
	t_bool          envbuffer_modified[GRANU_MAX_BUFFERS];
    long            envbuf_frames[GRANU_MAX_BUFFERS];
    long            envbuf_nchans[GRANU_MAX_BUFFERS];
    long            numenvbufs; //number of external buffers
    
    int             numwindows;
//includes count of internal and external buffers (internal are global vars)
    int             tab_w_index[GRANU_MAX_BUFFERS]; //offset by internal windows, +(GRANUBUF_NUMINTERNAL_WINDOWS-1)

    
    int             numoutlets;
    int             outletiter;
    
    long            maxoverlap;
    long            prev_maxoverlap;
    
    double          maxhz;
    t_sample_grain  *base;
    
    int             nosc;
    int             next_nosc;
    double          pk;
    double          pkw;
    
    double          tabSamp;
    double          halftab;
    int             shapetabscale;
    int             wtabscale;
        
    double          samplerate;
    double          sampleinterval;
    
    double          prev_in1;
    
    int         always_on;
    
    short           *count;
    
    
} t_grans;



// global class pointer variable
static t_class *granubuf_class = NULL;

//***********************************************************************************************


void grans_assist(t_grans *x, void *b, long m, long a, char *s)
{
    
	if (m == ASSIST_INLET ) { //inlet
        switch (x->inlet_type[a]) {
            case TRIGGER:
                sprintf(s, "(signal/float) trigger != 0 ");
                break;
            case LOCATION:
                sprintf(s, "(signal/float) sample start time ms ");
                break;
            case RATE:
                sprintf(s, "(signal/float) playback rate ");
                break;
            case DURATION:
                sprintf(s, "(signal/float) grain dur ms ");
                break;
            case TEX:
                sprintf(s, "(signal/float) 0-1 attack ratio for FOF window only ");
                break;
            case SHAPEX:
                sprintf(s, "(signal/float) x shape ");
                break;
            case SHAPEY:
                sprintf(s, "(signal/float) y shape ");
                break;
            case WIN_INDEX:
                sprintf(s, "(signal/int) window buffer index ");
                break;
            case OUTLET:
                sprintf(s, "(signal/int) outlet number (-1 for cycle (default)) ");
                break;
            case BUF_INDEX:
                sprintf(s, "(signal/int) buffer index ");
                break;
            case AMP:
                sprintf(s, "(signal/float) amplitude (default 1.0) ");
                break;
            default:
                sprintf(s, "I am inlet %ld", a);
                break;
        }
	} 
	else {	// outlet
		sprintf(s, "(signal) outlet %ld", a);
	}
}


void granubuf_float(t_grans *x, double f)
{
    int inlet = proxy_getinlet((t_object *)x);
    switch (x->inlet_type[inlet]) {
        case LOCATION:
            x->location = f;
            break;
        case RATE:
            x->rate = f;
            break;
        case DURATION:
            x->dur = f;
            break;
        case TEX:
            x->w_shapex = f;
            break;
        case SHAPEX:
            x->w_shapex = f;
            break;
        case SHAPEY:
            x->w_shapey = f;
            break;
        case WIN_INDEX:
            if(f >= 0 && f < x->numwindows)                
                x->window_index = (int)f;
            else
                object_error((t_object *)x, "no window at this index");

            break;
        case OUTLET:
            if(f >= -1 && f < x->numoutlets)
                x->outlet = (int)f;
            else
                object_error((t_object *)x, "no outlet at this index");
            
            break;
        case BUF_INDEX:
            if(f >= 0 && f < x->numbufs)
                x->buf_index = (int)f;
            else
                object_error((t_object *)x, "no buffer at this index");
            
            break;
        case AMP:
            x->amp = f;
            break;

        default:
            break;
    }

}

void granubuf_int(t_grans *x, int f)
{
    int inlet = proxy_getinlet((t_object *)x);
    switch (x->inlet_type[inlet]) {
        case LOCATION:
            x->location = (double)f;
            break;
        case RATE:
            x->rate = (double)f;
            break;
        case DURATION:
            x->dur = (double)f;
            break;
        case TEX:
            x->w_shapex = (double)f;
            break;
        case SHAPEX:
            x->w_shapex = (double)f;
            break;
        case SHAPEY:
            x->w_shapey = (double)f;
            break;
        case WIN_INDEX:
            if(f >= 0 && f < x->numwindows)
                x->window_index = f;
            else
                object_error((t_object *)x, "no window at this index");
            
            break;
        case OUTLET:
            if(f >= -1 && f < x->numoutlets)
                x->outlet = f;
            else
                object_error((t_object *)x, "no outlet at this index");
            
            break;
        case BUF_INDEX:
            if(f >= 0 && f < x->numbufs)
                x->buf_index = f;
            else
                object_error((t_object *)x, "no buffer at this index");

            break;
        case AMP:
            x->amp = (double)f;
            break;
        default:
            break;
    }
    
}

//***********************************************************************************************


static void grans_clear(t_grans *x)
{
	t_sample_grain *p = x->base;
	int i;
	for( i=0; i<DEFAULTMAXOSCILLATORS; ++i, p++)
	{
        //		p->next_phase_inc = 0.0;
		p->phase_inc = 0.0;
		p->phase_current = 0.0;
        p->window_phase_curent = 0.0;
        p->window_shapex = 1.0;
        p->window_shapey = 1.0;
        p->window_index  = 0;
        p->tex = 0.0;
	}
    x->nosc = 0;
}


void grans_setNewGrain(t_grans *x, int offset, double chirprate, double location, double rate, double duration, double attack, double slope, int window_index, int outlet, int bufferindex, double amp)
{
    int i, maxoverlap = x->maxoverlap;
    t_sample_grain *o = x->base;
    double pkw = x->pkw;
    
    for ( i = 0; i < maxoverlap; ++i )
    {
        if( o->phase_inc == 0.0 )
        {

            o->window_index = window_index;
            o->startpoint = location * x->samplerate * 0.001;
            o->phase_inc = rate;

            o->chirp_direction = (chirprate > 0) ? 1 : -1;
            o->chirp_rate = fabs(chirprate) * pkw;
            
            double dur_hz = 1000.0 / duration;

//            post("%s %d", __func__, x->tab_w_index[window_index]);
            if(x->tab_w_index[window_index] < GRANUBUF_NUMINTERNAL_WINDOWS) //do safety test on grain creation
            {
                o->window_phase_inc = dur_hz * pkw;
//                post("%s %f", __func__, o->window_phase_inc);
            }
            else
            {
                o->window_phase_inc = dur_hz * ((double)x->envbuf_frames[x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS]) * x->sampleinterval;
//                post("%s %f %d", __func__, o->window_phase_inc, x->envbuf_frames[x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS]);
            }
            
            
            o->offset = offset;
            
            o->window_phase_curent = 0.0; //offset for cos
            o->window_shapex = CLAMP(attack, 0.0000000001, 1.);
            o->window_shapey = CLAMP(slope, 0.0000000001, 1.);
            
            o->tex = 1.0 / o->window_shapex;

            o->outlet = (outlet == -1) ? x->outletiter++ % x->numoutlets : outlet % x->numoutlets;
            
            o->buf_index = bufferindex;
            o->amp = amp;
            
            x->nosc++;
            break;
        }
        o++;
    }
    
}

void grans_releaseVoice(t_grans *x, t_sample_grain *o)
{
    //with the window verison most of this clearing is probably unecessary as long as things are initialized correctly  ... i.e. just need to set phase_inc to 0.0
    o->phase_inc = 0.0;
    o->phase_current = 0.0;
    
    o->window_phase_inc = 0.0;
    o->window_phase_curent = 0.0;
    
    --x->nosc; //I think not used now, but might be useful
}



void grans_perform64(t_grans *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    int j, i;

    if(x->numbufs == 0)
    {
        for(i = 0; i < numouts; i++)
            for(j=0; j<sampleframes; j++)
                outs[i][j] = 0.0;
        
        return;
    }

    short *count = x->count;
    
    double **inlets = ins;
    double **outlets = outs;

    double prev_trig = x->prev_in1;
    double trigger;
    double location = x->location;
    double rate = x->rate;
    double dur = x->dur;
    double w_shapex = x->w_shapex;
    double w_shapey = x->w_shapey;
    double window_index = x->window_index;
    int    outlet = x->outlet;
    int    buf_index = x->buf_index;
    double gr_amp = x->amp;
    
    
    const int alwayson = x->always_on;
        
    const long maxoverlap = CLAMP(x->maxoverlap, 0, DEFAULTMAXOSCILLATORS);
    if(maxoverlap != x->maxoverlap) x->maxoverlap = maxoverlap;
    
    
    const double tabSamp = x->tabSamp;
    const double halftab = x->halftab;
    const int    shapetabscale = x->shapetabscale;
    const int    wtabscale = x->wtabscale;
    
    long       tex_pc, xshapetab, yshapetab;
    double     pc, wpc, chirpdir;
    double     pi, wpi, amp, tex, chirprate, pSamp, lowerSamp, upper_samp, linSamp;
    int        w_index, outletnum;

    t_float **internal_window = granu_internal_window;

    t_float     *w_tab;
    
    const long numbufs = x->numbufs;
    const long numenvbufs = x->numenvbufs;
    
    long frames[numbufs], nchans[numbufs], num_wframes, num_wchans, w_bufoffset;
    
    t_sample_grain *o = x->base;
    
    //in case of buffer array, premptively grab all buffers, check for exsistance, get frames and channels before creating grains
    //in case of x->buffer_modified[i], set to new information
    
    t_buffer_obj    *buffer[numbufs];
    t_float         *tab[numbufs];
    int             valid[numbufs];
    
    for (i=0; i<numbufs; i++) {
    
        buffer[i] = buffer_ref_getobject(x->buf[i]);
        tab[i] = buffer_locksamples(buffer[i]);
    
        if (buffer[i] && tab[i])
        {
            frames[i] = buffer_getframecount(buffer[i]);
            nchans[i] = buffer_getchannelcount(buffer[i]);
        
            if(nchans[i] > 0 && frames[i] > 0)
            {
                valid[i] = 1;
                if(x->buffer_modified[i])
                {
                    x->buf_frames[i] = frames[i];
                    x->buf_nchans[i] = nchans[i];
                    x->buffer_modified[i] = false;
                }
            } else {
              //  post("debug: no frames || no channels, probably do something here");
                valid[i] = 0;
                
            }
            
        } else {
          //  post("debug: no buffer || no tab, do something here");
             valid[i] = 0;
//            goto unlock;
        }
    }
    
    long            w_frames[numenvbufs], w_nchans[numenvbufs];
    t_float         *w_buftab[numenvbufs];
    t_buffer_obj    *w_buffer[numenvbufs];
    int             w_valid[numbufs];
    
    for (i=0; i<numenvbufs; i++) {
        
        w_buffer[i] = buffer_ref_getobject(x->envbuf[i]);
        w_buftab[i] = buffer_locksamples(w_buffer[i]);
        
        if (w_buffer[i] && w_buftab[i])
        {
            w_frames[i] = buffer_getframecount(w_buffer[i]);
            w_nchans[i] = buffer_getchannelcount(w_buffer[i]);
            
            
            if(w_nchans[i] > 0 && w_frames[i] > 0)
            {
                w_valid[i] = 1;
                
                if(x->envbuffer_modified[i])
                {
                    x->envbuf_frames[i] = w_frames[i];
                    x->envbuf_nchans[i] = w_nchans[i];
                    x->envbuffer_modified[i] = false;
                }
             //   post("%s %d %d", __func__, i, x->envbuf_frames[i]);
            } else {
                w_valid[i] = 0;
            }
        } else {
            w_valid[i] = 0;
        }
    }
    
    
    //zero out samples, and check for new triggers
    for ( j = 0; j < sampleframes; j++)
    {
        
        for( i = 0; i < numins; i++)
        {
            if(count[i] == 1)
            {
                if(x->inlet_type[i] == TRIGGER)
                    trigger = inlets[i][j]; //trigger also used for chirp rate
                else if(x->inlet_type[i] == LOCATION)
                    location = inlets[i][j];
                else if(x->inlet_type[i] == DURATION)
                    dur = inlets[i][j];
                else if(x->inlet_type[i] == RATE)
                    rate = inlets[i][j];
                else if(x->inlet_type[i] == TEX)
                    w_shapex = inlets[i][j];
                else if(x->inlet_type[i] == SHAPEX)
                    w_shapex = inlets[i][j];
                else if(x->inlet_type[i] == SHAPEY)
                    w_shapey = inlets[i][j];
                else if(x->inlet_type[i] == WIN_INDEX)
                    window_index = inlets[i][j];
                else if(x->inlet_type[i] == BUF_INDEX)
                    buf_index = inlets[i][j];
                else if(x->inlet_type[i] == OUTLET)
                    outlet = inlets[i][j];
                else if(x->inlet_type[i] == AMP)
                    gr_amp = inlets[i][j];
                
                
            }
            
            
        }
        
        //add valid tests here
        if ( trigger != 0.0 && location >= 0.0 && location < frames[buf_index] && dur > 0.0 && buf_index < numbufs && buf_index >= 0 && window_index < x->numwindows && window_index >= 0 && x->nosc < maxoverlap)
        {
           // post("j[%d] trig[%f] loc[%f] rate[%f] dur[%f] att[%f] slop[%f] type[%d] outlet[%d] buf_index[%d] gr_amp[%f]", j, trigger, location, rate, dur,  w_shapex, w_shapey, (int)window_index, outlet, buf_index, gr_amp);
            grans_setNewGrain( x, j, trigger, location, rate, dur,  w_shapex, w_shapey, window_index, outlet, buf_index, gr_amp );
        }


        prev_trig = trigger;
        
        for(i = 0; i < numouts; i++){
            outlets[i][j] = 0.0;
        }

    }

    x->prev_in1 = prev_trig;


    for( i = 0; i < DEFAULTMAXOSCILLATORS; i++, o++)
    {
        
        pi      = o->phase_inc;
        if( pi == 0.0 && !alwayson) continue;
        
        
        pc      = o->phase_current;
        
        wpc     = o->window_phase_curent;
        wpi     = o->window_phase_inc;
                
        w_shapex    = o->window_shapex;
        w_shapey    = o->window_shapey;

        xshapetab = (uint32_t)(w_shapex * shapetabscale); //static, so could be in the grain struct
        yshapetab = (uint32_t)(w_shapey * shapetabscale); //static, so could be in the grain struct
        
        outletnum   = o->outlet;
        
        location    = o->startpoint;
        
        buf_index     = o->buf_index;
        gr_amp      = o->amp;
        
        j           = o->offset;

        w_index      = o->window_index; //index of all windows (including internal windows)
        
        if(x->tab_w_index[w_index] < GRANUBUF_NUMINTERNAL_WINDOWS) //do safety test on grain creation
        {
            num_wframes = STABSZ;
            num_wchans = 1;
            w_tab = internal_window[x->tab_w_index[w_index]];
        }
        else
        {
            w_bufoffset = x->tab_w_index[w_index] - GRANUBUF_NUMINTERNAL_WINDOWS;
            num_wframes = w_frames[w_bufoffset];
            num_wchans = w_nchans[w_bufoffset];
            w_tab = w_buftab[w_bufoffset];
            
            if(!w_valid[w_bufoffset]) goto release;
        }
        
        if(!valid[buf_index]) goto release;
        
        
        if( x->tab_w_index[w_index] == FOF) //fof is a special case which requres two window tables
        {
            tex = o->tex;
            while(j < sampleframes && wpc < num_wframes && (pc + location) <= frames[buf_index] && ((frames[buf_index] - location) + pc) >= 0)
            {

                tex_pc = (uint32_t)wrapPhase(wpc * tex, STABSZ);
                if( tex_pc <= halftab )
                {
                    //skipping interpolation for internal window, since we know it is 2^16
                    amp = gr_amp * internal_window[FOF][ (uint32_t)wpc ] * internal_window[COS][ tex_pc ];
                    
                }
                else
                {
                    amp = gr_amp * internal_window[FOF][ (uint32_t)wpc ];
                }
                
                if(pi > 0)
                    pSamp = pc + location;
                else
                    pSamp = (frames[buf_index] - location) + pc;
                
                upper_samp = ceil(pSamp);
                linSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)floor(pSamp)  ], tab[buf_index][ nchans[buf_index] * (uint32_t)upper_samp  ], upper_samp - (pSamp));
                outlets[outletnum][j] += amp * linSamp;
                
                pc += pi;
                wpc += wpi;

                j++;
                
            }
        }
        else
        {
            while(j < sampleframes && wpc < num_wframes && (pc + location) <= frames[buf_index] && ((frames[buf_index] - location) + pc) >= 0)
            {
                
                lowerSamp = floor(wpc);
                amp = gr_amp * linear_interp(w_tab[ num_wchans * (uint32_t)lowerSamp  ], w_tab[ num_wchans * (uint32_t)ceil(wpc)  ], wpc - lowerSamp);
                
                if(pi > 0)
                    pSamp = pc + location;
                else
                    pSamp = (frames[buf_index] - location) + pc;

                lowerSamp = floor(pSamp);
                linSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)lowerSamp  ], tab[buf_index][ nchans[buf_index] * (uint32_t)ceil(pSamp)  ], pSamp - lowerSamp);
                outlets[outletnum][j] += amp * linSamp;

                pc += pi;
                wpc += wpi;
                
                j++;
            }

        }
        /*
        else if( w_index == 10 )
        {
            chirprate = o->chirp_rate;
            chirpdir  = o->chirp_direction;
            while(j < sampleframes && wpc < STABSZ && (pc + location) <= frames[buf_index] && ((frames[buf_index] - location) + pc) >= 0)
            {
                
                //amp = pow( cos_wind[ (uint32_t)(pow( wpc * tabSamp, att ) * STABSZ) ], slop);
                amp = gr_amp * granu_exptab[ yshapetab ][
                                          (uint32_t)(cos_wind[(uint32_t)(granu_exptab[ xshapetab ][ (uint32_t)wpc ] * wtabscale)] * wtabscale)
                                          ];

                if(pi > 0)
                    pSamp = pc + location;
                else
                    pSamp = (frames[buf_index] - location) + pc;
                
                upper_samp = ceil(pSamp);
                linSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)floor(pSamp)  ], tab[buf_index][ nchans[buf_index] * (uint32_t)upper_samp  ], upper_samp - (pSamp));
                outlets[outletnum][j] += amp * linSamp;
                
                pc = pc + pi + (wpc * tabSamp * chirprate * chirpdir);
                wpc += wpi;

                j++;
            }
            
        } 
        else if( w_index == 11 )
        {
            chirprate = o->chirp_rate;
            chirpdir  = o->chirp_direction;
            while(j < sampleframes && wpc < STABSZ && (pc + location) <= frames[buf_index] && ((frames[buf_index] - location) + pc) >= 0)
            {
                //amp = pow( cos_wind[ (uint32_t)(pow( wpc * tabSamp, att ) * STABSZ) ], slop);
                amp = gr_amp * granu_exptab[ yshapetab ][
                                          (uint32_t)(cos_wind[(uint32_t)(granu_exptab[ xshapetab ][ (uint32_t)wpc ] * wtabscale)] * wtabscale)
                                          ];
                
                if(pi > 0)
                    pSamp = pc + location;
                else
                    pSamp = (frames[buf_index] - location) + pc;
                
                upper_samp = ceil(pSamp);
                linSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)floor(pSamp)  ], tab[buf_index][ nchans[buf_index] * (uint32_t)upper_samp  ], upper_samp - (pSamp));
                outlets[outletnum][j] += amp * linSamp;
                
                pc = pc + pi + (chirpdir * pow(chirprate, wpc * tabSamp));
                wpc += wpi;
                
                j++;
            }
            
        } else {
            goto release;
        } 
         */
        
        o->offset = 0;

        
        if(wpc < num_wframes && (pc + location) <= frames[buf_index] && ((frames[buf_index] - location) + pc) >= 0)
        {
            o->window_phase_curent = wpc;
            o->phase_current = pc;
            
        }
        else
        {
release:
            grans_releaseVoice(x, o);
            
            if(x->nosc == 0 && !alwayson)
                break;
        }

    }
    
//unlock
    for(i=0; i<numbufs; i++)
        buffer_unlocksamples(buffer[i]);
    
    for(i=0; i<numenvbufs; i++)
        if(w_buffer[i]) buffer_unlocksamples(w_buffer[i]);
    
    return;
}

void grans_dsp64(t_grans *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    grans_clear(x);

    memcpy(x->count, count, sizeof(short) * x->numinlets);

    int i, connected = 0;
    for (i=0; i<x->numinlets; i++)
    {
        connected += count[i];
//        post("%s in count %d %d name %s ", __func__, i, x->count[i], x->inlet_name[i]->s_name);
    }
    
    if(connected)
        object_method(dsp64, gensym("dsp_add64"), x, grans_perform64, 0, NULL);
    
}


//*************************************************************************************************



void grans_free(t_grans *x)
{
    dsp_free((t_pxobject *)x);
    
    free(x->base);
    x->base = NULL;
    
    free(x->count);
    x->count = NULL;

}


//buffer name is unavailable on delete, so there is no way to tell which buffer to notify that it's not there...
//not good! -- but we check for it in the perform routine so I guess it's ok

//currently fails silently on unbind and free (but I think this is not a problem)
t_max_err granubuf_notify(t_grans *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
//    post("%s %s %s", __func__, s->s_name, msg->s_name);

    // ask attribute object for name
    
    t_buffer_info buffer_info;
    buffer_getinfo((t_buffer_obj *)data, &buffer_info); //<< magic
    
    if(!buffer_info.b_name)
        return -1;
        
    t_symbol *buffername = buffer_info.b_name;
    
    if (buffername && ((msg == ps_buffer_modified) || msg == gensym("globalsymbol_binding")))
    {
        //buffername = (t_symbol *)object_method((t_object *)data, gensym("getname"));
    
        
    
//        post("%s %s %s", __func__, buffername->s_name, msg->s_name);

        int i;
        for(i = 0; i < x->numbufs; i++)
        {
            if(x->buf_name[i] && x->buf_name[i] == buffername)
            {
                if (msg == ps_buffer_modified)
                    x->buffer_modified[i] = true;
                
                return buffer_ref_notify(x->buf[i], s, msg, sender, data);
            }
        }
        
        for(i = 0; i < x->numenvbufs; i++)
        {
            if(x->envbuf_name[i] && x->envbuf_name[i] == buffername)
            {
                if (msg == ps_buffer_modified)
                    x->envbuffer_modified[i] = true;
                
                return buffer_ref_notify(x->envbuf[i], s, msg, sender, data);
            }
        }
        
    } else {
        //object_error((t_object *)x, "notify failed: not sure which buffer to notify");
        return 1;
    }

    
    return 0;
}


t_max_err granubuf_envbuf_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
/*
    if(sys_getdspstate())
    {
        object_error((t_object *)x, "currently not loading buffer arrays with dsp on");
        return -1;
    }
  */  
    if(argc && argv)
    {

        if(atom_gettype(argv) == A_SYM)
        {
            t_symbol *envname = atom_getsym(argv);
            if(envname)
            {
                x->numwindows = argc;
                x->numenvbufs = 0;
                int i;
               for(i = 0; i < x->numwindows; i++)
               {
                   if(atom_gettype(argv+i) == A_SYM)
                   {
//                       post("envbuf %s", atom_getsym(argv+i)->s_name);
                   
//window index is the lookup indes for either x->envbuf, or x->granu_interal_window
//if the window index VALUE is >= number of internal windows, subtract number of internal windows (num-1) for external buffers
                       
                       x->envbuf_name[i] = atom_getsym(argv+i);

                       if (x->envbuf_name[i] == ps_granu_cos)
                           x->tab_w_index[i] = COS;
                       else if (x->envbuf_name[i] == ps_granu_fof)
                           x->tab_w_index[i] = FOF;
                       else if (x->envbuf_name[i] == ps_granu_dampedsine)
                           x->tab_w_index[i] = DAMPEDSINE;
                       else if (x->envbuf_name[i] == ps_granu_sinc)
                           x->tab_w_index[i] = SINC;
                       else
                       {
                           
                           if(!x->envbuf[x->numenvbufs])
                               x->envbuf[x->numenvbufs] = buffer_ref_new((t_object*)x, x->envbuf_name[i]);
                           else
                               buffer_ref_set(x->envbuf[x->numenvbufs], x->envbuf_name[i]);
                           
                           x->envbuffer_modified[x->numenvbufs] = 1;
                           x->tab_w_index[i] = x->numenvbufs + GRANUBUF_NUMINTERNAL_WINDOWS;
                           
                           x->numenvbufs++;

                       }

                   } else {
                       object_error((t_object *)x, "buffer name must be a symbol");
                   }
               }

            }
            else
            {
                object_error((t_object *)x, "must have buffer name");
            }
        }
    }
//    post("numwindows %d numenvbufs %d", x->numwindows, x->numenvbufs);
    
    return 0;
}

t_max_err granubuf_envbuf_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    char alloc;
    int i;
    
    atom_alloc_array(x->numwindows, argc, argv, &alloc);
    
    if (x->numwindows)
    {
        for(i = 0; i < x->numwindows; i++)
        {
            atom_setsym(*argv+i, x->envbuf_name[i]);
        }
        
    }
    return 0;
}

t_max_err granubuf_currentwindow_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc == 1 && argv && atom_gettype(argv) == A_SYM)
    {
        int i, sucess = 0;
        t_symbol *name = atom_getsym(argv);

        for(i = 0; i < x->numwindows; i++)
        {
            if(x->envbuf_name[i] == name)//names include internal buffers
            {
                x->window_index = i;
                sucess = 1;
                break;
            }
        }
        
        
        if(!sucess)
        {
            object_error((t_object *)x, "unknown buffer name");
        }
        else
        {
            post("set window buffer to %s", name->s_name);
        }
        
    }
    
    
    return 0;
}

t_max_err granubuf_currentwindow_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    
    atom_alloc(argc, argv, &alloc);
    
    if (x->numwindows)
    {
        atom_setsym(*argv, x->envbuf_name[x->window_index]);
    }
    else
    {
        atom_setsym(*argv, gensym("<empty>"));
    }
    
    return 0;
}

t_max_err granubuf_buf_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    /*
    if(sys_getdspstate())
    {
        object_error((t_object *)x, "currently not loading buffer arrays with dsp on");
        return -1;
    }
    */
    
    if(argc && argv)
    {
        x->numbufs = argc;
        if(atom_gettype(argv) == A_SYM)
        {
            
            int i;
            for(i = 0; i < x->numbufs; i++)
            {
                /*
                if(atom_gettype(argv+i) == A_SYM)
                    post("buf %s", atom_getsym(argv+i)->s_name);
                */
                
                x->buf_name[i] = atom_getsym(argv+i);
                if(x->buf_name[i])
                {
                    if(!x->buf[i])
                        x->buf[i] = buffer_ref_new((t_object*)x, x->buf_name[i]);
                    else
                        buffer_ref_set(x->buf[i], x->buf_name[i]);
                    
                } else {
                    object_error((t_object *)x, "must have buffer name");
                }
            }
        }
    }
//    post("numbufs %d", x->numbufs);

    return 0;
}

t_max_err granubuf_buf_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    int i;
    
    atom_alloc_array(x->numbufs, argc, argv, &alloc);
    
    if (x->numbufs)
    {
        for(i = 0; i < x->numbufs; i++)
        {
            atom_setsym(*argv+i, x->buf_name[i]);
        }
        
    }
    else
    {
        atom_setsym(*argv, gensym("<empty>"));
    }
    
    return 0;
}

t_max_err granubuf_currentbuf_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc == 1 && argv && atom_gettype(argv) == A_SYM)
    {
        int i, sucess = 0;
        t_symbol *name = atom_getsym(argv);
        for(i = 0; i < x->numbufs; i++)
        {        
            if(x->buf_name[i] == name && x->buf[i])
            {
                x->buf_index = i;
                sucess = 1;
                post("set buffer to %s", name->s_name);
            }
        }
        
        if(!sucess)
        {
            object_error((t_object *)x, "unknown buffer name");
        }    
            
    }

    
    return 0;
}
    
t_max_err granubuf_currentbuf_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    
    atom_alloc(argc, argv, &alloc);
    
    if (x->numbufs)
    {
        atom_setsym(*argv, x->buf_name[x->buf_index]);
    }
    else
    {
        atom_setsym(*argv, gensym("<empty>"));
    }
    
    return 0;
}


t_max_err granubuf_inlet_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
//    post("%s", __func__);
    
    if(argc&&argv)
    {
        int i;
        x->numinlets = 0;
        t_symbol *inlet_name;
        
        for(i = 0; i < argc; i++)
        {

            if(atom_gettype(argv+i) == A_SYM)
            {
                inlet_name = atom_getsym(argv+i);
                
                if(inlet_name == ps_granu_trigger)
                {
                    x->inlet_name[x->numinlets] = ps_granu_trigger;
                    x->inlet_type[x->numinlets++] = TRIGGER;
                }
                else if(inlet_name == ps_granu_location)
                {
                    x->inlet_name[x->numinlets] = ps_granu_location;
                    x->inlet_type[x->numinlets++] = LOCATION;
                }
                else if(inlet_name == ps_granu_dur)
                {
                    x->inlet_name[x->numinlets] = ps_granu_dur;
                    x->inlet_type[x->numinlets++] = DURATION;
                }
                else if(inlet_name == ps_granu_rate)
                {
                    x->inlet_name[x->numinlets] = ps_granu_rate;
                    x->inlet_type[x->numinlets++] = RATE;
                }
                else if(inlet_name == ps_granu_tex)
                {
                    x->inlet_name[x->numinlets] = ps_granu_tex;
                    x->inlet_type[x->numinlets++] = TEX;
                }
                else if(inlet_name == ps_granu_shapex)
                {
                    /*
                    if(!granu_shapingTableExists)
                        granu_makeShapeTables();
                      */
                    
                    x->inlet_name[x->numinlets] = ps_granu_shapex;
                    x->inlet_type[x->numinlets++] = SHAPEX;
                }
                else if(inlet_name == ps_granu_shapey)
                {
                    /*
                    if(!granu_shapingTableExists)
                        granu_makeShapeTables();
                    */
                    
                    x->inlet_name[x->numinlets] = ps_granu_shapey;
                    x->inlet_type[x->numinlets++] = SHAPEY;
                }
                else if(inlet_name == ps_granu_win_idx)
                {
                    x->inlet_name[x->numinlets] = ps_granu_win_idx;
                    x->inlet_type[x->numinlets++] = WIN_INDEX;
                }
                else if(inlet_name == ps_granu_buf_idx)
                {
                    x->inlet_name[x->numinlets] = ps_granu_buf_idx;
                    x->inlet_type[x->numinlets++] = BUF_INDEX;
                }
                else if(inlet_name == ps_granu_outlet)
                {
                    x->inlet_name[x->numinlets] = ps_granu_outlet;
                    x->inlet_type[x->numinlets++] = OUTLET;
                }
                else if(inlet_name == ps_granu_amp)
                {
                    x->inlet_name[x->numinlets] = ps_granu_amp;
                    x->inlet_type[x->numinlets++] = AMP;
                }
                else
                {                        
                    object_error((t_object *)x, "unknown parameter name: %s", inlet_name->s_name);
                    return -1;
                }
                
            }
            
        }
    }

 //   post("%s %d inlets", __func__, x->numinlets);
    
    return 0;
}
t_max_err granubuf_inlet_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    int i;
    
    atom_alloc_array(x->numinlets, argc, argv, &alloc);
    
    if (x->numinlets)
    {
        for(i = 0; i < x->numinlets; i++)
        {
            atom_setsym(*argv+i, x->inlet_name[i]);
        }
        
    }
    
    return 0;
    
}

void *grans_new(t_symbol *s, long argc, t_atom *argv)
{
    int n;
	t_grans *x = (t_grans *)object_alloc(granubuf_class);
    
	if (x) {
        
        x->base = (t_sample_grain *)calloc(DEFAULTMAXOSCILLATORS, sizeof(t_sample_grain));
        x->maxoverlap = DEFAULTMAXOSCILLATORS;
        
        x->numoutlets = 1;
        
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
                            x->maxoverlap = n;
                            // object_post((t_object *)x, "%d oscillators initialized", n);
                            break;
                        case 1:
                            n = atom_getlong(ap);
                            x->numoutlets = n;
                            // object_post((t_object *)x, "%d outlets?", n);
                            
                        default:
                            break;
                    }
                }
            }
            
        }        
		
        
        x->samplerate =  sys_getsr();
        if(x->samplerate<=0)
            x->samplerate = 44100;
        
        
        x->sampleinterval = 1.0 / x->samplerate;
        x->pkw = ( STABSZ * x->sampleinterval ) ;
        
        x->tabSamp = 1.0 / STABSZ;
        x->halftab = 0.5 * STABSZ;
        x->shapetabscale = NSHAPINGTABS - 1;
        x->wtabscale = STABSZ - 1;
        
        
        x->maxhz = (x->samplerate / 2) * x->pkw;
        x->nosc = x->next_nosc = 0;
        
        x->prev_in1 = 0.0;
                        
        grans_clear(x);
        
        x->always_on = 0;
        x->location = 0;
        x->rate = 1;
        x->dur = 100;
        x->w_shapex = 0.5;
        x->w_shapey = 0.5;
        x->outlet = -1;
        x->buf_index = 0;
        x->amp = 1.0;
        
        x->numbufs = 0;
        x->numenvbufs = 0;
        
        x->numwindows = 1;
        x->envbuf_name[0] = ps_granu_cos;
        x->tab_w_index[0] = COS;
        x->window_index = 0;

        x->numinlets = 1;
        x->inlet_type[0] = TRIGGER;
        x->inlet_name[0] = ps_granu_trigger;
        
        int i;
        for(i=0; i<GRANU_MAX_BUFFERS; i++ )
        {
            x->buffer_modified[i] = false;
            x->buf_frames[i] = 0;
            x->buf_nchans[i] = 0;
            x->buf[i] = NULL;
            
            x->envbuffer_modified[i] = false;
            x->envbuf_frames[i] = 0;
            x->envbuf_nchans[i] = 0;
            x->envbuf[i] = NULL;
        }
        
        for(i=1; i<GRANUBUF_NUMINLET_TYPES; i++)
        {
            x->inlet_type[i] = UNUSED;
        }
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d); //calls appropriate class_attr_accessors
            object_free(d);
        }
        
        dsp_setup((t_pxobject *)x, x->numinlets);
        x->count = (short *)malloc(x->numinlets * sizeof(short));
        
        n = x->numoutlets;
        while(n--)
            outlet_new(x, "signal");
        
        x->prev_maxoverlap = x->maxoverlap;

    //    object_attach_byptr_register(x, x, CLASS_BOX); // do we need this?
        
    } else {
        object_post((t_object *)x, "this is potentially bad!");
    }
    
	return (x);
}

int main(void)
{
	t_class *c = class_new("granubuf~", (method)grans_new, (method)grans_free, (long)sizeof(t_grans), NULL,   A_GIMME);
	
    class_addmethod(c, (method)grans_clear,		"clear",     0);
	class_addmethod(c, (method)grans_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)grans_assist,	"assist",	A_CANT, 0);
    class_addmethod(c, (method)granubuf_notify,	"notify",	A_CANT, 0);
    class_addmethod(c, (method)granubuf_float,  "float",    A_FLOAT, 0);
    class_addmethod(c, (method)granubuf_int,    "int",      A_LONG, 0);
    
    CLASS_ATTR_SYM_VARSIZE(c, "window", 0, t_grans, envbuf_name, numwindows, GRANU_MAX_BUFFERS );
    CLASS_ATTR_ACCESSORS(c, "window", granubuf_envbuf_get, granubuf_envbuf_set);
    CLASS_ATTR_LABEL(c, "window", 0, "window list");
    
    CLASS_ATTR_SYM_VARSIZE(c, "buffer", 0, t_grans, buf_name, numbufs, GRANU_MAX_BUFFERS);
    CLASS_ATTR_ACCESSORS(c, "buffer", granubuf_buf_get, granubuf_buf_set);
    CLASS_ATTR_LABEL(c, "buffer", 0, "buffer list");

    CLASS_ATTR_LONG(c, "alwayson", 0, t_grans, always_on);
  
    CLASS_ATTR_SYM_VARSIZE(c, "inlets", ATTR_SET_OPAQUE_USER, t_grans, inlet_type, numinlets, GRANUBUF_NUMINLET_TYPES);
    CLASS_ATTR_ACCESSORS(c, "inlets", granubuf_inlet_get, granubuf_inlet_set);
    CLASS_ATTR_LABEL(c, "inlets", 0, "inlet assignments");

    CLASS_ATTR_LONG(c, "numoutlets", ATTR_SET_OPAQUE_USER, t_grans, numoutlets);
    
    CLASS_ATTR_DOUBLE(c, "location", 0, t_grans, location);
    CLASS_ATTR_DOUBLE(c, "rate", 0, t_grans, rate);
    CLASS_ATTR_DOUBLE(c, "duration", 0, t_grans, dur);
    
    CLASS_ATTR_DOUBLE(c, "tex", 0, t_grans, w_shapex);
    CLASS_ATTR_LABEL(c, "tex", 0, "tex (for fof window only)");
    /*
    CLASS_ATTR_DOUBLE(c, "shapex", 0, t_grans, w_shapex);
    CLASS_ATTR_LABEL(c, "shapex", 0, "shapex(fof tex)");
    CLASS_ATTR_ALIAS(c, "shapex", "tex");
    
    CLASS_ATTR_DOUBLE(c, "shapey", 0, t_grans, w_shapey);
    */
    CLASS_ATTR_LONG(c, "window_index", 0, t_grans, window_index);
    CLASS_ATTR_LONG(c, "buffer_index", 0, t_grans, buf_index);
    CLASS_ATTR_LONG(c, "outlet", 0, t_grans, outlet);
    CLASS_ATTR_LONG(c, "amp", 0, t_grans, amp);
    
    CLASS_ATTR_LONG(c, "maxoverlap", 0, t_grans, maxoverlap);
    CLASS_ATTR_LABEL(c, "maxoverlap", 0, "max grain overlap");

    CLASS_ATTR_SYM(c, "setbuffer", 0, t_grans, current_buffer);
    CLASS_ATTR_ACCESSORS(c, "setbuffer", granubuf_currentbuf_get, granubuf_currentbuf_set);
    CLASS_ATTR_LABEL(c, "setbuffer", 0, "current buffer");

    CLASS_ATTR_SYM(c, "setwindow", 0, t_grans, current_window);
    CLASS_ATTR_ACCESSORS(c, "setwindow", granubuf_currentwindow_get, granubuf_currentwindow_set);
    CLASS_ATTR_LABEL(c, "setwindow", 0, "current window");
    
    if (!granu_tableExists)
    {
        granu_makeTables();
        
        ps_granu_cos = gensym("cos");
        ps_granu_fof = gensym("fof");
        ps_granu_dampedsine = gensym("dampedsine");
        ps_granu_sinc = gensym("sinc");
        ps_buffer_modified = gensym("buffer_modified");
        
        ps_granu_trigger = gensym("trigger");
        ps_granu_location = gensym("location");
        ps_granu_rate = gensym("rate");
        ps_granu_dur = gensym("duration");
        ps_granu_shapex = gensym("shapex");
        ps_granu_shapey = gensym("shapey");
        ps_granu_win_idx = gensym("window_index");
        ps_granu_outlet = gensym("outlet");
        ps_granu_buf_idx = gensym("buffer_index");
        ps_granu_amp = gensym("amp");
        ps_granu_tex = gensym("tex");
        
    }

    class_dspinit(c);
	class_register(CLASS_BOX, c);
	granubuf_class = c;
    
	return 0;
}
