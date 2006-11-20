/*	analyzer~ object by Tristan Jehan
	Copyright (c) 2001 Masachussetts Institute of Technology.
	All rights reserved.

	pitch tracker based on Miller Puckette's fiddle~
	Copyright (c) 1997-1999 University of California San Diego.
	All rights reserved.
	
	version 1.2.1 Matt Wright fixed pitch_getit() bounds error and added debug info
	version 1.2.2 Matt Wright more debug info: addr of most recent input sig vector and gettime()
	version 1.3 implements an altivec-optimized FFT and adds more windows
*/

#include "ext.h"
#include "z_dsp.h"
#include "fft.h"
#include <string.h>
#include <math.h>
#include <Accelerate/Accelerate.h> // to get veclib


// Add altivec function prototypes
#ifdef __ALTIVEC__
#include "vDSP.h"
#endif

#define DEBUG
#ifdef DEBUG
#define debug if (x->x_debug) post
#else
#define debug /* Do nothing */
#endif

#define VERSION "1.3"
#define RES_ID	7079
#define NUMBAND 25 // at 44100 Hz only (should be fixed in future version)
#define t_floatarg double
#define DEFAULT_FS 44100
#define DB_REF 96
#define SFM_MAX 60
//#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define THREEPI 9.424777960769379f
#define DEFBUFSIZE 1024		// Default signal buffer size
#define MAXPADDING 16		// Maximum FFT zero padding (in # of FFT sizes)
#define MAXDELAY 512		// Maximum initial delay (in # of signal vectors)
#define DEFDELAY 0			// Default initial delay (in # of signal vectors) 
#define DEFNPITCH 1			// Default number of pitches to output
#define DEFNPEAKANAL 20		// Default number of peaks to analyse 
#define DEFNPEAKOUT 0		// Default number of peaks to output
#define DEFNPARTIAL 7		// Default number of partials for threshold
#define DEFAMPLO 40			// Default Low attack threshold
#define DEFAMPHI 50			// Default High attack threshold
#define DEFATTACKTIME 100	// Default minimum attack update rate
#define DEFATTACKTHRESH 10	// Default attack threshold
#define DEFVIBTIME 50		// Default vibrato update rate
#define DEFVIBDEPTH 0.5		// Default vibrato depth (1 semitone)
#define MINFREQINBINS 5     // Minimum frequency in bins for reliable output
#define HISTORY 20			// Number of old values kept
#define MAXNPITCH 3			// Maximum number of pitch outputs
#define MAXNPEAK 100		// Maximum number of peaks
#define MINBIN 3			// Minimum FFT bin
#define BINPEROCT 48		// bins per octave
#define MINBW 0.03f			// consider BW >= 0.03 FFT bins
#define GLISS 0.7f			// Pitch glissando
#define BINAMPCOEFF 30.0f	// Don't know how to describe this
#define DBFUDGE 30.8f		// Don't know how to describe this
#define BPEROOVERLOG2 69.24936196f // BINSPEROCT/log(2)
#define FACTORTOBINS 275.00292191f // 4/(pow(2.0,1/48.0)-1)
#define BINGUARD 10			// extra bins to throw in front
#define PARTIALDEVIANCE 0.023f // acceptable partial detuning in %
#define LOGTODB 4.34294481903f // 10/log(10)
#define KNOCKTHRESH 4e5f 	// don't know how to describe this
#define MAXHIST 3		    // find N hottest peaks in histogram
#define POWERTHRES 1e-9f	// Total power minimum threshold
#define FIDDLEDB_REF 100.0f	// Fiddle dB Reference

#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN62_W(i,ac) (0.44859f - 0.49364f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.05677f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN70_W(i,ac) (0.42323f - 0.49755f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.07922f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN74_W(i,ac) (0.402217f - 0.49703f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.09892f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.00188 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN92_W(i,ac) (0.35875f - 0.48829f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.14128f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.01168 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))

#define MINF(A,B) ((A < B) ? A : B)
#define ftom pitch_ftom
#define mtof pitch_mtof
#define flog log
#define fexp exp
#define fsqrt sqrt

static t_float pitch_partialonset[] = {
	0, 48,76.0782000346154967102, 96, 111.45254855459339269887, 124.07820003461549671089,
	134.75303625876499715823, 144, 152.15640006923099342109, 159.45254855459339269887,
	166.05271769459026829915, 172.07820003461549671088, 177.62110647077242370064,
	182.75303625876499715892, 187.53074858920888940907, 192
};

static t_int pitch_intpartialonset[] = {
	0, 48, 76, 96, 111, 124, 135, 144, 152, 159, 166, 172, 178, 183, 188, 192
};

#define NPARTIALONSET ((t_int)(sizeof(pitch_partialonset)/sizeof(t_float)))

void *analyzer_class;

enum {Recta=0, Hann, Hamm, Blackman62, Blackman70, Blackman74, Blackman92};
enum {Log=0, Linear};
enum {List=0, noList};

#define DEFWIN	Blackman70		// Default window

// Some structures from Fiddle~
typedef struct peakout {    // a peak for output
    t_float po_freq;		// frequency in Hz
    t_float po_amp;	    	// amplitude
} t_peakout;

typedef struct peak {	    // a peak for analysis
    t_float p_freq;		    // frequency in bins
    t_float p_width;		// peak width in bins
    t_float p_pow;		    // peak power
    t_float p_loudness;	    // 4th root of power
    t_float *p_fp;		    // pointer back to spectrum
} t_peak;

typedef struct histopeak {	// Histogram for peaks
    t_float h_pitch;		// estimated pitch
    t_float h_value;		// value of peak
    t_float h_loud;		    // combined strength of found partials
    t_int h_index;		    // index of bin holding peak
    t_int h_used;			// true if an x_hist entry points here
} t_histopeak;

typedef struct pitchhist {		// struct for keeping history by pitch
    t_float h_pitch;		    // pitch to output
    t_float h_amps[HISTORY];	// past amplitudes
    t_float h_pitches[HISTORY]; // past pitches
    t_float h_noted;		    // last pitch output
    t_int h_age;			    // number of frames pitch has been there
    t_histopeak *h_wherefrom;	// new histogram peak to incorporate
} t_pitchhist;

// The actual main external structure
typedef struct _analyzer {

	t_pxobject x_obj;

	t_float x_Fs;			// Sample rate
	t_int x_overlap;		// Number of overlaping samples
	t_int x_hop;			// Number of non-overlaping samples
	t_int x_window;			// Type of window	
	char x_winName[32];		// Window name	
	t_int x_delay;			// Vector size delay before to start feeding the buffer
	t_int x_counter;		// Counter that goes with the vector size delay
	t_int x_scale;			// Type of output scale (log=0, linear=1)	
	t_int x_output;			// Type of output
	t_int x_bright;			// Type of brightness
	t_int x_loud;			// Type of loudness
	t_float x_loudness;		// Current loudness
	t_float x_brightness;	// Current brightness
	t_float x_noisiness;	// Current noisiness

	// Variables from Fiddle~
	t_int x_npitch;			// Number of pitches to output
	t_int x_npeakanal;		// Number of peaks to analyse
	t_int x_npeakout;		// Number of peaks to output
    t_int x_histphase;		// Phase into amplitude history vector
    t_pitchhist x_hist[MAXNPITCH]; // History of current pitches
    t_float x_dbs[HISTORY];	// DB history, indexed by "histphase"
    t_float x_peaked;		// peak since last attack
    t_int x_dbage;		    // number of bins DB has met threshold
    t_int x_attackvalue;
	t_peak x_peaklist[MAXNPEAK+1]; // This was originally a local buffer in pitch_getit
	t_histopeak x_histvec[MAXHIST];// This one too
		
	// Parameters from fiddle~
    t_float x_amplo;
    t_float x_amphi;
    t_int x_attacktime;
    t_int x_attackbins;
    t_float x_attackthresh;
    t_int x_vibtime;
    t_int x_vibbins;
    t_float x_vibdepth;
    t_float x_npartial;

	// Buffers
    t_int *Buf1;			// buffer 1 : Use buffers of integers to copy faster
    t_int *Buf2;			// buffer 2
    t_float *BufFFT;		// FFT buffer
    t_float *BufPower;		// Power spectrum buffer
    t_float *WindFFT;		// Window of FFTSize
    t_float *memFFT;		// fft.c memory space
    t_peakout *peakBuf;		// Spectral peaks for output
    t_float *histBuf;		// Histogram Buffer

    t_int BufSize;			// FFT buffer size
	t_int FFTSize;			// Size of FFT
	UInt32 x_FFTSizeOver2;	// Size of FFT/2 (UInt32 in G4 FFT)
    t_int BufWritePos;		// Where to write in buffer

    t_float *BufBark;		// Bark buffer
	t_int *BufSizeBark;		// Number of bins per band

	void **x_out;			// Outlet for all Bands
	void *x_clock;			// Use a clock for outputs... (better than Qelem)
	Atom *myList;			// Copy of outputs as Atoms
	void *x_outlet;			// List of outputs for Bark decomposition
	void *x_outloud;		// Outlet for the loudness
	void *x_outbright;		// Outlet for the brightness
	void *x_outnois;		// Outlet for the noisiness
    void *x_attackout;		// Outlet for Onset detection
    void *x_noteout;		// Outlet for cooked pitch
    void *x_peakout;		// Outlet for sinusoidal decomposition
    void *x_pitchout;		// Outlet for raw pitch & amplitude
		
	int x_debug;				// Debug mode?
	t_float *lastInputVector;	// Remembered from last analyzer_perform() for debug into
	t_int lastInputVectorSize;	// ditto

#ifdef __ALTIVEC__ // Additional stuff for managing the G4-optimized FFT by Apple
#pragma altivec_model on
	t_float x_scaleFactor;
	UInt32 x_log2n;
    COMPLEX_SPLIT x_A;
	FFTSetup x_setup;
#pragma altivec_model off
#endif
	
} t_analyzer;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman62, *ps_blackman70, *ps_blackman74, *ps_blackman92, 
		 *ps_list, *ps_nolist;

