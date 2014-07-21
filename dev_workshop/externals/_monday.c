/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009, The Regents of
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
NAME: postpone
DESCRIPTION: Delay anything.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"			// must be included with all Max objects
#include "ext_obex.h"		// attribute support and useful object extensions such as atom_getfloat() 
#include "ext_obex_util.h"	// macros that make attributes easier to deal with


typedef struct _ppn{
	t_object ob;
	void *outlet; 	
	long in;				// when a message is received in an inlet, the inlet number will be stored here.
	void *proxy;			// necessary for objects with multiple inlets
	long delay_time_ms;		// our delay time in milliseconds will be stored here
} t_ppn;

static t_class *ppn_class;

void ppn_bang(t_ppn *x);
void ppn_int(t_ppn *x, long n);
void ppn_float(t_ppn *x, double xx);
void ppn_list(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_anything(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_free(t_ppn *x);
void ppn_assist(t_ppn *x, void *b, long m, long a, char *s);
void ppn_inletinfo(t_ppn *x, void *b, long index, char *t);
void ppn_callback(t_ppn *x, t_symbol *s, unsigned short argc, t_atom *argv);
void *ppn_new(long deltime);

// This function will be called when the object receives a bang in either inlet
void ppn_bang(t_ppn *x){
	// Call proxy_getinlet() to find out which inlet the message was received in
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: schedule a bang for execution later.  
		{
			schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, _sym_bang, 0, NULL);
		}
		break;
	case 1:
		// right inlet:  doesn't understand bang, so do nothing
		break;
	}
}

// This function will be called when the object receives an int in either inlet
void ppn_int(t_ppn *x, long n){
	// Call proxy_getinlet() to find out which inlet the message was received in
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: create an atom and set the contents of it to the value of n
		// then call schedule_delay().
		break;
	case 1:
		// right inlet: set the delay time to the number received.
		break;
	}
}

// This function will be called when the object receives a float in either inlet
// This should look very similar to ppn_int()
void ppn_float(t_ppn *x, double f){
	
}

// This function will be called when the object receives a list in either outlet
void ppn_list(t_ppn *x, t_symbol *msg, short argc, t_atom *argv){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: call ppn_anything with "list" as the message selector
		ppn_anything(x, _sym_list, argc, argv);
		break;
	case 1:
		// right inlet: a list in the right inlet doesn't make sense, so just return
		break;
	}
}

// This will be called when the object receives a message in either inlet
void ppn_anything(t_ppn *x, t_symbol *msg, short argc, t_atom *argv){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: create a temp array and set the state as the first element, then
		// copy the contents of argv into the array and schedule it for delay.
		schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, msg, argc, argv);
		break;
	case 1:
		// right inlet: we don't understand, so just return
		break;
	}
}

// This will be called when your object is removed from the patcher.  If any memory has been allocated
// using malloc(), free it here.
void ppn_free(t_ppn *x){
	// We have to free our proxy
	object_free(x->proxy);
}

// Called when a user hovers the mouse over an inlet.  Use sprintf() to format
// an assistance string based on which inlet/outlet the mouse is over.
void ppn_assist(t_ppn *x, void *b, long io, long index, char *s){
	// io will be 1 if the mouse is over an inlet and 2 if it is over an outlet
	switch(io){
	case 1:
		// index is the inlet/outlet number
		switch(index){
		case 0:
			sprintf(s, "anything gets delayed");
			break;
		case 1:
			sprintf(s, "set the delay time in milliseconds");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "delayed version of input");
			break;
		}
		break;
	}
}

// Use this callback to tell Max which inlets are hot and which are cold.  Inlets are hot
// by default, so we just have to mark the right one as cold.
void ppn_inletinfo(t_ppn *x, void *b, long index, char *t){
	if(index == 1){
		*t = 1;
	}
}

// This is our function that will be called in response to schedule_delay().
void ppn_callback(t_ppn *x, t_symbol *s, unsigned short argc, t_atom *argv){
}

/**************************************************
Object and instance creation functions.
 **************************************************/

// Our object's new instance routine.  This will be called each time our object is 
// instantiated and is where we will set up our inlets and outlets, and any state
// variables we may have.  
void *ppn_new(void){
	t_ppn *x;

	if(x = (t_ppn *)object_alloc(ppn_class)){
		// Set up a proxy to handle two inlets.  The second argument to proxy_new() is an id
		// that will allow us to identify this particular proxy, and the third argument 
		// is a pointer to a location where the id value of the inlet will be written.
		// Don't access the value of x->in directly--instead use proxy_getinlet() to 
		// determine the inlet number.
		x->proxy = proxy_new((t_object *)x, 1, &x->in);

		x->outlet = outlet_new(x, NULL);
		
		x->delay_time_ms = deltime;

		return x;
	}
	return NULL;
}

// Our object's class initialization routine.  This will only be called the first time your object
// is instantiated.  This is a good place to print the version number of your object since
// it will only print the first time your object is used.
int main(void){
	t_class *c = class_new("postpone", (method)ppn_new, (method)ppn_free, sizeof(t_ppn), 0L, 0);
	
	// you must write class_addmethod() calls to register your object to respond to the following messages:
	// bang, int, float, list, anything
	
	
	class_addmethod(c, (method)ppn_assist, "assist", A_CANT, 0);
	// We could set stdinletinfo() as the callback for the ``inletinfo'' message and it would
	// mark every inlet to the right of the leftmost inlet as cold.
	class_addmethod(c, (method)ppn_inletinfo, "inletinfo", A_CANT, 0);

	class_register(CLASS_BOX, c);
	ppn_class = c;
	
	// Initialize a symbol table--this allows us to use predefined symbols like _sym_bang above
	common_symbols_init();

	return 0;
}

