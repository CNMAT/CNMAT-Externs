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
NAME: mcmc
DESCRIPTION: Monte Carlo Markov Chain for approximating spectral envelopes
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "commonsyms.h"
#include "version.c"
#include "math.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_spline.h>

#define DEBUG
#ifdef DEBUG
#define PDEBUG(s,args...) post(s, ##args)
#else
#define PDEBUG(s,args...)
#endif

#define MCMC_MAX_ARRAY_LEN 2048 // the max array len for incoming and outgoing lists
#define MCMC_STATE_FADE_OUT 0
#define MCMC_STATE_CHANGE_FREQ 1
#define MCMC_STATE_FADE_IN 2

#define PST 1.0594630943592953
#define MST 0.94387431268169353
#define PST_D10 1.0057929410678534
#define MST_D10 0.99424042381754729

#define MCMC_ACCEPTANCE_RATE_BUF_LEN 256

typedef struct _mcmc{
	t_object ob;
	void *out1;
	void *out2;
	void *info_out;
	long nOsc; // number of oscillators = half the length of the output array
	long nOsc_new;
	long arrayInLength; // length of the array that we received
	double *freq_in, *amp_in;
	double *cdf;
	t_atom *arrayOut; // the output array
	//double *currentProb; 
	double freq_min, freq_max;
	double centroid;
	double newFreq;
	long counter; // the current element of the list that's being updated
	// we don't want this to be changed in the middle of an update cycle,
	// so the value of numOscToUpdate gets moved into actualNumOscToUpdate only
	// at the beginning of a cycle.
	long numOscToUpdate;
	long actualNumOscToUpdate;
	long state;
	double tinterval;
	double oscamp;
	gsl_rng *rng;
	//gsl_interp_accel *acc;
	//gsl_spline *spline;
	double stdev;
	//double pro_stdev; // standard deviation for the proposal function
	double beta_alpha, beta_beta; // parameters to the beta distribution
	double proposal_density_width;
	long on;
	long acceptanceRateBuf[MCMC_ACCEPTANCE_RATE_BUF_LEN];
	long acceptanceRateBufPos;
	double acceptanceRateSum;
	long haveGoodAcceptanceData;
	double interp_alpha;
	double w0;
} t_mcmc;

void *mcmc_class;

void *mcmc_new(t_symbol *sym, long argc, t_atom *argv);
void mcmc_free(t_mcmc *x);
void mcmc_assist(t_mcmc *x, void *b, long m, long a, char *s);

void mcmc_list(t_mcmc *x, t_symbol *msg, long argc, t_atom *argv);
t_max_err mcmc_nOsc(t_mcmc *x, t_object *attr, long argc, t_atom *argv);
t_max_err mcmc_var_get(t_mcmc *x, t_object *attr, long *argc, t_atom **argv);
t_max_err mcmc_var_set(t_mcmc *x, t_object *attr, long argc, t_atom *argv);
void mcmc_makeCDF(long ac, double *av, double *cdf);
void mcmc_bang(t_mcmc *x);
long mcmc_randomFromCDF(double r, double *cdf, long len);
void mcmc_getNewFreq(t_mcmc *x, double currentFreq, double currentProb, double *newFreq, double *newProb);
void mcmc_clock_cb(t_mcmc *x, t_symbol *msg, long argc, t_atom *argv);
void mcmc_fadeOut(t_mcmc *x);
void mcmc_changeFreq(t_mcmc *x);
void mcmc_fadeIn(t_mcmc *x);
void mcmc_outputList(t_mcmc *x, short length, t_atom *array);
void mcmc_int(t_mcmc *x, long n);
double mcmc_interp(double *x_vec, double *y_vec, double x, long len, double minx, double maxx, double alpha, double w0);

double mcmc_accepted(t_mcmc *x);
double mcmc_rejected(t_mcmc *x);
double mcmc_calc_acceptance_rate(t_mcmc *x, long l);

