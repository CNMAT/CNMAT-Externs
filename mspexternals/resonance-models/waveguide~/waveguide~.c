/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Andy Schmeder <andy@cnmat.berkeley.edu>, 
 The Center for New Music and Audio Technologies,
 University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
 
 
*/

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: waveguide~
DESCRIPTION: Linear and non-linear parameterized waveguide mesh
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2008
VERSION 0.1: Initial version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

// MMJ includes
#include "ext.h"
#include "z_dsp.h"

// CNMAT versioning
#include "version.h"
#include "version.c"

// Standard library
#include <math.h>

/* 
 TODO:
 
 - Fractional delay line
 - Linear interpolation of waveguide parameters on a per-frame basis
 
 */

// ---------------------------------------------------------------------------------------------

// waveguide represents an edge in a waveguide mesh, consisting of
// an internal delay line
// a low pass filter at each end
// a bi-polar non-linearity filter at each end
// the current energy at its output terminals

typedef struct {
    
	int size;           // buffer size (power of two)
	float *buffer[2];   // left and right delay
	int ptr;            // pointer to buffer position

    float delay_ms;     // delay in milliseconds
	int delay;          // current delay (samples)
    
	float fc;           // filter coefficient for low-pass at junction
	float lp[2];        // temp variables for low-pass at junction
    
	float a1a;          // positive-going non-linearity
	float a1b;          // negative-going non-linearity
	float zm1[2];       // temp variables for non-linearity filter
    
    float out[2];       // outputs at each end
    
} waveguide;

waveguide* waveguide_new(int size, float fc, float nl_pos, float nl_neg)
{
	waveguide *w = malloc(sizeof(waveguide));
    
	w->size = size;
	w->buffer[0] = calloc(size, sizeof(float));
	w->buffer[1] = calloc(size, sizeof(float));
	w->ptr = 0;
    
	w->delay = size;
    
	w->fc = fc;
	w->lp[0] = 0.0f;
	w->lp[1] = 0.0f;
    
	w->a1a = (1.0f - nl_pos) / (1.0f + nl_pos);
	w->a1b = (1.0f - nl_neg) / (1.0f + nl_neg);
	w->zm1[0] = 0.0f;
	w->zm1[1] = 0.0f;
    
	return w;
}

void waveguide_free(waveguide* w) {
    if(! w) {
        return;
    }
    
    free(w->buffer[0]);
    free(w->buffer[1]);
    
    free(w);
}

void waveguide_reset(waveguide* w) {
    // set buffer to zero
	memset(w->buffer[0], 0, w->size * sizeof(float));
	memset(w->buffer[1], 0, w->size * sizeof(float));

    // reset lowpass filter
	w->lp[0] = 0.0f;
	w->lp[1] = 0.0f;

    // reset non-linearity filter
	w->zm1[0] = 0.0f;
	w->zm1[1] = 0.0f;    
    
    // reset output state
    w->out[0] = 0.0f;
    w->out[1] = 0.0f;
}

void waveguide_set_delay(waveguide* w, float delay_ms, float fs)
{
    int delay;
    
    delay = ((delay_ms / 1000.) * fs);
    
    w->delay_ms = delay_ms;
    
	if (delay > w->size) {
		w->delay = w->size;
	} else if (delay < 1) {
		w->delay = 1;
	} else {
		w->delay = delay;
	}
}

void waveguide_set_fc(waveguide* w, float fc)
{
	w->fc = fc;
}

void waveguide_set_nl_pos(waveguide* w, float nl_pos)
{
	w->a1a = (1.0f - nl_pos) / (1.0f + nl_pos);
    w->a1a = nl_pos;
}

void waveguide_set_nl_neg(waveguide* w, float nl_neg)
{
	w->a1b = (1.0f - nl_neg) / (1.0f + nl_neg);
    w->a1b = nl_neg;
}

