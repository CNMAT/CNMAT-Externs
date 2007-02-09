// gaussmodel object by Tristan Jehan 
// Normal distribution generator

#include "ext.h"
#include <math.h>

// CONSTANTS
#define VERSION "1.0"
#define MAX_N 5000		// Maximum number of elements
#define MAXRES 256		// Maximum number of elements in the histogram
#define DEF_N 500		// Default number of elements
#define DEFRES 32		// Default histogram resolution
#define DEFMIN 0		// Default histogram minimum
#define DEFMAX 128		// Default histogram maximum
#define DEFMEAN 0		// Default mean
#define DEFVAR 0		// Default variance
#define RES_ID 7035

// Main structure
typedef struct _gaussmodel
{
	Object x_ob;			// required header
	float x_mean;			// mean
	float x_var;			// var
	float x_min;			// min
	float x_max;			// max
	float x_incr;			// histogram resolution
	int x_res;				// histogram number of bars
	int x_N;				// number of elements
	int x_current;			// where to write in the buffer
	int x_last;				// where to finish reading in the buffer
	int x_count;			// initial counter
	float x_buff[MAX_N];	// buffer containing the data 
	float x_hist[MAXRES];	// buffer containing the histogram data 
	void *x_outmean;		// mean outlet
	void *x_outvar;			// var outlet
	void *x_outhist;		// histogram outlet
	Atom *myList;			// Copy of outputs as Atoms
	
} t_gaussmodel;

// globalthat holds the class definition
void *gaussmodel_class;

// gaussmodel prototypes
t_gaussmodel *gaussmodel_new(Symbol *x, short argc, Atom *argv);
void gaussmodel_assist(t_gaussmodel *x, void *b, long m, long a, char *s);
void gaussmodel_free(t_gaussmodel *x);
void gaussmodel_bang(t_gaussmodel *x);
void gaussmodel_int(t_gaussmodel *x, long n);
void gaussmodel_float(t_gaussmodel *x, float f);
void gaussmodel_int_1(t_gaussmodel *x, long n);
void gaussmodel_float_1(t_gaussmodel *x, float f);
void gaussmodel_min(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv);
void gaussmodel_max(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv);
void gaussmodel_numbars(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv);
void readx_N(t_gaussmodel *x, t_atom *argv);
void readx_res(t_gaussmodel *x, t_atom *argv);
void readx_min(t_gaussmodel *x, t_atom *argv);
void readx_max(t_gaussmodel *x, t_atom *argv);
void gaussmodel_hist(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv);
void gaussmodel_calculate(t_gaussmodel *x);
void gaussmodel_reset(t_gaussmodel *x);
void gaussmodel_print(t_gaussmodel *x);
void update_hist(t_gaussmodel *x);

// Main function
void main(void) 
{
	post("gaussmodel object version " VERSION " by Tristan Jehan");
    post("copyright © 2001 Massachusetts Institute of Technology");
	post("");
	
	setup(								// Setup Object
		(Messlist**) &gaussmodel_class,	// Pointer to class
		(method) gaussmodel_new,		// Constructor
		(method) gaussmodel_free,		// Destructor
		(short) sizeof(t_gaussmodel),	// Class Size
		0L,	A_GIMME, 0);

	/* bind your methods to symbols */
	addbang((method)gaussmodel_bang);
	addfloat((method)gaussmodel_float);
	addint((method)gaussmodel_int);
	addftx((method)gaussmodel_float_1,1);
	addinx((method)gaussmodel_int_1,1);
	addmess((method)gaussmodel_assist,"assist",A_CANT,0);
	addmess((method)gaussmodel_min,"min",A_GIMME,0);
	addmess((method)gaussmodel_max,"max",A_GIMME,0);
	addmess((method)gaussmodel_numbars,"numbars",A_GIMME,0);
	addmess((method)gaussmodel_reset,"reset",0L,0);
	addmess((method)gaussmodel_print,"print",0L,0);

	/* list object in the new object list */
	finder_addclass("Statistics", "gaussmodel");

	rescopy('STR#', RES_ID);
}

void gaussmodel_assist(t_gaussmodel *x, void *b, long m, long a, char *s) 
{
	assist_string(RES_ID,m,a,1,3,s);
}

void gaussmodel_int(t_gaussmodel *x, long n)
{
	gaussmodel_float(x, (float)n);
}

void gaussmodel_float(t_gaussmodel *x, float f)
{
	x->x_buff[x->x_current] = f;
	
	// Update locations of first and last element to consider
	x->x_current ++;
	x->x_current %= MAX_N;
	
	if (x->x_count <= 0) {
		x->x_last ++;
		x->x_last %= MAX_N;
	} else {
		x->x_count --;
	}
	
	// Calculate values and bang output to outlets
	gaussmodel_calculate(x);	
	gaussmodel_bang(x);
}

