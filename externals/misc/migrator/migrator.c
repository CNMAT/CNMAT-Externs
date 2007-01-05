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
NAME: migrator
DESCRIPTION: Spectral harmony a la David Wessel
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: Universal Binary
VERSION 1.0.1: Added mig_oscamp and a better tellmeeverything function
VERSION 1.0.2: The number of oscillators specified as an argument is now recognized.
VERSION 1.0.3: GPL compatible license
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "math.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef struct _mig
{
	t_object m_ob;
	long m_in0;
	void *m_out1;
	void *m_out2;
	void *m_out3;
	t_atom *m_arrayIn;
	float *m_decayRates;
	short m_arrayInLength;
	long m_nOsc;
	float *m_pmf;
	t_atom *m_arrayOut;
	long m_counter;
	void *m_clock1;
	void *m_clock2;
	void *m_forcefeed_clock1;
	void *m_forcefeed_clock2;
	double m_tinterval;
	double m_oscamp;
	gsl_rng *m_rng;
	double m_var;
	long m_on;
	//long m_idum[1];
	//long m_manCounter;
	long m_fade;
	int m_waitingToChangeNumOsc[2];
	int m_theyAreResonances;
	float m_decayTime;
} t_mig;

void *mig_class;

void mig_anything(t_mig *x, t_symbol *msg, short argc, t_atom *argv);
void mig_list(t_mig *x, t_symbol *msg, short argc, t_atom *argv);
void mig_nOsc(t_mig *x, long n);
void mig_nOsc_smooth(t_mig *x, long n);
void mig_var(t_mig *x, double var);
void mig_bang(t_mig *x);
void mig_fadeOut(t_mig *x);
void mig_changeFreq(t_mig *x);
void mig_fadeIn(t_mig *x);
void mig_int(t_mig *x, long n);
void mig_assist(t_mig *x, void *b, long m, long a, char *s);
void *mig_new(double var, long nOsc, double oscamp);
void mig_free(t_mig *x);
void makePMF(t_mig *x);
int randPMF(t_mig *x);
long counter(t_mig *x);
void mig_probDecay(t_mig *x);
float gaussBlur(t_mig *x, float m);
void forcefeed(t_mig *x);
void forcefeed_out(t_mig *x);
void forcefeed_change(t_mig *x);
void forcefeed_in(t_mig *x);
//float ran1(long *idum);
//float gasdev(long *idum);
void mig_auto(t_mig *x, long a);
void mig_fade(t_mig *x, long n);
void mig_tinterval(t_mig *x, double n);
void mig_oscamp(t_mig *x, double a);
void tellmeeverything(t_mig *x);

//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&mig_class, (method)mig_new, (method)mig_free, (short)sizeof(t_mig), 0L, A_DEFFLOAT, A_DEFLONG, A_DEFFLOAT, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addbang((method)mig_bang);
	addint((method)mig_int);
	addmess((method)mig_anything, "anything", A_GIMME, 0);
	addmess((method)mig_list, "list", A_GIMME, 0);
	addmess((method)mig_nOsc, "nOsc", A_LONG, 0);
	addmess((method)mig_nOsc_smooth, "nOsc_smooth", A_LONG, 0);
	addmess((method)mig_var, "var", A_FLOAT, 0);
	addmess((method)mig_assist, "assist", A_CANT, 0);
	addmess((method)mig_fade, "fade", A_LONG, 0);
	addmess((method)mig_tinterval, "time_interval", A_FLOAT, 0);
	addmess((method)mig_oscamp, "oscamp", A_FLOAT, 0);
	addmess((method)tellmeeverything, "tellmeeverything", 0L, 0);
		
	return 0;
}

//--------------------------------------------------------------------------

void mig_anything(t_mig *x, t_symbol *msg, short argc, t_atom *argv)
{
	if(msg){
		if(!strcmp(msg->s_name, "forcefeed")){
			mig_list(x, msg, argc, argv);
			forcefeed(x);	
		}else if(!strcmp(msg->s_name, "oscamp")){
			x->m_oscamp = argv[0].a_w.w_float;
		}
	}
	else {
	}
}

