/*
Copyright (c) 2006.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and Audio Technologies,
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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: mfcc~
DESCRIPTION: Compute Mel Frequency Cepstral Coefficients
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.1: First build--no options yet.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

/*---------------------------------------------------------------------------
Some of this code taken from:
	ETSI STQ WI007 DSR Front-End Feature Extraction Algorithm
	C-language software implementation 
	Version 1.1.3   May, 2003
	www.etsi.org
---------------------------------------------------------------------------*/

#include "version.h"
#include "ext.h"
#include "version.c"

#include "z_dsp.h"
#include "stdlib.h"
#include "math.h"
//#include <fftw3.h>

#define PIx2 6.28318530717958647692

#define	BOOLEAN int
#define	FALSE 0
#ifndef TRUE
	#define	TRUE (!FALSE)
#endif
//#define WAIT_A_KEY while( !getchar() );
//#define PRINTMOD 15
//#define IEEE_LE 0
//#define IEEE_BE 1

typedef struct FFT_Window_tag
{
    int StartingPoint;
    int Length;
    float *Data;
    struct FFT_Window_tag *Next;
}
FFT_Window;                     /* Structure for FFT window (one triangle in
				   the chained list) */

typedef struct _mfcc
{
	t_pxobject c_ob;
	void *c_out0;
	void *c_out1;
	void *c_out2;
	//fftwf_plan c_dftPlan;
	//fftw_plan c_dctPlan;
	float c_fs;
	long c_windowLength;
	long c_windowShift;
	float c_preEmphasis;
	float c_energyFloor_FB;
	float c_energyFloor_logE;
	long c_numMelChannels;
	long c_numCepCoeffs;
	float c_startingFreq;
	float *c_buffer;
	float *c_circBuffer;
	float *c_window;
	float *c_DCTMatrix;
	FFT_Window c_firstWindow;
	float c_logEnergy;
} t_mfcc;

void *mfcc_class;

void mfcc_anything(t_mfcc *x, t_symbol *msg, short argc, t_atom *argv);
void mfcc_bang(t_mfcc *x);
void mfcc_assist(t_mfcc *x, void *b, long m, long a, char *s);
void *mfcc_new(void);
void mfcc_init(t_mfcc *x);
void mfcc_free(t_mfcc *x);
t_int *mfcc_perform(t_int *w);
void mfcc_dsp(t_mfcc *x, t_signal **sp, short *count);
void mfcc_setupFFTs(t_mfcc *x);

// ETSI prototypes
void DCOffsetFilter( float *CircBuff, int BSize, int *BPointer, int nSamples );
void InitializeHamming (float *win, int len);
void Window (float *data, float *win, int len);
void rfft (float *x, int n, int m);
void InitFFTWindows (FFT_Window * FirstWin,
                     float StFreq,
                     float SmplFreq,
                     int FFTLength,
                     int NumChannels);
void ReleaseFFTWindows (FFT_Window *FirstWin );
void ComputeTriangle (FFT_Window * FirstWin);
void MelFilterBank (float *SigFFT, FFT_Window * FirstWin);

float *InitDCTMatrix (int NumCepstralCoeff, int NumChannels);
void DCT (float *Data, float *Mx, int NumCepstralCoeff, int NumChannels);

//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&mfcc_class, (method)mfcc_new, (method)mfcc_free, (short)sizeof(t_mfcc), 0L, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)mfcc_dsp, "dsp", A_CANT, 0);
	addmess((method)mfcc_anything, "anything", A_GIMME, 0);
	addmess((method)mfcc_assist, "assist", A_CANT, 0);
	addbang((method)mfcc_bang);
	
	dsp_initclass();
	
	return 0;
}

//--------------------------------------------------------------------------

void mfcc_anything(t_mfcc *x, t_symbol *msg, short argc, t_atom *argv)
{
}