void gaussmodel_int_1(t_gaussmodel *x, long n)
{
	int diff = ((MAX_N + x->x_current - x->x_last) % MAX_N);
	
	if ((n > 0) & (n <= MAX_N)) {
		if (n <= diff) {
			x->x_last = (MAX_N + x->x_current - n) % MAX_N;
			x->x_count = 0;
		} else if (n > diff) {
			x->x_count = n - diff;
		}
	x->x_N = n;
	} else {
		post("gaussmodel: Window must be between 1 and %ld", MAX_N);
	}
}

void gaussmodel_float_1(t_gaussmodel *x, float f)
{
	gaussmodel_int_1(x, (int)f);
}

void readx_N(t_gaussmodel *x, t_atom *argv) {
    
	if (argv[0].a_type == A_LONG) {
		x->x_N = argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->x_N = (int) argv[0].a_w.w_float;
	} else {
		x->x_N = DEF_N;
	}
}

void readx_res(t_gaussmodel *x, t_atom *argv) {
    
	if (argv[1].a_type == A_LONG) {
		x->x_res = argv[1].a_w.w_long;
	} else if (argv[1].a_type == A_FLOAT) {
		x->x_res = (int) argv[1].a_w.w_float;
	} else {
		x->x_res = DEFRES;
	}
}

void readx_min(t_gaussmodel *x, t_atom *argv) {
    
	if (argv[2].a_type == A_LONG) {
		x->x_min = (float)argv[2].a_w.w_long;
	} else if (argv[2].a_type == A_FLOAT) {
		x->x_min = argv[2].a_w.w_float;
	} else {
		x->x_min = DEFMIN;
	}
}

void readx_max(t_gaussmodel *x, t_atom *argv) {
    
	if (argv[3].a_type == A_LONG) {
		x->x_min = (float)argv[3].a_w.w_long;
	} else if (argv[3].a_type == A_FLOAT) {
		x->x_min = argv[3].a_w.w_float;
	} else {
		x->x_min = DEFMAX;
	}
}

t_gaussmodel *gaussmodel_new(Symbol *sym, short argc, Atom *argv)
{
	t_gaussmodel *x = (t_gaussmodel *)newobject(gaussmodel_class);	
	int i;

	x->x_outhist = listout((t_gaussmodel *)x); // Create a list outlet
	x->x_outvar = floatout((t_gaussmodel *)x); // Create a float outlet
	x->x_outmean = floatout((t_gaussmodel *)x); // Create a float outlet
	floatin(x,1); // Create 1 additional inlets

	switch (argc) { // Read arguments
		case 0: 
			x->x_N = DEF_N;
			x->x_res = DEFRES;
			x->x_min = DEFMIN;
			x->x_max = DEFMAX;
			break;
		case 1:
			readx_N(x,argv);
			x->x_res = DEFRES;
			x->x_min = DEFMIN;
			x->x_max = DEFMAX;
			break;
		case 2:
			readx_N(x,argv);
			readx_res(x,argv);
			x->x_min = DEFMIN;
			x->x_max = DEFMAX;
			break;
		case 3:
			readx_N(x,argv);
			readx_res(x,argv);
			readx_min(x,argv);
			x->x_max = DEFMAX;
			break;
		case 4:
			readx_N(x,argv);
			readx_res(x,argv);
			readx_min(x,argv);
			readx_max(x,argv);
			break;
		default:
			post("gaussmodel: too many arguments.");		
			readx_N(x,argv);
			readx_res(x,argv);
			readx_min(x,argv);
			readx_max(x,argv);
	}
	
	// Initializations
	gaussmodel_reset(x);
	
	for (i=0; i<MAX_N; i++) {
		x->x_buff[i] = 0.0f;
	}
	for (i=0; i<MAXRES; i++) {
		x->x_hist[i] = 0.0f;
	}
	

	// Allocate memory
	x->myList = (Atom *) NewPtr(MAXRES * sizeof(*x->myList));     
	
	return x;
}

void gaussmodel_free(t_gaussmodel *x)
{
	if (x->myList != NULL) DisposePtr((char *) x->myList);
}

void gaussmodel_bang(t_gaussmodel *x)
{
	int i;

	for (i=0; i<x->x_res; i++) {
		SETFLOAT(x->myList+i, x->x_hist[i]);
	}

 	outlet_list(x->x_outhist, 0L, x->x_res, x->myList);
    outlet_float(x->x_outmean, x->x_mean);
    outlet_float(x->x_outvar, x->x_var);
}

void gaussmodel_min(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv)
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
		post("gaussmodel: min can't be higher than max...");
	}	
	
	update_hist(x);
}

