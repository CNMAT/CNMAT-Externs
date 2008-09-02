/* Beat~: Beat analyzer of an arbitrary audio channel */
/* Written by Tristan Jehan at the MIT Media Laboratory */
/* Copyright MIT, all rights reserved */

/*
 beat~ object by Tristan Jehan
 Copyright (c) 2004 Masachussetts Institute of Technology.
 All rights reserved.
 
 Permission to use, copy, modify, and distribute this software and its
 documentation for educational, research, and not-for-profit purposes, without
 fee and without a signed licensing agreement, is hereby granted, provided that
 the above copyright notice, this paragraph and the following two paragraphs
 appear in all copies, modifications, and distributions.  Contact The Office of
 Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
 CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.
  
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
 DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
 REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.
 
 beat.c
 
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: beat~
 DESCRIPTION: Beat analyzer of an arbitrary audio channel
 AUTHORS: Tristan Jehan
 COPYRIGHT_YEARS: 2004
 DRUPAL_NODE: /patch/xxxx
 SVN_REVISION: $LastChangedRevision: 1916 $
 version 1.0.1: Tristans version.
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

#include "ext.h"
#include "z_dsp.h"
#include "fft.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Add altivec function prototypes
#ifdef __ALTIVEC__
#include "vDSP.h"
#endif

#define RES_ID	7307
#define COPYRIGHT "Copyright © 2004 Massachussets Institute of Technology"
#define VERSION "1.0.1"
#define DEFNUMBARKBANDS 3 // at 44100 Hz only
#define MAXNUMBARKBANDS 25
#define DEFAULT_FS 44100
#define DB_REF 60
#define MAXBARK 24.740177
#define BARK24FREQ 15429
#define BIG 999999999999.9f

/* BEAT CONSTANTS */
#define MIN_MINTEMPO 20   // Default minimum possible tempo
#define MAX_MAXTEMPO 300  // Default maximum possible tempo
#define MIN_NUMRESON 3    // Default number of resonators per channel
#define MAX_NUMRESON 300  // Default number of resonators per channel
#define MIN_HALFTIME 1000 // Default time of half energy drop (in ms)
#define MAX_HALFTIME 20000 // Default time of half energy drop (in ms)

#define DEF_MINTEMPO 60   // Default minimum possible tempo
#define DEF_MAXTEMPO 240  // Default maximum possible tempo
#define DEF_NUMRESON 100  // Default number of resonators per channel
#define DEF_HALFTIME 3000 // Default time of half energy drop (in ms)
#define DEF_FRAMERATE 10  // Default update time
#define DEF_THRESH 1  	  // Default threshold
#define DEF_PHASELOCK 0.8 // Default phaselock
#define DEF_TEMPOLOCK 1.0 // Default tempolock
#define DEF_TIME 150
#define MAX_TIME 1000
//#define DEF_TIME_TEMPOTHREAD 2000
//#define MAX_TIME_TEMPOTHREAD 10000
#define JUMP 256

//#define TWOPI 6.28318530717952646f
#define THREEPI 9.424777960769379f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024
#define MAXPADDING 16
#define MAXDELAY 512
#define DEFDELAY 0			// Default initial delay (in # of signal vectors) 

// Window description
#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN62_W(i,ac) (0.44859f - 0.49364f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.05677f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN70_W(i,ac) (0.42323f - 0.49755f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.07922f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN74_W(i,ac) (0.402217f - 0.49703f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.09892f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.00188 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN92_W(i,ac) (0.35875f - 0.48829f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.14128f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.01168 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))
#define DEFWIN Blackman70		// Default window

void *beat_class;

enum {Recta=0, Hann, Hamm, Blackman62, Blackman70, Blackman74, Blackman92};

typedef struct combfilt
{
	double tempo;   // Tempo in bpm of the resonnance of the filter 
	double alpha;   // Coefficient of the comb filter
	int delay;	   // Delay of the filter
	double *output; // Circular buffer of length 'delay' containing the filter output
	int curInd;	   // Current index (delayed index = (curInd+1) mod delay)
	double energy;  // Current energy of the filter
	double norm;    // Normalisation for the filter
	
} CombFilter;


typedef struct _beat {

	t_pxobject x_obj;

	int minTempo;		// Minimum possible tempo
	int maxTempo; 		// Maximum possible tempo
	int numReson;		// Number of resonators per channel
	int halfTime;		// Filter half-energy time (in ms)
	int numSamps;		// Size of the filter in number of samples
	int maxNumSamps;	// Maximum size of the filter in number of Samples
	double *cosine;		// Buffer containing the cosine data
	double *oldData;	// Buffer of last Filtered data
	double *heldData;	// Buffer of last Un-filtered data
	int *index;			// Buffer of indexes in the Cosine buffer
	double *curInput;	// Current list input
	double *curRecti;	// Current half-wave rectified signal
	double *oldInput;	// Old list input
	double *filtEstim;	// Buffer containing the total energy of each resonator type
	int frameRate;		// Frame rate in subdivisions of the input
	int frameCpt;		// Counter for the analysis frame rate
	CombFilter **comb;	// Comb filters
	int tempoIndex;
	double strength;
	int indexPhase;
	double thresh;
	double phaselock;
	double tempolock;
	int tempoInd1;
	int tempoInd2;
	int tempoWindow;
	int absTempoIndex;
	int jump;
	
	t_float x_Fs;			// Sample rate
	t_int x_overlap;		// Number of overlaping samples
	t_int x_hop;			// Number of non-overlaping samples
	t_int x_window;			// Type of window	
	char x_winName[32];		// Window name	
	t_int x_delay;			// Vector size delay before to start feeding the buffer
	t_int x_counter;		// Counter that goes with the vector size delay

    t_int *Buf1;			// buffer 1 : Use buffers of integers to copy faster
    t_int *Buf2;			// buffer 2
    t_float *BufFFT;		// FFT buffer
    t_float *WindFFT;		// Window of FFTSize
    t_float *memFFT;		// fftnobitrev.c memory space

    t_int BufSize;			// FFT buffer size
	t_int FFTSize;			// Size of FFT
	UInt32 x_FFTSizeOver2;	// Size of FFT/2 (UInt32 in G4 FFT)
    t_int BufWritePos;		// Where to write in buffer

    t_float *BufBark;		// Beat buffer
	t_int *BufSizeBark;		// Number of bins per band
	t_int numBarkBands;		// Number of Bark bands (default is 6)

	void **x_out;			// Outlet for all Bands
	void *x_clock;			// Use a clock for outputs... (better than Qelem)
	Atom *myList;			// Copy of outputs as Atoms
	void *x_outlet;			// List of outputs
	void *TempoOutlet;		// Outlet for Tempo
	void *StrengthOutlet;	// Outlet for Tempo
	void *BeatOutlet;		// Outlet for bangs

#ifdef __ALTIVEC__ // Additional stuff for managing the G4-optimized FFT by Apple
#pragma altivec_model on
	UInt32 x_log2n;
    COMPLEX_SPLIT x_A;
	FFTSetup x_setup;
#pragma altivec_model off
#endif
		
} t_beat;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman62, *ps_blackman70, *ps_blackman74, *ps_blackman92, *ps_list, *ps_nolist;

