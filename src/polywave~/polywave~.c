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

#define POLYWAVE_MAX_BUFFERS 128

typedef struct _polywave
{
	t_pxobject		ob;
    double          sr;
    t_buffer_proxy  **buf_proxy;
    long            numbufs;
    t_symbol        *buf_name[POLYWAVE_MAX_BUFFERS]; //<< for attr inspector
	short           w_connected[2];
    
} t_polywave;

t_max_err polywave_buf_set(t_polywave *x, t_object *attr, long argc, t_atom *argv)
{

    int i;

    if(argc && argv)
    {
        // probably should wait and do this between vectors?
        if(x->numbufs)
        {
            for (i = 0; i < x->numbufs; i++) {
                object_free(x->buf_proxy[i]->ref);
                object_free(x->buf_proxy[i]);
            }
        }
        
        if(argc < POLYWAVE_MAX_BUFFERS && atom_gettype(argv) == A_SYM)
        {
            x->numbufs = argc;

            for(i = 0; i < x->numbufs; i++)
            {
/*
                 if(atom_gettype(argv+i) == A_SYM)
                     post("%x buf %s", x, atom_getsym(argv+i)->s_name);
*/
                
                x->buf_name[i] = atom_getsym(argv+i);
                if(x->buf_name[i])
                {
                    x->buf_proxy[i] = buffer_proxy_new(x->buf_name[i]);

                } else {
                    object_error((t_object *)x, "must have buffer name");
                }
            }
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
        tab[i] = buffer_locksamples(buffer[i]);
        modified[i] = x->buf_proxy[i]->buffer_modified;
        
        if (buffer[i] && tab[i])
        {
            if(modified[i])
            {
                frames[i] = buffer_getframecount(buffer[i]);
                nchans[i] = buffer_getchannelcount(buffer[i]);
            }
            else
            {
                frames[i] = x->buf_proxy[i]->nframes;
                nchans[i] = x->buf_proxy[i]->nchans;
            }
            
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
            } else {
                post("debug: no frames || no channels, probably do something here");
                valid[i] = 0;
                
            }
            
        } else {
            //  post("debug: no buffer || no tab, do something here");
            valid[i] = 0;
        }
        
    }
   
    t_double p;
    long bindx = 0;
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
            *out++ = tab[bindx][nchans[bindx] * (long)(frames[bindx] * p)];
        else
            *out++ = 0.0;
        
    }
    
	
    for(i=0; i<numbufs; i++)
        buffer_unlocksamples(buffer[i]);
	
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
		snprintf_zero(s, 256, "(signal) sample outbut", a+1);
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
    
   free(x->buf_proxy);
}

void *polywave_new(t_symbol *s, long argc, t_atom *argv)
{
    t_polywave *x = (t_polywave *)object_alloc(polywave_class);
    if(x)
    {
    	dsp_setup((t_pxobject *)x, 2);
        outlet_new((t_object *)x, "signal");
        
        x->buf_proxy = (t_buffer_proxy **)malloc(POLYWAVE_MAX_BUFFERS * sizeof(t_buffer_proxy *));
        int n = POLYWAVE_MAX_BUFFERS;
        while (--n) {
            x->buf_proxy[n] = NULL;
        }
        
        t_dictionary *d = NULL;
        d = dictionary_new();
        
        if (d) {
            attr_args_dictionary(d, argc, argv);
            attr_dictionary_process(x, d); //calls appropriate class_attr_accessors
            object_free(d);
        }
        
    }
    return x;
}

int main(void)
{
	t_class *c = class_new("polywave~", (method)polywave_new, (method)polywave_free, (long)sizeof(t_polywave), NULL,   A_GIMME, 0);
	
	class_addmethod(c, (method)polywave_dsp64,		"dsp64",	A_CANT, 0);
    class_addmethod(c, (method)polywave_assist,     "assist",	A_CANT, 0);
//    class_addmethod(c, (method)polywave_print,      "print",    0);
    
    CLASS_ATTR_SYM_VARSIZE(c, "buffer", 0, t_polywave, buf_name, numbufs, POLYWAVE_MAX_BUFFERS);
    CLASS_ATTR_ACCESSORS(c, "buffer", polywave_buf_get, polywave_buf_set);
    CLASS_ATTR_LABEL(c, "buffer", 0, "buffer list");

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

