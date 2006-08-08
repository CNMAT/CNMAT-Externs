/*

Copyright (c) 1998.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Based on sample code from David Zicarelli.

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
*/

/*
DESCRIPTION: Output lists of consecutive signal values (as floats) every so many samples.  Sort of like snapshot~, but for an entire (rectangular) window of samples.
SVN_REVISION: $LastChangedRevision$
*/


*/

/* 2/12/98 */

#define WINDOWER_VERSION "0.0"
#define DEFAULT_WINDOW_SIZE 1024

#include "ext.h"
#include "z_dsp.h"

void *windower_class;

typedef struct _windower
{
	/* Max stuff */
    t_pxobject x_obj;
    void *outlet;
    void *outputQelem;	/* We don't want to output from within the perform routine! */
    
    /* Parameters */
    float windowSecs; /* Negative if they specified window in samples */
    int windowSamps;
    float hopSecs;		/* Negative if they specified hop size in samples */
    int hopSamps;
    
    /* Ring buffer for recent memory */
    float *ringBuf;
    int ringBufSize;
    int ringBufWritePos;
    
    
    int SampsSinceLastWindow;	/* How many samples have been read since the last window?
    							   When this equals hopSamps, it's time to output. */
    Atom *frameAtoms;			/* Should have windowSamps many atoms, and be preset
    							   to have all types be A_FLOAT. */
    int frameAtomsSize;
} t_windower;


void main(void);
void windower_assist(t_windower *x, void *b, long m, long a, char *s);
void *windower_new(Symbol *s, short argc, Atom *argv);
void ResizeBufferAndAtomList(t_windower *x);
void windower_free(t_windower *x);
void windower_dsp(t_windower *x, t_signal **sp, short *count);
t_int *windower_perform(t_int *w);
void *OutputProcedure(t_windower *x);
int secondsToSamples (float secs);

void main(void) {
	post("windower~ version " WINDOWER_VERSION " by Matt Wright");
	post("Copyright © 1998 Regents of the University of California.  ");

    setup(&windower_class, windower_new, (method)windower_free, (short)sizeof(t_windower),
          0L, A_GIMME, 0);
    addmess((method)windower_dsp, "dsp", A_CANT, 0);
    addmess((method)windower_assist,"assist",A_CANT,0);
    dsp_initclass();
    rescopy('STR#',3241);
}

void windower_assist(t_windower *x, void *b, long m, long a, char *s) {
	assist_string(3241,m,a,1,2,s);
}

void *windower_new(Symbol *s, short argc, Atom *argv) {
	float windowSecs, hopSecs;
	int windowSamps, hopSamps;
	t_windower *x;
	
	if (argc == 0) {
		hopSecs = windowSecs = -1.0f;
		hopSamps = windowSamps = DEFAULT_WINDOW_SIZE;
	} else if (argc == 1) {
		if (argv[0].a_type == A_FLOAT) {
			hopSecs = windowSecs = argv[0].a_w.w_float;
			hopSamps = windowSamps = secondsToSamples(windowSecs);
		} else if (argv[0].a_type == A_LONG) {
			hopSecs = windowSecs = -1.0f;
			hopSamps = windowSamps = argv[0].a_w.w_long;
		} else {
			goto argerror;
		}
	} else if (argc == 2) {
		if (argv[0].a_type == A_FLOAT) {
			windowSecs = argv[0].a_w.w_float;
		    windowSamps = secondsToSamples(windowSecs);
		} else if (argv[0].a_type == A_LONG) {
			windowSecs = -1.0f;
			windowSamps = argv[0].a_w.w_long;
		} else {
			goto argerror;
		}
		
		if (argv[1].a_type == A_FLOAT) {
			hopSecs = argv[1].a_w.w_float;
		    hopSamps = secondsToSamples(hopSecs);
		} else if (argv[1].a_type == A_LONG) {
			hopSecs = -1.0f;
			hopSamps = argv[1].a_w.w_long;
		} else {
			goto argerror;
		}
	} else {
		goto argerror;
	}
	
	if ((windowSamps <= 0) || (hopSamps <= 0) ){
		error("windower~: window and hop size must be positive!");
		goto argerror;
	}

	
	post ("*** windower~: window %f secs/%ld samps; hop %f secs/%ld samps",
		  windowSecs, windowSamps, hopSecs, hopSamps);

    x = (t_windower *)newobject(windower_class);
    x->outlet = listout(x);
    x->outputQelem = qelem_new(x, OutputProcedure);
    dsp_setup((t_pxobject *)x,1);
	x->windowSecs = windowSecs;
	x->windowSamps = windowSamps;
	x->hopSecs = hopSecs;
	x->hopSamps = hopSamps;
	x->ringBuf = 0;
	x->ringBufSize = 0;
	x->ringBufWritePos = 0;
	x->SampsSinceLastWindow = 0;
	x->frameAtoms = NULL;
	x->frameAtomsSize = 0;

	ResizeBufferAndAtomList(x);
    return (x);
    
argerror: 
	error("windower~: usage: windower~ [window size] [hop size]");
	post("If only 1 arg, it sets window and hop size.");
	post("If no args, window and hop use default of %ld.", (long) DEFAULT_WINDOW_SIZE);
	return 0;
}

