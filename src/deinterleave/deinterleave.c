/*
Copyright (c) 2000,01,02,03,04,05,06.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Tim Madden and Matt Wright, The Center for New Music and
Audio Technologies, University of California, Berkeley.

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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: deinterleave
DESCRIPTION: Divide a large input list into multiple output lists by de-interleaving.  E.g., for 2 outputs, it will put odd-numbered elements out the left list and even-numbered elements out the right list.
AUTHORS: Tim Madden and Matt Wright
COPYRIGHT_YEARS: 2000,01,02,03,04,05,06
DRUPAL_NODE: /patch/4071
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Tim's original version
VERSION 1.0: 2/20/02 Matt Wright changed to right-to-left output order
VERSION 1.1: Matt changed to use global version() procedure and compile on Windows/GCC
VERSION 1.2incomplete: beginning of rewrite to have any number of outputs and to work with "message plus argument" lists; not completed, since Javascript can now do the job.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
#define NAME "deinterleave"
#define DESCRIPTION "Divide a large input list into multiple output lists by de-interleaving.  E.g., for 2 outputs, it will put odd-numbered elements out the left list and even-numbered elements out the right list."
#define AUTHORS "Tim Madden and Matt Wright"
#define COPYRIGHT_YEARS "2000-06,12,13"


/**************************************************************************************
 * This code is a Max external for taking a list and de-interleaving it
 * into seperate lists. The object can have 2 to 4 outputs, to split into
 * 2 to 4 lists respectively. Example for 3 outputs:
 *
 * Input list: 1 2 3 4 5 6 7 8 9
 * Outputs 1 4 7, 2 5 8, 3 6 9
 *
 * The object responds to bang, to output most recent data, or list.
 *
 * Arguments: deinterleave 1 1 1 1, for 4 outputs. Number of arguments determine
 * number of outputs.
 *
 * OR- deinterleave 4 , for 4 outputs. If only one argument, then its value
 * determines number of outputs.
 *
 * Tim Madden, 2000

 *************************************************************************************/


/*
 * Includes.
 */


#include "ext.h"
#include "ext_obex.h"

#include "version.h"


// Define the max size of an output list
#define DEFAULT_MAX_OUTARGS 1000
#define MAX_OUTLETS 32

// Structure for this type of object.
typedef struct t_deinterleave
{
	struct object t_obj;
	// Outputs
	void *t_out[MAX_OUTLETS];	
	// data to output.	
	Atom *t_list_out[MAX_OUTLETS];
	// Length of each output list.
	short t_outsize;
	// Number of outputs.
	int num_outputs;
} t_deinterleave;

// Symbol (list) for output.
Symbol *ps_list;

t_class *deinterleave_class;

/*
 * Function Prototypes.
 */

// Make new object.
void *deinterleave_new(
	Symbol *s, 
	int ac,
	Atom *av);


