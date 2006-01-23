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

/* macros to provide indexes for processing units */
#define NUM_IN			nnet->numIN
#define NUM_HID			nnet->numHID*nnet->numHIDLayers
#define NUM_OUT			nnet->numOUT

#define IN_UID(X)       (X)
#define HID_UID(X)      (NUM_IN + X)
#define OUT_UID(X)      (NUM_IN + NUM_HID + X)
#define TARGET_INDEX(X) (X - (NUM_IN + NUM_HID))

#define TOTAL           (NUM_IN + NUM_HID + NUM_OUT)
#define BIAS_UID        (TOTAL) /* threshold unit */

#define NOTCENTERED
#ifdef CENTERED
#define NET_OFFSET		(double)(-0.5)
#define NET_MAX			(double)(0.5)
#define NET_MIN			(double)(-0.5)
#else
#define NET_OFFSET		(double)(0.0)
#define NET_MAX			(double)(1.0)
#define NET_MIN			(double)(0.0)
#endif

typedef	struct Unit *unitPtr;
typedef	struct Link *linkPtr;

struct Link {			/* link between two processing units */
  char		*label;				/* name of link */
  double	weight;				/* connection weight */
  double	data;				/* used to hold the change in weights */
  double	learningRate;		/* learning rate for this link */
  unitPtr	fromUnit;			/* pointer to source Unit */
  unitPtr	toUnit;				/* pointer to destination Unit */
  linkPtr	nextInlink;			/* links are stored as linked lists */
  linkPtr	nextOutlink;
};

struct Unit {           /* general processing unit */
	short	uid;				/* unit index number */
	char	*label;				/* unit name */
	double	netIn;				/* net input */
	double	upStreamSum;
	double	slope;				/* slope factor for output function (<1 ==> more linear)*/
								/* can be used to speed convergence.  Effectively scales */
								/* the weights */
	int		functionIndex;
	void	(*deltaSlopeFunction)(unitPtr,double,double);	/* slope learning function */
	double	deltaSlope;			/* previous factor for learning */

	void	(*activationFunction)(unitPtr);	/* activation function */
	double	output;				/* activation level */
	
	void	(*deltaWeightFunction)(unitPtr);	/* ptr to function to calc delta weights */
	double	deltaWeight;		/* delta for unit */
	
	double	trainingError;		/* training error for back prop */
	
	linkPtr	inLinks;         	/* for propagation */
	linkPtr	outLinks;			/* for back propagation */

#ifdef MAXObject		/* for graphics stuff */
#ifdef GRAPHICS
	Rect	r;
	float	value;
	Rect	prev;
#endif
#endif

};

typedef struct network {
	short	numIN;						/* number of input units */
	short	numHID;						/* number of hidden units */
	short	numHIDLayers;				/* number of hidden layers */
	short	numOUT;						/* number of output units */
	double	mu;							/* recurrent feedback strength */

	double	slopeLearningRate;			/* learning rate for the slope */
	double	slopeMomentum;				/* heuristic momentum term */
	double  inputWeightLearningRate;
	double  weightLearningRate;
	double  weightMomentum;

	double  lambda;						/* weight decay */

	unitPtr	units;
	linkPtr	links;
} Network,*NNetPtr;						/* network data structure */

#define UNITSIZE	(short)sizeof(struct Unit)*(TOTAL+1)
#define LINKSIZE	(short)(sizeof(struct Link)*(nnet->numHIDLayers?\
						((nnet->numIN+nnet->numHID*(nnet->numHIDLayers-1)+\
						nnet->numOUT+nnet->numHIDLayers)*nnet->numHID+nnet->numOUT) : \
						(nnet->numOUT*(nnet->numIN+1))))

#define WEIGHTS		(x->nn_net->numHIDLayers?\
						((x->nn_net->numIN+x->nn_net->numHID*(x->nn_net->numHIDLayers-1)+\
						x->nn_net->numOUT+x->nn_net->numHIDLayers)*x->nn_net->numHID+x->nn_net->numOUT) : \
						(x->nn_net->numOUT*(x->nn_net->numIN+1)))

/********  the object	********/

typedef	struct	PatternFile
{
	short	patterns;
	char	fileName[256];

	double	*input;
	double	*target;
	double	*outz;
	double	*errz;
	
	double	pss;
	double	tss;
	double	maxError;
}patternSet,*patternSetPtr;

struct	neuralNet
{
#ifdef MAXObject
	t_box my_box;
    /* MW 020526 changed this to t_box: 	struct 	object nn_obj; */
	short	nn_theFont;
	
	void	*nn_qelem;		/* for reading & writing */
	void	*nn_dqelem;		/* for drawing */
	char	nn_read,nn_write,nn_get_test,nn_get_train;/* flags for reading & writing */

	Symbol	*nn_readFile;
	Symbol	*nn_writeFile;
	Symbol	*nn_testFile;
	Symbol	*nn_trainingFile;

	struct atom *nn_errorAtoms;	
	struct outlet *nn_myOutlet;
	struct atom *nn_AtomList;
	Windpub *myWind;
	struct symbol *windsym;
	Point	origin;
	short	xoffset;
	short	yoffset;
	Boolean	nn_all;
	Boolean	graphicsOn;
#endif
	Boolean	nn_debug;
	NNetPtr	nn_net;
	
	double	*nn_exemplers;
	double	*nn_outputs;
	double	*nn_targets;
	
	patternSet	trainingSet;
	patternSet	testSet;
		
	double	nn_error_tolerance;
	
	short		maxHid;
	short	epochGrow;
	double	errorGrow;
	double	prevPSS;
	
	short	epochs;
}

typedef	struct	neuralNet NeuralNet;

typedef struct neuralNet *NeuralNetPtr;
