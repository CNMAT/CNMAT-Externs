/**
	@file
	simplemsp - an MSP object shell
	jeremy bernstein - jeremy@bootsquad.com	

	@ingroup	examples	
*/

#include "ext.h"							// standard Max include, always required (except in Jitter)
#include "ext_obex.h"						// required for new style objects
#include "z_dsp.h"							// required for MSP objects

////////////////////////// object struct
typedef struct _simplemsp 
{
	t_pxobject					ob;			// the object itself (t_pxobject in MSP)
	float						offset; 
} t_simplemsp;

///////////////////////// function prototypes
//// standard set
void *simplemsp_new(t_symbol *s, long argc, t_atom *argv);
void simplemsp_free(t_simplemsp *x);
void simplemsp_assist(t_simplemsp *x, void *b, long m, long a, char *s);

void simplemsp_float(t_simplemsp *x, double f);

void simplemsp_dsp(t_simplemsp *x, t_signal **sp, short *count);
t_int *simplemsp_perform(t_int *w);
//////////////////////// global class pointer variable
void *simplemsp_class;


int main(void)
{	
	// object initialization, note the use of dsp_free for the freemethod, which is required
	// unless you need to free allocated memory, in which case you should call dsp_free from
	// your custom free function.

	// OLD METHOD
	// setup((t_messlist **)&simplemsp_class, (method)simplemsp_new, (method)dsp_free, (short)sizeof(t_simplemsp), 0L, A_GIMME, 0);
	// addfloat((method)simplemsp_float);
	// you need this
	// addmess((method)simplemsp_dsp,				"dsp",			A_CANT, 0);
    // addmess((method)simplemsp_assist,			"assist",		A_CANT, 0);  
	// you need this
    // dsp_initclass();
	
	// NEW METHOD
	t_class *c;
	
	c = class_new("simplemsp~", (method)simplemsp_new, (method)dsp_free, (long)sizeof(t_simplemsp), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)simplemsp_float,		"float",	A_FLOAT, 0);
	class_addmethod(c, (method)simplemsp_dsp,		"dsp",		A_CANT, 0);
	class_addmethod(c, (method)simplemsp_assist,	"assist",	A_CANT, 0);
	
	class_register(CLASS_BOX, c); // register class as a box class
	
	class_dspinit(c); // new style object version of dsp_initclass();
	simplemsp_class = c;
	
	return 0;
}

void simplemsp_float(t_simplemsp *x, double f)
{
	x->offset = f;
}

// this function is called when the DAC is enabled, and "registers" a function
// for the signal chain. in this case, "simplemsp_perform"
void simplemsp_dsp(t_simplemsp *x, t_signal **sp, short *count)
{
	post("my sample rate is: %f", sp[0]->s_sr);
	
	// dsp_add
	// 1: (t_perfroutine p) perform method
	// 2: (long argc) number of args to your perform method
	// 3...: argc additional arguments, all must be sizeof(pointer) or long
	// these can be whatever, so you might want to include your object pointer in there
	// so that you have access to the info, if you need it.
	dsp_add(simplemsp_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

t_int *simplemsp_perform(t_int *w)
{
	// DO NOT CALL post IN HERE, but you can call defer_low (not defer)
	
	// args are in a vector, sized as specified in simplemsp_dsp method
	// w[0] contains &simplemsp_perform, so we start at w[1]
	t_simplemsp *x = (t_simplemsp *)(w[1]);
	t_float *inL = (t_float *)(w[2]);
	t_float *outL = (t_float *)(w[3]);
	int n = (int)w[4];
	
	// this perform method simply copies the input to the output
	while (n--)
		*outL++ = *inL++ + x->offset;
		
	// you have to return the NEXT pointer in the array OR MAX WILL CRASH
	return w + 5;
}

void simplemsp_assist(t_simplemsp *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { //inlet
		sprintf(s, "I am inlet %ld", a);
	} 
	else {	// outlet
		sprintf(s, "I am outlet %ld", a); 			
	}
}

// NOT CALLED!, we use dsp_free for a generic free function
void simplemsp_free(t_simplemsp *x) 
{
	;
}

void *simplemsp_new(t_symbol *s, long argc, t_atom *argv)
{
	t_simplemsp *x = NULL;

	// OLD VERSION
	// if (x = (t_simplemsp *)newobject(simplemsp_class)) {
	
	// NEW VERSION
	if (x = (t_simplemsp *)object_alloc(simplemsp_class)) {
		dsp_setup((t_pxobject *)x, 1);	// MSP inlets: arg is # of inlets and is REQUIRED! 
										// use 0 if you don't need inlets
		outlet_new(x, "signal"); // signal outlet (note "signal" rather than NULL)
		x->offset = 0.;
	}
	return (x);
}
