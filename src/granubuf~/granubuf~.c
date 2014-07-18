/*
  Written by Rama Gottfried, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2013, The Regents of
  the University of California (Regents). 
  Permission to use, copy, modify, distribute, and distribute modified versions
  of this software and its documentation without fee and without a signed
  licensing agreement, is hereby granted, provided that the above copyright
  notice, this paragraph and the following two paragraphs appear in all copies,
  modifications, and distributions.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
  HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

/*--
 granubuf~ v.0.2
 
 notes: 
 - in new version, window buffers should ALWAYS be declared, if none are declared use cos or hanning at index 0
 - special is the fof routinue which REQUIRES the tex shaping function... so fof cannot be added after instantiation if there are no shaping function inlets, so fof tex can not be adjusted at sample rate if there is no inlet, but not a show stopper
 
 
 internal window types:
    0: cos
    1: fof
    2: dampedsine
    3: sinc
 
 to do:
    - add samplerate check in dsp function, if this gets changed after buffer info is loaded it screws up the pitch, or calculate increment in the perform routine with the samplerate information
    - improve chirp
    - add information outlet to get grain information

 ***
 
 eventually:
    - cubic, bspline, resampled interpolation for nicer slow rate (but linear is sounding not too bad)
    - adjust for user's buffer sampling rate?
 
--*/


#define NAME "granubuf~"
#define DESCRIPTION "Generalized samplerate buffer granulator"
#define AUTHORS "Rama Gottfried"
#define COPYRIGHT_YEARS "2013"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "z_dsp.h"
#include "buffer.h"

#include "granu.h"

#define GRANUBUF_NUMINLET_TYPES 13

// -------- buffer proxy object class, to handle buffer notifications


t_class *graunu_buffer_proxy_class = NULL;

typedef struct _buffer_proxy
{
    t_object        ob;
    t_buffer_ref    *ref;
    t_symbol        *name;
    t_bool          buffer_modified;
    double          sr;
    long            nframes;
    long            nchans;
} t_buffer_proxy;


t_max_err buffer_proxy_notify(t_buffer_proxy *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == ps_buffer_modified)
        x->buffer_modified = true;
	return buffer_ref_notify(x->ref, s, msg, sender, data);
}

void buffer_proxy_set_ref(t_buffer_proxy *x, t_symbol *s)
{
    x->name = s;
    buffer_ref_set(x->ref, x->name);
    x->buffer_modified = true;
}


void *buffer_proxy_new(t_symbol *s)
{
    
    t_buffer_proxy *x = NULL;
    
    if((x = (t_buffer_proxy *)object_alloc(graunu_buffer_proxy_class)))
    {
        x->name = s;
        x->ref = buffer_ref_new((t_object *)x, x->name);
        x->buffer_modified = true;
    }
    
    return x;
}

// -------- graubuf~

typedef enum _granubuf_in
{
    UNUSED = -1,
    TRIGGER,
    STARTLOCATION,
    ENDLOCATION,
    RATE,
    DURATION,
    WIN_INDEX,
    OUTLET,
    BUF_INDEX,
    AMP,
    TEX, //only used with fof window
    CHIRP, //only used with chirp window (chirp window used to specify log or linear)
    CHIRPTYPE // 0 linear, 1 exponential
} e_granubuf_in;


typedef enum _granubuf_end_mode
{
    NOPLAY,
    RESIZE,
    CLIP
    //    LOOP, //might want to add these?
    //    REFLECT,
} e_granu_end_mode;

typedef struct _sample_grain
{
    double      startpoint; // in samples (floats for interpolation)
    double      endpoint; // in samples
    
    double      phase_current;
    double      phase_inc;
    
    double      chirp_rate;
    double      chirp_direction;
    long        chirp_type;
    
    int         window_index;
    
    double      window_phase_curent;
    double      window_phase_inc;
       
    double      tex;
    int         offset;

    int         outlet;
    int         buf_index;
    double      amp;
    

} t_sample_grain;


typedef struct _grans {
	t_pxobject		ob;
    
    int             end_mode;
    int             interpolation;
    
    //float input memory
    double          start; // location in ratio 0-1
    double          end; // end in ratio 0-1
    double          rate; 
    double          dur;
    
    double          tex;
    long            window_index; //change to window_index
    long            outlet;
    long            buf_index;
    double          amp;
    long            chirp_type; // 0 linear, 1 exponential
    double          chirp_rate;
    
    t_symbol        *current_window;
    t_symbol        *current_buffer;
    
    t_symbol        *inlet_name[GRANUBUF_NUMINLET_TYPES];
    e_granubuf_in   inlet_type[GRANUBUF_NUMINLET_TYPES];
    long            numinlets;
    
    int             parameter_error;
    e_granubuf_in   prev_params[2];
    e_granubuf_in   overwrite_param; //this should be checked in the grain creation since buffers sizes can vary
    int             num_time_param_inlets;
    
    //sample buffer array,
    t_buffer_proxy  **buf_proxy;
	t_symbol        *buf_name[GRANU_MAX_BUFFERS];
    long            numbufs;

    //sample buffer array for external buffers
    t_buffer_proxy  **envbuf_proxy;
    long            numenvbufs; //number of external buffers

    t_symbol        *window_name[GRANU_MAX_BUFFERS];
    int             numwindows;
//includes count of internal and external buffers (internal are global vars)
    int             tab_w_index[GRANU_MAX_BUFFERS]; //offset by internal windows, +(GRANUBUF_NUMINTERNAL_WINDOWS-1)

    
    int             numoutlets;
    int             outletiter;
    
    long            maxoverlap;
    long            prev_maxoverlap;
    
    double          maxhz;
    t_sample_grain  *base;
    
    long            nosc;
    long            next_nosc;
    double          pk;
    double          pkw;
    
    double          tabSamp;
    double          halftab;
    int             shapetabscale;
    int             wtabscale;
        
    double          samplerate;
    double          sampleinterval;
    
    double          prev_in1;
    
    int             always_on;
    
    short           *count;
    
    t_critical      lock;
    
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
            case STARTLOCATION:
                sprintf(s, "(signal/float) relative sample start time 0-1 ");
                break;
            case ENDLOCATION:
                sprintf(s, "(signal/float) relative sample end time 0-1 ");
                break;
            case RATE:
                sprintf(s, "(signal/float) playback rate ");
                break;
            case DURATION:
                sprintf(s, "(signal/float) grain dur ms ");
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
            case CHIRP:
                sprintf(s, "(signal/float) chirp rate ");
                break;
            case CHIRPTYPE:
                sprintf(s, "(signal/float) chirp type (0 linear (default), 1 exponential ");
                break;
            case TEX:
                sprintf(s, "(signal/float) 0-1 attack ratio for FOF window only ");
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
        case STARTLOCATION:
            x->start = f;
            break;
        case ENDLOCATION:
            x->end = f;
            break;
        case RATE:
            x->rate = f;
            break;
        case DURATION:
            x->dur = f;
            break;
        case TEX:
            x->tex = f;
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
        case CHIRP:
            x->chirp_rate = f;
            break;
        case CHIRPTYPE:
            x->chirp_type = f;
            break;
        default:
            break;
    }

}

