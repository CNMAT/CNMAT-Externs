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

#ifndef _FUNCTION_H_
#define _FULCTION_H_

typedef struct Funtions {
	void	(*activation)(unitPtr);		/* activation function */
	void	(*deltaOutput)(unitPtr);	/* function to calculate delta if output unit */
	void	(*deltaHidden)(unitPtr);	/* function to calculate delta if hidden unit */
	void	(*slopeLearn)(unitPtr,double,double);		/* slope learning function */
} function,*functionPtrs;

#define	MAXFUNCTIONS	10
typedef	int	fType;
#define	SIGMOID	0
#define	LINEAR	1
#define	RADIAL	2
#define	SIGMOIDT	3
#define	SIGMAPi	4
#define	WEIGHTEDLINEAR	5

double	sigmoid(double,double,double);
void	sigmoidActivation(unitPtr);
void	sigmoidHidden(unitPtr);
void	sigmoidOutput(unitPtr);
void	sigmoidSlope(unitPtr,double,double);

double	linear(double,double);
void	linearActivation(unitPtr);
void	linearHidden(unitPtr);
void	linearOutput(unitPtr);
void	linearSlope(unitPtr,double,double);

double	radial(double,double);
void	radialActivation(unitPtr);
void	radialHidden(unitPtr);
void	radialOutput(unitPtr);
void	radialSlope(unitPtr,double,double);

void	buildSigmoidT(void);
double	sigmoidT(double,double,double);
void	sigmoidTActivation(unitPtr);
void	sigmoidTHidden(unitPtr);
void	sigmoidTOutput(unitPtr);
void	sigmoidTSlope(unitPtr,double,double);

void	sigmaPiActivation(unitPtr);
void	sigmaPiHidden(unitPtr);
void	sigmaPiOutput(unitPtr);
void	sigmaPiSlope(unitPtr,double,double);

void	linearWeightedActivation(unitPtr);
void	linearWeightedHidden(unitPtr);
void	linearWeightedOutput(unitPtr);
void	linearWeightedSlope(unitPtr,double,double);

SCOPE function	functionTable[MAXFUNCTIONS];

#endif