t_int *beat_perform(t_int *w);
void beat_dsp(t_beat *x, t_signal **sp, short *connect);
void beat_log(t_beat *x);
void beat_float(t_beat *x, float f);
void beat_int(t_beat *x, long n);
void beat_assist(t_beat *x, void *b, long m, long a, char *s);
void *beat_new(t_symbol *s, short argc, t_atom *argv);
void beat_init(t_beat *x);
void beat_free(t_beat *x);
void beat_tick(t_beat *x);
void beat_clear(t_beat *x, Symbol *sym);
double filtEnergy(double *buf, int bufSize, double tempo);
int linReg(t_beat *x, double *vector, int numPoints);
void beat_thresh(t_beat *x, Symbol *s, short argc, Atom *argv);
void beat_phaselock(t_beat *x, Symbol *s, short argc, Atom *argv);
void beat_temposearch(t_beat *x, Symbol *s, short argc, Atom *argv);
void beat_tempolock(t_beat *x, Symbol *s, short argc, Atom *argv);

#ifdef __ALTIVEC__
void beat_tick_G4(t_beat *x);
long log2max(long n);
#endif

void main(void) {

    post("Beat~ object version " VERSION " by Tristan Jehan");
	post(COPYRIGHT);
    post("");

	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman62 = gensym("blackman62");
	ps_blackman70 = gensym("blackman70");
	ps_blackman74 = gensym("blackman74");
	ps_blackman92 = gensym("blackman92");
	ps_list = gensym("list");
	ps_nolist = gensym("nolist");

	setup((Messlist **)&beat_class, (method)beat_new, (method)beat_free, (short)sizeof(t_beat), 0L, A_GIMME, 0);
		
	addmess((method)beat_dsp, "dsp", A_CANT, 0);
	addmess((method)beat_assist, "assist", A_CANT, 0);
	addmess((method)beat_clear, "clear", 0);
	addmess((method)beat_thresh, "thresh", A_GIMME, 0);
	addmess((method)beat_phaselock, "phaselock", A_GIMME, 0);
	addmess((method)beat_tempolock, "tempolock", A_GIMME, 0);
	addmess((method)beat_temposearch, "temposearch", A_GIMME, 0);
	addmess((method)beat_assist, "assist", A_CANT, 0);
	addfloat((method)beat_float);
	addint((method)beat_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *beat_perform(t_int *w) {

	t_float *in = (t_float *)(w[1]);
	t_beat *x = (t_beat *)(w[2]);
	t_int n = (int)(w[3]);

	t_int *myintin = (t_int *)in; 				// Copy integers rather than floats -> faster
	t_int *myintBufFFT = (t_int *)(x->BufFFT);	// We assume sizeof(float) == sizeof(int) though
	t_int i, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->x_overlap;
	t_int *TmpBuf = x->Buf1;
			
	if (x->x_counter < 1) {
	
		// Copy input samples into FFT buffer	
		while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
			x->Buf1[x->BufWritePos] = myintin[index];
			x->BufWritePos++;
			index++;
			cpt--;
		}
	
		// When Buffer is full...
		if (x->BufWritePos >= x->BufSize) {
			
			// Save overlapping samples into Buffer 2
			for (i=0; i<x->x_overlap; i++) 
				x->Buf2[i] = x->Buf1[overlapindex+i];

			maxindex = n - index + x->x_overlap;

			// Copy the rest of incoming samples into Buffer 2
			for (i=x->x_overlap; i<maxindex; i++) {
				x->Buf2[i] = myintin[index];
				index++;
			}
		
			x->BufWritePos = maxindex;
										
			// Make a copy of Buffer 1 into Buffer FFT for computation outside the perform function
			for (i=0; i<x->BufSize; i++) 
				myintBufFFT[i] = x->Buf1[i];

			// Go for the FFT outside the perform function with a delay of 0 ms!
			clock_delay(x->x_clock,0);
		
			// Swap buffers
			x->Buf1 = x->Buf2;
			x->Buf2 = TmpBuf;
		}
	} else {
		x->x_counter--;
	}
	
	return (w+4);
}

void beat_dsp(t_beat *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	// Initializing the delay counter
	x->x_counter = x->x_delay;

	if (vs > x->BufSize) {
		post("Beat~: You need to use a smaller signal vector size...");
	} else if (connect[0]) {
		dsp_add(beat_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
	}
}

void beat_float(t_beat *x, float f) {

	int n = (t_int)(f * x->x_Fs/1000.0f); 
	beat_int(x, n);
}

void beat_int(t_beat *x, long n) {
	t_int vs = sys_getblksize();

	x->x_hop = n; 
	if (x->x_hop < vs) {
		post("Beat~: You can't overlap so much...");
		x->x_hop = vs;
	} else if (x->x_hop > x->BufSize) {
		x->x_hop = x->BufSize;
	}
	x->x_overlap = x->BufSize - x->x_hop;	
}

void beat_assist(t_beat *x, void *b, long m, long a, char *s) {

if (m == ASSIST_INLET) {
               sprintf(s, "(signal) audio");
			   }
       else {
		switch (a) {	
		case 0:
			sprintf(s,"(bang) beat");
			break;
		case 1:
			sprintf(s,"(float) tempo");
			break;
		case 2:
			sprintf(s,"(float) beat strength");
			break;
		case 3:
			sprintf(s,"(list) normaized tempo spectrum");
			break;
				}
	}


	//assist_string(RES_ID,m,a,1,2,s);
}

void *beat_new(t_symbol *s, short argc, t_atom *argv) {
	t_int i, j=1, band=0, oldband=0, sizeband=0;
	t_int vs = sys_getblksize(); // get vector size
	double freq=0.0f, oldfreq=0.0f;
    t_beat *x = (t_beat *)newobject(beat_class);
	double barkSize, currBarkMax;
	int tmpNumReson, oldDelay;
	int valid = 1;
    dsp_setup((t_pxobject *)x,1); // one inlet	
	x->x_Fs = sys_getsr();
	x->BufWritePos = 0;
	x->frameCpt = 0;
	x->frameRate = DEF_FRAMERATE;
	x->tempoIndex = 0;
	x->jump = vs;
	x->numBarkBands = DEFNUMBARKBANDS;
		
	/* Initialize default Variables */
	x->minTempo = DEF_MINTEMPO;
	x->maxTempo = DEF_MAXTEMPO;
	tmpNumReson = DEF_NUMRESON;
	x->numBarkBands = DEFNUMBARKBANDS;
	x->halfTime = DEF_HALFTIME;
	x->frameRate = (int)DEF_FRAMERATE / (x->jump/128.0);
	x->thresh = DEF_THRESH;
	x->phaselock = DEF_PHASELOCK;
	x->tempolock = DEF_TEMPOLOCK;
	x->frameCpt = 0;
	x->indexPhase = 0;
	x->tempoWindow = BIG;
	x->absTempoIndex = 0;
	x->x_window = DEFWIN;
	x->BufSize = DEFBUFSIZE;
	x->x_hop = vs;

	/* test arguments validity */
	for(i=0; i<argc; i++) {
		if (argv[i].a_type != A_LONG) {
			valid = 0;
		post("i = %d", i);
		}
	}

	post("--- Beat~ ---");	
	if (valid) {	
		switch(argc) {
			case 1: x->numBarkBands = argv[0].a_w.w_long; break;
			case 2: x->minTempo = argv[0].a_w.w_long;
					x->maxTempo = argv[1].a_w.w_long; break;
			case 3: x->minTempo = argv[0].a_w.w_long;
					x->maxTempo = argv[1].a_w.w_long;
					tmpNumReson = argv[2].a_w.w_long; break;
			case 4: x->minTempo = argv[0].a_w.w_long;
					x->maxTempo = argv[1].a_w.w_long;
					tmpNumReson = argv[2].a_w.w_long;
					x->numBarkBands = argv[3].a_w.w_long; break;
			case 5: x->minTempo = argv[0].a_w.w_long;
					x->maxTempo = argv[1].a_w.w_long;
					tmpNumReson = argv[2].a_w.w_long;
					x->numBarkBands = argv[3].a_w.w_long;
					x->halfTime = argv[4].a_w.w_long; break;
			case 6: x->minTempo = argv[0].a_w.w_long;
					x->maxTempo = argv[1].a_w.w_long;
					tmpNumReson = argv[2].a_w.w_long;
					x->numBarkBands = argv[3].a_w.w_long;
					x->halfTime = argv[4].a_w.w_long;
					x->BufSize = argv[5].a_w.w_long; break;
			default: ;
		}
		if (x->minTempo < MIN_MINTEMPO) x->minTempo = MIN_MINTEMPO;
		else if (x->minTempo > MAX_MAXTEMPO) x->minTempo = MAX_MAXTEMPO;
		if (x->maxTempo < MIN_MINTEMPO) x->maxTempo = MIN_MINTEMPO;
		else if (x->maxTempo > MAX_MAXTEMPO) x->maxTempo = MAX_MAXTEMPO;
		if (x->maxTempo < x->minTempo) {
			int tmp = x->maxTempo;
			x->maxTempo = x->minTempo;
			x->minTempo = tmp;
		}
		if (tmpNumReson < MIN_NUMRESON) tmpNumReson = MIN_NUMRESON;
		else if (tmpNumReson > MAX_NUMRESON) tmpNumReson = MAX_NUMRESON;
		if (x->numBarkBands < 1) x->numBarkBands = 1;
		else if (x->numBarkBands > MAXNUMBARKBANDS) x->numBarkBands = MAXNUMBARKBANDS;
		if (x->halfTime < MIN_HALFTIME) x->halfTime = MIN_HALFTIME;
		else if (x->halfTime > MAX_HALFTIME) x->halfTime = MAX_HALFTIME;
		
		if ((x->BufSize > vs) && (x->BufSize < 128))  x->BufSize = 128;
		else if ((x->BufSize > 128) && (x->BufSize < 256)) x->BufSize = 256;
		else if ((x->BufSize > 256) && (x->BufSize < 512)) x->BufSize = 512;
		else if ((x->BufSize > 512) && (x->BufSize < 1024)) x->BufSize = 1024;
		else if ((x->BufSize > 1024) && (x->BufSize < 2048)) x->BufSize = 2048;
		else if ((x->BufSize > 2048) && (x->BufSize < 4096)) x->BufSize = 4096;
		else if (x->BufSize > 8192) {
			post("   Maximum FFT size is 8192 samples!");
			x->BufSize = 8192;
		}

	} else {
		post("One or more arguments are not integers");
		post("Selecting default values...");
	}
		
	x->x_overlap = x->BufSize - x->x_hop;
	x->FFTSize = x->BufSize;
	x->x_delay = DEFDELAY;


	// Overlap case
	if (x->x_overlap > x->BufSize-vs) {
		x->x_overlap = x->BufSize-vs;
	} else if (x->x_overlap < 1)
		x->x_overlap = 0;
		
	x->x_FFTSizeOver2 = x->FFTSize/2;		

	// Here comes the choice for altivec optimization or not...
	if (sys_optimize()) { // note that we DON'T divide the vector size by four here

#ifdef __ALTIVEC__ // More code and a new ptr so that x->BufFFT is vector aligned.
#pragma altivec_model on 
		x->x_clock = clock_new(x,(method)beat_tick_G4); // Call altivec-optimized tick function
		post("	Using G4-optimized FFT");	
		// Allocate some memory for the altivec FFT
		x->x_A.realp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_A.imagp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_log2n = log2max(x->FFTSize);
      	x->x_setup = create_fftsetup (x->x_log2n, 0);
#pragma altivec_model off
#else
#endif

	} else { // Normal tick function
		post("  No optimization!");
		x->x_clock = clock_new(x,(method)beat_tick);
		x->memFFT = (t_float*) NewPtr(CMAX * x->FFTSize * sizeof(t_float)); // memory allocated for normal fft twiddle
	}

	if (x->numBarkBands > MAXNUMBARKBANDS) x->numBarkBands = MAXNUMBARKBANDS;
	currBarkMax = barkSize = MAXBARK / x->numBarkBands;
	x->numBarkBands = floor( (13.0f*atan(0.76*(x->x_Fs/2.0f)/1000.0f) + 3.5*atan(((x->x_Fs/2.0f)/7500.0f)*((x->x_Fs/2.0f)/7500.0f))) / MAXBARK * (double)x->numBarkBands);

	// Allocate memory
	x->Buf1 = (t_int*) NewPtr(x->BufSize * sizeof(t_float)); // Careful these are pointers to integers but the content is floats
	x->Buf2 = (t_int*) NewPtr(x->BufSize * sizeof(t_float));
	x->BufFFT = (t_float*) NewPtr(x->FFTSize * sizeof(t_float));
	x->WindFFT = (t_float*) NewPtr(x->BufSize * sizeof(t_float));
	x->BufBark = (t_float*) NewPtr(2*x->numBarkBands * sizeof(t_float));
	x->BufSizeBark = (t_int*) NewPtr(x->numBarkBands * sizeof(t_int));


	// Compute and store Windows
	if (x->x_window != Recta) {
		
		switch (x->x_window) {

			case Hann: 	for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = HANNING_W(i,x->BufSize);
 						break;
			case Hamm:	for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = HAMMING_W(i,x->BufSize);
						break;
			case Blackman62: for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = BLACKMAN62_W(i,x->BufSize);
						break;
			case Blackman70: for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = BLACKMAN70_W(i,x->BufSize);
						break;
			case Blackman74: for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = BLACKMAN74_W(i,x->BufSize);
						break;
			case Blackman92: for (i=0; i<x->BufSize; ++i)
							x->WindFFT[i] = BLACKMAN92_W(i,x->BufSize);
						break;
		}
	} else {
		for (i=0; i<x->BufSize; ++i) { // Just in case
			x->WindFFT[i] = 1.0f;
		}
	}
	
	x->BufBark[0] = 0.0f;


	// Compute and store Bark scale	
	for (i=0; i<x->FFTSize/2; i++) {
		freq = (i*x->x_Fs)/x->FFTSize;
            band = 13.0f*atan(0.76*freq/1000.0f) + 3.5*atan((freq/7500.0f)*(freq/7500.0f));
            if (band > currBarkMax) {
			x->BufBark[j] = oldfreq;
			x->BufBark[j+1] = freq;
			x->BufSizeBark[j/2] = sizeband;
			j+=2;
			sizeband = 0;
			currBarkMax += barkSize;
		}
		oldfreq = freq;
		sizeband++;
	}

	x->BufBark[2*x->numBarkBands-1] = freq;
	x->BufSizeBark[x->numBarkBands-1] = sizeband;


	// Manages half-cosine filtering	
	x->numSamps = (int) round((DEF_TIME / (x->jump * 1000.0f/x->x_Fs)));
	x->maxNumSamps = (int) round((MAX_TIME / (x->jump * 1000.0f/x->x_Fs)));
	
	x->curInput = calloc(x->numBarkBands, sizeof(double));
	x->curRecti = calloc(x->numBarkBands, sizeof(double));
	x->oldInput = calloc(x->numBarkBands, sizeof(double));
	x->filtEstim = calloc(tmpNumReson, sizeof(double));

	x->cosine = (double*) calloc(x->maxNumSamps, sizeof(*x->cosine));
	x->oldData = (double*) calloc(x->numBarkBands, sizeof(*x->oldData));
	x->heldData = (double*) calloc(x->numBarkBands, sizeof(*x->heldData));
	x->index = (int*) calloc(x->numBarkBands, sizeof(*x->index));

	for (i=0; i<x->numSamps; i++) {
		x->cosine[i] = 0.5f * cosf(PI * (i+1)/x->numSamps) + 0.5f;
	}

	/* 2D matrix of comb filters */
	x->comb = malloc(x->numBarkBands * sizeof(CombFilter *));
	for (i=0; i<x->numBarkBands; i++) {
  		x->comb[i] = malloc(tmpNumReson * sizeof(CombFilter));
	}
	
	/* Define tempo, alpha, and delay of all resonators */
	x->numReson = 0;
	oldDelay = 0;
	
	for (i=0; i<tmpNumReson; i++) {
		double tempo = exp(log(x->minTempo) + (log(x->maxTempo)-log(x->minTempo))/tmpNumReson * i);
		double alpha = exp ( log(0.5) * (60000. / tempo ) / (double)x->halfTime );
		int delay = (int) round ( ( (x->x_Fs / (double)x->jump) * (60000. / tempo) / 1000.) );
		double norm = 0.0f, sum = 0.0f;
		int k;
		
		if (delay != oldDelay) {
			for (j=0; j<delay; j++) {
				for (k=0; k<delay; k++) {
					sum += pow(alpha, (double)k);
				}
				norm += ((1.0f - alpha) * sum);
			}

			norm /= (double)delay;
		
			for (j=0; j<x->numBarkBands; j++) {
				x->comb[j][x->numReson].curInd = 0;
				x->comb[j][x->numReson].energy = 0.0f;
				x->comb[j][x->numReson].tempo = tempo;
				x->comb[j][x->numReson].alpha = alpha;
				x->comb[j][x->numReson].delay = delay;
				x->comb[j][x->numReson].norm = norm;
				x->comb[j][x->numReson].output = calloc(x->comb[j][x->numReson].delay, sizeof(double));
			}
			x->numReson++;
			oldDelay = delay;
		}
	}
	
	x->myList   = (Atom*) NewPtr(x->numReson * sizeof(*x->myList));     
	x->x_outlet = listout((t_object *)x);		// Create a list outlet
    x->StrengthOutlet = floatout((t_beat *)x);  // Create the tempo outlet
    x->TempoOutlet = floatout((t_beat *)x);     // Create the tempo outlet
    x->BeatOutlet = bangout((t_beat *)x);	    // Create the beat outlet

	post("	Minimum Tempo = %d BPM",x->minTempo);
	post("	Maximum Tempo = %d BPM",x->maxTempo);
	post("	Number of resonators = %d",x->numReson);
	post("	Number of channels = %d",x->numBarkBands);
	post("	Total resonators = %d",x->numReson * x->numBarkBands);
	post("	Echoic memory = %d ms",x->halfTime);
	post("	FFT size = %d samples",x->FFTSize);
//	post("	Frame rate = %d",x->frameRate);
	post("");
		
	if (JUMP < vs) { 
		post("You have chosen a vector size of %d!", vs);
		post("I recommend a smaller vector size <= 256 samples");
		post("This will increase the CPU load!");
		post("You can adjust the vector size in the Max menu under:");
		post("Options:DSP status...:I/O Vector Size");
		post("Please reload the object before using it...");
		post("");
	} 

	x->tempoInd1 = 0;
	x->tempoInd2 = x->numReson-1;

    return (x);
}



void  beat_free(t_beat *x) {

int i, j;

dsp_free((t_pxobject *)x);

#ifdef __ALTIVEC__
#pragma altivec_model on
	if (x->x_A.realp) t_freebytes(x->x_A.realp, x->x_FFTSizeOver2);
	if (x->x_A.imagp) t_freebytes(x->x_A.imagp, x->x_FFTSizeOver2);
	if (x->x_setup) destroy_fftsetup(x->x_setup);
#pragma altivec_model off
#else
	if (x->memFFT != NULL) DisposePtr((char *) x->memFFT);
#endif		

	if (x->Buf1 != NULL) DisposePtr((char *) x->Buf1);
	if (x->Buf2 != NULL) DisposePtr((char *) x->Buf2);
	if (x->BufFFT != NULL) DisposePtr((char *) x->BufFFT);
	if (x->WindFFT != NULL) DisposePtr((char *) x->WindFFT);
	if (x->x_clock != NULL) freeobject((t_object *)x->x_clock);
	if (x->BufBark != NULL) DisposePtr((char *) x->BufBark);
	if (x->BufSizeBark != NULL) DisposePtr((char *) x->BufSizeBark);
	if (x->x_out != NULL) DisposePtr((char *) x->x_out);
	if (x->myList != NULL) DisposePtr((char *) x->myList);

	if (x->cosine)	 free(x->cosine);
	if (x->oldData)	 free(x->oldData);
	if (x->heldData) free(x->heldData);
	if (x->index)	 free(x->index);
	if (x->curInput) free(x->curInput);
	if (x->curRecti) free(x->curRecti);
	if (x->oldInput) free(x->oldInput);
	if (x->filtEstim) free(x->filtEstim);

	for (j=0; j<x->numBarkBands; j++) {
		for (i=0; i<x->numReson; i++) {
			free(x->comb[j][i].output);
		}
			free(x->comb[j]);
	}
	free(x->comb);

	
}


void beat_thresh(t_beat *x, Symbol *s, short argc, Atom *argv) {
	
	if (argv[0].a_type == A_LONG) {
		x->thresh = (float)argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->thresh = argv[0].a_w.w_float;
	}
}

void beat_phaselock(t_beat *x, Symbol *s, short argc, Atom *argv) {
	
	if (argv[0].a_type == A_LONG) {
		x->phaselock = (float)argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->phaselock = argv[0].a_w.w_float;
	}	
	if (x->phaselock > 90) x->phaselock = 0.9f;
	else if (x->phaselock < 10) x->phaselock = 0.1f;
	else x->phaselock /= 100.0f;
}

void beat_tempolock(t_beat *x, Symbol *s, short argc, Atom *argv) {

	int val1, val2, val, k;
	int tempoInd1, tempoInd2;
		
	if (argv[0].a_type == A_LONG) {
		x->tempolock = (float)argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->tempolock = argv[0].a_w.w_float;
	}
	
	if (x->tempolock > 100) x->tempolock = 1.0f;
	else if (x->tempolock < 5) x->tempolock = 0.05f;
	else x->tempolock /= 100.0f;
	
	val1 = x->tempoIndex;
	val2 = x->numReson - 1 - x->tempoIndex;
	
	val = (val1 > val2) ? val1 : val2; 
	
	k = (int)(x->tempolock * val);
	
	if (k<1) k = 1;
	
	tempoInd1 = x->tempoIndex + x->tempoInd1 - k;
	tempoInd2 = x->tempoIndex + x->tempoInd1 + k;
	
	if (tempoInd1 < 0) tempoInd1 = 0;
	if (tempoInd2 > x->numReson-1) tempoInd2 = x->numReson-1;
	
	x->tempoInd1 = tempoInd1;
	x->tempoInd2 = tempoInd2;
	x->tempoWindow = k;

}

void beat_temposearch(t_beat *x, Symbol *s, short argc, Atom *argv) {
	double temp1=0, temp2=BIG, tempo;
	int i, tempoInd1 = 1, tempoInd2 = x->numReson-2;
	
	if (argv[0].a_type == A_LONG) {
		temp1 = (float)argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		temp1 = argv[0].a_w.w_float;
	}
	
	if (argv[1].a_type == A_LONG) {
		temp2 = (float)argv[1].a_w.w_long;
	} else if (argv[1].a_type == A_FLOAT) {
		temp2 = argv[1].a_w.w_float;
	}
		
	if (temp1 > temp2) {
		double tmp = temp1;
		temp2 = temp1;
		temp1 = tmp;
	}

	if (temp1 < x->comb[0][0].tempo) temp1 = x->comb[0][0].tempo;
	if (temp2 > x->comb[0][x->numReson-1].tempo) temp2 = x->comb[0][x->numReson-1].tempo;
	
	while (temp1 >  x->comb[0][tempoInd1].tempo) {
		tempoInd1++;
	}
	
	while (temp2 <  x->comb[0][tempoInd2].tempo) {
		tempoInd2--;
	}
	
	x->tempoInd1 = tempoInd1 - 1;
	x->tempoInd2 = tempoInd2 + 1;
	x->tempolock = 1.0;
	
}

void beat_tick(t_beat *x) {

	t_int i, index=0, cpt;
	double energy = 0.0f;
	double newData;
	double *ptr;
	int j, k;
	float m = 0.0f, phase;

	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; i++)
		x->BufFFT[i] = 0.0f;

	// Window the samples
	if (x->x_window != Recta)
		for (i=0; i<x->BufSize; ++i)
			x->BufFFT[i] *= x->WindFFT[i];
			
	// FFT
	fftRealfast(x->FFTSize, x->BufFFT, x->memFFT);
		
	// Squared Absolute
	for (i=0; i<x->FFTSize; i+=2) 
		x->BufFFT[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);

	// Calculate Bark Band energy
	for (i=0; i<x->numBarkBands; i++) {
		cpt = x->BufSizeBark[i];
		energy = 0.0f;
		while (cpt > 0) {
			energy += x->BufFFT[index];
			cpt--;
			index++;
		}
		
		if (energy) {
			x->curInput[i] = 10*log10((energy/x->BufSizeBark[i])/DB_REF);
		} else {
			x->curInput[i] = -DB_REF;					
		}	

		if((x->curRecti[i] = x->curInput[i] - x->oldInput[i]) < 0.0f) {
			x->curRecti[i] = 0.;
		}		

		x->oldInput[i] = x->curInput[i];

		// Calculate temporal masking
		newData = x->heldData[i] * x->cosine[x->index[i]];
		
		if (x->curRecti[i] < newData) {
			x->oldData[i] = newData;
		} else {
			x->oldData[i] = x->curRecti[i];
			x->heldData[i] = x->curRecti[i];
			x->index[i] = 0;
		}
		
		if (x->index[i] < x->numSamps-1) {
			x->index[i]++;
		}
		 
 	}
 	
 	// New frame becomes old	
	ptr = x->curInput;	
	x->curInput = x->oldInput;
	x->oldInput = ptr;
 		
 		
 	// Comb filtering the signal for each single resonator
	for (j=0; j<x->numBarkBands; j++) {
		for (i=0; i<x->numReson; i++) {
			int index = x->comb[j][i].curInd;
			int previous = (index + x->comb[j][i].delay - 1) % x->comb[j][i].delay;
			int next = (index + 1) % x->comb[j][i].delay;
			
			x->comb[j][i].output[index] = x->comb[j][i].alpha * x->comb[j][i].output[next] + (1 - x->comb[j][i].alpha) * x->curRecti[j]; // Comb filter
			x->comb[j][i].curInd = next; // The next current index = the old delayed index in the circular buffer of size 'delay'

			if (x->frameCpt == x->frameRate) {
				x->comb[j][i].energy = filtEnergy(x->comb[j][i].output, x->comb[j][i].delay, x->comb[j][i].tempo) / x->comb[j][i].norm;
			}
		}
	}


	// Estimate best candidate
	if (x->frameCpt == x->frameRate) {
	
		if (x->tempolock < 1.0) {
			x->tempoInd1 = x->absTempoIndex - x->tempoWindow;
			x->tempoInd2 = x->absTempoIndex + x->tempoWindow;	
			if (x->tempoInd1 < 0) x->tempoInd1 = 0;
			if (x->tempoInd2 > x->numReson-1) x->tempoInd2 = x->numReson-1;
		}
	
		for (i=x->tempoInd1; i<x->tempoInd2+1; i++) {
			x->filtEstim[i] = 0.;
			for (j=0; j<x->numBarkBands; j++) {
				x->filtEstim[i] += x->comb[j][i].energy;
			}
			x->filtEstim[i] /= (double)x->numBarkBands;
		}	
		x->frameCpt = 0;
			
		x->tempoIndex = linReg(x, &x->filtEstim[x->tempoInd1], x->tempoInd2+1 - x->tempoInd1);

		x->absTempoIndex = x->tempoIndex + x->tempoInd1;			
		
		cpt = 0;
		for (i=x->tempoInd1; i<x->tempoInd2+1; i++) {
			SETFLOAT(x->myList+cpt, x->filtEstim[i]);
			cpt++;
		}

		outlet_float(x->StrengthOutlet, x->strength);
		outlet_list(x->x_outlet, 0L,  x->tempoInd2+1 - x->tempoInd1, x->myList);
		outlet_float(x->TempoOutlet, x->comb[0][x->tempoIndex + x->tempoInd1].tempo);
 
 
 
 		/* Estimate Phase and maxPhase */	
		m = 0.;
		cpt = x->comb[0][x->absTempoIndex].curInd;
		
		if (x->comb[0][x->absTempoIndex].delay * x->phaselock > x->indexPhase) {
				
		for (i=0; i<x->comb[0][x->absTempoIndex].delay * x->phaselock; i++) {
			phase = 0.;
			for (j=0; j<x->numBarkBands; j++) {
				phase += x->comb[j][x->absTempoIndex].output[cpt];			
			}
			phase /= x->numBarkBands;
			
			if (phase > m) {
				m = phase;
				x->indexPhase = i;
			}
	
			if (cpt == x->comb[0][x->absTempoIndex].delay-1) {
				cpt = 0;
			} else {
				cpt++;
			}
		}
		
		}

	}
	
	x->frameCpt++;
	x->indexPhase--;

	if (x->indexPhase == 0) {
		if(x->strength > x->thresh) outlet_bang(x->BeatOutlet);
		x->indexPhase = x->comb[0][x->absTempoIndex].delay;
	}
				
 }	

#ifdef __ALTIVEC__
void beat_tick_G4(t_beat *x) {

	t_int index=0, cpt;
	double energy = 0.0f;
	double newData;
	double *ptr;
	int i, j, k;
	float m = 0.0f, phase;
	
	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; i++)
		x->BufFFT[i] = 0.0f;

	// Window the samples
	if (x->x_window != Recta)
		for (i=0; i<x->BufSize; ++i)
			x->BufFFT[i] *= x->WindFFT[i];
			
    // Look at the real signal as an interleaved complex vector by casting it.
    // Then call the transformation function ctoz to get a split complex vector,
    // which for a real signal, divides into an even-odd configuration.
    ctoz ((COMPLEX *) x->BufFFT, 2, &x->x_A, 1, x->x_FFTSizeOver2);
      
    // Carry out a Forward FFT transform
    fft_zrip(x->x_setup, &x->x_A, 1, x->x_log2n, FFT_FORWARD);

    // The output signal is now in a split real form.  Use the function
    // ztoc to get a split real vector.
    ztoc ( &x->x_A, 1, (COMPLEX *) x->BufFFT, 2, x->x_FFTSizeOver2);
		
	// Squared Absolute
	for (i=0; i<x->FFTSize; i+=2) 
		x->BufFFT[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);
					
	// Calculate Bark Band energy
	for (i=0; i<x->numBarkBands; i++) {
		cpt = x->BufSizeBark[i];
		energy = 0.0f;
		while (cpt > 0) {
			energy += x->BufFFT[index];
			cpt--;
			index++;
		}
		
		if (energy) {
			x->curInput[i] = 10*log10((energy/x->BufSizeBark[i])/DB_REF);
		} else {
			x->curInput[i] = -DB_REF;					
		}	

		if((x->curRecti[i] = x->curInput[i] - x->oldInput[i]) < 0.0f) {
			x->curRecti[i] = 0.;
		}		

		x->oldInput[i] = x->curInput[i];

		// Calculate temporal masking
		newData = x->heldData[i] * x->cosine[x->index[i]];
		
		if (x->curRecti[i] < newData) {
			x->oldData[i] = newData;
		} else {
			x->oldData[i] = x->curRecti[i];
			x->heldData[i] = x->curRecti[i];
			x->index[i] = 0;
		}
		
		if (x->index[i] < x->numSamps-1) {
			x->index[i]++;
		}
		 
 	}
 	
 	// New frame becomes old	
	ptr = x->curInput;	
	x->curInput = x->oldInput;
	x->oldInput = ptr;
 		
 		
 	// Comb filtering the signal for each single resonator
	for (j=0; j<x->numBarkBands; j++) {
		for (i=0; i<x->numReson; i++) {
			int index = x->comb[j][i].curInd;
			int previous = (index + x->comb[j][i].delay - 1) % x->comb[j][i].delay;
			int next = (index + 1) % x->comb[j][i].delay;
			
			x->comb[j][i].output[index] = x->comb[j][i].alpha * x->comb[j][i].output[next] + (1 - x->comb[j][i].alpha) * x->curRecti[j]; // Comb filter
			x->comb[j][i].curInd = next; // The next current index = the old delayed index in the circular buffer of size 'delay'

			if (x->frameCpt == x->frameRate) {
				x->comb[j][i].energy = filtEnergy(x->comb[j][i].output, x->comb[j][i].delay, x->comb[j][i].tempo) / x->comb[j][i].norm;
			}
		}
	}


	// Estimate best candidate
	if (x->frameCpt == x->frameRate) {
	
		if (x->tempolock < 1.0) {
			x->tempoInd1 = x->absTempoIndex - x->tempoWindow;
			x->tempoInd2 = x->absTempoIndex + x->tempoWindow;	
			if (x->tempoInd1 < 0) x->tempoInd1 = 0;
			if (x->tempoInd2 > x->numReson-1) x->tempoInd2 = x->numReson-1;
		}
	
		for (i=x->tempoInd1; i<x->tempoInd2+1; i++) {
			x->filtEstim[i] = 0.;
			for (j=0; j<x->numBarkBands; j++) {
				x->filtEstim[i] += x->comb[j][i].energy;
			}
			x->filtEstim[i] /= (double)x->numBarkBands;
		}	
		x->frameCpt = 0;
			
		x->tempoIndex = linReg(x, &x->filtEstim[x->tempoInd1], x->tempoInd2+1 - x->tempoInd1);

		x->absTempoIndex = x->tempoIndex + x->tempoInd1;			
		
		cpt = 0;
		for (i=x->tempoInd1; i<x->tempoInd2+1; i++) {
			SETFLOAT(x->myList+cpt, x->filtEstim[i]);
			cpt++;
		}

		outlet_float(x->StrengthOutlet, x->strength);
		outlet_list(x->x_outlet, 0L,  x->tempoInd2+1 - x->tempoInd1, x->myList);
		outlet_float(x->TempoOutlet, x->comb[0][x->tempoIndex + x->tempoInd1].tempo);
 
 
 
 		/* Estimate Phase and maxPhase */	
		m = 0.;
		cpt = x->comb[0][x->absTempoIndex].curInd;
		
		if (x->comb[0][x->absTempoIndex].delay * x->phaselock > x->indexPhase) {
				
		for (i=0; i<x->comb[0][x->absTempoIndex].delay * x->phaselock; i++) {
			phase = 0.;
			for (j=0; j<x->numBarkBands; j++) {
				phase += x->comb[j][x->absTempoIndex].output[cpt];			
			}
			phase /= x->numBarkBands;
			
			if (phase > m) {
				m = phase;
				x->indexPhase = i;
			}
	
			if (cpt == x->comb[0][x->absTempoIndex].delay-1) {
				cpt = 0;
			} else {
				cpt++;
			}
		}
		
		}

	}
	
	x->frameCpt++;
	x->indexPhase--;

	if (x->indexPhase == 0) {
		if(x->strength > x->thresh) outlet_bang(x->BeatOutlet);
		x->indexPhase = x->comb[0][x->absTempoIndex].delay;
	}
	
 }	
