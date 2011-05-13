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
 NAME: partconv~
 DESCRIPTION: Non-uniform partitioned FFT convolution, multiple in/out and parallel multithreaded
 AUTHORS: Andy Schmeder, Eric Battenberg
 COPYRIGHT_YEARS: 2011
 VERSION 0.1: Initial port of partconv code
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

// stdlib
#include <math.h>

// maxmsp
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_critical.h"
#include "ext_atomic.h"
#include "z_dsp.h"
#include "buffer.h"

// cnmat versioning
#include "version.h"
#include "version.c"

// fftw 3
#include "fftw3.h"

#include "partconvMulti.h"

typedef struct _partconv {
    
    // header
	t_pxobject x_obj;
    
    // buffer
    t_symbol* buffer;
    
    // number of signals in
    int n;
    
    // number of signals out
    int m;
    
    // number of filters
    int k;
    
    // length of filters in bank (must all be same length)
    int v;

    int scheme_32[256];
    int nparts_32;
    int scheme_64[256];
    int nparts_64;
    int scheme_128[256];
    int nparts_128;
    int scheme_256[256];
    int nparts_256;
    int scheme_512[256];
    int nparts_512;
    int scheme_1024[256];
    int nparts_1024;
    int scheme_2048[256];
    int nparts_2048;
    int scheme_4096[256];
    int nparts_4096;
    
    // dsp state, buffers
    t_int** w;
    float** input;
    float** output;
    
    PartConvMax *pc;
    
} t_partconv;

static t_class *partconv_class;

// symbols
t_symbol *ps_buffer_tilde;

// function prototypes
int main(void);
t_buffer* _sym_to_buffer(t_symbol*);

void *partconv_new(t_symbol*, short, t_atom*);
void partconv_dsp(t_partconv *x, t_signal **sp, short int *count);
t_max_err partconv_scheme_set(t_partconv *x, void *attr, long argc, t_atom *argv);
t_int *partconv_perform(t_int *w);
void partconv_free(t_partconv *x);
void partconv_assist(t_partconv *x, void *b, long m, long a, char *s);

// partconv_check_scheme()

// main
int main(void) {
    
    t_class *c = class_new("partconv~", (method)partconv_new, (method)partconv_free, sizeof(t_partconv), 0L, A_GIMME, 0);
    
    class_dspinit(c);
    
    CLASS_ATTR_SYM(c, "buffer", 0, t_partconv, buffer);
    CLASS_ATTR_LABEL(c, "n", 0, "Name of buffer containing filter impulse responses");
    
    CLASS_ATTR_LONG(c, "n", 0, t_partconv, n);
    CLASS_ATTR_FILTER_MIN(c, "n", 1);
    CLASS_ATTR_LABEL(c, "n", 0, "Number of input channels");
    
    CLASS_ATTR_LONG(c, "m", 0, t_partconv, m);
    CLASS_ATTR_FILTER_MIN(c, "m", 1);
    CLASS_ATTR_LABEL(c, "m", 0, "Number of output channels");
    
    CLASS_ATTR_LONG(c, "k", 0, t_partconv, k);
    CLASS_ATTR_FILTER_MIN(c, "k", 1);
    CLASS_ATTR_LABEL(c, "n", 0, "Number of filters in buffer");
    
    CLASS_ATTR_LONG(c, "v", 0, t_partconv, v);
    CLASS_ATTR_FILTER_MIN(c, "v", 1);
    CLASS_ATTR_LABEL(c, "v", 0, "Impulse response length");
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_32", 0, t_partconv, scheme_32, nparts_32, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_32", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_32", 0, "Partioning scheme for 32-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_32", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_64", 0, t_partconv, scheme_64, nparts_64, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_64", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_64", 0, "Partioning scheme for 64-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_64", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_128", 0, t_partconv, scheme_128, nparts_128, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_128", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_128", 0, "Partioning scheme for 128-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_128", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_256", 0, t_partconv, scheme_256, nparts_256, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_256", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_256", 0, "Partioning scheme for 256-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_256", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_512", 0, t_partconv, scheme_512, nparts_512, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_512", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_512", 0, "Partioning scheme for 512-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_512", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_1024", 0, t_partconv, scheme_1024, nparts_1024, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_1024", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_1024", 0, "Partioning scheme for 1024-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_1024", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_2048", 0, t_partconv, scheme_2048, nparts_2048, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_2048", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_2048", 0, "Partioning scheme for 2048-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_2048", 0);
    
    CLASS_ATTR_LONG_VARSIZE(c, "scheme_4096", 0, t_partconv, scheme_4096, nparts_4096, 256);
    CLASS_ATTR_ACCESSORS(c, "scheme_4096", NULL, partconv_scheme_set);
    CLASS_ATTR_LABEL(c, "scheme_4096", 0, "Partioning scheme for 4096-sample vector size");
    CLASS_ATTR_SAVE(c, "scheme_4096", 0);
    
    class_addmethod(c, (method)partconv_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)partconv_assist, "assist", A_CANT, 0);
    
	class_register(CLASS_BOX, c);
    
	partconv_class = c;
    
    ps_buffer_tilde = gensym("buffer~");
    
    version(0);
    
    return 0;
    
}

