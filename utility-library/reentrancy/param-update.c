/* Copyright (c) 2003.  The Regents of the University of California (Regents).
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

param-update.c
	Pointer-swapping mechanism for geting atomic parameter updates for Max/MSP objects
	Matt Wright, 4/21/2003
*/


/*
	Conceptually, it's simple.  Whenever coefficients change, we linearly interpolate the 
	coefficients from the old values to the new values over one signal vector.
	
	How do we know when they've changed and keep track of the old and new values?
	It's hairy because the audio interrupt might
	come in the middle of computing the new coefficients.  In the worst case, we get new
	parameters and compute new coefficients, then get new new parameters, and, in the
	middle of computing the new new coefficients, get an audio interrupt.  From this case
	we see that we need three buffers:  the old coefficients (that we used on the last
	vector), the new coefficients (that we just computed), and temporary space to compute
	a new set of coefficients.
	
	We have four pointers to these three arrays:
		coeff - the values we'll use next time
		oldcoeff - the values we used last time
		newcoeff - where we compute new values
		freecoeff - If coeff==oldcoeff, freecoeff points to the third buffer.
		
	The default state, when the coefficients haven't changed recently, is that
	coeff==oldcoeff, and the remaining two pointers point to the remaining two buffers.
	
	After new coefficients are computed we make them take effect by setting
	coeff = newcoeff; newcoeff = freecoeff; freecoeff = 0;
	
	But maybe we already computed new coefficients since the last time the perform routine
	happened.  In that case coeff will be the "old new" coefficients, computed recently
	but not yet actually used by a perform routine.  In that case we simply swap coeff and newcoeff.
	
	The perform routine sees that coeff != oldcoeff and interpolates.  Then it sets
	freecoeff = oldcoeff; oldcoeff = coeff;

*/

#include "param-update.h"




void InitParamPointers(paramPointers *pp, void *initialValues, void *blank1, void *blank2) {
	pp->params = pp->oldparams = initialValues;
	pp->newparams = blank1;
	pp->freeparams = blank2;
}

void *CheckOutParamBlock(paramPointers *pp) {
	if (pp->newparams == 0) {
		error();
	}
	return pp->newparams;
}

void *GetCurrentParams(paramPointers *pp) {
	return pp->params;
}

void *GetPreviousParams(paramPointers *pp) {
	return pp->oldparams;
}

void CheckInNewParams(paramPointers *pp) {
	float *prevcoeffs, *prevnew, *prevfree;
	int i;

	// To make the new coefficients take effect we swap around the pointers to the
	// coefficient buffers.  See the large comment at the top of this file.
	// The audio processing interrupt might come at any time.
	
	if (x->coeff == x->oldcoeff) {
		// Normal case: these are the first new coeffients since the last perform routine		

#ifdef DEBUG		
		if (!test_normal_state(x)) post("peqbank_compute: x->coeff == x->oldcoeff");
#endif

		if (x->freecoeff == 0) {
			post("¥ peqbank: disaster!  freecoeff shouldn't be zero here!");
			   post("  coeff = %p, oldcoeff = %p, newcoeff = %p, freecoeff = %p",
   					x->coeff, x->oldcoeff, x->newcoeff, x->freecoeff);

		}
		prevcoeffs = x->coeff;
		prevnew = x->newcoeff;
		prevfree = x->freecoeff;
		
		x->freecoeff = 0;
		x->coeff = x->newcoeff;	// Now if we're interrupted the new values will be used.
		x->newcoeff = prevfree;

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: just set first new coeffs");
#endif

	} else {
		// We already computed new coeffients since the last perform routine, and now we
		// have even newer ones.  The perform routine may have interrupted this procedure
		// already, but it only would have changed oldcoeffs and freecoeffs.

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: x->coeff != x->oldcoeff");
#endif
		
		prevcoeffs = x->coeff;
		prevnew = x->newcoeff;
		
		x->coeff = x->newcoeff;
		x->newcoeff = prevcoeffs;

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: just set new new coeffs");
#endif
	}

	// Output the new coefficients out the outlet
	for (i=0; i<(x->b_nbpeq+1)*NBPARAM; i++) SETFLOAT(x->myList+i, x->coeff[i]);		
	outlet_list(x->b_outlet, 0L, (x->b_nbpeq+1)*NBPARAM, x->myList);
}


Sanity Check

    if (x->coeff == x->oldcoeff) {
    	if (!test_normal_state(x)) post("Assertions failed in tme, x->coeff == x->oldcoeff");
    } else {
    	if (!test_newcoeffs_state(x)) post("Assertions failed in tme, x->coeff != x->oldcoeff");
    }





void DoneWithPreviousParams(paramPointers *pp) {
	if (x->coeff != x->oldcoeff) {

			// Now that we've made it to the end of the signal vector, the "old" coefficients are
			// the ones we just used.  The previous "old" coeffients, which we've now finished
			// interpolating away from, are not needed any more.
			
			
			if (x->freecoeff != 0) {
				post("¥ peqbank~ disaster!  freecoeff should be zero now!");
			}
			
	#ifdef DEBUG	
			x->testcoeff3 = x->oldcoeff[3];
	#endif
			
			x->freecoeff = x->oldcoeff;
			x->oldcoeff = mycoeff;		
	}
}