void granubuf_int(t_grans *x, int f)
{
    int inlet = proxy_getinlet((t_object *)x);
    switch (x->inlet_type[inlet]) {
        case STARTLOCATION:
            x->start = (double)f;
            break;
        case ENDLOCATION:
            x->end = (double)f;
            break;
        case RATE:
            x->rate = (double)f;
            break;
        case DURATION:
            x->dur = (double)f;
            break;
        case TEX:
            x->tex = (double)f;
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
        case CHIRP:
            x->chirp_rate = (double)f;
            break;
        case CHIRPTYPE:
            x->chirp_type = (double)f;
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
        p->window_index  = 0;
        p->tex = 0.0;
	}
    x->nosc = 0;
}

//pre-test that start and end are between 0-1, and bufferindex value
void grans_setNewGrain(t_grans *x, int offset, double chirprate, long chirptype, double start, double end, double rate, double duration, double tex, int window_index, int outlet, int bufferindex, double amp)
{
   // post("%s j[%d] start[%f] end[%f] rate[%f] dur[%f] type[%d] buf_index[%d] ", __func__, offset, start, end, rate, duration, (int)window_index, bufferindex);

    int i, maxoverlap = x->maxoverlap;
    t_sample_grain *o = x->base;
    double pkw = x->pkw;

    double frames, num_frameIdx, dur2ratioScalar, dur_hz, _dur, _rate, _end; // using _vars for "low_level" versions of inputs, i.e. calculated here, not passed in

    e_granubuf_in overwrite = x->overwrite_param;
    e_granu_end_mode end_mode = x->end_mode;
    
    for ( i = 0; i < maxoverlap; ++i )
    {
        if( o->phase_inc == 0.0 )
        {
            frames = (double)x->buf_proxy[bufferindex]->nframes;
//            post("%s frames %i", __func__, x->buf_proxy[bufferindex]->nframes);
            if(frames == 0)  return;
            
            num_frameIdx = frames - 1; //probably check buf_frames > 0
            dur2ratioScalar = 1 / (frames * x->sampleinterval * 1000.0);

            o->buf_index = bufferindex;
            o->startpoint = start * num_frameIdx;

            // ms to samples( ms * x->samplerate * 0.001 )
            // ratio to samples( ratio * numframes ) //numframes is length in samples

            switch (overwrite) {
                case DURATION:
                    if(rate == 0 || start >= end) return;

                    _end = end * num_frameIdx;   //calc end, rate(phase_inc)
                    _rate = rate;
                    _dur = ((end - start) * frames * x->sampleinterval * 1000.0) / fabs(rate);
                    //duration = rate * (end - start)
                   // post("duration overwrite end %f rate %f dur %f",_end, _rate, _dur);
                    break;
                case RATE:
                    if(start >= end) return;
                    _end = end * num_frameIdx;   //calc end, duration
                    _dur = duration;
                    _rate = (end - start) / (_dur * dur2ratioScalar);
                    //rate(phase_inc) = (end - start) / (duration / bufferduration_ms)
                    //post("rate overwrite end %f rate %f dur %f", _end, _rate, _dur);
                    break;
                case ENDLOCATION:
                    _rate = rate; //calc rate(phase_inc), duration
                    _dur = duration;
                    _end = (start + (_dur * dur2ratioScalar)) * num_frameIdx;
                    //end = (start + (duration / bufferduration_ms)) * rate
                    //post("end overwrite end %f rate %f dur %f", _end, _rate, _dur);

                    break;
                default:
                    post("NO PASSIVE PARAMETER SET");
                    return;
                    break;
            }
            
            if(_end >= frames)
            {
                switch (end_mode) {
                    case NOPLAY: // 0
                        return; //exit leaving phase_inc at 0.0
                        break;
                    case RESIZE: // 1
                        _end = num_frameIdx;
                        _dur = ((1.0 - start) * frames * x->sampleinterval * 1000.0) / fabs(rate);
                        dur_hz = 1000.0 / _dur;
                        break;
                    case CLIP: // 4
                    default:
                        dur_hz = 1000.0 / duration;
                        break; //let window duration be longer than file and just cut off
                        //loop and reflect modes deal with clipping in perform routine
                }
            } else {
                dur_hz = 1000.0 / _dur;
            }
            
            //do end check, and adjust accordingly for @end_mode
            
            o->window_index = window_index;
            o->phase_inc = _rate;
            o->endpoint = _end;
            
            o->chirp_direction = (chirprate > 0) ? 1 : -1;
            o->chirp_rate = fabs(chirprate) * pkw;
            o->chirp_type = chirptype;
            
            

//            post("%s %d", __func__, x->tab_w_index[window_index]);
            if(x->tab_w_index[window_index] < GRANUBUF_NUMINTERNAL_WINDOWS) //do safety test on grain creation
            {
                o->window_phase_inc = dur_hz * pkw;
//                post("%s %f %f %f", __func__, o->window_phase_inc, o->endpoint, frames );
            }
            else
            {
                o->window_phase_inc = dur_hz *
                ((double)x->envbuf_proxy[x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS]->nframes) * x->sampleinterval;
            /*    post("%s %f %i %i %f", __func__,
                     o->window_phase_inc,
                     x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS,
                     x->envbuf_proxy[x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS]->nframes ,
             ((double)x->envbuf_proxy[x->tab_w_index[window_index] - GRANUBUF_NUMINTERNAL_WINDOWS]->nframes) * x->sampleinterval);
            */
            }
            
            
            o->offset = offset;
            
            o->window_phase_curent = 0.0; //offset for cos
            
            o->tex = (tex == 0) ? 0 : 1.0 / CLAMP(tex, 0., 1.);;

            o->outlet = (outlet == -1) ? x->outletiter++ % x->numoutlets : outlet % x->numoutlets;
            
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
    double start = x->start;
    double end = x->end;
    double rate = x->rate;
    double dur = x->dur;
    double tex = x->tex;
    double window_index = x->window_index;
    int    outlet = x->outlet;
    int    buf_index = x->buf_index;
    double gr_amp = x->amp;
    double chirprate = x->chirp_rate;
    int    chirptype = x->chirp_type;
    
    int    interpolation = x->interpolation;
    
    const int alwayson = x->always_on;
        
    const long maxoverlap = CLAMP(x->maxoverlap, 0, DEFAULTMAXOSCILLATORS);
    
    if(maxoverlap != x->maxoverlap) x->maxoverlap = maxoverlap;
    
    const double halftab = x->halftab;
    
    long       tex_pc;
    double     pc, wpc, chirpdir, w_tabSamp;
    double     pi, wpi, amp, pSamp, lowerSamp, upperSamp, upperVal, outSamp;
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
    int             valid[numbufs], modified[numbufs];
    
    for (i=0; i<numbufs; i++)
    {
    
        buffer[i] = buffer_ref_getobject(x->buf_proxy[i]->ref);
        
        if(!buffer[i])
            valid[i] = 0;
        else
        {
            tab[i] = buffer_locksamples(buffer[i]);

            if(!tab[i])
                valid[i] = 0;
            else
            {
                modified[i] = x->buf_proxy[i]->buffer_modified;

                
                if(modified[i])
                {
                    frames[i] = buffer_getframecount(buffer[i]);
                    nchans[i] = buffer_getchannelcount(buffer[i]);
                    x->buf_proxy[i]->nframes = frames[i];
                    x->buf_proxy[i]->nchans = nchans[i];
                    x->buf_proxy[i]->buffer_modified = false;
                }
                else
                {
                    frames[i] = x->buf_proxy[i]->nframes;
                    nchans[i] = x->buf_proxy[i]->nchans;
                }
                
                valid[i] = (nchans[i] > 0 && frames[i] > 0);

            }
            
        }
    }
    
    t_buffer_obj    *w_buffer[numenvbufs];
    t_float         *w_buftab[numenvbufs];
    int             w_valid[numenvbufs], w_modified[numenvbufs];
    long w_frames[numbufs], w_nchans[numbufs];
    
    for (i=0; i<numenvbufs; i++) {
        
        w_buffer[i] = buffer_ref_getobject(x->envbuf_proxy[i]->ref);
        
        
        if(!w_buffer[i])
            w_valid[i] = 0;
        else
        {
            w_buftab[i] = buffer_locksamples(w_buffer[i]);
            
            if(!w_buftab[i])
                w_valid[i] = 0;
            else
            {
                w_modified[i] = x->envbuf_proxy[i]->buffer_modified;
                
                
                if(w_modified[i])
                {
                    w_frames[i] = buffer_getframecount(w_buffer[i]);
                    w_nchans[i] = buffer_getchannelcount(w_buffer[i]);
                    x->envbuf_proxy[i]->nframes = w_frames[i];
                    x->envbuf_proxy[i]->nchans = w_nchans[i];
                    x->envbuf_proxy[i]->buffer_modified = false;
                }
                else
                {
                    w_frames[i] = x->envbuf_proxy[i]->nframes;
                    w_nchans[i] = x->envbuf_proxy[i]->nchans;
                }
                
                w_valid[i] = (w_nchans[i] > 0 && w_frames[i] > 0);
                
            }
            
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
                else if(x->inlet_type[i] == STARTLOCATION)
                    start = inlets[i][j];
                else if(x->inlet_type[i] == ENDLOCATION)
                    end = inlets[i][j];
                else if(x->inlet_type[i] == DURATION)
                    dur = inlets[i][j];
                else if(x->inlet_type[i] == RATE)
                    rate = inlets[i][j];
                else if(x->inlet_type[i] == TEX)
                    tex = inlets[i][j];
                else if(x->inlet_type[i] == WIN_INDEX)
                    window_index = inlets[i][j];
                else if(x->inlet_type[i] == BUF_INDEX)
                    buf_index = inlets[i][j];
                else if(x->inlet_type[i] == OUTLET)
                    outlet = inlets[i][j];
                else if(x->inlet_type[i] == AMP)
                    gr_amp = inlets[i][j];
                else if(x->inlet_type[i] == CHIRP)
                    chirprate = inlets[i][j];
                else if(x->inlet_type[i] == CHIRPTYPE)
                    chirptype = inlets[i][j];
            }
            
            
        }
        
        //add valid tests here
        if ( trigger != 0.0 && start >= 0.0 && start < 1 && end > 0.0 && dur > 0.0 && buf_index < numbufs && buf_index >= 0 && window_index < x->numwindows && window_index >= 0 && x->nosc < maxoverlap)
        {
//            post("j[%d] start[%f] end[%f] rate[%f] dur[%f] type[%d] buf_index[%d] ", j, start, end, rate, dur, (int)window_index, buf_index);
            grans_setNewGrain( x, j, chirprate, chirptype, start, end, rate, dur, tex, window_index, outlet, buf_index, gr_amp );
        }



        prev_trig = trigger;
        
        for(i = 0; i < numouts; i++){
            outlets[i][j] = 0.0;
        }

    }

    x->prev_in1 = prev_trig;

    for( i = 0; i < x->maxoverlap; i++, o++)
    {
        
        pi      = o->phase_inc;
        if( pi == 0.0 && !alwayson) continue;
        
        
        pc      = o->phase_current;
        
        wpc     = o->window_phase_curent;
        wpi     = o->window_phase_inc;

        outletnum   = o->outlet;
        
        start       = o->startpoint;
        end         = o->endpoint;
        
        buf_index   = o->buf_index;
        gr_amp      = o->amp;
        
        j           = o->offset;

        w_index     = o->window_index; //index of all windows (including internal windows)
        
        chirprate   = o->chirp_rate;
        chirpdir    = o->chirp_direction;
        chirptype   = o->chirp_type;
        
        if(x->tab_w_index[w_index] < GRANUBUF_NUMINTERNAL_WINDOWS) //do safety test on grain creation
        {
            num_wframes = STABSZ;
            num_wchans = 1;
            w_tab = internal_window[x->tab_w_index[w_index]];
        }
        else
        {

            w_bufoffset = x->tab_w_index[w_index] - GRANUBUF_NUMINTERNAL_WINDOWS;
            if(!w_valid[w_bufoffset]) goto release;
            
            num_wframes = w_frames[w_bufoffset];
            num_wchans = w_nchans[w_bufoffset];
            w_tab = w_buftab[w_bufoffset];
            
        }
        
        if(!valid[buf_index]) goto release;
        
        w_tabSamp     = 1.0 / num_wframes;
 
        if( x->tab_w_index[w_index] == FOF) //fof is a special case which requres two window tables
        {
            tex = o->tex;
            while(j < sampleframes && wpc < num_wframes && (pc + start) < frames[buf_index] && (end + pc) >= 0)
            {

                tex_pc = (uint32_t)wrapPhase(wpc * tex, STABSZ); //<< phase wrap not really necessary if bounds are checked for tex
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
                    pSamp = pc + start;
                else
                    pSamp = pc + end;
                
                if (interpolation == 1) //<< probably not good to check this on every sample!, also should do loop interpolation
                {
                    lowerSamp = floor(pSamp);
                    upperSamp = ceil(wpc);
                    upperVal = (upperSamp < num_wframes) ? tab[buf_index][ nchans[buf_index] * (uint32_t)upperSamp ] : 0.0;

                    outSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)lowerSamp  ], upperVal, pSamp - lowerSamp);
                }
                else
                {
                    outSamp = tab[buf_index][ nchans[buf_index] * (uint32_t)pSamp  ];;
                }
                
                outlets[outletnum][j] += amp * outSamp;
                
                pc += pi;
                wpc += wpi;

                j++;
                
            }
        }
        else
        {
            while(j < sampleframes && wpc < num_wframes && (pc + start) < frames[buf_index] && (end + pc) >= 0.0)
            {
                
                
                if(pi > 0)
                    pSamp = pc + start;
                else
                    pSamp = pc + end;
                
                
                if (interpolation == 1) 
                {
                    
                    lowerSamp = floor(wpc);
                    upperSamp = ceil(wpc);
                    
                    upperVal = (upperSamp < num_wframes) ? w_tab[ num_wchans * (uint32_t)upperSamp ] : 0.0;

                    amp = gr_amp * linear_interp(w_tab[ num_wchans * (uint32_t)lowerSamp  ], upperVal, wpc - lowerSamp);
                    
                    lowerSamp = floor(pSamp);
                    upperSamp = ceil(pSamp);
                    upperVal = (upperSamp < frames[buf_index]) ? tab[buf_index][ nchans[buf_index] * (uint32_t)upperSamp ] : 0.0;

                    outSamp = linear_interp(tab[buf_index][ nchans[buf_index] * (uint32_t)lowerSamp  ], upperVal, pSamp - lowerSamp);
                }
                else
                {
                    amp = gr_amp * w_tab[ num_wchans * (uint32_t)wpc  ];
                    outSamp = tab[buf_index][ nchans[buf_index] * (uint32_t)pSamp  ];;
                }

                
                outlets[outletnum][j] += amp * outSamp;

                if(chirptype == 1)
                    pc += pi + (pow(chirprate, wpc * w_tabSamp) * chirpdir);
                else
                    pc += pi + (wpc * w_tabSamp * chirprate * chirpdir);
                
                wpc += wpi;
                
                j++;
            }

        }
        
        o->offset = 0;

        
        if(wpc < num_wframes && (pc + start) <= frames[buf_index] && pc >= 0 && ((frames[buf_index] - start) + pc) >= 0)
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
    {
        if(tab[i])
            buffer_unlocksamples(buffer[i]);
    }
    for(i=0; i<numenvbufs; i++)
    {
        if(w_tab[i])
            buffer_unlocksamples(w_buffer[i]);
    }
    
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


