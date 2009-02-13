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
NAME: vsnapshot~
DESCRIPTION: Like snapshot~ but outputs the entire current signal vector.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
VERSION 1.0.1: Fixed a memory bug
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include "math.h"

#define VSHT_MAX_VSIZE 4096

typedef struct _vsht
{
	t_pxobject v_ob;
	void *outlet;
	t_atom *v_currentVector;
	int v_vectorSize;
	void *v_clock;
	int v_interval;
	int v_running;
} t_vsht;

void *vsht_class;

void vsht_assist(t_vsht *x, void *b, long m, long a, char *s);
void *vsht_new(long t);
void vsht_start(t_vsht *x);
void vsht_stop(t_vsht *x);
void vsht_int(t_vsht *x, long i);
void vsht_in1(t_vsht *x, long i);
void vsht_bang(t_vsht *x);
t_int *vsht_perform(t_int *w);
void vsht_dsp(t_vsht *x, t_signal **sp, short *count);
void vsht_free(t_vsht *x);


//--------------------------------------------------------------------------

int main(void){
	setup((t_messlist **)&vsht_class, (method)vsht_new, (method)vsht_free, (short)sizeof(t_vsht), 0L, A_DEFLONG, 0);
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)vsht_dsp, "dsp", A_CANT, 0);
	addmess((method)vsht_assist, "assist", A_CANT, 0);
	addbang((method)vsht_bang);
	addinx((method)vsht_in1, 1);
	addmess((method)vsht_start, "start", 0);
	addmess((method)vsht_stop, "stop", 0);
	addint((method)vsht_int);
	
	dsp_initclass();
	
	return 0;
}

//--------------------------------------------------------------------------

void vsht_assist(t_vsht *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

//--------------------------------------------------------------------------

void *vsht_new(long t){
	t_vsht *x;

	x = (t_vsht *)newobject(vsht_class);
	intin(x, 1);
	dsp_setup((t_pxobject *)x, 1);
	x->outlet = listout((t_pxobject *)x);

	x->v_clock = clock_new(x, (method)vsht_bang);
	x->v_running = 0;
	x->v_interval = 0;

	x->v_currentVector = (t_atom *)calloc(VSHT_MAX_VSIZE, sizeof(t_atom));

	if(t > 0){
		x->v_interval = t;
		x->v_running = 1;
		clock_delay(x->v_clock, x->v_interval);
	}

	float foo[t * sizeof(float)];
	post("%d", sizeof(foo));
	return(x);
}

void vsht_start(t_vsht *x){
	if(x->v_interval > 0){
		x->v_running = 1;
		clock_delay(x->v_clock, 0);
	}
}

void vsht_stop(t_vsht *x){
	x->v_running = 0;
	clock_unset(x->v_clock);
}

void vsht_int(t_vsht *x, long i){
	x->v_running = (int)i;
	if(i == 0) vsht_stop(x);
	else vsht_start(x);
}

void vsht_in1(t_vsht *x, long i){
	x->v_interval = (int)i;
	vsht_start(x);
}

void vsht_bang(t_vsht *x){
	if(sys_getdspstate())
		outlet_list(x->outlet, 0L, x->v_vectorSize, x->v_currentVector);
	if(x->v_running && x->v_interval > 0) clock_delay(x->v_clock, x->v_interval);
}

void vsht_free(t_vsht *x){
	dsp_free((t_pxobject *)x);
	freeobject(x->v_clock);
}

void vsht_dsp(t_vsht *x, t_signal **sp, short *count){
	dsp_add(vsht_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *vsht_perform(t_int *w
	t_vsht *x = (t_vsht *)w[1];
	t_float *in = (t_float *)w[2];
	int n = (int)w[3];
	int i = 0;

	x->v_vectorSize = n;

	for(i = 0; i < n; i++){
		SETFLOAT(x->v_currentVector + i, in[i]);
	}

	return (w + 4);
}
