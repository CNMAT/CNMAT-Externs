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

