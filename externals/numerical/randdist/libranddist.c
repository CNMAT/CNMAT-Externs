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

void rdist_main(void){
        addint((method)rdist_int);
        addfloat((method)rdist_float);
        addmess((method)rdist_list, "list", A_GIMME, 0);
	addmess((method)rdist_anything, "anything", A_GIMME, 0);
        addmess((method)rdist_nonparametric, "nonparametric", A_GIMME, 0);
        addmess((method)rdist_seed, "seed", A_LONG, 0);
        addmess((method)rdist_tellmeeverything, "tellmeeverything", 0);
}

void rdist_init(t_rdist *x, short argc, t_atom *argv){
	int i;
	x->r_numVars = 0;
	
	gsl_rng_env_setup();
	x->r_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_taus);
	
	// makeseed() is from the PD code in x_misc.c
	gsl_rng_set(x->r_rng, makeseed());

	x->r_whichBuffer = 0;
	x->r_bufferPos = 0;

	x->r_vars = (t_atom *)calloc(R_MAX_N_VARS, sizeof(t_atom));
	x->r_data = (float *)calloc(x->r_stride, sizeof(float));

	if(argc){
		if(argv[0].a_type == A_SYM){
			t_symbol *m = argv[0].a_w.w_sym;
			for(i = 0; i < argc - 1; i++)
				argv[i] = argv[i + 1];
			rdist_anything(x, m, argc - 1, argv);
		}
	} else {
		t_atom *ar = (t_atom *)calloc(2, sizeof(t_atom));
		SETFLOAT(ar, 0.);
		SETFLOAT(ar + 1, 1.);
		rdist_anything(x, gensym("uniform"), 2, ar);
		free(ar);
	}

	x->r_should_refill_buffer = 1;
}

void rdist_anything(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc > R_MAX_N_VARS){
		error("randdist: too many variables");
		return;
	}

	x->r_should_refill_buffer = 0;

	if(x->r_numVars < argc)
		x->r_vars = (t_atom *)realloc(x->r_vars, argc * sizeof(t_atom));
	x->r_vars = (t_atom *)memcpy(x->r_vars, argv, argc * sizeof(t_atom));
	x->r_numVars = (int)argc;	

	int oldStride = x->r_stride;
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
	if(oldStride != x->r_stride)
		x->r_data = (float *)realloc(x->r_data, x->r_stride);

        if(x->r_useBuffer){
                x->r_should_refill_buffer = 1;
                rdist_fillBuffers((void *)x);
                x->r_whichBuffer = abs(x->r_whichBuffer - 1);
                x->r_bufferPos = 0;
                rdist_fillBuffers((void *)x);
        }

}

void rdist_list(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	rdist_anything(x, gensym(rdist_getDistString(x->r_dist)), argc, argv);
	
}

void rdist_int(t_rdist *x, long n){	
	t_atom argv[1];
	SETFLOAT(argv, (double)n);
	rdist_anything(x, gensym(rdist_getDistString(x->r_dist)), 1, argv);
}

void rdist_float(t_rdist *x, double n){
	t_atom argv[1];
	SETFLOAT(argv, n);
	rdist_anything(x, gensym(rdist_getDistString(x->r_dist)), 1, argv);
}

void rdist_nonparametric(t_rdist *x, t_symbol *msg, short argc, t_atom *argv){
	x->r_dist = R_USER_DEFINED;
	x->r_pmfLength = argc / 2;
		
	x->r_arIn = (t_atom *)realloc(x->r_arIn, argc * sizeof(t_atom));
	memcpy(x->r_arIn, argv, argc * sizeof(t_atom));
	rdist_makePMF(x);
	x->r_function = (*rdist_user_defined);

	x->r_should_refill_buffer = 1;
	if(x->r_useBuffer){
		x->r_should_refill_buffer = 1;
		rdist_fillBuffers((void *)x);
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		x->r_bufferPos = 0;
		rdist_fillBuffers((void *)x);
	}
}

void rdist_incBufPos(t_rdist *x){
	//if(x->r_bufferPos == (floor(x->r_bufferSize / x->r_stride) * x->r_stride) - 1){
	if(x->r_bufferPos == x->r_bufferSize - 1){
		pthread_t th;
		x->r_whichBuffer = abs(x->r_whichBuffer - 1);
		x->r_bufferPos = 0;
		pthread_create(&th, NULL, rdist_fillBuffers, (void *)x);
		pthread_detach(th);
		//rdist_fillBuffers((void *)x);
		//rdist_fillBuffers(x, x->r_bufferSize, x->r_buffers[abs(x->r_whichBuffer - 1)]);

		return;
	}
	x->r_bufferPos++;
	//x->r_bufferPos = (x->r_bufferPos + x->r_stride) % x->r_bufferSize;
}

