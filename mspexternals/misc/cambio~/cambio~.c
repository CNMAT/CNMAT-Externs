/*

Copyright (c) 2007.  The Regents of the University of California
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
NAME: cambio~
DESCRIPTION: Signal to event domain version of change: Output a float whenever the input signal's value changes.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version 
VERSION 0.2: Renamed to "cambio~"
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/



#include "ext.h"
#include "version.h"
#include "version.c"

#include "z_dsp.h"
#include <math.h>

void *cambio_class;


typedef struct _cambio
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    void *outlet;
	float prev;
	float speedlimit;
} t_cambio;



void main(void);
void *cambio_new(void);
static void Output(t_cambio *x);
void cambio_int(t_cambio *x, int i);
void cambio_continuous(t_cambio *x, long yesno);
void cambio_free(t_cambio *x);
void cambio_dsp(t_cambio *x, t_signal **sp, short *count);
t_int *cambio_perform(t_int *w);


void main(void) {
	version(0);	
    setup((t_messlist **)&cambio_class, (method)cambio_new, (method)dsp_free, (short)sizeof(t_cambio),
          0L, 0);
//    addint((method)cambio_int);
    addmess((method)version, "version", 0);
    addmess((method)cambio_dsp, "dsp", A_CANT, 0);
//    addmess((method)cambio_continuous, "continuous", A_LONG, 0);
    dsp_initclass();
}

void *cambio_new(void) {
	t_cambio *x;

	x = (t_cambio *)newobject(cambio_class);
	
	if (x==0) return 0;

	x->prev = -987654321.;  // So first sample will always be a change
	
  	dsp_setup((t_pxobject *)x,1);  
    x->outlet = floatout(x);
    return x;   
}
	

void cambio_int(t_cambio *x, int i) {
}


void cambio_dsp(t_cambio *x, t_signal **sp, short *count) {
	dsp_add(cambio_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);  // in, size
}

int bogus = 0;

t_int *cambio_perform(t_int *w) {
    t_cambio *x = (t_cambio *)(w[1]);  // object
    t_float *in = (t_float *)(w[2]); // input
    int size = w[3]; // vector size
	int events_this_vector = 0;
	
	int i;
	float p = x->prev;
	
	for (i = 0; i < size; ++i) {
		if (in[i] != p) {
			if (events_this_vector++ < 3) {
				outlet_float(x->outlet, in[i]);
			}
		}
		p = in[i];
	}

	x->prev = p;
    return (w+4);
}
