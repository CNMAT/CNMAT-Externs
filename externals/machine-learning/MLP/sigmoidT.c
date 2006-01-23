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

	sigmoidT.c
	
	computes activations, deltas and slopes for sigmoid units
	
*******************************************************************/

#define SCOPE	extern
#include <math.h>
#include "NNInclude.h"

#define	DTYPE	double

#define	NetOFFSET	((DTYPE)0.0)
#define QUANTILE	(1024)
#define TMAX		((DTYPE)5.0)
#define	TMIN		(-(DTYPE)5.0)
/****   #define FACTOR	QUANTILE/(TMAX-TMIN) *****/
#define FACTOR	102.4	

static	float	sigTable[QUANTILE+1];

/************************* buildSigmoidT *****************************/
void	buildSigmoidT(void)
{
	int	i;
	DTYPE	inc, xx;
	
	inc = (TMAX-TMIN)/(QUANTILE);
	for (xx=TMIN, i=0;xx<=TMAX;i++, xx+=inc)
		sigTable[i] = 1/(1.0 + exp(-xx));
}


/************************* sigmoidT *****************************/
DTYPE	sigmoidT(DTYPE x,DTYPE slope, DTYPE offset)
{
	int	index;
	
	index = (((slope*x)+offset)-TMIN)*FACTOR;
	index = index<0?0:index;
	index = index>(QUANTILE)?(QUANTILE):index;
	return sigTable[index];
}

/************************* sigmoidActivation *****************************/
/*
 * function to calculate the activation of output units
 */
void	sigmoidTActivation(neuron)
unitPtr	neuron;	/* pointer to neuron */
{
#ifdef debug
	post("sigmoidTActivation - start %d",neuron->uid);
#endif

	neuron->netIn = sumInputs(neuron);		/* save this for slope learning */
	neuron->output = sigmoidT(neuron->netIn,neuron->slope,NetOFFSET);
	
#ifdef debug
	post("sigmoidTActivation - end %lf",neuron->output);
#endif
}

/************************* sigmoidTHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	sigmoidTHidden(unitPtr neuron)
{
	DTYPE	derivative;
  
	targetErrorHidden(neuron);
	derivative = neuron->slope*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET));
#ifdef debug
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmoidTOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	sigmoidTOutput(unitPtr neuron)
{
	DTYPE	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = neuron->slope*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET));
#ifdef debug	
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmoidTSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	sigmoidTSlope(unitPtr neuron,DTYPE slopeLearningRate,DTYPE slopeMomentum)
{
	DTYPE	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = neuron->netIn*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET))+0.1;					
	deltaSlope = neuron->trainingError*derivative;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;	
}

