#define	SCOPE	extern
#include "NNInclude.h"

void	*nn_free_memory(x)
NeuralNet *x;
{
	if (x->nn_exemplers) freebytes(x->nn_exemplers,(short)(sizeof(double)*x->nn_net->numIN));
	if (x->nn_outputs) freebytes(x->nn_outputs,(short)(sizeof(double)*x->nn_net->numOUT));
	if (x->nn_targets) freebytes(x->nn_targets,(short)(sizeof(double)*x->nn_net->numOUT));
#ifdef MAXObject
	if (x->nn_AtomList) freebytes(x->nn_AtomList,(short)((x->nn_net->numIN+x->nn_net->numOUT)*sizeof(Atom)));
#endif
}

int	nn_get_memory(x)
NeuralNet *x;
{
	if (!(x->nn_exemplers = (double*) getbytes((short)(sizeof(double)*x->nn_net->numIN))))
		return(0);
	if (!(x->nn_outputs = (double*) getbytes((short)(sizeof(double)*x->nn_net->numOUT))))
		return(0);
	if (!(x->nn_targets = (double*) getbytes((short)(sizeof(double)*x->nn_net->numOUT))))
		return(0);
#ifdef MAXObject
	if (!(x->nn_AtomList = (Atom *)getbytes((short)((x->nn_net->numIN+x->nn_net->numOUT)*sizeof(struct atom)))))
		return(0);
#endif
	return (1);
	
}

/* ------------------------- newNeuralNet ------------------------- */

#ifdef MAXObject
void	*nn_qfn(),*nn_dqfn();
#endif

NeuralNetPtr newNeuralNet( in, hid, hid_layers, out,
							inputLR,learning_rate, momentum, mu,hidFuncts,outFuncts)
short	in;
short	hid;
short	hid_layers;
short	out;
double	inputLR;
double	learning_rate;
double	momentum;
double	mu;
fType	hidFuncts;
fType	outFuncts;
{
	NeuralNetPtr	x;
	int				i;
#ifdef MAXObject
	GrafPort		*oldGrafPort;
#endif	
#ifdef debug
	post("„newNeuralNet - start");
	post("      size:   left: %ld  top: %ld  width: %ld  height: %ld", (long)left, (long)top, (long)width, (long)height);
	post("in: %d hid: %d out: %d",in,hid,out);
	post("lr: %lf",learning_rate);
	post("mom: %lf",momentum);
	post("mu: %lf",mu);
#endif

#ifdef MAXObject
	x = (NeuralNetPtr)newobject(class);				/* initialize an instance of this class */

#ifdef USERINT
	box_new(x, patcher, F_DRAWFIRSTIN | F_NOGROW , 
			left, top, left + width, top + height);	/* fill in some fields in the box */
  	x->myBox.b_firstin = (struct inlet *)x; 		/* point inlet to object */
#endif

	x->nn_qelem = qelem_new(x, (method)nn_qfn);
	x->nn_dqelem = qelem_new(x,(method)nn_dqfn);
	x->nn_read = x->nn_write = x->nn_get_train = x->nn_get_test = 0;
	x->nn_myOutlet = listout(x);

	x->graphicsOn = false;
	x->nn_debug	= false;
	
#ifdef USERINT
	box_ready(x);
#endif

	x->myWind = 0L;
#else
	x = (NeuralNet *)getbytes((short)(sizeof(NeuralNet)));
#endif
	if ((long)(x->nn_net = createNet(in,hid,hid_layers,out,inputLR,learning_rate,momentum,mu,
								hidFuncts,outFuncts))<0L) {
		post("MaxNet:newNeuralNet - Not enough create memory");	/* create net */
		return(x);
	}
	if (!nn_get_memory(x)) {
		post("MaxNet:newNeuralNet - Not enough memory");
		return(x);
	}
	x->trainingSet.patterns = 0;
	strcpy(x->trainingSet.fileName,(const char*)"\p");
	x->testSet.patterns = 0;
	strcpy(x->testSet.fileName,(const char*)"\p");
	
	x->nn_error_tolerance = (double)0.00015*out;

	return (x);

}	/* newNeuralNet */