//void mcmc_makePMF(t_mcmc *x);
long mcmc_counter(t_mcmc *x);
double mcmc_gaussBlur(t_mcmc *x, double m, int r);
double mcmc_gaussdist(t_mcmc *x, double sigma);
int makeseed(void);
void mcmc_betafit(t_mcmc *x, double *freq, double *amp, long len, double *alpha, double *beta);
double mcmc_norm(double x, double min, double max);

void mcmc_tellmeeverything(t_mcmc *x);

void mcmc_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

t_symbol *ps_acceptance_rate, *ps_centroid, *ps_variance, *ps_cdf;

int main(void){
	t_class *c = class_new("mcmc", (method)mcmc_new, (method)mcmc_free, sizeof(t_mcmc), (method)0L, A_GIMME, 0); 

	t_object *attr;
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)mcmc_bang, "bang", 0);
	class_addmethod(c, (method)mcmc_int, "int", 0);
	class_addmethod(c, (method)mcmc_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)mcmc_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)mcmc_tellmeeverything, "tellmeeverything", 0L, 0);

	CLASS_ATTR_LONG(c, "nOsc", 0, t_mcmc, nOsc);
	attr = (t_object *)class_attr_get(c, gensym("nOsc"));
	object_method(attr, gensym("setmethod"), USESYM(set), mcmc_nOsc);

	CLASS_ATTR_DOUBLE(c, "stdev", 0, t_mcmc, stdev);

	CLASS_ATTR_DOUBLE(c, "var", 0, t_mcmc, stdev);
	CLASS_ATTR_ACCESSORS(c, "var", mcmc_var_get, mcmc_var_set);

	CLASS_ATTR_DOUBLE(c, "time_interval", 0, t_mcmc, tinterval);
	CLASS_ATTR_DOUBLE(c, "oscamp", 0, t_mcmc, oscamp);

	CLASS_ATTR_DOUBLE(c, "interp_alpha", 0, t_mcmc, interp_alpha);
	CLASS_ATTR_DOUBLE(c, "w0", 0, t_mcmc, w0);
	//CLASS_ATTR_DOUBLE(c, "pro_stdev", 0, t_mcmc, pro_stdev);

	// better name?
	CLASS_ATTR_DOUBLE(c, "proposal_density_width", 0, t_mcmc, proposal_density_width);


	class_register(CLASS_BOX, c);
	mcmc_class = c;
	version(0);

	return 0;
}

void *mcmc_new(t_symbol *sym, long argc, t_atom *argv){
	t_mcmc *x;
	int i;
	if(x = (t_mcmc *)object_alloc(mcmc_class)){
		x->freq_in = (double *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(double));
		x->amp_in = (double *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(double));
		x->cdf = (double *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(double));
		x->arrayOut = (t_atom *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(t_atom));
		//x->currentProb = (double *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(double));
		//x->pmf = (double *)calloc(MCMC_MAX_ARRAY_LEN, sizeof(t_atom));

		for(i = 0; i < MCMC_MAX_ARRAY_LEN; i++){
			SETFLOAT(&(x->arrayOut[i]), 0.);
		}

		x->info_out = outlet_new(x, NULL);
		x->out2 = floatout(x);
		x->out1 = listout(x);	

		x->newFreq = 0.;

		gsl_rng_env_setup();
		x->rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_waterman14);
	
		// makeseed() is from the PD code in x_misc.c
		gsl_rng_set(x->rng, makeseed());

		x->counter = 0;

		//x->pro_stdev = 10.;
		x->stdev = 0.0;
		x->nOsc = x->nOsc_new = 100;
		x->oscamp = 0.03;

		x->tinterval = 25.0; //ms

		x->on = 0;
		x->numOscToUpdate = 1;

		x->acceptanceRateSum = 0.;
		x->haveGoodAcceptanceData = 0;
		x->acceptanceRateBufPos = 0;
		for(i = 0; i < MCMC_ACCEPTANCE_RATE_BUF_LEN; i++){
			x->acceptanceRateBuf[i] = 0;
		}

		x->proposal_density_width = 0.75;
		x->beta_alpha = x->beta_beta = 0.;

		x->interp_alpha = 1.;
		x->w0 = 0.00001;

		ps_acceptance_rate = gensym("acceptance_rate");
		ps_centroid = gensym("centroid");
		ps_variance = gensym("variance");
		ps_cdf = gensym("cdf");

		attr_args_process(x, argc, argv);
	}
	return x;
}

