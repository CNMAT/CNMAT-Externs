/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006, The Regents of
the University of California (Regents). 
Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: bessel
DESCRIPTION: Bessel functions.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 0.1: Added functions for access to different orders of Bessel functions.
VERSION 0.2: Fixed error handler that was calling abort() due to underflow errors.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"

#include "math.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_bessel.h>

typedef struct _besl
{
	t_object b_ob;
	long b_in0;
	void *b_out0;
	int *b_functionList;
	short b_nFunctions;
} t_besl;

void *besl_class;

void besl_anything(t_besl *x, t_symbol *msg, short argc, t_atom *argv);
void besl_bang(t_besl *x);
void besl_int(t_besl *x, long n);
void *besl_new();
void besl_free(t_besl *x);
void besl_assist(t_besl *x, void *b, long m, long a, char *s);
void besl_list(t_besl *x, t_symbol *msg, short argc, t_atom *argv);
void besl_float(t_besl *x, double xx);
void besl_j0(t_besl *x, double xx);
void besl_j1(t_besl *x, double xx);
void besl_jn(t_besl *x, double xx, int n);
void besl_jmn(t_besl *x, int m, int n);


//--------------------------------------------------------------------------

int main(void)
{
	setup((t_messlist **)&besl_class, (method)besl_new, (method)besl_free, (short)sizeof(t_besl), 0L, 0); 
	
	version(0);

	addmess((method) version, "version", 0);
	addbang((method)besl_bang);
	addint((method)besl_int);
	addfloat((method)besl_float);
	addmess((method)besl_list, "list", A_GIMME, 0);
	addmess((method)besl_anything, "anything", A_GIMME, 0);
	addmess((method)besl_assist, "assist", A_CANT, 0);
	addmess((method)besl_j0, "j0", A_FLOAT, 0);
	addmess((method)besl_j1, "j1", A_FLOAT, 0);
	addmess((method)besl_jn, "jn", A_FLOAT, A_LONG, 0);
	addmess((method)besl_jmn, "jmn", A_LONG, A_LONG, 0);
	
	return 0;
}

void besl_anything(t_besl *x, t_symbol *msg, short argc, t_atom *argv)
{
}

void besl_bang(t_besl *x)
{
}

void besl_int(t_besl *x, long n)
{
}

void besl_assist(t_besl *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void *besl_new()
{
	t_besl *x;

	x = (t_besl *)newobject(besl_class); // create a new instance of this object
	
	x->b_out0 = outlet_new(x, 0);
	
	x->b_nFunctions = 0;
	x->b_functionList = (int *)calloc(1, sizeof(x->b_nFunctions));
	
	// turn off the gsl error handler which, by default, calls abort() if an error is received.
	// Low x values of high order functions (>175 or so) were reporting underflow errors.
	gsl_set_error_handler_off();
		   	
	return(x);
}

void besl_free(t_besl *x)
{
}

void besl_list(t_besl *x, t_symbol *msg, short argc, t_atom *argv){
	x->b_functionList = (int *)realloc(x->b_functionList, argc * sizeof(int));
	int i;
	for(i = 0; i < argc; i++){
		x->b_functionList[i] = (int)argv[i].a_w.w_long;
	}
	//memcpy(x->b_functionList, argv, argc * sizeof(t_atom));
	x->b_nFunctions = argc;
}

void besl_float(t_besl *x, double xx){
	t_atom *out = (t_atom *)calloc(x->b_nFunctions, sizeof(t_atom));
	int status;
	gsl_sf_result result;
	
	int i;
	for(i = 0; i < x->b_nFunctions; i++){
		status = gsl_sf_bessel_Jn_e(x->b_functionList[i], xx, &result);
		SETFLOAT(out + (i), result.val);
	}
	outlet_list(x->b_out0, 0L, x->b_nFunctions, out);
}

void besl_j0(t_besl *x, double xx){
	int status;
	gsl_sf_result result;
	
	status = (float)gsl_sf_bessel_J0_e(xx, &result);
	
	outlet_float(x->b_out0, result.val);
}

void besl_j1(t_besl *x, double xx){
	int status;
	gsl_sf_result result;
	
	status = (float)gsl_sf_bessel_J1_e(xx, &result);
	
	outlet_float(x->b_out0, result.val);
}

void besl_jn(t_besl *x, double xx, int n){
	int status;
	gsl_sf_result result;
	
	status = gsl_sf_bessel_Jn_e((int)n, xx, &result);
	
	outlet_float(x->b_out0, (float)result.val);
}

void besl_jmn(t_besl *x, int m, int n){
	int tmp;
	if(n < m){
		tmp = m;
		m = n;
		n = tmp;
	}
	int len = 1 + (n - m);
	
	x->b_functionList = (int *)realloc(x->b_functionList, len * sizeof(int));
	int i;
	for(i = 0; i < len; i++){
		x->b_functionList[i] = i + m;
	}
	x->b_nFunctions = len;
}
