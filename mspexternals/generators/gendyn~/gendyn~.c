/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2007, The Regents of
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
NAME: gendyn~
DESCRIPTION: Dynamic Stochastic Synthesis a la Xenakis.  This is a port of Nick Collins' Gendy[1-3] Supercollider UGens.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include "math.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef struct _gendyn
{
	t_pxobject g_ob;
	gsl_rng *g_rng;
	int g_type;
	
	// Gendy1-3
	double g_phase;
	float g_freqMul, g_amp, g_nextAmp, g_speed, g_dur;
	int g_memorySize, g_index;
	float *g_memoryAmp, *g_memoryDur;
	
	// Gendy2
	float *g_memoryAmpStep, *g_memoryDurStep;
	
	// Gendy3
	double g_nextPhase, g_lastPhase;
	float g_interpMult;
	double *g_phaseList;
	float *g_ampList;
	
	// user params
	int g_whichamp, g_whichdur;
	float g_aamp, g_adur, g_minfreq, g_maxfreq, g_scaleamp, g_scaledur;
	float g_freq;
	
} t_gendyn;

void *gendyn_class;

void gendyn_anything(t_gendyn *x, t_symbol *msg, short argc, t_atom *argv);
void gendyn_int(t_gendyn *x, long n);
void gendyn_assist(t_gendyn *x, void *b, long m, long a, char *s);
void *gendyn_new(t_symbol *msg, short argc, t_atom *argv);
void gendy1_constructor(t_gendyn *x);
void gendy2_constructor(t_gendyn *x);
void gendy3_constructor(t_gendyn *x);
static int makeseed(void);
t_int *gendyn_perform(t_int *w);
void gendyn_dsp(t_gendyn *x, t_signal **sp, short *count);
void gendy1_perform(t_gendyn *x, t_float *outL, int n);
void gendy2_perform(t_gendyn *x, t_float *outL, int n);
void gendy3_perform(t_gendyn *x, t_float *outL, int n);
void gendyn_free(t_gendyn *x);

float Gendyn_distribution(int which, float a, float f);
float Gendyn_mirroring (float lower, float upper, float in);;

void whichamp(t_gendyn *x, long l);
void whichdur(t_gendyn *x, long l);
void aamp(t_gendyn *x, double l);
void adur(t_gendyn *x, double l);
void minfreq(t_gendyn *x, double l);
void maxfreq(t_gendyn *x, double l);
void scaleamp(t_gendyn *x, double l);
void scaledur(t_gendyn *x, double l);
void freq(t_gendyn *x, double l);

void gendyn_tellmeeverything(t_gendyn *x);


//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&gendyn_class, (method)gendyn_new, (method)gendyn_free, (short)sizeof(t_gendyn), 0L, A_GIMME, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)gendyn_dsp, "dsp", A_CANT, 0);
	addmess((method)gendyn_anything, "anything", A_GIMME, 0);
	addint((method)gendyn_int);
	addmess((method)gendyn_assist, "assist", A_CANT, 0);
	addmess((method)gendyn_tellmeeverything, "tellmeeverything", 0);
	addmess((method)whichamp, "whichamp", A_LONG, 0);
	addmess((method)whichdur, "whichdur", A_LONG, 0);
	addmess((method)aamp, "aamp", A_FLOAT, 0);
	addmess((method)adur, "adur", A_FLOAT, 0);
	addmess((method)minfreq, "minfreq", A_FLOAT, 0);
	addmess((method)maxfreq, "maxfreq", A_FLOAT, 0);
	addmess((method)scaleamp, "scaleamp", A_FLOAT, 0);
	addmess((method)scaledur, "scaledur", A_FLOAT, 0);
	addmess((method)freq, "freq", A_FLOAT, 0);
	
	dsp_initclass();
		
	return 0;
}

//--------------------------------------------------------------------------

void gendyn_anything(t_gendyn *x, t_symbol *msg, short argc, t_atom *argv){	
}

void gendyn_int(t_gendyn *x, long n){
}

//--------------------------------------------------------------------------

