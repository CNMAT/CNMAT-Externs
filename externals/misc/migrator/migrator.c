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
COPYRIGHT_YEARS: 2006-07
DRUPAL_NODE: /patch/4060
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: Universal Binary
VERSION 1.0.1: Added mig_oscamp and a better tellmeeverything function
VERSION 1.0.2: The number of oscillators specified as an argument is now recognized.
VERSION 1.0.3: GPL compatible license
VERSION 1.0.4: Added three different output modes (concatenate, f/a pairs, only updated).
VERSION 1.0.5: Got rid of the third output mode and added an outlet that outputs only the changed frequency for each update.
VERSION 1.0.6: Unlimited list length.
VERSION 1.0.7: Now likes lists of ints too!
VERSION 1.1: Reads the contents of SDIF-buffers.
VERSION 1.1.1: Fixed helpfile
VERSION 1.1.2: Now uses SETFLOAT to set the contents of atoms.
VERSION 1.1.3: SDIF support fixed.
VERSION 2.0: Major overhaul.  Lots of bugfixes.  SDIF support temporarily removed.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "commonsyms.h"
#include "version.c"
#include "math.h"

#define MIG_MAX_NUM_OSC 2048
#define MIG_MAX_INPUT 2048

// for ran1 from Numerical Rec. in C
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS (1.2E-07)
#define RNMX (1.0-EPS)
#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b)?a:b 

typedef struct _mig{
	t_object ob;
	void *out1;
	void *out2;
	void *out3;
	void *out4;
	t_atom *arrayIn;
	long arrayInLength;
	long nOsc;
	float *pmf;
	t_atom *arrayOut;
	long counter;
	void *clock1;
	void *clock2;
	void *forcefeed_clock1;
	void *forcefeed_clock2;
	float tinterval;
	float oscamp;
	long idum; // for ran1
	float stdev;
	long on;
	long fade;
	long waitingToChangeNumOsc[2];
	long outputType;
} t_mig;

void *mig_class;

void *mig_new(t_symbol *sym, long argc, t_atom *argv);
void mig_free(t_mig *x);
void mig_assist(t_mig *x, void *b, long m, long a, char *s);

void mig_list(t_mig *x, t_symbol *msg, long argc, t_atom *argv);
t_max_err mig_nOsc(t_mig *x, t_object *attr, long argc, t_atom *argv);
void mig_nOsc_smooth(t_mig *x, long n);
t_max_err mig_var_get(t_mig *x, t_object *attr, long *argc, t_atom **argv);
t_max_err mig_var_set(t_mig *x, t_object *attr, long argc, t_atom *argv);
void mig_bang(t_mig *x);
void mig_fadeOut(t_mig *x);
void mig_changeFreq(t_mig *x);
void mig_fadeIn(t_mig *x);
void mig_outputList(t_mig *x, short length, t_atom *array);
void mig_int(t_mig *x, long n);
t_max_err mig_fade(t_mig *x, t_object *attr, long argc, t_atom *argv);

void mig_makePMF(t_mig *x);
int mig_randPMF(t_mig *x);
long mig_counter(t_mig *x);
float mig_gaussBlur(t_mig *x, float m, int r);
void mig_forcefeed(t_mig *x, t_symbol *sym, long argc, t_atom *argv);
void mig_forcefeed_out(t_mig *x);
void mig_forcefeed_change(t_mig *x);
void mig_forcefeed_in(t_mig *x);
float mig_ran1(long *idum);
float mig_gasdev(long *idum);
void mig_auto(t_mig *x, long a);
void mig_tellmeeverything(t_mig *x);