void granu_info(t_grans *x)
{

    object_post((t_object *)x, "nosc %i", x->nosc);
    
    t_sample_grain *o = x->base;
    int i;
    for( i = 0; i < x->maxoverlap; i++, o++)
    {
    post("[%i] buf_id %i start %f end %f pc %f pi %f wind_idx %i wpc %f wi %f amp %f",
         i,
         o->buf_index,
         o->startpoint, // in samples (floats for interpolation)
        o->endpoint, // in samples
        o->phase_current,
        o->phase_inc,
        o->window_index,
        o->window_phase_curent,
        o->window_phase_inc,
         o->amp); ////        o->tex, o->offset, o->outlet,

    }
    
    
}

//*************************************************************************************************

t_max_err granubuf_window_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    
    int i;
    t_symbol *s;
    
    if(argc && argv)
    {
        if(argc < GRANU_MAX_BUFFERS)
        {
            critical_enter(x->lock);
            i = 0;
            x->numenvbufs = 0;
            while(i < argc)
            {
                if(atom_gettype(argv+i) != A_SYM)
                {
                    object_error((t_object *)x, "non symbol buffer name");
                    return -1;
                }
                
                s = atom_getsym(argv+i);
                
                if(s) {
                    
                        x->window_name[i] = s;
                        
                        if (s == ps_granu_cos)
                            x->tab_w_index[i] = COS;
                        else if (s == ps_granu_fof)
                            x->tab_w_index[i] = FOF;
                        else if (s == ps_granu_dampedsine)
                            x->tab_w_index[i] = DAMPEDSINE;
                        else if (s == ps_granu_sinc)
                            x->tab_w_index[i] = SINC;
                        else
                        {
                            if(!x->envbuf_proxy[x->numenvbufs])
                                x->envbuf_proxy[x->numenvbufs] = buffer_proxy_new(x->window_name[i]);
                            else
                                buffer_proxy_set_ref(x->envbuf_proxy[x->numenvbufs], x->window_name[i]);
                            
                            x->tab_w_index[i] = x->numenvbufs + GRANUBUF_NUMINTERNAL_WINDOWS;
                            x->numenvbufs++;
                            
                        }
                    
                } else {
                    post("%i %s", __LINE__, s->s_name);
                    object_error((t_object *)x, "must have buffer name");
                }
                i++;
            }
            x->numwindows = argc;

            
            critical_exit(x->lock);
        }
    }
 //   post("numwindows %d numenvbufs %d", x->numwindows, x->numenvbufs);
    
    return 0;
}

