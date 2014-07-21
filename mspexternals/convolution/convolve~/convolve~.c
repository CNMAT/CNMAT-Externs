/*
SVN_REVISION: $LastChangedRevision$
DESCRIPTION: Convolution external, probably more efficient than doing it in Max.
*/


#include "ext.h"
#include "z_dsp.h"
#include "fftnobitrev.h" // No Bit Reversal !
#include <math.h>

#define RES_ID	7023

#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024

#define HANNING_W(i,ac) (sqrtf((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)) // square root of hanning!
#define HAMMING_W(i,ac) (sqrtf(0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f))))
#define BLACKMAN_W(i,ac) (sqrtf(0.42f - 0.5f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.08f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f)))))

void *convolve_class;

enum {Recta = 0, Hann, Hamm, Black};

typedef struct _convolve {

	t_pxobject c_obj;

	t_float c_Fs;			// Sample rate
	t_int c_window;			// Type of window

    t_float *Buf1;			// Sample buffer signal 1
    t_float *Buf2;			// Sample buffer signal 1 (for overlapping reasons)
    t_float *Buf3;			// Sample buffer signal 2
    t_float *Buf4;			// Sample buffer signal 2 (for overlapping reasons)
    t_float *Window;		// Window of BufSize
    t_float *WindFFT;		// Window of FFTSize or same as *Window
    t_float *BufFFT1;		// FFT buffer signal 1
    t_float *BufFFT2;		// FFT buffer signal 2
    t_float *BufIFFT1;		// IFFT buffer 1
    t_float *BufIFFT2;		// IFFT buffer 2 (for overlapping reasons)
    t_float *memFFT;		// fftnobitrev.c memory space

	t_int FFTSize;			// Size of FFT Buffer
	t_int BufSize;			// Size of Sample Buffer
    t_int BufWritePos;		// Where to write in buffer
    t_int BufReadPos;		// Where to read in buffer
	
} t_convolve;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman;

t_int *convolve_perform(t_int *w);
void convolve_dsp(t_convolve *x, t_signal **sp, short *connect);
void convolve_assist(t_convolve *x, void *b, long m, long a, char *s);
void *convolve_new(t_symbol *s, short argc, t_atom *argv);
void convolve_free(t_convolve *x);