t_int *analyzer_perform(t_int *w);
void analyzer_dsp(t_analyzer *x, t_signal **sp, short *connect);
void analyzer_log(t_analyzer *x);
void analyzer_linear(t_analyzer *x);
void analyzer_loud(t_analyzer *x, t_symbol *s, short argc, t_atom *argv);
void analyzer_bright(t_analyzer *x, t_symbol *s, short argc, t_atom *argv);
void analyzer_float(t_analyzer *x, double f);
void analyzer_int(t_analyzer *x, long n);
void analyzer_assist(t_analyzer *x, void *b, long m, long a, char *s);
void analyzer_print(t_analyzer *x);
void analyzer_tellmeeverything(t_analyzer *x);
void analyzer_amprange(t_analyzer *x, t_floatarg amplo, t_floatarg amphi);
void analyzer_reattack(t_analyzer *x, t_floatarg attacktime, t_floatarg attackthresh);
void analyzer_vibrato(t_analyzer *x, t_floatarg vibtime, t_floatarg vibdepth);
void analyzer_npartial(t_analyzer *x, t_floatarg npartial);
void readBufSize(t_analyzer *x, t_atom *argv);
void readx_overlap(t_analyzer *x, t_atom *argv);
void readFFTSize(t_analyzer *x, t_atom *argv);
void readx_window(t_analyzer *x, t_atom *argv);
void readx_delay(t_analyzer *x, t_atom *argv);
void readx_output(t_analyzer *x, t_atom *argv);
void readx_npitch(t_analyzer *x, t_atom *argv);
void readx_npeakanal(t_analyzer *x, t_atom *argv);
void readx_npeakout(t_analyzer *x, t_atom *argv);
void *analyzer_new(t_symbol *s, short argc, t_atom *argv);
void analyzer_free(t_analyzer *x);
void analyzer_tick(t_analyzer *x);
t_float pitch_mtof(t_float f);
t_float pitch_ftom(t_float f);
t_int pitch_ilog2(t_int n);
void pitch_getit(t_analyzer *x); // modified fiddle pitch tracker function
void analyzer_debug(t_analyzer *x, long n);

#ifdef __ALTIVEC__
void analyzer_tick_G4(t_analyzer *x);
long log2max(long n);
#endif


int main(void) {

    post("Analyzer~ object version " VERSION " by Tristan Jehan");
    post("copyright © 2001 Massachusetts Institute of Technology");
    post("Pitch tracker based on Miller Puckette's fiddle~");
    post("copyright © 1997-1999 Music Department UCSD");
    post(" ");

	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman62 = gensym("blackman62");
	ps_blackman70 = gensym("blackman70");
	ps_blackman74 = gensym("blackman74");
	ps_blackman92 = gensym("blackman92");
	ps_list = gensym("list");
	ps_nolist = gensym("nolist");

	setup((Messlist **)&analyzer_class, (method)analyzer_new, (method)analyzer_free, (short)sizeof(t_analyzer), 0L, A_GIMME, 0);
		
	addmess((method)analyzer_dsp, "dsp", A_CANT, 0);
	addmess((method)analyzer_assist, "assist", A_CANT, 0);
	addmess((method)analyzer_log, "log", A_GIMME, 0);
	addmess((method)analyzer_linear, "linear", A_GIMME, 0);
	addmess((method)analyzer_loud, "loud", A_GIMME, 0);
	addmess((method)analyzer_bright, "bright", A_GIMME, 0);
    addmess((method)analyzer_print, "print", 0);
    addmess((method)analyzer_tellmeeverything, "tellmeeverything", 0);
    addmess((method)analyzer_amprange, "amp-range", A_FLOAT, A_FLOAT, 0);
    addmess((method)analyzer_reattack, "reattack", A_FLOAT, A_FLOAT, 0);
    addmess((method)analyzer_vibrato, "vibrato", A_FLOAT, A_FLOAT, 0);
   	addmess((method)analyzer_npartial, "npartial", A_FLOAT, 0);
   	addmess((method)analyzer_debug, "debug", A_LONG, 0);
	
	addfloat((method)analyzer_float);
	addint((method)analyzer_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
	return 0;
}

t_int *analyzer_perform(t_int *w) {

	t_float *in = (t_float *)(w[1]);
	t_analyzer *x = (t_analyzer *)(w[2]);
	t_int n = (int)(w[3]);

	t_int *myintin = (t_int *)in; 				// Copy integers rather than floats -> faster
	t_int *myintBufFFT = (t_int *)(x->BufFFT);	// We assume sizeof(float) == sizeof(int) though
	t_int i, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->x_overlap;
	t_int *TmpBuf = x->Buf1;
			
    if (x->x_obj.z_disabled)
    	goto skip;

	
#ifdef DEBUG
	x->lastInputVector = in;
	x->lastInputVectorSize = n;
#endif
	
	if (x->x_counter < 1) {
	
		// Copy input samples into FFT buffer	
		while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
			x->Buf1[x->BufWritePos] = myintin[index];
			x->BufWritePos++;
			index++;
			cpt--;
		}
		
		debug("perf[%ld]", x->BufWritePos);
	
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
			debug("set clock");
		
			// Swap buffers
			x->Buf1 = x->Buf2;
			x->Buf2 = TmpBuf;
		}
	} else {
		x->x_counter--;
	}
	
	
#ifdef DEBUG
	/* Count down the debug value each time so we don't flood the Max window with text */
 	if (x->x_debug > 0) --(x->x_debug);
#endif 
	
skip:	
	return (w+4);
}

void analyzer_dsp(t_analyzer *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	// Initializing the delay counter
	x->x_counter = x->x_delay;

	if (vs > x->BufSize) post("Analyzer~: You need to use a smaller signal vector size...");
	else if (connect[0]) dsp_add(analyzer_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}

void analyzer_log(t_analyzer *x) {
 		x->x_scale = Log;
}

void analyzer_linear(t_analyzer *x) {
 		x->x_scale = Linear;
}

void analyzer_bright(t_analyzer *x, t_symbol *s, short argc, t_atom *argv) {
	if (argv[0].a_type == A_LONG) {
		if (argv[0].a_w.w_long == 0) {
			x->x_bright = 0;
		} else {
			x->x_bright = 1;
		}
	} else if (argv[0].a_type == A_FLOAT) {
		if (argv[0].a_w.w_float == 0.0f) {
			x->x_bright = 0;
		} else {
			x->x_bright = 1;
		}
	}
}

void analyzer_loud(t_analyzer *x, t_symbol *s, short argc, t_atom *argv) {
	if (argv[0].a_type == A_LONG) {
		if (argv[0].a_w.w_long == 0) {
			x->x_loud = 0;
		} else {
			x->x_loud = 1;
		}
	} else if (argv[0].a_type == A_FLOAT) {
		if (argv[0].a_w.w_float == 0.0f) {
			x->x_loud = 0;
		} else {
			x->x_loud = 1;
		}
	}
}

void analyzer_float(t_analyzer *x, double f) {

	int n = (t_int)(f * x->x_Fs/1000.0f); 
	analyzer_int(x, n);
}

void analyzer_int(t_analyzer *x, long n) {
	t_int vs = sys_getblksize();

	x->x_hop = n; 
	if (x->x_hop < vs) {
		post("Analyzer~: You can't overlap so much...");
		x->x_hop = vs;
	} else if (x->x_hop > x->BufSize) {
		x->x_hop = x->BufSize;
	}
	x->x_overlap = x->BufSize - x->x_hop;	
}

void analyzer_assist(t_analyzer *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID,m,a,1,2,s);
}

