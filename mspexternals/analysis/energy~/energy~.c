#include "ext.h"
#include "z_dsp.h"
#include "fftnobitrev.h"
#include <math.h>

#define RES_ID	7011

#define TWOPI 6.28318530717952646f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024

#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN_W(i,ac) (0.42f - 0.5f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.08f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))

void *energy_class;

enum {Recta = 0, Hann, Hamm, Black, Time};

typedef struct _energy {

	t_pxobject e_obj;

	t_float e_Fs;			// Sample rate

	t_int e_overlap;		// Number of overlaping samples
	t_int e_window;			// Type of window
	
	t_float e_Energy;		// Current Energy
	t_float e_logEnergy;	// Current Log of Energy

    t_float *Buf1;			// Sample buffer 1
    t_float *Buf2;			// Sample buffer 2
    t_float *BufFFT;		// FFT buffer
    t_float *WindFFT;		// Window of FFTSize
    t_float *memFFT;		// fftnobitrev.c memory space
    t_int BufSize;			// Sample buffer size
	t_int FFTSize;			// Size of FFT
    t_int BufWritePos;		// Where to write in buffer 1

	void *e_outener;		// Outlet for the Energy
	void *e_outlogener;		// Outlet for the Log of Energy
	void *e_clock;			// Use a clock for outputs... (better than Qelem)
		
} t_energy;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman, *ps_time;

t_int *energy_perform(t_int *w);
void energy_dsp(t_energy *x, t_signal **sp, short *connect);
void energy_float(t_energy *x, double f);
void energy_int(t_energy *x, long n);
void energy_assist(t_energy *x, void *b, long m, long a, char *s);
void *energy_new(t_symbol *s, short argc, t_atom *argv);
void energy_free(t_energy *x);
void energy_tick(t_energy *x);

