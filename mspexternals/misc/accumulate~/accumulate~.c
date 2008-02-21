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
DESCRIPTION: Signal integrator like +=~ with signal control for reset, leakiness and overflow
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision$
VERSION 0.2: New argument syntax
VERSION 0.1: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

#include "ext.h"
#include "z_dsp.h"
#include "version.h"
#include "version.c"

#include <math.h>
#include <float.h>

void *acc_class;

typedef struct _t_acc
{
    t_pxobject x_obj;	//  header
    float state;
    float initialValue;
    float maxValue;
    long maxMode;
    float leakP;
} t_acc;

int main(void);

void *acc_new(t_symbol*, short, t_atom*);
void acc_dsp(t_acc *x, t_signal **sp, short int *count);
t_int *acc_perform(t_int *w);
void acc_free(t_acc *x);

int main(void) {
	post("accumulate~ version " ACCUMULATE_VERSION " by Andy Schmeder");
	post("Copyright © 2007 Regents of the University of California.");

    setup((t_messlist **)&acc_class, (method)acc_new, (method)acc_free,
          (short)sizeof(t_acc), 0L, A_GIMME, 0);
    addmess((method)acc_dsp, "dsp", A_CANT, 0);
    
    dsp_initclass();
    
    return 0;
}

void *acc_new(t_symbol *s, short argc, t_atom *argv) {
	t_acc *x;
    int i;
	
	x = (t_acc *)newobject(acc_class);  // dynamic alloc
  	dsp_setup((t_pxobject *)x, 2);  // two signal inlets
    
    x->state = 0.f;
    x->initialValue = 0.f;
    x->maxValue = MAXFLOAT;
    x->maxMode = 0; // Saturate
    x->leakP = 0.f;
    
	outlet_new((t_object *)x,"signal");

    if(argc > 0) {

        for(i = 0; i < argc; i++) {

            switch (i) {
            
                case 0: // initial value
                    if (argv[i].a_type == A_FLOAT) {
                        x->initialValue = atom_getfloatarg(i,argc,argv);
                        x->state = x->initialValue;
                        //post("initial value: %f", x->initialValue);
                    } else {
                        //post("expected to get a float for argument #1");
                    }
                    break;
                case 1: // max value
                    if (argv[i].a_type == A_FLOAT) {
                        x->maxValue = atom_getfloatarg(i,argc,argv);
                        //post("max value is %f", x->maxValue);
                    } // otherwise just ignore it (inf or whatever)
                    else { 
                        //post("max value inf"); 
                    }
                    break;
                case 2: // max mode
                    if (argv[i].a_type == A_LONG) {
                        x->maxMode = atom_getintarg(i,argc,argv);
                        if(x->maxMode < 0 || x->maxMode > 2) {
                            post("invalid mode! valid modes are 0, 1, 2.");
                        }
                        //post("mode %d", x->maxMode);
                    } else {
                        //post("expected to get an int for argument #3");
                    }
                    break;
                case 3: // leak constant
                    if (argv[i].a_type == A_FLOAT) {
                        x->leakP = 1.f - atom_getfloatarg(i,argc,argv);
                        //post("leakiness %f", x->leakP);
                    } else {
                        //post("expected to get a float for argument #4");
                    }
                    break;
                default:
                    //post("unexpected extra arguments!");
            }
            
        }
        
    } else {
        //post("default operation mode");
    }
	
    return x;
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
    
    if (x->maxMode == 0) { // no maximum.
        
        while (size--) {
        
            next_signal = *signal++;
            next_control = *control++;

            // reset by control signal
            if (next_control == 0.) {
                x->state = x->initialValue;
            }
            
            x->state += next_signal;

            *out++ = x->state;

            x->state = x->state * x->leakP; // apply leaky integrator
        }
        
	} else if (x->maxMode == 1) { // saturation at maximum
            
        while (size--) {

            next_signal = *signal++;
            next_control = *control++;
            
            // reset by control signal
            if (next_control == 0.) {
                x->state = x->initialValue;
            }
            
            x->state += next_signal;
            
            x->state = fminf(x->state, x->maxValue);
            *out++ = x->state;
            
            x->state = x->state * x->leakP; // apply leaky integrator
            
        }
        
    } else if (x->maxMode == 2) {
        
        while (size--) {
            
            next_signal = *signal++;
            next_control = *control++;
            
            // reset by control signal
            if (next_control == 0.) {
                x->state = x->initialValue;
            }
            
            x->state += next_signal;
            
            x->state = fmodf(x->state + next_signal, x->maxValue);
            
            *out++ = x->state;

            x->state = x->state * x->leakP; // apply leaky integrator
        }
        
    }

    return (w+6);
}