int main(void){
	t_class *c = class_new("migrator", (method)mig_new, (method)mig_free, sizeof(t_mig), (method)0L, A_GIMME, 0); 
	//common_symbols_init();

	t_object *attr;
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)mig_bang, "bang", 0);
	class_addmethod(c, (method)mig_int, "int", 0);
	class_addmethod(c, (method)mig_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)mig_forcefeed, "forcefeed", A_GIMME, 0);
	class_addmethod(c, (method)mig_nOsc_smooth, "nOsc_smooth", A_LONG, 0);
	class_addmethod(c, (method)mig_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)mig_tellmeeverything, "tellmeeverything", 0L, 0);

	CLASS_ATTR_LONG(c, "nOsc", 0, t_mig, nOsc);
	attr = (t_object *)class_attr_get(c, gensym("nOsc"));
	object_method(attr, gensym("setmethod"), USESYM(set), mig_nOsc);

	CLASS_ATTR_FLOAT(c, "stdev", 0, t_mig, stdev);

	CLASS_ATTR_FLOAT(c, "var", 0, t_mig, stdev);
	CLASS_ATTR_ACCESSORS(c, "var", mig_var_get, mig_var_set);

	CLASS_ATTR_LONG(c, "fade", 0, t_mig, fade);
	attr = (t_object *)class_attr_get(c, gensym("fade"));
	object_method(attr, gensym("setmethod"), USESYM(set), mig_fade);

	CLASS_ATTR_FLOAT(c, "time_interval", 0, t_mig, tinterval);
	CLASS_ATTR_FLOAT(c, "oscamp", 0, t_mig, oscamp);

	CLASS_ATTR_LONG(c, "output", 0, t_mig, outputType);
	CLASS_ATTR_FILTER_CLIP(c, "output", 0, 1);

	class_register(CLASS_BOX, c);
	mig_class = c;
	version(0);

	return 0;
}

void *mig_new(t_symbol *sym, long argc, t_atom *argv){
	t_mig *x;
	int i = 0;

	if(x = (t_mig *)object_alloc(mig_class)){
		post("%p", x);

		x->arrayIn = (t_atom *)calloc(MIG_MAX_INPUT, sizeof(t_atom));
		x->arrayOut = (t_atom *)calloc(MIG_MAX_NUM_OSC, sizeof(t_atom));
		x->pmf = (float *)calloc(MIG_MAX_INPUT, sizeof(t_atom));

		x->out4 = bangout(x);
		x->out3 = intout(x);
		x->out2 = floatout(x);
		x->out1 = listout(x);	
	
		x->counter = 0;
		x->waitingToChangeNumOsc[0] = 0;
		x->waitingToChangeNumOsc[1] = 0;

		x->stdev = 0.0f;
		x->nOsc = 100;
		x->oscamp = 0.03f;

		/*
		if(argc > 0){
			if(argv[0].a_type == A_FLOAT) x->stdev = argv[0].a_w.w_float;
			else x->stdev = (float)(argv[0].a_w.w_long);
		}
		if(argc > 1){
			if(argv[1].a_type == A_FLOAT) x->nOsc = (int)(argv[1].a_w.w_float);
			else x->nOsc = argv[1].a_w.w_long;
		}
		if(argc > 2){
			if(argv[2].a_type == A_FLOAT) x->oscamp = argv[2].a_w.w_float;
			else x->oscamp = (float)(argv[2].a_w.w_long);
		}
		*/

		for(i = 0; i < MIG_MAX_NUM_OSC; i++){
			SETFLOAT(&(x->arrayOut[i]), 0.0);
		}

		x->clock1 = clock_new(x, (method)mig_changeFreq);
		x->clock2 = clock_new(x, (method)mig_fadeIn);
		x->forcefeed_clock1 = clock_new(x, (method)mig_forcefeed_change);
		x->forcefeed_clock2 = clock_new(x, (method)mig_forcefeed_in);
		x->tinterval = 25.0; //ms

		x->fade = 1;
		x->on = 0;
		x->outputType = 0;

		x->idum = rand() * -1;

		attr_args_process(x, argc, argv);
	}
	return x;
}

