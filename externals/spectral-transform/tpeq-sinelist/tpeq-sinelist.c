/*
Copyright (c) 1999.  The Regents of the University of California (Regents).
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

 tpeq-sinelist.c
 based on trk-param-eq.c, Matt Wright, 6/10/96
 
 tracking parametric EQ from softcast, implemented to work on the Max lists 
   used as arguments to sinusoids~: alternating freq/amplitude.
 
 */
 

#define TPEQ_SINELIST_VERSION "0.0"
#define RESOURCE_ID_WITH_ASSIST_STRINGS   12346

/* the required include files */
#include "ext.h"
#include <SetUpA4.h>
#include <A4Stuff.h>
#include <math.h>

#define NUM_KNOTS 10	/* Max # knots for a voice */
#define DEFAULT_MAX_PARTIALS (128)

#define PI 3.14159265358979323f

typedef enum {
    LINEAR,
    COSINE
} interpType;

/* structure definition of your object */
typedef struct tpeq_sinelist {
	Object o_ob;				// required header
	void *o_outlet;
	
    int n;                      /* Number of knots */
    float findex[NUM_KNOTS];	/* (Fractional) partial numbers */
    float gains[NUM_KNOTS];	    /* dB cut/boost @ each index */
    int maxpartials;			/* How many partials did we alloc memory for? */
    Atom *outputlist;			/* Array of size maxpartials */
    int validFactors;		    /* How many partialFactors are still valid? */
    float *partialFactors;	    /* Stored amp factor for each partial */
    interpType interp;		    /* Interpolation method */

} tpeq_sinelist;


/* globalthat holds the class definition */
void *tpeq_sinelist_class;

/* prototypes  */

void *tpeq_sinelist_new(long maxpartials);
void tpeq_sinelist_free(tpeq_sinelist *x);
void tpeq_sinelist_version (tpeq_sinelist *x);
void tpeq_sinelist_tellmeeverything (tpeq_sinelist *x);
void tpeq_sinelist_assist (tpeq_sinelist *x, void *box, long msg, long arg, char *dstString);
void tpeq_sinelist_points(tpeq_sinelist *x, Symbol *s, short argc, Atom *argv);
void tpeq_sinelist_list(tpeq_sinelist *x, Symbol *s, short argc, Atom *argv);
float dbtolin(float x);
void RecomputeLinear(tpeq_sinelist *x, int numPartials);
void RecomputeCosine(tpeq_sinelist *x, int numPartials);
void RecomputeFunction(tpeq_sinelist *x, int numPartials);



/* initialization routine */
void main(fptr *f) {
	/* tell Max about your class. The cast to short is important for 68K */
	setup(&tpeq_sinelist_class, tpeq_sinelist_new, (method) tpeq_sinelist_free, 
		  (short)sizeof(tpeq_sinelist), 0L, A_DEFLONG, 0);
	/* bind your methods to symbols */
	addmess((method)tpeq_sinelist_assist, "assist", A_CANT, 0);
	addmess((method)tpeq_sinelist_version, "version", 0);
	addmess((method)tpeq_sinelist_tellmeeverything, "tellmeeverything", 0);
	addmess((method)tpeq_sinelist_list, "list", A_GIMME, 0);
	addmess((method)tpeq_sinelist_points, "tpe_points", A_GIMME, 0);
	
	rescopy('STR#', RESOURCE_ID_WITH_ASSIST_STRINGS);
	
	post("tpeq_sinelist object version " TPEQ_SINELIST_VERSION " by Matt Wright. ");
	post("Copyright © 1999 Regents of the University of California. All Rights Reserved.");
}


void *tpeq_sinelist_new(long maxpartials) {
	tpeq_sinelist *x;
	int i;
				
	x = newobject(tpeq_sinelist_class);		// get memory for a new object & initialize	
	x->o_outlet = listout(x);

	if (maxpartials <= 0) maxpartials = DEFAULT_MAX_PARTIALS;

	x->n = 1;
	x->findex[0] = 0.f;     /* Doesn't matter, since n==1 */
	x->gains[0] = 0.f;	    /* Sets all gains to zero Db as default */
	x->maxpartials = maxpartials;
	x->validFactors = maxpartials;
	/* Output list has 2 Atoms per partials: freq, amp */
	x->outputlist = (Atom *) getbytes(2 * maxpartials * sizeof(Atom));
	x->partialFactors = (float *) getbytes(maxpartials * sizeof(float));
	for (i = 0; i < maxpartials; ++i) {
		x->outputlist[i].a_type = A_FLOAT;
		x->partialFactors[i] = 1.0f;
	}
	x->interp = COSINE;

	return (x);
}

