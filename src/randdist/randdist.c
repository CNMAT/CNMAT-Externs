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
VERSION 2.1.4: fixed a bug with the way that multinomial arguments were being handled
VERSION 2.1.5: seed is now an attribute to easily replicate random data
VERSION 2.1.6: multivariate_hypergeometric
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include <stdio.h>
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "jpatcher_api.h"
#include "libranddist.h"
#include "version.h"
#include "version.c"
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define RDIST_DEFAULT_BUF_SIZE 4096

typedef struct _rdist{
        t_object r_ob;
        void *r_out0;
	void *outlet_info;
        gsl_rng *r_rng; // random number generator
	gsl_rng *rng_last;
	gsl_ran_discrete_t *r_g; // lookup table for nonparametric pdf sampling
        t_atom *r_vars;
        t_symbol *r_dist;
        t_atom *r_arIn;
        short r_pmfLength;
        int r_numVars;
	int r_stride;
	void (*r_function)(gsl_rng*, int, void*, int, float*);
	t_atom *r_output_buffer;

	long seed;
	int saveseed;

	char state[128];
	int statelen;
	int savestate;
} t_rdist;

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_init_seed(t_rdist *x, t_symbol *msg, int argc, t_atom *argv);
void rdist_bang(t_rdist *x);
void rdist_distlist(t_rdist *x, long n);
void rdist_setsaveseed(t_rdist *x, t_object *attr, long argc, t_atom *argv);
void rdist_setseed(t_rdist *x, t_object *attr, long argc, t_atom *argv);
void rdist_getseed(t_rdist *x);
void rdist_resetseed(t_rdist *x);
//void rdist_setsaveseed(t_rdist *x, t_object *attr, long argc, t_atom *argv);
void rdist_setsavestate(t_rdist *x, t_object *attr, long argc, t_atom *argv);
void rdist_setstate(t_rdist *x, t_object *attr, int argc, t_atom *argv);
void rdist_rememberstate(t_rdist *x);
void rdist_resetstate(t_rdist *x);
void rdist_assist(t_rdist *x, void *b, long m, long a, char *s);
void *rdist_new(t_symbol *msg, short argc, t_atom *argv);
void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_int(t_rdist *x, long n);
void rdist_float(t_rdist *x, double n);
//void rdist_seed(t_rdist *x, long s);
void rdist_dirtywindow(t_rdist *x);
void rdist_free(t_rdist *x);
void rdist_tellmeeverything(t_rdist *x);
void rdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

void rdist_loadbang(t_rdist *x);

static t_class *rdist_class;

int main(void){
	t_class *c = class_new("randdist", (method)rdist_new, (method)rdist_free, (short)sizeof(t_rdist), 0L, A_GIMME, 0); 
	
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)rdist_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)rdist_bang, "bang", 0);
	class_addmethod(c, (method)rdist_distlist, "distlist", A_DEFLONG, 0);
	class_addmethod(c, (method)rdist_int, "int", A_LONG, 0);
        class_addmethod(c, (method)rdist_float, "float", A_FLOAT, 0);
        class_addmethod(c, (method)rdist_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)rdist_anything, "anything", A_GIMME, 0);
        class_addmethod(c, (method)rdist_nonparametric, "nonparametric", A_GIMME, 0);
        //class_addmethod(c, (method)rdist_setstate, "state", A_GIMME, 0);
        //class_addmethod(c, (method)rdist_getseed, "getseed", 0);
	//class_addmethod(c, (method)rdist_resetseed, "resetseed", 0);
	//class_addmethod(c, (method)rdist_resetstate, "resetstate", 0);
	//class_addmethod(c, (method)rdist_rememberstate, "rememberstate", 0);
        class_addmethod(c, (method)rdist_tellmeeverything, "tellmeeverything", 0);

	class_addmethod(c, (method)rdist_loadbang, "loadbang", 0);
	/*
	CLASS_ATTR_LONG(c, "seed", 0, t_rdist, seed);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "seed", 0, "-666");
	CLASS_ATTR_ACCESSORS(c, "seed", NULL, rdist_setseed); 

	CLASS_ATTR_LONG(c, "saveseed", 0, t_rdist, saveseed);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "saveseed", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "saveseed", 0, "onoff", "Save Seed With Patcher");
	CLASS_ATTR_ACCESSORS(c, "saveseed", NULL, rdist_setsaveseed);

	CLASS_ATTR_CHAR_VARSIZE(c, "state", 0, t_rdist, state, statelen, 128);
	CLASS_ATTR_SAVE(c, "state", 0);
	CLASS_ATTR_INVISIBLE(c, "state", 0);
	CLASS_ATTR_ACCESSORS(c, "state", NULL, rdist_setstate);

	CLASS_ATTR_LONG(c, "savestate", 0, t_rdist, savestate);
	CLASS_ATTR_DEFAULTNAME_SAVE(c, "savestate", 0, "0");
	CLASS_ATTR_STYLE_LABEL(c, "savestate", 0, "onoff", "Save State With Patcher");
	//CLASS_ATTR_ACCESSORS(c, "savestate", NULL, rdist_setsavestate);
	*/
	class_register(CLASS_BOX, c);
	rdist_class = c;

	version(0);

	return 0;
}

void *rdist_new(t_symbol *msg, short argc, t_atom *argv){
	t_rdist *x;
	int i;
	t_atom ar[2];
	srand(mach_absolute_time());

	//x = (t_rdist *)newobject(rdist_class); // create a new instance of this object
	if(x = (t_rdist *)object_alloc(rdist_class)){

		x->outlet_info = outlet_new(x, NULL);	
		x->r_out0 = outlet_new(x, 0);

		x->r_vars = (t_atom *)calloc(R_MAX_N_VARS, sizeof(t_atom));
		x->r_numVars = 0;

		// set up the random number generator	
		gsl_rng_env_setup();

		// waterman14 was the fastest according to my tests
		x->r_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_waterman14);
		x->rng_last = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_waterman14);
		x->seed = makeseed();
		gsl_rng_set(x->r_rng, x->seed);
		gsl_rng_set(x->rng_last, x->seed);
       

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

		attr_args_process(x, argc, argv);

		//schedule_delay(x, (method)rdist_init_seed, 0, NULL, 0, NULL);

		return x;
	}

	return NULL;
}