void mcmc_free(t_mcmc *x){
	free(x->freq_in);
	free(x->amp_in);
	free(x->cdf);
	//free(x->pmf);
	free(x->arrayOut);
	//free(x->currentProb);
}


void mcmc_assist(t_mcmc *x, void *b, long m, long a, char *s){
	if(m == ASSIST_INLET){
		sprintf(s,"(List) %ld frequency amplitude pairs", x->nOsc);
	}else{
		switch(a){	
		case 0:
			sprintf(s,"(List) frequency amplitude pairs");
			break;
		case 1:
			sprintf(s,"Only the updated partial");
			break;
		case 2:
			sprintf(s,"Index number of the partial being updated");
			break;
		}
	}
}

void mcmc_list(t_mcmc *x, t_symbol *msg, long argc, t_atom *argv){
	if(argc % 2){
		error("mcmc: multiple of 2 floats required (frequency amplitude)");
		return;
	}
	if(argc > MCMC_MAX_ARRAY_LEN){
		error("mcmc: maximum list length is %d", MCMC_MAX_ARRAY_LEN);
		return;
	}
	
	// copy the input array into our local storage
	//memcpy(x->arrayIn, argv, argc * sizeof(t_atom));
	int i;
	t_atom *ptr = argv;
	for(i = 0; i < argc / 2; i++){
		// convert to power spectrum?
		x->freq_in[i] = atom_getfloat(ptr++);
		//x->amp_in[i] = pow(atom_getfloat(ptr++), 2.);
		x->amp_in[i] = atom_getfloat(ptr++);
	}
	for(i = argc / 2; i < MCMC_MAX_ARRAY_LEN; i++){
		x->freq_in[i] = 0.;
		x->amp_in[i] = 0.;
	}

	x->arrayInLength = argc / 2;

	x->freq_min = x->freq_in[0];
	x->freq_max = x->freq_in[x->arrayInLength - 1];

	error("min = %f, max = %f", x->freq_min, x->freq_max);

	// consider sorting the input here?

	mcmc_betafit(x, x->freq_in, x->amp_in, x->arrayInLength, &(x->beta_alpha), &(x->beta_beta));

	mcmc_makeCDF(x->arrayInLength, x->amp_in, x->cdf);
}

void mcmc_makeCDF(long ac, double *av, double *cdf){
	int i;
	double sum = 0;
	for(i = 0; i < ac; i++){
		//post("raw = %f", av[i]);
		sum += av[i];
	}
	cdf[0] = av[0] / sum;
	for(i = 1; i < ac; i++){
		//post("adj = %f", av[i] / sum);
		cdf[i] = cdf[i - 1] + (av[i] / sum);
	}
}

void mcmc_bang(t_mcmc *x){
	error("mcmc: bang does nothing right now");
}

long mcmc_randomFromCDF(double r, double *cdf, long len){
	int i;
	for(i = 0; i < len; i++){
		//post("r = %f, cdf[%d] = %f", r, i, cdf[i]);
		if(r <= cdf[i]){
			return i;
		}
	}
	return -1;
}

double mcmc_accepted(t_mcmc *x){
	return mcmc_calc_acceptance_rate(x, 1);
}

double mcmc_rejected(t_mcmc *x){
	return mcmc_calc_acceptance_rate(x, 0);
}

