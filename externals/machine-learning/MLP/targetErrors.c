/* Copyright (c) 1990-2006.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Mike Lee, The Center for New Music and Audio Technologies,
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
*/

/*******************************************************************

	targetErrors.c
	
	handles error calculations for delta computations
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

/************************* targetErrorHidden *****************************/
/*	Calculate target errors for hidden units */

double	targetErrorHidden(neuron)
unitPtr	neuron;
{
	register double	errorSum=0.0;
	linkPtr		outLink;
	
	outLink = neuron->outLinks;
	while (outLink) {
		errorSum += (outLink->toUnit)->deltaWeight*outLink->weight;
		outLink = outLink->nextOutlink;
	}
	neuron->trainingError = errorSum;
	return	neuron->trainingError;
}

/************************* targetErrorOutput *****************************/
/*	Calculate target errors for output units */

double	targetErrorOutput(neuron,target)
unitPtr	neuron;
double	target;
{
	
	neuron->trainingError = target-neuron->output;
		
	return neuron->trainingError;
}