void rdist_init_seed(t_rdist *x, t_symbol *msg, int argc, t_atom *argv){
	if(x->saveseed == 0){
		//rdist_setseed(x, NULL, 0, NULL);
	}
	if(x->savestate == 1){
		//post("%s", __PRETTY_FUNCTION__);
		//rdist_resetstate(x);
	}
}

void rdist_bang(t_rdist *x){
	if(!(x->r_function)){
		object_error((t_object *)x, "you must specify a distribution!\n");
		return;
	}
	int stride = x->r_stride;
	float out[stride];
	int i;
	t_symbol *msg;

	//msg = (stride > 1) ? gensym("list") : gensym("float");

	gsl_rng_memcpy(x->rng_last, x->r_rng);
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
		object_error((t_object *)x, "too many variables (%d, max is %d)", argc, R_MAX_N_VARS);
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
	if(!(x->r_function)){
		object_error((t_object *)x, "%s is not a distribution that randdist is aware of\n", x->r_dist);
	}
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
		object_error((t_object *)x, "output list is too long (%d > %d)", 
		      n * stride, 
		      RDIST_DEFAULT_BUF_SIZE);
		return;
	}

	if(n < 1){
		object_error((t_object *)x, "distlist argument must be >= 1.");
		return;
	}
	gsl_rng_memcpy(x->rng_last, x->r_rng);

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

void rdist_setsaveseed(t_rdist *x, t_object *attr, long argc, t_atom *argv){
	if(!argc){
		return;
	}
	x->saveseed = atom_getlong(argv);
	t_atom out;
	atom_setlong(&out, x->seed);
	//outlet_anything(x->outlet_info, gensym("seed"), 1, &out);
	rdist_dirtywindow(x);
}

void rdist_setseed(t_rdist *x, t_object *attr, long argc, t_atom *argv){
	long s;
	if(argc){
		s = atom_getlong(argv);
	}else{
		s = makeseed();
	}
	x->seed = s;
	gsl_rng_set(x->r_rng, s);
}

void rdist_getseed(t_rdist *x){
	t_atom s;
	atom_setlong(&s, x->seed);
	outlet_anything(x->outlet_info, gensym("seed"), 1, &s);
}

void rdist_resetseed(t_rdist *x){
	gsl_rng_set(x->r_rng, 0);
	gsl_rng_set(x->r_rng, x->seed);
}

/*
void rdist_setsavestate(t_rdist *x, t_object *attr, long argc, t_atom *argv){
	if(!argc){
		return;
	}
	x->savestate = atom_getlong(argv);
	if(x->savestate != 0){
		char buf[L_tmpnam];
		int n = 0;
		if(tmpnam(buf)){
			FILE *fp = fopen(buf, "w");
			gsl_rng_fwrite(fp, x->r_rng);
			fclose(fp);
			fp = fopen(buf, "r");
			fseek(fp, 0, SEEK_END);
			int len = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			n = fread(x->state, 1, len, fp);
			x->statelen = n;
			fclose(fp);
		}
		rdist_dirtywindow(x);
		t_atom out[n];
		int i;
		for(i = 0; i < n; i++){
			atom_setlong(out + i, x->state[i]);
		}
		outlet_anything(x->outlet_info, gensym("state"), n, out);
	}
}
*/

void rdist_setstate(t_rdist *x, t_object *attr, int argc, t_atom *argv){
	int i;
	for(i = 0; i < argc; i++){
		x->state[i] = (char)atom_getlong(argv + i);
	}
	x->statelen = argc;
	rdist_resetstate(x);
}

void rdist_rememberstate(t_rdist *x){
	char buf[L_tmpnam];
	int n = 0;
	if(tmpnam(buf)){
		FILE *fp = fopen(buf, "w");
		gsl_rng_fwrite(fp, x->rng_last);
		fclose(fp);
		fp = fopen(buf, "r");
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		n = fread(x->state, 1, len, fp);
		x->statelen = n;
		fclose(fp);
	}
	rdist_dirtywindow(x);
	t_atom out[n];
	int i;
	for(i = 0; i < n; i++){
		atom_setlong(out + i, x->state[i]);
	}
	outlet_anything(x->outlet_info, gensym("state"), n, out);
}

void rdist_resetstate(t_rdist *x){
	if(x->statelen){
		char buf[L_tmpnam];
		if(tmpnam(buf)){
			FILE *fp = fopen(buf, "w");
			fwrite(x->state, 1, x->statelen, fp);
			fclose(fp);
			fp = fopen(buf, "r");
			gsl_rng_fread(fp, x->r_rng);
			fclose(fp);
		}
	}else{
		object_error((t_object *)x, "there is no valid state to restore");
		return;
	}
}

void rdist_dirtywindow(t_rdist *x){
	t_object *p;
	object_obex_lookup(x, gensym("#P"), &p);
	jpatcher_set_dirty(p, true);
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
	if(x->r_vars){
		free(x->r_vars);
	}
}

void rdist_errorHandler(const char * reason, const char * file, int line, int gsl_errno){
	error("randdist: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}

void rdist_loadbang(t_rdist *x){
	if(!x->saveseed){
		rdist_setseed(x, NULL, 0, NULL);
		rdist_resetseed(x);
	}
}
