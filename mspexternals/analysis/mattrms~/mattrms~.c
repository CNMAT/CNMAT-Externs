/*

Copyright (c) 2002.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: mattrms~
DESCRIPTION: RMS energy analyzer with an idiosyncratic control structure
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2002,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version 
VERSION 0.2: has "continuous" mode
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/


#define MATTRMS_VERSION "0.2"

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

void *mattrms_class;


typedef struct _mattrms
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    void *outlet;
    double sumsquared;	// Accumulate sum of squares of samples so far
    long samplesCounted; // How many samples have been counted so far?
    long sampleGoal;     // How many samples are we trying to count?
    int continuous;		 // if true, keep outputting.  If false, just output once per received int
} t_mattrms;



void main(void);
void *mattrms_new(void);
static void Output(t_mattrms *x);
void mattrms_int(t_mattrms *x, int i);
void mattrms_continuous(t_mattrms *x, long yesno);
void mattrms_free(t_mattrms *x);
void mattrms_dsp(t_mattrms *x, t_signal **sp, short *count);
t_int *mattrms_perform(t_int *w);


void main(void) {
	post("mattrms~ version " MATTRMS_VERSION " by Matt Wright");
	post("Copyright © 2002 Regents of the University of California.  ");
	post("  output format:  sum-of-squares num-counted mean-of-squares sqrt-of-mean-of-squares");
	
    setup((t_messlist **)&mattrms_class, (method)mattrms_new, (method)dsp_free, (short)sizeof(t_mattrms),
          0L, 0);
    addint((method)mattrms_int);
    addmess((method)mattrms_dsp, "dsp", A_CANT, 0);
    addmess((method)mattrms_continuous, "continuous", A_LONG, 0);
    dsp_initclass();
}

void *mattrms_new(void) {
	t_mattrms *x;

	x = (t_mattrms *)newobject(mattrms_class);
	
	if (x==0) return 0;
	
	x->sumsquared = 0.0;
	x->samplesCounted = 0;
	x->sampleGoal = 2000000000;
	x->continuous = 1;
	
  	dsp_setup((t_pxobject *)x,1);  
    x->outlet = listout(x);
    return x;   
}

void mattrms_continuous(t_mattrms *x, long yesno) {
	x->continuous = yesno;
	if (x->continuous) {
		post("mattrms~: continuous mode (keep outputting every n samples)");
	} else {
		post("mattrms~: one-shot mode (output only once per received int)");
	}
}

static void Output(t_mattrms *x) {
	Atom outputList[4];
	double meanSquare, rootMeanSquare;
	
	
	meanSquare = x->sumsquared / x->samplesCounted;
	rootMeanSquare = sqrt(meanSquare);
	
	SETFLOAT(outputList+0, ((float) x->sumsquared));
	SETLONG(outputList+1, x->samplesCounted);
	SETFLOAT(outputList+2, ((float) meanSquare));
	SETFLOAT(outputList+3, ((float) rootMeanSquare));
	
	outlet_list(x->outlet, 0L, 4, outputList);
	
	x->sumsquared = 0.0;
	x->samplesCounted = 0;

	if (!(x->continuous)) {
		x->sampleGoal = 2000000000;
	}
}
	

void mattrms_int(t_mattrms *x, int i) {
	if (x->continuous) {
		if (x->samplesCounted > 0) {
			Output(x);
		}
	}
	
	x->samplesCounted = 0;
	x->sampleGoal = i;
}


void mattrms_dsp(t_mattrms *x, t_signal **sp, short *count) {
	dsp_add(mattrms_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);  // in, size
}

int bogus = 0;

t_int *mattrms_perform(t_int *w) {
	t_float val;
    t_mattrms *x = (t_mattrms *)(w[1]);  // object
    t_float *in = (t_float *)(w[2]); // input
    int size = w[3]; // vector size
    
	int g, i, j;
	
	for (i = 0; i < size; ++i) {
		x->sumsquared += (in[i] * in[i]);
		++(x->samplesCounted);
		if (x->samplesCounted == x->sampleGoal) {
			Output(x);
		}
	}
		
    return (w+4);
}
