/*
  analyzer~ object by Tristan Jehan
  Copyright (c) 2001 Masachussetts Institute of Technology.
  All rights reserved.
 
  pitch tracker based on Miller Puckette's fiddle~
  Copyright (c) 1997-1999 University of California San Diego.
  All rights reserved.
 
  Copyright (c) 2008.  The Regents of the University of California (Regents).
  All Rights Reserved.
 
  Permission to use, copy, modify, and distribute this software and its
  documentation for educational, research, and not-for-profit purposes, without
  fee and without a signed licensing agreement, is hereby granted, provided that
  the above copyright notice, this paragraph and the following two paragraphs
  appear in all copies, modifications, and distributions.  Contact The Office of
  Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
  CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.
 
  Written by Adrian Freed, Matt Wright,  and Michael Zbyszynski, The Center for New Music and Audio Technologies,
  University of California, Berkeley.
 
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
 
  analyzer.c
 
 
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  NAME: analyzer~
  DESCRIPTION: FFT-Based Perceptual Analysis
  AUTHORS: Tristan Jehan, Adrian Freed, Matt Wright, and Michael Zbyszynski
  COPYRIGHT_YEARS: 1988,89,90-99,2000,01,02,03,04,05, 11
  DRUPAL_NODE: /patch/xxxx
  SVN_REVISION: $LastChangedRevision$
  version 1.2.1: Matt Wright fixed pitch_getit() bounds error and added debug info
  version 1.2.2: Matt Wright more debug info: addr of most recent input sig vector and gettime()
  version 1.3: implements an altivec-optimized FFT and adds more windows
  version 1.3.1: Port to Universal Binary, assist strings, changed free() routine to call dsp_free() *before* freeing memory. - mzed
  version 1.4: Sample rate agnostic - mzed
  version 1.4.1: fixed twiddle bug in fft code - mzed
  version 1.4.2: rewrote the error messages to be more informative - JM
  version 1.4.3: the hop size is now calculated when the dsp chain is compiled - JM
  version 1.4.4: changed the clock free method, but think there might be a different bug in the free method.
  version 1.4.5: addex obex include for object_free -mzed
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
*/
#define NAME "analyzer~"
#define DESCRIPTION "FFT-Based Perceptual Analysis"
#define AUTHORS "Tristan Jehan, Adrian Freed, Matt Wright, Michael Zbyszynski, John MacCallum, and Rama Gottfried"
#define COPYRIGHT_YEARS "1988,89,90-99,2000,01-05,11-17"

#include "ext.h"
#include "ext_obex.h"
#include "ext_critical.h"
#include "version.h"

#include "z_dsp.h"
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "fftw3.h"
//#include "fft.h"

#define OSC 1
#ifdef OSC
#include "osc_bundle_u.h"
#include "osc_message_u.h"
#include "osc_atom_u.h"
#include "osc_mem.h"
#include "osc_timetag.h"
//#define __OSC_PROFILE__
#include "osc_profile.h"
#endif

#if !defined(WINDOWS) && !defined(WIN_VERSION)
#include <Accelerate/Accelerate.h> // to get veclib
#endif

//#define DEBUG
#ifdef DEBUG
#define debug if (x->x_debug) post
#else
#define debug /* Do nothing */
#endif

#define MAXBUFSIZE 65536

#define NUMBAND 25 // at 44100 Hz only (should be fixed in future version)
#define t_floatarg double
#define DEFAULT_FS 44100
#define DB_REF 96
#define SFM_MAX 60
//#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292
#define THREEPI 9.424777960769379
#define DEFBUFSIZE 1024		// Default signal buffer size
#define MAXPADDING 16		// Maximum FFT zero padding (in # of FFT sizes)
#define MAXDELAY 512		// Maximum initial delay (in # of signal vectors)
#define DEFDELAY 0			// Default initial delay (in # of signal vectors) 
#define DEFNPITCH 1			// Default number of pitches to output
#define DEFNPEAKANAL 20		// Default number of peaks to analyse 
#define DEFNPEAKOUT 1		// Default number of peaks to output
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
#define MINBW 0.03			// consider BW >= 0.03 FFT bins
#define GLISS 0.7			// Pitch glissando
#define BINAMPCOEFF 30.0	// Don't know how to describe this
#define DBFUDGE 30.8f		// Don't know how to describe this
#define BPEROOVERLOG2 69.24936196 // BINSPEROCT/log(2)
#define FACTORTOBINS 275.00292191 // 4/(pow(2.0,1/48.0)-1)
#define BINGUARD 10			// extra bins to throw in front
#define PARTIALDEVIANCE 0.023 // acceptable partial detuning in %
#define LOGTODB 4.34294481903 // 10/log(10)
#define KNOCKTHRESH 4e5 	// don't know how to describe this
#define MAXHIST 3		    // find N hottest peaks in histogram
#define POWERTHRES 1e-9	// Total power minimum threshold
#define FIDDLEDB_REF 100.0	// Fiddle dB Reference

#define MINF(A,B) ((A < B) ? A : B)
#define ftom pitch_ftom
#define mtof pitch_mtof


static double pitch_partialonset[] = {
	0, 48,76.0782000346154967102, 96, 111.45254855459339269887, 124.07820003461549671089,
	134.75303625876499715823, 144, 152.15640006923099342109, 159.45254855459339269887,
	166.05271769459026829915, 172.07820003461549671088, 177.62110647077242370064,
	182.75303625876499715892, 187.53074858920888940907, 192
};

static long pitch_intpartialonset[] = {
	0, 48, 76, 96, 111, 124, 135, 144, 152, 159, 166, 172, 178, 183, 188, 192
};

#define NPARTIALONSET ((long)(sizeof(pitch_partialonset)/sizeof(t_float)))

t_class *analyzer_class;

enum {Recta=0, Hann, Hamm, Blackman62, Blackman70, Blackman74, Blackman92, windowcount};
enum {List=0, noList};

#define DEFWIN	Blackman70		// Default window

static double rect_w(int i, int ac)
{
	return 1.0;
}
static double hanning_w(int i, int ac)
{
	return ((1.0 - cos((i * TWOPI) / (ac - 1.0))) * 0.5);
}
static double hamming_w(int i, int ac)
{
	return (0.54 - 0.46 * cos((TWOPI * i) / (ac - 1.0)));
}
static double blackman62_w(int i, int ac)
{ 
	return (0.44859 - 0.49364 * cos(TWOPI * ((i - 1.0)/(ac - 1.0))) + 0.05677 * cos(FOURPI * ((i - 1.0)/(ac - 1.0))));
}
static double blackman70_w(int i, int ac)
{
	return (0.42323 - 0.49755 * cos(TWOPI * ((i - 1.0)/(ac - 1.0))) + 0.07922 * cos(FOURPI * ((i - 1.0)/(ac - 1.0))));
}
static double blackman74_w(int i, int ac)
{
	return (0.402217 - 0.49703 * cos(TWOPI * ((i - 1.0)/(ac - 1.0))) + 0.09892 * cos(FOURPI * ((i - 1.0)/(ac - 1.0))) - 0.00188 * cos(THREEPI * ((i - 1.0)/(ac - 1.0))));
}
static double blackman92_w(int i, int ac)
{
	return (0.35875 - 0.48829 * cos(TWOPI * ((i - 1.0)/(ac - 1.0))) + 0.14128 * cos(FOURPI * ((i - 1.0)/(ac - 1.0))) - 0.01168 * cos(THREEPI * ((i - 1.0)/(ac - 1.0))));
}

double (*window_functions[windowcount])(int, int) = {rect_w, hanning_w, hamming_w, blackman62_w, blackman70_w, blackman74_w, blackman92_w};

char *window_names[windowcount] = {"rectangular", "hanning", "hamming", "blackman62", "blackman70", "blackman74", "blackman92"};
t_symbol *window_names_s[windowcount];

char *fft_size_strings[12] = {"32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536" };
t_symbol *fft_size_names[12];

// Some structures from Fiddle~
typedef struct peakout {    // a peak for output
	double po_freq;		// frequency in Hz
	double po_amp;	    	// amplitude
} t_peakout;

typedef struct peak {	    // a peak for analysis
	double p_freq;		    // frequency in bins
	double p_width;		// peak width in bins
	double p_pow;		    // peak power
	double p_loudness;	    // 4th root of power
	double *p_fp;		    // pointer back to spectrum
} t_peak;

typedef struct histopeak {	// Histogram for peaks
	double h_pitch;		// estimated pitch
	double h_value;		// value of peak
	double h_loud;		    // combined strength of found partials
	long h_index;		    // index of bin holding peak
	long h_used;			// true if an x_hist entry points here
} t_histopeak;

typedef struct pitchhist {		// struct for keeping history by pitch
	double h_pitch;		    // pitch to output
	double h_amps[HISTORY];	// past amplitudes
	double h_pitches[HISTORY]; // past pitches
	double h_noted;		    // last pitch output
	long h_age;			    // number of frames pitch has been there
	t_histopeak *h_wherefrom;	// new histogram peak to incorporate
} t_pitchhist;

