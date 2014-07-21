/*
SVN_REVISION: $LastChangedRevision$
DESCRIPTION: Onset detector based on exceeding a threshold change in spectral energy.  Written by Tristan Jehan?
*/


#include "ext.h"
#include "z_dsp.h"
#include "fftnobitrev.h"
#include <math.h>

#define RES_ID	7010

#define PI 3.14159265358979323f
#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 512

#define DEFATTACKTIME 4400 // 100 ms
#define DEFATTACKTHRESH 1.0f
#define DEFDECAYTHRESH -1.0f
#define DEFHIGHTHRESH 0.0f
#define DEFLOWTHRESH -8.0f
#define DEFENERGYTHRESH ((DEFHIGHTHRESH + DEFLOWTHRESH) * 0.5f)
#define DEFCENTHRESH 6000.0f

#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f))
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN_W(i,ac) (0.42f - 0.5f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.08f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))

void *getonset_class;

enum {Recta = 0, Hann, Hamm, Black};
enum {OvLa = 0, Wind, AtTi, AtTh, DeTh, HiTh, EnTh, LoTh, CeTh};  

typedef struct _getonset {

	t_pxobject g_obj;

	t_float g_Fs;			// Sample rate

	t_int g_overlap;		// Number of overlaping samples
	t_int g_window;			// Type of window
	t_int g_deltattack;		// Minimum time separating 2 Attacks (number of samples)
	t_float g_attackthres;	// Attack Threshold
	t_float g_decaythres;	// Decay Threshold
	t_float g_highthres;	// High Energy Threshold
	t_float g_enerthres;	// Energy Threshold
	t_float g_lowthres;		// Low Energy Threshold
	t_float g_centhres;		// Centroid Threshold
	
	t_float g_energy;		// Current log(Energy)
	t_float g_lastEner;		// Last log(Energy)
	t_float g_centroid;		// Current Centroid
	t_int g_onset;			// Current Onset value
	t_int g_offset;			// Current Offset value
	t_int g_noteOn;			// Note On? (1=yes 0=no) 
	t_int g_attackReady;	// Ready for a new attack? (1=yes 0=no) 
	t_int g_timer;			// Counter of minimum number of samples between 2 onsets

    t_float *Buf1;			// FFT buffer 1
    t_float *Buf2;			// FFT buffer 2
    t_float *memFFT;		// fftnobitrev.c memory space
    t_int BufSize;			// FFT buffer size
    t_int Buf1WritePos;		// Where to write in buffer 1

	void *g_outonset;		// Outlet for the onsets!
	void *g_outoffset;		// Outlet for the offsets!
	void *g_outener;		// Outlet for the log(Energy)
	void *g_outcent;		// Outlet for the Centroid
	void *g_clock;			// Use a clock for outputs... (better than Qelem)
		
} t_getonset;

t_int *getonset_perform(t_int *w);
void getonset_dsp(t_getonset *x, t_signal **sp, short *connect);
void getonset_float(t_getonset *x, double f);
void getonset_int(t_getonset *x, long n);
void getonset_assist(t_getonset *x, void *b, long m, long a, char *s);
void *getonset_new(t_symbol *s, short argc, t_atom *argv);
void getonset_free(t_getonset *x);
void getonset_tick(t_getonset *x);

