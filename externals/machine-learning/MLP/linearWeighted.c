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

	linearWeighted.c
	
	computes activations, deltas and slopes for linear weigthed units
	
*******************************************************************/


#define SCOPE	extern
#include "NNInclude.h"
/* #include "function.h"   020526 MW removed, since NNInclude.h gets it*/


#define DTYPE	double

/************************* linearWeightedActivation ****************/
/*
 * function to calculate the activation of output units
 */
void	linearWeightedActivation(unitPtr neuron)
{
#ifdef debug
	post("linearWeightedActivation - start %d",neuron->uid);
#endif

	neuron->netIn = sumInputs(neuron);		/* save this for slope learning */
	neuron->upStreamSum = sumUpStream(neuron);
	neuron->output = neuron->netIn/neuron->upStreamSum;
#ifdef debug
	post("linearWeightedActivation - end %lf %lf %lf",neuron->netIn, neuron->upStreamSum, neuron->output);
#endif
}

/************************* linearWeightedHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	linearWeightedHidden(unitPtr neuron)
{
	DTYPE	derivative;
  
	targetErrorHidden(neuron);
	derivative = neuron->slope;
	neuron->deltaWeight = neuron->trainingError*derivative/neuron->upStreamSum;
				/* store delta to pass on */
}

/************************* linearWeightedOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	linearWeightedOutput(unitPtr neuron)
{
	DTYPE	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = neuron->slope;
	
	neuron->deltaWeight = neuron->trainingError*derivative/neuron->upStreamSum;		/* store delta to pass on */
}

/************************* linearWeightedSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	linearWeightedSlope(unitPtr neuron,DTYPE slopeLearningRate,DTYPE slopeMomentum)
{
	DTYPE	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = neuron->netIn;					
	deltaSlope = neuron->trainingError*derivative/neuron->upStreamSum;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;
}

