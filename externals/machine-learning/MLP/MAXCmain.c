/*------------------------------------------------------------------*

	MAXCmain.c
	
*-------------------------------------------------------------------*/


#include "NNInclude.h"

/* ------------------------- nn_auto_learn ------------------------ */

void	*nn_dump_weights();

#ifndef MAXObject
double	nn_auto_learn(x,n)
#else
void	*nn_auto_learn(x,n)
#endif
NeuralNetPtr	x;
long		n;
{
	short	i,j;
	double	sum,pss;
	double	delta_error,maxError;
#ifdef MAXObject
	EventRecord	myEvent;
	Cursor	**theCurse;
	short	whichOne=myRes;
#endif

	
	
	if (x->trainingSet.patterns) {
	
#ifdef MAXObject
		theCurse = GetCursor(myRes);
		SetCursor(*theCurse);
#endif
		
		j = 0;
		sum = 1.0;
		pss = x->nn_error_tolerance+1;		/* get in the loop */

		while((pss > x->nn_error_tolerance)&&(j<n)) {
			sum = 0.0;
			maxError = 0.0;
			for (i=0;i<x->trainingSet.patterns;i++) {
				bpLearn(x->nn_net,((x->trainingSet.input)+i*x->nn_net->numIN),
						((x->trainingSet.target)+i*x->nn_net->numOUT),
						((x->trainingSet.target)+(i-1)*x->nn_net->numOUT),
						((x->trainingSet.outz)+i*x->nn_net->numOUT),
						((x->trainingSet.errz)+i),x->nn_error_tolerance,true);
						
				sum += *((x->trainingSet.errz)+i);
				maxError = MAX(maxError,*((x->trainingSet.errz)+i));
			}
			
#ifdef multiFinderFriendly
				WaitNextEvent(everyEvent,&myEvent,0,nil);
#endif

			if ((pss=sum/x->trainingSet.patterns)<x->nn_error_tolerance) {
				x->prevPSS = pss;
				post("Error criteria met.");
				break;		/* don't grow if already okay */
			}
			
			if (j==0)			/* first time only */
				x->prevPSS = pss;
#ifdef GROW
			if (!((j+1)%x->epochGrow)){
				delta_error = (x->prevPSS-pss)/x->prevPSS;
				if (delta_error < x->errorGrow) {
					x->epochs = j+1;				
					if (!nn_grow(x))
						break;
					post("Epochs Executed: %d",j+1);
				}
				x->prevPSS = pss;
			}
#endif
			j++;
			
#ifdef USERINT
			if((j!=0)&&(!(j%10))) {				
				if (whichOne==myRes)
					whichOne=myRes1;
				else
					whichOne=myRes;
					
			    DrawPict(x,whichOne);
			}
#endif
			
			if ((n>10)&&(!(j%(n/10)))&&j!=0) {
				nn_xValidate(x,&x->trainingSet,x->epochs+j);
				nn_xValidate(x,&x->testSet,x->epochs+j);
			}
		}
		post("Epochs Executed: %d",j);
		x->epochs += j;
		post("Total Epochs Executed: %d",x->epochs);
		x->prevPSS = pss;				
		post("PSS: %lf",pss);
		post("TSS: %lf",sum);
		post("maxError: %lf",maxError);
		nn_xValidate(x,&x->trainingSet,x->epochs);
		nn_xValidate(x,&x->testSet,x->epochs);
		
#ifdef MAXObject
		InitCursor();
#endif
	}
#ifdef MAXObject
	SETFLOAT((x->nn_errorAtoms),(float)sum);
	nn_outlet_list(x->nn_myOutlet,"err",1,x->nn_errorAtoms);
#endif
	
		
}

#ifndef MAXObject

/* ------------------------- nn_dump_weights --------------------- */

void	*nn_dump_weights(x)
NeuralNet	*x;
{
	double	*weights;
	long	i;
	
	weights = (double*)malloc(sizeof(double)*WEIGHTS);	
	getWeights(x->nn_net,weights);
	for (i=0;i<MIN(20,WEIGHTS);i++) {
		printf("%ld: %lf\n",i,(float)*(weights+i));
	}
	
	for (i=0;i<x->nn_net->numOUT;i++)
		printf("%ld: %lf\n",i,getSlopeFactorOut(x->nn_net,i));
		
	free(weights);
}

#else

/* ------------------------- nn_dump_weights --------------------- */

void	*nn_dump_weights(x)
NeuralNet	*x;
{
	Atom	*AtomList;
	double	*weights;
	long	i;
	
	
	
	AtomList = (Atom *) getbytes((short)(WEIGHTS*(short)(sizeof(Atom))));
	if (!AtomList) {
		post("dumpWeights: Not enough memory! %lx",AtomList);
		goto outtaHere;
	}
	weights = (double*)getbytes((short)(sizeof(double)*WEIGHTS));	
	if (!weights) {
		post("dumpWeights: Not enough memory! %lx",weights);
		goto outtaHere;
	}
	getWeights(x->nn_net,weights);
	for (i=0;i<WEIGHTS;i++) {
		SETFLOAT(AtomList+i,(float)*(weights+i));
		post("%ld: %lf",i,(float)*(weights+i));
	}
	
	for (i=0;i<x->nn_net->numOUT;i++) {
		post("%ld: %lf",i,getSlopeFactorOut(x->nn_net,i));
	}
		
	nn_outlet_list(x->nn_myOutlet,"weights",WEIGHTS,AtomList);
	freebytes(AtomList,(short)(sizeof(double)*WEIGHTS));
	freebytes(weights,(short)(sizeof(double)*WEIGHTS));
outtaHere:
	1+1;
}

