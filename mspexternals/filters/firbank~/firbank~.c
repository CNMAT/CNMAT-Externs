/*
 Copyright (c) 2008.  The Regents of the University of California (Regents).
 All Rights Reserved.
 
 Permission to use, copy, modify, distribute, and distribute modified versions
 of this software and its documentation without fee and without a signed
 licensing agreement, is hereby granted, provided that the above copyright
 notice, this paragraph and the following two paragraphs appear in all copies,
 modifications, and distributions.

 Written by Andy Schmeder <andy@cnmat.berkeley.edu>, The Center for New Music and Audio Technologies,
 University of California, Berkeley.
 
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
 MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: firbank~
 DESCRIPTION: Implements optimized real FFT convolution of one ore more inputs and a bank of FIR filters using the overlap-save method.
 AUTHORS: Andy Schmeder
 COPYRIGHT_YEARS: 2008
 VERSION 0.1: Initial release
 VERSION 0.2: Fixed operation with multiple inputs
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

// stdlib
#include <math.h>

// maxmsp
#include "ext.h"
#include "z_dsp.h"
#include "buffer.h"

// cnmat versioning
#include "version.h"
#include "version.c"

// fftw 3
#include "fftw3.h"

void *firbank_class;

typedef struct _fir {
    
    // buffer
    t_buffer* buffer;
    
    // buffer offset
    int offset;
    
    // channel maps
    int channel_1;
    int channel_2;
    
} t_fir;

typedef struct _fir_state {

    // filter
    int k;
    
    // input, output map
    int i;
    int o;
    
    // frame
    float* tail;

} t_fir_state;

typedef struct _firbank {
    
    // header
	t_pxobject x_obj;
    
    // number of signals in
    int n;
    
    // number of signals out
    int m;
    
    // number of filters
    int k;
    
    // fir framesize
    int framesize;
    
    // vector size
    int v;

    // dsp state, buffers
    t_int** w;
    float** input;
    float** output;

    float* input_copy;

    /*
    // autosplit mode on/off
    int autosplit;
    
    // autosplit template
    float* template;
    
    // autosplit bands derived from template
    float** bands;
    
    // autosplit rc timeconstant, [0.-1.)
    float rc;
    
    // autosplit band-overlap factor (0.-1.)
    float overlap;
    
    // autosplit power for overlap (transformed variable)
    float overlap_power;
    */
    
    // filters
    t_fir* filters;
    
    // filters
    t_fir_state* filter_states;
    
    // fftw plans for forward and backward transform
    fftwf_plan x_forward;
    fftwf_plan x_inverse;

    float* x_forward_t;
    fftwf_complex* x_forward_c;
    
    float* x_inverse_t;
    fftwf_complex* x_inverse_c;
    
} t_firbank;

// symbols
t_symbol *ps_buffer_tilde;

// function prototypes
int main(void);
t_buffer* _sym_to_buffer(t_symbol*);
void *firbank_new(t_symbol*, short, t_atom*);
void firbank_dsp(t_firbank *x, t_signal **sp, short int *count);
t_int *firbank_perform(t_int *w);
void firbank_free(t_firbank *x);

// main
int main(void) {
    
    version(0);
    
    setup((t_messlist **)&firbank_class, (method)firbank_new, (method)firbank_free,
          (short)sizeof(t_firbank), 0L, A_GIMME, 0);
    
    addmess((method)firbank_dsp, "dsp", A_CANT, 0);
    
    dsp_initclass();
    
    ps_buffer_tilde = gensym("buffer~");
    
    return 0;
    
}

// msp arcana to extract t_buffer* from a t_symbol*
t_buffer* _sym_to_buffer(t_symbol* s) {

    t_buffer *b;
    if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer_tilde) {
		return b;
    } else {
        return 0;
    }

}