void gendyn_assist(t_gendyn *x, void *b, long m, long a, char *s)
{
	/*
	if (m == ASSIST_OUTLET){
		if(x->g_direction == forward){
			sprintf(s,"(Signal) DWT output");
		}
		else sprintf(s, "(Signal) Inverse DWT output");
	}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(Signal) Input");
			break;
		case 1:
			sprintf(s, "(int) k, index of the coefficient within each level");
			break;
		case 2:
			sprintf(s, "(int) size of the transform");
			break;
		}
	}
	*/
}

//--------------------------------------------------------------------------

void *gendyn_new(t_symbol *msg, short argc, t_atom *argv){
	t_gendyn *x;

	x = (t_gendyn *)newobject(gendyn_class); // create a new instance of this object
	dsp_setup((t_pxobject *)x, 1);
	outlet_new((t_pxobject *)x, "signal");
	
	if(argc == 0){
		error("you must specify the gendy type [1-3]");
		return NULL;
	}
	x->g_type = argv[0].a_w.w_long;
	
	x->g_whichamp = 1;
	x->g_whichdur = 1;
	x->g_aamp = 1.f;
	x->g_adur = 1.f;
	x->g_minfreq = 440.f;
	x->g_maxfreq = 660.f;
	x->g_scaleamp = 0.5f;
	x->g_scaledur = 0.5f;
	
	if(argc > 1) x->g_whichamp = argv[1].a_w.w_long;
	if(argc > 2) x->g_whichdur = argv[2].a_w.w_long;
	if(argc > 3){
		if(argv[3].a_type == A_LONG) x->g_aamp = argv[3].a_w.w_long;
		else x->g_aamp = (float)argv[3].a_w.w_float;
	}
	if(argc > 4){
		if(argv[4].a_type == A_LONG) x->g_adur = argv[4].a_w.w_long;
		else x->g_adur = (float)argv[4].a_w.w_float;
	}
	if(argc > 5){
		if(argv[5].a_type == A_LONG) x->g_minfreq = argv[5].a_w.w_long;
		else x->g_minfreq = (float)argv[5].a_w.w_float;
	}
	if(argc > 6){
		if(argv[6].a_type == A_LONG) x->g_maxfreq = argv[6].a_w.w_long;
		else x->g_maxfreq = (float)argv[6].a_w.w_float;
	}
	if(argc > 7){
		if(argv[7].a_type == A_LONG) x->g_scaleamp = argv[7].a_w.w_long;
		else x->g_scaleamp = (float)argv[7].a_w.w_float;
	}
	if(argc > 8){
		if(argv[8].a_type == A_LONG) x->g_scaledur = argv[8].a_w.w_long;
		else x->g_scaledur = (float)argv[8].a_w.w_float;
	}
	
	gsl_rng_env_setup();
	x->g_rng = gsl_rng_alloc((const gsl_rng_type *)gsl_rng_default);
	
	// systime_ms() is a really bad idea since it'll be the same or very close if a bunch
	// are instantiated when the patch opens.
	//gsl_rng_set(x->r_rng, systime_ms());
	
	// makeseed() is from the PD code in x_misc.c
	gsl_rng_set(x->g_rng, makeseed());
		
	switch(x->g_type){
		case 1:
			gendy1_constructor(x);
			break;
		case 2:
			gendy2_constructor(x);
			break;
		case 3:
			gendy3_constructor(x);
			break;
	}
			
	return(x);
}

void gendyn_free(t_gendyn *x){
	dsp_free((t_pxobject *)x);
}

// from PD x_misc.c
// JM made the last 9-digit number a random number.
static int makeseed(void){
	int r = rand();
    static unsigned int random_nextseed = 1489853723;
    random_nextseed = random_nextseed * 435898247 + r;//938284287;
    return (random_nextseed & 0x7fffffff);
}

void gendy1_constructor(t_gendyn *x){
	x->g_freqMul = 1.f / sys_getsr();
	x->g_phase = 1.f;	//should immediately decide on new target 
	x->g_amp = 0.0; 
	x->g_nextAmp = 0.0;
	x->g_speed = 100; 
	
	x->g_memorySize = 12;  //this is arg 8 in sc
	if(x->g_memorySize < 1) x->g_memorySize = 1;
	x->g_index = 0;
	//x->g_memoryAmp = (float *)RTAlloc(x->g_World, x->g_memorySize * sizeof(float));
	x->g_memoryAmp = (float *)calloc(x->g_memorySize, sizeof(float));
	x->g_memoryDur = (float *)calloc(x->g_memorySize, sizeof(float));
		
	//initialise to zeroes and separations
	int i = 0;
	for(i = 0; i < x->g_memorySize; ++i) {
		x->g_memoryAmp[i] = 2 * gsl_ran_flat(x->g_rng, 0, 1) - 1.0;
		x->g_memoryDur[i] = gsl_ran_flat(x->g_rng, 0, 1);
	}
}