#endif

/* ------------------------- nn_dump_pattern --------------------- */

void	*nn_dump_pattern(x,set)
NeuralNetPtr	x;
patternSetPtr	set;
{
	long	i,j;
	
	for (i=0;i<set->patterns;i++) {
		for (j=0;j<x->nn_net->numIN;j++) {
			post("pattern %ld input unit %ld value %lf",i,j,*((set->input)+(i*x->nn_net->numIN)+j));
		}
		for (j=0;j<x->nn_net->numOUT;j++){
			post("pattern %ld output unit %ld value %lf",i,j,*((set->target)+(i*x->nn_net->numOUT)+j));
		}
	}
}

#ifndef MAXObject

/* ------------------------- nn_info ------------------------- */

void *nn_info(in,hid,hid_layers,out,inputLR,learning_rate,momentum,mu,hidFuncts,outFuncts)
short		*in,*hid,*hid_layers,*out;
double	*inputLR,*learning_rate,*momentum,*mu;
short		*hidFuncts,*outFuncts;
{
	Boolean	ask=false;
	if (*in<0) {
		*in = (short)(get_num("in"));
		*hid = (short)(get_num("hid"));
		*hid_layers = (short)(get_num("hid_layers"));
		*out = (short)(get_num("out"));
		ask = true;
	}
	*inputLR = get_num("input layer learning_rate");
	*learning_rate = get_num("learning_rate");
	*momentum = get_num("momentum");
	*mu = get_num("mu");
	if ((*in<0)||ask) {
		*hidFuncts = get_num("Hidden Function Index");
		*outFuncts = get_num("Output Function Index");
	}
}	

/* ------------------------- nn_growParams ------------------------- */

void *nn_growParams(x)
NeuralNet	*x;
{
	x->maxHid = (short)(get_num("MaxHid"));
	x->errorGrow = get_num("errorGrow");
	x->epochGrow = (short)(get_num("epochGrow"));
	x->nn_error_tolerance = get_num("error tolerance");
}	

/* ------------------------- nn_recognize ------------------------ */

void	*nn_recognize(x,vector,out)
NeuralNet	*x;
double		*vector,*out;
{
	recognize(x->nn_net,vector,out,0L);
}

/* ------------------------- nn_reinitialise_weights --------------------- */

extern	double	random();

void	*nn_reinitialise_weights(x)
NeuralNet	*x;
{
	double	*weights;
	short		i,j,index;
	
	weights = (double*)malloc(sizeof(double)*WEIGHTS);

	index = 0;
	
	if (x->nn_net->numHIDLayers==0) {
		for (i=0;i<x->nn_net->numIN;i++) {
			for (j=0;j<x->nn_net->numOUT;j++,index++) {
				*(weights+index) = random();
			}				
		}
	}
	else {
		for (i=0;i<x->nn_net->numIN;i++) {
			for (j=0;j<x->nn_net->numHID;j++,index++) {
				*(weights+index) = random();
			}				
		}
	
		for (i=0;i<x->nn_net->numHID*x->nn_net->numHIDLayers;i++) {
			for (j=0;j<x->nn_net->numOUT;j++,index++)
				*(weights+index) = random();
		}
	}			

	for (i=0;i<x->nn_net->numHID*x->nn_net->numHIDLayers;i++,index++) {
		*(weights+index) = random();
	}
	
	for (i=0;i<x->nn_net->numOUT;i++,index++) {
		*(weights+index) = random();
	}
	
	setWeights(x->nn_net,weights,true);

	for (i=0;i<x->nn_net->numOUT;i++)
		setSlopeFactorOutAll(x->nn_net,1.0);
	
	x->epochs = 0;
	
	free(weights);
}

#else

/* ------------------------- nn_info ------------------------- */

void *nn_info(x,p)
	NeuralNet *x;
	void *p;
{
	short	savelock;
	
	
	savelock = lockout_set(1);
	info(x,p);
	lockout_set(savelock);
	
}	

/* ------------------------- nn_recognize ------------------------ */

void	*nn_recognize(x,s,argc,argv)
	NeuralNet	*x;
	Symbol	*s;
	short	argc;
	Atom	*argv;
{
	short	i;
	
	
	
	if (x->nn_debug) {
		post("RECOGNIZE in: %ld",gettime());
	}
	if (argc < x->nn_net->numIN) {
	   post("Not Enough Inputs");
	}
	else {
		for (i=0;i<x->nn_net->numIN;i++) {
			switch(argv[i].a_type) {
				case A_LONG:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_long;
					break;
				case A_SYM:
					i--;
					break;
				case A_FLOAT:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_float;
					break;
			}
		}
		recognize(x->nn_net,x->nn_exemplers,x->nn_outputs,0L);
		if (x->nn_debug) {
			post("RECOGNIZE out: %ld",gettime());
		}
		for (i=0;i<x->nn_net->numOUT;i++) {
		   SETFLOAT((x->nn_AtomList+i),(float)*(x->nn_outputs+i));
#ifdef debug
		   post("%d: %lf",i,(float)(x->nn_AtomList+i)->a_w.w_float);
#endif
		}		   
		nn_outlet_list(x->nn_myOutlet,"list",x->nn_net->numOUT,x->nn_AtomList);
	}
	qelem_set(x->nn_dqelem);
	
}

/* ------------------------- nn_reinitialise_weights --------------------- */

extern	double	random();

