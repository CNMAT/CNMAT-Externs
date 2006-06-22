/*
 * Copyright (c) 1996, 1997 Regents of the University of California.
 * All rights reserved.
 * The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

  /* 
        Author: Matt Wright
        
        6/29/99
        From Max OpenSoundControl object

DESCRIPTION: Receive audio in UDP packets and output it continuously as an MSP signal.


   */

#define UDP2AUDIO_VERSION "0.0"

#include "ext.h"
#include "z_dsp.h"

void *U2A_class;

#define NUM_CHANS 2
#define BUF_SIZE 1000000
#define HEADSTART 64

typedef struct ruc {
	t_pxobject O_ob;
	void *O_msgoutlet;
	short previousCount;	// Negative until we see the first packet
	
	long headStart;		// # samples to read before we output any.  Set to 0 once we start outputting
	
	long bufSize;		// Size of all ring buffers
	float *ringbuffers[NUM_CHANS];
	long readPtr;
	long writePtr;
	int underflowState;
} U2A;

Symbol *ps_skipped, *ps_dropped, *ps_duplicate, *ps_buffer_overflow, *ps_buffer_underflow;
float sampleScaleFactor;


void *U2A_new(long arg);
void U2A_free(U2A *x);
void U2A_assist(U2A *x, void *b, long m, long a, char *s);
void U2A_version (U2A *x);
void U2A_state (U2A *x);
void U2A_headstart (U2A *x, long l);
void U2A_ParseFullPacket(U2A *x, long size, long bufptr);
void U2A_dsp(U2A *x, t_signal **sp, short *count);
t_int *U2A_perform(t_int *w);
long isModuloishlyBetween(long x, long low, long high);
t_int *U2A_perform(t_int *w);
long num_can_read(U2A *x);
long num_can_write(U2A *x);


void main (fptr *f) {
	long oldA4;
	
	post("UDP2audio~ object version " UDP2AUDIO_VERSION " by Matt Wright");
	post("Copyright © 1999 Regents of the University of California.  ");
		
	setup(&U2A_class,U2A_new,(method) U2A_free,(short)sizeof(U2A),0L,0);

    addmess((method)U2A_dsp, "dsp", A_CANT, 0);
	addmess((method)U2A_assist, "assist",	A_CANT,0);
	addmess((method)U2A_version, "version", 0);
	addmess((method)U2A_state, "state", 0);
	addmess((method)U2A_headstart, "headstart", A_LONG, 0);
	addmess((method)U2A_ParseFullPacket, "FullPacket", A_LONG, A_LONG, 0);

    dsp_initclass();
	rescopy('STR#',3009);

	ps_skipped = gensym("skipped");
	ps_dropped = gensym("dropped");
	ps_duplicate = gensym("duplicate");
	ps_buffer_overflow = gensym("buffer_overflow");
	ps_buffer_underflow = gensym("buffer_underflow");
	
	sampleScaleFactor = 1.0f/2147483647.0f;
}

#define DEFAULT_BUFFER_SIZE 1024

void *U2A_new(long arg) {
	U2A *x;
	char *buf;
	int i;
		
	x = (U2A *) newobject(U2A_class);
	dsp_setup((t_pxobject *) x, 1);
	
	/* Create the outlets in right to left order */
	x->O_msgoutlet = outlet_new(x, 0L);		// Message outlet
	for (i = 0; i < NUM_CHANS; ++i) {
		outlet_new(x, "signal");				
	}
	
	x->previousCount = -1;
	x->headStart = HEADSTART;
	x->underflowState = 0;
	x->bufSize = BUF_SIZE;
	x->readPtr = x->writePtr = 0;
	for (i = 0; i < NUM_CHANS; ++i) {
		x->ringbuffers[i] = (float *) getbytes(BUF_SIZE * sizeof(float));
	}

	return (x);
}