// new
void *firbank_new(t_symbol *s, short argc, t_atom *argv) {

    // scope vars
    t_firbank *x;	
    t_buffer* default_buffer;
    int default_filters;
    int default_iomap;
    int default_channel_1;
    int default_channel_2;
    
    int i, j, c;
    
    // instantiate 
    x = newobject(firbank_class);
    
    // setup defaults
    /*
    x->autosplit = 0;
    x->overlap = 0.5;
    x->overlap_power = 1.0;
    x->rc = 0.9;
    x->template = NULL;
    x->bands = NULL;
    x->iomap = NULL;
    */
    
    x->framesize = 512;
    x->n = 1;
    x->m = 16;
    x->k = 16;
    x->v = 0;
    
    x->filters = NULL;
    x->filter_states = NULL;
    x->input_copy = NULL;
    x->input = NULL;
    x->output = NULL;
    x->w = NULL;
    
    // initialization parsing state
    default_buffer = NULL;
    
    default_iomap = 1;
    default_filters = 1;
    
    default_channel_1 = 0;
    default_channel_2 = 1;
    
    // read arguments
    for(i = 0; i < argc; i++) {
        
        if(argv[i].a_type == A_SYM) {
            
            // look for @n
            if(strcmp(argv[i].a_w.w_sym->s_name, "@n") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    x->n = argv[i].a_w.w_long;
                } else {
                    post("firbank~: expected int for @n");
                }
            }
            
            // look for @k
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@k") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    x->k = argv[i].a_w.w_long;
                } else {
                    post("firbank~: expected int for @k");
                }
            }
            
            // look for @m
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@m") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    x->m = argv[i].a_w.w_long;
                } else {
                    post("firbank~: expected int for @m");
                }
            }
            
            // look for @framesize
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@framesize") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    x->framesize = argv[i].a_w.w_long;
                } else {
                    post("firbank~: expected int for @framesize");
                }
            }
            
            // look for @buffer
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@buffer") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_SYM) {
                    default_buffer = _sym_to_buffer(argv[i].a_w.w_sym);
                    //x->autosplit = 0;
                } else {
                    post("firbank~: expected symbol for @buffer");
                }
            }
            
            // look for @channel
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@channel") == 0) {
                default_channel_1 = -1;
                default_channel_2 = -1;
                
                i++;
                
                if(i < argc && argv[i].a_type == A_LONG) {
                    default_channel_1 = argv[i].a_w.w_long;
                        
                    if(i + 1 < argc && argv[i+1].a_type == A_LONG) {
                        i++;
                        default_channel_2 = argv[i].a_w.w_long;
                    } else {
                        default_channel_2 = -1;
                    }
                } else {
                    post("firbank~: expected int for @channel");
                }
            }
            
            // look for @filter
            /*
            if(strcmp(argv[i].a_w.w_sym->s_name, "@filter") == 0) {
                // @todo
            }
            */
            
            // look for @mode
            /*
            if(strcmp(argv[i].a_w.w_sym->s_name, "@mode") == 0) {
                i++;
                if(strcmp(argv[i].a_w.w_sym->s_name, "complex") == 0) {
                    default_mode = FIRBANK_MODE_COMPLEX;
                } else if(strcmp(argv[i].a_w.w_sym->s_name, "polar") == 0) {
                    default_mode = FIRBANK_MODE_POLAR;
                } else if(strcmp(argv[i].a_w.w_sym->s_name, "timedomain") == 0) {
                    default_mode = FIRBANK_MODE_POLAR;
                } else {
                    post("firbank~: expected symbol, 'complex', 'polar', or 'timedomain' for @mode");
                }
            }
            */
            
            /*
            // look for @offset
            if(strcmp(argv[i].a_w.w_sym->s_name, "@offset") == 0) {
                // todo
            }
            
            // look for @iomap
            if(strcmp(argv[i].a_w.w_sym->s_name, "@iomap") == 0) {
                
            }
            
            // look for @autosplit
            if(strcmp(argv[i].a_w.w_sym->s_name, "@autosplit") == 0) {
                
            }
            
            // look for @overlap
            if(strcmp(argv[i].a_w.w_sym->s_name, "@overlap") == 0) {
                
            }
            
            // look for @rc
            if(strcmp(argv[i].a_w.w_sym->s_name, "@rc") == 0) {
                
            }
            */
            
        }
        
    }
    
    if(x->m) {
        // post("firbank~: allocating %d filter states, tail %d samples", x->m, x->framesize / 2);
        x->filter_states = (t_fir_state*)malloc(sizeof(t_fir_state)*x->m);
        for(i = 0; i < x->m; i++) {
            x->filter_states[i].tail = (float*)fftwf_malloc(sizeof(float) * x->framesize / 2);
            memset(x->filter_states[i].tail, 0, sizeof(float) * x->framesize / 2);
        }
    }
    
    /*
    if(x->autosplit) {
        // configure...
        post("firbank~: autosplit mode not supported yet");
    }
    */
    
    // setup filters if not from initialization
    if(default_filters && default_buffer != NULL) {
        // post("firbank~: setting up %d filters with framesize %d, channels (%d, %d)", x->k, x->framesize, default_channel_1, default_channel_2);
        
        x->filters = (t_fir*)malloc(sizeof(t_fir)*x->k);
        
        for(i = 0; i < x->k; i++) {
            x->filters[i].buffer = default_buffer;
            x->filters[i].channel_1 = default_channel_1;
            x->filters[i].channel_2 = default_channel_2;
            x->filters[i].offset = i * x->framesize;
        }
    } else {
        post("firbank~: no buffer specified");
    }
    
    // setup iomap if not from initialization
    if(default_iomap) {
        // post("firbank~: initializing input-output map...");
        for(i = 0; i < x->n; i++) {
            for(j = 0; j < x->k; j++) {
                c = (j*(x->m/x->k))+i;
                if(c < x->m) {
                    x->filter_states[c].o = c;      // output channel
                    x->filter_states[c].k = j;      // filter
                    x->filter_states[c].i = i;      // input channel
                    // post("firbank~: input: %d, filter %d, output %d", x->filter_states[c].i, x->filter_states[c].k, x->filter_states[c].o);
                }
            }
        }
    }
    
    // setup fftw
    x->x_forward_t = (float*)fftwf_malloc(sizeof(float) * (x->framesize + 2)); // two extra so fftw can work its in-place magic
    x->x_forward_c = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (x->framesize / 2 + 1));  // n/2+1 complex numbers (dc is first, nyquist last)
    
    x->x_inverse_t = (float*)fftwf_malloc(sizeof(float) * (x->framesize + 2));
    x->x_inverse_c = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * (x->framesize / 2 + 1));  // n/2+1 complex numbers (dc is first, nyquist last)

    x->input_copy = (float*)fftwf_malloc(sizeof(float) * (x->framesize / 2) * x->n);
    
    memset(x->x_forward_t, 0, sizeof(float) * (x->framesize));
    memset(x->x_inverse_t, 0, sizeof(float) * (x->framesize));

    // post("firbank~: preparing fftw plan...");
    
    x->x_forward = fftwf_plan_dft_r2c_1d(x->framesize,
                                         x->x_forward_t, x->x_forward_c,
                                         FFTW_MEASURE
                                         );

    x->x_inverse = fftwf_plan_dft_c2r_1d(x->framesize,
                                         x->x_inverse_c, x->x_inverse_t,
                                         FFTW_MEASURE
                                         );
    
    // post("firbank~: done.");

    x->w = (t_int**)malloc(sizeof(t_int*) * (x->n + x->m + 2));
    x->input = (float**)malloc(sizeof(float*) * x->n);
    x->output = (float**)malloc(sizeof(float*) * x->m);
    
    // don't share inlet and outlet buffers
    // this doesn't work!
    // x->x_obj.z_misc = Z_NO_INPLACE;
    
    // allocate inlets
    dsp_setup((t_pxobject *)x, x->n);
    
    // allocate outlets
    for(i = 0; i < x->m; i++) {
        outlet_new((t_object *)x, "signal");	// type of outlet: "signal"
    }
    
    return (x);
    
}