void gendy2_constructor(t_gendyn *x){
	x->g_freqMul = 1.f / sys_getsr();
	x->g_phase = 1.f;	//should immediately decide on new target 
	x->g_amp = 0.0; 
	x->g_nextAmp = 0.0;
	x->g_speed = 100; 
	
	x->g_memorySize = 12;	//default is 12
	//printf("memsize %d %f", x->g_MemorySize, ZIN0(8));
	if(x->g_memorySize < 1) x->g_memorySize = 1;
	x->g_index = 0;
	x->g_memoryAmp = (float *)calloc(x->g_memorySize, sizeof(float));
	x->g_memoryDur = (float *)calloc(x->g_memorySize,  sizeof(float));
	x->g_memoryAmpStep = (float *)calloc(x->g_memorySize,  sizeof(float));
	x->g_memoryDurStep = (float *)calloc(x->g_memorySize,  sizeof(float));
		
	//initialise to zeroes and separations
	int i = 0;
	for(i = 0; i < x->g_memorySize; ++i) {
		x->g_memoryAmp[i] = 2 * gsl_ran_flat(x->g_rng, 0, 1) - 1.0;
		x->g_memoryDur[i] = gsl_ran_flat(x->g_rng, 0, 1);
		x->g_memoryAmpStep[i] = 2 * gsl_ran_flat(x->g_rng, 0, 1) - 1.0;
		x->g_memoryDurStep[i] = 2 * gsl_ran_flat(x->g_rng, 0, 1) - 1.0;
	}
}

void gendy3_constructor(t_gendyn *x){
	x->g_freqMul = 1.f / sys_getsr();
	x->g_phase = 1.f;	//should immediately decide on new target 
	x->g_amp = 0.0; 
	x->g_nextAmp = 0.0;
	x->g_nextPhase = 0.0;
	x->g_lastPhase = 0.0;
	x->g_interpMult = 1.0;
	x->g_speed = 100; 
	
	x->g_freq = 440.f;
	
	x->g_memorySize = 12;//(int) ZIN0(7);
	if(x->g_memorySize < 1) x->g_memorySize = 1;
	x->g_index = 0;
	x->g_memoryAmp= (float *)calloc(x->g_memorySize, sizeof(float));
	x->g_memoryDur= (float *)calloc(x->g_memorySize, sizeof(float));
	
	//one more in amp list for guard (wrap) element
	x->g_ampList= (float *)calloc((x->g_memorySize + 1), sizeof(float));
	x->g_phaseList= (double *)calloc((x->g_memorySize + 1), sizeof(double));
		
	//initialise to zeroes and separations
	int i = 0;
	for(i = 0; i < x->g_memorySize; ++i) {
		x->g_memoryAmp[i] = 2 * gsl_ran_flat(x->g_rng, 0., 1.) - 1.0;
		x->g_memoryDur[i] = gsl_ran_flat(x->g_rng, 0., 1.);
		x->g_ampList[i] = 2 * gsl_ran_flat(x->g_rng, 0., 1.) - 1.0;
		x->g_phaseList[i] = 1.0; //will be intialised immediately
	}
	
	x->g_memoryAmp[0] = 0.0;	//always zeroed first BP
}
			