void mig_free(t_mig *x){
	clock_unset(x->clock1);
	clock_unset(x->clock2);
	free(x->arrayIn);
	free(x->pmf);
	free(x->arrayOut);
       	freeobject((t_object *)x->clock1);
	freeobject((t_object *)x->clock2);
}


void mig_assist(t_mig *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"(List) %ld frequency amplitude pairs", x->nOsc);
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(List) frequency amplitude pairs");
			break;
		case 1:
			sprintf(s,"Only the updated partial");
			break;
		case 2:
			sprintf(s,"Index number of the partial being updated");
			break;
		case 3:
			sprintf(s, "Bang when the index number wraps around to 0");
			break;
		}
	}
}

void mig_list(t_mig *x, t_symbol *msg, long argc, t_atom *argv){
	if(argc % 2){
		error("migrator: multiple of 2 floats required (frequency amplitude)");
		return;
	}
	if(argc > MIG_MAX_NUM_OSC){
		error("migrator: maximum list length is %d", MIG_MAX_NUM_OSC);
		return;
	}
	
	// memcpy would be better, but this is so we don't have to do 
	// typechecking throughout the rest of the program.
	int i;
	for(i = 0; i < argc; i++){
		if(argv[i].a_type == A_FLOAT)
			x->arrayIn[i] = argv[i];
		else
			SETFLOAT(&(x->arrayIn[i]), (float)(argv[i].a_w.w_long));
	}
	//memcpy(x->arrayIn, argv, argc * sizeof(t_atom));
	x->arrayInLength = argc;
	mig_makePMF(x);

}

void mig_bang(t_mig *x){
	if(x->on) return;
		
	mig_fadeOut(x);
	clock_fdelay(x->clock1, x->tinterval);
	clock_fdelay(x->clock2, x->tinterval * 2.);
}

void mig_fadeOut(t_mig *x){
	int i;
	for(i = 0; i < x->fade; i++){
		SETFLOAT(&(x->arrayOut[((((x->counter + i) % x->nOsc) * 2) + 1)]), x->oscamp * i / x->fade);
	}
	
	//outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
	mig_outputList(x, (short)x->nOsc * 2, x->arrayOut);
	
	if(x->on){
		clock_fdelay(x->clock1, x->tinterval);
	}
}

void mig_changeFreq(t_mig *x){	
	int r = mig_randPMF(x);
	float freq = x->arrayIn[(r * 2)].a_w.w_float;
	
	if(x->waitingToChangeNumOsc[0]){
		SETFLOAT(&(x->arrayOut[(x->counter * 2)]), 0.);
	}else{
		if(x->stdev){
			SETFLOAT(&(x->arrayOut[(x->counter * 2)]), mig_gaussBlur(x, freq, r));
		}else{
			SETFLOAT(&(x->arrayOut[(x->counter * 2)]), freq);
		}
	}
	
	//outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
	outlet_float(x->out2, freq);
	mig_outputList(x, (short)x->nOsc * 2, x->arrayOut);

	if(x->on){
		clock_fdelay(x->clock2, x->tinterval);
	}
}

void mig_fadeIn(t_mig *x){
	int i, n;
	if(x->waitingToChangeNumOsc[0]){
		SETFLOAT(&(x->arrayOut[((x->counter * 2) + 1)]), 0.);
	}else{
		for(i = 0; i < x->fade; i++){
			n = ((x->counter - i) < 0) ? x->nOsc + (x->counter - i) : (x->counter - i);
			SETFLOAT(&(x->arrayOut[((n * 2) + 1)]), x->oscamp * (i + 1) / x->fade);
		}
	}	
	//outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
	mig_outputList(x, (short)x->nOsc * 2, x->arrayOut);
	outlet_int(x->out3, x->counter);
		
	mig_counter(x);
	if(x->on){
		mig_fadeOut(x);
	}
}

void mig_outputList(t_mig *x, short length, t_atom *array){
	int i;
	t_atom tmp[2];
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
}