void partconv_assist(t_partconv *x, void *b, long io, long index, char *s){
	switch(io){
        case 1:
            switch(index){
                case 0:
                    sprintf(s, "input %ld", index);
                    break;
            }
            break;
        case 2:
            switch(index){
                case 0:
                    sprintf(s, "output %ld", index);
                    break;
            }
            break;
	}
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

t_max_err partconv_scheme_set(t_partconv *x, void *attr, long argc, t_atom *argv) 
{
    int i;
    int part1;
    int partn;
    int* nparts = NULL;
    int* scheme = NULL;
    
    // make sure the length of the list is ok
    if (argc >= 1 && argc <= 255) {
        // ok
    } else {
        error("partconv~: partitioning scheme is too long, or too short");
        return MAX_ERR_GENERIC;
    }
    
    // check that all elements are integers
    for(i = 0; i < argc; i++) {
        if(argv[i].a_type != A_LONG) {
            error("partconv~: partitioning scheme must be a list of integers", i);
            return MAX_ERR_GENERIC;
        }
    }
    
    partn = 0;
    
    for(i = 0; i < argc; i++) {
        if(argv[i].a_w.w_long <= partn) {
            error("partconv~: partitioning scheme must be monotonically increasing");
            return MAX_ERR_GENERIC;
        } else {
            partn = argv[i].a_w.w_long;
        }
    }
    
    // check that all elements are powers of two
    post("partconv~: check powers of two");
    
    for(i = 0; i < argc; i++) {
        partn = argv[i].a_w.w_long;
        
        if (((partn | (partn - 1)) + 1) / 2 != partn) {
            post("partconv~: %d at position %d is not a power of two", partn, i);
            return MAX_ERR_GENERIC;
        }
    }
    
    // check that the first element is equal to the corresponding scheme start-vector size
    
    if (object_attr_get(x, gensym("scheme_32")) == attr) {
        post("partconv~: check 32");
        part1 = 32;
        nparts = &(x->nparts_32);
        scheme = &(x->scheme_32[0]);
    }
    else if (object_attr_get(x, gensym("scheme_64")) == attr) {
        post("partconv~: check 64");

    }
    else if (object_attr_get(x, gensym("scheme_128")) == attr) {

    }
    else if (object_attr_get(x, gensym("scheme_256")) == attr) {

    }
    else if (object_attr_get(x, gensym("scheme_512")) == attr) {

    }
    else if (object_attr_get(x, gensym("scheme_1024")) == attr) {

    }
    else if (object_attr_get(x, gensym("scheme_2048")) == attr) {

    }
    else if (object_attr_get(x, gensym("scheme_4096")) == attr) {

    }
    else {
        post("partconv~: unknown attribute");
        return MAX_ERR_GENERIC;
    }
    
    *nparts = argc;
    
    for(i = 0; i < argc; i++) {
        scheme[i] = argv[i].a_w.w_long;
    }
    
    for(i = argc; i < 256; i++) {
        scheme[i] = 0;
    }
    
    // all okay
    return MAX_ERR_NONE;
    
}

// new
void *partconv_new(t_symbol *s, short argc, t_atom *argv) {

    int i;
    
    // scope vars
    t_partconv *x;
    
    // instantiate 
    x = (t_partconv *)object_alloc(partconv_class);
    
    if(! x) {
        return NULL;
    }
    
    x->n = 1;
    x->m = 1;
    x->k = 1;
    x->v = 4096;
    
    x->input = NULL;
    x->output = NULL;
    x->w = NULL;

    x->scheme_32[0] = 32;
    x->scheme_32[1] = 256;
    x->scheme_32[2] = 2048;
    x->scheme_32[3] = 16384;
    x->scheme_32[4] = 131072;
    x->nparts_32 = 5;
    
    x->scheme_64[0] = 64;
    x->scheme_64[1] = 256;
    x->scheme_64[2] = 2048;
    x->scheme_64[3] = 16384;
    x->scheme_64[4] = 131072;
    x->nparts_64 = 5;
    
    x->scheme_128[0] = 128;
    x->scheme_128[1] = 256;
    x->scheme_128[2] = 2048;
    x->scheme_128[3] = 16384;
    x->scheme_128[4] = 131072;
    x->nparts_128 = 5;
    
    x->scheme_256[0] = 256;
    x->scheme_256[1] = 2048;
    x->scheme_256[2] = 16384;
    x->scheme_256[3] = 131072;
    x->nparts_256 = 4;
    
    x->scheme_512[0] = 512;
    x->scheme_512[1] = 2048;
    x->scheme_512[2] = 16384;
    x->scheme_512[3] = 131072;
    x->nparts_512 = 4;
    
    x->scheme_1024[0] = 1024;
    x->scheme_1024[1] = 2048;
    x->scheme_1024[2] = 16384;
    x->scheme_1024[3] = 131072;
    x->nparts_1024 = 4;
    
    x->scheme_2048[0] = 2048;
    x->scheme_2048[1] = 16384;
    x->scheme_2048[2] = 131072;
    x->nparts_2048 = 3;
    
    x->scheme_4096[0] = 4096;
    x->scheme_4096[1] = 16384;
    x->scheme_4096[2] = 131072;
    x->nparts_4096 = 3;
    
    // pass attrs...
    attr_args_process(x, argc, argv);
    
    x->w = (t_int**)malloc(sizeof(t_int*) * (x->n + x->m + 2));
    x->input = (float**)malloc(sizeof(float*) * x->n);
    x->output = (float**)malloc(sizeof(float*) * x->m);
    
    // allocate inlets
    dsp_setup((t_pxobject *)x, x->n);
    
    // allocate outlets
    for(i = 0; i < x->m; i++) {
        outlet_new((t_object *)x, "signal");	// type of outlet: "signal"
    }
    
    x->pc = new PartConvMax();
    
    return (x);
    
}

// free
void partconv_free(t_partconv *x) {
	
    // @todo cleanup of partconv threads if they are still running

    free(x->input);
    free(x->output);
    
	dsp_free(&(x->x_obj));
    
    delete x->pc;
    
}

// dsp setup
void partconv_dsp(t_partconv *x, t_signal **sp, short *connect) {

    int i;
    
    t_buffer *b;
    
    float* bdata;
    int bstride;

    if(sp[0]->s_n < 32) {
        post("partconv~: vector size less than 32 is not supported");
    }
    
    if(sp[0]->s_n > 4096) {
        post("partconv~: vector size greater than 4096 is not supported");
    }
    
    // setup args
    x->w[0] = (t_int*)x;
    x->w[1] = (t_int*)(sp[0]->s_n);
    
    for(i = 0; i < (x->n + x->m); i++) {
        x->w[i+2] = (t_int*)(sp[i]->s_vec);
    }
    
    // @todo check for buffer validity
    
    b = _sym_to_buffer(x->buffer);
    
    if(b && (b->b_valid)) {
        bdata = b->b_samples;
        bstride = b->b_nchans;
        
        if(b->b_frames < (x->k * x->v)) {
            error("partconv~: frames in buffer is less than k*v");
            return;
        }
    } else {
        error("partconv~: invalid buffer");
        return;
    }
    
    // @todo eric; cleanup partconv threads if they are running
    
    // @todo eric; setup of partconv threads with new partitioning scheme
    
	dsp_addv(partconv_perform, 2 + x->n + x->m, (void**)(x->w));
    
}

t_int* partconv_perform(t_int *w) {

    t_int** wp;
    int i, j, s;
	t_partconv *x;
    
    wp = (t_int**)w;
    
    x = (t_partconv*)(wp[1]);
    s = (int)(wp[2]);
    
    if(s < 32 || s > 4096) {
        // not supported vector size
        return w + 3 + x->n + x->m;
    }
    
    for(i = 0; i < x->n; i++) {
        x->input[i] = (float*)(wp[3 + i]);
    }
    
    for(i = 0; i < x->m; i++) {
        x->output[i] = (float*)(wp[3 + i + x->n]);
    }
    
    // @todo eric; exchange samples with running partconv threads
        
    // outputs all zeros
    for(i = 0; i < x->m; i++) {
        for(j = 0; j < s; j++) {
            x->output[i][j] = 0.f;
        }
    }
    
    return w + 3 + x->n + x->m;
    
}

