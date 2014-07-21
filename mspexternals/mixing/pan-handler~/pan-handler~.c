/*
DESCRIPTION: An old matrix mixer (by Ville Pulkki?), superseded by MSP's built-in matrix~ object.
*/

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#define RES_ID	7050
#define FAST	1
#define SMOOTH  0

void *pan_handler_class;

typedef struct _pan_handler {

	t_pxobject m_obj;
	t_int numInlets;
	t_int numOutlets;
	t_int version;
    t_int **w;			
    t_float **Inlets;			
    t_float **Outlets;			
    	
} t_pan_handler;

t_symbol *ps_fast, *ps_smooth;

t_int *pan_handler_perform_fast(t_int *w);
t_int *pan_handler_perform_smooth(t_int *w);
void pan_handler_dsp(t_pan_handler *x, t_signal **sp, short *connect);
void pan_handler_fast(t_pan_handler *x, t_symbol *s, short argc, t_atom *argv);
void pan_handler_smooth(t_pan_handler *x, t_symbol *s, short argc, t_atom *argv);
void *pan_handler_new(t_symbol *s, short argc, t_atom *argv);
void pan_handler_free(t_pan_handler *x);

void main(void) {
	
	ps_fast    = gensym("fast");
	ps_smooth  = gensym("smooth");

	setup( &pan_handler_class, pan_handler_new, (method)pan_handler_free, (short)sizeof(t_pan_handler), 0L, A_GIMME, 0);
		
	addmess((method)pan_handler_dsp, "dsp", A_CANT, 0);
	addmess((method)pan_handler_fast, "fast", A_GIMME, 0);
	addmess((method)pan_handler_smooth, "smooth", A_GIMME, 0);
	dsp_initclass();

	rescopy('STR#', RES_ID);
}

t_int *pan_handler_perform_fast(t_int *wAsT_int) {

	t_int k, i;
	t_int **w = (t_int **)wAsT_int;
	t_pan_handler *x = (t_pan_handler *)(w[1]);
	t_int n = (t_int)(w[2]);	
	// use integers to copy blocs faster: was suggested by David Z.
	t_float **in = x->Inlets;
	t_float **out = x->Outlets;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	t_float tmp; // just for testing
	
	// Inlets and Outlets
	for (k=0; k<numInlets; ++k)
		in[k] = (t_float *)(w[k+3]);

	for (k=0; k<numOutlets; ++k)
		out[k] = (t_float *)(w[numInlets+k+3]);	
		
///////////////// WRITE YOUR CODE FOR FAST HERE ////////////////////

	for (i=0; i<n; ++i) {
		tmp = 0;
		for (k=0; k<numInlets; ++k)
			tmp += in[k][i];
		in[0][i] = tmp/numInlets;
	}		

////////////////////////////////////////////////////////////////////

	for (k=0; k<numOutlets; ++k)
		for (i=0; i<n; ++i)
			out[k][i] = in[0][i]; // out[k][i] = WHATEVER!
								
	return (wAsT_int+numInlets+numOutlets+3);
}

t_int *pan_handler_perform_smooth(t_int *wAsT_int) {

	t_int k, i;
	t_int **w = (t_int **)wAsT_int;
	t_pan_handler *x = (t_pan_handler *)(w[1]);
	t_int n = (t_int)(w[2]);	
	// use integers to copy blocs faster: was suggested by David Z.
	t_float **in = x->Inlets;
	t_float **out = x->Outlets;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	t_float tmp; // just for testing
	
	// Inlets and Outlets
	for (k=0; k<numInlets; ++k)
		in[k] = (t_float *)(w[k+3]);

	for (k=0; k<numOutlets; ++k)
		out[k] = (t_float *)(w[numInlets+k+3]);	
		
///////////////// WRITE YOUR CODE FOR SMOOTH HERE ////////////////////

	for (i=0; i<n; ++i) {
		tmp = 0;
		for (k=0; k<numInlets; ++k)
			tmp += in[k][i];
		in[0][i] = tmp/numInlets;
	}		

//////////////////////////////////////////////////////////////////////

	for (k=0; k<numOutlets; ++k)
		for (i=0; i<n; ++i)
			out[k][i] = in[0][i]; // out[k][i] = WHATEVER!
								
	return (wAsT_int+numInlets+numOutlets+3);
}

void pan_handler_dsp(t_pan_handler *x, t_signal **sp, short *connect) {
	t_int vs = sys_getblksize();
	t_int i, num = x->numInlets + x->numOutlets; // number of Inlets and Outlets 
	t_int **w = x->w;

	w[0] = (t_int *)x;
	w[1] = (t_int *)sp[0]->s_n;

	for (i=0; i<num; ++i)
		w[i+2] = (t_int *)sp[i]->s_vec;
			
	num += 2; // x and n

	if (x->version == FAST) 
		dsp_addv(pan_handler_perform_fast, num, (void **)w);
	else dsp_addv(pan_handler_perform_smooth, num, (void **)w);
}

void pan_handler_fast(t_pan_handler *x, t_symbol *s, short argc, t_atom *argv) {
     x->version = FAST;
}

void pan_handler_smooth(t_pan_handler *x, t_symbol *s, short argc, t_atom *argv) {
     x->version = SMOOTH;
}

void *pan_handler_new(t_symbol *s, short argc, t_atom *argv) {

	t_int i, k;
	t_int vs = sys_getblksize(); // Size of signal vector selected in MSP
    t_pan_handler *x = (t_pan_handler *)newobject(pan_handler_class);    
	t_float Fs = sys_getsr();
			
	// Look at 1st argument
	if (argv[0].a_type == A_LONG) x->numInlets = argv[0].a_w.w_long;
	else if (argv[0].a_type == A_FLOAT) x->numInlets = (int)argv[0].a_w.w_float;
	else x->numInlets = 2; // Just a default value

	// Look at 2nd argument
	if (argv[1].a_type == A_LONG) x->numOutlets = argv[1].a_w.w_long;
	else if (argv[1].a_type == A_FLOAT) x->numOutlets = (int)argv[1].a_w.w_float;
	else x->numOutlets = 2; // Just a default value
		
	// Look at 3rd argument
	if (argv[2].a_w.w_sym == ps_fast)
		x->version = 1;
	else x->version = 0; // smooth is default

	x->Inlets = t_getbytes(x->numInlets * sizeof(t_float*));
	x->Outlets = t_getbytes(x->numOutlets * sizeof(t_float*));
	x->w = t_getbytes((x->numInlets + x->numOutlets + 2) * sizeof(t_int*));
		
	// Create inlets and outlets
	dsp_setup((t_pxobject *)x, x->numInlets);
	x->m_obj.z_misc = Z_NO_INPLACE; // Necessary when outlets should have different vectors than inlets !!
	for (i=0; i<x->numOutlets; ++i)
		outlet_new((t_object *)x, "signal");
	
    return (x);
}

void  pan_handler_free(t_pan_handler *x) {

	if (x->Inlets) t_freebytes(x->Inlets, x->numInlets * sizeof(t_float*));
	if (x->Outlets) t_freebytes(x->Outlets, x->numOutlets * sizeof(t_float*));
	if (x->w) t_freebytes(x->w, (x->numInlets + x->numOutlets + 2) * sizeof(t_int*));
	dsp_free((t_pxobject *)x);
}

