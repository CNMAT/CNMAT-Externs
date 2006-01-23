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

	buildFunctionTable.c
	
	constructs function table for activation and delta functions
	
*******************************************************************/

#define SCOPE	extern
#include "NNInclude.h"

void	*buildFunctionTable()
{
	functionTable[SIGMOID].activation	= sigmoidActivation;
	functionTable[SIGMOID].deltaOutput	= sigmoidOutput;
	functionTable[SIGMOID].deltaHidden	= sigmoidHidden;
	functionTable[SIGMOID].slopeLearn	= sigmoidSlope;

	functionTable[LINEAR].activation	= linearActivation;
	functionTable[LINEAR].deltaOutput	= linearOutput;
	functionTable[LINEAR].deltaHidden	= linearHidden;
	functionTable[LINEAR].slopeLearn	= linearSlope;

	functionTable[SIGMOIDT].activation	= sigmoidTActivation;
	functionTable[SIGMOIDT].deltaOutput	= sigmoidTOutput;
	functionTable[SIGMOIDT].deltaHidden	= sigmoidTHidden;
	functionTable[SIGMOIDT].slopeLearn	= sigmoidTSlope;

	functionTable[RADIAL].activation	= radialActivation;
	functionTable[RADIAL].deltaOutput	= radialOutput;
	functionTable[RADIAL].deltaHidden	= radialHidden;
	functionTable[RADIAL].slopeLearn	= radialSlope;

	functionTable[SIGMAPi].activation	= sigmaPiActivation;
	functionTable[SIGMAPi].deltaOutput	= sigmaPiOutput;
	functionTable[SIGMAPi].deltaHidden	= sigmaPiHidden;
	functionTable[SIGMAPi].slopeLearn	= sigmaPiSlope;

	functionTable[WEIGHTEDLINEAR].activation	= linearWeightedActivation;
	functionTable[WEIGHTEDLINEAR].deltaOutput	= linearWeightedOutput;
	functionTable[WEIGHTEDLINEAR].deltaHidden	= linearWeightedHidden;
	functionTable[WEIGHTEDLINEAR].slopeLearn	= linearWeightedSlope;
}