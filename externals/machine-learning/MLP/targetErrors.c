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

