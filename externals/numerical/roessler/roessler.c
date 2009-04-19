/*
 Written by Michael Zbyszynski, The Center for New Music and Audio Technologies,
 University of California, Berkeley.  Copyright (c) 2000-09, The Regents of
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
 NAME: roessler
 DESCRIPTION: generate a rössler atttractor
 AUTHORS: Michael Zbyszynski
 COPYRIGHT_YEARS: 2006-09
 DRUPAL_NODE: /patch/6666
 SVN_REVISION: $LastChangedRevision: 2500 $
 VERSION 1.0: first release
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */


#include "ext.h"
/* structure definition of object */
typedef struct rossler
{
	t_object l_ob;		
	double r_x;					// stored values
	double r_y;
	double r_z;
	double r_h;
	double r_a;
	double r_b;
	double r_c;
	
	void *r_out0;				// outlets
	void *r_out1;
	void *r_out2;
					
} t_rossler;

/* global pointer that holds the class definition */
void *rossler_class;

/* prototypes for methods */

void rossler_bang(t_rossler *x);
void rossler_float(t_rossler *x, float n);   
void rossler_ft1(t_rossler *x, float o);
void rossler_ft2(t_rossler *x, float p);
void rossler_ft3(t_rossler *x, float q);
void *rossler_new(t_symbol *s, short argc, t_atom *argv);

/* initialization routine */

int main(void)
{
	/* define class. */
	setup((t_messlist **)&rossler_class, (method)rossler_new, (method)0L, (short)sizeof(t_rossler), 0L, A_GIMME,0);
	
	/* bind methods to symbols */
	addbang((method)rossler_bang);
	addfloat((method)rossler_float);
	addftx((method)rossler_ft1, 1);	
	addftx((method)rossler_ft2, 2);
	addftx((method)rossler_ft3, 3);
	
	post("rossler object by Michael F. Zbyszynski, 3.xi.06",0);
	
	/* list object in the new object list */
	finder_addclass("Data","rossler");
	
	return 0;
}
//--------------------------------------------------------------------------

void rossler_bang(t_rossler *x)
{
	double xnew;
	double ynew;
	double znew;
	
	xnew = x->r_x + x->r_h * (- x->r_y - x->r_z);
	ynew = x->r_y + x->r_h * (x->r_x + x->r_a * x->r_y);
	znew = x->r_z + x->r_h * (x->r_b + x->r_z * (x->r_x - x->r_c));
	
	outlet_float(x->r_out2, znew);
	outlet_float(x->r_out1, ynew);
	outlet_float(x->r_out0, xnew);
	
	
	/* set all the variables to new values */
	
	x->r_x = xnew;
	x->r_y = ynew;
	x->r_z = znew;
}

//--------------------------------------------------------------------------

void rossler_float(t_rossler *x, float n)
{
	double xnew;
	double ynew;
	double znew;
	
	x->r_x = n;									// store a value inside the object
	

	xnew = n + x->r_h * (- x->r_y - n);
	ynew = x->r_y + x->r_h * (n + x->r_a * x->r_y);	
	znew = x->r_z + x->r_h * (x->r_b + x->r_z * (n - x->r_c));
	
	outlet_float(x->r_out2, znew);
	outlet_float(x->r_out1, ynew);	
	outlet_float(x->r_out0, xnew);
	
	/* set all the variables to new values */
	
	x->r_x = xnew;
	x->r_y = ynew;
	x->r_z = znew;	
	
}

//--------------------------------------------------------------------------


void rossler_ft1(t_rossler *x, float o)
{
	
	x->r_y = o;									// store a value inside the object
	
}

//--------------------------------------------------------------------------


void rossler_ft2(t_rossler *x, float p)
{
	
	
	x->r_z = p;									// store a value inside the object
	
}

//--------------------------------------------------------------------------


void rossler_ft3(t_rossler *x, float q)
{

	
	x->r_h = q;									// store a value inside the object
	
}

//--------------------------------------------------------------------------

/* instance creation routine */

void *rossler_new(t_symbol *s, short argc, t_atom *argv)
{
	t_rossler *x;
	float AY, BE, CI, HACH;
	
	x = (t_rossler *)newobject(rossler_class);		// get memory for a new object & initialize 
	

	if (argc>0){
		if (argv[0].a_type == A_FLOAT)
			AY = argv[0].a_w.w_float;
		else if (argv[0].a_type == A_LONG)
			AY = (float)argv[0].a_w.w_long;
		else 	AY = 0.2;
	}
	else AY = BE = 0.2, CI = 5.7, HACH = 0.05;
	
	 if (argc>1){
		if (argv[1].a_type == A_FLOAT)
			BE = argv[1].a_w.w_float;
		else if (argv[1].a_type == A_LONG)
			BE = (float)argv[1].a_w.w_long;
		else 	BE = 0.2;
	}
	if (argc>2){
		if (argv[2].a_type == A_FLOAT)
			CI = argv[2].a_w.w_float;
		else if (argv[2].a_type == A_LONG)
			CI = (float)argv[2].a_w.w_long;
		else 	CI = 5.7;
	}
	 if (argc>3){
		if (argv[3].a_type == A_FLOAT)
			HACH = argv[3].a_w.w_float;
		else if (argv[3].a_type == A_LONG)
			HACH = (float)argv[3].a_w.w_long;
		else 	HACH = 0.05;
	}
	  	
	
	floatin(x,3);
	floatin(x,2);									// create inlets
	floatin(x,1);
	
	x->r_out2 = floatout(x);						// create outlets
	x->r_out1 = floatout(x);
	x->r_out0 = floatout(x);
	
	
	
	x->r_h = HACH;
	x->r_a = AY ;
	x->r_b = BE;
	x->r_c = CI;
	x->r_x = 0.01;
	x->r_y = 0.01;
	x->r_z = 0.01;


	return (x);									// return newly created object to caller
}

	
	

	