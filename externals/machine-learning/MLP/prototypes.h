/*---------------------------------------------------------------------------*

		prototypes.h
		
*----------------------------------------------------------------------------*/

#ifdef never
int		atoi(char *);
#endif
void	*bpAdjustSlopes(NNetPtr,double *);
void	*bpHiddenDeltas(NNetPtr);
void	*bpOutputDeltas(NNetPtr);
void	*bpAdjustWeights(NNetPtr);
void	*bpInputErrors(NNetPtr);
void	*bpInError(NNetPtr,double *,double *);

extern	void	*bpLearn(NNetPtr,double*,double*,double*,double*,double*,double,Boolean);

double	bpNoPropLearn(NNetPtr,double *);
void	*buildFunctionTable(void);

double	calculateError(NNetPtr,double *);
NNetPtr	createNet(int,int,int,int,double,double,double,double,fType,fType);

int		createIOLinks(NNetPtr);
linkPtr	createLink(unitPtr,linkPtr,linkPtr,int,linkPtr,int,char *,double,double);
int		createProcessingUnits(NNetPtr,fType,fType);
void	*createUnit(unitPtr,int,char *, int,
					void (*)(unitPtr,double,double),
					void (*)(unitPtr),double,void (*)(unitPtr),double);

void	*disposeGraphics(NeuralNetPtr);
void	*drawNeuron(NeuralNetPtr,int);
void	*drawNeurons(NeuralNetPtr);
void	*DrawPict(NeuralNetPtr,int);
void	*drawWeight(NNetPtr,int);
void	*drawWeights(NeuralNetPtr);
void	*eraseGraphics(NeuralNetPtr);
void	*freeNetwork(NNetPtr);
#ifdef MAC
void	*GetDlgTextItem(DialogPtr,int,char *);
Point	GetPictSize(int);
Rect	GetPictRect(int);
#endif
double	getSlopeFactorHid(NNetPtr,long);
double	getSlopeFactorOut(NNetPtr,long);
void	*getWeights(NNetPtr,double *);

#ifndef MAXObject
char	get_command(void);
long	get_iterations(void);
short	get_numb(char*);
float	get_num(char*);
short	get_str(char*);
short	get_vector(int,double*);
void 	*nn_info(short*,short*,short*,short*,double*,double*,double*,double*,short*,short*);
#else
void	*nn_info(NeuralNetPtr,void *);
void	*info(NeuralNetPtr,void *);
#endif

#ifdef MAC
void	*HiliteDefaultButton(DialogPtr);
#endif

void	*initialiseInputUnits(NNetPtr,double *,double *);
void	*initialiseNeurons(NeuralNetPtr);

NeuralNetPtr	newNeuralNet(int,int,int,int,double,double,double,double,fType,fType);

#ifndef MAXObject
double	nn_auto_learn(NeuralNetPtr,long);
void	*nn_recognize(NeuralNetPtr,double*,double*);
#else
void	*nn_auto_learn(NeuralNetPtr,long);
void	*nn_bpInputError(NeuralNetPtr,Symbol *,int,Atom *);
void	*nn_bplearn(NeuralNetPtr,Symbol *,int,Atom *);
void	*nn_bpNoPropLearn(NeuralNetPtr,Symbol *,int,Atom *);
void	*nn_file(Symbol *,int,Atom *);
void	*nn_learn(NeuralNetPtr,Symbol *,int,Atom *);
void	*nn_Menu(struct patcher *,long,long,int);
void	*nn_outlet_list(struct outlet *,char *,int,Atom *);
void	*nn_recognize(NeuralNetPtr,Symbol *,int,Atom *);
#endif