void main(void) {

	setup( (t_messlist **)&getonset_class, getonset_new, (method)getonset_free,
			(short)sizeof(t_getonset), 0L, A_GIMME, 0);
		
	addmess((method)getonset_dsp, "dsp", A_CANT, 0);
	addmess((method)getonset_assist, "assist", A_CANT, 0);
	addfloat((method)getonset_float);
	addint((method)getonset_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *getonset_perform(t_int *w) {
	t_float *in = (t_float *)(w[1]);
	t_getonset *x = (t_getonset *)(w[2]);
	t_int n = (int)(w[3]);
	t_int *myintin = (t_int *)in;
	t_int *myintBuf1 = (t_int *)(x->Buf1);
	t_int *myintBuf2 = (t_int *)(x->Buf2);
	t_int *myintBufFFT = (t_int *)(x->BufFFT);
	t_int i, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->e_overlap;
	t_float *TmpBuf = x->Buf1;

	// Copy input samples into FFT buffer	
	while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
		myintBuf1[x->BufWritePos] = myintin[index];
		x->BufWritePos++;
		index++;
		cpt--;
	}
	
	// When Buffer 1 is full...
	if (x->BufWritePos >= x->BufSize) {
			
		// Save overlapping samples into Buffer 2
		for (i=0; i<x->e_overlap; ++i) 
			myintBuf2[i] = myintBuf1[overlapindex+i];

		maxindex = n - index + x->e_overlap;

		// Copy the rest of incoming samples into Buffer 2
		for (i=x->e_overlap; i<maxindex; ++i) {
			myintBuf2[i] = myintin[index];
			index++;
		}
		
		x->BufWritePos = maxindex;

		// Make a copy of Buffer 1 into Buffer FFT for computation outside the perform function
		for (i=0; i<x->BufSize; ++i) 
			myintBufFFT[i] = myintBuf1[i];
			
		// Go for the FFT outside the perform function with a delay of 0 ms!
		clock_delay(x->e_clock,0);
		
		// Swap buffers
		x->Buf1 = x->Buf2;
		x->Buf2 = TmpBuf;
	}
	
	return (w+4);
	
}

void getonset_dsp(t_getonset *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	if (vs > x->BufSize) post(" You need to use a smaller signal vector size...",0);
	else if (connect[0]) dsp_add(getonset_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}

void getonset_float(t_getonset *x, double f) {
	long in = x->g_obj.z_in;

	if (!in)
		return;
	in--;
	switch (in) {
		case AtTh: x->g_attackthres = f; break;
		case DeTh: x->g_decaythres = f; break;
		case HiTh: x->g_highthres = f; break;
		case EnTh: x->g_enerthres = f; break;
		case LoTh: x->g_lowthres = f; break;
		case CeTh: x->g_centhres = f; break;
		default: getonset_int(x,(long)f);
	}
}

void getonset_int(t_getonset *x, long n) {
	long in = x->g_obj.z_in;

	if (!in)
		return;
	in--;
	switch (in) {
		case OvLa: {
			x->g_overlap = (int)(n * x->BufSize * 0.01f); 
			if (x->g_overlap > x->BufSize) x->g_overlap = x->BufSize-1; 
			else if (x->g_overlap < 1) x->g_overlap = 0; 
			} break;
		case Wind: {
			x->g_window = n;
			if ((x->g_window >= 4) || (x->g_window < 1)) x->g_window = 0;
			} break;
		case AtTi: x->g_deltattack = n * 44; break;
		default: getonset_float(x,(double)n);
	}
}

void getonset_assist(t_getonset *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID,m,a,1,11,s);
}

void *getonset_new(t_symbol *s, short argc, t_atom *argv) {

    t_getonset *x = (t_getonset *)newobject(getonset_class);
    t_float ms2samp;    
    dsp_setup((t_pxobject *)x,10);
	x->g_clock = clock_new(x,(method)getonset_tick);
	x->g_outcent = floatout((t_getonset *)x);
	x->g_outener = floatout((t_getonset *)x);
    x->g_outoffset = bangout((t_getonset *)x);
    x->g_outonset = bangout((t_getonset *)x);
 	x->g_Fs = sys_getsr();
	x->g_energy = 0.0f;
	x->g_lastEner = 0.0f;
	x->g_centroid = 0.0f;
	x->g_onset = 0;
	x->g_offset = 0;
	x->g_noteOn = 0;
	x->g_attackReady = 0;
	x->g_timer = 0;
	x->BufWritePos = 0;
	x->BufReadPos = vs;
	
	ms2samp = x->g_Fs * 0.001f;
	
	// Look at first argument
	if (argv[0].a_type == A_LONG) x->BufSize = argv[0].a_w.w_long; // Samples
	else if (argv[0].a_type == A_FLOAT) x->BufSize = (int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	else x->BufSize = DEFBUFSIZE;
	
	x->FFTSize = x->BufSize;

	// Take a convenient FFT Size
	if (x->BufSize < vs) { 
		post(" Minimum FFT Size is %i",vs);
		x->FFTSize = vs;
		x->BufSize = vs;
	}
	else if ((x->BufSize > vs) && (x->BufSize < 128))  x->FFTSize = 128;
	else if ((x->BufSize > 128) && (x->BufSize < 256)) x->FFTSize = 256;
	else if ((x->BufSize > 256) && (x->BufSize < 512)) x->FFTSize = 512;
	else if ((x->BufSize > 512) && (x->BufSize < 1024)) x->FFTSize = 1024;
	else if ((x->BufSize > 1024) && (x->BufSize < 2048)) x->FFTSize = 2048;
	else if ((x->BufSize > 2048) && (x->BufSize < 4096)) x->FFTSize = 4096;
	else if ((x->BufSize > 4096) && (x->BufSize < 8192)) x->FFTSize = 8192;
	else if ((x->BufSize > 8192) && (x->BufSize < 16384)) x->FFTSize = 16384;
	else if ((x->BufSize > 16384) && (x->BufSize < 32768)) x->FFTSize = 32768;
	else if (x->BufSize > 32768) {
		post(" Maximum FFT Size is 65536",0);
		x->FFTSize = 65536;
		x->BufSize = 65536;
	}

	// Look at second argument
	if (argv[1].a_type == A_LONG) x->g_overlap = argv[1].a_w.w_long; // Samples
	else if (argv[1].a_type == A_FLOAT) x->g_overlap = (int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	else x->g_overlap = DEFBUFSIZE/2;

	// Look at third argument
	if (argv[2].a_w.w_sym == ps_rectangular) x->c_window = Recta;
	else if (argv[2].a_w.w_sym == ps_hanning) x->c_window = Hann;
	else if (argv[2].a_w.w_sym == ps_hamming) x->c_window = Hamm;
	else if (argv[2].a_w.w_sym == ps_blackman) x->c_window = Black;
	else x->g_window = Hann;

	if (argv[3].a_type == A_LONG) x->g_deltattack = 44 * argv[3].a_w.w_long;
	else if (argv[3].a_type == A_FLOAT) x->g_deltattack = 44 * (long)argv[3].a_w.w_float;
	else x->g_deltattack = DEFATTACKTIME;

	if (argv[4].a_type == A_FLOAT) x->g_attackthres = argv[4].a_w.w_float;
	else if (argv[4].a_type == A_LONG) x->g_attackthres = (float)argv[4].a_w.w_long;
	else x->g_attackthres = DEFATTACKTHRESH;
	
	if (argv[5].a_type == A_FLOAT) x->g_decaythres = argv[5].a_w.w_float;
	else if (argv[5].a_type == A_LONG) x->g_decaythres = (float)argv[5].a_w.w_long;
	else x->g_decaythres = DEFDECAYTHRESH;
	
	if (argv[6].a_type == A_FLOAT) x->g_highthres = argv[6].a_w.w_float;
	else if (argv[6].a_type == A_LONG) x->g_highthres = (float)argv[6].a_w.w_long;
	else x->g_highthres = DEFHIGHTHRESH;
	
	if (argv[7].a_type == A_FLOAT) x->g_enerthres = argv[7].a_w.w_float;
	else if (argv[7].a_type == A_LONG) x->g_enerthres = (float)argv[7].a_w.w_long;
	else x->g_enerthres = DEFENERGYTHRESH;
	
	if (argv[8].a_type == A_FLOAT) x->g_lowthres = argv[8].a_w.w_float;
	else if (argv[8].a_type == A_LONG) x->g_lowthres = (float)argv[8].a_w.w_long;
	else x->g_lowthres = DEFLOWTHRESH;
	
	if (argv[9].a_type == A_FLOAT) x->g_centhres = argv[9].a_w.w_float;
	else if (argv[9].a_type == A_LONG) x->g_centhres = (float)argv[9].a_w.w_long;
	else x->g_centhres = DEFCENTHRESH;

	// Allocate memory
	x->Buf1 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf2 = t_getbytes(x->BufSize * sizeof(float));
	x->memFFT = t_getbytes(CMAX * x->FFTSize * sizeof(float)); // memory allocated for fft twiddle

	x->Window = t_getbytes(x->BufSize * sizeof(float));
	if (x->FFTSize != x->BufSize)	
		x->WindFFT = t_getbytes(x->FFTSize * sizeof(float));
	else x->WindFFT = x->Window;

	// Compute and store Windows
	if ((x->g_window > Recta) && (x->g_window <= Black)) {
		
		switch (x->c_window) {

			case Hann: 	for (i=0; i<x->BufSize; ++i)
							x->Window[i] = HANNING_W(i, x->BufSize);
						if (x->FFTSize != x->BufSize)	
							for (i=0; i<x->FFTSize; ++i)
								x->WindFFT[i] = HANNING_W(i, x->FFTSize);
 						break;
			case Hamm:	for (i=0; i<x->BufSize; ++i)
							x->Window[i] = HAMMING_W(i, x->BufSize);
						if (x->FFTSize != x->BufSize)	
							for (i=0; i<x->FFTSize; ++i)
								x->WindFFT[i] = HAMMING_W(i, x->FFTSize);
						break;
			case Black: for (i=0; i<x->BufSize; ++i)
							x->Window[i] = BLACKMAN_W(i, x->BufSize);
						if (x->FFTSize != x->BufSize)	
							for (i=0; i<x->FFTSize; ++i)
								x->WindFFT[i] = BLACKMAN_W(i, x->FFTSize);
						break;
		}
	}		
		
    return (x);
}

void  getonset_free(t_getonset *x) {
	if (x->Buf1) t_freebytes(x->Buf1, x->BufSize * sizeof(float));
	if (x->Buf2) t_freebytes(x->Buf2, x->BufSize * sizeof(float));
	if (x->BufFFT) t_freebytes(x->BufFFT, x->FFTSize * sizeof(float));
	if (x->memFFT) t_freebytes(x->memFFT, CMAX * x->FFTSize * sizeof(float));
	freeobject((t_object *)x->g_clock);
	dsp_free((t_pxobject *)x);
}

void getonset_tick(t_getonset *x) {

	t_int halfFFTSize = x->FFTSize/2;
	t_float FsOverFFTSize = x->g_Fs / x->FFTSize;
	t_float sumSpectrum = 0.0f;
	t_float derEner;
	t_float Energy = 0.0f;
	t_float Centroid = 0.0f;
	t_int i;

	// Timer control
	if (x->g_timer > 0) x->g_timer -= n;

	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; ++i)
		x->BufFFT[i] = 0.0f;

	if ((x->g_window > Recta) && (x->g_window <= Black)) {

		// Window the samples
		switch (x->g_window) {
			case Hann: 	for (i=0; i<x->FFTSize; ++i) 
							x->BufFFT[i] *= HANNING_W(i, x->FFTSize);
 						break;
			case Hamm:	for (i=0; i<x->FFTSize; ++i) 
							x->BufFFT[i] *= HAMMING_W(i, x->FFTSize);
						break;
			case Black: for (i=0; i<x->FFTSize; ++i) 
							x->BufFFT[i] *= BLACKMAN_W(i, x->FFTSize);
						break;
		}
	}
			
	// FFT
	fftRealfast(x->FFTSize, x->BufFFT, x->memFFT);
		
	// Absolute version
	for (i=2; i<x->BufSize; i+=2) 
		x->BufFFT[i/2] = sqrtf( (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]) );

	// Energy and Centroid			
	for (i=1; i<halfFFTSize; ++i) {
		Energy += (x->BufFFT[i] * x->BufFFT[i]);
		Centroid += (i * x->BufFFT[i]);
		sumSpectrum += x->BufFFT[i];
	}			
	if (sumSpectrum <= 0.0f) x->g_centroid = 0.0f;
	else x->g_centroid = (Centroid * FsOverBufSize) / sumSpectrum;
	Energy = Energy / (halfFFTSize-1);
	if (Energy > 0.0f) x->g_energy = log(Energy);
	else (x->g_energy = 0.0f);

	// Derived Energy
	if (x->g_lastEner == 0.0f) derEner = 0.0f;
	else derEner = (x->g_energy / x->g_lastener) - 1.0f; // Centered around zero
	x->g_lastener = x->g_energy;
				
	// Onset/Offset decision
	if ((derEner > x->g_attackthres) && (x->g_timer <= 0) && ((x->g_noteOn == 0) || (x->g_attackReady == 1)) 
		&& ((x->g_energy > x->g_highthres) || (x->g_lastener > x->g_highthres))) {
		x->g_noteOn = 1;
		x->g_attackReady = 0;
		x->g_onset = 1;
		x->g_timer = x->g_deltattack;
	} 
	else if ((x->g_noteOn == 1) && ((((x->g_energy < x->g_enerthres) || (x->g_lastEner < x->g_enerthres)) 
 			&& (derEner < x->g_decaythres)) || (x->g_energy < x->g_lowthres))) {
			x->g_noteOn = 0;
			x->g_offset = 1;
	}
	else if (x->g_energy < x->g_enerthres) x->g_attackReady = 1;

	// Output onset and offset
    outlet_float(x->g_outEner, x->g_energy);
    outlet_float(x->g_outCent, x->g_centroid);
	x->g_energy = 0.0f;
	x->g_centroid = 0.0f;
    if (x->g_onset == 1) {
    	outlet_bang(x->g_outonset);
		x->g_onset = 0;
	}
	else if (x->g_offset == 1) {
	    	outlet_bang(x->g_outoffset);
			x->g_offset = 0;
	}
}	
