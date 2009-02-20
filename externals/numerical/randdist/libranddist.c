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

#include "libranddist.h"

void librdist_init(void){
	ps_gaussian = gensym("gaussian");
	ps_gaussian_tail = gensym("gaussian_tail");
	ps_bivariate_gaussian = gensym("bivariate_gaussian");
	ps_exponential = gensym("exponential");
	ps_erlang = gensym("erlang");
	ps_laplace = gensym("laplace");
	ps_exppow = gensym("exppow");
	ps_cauchy = gensym("cauchy");
	ps_rayleigh = gensym("rayleigh");
	ps_rayleigh_tail = gensym("rayleigh_tail");
	ps_landau = gensym("landau");
	ps_levy = gensym("levy");
	ps_levy_skew = gensym("levy_skew");
	ps_gamma = gensym("gamma");
	ps_uniform = gensym("uniform");
	ps_lognormal = gensym("lognormal");
	ps_chisq = gensym("chisq");
	ps_f = gensym("f");
	ps_t = gensym("t");
	ps_beta = gensym("beta");
	ps_logistic = gensym("logistic");
	ps_pareto = gensym("pareto");
	ps_weibull = gensym("weibull");
	ps_gumbel1 = gensym("gumbel1");
	ps_gumbel2 = gensym("gumbel2");
	ps_dirichlet = gensym("dirichlet");
	ps_poisson = gensym("poisson");
	ps_bernoulli = gensym("bernoulli");
	ps_binomial = gensym("binomial");
	ps_multinomial = gensym("multinomial");
	ps_negative_binomial = gensym("negative_binomial");
	ps_pascal = gensym("pascal");
	ps_geometric = gensym("geometric");
	ps_hypergeometric = gensym("hypergeometric");
	ps_logarithmic = gensym("logarithmic");
	ps_nonparametric = gensym("nonparametric");
}

