/*
DESCRIPTION: Multiple convolutions with de-synchronised fast real fft
*/

#include "ext.h"
#include "z_dsp.h"
#include "fftnobitrev.h"
#include <math.h>

#define RES_ID	7030

#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024

#define HANNING_W(i,ac) (sqrtf((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)) // square root of hanning
#define HAMMING_W(i,ac) (sqrtf(0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f))))
#define BLACKMAN_W(i,ac) (sqrtf(0.42f - 0.5f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.08f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f)))))

void *mconv_class;

enum {Recta = 0, Hann, Hamm, Black};

typedef struct _mconv {

	t_pxobject m_obj;

	t_int m_window;			// Type of window
	t_int m_numConv;		// Number of Comvolutions

    t_int **w;			
    t_float **Buf1, *bigBuf1;	// Sample buffer signals 1
    t_float **Buf2, *bigBuf2;	// Sample buffer signals 1 (for overlapping reasons)
    t_float **Buf3, *bigBuf3;	// Sample buffer signals 2 
    t_float **Buf4, *bigBuf4;	// Sample buffer signals 2 (for overlapping reasons)
    t_float **BufIFFT1, *bigBufIFFT1;	// IFFT buffer 1
    t_float **BufIFFT2, *bigBufIFFT2;	// IFFT buffer 2 (for overlapping reasons)

    t_float *Window;		// Window of BufSize
    t_float *WindFFT;		// Window of FFTSize or same as *Window
    t_float *BufFFT1;		// FFT buffer signal 1
    t_float *BufFFT2;		// FFT buffer signal 2
    t_float *memFFT;		// fftnobitrev.c memory space

	t_int FFTSize;			// Size of FFT Buffer
	t_int BufSize;			// Size of Sample Buffer
    t_int *BufWritePos;		// Where to write in buffers
    t_int *BufReadPos;		// Where to read in buffers
	
} t_mconv;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman;

t_int *mconv_perform(t_int *w);
void mconv_dsp(t_mconv *x, t_signal **sp, short *connect);
void *mconv_new(t_symbol *s, short argc, t_atom *argv);
void mconv_free(t_mconv *x);

