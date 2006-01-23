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
