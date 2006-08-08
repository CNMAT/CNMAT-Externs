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

Written by Tim Madden, The Center for New Music and Audio
Technologies, University of California, Berkeley.

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
NAME: interleave
DESCRIPTION: Interleave multiple input lists to one output list.  E.g., if inputs are "A B C" and "1 2 3", the output will be "A 1 B 2 C 3".
AUTHORS: Tim Madden
COPYRIGHT_YEARS: 2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 1.0: Tim's original version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/


/**************************************************************************************
 * Max object to take 2 to 4 lists asinput and interleave them into 1.
 * 
 * Example: input: 1 2 3 4,   0 9 8 7
 *			output: 1 0 2 9 3 8 4 7
 *
 * This is the DSP interpolation algorithm. 
 * 
 * Inputs: 
 *	
 * 		list	input data. leftmost triggers output.
 *		bang	repeat output.
 *		list1,list2,list3,list4  In leftmost inlet, equivlent to list in the other
 *								inlets, except no output is triggered.
 *							
 *		output	Same as bang. 		
 *************************************************************************************/
/*
 * Includes.
 */
 
#define INTERLEAVE_VERSION "1.0"

#include "ext.h"

// Define the max number of args.
#define MAX_INARGS 256
#define MAX_OUTARGS 1024


// Structure for this type of object.
typedef struct t_interleave
{
	struct object t_obj;
	void *t_out;	
	Atom t_list_out[MAX_OUTARGS];
	void *t_proxy[3];
	long t_which_inlet;
	int t_outsize;
	int num_inputs;
	int sublist_sizes[4];		// -1 before we get a list, else length of that list
} t_Interleave;

Symbol *ps_list;

void *interleave_class;

/*
 * Function Prototypes.
 */

// Make new object.
void *interleave_new(
	Symbol *mess,	
	int ac,
	Atom *av);

void interleave_free(t_Interleave *x);
	
	
// Send our output data again.	
void Bang(t_Interleave *x);