double mcmc_calc_acceptance_rate(t_mcmc *x, long l){
	long prevIndex = x->acceptanceRateBufPos == 0 ? MCMC_ACCEPTANCE_RATE_BUF_LEN - 1 : x->acceptanceRateBufPos - 1;
	t_atom a;
	x->acceptanceRateBufPos = (x->acceptanceRateBufPos + 1) % MCMC_ACCEPTANCE_RATE_BUF_LEN;
	if(x->acceptanceRateBufPos == 0){
		x->haveGoodAcceptanceData = 1;
	}
	if(x->haveGoodAcceptanceData){
		x->acceptanceRateSum = (x->acceptanceRateSum - x->acceptanceRateBuf[prevIndex]) + l;
	}else{
		x->acceptanceRateSum += l;
	}
	x->acceptanceRateBuf[x->acceptanceRateBufPos] = l;
	SETFLOAT(&a, x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN);
	outlet_anything(x->info_out, ps_acceptance_rate, 1, &a);
	return x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN;
}

void mcmc_getNewFreq(t_mcmc *x, double currentFreq, double currentProb, double *newFreq, double *newProb){
	double r, proposal_x, proposal_y, ratio, min, max, norm;
	long lr;
	int i = 0;
	double q01, q10;

	if(currentFreq == 0.f){
		lr = mcmc_randomFromCDF(gsl_rng_uniform(x->rng), x->cdf, x->arrayInLength);
		*newFreq = x->freq_in[lr];
		*newProb = x->amp_in[lr];
		return;
	}

	//r = mcmc_gaussdist(x, x->pro_stdev);
	proposal_x = -1;
	while((proposal_x < x->freq_min || proposal_x > x->freq_max) && i < 50){
		r = gsl_ran_beta(x->rng, x->beta_alpha, x->beta_beta);
		norm = mcmc_norm(x->centroid, x->freq_min, x->freq_max);
		min = currentFreq - (norm * (currentFreq * x->proposal_density_width));
		max = currentFreq + ((1 - norm) * (currentFreq * x->proposal_density_width));
		PDEBUG("currentFreq = %f, norm = %f, min = %f, max = %f, freq_min = %f, freq_max = %f, cent = %f", currentFreq, norm, min, max, x->freq_min, x->freq_max, x->centroid);
		PDEBUG("pdw = %f", x->proposal_density_width);
		proposal_x = (r * (max - min)) + min;
		PDEBUG("proposal = %f", proposal_x);
		++i;
	}
	// if we couldn't find a new freq for some reason (stdev is too small, for instance)
	// just choose one from the cdf and bail out.
	if(i == 100){
		lr = mcmc_randomFromCDF(gsl_rng_uniform(x->rng), x->cdf, x->arrayInLength);
		*newFreq = x->freq_in[lr];
		*newProb = x->amp_in[lr];
		outlet_anything(x->info_out, ps_cdf, 0, NULL);
		return;
	}
	proposal_y = mcmc_interp(x->freq_in, 
				 x->amp_in, 
				 proposal_x, 
				 x->arrayInLength, 
				 x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
				 x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
				 x->interp_alpha, 
				 x->w0);

	// the proposal density centered around the current frequency and evaluated at the 
	// proposed frequency.
	q10 = gsl_ran_beta_pdf(r, x->beta_alpha, x->beta_beta);

	// the proposal density centered around the proposed frequency and evaluated at the
	// current frequency.
	min = proposal_x - (norm * (proposal_x * x->proposal_density_width));
	max = proposal_x + ((1 - norm) * (proposal_x * x->proposal_density_width));
	double currentFreq_scaled = mcmc_norm(currentFreq, min, max);
	if(currentFreq_scaled > 0 && currentFreq_scaled < 1){
		q01 = gsl_ran_beta_pdf(currentFreq_scaled, x->beta_alpha, x->beta_beta);
	}else{
		q01 = 0;
	}
	ratio = (proposal_y / currentProb) * (q01 / q10);
	PDEBUG("q01 = %f, q10 = %f, %f, %f %f", q01, q10, q01 / q10, (proposal_y / currentProb), ratio);

	PDEBUG("r = %f, pro_x = %f, pro_y = %f, current_y = %f, ratio = %f", r, proposal_x, proposal_y, currentProb, ratio);

	if(ratio >= 1){
		*newFreq = proposal_x;
		*newProb = proposal_y;
		return;
	}else{
		r = gsl_rng_uniform(x->rng);
		PDEBUG("ratio was less than 1, so here's a new random num: %f", r);
		if(r < ratio){
			PDEBUG("r (%f) < ratio (%f)", r, ratio);
			*newFreq = proposal_x;
			*newProb = proposal_y;
			return;
		}else{
			PDEBUG("r (%f) !< ratio (%f)", r, ratio);
			*newFreq = currentFreq;
			*newProb = currentProb;
			return;
		}
	}
}