void mig_list(t_mig *x, t_symbol *msg, short argc, t_atom *argv)
{
	/*
	if(((float)argc / 2.) - round((float)argc / 2.)){
		error("migrator: multiple of 2 floats required (frequency amplitude)");
		return;
	}*/
	
	x->m_theyAreResonances = 1;
	// heuristic to figure out whether the list is f/g pairs or f/g/r triples
	// Taken from Adrian Freed's res-display.js
	if((argc % 2)==0){
		if((argc % 3)==0){
			float av = 0.0;
			int n = 0;
			int i;
			for(i = 0; i < argc; i++){
				if(((i % 2) == 0) && ((i % 3) > 0)){
					++n;
					av += argv[i].a_w.w_float;
				}
			}
			if(n > 0 && ((av / n) > 30))
				x->m_theyAreResonances = 0;
		}
		else x->m_theyAreResonances = 0;
	}
	
	int i;
	if(x->m_theyAreResonances){
		post("%d resonances", argc / 3);
		x->m_decayTime = 0.;
		x->m_arrayIn = (t_atom *)realloc(x->m_arrayIn, ((argc / 3) * 2) * sizeof(t_atom));
		x->m_decayRates = (float *)realloc(x->m_decayRates, (argc / 3) * sizeof(float));
		for(i = 0; i < argc / 3; i++){
			x->m_arrayIn[(i * 2)] = argv[(i * 3)];
			x->m_arrayIn[(i * 2) + 1] = argv[(i * 3) + 1];
			x->m_decayRates[i] = argv[(i * 3) + 3].a_w.w_float;
			x->m_arrayInLength = (argc / 3) * 2;
			makePMF(x);
			x->m_arrayOut[(i * 2)].a_w.w_float = x->m_arrayIn[randPMF(x) * 2].a_w.w_float;
			x->m_arrayOut[(i * 2) + 1].a_w.w_float = x->m_oscamp;
			//SETFLOAT(x->m_arrayOut + ((i * 2) + 1), x->m_oscamp);
		}
	}else{
		x->m_arrayIn = (t_atom *)realloc(x->m_arrayIn, argc * sizeof(t_atom));
		memcpy(x->m_arrayIn, argv, argc * sizeof(t_atom));
		x->m_arrayInLength = argc;
		makePMF(x);
	}
}

void mig_bang(t_mig *x)
{
	if(x->m_on) return;
	if(!x->m_arrayIn) return;
		
	mig_fadeOut(x);
	clock_fdelay(x->m_clock1, x->m_tinterval);
	clock_fdelay(x->m_clock2, x->m_tinterval * 2.);
}

void mig_fadeOut(t_mig *x)
{
	int i;
	for(i = 0; i < x->m_fade; i++){
		x->m_arrayOut[(((x->m_counter + i) % x->m_nOsc) * 2) + 1].a_w.w_float = x->m_oscamp * i / x->m_fade;
	}
	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);
	
	if(x->m_on)
		clock_fdelay(x->m_clock1, x->m_tinterval);
}

void mig_changeFreq(t_mig *x)
{	
	int r = randPMF(x);
	float freq = x->m_arrayIn[(r * 2)].a_w.w_float;
	
	if(x->m_waitingToChangeNumOsc[0])
		x->m_arrayOut[(x->m_counter * 2)].a_w.w_float = 0.;
	else
		x->m_arrayOut[(x->m_counter * 2)].a_w.w_float = gaussBlur(x, freq);
	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);

	if(x->m_on)
		clock_fdelay(x->m_clock2, x->m_tinterval);
}

void mig_fadeIn(t_mig *x)
{
	int i, n;
	
	if(x->m_waitingToChangeNumOsc[0])
		x->m_arrayOut[(x->m_counter * 2) + 1].a_w.w_float = 0.;
	else{
		for(i = 0; i < x->m_fade; i++){
			n = ((x->m_counter - i) < 0) ? x->m_nOsc + (x->m_counter - i) : (x->m_counter - i);
			x->m_arrayOut[(n * 2) + 1].a_w.w_float = x->m_oscamp * (i + 1) / x->m_fade;
		}
	}	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);
	outlet_int(x->m_out2, x->m_counter);
		
	counter(x);
	if(x->m_theyAreResonances){
		//post("%f", x->m_oscamp);
		mig_probDecay(x);
		x->m_oscamp = x->m_oscamp * exp(-1. * 1000 * x->m_decayTime);
	}
	if(x->m_on) mig_fadeOut(x);
}

//--------------------------------------------------------------------------