inline void waveguide_process(waveguide *w, float in0, float in1)
{
	float tmp;
	float a1;
	float b;
    
	w->out[0] = w->buffer[0][(w->ptr + w->delay) % w->size];
	w->out[0] = w->lp[0] * (w->fc - 1.0f) + w->fc * w->out[0];
	w->lp[0] = w->out[0];
	b = (w->out[0] + 1.0) * 6.0f;
	if (b > 1.0f) {
		b = 1.0f;
	} else if (b < 0.0f) {
		b = 0.0f;
	}
	a1 = b * w->a1a + (1.0f - b) * w->a1b;
	tmp = w->out[0] * -a1 + w->zm1[0];
	w->zm1[0] = tmp * a1 + w->out[0];
	w->out[0] = tmp;
    
	w->out[1] = w->buffer[1][(w->ptr + w->delay) % w->size];
	w->out[1] = w->lp[1] * (w->fc - 1.0f) + w->fc * w->out[1];
	w->lp[1] = w->out[1];
	b = (w->out[1] + 1.0) * 6.0f;
	if (b > 1.0f) {
		b = 1.0f;
	} else if (b < 0.0f) {
		b = 0.0f;
	}
	a1 = b * w->a1a + (1.0f - b) * w->a1b;
	tmp = w->out[1] * -a1 + w->zm1[1];
	w->zm1[1] = tmp * a1 + w->out[1];
	w->out[1] = tmp;
    
	w->buffer[0][w->ptr] = in0;
	w->buffer[1][w->ptr] = in1;
	w->ptr--;
	if (w->ptr < 0) {
		w->ptr += w->size;
	}
}

// -------------------------------------------------------------------------------------------------

// waveguide_mesh reperesents a collection of waveguide edges,
// a list of edge-node connection pairs,
// a list of input to node mappings,
// a list of output to node mappings,
// and the deflections of the mesh at time t-0 and t-1

typedef int int_pair[2];

typedef struct {
    
    int n;              // how many nodes
    int e;              // how many edges

    int_pair* edge;     // edge-node connectivity matrix 0 => 0, 1, 1 => 0, 2... etc
    
    waveguide** w;      // waveguide edges
    
    float* d;           // mesh deflection state at each node
    float* dm1;           // previous mesh deflection state at each node
    
    int inputs;         // how many inputs
    int* in;            // input locations (nodes)
    
    int outputs;        // number of outputs
    int* out;           // output locations (nodes)
    
} waveguide_mesh;

waveguide_mesh* waveguide_mesh_new(int n, int e, int s, int inputs, int outputs) {
    
    int i;
    waveguide_mesh* m = malloc(sizeof(waveguide_mesh));

    m->n = n;
    m->e = e;

    m->edge = (int_pair*)calloc(e, sizeof(int[2]));
    memset(m->edge, 0, sizeof(int[2]) * e);
    
    m->d = calloc(n, sizeof(float));
    m->dm1 = calloc(n, sizeof(float));
    m->w = calloc(e, sizeof(waveguide*));
    
    for(i = 0; i < e; i++) {
        m->w[i] = waveguide_new(s, 0.9, 0.0, 0.0);
    }
    
    m->inputs = inputs;
    m->in = calloc(inputs, sizeof(int));
    memset(m->in, 0, sizeof(int) * inputs);
    
    m->outputs = outputs;
    m->out = calloc(outputs, sizeof(int));
    memset(m->out, 0, sizeof(int) * outputs);
    
    return m;
}

void waveguide_mesh_reset(waveguide_mesh* m) {
    
    int i;
    
    memset(m->d, 0, m->n * sizeof(float));
    memset(m->dm1, 0, m->n * sizeof(float));
    
    for(i = 0; i < m->e; i++) {
        waveguide_reset(m->w[i]);
    }
    
}

void waveguide_mesh_free(waveguide_mesh* m) {

    int i;

    if(! m) {
        return;
    }
    
    free(m->d);
    free(m->dm1);
    free(m->edge);
    
    free(m->in);
    free(m->out);
    
    for(i = 0; i < m->e; i++) {
        waveguide_free(m->w[i]);
    }
    
    free(m->w);
    free(m);
    
}

// propagate new delay caused by sample rate change
void waveguide_mesh_update_fs(waveguide_mesh* m, float fs) {
    int i;
    
    for(i = 0; i < m->e; i++) {
        waveguide_set_delay(m->w[i], m->w[i]->delay_ms, fs);
    }
}

void waveguide_mesh_connect_edge(waveguide_mesh* m, int e, int n0, int n1) {
    m->edge[e][0] = n0;
    m->edge[e][1] = n1;
}

void waveguide_mesh_connect_input(waveguide_mesh* m, int i, int n) {
    m->in[i] = n;
}