void analyzer_print(t_analyzer *x) {
    post("amp-range %.2f %.2f",  x->x_amplo, x->x_amphi);
    post("reattack %d %.2f",  x->x_attacktime, x->x_attackthresh);
    post("vibrato %d %.2f",  x->x_vibtime, x->x_vibdepth);
    post("npartial %.2f",  x->x_npartial);
    post("loud %d",  x->x_loud);
    post("bright %d",  x->x_bright);
    if (x->x_scale == Log) {
    	post("log");
    } else {
    	post("linear");
    }
}

#define TELLi(var) post("  " #var ": %ld", x->var);
#define TELLf(var) post("  " #var ": %f", x->var);
#define TELLp(var) post("  " #var ": %p", x->var);

void analyzer_tellmeeverything(t_analyzer *x) {
	long time;
	
	post("");
	post("Analyzer~ state:");
	
	TELLi(BufSize);
	TELLi(BufWritePos);
	TELLp(Buf1);
	TELLp(Buf2);
	TELLp(BufFFT);
	TELLp(lastInputVector);
	TELLi(lastInputVectorSize);
	
	analyzer_print(x);
	
	time = gettime();
	post("gettime(): %ld", time);
}

void analyzer_amprange(t_analyzer *x, t_floatarg amplo, t_floatarg amphi) {
    if (amplo < 0) amplo = 0;
    if (amphi < amplo) amphi = amplo + 1;
    x->x_amplo = amplo;
    x->x_amphi = amphi;
}

void analyzer_reattack(t_analyzer *x, t_floatarg attacktime, t_floatarg attackthresh) {
    if (attacktime < 0) attacktime = 0;
    if (attackthresh <= 0) attackthresh = 1000;
    x->x_attacktime = attacktime;
    x->x_attackthresh = attackthresh;
    x->x_attackbins = (t_int) (x->x_Fs * 0.001f * attacktime) / (x->BufSize - x->x_overlap);
    if (x->x_attackbins >= HISTORY) x->x_attackbins = HISTORY - 1;
}

void analyzer_vibrato(t_analyzer *x, t_floatarg vibtime, t_floatarg vibdepth) {
    if (vibtime < 0) vibtime = 0;
    if (vibdepth <= 0) vibdepth = 1000;
    x->x_vibtime = vibtime;
    x->x_vibdepth = vibdepth;
    x->x_vibbins = (t_int) (x->x_Fs * 0.001f * vibtime) / (x->BufSize - x->x_overlap);
    if (x->x_vibbins >= HISTORY) x->x_vibbins = HISTORY - 1;
    if (x->x_vibbins < 1) x->x_vibbins = 1;
}

void analyzer_npartial(t_analyzer *x, t_floatarg npartial) {
    if (npartial < 0.1) npartial = 0.1;
    x->x_npartial = npartial;
}

