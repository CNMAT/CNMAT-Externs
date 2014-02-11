/*
 Written by Rama Gottfried, The Center for New Music and Audio Technologies,
 University of California, Berkeley.  Copyright (c) 2014, The Regents of
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
#define NAME "polywave~"
#define DESCRIPTION "lookup wavetable with ability to switch between array of buffers at samplerate"
#define AUTHORS "Rama Gottfried"
#define COPYRIGHT_YEARS "2014"

#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "buffer.h"
#include "z_dsp.h"
#include "version.h"


// -------- buffer proxy object class, to handle buffer notifications


t_class *buffer_proxy_class = NULL;
t_class *polywave_class = NULL;
t_symbol *ps_buffer_modified = NULL;

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

    if((x = (t_buffer_proxy *)object_alloc(buffer_proxy_class)))
    {
        x->name = s;
        x->ref = buffer_ref_new((t_object *)x, x->name);
        x->buffer_modified = true;
    }

    return x;
}

 
//------------ polywave~

#define POLYWAVE_MAX_BUFFERS 2048

typedef enum _polywave_interp
{
    NONE,
    LINEAR,
    CUBIC
} t_polywave_interp;

typedef struct _polywave
{
	t_pxobject          ob;
    double              sr;
    t_buffer_proxy      **buf_proxy;
    long                numbufs;
    t_symbol            *buf_name[POLYWAVE_MAX_BUFFERS]; //<< for attr inspector
	short               w_connected[2];
    t_polywave_interp   interp_type;

    int                 process_flag;
    t_object            *set_attr;
    long                set_argc;
    t_atom              *set_argv;
    
    t_critical          lock;
    
} t_polywave;

void debug_printAtoms(long argc, t_atom *argv)
{
    int i;
    t_atom *ap = argv;
    for (i=0; i<argc; i++) {
        switch (atom_gettype(ap+i)) {
            case A_SYM:
                post("%s", atom_getsym(ap+i)->s_name);
                break;
            case A_FLOAT:
                post("%f", atom_getfloat(ap+i));
                break;
            case A_LONG:
                post("%ld", atom_getlong(ap+i));
                break;
            default:
                break;
        }
    }
}


t_max_err polywave_buf_set(t_polywave *x, t_object *attr, long argc, t_atom *argv)
{

    int i;
    t_symbol *s;
/*
    if(sys_getdspstate())
    {
        
        if(argc <= x->numbufs)
        {
            for (i = 0; i < argc; i++) {
                if(atom_gettype(argv+i) == A_SYM && atom_getsym(argv+i) != x->buf_name[i])
                    object_error((t_object *)x, "with DSP on samples maybe be replaced but must use the same buffer names");

            }
            x->numbufs = argc;
            return 0;
        }
        
        if(argc > x->numbufs)
        {
            object_post((t_object *)x, "cannot add new buffers with DSP on (samples maybe be replaced if they use the same buffer names)");
            return -1;
        }
    }
   
    if(sys_getdspstate() && x->process_flag == 0)
    {

        t_class *c = class_findbyname( CLASS_NOBOX, object_classname(attr));
        if(c)
        {
            if (x->set_attr)
                object_free(x->set_attr);
                
            x->set_attr = object_alloc(c);

            if(argc < POLYWAVE_MAX_BUFFERS)
                for (i=0; i<argc; i++) {
                    x->set_argv[i] = argv[i];
                }
            
            x->set_argc = argc;
            x->process_flag = 1;
        }
        
        return 0;
    }
*/

    if(argc && argv)
    {
        if(argc < POLYWAVE_MAX_BUFFERS)
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
                
                x->numbufs = argc;
                
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
            critical_exit(x->lock);
        }
    }
    
    return 0;
}

t_max_err polywave_buf_get(t_polywave *x, t_object *attr, long *argc, t_atom **argv)
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

void polywave_print(t_polywave *x)
{
    int i;
    for (i = 0; i < x->numbufs; i++) {
        post("%s nframes %d", x->buf_proxy[i]->name->s_name, x->buf_proxy[i]->nframes);
    }
}

double linear_interp( double v0, double v1, double t)
{
    return v0+((v1-v0)*t);
}

double cubicInterpolate (double a, double b, double c, double d, double x) {
	return b + 0.5 * x*(c - a + x*(2.0*a - 5.0*b + 4.0*c - d + x*(3.0*(b - c) + d - a)));
}

