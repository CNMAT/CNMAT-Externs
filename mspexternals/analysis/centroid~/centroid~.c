#include "ext.h"
#include "z_dsp.h"
#include "fft.h"
#include <math.h>

#define RES_ID	7012

#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024

#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN_W(i,ac) (0.42f - 0.5f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.08f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))

void *centroid_class;

enum {Recta = 0, Hann, Hamm, Black};

typedef struct _centroid {

	t_pxobject c_obj;

	t_float c_Fs;			// Sample rate
	t_int c_overlap;		// Number of overlaping samples
	t_int c_window;			// Type of window	
	t_float c_centroid;		// Current Centroid

    t_float *Buf1;			// buffer 1
    t_float *Buf2;			// buffer 2
    t_float *BufFFT;		// FFT buffer
    t_float *WindFFT;		// Window of FFTSize
    t_float *memFFT;		// fftnobitrev.c memory space

    t_int BufSize;			// FFT buffer size
	t_int FFTSize;			// Size of FFT
    t_int BufWritePos;		// Where to write in buffer
	void *c_outcent;		// Outlet for the centroid
	void *c_clock;			// Use a clock for outputs... (better than Qelem)
		
} t_centroid;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman;

t_int *centroid_perform(t_int *w);
void centroid_dsp(t_centroid *x, t_signal **sp, short *connect);
void centroid_float(t_centroid *x, double f);
void centroid_int(t_centroid *x, long n);
void centroid_assist(t_centroid *x, void *b, long m, long a, char *s);
void *centroid_new(t_symbol *s, short argc, t_atom *argv);
void centroid_free(t_centroid *x);
void centroid_tick(t_centroid *x);