void mig_int(t_mig *x, long n)
{
	if(!x->m_arrayIn) return;
	
	x->m_on = n;
	
	if(n){
		mig_fadeOut(x);
		return;
	}
	
	int i = 0;
	t_atom ar[2];
	SETFLOAT(ar, 0.0);
	SETFLOAT(ar + 1, 0.0);
	
	clock_unset(x->m_clock1);
	clock_unset(x->m_clock2);
	for(i = 0; i < (int)x->m_nOsc * 2; i++){
		x->m_arrayOut[i].a_w.w_float = 0.0;
	}
	outlet_list(x->m_out1, 0, 2, ar);
}

//--------------------------------------------------------------------------

void mig_assist(t_mig *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"(List) %ld frequency amplitude pairs", x->m_nOsc);
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(List) frequency amplitude pairs");
			break;
		case 1:
			sprintf(s,"Index number of the partial being updated");
			break;
		case 2:
			sprintf(s, "Bang when the index number wraps around to 0");
			break;
		}
	}
}

//--------------------------------------------------------------------------

void *mig_new(double var, long nOsc, double oscamp)
{
	t_mig *x;

	x = (t_mig *)newobject(mig_class); // create a new instance of this object
	
	x->m_out3 = bangout(x);
	x->m_out2 = intout(x);
	x->m_out1 = listout(x);	
	
	x->m_counter = 0;
	x->m_waitingToChangeNumOsc[0] = 0;
	x->m_waitingToChangeNumOsc[1] = 0;
	//x->m_manCounter = 0;
	
	if(nOsc)
		x->m_nOsc = (long)nOsc;
	else
		x->m_nOsc = 200;
	
	x->m_arrayOut = (t_atom *)calloc((int)x->m_nOsc * 2, sizeof(t_atom));
	x->m_arrayIn = (t_atom *)calloc(10, sizeof(t_atom));
	x->m_decayRates = (float *)calloc(10, sizeof(float));
	x->m_pmf = (float *)calloc(10, sizeof(float));

	int i = 0;
	for(i = 0; i < (int)x->m_nOsc * 2; i++){
		SETFLOAT(x->m_arrayOut + i, 0.0);
	}
	
	x->m_clock1 = clock_new(x, (method)mig_changeFreq);
	x->m_clock2 = clock_new(x, (method)mig_fadeIn);
	x->m_forcefeed_clock1 = clock_new(x, (method)forcefeed_change);
	x->m_forcefeed_clock2 = clock_new(x, (method)forcefeed_in);
	x->m_tinterval = 25.0; //ms
	
	if(oscamp) 
		x->m_oscamp = oscamp;
	else 
		x->m_oscamp = 0.03;
	
	x->m_var = var;
	x->m_fade = 1;
	x->m_on = 0;
	
	gsl_rng_env_setup();
	x->m_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_default);
	
	gsl_rng_set(x->m_rng, rand());
	
	//x->m_idum[0] = rand() * -1;
	   	
	return(x);
}

void mig_free(t_mig *x)
{
	clock_unset(x->m_clock1);
	clock_unset(x->m_clock2);
	
	free(x->m_arrayIn);
	free(x->m_arrayOut);
	free(x->m_pmf);
	freeobject((t_object *)x->m_clock1);
	freeobject((t_object *)x->m_clock2);
	gsl_rng_free(x->m_rng);
}

//--------------------------------------------------------------------------

void makePMF(t_mig *x)
{
	int i = 0;
	float sum = 0;
		
	for(i = 0; i < x->m_arrayInLength / 2; i++){
		sum = sum + x->m_arrayIn[(i * 2) + 1].a_w.w_float;
	}
	
	x->m_pmf = (float *)realloc(x->m_pmf, x->m_arrayInLength / 2 * sizeof(float));	
	
	for(i = 0; i < x->m_arrayInLength / 2; i++)
		x->m_pmf[i] = x->m_arrayIn[(i * 2) + 1].a_w.w_float / sum;
	
}

int randPMF(t_mig *x)
{
	int lastIndex, i;
	double u, sum;

	lastIndex = (x->m_arrayInLength / 2) - 1;
	//u = random() / (pow(2, 31) - 1);
	u = gsl_rng_uniform(x->m_rng);
	//u = ran1(x->m_idum);
	i = 0;
	sum = ((float *)(x->m_pmf))[0];
	while((i < lastIndex) && (u > sum)){
		i++;
		sum += ((float *)(x->m_pmf))[i];
	}
	return i;
}