//called once per period so OK to work out constants in here
float Gendyn_distribution( int which, float a, float f) {

	float temp, c;
	
	if(a>1.0) a=1.0;       //a must be in range 0 to 1
	if(a<0.0001) a=0.0001; 	//for safety with some distributions, don't want divide by zero errors
	
	switch (which)
	{
		case 0: //LINEAR
				//linear
			break;
		case 1: //CAUCHY
			//X has a*tan((z-0.5)*pi)
			//I went back to first principles of the Cauchy distribution and re-integrated with a 
			//normalisation constant 
			
			//choice of 10 here is such that f=0.95 gives about 0.35 for temp, could go with 2 to make it finer
			c= atan(10*a);		//PERHAPS CHANGE TO a=1/a;
			//incorrect- missed out divisor of pi in norm temp= a*tan(c*(2*pi*f - 1));	
			temp= (1/a)*tan(c*(2*f - 1));	//Cauchy distribution, C is precalculated
			
			//printf("a %f cauchy f %f c %f temp %f out %f \n", a, f,  c, temp, temp/10);
			
			return temp*0.1; //(temp+100)/200;
			
		case 2: //LOGIST (ic)
			//X has -(log((1-z)/z)+b)/a which is not very usable as is
			
			c=0.5+(0.499*a); //calculate normalisation constant
			c= log((1-c)/c); 
		   
			//remap into range of valid inputs to avoid infinities in the log
			
		   //f= ((f-0.5)*0.499*a)+0.5;
			f= ((f-0.5)*0.998*a)+0.5; //[0,1]->[0.001,0.999]; squashed around midpoint 0.5 by a
			//Xenakis calls this the LOGIST map, it's from the range [0,1] to [inf,0] where 0.5->1
			//than take natural log. to avoid infinities in practise I take [0,1] -> [0.001,0.999]->[6.9,-6.9]
			//an interesting property is that 0.5-e is the reciprocal of 0.5+e under (1-f)/f 
			//and hence the logs are the negative of each other
			temp= log((1-f)/f)/c;	//n range [-1,1]
			//X also had two constants in his- I don't bother
			
			//printf("logist f %f temp %f\n", f, temp);
			
			return temp; //a*0.5*(temp+1.0);	//to [0,1]
			
		case 3: //HYPERBCOS
			//X original a*log(tan(z*pi/2)) which is [0,1]->[0,pi/2]->[0,inf]->[-inf,inf]
			//unmanageable in this pure form
			c=tan(1.5692255*a);    //tan(0.999*a*pi*0.5);    	//[0, 636.6] maximum range
			temp= tan(1.5692255*a*f)/c;	//[0,1]->[0,1] 
			temp= log(temp*0.999+0.001)*(-0.1447648);  // multiplier same as /(-6.9077553); //[0,1]->[0,1]
		   
			 //printf("hyperbcos f %f c %f temp %f\n", f, c, temp);
			
			return 2*temp-1.0;
			
		case 4: //ARCSINE
			//X original a/2*(1-sin((0.5-z)*pi)) aha almost a better behaved one though [0,1]->[2,0]->[a,0] 
			c= sin(1.5707963*a); //sin(pi*0.5*a);	//a as scaling factor of domain of sine input to use
			temp= sin(pi*(f-0.5)*a)/c; //[-1,1] which is what I need
			
			//printf("arcsine f %f c %f temp %f\n", f, c, temp);
			
			return temp;
			
			case 5: //EXPON
			//X original -(log(1-z))/a [0,1]-> [1,0]-> [0,-inf]->[0,inf]
			c= log(1.0-(0.999*a));
			temp= log(1.0-(f*0.999*a))/c;
			
			//printf("expon f %f c %f temp %f\n", f, c, temp);
			
			return 2*temp-1.0;
			
		case 6: //SINUS
			//X original a*sin(smp * 2*pi/44100 * b) ie depends on a second oscillator's value- 
			//hmmm, plug this in as a I guess, will automatically accept control rate inputs then!
			return 2*a-1.0;
			
		default:
			break;
		}
	
	return 2*f-1.0;
}
		
float Gendyn_mirroring (float lower, float upper, float in){
	//mirroring for bounds- safe version 
	if(in>upper || in<lower) {
	
		float range= (upper-lower);
		
		if(in<lower) in= (2*upper-lower)-in;
		
		in=fmod(in-upper,2*range);
		
		if(in<range) in=upper-in;
		else in=in- (range);
	}
	
	return in;
}