void mcmc_clock_cb(t_mcmc *x, t_symbol *msg, long argc, t_atom *argv){
	if(!x->on){
		return;
	}
	double currentFreq, currentProb, newFreq, newProb;
	t_atom a;
	int i;
	switch(x->state){
	case MCMC_STATE_FADE_OUT:
		x->nOsc = x->nOsc_new;
		currentFreq = atom_getfloat(&(x->arrayOut[x->counter * 2]));
		currentProb = mcmc_interp(x->freq_in, 
					  x->amp_in, 
					  currentFreq, 
					  x->arrayInLength / 2, 
				 	  x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
					  x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
					  x->interp_alpha, 
					  x->w0);
		mcmc_getNewFreq(x, currentFreq, currentProb, &newFreq, &newProb);
		PDEBUG("%d %f %f", x->counter, currentFreq, newFreq, newProb);
		SETLONG(&a, x->counter);

		i = 0;
		while(newFreq == currentFreq && i < 100){
			mcmc_rejected(x);
			mcmc_getNewFreq(x, currentFreq, currentProb, &newFreq, &newProb);
			i++;
		}
		if(i < 100){
			mcmc_accepted(x);
			x->newFreq = newFreq;
			mcmc_fadeOut(x);
			x->state = MCMC_STATE_CHANGE_FREQ;
		}else{
			error("couldn't change freq!");
			mcmc_counter(x);
			if(x->on){
				schedule_fdelay(x, (method)mcmc_clock_cb, x->tinterval, NULL, 0, NULL);
			}
			return;
		}
		/*
		if(newFreq >= currentFreq * MST_D10 && newFreq <= currentFreq * PST_D10){
			outlet_anything(x->info_out, gensym("skipping"), 1, &a);
			mcmc_rejected(x);
			mcmc_counter(x);
			if(x->on){
				schedule_fdelay(x, (method)mcmc_clock_cb, x->tinterval, NULL, 0, NULL);
			}
			return;
			// do nothing
		}else{
			outlet_anything(x->info_out, gensym("changing"), 1, &a);
			mcmc_accepted(x);
			x->newFreq = newFreq;
			mcmc_fadeOut(x);
			x->state = MCMC_STATE_CHANGE_FREQ;
		}
		*/
		PDEBUG("**************************************************");
		PDEBUG("counter = %d", x->counter);
		break;
	case MCMC_STATE_CHANGE_FREQ:
		mcmc_changeFreq(x);
		x->state = MCMC_STATE_FADE_IN;
		break;
	case MCMC_STATE_FADE_IN:
		mcmc_fadeIn(x);
		x->state = MCMC_STATE_FADE_OUT;
		mcmc_counter(x);
		break;
	}

	mcmc_outputList(x, x->nOsc * 2, x->arrayOut);

	if(x->on){
		schedule_fdelay(x, (method)mcmc_clock_cb, x->tinterval, NULL, 0, NULL);
	}
}

void mcmc_fadeOut(t_mcmc *x){
	SETFLOAT(&(x->arrayOut[(x->counter * 2) + 1]), 0.0);
}

void mcmc_changeFreq(t_mcmc *x){	
	SETFLOAT(&(x->arrayOut[x->counter * 2]), x->newFreq);
}

void mcmc_fadeIn(t_mcmc *x){
	SETFLOAT(&(x->arrayOut[(x->counter * 2) + 1]), x->oscamp);
}