// The actual main external structure
typedef struct _analyzer {

	t_pxobject x_obj;

	t_critical lock;

	double x_Fs;		// Sample rate
	long vs; 			// vector size
	long x_overlap;		// Number of overlaping samples
	long x_hop;			// Number of non-overlaping samples
	int window;			// Type of window
    int fftsize_idx;    // index in fft size enum
	t_symbol *windname_dummy; // for the attribute---not actually used
	double *windows[7]; // store the pointers to the precomputed windows.  these can be looked up with the values defined in the enum
	long x_delay;			// Vector size delay before to start feeding the buffer
	long x_counter;		// Counter that goes with the vector size delay
	t_symbol *x_scale;			// Type of output scale
	long x_output;			// Type of output
	long x_bright;			// Type of brightness
	long x_loud;			// Type of loudness
	double x_loudness;		// Current loudness
	double x_brightness;	// Current brightness
	double x_noisiness;	// Current noisiness

	// Variables from Fiddle~
	long x_npitch;			// Number of pitches to output
	long x_npeakanal;		// Number of peaks to analyse
	long x_npeakout;		// Number of peaks to output
	long x_histphase;		// Phase into amplitude history vector
	t_pitchhist x_hist[MAXNPITCH]; // History of current pitches
	double x_dbs[HISTORY];	// DB history, indexed by "histphase"
	double x_peaked;		// peak since last attack
	long x_dbage;		    // number of bins DB has met threshold
	long x_attackvalue;
	t_peak x_peaklist[MAXNPEAK+1]; // This was originally a local buffer in pitch_getit
	t_histopeak x_histvec[MAXHIST];// This one too
		
	// Parameters from fiddle~
	double x_amplo;
	double x_amphi;
	long x_attacktime;
	long x_attackbins;
	double x_attackthresh;
	long x_vibtime;
	long x_vibbins;
	double x_vibdepth;
	double x_npartial;

	// Buffers
	t_atom *Buf1;			// buffer 1 : Use buffers of integers to copy faster
	t_atom *Buf2;			// buffer 2
	double *BufFFT_in;		// FFT buffer
	double *BufFFT_out;		// FFT buffer
	double *BufPower;		// Power spectrum buffer
//	double *WindFFT;		// Window of FFTSize
	//double *memFFT;		// fft.c memory space
	t_peakout *peakBuf;		// Spectral peaks for output
	double *histBuf;		// Histogram Buffer

	long BufSize;			// FFT buffer size
	long FFTSize;			// Size of FFT
	uint32_t x_FFTSizeOver2;	// Size of FFT/2 (uint32_t in G4 FFT)
	long BufWritePos;		// Where to write in buffer

	double *BufBark;		// Bark buffer
	long *BufSizeBark;		// Number of bins per band

	void **x_out;			// Outlet for all Bands
	t_atom *myList;			// Copy of outputs as Atoms
	void *x_outlet;			// List of outputs for Bark decomposition
	void *x_outloud;		// Outlet for the loudness
	void *x_outbright;		// Outlet for the brightness
	void *x_outnois;		// Outlet for the noisiness
	void *x_attackout;		// Outlet for Onset detection
	void *x_noteout;		// Outlet for cooked pitch
	void *x_peakout;		// Outlet for sinusoidal decomposition
	void *x_pitchout;		// Outlet for raw pitch & amplitude
	void *x_oscout;			// Outlet for OSC
		
	int x_debug;				// Debug mode?
	double *lastInputVector;	// Remembered from last analyzer_perform() for debug into
	long lastInputVectorSize;	// ditto

	fftw_plan fft_plan;

	t_osc_timetag timetag; // the time reported by gettimeofday---used to create timestamps for outgoing OSC packets
	uint64_t svctr; // the signal vector number---used to create timestamps for outgoing OSC packets
    
} t_analyzer;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman62, *ps_blackman70, *ps_blackman74, *ps_blackman92, *ps_list, *ps_nolist, *ps_FullPacket, *ps_linear, *ps_log;

void analyzer_perform64(t_analyzer *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
t_int *analyzer_perform(t_int *w);
void analyzer_dsp64(t_analyzer *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
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
void *analyzer_new(t_symbol *s, short argc, t_atom *argv);
void analyzer_free(t_analyzer *x);
void analyzer_tick(t_analyzer *x, t_symbol *msg, int argc, t_atom *argv);
double pitch_mtof(double f);
double pitch_ftom(double f);
long pitch_ilog2(long n);
void pitch_getit(t_analyzer *x); // modified fiddle pitch tracker function
void analyzer_debug(t_analyzer *x, long n);
void analyzer_compute_windows(t_analyzer *x);

// accessor setters
t_max_err analyzer_buffersize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_hopsize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_fftsize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_windowtype_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_initialdelay_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_numpitches_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_numpeakstofind_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_numpeakstooutput_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
t_max_err analyzer_barkformat_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv);
// accessor getters
t_max_err analyzer_windowtype_get(t_analyzer *x, t_object *attr, long *argc, t_atom **argv);
t_max_err analyzer_barkformat_get(t_analyzer *x, t_object *attr, long *argc, t_atom **argv);

void analyzer_perform64(t_analyzer *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	if(x->x_obj.z_disabled){
		return;
	}

	if(x->svctr == 0){
		x->timetag = osc_timetag_now();
	}

	long n = x->BufSize; // make a copy of this---it could change
	if(x->x_counter < 1){
		int index = 0;
		int overlapindex = n - x->x_overlap;
		while((x->BufWritePos < n) && (index < sampleframes)){
			atom_setfloat(x->Buf1 + x->BufWritePos, ins[0][index]);
			x->BufWritePos++;
			index++;
		}
		if(x->BufWritePos >= n){
			for(int i = 0; i < x->x_overlap; i++){
				x->Buf2[i] = x->Buf1[overlapindex + i];
			}
			int maxindex = sampleframes - index + x->x_overlap;
			for(int i = x->x_overlap; i < maxindex; i++){
				atom_setfloat(x->Buf2 + i, ins[0][index]);
				index++;
			}
			x->BufWritePos = maxindex;
			atom_setlong(x->Buf1 + n, x->svctr);
			schedule_delay(x, (method)analyzer_tick, 0, NULL, n + 1, x->Buf1);
			t_atom *tmp = x->Buf1;
			x->Buf1 = x->Buf2;
			x->Buf2 = tmp;
		}
	}else{
		x->x_counter--;
	}
	x->svctr++;
}

t_int *analyzer_perform(t_int *w)
{
	t_analyzer *x = (t_analyzer *)w[3];
	if(x->x_obj.z_disabled){
		return w + 4;
	}
	long n = w[2];
	t_float *vec = (t_float *)w[1];
	double cpy[n];
	for(int i = 0; i < n; i++){
		cpy[i] = (double)(vec[i]);
	}
	double *cpyptr = (double *)cpy;
	analyzer_perform64(x, NULL, &cpyptr, 1, NULL, 0, n, 0, NULL);
	return w + 4;
}

void analyzer_dsp64(t_analyzer *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	int vs = x->vs = sys_getblksize();
	x->x_Fs = samplerate; // store sampling rate

	// Initializing the delay counter
	x->x_counter = x->x_delay;
	x->timetag = OSC_TIMETAG_NULL;
	x->svctr = 0;

	// Overlap case
	if (x->x_overlap > x->BufSize - vs) {
		object_error((t_object *)x, "Overlap (%d) can't be larger than bufsize (%d) - sigvs (%d).\n", x->x_overlap, x->BufSize, vs);
		object_error((t_object *)x, "Will be left out of the dsp chain!\n");
		return;
	}
    else if (x->x_overlap < 1) // should never happen
		x->x_overlap = 0; 

	if(vs > x->BufSize){
		object_error((t_object *)x, "Sigvs (%d) can't be larger than the buffer size (%d)\n", vs, x->BufSize);
		object_error((t_object *)x, "Will be left out of the dsp chain!\n");
		return;
	}

    if(x->FFTSize < vs){
        object_error((t_object *)x, "FFT size (%d) cannot be less than ths signal vector size (%d).\n", x->FFTSize, vs);
        object_error((t_object *)x, "Will be left out of the dsp chain!\n");
        return;
    }
    
    //no longer necessary to do this:
	//x->x_hop = x->BufSize - x->x_overlap;
    
	x->x_FFTSizeOver2 = x->FFTSize/2;		

	if(count[0]){
		object_method(dsp64, gensym("dsp_add64"), x, analyzer_perform64, 0, NULL); 
	}
}

void analyzer_dsp(t_analyzer *x, t_signal **sp, short *count)
{
	int vs = x->vs = sys_getblksize();
	x->x_Fs = sp[0]->s_sr; // store sampling rate

	// Initializing the delay counter
	x->x_counter = x->x_delay;

	// Initialize the vars used to generate timetags
	x->timetag = OSC_TIMETAG_NULL;
	x->svctr = 0;

	// Overlap case
	if (x->x_overlap > x->BufSize - vs) {
		object_error((t_object *)x, "Overlap (%d) can't be larger than bufsize (%d) - sigvs (%d).\n", x->x_overlap, x->BufSize, vs);
		object_error((t_object *)x, "Will be left out of the dsp chain!\n");
		return;
	} else if (x->x_overlap < 1)
		x->x_overlap = 0; 

	if(vs > x->BufSize){
		object_error((t_object *)x, "Sigvs (%d) can't be larger than the buffer size (%d)\n", vs, x->BufSize);
		object_error((t_object *)x, "Will be left out of the dsp chain!\n");
		return;
	}

	x->x_hop = x->BufSize - x->x_overlap;
	x->x_FFTSizeOver2 = x->FFTSize/2;		

	if(count[0]){
		dsp_add(analyzer_perform, 3, sp[0]->s_vec, sp[0]->s_n, x);
	}
}

void analyzer_log(t_analyzer *x) {
	x->x_scale = ps_log;
}

void analyzer_linear(t_analyzer *x) {
	x->x_scale = ps_linear;
}

void analyzer_float(t_analyzer *x, double f) {
	t_atom a;
	atom_setfloat(&a, f);
	analyzer_hopsize_set(x, NULL, 1, &a);
}

void analyzer_int(t_analyzer *x, long n) {
	t_atom a;
	atom_setlong(&a, n);
	analyzer_hopsize_set(x, NULL, 1, &a);
}

//Assist strings

void analyzer_assist(t_analyzer *x, void *b, long m, long a, char *s) 
{
	if (m == ASSIST_INLET) {
		sprintf(s, "(signal) input, (float/int) hop size");
	}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(list) cooked MIDI pitch, pitch (Hz)");
			break;
		case 1:
			sprintf(s,"(float) loudness");
			break;
		case 2:
			sprintf(s,"(float) brightness");
			break;
		case 3:
			sprintf(s,"(float) noisiness");
			break;
		case 4:
			sprintf(s,"(list) bark");
			break;
		case 5:
			sprintf(s,"(bang) attack");
			break;
		case 6:
			sprintf(s,"(list) polyphonic pitches (freq, amp)");
			break;
		case 7:
			sprintf(s,"(list) sinusoidal components (freq, amp)");
			break;
		case 8:
			sprintf(s,"OSC");
			break;
		}
	}
}

