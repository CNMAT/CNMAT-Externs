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

