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

