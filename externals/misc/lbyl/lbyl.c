/*
Copyright (c) 2002,3.  The Regents of the University of California (Regents).
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

  lbyl:  "Look Before You Leap": 
  			Echo an input stream of numbers to the output, but "don't believe" large
  	        jumps in the value unless the output stays at that value for a while.
  	           
  0.0 Matt Wright 1/3/3
  0.1 Renamed LBYL
  0.2 Added outlets for "non-bogus" and "rejected"
  
  
  To-Do:  Make despair a return value, decision to repeat last pre-jump value in the float method.
  
  
*/
 
 
// #define DEBUG	
#define LBYL_VERSION "0.1"

 
#include "ext.h"


#define RINGBUF_SIZE 100

typedef struct LBYL
{
	struct object f_ob;
	void *nonbogus_outlet;
	void *outlet;
	void *reject_outlet;
	
	int numSeen;		/* Total #inputs seen */

	float ringbuffer[RINGBUF_SIZE];			/* Previously-seen values */
	int ringWritePos;				/* Next position that will be written to */
	
	float lastBeforeSpuriousJump;
	int jumped;				/* Have we jumped recently, or do we think we might be in a steady region? */
	float lastOutput;

	// User parameters
	float tolerance;	/* If successive values are within this close of each other, it's not a jump */
	int quota;			/* Number of values that must be seen in new range to believe a jump */
	
} LBYL;


void *class;

void *LBYL_new(Symbol *s, float tolerance, long quota);
void LBYL_tolerance(LBYL *x, float t);
void LBYL_quota(LBYL *x, long q);
void LBYL_version(LBYL *x);
void LBYL_tellmeeverything(LBYL *x);
static int Near(float x, float y, float tolerance);
void LBYL_float(LBYL *x, float f);
void LBYL_int(LBYL *x, int i);
static float processInput(LBYL *x, float f, int *rejected, int *despair);
void Reset(LBYL *x);
static void NewInput(LBYL *x, float f);
static int NumRemembered(LBYL *x);
static float PastInput(LBYL *x, int ago);



void main(fptr *f) {
	LBYL_version(0);
	setup((t_messlist **)&class, (method)LBYL_new, 0L, (short)sizeof(LBYL), 0L, 
		  A_DEFFLOAT, A_DEFLONG, 0);
	addfloat((method)LBYL_float);
	addint((method)LBYL_int);
	addmess((method) LBYL_tellmeeverything, "tellmeeverything", 0);
	addmess((method) LBYL_version, "version", 0);
	addmess((method) LBYL_tolerance, "tolerance", A_FLOAT, 0);
	addmess((method) LBYL_quota, "quota", A_LONG, 0);
	addmess((method) Reset, "reset", 0);
}


void *LBYL_new(Symbol *s, float tolerance, long quota) {
	LBYL *x;
	
	x = (LBYL *)newobject(class);
	x->reject_outlet = listout(x);
	x->outlet = listout(x);
	x->nonbogus_outlet = listout(x);	
	
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

	x->lastBeforeSpuriousJump = -999.0f;
	x->jumped = 1;
	Reset(x);
	return (x);
}

void LBYL_tolerance(LBYL *x, float t) {
	if (t < 0.0) {
		error("lbyl: tolerance must be nonnegative");
	} else {
		x->tolerance = t;
	}
}

void LBYL_quota(LBYL *x, long q) {
	if (q < 1) {
		error("lbyl: quota must be positive.");
	} else if (q > RINGBUF_SIZE) {
		error("lbyl: quota %ld can't be larger than internal ring buffer size %ld",
			  q, RINGBUF_SIZE);
	} else {
		x->quota = q;
	}
}			  

void LBYL_version(LBYL *x) {
	post("lbyl object version " LBYL_VERSION " by Matt Wright 1/3/3");
	if (x) {
		/* Not called from main(); */
		post("  compiled " __TIME__ " " __DATE__);
	}
}

void LBYL_tellmeeverything(LBYL *x) {
	int numToPrint, i;

	LBYL_version(x);
	post("  Tolerance: %f", x->tolerance);
	post("  Quota: %ld", x->quota);
	post("  %ld input values have been seen", x->numSeen);
	post("  Last non-jump value: %f", x->lastBeforeSpuriousJump);
	post("  I believe the input has recently %s", (x->jumped ? "jumped" : "stayed steady"));
	
	numToPrint = x->numSeen;
	if (numToPrint > 10) numToPrint = 10;
	post("  Last %ld inputs:", numToPrint);
	for (i = 0; i<numToPrint; ++i) {
		post("     %f",  PastInput(x, i));
	}
	post("  Last output: %f", x->lastOutput);
}