void U2A_free(U2A *x) {
	int i;
	for (i = 0; i < NUM_CHANS; ++i) {
		freebytes(x->ringbuffers[i], x->bufSize);
	}
}

void U2A_assist(U2A *x, void *b, long m, long a, char *s) {
	assist_string(3009,m,a,1,2,s);
}

void U2A_version (U2A *x) {
	post("UDP2audio~ Version " UDP2AUDIO_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

void U2A_state (U2A *x) {
	post("UDP2audio~ state:  obj %p ", x);
	if (x->headStart) {
		post("  Still waiting for %ld incoming samples before I output.", x->headStart);
	}
	post("  Read ptr %ld, write ptr %ld, bufSize %ld", x->readPtr, x->writePtr, x->bufSize);
	if (x->previousCount < 0) {
		post("  Still waiting to receive my first packet");
	} else {
		post("  Last packet I saw was number %ld", (long) x->previousCount);
	}
}

void U2A_headstart (U2A *x, long l) {
	x->headStart = l;
}


/* Ring buffer helpers */

long num_can_read(U2A *x) {
	long writePtrSnapshot, sampsAvailable;

	writePtrSnapshot = x->writePtr;
	if (x->readPtr <= writePtrSnapshot) {
		sampsAvailable = writePtrSnapshot - x->readPtr;
	} else {
		sampsAvailable = x->bufSize - (x->readPtr - writePtrSnapshot);
	}

	return sampsAvailable;
}

long num_can_write(U2A *x) {
 	/* This could be interrupted at any time by the audio processing interrupt.  But if that happens
 	   we know that more will be read, so our answer will be too conservative. */

	long readPosSnapshot, capacity;
	
	readPosSnapshot = x->readPtr;

	if (x->writePtr < readPosSnapshot) {
		capacity = readPosSnapshot - x->writePtr - 1;
	} else {
		capacity = x->bufSize - (x->writePtr - readPosSnapshot) - 1;
	}
	return capacity;
}


/*******************************************************************
 Parsing incoming UDP packets 
 *******************************************************************/

long isModuloishlyBetween(long x, long low, long high) {
	if (low < high) {
		return (low <= x) && (x <= high);
	} else {
		// Wrap-around case
		return (x >= low) || (x <= high);
	}
}

#define MAX_COUNT 65535U
#define MAX_PACKET_SKIP 100	// Most sequence numbers we're willing to skip forward
#define CHANS_IN_PACKET 8


void U2A_ParseFullPacket(U2A *x, long size, long bufptr) {
	Atom args[2];
	char *bytes;
	long *samples;
	unsigned short *shortPtr, count;
	int numFrames, i, c;
	long readPosSnapshot, capacity, myWritePtr;
	bytes = (char *)bufptr;
	
	// post("*U2A_ParseFullPacket: size %ld, bufptr %ld", size, bufptr);
	
	/* Last two bytes of packet are the count */
	shortPtr = (unsigned short *) (bytes+size-2);
	count = (*shortPtr);
	
	if (x->previousCount < 0) {
		// First packet we've seen; no problem
	} else {
		if (count != x->previousCount + 1) {
			if (count == x->previousCount) {
				/* Duplicate of the packet we just got; trash it. */
				SETLONG(args, count);
				outlet_anything(x->O_msgoutlet, ps_duplicate, 1, args);
				return;
			} else if (isModuloishlyBetween(count, x->previousCount, x->previousCount+MAX_PACKET_SKIP)) {
				// Assume we skipped some packets and carry on
				SETLONG(args, (long) x->previousCount);
				SETLONG(args+1, (long) count);
				outlet_anything(x->O_msgoutlet, ps_skipped, 2, args);
			} else {
				// This is some old packet we'll just trash
				SETLONG(args, count);
				SETLONG(args+1, (long) x->previousCount);
				outlet_anything(x->O_msgoutlet, ps_dropped, 2, args);
				return;
			}
		}
	}
	x->previousCount = count;
	
	/* The rest of the packet is the data.  If we got this far we want to write this data into our
	   circular buffers. */
	   
	samples = (long *) bufptr;
	numFrames = (size-2) / (CHANS_IN_PACKET * sizeof(float));
	
	/* Make sure the ring buffers have capacity. */
	
	capacity = num_can_write(x);
	
	if (capacity < numFrames) {
		SETLONG(args, (long) capacity);
		SETLONG(args+1, (long) numFrames);

		outlet_anything(x->O_msgoutlet, ps_buffer_overflow, 2, args);
		/* Just trash the incoming packet; no room for it. */
		return;
	}
	
	/* Put the data in the ring buffers */
	myWritePtr = x->writePtr;
	for (i = 0; i < numFrames; ++i) {
		for (c = 0; c < NUM_CHANS; ++c) {
	         x->ringbuffers[c][myWritePtr] = ((float) samples[(i * CHANS_IN_PACKET) + c]) * sampleScaleFactor;
		}
		myWritePtr = (myWritePtr + 1) % x->bufSize;
		/* Skip over the samples to ignore */
		samples += (CHANS_IN_PACKET - NUM_CHANS);			
	}
	x->writePtr = myWritePtr;
}


/* Outputting a signal */

void U2A_dsp(U2A *x, t_signal **sp, short *count){
	void *vectorArgs[NUM_CHANS+2];
	int i;
	
	for (i = 0; i < NUM_CHANS; ++i) {
		vectorArgs[i] = sp[i]->s_vec;
	}
	vectorArgs[NUM_CHANS] = (void *) sp[0]->s_n;
	vectorArgs[NUM_CHANS+1] = (void *) x;
	
	dsp_addv(U2A_perform, (long) NUM_CHANS+2, (void **) vectorArgs);
}

t_int *U2A_perform(t_int *w) {
	int i, j, vecSize, c;
	float *outSignals[NUM_CHANS];
	long sampsAvailable, writePtrSnapshot, myReadPtr;
	U2A *x;
	Atom args[2];
	
	for (i = 0; i < NUM_CHANS; ++i) {
		outSignals[i] = (float *)(w[i+1]);
	}
	vecSize = w[NUM_CHANS+1];
	x = (U2A *) w[NUM_CHANS+2];
	
#ifdef OUTPUTALLZEROES
	for (c = 0; c < NUM_CHANS; ++c) {
		for (i = 0; i < vecSize; ++i) {
			outSignals[c][i] = 0.0f;
		}
	}
#else
	/* Make sure we have enough samples.*/
	sampsAvailable = num_can_read(x);
	
	
	if (x->headStart) {
		if (sampsAvailable >= x->headStart) {
			x->headStart = 0;
		} else {
			goto outputzeroes;
		}
	}
	
	if (sampsAvailable < vecSize) {
		if (x->underflowState) {
			// We already complained
		} else {
			x->underflowState = 1;
			SETLONG(args, (long) sampsAvailable);
			SETLONG(args+1, (long) vecSize);
			outlet_anything(x->O_msgoutlet, ps_buffer_underflow, 2, args);
		}

		/* Output all zeroes this frame */
		outputzeroes:
		for (c = 0; c < NUM_CHANS; ++c) {
			for (i = 0; i < vecSize; ++i) {
				outSignals[c][i] = 0.0f;
			}
		}
	} else {
		/* We're happy */
		x->underflowState = 0;
		myReadPtr = x->readPtr;
		for (i = 0; i < vecSize; ++i) {
			for (c = 0; c < NUM_CHANS; ++c) {
		         outSignals[c][i] = x->ringbuffers[c][myReadPtr];
			}
			myReadPtr = (myReadPtr + 1) % x->bufSize;
		}
		x->readPtr = myReadPtr;
	}
	
#endif	
	
	return w + NUM_CHANS + 3;
}