void *librdist_get_function(t_symbol *dist){
	if(dist == ps_gaussian){
		return librdist_gaussian;
	}else if(dist == ps_gaussian_tail){
		return librdist_gaussian_tail;
	}else if(dist == ps_bivariate_gaussian){
		return librdist_bivariate_gaussian;
	}else if(dist == ps_exponential){
		return librdist_exponential;
	}else if(dist == ps_erlang){
		return librdist_erlang;
	}else if(dist == ps_laplace){
		return librdist_laplace;
	}else if(dist == ps_exppow){
		return librdist_exppow;
	}else if(dist == ps_cauchy){
		return librdist_cauchy;
	}else if(dist == ps_rayleigh){
		return librdist_rayleigh;
	}else if(dist == ps_rayleigh_tail){
		return librdist_rayleigh_tail;
	}else if(dist == ps_landau){
		return librdist_landau;
	}else if(dist == ps_levy){
		return librdist_levy;
	}else if(dist == ps_levy_skew){
		return librdist_levy_skew;
	}else if(dist == ps_gamma){
		return librdist_gamma;
	}else if(dist == ps_uniform){
		return librdist_uniform;
	}else if(dist == ps_lognormal){
		return librdist_lognormal;
	}else if(dist == ps_chisq){
		return librdist_chisq;
	}else if(dist == ps_f){
		return librdist_f;
	}else if(dist == ps_t){
		return librdist_t;
	}else if(dist == ps_beta){
		return librdist_beta;
	}else if(dist == ps_logistic){
		return librdist_logistic;
	}else if(dist == ps_pareto){
		return librdist_pareto;
	}else if(dist == ps_weibull){
		return librdist_weibull;
	}else if(dist == ps_gumbel1){
		return librdist_gumbel1;
	}else if(dist == ps_gumbel2){
		return librdist_gumbel2;
	}else if(dist == ps_dirichlet){
		return librdist_dirichlet;
	}else if(dist == ps_poisson){
		return librdist_poisson;
	}else if(dist == ps_bernoulli){
		return librdist_bernoulli;
	}else if(dist == ps_binomial){
		return librdist_binomial;
	}else if(dist == ps_multinomial){
		return librdist_multinomial;
	}else if(dist == ps_negative_binomial){
		return librdist_negative_binomial;
	}else if(dist == ps_pascal){
		return librdist_pascal;
	}else if(dist == ps_geometric){
		return librdist_geometric;
	}else if(dist == ps_hypergeometric){
		return librdist_hypergeometric;
	}else if(dist == ps_logarithmic){
		return librdist_logarithmic;
	}else if(dist == ps_nonparametric){
		return librdist_nonparametric;
	}else{
		return NULL;
	}
}
/*
void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc > R_MAX_N_VARS){
		error("randdist: too many variables");
		return;
	}

	if(x->r_numVars < argc)
		x->r_vars = (t_atom *)realloc(x->r_vars, argc * sizeof(t_atom));
	x->r_vars = (t_atom *)memcpy(x->r_vars, argv, argc * sizeof(t_atom));
	x->r_numVars = (int)argc;	

	x->r_stride = 1;

	if(msg){
		if(!strcmp(msg->s_name, "gaussian")){
			x->r_dist = R_GAUSSIAN;
			x->r_function = rdist_gaussian;
		}else if(!strcmp(msg->s_name, "gaussian_tail")){
			x->r_dist = R_GAUSSIAN_TAIL;
			x->r_function = rdist_gaussian_tail;
		}else if(!strcmp(msg->s_name, "bivariate_gaussian")){
			x->r_dist = R_BIVARIATE_GAUSSIAN;
			x->r_function = rdist_bivariate_gaussian;
			x->r_stride = 2;
		}else if(!strcmp(msg->s_name, "exponential")){
			x->r_dist = R_EXPONENTIAL;
			x->r_function = rdist_exponential;
		}else if(!strcmp(msg->s_name, "erlang")){
			x->r_dist = R_ERLANG;
			x->r_function = rdist_erlang;
		}else if(!strcmp(msg->s_name, "laplace")){
			x->r_dist = R_LAPLACE;
			x->r_function = rdist_laplace;
		}else if(!strcmp(msg->s_name, "exppow")){
			x->r_dist = R_EXPPOW;
			x->r_function = rdist_exppow;
		}else if(!strcmp(msg->s_name, "cauchy")){
			x->r_dist = R_CAUCHY;
			x->r_function = rdist_cauchy;
		}else if(!strcmp(msg->s_name, "rayleigh")){
			x->r_dist = R_RAYLEIGH;
			x->r_function = rdist_rayleigh;
		}else if(!strcmp(msg->s_name, "rayleigh_tail")){
			x->r_dist = R_RAYLEIGH_TAIL;
			x->r_function = rdist_rayleigh_tail;
		}else if(!strcmp(msg->s_name, "landau")){
			x->r_dist = R_LANDAU;
			x->r_function = rdist_landau;
		}else if(!strcmp(msg->s_name, "levy")){
			x->r_dist = R_LEVY;
			x->r_function = rdist_levy;
		}else if(!strcmp(msg->s_name, "levy_skew")){
			x->r_dist = R_LEVY_SKEW;
			x->r_function = rdist_levy_skew;
		}else if(!strcmp(msg->s_name, "gamma")){
			x->r_dist = R_GAMMA;
			x->r_function = rdist_gamma;
		}else if(!strcmp(msg->s_name, "uniform")){
			x->r_dist = R_UNIFORM;
			x->r_function = rdist_uniform;
		}else if(!strcmp(msg->s_name, "lognormal")){
			x->r_dist = R_LOGNORMAL;
			x->r_function = rdist_lognormal;
		}else if(!strcmp(msg->s_name, "chisq")){
			x->r_dist = R_CHISQ;
			x->r_function = rdist_chisq;
		}else if(!strcmp(msg->s_name, "f")){
			x->r_dist = R_F;
			x->r_function = rdist_fdist;
		}else if(!strcmp(msg->s_name, "t")){
			x->r_dist = R_T;
			x->r_function = rdist_tdist;
		}else if(!strcmp(msg->s_name, "beta")){
			x->r_dist = R_BETA;
			x->r_function = rdist_beta;
		}else if(!strcmp(msg->s_name, "logistic")){
			x->r_dist = R_LOGISTIC;
			x->r_function = rdist_logistic;
		}else if(!strcmp(msg->s_name, "pareto")){
			x->r_dist = R_PARETO;
			x->r_function = rdist_pareto;
		}else if(!strcmp(msg->s_name, "weibull")){
			x->r_dist = R_WEIBULL;
			x->r_function = rdist_weibull;
		}else if(!strcmp(msg->s_name, "gumbel1")){
			x->r_dist = R_GUMBEL1;
			x->r_function = rdist_gumbel1;
		}else if(!strcmp(msg->s_name, "gumbel2")){
			x->r_dist = R_GUMBEL2;
			x->r_function = rdist_gumbel2;
		}else if(!strcmp(msg->s_name, "dirichlet")){
			x->r_dist = R_DIRICHLET;
			x->r_function = rdist_dirichlet;
			x->r_stride = x->r_numVars;
		}else if(!strcmp(msg->s_name, "poisson")){
			x->r_dist = R_POISSON;
			x->r_function = rdist_poisson;
		}else if(!strcmp(msg->s_name, "bernoulli")){
			x->r_dist = R_BERNOULLI;
			x->r_function = rdist_bernoulli;
		}else if(!strcmp(msg->s_name, "binomial")){
			x->r_dist = R_BINOMIAL;
			x->r_function = rdist_binomial;
		}else if(!strcmp(msg->s_name, "multinomial")){
			x->r_dist = R_MULTINOMIAL;
			x->r_function = rdist_multinomial;
			x->r_stride = x->r_numVars - 1;
		}else if(!strcmp(msg->s_name, "negative_binomial")){
			x->r_dist = R_NEGATIVE_BINOMIAL;
			x->r_function = rdist_negative_binomial;
		}else if(!strcmp(msg->s_name, "pascal")){
			x->r_dist = R_PASCAL;
			x->r_function = rdist_pascal;
		}else if(!strcmp(msg->s_name, "geometric")){
			x->r_dist = R_GEOMETRIC;
			x->r_function = rdist_geometric;
		}else if(!strcmp(msg->s_name, "hypergeometric")){
			x->r_dist = R_HYPERGEOMETRIC;
			x->r_function = rdist_hypergeometric;
		}else if(!strcmp(msg->s_name, "logarithmic")){
			x->r_dist = R_LOGARITHMIC;
			x->r_function = rdist_logarithmic;
		}
	} else {
		if(x->r_dist == R_BIVARIATE_GAUSSIAN)
			x->r_stride = 2;
		else if(x->r_dist == R_DIRICHLET || x->r_dist == R_MULTINOMIAL)
			x->r_stride = x->r_numVars;
	}

        if(x->r_useBuffer){
		x->r_threadShouldWait = 1;
		x->r_bufferIsEmpty[0] = x->r_bufferIsEmpty[1] = 1;
		pthread_cond_signal(&x->r_cv);
        }
}
*/

