/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006-09, The Regents of
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
COPYRIGHT_YEARS: 2006-09
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
VERSION 2.1: Re-re-factored.  Nonparametric now uses gsl_ran_discrete().
VERSION 2.1.1: Fixed a bug that occurred when distlist was used with nonparametric
VERSION 2.1.2: Fixed a bug with the multinomial distribution
VERSION 2.1.3: Added multivariate hypergeometric distribution
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "libranddist.h"
#include "version.h"
#include "version.c"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define RDIST_DEFAULT_BUF_SIZE 1024

typedef struct _rdist{
        t_object r_ob;
        void *r_out0;
	void *r_out1;
        gsl_rng *r_rng; // random number generator
	gsl_ran_discrete_t *r_g; // lookup table for nonparametric pdf sampling
        t_atom r_vars[R_MAX_N_VARS];
        t_symbol *r_dist;
        t_atom *r_arIn;
        short r_pmfLength;
        int r_numVars;
	int r_stride;
	void (*r_function)(gsl_rng*, int, void*, int, float*);
	t_atom *r_output_buffer;
} t_rdist;

void *rdist_class;

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_bang(t_rdist *x);
void rdist_distlist(t_rdist *x, long n);
void rdist_assist(t_rdist *x, void *b, long m, long a, char *s);
void *rdist_new(t_symbol *msg, short argc, t_atom *argv);
void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_int(t_rdist *x, long n);
void rdist_float(t_rdist *x, double n);
void rdist_seed(t_rdist *x, long s);
void rdist_free(t_rdist *x);
void rdist_tellmeeverything(t_rdist *x);
void rdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

int main(void){
	setup((t_messlist **)&rdist_class, (method)rdist_new, (method)rdist_free, (short)sizeof(t_rdist), 0L, A_GIMME, 0); 
	
	addmess((method)version, "version", 0);
	addmess((method)rdist_assist, "assist", A_CANT, 0);
	addbang((method)rdist_bang);
	addmess((method)rdist_distlist, "distlist", A_DEFLONG, 0);
        addint((method)rdist_int);
        addfloat((method)rdist_float);
        addmess((method)rdist_list, "list", A_GIMME, 0);
	addmess((method)rdist_anything, "anything", A_GIMME, 0);
        addmess((method)rdist_nonparametric, "nonparametric", A_GIMME, 0);
        addmess((method)rdist_seed, "seed", A_LONG, 0);
        addmess((method)rdist_tellmeeverything, "tellmeeverything", 0);

	version(0);

	return 0;
}

void *rdist_new(t_symbol *msg, short argc, t_atom *argv){
	t_rdist *x;
	int i;
	t_atom ar[2];

	x = (t_rdist *)newobject(rdist_class); // create a new instance of this object
	
	x->r_out0 = outlet_new(x, 0);

	x->r_numVars = 0;

	// set up the random number generator	
	gsl_rng_env_setup();

	// waterman14 was the fastest according to my tests
	x->r_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_waterman14);
	
	// seed it by reading from /dev/random on mac os x and 
	// something similar on windows
	gsl_rng_set(x->r_rng, makeseed());

	// this is really fucking important.  if there's an error and the gsl's 
	// default handler gets called, it aborts the program!
	gsl_set_error_handler(rdist_errorHandler);  

	// setup a workspace
	x->r_output_buffer = (t_atom *)malloc(RDIST_DEFAULT_BUF_SIZE * sizeof(t_atom));

	// init the lib.  just gensyms all the distribution names
	librdist_init();

	// handle the args.  this should be done with attributes.
	if(argc){
		if(argv[0].a_type == A_SYM){
			rdist_anything(x, argv[0].a_w.w_sym, argc - 1, argv + 1);
		}
	} else {
		SETFLOAT(&(ar[0]), 0.);
		SETFLOAT(&(ar[1]), 1.);
		rdist_anything(x, gensym("uniform"), 2, ar);
	}

	return x;
}

