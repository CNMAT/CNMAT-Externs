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

/*---------------------------------------------------------------------------*

		prototypes.h
		
*----------------------------------------------------------------------------*/
/* Matt Wright put in dummy variable names for everything on 060123 to make it compile again. */


#ifdef never
int		atoi(char *);
#endif
void	*bpAdjustSlopes(NNetPtr a,double *b);
void	*bpHiddenDeltas(NNetPtr a);
void	*bpOutputDeltas(NNetPtr a);
void	*bpAdjustWeights(NNetPtr a);
void	*bpInputErrors(NNetPtr a);
void	*bpInError(NNetPtr a,double *b,double *c);

extern	void	*bpLearn(NNetPtr a,double* b,double* v,double* d,double* e,double* f,double g,Boolean h);

double	bpNoPropLearn(NNetPtr a,double * b);
void	*buildFunctionTable(void );

double	calculateError(NNetPtr a,double * b);
NNetPtr	createNet(int a,int v,int c,int d,double e,double f,double g,double h,fType  i,fType j);

int		createIOLinks(NNetPtr a);
linkPtr	createLink(unitPtr a,linkPtr b,linkPtr c,int d,linkPtr e,int f,char * g,double h,double i);
int		createProcessingUnits(NNetPtr a,fType b,fType c);
void	*createUnit(unitPtr a,int v,char * b, int c,
					void (*d)(unitPtr,double,double),
					void (*e)(unitPtr),double f,void (*g)(unitPtr),double h);

void	*disposeGraphics(NeuralNetPtr a);
void	*drawNeuron(NeuralNetPtr a,int b);
void	*drawNeurons(NeuralNetPtr a);
void	*DrawPict(NeuralNetPtr a,int b);
void	*drawWeight(NNetPtr r,int e);
void	*drawWeights(NeuralNetPtr a);
void	*eraseGraphics(NeuralNetPtr a);
void	*freeNetwork(NNetPtr a);
#ifdef MAC
void	*GetDlgTextItem(DialogPtr a,int b,char * c);
Point	GetPictSize(int a);
Rect	GetPictRect(int a);
#endif
double	getSlopeFactorHid(NNetPtr a,long b);
double	getSlopeFactorOut(NNetPtr a,long b);
void	*getWeights(NNetPtr a,double * r);

#ifndef MAXObject
char	get_command(void);
long	get_iterations(void);
short	get_numb(char* a);
float	get_num(char* a);
short	get_str(char* a);
short	get_vector(int a ,double* b);
void 	*nn_info(short* a,short* b,short* c,short* d,double* e,double* f,double* g,double* h,short* i,short* j);
#else
void	*nn_info(NeuralNetPtr a,void * b);
void	*info(NeuralNetPtr a,void * b);
#endif

#ifdef MAC
void	*HiliteDefaultButton(DialogPtr a);
#endif

void	*initialiseInputUnits(NNetPtr a,double * b,double * v);
void	*initialiseNeurons(NeuralNetPtr a);

NeuralNetPtr	newNeuralNet(int a,int b,int c,int d,double e,double f,double g,double h,fType i,fType j);

#ifndef MAXObject
double	nn_auto_learn(NeuralNetPtr ,long);
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

