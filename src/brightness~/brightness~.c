/*
 brightness~ object by Tristan Jehan
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
 
 brightness.c
 
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: brightness~
 DESCRIPTION: Spectral Centroid Measure
 AUTHORS: Tristan Jehan, Adrian Freed,  and Michael Zbyszynski
 COPYRIGHT_YEARS: 1988,89,90-99,2000-08
 DRUPAL_NODE: /patch/xxxx
 SVN_REVISION: $LastChangedRevision: 1916 $
 version 1.3.1: Port to Universal Binary, assist strings, changed free() routine to call dsp_free() *before* freeing memory. - mzed
 version 1.3.2: fixed twiddle bug in fft code - mzed
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
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

//#define ALTIVEC_ATTEMPT // needed for "d_signal.h"
#define RES_ID	7077
#define VERSION "1.3.2"

//#define TWOPI 6.28318530717952646f
#define THREEPI 9.424777960769379f
#define FOURPI 12.56637061435917292f
#define DEFBUFSIZE 1024		// Default signal buffer size
#define MAXPADDING 16		// Maximum FFT zero padding (in # of FFT sizes)
#define MAXDELAY 512		// Maximum initial delay (in # of signal vectors)
#define DEFDELAY 0			// Default initial delay (in # of signal vectors) 

// Window description
#define HANNING_W(i,ac) ((1.0f - cos((i * TWOPI) / (ac - 1.0f))) * 0.5f)
#define HAMMING_W(i,ac) (0.54f - 0.46f * cos((TWOPI * i) / (ac - 1.0f)))
#define BLACKMAN62_W(i,ac) (0.44859f - 0.49364f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.05677f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN70_W(i,ac) (0.42323f - 0.49755f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.07922f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN74_W(i,ac) (0.402217f - 0.49703f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.09892f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.00188 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))
#define BLACKMAN92_W(i,ac) (0.35875f - 0.48829f * cos(TWOPI * ((i - 1.0f)/(ac - 1.0f))) + 0.14128f * cos(FOURPI * ((i - 1.0f)/(ac - 1.0f))) - 0.01168 * cos(THREEPI * ((i - 1.0f)/(ac - 1.0f))))
#define DEFWIN Blackman70		// Default window

enum {Recta=0, Hann, Hamm, Blackman62, Blackman70, Blackman74, Blackman92};

void *brightness_class;

typedef struct _brightness {

	t_pxobject x_obj;

	t_float x_Fs;			// Sample rate
	t_int x_overlap;		// Number of overlaping samples
	t_int x_hop;			// Number of non-overlaping samples
	t_int x_window;			// Type of window	
	char x_winName[32];		// Window name	
	t_float x_brightness;	// Current brightness
	t_int x_delay;			// Vector size delay before to start feeding the buffer
	t_int x_counter;		// Counter that goes with the vector size delay

    t_int *Buf1;			// buffer 1 : Use buffers of integers to copy faster
    t_int *Buf2;			// buffer 2
    t_float *BufFFT;		// FFT buffer
    t_float *WindFFT;		// Window of FFTSize
    t_float *memFFT;		// fft.c memory space

    t_int BufSize;			// FFT buffer size
	t_int FFTSize;			// Size of FFT
	UInt32 x_FFTSizeOver2;	// Size of FFT/2 (UInt32 in G4 FFT)
    t_int BufWritePos;		// Where to write in buffer
	void *x_outcent;		// Outlet for the brightness
	void *x_clock;			// Use a clock for outputs... (better than Qelem)
	
	UInt32 x_log2n;
    COMPLEX_SPLIT x_A;
	FFTSetup x_setup;

#ifdef __ALTIVEC__ // Additional stuff for managing the G4-optimized FFT by Apple
#pragma altivec_model on

#pragma altivec_model off
#endif
		
} t_brightness;

t_symbol *ps_rectangular, *ps_hanning, *ps_hamming, *ps_blackman62, *ps_blackman70, *ps_blackman74, *ps_blackman92;

t_int *brightness_perform(t_int *w);
void brightness_dsp(t_brightness *x, t_signal **sp, short *connect);
void brightness_float(t_brightness *x, double f);
void brightness_int(t_brightness *x, long n);
void brightness_assist(t_brightness *x, void *b, long m, long a, char *s);
void readBufSize(t_brightness *x, t_atom *argv);
void readx_overlap(t_brightness *x, t_atom *argv);
void readFFTSize(t_brightness *x, t_atom *argv);
void readx_window(t_brightness *x, t_atom *argv);
void readx_delay(t_brightness *x, t_atom *argv);
void *brightness_new(t_symbol *s, short argc, t_atom *argv);
void brightness_free(t_brightness *x);
void brightness_tick(t_brightness *x);

#ifdef __ALTIVEC__
void brightness_tick_G4(t_brightness *x);
long log2max(long n);
#endif


void main(void) {

    post("Brightness~ object version " VERSION " by Tristan Jehan  (Media Laboratory), Adrian Freed, and Michael Zbyszynski");
    post("copyright © 2001 Massachusetts Institute of Technology");
	post("copyright © 2008 UC Regents");
    post("");

	ps_rectangular = gensym("rectangular");
	ps_hanning = gensym("hanning");
	ps_hamming = gensym("hamming");
	ps_blackman62 = gensym("blackman62");
	ps_blackman70 = gensym("blackman70");
	ps_blackman74 = gensym("blackman74");
	ps_blackman92 = gensym("blackman92");

	setup((Messlist **)&brightness_class, (method)brightness_new, (method)brightness_free, (short)sizeof(t_brightness), 0L, A_GIMME, 0);
		
	addmess((method)brightness_dsp, "dsp", A_CANT, 0);
	addmess((method)brightness_assist, "assist", A_CANT, 0);
	addfloat((method)brightness_float);
	addint((method)brightness_int);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *brightness_perform(t_int *w) {

	t_float *in = (t_float *)(w[1]);
	t_brightness *x = (t_brightness *)(w[2]);
	t_int n = (int)(w[3]);

	t_int *myintin = (t_int *)in; 				// Copy integers rather than floats -> faster
	t_int *myintBufFFT = (t_int *)(x->BufFFT);	// We assume sizeof(float) == sizeof(int) though
	t_int i, index = 0, cpt = n, maxindex;
	t_int overlapindex = x->BufSize - x->x_overlap;
	t_int *TmpBuf = x->Buf1;

	if (x->x_obj.z_disabled) // Save useless computation
		goto out;
			
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

out:	
	return (w+4);
}

void brightness_dsp(t_brightness *x, t_signal **sp, short *connect) {
	int vs = sys_getblksize();

	// Initializing the delay counter
	x->x_counter = x->x_delay;

	if (vs > x->BufSize) {
		post("Brightness~: You need to use a smaller signal vector size...");
	} else if (connect[0]) {
			dsp_add(brightness_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
	}
}

void brightness_float(t_brightness *x, double f) {
	int n = (t_int)(f * x->x_Fs/1000.0f); 
	brightness_int(x, n);
}

void brightness_int(t_brightness *x, long n) {
	t_int vs = sys_getblksize();

	x->x_hop = n; 
	if (x->x_hop < vs) {
		post("Brightness~: You can't overlap so much...");
		x->x_hop = vs;
	} else if (x->x_hop > x->BufSize) {
		x->x_hop = x->BufSize;
	}
	x->x_overlap = x->BufSize - x->x_hop;	
}

//Assist strings

void brightness_assist(t_brightness *x, void *b, long m, long a, char *s) 
{
       if (m == ASSIST_INLET) {
               sprintf(s, "(signal) input, (float/int) hop size");
			   }
       else {
			sprintf(s,"(float) brightness");
			}

	//assist_string(RES_ID,m,a,1,2,s);
}

void readBufSize(t_brightness *x, t_atom *argv) {
    t_float ms2samp = x->x_Fs * 0.001f;
    
	if (argv[0].a_type == A_LONG) {
		x->BufSize = argv[0].a_w.w_long; // Samples
	} else if (argv[0].a_type == A_FLOAT) {
		x->BufSize = (t_int)(argv[0].a_w.w_float * ms2samp); // Time in ms
	} else {
		x->BufSize = DEFBUFSIZE;
	}
}

void readx_overlap(t_brightness *x, t_atom *argv) {
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

void readFFTSize(t_brightness *x, t_atom *argv) {
    t_float ms2samp = x->x_Fs * 0.001f;
    
	if (argv[2].a_type == A_LONG) {
		x->FFTSize = argv[2].a_w.w_long; // Samples
	} else if (argv[2].a_type == A_FLOAT) {
		x->FFTSize = (t_int)(argv[2].a_w.w_float * ms2samp); // Time in ms
	} else {
		x->FFTSize = x->BufSize;
	}
}

void readx_window(t_brightness *x, t_atom *argv) {
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

void readx_delay(t_brightness *x, t_atom *argv) {
    
	if ((argv[4].a_type == A_LONG) && (argv[4].a_w.w_long >= 0) && (argv[4].a_w.w_long < MAXDELAY)) {
		x->x_delay = argv[4].a_w.w_long;
	} else if ((argv[4].a_type == A_FLOAT) && (argv[4].a_w.w_float >= 0) && (argv[4].a_w.w_float < MAXDELAY)) {
		x->x_delay = (t_int)(argv[4].a_w.w_float);
	} else {
		post("Brightness~: 'delay' argument may be out of range... Choosing default...");
		x->x_delay = DEFDELAY;
	}
}

void *brightness_new(t_symbol *s, short argc, t_atom *argv) {
	t_int i;
	t_int vs = sys_getblksize(); // get vector size
    t_brightness *x = (t_brightness *)newobject(brightness_class);
    dsp_setup((t_pxobject *)x,1); // one inlet		
	x->x_outcent = floatout((t_brightness *)x); // one outlet
	x->x_Fs = sys_getsr();
	x->x_brightness = 0.0f;
	x->BufWritePos = 0;
		
	switch (argc) { // Read arguments
		case 0: 
			x->BufSize = DEFBUFSIZE;
			x->x_overlap = x->BufSize/2;
			x->x_hop = x->BufSize/2;
			x->FFTSize = DEFBUFSIZE;
			x->x_window = DEFWIN;
			x->x_delay = 0;
			break;
		case 1:
			readBufSize(x,argv);
			x->x_overlap = x->BufSize/2;
			x->x_hop = x->BufSize/2;
			x->FFTSize = x->BufSize;
			x->x_window = DEFWIN;
			x->x_delay = 0;
			break;
		case 2:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			x->FFTSize = x->BufSize;
			x->x_window = DEFWIN;
			x->x_delay = 0;
			break;
		case 3:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			readFFTSize(x,argv);
			x->x_window = DEFWIN;
			x->x_delay = 0;
			break;
		case 4:
			readBufSize(x,argv);
			readx_overlap(x,argv);		
			readFFTSize(x,argv);
			readx_window(x,argv);
			x->x_delay = 0;
			break;
		default:
			readBufSize(x,argv);
			readx_overlap(x,argv);			
			readFFTSize(x,argv);
			readx_window(x,argv);
			readx_delay(x,argv);
	}		
	
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
			strcpy(x->x_winName,"blackman70");
	}
	
	if (x->BufSize < vs) { 
		post("Brightness~: Buffer size is smaller than the vector size, %d",vs);
		x->BufSize = vs;
	} else if (x->BufSize > 65536) {
		post("Brightness~: Maximum FFT size is 65536 samples");
		x->BufSize = 65536;
	}
		
	if (x->FFTSize < x->BufSize) {
		post("Brightness~: FFT size is at least the buffer size, %d",x->BufSize);
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
		post("Brightness~: Maximum FFT size is 65536 samples");
		x->FFTSize = 65536;
	}
	
	// Overlap case
	if (x->x_overlap > x->BufSize-vs) {
		post("Brightness~: You can't overlap so much...");
		x->x_overlap = x->BufSize-vs;
	} else if (x->x_overlap < 1)
		x->x_overlap = 0; 

	x->x_hop = x->BufSize - x->x_overlap;
	x->x_FFTSizeOver2 = x->FFTSize/2;		

	post("--- Brightness~ ---");	
	post("	Buffer size = %d",x->BufSize);
	post("	Hop size = %d",x->x_hop);
	post("	FFT size = %d",x->FFTSize);
	post("	Window type = %s",x->x_winName);
	post("	Initial delay = %d",x->x_delay);

	// Here comes the choice for altivec optimization or not...
	if (sys_optimize()) { // note that we DON'T divide the vector size by four here

#ifdef __ALTIVEC__ // More code and a new ptr so that x->BufFFT is vector aligned.
#pragma altivec_model on 
		x->x_clock = clock_new(x,(method)brightness_tick_G4); // Call altivec-optimized tick function
		post("	Using G4-optimized FFT");	
		// Allocate some memory for the altivec FFT
		x->x_A.realp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_A.imagp = t_getbytes(x->x_FFTSizeOver2 * sizeof(t_float));
		x->x_log2n = log2max(x->FFTSize);
      	x->x_setup = create_fftsetup (x->x_log2n, 0);
#pragma altivec_model off
#else
		error("  No G4 optimization available");
#endif

	} else { // Normal tick function
		x->x_clock = clock_new(x,(method)brightness_tick);
		x->memFFT = (t_float*) NewPtr(CMAX * x->FFTSize * sizeof(t_float)); // memory allocated for normal fft twiddle
	}
	post("");

	// Allocate more memory
	x->BufFFT = (t_float*) NewPtr(x->FFTSize * sizeof(t_float));
	x->Buf1 = (t_int*) NewPtr(x->BufSize * sizeof(t_float)); // Careful these are pointers to integers but the content is floats
	x->Buf2 = (t_int*) NewPtr(x->BufSize * sizeof(t_float));
	x->WindFFT = (t_float*) NewPtr(x->BufSize * sizeof(t_float));
		
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
	
    return (x);
}

void  brightness_free(t_brightness *x) {

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

	if (x->BufFFT != NULL) DisposePtr((char *) x->BufFFT);
	if (x->Buf1 != NULL) DisposePtr((char *) x->Buf1);
	if (x->Buf2 != NULL) DisposePtr((char *) x->Buf2);
	if (x->WindFFT != NULL) DisposePtr((char *) x->WindFFT);
	if (x->x_clock != NULL) freeobject((t_object *)x->x_clock);
	
}

void brightness_tick(t_brightness *x) {

	t_float FsOverFFTSize = x->x_Fs/x->FFTSize; // Keep it here since x_Fs may change
	t_float sumSpectrum = 0.0f;
	t_float numerator = 0.0f;
	t_int i;

	// Zero padding
	for (i=x->BufSize; i<x->FFTSize; i++)
		x->BufFFT[i] = 0.0f;

	// Window the samples
	if (x->x_window != Recta)
		for (i=0; i<x->BufSize; ++i)
			x->BufFFT[i] *= x->WindFFT[i];
			
	// FFT
	fftRealfast(x->FFTSize, x->BufFFT, x->memFFT);
		
	// Absolute version
	for (i=2; i<x->FFTSize; i+=2) 
		x->BufFFT[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);
					
	// brightness
	for (i=1; i<x->x_FFTSizeOver2; i++) {
		numerator += (i * x->BufFFT[i]);
		sumSpectrum += x->BufFFT[i];
	}
	if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
	else x->x_brightness = (numerator * FsOverFFTSize) / sumSpectrum;

    outlet_float(x->x_outcent, x->x_brightness);
}

void brightness_tick_G4(t_brightness *x) {

	t_float FsOverFFTSize = x->x_Fs/x->FFTSize;
	t_float sumSpectrum = 0.0f;
	t_float numerator = 0.0f;
	t_int i;

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
	for (i=2; i<x->FFTSize; i+=2) 
		x->BufFFT[i/2] = (x->BufFFT[i] * x->BufFFT[i]) + (x->BufFFT[i+1] * x->BufFFT[i+1]);
					
	// brightness
	for (i=1; i<x->x_FFTSizeOver2; i++) {
		numerator += (i * x->BufFFT[i]);
		sumSpectrum += x->BufFFT[i];
	}
	if (sumSpectrum <= 0.0f) x->x_brightness = 0.0f;
	else x->x_brightness = (numerator * FsOverFFTSize) / sumSpectrum;

    outlet_float(x->x_outcent, x->x_brightness);
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