#define fabs(x) (((x) < 0.0f) ? (-(x)) : (x))

static int Near(float x, float y, float tolerance) {
	return (fabs(x-y) <= tolerance);
}

void LBYL_float(LBYL *x, float f) {
	int rejected = 0, despair = 0;
	float output = processInput(x, f, &rejected, &despair);
	
	if (rejected) {
		 outlet_float(x->reject_outlet, f);
	}
	
	outlet_float(x->outlet, output);
	x->lastOutput = output;
	
	if (!despair) {
		outlet_float(x->nonbogus_outlet, output);
	}	
}


void LBYL_int(LBYL *x, int i) {
	LBYL_float(x, (float) i);
}

static float processInput(LBYL *x, float f, int *rejected, int *despair) {
	int i;

	NewInput(x, f);

	if (x->numSeen == 1) {
		x->lastBeforeSpuriousJump = f;
		x->jumped = 0;
		return f;
	} else if (x->numSeen < x->quota) {
		/* We haven't seen enough inputs to establish a believable trend, so pretend that the first 
		   input was repeated enough to count */
		
		for (i = 0; i+1 < x->numSeen; ++i) {
			if (! Near(PastInput(x, i), PastInput(x, i+1), x->tolerance)) {
				/* The first sub-quota of inputs haven't been consistent */
				*rejected = 1;
				x->jumped = 1;
				*despair = 1;
				return x->lastBeforeSpuriousJump; /* Bogus */
			}
		}
		/* First inputs have been consistent, even though quota not yet met. */		
		return f;
	} else if (x->jumped) {
		/* Have we now seen enough values in the new range to believe the jump? */
		for (i = 0; i+1 < x->quota; ++i) {
			if (! Near(PastInput(x, i), PastInput(x, i+1), x->tolerance)) {
				/* We failed our quota */
				*rejected = 1;
				*despair = 1;
				return x->lastBeforeSpuriousJump;  /* Bogus */
			}
		}

		/* We filled the quota in the new range */

		x->jumped = 0;
		*rejected = 0;
		x->lastBeforeSpuriousJump = f;
		return f;
	} else {
		/* We were in the steady state */
		
		if (Near(f, x->lastBeforeSpuriousJump, x->tolerance)) {
			/* No jump, still in steady state, this input is the new "last good value" */
			x->lastBeforeSpuriousJump = f;
			return f;	
		} else {
			/* This input was a jump.  Was it spurious?  Let's look at the last quota's worth of inputs
			   and see if we've made it to the new range. */
			
			for (i = 0; i+1 < x->quota; ++i) {
				if (Near(PastInput(x, i), x->lastBeforeSpuriousJump, x->tolerance)) {
					/* Saw a good old value recently, so assume this jump is still spurious */
					*rejected = 1;
					x->jumped = 0;
					return x->lastBeforeSpuriousJump;  /* Good */
				}
			}
					
			/* It's been longer than the quota since we saw a value near x->lastBeforeSpuriousJump,
			   so maybe we've met the quota in the new range */
			    
			for (i = 0; i+1 < x->quota; ++i) {
				if (! Near(PastInput(x, i), PastInput(x, i+1), x->tolerance)) {
					/* We failed our quota, so we're just jumping around too much to know what to output. */
					x->jumped = 1;
					*rejected = 1;
					*despair = 1;
					return x->lastBeforeSpuriousJump;  /* Bogus */
				}
			}
			
			/* We've met the quota in the new range. */
			x->jumped = 0;
			*rejected = 0;
			return f;
		}				
	}	
	
	error("lbyl: should never reach this line");
	return 0;
}



/*********** Ring buffer *************/


void Reset(LBYL *x) {
	x->numSeen = 0;
	x->ringWritePos = 0;
}

static void NewInput(LBYL *x, float f) {
	x->ringbuffer[x->ringWritePos] = f;
	
	++(x->ringWritePos);
	x->ringWritePos %= RINGBUF_SIZE;
	
	++(x->numSeen);
}

static int NumRemembered(LBYL *x) {
	if (x->numSeen < RINGBUF_SIZE) return x->numSeen;
	return RINGBUF_SIZE;
}

static float PastInput(LBYL *x, int ago) {
	/* ago=0 means the most recent input */
	int index;

	if (ago >= NumRemembered(x)) {
		error("lbyl: logic error: bad arg %ld to PastInput", ago);
		return -999;
	}
	
	index = x->ringWritePos - (ago+1);
	
	if (index < 0) index+= RINGBUF_SIZE;
	
	if (index < 0) {
		error("lbyl: internal logic error.");
		return -999;
	}
	
	return x->ringbuffer[index];
}
	
