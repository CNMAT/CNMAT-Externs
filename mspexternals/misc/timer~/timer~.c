/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006, The Regents of
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
NAME: timer~
DESCRIPTION: outputs the number of samples between clicks
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include "math.h"

typedef struct _tmr
{
	t_pxobject tmr_ob;
	long tmr_counter;
} t_tmr;

void *tmr_class;

void tmr_assist(t_tmr *x, void *b, long m, long a, char *s);
void *tmr_new(void);
t_int *tmr_perform(t_int *w);
void tmr_dsp(t_tmr *x, t_signal **sp, short *count);
void tmr_free(t_tmr *x);


//--------------------------------------------------------------------------

int main(void){
	setup((t_messlist **)&tmr_class, (method)tmr_new, (method)tmr_free, (short)sizeof(t_tmr), 0L, 0);
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)tmr_dsp, "dsp", A_CANT, 0);
	addmess((method)tmr_assist, "assist", A_CANT, 0);
	
	dsp_initclass();
	
	return 0;
}

//--------------------------------------------------------------------------

void tmr_assist(t_tmr *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

//--------------------------------------------------------------------------

void *tmr_new(void){
	t_tmr *x;

	x = (t_tmr *)newobject(tmr_class);
	dsp_setup((t_pxobject *)x, 1);
	outlet_new((t_object *)x, "signal");

	x->tmr_counter = 0;

	return(x);
}

void tmr_free(t_tmr *x){
	dsp_free((t_pxobject *)x);
}

void tmr_dsp(t_tmr *x, t_signal **sp, short *count){
	dsp_add(tmr_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *tmr_perform(t_int *w){
	t_tmr *x = (t_tmr *)w[1];
	t_float *in = (t_float *)w[2];
	t_float *out = (t_float *)w[3];
	int n = (int)w[4];
	int i = 0;

	for(i = 0; i < n; i++){
		if(in[i]){
			out[i] = x->tmr_counter;
			x->tmr_counter = 0;
		} else {
			out[i] = 0;
			++x->tmr_counter;
		}
	}

	return (w + 5);
}