//void rdist_fillBuffers(t_rdist *x, int n, float *buffer){
void *rdist_fillBuffers(void *args){
	t_rdist *x = (t_rdist *)args;
	//post("entering thread %d", x->r_bufferSize);

	/************************************************
	//need to make local copies of all vars
	************************************************/
	int i, j;
	i = j = 0;
	int b = abs(x->r_whichBuffer - 1);
	//for(i = 0; i < x->r_bufferSize; i++){
	while(i < x->r_bufferSize){
		if(!x->r_should_refill_buffer) return NULL;
		(*x->r_function)((void *)x);
		for(j = 0; j < x->r_stride && i < x->r_bufferSize; j++){
			x->r_buffers[b][i] = x->r_data[j];
			++i;
		}
	}
	return NULL;
}

void rdist_seed(t_rdist *x, long s){
	gsl_rng_set(x->r_rng, s);
}

// from PD x_misc.c
// JM made the last 9-digit number a random number.
int makeseed(void){
	int r = rand();
    static unsigned int random_nextseed = 1489853723;
    random_nextseed = random_nextseed * 435898247 + r;//938284287;
    return (random_nextseed & 0x7fffffff);
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

void rdist_tellmeeverything(t_rdist *x){
	post("DISTRIBUTION:");
	post("	%s", rdist_getDistString(x->r_dist));
	post("PARAMETERS:");
	int i;
	for(i = 0; i < x->r_numVars; i++)
		post("	%f", x->r_vars[i].a_type == A_LONG ? (float)x->r_vars[i].a_w.w_long : x->r_vars[i].a_w.w_float);
}

char *rdist_getDistString(int d){
	switch(d){
		case R_GAUSSIAN:
			return "gaussian";
		case R_GAUSSIAN_TAIL :
			return "gaussian_tail";
		case R_BIVARIATE_GAUSSIAN :
			return "bivariate_gaussian";
		case R_EXPONENTIAL :
			return "exponential";
		case R_LAPLACE :
			return "laplace";
		case R_EXPPOW :
			return "exppow";
		case R_CAUCHY :
			return "cauchy";
		case R_RAYLEIGH :
			return "rayleigh";
		case R_RAYLEIGH_TAIL :
			return "rayleigh_tail";
		case R_LANDAU :
			return "landau";
		case R_LEVY :
			return "levy";
		case R_LEVY_SKEW :
			return "levy_skew";
		case R_GAMMA :
			return "gamma";
		case R_UNIFORM :
			return "uniform";
		case R_LOGNORMAL :
			return "lognormal";
		case R_CHISQ :
			return "chisq";
		case R_F :
			return "f";
		case R_T :
			return "t";
		case R_BETA :
			return "beta";
		case R_LOGISTIC :
			return "logistic";
		case R_PARETO :
			return "pareto";
		case R_WEIBULL :
			return "weibull";
		case R_GUMBEL1 :
			return "gumbel1";
		case R_GUMBEL2 :
			return "gumbel2";
		case R_DIRICHLET :
			return "dirichlet";
		case R_POISSON :
			return "poisson";
		case R_BERNOULLI :
			return "bernoulli";
		case R_BINOMIAL :
			return "binomial";
		case R_MULTINOMIAL:
			return "multinomial";
		case R_NEGATIVE_BINOMIAL :
			return "negative_binomial";
		case R_PASCAL :
			return "pascal";
		case R_GEOMETRIC :
			return "geometric";
		case R_HYPERGEOMETRIC :
			return "hypergeometric";
		case R_LOGARITHMIC :
			return "logarithmic";
		case R_USER_DEFINED :
			return "nonparametric";
	}

	return NULL;
}

void rdist_gaussian(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double sigma = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_gaussian_ratio_method(x->r_rng, sigma);
}

void rdist_gaussian_tail(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double sigma = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_gaussian_tail(x->r_rng, a, sigma);
}

void rdist_bivariate_gaussian(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double sigma_x = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double sigma_y = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	const double rho = x->r_vars[2].a_type == A_LONG ? (double)x->r_vars[2].a_w.w_long : x->r_vars[2].a_w.w_float;
	double _x, _y;
	gsl_ran_bivariate_gaussian(x->r_rng, sigma_x, sigma_y, rho, &_x, &_y);
	x->r_data[0] = _x;
	x->r_data[1] = _y;
}

void rdist_exponential(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double mu = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_exponential(x->r_rng, mu);
}

void rdist_laplace(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_laplace(x->r_rng, a);
}

void rdist_exppow(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_exppow(x->r_rng, a, b);
}

void rdist_cauchy(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_cauchy(x->r_rng, a);
}

void rdist_rayleigh(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double sigma = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_rayleigh(x->r_rng, sigma);
}

void rdist_rayleigh_tail(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double sigma = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_rayleigh_tail(x->r_rng, a, sigma);
}

void rdist_landau(void *xx){
	t_rdist *x = (t_rdist *)xx;
	x->r_data[0] = (float)gsl_ran_landau(x->r_rng);
}

void rdist_levy(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double c = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double alpha = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_levy(x->r_rng, c, alpha);
}

void rdist_levy_skew(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double c = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double alpha = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	const double beta = x->r_vars[2].a_type == A_LONG ? (double)x->r_vars[2].a_w.w_long : x->r_vars[2].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_levy_skew(x->r_rng, c, alpha, beta);
}

void rdist_gamma(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_gamma(x->r_rng, a, b);
}

void rdist_uniform(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_flat(x->r_rng, a, b);
}

void rdist_lognormal(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double zeta = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double sigma = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_lognormal(x->r_rng, zeta, sigma);
}

void rdist_chisq(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double nu = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_chisq(x->r_rng, nu);
}

void rdist_fdist(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double nu1 = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double nu2 = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_fdist(x->r_rng, nu1, nu2);
}

void rdist_tdist(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double nu = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_tdist(x->r_rng, nu);
}

void rdist_beta(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_beta(x->r_rng, a, b);
}

void rdist_logistic(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_logistic(x->r_rng, a);
}

void rdist_pareto(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_pareto(x->r_rng, a, b);
}

void rdist_weibull(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_weibull(x->r_rng, a, b);
}

void rdist_gumbel1(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_gumbel1(x->r_rng, a, b);
}

void rdist_gumbel2(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double a = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double b = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_gumbel2(x->r_rng, a, b);
}

void rdist_dirichlet(void *xx){
	t_rdist *x = (t_rdist *)xx;
	int i;
	size_t k = x->r_numVars;
	double alpha[x->r_numVars];
	double theta[x->r_numVars];
	for(i = 0; i < k; i++)
		alpha[i] = x->r_vars[i].a_type == A_LONG ? (double)x->r_vars[i].a_w.w_long : x->r_vars[i].a_w.w_float;
	
	gsl_ran_dirichlet(x->r_rng, k, alpha, theta);
	for(i = 0; i < k; i++)
		x->r_data[i] = theta[i];
}

void rdist_poisson(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double mu = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_poisson(x->r_rng, mu);
}

void rdist_bernoulli(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_bernoulli(x->r_rng, p);
}

void rdist_binomial(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const unsigned int n = x->r_vars[1].a_type == A_LONG ? (unsigned int)x->r_vars[1].a_w.w_long : (unsigned int)x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_binomial(x->r_rng, p, n);
}

void rdist_multinomial(void *xx){
	t_rdist *x = (t_rdist *)xx;
	int i;
	size_t K = x->r_numVars - 1;
	unsigned int N = x->r_vars[0].a_type == A_LONG ? (unsigned int)x->r_vars[0].a_w.w_long : (unsigned int)x->r_vars[0].a_w.w_float;
	double p[K];
	unsigned int n[K];
	for(i = 0; i < K; i++)
		p[i] = x->r_vars[i + 1].a_type == A_LONG ? (unsigned int)x->r_vars[i + 1].a_w.w_long : (unsigned int)x->r_vars[i + 1].a_w.w_float;
	gsl_ran_multinomial(x->r_rng, K, N, p, n);
	for(i = 0; i < K; i++)
		x->r_data[i] = (float)n[i];
}

void rdist_negative_binomial(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const double n = x->r_vars[1].a_type == A_LONG ? (double)x->r_vars[1].a_w.w_long : x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_negative_binomial(x->r_rng, p, n);
}

void rdist_pascal(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	const unsigned int n = x->r_vars[1].a_type == A_LONG ? (unsigned int)x->r_vars[1].a_w.w_long : (unsigned int)x->r_vars[1].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_pascal(x->r_rng, p, n);
}

void rdist_geometric(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_geometric(x->r_rng, p);
}

void rdist_hypergeometric(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const unsigned int n1 = x->r_vars[0].a_type == A_LONG ? (unsigned int)x->r_vars[0].a_w.w_long : (unsigned int)x->r_vars[0].a_w.w_float;
	const unsigned int n2 = x->r_vars[1].a_type == A_LONG ? (unsigned int)x->r_vars[1].a_w.w_long : (unsigned int)x->r_vars[1].a_w.w_float;
	const unsigned int t = x->r_vars[2].a_type == A_LONG ? (unsigned int)x->r_vars[2].a_w.w_long : (unsigned int)x->r_vars[2].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_hypergeometric(x->r_rng, n1, n2, t);
}

void rdist_logarithmic(void *xx){
	t_rdist *x = (t_rdist *)xx;
	const double p = x->r_vars[0].a_type == A_LONG ? (double)x->r_vars[0].a_w.w_long : x->r_vars[0].a_w.w_float;
	x->r_data[0] = (float)gsl_ran_logarithmic(x->r_rng, p);
}

void rdist_user_defined(void *xx){
	t_rdist *x = (t_rdist *)xx;
	t_atom tmp = x->r_arIn[rdist_randPMF(x) * 2];
	x->r_data[0] = tmp.a_type == A_LONG ? (float)tmp.a_w.w_long : tmp.a_w.w_float;
}