void readBufSize(t_analyzer *x, t_atom *argv) {
    t_float ms2samp = x->x_Fs * 0.001f;
    
	if (argv[0].a_type == A_LONG) {
		x->BufSize = argv[0].a_w.w_long; // Samples
	} else if (argv[0].a_type == A_FLOAT) {
		x->BufSize = (t_int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	} else {
		x->BufSize = DEFBUFSIZE;
	}
}

void readx_overlap(t_analyzer *x, t_atom *argv) {
    t_float ms2samp = x->x_Fs * 0.001f;

	if (argv[1].a_type == A_LONG) {
		x->x_hop = argv[1].a_w.w_long; // Samples
	} else if (argv[1].a_type == A_FLOAT) {
		x->x_hop = (t_int)(argv[1].a_w.w_float * ms2samp); // Time in ms
	} else {
		x->x_hop = x->BufSize/2;
	}
	x->x_overlap = x->BufSize - x->x_hop;
}

void readFFTSize(t_analyzer *x, t_atom *argv) {
    t_float ms2samp = x->x_Fs * 0.001f;
    
	if (argv[2].a_type == A_LONG) {
		x->FFTSize = argv[2].a_w.w_long; // Samples
	} else if (argv[2].a_type == A_FLOAT) {
		x->FFTSize = (t_int)(argv[2].a_w.w_float * ms2samp); // Time in ms
	} else {
		x->FFTSize = x->BufSize;
	}
}

void readx_window(t_analyzer *x, t_atom *argv) {
	if (argv[3].a_w.w_sym == ps_rectangular) {
		x->x_window = Recta;
	} else if (argv[3].a_w.w_sym == ps_hanning) {
		x->x_window = Hann;
	} else if (argv[3].a_w.w_sym == ps_hamming) {
		x->x_window = Hamm;
	} else if (argv[3].a_w.w_sym == ps_blackman62) {
		x->x_window = Blackman62;
	} else if (argv[3].a_w.w_sym == ps_blackman70) {
		x->x_window = Blackman70;
	} else if (argv[3].a_w.w_sym == ps_blackman74) {
		x->x_window = Blackman74;
	} else if (argv[3].a_w.w_sym == ps_blackman92) {
		x->x_window = Blackman92;
	} else {
		x->x_window = DEFWIN;
	}
}

void readx_delay(t_analyzer *x, t_atom *argv) {
    
	if ((argv[4].a_type == A_LONG) && (argv[4].a_w.w_long >= 0) && (argv[4].a_w.w_long < MAXDELAY)) {
		x->x_delay = argv[4].a_w.w_long;
	} else if ((argv[4].a_type == A_FLOAT) && (argv[4].a_w.w_float >= 0) && (argv[4].a_w.w_float < MAXDELAY)) {
		x->x_delay = (t_int)(argv[4].a_w.w_float);
	} else {
		post("Analyzer~: 'delay' argument may be out of range... Choosing default...");
		x->x_delay = DEFDELAY;
	}
}

void readx_npitch(t_analyzer *x, t_atom *argv) {
    
	if ((argv[5].a_type == A_LONG) && (argv[5].a_w.w_long >= 0) && (argv[5].a_w.w_long <= MAXNPITCH)) {
		x->x_npitch = argv[5].a_w.w_long;
	} else if ((argv[5].a_type == A_FLOAT) && (argv[5].a_w.w_float >= 0) && (argv[5].a_w.w_float <= MAXNPITCH)) {
		x->x_npitch = (t_int)(argv[5].a_w.w_float);
	} else {
		post("Analyzer~: '# of pitches' argument may be out of range... Choosing default...");
		x->x_npitch = DEFNPITCH;
	}
}

void readx_npeakanal(t_analyzer *x, t_atom *argv) {
    
	if ((argv[6].a_type == A_LONG) && (argv[6].a_w.w_long >= 0) && (argv[6].a_w.w_long <= MAXNPEAK)) {
		x->x_npeakanal = argv[6].a_w.w_long;
	} else if ((argv[6].a_type == A_FLOAT) && (argv[6].a_w.w_float >= 0) && (argv[6].a_w.w_float <= MAXNPEAK)) {
		x->x_npeakanal = (t_int)(argv[6].a_w.w_float);
	} else {
		post("Analyzer~: '# of peaks to find' argument may be out of range... Choosing default...");
		x->x_npeakanal = DEFNPEAKANAL;
	}
}

void readx_npeakout(t_analyzer *x, t_atom *argv) {
    
	if ((argv[7].a_type == A_LONG) && (argv[7].a_w.w_long >= 0) && (argv[7].a_w.w_long <= MAXNPEAK)) {
		x->x_npeakout = argv[7].a_w.w_long;
	} else if ((argv[7].a_type == A_FLOAT) && (argv[7].a_w.w_float >= 0) && (argv[7].a_w.w_float <= MAXNPEAK)) {
		x->x_npeakout = (t_int)(argv[7].a_w.w_float);
	} else {
		post("Analyzer~: '# of peaks to output' argument may be out of range... Choosing default...");
		x->x_npeakout = DEFNPEAKOUT;
	}
}

void readx_output(t_analyzer *x, t_atom *argv) {
	if (argv[8].a_w.w_sym == ps_nolist) {
		x->x_output = noList;
	} else  {
		x->x_output = List;
	}
}

void *analyzer_new(t_symbol *s, short argc, t_atom *argv) {
	t_int i, j, band=0, oldband=0, sizeband=0;
	t_int vs = sys_getblksize(); // get vector size
	double freq = 0.0f, oldfreq = 0.0f;
    t_analyzer *x = (t_analyzer *)newobject(analyzer_class);
    dsp_setup((t_pxobject *)x,1); // one inlet	
	x->x_Fs = sys_getsr();
	x->BufWritePos = 0;
	x->x_scale = Log;
	x->x_loud = 0;
	x->x_bright = 0;
	x->x_loudness = 0.0f;
	x->x_brightness = 0.0f;
	x->x_noisiness = 0.0f;

	// From fiddle~
    x->x_histphase = 0;
    x->x_dbage = 0;
    x->x_peaked = 0;
    x->x_amplo = DEFAMPLO;
    x->x_amphi = DEFAMPHI;
    x->x_attacktime = DEFATTACKTIME;
    x->x_attackbins = 1; // real value calculated afterward
    x->x_attackthresh = DEFATTACKTHRESH;
    x->x_vibtime = DEFVIBTIME;
    x->x_vibbins = 1;	 // real value calculated afterward
    x->x_vibdepth = DEFVIBDEPTH;
    x->x_npartial = DEFNPARTIAL;
    x->x_attackvalue = 0;

	// More initializations from Fiddle~
    for (i=0; i<MAXNPITCH; i++) {
		x->x_hist[i].h_pitch = x->x_hist[i].h_noted = 0.0f;
		x->x_hist[i].h_age = 0;
		x->x_hist[i].h_wherefrom = NULL;
		
		for (j=0; j<HISTORY; j++)
	    	x->x_hist[i].h_amps[j] = x->x_hist[i].h_pitches[j] = 0.0f;
    }
        
    for (i=0; i<HISTORY; i++) 
    	x->x_dbs[i] = 0.0f;

	switch (argc) { // Read arguments
		case 0: 
			x->BufSize = DEFBUFSIZE;
			x->x_overlap = x->BufSize/2;
			x->x_hop = x->BufSize/2;
			x->FFTSize = DEFBUFSIZE;
			x->x_window = DEFWIN;
			x->x_delay = DEFDELAY;
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 1:
			readBufSize(x,argv);
			x->x_overlap = x->BufSize/2;
			x->x_hop = x->BufSize/2;
			x->FFTSize = x->BufSize;
			x->x_window = DEFWIN;
			x->x_delay = DEFDELAY;
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 2:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			x->FFTSize = x->BufSize;
			x->x_window = DEFWIN;
			x->x_delay = DEFDELAY;
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 3:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			readFFTSize(x,argv);
			x->x_window = DEFWIN;
			x->x_delay = DEFDELAY;
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 4:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			readFFTSize(x,argv);
			readx_window(x,argv);
			x->x_delay = DEFDELAY;
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 5:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
			x->x_npitch = DEFNPITCH;
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 6:
			readBufSize(x,argv);
			readx_overlap(x,argv);			
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
			readx_npitch(x,argv);
			x->x_npeakanal = DEFNPEAKANAL;
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 7:
			readBufSize(x,argv);
			readx_overlap(x,argv);			
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
			readx_npitch(x,argv);
			readx_npeakanal(x,argv);
			x->x_npeakout = DEFNPEAKOUT;
			x->x_output = List;
			break;
		case 8:
			readBufSize(x,argv);
			readx_overlap(x,argv);			
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
			readx_npitch(x,argv);
			readx_npeakanal(x,argv);
			readx_npeakout(x,argv);
			x->x_output = List;
			break;
		default:
			readBufSize(x,argv);
			readx_overlap(x,argv);			
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
			readx_npitch(x,argv);
			readx_npeakanal(x,argv);
			readx_npeakout(x,argv);
			readx_output(x,argv);
	}		

	if (x->x_npeakout > x->x_npeakanal) {
		post("Analyzer~: You can't output more peaks than you pick...");
		x->x_npeakout = x->x_npeakanal;
	}
	
	// Make an outlet for peaks out
	if (x->x_npeakout)
    	x->x_peakout = listout((t_object *)x); // one list out

 	// One outlet for fundamental & amplitude raw values
	if (x->x_npitch)
		x->x_pitchout = listout((t_object *)x);

	// Make bang outlet for onset detection
	x->x_attackout = bangout((t_object *)x);

	// Just storing the name of the window
	switch(x->x_window) {
		case 0:
			strcpy(x->x_winName,"rectangular");
			break;
		case 1:
			strcpy(x->x_winName,"hanning");
			break;		
		case 2:
			strcpy(x->x_winName,"hamming");
			break;		
		case 3:
			strcpy(x->x_winName,"blackman62");
			break;		
		case 4:
			strcpy(x->x_winName,"blackman70");
			break;		
		case 5:
			strcpy(x->x_winName,"blackman74");
			break;		
		case 6:
			strcpy(x->x_winName,"blackman92");
			break;		
		default:
			strcpy(x->x_winName,"blackman62");
	}
	
	if (x->BufSize < vs) { 
		post("Analyzer~: Buffer size is smaller than the vector size, %d",vs);
		x->BufSize = vs;
	} else if (x->BufSize > 65536) {
		post("Analyzer~: Maximum FFT size is 65536 samples");
		x->BufSize = 65536;
	}
	
	if (x->FFTSize < x->BufSize) {
		post("Analyzer~: FFT size is at least the buffer size, %d",x->BufSize);
		x->FFTSize = x->BufSize;
	}

	if ((x->FFTSize > vs) && (x->FFTSize < 128))  x->FFTSize = 128;
	else if ((x->FFTSize > 128) && (x->FFTSize < 256)) x->FFTSize = 256;
	else if ((x->FFTSize > 256) && (x->FFTSize < 512)) x->FFTSize = 512;
	else if ((x->FFTSize > 512) && (x->FFTSize < 1024)) x->FFTSize = 1024;
	else if ((x->FFTSize > 1024) && (x->FFTSize < 2048)) x->FFTSize = 2048;
	else if ((x->FFTSize > 2048) && (x->FFTSize < 4096)) x->FFTSize = 4096;
	else if ((x->FFTSize > 8192) && (x->FFTSize < 16384)) x->FFTSize = 16384;
	else if ((x->FFTSize > 16384) && (x->FFTSize < 32768)) x->FFTSize = 32768;
	else if ((x->FFTSize > 32768) && (x->FFTSize < 65536)) x->FFTSize = 65536;
	else if (x->FFTSize > 65536) {
		post("Analyzer~: Maximum FFT size is 65536 samples");
		x->FFTSize = 65536;
	}
	
	// Overlap case
	if (x->x_overlap > x->BufSize-vs) {
		post("Analyzer~: You can't overlap so much...");
		x->x_overlap = x->BufSize-vs;
	} else if (x->x_overlap < 1)
		x->x_overlap = 0; 

	x->x_hop = x->BufSize - x->x_overlap;
	x->x_FFTSizeOver2 = x->FFTSize/2;		

	post("--- Analyzer~ ---");	
	post("	Buffer size = %d",x->BufSize);
	post("	Hop size = %d",x->x_hop);
	post("	FFT size = %d",x->FFTSize);
	post("	Window type = %s",x->x_winName);
	post("	Initial delay = %d",x->x_delay);
	post("	Number of pitches = %d",x->x_npitch);
	post("	Number of peaks to search = %d",x->x_npeakanal);
	post("	Number of peaks to output = %d",x->x_npeakout);

	// Here comes the choice for altivec optimization or not...
	if (sys_optimize()) { // note that we DON'T divide the vector size by four here

#ifdef __ALTIVEC__ // More code and a new ptr so that x->BufFFT is vector aligned.
#pragma altivec_model on 
		x->x_clock = clock_new(x,(method)analyzer_tick_G4); // Call altivec-optimized tick function
		post("	Using G4-optimized FFT");	
		// Allocate some memory for the altivec FFT
		x->x_A.realp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_A.imagp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_log2n = log2max(x->FFTSize);
      	x->x_setup = create_fftsetup (x->x_log2n, 0);
    	x->x_scaleFactor = (t_float)1.0/(2.0*x->FFTSize);
#pragma altivec_model off
#else
		error("  No G4 optimization available");
#endif

	} else { // Normal tick function
		x->x_clock = clock_new(x,(method)analyzer_tick);
		x->memFFT = (t_float*) NewPtr(CMAX * x->FFTSize * sizeof(t_float)); // memory allocated for normal fft twiddle
	}
	post("");


	// Allocate memory
	x->Buf1 = (t_int*) NewPtr(x->BufSize * sizeof(t_float)); // Careful these are pointers to integers but the content is floats
	x->Buf2 = (t_int*) NewPtr(x->BufSize * sizeof(t_float));
	x->BufFFT = (t_float*) NewPtr(x->FFTSize * sizeof(t_float));
	x->BufPower = (t_float*) NewPtr((x->FFTSize/2) * sizeof(t_float));
	x->WindFFT = (t_float*) NewPtr(x->BufSize * sizeof(t_float));
	x->peakBuf = (t_peakout*) NewPtr(x->x_npeakout * sizeof(t_peakout)); // from Fiddle~
	x->histBuf = (t_float*) NewPtr((x->FFTSize + BINGUARD) * sizeof(t_float)); // for Fiddle~

	if (x->x_Fs != DEFAULT_FS) {
		error("Analyzer~: WARNING !!! Object set for 44.1 KHz only");
		return 0;
	} else {
		x->BufBark = (t_float*) NewPtr(2*NUMBAND * sizeof(t_float));
		x->BufSizeBark = (t_int*) NewPtr(NUMBAND * sizeof(t_int));
	}

	// Create the Bark outlet(s)
	if (x->x_output == noList) {
		// Allocate memory for all outlets
		x->x_out = (void**) NewPtr(NUMBAND * sizeof(t_float*));
		for (i=0; i<NUMBAND; i++) {
			x->x_out[i] = floatout((t_analyzer *)x); // Create float outlets
		}
	} else {
		x->myList   = (Atom*) NewPtr(NUMBAND * sizeof(*x->myList));     
		x->x_outlet = listout((t_analyzer *)x);	// Create a list outlet
	}

	// Create the Loudness/Brightness outlet
	x->x_outnois = floatout((t_analyzer *)x); // one outlet for noisiness
	x->x_outbright = floatout((t_analyzer *)x); // one outlet for brightness
	x->x_outloud = floatout((t_analyzer *)x); // one outlet for loudness
	x->x_noteout = listout((t_analyzer *)x); // one outlet for MIDI & frequency cooked pitch

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

	// More initializations from Fiddle~
	for (i=0; i<x->x_npeakout; i++)
		x->peakBuf[i].po_freq = x->peakBuf[i].po_amp = 0.0f;
	
	j = 1;	
	x->BufBark[0] = 0.0f;

	// Compute and store Analyzer scale	
	for (i=0; i<x->FFTSize/2; i++) {
		freq = (i*x->x_Fs)/x->FFTSize;
		band = floor(13*atan(0.76*freq/1000) + 3.5*atan((freq/7500)*(freq/7500)));
		if (oldband != band) {
			x->BufBark[j] = oldfreq;
			x->BufBark[j+1] = freq;
			x->BufSizeBark[j/2] = sizeband;
			j+=2;
			sizeband = 0;
		}
		oldband = band;
		oldfreq = freq;
		sizeband++;
	}

	x->BufBark[2*NUMBAND-1] = freq;
	x->BufSizeBark[NUMBAND-1] = sizeband;

	x->x_debug = 0;

    return (x);
}

void  analyzer_free(t_analyzer *x) {
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
	if (x->BufPower != NULL) DisposePtr((char *) x->BufPower);
	if (x->WindFFT != NULL) DisposePtr((char *) x->WindFFT);
	if (x->peakBuf != NULL) DisposePtr((char *) x->peakBuf);
	if (x->histBuf != NULL) DisposePtr((char *) x->histBuf);
	if (x->x_clock != NULL) freeobject((t_object *)x->x_clock);
	if (x->BufBark != NULL) DisposePtr((char *) x->BufBark);
	if (x->BufSizeBark != NULL) DisposePtr((char *) x->BufSizeBark);
	if (x->x_out != NULL) DisposePtr((char *) x->x_out);
	if (x->myList != NULL) DisposePtr((char *) x->myList);

	dsp_free((t_pxobject *)x);
}

void analyzer_tick(t_analyzer *x) {

	t_int i, index=0, cpt;
	t_float bark = 0.0f, loud = 0.0f, bright = 0.0f, sumSpectrum = 0.0f, SFM = 0.0f;
	t_float FsOverFFTSize = x->x_Fs/x->FFTSize; // Keep it here since x_Fs may change
	t_float FsOverBarkSize = x->x_Fs/(2.0f*NUMBAND); // Fix that problem in a next version
	double prod = 1.0f, sum = 0.0f;
	double invNumBand = 0.04f;
    t_pitchhist *ph;

	debug("Entering analyzer_tick");
	
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
		x->BufPower[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);
	
	// Go into fiddle~ code
	pitch_getit(x);
					
	// Output Band energy (find brightness #1)
	for (i=0; i<NUMBAND; i++) {
		cpt = x->BufSizeBark[i];
		bark = 0.0f;
		while (cpt > 0) {
			bark += x->BufPower[index];
			cpt--;
			index++;
		}
		bark = bark/x->BufSizeBark[i];
		
		// for Noisiness
		sum += bark;
		prod *= bark;

		// Special Brightness case
		if (x->x_bright != 0) {
			bright += ((i+0.5) * bark); // center around half the bandwidth of the band
			sumSpectrum += bark;
		}
		
		// Special Loudness case
		if (x->x_loud != 0) {
			loud += bark;
		}
		
		// output
		if (x->x_output == noList) {
			if (x->x_scale) {
				outlet_float(x->x_out[NUMBAND-1-i], bark);
			} else {
				outlet_float(x->x_out[NUMBAND-1-i], 10*log10(bark/DB_REF));
 			}
 		} else {
			if (x->x_scale) {
				SETFLOAT(x->myList+i, bark);
			} else {
				SETFLOAT(x->myList+i, 10*log10(bark/DB_REF));
 			}
 		}
 	}

	// loudness
	if (x->x_loud == 0) {
		for (i=1; i<x->x_FFTSizeOver2; i++) {
			loud += x->BufPower[i];
		}
		x->x_loudness = loud / (x->x_FFTSizeOver2-1);
	} else {
		x->x_loudness = loud / NUMBAND;
	}
	
	if (x->x_scale == Log) {
		if (x->x_loudness > 0) {
			x->x_loudness = 10*log10(x->x_loudness/DB_REF);
		} else {
			x->x_loudness = -DB_REF;
		}
	}
	 		 		
 	// for Noisiness
 	prod = pow(prod,invNumBand);
 	sum  = invNumBand * sum;
	if (sum!=0) {
	 	SFM = prod/sum;
	} else {
		SFM = 0.;
	}
	
	// Spectral Flatness Measure (SFM)
 	if (SFM > 0) {
		SFM = 10*log10(prod/sum);
	} else {
		SFM = 0.0f;
	}
	
	// Tonality factor or Peakiness
	x->x_noisiness = MINF((SFM/-SFM_MAX),1); // minimum

	// for Brightness
	if (x->x_bright == 0) {
	
		// Absolute
		for (i=1; i<x->x_FFTSizeOver2; i++) 
			x->BufPower[i] = sqrtf(x->BufPower[i]);

		// Brightness
		for (i=1; i<x->x_FFTSizeOver2; i++) {
			bright += (i * x->BufPower[i]);
			sumSpectrum += x->BufPower[i];
		}
	
		if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
		else x->x_brightness = (bright * FsOverFFTSize) / sumSpectrum;
	} else {
		if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
		else x->x_brightness = (bright * FsOverBarkSize) / sumSpectrum;
	}		

 	// Output Loudness/Brightness/Noisiness
	outlet_float(x->x_outloud, x->x_loudness);
    outlet_float(x->x_outbright, x->x_brightness);
    outlet_float(x->x_outnois, (1.0 - x->x_noisiness));

	// Output pitch results
    if (x->x_npeakout) { // Output peaks
    	t_peakout *po;
    	for (i=0, po=x->peakBuf; i<x->x_npeakout; i++, po++) {
			t_atom at[3];
	    	SETLONG(at, i+1);
	    	SETFLOAT(at+1, po->po_freq);
	    	SETFLOAT(at+2, po->po_amp);
	    	outlet_list(x->x_peakout, 0, 3, at);
		}
    }
      
    // Output fundamental frequencies + amplitudes
    if (x->x_npitch > 1) {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[3];
			SETLONG(at, i+1);
			SETFLOAT(at+1, ph->h_pitches[x->x_histphase]);
			SETFLOAT(at+2, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 3, at);
   		}
   	} else {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[2];
			SETFLOAT(at, ph->h_pitches[x->x_histphase]);
			SETFLOAT(at+1, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 2, at);
   		}
	}   	
       
    // Output cooked MIDI/Frequency pitch
    if (x->x_npitch > 1) {
	    for (i=0, ph=x->x_hist; i<x->x_npitch; i++, ph++)
 			if (ph->h_pitch) {
				t_atom at[3];
				SETLONG(at, i+1);
				SETFLOAT(at+1, ph->h_pitch);
				SETFLOAT(at+2, mtof(ph->h_pitch));
				outlet_list(x->x_noteout, 0, 3, at);
			}
	} else {
		ph = x->x_hist;
 		if (ph->h_pitch) {
			t_atom at[2];
			SETFLOAT(at, ph->h_pitch);
			SETFLOAT(at+1, mtof(ph->h_pitch));
			outlet_list(x->x_noteout, 0, 2, at);
		}
	}
			
    // Output attack bang 
    if (x->x_attackvalue) outlet_bang(x->x_attackout);

 	// Only for lists
	if (x->x_output == List) {
 		outlet_list(x->x_outlet, 0L, NUMBAND, x->myList);
 	}
 
 	debug("leaving analyzer_tick");
 }	

