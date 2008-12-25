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
*/

#include "ext.h"
#include "z_dsp.h"

#include "math.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "pthread.h"
#include "sched.h"
#include "unistd.h"

// Distributions
enum _distributions{
	R_GAUSSIAN,
	R_GAUSSIAN_TAIL,
	R_BIVARIATE_GAUSSIAN,
	R_EXPONENTIAL,
	R_ERLANG,
	R_LAPLACE,
	R_EXPPOW,
	R_CAUCHY,
	R_RAYLEIGH,
	R_RAYLEIGH_TAIL,
	R_LANDAU,
	R_LEVY,
	R_LEVY_SKEW,
	R_GAMMA,
	R_UNIFORM,
	R_LOGNORMAL,
	R_CHISQ,
	R_F,
	R_T,
	R_BETA,
	R_LOGISTIC,
	R_PARETO,
	R_WEIBULL,
	R_GUMBEL1,
	R_GUMBEL2,
	R_DIRICHLET,
	R_POISSON,
	R_BERNOULLI,
	R_BINOMIAL,
	R_MULTINOMIAL,
	R_NEGATIVE_BINOMIAL,
	R_PASCAL,
	R_GEOMETRIC,
	R_HYPERGEOMETRIC,
	R_LOGARITHMIC,
	R_USER_DEFINED
};

#define R_MAX_N_VARS 66
#define R_BUFFER_SIZE 16384

typedef struct _rdist{
        t_pxobject r_ob;
        long r_in_0;
        void *r_out0;
	void *r_out1;
        gsl_rng *r_rng;
        t_atom *r_vars;
        int r_dist;
        t_atom *r_arIn;
        float *r_pmf;
        short r_pmfLength;
        float **r_buffers;
	int r_bufferSize;
        int r_bufferPos;
        int r_whichBuffer;
        int r_useBuffer;
        int r_numVars;
	int r_stride;
	int r_threadShouldWait;
	int r_threadShouldExit;
	int r_bufferIsEmpty[2];
	pthread_t r_thread;
	pthread_mutex_t r_mx;
	pthread_cond_t r_cv;
	void (*r_function)(void *xx, float *buffer, int size);
	float r_defVal; // is object is connected, this is the value that will be output if not a random dev.
	int r_sampleAndHold; // if object is connected.
} t_rdist;

void rdist_main(void);
void rdist_init(t_rdist *x, short argc, t_atom *argv);
void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);
void rdist_int(t_rdist *x, long n);
void rdist_float(t_rdist *x, double n);
void rdist_incBufPos(t_rdist *x, int i);
void *rdist_fillBuffers(void *args);
void rdist_seed(t_rdist *x, long s);
int makeseed(void);
void rdist_makePMF(t_rdist *x);
int rdist_randPMF(t_rdist *x);
char *rdist_getDistString(int d);
void rdist_tellmeeverything(t_rdist *x);

extern void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv);

void rdist_gaussian(void *xx, float *buffer, int size);
void rdist_gaussian_tail(void *xx, float *buffer, int size);
void rdist_bivariate_gaussian(void *xx, float *buffer, int size);
void rdist_exponential(void *xx, float *buffer, int size);
void rdist_erlang(void *xx, float *buffer, int size);
void rdist_laplace(void *xx, float *buffer, int size);
void rdist_exppow(void *xx, float *buffer, int size);
void rdist_cauchy(void *xx, float *buffer, int size);
void rdist_rayleigh(void *xx, float *buffer, int size);
void rdist_rayleigh_tail(void *xx, float *buffer, int size);
void rdist_landau(void *xx, float *buffer, int size);
void rdist_levy(void *xx, float *buffer, int size);
void rdist_levy_skew(void *xx, float *buffer, int size);
void rdist_gamma(void *xx, float *buffer, int size);
void rdist_uniform(void *xx, float *buffer, int size);
void rdist_lognormal(void *xx, float *buffer, int size);
void rdist_chisq(void *xx, float *buffer, int size);
void rdist_fdist(void *xx, float *buffer, int size);
void rdist_tdist(void *xx, float *buffer, int size);
void rdist_beta(void *xx, float *buffer, int size);
void rdist_logistic(void *xx, float *buffer, int size);
void rdist_pareto(void *xx, float *buffer, int size);
void rdist_weibull(void *xx, float *buffer, int size);
void rdist_gumbel1(void *xx, float *buffer, int size);
void rdist_gumbel2(void *xx, float *buffer, int size);
void rdist_dirichlet(void *xx, float *buffer, int size);
void rdist_poisson(void *xx, float *buffer, int size);
void rdist_bernoulli(void *xx, float *buffer, int size);
void rdist_binomial(void *xx, float *buffer, int size);
void rdist_multinomial(void *xx, float *buffer, int size);
void rdist_negative_binomial(void *xx, float *buffer, int size);
void rdist_pascal(void *xx, float *buffer, int size);
void rdist_geometric(void *xx, float *buffer, int size);
void rdist_hypergeometric(void *xx, float *buffer, int size);
void rdist_logarithmic(void *xx, float *buffer, int size);
void rdist_user_defined(void *xx, float *buffer, int size);