void waveguide_mesh_connect_output(waveguide_mesh* m, int o, int n) {
    m->out[o] = n;
}

void waveguide_mesh_process(waveguide_mesh* m, int s, float** s_in, float** s_out) {
    
    int i;
    int j;
    int k;
    
    float p;  // energy entering the junction
    int q;    // number of edges entering the junction
    int m0;   // indicator that edge maps to this junction, edge side 0
    int m1;   // indicator that edge maps to this junction, edge side 1
    
    // update parameters for interpolation here...

    // iterate over samples in block
    
    for(k = 0; k < s; k++) {

        // update junction transfers
        for(i = 0; i < m->n; i++) {
            
            // save previous state
            m->dm1[i] = m->d[i];

            // find all edges incident to this junction, and add their energy
            // branch-free, constant overhead formulation 

            // temp variables for this junction calculation
            p = 0.f;
            q = 0;
            
            for(j = 0; j < m->e; j++) {
                m0 = (m->edge[j][0] == i);
                m1 = (m->edge[j][1] == i);
                
                // post("edge check: e = %d, m0 = %d, m1 = %d", j, m0, m1);
                
                p += (m->w[j]->out[0] * m0);
                p += (m->w[j]->out[1] * m1);
                
                q += (m0 + m1);
            }

            m->d[i] = p * (2.f / q); // junction dispersion = number of output / (number of inputs + number of outputs)
            
            // kill any denormal numbers here...
            if(m->d[i] < 1.0e-18f && m->d[i] > -1.0e-18f) { // about 200. dB down...
                m->d[i] = 0.f;
            }
        }
        
        // inject input energy
        for(i = 0; i < m->inputs; i++) {
            m->d[m->in[i]] += s_in[i][k];
        }
        
        // read output energy
        for(i = 0; i < m->outputs; i++) {
             s_out[i][k] = m->d[m->out[i]];
        }
        
        // update waveguides
        for(i = 0; i < m->e; i++) {
            // dispersed energy (- term) is the opposite side of the waveguide at the time t - 1
            waveguide_process(m->w[i], m->d[m->edge[i][1]] - m->w[i]->out[1], m->d[m->edge[i][0]] - m->w[i]->out[0]);
        }
        
    }
    
}

// -------------------------------------------------------------------------------------------------

// MaxMSP stuff starts here

#define ATOM_IS_INT(x) ((x).a_type == A_LONG)
#define ATOM_IS_INT_OR_STAR(x) ((x).a_type == A_LONG || ((x).a_type == A_SYM && strcmp((x).a_w.w_sym->s_name, "*") == 0))

void *waveguide_tilde_class;

typedef struct 
{
	t_pxobject x_obj;

    // dsp state 
    t_int** w;
    float** s_in;
    float** s_out;
    int s_n;
    
    int do_reset;
    
    waveguide_mesh* mesh;

} waveguide_tilde_state;

// dsp
t_int *waveguide_tilde_perform(t_int *w);
void waveguide_tilde_dsp(waveguide_tilde_state *x, t_signal **sp, short *connect);

// messages (all)
void waveguide_tilde_list(waveguide_tilde_state *x, t_symbol *s, short argc, t_atom *argv);
void waveguide_tilde_reset(waveguide_tilde_state *x);

void waveguide_tilde_assist(waveguide_tilde_state *x, void *b, long m, long a, char *s);
void* waveguide_tilde_new(t_symbol *s, short argc, t_atom *argv);
void waveguide_tilde_free(waveguide_tilde_state *x);

t_int *waveguide_tilde_perform(t_int *w) {

    // retreive inlet, outlet vectors...
    t_int** wp;
    int s;
    int i;
    waveguide_tilde_state* x;
    
    wp = (t_int**)w;
    
    x = (waveguide_tilde_state*)(wp[1]);
    s = (int)(wp[2]);
    
    for(i = 0; i < x->mesh->inputs; i++) {
        x->s_in[i] = (float*)(wp[3 + i]);
    }
    
    for(i = 0; i < x->mesh->outputs; i++) {
        x->s_out[i] = (float*)(wp[3 + i + x->mesh->inputs]);
    }
    
    if(x->do_reset) {
        post("waveguide~: reset");
        waveguide_mesh_reset(x->mesh);
        x->do_reset = 0;
    }
    
    waveguide_mesh_process(x->mesh, s, x->s_in, x->s_out);
    
    return w + 2 + x->mesh->inputs + x->mesh->outputs + 1;
    
}

