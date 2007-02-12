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
NAME: randdist
DESCRIPTION: Random number generator with over 30 statistical distributions.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.1: Changed the way the random seed it made
VERSION 1.2: Universal binary
VERSION 1.2.1: Changed the license to be GPL compatible
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"

#include "math.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

// Distributions
#define R_GAUSSIAN 0
#define R_GAUSSIAN_TAIL 1
#define R_BIVARIATE_GAUSSIAN 2
#define R_EXPONENTIAL 3
#define R_LAPLACE 4
#define R_EXPPOW 5
#define R_CAUCHY 6
#define R_RAYLEIGH 7
#define R_RAYLEIGH_TAIL 8
#define R_LANDAU 9
#define R_LEVY 10
#define R_LEVY_SKEW 11
#define R_GAMMA 12
#define R_UNIFORM 13
#define R_LOGNORMAL 14
#define R_CHISQ 15
#define R_F 16
#define R_T 17
#define R_BETA 18
#define R_LOGISTIC 19
#define R_PARETO 20
#define R_WEIBULL 21
#define R_GUMBEL1 22
#define R_GUMBEL2 23
#define R_DIRICHLET 24
#define R_POISSON 25
#define R_BERNOULLI 26
#define R_BINOMIAL 27
#define R_MULTINOMIAL 28
#define R_NEGATIVE_BINOMIAL 29
#define R_PASCAL 30
#define R_GEOMETRIC 31
#define R_HYPERGEOMETRIC 32
#define R_LOGARITHMIC 33
#define R_USER_DEFINED 34

#define R_MAX_N_VARS 66
#define R_BUFFER_SIZE 1024

typedef struct _rdist
{
	t_object r_ob;
	long r_in_0;
	void *r_out0;
	gsl_rng *r_rng;
	double r_vars[R_MAX_N_VARS];
	int r_dist;
	t_atom *r_arIn;
	float *r_pmf;
	short r_pmfLength;
	t_atom **r_buffers;
	int r_bufferPos;
	int r_whichBuffer;
} t_rdist;

void *rdist_class;

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_bang(t_rdist *x);
void rdist_int(t_rdist *x, long n);
void rdist_dump(t_rdist *x, long n);
void rdist_fillBuffers(t_rdist *x, int n);
void rdist_incBufPos(t_rdist *x);
void rdist_assist(t_rdist *x, void *b, long m, long a, char *s);
static int makeseed(void);
void *rdist_new();
void rdist_free(t_rdist *x);

void rdist_dirichlet(t_rdist *x, double *out);
void rdist_multinomial(t_rdist *x, unsigned int *out);

void rdist_makePMF(t_rdist *x);
int rdist_randPMF(t_rdist *x);


//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&rdist_class, (method)rdist_new, (method)rdist_free, (short)sizeof(t_rdist), 0L, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addbang((method)rdist_bang);
	addint((method)rdist_int);
	addmess((method)rdist_list, "list", A_GIMME, 0);
	addmess((method)rdist_anything, "anything", A_GIMME, 0);
	addmess((method)rdist_assist, "assist", A_CANT, 0);
	
	return 0;
}