void	*nn_reinitialise_weights(x)
	NeuralNet	*x;
{
	double	*weights;
	short		i,j,index;
	
	
	
	weights = (double*)getbytes((short)(sizeof(double)*WEIGHTS));

	index = 0;
	for (i=0;i<x->nn_net->numIN;i++) {
		for (j=0;j<x->nn_net->numHID;j++,index++) {
			*(weights+index) = random();
		}				
	}

	for (i=0;i<x->nn_net->numHID;i++) {
		for (j=0;j<x->nn_net->numOUT;j++,index++)
			*(weights+index) = random();
	}				

	for (i=0;i<x->nn_net->numHID;i++,index++) {
		*(weights+index) = random();
	}
	
	for (i=0;i<x->nn_net->numOUT;i++,index++) {
		*(weights+index) = random();
	}
	
	setWeights(x->nn_net,weights,true);
	freebytes(weights,(short)(sizeof(double)*WEIGHTS));

	for (i=0;i<x->nn_net->numOUT;i++)
		setSlopeFactorOutAll(x->nn_net,1.0);

	
}

#endif

#ifndef MAXObject

char get_command()
{
  char  command[BUFSIZ];
  
  printf("\nEnter Command (a)utolearn (g)et weights (s)ave weights (t)raining data (i)nfo\n ");
  printf("(r)ecognize (d)ump weights (o)set slope for outputs (p)dump patterns: \n");
#ifdef never
  fflush(stdin); fflush(stdout);
#endif
  scanf("%s",command);
  return((command[0]));         /* return 1st letter of command */
}

long get_iterations()
{
  long	iter;
  
  printf("\nEpochs: ");
#ifdef never
  fflush(stdin); fflush(stdout);
#endif
  scanf("%ld",&iter);
  return(iter);
}

short get_vector(n,vector)
short	n;
double	*vector;
{
	short	i;
	
	printf("\nVector: ");
#ifdef never
	fflush(stdin); fflush(stdout);
#endif
	for (i=0;i<n;i++)
  		(void)fscanf(stdin,"%lf",vector+i);
}

float get_num(prompt)
char	*prompt;
{
	float	num;
	printf("%s: ",prompt);
#ifdef never
	fflush(stdin); fflush(stdout);
#endif  
	scanf("%f",&num);
	return(num);
}

short get_str(string)
char	*string;
{
  char  buf[BUFSIZ];
  
#ifdef never
  fflush(stdin); fflush(stdout);
#endif
  scanf("%s",string);
}

/* ------------------------- main   ------------------------- */

double	*vector,*outVector;

main()
{
	char	command;
	char	filename[256];
	NeuralNetPtr	net;
	short	in=-1,hid,hid_layers,out;
	short	hidFuncts,outFuncts;
	long	iter;
	short	i;
	double	inputLR,learning_rate,momentum,mu;
	double	error;
	
	version();
	
	nn_info(&in,&hid,&hid_layers,&out,&inputLR,&learning_rate,&momentum,&mu,&hidFuncts,&outFuncts);
	
	vector = (double*)malloc(sizeof(double)*(size_t)in);
	outVector = (double*)malloc(sizeof(double)*(size_t)out);

	buildFunctionTable();
	
	net = newNeuralNet(in,hid,hid_layers,out,inputLR,learning_rate,momentum,mu,hidFuncts,outFuncts);

	net->epochs = 0;

	while((command=get_command())!='q'){
		switch(command) {
			case 'a':
#ifndef GROW
				iter = get_iterations();
#else
				nn_growParams(net);
				iter = 2000000;
#endif
				nn_auto_learn(net,iter);
				break;
			case 'A':
				printf("In: %d\n",net->nn_net->numIN);
				printf("Hidden: %d\n",net->nn_net->numHID);
				printf("Hidden Layers: %d\n",net->nn_net->numHIDLayers);
				printf("out: %d\n",net->nn_net->numOUT);
				printf("Error: %lf\n",net->nn_error_tolerance);
				printf("Lambda: %lf\n",net->nn_net->lambda);
				printf("Input Layer Learning Rate: %lf\n",net->nn_net->inputWeightLearningRate);
				printf("Learning Rate: %lf\n",net->nn_net->weightLearningRate);
				printf("Momentum: %lf\n",net->nn_net->weightMomentum);
				printf("Slope Learning Rate: %lf\n",net->nn_net->slopeLearningRate);
				printf("Slope Momentum: %lf\n",net->nn_net->slopeMomentum);
				break;
			case 'c':
				nn_reinitialise_weights(net);
				break;
			case 'd':
				nn_dump_weights(net);
				break;
			case 'e':
				get_vector(1,&net->nn_error_tolerance);
				printf("Error Tolerance: %lf\n",net->nn_error_tolerance);
				break;
			case 'f':
				hidFuncts = get_num("Hidden Function Index");
				setHidFunctAll(net->nn_net,hidFuncts);
				break;
			case 'F':
				outFuncts = get_num("Output Function Index");
				setOutFunctAll(net->nn_net,outFuncts);
				break;
			case 'g':
				printf("read weight file: ");
				get_str(filename);
				nn_doread_stdio(net,filename);
				break;
			case 'h':
				get_vector(1,vector);
				setSlopeFactorHidAll(net->nn_net,*vector);
				break;
			case 'i':
				nn_info(&in,&hid,&hid_layers,&out,
						&net->nn_net->inputWeightLearningRate,
						&net->nn_net->weightLearningRate,
						&net->nn_net->weightMomentum,
						&mu,&hidFuncts,&outFuncts);
				break;
			case 'l':
				printf("weight decay: ");
				get_vector(1,vector);
				setLambda(net->nn_net,*vector);
				break;
			case 's':
				printf("write weight file: ");
				get_str(filename);
				nn_dowrite_stdio(net,filename);
				break;
			case 'S':
				printf("slope learning rate: ");
				get_vector(1,vector);
				setSlopeLearningRate(net->nn_net,*vector);
				printf("slope momentum: ");
				get_vector(1,vector);
				setSlopeMomentum(net->nn_net,*vector);
				break;
			case 'o':
				get_vector(1,vector);
				setSlopeFactorOutAll(net->nn_net,*vector);
				break;
			case 't':
				printf("get training file: ");
				get_str(filename);
				nn_doget_pattern(net,&net->trainingSet,filename);
				break;
			case 'T':
				printf("get test file: ");
				get_str(filename);
				nn_doget_pattern(net,&net->testSet,filename);
				break;
			case 'p':
				nn_dump_pattern(net,&net->trainingSet);
				break;
			case 'P':
				nn_dump_pattern(net,&net->testSet);
				break;
			case 'r':
				get_vector(in,vector);
				nn_recognize(net,vector,outVector);
				for (i=0;i<out;i++)
					printf("%lf ",*(outVector+i));
				printf("\n");
				break;
			case 'v':
				printf("Training Error PSS: %f\n",nn_xValidate(net,&net->trainingSet,0));
				break;
			case 'V':
				printf("Test Error PSS: %f\n",nn_xValidate(net,&net->testSet,0));
				break;
		}
	}
}