void mfcc_bang(t_mfcc *x)
{
	t_atom mel_out[(x->c_numMelChannels) * 2];
	t_atom mfcc_out[(x->c_numCepCoeffs) * 2];
	double logen_out;
	int i;
	for(i = 0; i < (x->c_numCepCoeffs); i++){
		SETFLOAT(mfcc_out + (i * 2), i);
		SETFLOAT(mfcc_out + ((i * 2) + 1), x->c_buffer[i + (x->c_numMelChannels)]);
	}
	FFT_Window *w = &x->c_firstWindow;
	for(i = 0; i < x->c_numMelChannels; i++){
		//post("Channel %d: %f %f (StartingPoint: %d, Length: %d)", i, 
		//	(x->c_fs / x->c_windowLength) * ((float)w->StartingPoint + ((float)w->Length / 2)), x->c_buffer[i], w->StartingPoint, w->Length);
		SETFLOAT(mel_out + (i * 2), (x->c_fs / x->c_windowLength) * ((float)w->StartingPoint + ((float)w->Length / 2)));
		SETFLOAT(mel_out + ((i * 2) + 1), x->c_buffer[i]);
		w = w->Next;
	}
	logen_out = (double)x->c_buffer[x->c_numMelChannels + x->c_numCepCoeffs];
	//for(i = 0; i < x->c_numCepCoeffs; i++)
	//	post("Coefficient %d: %f", i, x->c_buffer[i + x->c_numMelChannels]);
	//post("Log Energy: %f", x->c_buffer[x->c_numMelChannels + x->c_numCepCoeffs]);
	outlet_list(x->c_out0, 0L, (short)((x->c_numMelChannels) * 2), mel_out);
	outlet_list(x->c_out1, 0L, (short)((x->c_numCepCoeffs) * 2), mfcc_out);
	outlet_float(x->c_out2, logen_out);
}

//--------------------------------------------------------------------------

void mfcc_assist(t_mfcc *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		switch (a) {	
		case 0:
			sprintf(s,"%ld Mel filterbank channels(list)", x->c_numMelChannels);
			break;
		case 1:
			sprintf(s,"%ld MFCCs (list)", x->c_numCepCoeffs);
			break;
		case 2:
			sprintf(s,"Log Energy (float)");
			break;
		}
	else {
		sprintf(s,"Signal input");
		
	}
}

//--------------------------------------------------------------------------

void *mfcc_new(void)
{
	t_mfcc *x;

	x = (t_mfcc *)newobject(mfcc_class); // create a new instance of this object
	dsp_setup((t_pxobject *)x, 1);
	x->c_out2 = floatout(x);
	x->c_out1 = listout(x);
	x->c_out0 = listout(x);
	//outlet_new((t_pxobject *)x, "signal");
	
	x->c_windowLength = 512;
	x->c_windowShift = 512;
	x->c_preEmphasis = 0.97;
	x->c_energyFloor_FB = expf(-50.f);
	x->c_energyFloor_logE = expf(-50.f);
	x->c_numMelChannels = 23;
	x->c_numCepCoeffs = 13;
	x->c_startingFreq = 64.f;
	x->c_fs = sys_getsr();
	x->c_logEnergy = 0.f;
	
	//x->c_dftPlan = NULL;
	//x->c_dctPlan = NULL;
	//mfcc_setupFFTs(x);
	
	x->c_window = NULL;
	x->c_circBuffer = NULL;
	x->c_buffer = NULL;
	mfcc_init(x);
		
	return(x);
}

void mfcc_init(t_mfcc *x){
	x->c_circBuffer = (float *)realloc(x->c_circBuffer, sizeof(float) * (x->c_windowLength + 2));
	x->c_buffer = (float *)realloc(x->c_buffer, sizeof(float) * (x->c_windowLength + 2));
	x->c_window = (float *)realloc(x->c_window, sizeof(float) * (x->c_windowLength));
	InitializeHamming(x->c_window, (int)x->c_windowLength);
	InitFFTWindows (&x->c_firstWindow, x->c_startingFreq, x->c_fs, x->c_windowLength, x->c_numMelChannels);
	ComputeTriangle (&x->c_firstWindow);
	x->c_DCTMatrix = InitDCTMatrix(x->c_numCepCoeffs, x->c_numMelChannels);
}