void analyzer_print(t_analyzer *x) {
	object_post((t_object *)x, "amp-range %.2f %.2f",  x->x_amplo, x->x_amphi);
	object_post((t_object *)x, "reattack %d %.2f",  x->x_attacktime, x->x_attackthresh);
	object_post((t_object *)x, "vibrato %d %.2f",  x->x_vibtime, x->x_vibdepth);
	object_post((t_object *)x, "npartial %.2f",  x->x_npartial);
	object_post((t_object *)x, "loud %d",  x->x_loud);
	object_post((t_object *)x, "bright %d",  x->x_bright);
	object_post((t_object *)x, "%s", x->x_scale->s_name);
}

#define TELLi(var) post("  " #var ": %ld", x->var);
#define TELLf(var) post("  " #var ": %f", x->var);
#define TELLp(var) post("  " #var ": %p", x->var);

void analyzer_tellmeeverything(t_analyzer *x) {
	long time;
	
	object_post((t_object *)x, "");
	object_post((t_object *)x, "Analyzer~ state:");
	
	TELLi(BufSize);
    TELLi(FFTSize);
	TELLi(BufWritePos);
	TELLp(Buf1);
	TELLp(Buf2);
	TELLp(BufFFT_in);
	TELLp(BufFFT_out);
	TELLp(lastInputVector);
	TELLi(lastInputVectorSize);
	
	analyzer_print(x);
	
	time = gettime();
	object_post((t_object *)x, "gettime(): %ld", time);
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
	x->x_attackbins = (long) (x->x_Fs * 0.001f * attacktime) / (x->BufSize - x->x_overlap);
	if (x->x_attackbins >= HISTORY) x->x_attackbins = HISTORY - 1;
}

void analyzer_vibrato(t_analyzer *x, t_floatarg vibtime, t_floatarg vibdepth) {
	if (vibtime < 0) vibtime = 0;
	if (vibdepth <= 0) vibdepth = 1000;
	x->x_vibtime = vibtime;
	x->x_vibdepth = vibdepth;
	x->x_vibbins = (long) (x->x_Fs * 0.001f * vibtime) / (x->BufSize - x->x_overlap);
	if (x->x_vibbins >= HISTORY) x->x_vibbins = HISTORY - 1;
	if (x->x_vibbins < 1) x->x_vibbins = 1;
}

void analyzer_npartial(t_analyzer *x, t_floatarg npartial) {
	if (npartial < 0.1) npartial = 0.1;
	x->x_npartial = npartial;
}

void analyzer_tick(t_analyzer *x, t_symbol *msg, int argc, t_atom *argv)
{
	// argv has an extra atom at the end that has the signal vector 
	//count that we'll use to generate a timetag
	argc--; 

	debug("Entering analyzer_tick");
	long i, index=0, cpt;
	double bark = 0.0, loud = 0.0, bright = 0.0, sumSpectrum = 0.0, SFM = 0.0;
	critical_enter(x->lock);
	fftw_plan fft_plan = x->fft_plan;
	double fs = x->x_Fs;
	double FFTSize = x->FFTSize;
	int window_index = x->window;
	double window[argc];
	memcpy(window, x->windows[window_index], argc * sizeof(double));
	critical_exit(x->lock);
	double FsOverFFTSize = fs / FFTSize; // Keep it here since x_Fs may change
	double FsOverBarkSize = fs / (2.0 * NUMBAND); // Fix that problem in a next version
	double prod = 1.0, sum = 0.0;
	double invNumBand = 0.04;
	t_pitchhist *ph;

	// this function is only ever called by the scheduler (via schedule_delay() in the perform
	// routine), so we don't need to enter a critical section to mess with the data stored
	// in our struct
	memset(x->BufFFT_in, '\0', FFTSize * sizeof(double)); // takes care of zero padding
	if(window_index == Recta){
		for(int i = 0; i < argc; i++){
			x->BufFFT_in[i] = atom_getfloat(argv + i);
		}
	}else{
		for(int i = 0; i < argc; i++){
			//x->BufFFT_in[i] = atom_getfloat(argv + i) * x->windows[window][i];
			x->BufFFT_in[i] = atom_getfloat(argv + i) * window[i];
		}
	}
	fftw_execute(fft_plan);

	double *BufFFT = x->BufFFT_out;
	// Squared Absolute
	for (i=0; i<x->FFTSize; i+=2) {
		x->BufPower[i/2] = (BufFFT[i] * BufFFT[i]) + (BufFFT[i+1] * BufFFT[i+1]);
	}
	
	// Go into fiddle~ code
	pitch_getit(x);

	double barkList[NUMBAND];
					
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
			if (x->x_scale == ps_linear) {
				outlet_float(x->x_out[NUMBAND-1-i], bark);
			} else {
				outlet_float(x->x_out[NUMBAND-1-i], 10*log10(bark/DB_REF));
 			}
 		} else {
			if (x->x_scale == ps_linear) {
				atom_setfloat(x->myList+i, bark);
			} else {
				atom_setfloat(x->myList+i, 10*log10(bark/DB_REF));
 			}
 		}
		if(x->x_scale == ps_linear){
			barkList[i] = bark;
		}else{
			barkList[i] = 10 * log10(bark / DB_REF);
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
	
	if (x->x_scale == ps_log) {
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
			x->BufPower[i] = sqrt(x->BufPower[i]);

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
			atom_setlong(at, i+1);
			atom_setfloat(at+1, po->po_freq);
			atom_setfloat(at+2, po->po_amp);
			outlet_list(x->x_peakout, 0, 3, at);
		}
	}
      
	// Output fundamental frequencies + amplitudes
	if (x->x_npitch > 1) {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[3];
			atom_setlong(at, i+1);
			atom_setfloat(at+1, ph->h_pitches[x->x_histphase]);
			atom_setfloat(at+2, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 3, at);
   		}
   	} else {
		for (i=0,  ph=x->x_hist; i<x->x_npitch; i++, ph++) {
			t_atom at[2];
			atom_setfloat(at, ph->h_pitches[x->x_histphase]);
			atom_setfloat(at+1, ph->h_amps[x->x_histphase]);
			outlet_list(x->x_pitchout, 0, 2, at);
   		}
	}   	
       
	// Output cooked MIDI/Frequency pitch
	if (x->x_npitch > 1) {
		for (i=0, ph=x->x_hist; i<x->x_npitch; i++, ph++)
 			if (ph->h_pitch) {
				t_atom at[3];
				atom_setlong(at, i+1);
				atom_setfloat(at+1, ph->h_pitch);
				atom_setfloat(at+2, mtof(ph->h_pitch));
				outlet_list(x->x_noteout, 0, 3, at);
			}
	} else {
		ph = x->x_hist;
 		if (ph->h_pitch) {
			t_atom at[2];
			atom_setfloat(at, ph->h_pitch);
			atom_setfloat(at+1, mtof(ph->h_pitch));
			outlet_list(x->x_noteout, 0, 2, at);
		}
	}
			
	// Output attack bang 
	if (x->x_attackvalue) outlet_bang(x->x_attackout);

 	// Only for lists
	if (x->x_output == List) {
 		outlet_list(x->x_outlet, 0L, NUMBAND, x->myList);
 	}