//--------------------------------------------------------------------------

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv)
{
	int i = 0;
	if(strcmp(msg->s_name, "seed") && strcmp(msg->s_name, "dump")){
		
		if(argc > R_MAX_N_VARS){
			error("randdist: whoa dude, too many variables.");
			return;
		}
		
		for(i = 0; i < argc; i++){
			switch(argv[i].a_type){
				case A_LONG:
					x->r_vars[i] = (double)(argv[i].a_w.w_long);
					break;
				case A_FLOAT:
					x->r_vars[i] = (double)(argv[i].a_w.w_float);
					break;
			}
		}
		for(i = argc; i < R_MAX_N_VARS; i++)
			x->r_vars[i] = 1.0;
	}
	
	if(msg){
		if(!strcmp(msg->s_name, "seed")){
			gsl_rng_set(x->r_rng, argv[0].a_w.w_long);
		}else if(!strcmp(msg->s_name, "dump")){
			rdist_dump(x, argv[0].a_w.w_long);
		}else if(!strcmp(msg->s_name, "gaussian")){
			x->r_dist = R_GAUSSIAN;
		}else if(!strcmp(msg->s_name, "gaussian_tail")){
			x->r_dist = R_GAUSSIAN_TAIL;
		}else if(!strcmp(msg->s_name, "bivariate_gaussian")){
			x->r_dist = R_BIVARIATE_GAUSSIAN;
		}else if(!strcmp(msg->s_name, "exponential")){
			x->r_dist = R_EXPONENTIAL;
		}else if(!strcmp(msg->s_name, "laplace")){
			x->r_dist = R_LAPLACE;
		}else if(!strcmp(msg->s_name, "exppow")){
			x->r_dist = R_EXPPOW;
		}else if(!strcmp(msg->s_name, "cauchy")){
			x->r_dist = R_CAUCHY;
		}else if(!strcmp(msg->s_name, "rayleigh")){
			x->r_dist = R_RAYLEIGH;
		}else if(!strcmp(msg->s_name, "rayleigh_tail")){
			x->r_dist = R_RAYLEIGH_TAIL;
		}else if(!strcmp(msg->s_name, "landau")){
			x->r_dist = R_LANDAU;
		}else if(!strcmp(msg->s_name, "levy")){
			x->r_dist = R_LEVY;
		}else if(!strcmp(msg->s_name, "levy_skew")){
			x->r_dist = R_LEVY_SKEW;
		}else if(!strcmp(msg->s_name, "gamma")){
			x->r_dist = R_GAMMA;
		}else if(!strcmp(msg->s_name, "uniform")){
			x->r_dist = R_UNIFORM;
		}else if(!strcmp(msg->s_name, "lognormal")){
			x->r_dist = R_LOGNORMAL;
		}else if(!strcmp(msg->s_name, "chisq")){
			x->r_dist = R_CHISQ;
		}else if(!strcmp(msg->s_name, "f")){
			x->r_dist = R_F;
		}else if(!strcmp(msg->s_name, "t")){
			x->r_dist = R_T;
		}else if(!strcmp(msg->s_name, "beta")){
			x->r_dist = R_BETA;
		}else if(!strcmp(msg->s_name, "logistic")){
			x->r_dist = R_LOGISTIC;
		}else if(!strcmp(msg->s_name, "pareto")){
			x->r_dist = R_PARETO;
		}else if(!strcmp(msg->s_name, "weibull")){
			x->r_dist = R_WEIBULL;
		}else if(!strcmp(msg->s_name, "gumbel1")){
			x->r_dist = R_GUMBEL1;
		}else if(!strcmp(msg->s_name, "gumbel2")){
			x->r_dist = R_GUMBEL2;
		}else if(!strcmp(msg->s_name, "dirichlet")){
			x->r_dist = R_DIRICHLET;
		}else if(!strcmp(msg->s_name, "poisson")){
			x->r_dist = R_POISSON;
		}else if(!strcmp(msg->s_name, "bernoulli")){
			x->r_dist = R_BERNOULLI;
		}else if(!strcmp(msg->s_name, "binomial")){
			x->r_dist = R_BINOMIAL;
		}else if(!strcmp(msg->s_name, "multinomial")){
			x->r_dist = R_MULTINOMIAL;
		}else if(!strcmp(msg->s_name, "negative_binomial")){
			x->r_dist = R_NEGATIVE_BINOMIAL;
		}else if(!strcmp(msg->s_name, "pascal")){
			x->r_dist = R_PASCAL;
		}else if(!strcmp(msg->s_name, "geometric")){
			x->r_dist = R_GEOMETRIC;
		}else if(!strcmp(msg->s_name, "hypergeometric")){
			x->r_dist = R_HYPERGEOMETRIC;
		}else if(!strcmp(msg->s_name, "logarithmic")){
			x->r_dist = R_LOGARITHMIC;
		}
	}
	rdist_fillBuffers(x, abs(x->r_whichBuffer - 1));
	x->r_whichBuffer = abs(x->r_whichBuffer - 1);
	x->r_bufferPos = 0;
	rdist_fillBuffers(x, abs(x->r_whichBuffer - 1));
}

void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv)
{
	x->r_dist = R_USER_DEFINED;
	x->r_pmfLength = argc / 2;
		
	x->r_arIn = (t_atom *)realloc(x->r_arIn, argc * sizeof(t_atom));
	memcpy(x->r_arIn, argv, argc * sizeof(t_atom));
	rdist_makePMF(x);
	
	rdist_fillBuffers(x, 0);
	rdist_fillBuffers(x, 1);
}