void main(void) {

	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman = gensym("blackman");
	ps_time = gensym("time");

	setup( &energy_class, energy_new, (method)energy_free, (short)sizeof(t_energy), 0L, A_GIMME, 0);
		
	addmess((method)energy_dsp, "dsp", A_CANT, 0);
	addmess((method)energy_assist, "assist", A_CANT, 0);
	addfloat((method)energy_float);
	addint((method)energy_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *energy_perform(t_int *w) {
	t_float *in = (t_float *)(w[1]);
	t_energy *x = (t_energy *)(w[2]);
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

void energy_dsp(t_energy *x, t_signal **sp, short *connect) {
	t_int vs = sys_getblksize();

	if (vs > x->BufSize) post(" You need to use a smaller signal vector size...",0);
	else if (connect[0]) dsp_add(energy_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}

void energy_float(t_energy *x, double f) { // Time
	long in = x->e_obj.z_in;
	t_int vs = sys_getblksize();

	if (!in)
		return;
	else {
		x->e_overlap = (t_int)(f * x->e_Fs * 0.001f); 
		if (x->e_overlap > x->BufSize-vs) {
			post(" You can't overlap so much...",0);
			x->e_overlap = x->BufSize-vs;
		} else if (x->e_overlap < 1)
			x->e_overlap = 0;
	}
}

void energy_int(t_energy *x, long n) { // Samples
	long in = x->e_obj.z_in;
	t_int vs = sys_getblksize();

	if (!in)
		return;
	else {
		x->e_overlap = n; 
		if (x->e_overlap > x->BufSize-vs) {
			post(" You can't overlap so much...",0);
			x->e_overlap = x->BufSize-vs;
		} else if (x->e_overlap < 1)
			x->e_overlap = 0;
	}
}

void energy_assist(t_energy *x, void *b, long m, long a, char *s) {
	assist_string(RES_ID,m,a,1,3,s);
}

void *energy_new(t_symbol *s, short argc, t_atom *argv) {

    t_int i;
	t_int vs = sys_getblksize();
    t_energy *x = (t_energy *)newobject(energy_class);
    t_float ms2samp;    
	dsp_setup((t_pxobject *)x,2);
	x->e_clock = clock_new(x,(method)energy_tick);	
	x->e_outlogener = floatout((t_energy *)x);
	x->e_outener = floatout((t_energy *)x);
	x->e_Fs = sys_getsr();	
	x->e_Energy = 0.0f;
	x->e_logEnergy = 0.0f;
	x->BufWritePos = 0;
	
	ms2samp = x->e_Fs * 0.001f;
	
	if (argv[0].a_type == A_LONG) x->BufSize = argv[0].a_w.w_long; // Samples
	else if (argv[0].a_type == A_FLOAT) x->BufSize = (int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	else x->BufSize = DEFBUFSIZE;
	if (argv[1].a_type == A_LONG) x->e_overlap = argv[1].a_w.w_long; // Samples
	else if (argv[1].a_type == A_FLOAT) x->e_overlap = (int)(argv[1].a_w.w_float * ms2samp); // Time in ms
	else x->e_overlap = x->BufSize/2;
	if (argv[2].a_w.w_sym == ps_rectangular) x->e_window = Recta;
	else if (argv[2].a_w.w_sym == ps_hanning) x->e_window = Hann;
	else if (argv[2].a_w.w_sym == ps_hamming) x->e_window = Hamm;
	else if (argv[2].a_w.w_sym == ps_blackman) x->e_window = Black;
	else if (argv[2].a_w.w_sym == ps_time) x->e_window = Time;
	else x->e_window = Black;

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
	else if ((x->BufSize > 4096) && (x->BufSize < 8192)) x->FFTSize = 8192;
	else if ((x->BufSize > 8192) && (x->BufSize < 16384)) x->FFTSize = 16384;
	else if ((x->BufSize > 16384) && (x->BufSize < 32768)) x->FFTSize = 32768;
	else if (x->BufSize > 32768) {
		post(" Maximum FFT Size is 65536",0);
		x->FFTSize = 65536;
		x->BufSize = 65536;
	}
		
	// Overlap case
	if (x->e_overlap > x->BufSize-vs) {
		post(" You can't overlap so much...",0);
		x->e_overlap = x->BufSize-vs;
	} else if (x->e_overlap < 1)
		x->e_overlap = 0; 

	// Allocate memory
	x->Buf1 = t_getbytes(x->BufSize * sizeof(float));
	x->Buf2 = t_getbytes(x->BufSize * sizeof(float));
	x->BufFFT = t_getbytes(x->FFTSize * sizeof(float));
	x->WindFFT = t_getbytes(x->FFTSize * sizeof(float));
	x->memFFT = t_getbytes(CMAX * x->FFTSize * sizeof(float)); // memory allocated for fft twiddle

	// Compute and store Windows
	if ((x->e_window > Recta) && (x->e_window <= Black)) {
		
		switch (x->e_window) {

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

void  energy_free(t_energy *x) {
	if (x->Buf1) t_freebytes(x->Buf1, x->BufSize * sizeof(float));
	if (x->Buf2) t_freebytes(x->Buf2, x->BufSize * sizeof(float));
	if (x->BufFFT) t_freebytes(x->BufFFT, x->FFTSize * sizeof(float));
	if (x->WindFFT) t_freebytes(x->WindFFT, x->FFTSize * sizeof(float));
	if (x->memFFT) t_freebytes(x->memFFT, CMAX * x->FFTSize * sizeof(float));
	freeobject((t_object *)x->e_clock);
	dsp_free((t_pxobject *)x);
}

void energy_tick(t_energy *x) {

	t_int halfFFTSize = x->FFTSize/2;
	t_float Energy = 0.0f;
	t_int i;
		
	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; ++i)
		x->BufFFT[i] = 0.0f;

	if ((x->e_window >= Recta) && (x->e_window < Time)) { // Frequency Domain
		
		// Window the samples
		if (x->e_window != Recta)		
			for (i=0; i<x->FFTSize; ++i)
				x->BufFFT[i] *= x->WindFFT[i];
			
		// FFT
		fftRealfastnbr(x->FFTSize, x->BufFFT, x->memFFT);
							
		// Absolute version
		for (i=2; i<x->BufSize; i+=2) 
			x->BufFFT[i/2] = sqrtf( (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]) );

		// Energy
		for (i=1; i<halfFFTSize; ++i) {
			Energy += (x->BufFFT[i] * x->BufFFT[i]);
		}
		x->e_Energy = Energy / (halfFFTSize-1);

	} else { // Time Domain
		for (i=0; i<x->BufSize; ++i) {
			Energy += sqrtf(x->BufFFT[i] * x->BufFFT[i]);
		}
		x->e_Energy = Energy / x->BufSize;
	}
		
	if (x->e_Energy > 0.0f) x->e_logEnergy = log(x->e_Energy);
	else (x->e_logEnergy = 0.0f);

    outlet_float(x->e_outener, x->e_Energy);
    outlet_float(x->e_outlogener, x->e_logEnergy);
	x->e_Energy = 0.0f;
	x->e_logEnergy = 0.0f;
}	
