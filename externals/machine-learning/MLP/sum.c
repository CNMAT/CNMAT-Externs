#define SCOPE	extern
#include "NNInclude.h"

/************************* multiplyInputs *****************************/
double	multiplyInputs(neuron)
unitPtr	neuron;
{
	register double sum = 1.0;
	linkPtr	inLink;

	inLink = neuron->inLinks;
	while (inLink) {
		/* sum up (outputs from inlinks times weights on the inlinks) */
		sum *= (inLink->fromUnit)->output * inLink->weight;
		inLink = inLink->nextInlink;
	}
	return sum;
}

/************************* sumInputs *****************************/
double	sumInputs(neuron)
unitPtr	neuron;
{
	register double sum = 0.0;
	linkPtr	inLink;

	inLink = neuron->inLinks;
	while (inLink) {
		/* sum up (outputs from inlinks times weights on the inlinks) */
		sum += (inLink->fromUnit)->output * inLink->weight;
		inLink = inLink->nextInlink;
	}
	return sum;
}

/************************* sumUpStream *****************************/
double	sumUpStream(neuron)
unitPtr	neuron;
{
	register double sum = 0.0;
	linkPtr	inLink;
	float	v;

	inLink = neuron->inLinks;
	while (inLink) {
		/* sum up (outputs from inlinks times weights on the inlinks) */
		sum += (inLink->fromUnit)->output;
#ifdef debug
printf("%f ", v=(inLink->fromUnit)->output);
#endif
		inLink = inLink->nextInlink;
	}
#ifdef debug
printf("\n");
#endif
	return sum;
}

/************************* euclideanDist2 *****************************/
double	euclideanDist2(vector1,vector2,dimension)
double	*vector1;
double	*vector2;
int		dimension;
{
	int	i;
	double	diff;
	double	sum=0.0;
	
	for (i=0;i<dimension;i++) {
		diff = *(vector1+i) - *(vector2+i);
		sum += diff*diff;
	}
	return sum;
}
