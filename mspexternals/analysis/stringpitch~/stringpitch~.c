#include "ext.h"
#include "z_dsp.h"

#include <math.h>
#include "vector.h"
#include "fft.h"

#define RES_ID	7013

#define PI 3.14159265358979323f
#define TWOPI 6.28318530717952646f
#define DEFBUFSIZE 512

#define DEFATTACKTIME 4400 // 100 ms
#define DEFATTACKTHRESH 1.0f
#define DEFDECAYTHRESH -1.0f
#define DEFHIGHTHRESH 0.0f
#define DEFLOWTHRESH -8.0f
#define DEFENERGYTHRESH ((DEFHIGHTHRESH + DEFLOWTHRESH) * 0.5f)
#define DEFCENTHRESH 6000.0f
#define DEFMAXPEAK 20
#define DEFPEAKTHRES 1.0f
#define DEFMINFREQ 60
#define DEFMAXFREQ 3000

void *stringpitch_class;

enum {OvLa = 0, AtTi, AtTh, DeTh, HiTh, EnTh, LoTh, CeTh, MaxP, PeTh, MinF, MaxF};  

typedef struct _stringpitch {

	t_pxobject s_obj;

	t_float s_Fs;			// Sample rate

	t_int s_overlap;		// Number of overlaping samples
	t_int s_deltattack;		// Minimum time separating 2 Attacks (number of samples)
	t_float s_attackthres;	// Attack Threshold
	t_float s_decaythres;	// Decay Threshold
	t_float s_highthres;	// High Energy Threshold
	t_float s_enerthres;	// Energy Threshold
	t_float s_lowthres;		// Low Energy Threshold
	t_float s_centhres;		// Centroid Threshold
	t_int s_maxpeak;		// Maximum of peaks possibly detected
	t_float s_peakthres;	// Peak Threshold
	t_int s_minfreq;		// Minimum possible pitch
	t_int s_maxfreq;		// Maximum possible pitch
	
	t_float s_pitch;		// Current pitch estimation
	t_float s_energy;		// Current log(Energy)
	t_float s_lastEner;		// Last log(Energy)
	t_float s_centroid;		// Current Centroid
	t_int s_onset;			// Current Onset value
	t_int s_offset;			// Current Offset value
	t_int s_noteOn;			// Note On? (1=yes 0=no) 
	t_int s_attackReady;	// Ready for a new attack? (1=yes 0=no) 
	t_int s_timer;			// Counter of minimum number of samples between 2 onsets

    t_float *Bufft;			// FFT and Time domain buffer
    t_float *BufSamples;	// Temporary time domain buffer
    t_float *BufAbs;		// Absolute FFT
    t_float *BufInstFreq;	// Instantaneous Frequency buffer (contains frequencies)
    t_float *PeakBuf;		// Buffer containing peaks

    t_int BufSize;			// FFT buffer size
    t_int BufWritePos;		// Where to write in buffer

	void *s_outpitch;		// Outlet for the Pitch
	void *s_outonset;		// Outlet for the onsets!
	void *s_outoffset;		// Outlet for the offsets!
	void *s_outener;		// Outlet for the log(Energy)
	void *s_outcent;		// Outlet for the Centroid
	void *s_clock;			// Use a clock for outputs... (better than Qelem)
		
} t_stringpitch;

t_int *stringpitch_perform(t_int *w);
void stringpitch_dsp(t_stringpitch *x, t_signal **sp, short *connect);
void stringpitch_float(t_stringpitch *x, double f);
void stringpitch_int(t_stringpitch *x, long n);
void stringpitch_assist(t_stringpitch *x, void *b, long m, long a, char *s);
void *stringpitch_new(t_symbol *s, short argc, t_atom *argv);
void stringpitch_free(t_stringpitch *x);
void stringpitch_tick(t_stringpitch *x);