t_max_err granubuf_window_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    char alloc;
    int i;
    
    atom_alloc_array(x->numwindows, argc, argv, &alloc);
    
    if (x->numwindows)
    {
        for(i = 0; i < x->numwindows; i++)
        {
            atom_setsym(*argv+i, x->window_name[i]);
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
            if(x->window_name[i] == name)//names include internal buffers
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
        atom_setsym(*argv, x->window_name[x->window_index]);
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
   
    
    int i;
    t_symbol *s;
    
    if(argc && argv)
    {
        if(argc < GRANU_MAX_BUFFERS)
        {
            critical_enter(x->lock);
            if(argc >= x->numbufs)
            {
                i = 0;
                while(i < x->numbufs)
                {
                    if(atom_gettype(argv+i) != A_SYM)
                    {
                        object_error((t_object *)x, "non symbol buffer name");
                        return -1;
                    }
                    
                    s = atom_getsym(argv+i);
                    if(s) {
                        if(s != x->buf_name[i])
                        {
                            buffer_proxy_set_ref(x->buf_proxy[i], s);
                            x->buf_name[i] = s;
                        }
                    } else {
                        object_error((t_object *)x, "must have buffer name");
                    }
                    i++;
                }
                
                while (i < argc)
                {
                    if(atom_gettype(argv+i) != A_SYM)
                    {
                        object_error((t_object *)x, "non symbol buffer name");
                        return -1;
                    }
                    
                    x->buf_name[i] = atom_getsym(argv+i);
                    if(x->buf_name[i])
                        x->buf_proxy[i] = buffer_proxy_new(x->buf_name[i]);
                    else
                        object_error((t_object *)x, "must have buffer name");
                    
                    i++;
                }
                
            }
            else if(argc < x->numbufs)
            {
                i = 0;
                while(i < argc)
                {
                    
                    if(atom_gettype(argv+i) != A_SYM)
                    {
                        object_error((t_object *)x, "non symbol buffer name");
                        return -1;
                    }
                    
                    s = atom_getsym(argv+i);
                    if(s && s != x->buf_name[i] ) {
                        buffer_proxy_set_ref(x->buf_proxy[i], s);
                        x->buf_name[i] = s;
                    } else {
                        //object_error((t_object *)x, "must have buffer name");
                    }
                    i++;
                }
                
            }
            x->numbufs = argc;
            critical_exit(x->lock);
        }
    }
    
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
            if(x->buf_name[i] == name && x->buf_proxy[i])
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

void granubuf_postAdjustParam(t_grans *x, e_granubuf_in b)
{
    switch (b) {
        case RATE:
            object_post((t_object *)x, "passive parameter set to: rate");
            break;
        case DURATION:
            object_post((t_object *)x, "passive parameter set to: duration");
            break;
        case ENDLOCATION:
            object_post((t_object *)x, "passive parameter set to: end");
            break;
        case UNUSED:
            post("unused");
            break;
        default:
            break;
    }
}

int granubuf_getPassive(t_grans *x, int a, int b)
{
    if((a == DURATION && b == ENDLOCATION) || (b == DURATION && a == ENDLOCATION))
    {
//        post("RATE");
        return RATE;
    }
    else if((a == DURATION && b == RATE) || (b == DURATION && a == RATE) || b == UNUSED)
    {
//        post("ENDLOCATION");
        return ENDLOCATION;
    }
    else if((a == ENDLOCATION && b == RATE) || (b == ENDLOCATION && a == RATE))
    {
//        post("DURATION");
        return DURATION;
    }
    
    return UNUSED;
}

void granubuf_setAdjustParam(t_grans *x, e_granubuf_in thisIn, int attrInit)
{
    
    if(x->num_time_param_inlets == 0)
    {
        e_granubuf_in a = x->prev_params[0];
        if(a == thisIn && !attrInit) return;
        
//        e_granubuf_in b = x->prev_params[1];

        //push to stack
        x->prev_params[0] = thisIn;
        x->prev_params[1] = a;
/*
        post("0");
        granubuf_postAdjustParam(x, thisIn);
        post("1");
        granubuf_postAdjustParam(x, a);
        post("---------------");
*/
        e_granubuf_in passive = granubuf_getPassive(x, thisIn, a);
        
        if(x->overwrite_param != passive)
        {
            x->overwrite_param = passive;
            granubuf_postAdjustParam(x, passive);
        }
        
        
    }
    else if(x->num_time_param_inlets == 1)
    {
        e_granubuf_in a = x->prev_params[1];
        if(a == thisIn) return;
        
        x->prev_params[1] = thisIn;
        e_granubuf_in inlet = x->prev_params[0];
/*
        post("0b");
        granubuf_postAdjustParam(x, thisIn);
        post("1b");
        granubuf_postAdjustParam(x, inlet);
        post("---------------");
*/
        e_granubuf_in passive = granubuf_getPassive(x, inlet, thisIn);
        
        if(x->overwrite_param != passive)
        {
            x->overwrite_param = passive;
            granubuf_postAdjustParam(x, passive);
        }
    
    }
    
}

t_max_err granubuf_duration_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    if(x->num_time_param_inlets == 1 && x->prev_params[0] == DURATION)
    {
        object_error((t_object *)x, "duration is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && (x->prev_params[1] == DURATION || x->prev_params[0] == DURATION))
    {
        object_error((t_object *)x, "duration is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && x->prev_params[1] != DURATION && x->prev_params[0] != DURATION)
    {
        object_error((t_object *)x, "duration is configured as a passive parameter ");
        return -1;
    }
    
    
    if(argc == 1 && argv)
    {
        double duration = -1;
        
        if(atom_gettype(argv) == A_FLOAT)
            duration = atom_getfloat(argv);
        else if(atom_gettype(argv) == A_LONG)
            duration = (double)atom_getfloat(argv);
        else if (atom_gettype(argv) == A_SYM || duration < 0)
        {
            object_error((t_object *)x, "unknown duration value ");
            return -1;
        }
        
        x->dur = duration;
        granubuf_setAdjustParam(x, DURATION, 0);
        
        
        return 0;
    }
    
    return -1;
}

t_max_err granubuf_duration_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    atom_alloc(argc, argv, &alloc);
    atom_setfloat(*argv, x->dur);

    return 0;
}

t_max_err granubuf_rate_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    if(x->num_time_param_inlets == 1 && x->prev_params[0] == RATE)
    {
        object_error((t_object *)x, "rate is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && (x->prev_params[1] == RATE || x->prev_params[0] == RATE))
    {
        object_error((t_object *)x, "rate is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && x->prev_params[1] != RATE && x->prev_params[0] != RATE)
    {
        object_error((t_object *)x, "rate is configured as a passive parameter ");
        return -1;
    }
    
    
    if(argc == 1 && argv)
    {
        double rate = 0;
        
        if(atom_gettype(argv) == A_FLOAT)
            rate = atom_getfloat(argv);
        else if(atom_gettype(argv) == A_LONG)
            rate = (double)atom_getfloat(argv);
        else if (atom_gettype(argv) == A_SYM )
        {
            object_error((t_object *)x, "unknown rate value ");
            return -1;
        }
        
        x->rate = rate;
        granubuf_setAdjustParam(x, RATE, 0);

        return 0;
    }
    
    return -1;
}

t_max_err granubuf_rate_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    atom_alloc(argc, argv, &alloc);
    atom_setfloat(*argv, x->rate);
    
    return 0;
}

t_max_err granubuf_end_set(t_grans *x, t_object *attr, long argc, t_atom *argv)
{
    if(x->num_time_param_inlets == 1 && x->prev_params[0] == ENDLOCATION)
    {
        object_error((t_object *)x, "end is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && (x->prev_params[1] == ENDLOCATION || x->prev_params[0] == ENDLOCATION))
    {
        object_error((t_object *)x, "end is set as inlet  ");
        return -1;
    }
    else if(x->num_time_param_inlets == 2 && x->prev_params[1] != ENDLOCATION && x->prev_params[0] != ENDLOCATION)
    {
        object_error((t_object *)x, "end is configured as a passive parameter ");
        return -1;
    }
    
    if(argc == 1 && argv)
    {
        double end = 0;
        
        if(atom_gettype(argv) == A_FLOAT)
            end = atom_getfloat(argv);
        else if(atom_gettype(argv) == A_LONG)
            end = (double)atom_getfloat(argv);
        else if (atom_gettype(argv) == A_SYM )
        {
            object_error((t_object *)x, "unknown end value ");
            return -1;
        }
        
        x->end = end;
        granubuf_setAdjustParam(x, ENDLOCATION, 0);

        return 0;
    }
    
    return -1;
}

t_max_err granubuf_end_get(t_grans *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    atom_alloc(argc, argv, &alloc);
    atom_setfloat(*argv, x->end);
    
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
        int param_interaction_count = 0;
        
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
                else if(inlet_name == gensym("location"))
                {
                    x->inlet_name[x->numinlets] = ps_granu_start;
                    x->inlet_type[x->numinlets++] = STARTLOCATION;
                }
                else if(inlet_name == ps_granu_start)
                {
                    x->inlet_name[x->numinlets] = ps_granu_start;
                    x->inlet_type[x->numinlets++] = STARTLOCATION;
                }
                else if(inlet_name == ps_granu_end)
                {
                    x->inlet_name[x->numinlets] = ps_granu_end;
                    x->inlet_type[x->numinlets++] = ENDLOCATION;
                    if(++param_interaction_count == 3)
                    {
                        object_error((t_object *)x, "duration, rate & end are potentially conflicting parameters! ");
                        object_error((t_object *)x, "please select only two of these options (see help patch for more information) ");
                        x->parameter_error = 1;
                        return -1;
                    }
                    
                    granubuf_setAdjustParam(x, ENDLOCATION, 1);
                }
                else if(inlet_name == ps_granu_dur)
                {
                    x->inlet_name[x->numinlets] = ps_granu_dur;
                    x->inlet_type[x->numinlets++] = DURATION;
                    if(++param_interaction_count == 3)
                    {
                        object_error((t_object *)x, "duration, rate & end are potentially conflicting parameters! ");
                        object_error((t_object *)x, "please select only two of these options (see help patch for more information) ");
                        x->parameter_error = 1;
                        return -1;
                    }
                    granubuf_setAdjustParam(x, DURATION, 1);
                }
                else if(inlet_name == ps_granu_rate)
                {
                    x->inlet_name[x->numinlets] = ps_granu_rate;
                    x->inlet_type[x->numinlets++] = RATE;
                    if(++param_interaction_count == 3)
                    {
                        object_error((t_object *)x, "duration, rate & end are potentially conflicting parameters! ");
                        object_error((t_object *)x, "please select only two of these options (see help patch for more information) ");
                        x->parameter_error = 1;
                        return -1;
                    }
                    granubuf_setAdjustParam(x, RATE, 1);

                }
                else if(inlet_name == ps_granu_tex)
                {
                    x->inlet_name[x->numinlets] = ps_granu_tex;
                    x->inlet_type[x->numinlets++] = TEX;
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
                else if(inlet_name == ps_granu_chirprate)
                {
                    x->inlet_name[x->numinlets] = ps_granu_chirprate;
                    x->inlet_type[x->numinlets++] = CHIRP;
                }
                else if(inlet_name == ps_granu_chirptype)
                {
                    x->inlet_name[x->numinlets] = ps_granu_chirptype;
                    x->inlet_type[x->numinlets++] = CHIRPTYPE;
                }
                else
                {                        
                    object_error((t_object *)x, "unknown parameter name: %s", inlet_name->s_name);
                    return -1;
                }
                
            }
            
        }
        x->num_time_param_inlets = param_interaction_count;
       // post("num param inlets %d", x->num_time_param_inlets);

    }


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


void grans_free(t_grans *x)
{
    dsp_free((t_pxobject *)x);
    
    free(x->base);
    x->base = NULL;
    
    free(x->count);
    x->count = NULL;
    
    int i;
    for( i = 0; i < x->numbufs; i++)
    {
        if(x->buf_proxy[i])
        {
            if(x->buf_proxy[i]->ref)
                object_free(x->buf_proxy[i]->ref);
            
            object_free(x->buf_proxy[i]);
        }
    }
    sysmem_freeptr(x->buf_proxy);
    x->buf_proxy = NULL;

    for( i = 0; i < x->numenvbufs; i++)
    {
        if(x->envbuf_proxy[i])
        {
            if(x->envbuf_proxy[i]->ref)
                object_free(x->envbuf_proxy[i]->ref);
            
            object_free(x->envbuf_proxy[i]);
        }
    }
    sysmem_freeptr(x->envbuf_proxy);
    x->envbuf_proxy = NULL;

    critical_free(x->lock);

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
        
        x->interpolation = 1;
        
        x->always_on = 0;
        x->start = 0;
        x->end = 1;
        x->rate = 1;
        x->dur = 100;
        x->tex = 0.5;
        x->outlet = -1;
        x->buf_index = 0;
        x->amp = 1.0;
        x->chirp_type = 0;
        x->chirp_rate = 0;
        
        x->numbufs = 0;
        x->numenvbufs = 0;

        x->numinlets = 1;
        x->inlet_type[0] = TRIGGER;
        x->inlet_name[0] = ps_granu_trigger;
        
        x->buf_proxy = (t_buffer_proxy **)sysmem_newptr(GRANU_MAX_BUFFERS * sizeof(t_buffer_proxy *));
        x->envbuf_proxy = (t_buffer_proxy **)sysmem_newptr(GRANU_MAX_BUFFERS * sizeof(t_buffer_proxy *));

        critical_new(&x->lock);

        int i;
        for(i=0; i<GRANU_MAX_BUFFERS; i++ )
        {
            x->buf_proxy[i] = NULL;
            x->buf_name[i] = NULL;
            
            x->envbuf_proxy[i] = NULL;
            x->window_name[i] = NULL;
        }
        
        for(i=1; i<GRANUBUF_NUMINLET_TYPES; i++)
        {
            x->inlet_type[i] = UNUSED;
        }
        
        x->numwindows = 1;
        x->window_name[0] = ps_granu_cos;
        x->tab_w_index[0] = COS;
        x->window_index = 0;
        
        x->num_time_param_inlets = 0;
        x->prev_params[0] = UNUSED;
        x->prev_params[1] = UNUSED;
        x->overwrite_param = ENDLOCATION;
        
        x->end_mode = RESIZE;
        
        x->parameter_error = 0;
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d); //calls appropriate class_attr_accessors
            object_free(d);
        }
        
        if (x->parameter_error) return NULL;
        
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
//    class_addmethod(c, (method)granubuf_notify,	"notify",	A_CANT, 0);
    class_addmethod(c, (method)granubuf_float,  "float",    A_FLOAT, 0);
    class_addmethod(c, (method)granubuf_int,    "int",      A_LONG, 0);
    class_addmethod(c, (method)granu_info,      "info",     0);
    
    CLASS_ATTR_SYM_VARSIZE(c, "window", 0, t_grans, window_name, numwindows, GRANU_MAX_BUFFERS );
    CLASS_ATTR_ACCESSORS(c, "window", granubuf_window_get, granubuf_window_set);
    CLASS_ATTR_LABEL(c, "window", 0, "window list");
    
    CLASS_ATTR_SYM_VARSIZE(c, "buffer", 0, t_grans, buf_name, numbufs, GRANU_MAX_BUFFERS);
    CLASS_ATTR_ACCESSORS(c, "buffer", granubuf_buf_get, granubuf_buf_set);
    CLASS_ATTR_LABEL(c, "buffer", 0, "buffer list");

    CLASS_ATTR_LONG(c, "alwayson", 0, t_grans, always_on);
  
    CLASS_ATTR_SYM_VARSIZE(c, "inlets", ATTR_SET_OPAQUE_USER, t_grans, inlet_type, numinlets, GRANUBUF_NUMINLET_TYPES);
    CLASS_ATTR_ACCESSORS(c, "inlets", granubuf_inlet_get, granubuf_inlet_set);
    CLASS_ATTR_LABEL(c, "inlets", 0, "inlet assignments");

    CLASS_ATTR_LONG(c, "numoutlets", ATTR_SET_OPAQUE_USER, t_grans, numoutlets);
    
    CLASS_ATTR_DOUBLE(c, "start", 0, t_grans, start);
    CLASS_ATTR_ALIAS(c, "start", "location");
    
    CLASS_ATTR_DOUBLE(c, "end", 0, t_grans, end);
    CLASS_ATTR_ACCESSORS(c, "end", granubuf_end_get, granubuf_end_set);
    
    CLASS_ATTR_DOUBLE(c, "rate", 0, t_grans, rate);
    CLASS_ATTR_ACCESSORS(c, "rate", granubuf_rate_get, granubuf_rate_set);
    
    CLASS_ATTR_DOUBLE(c, "duration", 0, t_grans, dur);
    CLASS_ATTR_ACCESSORS(c, "duration", granubuf_duration_get, granubuf_duration_set);
    
    CLASS_ATTR_DOUBLE(c, "tex", 0, t_grans, tex);
    CLASS_ATTR_LABEL(c, "tex", 0, "tex (for fof window only)");

    CLASS_ATTR_LONG(c, "window_index", 0, t_grans, window_index);
    CLASS_ATTR_LONG(c, "buffer_index", 0, t_grans, buf_index);
    CLASS_ATTR_LONG(c, "outlet", 0, t_grans, outlet);
    CLASS_ATTR_DOUBLE(c, "amp", 0, t_grans, amp);

    CLASS_ATTR_DOUBLE(c, "chirprate", 0, t_grans, chirp_rate);
    CLASS_ATTR_LONG(c, "chirptype", 0, t_grans, chirp_type);
    
    CLASS_ATTR_LONG(c, "maxoverlap", 0, t_grans, maxoverlap);
    CLASS_ATTR_LABEL(c, "maxoverlap", 0, "max grain overlap");

    CLASS_ATTR_SYM(c, "setbuffer", 0, t_grans, current_buffer);
    CLASS_ATTR_ACCESSORS(c, "setbuffer", granubuf_currentbuf_get, granubuf_currentbuf_set);
    CLASS_ATTR_LABEL(c, "setbuffer", 0, "current buffer");

    CLASS_ATTR_SYM(c, "setwindow", 0, t_grans, current_window);
    CLASS_ATTR_ACCESSORS(c, "setwindow", granubuf_currentwindow_get, granubuf_currentwindow_set);
    CLASS_ATTR_LABEL(c, "setwindow", 0, "current window");
    
    CLASS_ATTR_INT32(c, "end_mode", 0, t_grans, end_mode);
    CLASS_ATTR_INT32(c, "interpolation", 0, t_grans, interpolation);
    
    
    if (!granu_tableExists)
    {
        granu_makeTables();

        //internal symbols
        ps_granu_cos = gensym("cos");
        ps_granu_fof = gensym("fof");
        ps_granu_dampedsine = gensym("dampedsine");
        ps_granu_sinc = gensym("sinc");
        ps_buffer_modified = gensym("buffer_modified");
        
        //inlet types
        ps_granu_trigger = gensym("trigger");
        ps_granu_start = gensym("start");
        ps_granu_end = gensym("end");
        ps_granu_rate = gensym("rate");
        ps_granu_dur = gensym("duration");
        ps_granu_win_idx = gensym("window_index");
        ps_granu_outlet = gensym("outlet");
        ps_granu_buf_idx = gensym("buffer_index");
        ps_granu_amp = gensym("amp");
        ps_granu_tex = gensym("tex");
        ps_granu_chirprate = gensym("chirprate");
        ps_granu_chirptype = gensym("chirptype");

    }

    t_class *bufpxy = class_new("granu_bufferproxy", NULL, NULL, sizeof(t_buffer_proxy), 0L, 0);
    class_addmethod(bufpxy, (method)buffer_proxy_notify, "notify",	A_CANT, 0);
    class_register(CLASS_NOBOX, bufpxy);
    graunu_buffer_proxy_class = bufpxy;
    
    class_dspinit(c);
	class_register(CLASS_BOX, c);
	granubuf_class = c;

	version_post_copyright();
    
    //post("test");
    
    error("n.b. granubuf~ is currently in alpha development -- configuration will be stabilized soon, please be sure to see the help patch for updated information,");
    
	return 0;
}
