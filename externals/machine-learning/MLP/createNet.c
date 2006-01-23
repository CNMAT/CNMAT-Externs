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

	createNet.c
	
	allocate memory and initialise the structure
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

/************************* createNet *****************************/

NNetPtr createNet(int in,int hid,int hiddenLayers,int out,
				double inputLR,double learningRate,double momentum,double mu,
				fType hidFuncts,fType outFuncts)
{
	NNetPtr	nnet;
	int		err;

#ifdef debug
	post("createNet");
#endif
	if (!(nnet = (NNetPtr) getbytes((short)(sizeof(Network)))))
		return((NNetPtr)NotEnoughMemory);

	nnet->numIN			= in;
	if (hiddenLayers==0)
		nnet->numHID		= 0;
	else
		nnet->numHID = hid;
	nnet->numHIDLayers	= hiddenLayers;
	nnet->numOUT		= out;
	nnet->mu			= mu;
	
	nnet->slopeLearningRate		= 0.0;
	nnet->slopeMomentum			= 0.0;
	nnet->inputWeightLearningRate	= inputLR;
	nnet->weightLearningRate	= learningRate;
	nnet->weightMomentum		= momentum;

	nnet->lambda		= 5.0E-05;
	
	if ((err=(createProcessingUnits(nnet,hidFuncts,outFuncts)))<0)
		return((NNetPtr)err);
	if ((err=(createIOLinks(nnet)))<0)
		return((NNetPtr)err);
  
	return(nnet);
}

/************************* createProcessingUnits *****************************/
/*
 * create input, hidden, output units (and threshold or bias unit)
 */
int		createProcessingUnits(nnet,hidFuncts,outFuncts)
NNetPtr nnet;
fType	hidFuncts;
fType	outFuncts;
{
	int	id;                     /* processing unit index */
	unitPtr	neurons;
 
#ifdef debug
	post("createProcessingUnits");
#endif
	if (!(nnet->units = neurons = (unitPtr)getbytes((short)UNITSIZE)))
		return(NotEnoughMemory);

	for (id=IN_UID(0);id<IN_UID(NUM_IN);id++)
		createUnit(neurons,id,"input",(short)LINEAR,nil,nil,0.0,nil,0.0);
		
	for (id=HID_UID(0);id<HID_UID(NUM_HID);id++)
		createUnit(neurons,id,"hidden",(short)hidFuncts,functionTable[hidFuncts].slopeLearn,
					functionTable[hidFuncts].activation,0.0,
					functionTable[hidFuncts].deltaHidden,0.0);

	for (id=OUT_UID(0);id<OUT_UID(NUM_OUT);id++)
		createUnit(neurons,id,"output",(short)outFuncts,functionTable[outFuncts].slopeLearn,
					functionTable[outFuncts].activation,0.0,
					functionTable[outFuncts].deltaOutput,0.0);

	createUnit(neurons,BIAS_UID,"bias",(short)LINEAR,nil,nil,1.0,nil,1.0);
	
	return Successful;
}

/************************* createUnit *****************************/

void	*createUnit(neurons,uid,label,functionIndex,deltaSlopeFunction,activationFunction,
					output,deltaWeightFunction,deltaWeight)
unitPtr	neurons;
short	uid;
char	*label;
short	functionIndex;
void	(*deltaSlopeFunction)(unitPtr,double,double);
void	(*activationFunction)(unitPtr);
double	output;
void	(*deltaWeightFunction)(unitPtr);
double	deltaWeight;
{
	unitPtr neuron;

	neuron = neurons+uid;

	/* initialize unit data */
	neuron->uid 	= uid;
	neuron->label 	= label;
	neuron->netIn	= 0.0;
	neuron->slope	= 1.0;
	
	neuron->functionIndex = functionIndex;
	neuron->deltaSlopeFunction	= deltaSlopeFunction;
	neuron->deltaSlope = 0.0;
	
	neuron->activationFunction	= activationFunction;
	neuron->output = output;

	neuron->deltaWeightFunction	= deltaWeightFunction;
	neuron->deltaWeight = deltaWeight;
	
	neuron->inLinks = nil;
	neuron->outLinks = nil;	
}


/************************* createIOLinks *****************************/
/*
 * createIOLinks - fully connected for each layer
 *                note: the bias unit has one link to ea hid and out unit
 */
