// gaussdraw object by Tristan Jehan 
// Normal distribution list output

#include "ext.h"
#include <math.h>

// CONSTANTS
#define VERSION "1.0"
#define TWOPI 6.28318530717952646f
#define SQRT2PI 2.506628274794649f
#define MAXNUMPTS 512	// Default maximum number of pts
#define DEFMEAN 64		// Default mean
#define DEFVAR 20		// Default variance
#define DEF_N 128		// Default number of points
#define DEFMIN 0		// Default minimum
#define DEFMAX 127		// Default maximum
#define DEFHEIGHT 127	// Default height
#define RES_ID 7034

// Main structure
typedef struct _gaussdraw
{
	Object x_ob;		// required header
	float x_mean;		// mean
	float x_var;		// var
	int x_N;			// Number of points
	float x_min;		// Minimum
	float x_max;		// Maximum
	float x_height;		// Height
	void *x_outlet;		// List outlet
	Atom *myList;		// Copy of outputs as Atoms

} t_gaussdraw;

// globalthat holds the class definition
void *gaussdraw_class;

// gaussdraw prototypes
t_gaussdraw *gaussdraw_new(Symbol *x, short argc, Atom *argv);
void gaussdraw_free(t_gaussdraw *x);
void gaussdraw_bang(t_gaussdraw *x);
void gaussdraw_int(t_gaussdraw *x, long n);
void gaussdraw_float(t_gaussdraw *x, float f);
void gaussdraw_int_1(t_gaussdraw *x, long n);
void gaussdraw_float_1(t_gaussdraw *x, float f);
void gaussdraw_int_2(t_gaussdraw *x, long n);
void gaussdraw_float_2(t_gaussdraw *x, float f);
void gaussdraw_assist(t_gaussdraw *x, void *b, long m, long a, char *s);
void gaussdraw_min(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv);
void gaussdraw_max(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv);
void gaussdraw_numpts(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv);
void gaussdraw_default(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv);
void gaussdraw_print(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv);
void readx_N(t_gaussdraw *x, t_atom *argv);
void readx_height(t_gaussdraw *x, t_atom *argv);
void readx_min(t_gaussdraw *x, t_atom *argv);
void readx_max(t_gaussdraw *x, t_atom *argv);

// Main function
void main(void) 
{
	post("gaussdraw object version " VERSION " by Tristan Jehan");
    post("copyright © 2001 Massachusetts Institute of Technology");
	post("");
	
	setup(								// Setup Object
		(Messlist**) &gaussdraw_class,	// Pointer to class
		(method) gaussdraw_new,			// Constructor
		(method) gaussdraw_free,		// Destructor
		(short) sizeof(t_gaussdraw),	// Class Size
		0L,	A_GIMME, 0);

	/* bind your methods to symbols */
	addbang((method)gaussdraw_bang);
	addfloat((method)gaussdraw_float);
	addint((method)gaussdraw_int);
	addinx((method)gaussdraw_int_1,1);
	addinx((method)gaussdraw_int_2,2);
	addftx((method)gaussdraw_float_1,1);
	addftx((method)gaussdraw_float_2,2);
	addmess((method)gaussdraw_assist,"assist",A_CANT,0);
	addmess((method)gaussdraw_min,"min",A_GIMME,0);
	addmess((method)gaussdraw_max,"max",A_GIMME,0);
	addmess((method)gaussdraw_numpts,"numbars",A_GIMME,0);
	addmess((method)gaussdraw_default,"default",A_GIMME,0);
	addmess((method)gaussdraw_print,"print",A_GIMME,0);

	/* list object in the new object list */
	finder_addclass("Statistics", "gaussdraw");

	rescopy('STR#', RES_ID);
}

void gaussdraw_assist(t_gaussdraw *x, void *b, long m, long a, char *s) 
{
	assist_string(RES_ID,m,a,1,4,s);
}

void readx_N(t_gaussdraw *x, t_atom *argv) {
    
	if ((argv[0].a_type == A_LONG) & (argv[0].a_w.w_long <= MAXNUMPTS) & (argv[0].a_w.w_long >= 1)) {
		x->x_N = argv[0].a_w.w_long;
	} else if ((argv[0].a_type == A_FLOAT) & (argv[0].a_w.w_float <= MAXNUMPTS) & (argv[0].a_w.w_float >= 1)) {
		x->x_N = (int) argv[0].a_w.w_float;
	} else {
		x->x_N = DEF_N;
	}
}

void readx_height(t_gaussdraw *x, t_atom *argv) {
    
	if (argv[1].a_type == A_LONG) {
		x->x_height = (float) argv[1].a_w.w_long;
	} else if (argv[1].a_type == A_FLOAT) {
		x->x_height = argv[1].a_w.w_float;
	} else {
		x->x_height = DEFHEIGHT;
	}
}

void readx_min(t_gaussdraw *x, t_atom *argv) {
    
	if (argv[2].a_type == A_LONG) {
		x->x_min = (float) argv[2].a_w.w_long;
	} else if (argv[2].a_type == A_FLOAT) {
		x->x_min = argv[2].a_w.w_float;
	} else {
		x->x_min = DEFMIN;
	}
}

void readx_max(t_gaussdraw *x, t_atom *argv) {
    
	if (argv[3].a_type == A_LONG) {
		x->x_max = (float) argv[3].a_w.w_long;
	} else if (argv[3].a_type == A_FLOAT) {
		x->x_max = argv[3].a_w.w_float;
	} else {
		x->x_max = DEFMAX;
	}
}