void mig_int(t_mig *x, long n){
	int i = 0;
	t_atom ar[2];
	x->on = n;	
	if(n){
		mig_fadeOut(x);
		return;
	}
	
	SETFLOAT(&(ar[0]), 0.0);
	SETFLOAT(&(ar[1]), 0.0);
	
	clock_unset(x->clock1);
	clock_unset(x->clock2);
	for(i = 0; i < (int)x->nOsc * 2; i++){
		SETFLOAT(&(x->arrayOut[i]), 0.0);
	}
	outlet_list(x->out1, 0, 2, ar);
}

void mig_makePMF(t_mig *x){
	int i = 0;
	float sum = 0;
		
	for(i = 0; i < x->arrayInLength / 2; i++){
		sum = sum + x->arrayIn[(i * 2) + 1].a_w.w_float;
	}
	
	for(i = 0; i < x->arrayInLength / 2; i++){
		x->pmf[i] = x->arrayIn[(i * 2) + 1].a_w.w_float / sum;
	}
}

int mig_randPMF(t_mig *x){
	int lastIndex, i;
	double u, sum;
	lastIndex = (x->arrayInLength / 2) - 1;
	u = mig_ran1(&(x->idum));
	i = 0;
	sum = x->pmf[0];
	while((i < lastIndex) && (u > sum)){
		i++;
		sum += x->pmf[i];
	}
	return i;
}

long mig_counter(t_mig *x){	
	int oldcounter;

	if(x->waitingToChangeNumOsc[0]){
		if(x->counter == x->nOsc - 1){
			x->nOsc = x->waitingToChangeNumOsc[1];
			x->waitingToChangeNumOsc[0] = 0;
		}
	}
	oldcounter = x->counter;
	x->counter = (x->counter + 1) % x->nOsc;	
	
	if(oldcounter > x->counter){
		outlet_bang(x->out4);
	}
	
	return x->counter;
}

float mig_gaussBlur(t_mig *x, float m, int r){
	return m * (pow(pow(2., 1./12.), x->stdev * mig_gasdev(&(x->idum))));
}

void mig_forcefeed(t_mig *x, t_symbol *sym, long argc, t_atom *argv){
	mig_list(x, sym, argc, argv);
	mig_forcefeed_out(x);	
}

void mig_forcefeed_out(t_mig *x){
	int i;
	for(i = 0; i < (int)x->nOsc; i++){
		SETFLOAT(&(x->arrayOut[((i * 2) + 1)]), 0.0);
	}
	
	outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
	clock_fdelay(x->forcefeed_clock1, x->tinterval * 2);
}

void mig_forcefeed_change(t_mig *x){
	int i;
	for(i = 0; i < (int)x->nOsc; i++){
		x->arrayOut[i * 2] = x->arrayIn[mig_randPMF(x) * 2];
		//SETFLOAT(&(x->arrayOut[(i * 2)]), x->arrayIn[mig_randPMF(x) * 2].a_w.w_float);
	}
	
	outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
	clock_fdelay(x->forcefeed_clock2, x->tinterval * 2);
}

void mig_forcefeed_in(t_mig *x){
	int i;
	for(i = 0; i < (int)x->nOsc; i++){
		SETFLOAT(&(x->arrayOut[((i * 2) + 1)]), x->oscamp);
	}
	
	outlet_list(x->out1, 0, (short)x->nOsc * 2, x->arrayOut);
}