#else

/* ------------------------- nn_bang ------------------------- */

void	nn_bang(x)
NeuralNet	*x;
{
 	short	i;
 	
	
#ifdef debug
	post("¥nn_bang - start");
#endif

	for (i=0;i<x->nn_net->numOUT;i++) {
	   SETFLOAT((x->nn_AtomList+i),(float)*(x->nn_outputs+i));
#ifdef debug
	   post("%d: %lf",i,(float)(x->nn_AtomList+i)->a_w.w_float);
#endif
	}		   
	nn_outlet_list(x->nn_myOutlet,"list",x->nn_net->numOUT,x->nn_AtomList);

	
}	/* nn_bang */

/* ------------------------- nn_bpInputError ------------------------ */

void	*nn_bpInputError(x,s,argc,argv)
NeuralNet	*x;
Symbol	*s;
short	argc;
Atom	*argv;
{
	short i,j;
	double error_measure;
	
	
	
	if (argc < (x->nn_net->numIN+x->nn_net->numOUT)) {
	   post("Not Enough Inputs");
	}
	else {
		for (i=0;i<x->nn_net->numIN;i++) {
			switch(argv[i].a_type) {
				case A_LONG:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_float;
					break;
			}
		}
		for (j=0;j<x->nn_net->numOUT;i++,j++) {
			switch(argv[i].a_type) {
				case A_LONG:
				    *(x->nn_targets+j) = (double) argv[i].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_targets+j) = (double) argv[i].a_w.w_float;
					break;
			}
		}
		bpInError(x->nn_net,x->nn_exemplers,x->nn_targets);
		for (i=0;i<x->nn_net->numIN;i++)
			SETFLOAT((x->nn_AtomList+i),(float)((x->nn_net->units+i)->trainingError));
		nn_outlet_list(x->nn_myOutlet,"bplearn",x->nn_net->numIN,x->nn_AtomList);
	}
	
}

/* ------------------------- nn_bplearn ------------------------ */

void	*nn_bplearn(x,s,argc,argv)
NeuralNet	*x;
Symbol	*s;
short	argc;
Atom	*argv;
{
	short	j;
	long	iter;
	double error_measure;
	
	

	if (x->nn_debug) {
		post("BPLEARN in: %ld",gettime());
	}
	iter = argv[0].a_w.w_long;
	argc--;
	argv++;
	if (argc < x->nn_net->numOUT) {
	   post("Not Enough Inputs");
	}
	else {
		for (j=0;j<x->nn_net->numOUT;j++) {
			switch(argv[j].a_type) {
				case A_LONG:
				    *(x->nn_targets+j) = (double) argv[j].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_targets+j) = (double) argv[j].a_w.w_float;
					break;
			}
		}
		for (j=0;j<iter;j++)
			bpLearn(x->nn_net,x->nn_exemplers,
					x->nn_outputs,
					x->nn_targets,
					0L,
					&error_measure,x->nn_error_tolerance,true);
			
	if (x->nn_debug) {
		post("BPLEARN out: %ld",gettime());
	}
#ifdef debug
	post("Error: %lf",error_measure);
#endif
	}
	
}

/* ------------------------- nn_bpNoPropLearn ------------------------ */
/* backward pass only */
void	*nn_bpNoPropLearn(x,s,argc,argv)
NeuralNet	*x;
Symbol	*s;
short	argc;
Atom	*argv;
{
	short	j;
	double	err;
	Atom	myAtom;
	
	

	if (x->nn_debug) {
		post("BPLEARN in: %ld",gettime());
	}
	if (argc < x->nn_net->numOUT) {
	   post("Not Enough Inputs");
	}
	else {
		for (j=0;j<x->nn_net->numOUT;j++) {
			switch(argv[j].a_type) {
				case A_LONG:
				    *(x->nn_targets+j) = (double) argv[j].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_targets+j) = (double) argv[j].a_w.w_float;
				    if (x->nn_debug)
				    	post("Target %d: %lf",j,(float) *(x->nn_targets+j));
					break;
			}
		}
		err = bpNoPropLearn(x->nn_net,x->nn_targets);
		SETFLOAT(&myAtom,err);
		nn_outlet_list(x->nn_myOutlet,"err",(short)1,&myAtom);
	}
	if (x->nn_debug)
		post("BPLEARN out: %ld",gettime());

	
}

/* ------------------------- nn_click ------------------------- */