void rdist_bang(t_rdist *x){
	int stride = x->r_stride;
	float out[stride];
	int i;
	t_symbol *msg;

	//msg = (stride > 1) ? gensym("list") : gensym("float");

	//post("buffer: %d, pos: %d, val: %f", x->r_whichBuffer, x->r_bufferPos, out[0].a_w.w_float);
	if(x->r_dist == ps_nonparametric){
		x->r_function(x->r_rng, 0, x->r_g, stride, out);
	}else{
		x->r_function(x->r_rng, x->r_numVars, x->r_vars, stride, out);
	}

	if(stride > 1){
		for(i = 0; i < stride; i++){
			SETFLOAT(x->r_output_buffer + i, out[i]);
		}	
		outlet_list(x->r_out0, NULL, stride, x->r_output_buffer);
	}else{
		outlet_float(x->r_out0, out[0]);
	}
	//outlet_anything(x->r_out0, msg, stride, x->r_output_buffer);
}

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc > R_MAX_N_VARS){
		error("randdist: too many variables");
		return;
	}

	// need to check that this is a valid dist.
	x->r_dist = msg;
	int i;
	if(x->r_dist == gensym("multinomial")){
		double sum = 0;
		for(i = 1; i < argc; i++){
			sum += atom_getfloat(argv + i);
		}
		x->r_vars[0] = argv[0];
		for(i = 1; i < argc; i++){
			atom_setfloat(x->r_vars + i, atom_getfloat(argv + i) / sum);
		}
	}else{
		memcpy(x->r_vars, argv, argc * sizeof(t_atom));
	}

	x->r_numVars = (int)argc;	

	if(x->r_dist == ps_bivariate_gaussian){
		x->r_stride = 2;
	}else if(x->r_dist == ps_dirichlet){
		x->r_stride = x->r_numVars;
	}else if(x->r_dist == ps_multinomial || x->r_dist == ps_multivariate_hypergeometric){
		x->r_stride = x->r_numVars - 1;
	}else{
		x->r_stride = 1;
	}

	x->r_function = librdist_get_function(x->r_dist);
}

void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	double f[argc];
	int i;
	x->r_dist = msg;
	for(i = 0; i < argc; i++){
		f[i] = librdist_atom_getfloat(argv + i);
		//post("%d, %f", i, f[i]);
	}
	if(x->r_g){
		gsl_ran_discrete_free(x->r_g);
	}
	x->r_g = gsl_ran_discrete_preproc(argc, f);
	x->r_function = librdist_nonparametric;
}

void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	rdist_anything(x, x->r_dist, argc, argv);
}

void rdist_int(t_rdist *x, long n){	
	t_atom argv[1];
	SETFLOAT(argv, (double)n);
	rdist_anything(x, x->r_dist, 1, argv);
}

void rdist_float(t_rdist *x, double n){
	t_atom argv[1];
	SETFLOAT(argv, n);
	rdist_anything(x, x->r_dist, 1, argv);
}

void rdist_distlist(t_rdist *x, long n){
	int i, j;
	int stride = x->r_stride;
	float out[n * stride];
	if(stride * n > RDIST_DEFAULT_BUF_SIZE){
		error("randdist: output list is too long (%d > %d)", 
		      n * stride, 
		      RDIST_DEFAULT_BUF_SIZE);
		return;
	}

	if(n < 1){
		error("randdist: distlist argument must be >= 1.");
		return;
	}

	for(i = 0; i < n; i++){
		if(x->r_dist == gensym("nonparametric")){
			x->r_function(x->r_rng, x->r_numVars, x->r_g, stride, out);
		}else{
			x->r_function(x->r_rng, x->r_numVars, x->r_vars, stride, out);
		}
		for(j = 0; j < stride; j++){
			SETFLOAT(x->r_output_buffer + ((i * stride) + j), out[j]);
		}
	}
	
	outlet_anything(x->r_out0, gensym("list"), n * stride, x->r_output_buffer);
}


void rdist_assist(t_rdist *x, void *b, long m, long a, char *s){
	if (m == ASSIST_INLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void rdist_tellmeeverything(t_rdist *x){
	int i;
	float vars[x->r_numVars];
	post("Distribution: %s", x->r_dist->s_name);
	post("Args:");
	for(i = 0; i < x->r_numVars; i++){
		post("\t%f", x->r_vars[i]);
	}
}

void rdist_free(t_rdist *x){
	if(x->r_rng){
		gsl_rng_free(x->r_rng);
	}
	if(x->r_g){
		gsl_ran_discrete_free(x->r_g);
	}
	if(x->r_arIn){
		free(x->r_arIn);
	}
	if(x->r_output_buffer){
		free(x->r_output_buffer);
	}
}

void rdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno){
	error("randdist: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}