void tpeq_sinelist_free(tpeq_sinelist *x) {
	freebytes(x->outputlist, x->maxpartials * sizeof(*(x->outputlist)));
	freebytes(x->partialFactors, x->maxpartials * sizeof(*(x->partialFactors)));
}

void tpeq_sinelist_version (tpeq_sinelist *x) {
	post("tpeq_sinelist Version " TPEQ_SINELIST_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

void tpeq_sinelist_tellmeeverything (tpeq_sinelist *x) {
    int i;
	tpeq_sinelist_version(x);
	
	post("  x %p, x->outputlist %p, x->partialFactors %p", x, x->outputlist, x->partialFactors);
	post("  %d maxpartials, %d max points", x->maxpartials, NUM_KNOTS);
    post("  Interpolation method: %s", 
    	 x->interp == LINEAR ? "linear" : (x->interp == COSINE ? "cosine" : "unknown"));
    post("  %d knots: ", x->n);
    for (i = 0; i < x->n; i++) {
		post("    f(%f) = %f dB", x->findex[i], x->gains[i]);
    }

    post("  %d Valid partialFactors:", x->validFactors);
    for (i = 0; i < x->validFactors; i++) {
		post("    factor[%d] = %f", i, x->partialFactors[i]);
    }
    
}

void tpeq_sinelist_assist (tpeq_sinelist *x, void *box, long msg, long arg, char *dstString) {
	// 1 and 2 args are index, 1-based of first strings for inlet and outlet, respectively
	assist_string(RESOURCE_ID_WITH_ASSIST_STRINGS, msg, arg, 1, 2, dstString);
}


#define FLOATVAL(a) (((a).a_type == A_FLOAT)?((a).a_w.w_float):((a).a_w.w_long))

void tpeq_sinelist_points(tpeq_sinelist *x, Symbol *s, short argc, Atom *argv) {
	float prev;
	int i;
	
    if (argc%2 != 0) {
    	post("¥ tpeq-sinelist: tpe_points: args must be a multiple of 2 floats");
    	return;
    } 
    if (argc < 2) {
    	 post("¥ tpeq-sinelist: tpe_points: not enough arguments");
    	 return;
    } 
    
	if (argc/2 > NUM_KNOTS) {
   	    post("¥ warning: tpeq-sinelist max # points %d; ignoring extra args", NUM_KNOTS);
   	    argc = NUM_KNOTS * 2;
   	}
   	
   	for (i = 0; i < argc; ++i) {
   		if (argv[i].a_type == A_SYM) {
   			post("¥ tpeq-sinelist: tpe_points: args must all be numbers");
   			return;
   		}
   	}

    /* Make sure findex are in increasing order */
    prev = FLOATVAL(argv[0])-1.0f;
    for (i = 0; i < argc; i+= 2) {
		if (FLOATVAL(argv[i]) <= prev) {
	    	post("¥ tpeq-sinelist: tpe_points: partial indices must be increasing");
	    	return;
	    }
		prev = FLOATVAL(argv[i]);
    }

	/* Phew! */
    for (i = 0; i < argc/2; i++) {
		x->findex[i] = FLOATVAL(argv[i*2]);
		x->gains[i] = FLOATVAL(argv[i*2 + 1]);
    }

    x->n = argc/2;
    x->validFactors = 0;
}


void tpeq_sinelist_list(tpeq_sinelist *x, Symbol *s, short argc, Atom *argv) {
	int i;
	
   	for (i = 0; i < argc; ++i) {
   		if (argv[i].a_type == A_SYM) {
   			post("¥ tpeq-sinelist: freq/amp list must be all numbers");
   			return;
   		}
   	}
	
	if (argc/2 > x->maxpartials) {
		post("¥ tpeq-sinelist: warning: ignoring all but the first %ld partials in your list",
			 x->maxpartials);
	    argc = x->maxpartials * 2;
	}
	
    if (x->validFactors < argc/2) {
		RecomputeFunction(x, argc/2);
	}
	
	if ((x->n == 1) && (x->gains[0] == 0.0f)) {
		/* Pass all values unchanged. */
		outlet_list(x->o_outlet, 0L, argc, argv);
		return;
    }

    for(i=0; i<argc/2; ++i) {
    	// Freq
    	x->outputlist[2*i] = argv[2*i];	
    	
    	// Amp
		x->outputlist[2*i+1].a_w.w_float = 
			FLOATVAL(argv[2*i+1]) * x->partialFactors[i];
    }
    outlet_list(x->o_outlet, 0L, argc, x->outputlist);
}

float dbtolin(float x) {
	float result = expf(x*0.1151292546497f);
	// post("*** dbtolin(%f) = %f", x, result);
	return result;
}

void RecomputeLinear(tpeq_sinelist *x, int numPartials) {
    int i, thisKnot;
    float oneoverdeltai, deltag;


    /* set i to first uncomputed gain */
    if (x->validFactors >= numPartials) return;
    i = x->validFactors;

    for (; i < x->findex[0] && i < numPartials; i++) {
	/* Extrapolation */
	oneoverdeltai = 1.0f / (x->findex[1] - x->findex[0]);
	deltag = x->gains[1] - x->gains[0];
	x->partialFactors[i] = dbtolin(x->gains[0] + 
	    ((float) i - x->findex[0]) * oneoverdeltai * deltag);
    }

    /* Now i >= x->findex[0].  Invariant for this next loop:
       x->findex[thisKnot] <= i < x->findex[thisKnot+1]
     */

    for (thisKnot = 0; thisKnot < x->n-1; thisKnot++) {
		/* Deal with all partials between findex[thisKnot] and findex[thisKnot+1]: */

		while (i < x->findex[thisKnot+1]) {
		    oneoverdeltai = 1.0f / (x->findex[thisKnot+1] - x->findex[thisKnot]);
		    deltag = x->gains[thisKnot+1] - x->gains[thisKnot];
		    x->partialFactors[i] = dbtolin(x->gains[thisKnot] + 
				(((float) i) - x->findex[thisKnot]) * oneoverdeltai * deltag);

		    i++;
		    if (i >= numPartials) goto end;
		}
    }

    /* Now extrapolate */
    for (; i < numPartials; i++) {
		oneoverdeltai = 1.0f / (x->findex[x->n-1] - x->findex[x->n-2]);
		deltag = x->gains[x->n-1] - x->gains[x->n-2];
		x->partialFactors[i] = dbtolin(x->gains[x->n-2] + 
		    ((float) i - x->findex[x->n-2]) * oneoverdeltai * deltag);
    }

    end:
    x->validFactors = numPartials;
} 

void RecomputeCosine(tpeq_sinelist *x, int numPartials) {
    int i, thisKnot;
    float oneoverdeltai, deltag;
	
    /* set i to first uncomputed gain */
    if (x->validFactors >= numPartials) return;
    i = x->validFactors;

    for (; i < x->findex[0] && i < numPartials; i++) {
		/* Extrapolate x->gains[0] with slope of zero to minus infinity */
		x->partialFactors[i] = dbtolin(x->gains[0]);
    }

    /* Now i >= x->findex[0].  Invariant for this next loop:
       x->findex[thisKnot] <= i < x->findex[thisKnot+1]
     */

    for (thisKnot = 0; thisKnot < x->n-1; thisKnot++) {
		/* Deal with all partials between findex[thisKnot] and findex[thisKnot+1]: */

		for (; i < x->findex[thisKnot+1]; i++) {	    
		    float cosInputScale;
		    float cosVal;
		    float outScale, outOffset;

		    if (i >= numPartials) goto end;

		    /* So now we want to draw a cosine-shaped curve from the point
		       (x->findex[thisKnot], x->gains[thisKnot]) to the point
		       (x->findex[thisKnot+1], x->gains[thisKnot+1]) and find
		       the value of i on the curve. */

		    /* These values are constant with respect to i, so the compiler should
		       take them out of the inner loop: */
		    cosInputScale = PI / (x->findex[thisKnot+1] - x->findex[thisKnot]);
		    outScale = (x->gains[thisKnot] - x->gains[thisKnot+1]) * 0.5f;
		    outOffset = (x->gains[thisKnot] + x->gains[thisKnot+1]) * 0.5f;

		    cosVal = cosf(cosInputScale * ((float) i - x->findex[thisKnot]));

		    x->partialFactors[i] = dbtolin((cosVal * outScale) + outOffset);
		}
    }

    /* Now extrapolate x->gains[x->n-1] with slope of zero to infinity*/
    for (; i < numPartials; i++) {
		x->partialFactors[i] = dbtolin(x->gains[x->n-1]);
    }

    end:
    x->validFactors = numPartials;
}

void RecomputeFunction(tpeq_sinelist *x, int numPartials) {
    switch(x->interp) {
		case LINEAR:
		RecomputeLinear(x, numPartials);
		break;

		case COSINE:
		RecomputeCosine(x, numPartials);
		break;
    
		default:
		post("¥ tpeq-sinelist: uh-oh, unimplemented function type.");
    }
}
