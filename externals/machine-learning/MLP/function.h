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