void	nn_bang(NeuralNetPtr);
#ifdef MAC
void	*nn_click(NeuralNetPtr,Point,int);
#endif
void	*nn_debug(NeuralNetPtr);
#ifdef MAXObject
void	*nn_doget_pattern(NeuralNetPtr,patternSetPtr,Symbol*);
void	*nn_doread(NeuralNetPtr,Symbol*);
void	*nn_dowrite(NeuralNetPtr,Symbol*);
#else
void	*nn_doget_pattern(NeuralNetPtr,patternSetPtr,char*);
void	*nn_doread(NeuralNetPtr,char*);
void	*nn_dowrite(NeuralNetPtr,char*);
#endif
void	*nn_dqfn(NeuralNetPtr);
void	*nn_dump_test(NeuralNetPtr);
void	*nn_dump_train(NeuralNetPtr);
void	*nn_dump_weights(NeuralNetPtr);
void	*nn_dump_pattern(NeuralNetPtr,patternSetPtr);
void	*nn_error_stats(NeuralNetPtr,patternSetPtr,int);
void	*nn_error_tolerance(NeuralNetPtr,double);
double	euclideanDist2(double*,double*,int);
#ifdef MAC
void	*nn_fields(DialogPtr,int,int,int,int,double,double,double);
#endif
void	*nn_free(NeuralNetPtr);
void	*nn_free_memory(NeuralNetPtr);
void	*nn_free_pattern_memory(NeuralNetPtr,patternSetPtr);
void	*nn_functHid(NeuralNetPtr,Symbol*);
void	*nn_functOut(NeuralNetPtr,Symbol*);
int		nn_get_memory(NeuralNetPtr);
int		nn_get_pattern_memory(NeuralNetPtr,patternSetPtr);
void	*nn_get_test(NeuralNetPtr,Symbol*);
void	*nn_get_train(NeuralNetPtr,Symbol*);
void	*nn_get_weights(NeuralNetPtr,Symbol*);
void	*nn_graphicsOn(NeuralNetPtr);
void	*nn_graphicsOff(NeuralNetPtr);
void	*nn_growParams(NeuralNetPtr);
void	*nn_hid_slope(NeuralNetPtr,double);
void	*nn_lambda(NeuralNetPtr,double);
void	*nn_learningRate(NeuralNetPtr,double);
void	*nn_out_slope(NeuralNetPtr,double);
void	*nn_out_slope_learn(NeuralNetPtr,double);
void	*nn_out_slope_mom(NeuralNetPtr,double);
void	*nn_qfn(NeuralNetPtr);
void	*nn_reinitialise_weights(NeuralNetPtr);
void	*nn_save(NeuralNetPtr,void *);
void	*nn_save_weights(NeuralNetPtr,Symbol*);
void	*nn_scroll(NeuralNetPtr);
void	*nn_status(NeuralNetPtr);
void	*nn_update(NeuralNetPtr);
void	*nn_wsize(NeuralNetPtr);
void	*nn_xValidateTest(NeuralNetPtr);
void	*nn_xValidateTrain(NeuralNetPtr);
double	nn_xValidate(NeuralNetPtr,patternSetPtr,int);

#ifdef MAC
Boolean	PlacePict (int, Point);
#endif
void	*propagate(NNetPtr);

#ifdef nver
short		rand(void);
#endif
double	random(void);
void	*recognize(NNetPtr,double *,double *,double *);

void	*scroll_it(NeuralNetPtr,int,int);
#ifdef MAC
void	*SetDlgTextItem(DialogPtr,int,char *,int);
#endif
void	*setHidFunctAll(NNetPtr,int);
void	*setLambda(NNetPtr,double);
void	*setLearningRates(NNetPtr,double,double);
void	*setOutFunctAll(NNetPtr,int);
void	*setSlopeFactorHid(NNetPtr,int,double);
void	*setSlopeFactorHidAll(NNetPtr,double);
void	*setSlopeFactorOut(NNetPtr,int,double);
void	*setSlopeFactorOutAll(NNetPtr,double);
void	*setSlopeLearningRate(NNetPtr,double);
void	*setSlopeMomentum(NNetPtr,double);
void	*setupGraphics(NeuralNetPtr);

void	*setWeights(NNetPtr,double *,Boolean);

double	multiplyInputs(unitPtr);
double	sumInputs(unitPtr);
double	sumUpStream(unitPtr);

double	targetErrorHidden(unitPtr);
double	targetErrorOutput(unitPtr,double);

void	*version(void);

void	*segment1(void),*segment2(void);
