/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2004-06, The Regents of 
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
NAME: 2threshattack~
DESCRIPTION: Two-threshold attack detector, aka a Schmitt Trigger (http://en.wikipedia.org/wiki/Schmitt_trigger)
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2004-06
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version (5/31/4)
VERSION 0.2: UB recompile 
VERSION 0.2.1: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/


#include "ext.h"
#include "version.h"
#include "version.c"

#include "z_dsp.h"



void *tta_class;

typedef struct _tta
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    void *event_outlet;
    
    float low;
    float high;
    int in_attack;
} t_tta;    

void main(void);
void *tta_new(double low, double high);
void tta_dsp(t_tta *x, t_signal **sp, short *count);
t_int *tta_perform(t_int *w);

void tta_set_low(t_tta *x, double low);
void tta_set_high(t_tta *x, double high);
void tta_tellmeeverything(t_tta *x);

void main(void) {
    version(0);
    setup((t_messlist **)&tta_class, (method)tta_new, (method)dsp_free,
          (short)sizeof(t_tta), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);
    addmess((method)version, "version", 0);
    addmess((method)tta_dsp, "dsp", A_CANT, 0);
    addmess((method)tta_set_low, "low", A_FLOAT, 0);
    addmess((method)tta_set_high, "high", A_FLOAT, 0);
    addmess((method)tta_tellmeeverything, "tellmeeverything", 0);
    
    dsp_initclass();
}

void *tta_new(double l, double h) {
	t_tta *x;
	
	// double taua=0, taur=0;	
	
	x = (t_tta *)newobject(tta_class);  // dynamic alloc
  	dsp_setup((t_pxobject *)x,2);  // two inlets
    x->event_outlet = listout (x);	
	outlet_new((t_object *)x,"signal");
	
	// post("Args: low %f, high %f", l, h);
	
	if (l == 0.0) {
		l = 0.25;
	}
	
	if (h == 0.0) {
		h = 0.75;
	}
	
	x->low = l;
	x->high = h;

	x->in_attack = 0;
    return x;
}


void tta_dsp(t_tta *x, t_signal **sp, short *count) {
	dsp_add(tta_perform, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n, x);  
	// in, in, out, size, x
}

#define ABS(x) (((x)>=0) ? (x) : -(x))

t_int *tta_perform(t_int *w) {
	t_float val, l, lowthresh, highthresh, attack;
    t_float *signal = (t_float *)(w[1]);  // input1
    t_float *level = (t_float *)(w[2]);  // input2
    t_float *out = (t_float *)(w[3]); // output
    int size = w[4]; // vector size
    t_tta *x = (t_tta *) w[5];
    int in = x->in_attack;
    Atom outputlist[2];
    
    lowthresh = x->low;
    highthresh = x->high;
	
	// calculation loop
	while (size--) {
		val = *signal++;
		l = *level++;

		attack = 0;  // Probably not an attack this sample
		
		if (in) {
			if (val < (l * lowthresh)) {
				/* Back below low threshold, so reset trigger */
				in = 0;
			}
		} else {
			if (val >= (l * highthresh)) {
				in = 1;
				attack = 1;
				SETFLOAT(outputlist, val);
				SETFLOAT(outputlist+1, l);
				outlet_list (x->event_outlet, 0L, 2, outputlist);
			}
		}
		*out++ = attack;
	}	

	x->in_attack = in;
    return (w+6);
}

void tta_set_low(t_tta *x, double l) {
	x->low = (float) l;
}

void tta_set_high(t_tta *x, double h) {
	x->high = (float) h;
}


void tta_tellmeeverything(t_tta *x) {
	post("low %f, high %f, currently in attack %d",
		x->low, x->high, x->in_attack);
}