//void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	/*
	x->r_dist = R_USER_DEFINED;
	x->r_pmfLength = argc / 2;
		
	x->r_arIn = (t_atom *)realloc(x->r_arIn, argc * sizeof(t_atom));
	memcpy(x->r_arIn, argv, argc * sizeof(t_atom));
	rdist_makePMF(x);
	x->r_function = (*rdist_user_defined);
	*/
	//error("randdist: fillbuffer block is commented out!");
	/*
	x->r_threadShouldWait = 1;
	if(x->r_useBuffer){
		x->r_threadShouldWait = 1;
		rdist_fillBuffers((void *)x);
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		x->r_bufferPos = 0;
		rdist_fillBuffers((void *)x);
	}
	*/
//}
/*
void rdist_incBufPos(t_rdist *x, int i){
	if(x->r_bufferPos + i >= x->r_bufferSize){
		x->r_bufferIsEmpty[x->r_whichBuffer] = 1;
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		x->r_bufferPos = 0;
		pthread_cond_signal(&x->r_cv);
		//pthread_create(&th, NULL, rdist_fillBuffers, (void *)x);
		//pthread_detach(th);
		//rdist_fillBuffers((void *)x);
		//rdist_fillBuffers(x, x->r_bufferSize, x->r_buffers[abs(x->r_whichBuffer - 1)]);

		return;
	}
	x->r_bufferPos = (x->r_bufferPos + i) % x->r_bufferSize;
	//x->r_bufferPos++;
}
*/
/*
//void rdist_fillBuffers(t_rdist *x, int n, float *buffer){
void *rdist_fillBuffers(void *args){
	t_rdist *x = (t_rdist *)args;

	int i, j;
	i = j = 0;
	int b = abs(x->r_whichBuffer - 1);
	int cont = x->r_threadShouldWait;
	void (*f)(void *x, float *buffer, int size) = x->r_function;
	int stride = x->r_stride;
	int bufferSize = x->r_bufferSize;
	float *buffer = x->r_buffers[b];
	//while(i < x->r_bufferSize){
		//if(!cont) return NULL;
	post("filling buffer %p with %d elements", buffer, bufferSize);
		(*f)((void *)x, buffer, bufferSize);
		//for(j = 0; j < x->r_stride && i < x->r_bufferSize; j++){
		//x->r_buffers[b][i] = x->r_data[j];
		//++i;
		//}
		//}
	return NULL;
}
*/
/*
void *rdist_fillBuffers(void *args){
	t_rdist *x = (t_rdist *)args;
	int whichBuffer = x->r_whichBuffer;
	int stride;
	int bufferSize;
	float *buf;
	int i;

	while(1){
		pthread_cond_wait(&x->r_cv, &x->r_mx);
		if(x->r_threadShouldExit) break;
		void (*f)(void *x, float *buffer, int size) = x->r_function;
		bufferSize = x->r_bufferSize;
		buf = x->r_buffers[whichBuffer];
		for(i = 0; i < 2; i++){
			if(!x->r_threadShouldWait) pthread_cond_wait(&x->r_cv, &x->r_mx);
			if(x->r_bufferIsEmpty[i]){
				whichBuffer = i;
				sched_yield();
				(*f)(args, buf, bufferSize);
			}
		}
	}
	pthread_exit(NULL);
}
*/
void rdist_seed(gsl_rng *rng, long s){
	gsl_rng_set(rng, s);
}

