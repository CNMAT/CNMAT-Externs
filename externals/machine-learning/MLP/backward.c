/*******************************************************************

	backward.c
	
	back propagation learning
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

/************************* bpAdjustSlopes *****************************/

void	*bpAdjustSlopes(nnet,targets)
NNetPtr	nnet;
double *targets;
{
	int		uid;              /* processing units id */
	double	deltaSlope,derivative,target_error;
	unitPtr	neurons,neuron;

#ifdef debug
	post("bpAdjustSlopes");
#endif
  
	neurons = nnet->units;
	/* calc deltas */
	for (uid = OUT_UID(0); uid < OUT_UID(NUM_OUT); uid++) { /* for each output unit */
		neuron = neurons+uid;
		(*neuron->deltaSlopeFunction)(neuron,nnet->slopeLearningRate,nnet->slopeMomentum);
	}
}

double	LAMBDA=5.0E-03;

/************************* bpInputErrors *****************************/
/* 	note that this function will not give right answers unless it is */
/*	run AFTER the deltas for the hidden units have been calculated!  */

void	*bpInputErrors(nnet)
NNetPtr	nnet;
{
	int		i;
	unitPtr	neurons;

#ifdef debug
	post("bpInputErrors");
#endif
	neurons = nnet->units;
	/* calc deltas */
	for (i = IN_UID(NUM_IN-1); i >= IN_UID(0); i--)		/* for each input unit */
		targetErrorHidden(neurons+i);
}

/************************* bpHiddenDeltas *****************************/

void	*bpHiddenDeltas(nnet)
NNetPtr	nnet;
{
	int		i;
	unitPtr	neurons;

#ifdef debug
	post("bpHiddenDeltas");
#endif
	neurons = nnet->units;
	/* calc deltas */
	for (i = HID_UID(NUM_HID-1); i >= HID_UID(0); i--){		/* for each hidden unit */
		(*((neurons+i)->deltaWeightFunction))(neurons+i);	/* calc delta */
#ifdef debug
		post("delta for unit %d: %lf",i,(neurons+i)->deltaWeight);
#endif
	}
}

/************************* bpOutputDeltas *****************************/

void	*bpOutputDeltas(nnet)
NNetPtr	nnet;
{
	int		i;
	unitPtr	neurons;

#ifdef debug
	post("bpOutputDeltas");
#endif
	neurons = nnet->units;
	/* calc deltas */
	for (i = OUT_UID(0); i < OUT_UID(NUM_OUT); i++){		/* for each output unit */
		(*((neurons+i)->deltaWeightFunction))(neurons+i);	/* calc delta */
#ifdef debug
		post("delta for unit %d: %lf",i,(neurons+i)->deltaWeight);
#endif
	}
}

/************************* bpAdjustWeights *****************************/

void	*bpAdjustWeights(nnet)
NNetPtr	nnet;
{
	int		i;              /* processing units id */
	double	temp1, temp2, deltaWeight, error_sum;
	linkPtr	inLinkPtr,outLinkPtr;
	unitPtr	neurons;
	double	weightDecay;
	double	lr;

#ifdef debug
	post("bpAdjustWeights");
#endif
	neurons = nnet->units;
		
	/* calculate weights */
	lr = nnet->weightLearningRate;
	for (i = OUT_UID(0); i < OUT_UID(NUM_OUT); i++) {		/* for output units */
		inLinkPtr = (neurons+i)->inLinks;
		while (inLinkPtr) {        							/* for each inlink to output unit */
			weightDecay = -inLinkPtr->weight*nnet->lambda*2;
			if (neurons[i].functionIndex==SIGMAPi)
				temp1 = lr*neurons[i].deltaWeight/inLinkPtr->weight;
			else
				temp1 = lr*neurons[i].deltaWeight*inLinkPtr->fromUnit->output;

			temp2 = nnet->weightMomentum*inLinkPtr->data;
			
			inLinkPtr->data = temp1 + temp2; /* new delta weight */
			inLinkPtr->weight += inLinkPtr->data;   /* new weight */
			inLinkPtr = inLinkPtr->nextInlink;
		}
	}
	for (i = HID_UID(NUM_HID-1); i >= HID_UID(0); i--) {	/* for each hidden unit */
		inLinkPtr = (neurons+i)->inLinks;
		while (inLinkPtr) {									/* for each inlink to output unit */
			if (neurons[i].functionIndex==SIGMAPi)
				temp1 = lr*neurons[i].deltaWeight/inLinkPtr->weight;
			else
				temp1 = lr*neurons[i].deltaWeight*inLinkPtr->fromUnit->output;

			temp2 = nnet->weightMomentum*inLinkPtr->data;
			
			inLinkPtr->data = temp1 + temp2; /* new delta weight */
			inLinkPtr->weight += inLinkPtr->data;   /* new weight */
			inLinkPtr = inLinkPtr->nextInlink;
		}
	}
}

/************************* bpLearn *****************************/
/*
 * back propagation learning
 */
void	*bpLearn(NNetPtr nnet, double *exempler,double *targets,
				double *prevTargets,double *outputs, double	*err,double errorTolerance,
				Boolean backProp)
{
	recognize(nnet,exempler,outputs,prevTargets);
	*err = calculateError(nnet,targets);
	
	if (!backProp)	return;			/* used for cross validation */
	
	if ((*err>errorTolerance)&&(nnet->weightLearningRate>0.0)) {
		bpOutputDeltas(nnet);
		bpHiddenDeltas(nnet);
		bpAdjustWeights(nnet);
	}

	if ((*err>errorTolerance)&&(nnet->slopeLearningRate>0.0))	
		bpAdjustSlopes(nnet,targets);
}

/************************* bpNoPropLearn *****************************/
/*
 * back propagation learning
 */
double	bpNoPropLearn(nnet,targets)
NNetPtr	nnet;
double	*targets;
{
	int	i;
	double sum=0.0;
	
	for (i=0;i<NUM_OUT;i++) {
		(nnet->units+OUT_UID(i))->trainingError = *(targets+i);
		sum += (*(targets+i))*(*(targets+i));
	}
	
	bpOutputDeltas(nnet);
	bpHiddenDeltas(nnet);
	bpAdjustWeights(nnet);

	if (nnet->slopeLearningRate>0.0)	
		bpAdjustSlopes(nnet,targets);
	
	return sum;
}

/************************* calculateError *****************************/
/*
 * half of the sum of the squares of the errors of the
 * output versus target values
 */
double calculateError(nnet,target)
NNetPtr	nnet;
double	*target;
{
	int		i;
	double	targetError, sum=0.0;
	unitPtr	neurons;
  
#ifdef debug
	post("calculate_error");
#endif
  
	neurons = nnet->units;
	for (i = OUT_UID(0); i < OUT_UID(NUM_OUT); i++) {
		targetError = targetErrorOutput(neurons+i,*(target+TARGET_INDEX(i)));
		sum += targetError*targetError;
	}
	return (sum/2.0);
}

/************************* bpInError *****************************/
/*
 * back propagate the input error
 */
void	*bpInError(nnet,exempler,targets)
NNetPtr	nnet;
double	*exempler;
double	*targets;
{
	double	err;
	
	recognize(nnet,exempler,0L,0L);
	err = calculateError(nnet,targets);

	bpOutputDeltas(nnet);
	bpHiddenDeltas(nnet);
	bpInputErrors(nnet);
}


