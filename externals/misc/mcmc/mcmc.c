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

//#define DEBUG
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
	long numPartials; // length of the array that we received
	double *freq_in, *amp_in;
	double *cdf;
	t_atom *arrayOut; // the output array
	//double *currentProb; 
	double freq_min, freq_max;
	double centroid;
	double newFreq;
	double avg_amp;
	double stdev_amp;
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
	long sample_mode;
	double temp;
	FILE *fp;
	long log;
	long force_freq_change;
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
long mcmc_samplePartialCDF(t_mcmc *x, double freq);
int mcmc_isFreqValid(t_mcmc *x, double freq);
int mcmc_findNearestInputFreq(t_mcmc *x, double freq);
void mcmc_getNewFreq(t_mcmc *x, double currentFreq, double currentProb, double *newFreq, double *newProb);
void mcmc_clock_cb(t_mcmc *x, t_symbol *msg, long argc, t_atom *argv);
void mcmc_fadeOut(t_mcmc *x);
void mcmc_changeFreq(t_mcmc *x);
void mcmc_fadeIn(t_mcmc *x);
void mcmc_outputList(t_mcmc *x, short length, t_atom *array);
double mcmc_quantizeFreq(double freq, int arrayLen, double *freq_array);
void mcmc_int(t_mcmc *x, long n);
double mcmc_interp(double *x_vec, double *y_vec, double x, long len, double minx, double maxx, double alpha, double w0);

double mcmc_accepted(t_mcmc *x);
double mcmc_rejected(t_mcmc *x);
double mcmc_calc_acceptance_rate(t_mcmc *x, long l);
double mcmc_getAcceptanceRate(t_mcmc *x);

//void mcmc_makePMF(t_mcmc *x);
long mcmc_counter(t_mcmc *x);
double mcmc_gaussBlur(t_mcmc *x, double m, int r);
double mcmc_gaussdist(t_mcmc *x, double sigma);
int makeseed(void);
void mcmc_betafit(t_mcmc *x, double *freq, double *amp, long len, double *alpha, double *beta);
double mcmc_scale(double f, double min_in, double max_in, double min_out, double max_out);
double mcmc_norm(double x, double min, double max);

void mcmc_tellmeeverything(t_mcmc *x);

void mcmc_errorHandler(const char * reason, const char * file, int line, int gsl_errno);

void mcmc_interp_test(t_mcmc *x, double f);
void mcmc_log(t_mcmc *x, long l);

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

	class_addmethod(c, (method)mcmc_interp_test, "interp", A_FLOAT, 0);
	class_addmethod(c, (method)mcmc_log, "log", A_LONG, 0);

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

	CLASS_ATTR_LONG(c, "sample_mode", 0, t_mcmc, sample_mode);
	CLASS_ATTR_LONG(c, "force_freq_change", 0, t_mcmc, force_freq_change);

	CLASS_ATTR_DOUBLE(c, "temperature", 0, t_mcmc, temp);


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
		x->out2 = listout(x);
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

		x->proposal_density_width = 1.;
		x->beta_alpha = x->beta_beta = 0.;

		x->interp_alpha = 1.;
		x->w0 = 0.00001;
		x->temp = 0.;

		ps_acceptance_rate = gensym("acceptance_rate");
		ps_centroid = gensym("centroid");
		ps_variance = gensym("variance");
		ps_cdf = gensym("cdf");
		x->log = 0;
		x->force_freq_change = 0;
		//if(x->log) x->fp = fopen("/Users/johnmac/mcmc.txt", "w");

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
	if(x->log) fclose(x->fp);
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
	if(x->log) fprintf(x->fp, "LIST ++++++++++++++++++++\n");
	x->avg_amp = 0.;
	for(i = 0; i < argc / 2; i++){
		// convert to power spectrum?
		x->freq_in[i] = atom_getfloat(ptr++);
		//x->amp_in[i] = pow(atom_getfloat(ptr++), 2.);
		x->amp_in[i] = atom_getfloat(ptr++);
		x->avg_amp += x->amp_in[i];
		if(x->log) fprintf(x->fp, "%f %f\n", x->freq_in[i], x->amp_in[i]);
	}
	x->avg_amp /= (argc / 2.);
	for(i = 0; i < argc / 2; i++){
		x->stdev_amp += pow(x->amp_in[i] - x->avg_amp, 2.);
	}
	//post("avg_amp = %f, stdev_amp = %f", x->avg_amp, x->stdev_amp);
	x->stdev_amp /= (argc / 2.);
	for(i = argc / 2; i < MCMC_MAX_ARRAY_LEN; i++){
		x->freq_in[i] = 0.;
		x->amp_in[i] = 0.;
	}

	x->numPartials = argc / 2;

	x->freq_min = x->freq_in[0];
	x->freq_max = x->freq_in[x->numPartials - 1];

	//post("min = %f, max = %f", x->freq_min, x->freq_max);

	// consider sorting the input here?

	mcmc_betafit(x, x->freq_in, x->amp_in, x->numPartials, &(x->beta_alpha), &(x->beta_beta));
	//post("shape parameters: a = %f, b = %f", x->beta_alpha, x->beta_beta);

	mcmc_makeCDF(x->numPartials, x->amp_in, x->cdf);
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

