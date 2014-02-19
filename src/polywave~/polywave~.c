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

typedef enum _polywave_dims
{
    ZERO,
    ONE_D,
    TWO_D
} t_polywave_dims;

typedef struct _polywave
{
	t_pxobject          ob;
    double              sr;
    t_buffer_proxy      **buf_proxy;
    long                numbufs;
    t_symbol            *buf_name[POLYWAVE_MAX_BUFFERS];
	int                 w_connected[4];
    t_polywave_interp   interp_type;
    t_polywave_interp   backup; // << hack to work around weird attr bug?
    long                dims;
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


t_max_err polywave_interp_set(t_polywave *x, t_object *attr, long argc, t_atom *argv)
{
    
    if(argc == 1 && atom_gettype(argv) == A_LONG)
    {
        critical_enter(x->lock);
        x->interp_type = atom_getlong(argv);
        x->backup = x->interp_type; // << HORRIBLE KLUDGE
        //post("%d %d", x->interp_type, x->backup);
        critical_exit(x->lock);
    } else {
        object_error((t_object *)x, "unknown interpolation ");
    }
    
    return 0;
}

t_max_err polywave_interp_get(t_polywave *x, t_object *attr, long *argc, t_atom **argv)
{
    
    char alloc;
    
    atom_alloc(argc, argv, &alloc);
    atom_setlong(*argv, x->interp_type);
    
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

//credits: http://www.paulinternet.nl/?page=bicubic
double bicubicInterp_acc(double p[4][4], double x, double y){
    double a00, a01, a02, a03;
    double a10, a11, a12, a13;
    double a20, a21, a22, a23;
    double a30, a31, a32, a33;
    
    a00 = p[1][1];
    a01 = -.5*p[1][0] + .5*p[1][2];
    a02 = p[1][0] - 2.5*p[1][1] + 2*p[1][2] - .5*p[1][3];
    a03 = -.5*p[1][0] + 1.5*p[1][1] - 1.5*p[1][2] + .5*p[1][3];
    a10 = -.5*p[0][1] + .5*p[2][1];
    a11 = .25*p[0][0] - .25*p[0][2] - .25*p[2][0] + .25*p[2][2];
    a12 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + .5*p[2][0] - 1.25*p[2][1] + p[2][2] - .25*p[2][3];
    a13 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .25*p[2][0] + .75*p[2][1] - .75*p[2][2] + .25*p[2][3];
    a20 = p[0][1] - 2.5*p[1][1] + 2*p[2][1] - .5*p[3][1];
    a21 = -.5*p[0][0] + .5*p[0][2] + 1.25*p[1][0] - 1.25*p[1][2] - p[2][0] + p[2][2] + .25*p[3][0] - .25*p[3][2];
    a22 = p[0][0] - 2.5*p[0][1] + 2*p[0][2] - .5*p[0][3] - 2.5*p[1][0] + 6.25*p[1][1] - 5*p[1][2] + 1.25*p[1][3] + 2*p[2][0] - 5*p[2][1] + 4*p[2][2] - p[2][3] - .5*p[3][0] + 1.25*p[3][1] - p[3][2] + .25*p[3][3];
    a23 = -.5*p[0][0] + 1.5*p[0][1] - 1.5*p[0][2] + .5*p[0][3] + 1.25*p[1][0] - 3.75*p[1][1] + 3.75*p[1][2] - 1.25*p[1][3] - p[2][0] + 3*p[2][1] - 3*p[2][2] + p[2][3] + .25*p[3][0] - .75*p[3][1] + .75*p[3][2] - .25*p[3][3];
    a30 = -.5*p[0][1] + 1.5*p[1][1] - 1.5*p[2][1] + .5*p[3][1];
    a31 = .25*p[0][0] - .25*p[0][2] - .75*p[1][0] + .75*p[1][2] + .75*p[2][0] - .75*p[2][2] - .25*p[3][0] + .25*p[3][2];
    a32 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + 1.5*p[1][0] - 3.75*p[1][1] + 3*p[1][2] - .75*p[1][3] - 1.5*p[2][0] + 3.75*p[2][1] - 3*p[2][2] + .75*p[2][3] + .5*p[3][0] - 1.25*p[3][1] + p[3][2] - .25*p[3][3];
    a33 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .75*p[1][0] + 2.25*p[1][1] - 2.25*p[1][2] + .75*p[1][3] + .75*p[2][0] - 2.25*p[2][1] + 2.25*p[2][2] - .75*p[2][3] - .25*p[3][0] + .75*p[3][1] - .75*p[3][2] + .25*p[3][3];
    
    
    double x2 = x * x;
    double x3 = x2 * x;
    double y2 = y * y;
    double y3 = y2 * y;
    
    return (a00 + a01 * y + a02 * y2 + a03 * y3) +
    (a10 + a11 * y + a12 * y2 + a13 * y3) * x +
    (a20 + a21 * y + a22 * y2 + a23 * y3) * x2 +
    (a30 + a31 * y + a32 * y2 + a33 * y3) * x3;
	
}

void polywave_perform64_two(t_polywave *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    
    int i;
    t_double		*out = outs[0];
    t_double		*x1_in = ins[0];
    t_double		*x2_in = ins[1];
    t_double        *interp_in = ins[2];
    t_double		*idx1_in = ins[3];
    t_double		*idx2_in = ins[4];
    int	n = sampleframes;
    
    if(x->numbufs == 0 || !x->w_connected[0])
    {
        while (n--)
            *out++ = 0.;
        return;
    }
    
    int             *connected = x->w_connected;

    long            numbufs = x->numbufs;
    long            frames[numbufs], nchans[numbufs];
    
    t_buffer_obj    *buffer[numbufs];
    t_float         *tab[numbufs];
    int             valid[numbufs], modified[numbufs];
    
    t_polywave_interp interp_t = x->interp_type;
   // post("%d %d", x->interp_type, x->backup);
    
    for (i=0; i<numbufs; i++) {
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
    
    double x1_p, x2_p, interp_p = 0, pSamp1, pSamp2, upperVal, lowerSamp, upperSamp, frac, a1, a2, b, c, d;
    long  idx1 = 0, idx2 = 0;
    
    switch (interp_t) {
        case CUBIC:
            while(n--)
            {
                x1_p = *x1_in++;
                x1_p = CLAMP(x1_p, 0, 1);
                
                if(connected[1])
                {
                    x2_p = *x2_in++;
                    x2_p = CLAMP(x2_p, 0, 1);
                } else {
                    x2_p = x1_p;
                }
                
                if (connected[2]) {
                    interp_p = *interp_in++;
                    interp_p = CLAMP(interp_p, 0, 1);
                }
                
                if(connected[3])
                {
                    idx1 = (long)*idx1_in++;
                    idx1 = CLAMP(idx1, 0, numbufs-1);
                }
                
                if(connected[4])
                {
                    idx2 = (long)*idx2_in++;
                    idx2 = CLAMP(idx2, 0, numbufs-1);
                }
                
                if(valid[idx1] && valid[idx2])
                {
                    pSamp1 = frames[idx1] * x1_p;
                    lowerSamp = floor(pSamp1);
                    frac = pSamp1 - lowerSamp;
                    
                    a1 = (long)lowerSamp - 1 < 0 ? 0 : tab[idx1][ nchans[idx1] * ((long)lowerSamp - 1)];
                    b = tab[idx1][ nchans[idx1] * (long)lowerSamp];
                    c = (long)lowerSamp + 1 > frames[idx1] ? 0 : tab[idx1][ nchans[idx1] * ((long)lowerSamp + 1)];
                    d = (long)lowerSamp + 2 > frames[idx1] ? 0 : tab[idx1][ nchans[idx1] * ((long)lowerSamp + 2)];
                    
                    pSamp1 = cubicInterpolate(a1,b,c,d,frac);
                    
                    pSamp2 = frames[idx2] * x2_p;
                    lowerSamp = floor(pSamp2);
                    frac = pSamp2 - lowerSamp;
                    
                    a2 = (long)lowerSamp - 1 < 0 ? 0 : tab[idx2][ nchans[idx2] * ((long)lowerSamp - 1)];
                    b = tab[idx2][ nchans[idx2] * (long)lowerSamp];
                    c = (long)lowerSamp + 1 > frames[idx2] ? 0 : tab[idx2][ nchans[idx2] * ((long)lowerSamp + 1)];
                    d = (long)lowerSamp + 2 > frames[idx2] ? 0 : tab[idx2][ nchans[idx2] * ((long)lowerSamp + 2)];
                    
                    pSamp2 = cubicInterpolate(a2,b,c,d,frac);
                    
                    *out++ = cubicInterpolate(a1,pSamp1,pSamp2,d,interp_p);
                }
                else
                    *out++ = 0.5;
            }
            break;
        case LINEAR:
            while(n--)
            {
                x1_p = *x1_in++;
                x1_p = CLAMP(x1_p, 0, 1);
                
                if(connected[1])
                {
                    x2_p = *x2_in++;
                    x2_p = CLAMP(x2_p, 0, 1);
                } else {
                    x2_p = x1_p;
                }
                
                if (connected[2]) {
                    interp_p = *interp_in++;
                    interp_p = CLAMP(interp_p, 0, 1);
                }
                
                if(connected[3])
                {
                    idx1 = (long)*idx1_in++;
                    idx1 = CLAMP(idx1, 0, numbufs-1);
                }
                
                if(connected[4])
                {
                    idx2 = (long)*idx2_in++;
                    idx2 = CLAMP(idx2, 0, numbufs-1);
                }

                
                if(valid[idx1] && valid[idx2])
                {
                    pSamp1 = frames[idx1] * x1_p;
                    lowerSamp = floor(pSamp1);
                    upperSamp = ceil(pSamp1);
                    upperVal = (upperSamp < frames[idx1]) ? tab[idx1][ nchans[idx1] * (long)upperSamp ] : 0.0;
                    
                    pSamp1 = linear_interp(tab[idx1][ nchans[idx1] * (long)lowerSamp  ], upperVal, pSamp1 - lowerSamp);

                    pSamp2 = frames[idx2] * x2_p;
                    lowerSamp = floor(pSamp2);
                    upperSamp = ceil(pSamp2);
                    upperVal = (upperSamp < frames[idx2]) ? tab[idx2][ nchans[idx2] * (long)upperSamp ] : 0.0;
                    
                    pSamp2 = linear_interp(tab[idx2][ nchans[idx2] * (long)lowerSamp  ], upperVal, pSamp2 - lowerSamp);

                    *out++ = linear_interp(pSamp1, pSamp2, interp_p);
                }
                else
                    *out++ = 0.0;
                
            }
            break;
        default:
        case NONE:
            while(n--)
            {
                x1_p = *x1_in++;
                x1_p = CLAMP(x1_p, 0, 1);
                
                if(connected[2])
                {
                    idx1 = (long)*idx1_in++;
                    idx1 = CLAMP(idx1, 0, numbufs-1);
                }
                
                if(valid[idx1])
                {
                    *out++ = tab[idx1][nchans[idx1] * (long)(frames[idx1] * x1_p)];
                }
                else
                    *out++ = 0.0;
                
            }
            break;
    }
    
    for(i=0; i<numbufs; i++)
    {
        if(valid[i])
            buffer_unlocksamples(buffer[i]);
    }
    
    return;
    
}

void polywave_perform64(t_polywave *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{

    int i;
    t_double		*out = outs[0];
    t_double		*in1 = ins[0];
    t_double		*in2 = ins[1];
    int	n = sampleframes;

    if(x->numbufs == 0 || !x->w_connected[0])
    {
        while (n--)
            *out++ = 0.;
        return;
    }

    int             idx_connected = x->w_connected[1];
    long            numbufs = x->numbufs;
    long            frames[numbufs], nchans[numbufs];

    t_buffer_obj    *buffer[numbufs];
    t_float         *tab[numbufs];
    int             valid[numbufs], modified[numbufs];
    
    

    for (i=0; i<numbufs; i++) {
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

    t_polywave_interp interp = x->interp_type;
   
    double p, pSamp, upperVal, lowerSamp, upperSamp, frac, a, b, c, d;
    long  bindx = 0;
    
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
                }
                
                
                if(valid[bindx])
                {
                    pSamp = frames[bindx] * p;
                    lowerSamp = floor(pSamp);
                    frac = pSamp - lowerSamp;
                    
                    a = (long)lowerSamp - 1 < 0 ? 0 : tab[bindx][ nchans[bindx] * ((long)lowerSamp - 1)];
                    b = tab[bindx][ nchans[bindx] * (long)lowerSamp];
                    c = (long)lowerSamp + 1 > frames[bindx] ? 0 : tab[bindx][ nchans[bindx] * ((long)lowerSamp + 1)];
                    d = (long)lowerSamp + 2 > frames[bindx] ? 0 : tab[bindx][ nchans[bindx] * ((long)lowerSamp + 2)];
                    
                    
                    *out++ = cubicInterpolate(a,b,c,d,frac);
                    
                }
                else
                    *out++ = 0.0;
            }
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
                }
                
                if(valid[bindx])
                {
                    pSamp = frames[bindx] * p;
                    lowerSamp = floor(pSamp);
                    upperSamp = ceil(pSamp);
                    upperVal = (upperSamp < frames[bindx]) ? tab[bindx][ nchans[bindx] * (long)upperSamp ] : 0.0;
                    
                    *out++ = linear_interp(tab[bindx][ nchans[bindx] * (long)lowerSamp  ], upperVal, pSamp - lowerSamp);
                }
                else
                    *out++ = 0.0;
                
            }
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
                }
                
                if(valid[bindx])
                {
                    *out++ = tab[bindx][nchans[bindx] * (long)(frames[bindx] * p)];
                }
                else
                    *out++ = 0.0;
                
            }
            break;
    }
    
    for(i=0; i<numbufs; i++)
    {
        if(valid[i])
            buffer_unlocksamples(buffer[i]);
    }

    return;
    
}