void	*nn_click(x,where, modifiers)
NeuralNet	*x;
Point	where;
short	modifiers;
{
	short	i;
	
	
#ifdef debug
	post("¥nn_click - start");
#endif
    if (!box_nodraw(&(x->my_box)))
#ifdef never
		wind_defaultscroll(x->myWind,where);
#endif

	for (i=0;i<x->nn_net->numOUT;i++) {
	   SETFLOAT((x->nn_AtomList+i),(float)*(x->nn_outputs+i));
#ifdef debug
		post("%d: %lf",i,(float)(x->nn_AtomList+i)->a_w.w_float);
#endif
	}		   
	nn_outlet_list(x->nn_myOutlet,"list",x->nn_net->numOUT,x->nn_AtomList);
	
}	/* nn_click */

/* ------------------------- nn_debug ------------------------- */

void	*nn_debug(x)
NeuralNet	*x;
{
	
	
	x->nn_debug = !x->nn_debug;
	if (x->nn_debug) {
		post("MAXNET: debug on.");
	}
	else {
		post("MAXNET: debug off.");
	}
	
	
}	/* nn_click */

/* ------------------------- nn_dump_test --------------------- */

void	*nn_dump_test(x)
NeuralNet	*x;
{
	
	nn_dump_pattern(x,&x->testSet);
	
}
	
/* ------------------------- nn_dump_train --------------------- */

void	*nn_dump_train(x)
NeuralNet	*x;
{
	
	nn_dump_pattern(x,&x->trainingSet);
	
}
	
/* ------------------------- nn_error_tolerance --------------------- */

void *nn_error_tolerance(x,err)
NeuralNet	*x;
double		err;
{
	
	x->nn_error_tolerance = err;
	
}

/* ------------------------- nn_create ------------------------- */
void	*nn_create(long in,long hid,long hid_layers,long out,Symbol *weightFile);

void	*nn_create(long in,long hid,long hid_layers,long out,Symbol *weightFile)
{
	NeuralNet	*x;	
	float	learning_rate=0.1;
	float	momentum=0.1;
	float	mu=0.1;
	
	
#ifdef debug
	post("¥nn_create - start");
#endif
	x = newNeuralNet((short)in, (short)hid, (short)hid_layers,(short)out,
						(double)learning_rate,(double)learning_rate,
						(double)momentum,(double)mu,SIGMOID,SIGMOID);				
	if (weightFile!=gensym(""))
		nn_get_weights(x,weightFile);
		
	
	return(x);

}	/* nn_file */

#ifdef NEVER
/* ------------------------- nn_File ------------------------- */

void	*nn_file(Symbol *s)
{
	NeuralNet	*x;
	long	in;
	long	hid;
	long	hid_layers;
	long	out;
	float	learning_rate=0.1;
	float	momentum=0.1;
	float	mu=0.1;
	Symbol	*weightFile;
	
	
#ifdef debug
	post("¥nn_file - start");
#endif

	in = argv[1].a_w.w_long;
	hid = argv[2].a_w.w_long;
	hid_layers = argv[3].a_w.w_long;
	out = argv[4].a_w.w_long;
	learning_rate = argv[5].a_w.w_float;
	momentum = argv[6].a_w.w_float;
	mu = argv[7].a_w.w_float;
	if (argc>7)
		weightFile = argv[8].a_w.w_sym;
	else
		weightFile = gensym("");

	res = newNeuralNet((short)in, (short)hid, (short)hid_layers,(short)out,
						(double)learning_rate,(double)learning_rate,
						(double)momentum,(double)mu,SIGMOID,SIGMOID);

	if (s!=gensym(""))
		nn_get_weights(x,weightFile);
		
	
	return(x);
}	/* nn_file */
#endif

/* ------------------------- nn_free ------------------------- */

void *nn_free(x)
NeuralNet *x;
{
	
	qelem_free(x->nn_qelem);
	qelem_free(x->nn_dqelem);
#ifdef GRAPHICS
	if (x->graphicsOn)
		disposeGraphics(x);
#endif
	nn_free_memory(x);
	if (x->testSet.patterns > 0)
		nn_free_pattern_memory(x,&x->testSet);
	if (x->trainingSet.patterns > 0)
		nn_free_pattern_memory(x,&x->trainingSet);
	if (x->nn_net) {
		freeNetwork(x->nn_net);
		if (x->nn_net) freebytes(x->nn_net,(short)(sizeof(Network)));
	}
#ifdef USERINT
	box_free(x);
#endif
	
}

/* ------------------------- nn_functHid --------------------- */

void *nn_functHid(NeuralNet *x,Symbol *fnType)
{
	int	index=SIGMOID;
	
	if (fnType==gensym("sigmoid"))
		index = SIGMOID;
	else if (fnType==gensym("sigmoidTable"))
		index = SIGMOIDT;
	else if (fnType==gensym("linear"))
		index = LINEAR;
	else if (fnType==gensym("radial"))
		index = RADIAL;
	else if (fnType==gensym("sigmapi"))
		index = SIGMAPi;
	else if (fnType==gensym("weightedLinear"))
		index = WEIGHTEDLINEAR;
		
	setHidFunctAll(x->nn_net,index);
	
}

/* ------------------------- nn_functOut --------------------- */

void *nn_functOut(NeuralNet *x,Symbol *fnType)
{
	int	index=SIGMOID;
	
	

	if (fnType==gensym("sigmoid"))
		index = SIGMOID;
	else if (fnType==gensym("sigmoidTable"))
		index = SIGMOIDT;
	else if (fnType==gensym("linear"))
		index = LINEAR;
	else if (fnType==gensym("radial"))
		index = RADIAL;
	else if (fnType==gensym("sigmapi"))
		index = SIGMAPi;
	else if (fnType==gensym("weightedLinear"))
		index = WEIGHTEDLINEAR;
	
		
	setOutFunctAll(x->nn_net,index);
	
}

