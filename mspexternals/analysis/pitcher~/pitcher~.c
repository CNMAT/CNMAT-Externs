/*

Copyright (c) 1999, 2000,01,02,03,04,05  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Brian K. Vogel, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Some of the MAX/MSP setup code is from Matt
Wright's pitcher~ external.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
     
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: pitcher~
DESCRIPTION: Perform real-time pitch scaling/shifting on a monophonic pitched signal.
AUTHORS: Brian Vogel
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
VERSION 0.1: Brian's initial version 7/19/99 
VERSION 0.6: 031215 Matt+MZed, compiles for OSX
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

     */


/* The total delay (latency) for this external is (x->minimumPitchSamps + 
  * x->outputDelay) samples. The delay of the (external) pitch detectro should
  * ideally be x->outputDelay samples. If the pitch detector delay is more than
  * this, a delay line should be added to the audio input of this external to
  * compensate.
  */

#define PITCHER_VERSION "0.6"

/* The number of elements in the circular array. */
/* The size of this buffer will not affect latency. It should be chosen according
  * to RING_BUFFER_SIZE > OUTPUT_BUFFER_DELAY. Larger values will
  * only result in more memory usage.
  */
// This needs to be an even number!
#define RING_BUFFER_SIZE 2000

/* Output delay (latency). This is the number of samples that the current
  * output sample lags the current input sample. This should be set to at least
  * 1-2 times the lowest period expected to be encountered.
  */
#define OUTPUT_BUFFER_DELAY 1000


/* Minimum pitch to allow
  */
#define MINIMUM_PITCH 75.0


/* use this pitch when the input pitch falls below range or is unknown (a somewhat
  * arbitrary choice).
  * This does not necessarily need to be the same as MINIMUM_PITCH.
  */
#define DEFAULT_PITCH MINIMUM_PITCH


#include <math.h>

#include "ext.h"
#include "z_dsp.h"

void *pitcher_class;

typedef struct _pitcher
{
	/* Max stuff */
    t_pxobject x_obj;
    
    ////////////////////////////////////////////////////////
    // Input signal ring buffer
    //
    /* The input ring buffer (circular array). This buffer is an array
      *  of floats.
      */
    float *inputRingBuf;
    
    /* This contains the element corresponding to the current input
      * sample. For each call to perfom(), the MSP vector size many
      * input samples are processed and this gets incremented the
      * MSP vector size many times.
      */
    int inputRingBufWritePos;
    
    ////////////////////////////////////////////////////////////
    // Output signal ring buffer
    //
    /* The output ring buffer (circular array). This buffer is an array
      *  of floats.
      */
    float *outputRingBuf;

    
    /* This contains the element in the output ring buffer corresponding to
      * to the current synthesis pitch marker, about which OLA (OverLap Add)
      * is being performed. This variable gets incremented by the current
      * synthesis period when necessary. This parameter is called <outptr> in
      * Keith Lent's paper.
      */
    int outputRingBufPitchMarkerPos;
    
    /* The ring buffer size (number of elements). Both the input and output
      * ring buffers are the same size. The choice of size is somewhat arbitrary
      * but must be chosen large enough that a few periods of the waveform
      * can fit in it.
      */
    int ringBufSize; // This will be set to RING_BUFFER_SIZE in initialization.
    
    // Output buffer delay, in samples
    int outputDelay;
    /* Current element to read from in output ring buffer.
      *  This element is obtained by subtracting OUTPUT_BUFFER_DELAY from
      *  inputRingBufWritePos.
      */
    int readPos;
    
    /////////////////////////////////////////////////////////////////
    // Other parameters
    
    /* Current period length (in samples) of input signal. This gets updated once
      * per input signal period.
      */
    int inputPeriodLength; 
    
    /* The number of remaining samples to process before upding <inputPeriodLength>.
      * <inputPeriodLength> is updated when <samplesLeftInPeriod> is 0.
      */
    int samplesLeftInPeriod;

    /* Output signal pitch = input signal pitch * pitchScaleFactor.
      */
    float pitchScaleFactor;  

    int isUnvoiced;  // 1 if unvoice (no pitch), else set to 0.
  
    float minimumPitch;  // Lowest allowable pitch from pitch detector. Anything lower is a "don't know."
  
    int minimumPitchSamps; // This is minimumPitch converted to samples.
  
    int sampleRate; // sampling rate.
  
} t_pitcher;


