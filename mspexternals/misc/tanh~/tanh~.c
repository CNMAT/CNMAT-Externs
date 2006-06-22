/*

Copyright (c) 1998.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Raymond Wang, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Based on sample code from David Zicarelli.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

DESCRIPTION: hyperbolic tangent soft-clipping function, now part of standard MSP
*/

/* 11/24/98 -- tanh~ */

#define TANH_VERSION "0.0"

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

void *tanh_class;

typedef struct _tanh
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
} t_tanh;    

void main(void);
void *tanh_new(void);
void tanh_dsp(t_tanh *x, t_signal **sp, short *count);
t_int *tanh_perform(t_int *w);

void main(void) {
	post("tanh~ version " TANH_VERSION " by Raymond Wang");
	post("Copyright © 1998 Regents of the University of California.  ");

    setup(&tanh_class, tanh_new, (method)dsp_free, (short)sizeof(t_tanh),
          0L, A_DEFFLOAT, 0);
    addmess((method)tanh_dsp, "dsp", A_CANT, 0);
    dsp_initclass();
}

void *tanh_new(void) {
	t_tanh *x;
	
	x = (t_tanh *)newobject(tanh_class);  // dynamic alloc
  	dsp_setup((t_pxobject *)x,1);  // one inlet
	outlet_new((t_object *)x,"signal");
    return x;
    
}


void tanh_dsp(t_tanh *x, t_signal **sp, short *count) {
			
	dsp_add(tanh_perform, 3, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);  // in, out, size
}


t_int *tanh_perform(t_int *w) {
	t_float val;
    t_float *in = (t_float *)(w[1]);  // input
    t_float *out = (t_float *)(w[2]); // output
    int size = w[3]; // vector size

	// calculation loop
	while (size--) {
		val = *in++;
		*out++ = tanh(val);
	}	

    return (w+4);
}