long mcmc_samplePartialCDF(t_mcmc *x, double freq){
	int nearestPartial = mcmc_findNearestInputFreq(x, freq);
	double nearestFreq = x->freq_in[nearestPartial];
	//post("freq = %f, nearestFreq = %f, nearestPartial = %d", freq, nearestFreq, nearestPartial);
	double min = x->freq_min, max = x->freq_max;
	int mini = 0, maxi = x->numPartials;
	double min_ex = 0, max_ex = 1;
	if(nearestFreq < freq){
		maxi = nearestPartial;
		max = nearestFreq;
	}else if(nearestFreq > freq){
		min_ex = -1;
		max_ex = 0;
		mini = nearestPartial;
		min = nearestFreq;
	}else{
		// nearest freq is the current freq, so we should look for the next one
		if(x->numPartials == 1){
			return nearestFreq;
		}
		if(nearestPartial == 0){
			min_ex = -1;
			max_ex = 0;
			mini = nearestPartial;
			min = nearestFreq;
		}else if(nearestPartial == (x->numPartials - 1)){
			maxi = nearestPartial;
			max = nearestFreq;
		}else{
			if(fabs(x->freq_in[nearestPartial - 1] - freq) > fabs(x->freq_in[nearestPartial + 1])){
				maxi = nearestPartial;
				max = nearestFreq;
			}else{
				min_ex = -1;
				max_ex = 0;
				mini = nearestPartial;
				min = nearestFreq;
			}
		}
	}

	double r = gsl_rng_uniform(x->rng);
	post("r = %f", r);
	r = mcmc_scale(r, 0., 1., mcmc_norm(min, x->freq_min, x->freq_max), mcmc_norm(max, x->freq_min, x->freq_max));
	int i;
	double cdf[maxi - mini];
	double sum = 0;
	//post("freq = %f", freq);
	//post("mini = %d, maxi = %d, min = %f, max = %f", mini, maxi, min, max);
	cdf[0] = 0;
	for(i = 1; i < (maxi - mini); i++){
		//post("%d scale = %f", i, fabs(mcmc_scale(x->freq_in[i + mini], min, max, min_ex, max_ex)));
		cdf[i] = (sum += x->amp_in[i + mini] * pow(fabs(mcmc_scale(x->freq_in[i + mini], min, max, min_ex, max_ex)), 1.5));
		//post("cdf[%d] = %f", i, cdf[i]);
	}
	for(i = 0; i < (maxi - mini); i++){
		cdf[i] /= sum;
		//post("cdf[%d] = %f", i, cdf[i]);
	}
	for(i = 0; i < (maxi - mini); i++){
		//post("r = %f, cdf[%d] = %f", r, i, cdf[i]);
		if(r <= cdf[i]){
			//post("choosing %d %f", i + mini, x->freq_in[i + mini]);
			return i + mini;
		}
	}
	return 0;
}