// free
void firbank_free(t_firbank *x) {
	
    int i;
    
	dsp_free(&(x->x_obj));
    
    if(x->input != NULL) {
        free(x->input);
    }
	
    if(x->output != NULL) {
        free(x->output);
    }
    
    if(x->framesize) {
        fftwf_free(x->x_forward_t);
        fftwf_free(x->x_forward_c);
        
        fftwf_free(x->x_inverse_t);
        fftwf_free(x->x_inverse_c);
        
        fftwf_destroy_plan(x->x_forward);
        fftwf_destroy_plan(x->x_inverse);
        
        fftwf_free(x->input_copy);
    }
    
    if(x->filter_states != NULL) {
        for(i = 0; i < x->m; i++) {
            fftwf_free(x->filter_states[i].tail);
        }
        free(x->filter_states);
    }
    
    if(x->filters != NULL) {
        free(x->filters);
    }
    
    if(x->w != NULL) {
        free(x->w);
    }
    
    // post("firbank~: object destroyed.");
    
}

// dsp setup
void firbank_dsp(t_firbank *x, t_signal **sp, short *connect) {

    int i;

    x->v = sp[0]->s_n;

    if(x->v != x->framesize / 2) {
        post("firbank~: vector size (%d) is not equal to framesize / 2 (%d)", x->v, x->framesize / 2);
    }

    // setup args
    x->w[0] = (t_int*)x;
    x->w[1] = (t_int*)(sp[0]->s_n);
    
    for(i = 0; i < (x->n + x->m); i++) {
        x->w[i+2] = (t_int*)(sp[i]->s_vec);
    }
    
	dsp_addv(firbank_perform, 2 + x->n + x->m, (void**)(x->w)); // not sure if this is right for number of inlets, outlets
    
}

