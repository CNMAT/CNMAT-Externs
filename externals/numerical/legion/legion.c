/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: legion
DESCRIPTION: Evil Recurrant Oscillatory Self-Organizing Map
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.1: First Max version in C
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "jit.common.h"
#include "commonsyms.h"
#include "version.c"
#include "math.h"

#define VERY_SMALL DBL_MIN

// This is the neuron data structure
typedef struct t_neuron{
	double *weights;
	double *context;
	double error;
	double age;
	double period;
} t_neuron;

typedef struct t_coords{
	int row;
	int col;
} t_coords;

typedef struct _legion{
	t_object t_ob;
	void *obex;
	void *t_out;
	long t_in;
	t_neuron **t_rosom;
	int t_numColumns;
	int t_numRows;
	int t_vectorLength;
	double t_kappa;
	double t_lambda;
	double t_temperature;
	double t_learningRate;
	double t_entrainmentRate;
	double t_radius;
	int t_epoch;
	double (*t_neighborhoodFunction)(double, double);
	t_symbol *t_nfName;
	double t_nfParam; // neighborhood function parameter: for sinc this is frequency, for gaussian it's sigma
	int t_learn;
	double t_randMin, t_randMax;
} t_legion;

void *legion_class;

void legion_assist(t_legion *x, void *b, long m, long a, char *s);
void *legion_new(t_symbol *msg, short argc, t_atom *argv);
void legion_init(t_legion *x);
void legion_list(t_legion *x, t_symbol *msg, short argc, t_atom *argv);
t_coords legion_getWinner(t_legion *x, double *vec);
t_coords legion_updateNeurons(t_legion *x, double *vec);
void legion_computeWeights(t_neuron *n, int vecLen, double *vec, double learningRate, double distanceScale, double temp);
void legion_computeContext(t_neuron *n, int vecLen, double *vec, double distanceScale, double entrainmentRate);
void legion_setneighborhoodFunction(t_legion *x, t_symbol *name);
double legion_sinc(double x, double s);
double legion_gaussian(double x, double s);
double myrand(double min, double max);
void legion_outputWinnerList(t_legion *x, t_coords c);
void legion_outputErrorMatrix(t_legion *x);
void legion_free(t_legion *x);
void legion_tellmeeverything(t_legion *x);

int main(void){
	t_class *c;
	t_object *attr;
	long attrflags = 0;

	c = class_new("legion", (method)legion_new, (method)legion_free, (short)sizeof(t_legion), (method)0L, A_GIMME, 0); 
	class_obexoffset_set(c, calcoffset(t_legion, obex));
	common_symbols_init();
	
	version(0);

	class_addmethod(c, (method) version, "version", 0);
	class_addmethod(c, (method)legion_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)legion_tellmeeverything, "tellmeeverything", 0L, 0);
	class_addmethod(c, (method)legion_list, "list", A_GIMME, 0);
	
	class_addmethod(c, (method)object_obex_dumpout, "dumpout", A_CANT, 0);
	class_addmethod(c, (method)object_obex_quickref, "quickref", A_CANT, 0);

	attr = attr_offset_new("numColumns", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_numColumns));
	class_addattr(c, attr);
	attr = attr_offset_new("numRows", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_numRows));
	class_addattr(c, attr);
	attr = attr_offset_new("vectorLength", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_vectorLength));
	class_addattr(c, attr);
	attr = attr_offset_new("kappa", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_kappa));
	class_addattr(c, attr);
	attr = attr_offset_new("lambda", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_lambda));
	class_addattr(c, attr);
	attr = attr_offset_new("temperature", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_temperature));
	class_addattr(c, attr);
	attr = attr_offset_new("learningRate", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_learningRate));
	class_addattr(c, attr);
	attr = attr_offset_new("entrainmentRate", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_entrainmentRate));
	class_addattr(c, attr);
	attr = attr_offset_new("radius", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_radius));
	class_addattr(c, attr);
	attr = attr_offset_new("nfParam", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_nfParam));
	class_addattr(c, attr);
	attr = attr_offset_new("learn", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_learn));
	class_addattr(c, attr);
	attr = attr_offset_new("neighborhoodFunction", _sym_symbol,attrflags, (method)0L, (method)legion_setneighborhoodFunction, calcoffset(t_legion, t_nfName));
	class_addattr(c, attr);
	attr = attr_offset_new("randMin", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_randMin));
	class_addattr(c, attr);
	attr = attr_offset_new("randMax", _sym_float64, attrflags, (method)0L, (method)0L, calcoffset(t_legion, t_randMax));
	class_addattr(c, attr);
	class_register(CLASS_BOX, c);
	legion_class = c;
	return 0;
}