// Respond to list.
void List(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

	
// Respond to list1, in left inlet.
void List1(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);


// Respond to list2 in left inlet.
void List2(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// list3 inleft inlet.
void List3(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// REspond to list4 in left inlet.
void List4(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Snap frequencies to the grid.
void Output(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Altorithm for interleaving data from first inlet, or list1 command.
void List1guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Algoritm for interleaving data from second inlet or list2.
void List2guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Algoritm for interleaving data from third inlet or list3.
void List3guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Algoritm for interleaving data from fourth inlet or list4.
void List4guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);

// Algoruthm to output the data.
void Outputguts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
// REspond to list command.	
void List(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv);
	
void interleave_version (t_Interleave *x);

/**************************************************************************************
 * Set up object.
 *************************************************************************************/


void main(void)
{
	

	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&interleave_class, (method)interleave_new, (method) interleave_free,
			(short)sizeof(t_Interleave), 0L, 
			A_GIMME, 0);
			
	addbang((method)Bang);			
	addmess((method)List, "list", A_GIMME, 0);
	addmess((method)List1, "list1", A_GIMME, 0);
	addmess((method)List2, "list2", A_GIMME, 0);
	addmess((method)List3, "list3", A_GIMME, 0);
	addmess((method)List4, "list4", A_GIMME, 0);
	addmess((method)Output, "output", A_GIMME, 0);
	addmess((method)interleave_version, "version", 0);

	ps_list = gensym("list");


}


/**************************************************************************************
 * make new object.
 *************************************************************************************/

void *interleave_new(
	Symbol *mess,
	int ac,
	Atom *av)
{
	int i;

	t_Interleave *x;
	

	x = (t_Interleave*)newobject(interleave_class);
	
	x->t_out = listout(x);
	
	/*
	 * Parse args.
	 */
	 
	if (ac == 0)
	{
		// If no args, then 2 inlets.
		x->num_inputs = 2;
	}
	else if (ac == 1)
	{
		// If 1 arg, then set inlets.
		x->num_inputs = av[0].a_w.w_long;
	}
	else
	{
		// If 2= args, then ac arguments sets up ac inlets.
		x->num_inputs = ac;
	}

	// Allow up to 4 inlets.
	if (x->num_inputs > 4)
		x->num_inputs = 4;
		
	if (x->num_inputs < 2)
		x->num_inputs = 2;
		

	// Make our proxies based on num-inputs.
	if (x->num_inputs == 2)
		x->t_proxy[0] = proxy_new(x, 1L, &x->t_which_inlet);

	if (x->num_inputs == 3)
	{
		x->t_proxy[1] = proxy_new(x, 2L, &x->t_which_inlet);
		x->t_proxy[0] = proxy_new(x, 1L, &x->t_which_inlet);
	}
	
	if (x->num_inputs == 4)
	{
		x->t_proxy[2] = proxy_new(x, 3L, &x->t_which_inlet);
		x->t_proxy[1] = proxy_new(x, 2L, &x->t_which_inlet);
		x->t_proxy[0] = proxy_new(x, 1L, &x->t_which_inlet);
	}
	
	for (i = 0; i < x->num_inputs; ++i) {
		x->sublist_sizes[i] = -1;
	}
	
	
	return(x);
}

/**************************************************************************************
 * Free proxies when this object goes away.
 *************************************************************************************/

void interleave_free(t_Interleave *x)
{
		// Free our proxies based on num-inputs.
	if (x->num_inputs >= 2)
		freeobject(x->t_proxy[0]);

	if (x->num_inputs >= 3)
		freeobject(x->t_proxy[1]);
	
	if (x->num_inputs >= 4)
		freeobject(x->t_proxy[2]);	
}


/**************************************************************************************
 * Main list function. Respond to a list in any inlet.
 *************************************************************************************/

void List(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	
	// Which input?
	switch (x->t_which_inlet)
	{
		case 0:
			List1guts(x, mess, argc, argv);
			Outputguts(x, mess, argc, argv);
			break;
			
		case 1:
			List2guts(x, mess, argc, argv);
			break;
			
		case 2:
			List3guts(x, mess, argc, argv);
			break;
			
		case 3:
			List4guts(x, mess, argc, argv);
			break;
			
	}
}

/**************************************************************************************
 * Main list function. Respond to a list in any inlet.
 *************************************************************************************/

void Bang(t_Interleave *x)
{
	Outputguts(x, 0, 0, 0);
}


/**************************************************************************************
 * Respond to list1.
 *************************************************************************************/

void List1(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{	
	List1guts(x, mess, argc, argv);
}

/**************************************************************************************
 * Respond to list1.
 *************************************************************************************/

void Output(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{	
	Outputguts(x, mess, argc, argv);
}


/**************************************************************************************
 * Respond to list2.
 *************************************************************************************/

void List2(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	List2guts(x, mess, argc, argv);
}

/**************************************************************************************
 * Respond to list3.
 *************************************************************************************/

void List3(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	List3guts(x, mess, argc, argv);
}

/**************************************************************************************
 * Respond to list4.
 *************************************************************************************/

void List4(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{	
	List4guts(x, mess, argc, argv);
}

/**************************************************************************************
 * Respond to list1 message. Put args into list with proper interleaving, and output
 * a list.
 *************************************************************************************/

void List1guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	Symbol message;
	int counter;
	int i;
	message.s_name = "list";
	
	// Set counter for output array.
	counter = 0;
	
	// Put args into storage.
	for (i = 0; i < argc; i++)
	{

#ifdef THE_HARD_WAY	
		x->t_list_out[counter].a_type = argv[i].a_type;
		switch (argv[i].a_type)
		{
			case A_FLOAT:
				x->t_list_out[counter].a_w.w_float = argv[i].a_w.w_float;
				break;
			case A_LONG:
				x->t_list_out[counter].a_w.w_long = argv[i].a_w.w_long;
				break;
		}
#else
		x->t_list_out[counter] = argv[i];
#endif
		
		// increment counter.
		counter+= x->num_inputs;
	}
	
	x->sublist_sizes[0] = argc;
	// Set outsize.
	x->t_outsize = argc * x->num_inputs;

	 
}

/**************************************************************************************
 * Ouput the data.
 *************************************************************************************/

void Outputguts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int i;
	
	for (i = 0; i < x->num_inputs; ++i) {
		if (x->sublist_sizes[i] < 0) {
			post("¥ InterleaveList: can't output because input list(s) missing.");
			return;
		}
	}
	
	for (i = 1; i < x->num_inputs; ++i) {
		if (x->sublist_sizes[i] != x->sublist_sizes[0]) {
			post("¥ InterleaveList: input list(s) are not all the same length.");
			return;
		}
	}
	
	// We ouput data with list1 message only.
	outlet_list(x->t_out, ps_list, x->t_outsize, x->t_list_out);
}


/**************************************************************************************
 * Respond to list2 message. Put args into list with proper interleaving, dont output
 * a list.
 *************************************************************************************/

void List2guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int counter;
	int i;

	// Set counter for output array.
	counter = 1;
	
	// Put args into storage.
	for (i = 0; i < argc; i++)
	{
	
#ifdef THE_HARD_WAY	
		x->t_list_out[counter].a_type = argv[i].a_type;
		switch (argv[i].a_type)
		{
			case A_FLOAT:
				x->t_list_out[counter].a_w.w_float = argv[i].a_w.w_float;
				break;
			case A_LONG:
				x->t_list_out[counter].a_w.w_long = argv[i].a_w.w_long;
				break;
		}
#else
		x->t_list_out[counter] = argv[i];
#endif
		
		// increment counter.
		counter+= x->num_inputs;
	}
	
	x->sublist_sizes[1] = argc;

//	ExitCallback();
}


/**************************************************************************************
 * Respond to list3 message. Put args into list with proper interleaving, dont output
 * a list.
 *************************************************************************************/

void List3guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int counter;
	int i;
//	EnterCallback();

	// Set counter for output array.
	counter = 2;
	
	// Put args into storage.
	for (i = 0; i < argc; i++)
	{
#ifdef THE_HARD_WAY	
		x->t_list_out[counter].a_type = argv[i].a_type;
		switch (argv[i].a_type)
		{
			case A_FLOAT:
				x->t_list_out[counter].a_w.w_float = argv[i].a_w.w_float;
				break;
			case A_LONG:
				x->t_list_out[counter].a_w.w_long = argv[i].a_w.w_long;
				break;
		}
#else
		x->t_list_out[counter] = argv[i];
#endif

		// increment counter.
		counter+= x->num_inputs;
	}
	
	
	x->sublist_sizes[2] = argc;
}


/**************************************************************************************
 * Respond to list4 message. Put args into list with proper interleaving, dont output
 * a list.
 *************************************************************************************/

void List4guts(
	t_Interleave *x, 
	Symbol *mess,
	int argc, 
	Atom *argv)
{
	int counter;
	int i;
	
	// Set counter for output array.
	counter = 3;
	
	// Put args into storage.
	for (i = 0; i < argc; i++)
	{
#ifdef THE_HARD_WAY	
		x->t_list_out[counter].a_type = argv[i].a_type;
		switch (argv[i].a_type)
		{
			case A_FLOAT:
				x->t_list_out[counter].a_w.w_float = argv[i].a_w.w_float;
				break;
			case A_LONG:
				x->t_list_out[counter].a_w.w_long = argv[i].a_w.w_long;
				break;
		}
#else
		x->t_list_out[counter] = argv[i];
#endif
		
		// increment counter.
		counter+= x->num_inputs;
	}
	
	x->sublist_sizes[3] = argc;
}


void interleave_version (t_Interleave *x) {
	post("interleave Version " INTERLEAVE_VERSION
		  ", by Tim Madden. Compiled " __TIME__ " " __DATE__);	
}