void mcmc_outputList(t_mcmc *x, short length, t_atom *array){
	//int i;
	//t_atom tmp[2];
	outlet_list(x->out1, 0, length, array);
	/*
	switch(x->outputType){
	case 0:
		outlet_list(x->out1, 0, length, array);
		break;
	case 1:
		for(i = 0; i < length / 2; i++){
			tmp[0] = array[i * 2];
			tmp[1] = array[(i * 2) + 1];
			outlet_list(x->out1, 0, 2, tmp);
		}
		break;
	}
	*/
}

void mcmc_int(t_mcmc *x, long n){
	int i = 0;
	t_atom ar[2];
	x->on = n;	
	if(n){
		schedule_fdelay(x, (method)mcmc_clock_cb, x->tinterval, NULL, 0, NULL);
		return;
	}

	SETFLOAT(&(ar[0]), 0.0);
	SETFLOAT(&(ar[1]), 0.0);
	
	for(i = 0; i < (int)x->nOsc * 2; i++){
		SETFLOAT(&(x->arrayOut[i]), 0.0);
	}
	outlet_list(x->out1, 0, 2, ar);
}

long mcmc_counter(t_mcmc *x){	
	x->counter = (x->counter + 1) % x->nOsc;
	return x->counter;
}

double mcmc_gaussBlur(t_mcmc *x, double m, int r){
	return m * (pow(pow(2., 1./12.), mcmc_gaussdist(x, x->stdev)));
}

double mcmc_gaussdist(t_mcmc *x, double sigma){
	return (double)gsl_ran_gaussian_ratio_method(x->rng, sigma);
}

int makeseed(void){
	int r = rand();
	static unsigned int random_nextseed = 1489853723;
	random_nextseed = random_nextseed * 435898247 + r;//938284287;
	return (random_nextseed & 0x7fffffff);
}

void mcmc_betafit(t_mcmc *x, double *freq, double *amp, long len, double *alpha, double *beta){
	double mean, var;
	double num = 0;
	double den = 0;
	double min = DBL_MAX;
	double max = 0.;
	t_atom a;
	int i;
	// spectral centroid
	for(i = 0; i < len; i++){
		num += freq[i] * amp[i];
		den += amp[i];
		if(freq[i] < min){
			min = freq[i];
		}
		if(freq[i] > max){
			max = freq[i];
		}
	}
	mean = num / den;
	num = 0;
	den = 0;
	// variance
	for(i = 0; i < len; i++){
		num += (freq[i] * (amp[i] - mean));
		den += (amp[i] - mean);
	}
	var = pow((num / den), 2.);

	x->centroid = mean;
	// since we have to compute the spectral centroid and variance, we may as well output them
	SETFLOAT(&a, mean);
	outlet_anything(x->info_out, ps_centroid, 1, &a);
	SETFLOAT(&a, var);
	outlet_anything(x->info_out, ps_variance, 1, &a);

	// scale to (0-1);
	mean = (mean - min) / (max - min);
	var = var / pow(max - min, 2.);

	// estimate params
	*alpha = (((mean * (1 - mean)) / var) - 1);
	// add 2 to narrow the curve a bit--this worked well in the matlab simulations I did,
	// but may need to be adjusted.
	*beta = ((1 - mean) * *alpha) + 2;
	*alpha = (mean * *alpha) + 2;
}

double mcmc_norm(double x, double min, double max){
	return (x - min) / (max - min);
}