#endif // __ALTIVEC_

// Linear Regression
int linReg(t_beat *x, double *vector, int numPoints)
{
	int i, maxIndex;
	double minVal = BIG, maxVal = -BIG;
	double sumx = 0.0f, sumy = 0.0f, sumxx = 0.0f, sumyy = 0.0f, sumxy = 0.0f;
	double Sxx, yy, Sxy, a ,b;
	
	for (i=0; i<numPoints; i++) {


		sumx += (double)i;
		sumy += vector[i];
		sumxx += (double)i*(double)i;
		sumyy += vector[i]*vector[i];
		sumxy += (double)i*vector[i];
		
	}

	Sxx = sumxx-sumx*sumx/(double)numPoints;
	yy = sumyy-sumy*sumy/(double)numPoints;
	Sxy = sumxy-sumx*sumy/(double)numPoints;
	b = Sxy/Sxx;
	a = (sumy-b*sumx)/(double)numPoints;

	for (i=0; i<numPoints; i++) {
		vector[i] = vector[i] - (a+b*(double)i);
		vector[i] *= 100.;
		
		if (vector[i] > maxVal) {
			maxVal = vector[i];
			maxIndex = i;
		} else if (vector[i] < minVal) minVal = vector[i];

	}

	x->strength = (maxVal-minVal);
	if (x->strength == 0) x->strength = 1/BIG;
	
	for (i=0; i<numPoints; i++) {
		vector[i] = (vector[i]-minVal) / x->strength;
	}
	
	x->strength *= 5.0;
	return maxIndex;
}


