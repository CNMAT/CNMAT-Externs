/*******************************************************************

	sigmoid.c
	
	computes activations, deltas and slopes for sigmoid units
	
*******************************************************************/

#define SCOPE	extern
#include <math.h>
#include "NNInclude.h"

#define	NetOFFSET	((double)0.0)

/************************* sigmoid *****************************/
double	sigmoid(x,slope,offset)
double	x;
double	slope;
double	offset;
{
#ifdef debug
	post("sigmoid %lf %lf",x,slope);
	post("sigmoid %lf",(1/(1.0 + exp((double)-slope*x)))+offset);
#endif
	return (1/(1.0 + exp(-slope*x))+offset);
}

/************************* sigmoidActivation *****************************/
/*
 * function to calculate the activation of output units
 */
void	sigmoidActivation(neuron)
unitPtr	neuron;	/* pointer to neuron */
{
#ifdef debug
	post("sigmoidActivation - start %d",neuron->uid);
#endif

	neuron->netIn = sumInputs(neuron);		/* save this for slope learning */
	neuron->output = sigmoid(neuron->netIn,neuron->slope,NetOFFSET);
	
#ifdef debug
	post("sigmoidActivation - end %lf",neuron->output);
#endif
}

/************************* sigmoidHidden *****************************/
/*
 * function to calculate delta for hidden unit
 */
void	sigmoidHidden(neuron)
unitPtr	neuron;	/* pointer to all neurons */
{
	double	derivative;
  
	targetErrorHidden(neuron);
	derivative = neuron->slope*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET))+0.1;
#ifdef debug
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmoidOutput *****************************/
/*
 * function to calculate delta for output unit
 */
void	sigmoidOutput(neuron)
unitPtr	neuron;			/* pointer neuron */
{
	double	derivative;
  
  	/* note: neuron->trainingError calculated when first entering bpLearn */
	derivative = neuron->slope*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET))+0.1;
#ifdef debug	
	post("Unit: %d derv: %lf te: %lf",neuron->uid,derivative,neuron->trainingError);
#endif
	neuron->deltaWeight = neuron->trainingError*derivative;		/* store delta to pass on */
}

/************************* sigmoidSlope *****************************/
/*
 * function to calculate delta and learn slope
 */
void	sigmoidSlope(neuron,slopeLearningRate,slopeMomentum)
unitPtr	neuron;	/* pointer to neuron */
double	slopeLearningRate;
double	slopeMomentum;
{
	double	derivative,deltaSlope;

	/* note trainingError and input is caluclated previously in deltaWeight function */
	derivative = neuron->netIn*(neuron->output-NetOFFSET)*(1.0 - (neuron->output-NetOFFSET))+0.1;					
	deltaSlope = neuron->trainingError*derivative;	
	
	neuron->slope += slopeLearningRate*deltaSlope + slopeMomentum*neuron->deltaSlope;
	neuron->deltaSlope = deltaSlope;	
}