int makeseed(void){
	/* from PD x_misc.c
	int r = rand();
    	static unsigned int random_nextseed = 1489853723;
    	random_nextseed = random_nextseed * 435898247 + 938284287;
    	return (random_nextseed & 0x7fffffff);
	*/

	unsigned int r;
	FILE *f;

#ifdef Windows
#else
	f = fopen("/dev/random", "r");
#endif
	fread(&r, sizeof(r), 1, f);
	fclose(f);

	//post("%u\n", randval);
	return r;
}

///////////////////////////////////////////////////////////////////////////////
// User defined PMF
/*
void rdist_makePMF(t_rdist *x){
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

int rdist_randPMF(t_rdist *x){
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
*/
long librdist_getnargs(t_symbol *dist){
	if(dist == ps_landau){
		return 0;
	}else if(dist == ps_gaussian || dist == ps_exponential || dist == ps_laplace || dist == ps_cauchy || dist == ps_rayleigh || dist == ps_chisq || dist == ps_t || dist == ps_logistic || dist == ps_poisson || dist == ps_bernoulli || dist == ps_geometric || dist == ps_logarithmic){
		return 1;
	}else if(dist == ps_gaussian_tail || dist == ps_erlang || dist == ps_exppow || dist == ps_rayleigh_tail || dist == ps_levy || dist == ps_gamma || dist == ps_uniform || dist == ps_lognormal || dist == ps_f || dist == ps_beta || dist == ps_pareto || dist == ps_weibull || dist == ps_gumbel1 || dist == ps_gumbel2 || dist == ps_binomial || dist == ps_negative_binomial || dist == ps_pascal){
		return 2;
	}else if(dist == ps_bivariate_gaussian || dist == ps_levy_skew || dist == ps_hypergeometric){
		return 3;
	}else{
		return -1;
	}
}