void beat_clear(t_beat *x, Symbol *sym)
{
	int i,j,k;
	
	for (i=0; i<x->numBarkBands; i++) {
		x->oldInput[i] = 0.0f;
		x->curRecti[i] = 0.0f;
    	x->oldData[i] = 0.0f;
    	x->heldData[i] = 0.0f;
    	x->index[i] = 0;		
	}	

/*
	for (j=0; j<x->numBarkBands; j++) {
		for (i=0; i<x->numReson; i++) {
			for (k=0; k<x->comb[j][i].delay; k++) {
				x->comb[j][i].output = 0;
			}
		}
	}		
*/
}


// Computes the ceiling of log2(n) 
// i.e. log2max(7) = 3, log2max(8) = 3, log2max(9) = 4
long log2max(long n) {
	
	long power = 1;
	long k = 1;
	
	if (n==1) return 0;
	while ((k <<= 1) < n) power++;
	
	return power;
}

double filtEnergy(double *buf, int bufSize, double tempo) 
{
	int i;
	double pwr=0., sum=0.;
	
	for(i=0; i<bufSize; i++) {
		pwr += buf[i]*buf[i];
	}
	pwr = sqrt(pwr);

	for(i=0; i<bufSize; i++) {
		sum += pwr - buf[i];
	}
	return (sum/bufSize);
}