void polywave_dsp64(t_polywave *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    int i;
    int numinlets = x->dims == ONE_D ? 2 : (x->dims == TWO_D ? 5 : 0);
    for(i=0; i<numinlets; i++)
    {
        x->w_connected[i] = count[i];
    }
    x->sr = samplerate;
    
    x->interp_type = x->backup; // WTF!!  I have no idea why this is necessary, for some reason max bollocks up the interp value

    if(x->dims == ONE_D)
        object_method(dsp64, gensym("dsp_add64"), x, polywave_perform64, 0, NULL);
    else if(x->dims == TWO_D)
        object_method(dsp64, gensym("dsp_add64"), x, polywave_perform64_two, 0, NULL);
    
}


void polywave_assist(t_polywave *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) {	// inlets
        if(x->dims == ONE_D)
            switch (a) {
                case 0:	snprintf_zero(s, 256, "(signal) Table Position (0 to 1)");	break;
                case 1:	snprintf_zero(s, 256, "(signal) Buffer index in buffer name array");	break;
            }
        else if(x->dims == TWO_D)
            switch (a) {
                case 0:	snprintf_zero(s, 256, "(signal) Table 1 Position (0 to 1)");	break;
                case 1:	snprintf_zero(s, 256, "(signal) Table 2 Position (0 to 1), same as table 1 if not attached");	break;
                case 2:	snprintf_zero(s, 256, "(signal) Interpolation weight (0 to 1)");	break;
                case 3:	snprintf_zero(s, 256, "(signal) Buffer index 1");	break;
                case 4:	snprintf_zero(s, 256, "(signal) Buffer index 2");	break;

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
    
    critical_free(x->lock);

}

void *polywave_new(t_symbol *s, long argc, t_atom *argv)
{
    t_polywave *x = (t_polywave *)object_alloc(polywave_class);
    if(x)
    {
    	   
        x->buf_proxy = (t_buffer_proxy **)sysmem_newptr(POLYWAVE_MAX_BUFFERS * sizeof(t_buffer_proxy *));
        
        critical_new(&x->lock);
        
        int n = POLYWAVE_MAX_BUFFERS;
        while (--n) {
            x->buf_proxy[n] = NULL;
        }
        
        x->dims = ONE_D;
        x->interp_type = LINEAR;
        x->backup = x->interp_type;
        x->numbufs = 0;
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d); //calls appropriate class_attr_accessors
            object_free(d);
        }
        
        int numlets = 0;
        switch (x->dims) {
            case ONE_D: numlets = 2; break;
            case TWO_D: numlets = 5; break;
            default:
                object_error((t_object *)x, "dimention attribute set to unknown value, cannont create inlets");
                break;
        }
        
        dsp_setup((t_pxobject *)x, numlets);
        outlet_new((t_object *)x, "signal");
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
    CLASS_ATTR_ACCESSORS(c, "interpolation", polywave_interp_get, polywave_interp_set);
    CLASS_ATTR_ENUMINDEX3(c, "interpolation", 0, "none", "linear", "cubic");
    
    CLASS_ATTR_INT32(c, "dimensions", ATTR_SET_OPAQUE_USER, t_polywave, dims);
//    CLASS_ATTR_ENUMINDEX3(c, "dimensions", ATTR_SET_OPAQUE_USER, "0D", "1D", "2D");
    
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

