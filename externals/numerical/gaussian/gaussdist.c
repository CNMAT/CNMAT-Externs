// gaussdist object by Tristan Jehan 
// Normal distribution generator

#include "ext.h"
#include <math.h>
#include <stdlib.h>

// CONSTANTS
#define VERSION "1.0"
#define SQRT2PI 2.506628274794649f
#define DEFMEAN 0	// Default mean
#define DEFVAR 1	// Default variance
#define RES_ID 7033

// Main structure
typedef struct _gaussdist
{
	Object x_ob;	// required header
	float x_mean;	// mean
	float x_var;	// var
	void *x_out;	// outlet

} t_gaussdist;

// globalthat holds the class definition
void *gaussdist_class;

// gaussdist prototypes
t_gaussdist *gaussdist_new(Symbol *x, short argc, Atom *argv);
void gaussdist_assist(t_gaussdist *x, void *b, long m, long a, char *s);
void gaussdist_free(t_gaussdist *x);
void gaussdist_bang(t_gaussdist *x);
void gaussdist_int(t_gaussdist *x, long n);
void gaussdist_float(t_gaussdist *x, float f);
void gaussdist_int_1(t_gaussdist *x, long n);
void gaussdist_float_1(t_gaussdist *x, float f);
void readx_mean(t_gaussdist *x, t_atom *argv);
void readx_var(t_gaussdist *x, t_atom *argv);

// Main function
void main(void) 
{
	post("gaussdist object version " VERSION " by Tristan Jehan");
    post("copyright © 2001 Massachusetts Institute of Technology");
	post("");
	
	setup(								// Setup Object
		(Messlist**) &gaussdist_class,	// Pointer to class
		(method) gaussdist_new,			// Constructor
		(method) gaussdist_free,		// Destructor
		(short) sizeof(t_gaussdist),	// Class Size
		0L,	A_GIMME, 0);

	/* bind your methods to symbols */
	addbang((method)gaussdist_bang);
	addfloat((method)gaussdist_float);
	addint((method)gaussdist_int);
	addftx((method)gaussdist_float_1,1);
	addinx((method)gaussdist_int_1,1);
	addmess((method)gaussdist_assist,"assist",A_CANT,0);

	/* list object in the new object list */
	finder_addclass("Statistics", "gaussdist");

	rescopy('STR#', RES_ID);
}

void gaussdist_assist(t_gaussdist *x, void *b, long m, long a, char *s) 
{
	assist_string(RES_ID,m,a,1,3,s);
}

void gaussdist_int(t_gaussdist *x, long n)
{
	gaussdist_float(x, (float)n);
}

void gaussdist_float(t_gaussdist *x, float f)
{
	x->x_mean = f;
}

void gaussdist_int_1(t_gaussdist *x, long n)
{
	gaussdist_float_1(x, (float)n);
}

void gaussdist_float_1(t_gaussdist *x, float f)
{
	if (f >= 0) {
		x->x_var = f;
	} else {
		x->x_var = 0.;
	}		
}

void readx_mean(t_gaussdist *x, t_atom *argv) {
    
	if (argv[0].a_type == A_LONG) {
		x->x_mean = (float)argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->x_mean = argv[0].a_w.w_float;
	} else {
		x->x_mean = DEFMEAN;
	}
}

void readx_var(t_gaussdist *x, t_atom *argv) {
    
	if (argv[1].a_type == A_LONG) {
		x->x_var = (float)argv[1].a_w.w_long;
	} else if (argv[1].a_type == A_FLOAT) {
		x->x_var = argv[1].a_w.w_float;
	} else {
		x->x_var = DEFVAR;
	}
}

t_gaussdist *gaussdist_new(Symbol *sym, short argc, Atom *argv)
{
	t_gaussdist *x = (t_gaussdist *)newobject(gaussdist_class);	

	x->x_out = floatout((t_gaussdist *)x); // Create a list outlet
	floatin(x,1); // Create 1 additional inlets

	switch (argc) { // Read arguments
		case 0: 
			x->x_var = DEFVAR;
			x->x_mean = DEFMEAN;
			break;
		case 1:
			x->x_var = DEFVAR;
			readx_mean(x,argv);
			break;
		case 2:
			readx_var(x,argv);
			readx_mean(x,argv);
			break;
		default:
			post("gaussdist: too many arguments.");		
			readx_var(x,argv);
			readx_mean(x,argv);
	}
	
	// Seed randomly the random generator
	srand(rand());

	return x;
}

void gaussdist_free(t_gaussdist *x)
{
}

void gaussdist_bang(t_gaussdist *x)
{
	float gauss, S = 2, U1,U2,V1,V2;
	
	while (S > 1) {
		U1 = (float)rand()/32678.0;
		U2 = (float)rand()/32678.0;
		V1 = 2 * U1 - 1.0;
		V2 = 2 * U2 - 1.0;
		S = V1 * V1 + V2 * V2;
	}
	gauss = sqrtf((-2 * logf(S))/S) * V1;
	gauss = gauss * sqrtf(x->x_var) + x->x_mean;
	
	outlet_float(x->x_out, gauss);
}