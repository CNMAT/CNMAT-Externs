/*******************************************************************

	sigmaPi.c
	
	computes activations, deltas and slopes for sigmaPi units
	
*******************************************************************/
#define SCOPE	extern
#include <math.h>
#include "NNInclude.h"
/* #include "function.h"   020526 MW removed, since NNInclude.h gets it*/

#define DTYPE	double

/************************* sigmaPiActivation *****************************/
/*
 * function to calculate the activation of output units
 */
void	sigmaPiActivation(unitPtr neuron)
{
#ifdef debug
	post("sigmaPiActivation - start %d",neuron->uid);
#endif

	neuron->netIn = multiplyInputs(neuron);		/* save this for slope learning */
	neuron->output = neuron->netIn;
	
#ifdef debug
	post("sigmaPiActivation - end %lf",neuron->output);
#endif
}

/************************* sigmaPiHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	sigmaPiHidden(unitPtr neuron)
{
	DTYPE	derivative;
  
	targetErrorHidden(neuron);
	derivative = neuron->netIn;
#ifdef debug
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmaPiOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	sigmaPiOutput(unitPtr neuron)
{
	DTYPE	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = neuron->netIn;
#ifdef debug	
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmaPiSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	sigmaPiSlope(unitPtr neuron, DTYPE slopeLearningRate, DTYPE slopeMomentum)
{
	DTYPE	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = neuron->netIn*neuron->output*(1.0 - neuron->output)+0.1;					
	deltaSlope = neuron->trainingError*derivative;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;	
}

