#define SCOPE	extern
#include "NNInclude.h"

/************************* initialiseInputUnits *****************************/
/*
 * initialize the input units with a specific input pattern to learn
 */
void	*initialiseInputUnits(nnet,inputPattern,prevTargetPattern)
NNetPtr	nnet;
double *inputPattern;
double *prevTargetPattern;		/* used for teacher forcing */
{
	int		id;
	unitPtr	neurons;

#ifdef debug
	post("initialiseInputUnits");
#endif
  
	neurons = nnet->units;
	for (id = IN_UID(0); id < IN_UID(NUM_IN); id++) {
	if (*(inputPattern+id) < -0.999) {						/* for feedback */
		if (prevTargetPattern) {
			(neurons+id)->output = *(prevTargetPattern+TARGET_INDEX(-(int)(*(inputPattern+id))))
								+ nnet->mu*(neurons+id)->output;
		}
		else
			(neurons+id)->output = (neurons-(int)(*(inputPattern+id)))->output
								+ nnet->mu*(neurons+id)->output;
	}
	else  	
		(neurons+id)->output = *(inputPattern+id);
	}
}

/************************* propagate *****************************/
/*
 * calculate the activation level of each unit
 */
void	*propagate(nnet)
NNetPtr	nnet;
{
	int		id;
	unitPtr	neurons;

#ifdef debug
	post("propagate - start");
#endif
  
	neurons = nnet->units;
	for (id = HID_UID(0); id < HID_UID(NUM_HID); id++)
		(*((neurons+id)->activationFunction))(neurons+id);
	for (id = OUT_UID(0); id < OUT_UID(NUM_OUT); id++)
		(*((neurons+id)->activationFunction))(neurons+id);

#ifdef debug
	post("propagate - end");
#endif
}

/************************* recognize *****************************/

void* recognize(nnet,inputPattern,outputPattern,targetPattern)
NNetPtr	nnet;
double	*inputPattern;
double	*outputPattern;
double	*targetPattern;
{
	int id;
	unitPtr	neurons;

#ifdef debug
	post("recognize - start");
#endif
  
	neurons = nnet->units;  
	initialiseInputUnits(nnet,inputPattern,targetPattern);
	propagate(nnet);
	
	if (outputPattern)
		for (id=0;id<NUM_OUT;id++)
			*(outputPattern+id) = (neurons+(int)OUT_UID(id))->output;

#ifdef debug
	for (id=0;id<NUM_IN;id++)
		post("in %d: %lf",id,(float)*(inputPattern+id));
	for (id=0;id<NUM_OUT;id++)
		post("out %d: %lf",id,(float)(neurons+(int)OUT_UID(id))->output);
	post("recognize - end");
#endif
}