void *legion_new(t_symbol *msg, short argc, t_atom *argv){
	t_legion *x = NULL;
	if(x = (t_legion *)object_alloc(legion_class)){
		object_obex_store(x, _sym_dumpout, outlet_new(x, NULL));
		x->t_out = listout(x);

		x->t_kappa = 1.0;
		x->t_lambda = 0.5;
		x->t_temperature = 0.0;
		x->t_learningRate = 0.3;
		x->t_entrainmentRate = 0.3;
		x->t_radius = 3.0;
		x->t_epoch = 0;
		//x->t_neighborhoodFunction = legion_gaussian;
		legion_setneighborhoodFunction(x, gensym("gaussian"));
		x->t_nfParam = 1.0;
		x->t_learn = 1;
		x->t_numColumns = 10;
		x->t_numRows = 10;
		x->t_vectorLength = 10;
		x->t_randMin = 0.;
		x->t_randMax = 10.;

		attr_args_process(x, argc, argv);

		legion_init(x);
	}
	return x;
}

void legion_init(t_legion *x){
	int row, col, i;
	x->t_rosom = (t_neuron **)calloc(x->t_numRows, sizeof(t_neuron *));
	for(i = 0; i < x->t_numRows; i++)
		x->t_rosom[i] = (t_neuron *)calloc(x->t_numColumns, sizeof(t_neuron));

	for(row = 0; row < x->t_numRows; row++){
		for(col = 0; col < x->t_numColumns; col++){
			x->t_rosom[row][col].weights = (double *)calloc(x->t_vectorLength, sizeof(double));
			x->t_rosom[row][col].context = (double *)calloc(x->t_vectorLength, sizeof(double));
			for(i = 0; i < x->t_vectorLength; i++){
				x->t_rosom[row][col].weights[i] = myrand(x->t_randMin, x->t_randMax);
				x->t_rosom[row][col].context[i] = myrand(x->t_randMin, x->t_randMax);
			}

			x->t_rosom[row][col].error = 0.0;
			x->t_rosom[row][col].age = round(myrand(1.0, 10.0));
			x->t_rosom[row][col].period = round(myrand(1.0, 10.0));
		}
	}
}

void legion_list(t_legion *x, t_symbol *msg, short argc, t_atom *argv){
	double vec[argc];
	int i;
	t_coords w;
	for(i = 0; i < argc; i++){
		if(argv[i].a_type == A_FLOAT)
			vec[i] = (double)argv[i].a_w.w_float;
		else if(argv[i].a_type == A_LONG)
			vec[i] = (double)argv[i].a_w.w_long;
		else{
			error("legion: List can't contain symbols");
			return;
		}
	}
	if(x->t_learn) w = legion_updateNeurons(x, vec);
	else w = legion_getWinner(x, vec);
	legion_outputWinnerList(x, w);
	legion_outputErrorMatrix(x);
}

t_coords legion_getWinner(t_legion *x, double *vec){
	double activation = DBL_MAX;
	t_coords winner;
	winner.row = winner.col = 0;
	int row, col, i;
	double contentActivation = 0;
	double contextActivation = 0;
	double tmp = 0;
	t_neuron n;

	for(row = 0; row < x->t_numRows; row++){
		for(col = 0; col < x->t_numColumns; col++){
			contentActivation = 0;
			contextActivation = 0;
			n = x->t_rosom[row][col];
			for(i = 0; i < x->t_vectorLength; i++){
				contentActivation += pow(n.weights[i] - vec[i], 2.0);
				contextActivation += pow(n.context[i] - (n.age / n.period), 2.0);
			}
			contentActivation = sqrt(contentActivation);
			contextActivation = sqrt(contextActivation);
			x->t_rosom[row][col].error = contentActivation;
			tmp = (1 + (contentActivation * x->t_kappa) - x->t_kappa) * (1 + (contextActivation * x->t_lambda) - x->t_lambda) * (1. / n.age);
			if(tmp < activation){
				activation = tmp;
				winner.row = row;
				winner.col = col;
			}
		}
	}
	return winner;
}