void mfcc_free(t_mfcc *x)
{
	dsp_free((t_pxobject *)x);
	//fftwf_destroy_plan(x->c_dftPlan);
	free(x->c_circBuffer);
	free(x->c_DCTMatrix);
	ReleaseFFTWindows(&x->c_firstWindow);
	//fftw_destroy_plan(x->c_dctPlan);
}

void mfcc_dsp(t_mfcc *x, t_signal **sp, short *count)
{
	x->c_fs = (float)sp[0]->s_sr;
	dsp_add(mfcc_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

t_int *mfcc_perform(t_int *w)
{
	t_mfcc *x = (t_mfcc *)w[1];
	t_float *in = (t_float *)w[2];
	int signalVectorLength = (int)w[3];
	
	int i, j, TmpInt, CFBSize, CFBPointer;

	long FrameLength = x->c_windowLength;
	long FrameShift = x->c_windowShift;
	long FFTLength = x->c_windowLength;

    float LogEnergy = x->c_logEnergy;
	  
	float EnergyFloor_FB = x->c_energyFloor_FB;
	float EnergyFloor_logE = x->c_energyFloor_logE;
	float *FloatBuffer = x->c_buffer;
	float *FloatWindow = x->c_window;
	float *pDCTMatrix = x->c_DCTMatrix;
	float *CircFloatBuffer = x->c_circBuffer;
	
    FFT_Window FirstWindow = x->c_firstWindow;
	
    //EnergyFloor_FB = expf(ENERGYFLOOR_FB);
    //EnergyFloor_logE = expf(ENERGYFLOOR_logE);
	
	CFBSize = FrameLength + 2;
	CircFloatBuffer[0] = 0.0;
	CircFloatBuffer[1] = 0.0;
	CFBPointer = 0;

	//ReadWave(fp_in, CircFloatBuffer, CFBSize, CFBPointer+2, FrameLength-FrameShift, (SwapSpecified || InputByteOrder != NativeByteOrder));
	
	DCOffsetFilter( CircFloatBuffer, CFBSize, &CFBPointer, FrameLength-FrameShift );
	
	//while (ReadWave (fp_in, CircFloatBuffer, CFBSize, (CFBPointer+2)%CFBSize, FrameShift, (SwapSpecified || InputByteOrder != NativeByteOrder)))
    //    {
	
	for(j = 0; j < (int)round(signalVectorLength / FrameLength); j++){
		
		for(i = 0; i < FrameLength; i++)
			CircFloatBuffer[i] = in[i + (j * FrameLength)];
	
		//---------------------
		// DC offset removal --
		//---------------------
		DCOffsetFilter( CircFloatBuffer, CFBSize, &CFBPointer, FrameShift );
	
		//--------------------
		// logE computation --
		//--------------------
		LogEnergy = 0.0;
		for (i = 0; i < FrameLength; i++)
			LogEnergy += CircFloatBuffer[(CFBPointer+i+3)%CFBSize] * CircFloatBuffer[(CFBPointer+i+3)%CFBSize];

		if (LogEnergy < EnergyFloor_logE)
			LogEnergy = EnergyFloor_logE;
		else
			LogEnergy = logf(LogEnergy);
		
		//-------------------------------------------------------
		// Pre-emphasis, moving from circular to linear buffer --
		//-------------------------------------------------------
		for (i = 0; i < FrameLength; i++)
			FloatBuffer[i] = CircFloatBuffer[(CFBPointer + i + 3) % CFBSize] - x->c_preEmphasis * CircFloatBuffer[(CFBPointer + i + 2) % CFBSize];
		
		//-------------
		// Windowing --
		//-------------
		Window (FloatBuffer, FloatWindow, (int)FrameLength);

		//-------
		// FFT --
		//-------

		// Zero padding --
		for (i = FrameLength; i < FFTLength; i++)
			FloatBuffer[i] = 0.0;
		
		//x->c_tmpArray = (float *)memcpy(x->c_tmpArray, FloatBuffer, x->c_windowLength * sizeof(float));
		//for(i = 0; i < x->c_windowLength; i++)
		//	x->c_tmpArray[i] = FloatBuffer[i];
		
		// Real valued, in-place split-radix FFT --
		TmpInt = (int) (log10 (FFTLength) / log10 (2));
		rfft (FloatBuffer, FFTLength, TmpInt); //TmpInt = log2(FFTLength)--
		
		// Magnitude spectrum --
		FloatBuffer[0] = fabsf(FloatBuffer[0]);  // DC --
		// pi/(N/2), 2pi/(N/2), ...,(N/2-1)*pi/(N/2) --
		for (i = 1; i < FFTLength / 2; i++)  
			FloatBuffer[i] = sqrtf(FloatBuffer[i] * FloatBuffer[i] + FloatBuffer[FFTLength - i] * FloatBuffer[FFTLength - i]);
		FloatBuffer[FFTLength / 2] = fabsf(FloatBuffer[FFTLength / 2]);  // pi/2 --

		//-----------------
		// Mel filtering --
		//-----------------
		MelFilterBank (FloatBuffer, &FirstWindow);

		//---------------------------------
		// Natural logarithm computation --
		//---------------------------------
		for (i = 0; i < x->c_numMelChannels; i++){
			if (FloatBuffer[i] < EnergyFloor_FB)
				FloatBuffer[i] = EnergyFloor_FB;
			else
				FloatBuffer[i] = logf(FloatBuffer[i]);
		}

		//-----------------------------
		// Discrete Cosine Transform --
		//-----------------------------
		DCT(FloatBuffer, pDCTMatrix, x->c_numCepCoeffs, x->c_numMelChannels);

		//----------------------------------------
		// Append logE after c0 or overwrite c0 --
		//----------------------------------------
		BOOLEAN Noc0 = FALSE;
		FloatBuffer[x->c_numMelChannels + x->c_numCepCoeffs - (Noc0 ? 1:0)] = LogEnergy;
		
	}
	
	return (w + 4);
}

void mfcc_setupFFTs(t_mfcc *x)
{
	/*
	x->c_buffer = (float *)realloc(x->c_buffer, x->c_windowLength * sizeof(float));
	if(x->c_dftPlan){
		fftwf_destroy_plan(x->c_dftPlan);
	}
	x->c_dftPlan = fftwf_plan_r2r_1d(x->c_windowLength, x->c_buffer, x->c_buffer, FFTW_R2HC, FFTW_ESTIMATE);
	*/
	/*
	if(x->c_dctPlan){
		fftw_destroy_plan(x->c_dctPlan);
	}
	x->c_dctPlan = fftw_plan_r2r_1d(x->c_windowSize, x->c_tmpArray, x->c_tmpArray, FFTW_REDFT10, FFTW_ESTIMATE);
	*/
}

/*---------------------------------------------------------------------------
The following code was taken from:
	ETSI STQ WI007 DSR Front-End Feature Extraction Algorithm
	C-language software implementation 
	Version 1.1.3   May, 2003
	www.etsi.org
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 * FUNCTION NAME: DCOffsetFilter
 *
 * PURPOSE:       DC offset removal from speech waveform
 *
 * INPUT:
 *   CircBuff     Pointer to input circular buffer
 *   BSize        Buffer size
 *   BPointer     Pointer to buffer pointer
 *   nSamples     Number of samples to be filtered
 *
 * OUTPUT
 *                Filtered data in the circular buffer
 *                last output sample pointed by the buffer pointer
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void
DCOffsetFilter( float *CircBuff, int BSize, int *BPointer, int nSamples )
{
  int i;

  for ( i=0; i<nSamples; i++ )
    {
      /* y[n]=x[n]-x[n-1]+0.999*y[n-1] */
      CircBuff[(*BPointer+1)%BSize]=CircBuff[(*BPointer+2)%BSize]-
	CircBuff[(*BPointer+1)%BSize]+0.999*CircBuff[*BPointer];
      *BPointer=(*BPointer+1)%BSize;
    }
}