#ifdef OSC
	{
		OSC_PROFILE_TIMER_START(foo);
		t_osc_bndl_u *bndl = osc_bundle_u_alloc();

		// bark
		t_osc_msg_u *bark = osc_message_u_alloc();
		osc_message_u_setAddress(bark, "/bark");
		for(int i = 0; i < NUMBAND; i++){
			osc_message_u_appendDouble(bark, barkList[i]);
		}
		osc_bundle_u_addMsg(bndl, bark);

		// loudness
		t_osc_msg_u *loudness = osc_message_u_alloc();
		osc_message_u_setAddress(loudness, "/loudness");
		osc_message_u_appendDouble(loudness, x->x_loudness);
		osc_bundle_u_addMsg(bndl, loudness);

		// brightness
		t_osc_msg_u *brightness = osc_message_u_alloc();
		osc_message_u_setAddress(brightness, "/brightness");
		osc_message_u_appendDouble(brightness, x->x_brightness);
		osc_bundle_u_addMsg(bndl, brightness);

		// noisiness
		t_osc_msg_u *noisiness = osc_message_u_alloc();
		osc_message_u_setAddress(noisiness, "/noisiness");
		osc_message_u_appendDouble(noisiness, 1.0 - x->x_noisiness);
		osc_bundle_u_addMsg(bndl, noisiness);

		// peaks
		t_osc_msg_u *peaks = osc_message_u_alloc();
		osc_message_u_setAddress(peaks, "/peaks");
		t_peakout *po;
		int i;
		for(i = 0, po = x->peakBuf; i < x->x_npeakout; i++, po++){
			osc_message_u_appendDouble(peaks, po->po_freq);
			osc_message_u_appendDouble(peaks, po->po_amp);
		}
		osc_bundle_u_addMsg(bndl, peaks);

		// fundamental freqs/amps
		if(x->x_npitch > 1)
        {
			for(i = 0, ph = x->x_hist; i < x->x_npitch; i++, ph++)
            {
				//if(ph->h_pitch)
                {
					t_osc_msg_u *hz = osc_message_u_alloc();
					t_osc_msg_u *amp = osc_message_u_alloc();
					char buf[64];
					sprintf(buf, "/pitch/raw/hz/%d", i);
					osc_message_u_setAddress(hz, buf);
					osc_message_u_appendDouble(hz, ph->h_pitches[x->x_histphase]);
					osc_bundle_u_addMsg(bndl, hz);

					sprintf(buf, "/pitch/raw/amp/%d", i);
					osc_message_u_setAddress(amp, buf);
					osc_message_u_appendDouble(amp, ph->h_amps[x->x_histphase]);
					osc_bundle_u_addMsg(bndl, amp);
				}
			}
		}else{
			ph = x->x_hist;
			//if(ph->h_pitch)
            {
				t_osc_msg_u *hz = osc_message_u_alloc();
				t_osc_msg_u *amp = osc_message_u_alloc();
				osc_message_u_setAddress(hz, "/pitch/raw/midi");
				osc_message_u_appendDouble(hz, ph->h_pitches[x->x_histphase]);
				osc_bundle_u_addMsg(bndl, hz);

				osc_message_u_setAddress(amp, "/pitch/raw/amp");
				osc_message_u_appendDouble(amp, ph->h_amps[x->x_histphase]);
				osc_bundle_u_addMsg(bndl, amp);
			}
		}

		// cooked pitch
		if(x->x_npitch > 1){
			for(i = 0, ph = x->x_hist; i < x->x_npitch; i++, ph++){
				if(ph->h_pitch){
					t_osc_msg_u *hz = osc_message_u_alloc();
					t_osc_msg_u *midi = osc_message_u_alloc();
					char buf[64];
					sprintf(buf, "/pitch/cooked/midi/%d", i);
					osc_message_u_setAddress(hz, buf);
					osc_message_u_appendDouble(hz, ph->h_pitch);
					osc_bundle_u_addMsg(bndl, hz);

					sprintf(buf, "/pitch/cooked/hz/%d", i);
					osc_message_u_setAddress(midi, buf);
					osc_message_u_appendDouble(midi, mtof(ph->h_pitch));
					osc_bundle_u_addMsg(bndl, midi);
				}
			}
		}else{
			ph = x->x_hist;
			if(ph->h_pitch)
            {
				t_osc_msg_u *hz = osc_message_u_alloc();
				t_osc_msg_u *midi = osc_message_u_alloc();
				osc_message_u_setAddress(hz, "/pitch/cooked/midi");
				osc_message_u_appendDouble(hz, ph->h_pitch);
				osc_bundle_u_addMsg(bndl, hz);

				osc_message_u_setAddress(midi, "/pitch/cooked/hz");
				osc_message_u_appendDouble(midi, mtof(ph->h_pitch));
				osc_bundle_u_addMsg(bndl, midi);
			}
		}

		// attack
		if(x->x_attackvalue){
			t_osc_msg_u *attack = osc_message_u_alloc();
			osc_message_u_setAddress(attack, "/attack");
			osc_message_u_appendTrue(attack);
			osc_bundle_u_addMsg(bndl, attack);
		}

		// timetag
		double svct = (double)atom_getlong(argv + argc); // this is correct---we decremented argc at the top
		t_osc_timetag time = osc_timetag_add(x->timetag, osc_timetag_floatToTimetag(svct * (x->vs / fs)));
		t_osc_msg_u *timetag = osc_message_u_alloc();
		osc_message_u_setAddress(timetag, "/time");
		// osc_message_u.c doesn't have an appendTimetag function yet...
		t_osc_atom_u *tta = osc_atom_u_alloc();
		osc_atom_u_setTimetag(tta, time);
		osc_message_u_appendAtom(timetag, tta);
		osc_bundle_u_addMsg(bndl, timetag);

		//long len = 0;
		//char *buf = NULL;
		//osc_bundle_u_serialize(bndl, &len, &buf);
		t_osc_bndl_s *bs = osc_bundle_u_serialize(bndl);

		OSC_PROFILE_TIMER_STOP(foo);
		OSC_PROFILE_TIMER_PRINTF(foo);
		if(bs){
			t_atom out[2];
			atom_setlong(out, osc_bundle_s_getLen(bs));
			atom_setlong(out + 1, (long)osc_bundle_s_getPtr(bs));
			outlet_anything(x->x_oscout, ps_FullPacket, 2, out);
			osc_bundle_s_deepFree(bs);
		}
		osc_bundle_u_free(bndl);

	}
#endif // OSC 
 	debug("leaving analyzer_tick");
}	

// Convert from MIDI to Hz and Hz to MIDI
double pitch_mtof(double f) {
	return (8.17579891564 * exp(.0577622650 * f));
}

double pitch_ftom(double f) {
	return (17.3123405046 * log(.12231220585 * f));
}

long pitch_ilog2(long n) {
	long ret = -1;
    
	while (n) {
		n >>= 1;
		ret++;
	}
	return ret;
}