void main(void) {

	setup( (t_messlist **)&stringpitch_class, stringpitch_new, (method)stringpitch_free,
			(short)sizeof(t_stringpitch), 0L, A_GIMME, 0);
		
	addmess((method)stringpitch_dsp, "dsp", A_CANT, 0);
	addmess((method)stringpitch_assist, "assist", A_CANT, 0);
	addfloat((method)stringpitch_float);
	addint((method)stringpitch_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *stringpitch_perform(t_int *w) {
	t_float *in = (t_float *)(w[1]);
	t_stringpitch *x = (t_stringpitch *)(w[2]);
	t_int n = (int)(w[3]);
	t_int *myintin = (t_int *)in;
	t_int *myintBufft = (t_int *)(x->Bufft);
	t_int *myintBufSamples = (t_int *)(x->BufSamples);
	t_int i, k = 0, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->s_overlap;
	t_int halfBufSize = (int)(x->BufSize * 0.5f);
	t_int quarterBufSize = (int)(x->BufSize * 0.25f);
	t_float sumSpectrum = 0.0f;
	t_float *TmpBufft = x->Bufft;
	t_float derEner;
	t_float Energy = 0.0f;
	t_float Centroid = 0.0f;
	t_float maxAmp = 0.0f;
	t_float Areal, Aimag, Breal, Bimag;
	t_float W = TWOPI / x->BufSize; 

	// Clean Buffers
	for (i=0; i<x->BufSize; ++i)
		x->BufSamples[i] = 0.0f;
	for (i=0; i<x->s_maxpeak; ++i) 
		x->PeakBuf[i] = 0.0f;
		
	// Copy input samples into FFT buffer	
	while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
		myintBufft[x->BufWritePos] = myintin[index];
		x->BufWritePos++;
		index++;
		cpt--;
	}
	
	// Timer control
	if (x->s_timer > 0) x->s_timer -= n;
	
	// When FFT Buffer is full...
	if (x->BufWritePos >= x->BufSize) {
			
		// Save overlaping samples into Buffer of extra Samples
		for (i=0; i<x->s_overlap; ++i) 
			myintBufSamples[i] = myintBufft[overlapindex + i];

		maxindex = n - index + x->s_overlap;

		// Copy the rest of incoming samples into Buffer 2
		for (i=x->s_overlap; i<maxindex; ++i) {
			myintBufSamples[i] = myintin[index];
			index++;
		}		
		
		x->BufWritePos = maxindex;
						
		// FFT	
		fftRealfast(x->BufSize, x->Bufft); // Result is real & imaginary parts of FFT alternately in Bufft
		
		// Instantaneous Frequency Estimation (includes spectrum domain hanning window) and Absolute version of FFT		
		x->BufAbs[0] = sqrtf((x->Bufft[0] * x->Bufft[0]) + (x->Bufft[1] * x->Bufft[1])); // Case of 2 Abs FFT edges
		x->BufAbs[halfBufSize-1] = sqrtf((x->Bufft[x->BufSize-2] * x->Bufft[x->BufSize-2]) + 
										 (x->Bufft[x->BufSize-1] * x->Bufft[x->BufSize-1]));
		
		for (i=2; i<x->BufSize-2; i+=2) {
			Areal = x->Bufft[i] - 0.5f * (x->Bufft[i-2] + x->Bufft[i+2]);
			Aimag = x->Bufft[i+1] - 0.5f * (x->Bufft[i-1] + x->Bufft[i+3]);			
			Breal = x->Bufft[i] - 0.5f * (cos(W) * (x->Bufft[i-2] + x->Bufft[i+2]) + sin(W) * (x->Bufft[i+3] - x->Bufft[i-1]));
			Bimag = x->Bufft[i+1] - 0.5f * (cos(W) * (x->Bufft[i-1] + x->Bufft[i+3]) + sin(W) * (x->Bufft[i-2] - x->Bufft[i+2]));											
			x->BufInstFreq[i/2] = x->s_Fs * (((i/2)/x->BufSize) + ((atanf(Aimag/Areal) - atanf(Bimag/Breal)) / TWOPI ));
			x->BufAbs[i/2] = sqrtf((x->Bufft[i] * x->Bufft[i]) + (x->Bufft[i+1] * x->Bufft[i+1])); // Abs FFT
			if (x->BufAbs[i/2] > maxAmp) maxAmp = x->BufAbs[i/2]; // find maximum amplitude
		}
		
		// Peak Detection	
		// Now BufInstFreq contains accurate frequencies and BufAbs contains associated amplitudes 		
		for (i=1; i<quarterBufSize-1; ++i) { // We limit the search to half the frequency range
			if ((x->BufAbs[i] != 0) && 
					(((x->BufAbs[i-2] < x->BufAbs[i-1]) && (x->BufAbs[i-1] < x->BufAbs[i]) && (x->BufAbs[i] > x->BufAbs[i+1])) ||
					 ((x->BufAbs[i-1] < x->BufAbs[i]) && (x->BufAbs[i] > x->BufAbs[i+1]) && (x->BufAbs[i+1] > x->BufAbs[i+2])))) {
					 x->PeakBuf[k] = x->BufInstFreq[i];
					 k++;
					 if (k > x->s_maxpeak) break;
			}
		}
											
		// Energy and Centroid			
		for (i=1; i<halfBufSize; ++i) {
			Energy += (x->BufAbs[i] * x->BufAbs[i]);
			Centroid += (x->BufInstFreq[i] * x->BufAbs[i]);
			sumSpectrum += x->BufAbs[i];
		}
		if (sumSpectrum <= 0.0f) x->s_centroid = 0.0f;
		else x->s_centroid = Centroid / sumSpectrum;
		Energy = Energy / (halfBufSize-1);
		if (Energy > 0.0f) x->s_energy = log(Energy);
		else (x->s_energy = 0.0f);
			
		// Derived Energy
		if (x->s_lastEner == 0.0f) derEner = 0.0f;
		else derEner = (x->s_energy / x->s_lastEner) - 1.0f; // Centered around zero
		x->s_lastEner = x->s_energy;
				
		// Onset/Offset decision
		if ((derEner > x->s_attackthres) && (x->s_timer <= 0) && ((x->s_noteOn == 0) || (x->s_attackReady == 1)) 
			&& ((x->s_energy > x->s_highthres) || (x->s_lastEner > x->s_highthres))) {
			x->s_noteOn = 1;
			x->s_attackReady = 0;
			x->s_onset = 1;
			x->s_timer = x->s_deltattack;
		}
 		else if ((x->s_noteOn == 1) && ((((x->s_energy < x->s_enerthres) || (x->s_lastEner < x->s_enerthres)) 
 				&& (derEner < x->s_decaythres)) || (x->s_energy < x->s_lowthres))) {
			x->s_noteOn = 0;
			x->s_offset = 1;
		}
		else if (x->s_energy < x->s_enerthres) x->s_attackReady = 1;
			
		// Output data with a delay of 0 ms!
		clock_delay(x->s_clock,0);
		
		// Swap buffers
		x->Bufft = x->BufSamples;
		x->BufSamples = TmpBufft;
	}

	return (w+4);
}

void stringpitch_dsp(t_stringpitch *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	if (vs > x->BufSize) post(" You need to use a smaller signal vector size...",0);
	else if (connect[0]) dsp_add(stringpitch_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}

void stringpitch_float(t_stringpitch *x, double f) {
	long in = x->s_obj.z_in;

	if (!in)
		return;
	in--;
	switch (in) {
		case AtTh: x->s_attackthres = f; break;
		case DeTh: x->s_decaythres = f; break;
		case HiTh: x->s_highthres = f; break;
		case EnTh: x->s_enerthres = f; break;
		case LoTh: x->s_lowthres = f; break;
		case CeTh: x->s_centhres = f; break;
		case PeTh: x->s_peakthres = f; break;
		default: stringpitch_int(x,(long)f);
	}
}

void stringpitch_int(t_stringpitch *x, long n) {
	long in = x->s_obj.z_in;
	int vs = sys_getblksize();

	if (!in)
		return;
	in--;
	switch (in) {
		case OvLa:
		{
			x->s_overlap = (int)(n * x->BufSize * 0.01f); 
			if ((x->s_overlap + vs) > x->BufSize) {
				x->s_overlap = x->BufSize - vs;
				post(" Maximum overlaping is %i \%. For more overlap, \
						you need to use a smaller signal vector size...",x->BufSize - vs);
			}
			else if (x->s_overlap < 1) x->s_overlap = 0; 
		} break;
		case AtTi: x->s_deltattack = n * 44; break;
		case MaxP: x->s_maxpeak = n; break;
		case MinF: x->s_minfreq = n; break;
		case MaxF: x->s_maxfreq = n; break;
		default: stringpitch_float(x,(double)n);
	}
}

void stringpitch_assist(t_stringpitch *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID,m,a,1,14,s);
}

void *stringpitch_new(t_symbol *s, short argc, t_atom *argv) {
    t_stringpitch *x = (t_stringpitch *)newobject(stringpitch_class);
	x->s_clock = clock_new(x,(method)stringpitch_tick);
	x->s_outcent = floatout((t_stringpitch *)x);
	x->s_outener = floatout((t_stringpitch *)x);
    x->s_outoffset = bangout((t_stringpitch *)x);
    x->s_outonset = bangout((t_stringpitch *)x);
    x->s_outpitch = floatout((t_stringpitch *)x);
    dsp_setup((t_pxobject *)x,13);
	
	x->s_Fs = sys_getsr();
	x->s_deltattack = DEFATTACKTIME;
	x->s_attackthres = DEFATTACKTHRESH;
	x->s_decaythres = DEFDECAYTHRESH;
	x->s_highthres = DEFHIGHTHRESH;
	x->s_enerthres = DEFENERGYTHRESH;
	x->s_lowthres = DEFLOWTHRESH;
	x->s_centhres = DEFCENTHRESH;
	x->BufSize = DEFBUFSIZE;
	x->s_maxpeak = DEFMAXPEAK;
	x->s_peakthres = DEFPEAKTHRES;
	x->s_minfreq = DEFMINFREQ;
	x->s_maxfreq = DEFMAXFREQ;
	x->s_peakthres = DEFPEAKTHRES;
	x->s_minfreq = DEFMINFREQ;
	x->s_maxfreq = DEFMAXFREQ;
	x->s_overlap = (int)(x->BufSize * 0.5f);
	x->s_pitch = 0.0f;
	x->s_energy = 0.0f;
	x->s_lastEner = 0.0f;
	x->s_centroid = 0.0f;
	x->s_onset = 0;
	x->s_offset = 0;
	x->s_noteOn = 0;
	x->s_attackReady = 0;
	x->s_timer = 0;
		
	if (argv[0].a_type == A_LONG) x->BufSize = argv[0].a_w.w_long;
	else if (argv[0].a_type == A_FLOAT) x->BufSize = (long)argv[0].a_w.w_float;
	if (argv[1].a_type == A_LONG) x->s_overlap = (long)(argv[1].a_w.w_long * x->BufSize * 0.01f);
	else if (argv[1].a_type == A_FLOAT) x->s_overlap = (long)(argv[1].a_w.w_float * x->BufSize * 0.01f);
	if (argv[2].a_type == A_LONG) x->s_deltattack = 44 * argv[2].a_w.w_long;
	else if (argv[2].a_type == A_FLOAT) x->s_deltattack = 44 * (long)argv[2].a_w.w_float;
	if (argv[3].a_type == A_FLOAT) x->s_attackthres = argv[3].a_w.w_float;
	else if (argv[3].a_type == A_LONG) x->s_attackthres = (float)argv[3].a_w.w_long;
	if (argv[4].a_type == A_FLOAT) x->s_decaythres = argv[4].a_w.w_float;
	else if (argv[4].a_type == A_LONG) x->s_decaythres = (float)argv[4].a_w.w_long;
	if (argv[5].a_type == A_FLOAT) x->s_highthres = argv[5].a_w.w_float;
	else if (argv[5].a_type == A_LONG) x->s_highthres = (float)argv[5].a_w.w_long;
	if (argv[6].a_type == A_FLOAT) x->s_enerthres = argv[6].a_w.w_float;
	else if (argv[6].a_type == A_LONG) x->s_enerthres = (float)argv[6].a_w.w_long;
	if (argv[7].a_type == A_FLOAT) x->s_lowthres = argv[7].a_w.w_float;
	else if (argv[7].a_type == A_LONG) x->s_lowthres = (float)argv[7].a_w.w_long;
	if (argv[8].a_type == A_FLOAT) x->s_centhres = argv[8].a_w.w_float;
	else if (argv[8].a_type == A_LONG) x->s_centhres = (float)argv[8].a_w.w_long;
	if (argv[9].a_type == A_LONG) x->s_maxpeak = argv[9].a_w.w_long;
	else if (argv[9].a_type == A_FLOAT) x->s_maxpeak = (long)argv[9].a_w.w_float;
	if (argv[10].a_type == A_FLOAT) x->s_peakthres = argv[10].a_w.w_float;
	else if (argv[10].a_type == A_LONG) x->s_peakthres = (float)argv[10].a_w.w_long;
	if (argv[11].a_type == A_LONG) x->s_minfreq = argv[11].a_w.w_long;
	else if (argv[11].a_type == A_FLOAT) x->s_minfreq = (long)argv[11].a_w.w_float;
	if (argv[12].a_type == A_LONG) x->s_maxfreq = argv[12].a_w.w_long;
	else if (argv[12].a_type == A_FLOAT) x->s_maxfreq = (long)argv[12].a_w.w_float;

	x->Bufft = t_getbytes(x->BufSize * sizeof(float));
	x->BufSamples = t_getbytes(x->BufSize * sizeof(float));
	x->BufAbs = t_getbytes(x->BufSize * sizeof(float) / 2);
	x->BufInstFreq = t_getbytes(x->BufSize * sizeof(float) / 2);
	x->PeakBuf  = t_getbytes(x->s_maxpeak * sizeof(float));
	x->BufWritePos = 0;
		
    return (x);
}

void  stringpitch_free(t_stringpitch *x) {
	if (x->Bufft) t_freebytes(x->Bufft, x->BufSize * sizeof(float));
	if (x->BufSamples) t_freebytes(x->BufSamples, x->BufSize * sizeof(float));
	if (x->BufAbs) t_freebytes(x->BufAbs, x->BufSize * sizeof(float) / 2);
	if (x->BufInstFreq) t_freebytes(x->BufInstFreq, x->BufSize * sizeof(float) / 2);
	if (x->PeakBuf)  t_freebytes(x->PeakBuf, x->s_maxpeak * sizeof(float));
	freeobject((t_object *)x->s_clock);
	dsp_free((t_pxobject *)x);
}

void stringpitch_tick(t_stringpitch *x) {
    outlet_float(x->s_outpitch, x->s_pitch);
    outlet_float(x->s_outener, x->s_energy);
    outlet_float(x->s_outcent, x->s_centroid);
	x->s_energy = 0.0f;
	x->s_centroid = 0.0f;
    if (x->s_onset == 1) {
    	outlet_bang(x->s_outonset);
		x->s_onset = 0;
	}
	else if (x->s_offset == 1) {
	    	outlet_bang(x->s_outoffset);
			x->s_offset = 0;
	}
}	
