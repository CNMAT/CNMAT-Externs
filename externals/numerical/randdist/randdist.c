/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006-07, The Regents of
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
NAME: randdist
DESCRIPTION: Random number generator with over 30 statistical distributions.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-07
DRUPAL_NODE: /patch/4023
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.1: Changed the way the random seed it made
VERSION 1.2: Universal binary
VERSION 1.2.1: Changed the license to be GPL compatible
VERSION 1.3: The object now takes arguments to specify the distribution
VERSION 1.3.1: Use of the buffering system is now optional and off by default
VERSION 1.3.2: Fixed a bug that would cause a crash if randdist was instantiated with a number instead of a symbol as its first arg.
VERSION 1.3.3: Added Gaussdist faker to helpfile, bump version to re-release. -mzed
VERSION 1.4: Default state, changed dump to distlist, user-defined pmfs are now done with the nonparametric message, bugfixes.
VERSION 2.0: Rewritten with common code between randdist and randdist~ put in libranddist.h/c
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "libranddist.h"
#include "version.h"
#include "version.c"

void *rdist_class;

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_bang(t_rdist *x);
void rdist_distlist(t_rdist *x, long n);
void rdist_useBuffer(t_rdist *x, long b);
void rdist_incBufPos(t_rdist *x);
void rdist_assist(t_rdist *x, void *b, long m, long a, char *s);
void *rdist_new(t_symbol *msg, short argc, t_atom *argv);
void rdist_free(t_rdist *x);

int main(void)
{
	setup((t_messlist **)&rdist_class, (method)rdist_new, (method)rdist_free, (short)sizeof(t_rdist), 0L, A_GIMME, 0); 
	
	rdist_main();
	addmess((method)version, "version", 0);
	addmess((method)rdist_assist, "assist", A_CANT, 0);
	addbang((method)rdist_bang);
	addmess((method)rdist_useBuffer, "useBuffer", A_DEFLONG, 0);
	addmess((method)rdist_distlist, "distlist", A_DEFLONG, 0);

	version(0);

	return 0;
}

//--------------------------------------------------------------------------

void rdist_bang(t_rdist *x){
	t_atom *out;
	int i;
	t_symbol *msg;

	out = (t_atom *)calloc(x->r_stride, sizeof(t_atom));
	
	if(x->r_useBuffer){
		for(i = 0; i < x->r_stride; i++){
			SETFLOAT(out + i, x->r_buffers[x->r_whichBuffer][x->r_bufferPos]);
			rdist_incBufPos(x);
		}
	} else {
		(*x->r_function)((void *)x);
		for(i = 0; i < x->r_stride; i++)
			SETFLOAT(out + i, x->r_data[i]);
	}

	msg = (x->r_stride > 1) ? gensym("list") : gensym("float");

	//post("buffer: %d, pos: %d, val: %f", x->r_whichBuffer, x->r_bufferPos, out[0].a_w.w_float);
	
	outlet_anything(x->r_out0, msg, x->r_stride, out);
	free(out);
}

void rdist_distlist(t_rdist *x, long n){
	int i, j;
	t_atom *out;
	if(n < 1){
		error("randdist: distlist argument must be greater than 1.");
		return;
	}

	out = (t_atom *)calloc(n * x->r_stride, sizeof(t_atom));
	
	if(x->r_useBuffer){
		for(i = 0; i < n; i++){
			for(j = 0; j < x->r_stride; j++){
				SETFLOAT(out + ((i * x->r_stride) + j), x->r_buffers[x->r_whichBuffer][x->r_bufferPos]);
				rdist_incBufPos(x);
			}
		}
	}else {
		for(i = 0; i < n; i++){
			(*x->r_function)((void *)x);
			for(j = 0; j < x->r_stride; j++)
				SETFLOAT(out + ((i * x->r_stride) + j), x->r_data[j]);
		}
	}
	
	outlet_anything(x->r_out0, gensym("list"), n * x->r_stride, out);
	free(out);
}


void rdist_assist(t_rdist *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void rdist_useBuffer(t_rdist *x, long b){
	x->r_useBuffer = b;
	x->r_should_refill_buffer = 0;
	if(b){
		if(x->r_buffers == NULL){
			x->r_buffers = (float **)calloc(2, sizeof(float *));
			x->r_buffers[0] = (float *)calloc(R_BUFFER_SIZE, sizeof(float));
			x->r_buffers[1] = (float *)calloc(R_BUFFER_SIZE, sizeof(float));
		}
		x->r_should_refill_buffer = 1;
		rdist_fillBuffers((void *)x);
		x->r_bufferPos = 0;
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		rdist_fillBuffers((void *)x);
	}
}

void *rdist_new(t_symbol *msg, short argc, t_atom *argv){
	t_rdist *x;

	x = (t_rdist *)newobject(rdist_class); // create a new instance of this object
	
	x->r_out0 = outlet_new(x, 0);
	
	x->r_bufferSize = R_BUFFER_SIZE;
	x->r_buffers = NULL;

	x->r_useBuffer = 0;

	rdist_init(x, argc, argv);
	return(x);
}

void rdist_free(t_rdist *x){
	//gsl_rng_free(x->r_rng);
}