int createIOLinks(nnet)
NNetPtr nnet;
{
	int   	i,j,k;              /* i == to and j == from unit id's */
	linkPtr	freeLink;
	unitPtr	neurons;
  
#ifdef debug
	post("createIOLinks");
#endif
  
	if (!(nnet->links = (linkPtr)getbytes((short)LINKSIZE)))
		return(NotEnoughMemory);
  	
	neurons = nnet->units;
	freeLink = nnet->links;
  
	/* fully connected units */
    if (nnet->numHIDLayers>0) {
		for (i=IN_UID(0);i<IN_UID(NUM_IN);i++) 
			for (j=HID_UID(nnet->numHID-1);j>=HID_UID(0);j--)
				(neurons+j)->inLinks  =
				(neurons+i)->outLinks = createLink(neurons,freeLink++,
													(neurons+j)->inLinks, j,
													(neurons+i)->outLinks, i,
													(char *)nil,
													random(), 0.0);
     	for (k=1;k<nnet->numHIDLayers;k++) {
			for (i=HID_UID((k-1)*nnet->numHID);i<HID_UID(k*nnet->numHID);i++) 
				for (j=HID_UID((k+1)*nnet->numHID-1);j>=HID_UID(k*nnet->numHID);j--)
					(neurons+j)->inLinks  =
					(neurons+i)->outLinks = createLink(neurons,freeLink++,
													(neurons+j)->inLinks, j,
													(neurons+i)->outLinks, i,
													(char *)nil,
													random(), 0.0);
	     	}
     	
     	k = nnet->numHIDLayers;
		for (i=HID_UID((k-1)*nnet->numHID);i<HID_UID(k*nnet->numHID);i++) 
			for (j=OUT_UID(NUM_OUT)-1;j>=OUT_UID(0);j--) 
				(neurons+j)->inLinks  =
				(neurons+i)->outLinks = createLink(neurons,freeLink++,
													(neurons+j)->inLinks, j,
													(neurons+i)->outLinks, i,
													(char *)nil,
													random(), 0.0);
	}
	else {
		for (i=IN_UID(0);i<IN_UID(NUM_IN);i++) 
			for (j=OUT_UID(NUM_OUT)-1;j>=OUT_UID(0);j--) 
				(neurons+j)->inLinks  =
				(neurons+i)->outLinks = createLink(neurons,freeLink++,
													(neurons+j)->inLinks, j,
													(neurons+i)->outLinks, i,
													(char *)nil,
													random(), 0.0);
	}
     
	for (j=OUT_UID(NUM_OUT-1);j>=OUT_UID(0);j--)
		(neurons+j)->inLinks  =
		(neurons+BIAS_UID)->outLinks = createLink(neurons,freeLink++,(neurons+j)->inLinks, j,
													(neurons+BIAS_UID)->outLinks, BIAS_UID,
													(char *)nil,
													random(), 0.0);

	for (j=HID_UID(NUM_HID-1);j>=HID_UID(0);j--)
		(neurons+j)->inLinks  =
		(neurons+BIAS_UID)->outLinks = createLink(neurons,freeLink++,(neurons+j)->inLinks, j,
													(neurons+BIAS_UID)->outLinks, BIAS_UID,
													(char *)nil,
													random(), 0.0);     
	return Successful;
}


/************************* random *****************************/
/*
 * return a random number bet -0.03 and 0.03
 */
double random()
{
  return 0.5*((rand() % 32727) / 32737.0)-0.25;
}


/************************* createLink *****************************/

linkPtr	createLink(neurons,linkptr, startInlist, toId, startOutlist, fromId, 
					label, wt, data)
unitPtr	neurons;
linkPtr	linkptr;
linkPtr	startInlist;
int     toId;
linkPtr	startOutlist;
int		fromId;
char*	label;
double  wt;
double	data;
{
	/* initialize link data */
	linkptr->label 		= label;
	linkptr->weight 	= wt;
	linkptr->data 		= data;
	linkptr->fromUnit	= neurons+fromId;
	linkptr->toUnit		= neurons+toId;
	linkptr->nextInlink  = startInlist;
	linkptr->nextOutlink = startOutlist;
	
	return(linkptr);
}

/************************* freeNetwork *****************************/

void	*freeNetwork(nnet)
NNetPtr nnet;
{
	if (nnet->units) freebytes(nnet->units,(short)UNITSIZE);
	if (nnet->links) freebytes(nnet->links,(short)LINKSIZE);
}