#ifdef __ALTIVEC__
void analyzer_tick_G4(t_analyzer *x) {

	t_int i, index=0, cpt;
	t_float bark = 0.0f, loud = 0.0f, bright = 0.0f, sumSpectrum = 0.0f, SFM = 0.0f;
	t_float FsOverFFTSize = x->x_Fs/x->FFTSize; // Keep it here since x_Fs may change
	t_float FsOverBarkSize = x->x_Fs/(2.0f*NUMBAND); // Fix that problem in a next version
	double prod = 1.0f, sum = 0.0f;
	double invNumBand = 0.04f;
    t_pitchhist *ph;

	debug("Entering analyzer_tick_G4");
	
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

	// Fast rescaling required
//    vsmul( x->x_A.realp, 1, &x->x_scaleFactor, x->x_A.realp, 1, x->x_FFTSizeOver2);
//    vsmul( x->x_A.imagp, 1, &x->x_scaleFactor, x->x_A.imagp, 1, x->x_FFTSizeOver2);

    // The output signal is now in a split real form.  Use the function
    // ztoc to get a split real vector.
    ztoc ( &x->x_A, 1, (COMPLEX *) x->BufFFT, 2, x->x_FFTSizeOver2);
		
	// Squared Absolute
	for (i=0; i<x->FFTSize; i+=2) 
		x->BufPower[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);
	
	// Go into fiddle~ code
	pitch_getit(x);
					
	// Output Band energy (find brightness #1)
	for (i=0; i<NUMBAND; i++) {
		cpt = x->BufSizeBark[i];
		bark = 0.0f;
		while (cpt > 0) {
			bark += x->BufPower[index];
			cpt--;
			index++;
		}
		bark = bark/x->BufSizeBark[i];
		
		// for Noisiness
		sum += bark;
		prod *= bark;

		// Special Brightness case
		if (x->x_bright != 0) {
			bright += ((i+0.5) * bark); // center around half the bandwidth of the band
			sumSpectrum += bark;
		}
		
		// Special Loudness case
		if (x->x_loud != 0) {
			loud += bark;
		}
		
		// output
		if (x->x_output == noList) {
			if (x->x_scale) {
				outlet_float(x->x_out[NUMBAND-1-i], bark);
			} else {
				outlet_float(x->x_out[NUMBAND-1-i], 10*log10(bark/DB_REF));
 			}
 		} else {
			if (x->x_scale) {
				SETFLOAT(x->myList+i, bark);
			} else {
				SETFLOAT(x->myList+i, 10*log10(bark/DB_REF));
 			}
 		}
 	}

	// loudness
	if (x->x_loud == 0) {
		for (i=1; i<x->x_FFTSizeOver2; i++) {
			loud += x->BufPower[i];
		}
		x->x_loudness = loud / (x->x_FFTSizeOver2-1);
	} else {
		x->x_loudness = loud / NUMBAND;
	}
	
	if (x->x_scale == Log) {
		if (x->x_loudness > 0) {
			x->x_loudness = 10*log10(x->x_loudness/DB_REF);
		} else {
			x->x_loudness = -DB_REF;
		}
	}
	 		 		
 	// for Noisiness
 	prod = pow(prod,invNumBand);
 	sum  = invNumBand * sum;
	if (sum!=0) {
	 	SFM = prod/sum;
	} else {
		SFM = 0.;
	}
	
	// Spectral Flatness Measure (SFM)
 	if (SFM > 0) {
		SFM = 10*log10(prod/sum);
	} else {
		SFM = 0.0f;
	}
	
	// Tonality factor or Peakiness
	x->x_noisiness = MINF((SFM/-SFM_MAX),1); // minimum

	// for Brightness
	if (x->x_bright == 0) {
	
		// Absolute
		for (i=1; i<x->x_FFTSizeOver2; i++) 
			x->BufPower[i] = sqrtf(x->BufPower[i]);

		// Brightness
		for (i=1; i<x->x_FFTSizeOver2; i++) {
			bright += (i * x->BufPower[i]);
			sumSpectrum += x->BufPower[i];
		}
	
		if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
		else x->x_brightness = (bright * FsOverFFTSize) / sumSpectrum;
	} else {
		if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
		else x->x_brightness = (bright * FsOverBarkSize) / sumSpectrum;
	}		

 	// Output Loudness/Brightness/Noisiness
	outlet_float(x->x_outloud, x->x_loudness);
    outlet_float(x->x_outbright, x->x_brightness);
    outlet_float(x->x_outnois, (1.0 - x->x_noisiness));

	// Output pitch results
    if (x->x_npeakout) { // Output peaks
    	t_peakout *po;
    	for (i=0, po=x->peakBuf; i<x->x_npeakout; i++, po++) {
			t_atom at[3];
	    	SETLONG(at, i+1);
	    	SETFLOAT(at+1, po->po_freq);
	    	SETFLOAT(at+2, po->po_amp);
	    	outlet_list(x->x_peakout, 0, 3, at);
		}
    }
      
    // Output fundamental frequencies + amplitudes
    if (x->x_npitch > 1) {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[3];
			SETLONG(at, i+1);
			SETFLOAT(at+1, ph->h_pitches[x->x_histphase]);
			SETFLOAT(at+2, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 3, at);
   		}
   	} else {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[2];
			SETFLOAT(at, ph->h_pitches[x->x_histphase]);
			SETFLOAT(at+1, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 2, at);
   		}
	}   	
       
    // Output cooked MIDI/Frequency pitch
    if (x->x_npitch > 1) {
	    for (i=0, ph=x->x_hist; i<x->x_npitch; i++, ph++)
 			if (ph->h_pitch) {
				t_atom at[3];
				SETLONG(at, i+1);
				SETFLOAT(at+1, ph->h_pitch);
				SETFLOAT(at+2, mtof(ph->h_pitch));
				outlet_list(x->x_noteout, 0, 3, at);
			}
	} else {
		ph = x->x_hist;
 		if (ph->h_pitch) {
			t_atom at[2];
			SETFLOAT(at, ph->h_pitch);
			SETFLOAT(at+1, mtof(ph->h_pitch));
			outlet_list(x->x_noteout, 0, 2, at);
		}
	}
			
    // Output attack bang 
    if (x->x_attackvalue) outlet_bang(x->x_attackout);

 	// Only for lists
	if (x->x_output == List) {
 		outlet_list(x->x_outlet, 0L, NUMBAND, x->myList);
 	}
 
 	debug("leaving analyzer_tick_G4");
 }	