void main(void) {
	
	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman = gensym("blackman");

	setup( &mconv_class, mconv_new, (method)mconv_free, (short)sizeof(t_mconv), 0L, A_GIMME, 0);
		
	addmess((method)mconv_dsp, "dsp", A_CANT, 0);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *mconv_perform(t_int *wAsT_int) {

	t_int **w = (t_int **)wAsT_int;
	t_mconv *x = (t_mconv *)(w[1]);
	t_int n = (t_int)(w[2]);	
	t_float *out;

	t_int *myintin1; // use integers to copy blocs faster
	t_int *myintin2; // was suggested by David Z.
	t_int **myintBuf1 = (t_int **)(x->Buf1);
	t_int **myintBuf2 = (t_int **)(x->Buf2);
	t_int **myintBuf3 = (t_int **)(x->Buf3);
	t_int **myintBuf4 = (t_int **)(x->Buf4);
	t_int *myintBufFFT1 = (t_int *)(x->BufFFT1);
	t_int *myintBufFFT2 = (t_int *)(x->BufFFT2);

	t_int newReadPos;
	t_int num = x->m_numConv; 
	t_int i, k, index, cpt, maxindex;
	t_int halfBufSize = x->BufSize/2;
	t_float magn;

	t_float *TmpBuf1;
	t_float *TmpBuf3;
	t_float *TmpBufIFFT1;
	
	for (k=0; k<num; ++k) {
	
		// In and Out (re-allocate the right inlets and outlet for each convolution) 
		myintin1 = (t_int *)(w[2*k+3]);
		myintin2 = (t_int *)(w[2*k+4]);
    	out = (t_float *)(w[2*num+3+k]);	
		
		// Initialisation
		index = 0;
		cpt = n;
		TmpBuf1 = x->Buf1[k];
		TmpBuf3 = x->Buf3[k];
		TmpBufIFFT1 = x->BufIFFT1[k];
				
		// Copy input samples into FFT buffers	
		while ((x->BufWritePos[k] < x->BufSize) && (cpt > 0)) {
			myintBuf1[k][x->BufWritePos[k]] = myintin1[index];
			myintBuf3[k][x->BufWritePos[k]] = myintin2[index];
			x->BufWritePos[k]++;
			index++;
			cpt--;
		}
	
		// When Sample Buffers are full...
		if (x->BufWritePos[k] >= x->BufSize) {
			
			// Save overlapping samples into Buffer 2 and 4
			for (i=0; i<halfBufSize; ++i) {
				myintBuf2[k][i] = myintBuf1[k][halfBufSize+i];
				myintBuf4[k][i] = myintBuf3[k][halfBufSize+i];
			}
		
			maxindex = n - index + halfBufSize;
	
			// Copy the rest of incoming samples into Buffer 2 and 4
			for (i=halfBufSize; i<maxindex; ++i) {
				myintBuf2[k][i] = myintin1[index];
				myintBuf4[k][i] = myintin2[index];
				index++;
			}
		
			x->BufWritePos[k] = maxindex;
						
			// Make a copy of Buffers into Buffer FFT1 and Buffer FFT2
			for (i=0; i<x->BufSize; ++i) {
				myintBufFFT1[i] = myintBuf1[k][i];
				myintBufFFT2[i] = myintBuf3[k][i];
			}
			
			// Zero padding
			for (i=x->BufSize; i<x->FFTSize; ++i) {
				myintBufFFT1[i] = 0;
				myintBufFFT2[i] = 0;
			}
		
			// Window the samples
			if ((x->m_window > Recta) && (x->m_window <= Black))
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

				x->BufIFFT2[k][i] = x->BufFFT1[i] * magn; // Real part
				x->BufIFFT2[k][i+1] = x->BufFFT1[i+1] * magn; // Imag part
			}

			// IFFT
			ifftRealfastnbr(x->FFTSize, x->BufIFFT2[k], x->memFFT);	

			// Window IFFT result at overlapping
			if ((x->m_window > Recta) && (x->m_window <= Black))
				for (i=0; i<x->BufSize; ++i)
					x->BufIFFT2[k][i] *= x->Window[i];
		
			// Interpolation between IFFT1 and IFFT2
			for (i=halfBufSize; i<x->BufSize; ++i)
				x->BufIFFT1[k][i] += x->BufIFFT2[k][i - halfBufSize];
	
			// Swap IFFT buffers
			x->BufIFFT1[k] = x->BufIFFT2[k];
			x->BufIFFT2[k] = TmpBufIFFT1;

			// Swap sample buffers
			x->Buf1[k] = x->Buf2[k];
			x->Buf2[k] = TmpBuf1;
			x->Buf3[k] = x->Buf4[k];
			x->Buf4[k] = TmpBuf3;
		}

		// Output convolved sound
		if ((x->BufReadPos[k] + n) < x->BufSize) {
			for (i=0; i<n; ++i)
				out[i] = x->BufIFFT2[k][i + x->BufReadPos[k]];
			x->BufReadPos[k] += n;

		} else if ((x->BufReadPos[k] + n) > x->BufSize) { 
			for (i=0; i<(x->BufSize - x->BufReadPos[k]); ++i)
				out[i] = x->BufIFFT2[k][i + x->BufReadPos[k]];
			newReadPos = halfBufSize + n - i;
			for (i=halfBufSize; i<newReadPos; ++i)
				out[i] = x->BufIFFT1[k][i];
			x->BufReadPos[k] = newReadPos;

		} else { // (x->BufReadPos + n) == x->BufSize
			for (i=0; i<n; ++i)
				out[i] = x->BufIFFT2[k][i + x->BufReadPos[k]];
			x->BufReadPos[k] = halfBufSize;
		}
	
	}
								
	return (wAsT_int+3*num+3);
}

