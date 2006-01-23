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

	netIO.c
	
	handles loading and unloading of weights and slope factors
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

/************************* getSlopeFactorHid *****************************/
/*
 * get output slope factor for set of units
 */
double	getSlopeFactorHid(nnet,index)
NNetPtr nnet;
long		index;
{
#ifdef debug
	post("getSlopeFactorHid");
#endif
  
	return ((nnet->units+HID_UID(index))->slope);
}

/************************* getSlopeFactorOut *****************************/
/*
 * get output slope factor for set of units
 */
double	getSlopeFactorOut(nnet,index)
NNetPtr nnet;
long		index;
{
#ifdef debug
	post("getSlopeFactorOut");
#endif
  
	return ((nnet->units+OUT_UID(index))->slope);
}

/************************* getWeights *****************************/

void *getWeights(nnet,weights)
NNetPtr	nnet;
double *weights;
{
   int     	i,j=0;
   linkPtr 	outLink;
   unitPtr	neurons;

#ifdef debug
   post("getWeights");
#endif

   neurons = nnet->units;
   
   for (i=IN_UID(0);i<IN_UID(NUM_IN);i++) {
      outLink = (neurons+i)->outLinks;
      while (outLink) {
         *(weights+j++) = outLink->weight;
         outLink = outLink->nextOutlink;
      }
   }
      
   for (i=HID_UID(0);i<HID_UID(NUM_HID);i++) {
      outLink = (neurons+i)->outLinks;
      while (outLink) {
         *(weights+j++) = outLink->weight;
         outLink = outLink->nextOutlink;
      }
   }
     
   outLink = (neurons+BIAS_UID)->outLinks;
   while (outLink) {
         *(weights+j++) = outLink->weight;
         outLink = outLink->nextOutlink;
      }
}


/************************* setLambda *****************************/
/*
 * set weight decay
 */
void	*setLambda(nnet,lambda)
NNetPtr nnet;
double	lambda;
{
#ifdef debug
	post("setLambda %lf",lambda);
#endif
  
	nnet->lambda = lambda;
}

/************************* setLearningRates *****************************/
/*
 * set weight decay
 */
void	*setLearningRates(nnet,inputLayer,hiddenLayer)
NNetPtr nnet;
double		inputLayer;
double		hiddenLayer;
{
#ifdef debug
	post("setLearningRates %lf %lf",inputLayer,hiddenLayer);
#endif
  
	nnet->inputWeightLearningRate = inputLayer;
	nnet->weightLearningRate = hiddenLayer;
}

/************************* setSlopeFactorHid *****************************/
/*
 * set output slope factor for set of units
 */
void	*setSlopeFactorHid(nnet,index,slope)
NNetPtr nnet;
int		index;
double	slope;
{
#ifdef debug
	post("slopeFactorHid %lf",slope);
#endif
  
	(nnet->units+HID_UID(index))->slope = slope;
}

/************************* setSlopeFactorHidAll *****************************/
/*
 * set output slope factor for set of units
 */
void	*setSlopeFactorHidAll(nnet,slope)
NNetPtr nnet;
double	slope;
{
  int   id;                     /* processing unit index */
 
#ifdef debug
	post("slopeFactorHidAll %lf",slope);
#endif
  
	for (id = HID_UID(0); id < HID_UID(NUM_HID); id++)
		(nnet->units+id)->slope = slope;
}

/************************* setSlopeFactorOut *****************************/
/*
 * set output slope factor for set of units
 */
void	*setSlopeFactorOut(nnet,index,slope)
NNetPtr nnet;
int		index;
double	slope;
{
  int   id;                     /* processing unit index */
 
#ifdef debug
	post("slopeFactorOut %lf",slope);
#endif
  
	(nnet->units+OUT_UID(index))->slope = slope;
}

/************************* setSlopeFactorOutAll *****************************/
/*
 * set output slope factor for set of units
 */
void	*setSlopeFactorOutAll(nnet,slope)
NNetPtr nnet;
double	slope;
{
  int   id;                     /* processing unit index */
 
#ifdef debug
	post("slopeFactorOutAll %lf",slope);
#endif
  
	for (id = OUT_UID(0); id < OUT_UID(NUM_OUT); id++) {
		(nnet->units+id)->slope = slope;
		(nnet->units+id)->deltaSlope = 0.0;
	}
}

/************************* setSlopeLearningRate *****************************/
/*
 * set slope learning rate
 */
void	*setSlopeLearningRate(nnet,rate)
NNetPtr nnet;
double	rate;
{
#ifdef debug
	post("setSlopeLearningRate %lf",rate);
#endif
  
	nnet->slopeLearningRate = rate;
}

/************************* setSlopeMomentum *****************************/
/*
 * set slope learning momentum
 */
void	*setSlopeMomentum(nnet,momentum)
NNetPtr nnet;
double	momentum;
{
#ifdef debug
	post("setSlopeMomentum %lf",momentum);
#endif
  
	nnet->slopeMomentum = momentum;
}

/************************* setWeights *****************************/

void *setWeights(NNetPtr nnet, double *weights,Boolean all)
{
   int     i,j=0;
   linkPtr outLink;
   unitPtr	neurons;

   neurons = nnet->units;

   for (i=IN_UID(0);i<IN_UID(NUM_IN);i++) {
      outLink = (neurons+i)->outLinks;
      while (outLink) {
         outLink->weight = *(weights+j++);
         if (all)
         	outLink->data = 0.0;
         outLink = outLink->nextOutlink;
      }
   }
      
   for (i=HID_UID(0);i<HID_UID(NUM_HID);i++) {
      outLink = (neurons+i)->outLinks;
      while (outLink) {
         outLink->weight = *(weights+j++);
         if (all)
         	outLink->data = 0.0;
         outLink = outLink->nextOutlink;
      }
   }
     
   outLink = (neurons+BIAS_UID)->outLinks;
   while (outLink) {
         outLink->weight = *(weights+j++);
         if (all)
         	outLink->data = 0.0;
         outLink = outLink->nextOutlink;
      }
}

/************************* setHidFunctAll *****************************/
/*
 * set activation function for hidden units
 */
void	*setHidFunctAll(nnet,index)
NNetPtr nnet;
int		index;
{
  int   id;                     /* processing unit index */
 
#ifdef debug
	post("setHidFunctAll %d",index);
#endif
  
	for (id = HID_UID(0); id < HID_UID(NUM_HID); id++) {
		nnet->units[id].functionIndex = index;
		(nnet->units+id)->deltaSlopeFunction	= functionTable[index].slopeLearn;		
		(nnet->units+id)->activationFunction	= functionTable[index].activation;
		(nnet->units+id)->deltaWeightFunction	= functionTable[index].deltaHidden;
	}
}

/************************* setOutFunctAll *****************************/
/*
 * set activation function for hidden units
 */
void	*setOutFunctAll(nnet,index)
NNetPtr nnet;
int		index;
{
  int   id;                     /* processing unit index */
 
#ifdef debug
	post("setOutFunctAll %d",index);
#endif
  
	for (id = OUT_UID(0); id < OUT_UID(NUM_OUT); id++) {
		nnet->units[id].functionIndex = index;
		(nnet->units+id)->deltaSlopeFunction	= functionTable[index].slopeLearn;		
		(nnet->units+id)->activationFunction	= functionTable[index].activation;
		(nnet->units+id)->deltaWeightFunction	= functionTable[index].deltaOutput;
	}
}