void waveguide_tilde_dsp(waveguide_tilde_state *x, t_signal **sp, short *connect) {
    
    int i;
    
    // reset mesh
    waveguide_mesh_reset(x->mesh);

    // update sampling rate
    waveguide_mesh_update_fs(x->mesh, sys_getsr());
    
    // setup crazy pointers for dsp callback
    x->w[0] = (t_int*)x;
    x->w[1] = (t_int*)(sp[0]->s_n);
    
    for(i = 0; i < (x->mesh->inputs + x->mesh->outputs); i++) {
        x->w[i+2] = (t_int*)(sp[i]->s_vec);
    }
    
	dsp_addv(waveguide_tilde_perform, 2 + x->mesh->inputs + x->mesh->outputs, (void**)(x->w)); // not sure if this is right for number of inlets, outlets
    
}

void waveguide_tilde_reset(waveguide_tilde_state *x) {

    x->do_reset = 1;
    
}

void waveguide_tilde_list(waveguide_tilde_state *x, t_symbol *s, short argc, t_atom *argv) {

    int i, j;
    int arg0;
    int arg1;
    int arg2;
    
    int arg0_star;
    
    float v;
    
    // iterates over the list and interprets any commands encountered
    // user can update multiple things in a single list command

    // messages:

    // connect input i n
    // connect output o n
    // connect edge i n0 n1
    
    // set fc i (0. - 1.)
    // set nl_pos i (0. - 1.)
    // set nl_neg i (0. - 1.)
    // set nl i (0. - 1.) (symmetric)
    // set delay i (0. - ... msec)
    
    for(i = 0; i < argc; i++) {
        
        if(i == 0 || (i > 0 && argv[i].a_type == A_SYM)) {
            
            if((i == 0 && strcmp(s->s_name, "connect") == 0) || (i > 0 && strcmp(argv[i].a_w.w_sym->s_name, "connect") == 0)) {
                if(i > 0) {
                    i++;
                }
                
                if(argv[i].a_type == A_SYM) {
                    
                    if(strcmp(argv[i].a_w.w_sym->s_name, "input") == 0) {
                        i++;
                        
                        if(i + 1 < argc && argv[i].a_type == A_LONG && argv[i+1].a_type == A_LONG) {
                            arg0 = argv[i].a_w.w_long;
                            i++;
                            arg1 = argv[i].a_w.w_long;
                            
                            if(arg0 >= x->mesh->inputs || arg0 < 0) {
                                post("waveguide~: connect input (%d) (n): invalid input channel", arg0);
                                return;
                            }
                            if(arg1 >= x->mesh->n || arg1 < 0) {
                                post("waveguide~: connect input (%d) (%d): invalid node number", arg0, arg1);
                                return;
                            }
                            
                            waveguide_mesh_connect_input(x->mesh, arg0, arg1);
                            
                        } else {
                            post("waveguide~: connect input (i) (n): requires two ints");
                            return;
                        }
                        
                    }
                    else if(strcmp(argv[i].a_w.w_sym->s_name, "output") == 0) {
                        i++;
                        
                        if(i + 1 < argc && argv[i].a_type == A_LONG && argv[i+1].a_type == A_LONG) {
                            arg0 = argv[i].a_w.w_long;
                            i++;
                            arg1 = argv[i].a_w.w_long;
                            
                            if(arg0 >= x->mesh->outputs || arg0 < 0) {
                                post("waveguide~: connect output (%d) (n): invalid output channel", arg0);
                                return;
                            }
                            if(arg1 >= x->mesh->n || arg1 < 0) {
                                post("waveguide~: connect output (%d) (%d): invalid node number", arg0, arg1);
                                return;
                            }
                            
                            waveguide_mesh_connect_output(x->mesh, arg0, arg1);
                            
                        } else {
                            post("waveguide~: connect output (o) (n): requires two ints");
                            return;
                        }
                        
                    }

                    else if(strcmp(argv[i].a_w.w_sym->s_name, "edge") == 0) {
                        i++;
                        
                        if(i + 2 < argc && argv[i].a_type == A_LONG && argv[i+1].a_type == A_LONG && argv[i+1].a_type == A_LONG) {
                            arg0 = argv[i].a_w.w_long;
                            i++;
                            arg1 = argv[i].a_w.w_long;
                            i++;
                            arg2 = argv[i].a_w.w_long;

                            if(arg0 >= x->mesh->e || arg0 < 0) {
                                post("waveguide~: connect edge (%d) (n0) (n1): invalid edge number", arg0);
                                return;
                            }
                            if(arg1 >= x->mesh->n || arg1 < 0) {
                                post("waveguide~: connect edge (%d) (%d) (n1): invalid first node number", arg0, arg1);
                                return;
                            }
                            if(arg2 >= x->mesh->n || arg2 < 0) {
                                post("waveguide~: connect edge (%d) (%d) (%d): invalid second node number", arg0, arg1, arg2);
                                return;
                            }
                            
                            waveguide_mesh_connect_edge(x->mesh, arg0, arg1, arg2);
                            
                        } else {
                            post("waveguide~: connect edge (e) (n0) (n1): requires three ints");
                            return;
                        }
                        
                    }
                    
                    else {
                        post("waveguide~: connect (input | output | edge): requires connection type");
                        return;
                    }
                    
                } else {
                    post("waveguide~: connect (input | output | edge): requires connection type");
                    return;
                }
                
            }
            
            else if((i == 0 && strcmp(s->s_name, "set") == 0) || (i > 0 && strcmp(argv[i].a_w.w_sym->s_name, "set") == 0)) {
                if(i > 0) {
                    i++;
                }

                if(argv[i].a_type == A_SYM) {
                    
                    if(strcmp(argv[i].a_w.w_sym->s_name, "fc") == 0) {
                        i++;
                        
                        if(i + 1 < argc && ATOM_IS_INT_OR_STAR(argv[i]) && argv[i+1].a_type == A_FLOAT) {
                            if(ATOM_IS_INT(argv[i])) {
                                arg0 = argv[i].a_w.w_long;
                                arg0_star = 0;
                            } else {
                                arg0_star = 1;
                            }
                            
                            i++;
                            v = argv[i].a_w.w_float;

                            if(v <= 0.f || v >= 1.f) {
                                post("waveguide~: set fc (%d) (%f): v out of bounds (0.0 - 1.0)", arg0, v);
                                return;
                            }
                            
                            if(arg0_star == 0) {
                                if(arg0 >= x->mesh->e || arg0 < 0) {
                                    post("waveguide~: set fc (%d) (v): invalid edge number", arg0);
                                    return;
                                }
                                
                                waveguide_set_fc(x->mesh->w[arg0], v);
                            } else {
                                for(j = 0; j < x->mesh->e; j++) {
                                    waveguide_set_fc(x->mesh->w[j], v);
                                }
                            }
                            
                        } else {
                            post("waveguide~: set fc (e) (v): requires int, float arguments");
                            return;
                        }
                        
                    }
                    
                    else if(strcmp(argv[i].a_w.w_sym->s_name, "nl_pos") == 0) {
                        i++;
                        
                        if(i + 1 < argc && ATOM_IS_INT_OR_STAR(argv[i]) && argv[i+1].a_type == A_FLOAT) {
                            if(ATOM_IS_INT(argv[i])) {
                                arg0 = argv[i].a_w.w_long;
                                arg0_star = 0;
                            } else {
                                arg0_star = 1;
                            }
                            i++;
                            v = argv[i].a_w.w_float;
                            
                            if(v < 0.f || v >= 1.f) {
                                post("waveguide~: set nl_pos (%d) (%f): v out of bounds [0.0 - 1.0)", arg0, v);
                                return;
                            }
                            
                            if(arg0_star == 0) {
                                if(arg0 >= x->mesh->e || arg0 < 0) {
                                    post("waveguide~: set nl_pos (%d) (v): invalid edge number", arg0);
                                    return;
                                }
                                
                                waveguide_set_nl_pos(x->mesh->w[arg0], v);
                            } else {
                                for(j = 0; j < x->mesh->e; j++) {
                                    waveguide_set_nl_pos(x->mesh->w[j], v);
                                }
                            }
                            
                        } else {
                            post("waveguide~: set nl_pos (e) (v): requires int, float arguments");
                            return;
                        }
                        
                    }
                        
                    else if(strcmp(argv[i].a_w.w_sym->s_name, "nl_neg") == 0) {
                        i++;
                        
                        if(i + 1 < argc && ATOM_IS_INT_OR_STAR(argv[i]) && argv[i+1].a_type == A_FLOAT) {
                            if(ATOM_IS_INT(argv[i])) {
                                arg0 = argv[i].a_w.w_long;
                                arg0_star = 0;
                            } else {
                                arg0_star = 1;
                            }
                            i++;
                            v = argv[i].a_w.w_float;
                            
                            if(v < 0.f || v >= 1.f) {
                                post("waveguide~: set nl_neg (%d) (%f): v out of bounds [0.0 - 1.0)", arg0, v);
                                return;
                            }
                            
                            if(arg0_star == 0) {
                                if(arg0 >= x->mesh->e || arg0 < 0) {
                                    post("waveguide~: set nl_neg (%d) (v): invalid edge number", arg0);
                                    return;
                                }
                                
                                waveguide_set_nl_neg(x->mesh->w[arg0], v);
                            } else {
                                for(j = 0; j < x->mesh->e; j++) {
                                    waveguide_set_nl_neg(x->mesh->w[j], v);
                                }
                            }
                            
                        } else {
                            post("waveguide~: set nl_neg (e) (v): requires int, float arguments");
                            return;
                        }
                        
                    }
                    
                    else if(strcmp(argv[i].a_w.w_sym->s_name, "nl") == 0) {
                        i++;
                        
                        if(i + 1 < argc && ATOM_IS_INT_OR_STAR(argv[i]) && argv[i+1].a_type == A_FLOAT) {
                            if(ATOM_IS_INT(argv[i])) {
                                arg0 = argv[i].a_w.w_long;
                                arg0_star = 0;
                            } else {
                                arg0_star = 1;
                            }
                            i++;
                            v = argv[i].a_w.w_float;
                            
                            if(v < 0.f || v >= 1.f) {
                                post("waveguide~: set nl (%d) (%f): v out of bounds [0.0 - 1.0)", arg0, v);
                                return;
                            }
                            
                            if(arg0_star == 0) {
                                if(arg0 >= x->mesh->e || arg0 < 0) {
                                    post("waveguide~: set nl (%d) (v): invalid edge number", arg0);
                                    return;
                                }
                                
                                waveguide_set_nl_pos(x->mesh->w[arg0], v);
                                waveguide_set_nl_neg(x->mesh->w[arg0], v);
                            } else {
                                for(j = 0; j < x->mesh->e; j++) {
                                    waveguide_set_nl_pos(x->mesh->w[j], v);
                                    waveguide_set_nl_neg(x->mesh->w[j], v);
                                }
                            }
                            
                        } else {
                            post("waveguide~: set nl (e) (v): requires int, float arguments");
                            return;
                        }
                        
                    }
                    
                    else if(strcmp(argv[i].a_w.w_sym->s_name, "delay") == 0) {
                        i++;
                        
                        if(i + 1 < argc && ATOM_IS_INT_OR_STAR(argv[i]) && argv[i+1].a_type == A_FLOAT) {
                            if(ATOM_IS_INT(argv[i])) {
                                arg0 = argv[i].a_w.w_long;
                                arg0_star = 0;
                            } else {
                                arg0_star = 1;
                            }
                            i++;
                            v = argv[i].a_w.w_float;
                            
                            if(v <= 0.f) {
                                post("waveguide~: set delay (%d) (%f): v out of bounds [0.0 - Inf)", arg0, v);
                                return;
                            }
                            
                            if(arg0_star == 0) {
                                if(((v / 1000.) * sys_getsr()) > x->mesh->w[arg0]->size) {
                                    post("waveguide~: set delay (%d) (%f): v too large for buffer size", arg0, v);
                                    return;
                                }
                                
                                if(arg0 >= x->mesh->e || arg0 < 0) {
                                    post("waveguide~: set delay (%d) (v): invalid edge number", arg0);
                                    return;
                                }
                                
                                waveguide_set_delay(x->mesh->w[arg0], v, sys_getsr());
                            } else {
                                for(j = 0; j < x->mesh->e; j++) {
                                    if(((v / 1000.) * sys_getsr()) > x->mesh->w[j]->size) {
                                        post("waveguide~: set delay (%d) (%f): v too large for buffer size", j, v);
                                        return;
                                    }
                                    
                                    waveguide_set_delay(x->mesh->w[j], v, sys_getsr());
                                }
                            }
                            
                        } else {
                            post("waveguide~: set delay (e) (v): requires int, float arguments");
                            return;
                        }
                        
                    } else {
                        post("waveguide~: set (fc | nl_pos | nl_neg | nl | delay): command required");
                        return;
                    }
                    
                } else {
                    post("waveguide~: set (fc | nl_pos | nl_neg | nl | delay): command type required");
                    return;
                }
                    
            }
            
            else {
                post("waveguide~: (connect | set): command required");
                return;
            }

        } else {
            post("waveguide~: (connect | set): command required");
            return;
        }
        
    }
    
}