void polywave_perform64(t_polywave *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{

    

    int i;
    t_double		*out = outs[0];
    t_double		*in1 = ins[0];
    t_double		*in2 = ins[1];
    int	n = sampleframes;
/*
    
    if(x->process_flag == 1)
    {
        polywave_buf_set(x, x->set_attr, x->set_argc, x->set_argv);
        x->process_flag = 0;
        
        //        post("%s", __func__);
        //debug_printAtoms(x->set_argc, x->set_argv);
    }
*/
    if(x->numbufs == 0 || !x->w_connected[0])
    {
        while (n--)
            *out++ = 0.;
        return;
    }

    int             idx_connected = x->w_connected[1];
    
    long            numbufs = x->numbufs;
    
    t_buffer_obj    *w_buf;
    t_float         *w_tab;
    long            w_frames, w_chans, w_valid;
    
    /*
    for (i=0; i<numbufs; i++) {
        buffer[i] = buffer_ref_getobject(x->buf_proxy[i]->ref);
        
        if(!buffer[i])
            valid[i] = 0;
        else
        {
            tab[i] = buffer_locksamples(buffer[i]);
            modified[i] = x->buf_proxy[i]->buffer_modified;
            
            if(!tab[i])
                valid[i] = 0;
            else
            {
                //if(modified[i])
                //{
                    frames[i] = buffer_getframecount(buffer[i]);
                    nchans[i] = buffer_getchannelcount(buffer[i]);
                //}
                else
                //{
                //    frames[i] = x->buf_proxy[i]->nframes;
                //    nchans[i] = x->buf_proxy[i]->nchans;
                //}
     
                if(nchans[i] > 0 && frames[i] > 0)
                {
                    valid[i] = 1;
                    if(modified[i])
                    {
                        x->buf_proxy[i]->nframes = frames[i];
                        x->buf_proxy[i]->nchans = nchans[i];
                        x->buf_proxy[i]->buffer_modified = false;
                    }
                    //  post("%s frames %d", __func__, x->buf_frames[i]);
                }
            }
            
        }
        
    }
    */
    t_polywave_interp interp = x->interp_type;
   
    double p, pSamp, upperVal, lowerSamp, upperSamp, frac, a, b, c, d;
    long  bindx = 0, prevBindx = -1;
    
    switch (interp) {
        case CUBIC:
            while(n--)
            {
                p = *in1++;
                p = CLAMP(p, 0, 1);
                
                if(idx_connected)
                {
                    bindx = (long)*in2++;
                    bindx = CLAMP(bindx, 0, numbufs-1);
                    
                    if (bindx != prevBindx) {
                        if(prevBindx > -1 && w_tab)
                            buffer_unlocksamples(w_buf);
                        
                        w_buf = buffer_ref_getobject(x->buf_proxy[bindx]->ref);
                        w_tab = buffer_locksamples(w_buf);
                        
                        if (w_tab)
                        {
                            w_frames = buffer_getframecount(w_buf);
                            w_chans = buffer_getchannelcount(w_buf);
                            w_valid = 1;
                        } else {
                            w_valid = 0;
                        }
                    }
                }
                
                if(w_valid)
                {
                    pSamp = w_frames * p;
                    lowerSamp = floor(pSamp);
                    frac = pSamp - lowerSamp;
                    
                    a = (long)lowerSamp - 1 < 0 ? 0 : w_tab[ w_chans * ((long)lowerSamp - 1)];
                    b = w_tab[ w_chans * (long)lowerSamp];
                    c = (long)lowerSamp + 1 > w_frames ? 0 : w_tab[ w_chans * ((long)lowerSamp + 1)];
                    d = (long)lowerSamp + 2 > w_frames ? 0 : w_tab[ w_chans * ((long)lowerSamp + 2)];


                    *out++ = cubicInterpolate(a,b,c,d,frac);

                }
                else
                    *out++ = 0.0;
                
                prevBindx = bindx;
            }
            buffer_unlocksamples(w_buf);
            break;
        case LINEAR:
            while(n--)
            {
                p = *in1++;
                p = CLAMP(p, 0, 1);
                
                if(idx_connected)
                {
                    bindx = (long)*in2++;
                    bindx = CLAMP(bindx, 0, numbufs-1);
                    
                    if (bindx != prevBindx) {
                        if(prevBindx > -1 && w_tab)
                            buffer_unlocksamples(w_buf);
                        
                        w_buf = buffer_ref_getobject(x->buf_proxy[bindx]->ref);
                        w_tab = buffer_locksamples(w_buf);
                        
                        if (w_tab)
                        {
                            w_frames = buffer_getframecount(w_buf);
                            w_chans = buffer_getchannelcount(w_buf);
                            w_valid = 1;
                        } else {
                            w_valid = 0;
                        }
                    }
                }
                
                if(w_valid)
                {
                    pSamp = w_frames * p;
                    lowerSamp = floor(pSamp);
                    upperSamp = ceil(pSamp);
                    upperVal = (upperSamp < w_frames) ? w_tab[ w_chans * (long)upperSamp ] : 0.0;
                    
                    *out++ = linear_interp(w_tab[ w_chans * (long)lowerSamp  ], upperVal, pSamp - lowerSamp);
                }
                else
                    *out++ = 0.0;
                
                prevBindx = bindx;
            }
            buffer_unlocksamples(w_buf);
            break;
        default:
        case NONE:
            while(n--)
            {
                p = *in1++;
                p = CLAMP(p, 0, 1);
                
                if(idx_connected)
                {
                    bindx = (long)*in2++;
                    bindx = CLAMP(bindx, 0, numbufs-1);
                    
                    if (bindx != prevBindx) {
                        if(prevBindx > -1 && w_tab)
                            buffer_unlocksamples(w_buf);
                        
                        w_buf = buffer_ref_getobject(x->buf_proxy[bindx]->ref);
                        w_tab = buffer_locksamples(w_buf);
                        
                        if (w_tab)
                        {
                            w_frames = buffer_getframecount(w_buf);
                            w_chans = buffer_getchannelcount(w_buf);
                            w_valid = 1;
                        } else {
                            w_valid = 0;
                        }
                    }
                }
                
                if(w_valid)//alid[bindx])
                {
                    *out++ = w_tab[w_chans * (long)(w_frames * p)];
                }
                else
                    *out++ = 0.0;
                
                prevBindx = bindx;
            }
            buffer_unlocksamples(w_buf);
            break;
    }

    return;
    
}


void polywave_dsp64(t_polywave *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->w_connected[0] = count[0];
	x->w_connected[1] = count[1];
    x->sr = samplerate;
	object_method(dsp64, gensym("dsp_add64"), x, polywave_perform64, 0, NULL);
}


void polywave_assist(t_polywave *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) {	// inlets
		switch (a) {
			case 0:	snprintf_zero(s, 256, "(signal) Table Position (0 to 1)");	break;
			case 1:	snprintf_zero(s, 256, "(signal) Buffer index in buffer name array");	break;
		}
	}
	else {	// outlet
		snprintf_zero(s, 256, "(signal) sample output", a+1);
	}
}