#endif // __ALTIVEC__


// Convert from MIDI to Hz and Hz to MIDI
t_float pitch_mtof(t_float f) {
	return (8.17579891564f * exp(.0577622650f * f));
}

t_float pitch_ftom(t_float f) {
	return (17.3123405046f * log(.12231220585f * f));
}

t_int pitch_ilog2(t_int n) {
    t_int ret = -1;
    
    while (n) {
		n >>= 1;
		ret++;
    }
    return (ret);
}

// This is the actual Fiddle~ code
void pitch_getit(t_analyzer *x)
{
    t_int i, j, k;
    t_peak *pk1; // peaks found
    t_peakout *pk2; // peaks to output
    t_histopeak *hp1;
    t_float power_spec = 0.0f, total_power = 0.0f, total_loudness = 0.0f, total_db = 0.0f;
    t_float *fp1, *fp2;
    t_float *spec = x->BufFFT, *powSpec = x->BufPower, threshold, mult;
    t_int n = x->FFTSize/2;
    t_int npitch, newphase, oldphase, npeak = 0;
    t_int logn = pitch_ilog2(n);
    t_float maxbin = BINPEROCT * (logn-2);
    t_float hzperbin = x->x_Fs/x->FFTSize;
    t_float coeff = x->FFTSize/(t_float)x->BufSize;
    t_float *histogram = x->histBuf + BINGUARD;
    t_int npeaktot = (x->x_npeakout > x->x_npeakanal ? x->x_npeakout : x->x_npeakanal);
    t_pitchhist *phist;
       
    debug("entering pitch_getit");
    
    // Circular buffer for History
    oldphase = x->x_histphase;
    newphase = x->x_histphase + 1;
    if (newphase == HISTORY) newphase = 0;
    x->x_histphase = newphase;

	// Get spectrum power
	for (i=0; i<n; i++)
		power_spec += powSpec[i];
			    
	total_power = 4.0f * power_spec; // Compensate for fiddle~ power estimation (difference of 6 dB)

    if (total_power > POWERTHRES) {
		total_db = (FIDDLEDB_REF-DBFUDGE) + LOGTODB*flog(total_power/n); // dB power estimation of fiddle~
		total_loudness = fsqrt(fsqrt(power_spec)); // Use the actual real estimation rather than fiddle~'s
		if (total_db < 0) total_db = 0.0f;
    } else {
    	total_db = total_loudness = 0.0f;
    }
    
	// Store new db in history vector
    x->x_dbs[newphase] = total_db;

	// Not enough power to find anything
    if (total_db < x->x_amplo) goto nopow;

	// search for peaks
	pk1 = x->x_peaklist;
	
	debug("*** About to look for peaks.  npeaktot %ld    n %ld    FFTsize  %ld", npeaktot, n, x->FFTSize);
		
	// Matt changed this from (i<n-2) to (i<n-3) */
    for (i=MINBIN, fp1=spec+2*MINBIN, fp2=powSpec+MINBIN; (i<n-3) && (npeak<npeaktot); i++, fp1+=2, fp2++) {    	 
		t_float height = fp2[0], h1 = fp2[-1], h2 = fp2[1]; // Bin power and adjacents
		t_float totalfreq, pfreq, f1, f2, m, var, stdev;
	
		// Get worried if you see this
		if (fp1+7 >= spec+x->FFTSize) {
    		post("*** fp1 %p, fp1+7 %p, spec %p, spec+FFTsize %p", fp1, fp1+7, spec, spec+x->FFTSize);
    	}
	
		if (height<h1 || height<h2 || h1*coeff<POWERTHRES*total_power || h2*coeff<POWERTHRES*total_power) continue; // Go to next

    	// Use an informal phase vocoder to estimate the frequency
		pfreq = ((fp1[-4] - fp1[4]) * (2.0f * fp1[0] - fp1[4] - fp1[-4]) +
				 (fp1[-3] - fp1[5]) * (2.0f * fp1[1] - fp1[5] - fp1[-3])) / (2.0f * height);
		    
    	// Do this for the two adjacent bins too
		f1 = ((fp1[-6] - fp1[2]) * (2.0f * fp1[-2] - fp1[2] - fp1[-6]) +
			  (fp1[-5] - fp1[3]) * (2.0f * fp1[-1] - fp1[3] - fp1[-5])) / (2.0f * h1) - 1;
		f2 = ((fp1[-2] - fp1[6]) * (2.0f * fp1[2] - fp1[6] - fp1[-2]) +
			  (fp1[-1] - fp1[7]) * (2.0f * fp1[3] - fp1[7] - fp1[-1])) / (2.0f * h2) + 1;

    	// get sample mean and variance of the three
		m = 0.333333f * (pfreq + f1 + f2);
		var = 0.5f * ((pfreq-m)*(pfreq-m) + (f1-m)*(f1-m) + (f2-m)*(f2-m));

		totalfreq = i + m;
		
		// BAD HACK TO BE CHANGED IN NEXT VERSION !!!!
		if (coeff > 1) {
			switch ((t_int)coeff) {
				case 2:
					mult = 0.005;
					break;
				case 4:
					mult = 0.125;
					break;
				case 8:
					mult = 0.2;
					break;
				case 16:
					mult = 0.25; // weird values found by trying to get npeak around 6-7
					break;
				default:
					mult = 0.25;
			}			
			threshold = KNOCKTHRESH * height * mult;
		} else {
			threshold = KNOCKTHRESH * height;
		}

		if ((var * total_power) > threshold || (var < 1e-30)) continue;

		stdev = fsqrt(var);
		if (totalfreq < 4) totalfreq = 4;
		
		// Store the peak info in the list of peaks
		pk1->p_width = stdev;
		pk1->p_pow = height;
		pk1->p_loudness = fsqrt(fsqrt(height));
		pk1->p_fp = fp1;
		pk1->p_freq = totalfreq;
	
		npeak++;
		pk1++;
    } // end for
		
    // prepare the raw peaks for output
    for (i=0, pk1=x->x_peaklist, pk2=x->peakBuf; i<npeak; i++, pk1++, pk2++) {
    	
    	t_float loudness = pk1->p_loudness;
    	if (i>=x->x_npeakout) break;
    	
    	pk2->po_freq = hzperbin * pk1->p_freq;
    	pk2->po_amp = (2.f/(t_float)n) * loudness * loudness * coeff;
    }
        
    // in case npeak < x->x_npeakout
    for (; i<x->x_npeakout; i++, pk2++) pk2->po_amp = pk2->po_freq = 0;

	// now, make a sort of "likelihood" spectrum. Proceeding in 48ths of an octave,  
	// from 2 to n/2 (in bins), the likelihood of each pitch range is contributed
	// to by every peak in peaklist that's an integer multiple of it in frequency

    if (npeak > x->x_npeakanal) npeak = x->x_npeakanal; // max # peaks to analyze
        
    // Initialize histogram buffer to 0
    for (i=0, fp1=histogram; i<maxbin; i++) *fp1++ = 0.0f;

    for (i=0, pk1=x->x_peaklist; i<npeak; i++, pk1++) {
    
		t_float pit = BPEROOVERLOG2 * flog(pk1->p_freq) - 96.0f;
		t_float binbandwidth = FACTORTOBINS * pk1->p_width/pk1->p_freq;
		t_float putbandwidth = (binbandwidth < 2 ? 2 : binbandwidth);
		t_float weightbandwidth = (binbandwidth < 1.0f ? 1.0f : binbandwidth);
		t_float weightamp = 4.0f * pk1->p_loudness / total_loudness;

		for (j=0, fp2=pitch_partialonset; j<NPARTIALONSET; j++, fp2++) {
	    	t_float bin = pit - *fp2;
	    	if (bin<maxbin) {
				t_float para, pphase, score = BINAMPCOEFF * weightamp / ((j+x->x_npartial) * weightbandwidth);
				t_int firstbin = bin + 0.5f - 0.5f * putbandwidth;
				t_int lastbin = bin + 0.5f + 0.5f * putbandwidth;
				t_int ibw = lastbin - firstbin;
				if (firstbin < -BINGUARD) break;
				para = 1.0f / (putbandwidth * putbandwidth);
				for (k=0, fp1=histogram+firstbin, pphase=firstbin-bin; k<=ibw; k++, fp1++, pphase+=1.0f)
		    		*fp1 += score * (1.0f - para * pphase * pphase);
	    	}
		} // end for
    } // end for
      
    //post("npeaks = %d, %f",npeak,mult); // For debugging weird hack!!!
   
    
	// Next we find up to NPITCH strongest peaks in the histogram.
	// If a peak is related to a stronger one via an interval in
	// the pitch_partialonset array, we suppress it.

    for (npitch=0; npitch<x->x_npitch; npitch++) {
		t_int index;
		t_float best;
		if (npitch) {
	    	for (best=0, index=-1, j=1; j<maxbin-1; j++) {
				if ((histogram[j]>best) && (histogram[j]>histogram[j-1]) && (histogram[j]>histogram[j+1])) {
		    		for (k=0; k<npitch; k++)
						if (x->x_histvec[k].h_index == j) goto peaknogood;
		    		for (k=0; k<NPARTIALONSET; k++) {
						if ((j-pitch_intpartialonset[k]) < 0) break;
						if (histogram[j-pitch_intpartialonset[k]] > histogram[j]) goto peaknogood;
					}
		    		for (k=0; k<NPARTIALONSET; k++) {
						if (j+ pitch_intpartialonset[k] >= maxbin) break;
						if (histogram[j+pitch_intpartialonset[k]] > histogram[j]) goto peaknogood;
		    		}
		    		index = j;
		    		best = histogram[j];
				}
	    		peaknogood: ;
	    	}
		} else {
			best = 0; 
			index = -1;
	    	for (j=0; j<maxbin; j++)
				if (histogram[j] > best) {
		    		index = j; 
		    		best = histogram[j];
		    	}
		}

		if (index < 0) break;
	
		x->x_histvec[npitch].h_value = best;
		x->x_histvec[npitch].h_index = index;
    }
       
	// for each histogram peak, we now search back through the
	// FFT peaks.  A peak is a pitch if either there are several
	// harmonics that match it, or else if (a) the fundamental is
	// present, and (b) the sum of the powers of the contributing peaks
	// is at least 1/100 of the total power.
	//
	// A peak is a contributor if its frequency is within 25 cents of
	// a partial from 1 to 16.
	//
	// Finally, we have to be at least 5 bins in frequency, which
	// corresponds to 2-1/5 periods fitting in the analysis window.

    for (i=0; i<npitch; i++) {
    	t_float cumpow=0, cumstrength=0, freqnum=0, freqden=0;
		t_int npartials=0,  nbelow8=0;
	    // guessed-at frequency in bins
		t_float putfreq = fexp((1.0f / BPEROOVERLOG2) * (x->x_histvec[i].h_index + 96.0f));
	
		for (j=0; j<npeak; j++) {
	    	t_float fpnum = x->x_peaklist[j].p_freq/putfreq;
	    	t_int pnum = fpnum + 0.5f;
	    	t_float fipnum = pnum;
	    	t_float deviation;
	    
	    	if ((pnum>16) || (pnum<1)) continue;
	    
	    	deviation = 1.0f - fpnum/fipnum;
	   		if ((deviation > -PARTIALDEVIANCE) && (deviation < PARTIALDEVIANCE)) {
		 	// we figure this is a partial since it's within 1/4 of
		 	// a halftone of a multiple of the putative frequency.
				t_float stdev, weight;
				npartials++;
				if (pnum<8) nbelow8++;
				cumpow += x->x_peaklist[j].p_pow;
				cumstrength += fsqrt(fsqrt(x->x_peaklist[j].p_pow));
				stdev = (x->x_peaklist[j].p_width > MINBW ? x->x_peaklist[j].p_width : MINBW);
				weight = 1.0f / ((stdev*fipnum) * (stdev*fipnum));
				freqden += weight;
				freqnum += weight * x->x_peaklist[j].p_freq/fipnum;		
	    	} // end if
		} // end for
	
		if (((nbelow8<4) || (npartials<DEFNPARTIAL)) && (cumpow < (0.01f * total_power))) {
			x->x_histvec[i].h_value = 0;
		} else {
	  	  	t_float pitchpow = (cumstrength * cumstrength * cumstrength * cumstrength);
			t_float freqinbins = freqnum/freqden;
		
			// check for minimum output frequency
			if (freqinbins < MINFREQINBINS) {
				x->x_histvec[i].h_value = 0;
			} else {
    		    // we passed all tests... save the values we got
	    		x->x_histvec[i].h_pitch = ftom(hzperbin * freqnum/freqden);
	    		x->x_histvec[i].h_loud = (FIDDLEDB_REF-DBFUDGE) + LOGTODB*flog(pitchpow*coeff/n);
	    	}	
		} // end else
    } // end for
    
	// Now try to find continuous pitch tracks that match the new pitches. 
	// First mark each peak unmatched.

    for (i=0, hp1=x->x_histvec; i<npitch; i++, hp1++)
		hp1->h_used = 0;

	// For each old pitch, try to match a new one to it.
    for (i=0, phist=x->x_hist; i<x->x_npitch; i++, phist++) {
		t_float thispitch = phist->h_pitches[oldphase];
		phist->h_pitch = 0;	    // no output, thanks...
		phist->h_wherefrom = 0;
		if (thispitch == 0.0f) continue;
		for (j=0, hp1=x->x_histvec; j<npitch; j++, hp1++)
	    	if ((hp1->h_value > 0) && (hp1->h_pitch > thispitch - GLISS) && (hp1->h_pitch < thispitch + GLISS)) {
	    		phist->h_wherefrom = hp1;
	    		hp1->h_used = 1;
			}
    }
    
    for (i=0, hp1=x->x_histvec; i<npitch; i++, hp1++)
		if ((hp1->h_value > 0) && !hp1->h_used) {
			for (j=0, phist=x->x_hist; j<x->x_npitch; j++, phist++)
	    		if (!phist->h_wherefrom) {
	    			phist->h_wherefrom = hp1;
					phist->h_age = 0;
					phist->h_noted = 0;
					hp1->h_used = 1;
					goto happy;
				}
				break;
    			happy: ;
    	} // end if
    	
	// Copy the pitch info into the history vector
    for (i=0, phist=x->x_hist; i<x->x_npitch; i++, phist++) {
		if (phist->h_wherefrom) {
			phist->h_amps[newphase] = phist->h_wherefrom->h_loud;
			phist->h_pitches[newphase] = phist->h_wherefrom->h_pitch;
			(phist->h_age)++;
		} else {
			phist->h_age = 0;
			phist->h_amps[newphase] = phist->h_pitches[newphase] = 0;
		}
    } // end for
    	
	// Look for envelope attacks
    x->x_attackvalue = 0;

    if (x->x_peaked) {
		if (total_db > x->x_amphi) {
			t_int binlook = newphase - x->x_attackbins;
	    	if (binlook < 0) binlook += HISTORY;
	    	if (total_db > x->x_dbs[binlook] + x->x_attackthresh) {
				x->x_attackvalue = 1;
				x->x_peaked = 0;
	    	}
		}
    } else {
		t_int binlook = newphase - x->x_attackbins;
		if (binlook < 0) binlook += HISTORY;
		if ((x->x_dbs[binlook] > x->x_amphi) && (x->x_dbs[binlook] > total_db))
	    	x->x_peaked = 1;
    }

	// For each current frequency track, test for a new note using a
	// stability criterion. Later perhaps we should also do as in
	// pitch~ and check for unstable notes a posteriori when
	// there's a new attack with no note found since the last onset;
	// but what's an attack &/or onset when we're polyphonic?

    for (i=0, phist=x->x_hist; i<x->x_npitch; i++, phist++) {
    	// if we've found a pitch but we've now strayed from it, turn it off
		if (phist->h_noted) {
	    	if (phist->h_pitches[newphase] > phist->h_noted + x->x_vibdepth
				|| phist->h_pitches[newphase] < phist->h_noted - x->x_vibdepth)
				phist->h_noted = 0;
		} else {
			if (phist->h_wherefrom && phist->h_age >= x->x_vibbins) {
				t_float centroid = 0;
				t_int not = 0;
				for (j=0, k=newphase; j<x->x_vibbins; j++) {
					centroid += phist->h_pitches[k];
					k--;
					if (k<0) k = HISTORY-1;
				}
				centroid /= x->x_vibbins;
				for (j=0, k=newphase; j<x->x_vibbins; j++) {
					// calculate deviation from norm
					t_float dev = centroid - phist->h_pitches[k];
					k--;
		    		if (k<0) k = HISTORY-1;
					if ((dev > x->x_vibdepth) || (-dev > x->x_vibdepth)) not = 1;
				}
				if (!not) {
		    		phist->h_pitch = phist->h_noted = centroid;
		    	}
	    	} // end if
	    } // end else
	}
    return;

	nopow:

    for (i=0; i<x->x_npitch; i++) {
		x->x_hist[i].h_pitch = 
		x->x_hist[i].h_noted =
	    x->x_hist[i].h_pitches[newphase] =
	    x->x_hist[i].h_amps[newphase] =
		x->x_hist[i].h_age = 0;
    }
    x->x_peaked = 1;
    x->x_dbage = 0;

	return;
}

// Function added by Matt
void analyzer_debug(t_analyzer *x, long n) {
	post("Analyzer~: debug is %ld", n);
	x->x_debug = n;
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

