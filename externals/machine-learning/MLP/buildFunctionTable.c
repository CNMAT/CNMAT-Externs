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