double mcmc_accepted(t_mcmc *x){
	return mcmc_calc_acceptance_rate(x, 1);
}

double mcmc_rejected(t_mcmc *x){
	return mcmc_calc_acceptance_rate(x, 0);
}

double mcmc_calc_acceptance_rate(t_mcmc *x, long l){
	x->acceptanceRateSum -= x->acceptanceRateBuf[x->acceptanceRateBufPos % MCMC_ACCEPTANCE_RATE_BUF_LEN];
	x->acceptanceRateBuf[x->acceptanceRateBufPos % MCMC_ACCEPTANCE_RATE_BUF_LEN] = l;
	x->acceptanceRateSum += l;
	x->acceptanceRateBufPos++;
	t_atom a;
	SETFLOAT(&a, x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN);
	outlet_anything(x->info_out, ps_acceptance_rate, 1, &a);
	return x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN;
	/*
	long prevIndex = x->acceptanceRateBufPos == 0 ? MCMC_ACCEPTANCE_RATE_BUF_LEN - 1 : x->acceptanceRateBufPos - 1;
	t_atom a;
	x->acceptanceRateBufPos = (x->acceptanceRateBufPos + 1) % MCMC_ACCEPTANCE_RATE_BUF_LEN;
	//if(x->acceptanceRateBufPos == 0){
		x->haveGoodAcceptanceData = 1;
		//}
	if(x->haveGoodAcceptanceData){
		x->acceptanceRateSum = (x->acceptanceRateSum - x->acceptanceRateBuf[prevIndex]) + l;
	}else{
		x->acceptanceRateSum += l;
	}
	x->acceptanceRateBuf[x->acceptanceRateBufPos] = l;
	SETFLOAT(&a, x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN);
	outlet_anything(x->info_out, ps_acceptance_rate, 1, &a);
	return x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN;
	*/
}

double mcmc_getAcceptanceRate(t_mcmc *x){
	if(x->acceptanceRateBufPos < MCMC_ACCEPTANCE_RATE_BUF_LEN){
		return x->acceptanceRateSum / (x->acceptanceRateBufPos + 1);
	}else{
		return x->acceptanceRateSum / MCMC_ACCEPTANCE_RATE_BUF_LEN;
	}
}

int mcmc_isFreqValid(t_mcmc *x, double freq){
	int nearestFreqIndex = mcmc_findNearestInputFreq(x, freq);
	double nearestFreq = x->freq_in[nearestFreqIndex];
	double nearestAmp = x->amp_in[nearestFreqIndex];
	double cutoff = nearestAmp * .707;
	double amp = mcmc_interp(x->freq_in, 
				 x->amp_in, 
				 freq, 
				 x->numPartials, 
				 x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
				 x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
				 x->interp_alpha, 
				 x->w0);
	if(amp > cutoff){
		return 1;
	}else{
		return 0;
	}
}

int mcmc_findNearestInputFreq(t_mcmc *x, double freq){
	int i;
	int mini = 0;
	double min = DBL_MAX;
	for(i = 0; i < x->numPartials; i++){
		if(abs(x->freq_in[i] - freq) < min){
			min = abs(x->freq_in[i] - freq);
			mini = i;
		}
	}
	return mini;
}