float librdist_atom_getfloat(t_atom *a){
	if(a->a_type == A_FLOAT){
		return a->a_w.w_float;
	}else if(a->a_type == A_LONG){
		return (float)(a->a_w.w_long);
	}else{
		return 0;
	}
}

long librdist_atom_getlong(t_atom *a){
	if(a->a_type == A_LONG){
		return a->a_w.w_long;
	}else if(a->a_type == A_FLOAT){
		return (float)(a->a_w.w_float);
	}else{
		return 0;
	}
}

void librdist_gaussian(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_gaussian)){
		return;
	}
	const double sigma = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
		buf[i] = (float)gsl_ran_gaussian_ratio_method(rng, sigma);
}

void librdist_gaussian_tail(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_gaussian_tail)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double sigma = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
		buf[i] = (float)gsl_ran_gaussian_tail(rng, a, sigma);
}

void librdist_bivariate_gaussian(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_bivariate_gaussian)){
		return;
	}
	const double sigma_x = librdist_atom_getfloat(av);
	const double sigma_y = librdist_atom_getfloat(av + 1);
	const double rho = librdist_atom_getfloat(av + 2);
	double _x, _y;
	gsl_ran_bivariate_gaussian(rng, sigma_x, sigma_y, rho, &_x, &_y);
	int i;
	for(i = 0; i < bufc / 2; i += 2){
		buf[i] = _x;
		buf[i + 1] = _y;
	}
}

void librdist_exponential(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_exponential)){
		return;
	}
	const double mu = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
		buf[i] = (float)gsl_ran_exponential(rng, mu);
}

void librdist_erlang(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_erlang)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double n = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	     	buf[i] = (float)gsl_ran_erlang(rng, a, n);
}

void librdist_laplace(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_laplace)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
		buf[i] = (float)gsl_ran_laplace(rng, a);
}

void librdist_exppow(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_exppow)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_exppow(rng, a, b);
}

void librdist_cauchy(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_cauchy)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_cauchy(rng, a);
}

void librdist_rayleigh(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_rayleigh)){
		return;
	}
	const double sigma = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_rayleigh(rng, sigma);
}

void librdist_rayleigh_tail(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_rayleigh_tail)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double sigma = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_rayleigh_tail(rng, a, sigma);
}

void librdist_landau(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_landau)){
		return;
	}
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_landau(rng);
}

void librdist_levy(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_levy)){
		return;
	}
	const double c = librdist_atom_getfloat(av);
	const double alpha = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_levy(rng, c, alpha);
}

void librdist_levy_skew(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_levy_skew)){
		return;
	}
	const double c = librdist_atom_getfloat(av);
	const double alpha = librdist_atom_getfloat(av + 1);
	const double beta = librdist_atom_getfloat(av + 2);
	int i;
	for(i = 0; i < bufc; i++)
		buf[i] = (float)gsl_ran_levy_skew(rng, c, alpha, beta);
}

void librdist_gamma(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_gamma)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_gamma(rng, a, b);
}

void librdist_uniform(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_uniform)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_flat(rng, a, b);
}

void librdist_lognormal(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_lognormal)){
		return;
	}
	const double zeta = librdist_atom_getfloat(av);
	const double sigma = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_lognormal(rng, zeta, sigma);
}

void librdist_chisq(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_chisq)){
		return;
	}
	const double nu = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_chisq(rng, nu);
}

void librdist_f(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_f)){
		return;
	}
	const double nu1 = librdist_atom_getfloat(av);
	const double nu2 = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_fdist(rng, nu1, nu2);
}