void main(void);
void pitcher_assist(t_pitcher *x, void *b, long m, long a, char *s);
void *pitcher_new(Symbol *s, short argc, Atom *argv);
void ResizeBufferAndAtomList(t_pitcher *x);
void pitcher_free(t_pitcher *x);
void pitcher_dsp(t_pitcher *x, t_signal **sp, short *count);
t_int *pitcher_perform(t_int *w);


void main(void) {
	post("pitcher~ version " PITCHER_VERSION " by Brian K. Vogel");
	post("Copyright © 1999 Regents of the University of California.  ");

    setup((t_messlist **)&pitcher_class, (method)pitcher_new, (method)pitcher_free, (short)sizeof(t_pitcher),
          0L, A_GIMME, 0);
    addmess((method)pitcher_dsp, "dsp", A_CANT, 0);
    addmess((method)pitcher_assist,"assist",A_CANT,0);
    dsp_initclass();
    rescopy('STR#',3241);
}

void pitcher_assist(t_pitcher *x, void *b, long m, long a, char *s) {
	assist_string(3241,m,a,1,2,s);
}

void *pitcher_new(Symbol *s, short argc, Atom *argv) {
	/* Lowest pitch to allow from the pitch detector. Lower pitches are interpreted
	  * as a "don't know." A "don't know" causes the input signal to pass through
	  * this external unmodified.
	  */
	float min_pitch;
	/* approx. input-output delay, in samples.
	  */
	int delay_samps;
	int i;
	t_pitcher *x;
	
	if (argc == 0) {
		min_pitch = MINIMUM_PITCH;
		delay_samps = OUTPUT_BUFFER_DELAY;
	} else if (argc == 1) {
		if (argv[0].a_type == A_FLOAT) {
			min_pitch = (float)argv[0].a_w.w_float;
		} else if (argv[0].a_type == A_LONG) {
			
			min_pitch = (float)argv[0].a_w.w_long;
		} else {
			goto argerror;
		}
	} else if (argc == 2) {
		if (argv[0].a_type == A_FLOAT) {
			min_pitch = (float)argv[0].a_w.w_float;
		} else if (argv[0].a_type == A_LONG) {
			min_pitch = (float)argv[0].a_w.w_long;
		} else {
			goto argerror;
		}
		
		if (argv[1].a_type == A_FLOAT) {
			delay_samps = (int)argv[1].a_w.w_float;
		} else if (argv[1].a_type == A_LONG) {
			delay_samps = (int)argv[1].a_w.w_long;
		} else {
			goto argerror;
		}
	} else {
		goto argerror;
	}
	
	if ((min_pitch <= 0) || (delay_samps <= 0) ){
		error("pitcher~: Minimum pitch and Delay samples must be positive!");
		goto argerror;
	}

	
	post ("*** pitcher~: Minimum pitch %f Hz, %ld delay samples",
		  min_pitch, delay_samps);
	post ("*** pitcher~: latency = minimum pitch (in samples) + delay samples");

    x = (t_pitcher *)newobject(pitcher_class);
    
    /*
    x->outlet = listout(x);
    x->outputQelem = qelem_new(x, OutputProcedure);
    */
    
    dsp_setup((t_pxobject *)x,3);  // The object has 3 signal inlets: audio in, pitch scale factor, pitch in
    
    outlet_new((t_object *)x, "signal"); // Add a signal outlet.
    
    	x->sampleRate = sys_getsr(); // Set the sampling rate.
    
    	x->outputDelay = delay_samps;
	
	x->ringBufSize = (int) (2*delay_samps);

	x->minimumPitch = min_pitch;
	
	x->minimumPitchSamps =(int)((1/x->minimumPitch)*x->sampleRate);
	
	/* Initialize input ring buffer */
	
	x->inputRingBuf = 0;
	
	x->inputRingBufWritePos = 0;
	
	// Allocate memory for the input circular buffer.
	x->inputRingBuf = (float *)t_getbytes((long) (x->ringBufSize* sizeof(float)));
	
	// Check if the allocation suceeded.
	if (x->inputRingBuf == NULL) {
			ouchstring("pitcher~: out of memory!");
			x->ringBufSize = 0;
			return;
		}
	
	// Initiallize the input circular buffer to a zero value in each element.
	for (i = 0; i<x->ringBufSize; i++) {
			x->inputRingBuf[i] = 0.0f;
		}
		
	/* Initialize output ring buffer */

	x->outputRingBuf = 0;
	
	x->outputRingBufPitchMarkerPos = 0;
	
	// Allocate memory for the output circular buffer.
	x->outputRingBuf = (float *)t_getbytes((long) (x->ringBufSize* sizeof(float)));
	
	// Check if the allocation suceeded.
	if (x->outputRingBuf == NULL) {
			ouchstring("pitcher~: out of memory!");
			x->ringBufSize = 0;
			return;
		}
	
	// Initiallize the output circular buffer to a zero value in each element.
	for (i = 0; i<x->ringBufSize; i++) {
			x->outputRingBuf[i] = 0.0f;
		}
	
	/* Initialize the current output ring buffer read postition.
	  * Maybe need to add ringBuffSize because % does not like negative numbers?
	  */
	x->readPos = (x->inputRingBufWritePos - x->outputDelay + x->ringBufSize) % x->ringBufSize;
	//x->readPos = 98000;  // FOR DEBUG
	
	/* Initialize this to 0 so that the <inputPeriodLength> parameter will be updated in the first
	  * call to the perform method.
	  */
	x->samplesLeftInPeriod = 0;
	
	x->inputPeriodLength = 100;
	
	x->isUnvoiced = 1; // Set unvoiced until we get a pitch to use.
    return (x);
argerror: 
	error("pitcher~: usage: pitcher~ [Minimum input pitch] [Delay samples]");
	post("If only 1 arg, it sets minimum input pitch.");
	post("If no args, minimum pitch and delay samples use default of %f Hz, %ld delay samples.", (float) MINIMUM_PITCH, (long) OUTPUT_BUFFER_DELAY);
	return 0;
 
}