t_gaussdraw *gaussdraw_new(Symbol *sym, short argc, Atom *argv)
{
	t_gaussdraw *x = (t_gaussdraw *)newobject(gaussdraw_class);	

	x->x_outlet = listout((t_gaussdraw *)x); // Create a list outlet
	floatin(x,2); // Create 1 additional inlets
	floatin(x,1); // Create 1 additional inlets

	switch (argc) { // Read arguments
		case 0: 
			x->x_max = DEFMAX;
			x->x_min = DEFMIN;
			x->x_height = DEFHEIGHT;
			x->x_N = DEF_N;
			break;
		case 1:
			x->x_max = DEFMAX;
			x->x_min = DEFMIN;
			x->x_height = DEFHEIGHT;
			readx_N(x,argv);
			break;
		case 2:
			x->x_max = DEFMAX;
			x->x_min = DEFMIN;
			readx_height(x,argv);
			readx_N(x,argv);
			break;
		case 3:
			x->x_max = DEFMAX;
			readx_min(x,argv);
			readx_height(x,argv);
			readx_N(x,argv);
			break;
		case 4:
			readx_max(x,argv);
			readx_min(x,argv);
			readx_height(x,argv);
			readx_N(x,argv);
			break;
		default:
			post("gaussdraw: too many arguments.");		
			readx_max(x,argv);
			readx_min(x,argv);
			readx_height(x,argv);
			readx_N(x,argv);
	}
	
	if (x->x_max <= x->x_min) {
		post("gaussdraw: min can't be higher than max. Taking default...");		
		x->x_max = DEFMAX;
		x->x_min = DEFMIN;
	}	
	x->myList = (Atom *) NewPtr(MAXNUMPTS * sizeof(*x->myList));     
	
	return x;
}

void gaussdraw_free(t_gaussdraw *x)
{
	if (x->myList != NULL) DisposePtr((char *) x->myList);
}

void gaussdraw_bang(t_gaussdraw *x)
{
	int i;
	float pt, val, max = 0.0f;
	float temp = (x->x_max - x->x_min)/x->x_N;
	
	max = 1.0/sqrtf(TWOPI * x->x_var);

	for (i=0; i<x->x_N; i++) {
		pt = x->x_min + i*temp;
		val = expf((pt - x->x_mean) * (pt - x->x_mean) / (-2 * x->x_var)) / sqrtf(TWOPI * x->x_var);
		SETFLOAT(x->myList+i, val/max * x->x_height);
 	}

 	outlet_list(x->x_outlet, 0L, x->x_N, x->myList);
}

void gaussdraw_int(t_gaussdraw *x, long n)
{
	gaussdraw_float(x,(float)n);
}

void gaussdraw_float(t_gaussdraw *x, float f)
{
	x->x_mean = f;
	gaussdraw_bang(x);
}

void gaussdraw_int_1(t_gaussdraw *x, long n)
{
	gaussdraw_float_1(x,(float)n);
}

void gaussdraw_float_1(t_gaussdraw *x, float f)
{
	x->x_var = f;	
}

void gaussdraw_int_2(t_gaussdraw *x, long n)
{
	gaussdraw_float_2(x,(float)n);
}

void gaussdraw_float_2(t_gaussdraw *x, float f)
{
	x->x_height = f;	
}

void gaussdraw_min(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv)
{
	float val;
	
	if (argv[0].a_type == A_FLOAT) {
		val = argv[0].a_w.w_float;
	} else if (argv[0].a_type == A_LONG) {
		val = (float)argv[0].a_w.w_long;
	}
	
	if (val < x->x_max) {
		x->x_min = val;
	} else {
		post("gaussdraw: min can't be higher than max...");
	}	
}

void gaussdraw_max(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv)
{
	float val;
	
	if (argv[0].a_type == A_FLOAT) {
		val = argv[0].a_w.w_float;
	} else if (argv[0].a_type == A_LONG) {
		val = (float)argv[0].a_w.w_long;
	}

	if (val > x->x_min) {
		x->x_max = val;	
	} else {
		post("gaussdraw: max can't be lower than min...");
	}	
}

void gaussdraw_numpts(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv)
{
	int val;
	
	if (argv[0].a_type == A_FLOAT) {
		val = (int)argv[0].a_w.w_float;
	} else if (argv[0].a_type == A_LONG) {
		val = argv[0].a_w.w_long;
	}

	if ((val <= MAXNUMPTS) & (val >= 1))  {
		x->x_N = val;
	} else {
		post("gaussdraw: numbars must be between 1 and %ld...", MAXNUMPTS);
	}
}

void gaussdraw_default(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv)
{
	x->x_mean = DEFMEAN;
	x->x_var = DEFVAR;
	x->x_N = DEF_N;
	x->x_min = DEFMIN;
	x->x_max = DEFMAX;
	x->x_height = DEFHEIGHT;
}

void gaussdraw_print(t_gaussdraw *x, t_symbol *s, short argc, t_atom *argv)
{
	post("--- gaussdraw ---");
	post("  Mean = %.2f",x->x_mean);
	post("  Variance = %.2f",x->x_var);
	post("  NumBars = %ld",x->x_N);
	post("  Height = %.2f",x->x_height);
	post("  Minimum = %.2f",x->x_min);
	post("  Maximum = %.2f",x->x_max);
	post("");	
}