void main(void) {
	
	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman = gensym("blackman");

	setup( &convolve_class, convolve_new, (method)convolve_free, (short)sizeof(t_convolve), 0L, A_GIMME, 0);
		
	addmess((method)convolve_dsp, "dsp", A_CANT, 0);
	addmess((method)convolve_assist, "assist", A_CANT, 0);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *convolve_perform(t_int *w) {

	t_float *in1 = (t_float *)(w[1]);
	t_float *in2 = (t_float *)(w[2]);
	t_float *out = (t_float *)(w[3]);
	t_convolve *x = (t_convolve *)(w[4]);
	t_int n = (int)(w[5]);	

	t_int *myintin1 = (t_int *)in1; // use integers to copy blocs faster
	t_int *myintin2 = (t_int *)in2; // was suggested by David Z.
	t_int *myintBuf1 = (t_int *)(x->Buf1);
	t_int *myintBuf2 = (t_int *)(x->Buf2);
	t_int *myintBuf3 = (t_int *)(x->Buf3);
	t_int *myintBuf4 = (t_int *)(x->Buf4);
	t_int *myintBufFFT1 = (t_int *)(x->BufFFT1);
	t_int *myintBufFFT2 = (t_int *)(x->BufFFT2);

	t_int newReadPos;
	t_int i, index = 0, cpt = n, maxindex;
	t_int halfFFTSize = x->FFTSize/2;
	t_int halfBufSize = x->BufSize/2;
	t_float FsOverFFTSize = x->c_Fs / x->FFTSize;
	t_float magn, RealNyquist1, RealNyquist2;

	t_float *TmpBuf1 = x->Buf1;
	t_float *TmpBuf3 = x->Buf3;
	t_float *TmpBufIFFT1 = x->BufIFFT1;
			
	// Copy input samples into FFT buffers	
	while ((x->BufWritePos < x->BufSize) && (cpt > 0)) {
		myintBuf1[x->BufWritePos] = myintin1[index];
		myintBuf3[x->BufWritePos] = myintin2[index];
		x->BufWritePos++;
		index++;
		cpt--;
	}
	
	// When Sample Buffers are full...
	if (x->BufWritePos >= x->BufSize) {
			
		// Save overlapping samples into Buffer 2 and 4
		for (i=0; i<halfBufSize; ++i) {
			myintBuf2[i] = myintBuf1[halfBufSize+i];
			myintBuf4[i] = myintBuf3[halfBufSize+i];
		}
		
		maxindex = n - index + halfBufSize;

		// Copy the rest of incoming samples into Buffer 2 and 4
		for (i=halfBufSize; i<maxindex; ++i) {
			myintBuf2[i] = myintin1[index];
			myintBuf4[i] = myintin2[index];
			index++;
		}
		
		x->BufWritePos = maxindex;
						
		// Make a copy of Buffers into Buffer FFT1 and Buffer FFT2
		for (i=0; i<x->BufSize; ++i) {
			myintBufFFT1[i] = myintBuf1[i];
			myintBufFFT2[i] = myintBuf3[i];
		}
			
		// Zero padding
		for (i=x->BufSize; i<x->FFTSize; ++i) {
			myintBufFFT1[i] = 0;
			myintBufFFT2[i] = 0;
		}
		
		// Window the samples
		if ((x->c_window > Recta) && (x->c_window <= Black))
			for (i=0; i<x->FFTSize; ++i) {
				x->BufFFT1[i] *= x->WindFFT[i];
				x->BufFFT2[i] *= x->WindFFT[i];
			}	

		// FFT
		fftRealfastnbr(x->FFTSize, x->BufFFT1, x->memFFT);
		fftRealfastnbr(x->FFTSize, x->BufFFT2, x->memFFT);

		// Extraction of magnitude of second signal and multiplication in the frequency domaine
		for (i=0; i<x->FFTSize; i+=2) {
			magn = sqrtf( (x->BufFFT2[i] * x->BufFFT2[i]) + (x->BufFFT2[i+1] * x->BufFFT2[i+1]) );

			if (magn > 0.9f) magn = 0.9f;
			else if  (magn < 0.0f) magn = 0.0f;

			x->BufIFFT2[i] = x->BufFFT1[i] * magn; // Real part
			x->BufIFFT2[i+1] = x->BufFFT1[i+1] * magn; // Imag part
		}
		
		// IFFT
		ifftRealfastnbr(x->FFTSize, x->BufIFFT2, x->memFFT);	

		// Window IFFT result at overlapping
		if ((x->c_window > Recta) && (x->c_window <= Black))
			for (i=0; i<x->BufSize; ++i)
				x->BufIFFT2[i] *= x->Window[i];
		
		// Interpolation between IFFT1 and IFFT2
		for (i=halfBufSize; i<x->BufSize; ++i)
			x->BufIFFT1[i] += x->BufIFFT2[i - halfBufSize];
	
		// Swap IFFT buffers
		x->BufIFFT1 = x->BufIFFT2;
		x->BufIFFT2 = TmpBufIFFT1;

		// Swap sample buffers
		x->Buf1 = x->Buf2;
		x->Buf2 = TmpBuf1;
		x->Buf3 = x->Buf4;
		x->Buf4 = TmpBuf3;
	}

	// Output convolved sound
	if ((x->BufReadPos + n) < x->BufSize) {
		for (i=0; i<n; ++i)
			out[i] = x->BufIFFT2[i + x->BufReadPos];
		x->BufReadPos += n;

	} else if ((x->BufReadPos + n) > x->BufSize) { 
		for (i=0; i<(x->BufSize - x->BufReadPos); ++i)
			out[i] = x->BufIFFT2[i + x->BufReadPos];
		newReadPos = halfBufSize + n - i;
		for (i=halfBufSize; i<newReadPos; ++i)
			out[i] = x->BufIFFT1[i];
		x->BufReadPos = newReadPos;

	} else { // (x->BufReadPos + n) == x->BufSize
		for (i=0; i<n; ++i)
			out[i] = x->BufIFFT2[i + x->BufReadPos];
		x->BufReadPos = halfBufSize;
	}
							
	return (w+6);
}

void convolve_dsp(t_convolve *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	if (vs > x->BufSize) post(" You need to use a smaller signal vector size or a bigger FFT size...",0);
	else if (connect[0] && connect[1]) dsp_add(convolve_perform, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, x, sp[0]->s_n);
}

void convolve_assist(t_convolve *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID, m, a, 1, 3, s);
}