void	nn_bang(NeuralNetPtr a);
#ifdef MAC
void	*nn_click(NeuralNetPtr a,Point b,int c);
#endif
void	*nn_debug(NeuralNetPtr a);
#ifdef MAXObject
void	*nn_doget_pattern(NeuralNetPtr a,patternSetPtr r,Symbol* c);
void	*nn_doread(NeuralNetPtr a,Symbol* b);
void	*nn_dowrite(NeuralNetPtr a,Symbol* b);
#else
void	*nn_doget_pattern(NeuralNetPtr a,patternSetPtr b,char* v);
void	*nn_doread(NeuralNetPtr a,char* b);
void	*nn_dowrite(NeuralNetPtr a,char* b);
#endif
void	*nn_dqfn(NeuralNetPtr a);
void	*nn_dump_test(NeuralNetPtr b);
void	*nn_dump_train(NeuralNetPtr a);
void	*nn_dump_weights(NeuralNetPtr a);
void	*nn_dump_pattern(NeuralNetPtr a,patternSetPtr b);
void	*nn_error_stats(NeuralNetPtr r,patternSetPtr a,int b);
void	*nn_error_tolerance(NeuralNetPtr a,double b);
double	euclideanDist2(double* a,double* b,int c);
#ifdef MAC
void	*nn_fields(DialogPtr a,int b,int c,int d,int e,double f,double g,double h);
#endif
void	*nn_free(NeuralNetPtr a);
void	*nn_free_memory(NeuralNetPtr a);
void	*nn_free_pattern_memory(NeuralNetPtr a,patternSetPtr r);
void	*nn_functHid(NeuralNetPtr a,Symbol* b);
void	*nn_functOut(NeuralNetPtr a,Symbol* b);
int		nn_get_memory(NeuralNetPtr a);
int		nn_get_pattern_memory(NeuralNetPtr r,patternSetPtr a);
void	*nn_get_test(NeuralNetPtr a,Symbol* b);
void	*nn_get_train(NeuralNetPtr a,Symbol* b);
void	*nn_get_weights(NeuralNetPtr a,Symbol* b);
void	*nn_graphicsOn(NeuralNetPtr a);
void	*nn_graphicsOff(NeuralNetPtr a);
void	*nn_growParams(NeuralNetPtr a);
void	*nn_hid_slope(NeuralNetPtr a,double b);
void	*nn_lambda(NeuralNetPtr a,double b);
void	*nn_learningRate(NeuralNetPtr a,double b);
void	*nn_out_slope(NeuralNetPtr a,double b);
void	*nn_out_slope_learn(NeuralNetPtr a,double b);
void	*nn_out_slope_mom(NeuralNetPtr a,double r);
void	*nn_qfn(NeuralNetPtr a);
void	*nn_reinitialise_weights(NeuralNetPtr r);
void	*nn_save(NeuralNetPtr a,void * b);
void	*nn_save_weights(NeuralNetPtr a,Symbol* b);
void	*nn_scroll(NeuralNetPtr a);
void	*nn_status(NeuralNetPtr a);
void	*nn_update(NeuralNetPtr a);
void	*nn_wsize(NeuralNetPtr a);
void	*nn_xValidateTest(NeuralNetPtr r);
void	*nn_xValidateTrain(NeuralNetPtr a);
double	nn_xValidate(NeuralNetPtr r,patternSetPtr a,int b);

#ifdef MAC
Boolean	PlacePict (int a, Point b);
#endif
void	*propagate(NNetPtr a);

#ifdef nver
short		rand(void a);
#endif
double	random(void a);
void	*recognize(NNetPtr a,double * b,double * c,double * d);

void	*scroll_it(NeuralNetPtr a,int b,int c);
#ifdef MAC
void	*SetDlgTextItem(DialogPtr a,int v,char * c,int d);
#endif
void	*setHidFunctAll(NNetPtr a,int v);
void	*setLambda(NNetPtr a,double v);
void	*setLearningRates(NNetPtr a,double b,double v);
void	*setOutFunctAll(NNetPtr a,int b);
void	*setSlopeFactorHid(NNetPtr a,int b,double c);
void	*setSlopeFactorHidAll(NNetPtr a,double b);
void	*setSlopeFactorOut(NNetPtr a,int b,double c);
void	*setSlopeFactorOutAll(NNetPtr a,double b);
void	*setSlopeLearningRate(NNetPtr a,double b);
void	*setSlopeMomentum(NNetPtr a,double b);
void	*setupGraphics(NeuralNetPtr a);

void	*setWeights(NNetPtr a,double * v,Boolean b);

double	multiplyInputs(unitPtr a);
double	sumInputs(unitPtr a);
double	sumUpStream(unitPtr a);

double	targetErrorHidden(unitPtr a);
double	targetErrorOutput(unitPtr a,double b);

void	*version(void );

void	*segment1(void ) ,*segment2(void);