void librdist_t(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_t)){
		return;
	}
	const double nu = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_tdist(rng, nu);
}

void librdist_beta(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_beta)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_beta(rng, a, b);
}

void librdist_logistic(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_logistic)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_logistic(rng, a);
}

void librdist_pareto(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_pareto)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_pareto(rng, a, b);
}

void librdist_weibull(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_weibull)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_weibull(rng, a, b);
}

void librdist_gumbel1(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_gumbel1)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_gumbel1(rng, a, b);
}

void librdist_gumbel2(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_gumbel2)){
		return;
	}
	const double a = librdist_atom_getfloat(av);
	const double b = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_gumbel2(rng, a, b);
}

void librdist_dirichlet(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
/*
	if(argc != librdist_getnargs(ps_dirichlet)){
		return;
	}
	int i, j;
	bufc_t k = x->r_numVars;
	double alpha[x->r_numVars];
	double theta[x->r_numVars];
	for(i = 0; i < k; i++)
		alpha[i] = x->r_vars[i].a_type == A_LONG ? (double)x->r_vars[i].a_w.w_long : x->r_vars[i].a_w.w_float;

	for(j = 0; j < floor(bufc / k); j++){
		gsl_ran_dirichlet(rng, k, alpha, theta);
		for(i = 0; i < k; i++)
			buf[i] = theta[i];
	}
*/
}

void librdist_poisson(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_poisson)){
		return;
	}
	const double mu = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_poisson(rng, mu);
}

void librdist_bernoulli(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_bernoulli)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_bernoulli(rng, p);
}

void librdist_binomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_binomial)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	const unsigned int n = (unsigned int)librdist_atom_getlong(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_binomial(rng, p, n);
}

void librdist_multinomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
/*
	if(argc != librdist_getnargs(ps_multinomial)){
		return;
	}
	int i, j;
	bufc_t K = x->r_numVars - 1;
	unsigned int N = x->r_vars[0].a_type == A_LONG ? (unsigned int)x->r_vars[0].a_w.w_long : (unsigned int)x->r_vars[0].a_w.w_float;
	double p[K];
	unsigned int n[K];
	for(i = 0; i < K; i++)
		p[i] = x->r_vars[i + 1].a_type == A_LONG ? (unsigned int)x->r_vars[i + 1].a_w.w_long : (unsigned int)x->r_vars[i + 1].a_w.w_float;

	for(j = 0; j < floor(bufc / K); j++){
		gsl_ran_multinomial(rng, K, N, p, n);
		for(i = 0; i < K; i++)
			buf[i] = (float)n[i];
	}
*/
}

void librdist_negative_binomial(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_negative_binomial)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	const double n = librdist_atom_getfloat(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_negative_binomial(rng, p, n);
}

void librdist_pascal(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_pascal)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	const unsigned int n = (unsigned int)librdist_atom_getlong(av + 1);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_pascal(rng, p, n);
}

void librdist_geometric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_geometric)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_geometric(rng, p);
}

void librdist_hypergeometric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_hypergeometric)){
		return;
	}
	const unsigned int n1 = (unsigned int)librdist_atom_getlong(av);
	const unsigned int n2 = (unsigned int)librdist_atom_getlong(av + 1);
	const unsigned int t = (unsigned int)librdist_atom_getlong(av + 2);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_hypergeometric(rng, n1, n2, t);
}

void librdist_logarithmic(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	t_atom *av = (t_atom *)argv;
	if(argc != librdist_getnargs(ps_logarithmic)){
		return;
	}
	const double p = librdist_atom_getfloat(av);
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = (float)gsl_ran_logarithmic(rng, p);
}

void librdist_nonparametric(gsl_rng *rng, int argc, void *argv, int bufc, float *buf){
	gsl_ran_discrete_t *g = (gsl_ran_discrete_t *)argv;
	int i;
	for(i = 0; i < bufc; i++)
	       	buf[i] = gsl_ran_discrete(rng, g);
}