void *convolve_new(t_symbol *s, short argc, t_atom *argv) {

	t_int i;
	t_int vs = sys_getblksize(); // Size of signal vector selected in MSP
    t_convolve *x = (t_convolve *)newobject(convolve_class);    
    t_float ms2samp;    
	dsp_setup((t_pxobject *)x,2);
	outlet_new((t_object *)x, "signal");
	x->c_Fs = sys_getsr();
	x->BufWritePos = 0;
	x->BufReadPos = vs;
			
	ms2samp = x->c_Fs * 0.001f;

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
	if (argv[1].a_w.w_sym == ps_rectangular) x->c_window = Recta;
	else if (argv[1].a_w.w_sym == ps_hanning) x->c_window = Hann;
	else if (argv[1].a_w.w_sym == ps_hamming) x->c_window = Hamm;
	else if (argv[1].a_w.w_sym == ps_blackman) x->c_window = Black;
	else x->c_window = Hann;

	// Allocate memory
	x->Buf1 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf2 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf3 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf4 = t_getbytes(x->BufSize * sizeof(float));
	x->BufFFT1 = t_getbytes(x->FFTSize * sizeof(float));
	x->BufFFT2 = t_getbytes(x->FFTSize * sizeof(float));
	x->BufIFFT1 = t_getbytes(x->FFTSize * sizeof(float));
	x->BufIFFT2 = t_getbytes(x->FFTSize * sizeof(float));
	x->memFFT = t_getbytes(CMAX * x->FFTSize * sizeof(float)); // memory allocated for fft twiddle

	x->Window = t_getbytes(x->BufSize * sizeof(float));
	if (x->FFTSize != x->BufSize)	
		x->WindFFT = t_getbytes(x->FFTSize * sizeof(float));
	else x->WindFFT = x->Window;

	// Compute and store Windows
	if ((x->c_window > Recta) && (x->c_window <= Black)) {
		
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

	// Clean IFFT1
	for (i=0; i<x->FFTSize; ++i) {
		x->BufIFFT1[i] = 0.0f;
		x->BufIFFT2[i] = 0.0f;
	}
	
    return (x);
}

void  convolve_free(t_convolve *x) {
	if (x->Buf1) t_freebytes(x->Buf1, x->BufSize * sizeof(float));
	if (x->Buf2) t_freebytes(x->Buf2, x->BufSize * sizeof(float));
	if (x->Buf3) t_freebytes(x->Buf3, x->BufSize * sizeof(float));
	if (x->Buf4) t_freebytes(x->Buf4, x->BufSize * sizeof(float));
	if (x->BufFFT1) t_freebytes(x->BufFFT1, x->FFTSize * sizeof(float));
	if (x->BufFFT2) t_freebytes(x->BufFFT2, x->FFTSize * sizeof(float));
	if (x->BufIFFT1) t_freebytes(x->BufIFFT1, x->FFTSize * sizeof(float));
	if (x->BufIFFT2) t_freebytes(x->BufIFFT2, x->FFTSize * sizeof(float));
	if (x->memFFT) t_freebytes(x->memFFT, CMAX * x->FFTSize * sizeof(float));

	if (x->Window) t_freebytes(x->Window, x->BufSize * sizeof(float));
	if (x->FFTSize != x->BufSize)	
		if (x->WindFFT) t_freebytes(x->WindFFT, x->FFTSize * sizeof(float));

	dsp_free((t_pxobject *)x);
}