// Respond to a list.
void List(
	t_deinterleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
// Respond to a bang.
void Bang(t_deinterleave *x);


// Output our data.	
void Output(t_deinterleave *x);

void deinterleave_free(t_deinterleave *x);

/**************************************************************************************
 * Main, or most central, encompassing, and critical routine.
 *************************************************************************************/


int main(void){

	/* tell Max about my class. The cast to short is important for 68K */
	deinterleave_class = class_new("deinterleave", (method)deinterleave_new, (method)deinterleave_free,
			(short)sizeof(t_deinterleave), 0L, A_GIMME, 0);
			
			
	class_addmethod(deinterleave_class, (method)List, "list", A_GIMME, 0);
	class_addmethod(deinterleave_class, (method)Bang, "bang", 0);
	class_addmethod(deinterleave_class, (method)version, "version", 0);

	ps_list = gensym("list");
	
	version_post_copyright();

	class_register(CLASS_BOX, deinterleave_class);
	return 0;
}


/**************************************************************************************
 * Make new object.
 *************************************************************************************/

void *deinterleave_new(
	Symbol *s, 
	int ac,
	Atom *av)
{

	t_deinterleave *x;
	
	//post("Running deinterleave_new");

	x = (t_deinterleave *)object_alloc(deinterleave_class);
    if(!x){
        return NULL;
    }
	
	/* 
	 * Deal with arguments.
	 */
	
	if (ac == 0)
	{
		// If no args, then 2 outputs.
		x->num_outputs = 2;
	}
	else if (ac == 1)
	{
		// If 1 arg, then its value is number of outputs.
		x->num_outputs = av[0].a_w.w_long;
	}
	else
	{
		// if 2 or more args, then number of args= number of outputs.
		x->num_outputs = ac;
	}
	
	// Allow 2 to MAX_OUTLETS outputs.
	if (x->num_outputs > MAX_OUTLETS) {
		object_post((t_object *)x, "deinterleave: can't have more than %ld outlets (compile-time constant MAX_OUTLETS)", MAX_OUTLETS);
		x->num_outputs = MAX_OUTLETS;
	}
			
	if (x->num_outputs < 2)
		x->num_outputs = 2;
		
	// Make outputs (in right to left order) and allocate space to hold output lists
		for (int i = x->num_outputs-1; i >= 0; --i) {
		x->t_out[i] = listout(x);
		x->t_list_out[i] = (Atom *) getbytes(DEFAULT_MAX_OUTARGS * sizeof(Atom));
	}
		
	// Set output size 0	
	x->t_outsize = 0;
		
	return(x);

}

void deinterleave_free(t_deinterleave *x) {
	int i;
	for (i = 0; i<x->num_outputs; ++i) {
		freebytes(x->t_list_out[i], DEFAULT_MAX_OUTARGS * sizeof(Atom));
	}
}


/**************************************************************************************
 * Respond to list message.
 *************************************************************************************/

void List(
	t_deinterleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int counter;
	int i;
	short outsize;
	
	outsize = (short) (argc / x->num_outputs);
	
	if (outsize > DEFAULT_MAX_OUTARGS) {
		object_error((t_object *)x, "can't output %ld-length list (max is %ld)",
			 outsize, DEFAULT_MAX_OUTARGS);
		return;
	}
	
	if (argc % x->num_outputs == 0)
	{
		// Set counter for output array.
		counter = 0;
		
		// Put args into storage.
		for (i = 0; i < argc; i = i + x->num_outputs)
		{	
			x->t_list_out[0][counter] = argv[i];
	
			if (x->num_outputs >= 2)
				x->t_list_out[1][counter] = argv[i+1];
	
			if (x->num_outputs >= 3)
				x->t_list_out[2][counter] = argv[i+2];
	
			if (x->num_outputs >= 4)
				x->t_list_out[3][counter] = argv[i+3];
				
			// increment counter.
			counter++;
		}
		
		// Set outsize.
		x->t_outsize = outsize;
	
		Output(x);
	}
	else
	{
		object_post((t_object *)x, "� deinterleave: List length must be multiple of number of outputs.");
	} 
}

/**************************************************************************************
 * Do a bang. Output most recent data.
 *************************************************************************************/
void Bang(t_deinterleave *x)
{
	Output(x);
}

/**************************************************************************************
 * Output our stuff.
 *************************************************************************************/

void Output(t_deinterleave *x)
{
	

	if (x->t_outsize > 0)
	{
	
		if (x->num_outputs >= 4)
			outlet_list(x->t_out[3], ps_list, x->t_outsize, x->t_list_out[3]);
	
		if (x->num_outputs >= 3)
			outlet_list(x->t_out[2], ps_list, x->t_outsize, x->t_list_out[2]);

		if (x->num_outputs >= 2)
			outlet_list(x->t_out[1], ps_list, x->t_outsize, x->t_list_out[1]);

		outlet_list(x->t_out[0], ps_list, x->t_outsize, x->t_list_out[0]);

	}	
} 
