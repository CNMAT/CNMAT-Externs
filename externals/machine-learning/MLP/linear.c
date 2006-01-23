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

	linear.c
	
	computes activations, deltas and slopes for linear units
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

/************************* linear *****************************/
double	linear(x,slope)
double	x;
double	slope;
{
	register	ret;
	
	ret = slope*x;
}

/************************* linearActivation *****************************/
/*
 * function to calculate the activation of output units
 */
void	linearActivation(neuron)
unitPtr	neuron;	/* pointer to neuron */
{
#ifdef debug
	post("linearActivation - start %d",neuron->uid);
#endif

	neuron->netIn = sumInputs(neuron);		/* save this for slope learning */
	neuron->output = linear(neuron->netIn,neuron->slope);
	
#ifdef debug
	post("linearActivation - end %lf",neuron->output);
#endif
}

/************************* linearHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	linearHidden(neuron)
unitPtr	neuron;					/* pointer to neuron */
{
	double	derivative;
  
	targetErrorHidden(neuron);
	derivative = neuron->slope;
#ifdef never	
	neuron->deltaWeight = neuron->trainingError*derivative;
#endif
									/* store delta to pass on */
}

/************************* linearOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	linearOutput(neuron)
unitPtr	neuron;			/* pointer neuron */
{
	double	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = neuron->slope;
	
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* linearSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	linearSlope(neuron,slopeLearningRate,slopeMomentum)
unitPtr	neuron;	/* pointer to neuron */
double	slopeLearningRate;
double	slopeMomentum;
{
	double	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = neuron->netIn;					
	deltaSlope = neuron->trainingError*derivative;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;
}