void main(void) {

	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman = gensym("blackman");

	setup( &centroid_class, centroid_new, (method)centroid_free, (short)sizeof(t_centroid), 0L, A_GIMME, 0);
		
	addmess((method)centroid_dsp, "dsp", A_CANT, 0);
	addmess((method)centroid_assist, "assist", A_CANT, 0);
	addfloat((method)centroid_float);
	addint((method)centroid_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *centroid_perform(t_int *w) {

	t_float *in = (t_float *)(w[1]);
	t_centroid *x = (t_centroid *)(w[2]);
	t_int n = (int)(w[3]);

	t_int *myintin = (t_int *)in;
	t_int *myintBuf1 = (t_int *)(x->Buf1);
	t_int *myintBuf2 = (t_int *)(x->Buf2);
	t_int *myintBufFFT = (t_int *)(x->BufFFT);
	t_int i, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->c_overlap;
	t_float *TmpBuf = x->Buf1;
			
	// Copy input samples into FFT buffer	
	while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
		myintBuf1[x->BufWritePos] = myintin[index];
		x->BufWritePos++;
		index++;
		cpt--;
	}
	
	// When Buffer is full...
	if (x->BufWritePos >= x->BufSize) {
			
		// Save overlapping samples into Buffer 2
		for (i=0; i<x->c_overlap; ++i) 
			myintBuf2[i] = myintBuf1[overlapindex+i];

		maxindex = n - index + x->c_overlap;

		// Copy the rest of incoming samples into Buffer 2
		for (i=x->c_overlap; i<maxindex; ++i) {
			myintBuf2[i] = myintin[index];
			index++;
		}
		
		x->BufWritePos = maxindex;
										
		// Make a copy of Buffer 1 into Buffer FFT for computation outside the perform function
		for (i=0; i<x->BufSize; ++i) 
			myintBufFFT[i] = myintBuf1[i];

		// Go for the FFT outside the perform function with a delay of 0 ms!
		clock_delay(x->c_clock,0);
		
		// Swap buffers
		x->Buf1 = x->Buf2;
		x->Buf2 = TmpBuf;
	}

	return (w+4);
}

void centroid_dsp(t_centroid *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	if (vs > x->BufSize) post(" You need to use a smaller signal vector size...",0);
	else if (connect[0]) dsp_add(centroid_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}

void centroid_float(t_centroid *x, double f) {
	long in = x->c_obj.z_in;
	t_int vs = sys_getblksize();

	if (!in)
		return;
	else {
		x->c_overlap = (int)(f * x->c_Fs/1000.0f); 
		if (x->c_overlap > x->BufSize-vs) {
			post(" You can't overlap so much...",0);
			x->c_overlap = x->BufSize-vs;
		} else if (x->c_overlap < 1)
			x->c_overlap = 0;
	}
}

void centroid_int(t_centroid *x, long n) {
	long in = x->c_obj.z_in;
	t_int vs = sys_getblksize();

	if (!in)
		return;
	else {
		x->c_overlap = n; 
		if (x->c_overlap > x->BufSize-vs) {
			post(" You can't overlap so much...",0);
			x->c_overlap = x->BufSize-vs;
		} else if (x->c_overlap < 1)
			x->c_overlap = 0;
	}
}

void centroid_assist(t_centroid *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID,m,a,1,3,s);
}

void *centroid_new(t_symbol *s, short argc, t_atom *argv) {
	t_int i;
	t_int vs = sys_getblksize();
    t_centroid *x = (t_centroid *)newobject(centroid_class);
    t_float ms2samp;    
    dsp_setup((t_pxobject *)x,2);	
	x->c_clock = clock_new(x,(method)centroid_tick);
	x->c_outcent = floatout((t_centroid *)x);
	x->c_Fs = sys_getsr();
	x->c_centroid = 0.0f;
	x->BufWritePos = 0;
	
	ms2samp = x->c_Fs * 0.001f;

	if (argv[0].a_type == A_LONG) x->BufSize = argv[0].a_w.w_long; // Samples 
	else if (argv[0].a_type == A_FLOAT) x->BufSize = (int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	else x->BufSize = DEFBUFSIZE;
	if (argv[1].a_type == A_LONG) x->c_overlap = argv[1].a_w.w_long; // Samples 
	else if (argv[1].a_type == A_FLOAT) x->c_overlap = (int)(argv[1].a_w.w_float * ms2samp); // Time in ms
	else x->c_overlap = x->BufSize/2;
	if (argv[2].a_w.w_sym == ps_rectangular) x->c_window = Recta;
	else if (argv[2].a_w.w_sym == ps_hanning) x->c_window = Hann;
	else if (argv[2].a_w.w_sym == ps_hamming) x->c_window = Hamm;
	else if (argv[2].a_w.w_sym == ps_blackman) x->c_window = Black;
	else x->c_window = Black;

	x->FFTSize = x->BufSize;

	if (x->BufSize < vs) { 
		x->FFTSize = vs;
		x->BufSize = vs;
	}
	else if ((x->BufSize > vs) && (x->BufSize < 128))  x->FFTSize = 128;
	else if ((x->BufSize > 128) && (x->BufSize < 256)) x->FFTSize = 256;
	else if ((x->BufSize > 256) && (x->BufSize < 512)) x->FFTSize = 512;
	else if ((x->BufSize > 512) && (x->BufSize < 1024)) x->FFTSize = 1024;
	else if ((x->BufSize > 1024) && (x->BufSize < 2048)) x->FFTSize = 2048;
	else if ((x->BufSize > 2048) && (x->BufSize < 4096)) x->FFTSize = 4096;
	else if ((x->BufSize > 8192) && (x->BufSize < 16384)) x->FFTSize = 16384;
	else if ((x->BufSize > 16384) && (x->BufSize < 32768)) x->FFTSize = 32768;
	else if (x->BufSize > 32768) {
		post(" Maximum FFT Size is 65536",0);
		x->FFTSize = 65536;
		x->BufSize = 65536;
	}
		
	// Overlap case
	if (x->c_overlap > x->BufSize-vs) {
		post(" You can't overlap so much...",0);
		x->c_overlap = x->BufSize-vs;
	} else if (x->c_overlap < 1)
		x->c_overlap = 0; 

	// Allocate memory
	x->Buf1 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf2 = t_getbytes(x->BufSize * sizeof(float));
	x->BufFFT = t_getbytes(x->FFTSize * sizeof(float));
	x->WindFFT = t_getbytes(x->FFTSize * sizeof(float));
	x->memFFT = t_getbytes(CMAX * x->FFTSize * sizeof(float)); // memory allocated for fft twiddle
		
	// Compute and store Windows
	if ((x->c_window > Recta) && (x->c_window <= Black)) {
		
		switch (x->c_window) {

			case Hann: 	for (i=0; i<x->FFTSize; ++i)
							x->WindFFT[i] = HANNING_W(i, x->FFTSize);
 						break;
			case Hamm:	for (i=0; i<x->FFTSize; ++i)
							x->WindFFT[i] = HAMMING_W(i, x->FFTSize);
						break;
			case Black: for (i=0; i<x->FFTSize; ++i)
							x->WindFFT[i] = BLACKMAN_W(i, x->FFTSize);
						break;
		}
	}		

    return (x);
}

void  centroid_free(t_centroid *x) {
	if (x->Buf1) t_freebytes(x->Buf1, x->BufSize * sizeof(float));
	if (x->Buf2) t_freebytes(x->Buf2, x->BufSize * sizeof(float));
	if (x->BufFFT) t_freebytes(x->BufFFT, x->FFTSize * sizeof(float));
	if (x->WindFFT) t_freebytes(x->WindFFT, x->FFTSize * sizeof(float));
	if (x->memFFT) t_freebytes(x->memFFT, CMAX * x->FFTSize * sizeof(float));
	freeobject((t_object *)x->c_clock);
	dsp_free((t_pxobject *)x);
}

void centroid_tick(t_centroid *x) {

	t_int halfFFTSize = x->FFTSize/2;
	t_float FsOverFFTSize = x->c_Fs / x->FFTSize;
	t_float sumSpectrum = 0.0f;
	t_float Centroid = 0.0f;
	t_int i;

	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; ++i)
		x->BufFFT[i] = 0.0f;

	// Window the samples
	if ((x->c_window > Recta) && (x->c_window <= Black))
		for (i=0; i<x->FFTSize; ++i)
			x->BufFFT[i] *= x->WindFFT[i];
			
	// FFT
	fftRealfast(x->FFTSize, x->BufFFT, x->memFFT);
		
	// Absolute version
	for (i=2; i<x->BufSize; i+=2) 
		x->BufFFT[i/2] = sqrtf( (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]) );
					
	// Centroid
	for (i=1; i<halfFFTSize; ++i) {
		Centroid += (i * x->BufFFT[i]);
		sumSpectrum += x->BufFFT[i];
	}
	if (sumSpectrum <= 0.0f) x->c_centroid = 0.0f;
	else x->c_centroid = (Centroid * FsOverFFTSize) / sumSpectrum;

    outlet_float(x->c_outcent, x->c_centroid);
	x->c_centroid = 0.0f;
}	