/* -------------------- nn_get_test ------------------- */

void *nn_get_test(NeuralNet *x,Symbol *fileName)
{
	
	x->nn_get_test = 1;
	x->nn_testFile = fileName;
	qelem_set(x->nn_qelem);
	
}

/* -------------------- nn_get_train ------------------- */

void *nn_get_train(NeuralNet *x,Symbol *fileName)
{
	
	x->nn_get_train = 1;
	x->nn_trainingFile = fileName;
	qelem_set(x->nn_qelem);
	
}

/* ------------------------- nn_get_weights --------------------- */

void *nn_get_weights(NeuralNet *x,Symbol *fileName)
{
	
	x->nn_read = 1;
	x->nn_readFile = fileName;
	qelem_set(x->nn_qelem);
	
}
	
/* ------------------------- nn_graphicsOn --------------------- */

void *nn_graphicsOn(x)
	NeuralNet	*x;
{
	
	
	if (!x->graphicsOn) {
		x->graphicsOn = true;
#ifdef GRAPHICS
		setupGraphics(x);
#endif
	}
	
	
}
	
/* ------------------------- nn_graphicsOff --------------------- */

void *nn_graphicsOff(x)
	NeuralNet	*x;
{
	
	x->graphicsOn = false;
#ifdef GRAPHICS
	disposeGraphics(x);
#endif
	
}
	
/* ------------------------- nn_hid_slope --------------------- */

void *nn_hid_slope(x,slope)
	NeuralNet	*x;
	double		slope;
{
	
	setSlopeFactorHidAll(x->nn_net,slope);
	
}

/* ------------------------- nn_lambda --------------------- */

void *nn_lambda(x,lambda)
NeuralNet	*x;
double		lambda;
{
	
	setLambda(x->nn_net,lambda);
	
}

/* ------------------------- nn_learningRate --------------------- */

void *nn_learningRate(x,inputLayer)
NeuralNet	*x;
double		inputLayer;
{
	
	setLearningRates(x->nn_net,inputLayer,inputLayer);
	
}

/* ------------------------- nn_momentum --------------------- */

void *nn_momentum(NeuralNet	* x, double mom);
void *nn_momentum(NeuralNet	* x, double mom) {
	
	x->nn_net->weightMomentum = mom;
	
}

/* ------------------------- nn_mu --------------------- */

void *nn_mu(NeuralNet *x, double mu);
void *nn_mu(NeuralNet *x, double mu) {
	x->nn_net->mu = mu;
	
}

/* ------------------------- nn_learn ------------------------ */

void	*nn_learn(x,s,argc,argv)
NeuralNet	*x;
Symbol	*s;
short	argc;
Atom	*argv;
{
	short	i,j;
	double error_measure;
	
	
	
	if (argc < (x->nn_net->numIN+x->nn_net->numOUT)) {
	   post("Not Enough Inputs");
	}
	else {
		for (i=0;i<x->nn_net->numIN;i++) {
			switch(argv[i].a_type) {
				case A_LONG:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_exemplers+i) = (double) argv[i].a_w.w_float;
					break;
			}
		}
		for (j=0;j<x->nn_net->numOUT;i++,j++) {
			switch(argv[i].a_type) {
				case A_LONG:
				    *(x->nn_targets+j) = (double) argv[i].a_w.w_long;
					break;
				case A_SYM:
					break;
				case A_FLOAT:
				    *(x->nn_targets+j) = (double) argv[i].a_w.w_float;
					break;
			}
		}
		bpLearn(x->nn_net,x->nn_exemplers,x->nn_targets,0L,x->nn_outputs,
				&error_measure,x->nn_error_tolerance,true);
#ifdef debug
	post("Error: %lf",error_measure);
#endif
	}
	
}

#ifdef USERINT
/* ------------------------- nn_Menu ------------------------- */

void	*nn_Menu(p, x, y, font)
struct	patcher *p;
long	x;
long	y;
short	font;
{
	NeuralNet	*res;
	Point		tempPt;

	
	
#ifdef debug
	post("¥nn_Menu - start");
	post("     x is: %ld  y is: %ld", (long)x, (long)y);
#endif
	tempPt = GetPictSize(myRes);
	tempPt.h = tempPt.h;
	tempPt.v = tempPt.v;
	res = newNeuralNet(p,(short)x,(short)y, tempPt.h, tempPt.v, 
						(short)2, (short)2, (short)1, (short)1,
						(double)0.2,(double)0.2,(double)0.9,
						(double)0.5,SIGMOID,SIGMOID);
	res->nn_theFont = font;	
	
	
	return(res);	
}	/* nn_Menu */
#endif

/* ------------------------- nn_outlet_list   ------------------------- */

void	*nn_outlet_list(outletPtr,sym,out,list)
struct	outlet	*outletPtr;
char	*sym;
short	out;
Atom	*list;
{
	short	savelock;
	
	savelock = lockout_set(1);
	outlet_anything(outletPtr,gensym(sym),out,list);
	lockout_set(savelock);
}

/* ------------------------- nn_out_slope --------------------- */

void *nn_out_slope(x,slope)
	NeuralNet	*x;
	double		slope;
{
	
	setSlopeFactorOutAll(x->nn_net,slope);
	
}

/* ------------------------- nn_out_slope_learn --------------------- */

