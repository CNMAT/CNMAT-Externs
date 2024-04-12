/*
Copyright (c) 2002,3,4,5.  The Regents of the University of California (Regents).
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: lbyl ("Look Before You Leap")
DESCRIPTION: Echo an input stream of numbers to the output, but "don't believe" large jumps in the value unless the output stays at that value for a while.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2002,3,4,5,6
DRUPAL_NODE: /patch/4067
SVN_REVISION: $LastChangedRevision$
VERSION 0.0: First version  1/3/3
VERSION 0.1: Renamed LBYL
VERSION 0.2:  Added outlets for "non-bogus" and "rejected"
VERSION 0.2.1:  Force Package Info Generation
VERSION 0.2.2:  Removed MaxAPI.framework dependence; updated for Max 8.6.1; JWagner, 4/2024
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Version history:  	           
  
  To-Do:  Make despair a return value, decision to repeat last pre-jump value in the float method.
  
  
*/
 
 
// #define DEBUG	
#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "version.c"


#define RINGBUF_SIZE 100

typedef struct LBYL
{
	struct object f_ob;
	void *nonbogus_outlet;
	void *outlet;
	void *reject_outlet;
	
	t_atom_long numSeen;		/* Total #inputs seen */

	double ringbuffer[RINGBUF_SIZE];			/* Previously-seen values */
	t_atom_long ringWritePos;				/* Next position that will be written to */
	
	double lastBeforeSpuriousJump;
	t_atom_long jumped;				/* Have we jumped recently, or do we think we might be in a steady region? */
	double lastOutput;

	// User parameters
	double tolerance;	/* If successive values are within this close of each other, it's not a jump */
	t_atom_long quota;			/* Number of values that must be seen in new range to believe a jump */
	
} LBYL;


//void *class;

void *LBYL_new(t_symbol *s, long argc, t_atom *argv);
void LBYL_free(LBYL *x);
void LBYL_tolerance(LBYL *x, double t);
void LBYL_quota(LBYL *x, t_atom_long q);
void LBYL_tellmeeverything(LBYL *x);
static t_atom_long Near(double x, double y, double tolerance);
void LBYL_double(LBYL *x, double d);
void LBYL_int(LBYL *x, t_atom_long i);
static double processInput(LBYL *x, double f, t_atom_long *rejected, t_atom_long *despair);
void Reset(LBYL *x);
static void NewInput(LBYL *x, double f);
static t_atom_long NumRemembered(LBYL *x);
static double PastInput(LBYL *x, t_atom_long ago);

void *lbyl_class;

C74_EXPORT void ext_main(void *r) {
	version(0);
    
    // *** ORIGINAL CODE NOW DEPRECATED *** -- JWagner2024
    //    setup((t_messlist **)&class, (method)LBYL_new, 0L, (short)sizeof(LBYL), 0L,
    //          A_DEFFLOAT, A_DEFLONG, 0);
    //    addmess((method) version, "version", 0);
    //    addfloat((method) LBYL_float);
    //    addint((method) LBYL_int);
    //    addmess((method) LBYL_tellmeeverything, "tellmeeverything", 0);
    //    addmess((method) LBYL_tolerance, "tolerance", A_FLOAT, 0);
    //    addmess((method) LBYL_quota, "quota", A_LONG, 0);
    //    addmess((method) Reset, "reset", 0);
    
    t_class *c = class_new("lbyl", (method)LBYL_new, (method)LBYL_free, (long)sizeof(LBYL), 0L, A_GIMME, 0);
    class_addmethod(c, (method) version, "version", A_SYM, 0);
    class_addmethod(c, (method) LBYL_double, "float", A_FLOAT, 0);
    class_addmethod(c, (method) LBYL_int, "int", A_LONG, 0);
    class_addmethod(c, (method) LBYL_tellmeeverything, "tellmeeverything", A_DEFSYM, 0);
    class_addmethod(c, (method) LBYL_tolerance, "tolerance", A_FLOAT, 0);
    class_addmethod(c, (method) LBYL_quota, "quota", A_LONG, 0);
    class_addmethod(c, (method) Reset, "reset", 0);
    class_register(CLASS_BOX, c);
    lbyl_class = c;
}


void *LBYL_new(t_symbol *s, long argc, t_atom *argv) {
    double tolerance;
    t_atom_long quota;
	LBYL *x = NULL;
    
    switch (argc){
        case 1:
            if(argv->a_type == A_FLOAT){
                tolerance = atom_getfloat(argv);
            }else{
                tolerance = 5.0;
            }
            quota =2;
            break;
        case 2:
            if(argv->a_type == A_FLOAT){
                tolerance = atom_getfloat(argv);
            }else{
                tolerance = 5.0;
            }
            if((argv+1)->a_type == A_LONG){
                quota = atom_getlong(argv+1);
            }else{
                quota = 2;
            }
            break;
        default:
            tolerance = 5.0;
            quota = 2;
            
    }
        
    
    
	
//	x = (LBYL *)newobject(class);
    x = object_alloc(lbyl_class);
    x->reject_outlet = listout(x);
	x->outlet = listout(x);
	x->nonbogus_outlet = listout(x);
	
	if (tolerance == 0.0) {
		x->tolerance = 5.0f;
	} else {
		x->tolerance = tolerance;
	}

	if (quota < RINGBUF_SIZE) {
		x->quota = quota;
    }else{
        x->quota = 2;
    }
//    LBYL_quota(x, quota);

	x->lastBeforeSpuriousJump = -999.0f;
	x->jumped = 1;
	Reset(x);
	return (x);
}

void LBYL_tolerance(LBYL *x, double t) {
	if (t < 0.0) {
		error("lbyl: tolerance must be nonnegative");
	} else {
		x->tolerance = (double) t;
	}
}

void LBYL_quota(LBYL *x, t_atom_long q) {
	if (q < 1) {
		error("lbyl: quota must be positive.");
	} else if (q > RINGBUF_SIZE) {
		error("lbyl: quota %ld can't be larger than internal ring buffer size %ld",
			  q, RINGBUF_SIZE);
	} else {
		x->quota = q;
	}
}			  

void LBYL_tellmeeverything(LBYL *x) {
	t_atom_long numToPrint, i;

	version(x);
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

static t_atom_long Near(double x, double y, double tolerance) {
	return (fabs(x-y) <= tolerance);
}

void LBYL_double(LBYL *x, double d) {
	double f = (double) d;
	t_atom_long rejected = 0, despair = 0;
	double output = processInput(x, f, &rejected, &despair);
	
	if (rejected) {
		 outlet_float(x->reject_outlet, f);
	}
	
	outlet_float(x->outlet, output);
	x->lastOutput = output;
	
	if (!despair) {
		outlet_float(x->nonbogus_outlet, output);
	}
}


void LBYL_int(LBYL *x, t_atom_long i) {
	LBYL_double(x, (double) i);
}

static double processInput(LBYL *x, double f, t_atom_long *rejected, t_atom_long *despair) {
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

static void NewInput(LBYL *x, double f) {
	x->ringbuffer[x->ringWritePos] = f;
	
	++(x->ringWritePos);
	x->ringWritePos %= RINGBUF_SIZE;
	
	++(x->numSeen);
}

static t_atom_long NumRemembered(LBYL *x) {
	if (x->numSeen < (t_atom_long)RINGBUF_SIZE) return x->numSeen;
	return (t_atom_long)RINGBUF_SIZE;
}

static double PastInput(LBYL *x, t_atom_long ago) {
	/* ago=0 means the most recent input */
	t_atom_long index;

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

void LBYL_free(LBYL *x)
{
    ;
}
	
