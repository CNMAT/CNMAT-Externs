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
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

// Distributions
t_symbol *ps_gaussian, *ps_gaussian_tail, *ps_bivariate_gaussian, *ps_exponential, *ps_erlang, *ps_laplace, *ps_exppow, *ps_cauchy, *ps_rayleigh, *ps_rayleigh_tail, *ps_landau, *ps_levy, *ps_levy_skew, *ps_gamma, *ps_uniform, *ps_lognormal, *ps_chisq, *ps_f, *ps_t, *ps_beta, *ps_logistic, *ps_pareto, *ps_weibull, *ps_gumbel1, *ps_gumbel2, *ps_dirichlet, *ps_poisson, *ps_bernoulli, *ps_binomial, *ps_multinomial, *ps_negative_binomial, *ps_pascal, *ps_geometric, *ps_hypergeometric, *ps_logarithmic, *ps_nonparametric;

#define R_MAX_N_VARS 66
#define R_BUFFER_SIZE 16384

void librdist_init(void);
int librdist_makeseed(void);
void *librdist_get_function(t_symbol *dist);

long librdist_getnargs(t_symbol *dist);
float librdist_atom_getfloat(t_atom *a);
long librdist_atom_getlong(t_atom *a);

void librdist_seed(gsl_rng *rng, long s);

void librdist_gaussian(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_gaussian_tail(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_bivariate_gaussian(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_exponential(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_erlang(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_laplace(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_exppow(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_cauchy(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_rayleigh(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_rayleigh_tail(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_landau(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_levy(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_levy_skew(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_gamma(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_uniform(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_lognormal(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_chisq(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_t(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_f(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_beta(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_logistic(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_pareto(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_weibull(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_gumbel1(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_gumbel2(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_dirichlet(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_poisson(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_bernoulli(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_binomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_multinomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_negative_binomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_pascal(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_geometric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_hypergeometric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_logarithmic(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
void librdist_nonparametric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf);