void *nn_out_slope_learn(x,learning_rate)
NeuralNet	*x;
float		learning_rate;
{
	
	setSlopeLearningRate(x->nn_net,learning_rate);
	
}

/* ------------------------- nn_out_slope_mom --------------------- */

void *nn_out_slope_mom(x,momentum)
	NeuralNet	*x;
	double		momentum;
{
	
	setSlopeMomentum(x->nn_net,momentum);
	
}



/* ------------------------- nn_dqfn ------------------------- */

void	*nn_dqfn(x)
NeuralNet	*x;
{
#ifdef GRAPHICS
	short		savelock;
	GrafPort	*oldGrafPort;

	
#ifdef debug
	post("¥nn_dqfn - start");
#endif
	savelock = lockout_set(1);
#ifdef never
	oldGrafPort = patcher_setport(x->myBox.b_patcher);
    if (oldGrafPort) {
    	if (!box_nodraw(x))
    		DrawPict(x,myRes);
		SetPort(oldGrafPort);
	}
#endif
#ifdef GRAPHICS
	if (x->graphicsOn) {
		drawNeurons(x);
		if (x->nn_all) {
			drawWeights(x);
		}
		x->nn_all = false;
	}
#endif
#ifdef debug
	post("     nn_dqfn - end");
#endif
	lockout_set(savelock);
#endif	
}	/* nn_dqfn */

/* ------------------------- nn_qfn ---------------------------- */

void *nn_qfn(x)
	NeuralNet	*x;
{
	short	savelock;
	
	
	
	savelock = lockout_set(1);
	if (x->nn_read) {
		nn_doread(x,x->nn_readFile);
		x->nn_read = 0;
		nn_update(x);
	}
	if (x->nn_write) {
		nn_dowrite(x,x->nn_writeFile);
		x->nn_write = 0;
	}
	if (x->nn_get_test) {
		nn_doget_pattern(x,&x->testSet,x->nn_testFile);
		x->nn_get_test = 0;
	}
	
	if (x->nn_get_train) {
		nn_doget_pattern(x,&x->trainingSet,x->nn_trainingFile);
		x->nn_get_train = 0;
	}
	lockout_set(savelock);
	
}

#ifdef never
/* ------------------------- nn_save ------------------------- */

void *nn_save(x, w)
NeuralNet *x;
void	*w;
{
	Rect *r;
	
	
	r = &x->myBox.b_rect;
	
	if (x->myBox.b_hidden) {
		binbuf_vinsert(w,"sssslllllllfff", gensym("#P"),
			gensym("MaxNet"), (long)(r->left), (long)(r->top),
				(long)(r->right - r->left), (long)(r->bottom - r->top), 
				(long)x->nn_net->numIN,
				(((long)x->nn_net->numHID)<<16)+(long)x->nn_net->numHIDLayers,
				(long)x->nn_net->numOUT,
				(float)x->nn_net->weightLearningRate,
				(float)x->nn_net->weightMomentum,
				(float)x->nn_net->mu);
	}
	else {
		binbuf_vinsert(w,"ssslllllllfff", gensym("#P"),gensym("user"), 
			gensym("MaxNet"), (long)(r->left), (long)(r->top),
				(long)(r->right - r->left), (long)(r->bottom - r->top), 
				(long)x->nn_net->numIN,
				(((long)x->nn_net->numHID)<<16)+(long)x->nn_net->numHIDLayers,
				(long)x->nn_net->numOUT,
				(float)x->nn_net->weightLearningRate,
				(float)x->nn_net->weightMomentum,
				(float)x->nn_net->mu);
	}
			
}	/* nn_save */

#endif

/* ------------------------- nn_save_weights --------------------- */

void	*nn_save_weights(NeuralNetPtr x,Symbol *fileName)
{
	
	x->nn_write = 1;
	x->nn_writeFile = fileName;
	qelem_set(x->nn_qelem);
	
}

/* ------------------------- nn_scroll --------------------- */

void	*nn_scroll(x)
NeuralNet	*x;
{
	Point	delta;
	
	
#ifdef GRAPHICS
	delta.h = x->myWind->w_xoffset - x->xoffset;
	delta.v = x->myWind->w_yoffset - x->yoffset;
	scroll_it(x,delta.h,delta.v);
	x->xoffset = x->myWind->w_xoffset;
	x->yoffset = x->myWind->w_yoffset;
	nn_update(x);
#endif
	
}

/* ------------------------- nn_status --------------------- */

void	*nn_status(net)
	NeuralNet	*net;
{
	

	post("In: %d",net->nn_net->numIN);
	post("Hidden: %d",net->nn_net->numHID);
	post("Hidden Layers: %d",net->nn_net->numHIDLayers);
	post("out: %d",net->nn_net->numOUT);
	post("Error: %lf",net->nn_error_tolerance);
	post("Input Layer Learning Rate: %lf",net->nn_net->inputWeightLearningRate);
	post("Learning Rate: %lf",net->nn_net->weightLearningRate);
	post("Momentum: %lf",net->nn_net->weightMomentum);
	post("Slope Learning Rate: %lf",net->nn_net->slopeLearningRate);
	post("Slope Momentum: %lf",net->nn_net->slopeMomentum);

	
}

/* ------------------------- nn_update ------------------------- */

void	*nn_update(x)
NeuralNet	*x;
{
#ifdef GRAPHICS
GrafPort  *	oldGrafPort;
	int	savelock;
	
	if (x->nn_debug)
		post("¥nn_update - start");

	x->nn_all = true;
	
#ifdef USERINT
	oldGrafPort = patcher_setport(x->myBox.b_patcher);
    if (oldGrafPort) {
    	if (!box_nodraw(x))
    		DrawPict(x,myRes);
		SetPort(oldGrafPort);
	}
#endif
	qelem_set(x->nn_dqelem);
	
#endif	
}	/* nn_update */