// This is the actual Fiddle~ code
void pitch_getit(t_analyzer *x)
{
	long i, j, k;
	t_peak *pk1; // peaks found
	t_peakout *pk2; // peaks to output
	t_histopeak *hp1;
	double power_spec = 0.0, total_power = 0.0, total_loudness = 0.0, total_db = 0.0;
	double *fp1, *fp2;
	double *spec = x->BufFFT_out, *powSpec = x->BufPower, threshold, mult;
	long n = x->FFTSize/2;
	long npitch, newphase, oldphase, npeak = 0;
	long logn = pitch_ilog2(n);
	double maxbin = BINPEROCT * (logn-2);
	double hzperbin = x->x_Fs/x->FFTSize;
	double coeff = x->FFTSize/(double)x->BufSize;
	double *histogram = x->histBuf + BINGUARD;
	long npeaktot = (x->x_npeakout > x->x_npeakanal ? x->x_npeakout : x->x_npeakanal);
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
			    
	total_power = 4.0 * power_spec; // Compensate for fiddle~ power estimation (difference of 6 dB)

	if (total_power > POWERTHRES) {
		total_db = (FIDDLEDB_REF-DBFUDGE) + LOGTODB*log(total_power/n); // dB power estimation of fiddle~
		total_loudness = sqrt(sqrt(power_spec)); // Use the actual real estimation rather than fiddle~'s
		if (total_db < 0) total_db = 0.0;
	} else {
		total_db = total_loudness = 0.0;
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
		double height = fp2[0], h1 = fp2[-1], h2 = fp2[1]; // Bin power and adjacents
		double totalfreq, pfreq, f1, f2, m, var, stdev;
	
		// Get worried if you see this
		if (fp1+7 >= spec+x->FFTSize) {
			object_post((t_object *)x, "*** fp1 %p, fp1+7 %p, spec %p, spec+FFTsize %p", fp1, fp1+7, spec, spec+x->FFTSize);
		}
	
		if (height<h1 || height<h2 || h1*coeff<POWERTHRES*total_power || h2*coeff<POWERTHRES*total_power) continue; // Go to next

		// Use an informal phase vocoder to estimate the frequency
		pfreq = ((fp1[-4] - fp1[4]) * (2.0f * fp1[0] - fp1[4] - fp1[-4]) +
			 (fp1[-3] - fp1[5]) * (2.0f * fp1[1] - fp1[5] - fp1[-3])) / (2.0 * height);
		    
		// Do this for the two adjacent bins too
		f1 = ((fp1[-6] - fp1[2]) * (2.0f * fp1[-2] - fp1[2] - fp1[-6]) +
		      (fp1[-5] - fp1[3]) * (2.0f * fp1[-1] - fp1[3] - fp1[-5])) / (2.0 * h1) - 1;
		f2 = ((fp1[-2] - fp1[6]) * (2.0f * fp1[2] - fp1[6] - fp1[-2]) +
		      (fp1[-1] - fp1[7]) * (2.0f * fp1[3] - fp1[7] - fp1[-1])) / (2.0 * h2) + 1;

		// get sample mean and variance of the three
		m = 0.333333 * (pfreq + f1 + f2);
		var = 0.5 * ((pfreq-m)*(pfreq-m) + (f1-m)*(f1-m) + (f2-m)*(f2-m));

		totalfreq = i + m;
		
		// BAD HACK TO BE CHANGED IN NEXT VERSION !!!!
		if (coeff > 1) {
			switch ((long)coeff) {
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

		stdev = sqrt(var);
		if (totalfreq < 4) totalfreq = 4;
		
		// Store the peak info in the list of peaks
		pk1->p_width = stdev;
		pk1->p_pow = height;
		pk1->p_loudness = sqrt(sqrt(height));
		pk1->p_fp = fp1;
		pk1->p_freq = totalfreq;
	
		npeak++;
		pk1++;
	} // end for
		
	// prepare the raw peaks for output
	for (i=0, pk1=x->x_peaklist, pk2=x->peakBuf; i<npeak; i++, pk1++, pk2++) {
    	
		double loudness = pk1->p_loudness;
		if (i>=x->x_npeakout) break;
    	
		pk2->po_freq = hzperbin * pk1->p_freq;
		pk2->po_amp = (2./(double)n) * loudness * loudness * coeff;
	}
        
	// in case npeak < x->x_npeakout
	for (; i<x->x_npeakout; i++, pk2++) pk2->po_amp = pk2->po_freq = 0;

	// now, make a sort of "likelihood" spectrum. Proceeding in 48ths of an octave,  
	// from 2 to n/2 (in bins), the likelihood of each pitch range is contributed
	// to by every peak in peaklist that's an integer multiple of it in frequency

	if (npeak > x->x_npeakanal) npeak = x->x_npeakanal; // max # peaks to analyze
        
	// Initialize histogram buffer to 0
	for (i=0, fp1=histogram; i<maxbin; i++) *fp1++ = 0.0;

	for (i=0, pk1=x->x_peaklist; i<npeak; i++, pk1++) {
    
		double pit = BPEROOVERLOG2 * log(pk1->p_freq) - 96.0;
		double binbandwidth = FACTORTOBINS * pk1->p_width/pk1->p_freq;
		double putbandwidth = (binbandwidth < 2 ? 2 : binbandwidth);
		double weightbandwidth = (binbandwidth < 1.0 ? 1.0 : binbandwidth);
		double weightamp = 4.0 * pk1->p_loudness / total_loudness;

		for (j=0, fp2=pitch_partialonset; j<NPARTIALONSET; j++, fp2++) {
			double bin = pit - *fp2;
			if (bin<maxbin) {
				double para, pphase, score = BINAMPCOEFF * weightamp / ((j+x->x_npartial) * weightbandwidth);
				long firstbin = bin + 0.5 - 0.5 * putbandwidth;
				long lastbin = bin + 0.5 + 0.5 * putbandwidth;
				long ibw = lastbin - firstbin;
				if (firstbin < -BINGUARD) break;
				para = 1.0 / (putbandwidth * putbandwidth);
				for (k=0, fp1=histogram+firstbin, pphase=firstbin-bin; k<=ibw; k++, fp1++, pphase+=1.0)
					*fp1 += score * (1.0 - para * pphase * pphase);
			}
		} // end for
	} // end for
      
	//post("npeaks = %d, %f",npeak,mult); // For debugging weird hack!!!
   
    
	// Next we find up to NPITCH strongest peaks in the histogram.
	// If a peak is related to a stronger one via an interval in
	// the pitch_partialonset array, we suppress it.

	for (npitch=0; npitch<x->x_npitch; npitch++) {
		long index;
		double best;
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
		double cumpow=0, cumstrength=0, freqnum=0, freqden=0;
		long npartials=0,  nbelow8=0;
		// guessed-at frequency in bins
		double putfreq = exp((1.0 / BPEROOVERLOG2) * (x->x_histvec[i].h_index + 96.0));
	
		for (j=0; j<npeak; j++) {
			double fpnum = x->x_peaklist[j].p_freq/putfreq;
			long pnum = fpnum + 0.5;
			double fipnum = pnum;
			double deviation;
	    
			if ((pnum>16) || (pnum<1)) continue;
	    
			deviation = 1.0 - fpnum/fipnum;
	   		if ((deviation > -PARTIALDEVIANCE) && (deviation < PARTIALDEVIANCE)) {
				// we figure this is a partial since it's within 1/4 of
				// a halftone of a multiple of the putative frequency.
				double stdev, weight;
				npartials++;
				if (pnum<8) nbelow8++;
				cumpow += x->x_peaklist[j].p_pow;
				cumstrength += sqrt(sqrt(x->x_peaklist[j].p_pow));
				stdev = (x->x_peaklist[j].p_width > MINBW ? x->x_peaklist[j].p_width : MINBW);
				weight = 1.0 / ((stdev*fipnum) * (stdev*fipnum));
				freqden += weight;
				freqnum += weight * x->x_peaklist[j].p_freq/fipnum;		
			} // end if
		} // end for
	
		if (((nbelow8<4) || (npartials<DEFNPARTIAL)) && (cumpow < (0.01 * total_power))) {
			x->x_histvec[i].h_value = 0;
		} else {
	  	  	double pitchpow = (cumstrength * cumstrength * cumstrength * cumstrength);
			double freqinbins = freqnum/freqden;
		
			// check for minimum output frequency
			if (freqinbins < MINFREQINBINS) {
				x->x_histvec[i].h_value = 0;
			} else {
				// we passed all tests... save the values we got
				x->x_histvec[i].h_pitch = ftom(hzperbin * freqnum/freqden);
				x->x_histvec[i].h_loud = (FIDDLEDB_REF-DBFUDGE) + LOGTODB*log(pitchpow*coeff/n);
			}	
		} // end else
	} // end for
    
	// Now try to find continuous pitch tracks that match the new pitches. 
	// First mark each peak unmatched.

	for (i=0, hp1=x->x_histvec; i<npitch; i++, hp1++)
		hp1->h_used = 0;

	// For each old pitch, try to match a new one to it.
	for (i=0, phist=x->x_hist; i<x->x_npitch; i++, phist++) {
		double thispitch = phist->h_pitches[oldphase];
		phist->h_pitch = 0;	    // no output, thanks...
		phist->h_wherefrom = 0;
		if (thispitch == 0.0) continue;
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
			long binlook = newphase - x->x_attackbins;
			if (binlook < 0) binlook += HISTORY;
			if (total_db > x->x_dbs[binlook] + x->x_attackthresh) {
				x->x_attackvalue = 1;
				x->x_peaked = 0;
			}
		}
	} else {
		long binlook = newphase - x->x_attackbins;
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
				double centroid = 0;
				long not = 0;
				for (j=0, k=newphase; j<x->x_vibbins; j++) {
					centroid += phist->h_pitches[k];
					k--;
					if (k<0) k = HISTORY-1;
				}
				centroid /= x->x_vibbins;
				for (j=0, k=newphase; j<x->x_vibbins; j++) {
					// calculate deviation from norm
					double dev = centroid - phist->h_pitches[k];
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
	object_post((t_object *)x, "Analyzer~: debug is %ld", n);
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


// accessor setters

void analyzer_hopsize_do_set(t_analyzer *x, long n)
{
    long vs = sys_getblksize();
    if(n < vs){
        object_error((t_object *)x, "Hop size (%d) cannot be less than ths signal vector size (%d)\n", n, vs);
        n = vs;
    }
    
    if(n > MAXBUFSIZE){
        object_error((t_object *)x, "Maximum hop size is %d samples.  Setting hop size to %d.", MAXBUFSIZE, MAXBUFSIZE);
        n = MAXBUFSIZE;
    }
    
    critical_enter(x->lock);
    
    if( n > x->BufSize )
    {
        object_error((t_object *)x, "Hop size (%d) cannot be larger than buffer size.  Setting hop size to %d.", n, x->BufSize);
        n = x->BufSize;
    }
    
    x->x_hop = n;
    x->x_overlap = x->BufSize - x->x_hop;
    critical_exit(x->lock);
}

t_max_err analyzer_hopsize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	if(!argc){
		return MAX_ERR_NONE;
	}
	double ms2samp = x->x_Fs * 0.001;
    long half_buf = x->BufSize / 2;

    long n;
	switch(atom_gettype(argv))
    {
        case A_LONG:
            n = atom_getlong(argv); // samples
            break;
        case A_FLOAT:
            n = (long)(atom_getfloat(argv) * ms2samp); // << make sure this is noted in the help patch
            break;
        default:
            n = half_buf;
	}
    
    analyzer_hopsize_do_set(x,n);
    
	return MAX_ERR_NONE;
}


void analyzer_compute_windows(t_analyzer *x)
{
    // critical_enter(x->lock);
    for(int j = 0; j < x->BufSize; j++){
        for(int i = 0; i < windowcount; i++){
            x->windows[i][j] = window_functions[i](j, x->BufSize);
        }
    }
    // critical_exit(x->lock);
}

void analyzer_buffersize_do_set( t_analyzer *x, long n )
{
    long vs = sys_getblksize();
    if(n < vs){
        object_error((t_object *)x, "Buffer size (%d) is smaller than the vector size (%d).  Setting the buffer size to the signal vector size", n, vs);
        n = vs;
    }
    
    if(n > MAXBUFSIZE){
        object_error((t_object *)x, "Maximum buffer size is %d samples.  Setting buffer size to %d.", MAXBUFSIZE, MAXBUFSIZE);
        n = MAXBUFSIZE;
    }
    
    critical_enter(x->lock);
    
    if(n > x->FFTSize){
        object_error((t_object *)x, "Buffer size (%d) cannot be greater than the FFT size. Setting buffer size to %d.", n, x->FFTSize);
        n = x->FFTSize;
    }
    
    x->BufSize = n;
    
    analyzer_compute_windows(x);

    if( x->x_hop > n )
    {
        t_atom at;
        atom_setlong(&at, x->x_hop);
        object_method_typed(x, gensym("hopsize"), 1, &at, NULL);
    }
    else
        x->x_overlap = x->BufSize - x->x_hop;

    critical_exit(x->lock);

}

t_max_err analyzer_buffersize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
    
    if(!argc){
        return MAX_ERR_NONE;
    }
    double ms2samp = x->x_Fs * 0.001;
    long n;
    switch(atom_gettype(argv)){
        case A_LONG:
            n = atom_getlong(argv); // samples
            break;
        case A_FLOAT:
            n = (long)(atom_getfloat(argv) * ms2samp);
            break;
        default:
            n = DEFBUFSIZE;
    }
    
    analyzer_buffersize_do_set(x, n);
    
    return MAX_ERR_NONE;
}


void analyzer_setBarkBins( t_analyzer *x )
{
    long band=0, oldband=0, sizeband=0;
    double freq = 0.0, oldfreq = 0.0;
    
    int j = 1;
    x->BufBark[0] = 0.0;

    // Compute and store Analyzer scale
    
    // what happens if the fft size / 2 is < 25 bark bands?
    // for now setting minimum fftsize to be 128
    for (int i=0; i<x->FFTSize/2; i++)
    {
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
}

void analyzer_fftsize_do_set(t_analyzer *x, long n)
{
    if (n < 128)  n = 128;
    else if ((n > 128) && (n < 256)) n = 256;
    else if ((n > 256) && (n < 512)) n = 512;
    else if ((n > 512) && (n < 1024)) n = 1024;
    else if ((n > 1024) && (n < 2048)) n = 2048;
    else if ((n > 2048) && (n < 4096)) n = 4096;
    else if ((n > 4096) && (n < 8192)) n = 8192;
    else if ((n > 8192) && (n < 16384)) n = 16384;
    else if ((n > 16384) && (n < 32768)) n = 32768;
    else if ((n > 32768) && (n < MAXBUFSIZE)) n = MAXBUFSIZE;
    else if (n > MAXBUFSIZE) {
        object_error((t_object *)x, "Maximum FFT size is %d samples.  Setting FFT size to %d.\n", MAXBUFSIZE, MAXBUFSIZE);
        n = MAXBUFSIZE;
    }
    
    long vs = sys_getblksize();
    if(n < vs){
        object_error((t_object *)x, "FFT size (%d) is less than ths signal vector size (%d)\n", n, vs);
        n = vs;
    }
    
    fftw_plan p = x->fft_plan;
    if( n != x->FFTSize || !p )
    {
        critical_enter(x->lock);
        
        p = fftw_plan_dft_r2c_1d(n, x->BufFFT_in, (fftw_complex*)x->BufFFT_out, FFTW_MEASURE);
        x->FFTSize = n;
        x->fft_plan = p;
        
        analyzer_setBarkBins(x);
        
        if (x->BufSize > n)
        {
            t_atom at;
            atom_setlong(&at, x->BufSize);
            object_method_typed(x, gensym("buffersize"), 1, &at, NULL);
        }
        
        critical_exit(x->lock);
    }
    
        
}

t_max_err analyzer_fftsize_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	if(!argc){
		return MAX_ERR_NONE;
	}

    double ms2samp = x->x_Fs * 0.001;
	long n;
	switch(atom_gettype(argv)){
	case A_LONG:
		n = atom_getlong(argv); // samples
		break;
	case A_FLOAT:
		n = (long)(atom_getfloat(argv) * ms2samp); //<< make sure this is in the help file
		break;
	default:
		n = x->BufSize;
	}
    
    analyzer_fftsize_do_set(x, n);
    
	return MAX_ERR_NONE;
}

t_max_err analyzer_windowtype_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	if(!argc){
		return MAX_ERR_NONE;
	}
	t_symbol *s = atom_getsym(argv);
	for(int i = 0; i < windowcount; i++){
		if(s == window_names_s[i]){
			x->window = i;
			break;
		}
	}
	return MAX_ERR_NONE;
}

t_max_err analyzer_handle_attr_type2(t_analyzer *x,
				     long argc,
				     t_atom *argv, 
				     long *field,
				     char *paramname,
				     int maxval,
				     int defval)
{
	if(!argc){
		return MAX_ERR_NONE;
	}
	long val = 0;
	switch(atom_gettype(argv)){
	case A_LONG:
		val = atom_getlong(argv);
		break;
	case A_FLOAT:
		val = (long)atom_getfloat(argv);
		break;
	default:
		object_error((t_object *)x, "'%s' must be either an int or a float (which will be truncated to an int).\n", paramname);
	}
	if(val >= 0 && val < maxval){
		*field = val;
	}else{
		object_error((t_object *)x, "'%s' (%d) must be between [0, %d).  Setting to %d\n", paramname, val, maxval, defval);
	}
	return MAX_ERR_NONE;
}

t_max_err analyzer_initialdelay_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	return analyzer_handle_attr_type2(x, argc, argv, &(x->x_delay), "initial delay", MAXDELAY, DEFDELAY);
}