t_int* firbank_perform(t_int *w) {

    t_int** wp;
    int v;
    int i, p, s, k;
    float c, d;
	t_firbank *x;

    wp = (t_int**)w;
    
    x = (t_firbank*)(wp[1]);
    v = (int)(wp[2]);
    
    if(v != x->v) {
        post("firbank~: unexpected block size, v != x->v, %d", v);
        return w + 3 + x->n + x->m;
    }

    for(i = 0; i < x->n; i++) {
        x->input[i] = (float*)(wp[3 + i]);
    }
    
    for(i = 0; i < x->m; i++) {
        x->output[i] = (float*)(wp[3 + i + x->n]);
    }
        
    if(x->v == x->framesize / 2) {

        // copy input
        for(i = 0; i < x->n; i++) {
            memcpy(x->input_copy + (i * x->v), x->input[i], x->v * sizeof(float));
        }
        
        // do overlap-save
        
        // for each input...
        for(i = 0; i < x->n; i++) {
            
            // copy input into first half of x_forward_t
            for(s = 0; s < x->v; s++) {
                x->x_forward_t[s] = x->input_copy[i * x->v + s];
                x->x_forward_t[s + x->v] = 0.f;
            }
            
            // transform -> x_forward_c
            fftwf_execute(x->x_forward);
            
            // for each filter state using this input...
            for(p = 0; p < x->m; p++) {
                
                if(x->filter_states[p].i == i) {
                    
                    // find the corresponding filter index
                    k = x->filter_states[p].k;
                    
                    // verify that its buffer valid and meets the channel and length requirements...
                    if((x->filters[k].buffer->b_valid) && 
                       (x->filters[k].channel_1 >= 0 && x->filters[k].channel_1 < x->filters[k].buffer->b_nchans) && 
                       ((x->filters[k].channel_2 >= 0 && x->filters[k].channel_2 < x->filters[k].buffer->b_nchans) || (x->filters[k].channel_2 < 0)) &&
                       (x->filters[k].offset + x->framesize < x->filters[k].buffer->b_frames)) {

                        // real convolution only (magnitude) 
                        if(x->filters[k].channel_2 < 0) {
                            
                            for(s = 0; s < x->framesize / 2 + 1; s++) { // note symmetry; second half of filter is ignored
                                c = x->filters[k].buffer->b_samples[(x->filters[k].offset + s) * x->filters[k].buffer->b_nchans + x->filters[k].channel_1];
                                
                                // (a + b I) * c = ac + bc I
                                x->x_inverse_c[s][0] = x->x_forward_c[s][0] * c; // ac
                                x->x_inverse_c[s][1] = x->x_forward_c[s][1] * c; // bc
                            }
                            
                        } 
                        
                        // complex convolution
                        else {
                            
                            for(s = 0; s < (x->framesize / 2 + 1); s++) { // note symmetry
                                c = x->filters[k].buffer->b_samples[(x->filters[k].offset + s) * x->filters[k].buffer->b_nchans + x->filters[k].channel_1];
                                d = x->filters[k].buffer->b_samples[(x->filters[k].offset + s) * x->filters[k].buffer->b_nchans + x->filters[k].channel_2];
                                
                                // (a + b I) * (c + d I) = (ac - bd) + (ad + bc) I
                                x->x_inverse_c[s][0] = x->x_forward_c[s][0] * c - x->x_forward_c[s][1] * d;  // ac - bd
                                x->x_inverse_c[s][1] = x->x_forward_c[s][0] * d +  x->x_forward_c[s][1] * c; // ad + bc
                            }
                            
                        }
                        
                        // invert result
                        fftwf_execute(x->x_inverse);
                        
                        // normalize 
                        for(s = 0; s < x->framesize; s++) {
                            x->x_inverse_t[s] /= x->framesize;
                        }
                        
                        // copy first half of result into output + tail of filter state
                        for(s = 0; s < x->v; s++) {
                            x->output[x->filter_states[p].o][s] = x->x_inverse_t[s] + x->filter_states[p].tail[s];
                        }
                        
                        // copy second half of result into tail
                        for(s = 0; s < x->v; s++) {
                            x->filter_states[p].tail[s] = x->x_inverse_t[s + x->v];
                        }
                                                
                   } else {
                       post("firbank~: buffer for filter %d does not meet specifications", k);
                   }
                        
                } // filter state corresponds to this input
                
            } // for each filter state
            
        } // for each input

    } else {

        // outputs all zeros
        for(i = 0; i < x->m; i++) {
            for(s = 0; s < x->v; s++) {
                x->output[i][s] = 0.f;
            }
        }
        
    }
    
    return w + 3 + x->n + x->m;
    
}