//////////////////////////////////////////////////
// Box-Mueller Gaussian from Num Rec in C
float mig_ran1(long *idum) {
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

float mig_gasdev(long *idum){ 
	float mig_ran1(long *idum); 
	static int iset=0; 
	static float gset; 
	float fac,rsq,v1,v2; 
	if (*idum <0) iset=0; //Reinitialize. 
	if (iset == 0) { //We don’t have an extra deviate handy, so 
		do { 
			v1=2.0*mig_ran1(idum)-1.0; //pick two uniform numbers in the square extending from -1 to +1 in each direction,
			v2=2.0*mig_ran1(idum)-1.0; 
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

///////////////////////////////////////////////////////////////////////////////////////////
// OPTIONS

t_max_err mig_nOsc(t_mig *x, t_object *attr, long argc, t_atom *argv){
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}
	long n = atom_getlong(argv);
	if(n > MIG_MAX_NUM_OSC){
		error("migrator: the maximum number of oscillators is %d", MIG_MAX_NUM_OSC);
		return MAX_ERR_GENERIC;
	}

	int i;
	int oldNumOsc;
	if(n < 1){
		error("migrator: the number of oscillators must be greater than 0");
		return MAX_ERR_GENERIC;
	}
		
	if(x->fade >= n){
		error("migrator: fade must be less than nOsc.  Setting fade to %f", floor((double)n / 2));
		x->fade = (long)floor((double)n / 2);
		return MAX_ERR_GENERIC;
	}
	
	oldNumOsc = x->nOsc;
	
	if(n < (int)x->nOsc)
		x->counter = 0.;
	x->nOsc = n;
	
	if(x->nOsc > oldNumOsc){
		for(i = oldNumOsc; i < x->nOsc; i++){
			x->arrayOut[i * 2] = x->arrayIn[mig_randPMF(x) * 2];
			//SETFLOAT(&(x->arrayOut[(i * 2)]), x->arrayIn[mig_randPMF(x) * 2].a_w.w_float);
			SETFLOAT(&(x->arrayOut[((i * 2) + 1)]), x->oscamp);
		}
	}
	return MAX_ERR_GENERIC;
}

void mig_nOsc_smooth(t_mig *x, long n){
	int i;
	int oldNumOsc;
	if(n < 1){
		error("migrator: the number of oscillators must be greater than 0");
		return;
	}
	
	if(x->fade >= n){
		error("migrator: fade must be less than nOsc.  Setting fade to %f", floor((double)n / 2));
		x->fade = (long)floor((double)n / 2);
	}
		
	if(n < (int)x->nOsc){	
		x->counter = n;
	
		x->waitingToChangeNumOsc[0] = 1;
		x->waitingToChangeNumOsc[1] = (int)n;
		return;
	}
	
	oldNumOsc = x->nOsc;
	if(oldNumOsc < n){
		x->counter = oldNumOsc;
		x->nOsc = n;
		for(i = oldNumOsc * 2; i < x->nOsc * 2; i++){
			SETFLOAT(&(x->arrayOut[i]), 0.0);
		}
	}
}

t_max_err mig_var_get(t_mig *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setfloat(*argv, powf(x->stdev, 2.f));
	return MAX_ERR_NONE;
}

t_max_err mig_var_set(t_mig *x, t_object *attr, long argc, t_atom *argv){
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}
	x->stdev = sqrt(atom_getfloat(argv));
	return MAX_ERR_NONE;
}

t_max_err mig_fade(t_mig *x, t_object *attr, long argc, t_atom *argv){
	if(argc < 1){
		return MAX_ERR_GENERIC;
	}
	long n = atom_getlong(argv);
	if(n < 1){
		error("migrator: fade must be greater than zero");
		return MAX_ERR_GENERIC;
	}
	if(n > x->nOsc){
		error("migrator: fade must be less than nOsc");
		return MAX_ERR_GENERIC;
	}
	x->fade = n;
	return MAX_ERR_NONE;
}

void mig_tellmeeverything(t_mig *x){
	version(0);
	post("Migrator is %s", (x->on) ? "ON" : "OFF");
	post("Number of oscillators: %d", x->nOsc);
	post("Oscillator amplitude: %f", x->oscamp);
	post("Time interval: %f", x->tinterval);
	post("Fade: %d", x->fade);
	post("Standard deviation (Gaussian blur): %f", x->stdev);
}