long counter(t_mig *x)
{	
	if(x->m_waitingToChangeNumOsc[0]){
		if(x->m_counter == x->m_nOsc - 1){
			x->m_nOsc = x->m_waitingToChangeNumOsc[1];
			x->m_arrayOut = realloc(x->m_arrayOut, (2 * x->m_nOsc) * sizeof(t_atom));
			x->m_waitingToChangeNumOsc[0] = 0;
		}
	}
	int oldcounter = x->m_counter;
	x->m_counter = (x->m_counter + 1) % x->m_nOsc;	
	
	if(oldcounter > x->m_counter)
		outlet_bang(x->m_out3);
	
	//if(x->m_counter == 0) mig_probDecay(x);
	
	return x->m_counter;
}

void mig_probDecay(t_mig *x)
{	
	int i;
	
	for(i = 0; i < x->m_arrayInLength / 2; i++){
		x->m_arrayIn[(i * 2) + 1].a_w.w_float = x->m_arrayIn[(i * 2) + 1].a_w.w_float * expf(-1. * x->m_decayRates[i] * x->m_decayTime);
	}
	makePMF(x);
	x->m_decayTime = x->m_decayTime + .00000005;
}

float gaussBlur(t_mig *x, float m)
{
	//return m * (pow(pow(2., 1./12.), gasdev(x->m_idum) * x->m_var));
	return m * (pow(pow(2., 1./12.), gsl_ran_gaussian(x->m_rng, x->m_var)));
}

void forcefeed(t_mig *x)
{
	forcefeed_out(x);
}

void forcefeed_out(t_mig *x)
{
	int i;
	for(i = 0; i < (int)x->m_nOsc; i++){
		//x->m_arrayOut[(i * 2)].a_w.w_float = 0.0;
		x->m_arrayOut[(i * 2) + 1].a_w.w_float = 0.0;
	}
	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);
	clock_fdelay(x->m_forcefeed_clock1, x->m_tinterval * 2);
}

void forcefeed_change(t_mig *x)
{
	int i;
	for(i = 0; i < (int)x->m_nOsc; i++){
		x->m_arrayOut[(i * 2)].a_w.w_float = x->m_arrayIn[randPMF(x) * 2].a_w.w_float;
	}
	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);
	clock_fdelay(x->m_forcefeed_clock2, x->m_tinterval * 2);
}

void forcefeed_in(t_mig *x)
{
	int i;
	for(i = 0; i < (int)x->m_nOsc; i++){
		x->m_arrayOut[(i * 2) + 1].a_w.w_float = x->m_oscamp;
	}
	
	outlet_list(x->m_out1, 0, (short)x->m_nOsc * 2, x->m_arrayOut);
}

/*
float ran1(long *idum) 
{
	int j; 
	long k; 
	static long iy=0; 
	static long iv[NTAB]; 
	float temp; 
	if (*idum <= 0|| !iy) { //Initialize. 
		if (-(*idum) < 1) *idum=1; //Be sure to prevent idum=0. 
		else *idum =-(*idum); 
		for (j=NTAB+7;j>=0;j--) { //Load the shuffle table (after 8 warm-ups). 
			k=(*idum)/IQ; 
			*idum=IA*(*idum-k*IQ)-IR*k; 
			if(*idum < 0) *idum += IM; 
			if(j < NTAB) iv[j] = *idum; 
		} 
		iy=iv[0]; 
	} 
	k=(*idum)/IQ; //Start here when not initializing. 
	*idum=IA*(*idum-k*IQ)-IR*k; //Compute idum=(IA*idum) % IM without overflows by Schrage’s method.
	if (*idum <0) *idum += IM; 
	j=iy/NDIV; //Will be in the range 0..NTAB-1. 
	iy=iv[j]; //Output previously stored value and refill the shuffle table.
	iv[j] =*idum; 
	if ((temp=AM*iy) >RNMX) return RNMX; //Because users don’t expect end point values. 
	else return temp; 
} 

float gasdev(long *idum)
{ 
	float ran1(long *idum); 
	static int iset=0; 
	static float gset; 
	float fac,rsq,v1,v2; 
	if (*idum <0) iset=0; //Reinitialize. 
	if (iset == 0) { //We don’t have an extra deviate handy, so 
		do { 
			v1=2.0*ran1(idum)-1.0; //pick two uniform numbers in the square extending from -1 to +1 in each direction,
			v2=2.0*ran1(idum)-1.0; 
			rsq=v1*v1+v2*v2; //see if they are in the unit circle,
		}while (rsq >= 1.0 || rsq == 0.0); //and if they are not, try again. 
		fac=sqrt(-2.0*log(rsq)/rsq); //Now make the Box-Muller transformation to get two normal deviates. 
										//Return one and save the other for next time. 
		gset=v1*fac; 
		iset=1; //Set flag. 
		return v2*fac; 
	}else { //We have an extra deviate handy, 
		iset=0; //so unset the flag, 
		return gset; //and return it. 
	} 
} 
*/