double mcmc_interp(double *x_vec, double *y_vec, double x, long len, double minx, double maxx, double alpha, double w0){
	int i;
	double num = 0.;
	double den = 0.;
	double d = 0;
	double x_w[len + 2], y_w[len + 2], w[len + 2];
	double sum = 0;
	x_w[0] = 0.;
	y_w[0] = 0.;

	for(i = 0; i < len; i++){
		x_w[i + 1] = x_vec[i];
		y_w[i + 1] = y_vec[i];
		w[i + 1] = 1;
		sum += y_vec[i];
	}
	sum /= w0;

	x_w[len + 1] = x_vec[len - 1] * 1.1 < 22050. ? x_vec[len - 1] * 1.1 : 22050;
	y_w[len + 1] = 0.;

	w[0] = sum / 2;
	w[len + 1] = sum / 2;

	for(i = 0; i < len + 2; i++){
		d = pow(abs(x - x_w[i]), alpha);
		if(d < 0.0001){
			num = y_w[i];
			den = 1.;
			break;
		}else{
			num += (w[i] * y_w[i]) / d;
			den += w[i] / d;
		}
	}
	return num / den;
}

/*
double mcmc_interp(double *x_vec, double *y_vec, double x, long len, double minx, double maxx, double alpha, double w0){
	int i;
	double numer = 0.;
	double denom = 0.;
	double d;
	//post("x = %f, mind = %f, maxd = %f", x, mind, maxd);
	double boundary_weight = (len * (exp(w0) - 1));

	d = pow(abs(x - minx), alpha);
	//post("d = %f", d);
	if(d < 0.0001){
		return 0.;
	}
	denom += boundary_weight / d;
	for(i = 0; i < len; i++){
		d = pow(abs(x - x_vec[i]), alpha);
		//post("d = %f", d);
		if(d < 0.0001){
			return y_vec[i];
		}else{
			numer += y_vec[i] / d;
			denom += 1. / d;
		}
	}
	d = pow(abs(x - maxx), alpha);
	//post("d = %f", d);
	//post("**************************************************");
	if(d < 0.0001){
		return 0.;
	}
	denom += boundary_weight / d;
	return numer / denom;
}
*/

/*
double mcmc_interp(double *x_vec, double *y_vec, double x, long len, double minx, double maxx, double alpha){
	int i;
	double numer = 0.;
	double denom = 0.;
	double d;

	d = pow(abs(x - minx), alpha);
	// numer += 0. / d;
	denom += 1 / d;

	for(i = 0; i < len; i++){
		if(x >= x_vec[i] * pow(pow(2. / 12.), -.1) && 
		   x <= x_vec[i] * pow(pow(2. / 12.), .1)){
			return y_vec[i];
		}
		d = pow(abs(x - x_vec[i]), alpha);
		numer += (len * y_vec[i]) / d;
		denom += len / d;
	}

	d = pow(abs(x - maxx), alpha);
	// numer += 0. / d;
	denom += 1 / d;

	return numer / denom;

}
*/

///////////////////////////////////////////////////////////////////////////////////////////
// OPTIONS

t_max_err mcmc_nOsc(t_mcmc *x, t_object *attr, long argc, t_atom *argv){
	long n;
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}

	n = atom_getlong(argv);

	if(n > MCMC_MAX_ARRAY_LEN){
		error("mcmc: the maximum number of oscillators is %d", MCMC_MAX_ARRAY_LEN);
		return MAX_ERR_GENERIC;
	}
	if(n < 1){
		error("mcmc: the number of oscillators must be greater than 0");
		return MAX_ERR_GENERIC;
	}

	x->nOsc_new = n;	

	return 0;
}

t_max_err mcmc_var_get(t_mcmc *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setfloat(*argv, powf(x->stdev, 2.f));
	return MAX_ERR_NONE;
}

t_max_err mcmc_var_set(t_mcmc *x, t_object *attr, long argc, t_atom *argv){
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}
	x->stdev = sqrt(atom_getfloat(argv));
	return MAX_ERR_NONE;
}

void mcmc_tellmeeverything(t_mcmc *x){
	version(0);
	post("MCMC is %s", (x->on) ? "ON" : "OFF");
	post("Number of oscillators: %d", x->nOsc);
	post("Oscillator amplitude: %f", x->oscamp);
	post("Time interval: %f", x->tinterval);
	post("Standard deviation (Gaussian blur): %f", x->stdev);
}

void mcmc_errorHandler (const char * reason, const char * file, int line, int gsl_errno){
	error("mcmc: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}
