/*
Copyright (c) 2003.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
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

  IOIanal:  Analyze Inter-Onset-Intervals of a series of events
  
  0.0 Matt Wright 5/29/3
  
*/
 
 
// #define DEBUG	
#define IOIA_VERSION "0.1"

 
#include "ext.h"


#define MAX_RINGBUF_SIZE 1000

typedef struct IOIA
{
	struct object f_ob;
	void *outlet;
	
	int numSeen;		/* Total #inputs seen */

	long ringbuffer[MAX_RINGBUF_SIZE];			/* Previously-seen values */
	int ringWritePos;				/* Next position that will be written to */
	


	// User parameters
	long ringBufSize;
	


	
} IOIA;


void *class;

void *IOIA_new(Symbol *s, float tolerance, long quota);
void IOIA_tolerance(IOIA *x, float t);
void IOIA_quota(IOIA *x, long q);
void IOIA_version(IOIA *x);
void IOIA_tellmeeverything(IOIA *x);
static int Near(float x, float y, float tolerance);
void IOIA_float(IOIA *x, float f);
static float processInput(IOIA *x, float f);
void Reset(IOIA *x);
static void NewInput(IOIA *x, float f);
static int NumRemembered(IOIA *x);
static float PastInput(IOIA *x, int ago);



void main(fptr *f) {
	IOIA_version(0);
	setup((t_messlist **)&class, (method)IOIA_new, 0L, (short)sizeof(IOIA), 0L, 
		  A_DEFFLOAT, A_DEFLONG, 0);
	addfloat((method)IOIA_float);
	addmess((method) IOIA_tellmeeverything, "tellmeeverything", 0);
	addmess((method) IOIA_version, "version", 0);
	addmess((method) IOIA_tolerance, "tolerance", A_FLOAT, 0);
	addmess((method) IOIA_quota, "quota", A_LONG, 0);
	addmess((method) Reset, "reset", 0);
}


void *IOIA_new(Symbol *s, float tolerance, long quota) {
	IOIA *x;
	
	x = (IOIA *)newobject(class);
	x->outlet = listout(x);
	
	if (tolerance == 0.0) {
		x->tolerance = 5.0f;
	} else {
		x->tolerance = tolerance;
	}

	if (quota == 0) {
		x->quota = 2;
	} else {
		x->quota = quota;
	}

	x->lastBeforeJump = -999.0f;
	Reset(x);
	return (x);
}

void IOIA_tolerance(IOIA *x, float t) {
	if (t < 0.0) {
		error("jump-limit: tolerance must be nonnegative");
	} else {
		x->tolerance = t;
	}
}

void IOIA_quota(IOIA *x, long q) {
	if (q < 1) {
		error("jump-limit: quota must be positive.");
	} else if (q > RINGBUF_SIZE) {
		error("jump-limit: quota %ld can't be larger than internal ring buffer size %ld",
			  q, RINGBUF_SIZE);
	} else {
		x->quota = q;
	}
}			  

void IOIA_version(IOIA *x) {
	post("jump-limit object version " IOIA_VERSION " by Matt Wright 1/3/3");
	if (x) {
		/* Not called from main(); */
		post("  compiled " __TIME__ " " __DATE__);
	}
}

void IOIA_tellmeeverything(IOIA *x) {
	int numToPrint, i;

	IOIA_version(x);
	post("  Tolerance: %f", x->tolerance);
	post("  Quota: %ld", x->quota);
	post("  %ld input values have been seen", x->numSeen);
	post("  Last non-jump value: %f", x->lastBeforeJump);
	
	numToPrint = x->numSeen;
	if (numToPrint > 10) numToPrint = 10;
	post("  Last %ld inputs:", numToPrint);
	for (i = 0; i<numToPrint; ++i) {
		post("     %f",  PastInput(x, i));
	}
}

#define fabs(x) (((x) < 0.0f) ? (-(x)) : (x))

static int Near(float x, float y, float tolerance) {
	return (fabs(x-y) <= tolerance);
}

void IOIA_float(IOIA *x, float f) {
	 outlet_float(x->outlet, processInput(x, f));
}


static float processInput(IOIA *x, float f) {

	NewInput(x, f);

	if (x->numSeen == 1) {
		x->lastBeforeJump = f;
		return f;
	} else if (x->numSeen < x->quota) {
		/* We haven't seen enough inputs to establish a believable trend, so pretend that the first 
		   input was repeated enough to count */
		
		if (Near(f, x->lastBeforeJump, x->tolerance)) {
			return f;
		} else {
			return x->lastBeforeJump;
		}
	} else if (Near(f, x->lastBeforeJump, x->tolerance)) {
		/* No jump */
		x->lastBeforeJump = f;
		return f;
	} else {
		int i;

		/* It was a jump.  Have we now seen enough values in the new range to believe the jump? */
				
		for (i = 0; i+1 < x->quota; ++i) {
			if (! Near(PastInput(x, i), PastInput(x, i+1), x->tolerance)) {
				/* We failed our quota */
				return x->lastBeforeJump;
			}
		}

		/* We filled the quota in the new range */
		
		x->lastBeforeJump = f;
		return f;
	}	
	
	error("jump-limit: should never reach this line");
	return 0;
}



/*********** Ring buffer *************/


void Reset(IOIA *x) {
	x->numSeen = 0;
	x->ringWritePos = 0;
}

static void NewInput(IOIA *x, float f) {
	x->ringbuffer[x->ringWritePos] = f;
	
	++(x->ringWritePos);
	x->ringWritePos %= RINGBUF_SIZE;
	
	++(x->numSeen);
}

static int NumRemembered(IOIA *x) {
	if (x->numSeen < RINGBUF_SIZE) return x->numSeen;
	return RINGBUF_SIZE;
}

static float PastInput(IOIA *x, int ago) {
	/* ago=0 means the most recent input */
	int index;

	if (ago >= NumRemembered(x)) {
		error("jump-limit: logic error: bad arg %ld to PastInput", ago);
		return -999;
	}
	
	index = x->ringWritePos - (ago+1);
	
	if (index < 0) index+= RINGBUF_SIZE;
	
	if (index < 0) {
		error("jump-limit: internal logic error.");
		return -999;
	}
	
	return x->ringbuffer[index];
}
	