t_max_err analyzer_numpitches_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	return analyzer_handle_attr_type2(x, argc, argv, &(x->x_npitch), "# of pitches", MAXNPITCH, DEFNPITCH);
}

t_max_err analyzer_numpeakstofind_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	t_max_err e = analyzer_handle_attr_type2(x, argc, argv, &(x->x_npeakanal), "# of pitches to analyze", MAXNPEAK, DEFNPEAKANAL);
	return e;
}

t_max_err analyzer_numpeakstooutput_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	t_max_err e = analyzer_handle_attr_type2(x, argc, argv, &(x->x_npeakout), "# of pitches to output", MAXNPEAK, DEFNPEAKOUT);
	if(x->x_npeakout > x->x_npeakanal){
		object_error((t_object *)x, "# of peaks to output (%d) must not be larger than the # of peaks to analyze (%d).  Setting the former to the latter.", x->x_npeakout, x->x_npeakanal);
		x->x_npeakout = x->x_npeakanal;
	}
    return e;
}

t_max_err analyzer_barkformat_set(t_analyzer *x, t_object *attr, long argc, t_atom *argv)
{
	if(!argc){
		return MAX_ERR_NONE;
	}
    
    if( x->x_output != -1 )
    {
        object_error((t_object *)x, "bark format cannont be changed dynamically");
        return MAX_ERR_GENERIC;
    }
    
	if(atom_gettype(argv) == A_SYM){
		t_symbol *sym = atom_getsym(argv);
		if(sym == ps_list){
			x->x_output = List;
		}else if(sym == ps_nolist){
			x->x_output = noList;
		}else{
			object_error((t_object *)x, "bark format must be a symbol, either 'list' or 'nolist'");
		}
	}else{
		object_error((t_object *)x, "bark format must be a symbol, either 'list' or 'nolist'");
	}
	return MAX_ERR_NONE;
}

// accessor getters
t_max_err analyzer_windowtype_get(t_analyzer *x, t_object *attr, long *argc, t_atom **argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setsym(*argv, window_names_s[x->window]);
	return MAX_ERR_NONE;
}

t_max_err analyzer_fftsize_get(t_analyzer *x, t_object *attr, long *argc, t_atom **argv)
{
    char alloc;
    atom_alloc(argc, argv, &alloc);
    atom_setsym(*argv, fft_size_names[x->fftsize_idx]);
    return MAX_ERR_NONE;
}

t_max_err analyzer_barkformat_get(t_analyzer *x, t_object *attr, long *argc, t_atom **argv)
{
	char alloc;
	atom_alloc(argc, argv, &alloc);
	if(x->x_output == List){
		atom_setsym(*argv, ps_list);
	}else if(x->x_output == noList){
		atom_setsym(*argv, ps_nolist);
	}else{
		object_error((t_object *)x, "bark format is set to an unknown state!");
	}
	return MAX_ERR_NONE;
}