/* ------------------------- nn_wsize ------------------------- */

void	*nn_wsize(x)
NeuralNet	*x;
{
	
#ifdef debug
	post("¥nn_wsize - start");
#endif
	x->nn_all = true;
	qelem_set(x->nn_dqelem);
	
	
}	/* nn_wsize */

/* ------------------------- nn_xValidateTest ------------------------- */

void	*nn_xValidateTest(x)
NeuralNet	*x;
{
	
	nn_xValidate(x,&x->testSet,0);
	
}	/* nn_xValidateTest */

/* ------------------------- nn_xValidateTrain ------------------------- */

void	*nn_xValidateTrain(x)
NeuralNet	*x;
{
	
	nn_xValidate(x,&x->trainingSet,0);
	
}	/* nn_xValidateTrain */

/* ------------------------- main   ------------------------- */

void main(fptr *f)
{
	CInfoRec ct;
		
		
	setup((t_messlist **)&class, (method)nn_create, (method) nn_free, 
		  (short)sizeof(struct neuralNet), 0L, 
		  A_LONG,A_LONG,A_LONG,A_LONG,A_DEFSYM, 0);

	/* the arguments are: in hid hidlayers out */

	addbang((method)nn_bang);
	addmess((method) nn_auto_learn,"auto_learn",A_LONG,0);
	addmess((method) nn_bpInputError,"bpInError",A_GIMME,0);
	addmess((method) nn_bplearn,"bplearn",A_GIMME,0);
	addmess((method) nn_bpNoPropLearn,"back",A_GIMME,0);
 	addmess((method) nn_click, "click", A_CANT, 0);
 	addmess((method) nn_debug, "debug", 0);
	addmess((method) nn_dump_test,"dump_test",0);
	addmess((method) nn_dump_train,"dump_train",0);
	addmess((method) nn_dump_weights,"dump",0);
	addmess((method) nn_error_tolerance,"error",A_FLOAT,0);
	addmess((method) nn_functHid,"hidFunct",A_SYM,0);
	addmess((method) nn_functOut,"outFunct",A_SYM,0);
#ifdef NEVER
	addmess((method) nn_info, "info", A_CANT,0);
#endif
	addmess((method) nn_get_test,"get_test_data",A_DEFSYM,0);
	addmess((method) nn_get_train,"get_training_data",A_DEFSYM,0);
	addmess((method) nn_get_weights,"get_weights",A_DEFSYM,0);
	addmess((method) nn_graphicsOn,"graphicsOn",0);
	addmess((method) nn_graphicsOff,"graphicsOff",0);
	addmess((method) nn_hid_slope,"hid_slope",A_FLOAT,0);
	addmess((method) nn_lambda,"lambda",A_FLOAT,0);
	addmess((method) nn_learningRate,"learningRate",A_FLOAT,0);
	addmess((method) nn_momentum,"momentum",A_FLOAT,0);
	addmess((method) nn_mu,"mu",A_FLOAT,0);
	addmess((method) nn_learn,"learn",A_GIMME,0);
	addmess((method) nn_recognize,"list",A_GIMME,0);
	addmess((method) nn_out_slope,"out_slope",A_FLOAT,0);
	addmess((method) nn_out_slope_learn,"out_slope_learn",A_FLOAT,0);
	addmess((method) nn_out_slope_mom,"out_slope_mom",A_FLOAT,0);
	addmess((method) nn_recognize,"recognize",A_GIMME,0);
	addmess((method) nn_reinitialise_weights,"reinitialise",0);
#ifdef NEVER
	addmess((method) nn_save, "psave", A_CANT, 0);
#endif
	addmess((method) nn_save_weights,"save_weights",A_DEFSYM,0);
	addmess((method) nn_scroll,"scroll",A_CANT,0);
	addmess((method) nn_status,"status",0);
	addmess((method) nn_update, "update", A_CANT, 0);
	addmess((method) nn_wsize,"wsize",A_CANT,0);
	addmess((method) nn_xValidateTest,"xValidateTest",0);
	addmess((method) nn_xValidateTrain,"xValidateTrain",0);

	rescopy('PICT',myRes);
	rescopy('PICT',myRes1);
	rescopy('DLOG',myRes);
	rescopy('DITL',myRes);
	rescopy('CURS',myRes);
	
	
/* MW removed segment stuff 021211 	
	{
		short	i,segments;
		Handle	hand;
		
		short	id;
		OSType	type;
		Str255	name;
		
		segment1();
		segment2();
		segments = Count1Resources('CCOD');
		for (i=1;i<=segments;i++) {
			hand = Get1IndResource('CCOD',i);
			GetResInfo(hand,&id,&type,name);
			rescopy('CCOD',id);
		}
	}
*/

	/* eventually get these from a colorinfo() trap */
	hasColorQD = 1;
	numPaletteColors = 256;
	GetIndPattern(&blk,sysPatListID,1);
	GetIndPattern(&wht,sysPatListID,20);
	colorinfo(&ct);
	hasColorQD = ct.c_hasColorQD;
	numPaletteColors = ct.c_depth==8? 256 : (ct.c_depth==4? 16 : 2);
	
	buildFunctionTable();
	buildSigmoidT();
	version();
	finder_addclass("Data","mlp");
	
	
}
#endif

void	*version()
{
	post("mlp v2.3 Michael Lee, Matt Wright");
	post("copyright © 1990-2003 Regents of the University of California. All Rights Reserved.");
}
	