void polywave_free(t_polywave *x)
{
    dsp_free((t_pxobject *)x);
    
    int i;
    for (i=0; i<x->numbufs; i++)
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
    
    sysmem_freeptr(x->set_argv);
    x->set_argv = NULL;
    
    if (x->set_attr)
        object_free(x->set_attr);
    
    critical_free(x->lock);

}

void *polywave_new(t_symbol *s, long argc, t_atom *argv)
{
    t_polywave *x = (t_polywave *)object_alloc(polywave_class);
    if(x)
    {
    	dsp_setup((t_pxobject *)x, 2);
        outlet_new((t_object *)x, "signal");
        
        x->buf_proxy = (t_buffer_proxy **)sysmem_newptr(POLYWAVE_MAX_BUFFERS * sizeof(t_buffer_proxy *));
        x->set_argv = (t_atom *)sysmem_newptr(POLYWAVE_MAX_BUFFERS * sizeof(t_atom));
        
        critical_new(&x->lock);
        
        int n = POLYWAVE_MAX_BUFFERS;
        while (--n) {
            x->buf_proxy[n] = NULL;
        }
        
        x->interp_type = NONE;
        x->numbufs = 0;
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d); //calls appropriate class_attr_accessors
            object_free(d);
        }

        x->process_flag = 0;
    }
    return x;
}

int main(void)
{
	t_class *c = class_new("polywave~", (method)polywave_new, (method)polywave_free, (long)sizeof(t_polywave), NULL,   A_GIMME, 0);
	
	class_addmethod(c, (method)polywave_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)polywave_assist,     "assist",	A_CANT, 0);
    
    CLASS_ATTR_SYM_VARSIZE(c, "buffer", 0, t_polywave, buf_name, numbufs, POLYWAVE_MAX_BUFFERS);
    CLASS_ATTR_ACCESSORS(c, "buffer", polywave_buf_get, polywave_buf_set);
    CLASS_ATTR_LABEL(c, "buffer", 0, "buffer list");

    CLASS_ATTR_INT32(c, "interpolation", 0, t_polywave, interp_type);
    CLASS_ATTR_ENUMINDEX3(c, "interpolation", 0, "none", "linear", "cubic");
    
    t_class *bufpxy = class_new("bufferproxy", NULL, NULL, sizeof(t_buffer_proxy), 0L, 0);
    class_addmethod(bufpxy, (method)buffer_proxy_notify, "notify",	A_CANT, 0);
    class_register(CLASS_NOBOX, bufpxy);
    buffer_proxy_class = bufpxy;
    
    class_dspinit(c);
	class_register(CLASS_BOX, c);
	polywave_class = c;
    
    ps_buffer_modified = gensym("buffer_modified");
	version_post_copyright();
    
	return 0;
}