/*---------------------------------------------------------------------------
 * FUNCTION NAME: InitializeHamming
 *
 * PURPOSE:       Initializes Hamming window coefficients
 *
 * INPUT:
 *   win          Pointer to window buffer
 *   len          Window length
 *
 * OUTPUT
 *                Hamming window coefficients stored in window buffer pointed
 *                to by *win*
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
InitializeHamming (float *win, int len)
{
    int i;

    for (i = 0; i < len / 2; i++)
        win[i] = 0.54 - 0.46 * cos (PIx2 * i / (len - 1));
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: Window
 *
 * PURPOSE:       Performs windowing on input speech frame (multiplies input
 *                samples by the corresponding window coefficients)
 *
 * INPUT:
 *   data         Pointer to input speech buffer
 *   win          Pointer to window buffer
 *   len          Window (or frame) length
 *
 * OUTPUT
 *                Windowed speech frame stored at the same place as the
 *                original speech samples (pointed by *data*)
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
Window (float *data, float *win, int len)
{
    long i;

    for (i = 0; i < len / 2; i++)
        data[i] *= win[i];
    for (i = len / 2; i < len; i++)
        data[i] *= win[len - 1 - i];
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: rfft
 *
 * PURPOSE:       Real valued, in-place split-radix FFT
 *
 * INPUT:
 *   x            Pointer to input and output array
 *   n            Length of FFT, must be power of 2
 *
 * OUTPUT         Output order
 *                  Re(0), Re(1), ..., Re(n/2), Im(N/2-1), ..., Im(1)
 *
 * RETURN VALUE
 *   none
 *
 * DESIGN REFERENCE:
 *                IEEE Transactions on Acoustic, Speech, and Signal Processing,
 *                Vol. ASSP-35. No. 6, June 1987, pp. 849-863.
 *
 *                Subroutine adapted from fortran routine pp. 858-859.
 *                Note corrected printing errors on page 859:
 *                    SS1 = SIN(A3) -> should be SS1 = SIN(A);
 *                    CC3 = COS(3)  -> should be CC3 = COS(A3)
 *---------------------------------------------------------------------------*/
