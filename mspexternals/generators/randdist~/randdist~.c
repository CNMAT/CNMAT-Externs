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
NAME: randdist~
DESCRIPTION: Random number generator with over 30 statistical distributions.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-07
DRUPAL_NODE: /patch/4023
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "libranddist.h"
#include "version.h"
#include "version.c"

void *rdistsig_class;

void rdistsig_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdistsig_assist(t_rdist *x, void *b, long m, long a, char *s);
void *rdistsig_new(t_symbol *msg, short argc, t_atom *argv);
void rdistsig_setDefaultValue(t_rdist *x, float v);
void rdistsig_sampleAndHold(t_rdist *x, int v);
void rdistsig_free(t_rdist *x);
void rdistsig_dsp(t_rdist *x, t_signal **sp, short *count);
t_int *rdistsig_perform(t_int *w);
t_int *rdistsig_perform_connected(t_int *w);

int main(void){
	setup((t_messlist **)&rdistsig_class, (method)rdistsig_new, (method)rdistsig_free, (short)sizeof(t_rdist), 0L, A_GIMME, 0); 
	
	rdist_main();
	addmess((method)version, "version", 0);
	addmess((method)rdistsig_assist, "assist", A_CANT, 0);
	addmess((method)rdistsig_setDefaultValue, "setDefaultValue", A_FLOAT, 0);
	addmess((method)rdistsig_sampleAndHold, "sampleAndHold", A_LONG, 0);
	addmess((method)rdistsig_dsp, "dsp", A_CANT, 0);

	dsp_initclass();

	version(0);

	return 0;
}

//--------------------------------------------------------------------------

void rdistsig_assist(t_rdist *x, void *b, long m, long a, char *s){
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

void *rdistsig_new(t_symbol *msg, short argc, t_atom *argv){
	t_rdist *x;

	x = (t_rdist *)newobject(rdistsig_class); // create a new instance of this object
	
	dsp_setup((t_pxobject *)x, 1);
	outlet_new((t_object *)x, "signal");
	
	// use rand() because the gsl rng isn't initialized until rdist_init() is called below.
	//x->r_bufferSize = (int)(gsl_ran_flat(x->r_rng, R_BUFFER_SIZE, 2 * R_BUFFER_SIZE));
	x->r_bufferSize = R_BUFFER_SIZE + ((rand() / ((double)RAND_MAX + 1)) * R_BUFFER_SIZE);

	x->r_buffers = (float **)calloc(2, sizeof(float *));
	x->r_buffers[0] = (float *)calloc(x->r_bufferSize, sizeof(float));
	x->r_buffers[1] = (float *)calloc(x->r_bufferSize, sizeof(float));

	x->r_useBuffer = 1;
	
	rdist_init(x, argc, argv);

	return(x);
}

void rdistsig_setDefaultValue(t_rdist *x, float v){
        x->r_defVal = v;
}

void rdistsig_sampleAndHold(t_rdist *x, int v){
        x->r_sampleAndHold = v;
}

void rdistsig_free(t_rdist *x){
	x->r_should_refill_buffer = 0;
	dsp_free((t_pxobject *)x);
	gsl_rng_free(x->r_rng);
	free(x->r_buffers);
}

void rdistsig_dsp(t_rdist *x, t_signal **sp, short *count){
	if(count[0])
		dsp_add(rdistsig_perform_connected, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
	else dsp_add(rdistsig_perform, 3, x, sp[1]->s_vec, sp[0]->s_n);
}

t_int *rdistsig_perform(t_int *w){
	t_rdist *x = (t_rdist *)w[1];
	t_float *out = (t_float *)w[2];
	int n = (int)w[3];
	int i;

	for(i = 0; i < n; i++){
		out[i] = x->r_buffers[x->r_whichBuffer][x->r_bufferPos];
		rdist_incBufPos(x);
		//(*x->r_function)(x->r_rng, x->r_vars, x->r_data);
		//out[i] = x->r_data[0];
	}

	return (w + 4);
}

t_int *rdistsig_perform_connected(t_int *w){
	t_rdist *x = (t_rdist *)w[1];
	t_float *in = (t_float *)w[2];
	t_float *out = (t_float *)w[3];
	int n = (int)w[4];
	int i;

	for(i = 0; i < n; i++){
		out[i] = in[i] ? x->r_buffers[x->r_whichBuffer][x->r_bufferPos] : x->r_defVal;
		in[i] ? rdist_incBufPos(x) : NULL;
		x->r_defVal = x->r_sampleAndHold ? out[i] : x->r_defVal;
		//(*x->r_function)(x->r_rng, x->r_vars, x->r_data);
		//out[i] = x->r_data[0];
	}

	return (w + 5);
}
