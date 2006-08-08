/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2004, The Regents of 
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
NAME: stpd~
DESCRIPTION: A short-term peak detector (aka leaky peak detector, envelope follower), useful, e.g., as part of a compressor/limiter.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2004,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.11: Matt's initial version 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

#define STPD_VERSION "0.11"

#include "ext.h"
#include "z_dsp.h"

void *stpd_class;

typedef struct _stpd
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    float oneover_taua;
    float oneover_taur;
    float last_output;
} t_stpd;    

void main(void);
void *stpd_new(double taua, double taur);
void stpd_dsp(t_stpd *x, t_signal **sp, short *count);
t_int *stpd_perform(t_int *w);

void stpd_set_taua(t_stpd *x, double taua);
void stpd_set_taur(t_stpd *x, double taur);
void stpd_tellmeeverything(t_stpd *x);

void main(void) {
	post("stpd~ version " STPD_VERSION " by Matt Wright");
	post("Copyright © 2004 Regents of the University of California.");

    setup((t_messlist **)&stpd_class, (method)stpd_new, (method)dsp_free,
          (short)sizeof(t_stpd), 0L, A_DEFFLOAT, A_DEFFLOAT, 0);
    addmess((method)stpd_dsp, "dsp", A_CANT, 0);
    addmess((method)stpd_set_taua, "taua", A_FLOAT, 0);
    addmess((method)stpd_set_taur, "taur", A_FLOAT, 0);
    addmess((method)stpd_tellmeeverything, "tellmeeverything", 0);
    
    dsp_initclass();
}

void *stpd_new(double taua, double taur) {
	t_stpd *x;
	
	// double taua=0, taur=0;	
	
	x = (t_stpd *)newobject(stpd_class);  // dynamic alloc
  	dsp_setup((t_pxobject *)x,1);  // one inlet
	outlet_new((t_object *)x,"signal");
	
	
	// post("Args: taua %f, taur %f", taua, taur);
	
	if (taua == 0.0) {
		taua = 1.0;
	}
	
	if (taur == 0.0) {
		taur = 2000.0;
	}
	
	x->oneover_taua = 1.0f/((float)taua);
	x->oneover_taur = 1.0f/((float)taur);

	x->last_output = 0.0;
    return x;
}


void stpd_dsp(t_stpd *x, t_signal **sp, short *count) {
	dsp_add(stpd_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n, x);  // in, out, size, x
}

#define ABS(x) (((x)>=0) ? (x) : -(x))

t_int *stpd_perform(t_int *w) {
	t_float val, absx, lasty, oneover_taua, oneover_taur;
    t_float *in = (t_float *)(w[1]);  // input
    t_float *out = (t_float *)(w[2]); // output
    int size = w[3]; // vector size
    t_stpd *x = (t_stpd *) w[4];
    
	oneover_taua = x->oneover_taua;
	oneover_taur = x->oneover_taur;
	lasty = x->last_output;
	
	// calculation loop
	while (size--) {
		val = *in++;
		absx = ABS(val);
		
    	if (absx > lasty) {
   		    lasty = lasty + (oneover_taua * (absx - lasty));
  		} else {
	 	    lasty = lasty + (oneover_taur * (absx - lasty));
		}
		
		*out++ = lasty;
	}	

	x->last_output = lasty;
    return (w+5);
}

void stpd_set_taua(t_stpd *x, double taua_d) {
  float taua = (float) tauad;
  x->oneover_taua = 1.0f / taua;
  // post("1/taua %f", x->oneover_taua);
}

void stpd_set_taur(t_stpd *x, double taur_d) {
  float taur = (float) taur_d;
  x->oneover_taur = 1.0f / taur;
  // post("1/taur %f", x->oneover_taur);
	
}

void stpd_tellmeeverything(t_stpd *x) {
	post("last %f, 1/taua %f, 1/taur %f",
		x->last_output, x->oneover_taua, x->oneover_taur);
}

