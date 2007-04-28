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
NAME: change~~
DESCRIPTION: Signal to event domain version of change
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's initial version 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/



#include "ext.h"
#include "version.h"
#include "version.c"

#include "z_dsp.h"
#include <math.h>

void *change_class;


typedef struct _change
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    void *outlet;
	float prev;
	float speedlimit;
} t_change;



void main(void);
void *change_new(void);
static void Output(t_change *x);
void change_int(t_change *x, int i);
void change_continuous(t_change *x, long yesno);
void change_free(t_change *x);
void change_dsp(t_change *x, t_signal **sp, short *count);
t_int *change_perform(t_int *w);


void main(void) {
	version(0);	
    setup((t_messlist **)&change_class, (method)change_new, (method)dsp_free, (short)sizeof(t_change),
          0L, 0);
//    addint((method)change_int);
    addmess((method)version, "version", 0);
    addmess((method)change_dsp, "dsp", A_CANT, 0);
//    addmess((method)change_continuous, "continuous", A_LONG, 0);
    dsp_initclass();
}

void *change_new(void) {
	t_change *x;

	x = (t_change *)newobject(change_class);
	
	if (x==0) return 0;

	x->prev = -987654321.;  // So first sample will always be a change
	
  	dsp_setup((t_pxobject *)x,1);  
    x->outlet = floatout(x);
    return x;   
}
	

void change_int(t_change *x, int i) {
}


void change_dsp(t_change *x, t_signal **sp, short *count) {
	dsp_add(change_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);  // in, size
}

int bogus = 0;

t_int *change_perform(t_int *w) {
    t_change *x = (t_change *)(w[1]);  // object
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