void gendyn_dsp(t_gendyn *x, t_signal **sp, short *count){
	dsp_add(gendyn_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *gendyn_perform(t_int *w){
	t_gendyn *x = (t_gendyn *)w[1];
	t_float *inL = (t_float *)w[2];
	t_float *outL = (t_float *)w[3];
	int n = (int)w[4];
	
	switch(x->g_type){
		case 1:
			gendy1_perform(x, outL, n);
			break;
		case 2:
			gendy2_perform(x, outL, n);
			break;
		case 3:
			gendy3_perform(x, outL, n);
			break;
	}
	
	return (w + 5);
}

void gendy1_perform(t_gendyn *x, t_float *outL, int n){
	int i;
	
	int whichamp = x->g_whichamp;
	int whichdur = x->g_whichdur;
	float aamp = x->g_aamp;
	float adur = x->g_adur;
	float minfreq = x->g_minfreq;
	float maxfreq = x->g_maxfreq;
	float scaleamp = x->g_scaleamp;
	float scaledur = x->g_scaledur;
	
	float rate = x->g_dur;
	double phase = x->g_phase;
	float amp = x->g_amp;
	float nextamp = x->g_nextAmp;
	float speed = x->g_speed;
	
	for(i = 0; i < n; i++){
		float z;
		if (phase >= 1.f) {
			phase -= 1.f;

			int index = x->g_index;
			int num = 12;//(int)(ZIN0(9));//(unit->mMemorySize);(((int)ZIN0(9))%(unit->mMemorySize))+1;
			if((num > (x->g_memorySize)) || (num < 1)) num = x->g_memorySize;
			
			//new code for indexing
			index = (index + 1) % num;
			amp = nextamp;
		   
			x->g_index = index;
			
			//Gendy dist gives value [-1,1], then use scaleamp
			//first term was amp before, now must check new memory slot
			nextamp = (x->g_memoryAmp[index]) + (scaleamp * Gendyn_distribution(whichamp, aamp, gsl_ran_flat(x->g_rng, 0., 1.)));
		
			//mirroring for bounds- safe version 
			if(nextamp > 1.0 || nextamp < -1.0) {
			
				//printf("mirroring nextamp %f ", nextamp);

				//to force mirroring to be sensible
				if(nextamp < 0.0) nextamp = nextamp + 4.0;
			
				nextamp = fmod(nextamp, 4.0f); 
				//printf("fmod  %f ", nextamp);

				if(nextamp > 1.0 && nextamp < 3.0)
					nextamp = 2.0 - nextamp;
			
				else if(nextamp > 1.0)
					nextamp = nextamp - 4.0;
			
				//printf("mirrorednextamp %f \n", nextamp);
			};
			
			x->g_memoryAmp[index] = nextamp;
			
			//Gendy dist gives value [-1,1]
			rate = (x->g_memoryDur[index]) + (scaledur * Gendyn_distribution(whichdur, adur, gsl_ran_flat(x->g_rng, 0., 1.)));
		 
			if(rate > 1.0 || rate < 0.0){
				if(rate < 0.0) rate = rate + 2.0;
				rate = fmod(rate, 2.0f);
				rate = 2.0 - rate;
			}
		
			x->g_memoryDur[index] = rate;
			
			//printf("nextamp %f rate %f \n", nextamp, rate);

			//define range of speeds (say between 20 and 1000 Hz)
			//can have bounds as fourth and fifth inputs
			speed =  (minfreq + ((maxfreq - minfreq) * rate)) * (x->g_freqMul);
			
			//if there are 12 control points in memory, that is 12 per cycle
			//the speed is multiplied by 12
			//(I don't store this because updating rates must remain in range [0,1]
			speed *= num;
		} 
			
		//linear interpolation could be changed
		z = ((1.0 - phase) * amp) + (phase * nextamp);
	
		phase +=  speed;
		outL[i] = z;
	};

	x->g_phase = phase;
	x->g_amp =  amp;      
	x->g_nextAmp = nextamp;
	x->g_speed = speed;
	x->g_dur = rate;
}

void gendy2_perform(t_gendyn *x, t_float *outL, int n){
	//distribution choices for amp and dur and constants of distribution
	int whichamp = x->g_whichamp;
	int whichdur = x->g_whichdur;
	float aamp = x->g_aamp;
	float adur = x->g_adur;
	float minfreq = x->g_minfreq;
	float maxfreq = x->g_maxfreq;
	float scaleamp = x->g_scaleamp;
	float scaledur = x->g_scaledur;
	
	float rate = x->g_dur;
	double phase = x->g_phase;
	float amp = x->g_amp;
	float nextamp = x->g_nextAmp;
	float speed = x->g_speed;
	
	int i;

	for(i = 0; i < n; i++){
		float z;
				
		if (phase >= 1.f) {
			phase -= 1.f;

			int index= x->g_index;
			int num = 12;//(int)(ZIN0(9));//(unit->mMemorySize);(((int)ZIN0(9))%(unit->mMemorySize))+1;
			if((num > (x->g_memorySize)) || (num < 1)) num = x->g_memorySize;
			
			//new code for indexing
			index = (index + 1) % num;
			
			//using last amp value as seed
			//random values made using a lehmer number generator xenakis style
			float a = 1.17;//ZIN0(10); 
			float c = .31;//ZIN0(11);
			
			float lehmerxen = fmod(((amp) * a) + c, 1.0f);  
			
			//printf("lehmer %f \n", lehmerxen);

			amp = nextamp;
		   
			x->g_index = index;
			
			//Gendy dist gives value [-1,1], then use scaleamp
			//first term was amp before, now must check new memory slot
		   
			float ampstep = (x->g_memoryAmpStep[index]) + Gendyn_distribution(whichamp, aamp, fabs(lehmerxen));
			ampstep = Gendyn_mirroring(-1.0, 1.0, ampstep);
		   
			x->g_memoryAmpStep[index] = ampstep;
			
			nextamp = (x->g_memoryAmp[index]) + (scaleamp * ampstep);
			
			nextamp = Gendyn_mirroring(-1.0, 1.0, nextamp);
			
			x->g_memoryAmp[index] = nextamp;
			
			float durstep = (x->g_memoryDurStep[index]) + Gendyn_distribution(whichdur, adur, gsl_ran_flat(x->g_rng, 0., 1.));
			durstep = Gendyn_mirroring(-1.0, 1.0, durstep);
		   
			x->g_memoryDurStep[index] = durstep;
			
			rate = (x->g_memoryDur[index]) + (scaledur * durstep);
			
			rate = Gendyn_mirroring(0.0, 1.0, rate);
			
			x->g_memoryDur[index] = rate;
			
			//printf("nextamp %f rate %f \n", nextamp, rate);

			//define range of speeds (say between 20 and 1000 Hz)
			//can have bounds as fourth and fifth inputs
			speed =  (minfreq + ((maxfreq - minfreq) * rate)) * (x->g_freqMul);
			
			//if there are 12 control points in memory, that is 12 per cycle
			//the speed is multiplied by 12
			//(I don't store this because updating rates must remain in range [0,1]
			speed *= num;
		} 
				
		//linear interpolation could be changed
		z = ((1.0 - phase) * amp) + (phase * nextamp);
		
		phase +=  speed;
		outL[i] = z;
	};

	x->g_phase = phase;
	x->g_amp =  amp;      
	x->g_nextAmp = nextamp;
	x->g_speed = speed;
	x->g_dur = rate;
}

void gendy3_perform(t_gendyn *x, t_float *outL, int n){
	//distribution choices for amp and dur and constants of distribution
	int whichamp = x->g_whichamp;
	int whichdur = x->g_whichdur;
	float aamp = x->g_aamp;
	float adur = x->g_adur;
	float scaleamp = x->g_scaleamp;
	float scaledur = x->g_scaledur;
	float freq = x->g_freq;

	double phase = x->g_phase;
	float amp = x->g_amp;
	float nextamp = x->g_nextAmp;
	float speed = x->g_speed;
	int index = x->g_index;  
	int interpmult = (int)x->g_interpMult;
	double lastphase = x->g_lastPhase;
	double nextphase = x->g_nextPhase;
	
	int i, j;

	float *amplist = x->g_ampList;
	double *phaselist = x->g_phaseList;

	for(i = 0; i < n; i++){ 
		float z;
				
		if (phase >= 1.f) { //calculate all targets for new period
			phase -= 1.f;

			int num = 12;//(int)(ZIN0(8));
			if((num > (x->g_memorySize)) || (num < 1)) num = x->g_memorySize;
			
			float dursum = 0.0;
			
			float *memoryamp= x->g_memoryAmp;
			float *memorydur= x->g_memoryDur;
			
			for(j = 0; j < num; ++j) {
			
				if(j > 0) {   //first BP always stays at 0
					float amp = (memoryamp[j]) + (scaleamp * Gendyn_distribution(whichamp, aamp, gsl_ran_flat(x->g_rng, 0., 1.)));
					amp = Gendyn_mirroring(-1.0, 1.0, amp);
					memoryamp[j] = amp;
				}
			   
				float dur = (memorydur[j]) + (scaledur * Gendyn_distribution(whichdur, adur, gsl_ran_flat(x->g_rng, 0., 1.)));
				dur = Gendyn_mirroring(0.01, 1.0, dur);	//will get normalised in a moment, don't allow zeroes
				memorydur[j] = dur;
				dursum += dur;
			}
			
			//normalising constant
			dursum = 1.0 / dursum;
			
			int active = 0;
			
			//phase duration of a sample
			float minphase = x->g_freqMul;
			
			speed = freq * minphase;
	
			//normalise and discard any too short (even first)
			for(j = 0; j < num; ++j) {
			
				float dur = memorydur[j];
				dur *= dursum;
				
				if(dur >= minphase) {
					amplist[active] = memoryamp[j];
					phaselist[active] = dur;
					++active;
				}
			}
			
			//add a zero on the end at active
			amplist[active] = 0.0; //guard element
			phaselist[active] = 2.0; //safety element
			
			//lastphase=0.0;
//					nextphase= phaselist[0];
//					amp=amplist[0];
//					nextamp=amplist[1];
//					index=0;
//					unit->mIndex=index;
//					
			//setup to trigger next block
			nextphase = 0.0;
			nextamp = amplist[0];
			index = -1;
		} 
			
		
		if (phase >= nextphase) { //are we into a new region?
		
			//new code for indexing
			++index; //=index+1; //%num;
			
			amp = nextamp;
		   
			x->g_index = index;
			
			lastphase = nextphase;
			nextphase = lastphase + phaselist[index];
			nextamp = amplist[index + 1]; 
								
			interpmult = (int)(1.0 / (nextphase - lastphase));
			
		}	
				
		float interp = (phase - lastphase) * interpmult;		
				
		//linear interpolation could be changed
		z = ((1.0 - interp) * amp) + (interp * nextamp);
		
		phase +=  speed;
		outL[i] = z;
	};

	x->g_phase = phase;
	x->g_speed = speed;
	x->g_interpMult = interpmult; 
	x->g_amp = amp;      
	x->g_nextAmp = nextamp;
	x->g_lastPhase = lastphase;
	x->g_nextPhase = nextphase;
}

void whichamp(t_gendyn *x, long l){
	x->g_whichamp = l;
}

void whichdur(t_gendyn *x, long l){
	x->g_whichdur = l;
}

void aamp(t_gendyn *x, double l){
	x->g_aamp = l;
}

void adur(t_gendyn *x, double l){
	x->g_adur = l;
}

void minfreq(t_gendyn *x, double l){
	if(l < 0) l = 0;
	if(l > sys_getsr()) l = sys_getsr();
	x->g_minfreq = l;
}

void maxfreq(t_gendyn *x, double l){
	if(l < 0) l = 0;
	if(l > sys_getsr()) l = sys_getsr();
	x->g_maxfreq = l;
}

void scaleamp(t_gendyn *x, double l){
	x->g_scaleamp = l;
}

void scaledur(t_gendyn *x, double l){
	x->g_scaledur = l;
}

void freq(t_gendyn *x, double l){
	x->g_freq = fabs(l);
}

void gendyn_tellmeeverything(t_gendyn *x){
	version(0);
	
	post("x->g_whichamp %d", x->g_whichamp);
	post("x->g_whichdur %d", x->g_whichdur);
	post("x->g_aamp %f", x->g_aamp);
	post("x->g_adur %f", x->g_adur);
	post("x->g_minfreq %f", x->g_minfreq);
	post("x->g_maxfreq %f", x->g_maxfreq);
	post("x->g_scaleamp %f", x->g_scaleamp);
	post("x->g_scaledur %f", x->g_scaledur);
}