void* waveguide_tilde_new(t_symbol *s, short argc, t_atom *argv) {
    
    waveguide_tilde_state* x;
    
    int init_e = 1;
    int init_n = 2;
    int init_s = 1024;
    int init_inputs = 1;
    int init_outputs = 1;
    float init_delay = 1.0;
    
    int i;
    
    x = (waveguide_tilde_state*)newobject(waveguide_tilde_class);
    
    // read arguments
    // initialization args
    // @edges
    // @nodes
    // @size
    // @inputs
    // @outputs
    // @delay

    for(i = 0; i < argc; i++) {

        if(argv[i].a_type == A_SYM) {
            
            if(strcmp(argv[i].a_w.w_sym->s_name, "@edges") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    init_e = argv[i].a_w.w_long;
                } else {
                    post("waveguide~: expected int for @edges");
                }
            }
            
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@nodes") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    init_n = argv[i].a_w.w_long;
                } else {
                    post("waveguide~: expected int for @nodes");
                }
            }
            
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@size") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    init_s = argv[i].a_w.w_long;
                } else {
                    post("waveguide~: expected int for @size");
                }
            }
            
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@inputs") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    init_inputs = argv[i].a_w.w_long;
                } else {
                    post("waveguide~: expected int for @inputs");
                }
            }
            
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@outputs") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_LONG) {
                    init_outputs = argv[i].a_w.w_long;
                } else {
                    post("waveguide~: expected int for @outputs");
                }
            }
            
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@delay") == 0) {
                i++;
                if(i < argc && argv[i].a_type == A_FLOAT) {
                    init_delay = argv[i].a_w.w_float;
                } else {
                    post("waveguide~: expected float for @delay");
                }
            }
        }
    }
    
    x->mesh = waveguide_mesh_new(init_n, init_e, init_s, init_inputs, init_outputs);
    
    for(i = 0; i < x->mesh->e; i++) {
        waveguide_set_delay(x->mesh->w[i], init_delay, sys_getsr());
    }
    
    x->w = (t_int**)malloc(sizeof(t_int*) * (x->mesh->inputs + x->mesh->outputs + 2));
    x->s_in = (float**)malloc(sizeof(float*) * x->mesh->inputs);
    x->s_out = (float**)malloc(sizeof(float*) * x->mesh->outputs);
    
    x->do_reset = 0;

    // allocate inlets, setup
    dsp_setup((t_pxobject *)x, x->mesh->inputs);
    
    // allocate outlets
    for(i = 0; i < x->mesh->outputs; i++) {
        outlet_new((t_object *)x, "signal");
    }
    
    return (void*)x;
}

void waveguide_tilde_free(waveguide_tilde_state* x) {
    
    dsp_free(&(x->x_obj));
    
    free(x->w);
    free(x->s_in);
    free(x->s_out);
    
    free(x->mesh);

}

int main(void) {
    
	version(0);
    
	setup((t_messlist **)&waveguide_tilde_class, (method)waveguide_tilde_new, 
		  (method)waveguide_tilde_free, (short)sizeof(waveguide_tilde_state),
		  0L, A_GIMME, 0);

	addmess((method)waveguide_tilde_dsp, "dsp", A_CANT, 0);
	addmess((method)waveguide_tilde_list, "connect", A_GIMME, 0);
	addmess((method)waveguide_tilde_list, "set", A_GIMME, 0);
	addmess((method)waveguide_tilde_reset, "reset", 0);

	dsp_initclass();
   
    return 0;
}