///////////////////////////////////////////////////////////////////////////////////////////
// OPTIONS

void mig_nOsc(t_mig *x, long n)
{
	if(n < 1){
		error("migrator: the number of oscillators must be greater than 0");
		return;
	}
	
	if(x->m_fade >= n){
		error("migrator: fade must be less than nOsc.  Setting fade to %f", floor((double)n / 2));
		x->m_fade = (long)floor((double)n / 2);
	}
	
	int oldNumOsc = x->m_nOsc;
	int i;
	
	if(n < (int)x->m_nOsc)
		x->m_counter = 0.;
	x->m_nOsc = n;
	x->m_arrayOut = realloc(x->m_arrayOut, (2 * n) * sizeof(t_atom));
	
	if(x->m_nOsc > oldNumOsc){
		for(i = oldNumOsc; i < x->m_nOsc; i++){
			x->m_arrayOut[(i * 2)].a_w.w_float = x->m_arrayIn[randPMF(x) * 2].a_w.w_float;
			x->m_arrayOut[(i * 2) + 1].a_w.w_float = x->m_oscamp;
		}
	}
}

void mig_nOsc_smooth(t_mig *x, long n)
{
	if(n < 1){
		error("migrator: the number of oscillators must be greater than 0");
		return;
	}
	
	if(x->m_fade >= n){
		error("migrator: fade must be less than nOsc.  Setting fade to %f", floor((double)n / 2));
		x->m_fade = (long)floor((double)n / 2);
	}
		
	if(n < (int)x->m_nOsc){	
		x->m_counter = n;
	
		x->m_waitingToChangeNumOsc[0] = 1;
		x->m_waitingToChangeNumOsc[1] = (int)n;
		return;
	}
	
	int i;
	int oldNumOsc = x->m_nOsc;
	if(oldNumOsc < n){
		x->m_counter = oldNumOsc;
		x->m_nOsc = n;
		x->m_arrayOut = realloc(x->m_arrayOut, (2 * n) * sizeof(t_atom));
		for(i = oldNumOsc * 2; i < x->m_nOsc * 2; i++){
			x->m_arrayOut[i].a_w.w_float = 0.;
		}
	}
}

void mig_var(t_mig *x, double var)
{
	if(var < 0.){
		error("migrator: var must be positive");
		return;
	}
	
	x->m_var = var;
}

void mig_fade(t_mig *x, long n)
{
	if(n < 1){
		error("migrator: fade must be greater than zero");
		return;
	}
	if(n > x->m_nOsc){
		error("migrator: fade must be less than nOsc");
		return;
	}
	x->m_fade = n;
}

void mig_tinterval(t_mig *x, double n){
	x->m_tinterval = n;
}

void mig_oscamp(t_mig *x, double a){
	x->m_oscamp = a;
}

void tellmeeverything(t_mig *x)
{
	version(0);
	post("Migrator is %s", (x->m_on) ? "ON" : "OFF");
	post("Number of oscillators: %ld", x->m_nOsc);
	post("Oscillator amplitude: %f", x->m_oscamp);
	post("Time interval: %f", x->m_tinterval);
	post("Fade: %ld", x->m_fade);
	post("Variance (Gaussian blur): %f", x->m_var);
	if(x->m_theyAreResonances) post("Migrator thinks your data is in frequency / amplitude / decayrate triples");
	post("Number of elements in timbre: %d", x->m_arrayInLength);
	post("First 10 elements from timbre:");
	int i;
	for(i = 0; i < 10; i++){
		if(x->m_theyAreResonances)
			post("%f %f %f", x->m_arrayIn[i * 3].a_w.w_float, x->m_arrayIn[(i * 3) + 1].a_w.w_float, x->m_arrayIn[(i * 3) + 2].a_w.w_float);
		else post("%f %f", x->m_arrayIn[i * 2].a_w.w_float, x->m_arrayIn[(i * 2) + 1].a_w.w_float);
	}
		
}