void analyzer_free(t_analyzer *x)
{
    
    dsp_free((t_pxobject *)x);
    //This is an easier way, since object_free checks before freeing.  -mzed
    
    //if (x->memFFT != NULL) sysmem_freeptr((char *) x->memFFT);
    // switch back to sysmem and test
    if (x->Buf1 != NULL) sysmem_freeptr((char *)x->Buf1);
    if (x->Buf2 != NULL) sysmem_freeptr((char *)x->Buf2);
    if (x->BufFFT_in != NULL) fftw_free((char *)x->BufFFT_in);
    if (x->BufFFT_out != NULL) fftw_free((char *) x->BufFFT_out);
    if (x->BufPower != NULL) sysmem_freeptr((char *)x->BufPower);
    if (x->peakBuf != NULL) sysmem_freeptr((char *)x->peakBuf);
    if (x->histBuf != NULL) sysmem_freeptr((char *)x->histBuf);
    if (x->BufBark != NULL) sysmem_freeptr((char *)x->BufBark);
    if (x->BufSizeBark != NULL) sysmem_freeptr((char *)x->BufSizeBark);
    if (x->x_out != NULL) sysmem_freeptr((char *) x->x_out);
    if (x->myList != NULL) sysmem_freeptr((char *) x->myList);
    
    if(x->windows){
        for(int i = 0; i < windowcount; i++){
            if(x->windows[i]){
                sysmem_freeptr(x->windows[i]);
            }
        }
        //sysmem_freeptr(x->windows);
    }
    
    fftw_destroy_plan(x->fft_plan);
    
    critical_free(x->lock);
}


void *analyzer_new(t_symbol *s, short argc, t_atom *argv) {
    
    t_analyzer *x = (t_analyzer *)object_alloc(analyzer_class);
    if(!x){
        return NULL;
    }
    
    critical_new(&x->lock);
    
    dsp_setup((t_pxobject *)x,1); // one inlet
    x->x_Fs = sys_getsr();
    x->BufWritePos = 0;
    x->x_scale = ps_log;
    x->x_loud = 0;
    x->x_bright = 0;
    x->x_loudness = 0.0;
    x->x_brightness = 0.0;
    x->x_noisiness = 0.0;
    
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
    
    x->x_output = -1;
    
    x->fftsize_idx = 5;
    
    // More initializations from Fiddle~
    for (int i=0; i<MAXNPITCH; i++) {
        x->x_hist[i].h_pitch = x->x_hist[i].h_noted = 0.0;
        x->x_hist[i].h_age = 0;
        x->x_hist[i].h_wherefrom = NULL;
        
        for (int j=0; j<HISTORY; j++)
            x->x_hist[i].h_amps[j] = x->x_hist[i].h_pitches[j] = 0.0;
    }
    
    for (int i=0; i<HISTORY; i++){
        x->x_dbs[i] = 0.0;
    }
    
    
    // get memory before processing attr settings
    for(int i = 0; i < windowcount; i++){
        x->windows[i] = (double *)sysmem_newptr(MAXBUFSIZE * sizeof(double));
        memset(x->windows[i], '\0', MAXBUFSIZE * sizeof(double));
    }
    // the actual windows will be computed when the buffersize setter is called
    
    // + 1 to stick the signal vector count to generate the timetag
    x->Buf1 = (t_atom*)sysmem_newptr( (MAXBUFSIZE + 1) * sizeof(t_atom));
    x->Buf2 = (t_atom*)sysmem_newptr( (MAXBUFSIZE + 1) * sizeof(t_atom));
    
    x->BufFFT_in = (double *)fftw_malloc(sizeof(double) * MAXBUFSIZE);
    x->BufFFT_out = (double *)fftw_malloc(sizeof(double) * MAXBUFSIZE * 2);
    memset(x->BufFFT_in, '\0', MAXBUFSIZE * sizeof(double));
    memset(x->BufFFT_out, '\0', MAXBUFSIZE * 2 * sizeof(double));
    
    x->BufPower = (double*) sysmem_newptr((MAXBUFSIZE / 2) * sizeof(double));
    
    x->peakBuf = (t_peakout*) sysmem_newptr(MAXNPEAK * sizeof(t_peakout)); // from Fiddle~
    //x->histBuf = (double*) sysmem_newptr((x->FFTSize + BINGUARD) * sizeof(double)); // for Fiddle~
    x->histBuf = (double*) sysmem_newptr((MAXBUFSIZE + BINGUARD) * sizeof(double)); // for Fiddle~
    x->BufBark = (double*) sysmem_newptr(2*NUMBAND * sizeof(double));
    x->BufSizeBark = (long*) sysmem_newptr(NUMBAND * sizeof(long));
    
    
    t_dictionary *attrs = dictionary_new();
    
    t_symbol *attrnames[] = {gensym("buffersize"), gensym("hopsize"), gensym("fftsize"), gensym("windowtype"), gensym("initialdelay"), gensym("numpitches"), gensym("numpeakstofind"), gensym("numpeakstooutput"), gensym("barkformat")};
    
    
    // get number of arguments (non-attributes)
    int ac = attr_args_offset(argc, argv);
    
    // set user defined attrs into dictionary
    attr_args_dictionary(attrs, argc, argv);
    
    // check for duplicates between args and attribute settings
    
    if( dictionary_hasentry(attrs, attrnames[0]) && ac > 0 )
    {
        object_error((t_object*)x, "buffersize was set twice, please use either attribute or argument, but not both.");
        return NULL;
    }
    else if( dictionary_hasentry(attrs, attrnames[1]) && ac > 1 )
    {
        object_error((t_object*)x, "hopsize was set twice, please use either attribute or argument, but not both.");
        return NULL;
    }
    else if( dictionary_hasentry(attrs, attrnames[2]) && ac > 2 )
    {
        object_error((t_object*)x, "fftsize was set twice, please use either attribute or argument, but not both.");
        return NULL;
    }
    
    
    // put legacy arguments into attr dict (overriding existing attributes, maybe should add more warnings above)
    for( int i = 0; i < ac; i++ )
        dictionary_appendatom(attrs, attrnames[i], argv + i);
    
    
    // get the attributes in order to see if buffersize and hopsize are both specified.
    // if buffersize is and hopsize isn't, set hopsize to buffersize / 2
    
    t_atom_long val;
    
    bool have_bufsize = dictionary_hasentry(attrs, attrnames[0]);
    bool have_hopsize = dictionary_hasentry(attrs, attrnames[1]);
    bool have_fftsize = dictionary_hasentry(attrs, attrnames[2]);
    
    
    long vs = sys_getblksize();
    
    if( have_bufsize )
    {
        dictionary_getlong(attrs, attrnames[0], &val);
        x->BufSize = val;
    }
    
    if( have_fftsize )
    {
        dictionary_getlong(attrs, attrnames[2], &val);
        
        if( val < 128 )
        {
            object_error((t_object *)x, "FFT size (%d) cannot be less than 128!", val );
            return NULL;

        }
        
        x->FFTSize = val;
        
        if( !have_bufsize )
        {
            x->BufSize = x->FFTSize;
            dictionary_appendlong( attrs, attrnames[0], x->FFTSize );
            have_bufsize = true;
        }
    }
    else
    {
        if( have_bufsize )
        {
            x->FFTSize = x->BufSize;
        }
        else
            x->FFTSize = DEFBUFSIZE;
        
        dictionary_appendlong( attrs, attrnames[2], x->FFTSize );
    }
    
    
    if( !have_bufsize )
    {
        x->BufSize = DEFBUFSIZE;
        dictionary_appendlong( attrs, attrnames[0], x->BufSize );
    }
    
    
    if( have_hopsize )
    {
        dictionary_getlong(attrs, attrnames[1], &val);
        
        if(val < vs){
            object_error((t_object *)x, "FFT size (%d) cannot be less than ths signal vector size (%d). Setting to vector size\n", val, vs);
            val = vs;
        }
        x->x_hop = val;
    }
    else
    {
        x->x_hop = x->BufSize / 2;

        if( x->x_hop < vs )
            x->x_hop = vs;
        
        dictionary_appendlong( attrs, attrnames[1], x->x_hop );
    }
    
    
    if( !dictionary_hasentry(attrs, attrnames[3]) )
        dictionary_appendsym(attrs, attrnames[3], window_names_s[DEFWIN]);
    
    if( !dictionary_hasentry(attrs, attrnames[4]) )
        dictionary_appendlong(attrs, attrnames[4], DEFDELAY);
    
    if( !dictionary_hasentry(attrs, attrnames[5]) )
        dictionary_appendlong(attrs, attrnames[5], DEFNPITCH);
    
    if( !dictionary_hasentry(attrs, attrnames[6]) )
        dictionary_appendlong(attrs, attrnames[6], DEFNPEAKANAL);
    
    dictionary_getlong(attrs, attrnames[6], &val);
    x->x_npeakanal = val;
    
    if( !dictionary_hasentry(attrs, attrnames[7]) )
        dictionary_appendlong(attrs, attrnames[7], DEFNPEAKOUT);
    
    dictionary_getlong(attrs, attrnames[7], &val);
    x->x_npeakout = val;
    
    if( !dictionary_hasentry(attrs, attrnames[8]) )
        dictionary_appendsym(attrs, attrnames[8], ps_list);
    
    
    if (x->x_npeakout > x->x_npeakanal) {
        object_error((t_object *)x, "Analyzer~: '# of peaks to output' (%d) must not be larger than '# of peaks to analyze' (%d).  Setting the former to the latter.\n", x->x_npeakout, x->x_npeakanal);
        x->x_npeakout = x->x_npeakanal;
    }
    
    
    /*
     object_post((t_object *)x, "--- Analyzer~ ---");
     object_post((t_object *)x, "	Buffer size = %d",x->BufSize);
     object_post((t_object *)x, "	Hop size = %d",x->x_hop);
     object_post((t_object *)x, "	FFT size = %d",x->FFTSize);
     object_post((t_object *)x, "	Window type = %s",window_names[x->window]);
     object_post((t_object *)x, "	Initial delay = %d",x->x_delay);
     object_post((t_object *)x, "	Number of pitches = %d",x->x_npitch);
     object_post((t_object *)x, "	Number of peaks to search = %d",x->x_npeakanal);
     object_post((t_object *)x, "	Number of peaks to output = %d",x->x_npeakout);
     */
    
    // first set fftsize, buffersize and hopsize in that order.
    analyzer_fftsize_do_set(x, x->FFTSize);
    analyzer_buffersize_do_set(x, x->BufSize);
    analyzer_hopsize_do_set(x, x->x_hop);
    
    // then process the rest of the attrs
    // (this will reprocess the fft, buffer and hop size, but it shouldn't change anything)
    attr_dictionary_process(x, attrs);
    object_free(attrs);
    
    // Make an outlet for OSC out
    x->x_oscout = outlet_new((t_object *)x, "FullPacket");
    
    // Make an outlet for peaks out
    x->x_peakout = listout((t_object *)x); // one list out
    
    // One outlet for fundamental & amplitude raw values
    x->x_pitchout = listout((t_object *)x);
    
    // Make bang outlet for onset detection
    x->x_attackout = bangout((t_object *)x);
    
    
    // Create the Bark outlet(s)
    if (x->x_output == noList) {
        // Allocate memory for all outlets
        x->x_out = (void**) sysmem_newptr(NUMBAND * sizeof(double*));
        for (int i=0; i<NUMBAND; i++) {
            x->x_out[i] = floatout((t_analyzer *)x); // Create float outlets
        }
    } else {
        x->myList   = (t_atom*) sysmem_newptr(NUMBAND * sizeof(*x->myList));
        x->x_outlet = listout((t_analyzer *)x);	// Create a list outlet
    }
    
    // Create the Loudness/Brightness outlet
    x->x_outnois = floatout((t_analyzer *)x); // one outlet for noisiness
    x->x_outbright = floatout((t_analyzer *)x); // one outlet for brightness
    x->x_outloud = floatout((t_analyzer *)x); // one outlet for loudness
    x->x_noteout = listout((t_analyzer *)x); // one outlet for MIDI & frequency cooked pitch
    
    
    // More initializations from Fiddle~
    for (int i=0; i<x->x_npeakout; i++)
        x->peakBuf[i].po_freq = x->peakBuf[i].po_amp = 0.0;
    
    x->x_debug = 0;
    
    return x;
}