void 
rfft (float *x, int n, int m)
{
    int j, i, k, is, id;
    int i0, i1, i2, i3, i4, i5, i6, i7, i8;
    int n2, n4, n8;
    float xt, a0, e, a, a3;
    float t1, t2, t3, t4, t5, t6;
    float cc1, ss1, cc3, ss3;
    float *r0;

    /* Digit reverse counter */

    j = 0;
    r0 = x;

    for (i = 0; i < n - 1; i++)
    {

        if (i < j)
        {
            xt = x[j];
            x[j] = *r0;
            *r0 = xt;
        }
        r0++;

        k = n >> 1;

        while (k <= j)
        {
            j = j - k;
            k >>= 1;
        }
        j += k;
    }

    /* Length two butterflies */
    is = 0;
    id = 4;

    while (is < n - 1)
    {

        for (i0 = is; i0 < n; i0 += id)
        {
            i1 = i0 + 1;
            a0 = x[i0];
            x[i0] += x[i1];
            x[i1] = a0 - x[i1];
        }

        is = (id << 1) - 2;
        id <<= 2;
    }

    /* L shaped butterflies */
    n2 = 2;
    for (k = 1; k < m; k++)
    {
        n2 <<= 1;
        n4 = n2 >> 2;
        n8 = n2 >> 3;
        e = (M_PI * 2) / n2;
        is = 0;
        id = n2 << 1;
        while (is < n)
        {
            for (i = is; i <= n - 1; i += id)
            {
                i1 = i;
                i2 = i1 + n4;
                i3 = i2 + n4;
                i4 = i3 + n4;
                t1 = x[i4] + x[i3];
                x[i4] -= x[i3];
                x[i3] = x[i1] - t1;
                x[i1] += t1;

                if (n4 != 1)
                {
                    i1 += n8;
                    i2 += n8;
                    i3 += n8;
                    i4 += n8;
                    t1 = (x[i3] + x[i4]) / M_SQRT2;
                    t2 = (x[i3] - x[i4]) / M_SQRT2;
                    x[i4] = x[i2] - t1;
                    x[i3] = -x[i2] - t1;
                    x[i2] = x[i1] - t2;
                    x[i1] = x[i1] + t2;
                }
            }
            is = (id << 1) - n2;
            id <<= 2;
        }

        for (j = 1; j < n8; j++)
        {
            a = j * e;
            a3 = 3 * a;
            cc1 = cos (a);
            ss1 = sin (a);
            cc3 = cos (a3);
            ss3 = sin (a3);

            is = 0;
            id = n2 << 1;

            while (is < n)
            {
                for (i = is; i <= n - 1; i += id)
                {
                    i1 = i + j;
                    i2 = i1 + n4;
                    i3 = i2 + n4;
                    i4 = i3 + n4;
                    i5 = i + n4 - j;
                    i6 = i5 + n4;
                    i7 = i6 + n4;
                    i8 = i7 + n4;
                    t1 = x[i3] * cc1 + x[i7] * ss1;
                    t2 = x[i7] * cc1 - x[i3] * ss1;
                    t3 = x[i4] * cc3 + x[i8] * ss3;
                    t4 = x[i8] * cc3 - x[i4] * ss3;
                    t5 = t1 + t3;
                    t6 = t2 + t4;
                    t3 = t1 - t3;
                    t4 = t2 - t4;
                    t2 = x[i6] + t6;
                    x[i3] = t6 - x[i6];
                    x[i8] = t2;
                    t2 = x[i2] - t3;
                    x[i7] = -x[i2] - t3;
                    x[i4] = t2;
                    t1 = x[i1] + t5;
                    x[i6] = x[i1] - t5;
                    x[i1] = t1;
                    t1 = x[i5] + t4;
                    x[i5] = x[i5] - t4;
                    x[i2] = t1;
                }
                is = (id << 1) - n2;
                id <<= 2;
            }
        }
    }
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: InitFFTWindows
 *
 * PURPOSE:       Initializes data structure for FFT windows (mel filter bank).
 *                Computes starting point and length of each window, allocates
 *                memory for window coefficients.
 *
 * INPUT:
 *   FirstWin     Pointer to first FFT window structure
 *   StFreq       Starting frequency of mel filter bank
 *   SmplFreq     Sampling frequency
 *   FFTLength    FFT length
 *   NumChannels  Number of channels
 *
 * OUTPUT
 *                Chained list of FFT window data structures. NOTE FFT window
 *                coefficients are not computed yet.
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
InitFFTWindows (FFT_Window * FirstWin,
                float StFreq,
                float SmplFreq,
                int FFTLength,
                int NumChannels)

{
    int i, TmpInt;
    float freq, start_mel, fs_per_2_mel;
    FFT_Window *p1, *p2;

    /* Constants for calculation */
    start_mel = 2595.0 * log10 (1.0 + (float) StFreq / 700.0);
    fs_per_2_mel = 2595.0 * log10 (1.0 + (SmplFreq / 2) / 700.0);

    p1 = FirstWin;

    for (i = 0; i < NumChannels; i++)
    {
        /* Calculating mel-scaled frequency and the corresponding FFT-bin */
        /* number for the lower edge of the band                          */
        freq = 700 * (pow (10, (start_mel + (float) i / (NumChannels + 1) *
                            (fs_per_2_mel - start_mel)) / 2595.0) - 1.0);
        TmpInt = (int) (FFTLength * freq / SmplFreq + 0.5);
		
        /* Storing */
        p1->StartingPoint = TmpInt;
		
        /* Calculating mel-scaled frequency for the upper edge of the band */
        freq = 700 * (pow (10, (start_mel + (float) (i + 2) / (NumChannels + 1)
				* (fs_per_2_mel - start_mel)) / 2595.0) - 1.0);

        /* Calculating and storing the length of the band in terms of FFT-bins*/
        p1->Length = (int) (FFTLength * freq / SmplFreq + 0.5) - TmpInt + 1;

        /* Allocating memory for the data field */
        p1->Data = (float *) malloc (sizeof (float) * p1->Length);

        /* Continuing with the next data structure or close the last structure
	   with NULL */
        if (i < NumChannels - 1)
        {
            p2 = (FFT_Window *) malloc (sizeof (FFT_Window));
            p1->Next = p2;
            p1 = p2;
        }
        else
            p1->Next = NULL;
    }
    return;
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: ReleaseFFTWindows
 *
 * PURPOSE:       Releases memory allocated for FFT windows
 *
 * INPUT:
 *   FirstWin     Pointer to first FFT window structure
 *
 * OUTPUT
 *   none
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/

void ReleaseFFTWindows (FFT_Window *FirstWin )
{
  FFT_Window *p;

  while ( FirstWin->Next!=NULL )
    {
    p=FirstWin->Next->Next;
    free(FirstWin->Next->Data);
    free(FirstWin->Next);
    FirstWin->Next=p;
    }
  free(FirstWin->Data);
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: ComputeTriangle
 *
 * PURPOSE:       Computes and stores FFT window coefficients (triangle points)
 *                into initialized chained list of FFT window structures
 *
 * INPUT:
 *   FirstWin     Pointer to first FFT window structure
 *
 * OUTPUT
 *                Chained list of FFT window data structures with correct
 *                window coefficients
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
ComputeTriangle (FFT_Window * FirstWin)
{
    FFT_Window *p1;

    int low_part_length, hgh_part_length, TmpInt=0, i, j;

    p1 = FirstWin;
    j = 0;
    while (p1)
    {
        low_part_length = p1->Next ?
	                  p1->Next->StartingPoint - p1->StartingPoint + 1 :
                          TmpInt - p1->StartingPoint + 1;
        hgh_part_length = p1->Length - low_part_length + 1;

        /* Lower frequency part of the triangle */
        for (i = 0; i < low_part_length; i++)
            p1->Data[i] = (float) (i + 1) / low_part_length;

        /* Higher frequency part of the triangle */
        for (i = 1; i < hgh_part_length; i++)
            p1->Data[low_part_length + i - 1] = (float) (hgh_part_length - i) /
	      hgh_part_length;

        /* Store upper edge (for calculating the last triangle) */
        TmpInt = p1->StartingPoint + p1->Length - 1;

        /* Next triangle ... */
        p1 = p1->Next;
    }
    return;
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: MelFilterBank
 *
 * PURPOSE:       Performs mel filtering on FFT magnitude spectrum using the
 *                filter bank defined by a chained list of FFT window
 *                structures
 *
 * INPUT:
 *   SigFFT       Pointer to signal FFT magnitude spectrum
 *   FirstWin     Pointer to the first channel of the filter bank (first
 *                element in the chained list of FFT window data structures)
 *
 * OUTPUT
 *                Filter bank outputs stored at the beginning of input signal
 *                FFT buffer pointed by *SigFFT*
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
MelFilterBank (float *SigFFT, FFT_Window * FirstWin)
{
    FFT_Window *p1;
    float Sum;
    int i, j;

    p1 = FirstWin;
    j = 0;
    while (p1)
    {
        Sum = 0.0;
        for (i = 0; i < p1->Length; i++)
            Sum += SigFFT[p1->StartingPoint + i] * p1->Data[i];
        SigFFT[j] = Sum;
        j++;
        p1 = p1->Next;
    }
    return;
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: InitDCTMatrix
 *
 * PURPOSE:       Initializes matrix for DCT computation (DCT is implemented
 *                as matrix-vector multiplication). The DCT matrix is of size
 *                (NumCepstralCoeff-1)-by-NumChannels. The zeroth cepstral
 *                coefficient is computed separately (needing NumChannels
 *                additions and only one multiplication), so the zeroth row
 *                of DCT matrix corresponds to the first DCT basis vector, the
 *                first one to the second one, and so on up to
 *                NumCepstralCoeff-1.
 *
 * INPUT:
 *   NumCepstralCoeff
 *                Number of cepstral coeffficients
 *   NumChannels  Number of filter bank channels
 *
 * OUTPUT
 *   none
 *
 * RETURN VALUE
 *                Pointer to the initialized DCT matrix
 *---------------------------------------------------------------------------*/
float *
InitDCTMatrix (int NumCepstralCoeff, int NumChannels)
{
    int i, j;
    float *Mx;

    /* Allocating memory for DCT-matrix */
    Mx = (float *) malloc (sizeof (float) * (NumCepstralCoeff - 1) *
			   NumChannels);

    /* Computing matrix entries */
    for (i = 1; i < NumCepstralCoeff; i++)
        for (j = 0; j < NumChannels; j++)
            Mx[(i - 1) * NumChannels + j] = cos (M_PI * (float) i /
						 (float) NumChannels
						 * ((float) j + 0.5));
    return Mx;
}

/*---------------------------------------------------------------------------
 * FUNCTION NAME: DCT
 *
 * PURPOSE:       Computes DCT transformation of filter bank outputs, results
 *                in cepstral coefficients. The DCT transformation is
 *                implemented as matrix-vector multiplication. The zeroth
 *                cepstral coefficient is computed separately and appended.
 *                Final cepstral coefficient order is c1, c2, ...,c12, c0. The
 *                output is stored right after the input values in the memory.
 *                Since the mel filter bank outputs are stored at the beginning
 *                of the FFT magnitude array it shouldn`t cause any problems.
 *                Some memory saving can be done this way.
 *
 * INPUT:
 *   Data         Pointer to input data buffer (filter bank outputs)
 *   Mx           Pointer to DCT matrix
 *   NumCepstralCoeff
 *                Number of cepstral coefficients
 *   NumChannels  Number of filter bank channels
 *
 * OUTPUT
 *                Cepstral coefficients stored after the input filter bank
 *                values pointed to by *Data*
 *
 * RETURN VALUE
 *   none
 *---------------------------------------------------------------------------*/
void 
DCT (float *Data, float *Mx, int NumCepstralCoeff, int NumChannels)
{
    int i, j;

    /* Computing c1..c/NumCepstralCoeff-1/, storing result after the incoming
       data vector */
    for (i = 1; i < NumCepstralCoeff; i++)
    {
        Data[NumChannels + (i - 1)] = 0.0;
        for (j = 0; j < NumChannels; j++)
            Data[NumChannels + (i - 1)] += Data[j] * Mx[(i - 1) * NumChannels + j];
    }

    /* Computing c0, as the last element of output vector */
    Data[NumChannels + NumCepstralCoeff - 1] = 0.0;
    for (i = 0; i < NumChannels; i++)
        Data[NumChannels + NumCepstralCoeff - 1] += Data[i];
    return;
}