void mcmc_getNewFreq(t_mcmc *x, double currentFreq, double currentProb, double *newFreq, double *newProb){
	double r, proposal_x, proposal_y, ratio, min, max, norm;
	long lr;
	int i = 0;
	double q01, q10;

	if(x->freq_min == x->freq_max){
		*newFreq = currentFreq;
		*newProb = currentProb;
		return;
	}

	/*
	if(currentFreq == 0.f){
		lr = mcmc_randomFromCDF(gsl_rng_uniform(x->rng), x->cdf, x->numPartials);
		*newFreq = x->freq_in[lr];
		*newProb = x->amp_in[lr];
		return;
	}
	*/
	if(x->log) fprintf(x->fp, "******************************\n");
	if(x->log) fprintf(x->fp, "CURRENT FREQ = %f, CURRENT PROB = %f\n\n", currentFreq, currentProb);
	//r = mcmc_gaussdist(x, x->pro_stdev);
	//double pdw = x->proposal_density_width * (1. - mcmc_getAcceptanceRate(x));
	double pdw = x->proposal_density_width;
	int maxTries = 256;
	proposal_x = -1;
	int good = 0;
	norm = mcmc_norm(x->centroid, x->freq_min, x->freq_max);
	min = currentFreq - (norm * (currentFreq * pdw));
	max = currentFreq + ((1 - norm) * (currentFreq * pdw));
	while(i < maxTries && !good){
		r = gsl_ran_beta(x->rng, x->beta_alpha, x->beta_beta);
		PDEBUG("currentFreq = %f, norm = %f, min = %f, max = %f, freq_min = %f, freq_max = %f, cent = %f", currentFreq, norm, min, max, x->freq_min, x->freq_max, x->centroid);
		PDEBUG("pdw = %f", x->proposal_density_width);
		proposal_x = (r * (max - min)) + min;
		good = mcmc_isFreqValid(x, proposal_x);
		PDEBUG("proposal = %f", proposal_x);
		++i;
	}
	if(x->log) fprintf(x->fp, "r = %f, norm = %f, min = %f, max = %f\n", r, norm, min, max);
	if(x->log) fprintf(x->fp, "proposal_x = %f %s\n", proposal_x, mcmc_isFreqValid(x, proposal_x) ? "valid" : "not valid");
	// if we couldn't find a new freq for some reason (stdev is too small, for instance)
	// just choose one from the cdf and bail out.
	if(!good){
		//error("couldn't get a valid freq between %f and %f!", min, max);
		PDEBUG("sampling from CDF");
		//lr = mcmc_randomFromCDF(gsl_rng_uniform(x->rng), x->cdf, x->numPartials);
		lr = mcmc_samplePartialCDF(x, currentFreq);
		*newFreq = x->freq_in[lr];
		*newProb = x->amp_in[lr];
		outlet_anything(x->info_out, ps_cdf, 0, NULL);
		mcmc_rejected(x);
		return;
	}

	// we should have an option to select from the actual spectrum rather than interpolating
	proposal_y = mcmc_interp(x->freq_in, 
				 x->amp_in, 
				 proposal_x, 
				 x->numPartials, 
				 x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
				 x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
				 x->interp_alpha, 
				 x->w0);

	// the proposal density centered around the current frequency and evaluated at the 
	// proposed frequency.
	q10 = gsl_ran_beta_pdf(r, x->beta_alpha, x->beta_beta);

	// the proposal density centered around the proposed frequency and evaluated at the
	// current frequency.
	//post("acceptanceRateSum = %f, %d, %f %f, pdw = %f", x->acceptanceRateSum, MCMC_ACCEPTANCE_RATE_BUF_LEN, mcmc_getAcceptanceRate(x), mcmc_getAcceptanceRate(x) * x->proposal_density_width, pdw);
	if(x->log) fprintf(x->fp, "pdw = %f\n", pdw);
	min = proposal_x - (norm * (proposal_x * pdw));
	max = proposal_x + ((1 - norm) * (proposal_x * pdw));
	double currentFreq_scaled = mcmc_norm(currentFreq, min, max);
	if(x->log) fprintf(x->fp, "norm = %f, min = %f, max = %f, currentFreq_scaled = %f\n", norm, min, max, currentFreq_scaled);
	if(currentFreq_scaled > 0 && currentFreq_scaled < 1){
		q01 = gsl_ran_beta_pdf(currentFreq_scaled, x->beta_alpha, x->beta_beta);
	}else{
		q01 = 0;
	}
	if(x->log) fprintf(x->fp, "stdev = %f, proposal is %f stdevs away, and current is %f\n", x->stdev_amp, x->stdev_amp / proposal_y, x->stdev_amp / currentProb);
	ratio = ((proposal_y / currentProb) * (q01 / q10)) * (1. / pow(1. + (x->stdev_amp / proposal_y), 2.));
	PDEBUG("q01 = %f, q10 = %f, %f, %f %f", q01, q10, q01 / q10, (proposal_y / currentProb), ratio);

	PDEBUG("r = %f, pro_x = %f, pro_y = %f, current_y = %f, ratio = %f * %f = %f", r, proposal_x, proposal_y, currentProb, ((proposal_y / currentProb) * (q01 / q10)), (1. / (x->stdev_amp / proposal_y)), ratio);
	if(x->log) fprintf(x->fp, "currentFreq = %f, currentProb = %f\n", currentFreq, currentProb);
	if(x->log) fprintf(x->fp, "q01 = %f, q10 = %f\n", q01, q10);
	if(x->log) fprintf(x->fp, "r = %f, cf_s = %f\n", r, currentFreq_scaled);
	if(x->log) fprintf(x->fp, "proposal_y = %f, currentProb = %f\n", proposal_y, currentProb);
if(x->log) fprintf(x->fp, "ratio = %f * %f = %f\n", ((proposal_y / currentProb) * (q01 / q10)), (1. / pow(1. + (x->stdev_amp / proposal_y), 2.)), ratio);


	if(ratio >= 1){
		if(x->log) fprintf(x->fp, "ACCEPTED %f as the new frequency\n", proposal_x);
		*newFreq = proposal_x;
		*newProb = proposal_y;
		//return;
	}else{
		if(x->log) fprintf(x->fp, "REJECTED %f as the new frequency\n", proposal_x);
		r = gsl_rng_uniform(x->rng);
		if(x->log) fprintf(x->fp, "new random num: %f\n", r);
		PDEBUG("ratio was less than 1, so here's a new random num: %f", r);
		if(r < ratio){
			if(x->log) fprintf(x->fp, "%f is less than %f so we will accept %f as the new frequency\n", r, ratio, proposal_x);
			PDEBUG("r (%f) < ratio (%f)", r, ratio);
			*newFreq = proposal_x;
			*newProb = proposal_y;
			//return;
		}else{
			if(x->log) fprintf(x->fp, "%f is greater than %f so we will reject %f again\n", r, ratio, proposal_x);
			PDEBUG("r (%f) !< ratio (%f)", r, ratio);
			*newFreq = currentFreq;
			*newProb = currentProb;
			//return;
		}
	}
	if(*newFreq == currentFreq){
		mcmc_rejected(x);
	}else{
		mcmc_accepted(x);
	}
	if(x->log){
		if(*newFreq != currentFreq){
			if(mcmc_isFreqValid(x, *newFreq)){
				fprintf(x->fp, "%f is VALID\n", *newFreq);
			}else{
				fprintf(x->fp, "%f is NOT VALID\n", *newFreq);
			}
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
		if(currentFreq == 0.f){
			currentFreq = gsl_rng_uniform(x->rng);
			currentFreq = (currentFreq * (x->freq_max - x->freq_min)) + x->freq_min;
		}
		currentProb = mcmc_interp(x->freq_in, 
					  x->amp_in, 
					  currentFreq, 
					  //x->numPartials / 2, 
					  x->numPartials,
				 	  x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
					  x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
					  x->interp_alpha, 
					  x->w0);
		if(x->log) fprintf(x->fp, "******************************\n");
		if(x->log) fprintf(x->fp, "%d\n", 0);
		mcmc_getNewFreq(x, currentFreq, currentProb, &newFreq, &newProb);
		PDEBUG("%d %f %f %f", x->counter, currentFreq, newFreq, newProb);
		SETLONG(&a, x->counter);

		i = 0;
		int n = 50;
		int success = 0;
		if(x->freq_min == x->freq_max){
			success = 1;
		}
		while(success == 0 && i < n){
			if(x->log) fprintf(x->fp, "******************************\n");
			if(x->log) fprintf(x->fp, "%d\n", i + 1);
			PDEBUG("%d", i);
			//mcmc_rejected(x);
			mcmc_getNewFreq(x, currentFreq, currentProb, &newFreq, &newProb);
			if(mcmc_quantizeFreq(currentFreq, x->numPartials, x->freq_in) != mcmc_quantizeFreq(newFreq, x->numPartials, x->freq_in)){
				success = 1;
			}
			i++;
		}
		//if(i < (n - 1)){
		if(success){
			//mcmc_accepted(x);
			if(x->sample_mode != 0){
				double qf = mcmc_quantizeFreq(newFreq, x->numPartials, x->freq_in);
				x->newFreq = qf;
				PDEBUG("quantizing %f -> %f", newFreq, qf);
			}else{
				x->newFreq = newFreq;
			}
			mcmc_fadeOut(x);
			x->state = MCMC_STATE_CHANGE_FREQ;
			t_atom kludge[3];
			atom_setlong(kludge, x->counter);
			atom_setfloat(kludge + 1, currentFreq);
			atom_setfloat(kludge + 2, newFreq);
			outlet_list(x->out2, NULL, 3, kludge);
			//post("%f -> %f", currentFreq, newFreq);

		}else{
			//error("couldn't change freq!");
			//mcmc_rejected(x);
			if(x->force_freq_change){
				i = 0;
				x->newFreq = x->freq_in[mcmc_samplePartialCDF(x, currentFreq)];
				post("forcing freq change");
				while(mcmc_quantizeFreq(x->newFreq, x->numPartials, x->freq_in) == mcmc_quantizeFreq(currentFreq, x->numPartials, x->freq_in) && i < 100){
					post("%d %f", i, x->newFreq);
					x->newFreq = x->freq_in[mcmc_samplePartialCDF(x, currentFreq)];
					i++;
				}
				mcmc_fadeOut(x);
				x->state = MCMC_STATE_CHANGE_FREQ;
			}
			mcmc_counter(x);
			if(x->on){
				schedule_fdelay(x, (method)mcmc_clock_cb, x->tinterval, NULL, 0, NULL);
			}
			return;
		}
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

double mcmc_quantizeFreq(double freq, int arrayLen, double *freq_array){
	int i;
	double min = DBL_MAX;
	int mini;
	for(i = 0; i < arrayLen; i++){
		if(fabs(freq - freq_array[i]) < min){
			min = fabs(freq - freq_array[i]);
			mini = i;
		}
	}
	return freq_array[mini];
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

double mcmc_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double mcmc_norm(double x, double min, double max){
	if(max - min == 0){
		return 0.;
	}
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
	post("w0: %.20f", x->w0);
	post("interp_alpha: %.20f", x->interp_alpha);
}

void mcmc_errorHandler (const char * reason, const char * file, int line, int gsl_errno){
	error("mcmc: a(n) %s has occured in file %s at line %d (error %d)", reason, file, line, gsl_errno);
}

void mcmc_interp_test(t_mcmc *x, double f){
	float y = mcmc_interp(x->freq_in, 
				  x->amp_in, 
				  f, 
				  //x->numPartials / 2, 
				  x->numPartials,
				  x->freq_min * MST < 0. ? 0. : x->freq_min * MST, 
				  x->freq_max * PST > 22050. ? 22020. : x->freq_max * PST, 
				  x->interp_alpha, 
				  x->w0);

	t_atom a[2];
	atom_setfloat(a, f);
	atom_setfloat(a + 1, y);
	outlet_anything(x->info_out, gensym("interp"), 2, a);
	//post("%.30f", y);
}

void mcmc_log(t_mcmc *x, long l){
	if(l){
		x->fp = fopen("/Users/johnmac/mcmc.txt", "w");
	}else{
		fclose(x->fp);
	}
	x->log = l;
}