t_coords legion_updateNeurons(t_legion *x, double *vec){
	double learningRate = x->t_learningRate;
	double entrainmentRate = x->t_entrainmentRate;
	//double radius = x->t_radius;
	double temperature = x->t_temperature;
	t_coords winner = legion_getWinner(x, vec);
	int row, col;
	double distanceFromWinner;
	t_neuron winningNeuron = x->t_rosom[winner.row][winner.col];
	t_neuron n;

	if(winningNeuron.age < winningNeuron.period){
		--x->t_rosom[winner.row][winner.col].period;
	}else if(winningNeuron.age > winningNeuron.period){
		++x->t_rosom[winner.row][winner.col].period;
	}
	x->t_rosom[winner.row][winner.col].age = 1;

	for(row = 0; row < x->t_numRows; row++){
		for(col = 0; col < x->t_numColumns; col++){
			distanceFromWinner = pow(row - winner.row, 2) + pow(col - winner.col, 2);
			n = x->t_rosom[row][col];

			/*
			if(distanceFromWinner <= radius){
				rosom_computeWeights(n, vec, learningRate, neighborhoodFunction(distanceFromWinner, nfParam), temperature);
				rosom_computeContext(n, vec, neighborhoodFunction(distanceFromWinner, nfParam), entrainmentRate);
			}
			*/
			legion_computeWeights(&n, x->t_vectorLength, vec, learningRate, x->t_neighborhoodFunction(distanceFromWinner, x->t_nfParam), temperature);
			legion_computeContext(&n, x->t_vectorLength, vec, x->t_neighborhoodFunction(distanceFromWinner, x->t_nfParam), entrainmentRate);

			++x->t_rosom[row][col].age;
		}
	}
	++x->t_epoch;
	return winner;
}

void legion_computeWeights(t_neuron *n, int vecLen, double *vec, double learningRate, double distanceScale, double temp){
	int i;
	double r;
	for(i = 0; i < vecLen; i++){
		r = (myrand(0, 1) * temp) - (temp / 2.);
		n->weights[i] += (learningRate * (vec[i] - n->weights[i]) * distanceScale) + r;
	}
}

void legion_computeContext(t_neuron *n, int vecLen, double *vec, double distanceScale, double entrainmentRate){
	int i;
	for(i = 0; i < vecLen; i++)
		n->context[i] += entrainmentRate * ((n->age / n->period) - n->context[i]) * distanceScale;
}

void legion_setneighborhoodFunction(t_legion *x, t_symbol *name){
	char *n = name->s_name;
	if(!strcmp(n, "gaussian")){
		x->t_neighborhoodFunction = legion_gaussian;
	}else if(!strcmp(n, "mexican_hat") || !strcmp(n, "sinc")){
		x->t_neighborhoodFunction = legion_sinc;
	}else{
		error("legion: %s is not a valid function name.  Currently, the function names available are \"gaussian\", and \"sinc\"", n);
		return;
	}
	x->t_nfName = name;
}

double legion_gaussian(double x, double s){
  	return exp(-1.0 * (pow(x, 2.0) / (2.0 * pow(s, 2.0))));
}

double legion_sinc(double x, double s){
	double th = (M_PI * s * x);
	return x == 0.0 ? 1.0 : fabs(sin(th) / th);
}

void legion_outputWinnerList(t_legion *x, t_coords c){
	t_neuron w = x->t_rosom[c.row][c.col];
	t_atom out[x->t_vectorLength];
	int i;
	for(i = 0; i < x->t_vectorLength; i++){
		SETFLOAT(&(out[i]), (float)w.weights[i]);
	}
	outlet_list(x->t_out, NULL, x->t_vectorLength, out);
}

void legion_outputErrorMatrix(t_legion *x){
	void *m;
	t_jit_matrix_info info;
	char *bp, *p;
	int row, col, i;

	jit_matrix_info_default(&info);
	info.type = _jit_sym_float64;
	info.planecount = 1;
	info.dim[0] = x->t_numRows;
	info.dim[1] = x->t_numColumns;
	t_symbol *name = jit_symbol_unique();
	m = jit_object_new(_jit_sym_jit_matrix, &info);
	//m = jit_object_method(m, _jit_sym_register, name);
	m = jit_object_register(m, name);
	//jit_object_method(m, gensym("setinfo"), &info);
	info.dimstride[0] = sizeof(double);
	info.dimstride[1] = info.dimstride[0] * info.dim[0];
	post("%d %d", info.dimstride[0], info.dimstride[1]);
	jit_object_method(m, _jit_sym_getdata, &bp);
	for(row = 0; row < x->t_numRows; row++){
		for(col = 0; col < x->t_numColumns; col++){
			i = (row * x->t_numRows) + col;
			p = bp + (i / info.dim[0]) * info.dimstride[1] + (i % info.dim[0]) * info.dimstride[0];
			*((double *)p) = x->t_rosom[row][col].error;
			post("%d [%d, %d] %f %p", i, row, col, x->t_rosom[row][col].error, p);
		}
	}
	t_atom a;
	SETSYM(&a, name);
	outlet_anything(x->t_out, _jit_sym_jit_matrix, 1, &a);
}

double myrand(double min, double max){
	return ((((double)rand()) / ((double)RAND_MAX)) * (max - min)) + min;
}

void legion_free(t_legion *x){
}

void legion_tellmeeverything(t_legion *x){
}


void legion_assist(t_legion *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"help");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"help");
			break;
		}
	}
}