void mconv_dsp(t_mconv *x, t_signal **sp, short *connect) {
	t_int vs = sys_getblksize();
	t_int i, num = 3*x->m_numConv; // number of Inlets and Outlets 
	t_int **w = x->w;

	w[0] = (t_int *)x;
	w[1] = (t_int *)sp[0]->s_n;

	for (i=0; i<num; ++i)
		w[i+2] = (t_int *)sp[i]->s_vec;
			
	num += 2; // x and n
	if (vs > x->BufSize) post(" You need to use a smaller signal vector size or a bigger FFT size...",0);
	else dsp_addv(mconv_perform, num, (void **)w);
}

void *mconv_new(t_symbol *s, short argc, t_atom *argv) {

	t_int i, k;
	t_int vs = sys_getblksize(); // Size of signal vector selected in MSP
    t_mconv *x = (t_mconv *)newobject(mconv_class);    
    t_float ms2samp;    
	t_float Fs = sys_getsr();
	t_int maxDelayFFT, num;
			
	ms2samp = Fs * 0.001f; // milli-seconds to samples

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
	if (argv[1].a_w.w_sym == ps_rectangular) x->m_window = Recta;
	else if (argv[1].a_w.w_sym == ps_hanning) x->m_window = Hann;
	else if (argv[1].a_w.w_sym == ps_hamming) x->m_window = Hamm;
	else if (argv[1].a_w.w_sym == ps_blackman) x->m_window = Black;
	else x->m_window = Hann;

	// Look at third argument
	if (argv[1].a_type == A_LONG) x->m_numConv = argv[1].a_w.w_long;
	else if (argv[2].a_type == A_LONG) x->m_numConv = argv[2].a_w.w_long;
	else if (argv[2].a_type == A_FLOAT) x->m_numConv = (int)argv[2].a_w.w_float;
	else x->m_numConv = 1;

	if (x->m_numConv <= 0) {
		x->m_numConv = 1;
		post(" The number of convolutions is 1 or more!",0);
	}
	
	num = x->m_numConv;

	// Allocate memory
	x->bigBuf1 = t_getbytes(num * x->BufSize * sizeof(float));
	x->bigBuf2 = t_getbytes(num * x->BufSize * sizeof(float));
	x->bigBuf3 = t_getbytes(num * x->BufSize * sizeof(float));
	x->bigBuf4 = t_getbytes(num * x->BufSize * sizeof(float));
	x->Buf1 = t_getbytes(num * sizeof(float*));
	x->Buf2 = t_getbytes(num * sizeof(float*));
	x->Buf3 = t_getbytes(num * sizeof(float*));
	x->Buf4 = t_getbytes(num * sizeof(float*));
	
	x->bigBufIFFT1 = t_getbytes(num * x->FFTSize * sizeof(float));
	x->bigBufIFFT2 = t_getbytes(num * x->FFTSize * sizeof(float));
	x->BufIFFT1 = t_getbytes(num * sizeof(float*));
	x->BufIFFT2 = t_getbytes(num * sizeof(float*));
	
	for (k=0; k<num; ++k) {
		x->Buf1[k] = x->bigBuf1 + k * x->BufSize;
		x->Buf2[k] = x->bigBuf2 + k * x->BufSize;
		x->Buf3[k] = x->bigBuf3 + k * x->BufSize;
		x->Buf4[k] = x->bigBuf4 + k * x->BufSize;
		x->BufIFFT1[k] = x->bigBufIFFT1 + k * x->FFTSize;
		x->BufIFFT2[k] = x->bigBufIFFT2 + k * x->FFTSize;
	}
	
	x->BufFFT1 = t_getbytes(x->FFTSize * sizeof(float));
	x->BufFFT2 = t_getbytes(x->FFTSize * sizeof(float));
	x->memFFT = t_getbytes(CMAX * x->FFTSize * sizeof(float)); // memory allocated for fft twiddle
	x->BufWritePos = t_getbytes(num * sizeof(t_int));
	x->BufReadPos = t_getbytes(num * sizeof(t_int));
	x->w = t_getbytes((3 * num + 2) * sizeof(t_int*));
	x->Window = t_getbytes(x->BufSize * sizeof(float));
	
	if (x->FFTSize != x->BufSize) {	
		x->WindFFT = t_getbytes(x->FFTSize * sizeof(float));
		maxDelayFFT = x->FFTSize/2;
	} else { 
		x->WindFFT = x->Window;
		maxDelayFFT = x->FFTSize;
	}
	
	// Create inlets and outlets
	dsp_setup((t_pxobject *)x, 2*num);
	x->m_obj.z_misc = Z_NO_INPLACE; // Necessary when outlets should have different vectors than inlets !!
	for (i=0; i<num; ++i)
		outlet_new((t_object *)x, "signal");

	// De-Synchronise FFT events
	for (k=0; k<num; ++k) {
		x->BufWritePos[k] = k*vs % maxDelayFFT;		// This is the trick to de-synchronise FFTs
		x->BufReadPos[k] = (k+1)*vs % maxDelayFFT;	// And this is the trick to output at the right time
	}	

	// Compute and store Windows
	if ((x->m_window > Recta) && (x->m_window <= Black)) {
		
		switch (x->m_window) {

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
	for (k=0; k<num; ++k) {
		for (i=0; i<x->FFTSize; ++i) {
			x->BufIFFT1[k][i] = 0.0f;
			x->BufIFFT2[k][i] = 0.0f;
		}
	}
	
    return (x);
}

void  mconv_free(t_mconv *x) {

	t_int num = x->m_numConv; 

	if (x->Buf1) t_freebytes(x->Buf1, num * sizeof(float*));
	if (x->Buf2) t_freebytes(x->Buf2, num * sizeof(float*));
	if (x->Buf3) t_freebytes(x->Buf3, num * sizeof(float*));
	if (x->Buf4) t_freebytes(x->Buf4, num * sizeof(float*));
	if (x->bigBuf1) t_freebytes(x->bigBuf1, num * x->BufSize * sizeof(float));
	if (x->bigBuf2) t_freebytes(x->bigBuf2, num * x->BufSize * sizeof(float));
	if (x->bigBuf3) t_freebytes(x->bigBuf3, num * x->BufSize * sizeof(float));
	if (x->bigBuf4) t_freebytes(x->bigBuf4, num * x->BufSize * sizeof(float));

	if (x->BufIFFT1) t_freebytes(x->BufIFFT1, num * sizeof(float*));
	if (x->BufIFFT2) t_freebytes(x->BufIFFT2, num * sizeof(float*));
	if (x->bigBufIFFT1) t_freebytes(x->bigBufIFFT1, num * x->FFTSize * sizeof(float));
	if (x->bigBufIFFT2) t_freebytes(x->bigBufIFFT2, num * x->FFTSize * sizeof(float));

	if (x->BufFFT1) t_freebytes(x->BufFFT1, x->FFTSize * sizeof(float));
	if (x->BufFFT2) t_freebytes(x->BufFFT2, x->FFTSize * sizeof(float));
	if (x->memFFT) t_freebytes(x->memFFT, CMAX * x->FFTSize * sizeof(float));
	if (x->BufWritePos) t_freebytes(x->BufWritePos, num * sizeof(t_int));
	if (x->BufReadPos) t_freebytes(x->BufReadPos, num * sizeof(t_int));
	if (x->w) t_freebytes(x->w, (3 * num + 2) * sizeof(t_int*));
	if (x->Window) t_freebytes(x->Window, x->BufSize * sizeof(float));
	if (x->FFTSize != x->BufSize)	
		if (x->WindFFT) t_freebytes(x->WindFFT, x->FFTSize * sizeof(float));

	dsp_free((t_pxobject *)x);
}