void ResizeBufferAndAtomList(t_windower *x) {
	/* Both the ring buffer (ringBuf) and the list of atoms to be output (frameAtoms)
	   should be as large as the number of samples in the window that will be output.
	   If the sampling rate changes and the user specified window size in seconds,
	   this will change the number of samples in the window to be output, so call this
	   procedure to resize everything.  */
	int i;
	
	if (x->ringBufSize < x->windowSamps) {
		if (x->ringBufSize != 0) {
			freebytes(x->ringBuf, (short) (x->ringBufSize * sizeof(float)));
		}
		x->ringBuf = (float *)getbytes((short) (x->windowSamps* sizeof(float)));
		if (x->ringBuf == NULL) {
			ouchstring("windower~: out of memory!");
			x->ringBufSize = 0;
			return;
		}

		x->ringBufSize = x->windowSamps;
		x->ringBufWritePos = 0;
		x->SampsSinceLastWindow = 0;
		for (i = 0; i<x->ringBufSize; i++) {
			x->ringBuf[i] = 0.0f;
		}
	}
	   	
	if (x->frameAtomsSize < x->windowSamps) {
		if(x->frameAtomsSize != 0) {
			freebytes(x->frameAtoms, (short) (x->frameAtomsSize * sizeof(Atom)));
		}
		x->frameAtoms = (Atom *)getbytes((short) (x->windowSamps * sizeof(Atom)));
		if (x->frameAtoms == NULL) {
			ouchstring("windower~: out of memory!");
			x->frameAtomsSize = 0;
			return;
		}
		x->frameAtomsSize = x->windowSamps;
		for (i = 0; i<x->frameAtomsSize; i++) {
			x->frameAtoms[i].a_type = A_FLOAT;
		}
	}
}

void windower_free(t_windower *x) {
	qelem_free(x->outputQelem);
	if (x->ringBufSize != 0) {
		freebytes(x->ringBuf, (short) (x->ringBufSize * sizeof(float)));
	}	
	if(x->frameAtomsSize != 0) {
		freebytes(x->frameAtoms, (short) (x->frameAtomsSize * sizeof(Atom)));
	}
	dsp_free(&(x->x_obj));
}

void windower_dsp(t_windower *x, t_signal **sp, short *count) {
	long i;
	int old;
	
	if (x->hopSecs > 0) {
		/* They specified the hop size in seconds, and the sampling rate may have changed. */
		x->hopSamps = secondsToSamples(x->hopSecs);
	}

	if (x->windowSecs > 0) {
		/* They specified the window in seconds, and the sampling rate may have changed. */
		old = x->windowSamps;
		x->windowSamps = secondsToSamples(x->windowSecs);
		if (x->windowSamps > old) {
			/* Have to reallocate buffers. */
			ResizeBufferAndAtomList(x);
		}
	}
			
	dsp_add(windower_perform, 3, sp[0]->s_vec, sp[0]->s_n, x);
}


t_int *windower_perform(t_int *w) {
    t_float *in = (t_float *)(w[1]);
    int size = w[2];
	t_windower *x = (t_windower *)(w[3]);
	
	/* For efficiency, we could check if we won't output from this vector ... 
	if (size + SampsSinceLastWindow < hopSamps) { 
		...just copy into buffer; don't check for output...
    }
	*/
	
	
	/* Write new samples into ring buffer and see if we have enough to output */
	while (size > 0) {
		if (x->SampsSinceLastWindow == x->hopSamps) {
			/* Time to output a frame */
			int i;
			int ringReadPos = (x->ringBufWritePos - x->windowSamps + x->ringBufSize)
				 % x->ringBufSize;
			for (i = 0; i < x->windowSamps; i++) {
				x->frameAtoms[i].a_w.w_float = x->ringBuf[ringReadPos];
				ringReadPos++;
				ringReadPos %= x->ringBufSize;
			}
			/* Set the Qelem that will output the list. */
			qelem_set(x->outputQelem);
			x->SampsSinceLastWindow = 0;
		}
		x->ringBuf[x->ringBufWritePos] = (*in)++;
		x->ringBufWritePos++;
		x->ringBufWritePos %= x->ringBufSize;
		
		x->SampsSinceLastWindow++;
		size--;
	}
	
    return (w+4);
}

void *OutputProcedure(t_windower *x) {
	/* This is called by a Qelem. */
	outlet_list(x->outlet, 0L, x->windowSamps, x->frameAtoms);
}

int secondsToSamples (float secs) {
	return secs * sys_getsr();
}
