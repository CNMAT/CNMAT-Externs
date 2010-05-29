/*

Written by Andy Schmeder, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2008, The Regents of 
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: accumulate~
DESCRIPTION: Signal integrator with signal-rate reset control, configurable bounds, leakiness and overflow modes
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision$
VERSION 0.3: implemented attributes properly -JM
VERSION 0.2: Added argument syntax, updated mode controls
VERSION 0.1: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "version.h"
#include "version.c"

#include <math.h>
#include <float.h>
#include <string.h>
/*
#define MODE_CLIP 0
#define MODE_WRAP 1
#define MODE_RESET 2
*/
t_symbol *ps_clip, *ps_wrap, *ps_reset;
t_class *acc_class;

#ifndef MAXFLOAT
#define MAXFLOAT ((float)3.40282346638528860e+38)
#endif

typedef struct _t_acc
{
    t_pxobject x_obj;	//  header
    float current;
    float start;
    float max;
    float min;
    t_symbol  *mode;
    float scale;
} t_acc;

int main(void);

void *acc_new(t_symbol*, short, t_atom*);
void acc_dsp(t_acc *x, t_signal **sp, short int *count);
t_int *acc_perform(t_int *w);
void acc_free(t_acc *x);

int main(void) {
    t_class *c = class_new("accumulate~", (method)acc_new, (method)acc_free, (short)sizeof(t_acc), 0L, A_GIMME, 0);
    class_dspinit(c);

    class_addmethod(c, (method)acc_dsp, "dsp", A_CANT, 0);

    CLASS_ATTR_FLOAT(c, "min", 0, t_acc, min);
    CLASS_ATTR_FLOAT(c, "max", 0, t_acc, max);
    CLASS_ATTR_FLOAT(c, "start", 0, t_acc, start);
    CLASS_ATTR_FLOAT(c, "scale", 0, t_acc, scale);
    CLASS_ATTR_SYM(c, "mode", 0, t_acc, mode);

    ps_clip = gensym("clip");
    ps_wrap = gensym("wrap");
    ps_reset = gensym("reset");

    class_register(CLASS_BOX, c);
    acc_class = c;

    version(0);    
    return 0;
}

void *acc_new(t_symbol *s, short argc, t_atom *argv) {
    t_acc *x;
    int i;
	
    if(x = (t_acc *)object_alloc(acc_class)){
	    dsp_setup((t_pxobject *)x, 2);      // two signal inlets
    
	    x->current = 0.f;
	    x->start = 0.f;
	    x->max = MAXFLOAT;
	    x->min = -1.0f * MAXFLOAT;
	    x->mode = ps_clip; // Saturate
	    x->scale = 1.0f;
    
	    outlet_new((t_object *)x,"signal");
	    attr_args_process(x, argc, argv);
    
	    x->current = x->start;
    	return x;
    }
    return NULL;
}

void acc_dsp(t_acc *x, t_signal **sp, short int *count) {
	// in, in, out, size, x
	dsp_add(acc_perform, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n, x);  
}

void acc_free(t_acc *x) {
    dsp_free((t_pxobject *)x);
}

t_int *acc_perform(t_int *w) {
    
    t_float *signal = (t_float *)(w[1]);  // input1
    t_float *control = (t_float *)(w[2]);  // input2
    t_float *out = (t_float *)(w[3]); // output
    int size = w[4]; // vector size
    t_acc *x = (t_acc *) w[5];

    t_float next_signal;
    t_float next_control;

    /*
     
     When the control signal is zero, accumulate~ acts as a pass-through; 
     i.e. the current sample is still "accumulated", or, the control signal causes the
     accumulator to be reset for the *next* sample output.  
     
     This behavior ensures that accumulate~ has zero sample delay on its output.
     
     */

    // here the loop is copied three times for each case to avoid unnecessary conditions in the inner loop
    
    if (x->mode == ps_clip) {
        
        while (size--) {
        
            next_signal = *signal++;
            next_control = *control++;

            // reset by control signal
            if (next_control == 0.) {
                x->current = x->start;
            }
            
            x->current = fmaxf(fminf(x->current + next_signal, x->max), x->min);
            
            *out++ = x->current;
            
            x->current = x->current * x->scale; // apply leaky integrator with one-sample delay
        }
        
	} else if (x->mode == ps_wrap) { // saturation at maximum
            
        while (size--) {

            next_signal = *signal++;
            next_control = *control++;
            
            // reset by control signal
            if (next_control == 0.) {
                x->current = x->start;
            }
            
            x->current += next_signal;
            
            // wrap top and bottom boundary
            if(x->current > x->max) {
                x->current = x->min + fmodf(x->current, (x->max - x->min));
            }
            else if(x->current < x->min) {
                x->current = x->max - fmodf(x->min - x->current, (x->max - x->min));
            }
            
            *out++ = x->current;
            
            x->current = x->current * x->scale; // apply leaky integrator
            
        }
        
    } else if (x->mode == ps_reset) {
        
        while (size--) {
            
            next_signal = *signal++;
            next_control = *control++;
            
            // reset by control signal
            if (next_control == 0.) {
                x->current = x->start;
            }
            
            x->current += next_signal;
            
            if(x->current > x->max || x->current < x->min) {
                x->current = x->start;
            }
            
            *out++ = x->current;

            x->current = x->current * x->scale; // apply leaky integrator
        }
        
    }

    return (w+6);
}


