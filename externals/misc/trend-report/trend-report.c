/*
Copyright (c) 2002.  The Regents of the University of California (Regents).
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

  peak-report:  report data about max and min values of a stream of float events
  
*/
 
 
// #define DEBUG	
#define TRENDREPORT_VERSION "0.1"

 
#include "ext.h"

typedef struct TrendReport
{
	struct object f_ob;
	void *outlet;
	
	int numSeen;		/* Total #inputs seen */
	
	float initialValue;	/* First value of trend - a local max or min */
	int direction;		/* Is the current trend positive (1) or negative (-1) ? */
	int numInTrend; /* How many of the previous deltas have been in the same direction? */


	float prev;			/* Last value */
	float prevprev;		/* 2-ago last value */
	
	float tolerance;	/* If successive values are within this close of each other, consider the 
					       trend still to be going */
	int lastDeltaWithinTolerance; 	/* Was the last delta less than the tolerance? */
} TrendReport;


void *class;

void TrendReport_float(TrendReport *x, float f);
void *TrendReport_new(Symbol *s, float arg);

void Reset(TrendReport *x);
int compare (float x, float y, float tolerance);
void OutputTrend(TrendReport *x);



void main(fptr *f) {
	long oldA4;

	post("trend-report object version " TRENDREPORT_VERSION " by Matt Wright 9/5/02");

	setup((t_messlist **)&class, (method)TrendReport_new, 0L, (short)sizeof(TrendReport), 0L, A_DEFFLOAT, 0);
	addfloat((method)TrendReport_float);
}

void Reset(TrendReport *x) {
	x->numSeen = 0;
	x->initialValue = 0.0;
	x->direction = 0;
	x->numInTrend = 0;
	x->prev = x->prevprev = 0.0;
	x->lastDeltaWithinTolerance = 0;
}

void *TrendReport_new(Symbol *s, float arg)
{
	TrendReport *x;
	
	x = (TrendReport *)newobject(class);
	x->outlet = listout(x);
	
	if (arg == 0.0) {
		x->tolerance = 5.0f;
	} else {
		x->tolerance = arg;
	}

	Reset(x);
	return (x);
}

int compare (float x, float y, float tolerance) {
	if (x + tolerance < y) return -1;
	if (x > y + tolerance) return 1;
	return 0;
}

void OutputTrend(TrendReport *x) {
	Atom outputList[4];
	SETFLOAT(outputList+0, x->initialValue);
	SETLONG(outputList+1, x->direction);
	SETLONG(outputList+2, x->numInTrend);
	SETFLOAT(outputList+3, x->prev);
	outlet_list(x->outlet, 0L, 4, outputList);
}


void TrendReport_float(TrendReport *x, float f) {
	if (x->numSeen == 0) {
		x->initialValue = x->prevprev = x->prev = f;
		x->direction = 0;
	} else if (x->numSeen == 1) {
		/* Just to start out, use absolute < comparison with no tolerance */
		x->direction = ((f < x->initialValue) ? -1 : 1);
		x->numInTrend = 1;
		x->lastDeltaWithinTolerance = 0;
	} else {
		/* A trend has already been established */
		int comparison = compare(f, x->prev, x->tolerance);
		// post("** compare (%f, %f, %f) = %d", f, x->prev, x->tolerance, comparison);
		
		if (comparison == x->direction) {
			/* Another value in the same trend */
			// post("* trend continues...");
			++(x->numInTrend);
		} else if (comparison + x->direction == 0) {
			/* This value has broken the trend */
			// post("* trend broken: outputting...");
			OutputTrend(x);
						
			/* Therefore the previous value was a local maximum or minimum and began the current trend */
			x->initialValue = x->prev;
			x->direction = comparison;
			x->numInTrend = 1;
		} else {
			// post("* last two values are within the tolerance...");
			/* The current value is not different enough from the previous value to say the direction */
			if (!(x->lastDeltaWithinTolerance)) {
				/* The previous delta was unambiguous, so we'll coast through this round assuming the
				   current trend holds */
				   
				++(x->numInTrend);   
			} else {
				/* The previous delta value was also not very different, so our tolerance for tolerance
				   has run out.  We'll ignore the previous value and make a direct, no-tolerance comparison
				   with the value before that. */
				int newComparison = (f < x->prevprev) ? -1 : 1;
				
				if (newComparison == x->direction) {
					/* The trend holds */
					++(x->numInTrend);
				} else {
					/* We broke the trend */
					OutputTrend(x);
					x->initialValue = x->prevprev;
					x->direction = newComparison;
					x->numInTrend = 2;
				}					
					
			}
		}
		
		x->lastDeltaWithinTolerance = (comparison == 0);
		
		/* Don't hold it against us if the last delta was within the tolerance as long as the direction was
		   OK.  */  
		if (x->lastDeltaWithinTolerance) {
			int newComparison = (f < x->prev) ? -1 : 1;
			if (newComparison == x->direction) x->lastDeltaWithinTolerance = 0;
		}
	}
	
	
	x->prevprev = x->prev;
	x->prev = f;
	x->numSeen++;
	
#ifdef DEBUG	
	post("TrendReport received %f.   numSeen %d, init %f, dir %d, numTrend %d, prev %f, prevprev %f",
		 f, x->numSeen, x->initialValue, x->direction, x->numInTrend, x->prev, x->prevprev);
#endif
}


#if 0
/* Stuff for correcting overtolerance when the trend breaks:  */

					if ((x->direction ==  1 && (x->prevprev < x->prev)) ||
					    (x->direction == -1 && (x->prevprev > x->prev))) {
						/* we were wrong to count the last value in the trend */
						float realPrev = x->prev;
						
						--(x->numInTrend);
						x->prev = x->prevprev;
						OutputTrend(x);
						
						x->initialValue = x->prev = x->realPrev;
						x->direction *= -1;
						x->numInTrend = 1;

#endif