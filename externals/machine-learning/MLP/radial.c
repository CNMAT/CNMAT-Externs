/*******************************************************************

	radial.c
	
	computes activations, deltas and slopes for radial units
	
*******************************************************************/

#define SCOPE	extern
#include <math.h>
#include "NNInclude.h"
/* #include "function.h"   020526 MW removed, since NNInclude.h gets it*/

#define DTYPE	double

#define	NetOFFSET	((DTYPE)0.0)

/************************* radial *****************************/
DTYPE	radial(DTYPE x,DTYPE slope)
{
#ifdef debug
	post("radial %lf\n",x);
#endif
	return exp(-slope*x*x);
}

/************************* radialActivation *****************************/
/*
 * function to calculate the activation of output units
 */
void	radialActivation(unitPtr neuron)
{
#ifdef debug
	post("radialActivation - start %d",neuron->uid);
#endif

	neuron->netIn = sumInputs(neuron);		/* save this for slope learning */
	neuron->output = radial(neuron->netIn,neuron->slope);
	
#ifdef debug
	post("radialActivation - end %lf",neuron->output);
#endif
}

/************************* radialHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	radialHidden(unitPtr neuron)
{
	DTYPE	derivative;
  
	targetErrorHidden(neuron);
	derivative = -2*neuron->netIn*neuron->slope*neuron->output;
#ifdef debug
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* radialOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	radialOutput(unitPtr neuron)
{
	DTYPE	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = -2*neuron->netIn*neuron->slope*(neuron->output-NetOFFSET);
#ifdef debug	
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* radialSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	radialSlope(unitPtr neuron,DTYPE slopeLearningRate,DTYPE slopeMomentum)
{
	double	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = -neuron->netIn*neuron->netIn*(neuron->output-NetOFFSET);					
	deltaSlope = neuron->trainingError*derivative;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;	
}