void gaussmodel_max(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv)
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
		post("gaussmodel: max can't be lower than min...");
	}
	
	update_hist(x);
}

void gaussmodel_numbars(t_gaussmodel *x, t_symbol *s, short argc, t_atom *argv)
{
	int val;
	
	if (argv[0].a_type == A_FLOAT) {
		val = (int)argv[0].a_w.w_float;
	} else if (argv[0].a_type == A_LONG) {
		val = argv[0].a_w.w_long;
	}

	if ((val <= MAXRES) & (val >= 1))  {
		x->x_res = val;
	} else {
		post("gaussmodel: numbars must be between 1 and %ld...", MAXRES);
	}

	update_hist(x);
}

void gaussmodel_calculate(t_gaussmodel *x) 
{
	int i, found, ind = 0;
	float mean = 0.0f;
	float var = 0.0f;
	float minlim, maxlim, val;
	
	// Initialize histogram
	for (i=0; i<x->x_res; i++) {
		x->x_hist[i] = 0.0f;
	}
		
	// Normal case
	if (x->x_current > x->x_last) {
		// Mean
		for (i=x->x_last; i<x->x_current; i++) {
			mean += x->x_buff[i]; 
		}
		mean /= (x->x_current - x->x_last);		
		// Variance
		for (i=x->x_last; i<x->x_current; i++) {
			var += (x->x_buff[i] - mean) * (x->x_buff[i] - mean); 
		}
		var /= (x->x_current - x->x_last);
		// Histogram
		for (i=x->x_last; i<x->x_current; i++) {
			ind = 0;
			found = 0;
			minlim = x->x_min;
			maxlim = x->x_min + x->x_incr;
			val = x->x_buff[i];
			if ((minlim < val) & (val < maxlim)) {
				x->x_hist[ind] ++;
				found = 1;
			}
			while(!found & (ind < x->x_res)) {
				ind ++;
				minlim = maxlim;
				maxlim += x->x_incr;
				if ((minlim < val) & (val < maxlim)) {
					x->x_hist[ind] ++;
					found = 1;
				}
			}
		}
		
	// Transition case
	} else {
		// Mean
		for (i=x->x_last; i<MAX_N; i++) {
			mean += x->x_buff[i]; 
		}
		for (i=0; i<x->x_current; i++) {
			mean += x->x_buff[i]; 
		}
		mean /= (MAX_N - x->x_last + x->x_current);
		// Variance
		for (i=x->x_last; i<MAX_N; i++) {
			var += (x->x_buff[i] - mean) * (x->x_buff[i] - mean); 
		}
		for (i=0; i<x->x_current; i++) {
			var += (x->x_buff[i] - mean) * (x->x_buff[i] - mean); 
		}
		var /= (MAX_N - x->x_last + x->x_current);
		// Histogram
		for (i=x->x_last; i<MAX_N; i++) {
			ind = 0;
			found = 0;
			minlim = x->x_min;
			maxlim = x->x_min + x->x_incr;
			val = x->x_buff[i];
			if ((minlim < val) & (val < maxlim)) {
				x->x_hist[ind] ++;
				found = 1;
			}
			while(!found & (ind < x->x_res)) {
				ind ++;
				minlim = maxlim;
				maxlim += x->x_incr;
				if ((minlim < val) & (val < maxlim)) {
					x->x_hist[ind] ++;
					found = 1;
				}
			}
		}
		for (i=0; i<x->x_current; i++) {
			ind = 0;
			found = 0;
			minlim = x->x_min;
			maxlim = x->x_min + x->x_incr;
			val = x->x_buff[i];
			if ((minlim < val) & (val < maxlim)) {
				x->x_hist[ind] ++;
				found = 1;
			}
			while(!found & (ind < x->x_res)) {
				ind ++;
				minlim = maxlim;
				maxlim += x->x_incr;
				if ((minlim < val) & (val < maxlim)) {
					x->x_hist[ind] ++;
					found = 1;
				}
			}
		}

	}
	
	x->x_mean = mean;
	x->x_var = var;	
}

void update_hist(t_gaussmodel *x) 
{
	x->x_incr = (x->x_max - x->x_min) / x->x_res;
}

void gaussmodel_reset(t_gaussmodel *x)
{
	x->x_current = 0;
	x->x_last = 0;
	x->x_count = x->x_N;
	x->x_mean = DEFMEAN;
	x->x_var = DEFVAR;
	update_hist(x);
}

void gaussmodel_print(t_gaussmodel *x)
{
	post("--- gaussmodel ---");
	post("  Window length = %ld",x->x_N);
	post("  Number of bars = %ld",x->x_res);
	post("  Minimum = %.2f",x->x_min);
	post("  Maximum = %.2f",x->x_max);
	post("");	
}