void pitcher_free(t_pitcher *x) {
	
	if (x->ringBufSize != 0) {
		t_freebytes(x->inputRingBuf, (long) (x->ringBufSize * sizeof(float)));
	}	
	if (x->ringBufSize != 0) {
		t_freebytes(x->outputRingBuf, (long) (x->ringBufSize * sizeof(float)));
	}	
	dsp_free(&(x->x_obj));
}

void pitcher_dsp(t_pitcher *x, t_signal **sp, short *count) {
	long i;
	int old;
	

			
	dsp_add(pitcher_perform, 6, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n, x);
}


t_int *pitcher_perform(t_int *w) {
    t_float *in = (t_float *)(w[1]);
    t_float *pitchScaleIn_ptr = (t_float *)(w[2]);
    t_float *inputPitchIn_ptr = (t_float *)(w[3]);
    t_float *out = (t_float *)(w[4]);
    // Temporary variables
    int size = w[5];
    
    float periodRatio;
    
    float correctedPitchScale;
    float correctedPitchIn;
    
    float windowVal;  // Current value of window function.
    
       /* Current position in OLA process relative to current synthesis pitch marker.
       */
    int olaIndex;

    int outLag; // Set to 1 if x->outputRingBufPitchMarkerPos lags x->inputRingBufWritePos, else set to zero.
    
    /* This is a marker for the element half way around the cirucular buffer w.r.t. x->inputRingBufWritePos */
    int inHalfAway;
    
	t_pitcher *x = (t_pitcher *)(w[6]);
	
	/*The main loop. This will iterate <MSP vector size> times for each call to this perform
	  * method. */
	while (size > 0) {
	   
		
		
		// Write the current input sample into the input circular array.
		x->inputRingBuf[x->inputRingBufWritePos] = (*in);
		
		//////////////////////////////////////////////////////
		//////////////////////////////////////////////////////
		// Do all interesting processing here.
		
		/* Check if have reached the end of the current period in the
		    input signal.
		  */
		if (x->samplesLeftInPeriod == 0)
	           {
	           	/* Check if ok to do an OLA in the output buffer. */
	           	/* That is, check if the the outputRingBufPitchMarkerPos lags nputRingBufWritePos. */
	           	//lag = (x->inputRingBufWritePos - x->outputRingBufPitchMarkerPos) % x->ringBufSize;
	           	
	           	outLag = 1;
	           	inHalfAway = (x->inputRingBufWritePos + x->ringBufSize/2) % x->ringBufSize;
	           	if (inHalfAway < (x->ringBufSize/2))
	           	    {
	           	   	/* The zero element of the input buffer lies in (inptr, inHalfAway] */
	           	   	if ((x->outputRingBufPitchMarkerPos < inHalfAway) || (x->outputRingBufPitchMarkerPos > x->inputRingBufWritePos))
	           	   	// The current input element lags current synthesis pitch marker.
	           	   	outLag = 0;
	           	    }
	           	else
	           	    {
	           	    	/* The zero element of the input buffer lies in (inHalfAway, inptr] */
	           	    	if ((x->outputRingBufPitchMarkerPos > x->inputRingBufWritePos) && (x->outputRingBufPitchMarkerPos < inHalfAway))
	           	    	    {
	           	    	    	// The current input element lags current synthesis pitch marker.
	           	    	    	outLag = 0;
	           	    	    }
	           	    }
	           	
	           	while (outLag == 1)	   
	           	    {
	           	    	// Do an OLA
	           	    	
	           	    	/* Update the synthesis pitch marker posistion (in the output buffer)/
	           	    	  */
	           	    	// Do error checking
	           	    	if ((*pitchScaleIn_ptr <= 0.1) || (*pitchScaleIn_ptr > 6.0) || (x->isUnvoiced == 1))
	           	    	    {
	           	    	    	// UhOh, out of range. Fix that.
	           	    	    	correctedPitchScale = 1.0f;
	           	    	    }
	           	    	else
	           	    	    {
	           	    	    	correctedPitchScale = *pitchScaleIn_ptr;
	           	    	    }  
	           	    	  
	           	    	// FOR DEBUG ONLY  
	           	    	//correctedPitchScale = 1.5f;
	           	    	  
	           	   	periodRatio = 1.0f/(correctedPitchScale); // Period scale factor.
	           	    	x->outputRingBufPitchMarkerPos = (int)(x->outputRingBufPitchMarkerPos + (int)(x->inputPeriodLength*periodRatio)) % x->ringBufSize;
	           	    	//x->outputRingBufPitchMarkerPos = (x->outputRingBufPitchMarkerPos + 100) % x->ringBufSize;
	           	    
	           	    	
	           	    	/* Do an OLA (in the output buffer) about the synthesis pitch marker. Note that
	           	    	  * this implementation differs slightly from Lent's algorithm, in that 1 input signal
	           	    	  * is subtracted from the current input pointer position. This is done in order to
	           	    	  * reduce latency and should not have an audible impact, I think.
	           	    	  */
	           	    	for (olaIndex = -x->inputPeriodLength; olaIndex <= x->inputPeriodLength; ++olaIndex)
	           	    	    {
	           	    	    
	           	    	    	windowVal = (1 + cos(3.14159265f*olaIndex/(float)x->inputPeriodLength))*0.5f;
	           	    	    	// windowVal = 1;  // For debuging only (no window!).
	           	    	  	x->outputRingBuf[(olaIndex + x->outputRingBufPitchMarkerPos + x->ringBufSize) % x->ringBufSize] += windowVal*x->inputRingBuf[(olaIndex + x->inputRingBufWritePos - x->minimumPitchSamps + x->ringBufSize) % x->ringBufSize];
	           	    	    	//x->outputRingBuf[(olaIndex + x->outputRingBufPitchMarkerPos + x->ringBufSize + 50000) % x->ringBufSize] += windowVal*x->inputRingBuf[(olaIndex + x->inputRingBufWritePos - x->inputPeriodLength + x->ringBufSize) % x->ringBufSize];
	           	    	    }  
	           	    	// Update loop condition variable.    	
	           	    	outLag = 1;
	           	    	inHalfAway = (x->inputRingBufWritePos + x->ringBufSize/2) % x->ringBufSize;
	      		     	if (inHalfAway < (x->ringBufSize/2))
	           		    {
	           	 	  	/* The zero element of the input buffer lies in (inptr, inHalfAway] */
	           		   	if ((x->outputRingBufPitchMarkerPos < inHalfAway) || (x->outputRingBufPitchMarkerPos > x->inputRingBufWritePos))
	           		   	// The current input element lags current synthesis pitch marker.
	           		   	outLag = 0;
	           		    }
	           		else
	           		    {
	           		    	/* The zero element of the input buffer lies in (inHalfAway, inptr] */
	           		    	if ((x->outputRingBufPitchMarkerPos > x->inputRingBufWritePos) && (x->outputRingBufPitchMarkerPos <= inHalfAway))
	           	 	   	    {
	           	 	   	    	// The current input element lags current synthesis pitch marker.
	           		    	    	outLag = 0;
	           	  	  	    }
	           	  	  }
	           	    }
	           
	           	/* Update input period value */
	           	
	           	
	           	// Do error checking on input pitch signal value.
	           	    	if (*inputPitchIn_ptr <= x->minimumPitch)
	           	    	    {
	           	    	    	// UhOh, pitch below range. Fix that.
	           	    	    	correctedPitchIn = DEFAULT_PITCH;
	           	    	    	x->isUnvoiced = 1;
	           	    	    }
	           	    	else
	           	    	    {
	           	    	    	correctedPitchIn = *inputPitchIn_ptr;
	           	    	    	x->isUnvoiced = 0;
	           	    	    }  
	           	// correctedPitchIn = 441.0;  // FOR DEBUG
	           	x->inputPeriodLength = (int)((1.0f/correctedPitchIn)*(float)x->sampleRate);
	           	// x->inputPeriodLength = 100;  // FOR DEBUG
	           	x->samplesLeftInPeriod = x->inputPeriodLength;
	           }
		  
		 --x->samplesLeftInPeriod; 
		
		// For debugging, simply copy from input buffer to output buffer.
		//x->outputRingBuf[x->inputRingBufWritePos] = x->inputRingBuf[x->inputRingBufWritePos];
		
		
		// End of all interesting processing.
		////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////
		
		// Read an output sample from the output Ring buffer.
		*out = x->outputRingBuf[x->readPos];
		//*out = x->inputRingBuf[x->readPos];
		
		// Now set the element just read from to zero, since it is no longer needed.
		x->outputRingBuf[x->readPos] = 0;	
		
		// Update the pointers.
		x->inputRingBufWritePos++;
		// Make the write postition pointer wrap back to the begining after it
		// reaches the end of the buffer.
		x->inputRingBufWritePos %= x->ringBufSize;
		
		x->readPos++;
		// Make the write postition pointer wrap back to the begining after it
		// reaches the end of the buffer.
		x->readPos %= x->ringBufSize;
		pitchScaleIn_ptr++;
		inputPitchIn_ptr++;
		in++;
		out++;
		
		size--;
	}
	
    return (w+7);
}