void rdist_bang(t_rdist *x)
{
	t_atom *out;
	int numOut = 1;
	int i;
	t_symbol *msg;
	if(x->r_dist == R_BIVARIATE_GAUSSIAN || x->r_dist == R_DIRICHLET || x->r_dist == R_MULTINOMIAL){
		switch(x->r_dist){
			case R_BIVARIATE_GAUSSIAN:
				numOut = 2;
				break;
			case R_DIRICHLET:
				numOut = x->r_vars[0];
				break;
			case R_MULTINOMIAL:
				numOut = x->r_vars[0];
				break;
		}
	}
	
	out = (t_atom *)calloc(numOut, sizeof(t_atom));
	for(i = 0; i < numOut; i++){
		out[i] = x->r_buffers[x->r_whichBuffer][x->r_bufferPos];
		rdist_incBufPos(x);
	}
	
	if(numOut > 1) msg = gensym("list");
	else if(out[0].a_type == A_LONG) msg = gensym("int");
	else msg = gensym("float");
	
	//post("buffer: %d, pos: %d, val: %f", x->r_whichBuffer, x->r_bufferPos, out[0].a_w.w_float);
	
	outlet_anything(x->r_out0, msg, numOut, out);
	free(out);
}

void rdist_int(t_rdist *x, long n)
{
	
}

void rdist_dump(t_rdist *x, long n)
{
	int i = 0;
	t_atom *out;
	if(n < 1){
		error("randdist: dump argument must be greater than 1.");
		return;
	}
	
	out = (t_atom *)calloc(n, sizeof(t_atom));
	for(i = 0; i < n; i++){
		out[i] = x->r_buffers[x->r_whichBuffer][x->r_bufferPos];
		rdist_incBufPos(x);
	}
	outlet_anything(x->r_out0, gensym("list"), n, out);
	free(out);
}