int main(void)
{
    /*
     post("Analyzer~ object version " VERSION " by Tristan Jehan, Adrian Freed, Matt Wright, and Michael Zbyszynski");
     post("copyright (c) 2001 Massachusetts Institute of Technology, 2007-8 UC Regents");
     post("Pitch tracker based on Miller Puckette's fiddle~");
     post("copyright (c) 1997-1999 Music Department UCSD");
     post(" ");
     */
    
    version(NULL);
    for(int i = 0; i < windowcount; i++){
        window_names_s[i] = gensym(window_names[i]);
    }
    
    for( int i = 0; i < 12; i++)
    {
        fft_size_names[i] = gensym(fft_size_strings[i]);
    }
    
    ps_list = gensym("list");
    ps_nolist = gensym("nolist");
    ps_linear = gensym("linear");
    ps_log = gensym("log");
    ps_FullPacket = gensym("FullPacket");
    
    analyzer_class = class_new("analyzer~", (method)analyzer_new, (method)analyzer_free, (short)sizeof(t_analyzer), 0L, A_GIMME, 0);
    
    class_addmethod(analyzer_class, (method)analyzer_dsp, "dsp", A_CANT, 0);
    class_addmethod(analyzer_class, (method)analyzer_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(analyzer_class, (method)analyzer_assist, "assist", A_CANT, 0);
    class_addmethod(analyzer_class, (method)analyzer_print, "print", 0);
    class_addmethod(analyzer_class, (method)analyzer_tellmeeverything, "tellmeeverything", 0);
    class_addmethod(analyzer_class, (method)analyzer_amprange, "amp-range", A_FLOAT, A_FLOAT, 0);
    class_addmethod(analyzer_class, (method)analyzer_reattack, "reattack", A_FLOAT, A_FLOAT, 0);
    class_addmethod(analyzer_class, (method)analyzer_vibrato, "vibrato", A_FLOAT, A_FLOAT, 0);
   	class_addmethod(analyzer_class, (method)analyzer_npartial, "npartial", A_FLOAT, 0);
   	class_addmethod(analyzer_class, (method)analyzer_debug, "debug", A_LONG, 0);
    
    class_addmethod(analyzer_class, (method)analyzer_float, "float", A_FLOAT, 0);
    class_addmethod(analyzer_class, (method)analyzer_int, "int", A_LONG, 0);
    
    // These now have aliases, but the messages remain for backwards compatibility
    class_addmethod(analyzer_class, (method)analyzer_log, "log", 0);
    class_addmethod(analyzer_class, (method)analyzer_linear, "linear", 0);
    
    // attributes
    
    CLASS_ATTR_LONG(analyzer_class, "buffersize", 0, t_analyzer, BufSize);
    CLASS_ATTR_ACCESSORS(analyzer_class, "buffersize", NULL, analyzer_buffersize_set);
    CLASS_ATTR_LABEL(analyzer_class, "buffersize", 0, "Buffer Size");
    
    CLASS_ATTR_LONG(analyzer_class, "hopsize", 0, t_analyzer, x_hop);
    CLASS_ATTR_ACCESSORS(analyzer_class, "hopsize", NULL, analyzer_hopsize_set);
    CLASS_ATTR_LABEL(analyzer_class, "hopsize", 0, "Hop Size");
    
    CLASS_ATTR_LONG(analyzer_class, "fftsize", 0, t_analyzer, FFTSize);
    CLASS_ATTR_ACCESSORS(analyzer_class, "fftsize", NULL, analyzer_fftsize_set);
    CLASS_ATTR_LABEL(analyzer_class, "fftsize", 0, "FFT Size");
    
    
    CLASS_ATTR_SYM(analyzer_class, "windowtype", 0, t_analyzer, windname_dummy);
    CLASS_ATTR_ACCESSORS(analyzer_class, "windowtype", analyzer_windowtype_get, analyzer_windowtype_set);
    CLASS_ATTR_LABEL(analyzer_class, "windowtype", 0, "Window Type");
    char buf[1024];
    char *bufptr = buf;
    for(int i = 0; i < windowcount; i++){
        bufptr += sprintf(bufptr, "%s ", window_names[i]);
    }
    CLASS_ATTR_ENUM(analyzer_class, "windowtype", 0, buf);
    
    CLASS_ATTR_LONG(analyzer_class, "initialdelay", 0, t_analyzer, x_delay);
    CLASS_ATTR_ACCESSORS(analyzer_class, "initialdelay", NULL, analyzer_initialdelay_set);
    CLASS_ATTR_LABEL(analyzer_class, "initialdelay", 0, "Initial Delay");
    
    CLASS_ATTR_LONG(analyzer_class, "numpitches", 0, t_analyzer, x_npitch);
    CLASS_ATTR_ACCESSORS(analyzer_class, "numpitches", NULL, analyzer_numpitches_set);
    CLASS_ATTR_LABEL(analyzer_class, "numpitches", 0, "Number of Pitches to Output");
    
    CLASS_ATTR_LONG(analyzer_class, "numpeakstofind", 0, t_analyzer, x_npeakanal);
    CLASS_ATTR_ACCESSORS(analyzer_class, "numpeakstofind", NULL, analyzer_numpeakstofind_set);
    CLASS_ATTR_LABEL(analyzer_class, "numpeakstofind", 0, "Number of Peaks to Analyze");
    
    CLASS_ATTR_LONG(analyzer_class, "numpeakstooutput", 0, t_analyzer, x_npeakout);
    CLASS_ATTR_ACCESSORS(analyzer_class, "numpeakstooutput", NULL, analyzer_numpeakstooutput_set);
    CLASS_ATTR_LABEL(analyzer_class, "numpeakstooutput", 0, "Number of Peaks to Output");
    
    CLASS_ATTR_LONG(analyzer_class, "barkformat", 0, t_analyzer, x_output);
    CLASS_ATTR_ACCESSORS(analyzer_class, "barkformat", analyzer_barkformat_get, analyzer_barkformat_set);
    CLASS_ATTR_LABEL(analyzer_class, "barkformat", 0, "Bark Format");
    CLASS_ATTR_ENUM(analyzer_class, "barkformat", 0, "list nolist");
    
    CLASS_ATTR_SYM(analyzer_class, "scale", 0, t_analyzer, x_scale);
    CLASS_ATTR_LABEL(analyzer_class, "scale", 0, "Scale");
    CLASS_ATTR_ENUM(analyzer_class, "scale", 0, "log linear");
    
    CLASS_ATTR_LONG(analyzer_class, "loud", 0, t_analyzer, x_loud);
    CLASS_ATTR_LABEL(analyzer_class, "loud", 0, "Loudness Type");
    
    CLASS_ATTR_LONG(analyzer_class, "bright", 0, t_analyzer, x_bright);
    CLASS_ATTR_LABEL(analyzer_class, "bright", 0, "Brightness Type");
    
    class_dspinit(analyzer_class);
    
    class_register(CLASS_BOX, analyzer_class);
    return 0;
}