void rdist_fillBuffers(t_rdist *x, int n)
{
	int i, j, k = 0;
	t_atom tmp;
	double result[2];
	double *diricResult;
	unsigned int *multiResult;

	switch(x->r_dist){
		case -1:
			break;
		case R_GAUSSIAN:
			
			if(x->r_vars[0] < 0){
				error("randdist: standard deviation (sigma) must be positive.");
				return;
			}			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_gaussian(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_GAUSSIAN_TAIL:
			
			if(x->r_vars[1] < 0 || x->r_vars[2] < 0){
				error("randdist: the lower limit (a) and standard deviation (sigma) must both be positive.");
				return;
			}
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_gaussian_tail(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_BIVARIATE_GAUSSIAN:
			
			if(x->r_vars[2] < -1 || x->r_vars[2] > 1){
				error("randdist: the correlation coefficient (rho) must be between -1 and 1.");
				return;
			}
			
			for(i = 0; i < R_BUFFER_SIZE / 2; i++){
				gsl_ran_bivariate_gaussian(x->r_rng, x->r_vars[0], x->r_vars[1], x->r_vars[2], &result[0], &result[1]);
				SETFLOAT(x->r_buffers[n] + (i * 2), result[0]);
				SETFLOAT(x->r_buffers[n] + ((i * 2) + 1), result[1]);
			}
			break;
		case R_EXPONENTIAL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_exponential(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_LAPLACE:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_laplace(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_EXPPOW:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_exppow(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_CAUCHY:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_cauchy(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_RAYLEIGH:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_rayleigh(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_RAYLEIGH_TAIL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_rayleigh_tail(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_LANDAU:
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_landau(x->r_rng));
			}
			break;
		case R_LEVY:
			
			if(x->r_vars[1] <= 0 || x->r_vars[1] > 2){
				error("randdist: the exponent (alpha) must be greater than zero and less than or equal to 2.");
				return;
			}
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_levy(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_LEVY_SKEW:
			
			if(x->r_vars[1] <= 0 || x->r_vars[1] > 2){
				error("randdist: the exponent (alpha) must be greater than zero and less than or equal to 2.");
				return;
			}
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_levy_skew(x->r_rng, x->r_vars[0], x->r_vars[1], x->r_vars[2]));
			}
			break;
		case R_GAMMA:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_gamma(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_UNIFORM:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_flat(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_LOGNORMAL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_lognormal(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_CHISQ:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_chisq(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_F:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_fdist(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_T:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_tdist(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_BETA:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_beta(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_LOGISTIC:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_logistic(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_PARETO:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_pareto(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_WEIBULL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_weibull(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_GUMBEL1:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_gumbel1(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_GUMBEL2:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETFLOAT(x->r_buffers[n] + i, gsl_ran_gumbel2(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_DIRICHLET:
			
			k = (int)x->r_vars[0];
			diricResult = (double *)calloc(k, sizeof(double));
			
			for(i = 0; i < (int)(floor(R_BUFFER_SIZE / k)); i++){
				rdist_dirichlet(x, diricResult);
				for(j = 0; j < k; j++){
					SETFLOAT(x->r_buffers[n] + ((i * (int)k) + j), diricResult[j]);
				}
			}
			for(i = R_BUFFER_SIZE - ((R_BUFFER_SIZE / k) - floor(R_BUFFER_SIZE / k)); i < R_BUFFER_SIZE; i++)
				SETFLOAT(x->r_buffers[n] + i, 0.0);
			
			free(diricResult);
			
			break;
		case R_POISSON:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_poisson(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_BERNOULLI:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_bernoulli(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_BINOMIAL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_binomial(x->r_rng, x->r_vars[0], (int)(x->r_vars[1])));
			}
			break;
		case R_MULTINOMIAL:
			
			k = (int)x->r_vars[0];
			multiResult = (unsigned int *)calloc(k, sizeof(unsigned int));
			
			for(i = 0; i < (int)(floor(R_BUFFER_SIZE / k)); i++){
				rdist_multinomial(x, multiResult);
				for(j = 0; j < k; j++){
					SETFLOAT(x->r_buffers[n] + ((i * (int)k) + j), multiResult[j]);
				}
			}
			for(i = R_BUFFER_SIZE - ((R_BUFFER_SIZE / k) - floor(R_BUFFER_SIZE / k)); i < R_BUFFER_SIZE; i++)
				SETFLOAT(x->r_buffers[n] + i, 0.0);
			
			free(multiResult);
			
			break;
		case R_NEGATIVE_BINOMIAL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_negative_binomial(x->r_rng, x->r_vars[0], x->r_vars[1]));
			}
			break;
		case R_PASCAL:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_pascal(x->r_rng, x->r_vars[0], (int)(x->r_vars[1])));
			}
			break;
		case R_GEOMETRIC:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_geometric(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_HYPERGEOMETRIC:
			
			if((int)(x->r_vars[2]) > (int)(x->r_vars[0]) + (int)(x->r_vars[1])){
				error("randdist: t must be less than or equal to n1 + n2.");
				return;
			}
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_hypergeometric(x->r_rng, (int)(x->r_vars[0]), (int)(x->r_vars[1]), (int)(x->r_vars[2])));
			}
			break;
		case R_LOGARITHMIC:
			
			
			for(i = 0; i < R_BUFFER_SIZE; i++){
				SETLONG(x->r_buffers[n] + i, gsl_ran_logarithmic(x->r_rng, x->r_vars[0]));
			}
			break;
		case R_USER_DEFINED:
			for(i = 0; i < R_BUFFER_SIZE; i++){
				tmp = x->r_arIn[rdist_randPMF(x) * 2];
				switch(tmp.a_type){
					case A_LONG:
						SETLONG(x->r_buffers[n] + i, tmp.a_w.w_long);
						break;
					case A_FLOAT:
						SETFLOAT(x->r_buffers[n] + i, tmp.a_w.w_float);
						break;
				}
			}
			break;
	}
}

//--------------------------------------------------------------------------

void rdist_assist(t_rdist *x, void *b, long m, long a, char *s)
{
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

void rdist_incBufPos(t_rdist *x)
{
	int stride = 1;
	if(x->r_bufferPos == (floor(R_BUFFER_SIZE / stride) * stride) - 1){
		switch(x->r_dist){
			case R_BIVARIATE_GAUSSIAN:
				stride = 2;
				break;
			case R_DIRICHLET:
				stride = x->r_vars[0];
				break;
			case R_MULTINOMIAL:
				stride = x->r_vars[0];
				break;
		}
	
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		x->r_bufferPos = 0;
		rdist_fillBuffers(x, abs(x->r_whichBuffer - 1));
		
		return;
	}
	x->r_bufferPos++;
}

//--------------------------------------------------------------------------

// from PD x_misc.c
// JM made the last 9-digit number a random number.
static int makeseed(void)
{
	int r = rand();
    static unsigned int random_nextseed = 1489853723;
    random_nextseed = random_nextseed * 435898247 + r;//938284287;
    return (random_nextseed & 0x7fffffff);
}

void *rdist_new()
{
	int i = 0;
	t_rdist *x;

	x = (t_rdist *)newobject(rdist_class); // create a new instance of this object
	
	x->r_out0 = outlet_new(x, 0);
	
	x->r_dist = -1;
		
	gsl_rng_env_setup();
	x->r_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_default);
	
	// systime_ms() is a really bad idea since it'll be the same or very close if a bunch
	// are instantiated when the patch opens.
	//gsl_rng_set(x->r_rng, systime_ms());
	
	// makeseed() is from the PD code in x_misc.c
	gsl_rng_set(x->r_rng, makeseed());
	
	x->r_buffers = (t_atom **)calloc(2, sizeof(t_atom *));
	x->r_buffers[0] = (t_atom *)calloc(R_BUFFER_SIZE, sizeof(t_atom));
	x->r_buffers[1] = (t_atom *)calloc(R_BUFFER_SIZE, sizeof(t_atom));
	
	for(i = 0; i < R_BUFFER_SIZE; i++){
		SETFLOAT(x->r_buffers[0] + i, 0.0);
		SETFLOAT(x->r_buffers[1] + i, 0.0);
	}
	
	x->r_whichBuffer = 0;
	x->r_bufferPos = 0;
	   	
	return(x);
}

void rdist_free(t_rdist *x)
{
	gsl_rng_free(x->r_rng);
}

//--------------------------------------------------------------------------

void rdist_dirichlet(t_rdist *x, double *out)
{
	size_t k;
	double *alpha;
	int i;

	k = (size_t)x->r_vars[0];
	alpha = (double *)calloc(k, sizeof(double));

	for(i = 0; i < (int)k; i++){
		alpha[i] = x->r_vars[i + 1];
		out[i] = x->r_vars[i + 1 + k];
	}
		
	gsl_ran_dirichlet(x->r_rng, k, alpha, out);
}

void rdist_multinomial(t_rdist *x, unsigned int *out)
{
	size_t k = (size_t)x->r_vars[0];
	unsigned int N = (unsigned int)x->r_vars[1];
	double *p = (double *)calloc(k, sizeof(double));
	int i;
	for(i = 0; i < k; i++){
		p[i] = x->r_vars[i + 2];
		out[i] = x->r_vars[i + 2 + k];
	}
	
	gsl_ran_multinomial(x->r_rng, k, N, p, out);
}


///////////////////////////////////////////////////////////////////////////////
// User defined PMF

void rdist_makePMF(t_rdist *x)
{
	int i = 0;
	float sum = 0;
	//if(x->r_pmf)
	//	free(x->r_pmf);
		
	//float *pmf = (float *)calloc(x->r_pmfLength, sizeof(float));
	x->r_pmf = (float *)realloc(x->r_pmf, x->r_pmfLength * sizeof(float));
		
	for(i = 0; i < x->r_pmfLength; i++)
		sum = sum + x->r_arIn[(i * 2) + 1].a_w.w_float;
		
	for(i = 0; i < x->r_pmfLength; i++)
		x->r_pmf[i] = x->r_arIn[(i * 2) + 1].a_w.w_float / sum;
	
	//x->r_pmf = pmf;
}

int rdist_randPMF(t_rdist *x)
{
	int lastIndex, i;
	double u, sum;

	lastIndex = (x->r_pmfLength) - 1;
	u = gsl_rng_uniform(x->r_rng);
	i = 0;
	sum = ((float *)(x->r_pmf))[0];
	while((i < lastIndex) && (u > sum)){
		i++;
		sum += ((float *)(x->r_pmf))[i];
	}
	return i;
}
