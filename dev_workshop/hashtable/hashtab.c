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

/* Description of the object:
A simple object that will delay any incoming data by a specified amount of time.

We will use schedule_delay()--a function new to the Max5 SDK--to schedule the delay.  This 
has an advantage over the traditional method of creating a clock object in that it is simpler
to use.  However, once something has been scheduled for execution at a later time, it cannot
be unscheduled.  To get around this, we will use a state variable.  Whenever we schedule something,
we will prepend the value of the state variable to the message that we schedule, and only outlet that message
if the object's state matches the state contained in the message.  If the user sends the message
``stop'', we will increment the value of the state variable causing all messages scheduled
with the old value to be ignored.
 */

#include "ext.h"		// must be included with all Max objects
#include "ext_obex.h"		// attribute support and useful object extensions such as atom_getfloat() 
#include "ext_obex_util.h"	// macros that make attributes easier to deal with


// This struct will hold your object's instance dada.  The first field must always be a t_object.  
typedef struct _ppn{
	t_object ob;
	void *outlet; 	// a pointer to our object's only outlet
	long in;	// when a message is received in an inlet, the inlet number will be stored here.
	void *proxy;	// necessary for objects with multiple inlets
	long delay_time_ms;  // our delay time in milliseconds will be stored here
	long state;  	// see the description above for an explanation of this
} t_ppn;

// A pointer to your object's class.
static t_class *ppn_class;

// Standard Max callbacks
void ppn_bang(t_ppn *x);
void ppn_int(t_ppn *x, long n);
void ppn_float(t_ppn *x, double xx);
void ppn_list(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_anything(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_free(t_ppn *x);
void ppn_assist(t_ppn *x, void *b, long m, long a, char *s);
void ppn_inletinfo(t_ppn *x, void *b, long index, char *t);
void *ppn_new(long deltime);

// Functions specific to this object
void ppn_callback(t_ppn *x, t_symbol *s, unsigned short argc, t_atom *argv);
void ppn_stop(t_ppn *x);

// This function will be called when the object receives a bang in either inlet
void ppn_bang(t_ppn *x){
	// Call proxy_getinlet() to find out which inlet the message was received in
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: schedule a bang for execution later.  Also, pass along the state
		// see the description above for an explanation of the state variable.
		{
			t_atom a;
			atom_setlong(&a, x->state);
			schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, _sym_bang, 1, &a);
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
		// left inlet: create a temp array and set the state as the first element, then 
		// schedule it to be output later
		{
			t_atom a[2];
			atom_setlong(&(a[0]), x->state);
			atom_setlong(&(a[1]), n);
			schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, _sym_int, 2, a);
		}
		break;
	case 1:
		// right inlet: set the delay time to the number received.
		if(n < 0){
			error("delay: delay time must be greater than 0");
			return;
		}
		x->delay_time_ms = n;
		break;
	}
}

// This function will be called when the object receives a float in either inlet
void ppn_float(t_ppn *x, double f){
	// Call proxy_getinlet() to find out which inlet the message was received in
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		// left inlet: create a temp array and set the state as the first element, then 
		// schedule it to be output later
		{
			t_atom a[2];
			atom_setlong(&(a[0]), x->state);
			atom_setfloat(&(a[1]), f);
			schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, _sym_float, 2, a);
		}
		break;
	case 1:
		// right inlet: set the delay time to the number received.
		if(f < 0){
			error("delay: delay time must be greater than 0");
			return;
		}
		x->delay_time_ms = (long)f;
		break;
	}
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
		{
			t_atom a[argc + 1];
			atom_setlong(&(a[0]), x->state);
			memcpy(&(a[1]), argv, argc * sizeof(t_atom));
			schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time_ms, msg, argc + 1, a);
		}
		break;
	case 1:
		// right inlet: we don't understand, so just return
		break;
	}
}

// This will be called when your object is removed from the patcher.  If any memory has been allocated
// using malloc(), free it here.
void ppn_free(t_ppn *x){
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
	// This shouldn't happen under any circumstances, but if it does, 
	// post an error an bail out
	if(argc == 0){
		error("postpone: something really bad happened");
		return;
	}
	// If the current state of our object doesn't match the state of the message that we 
	// just received, bail out.
	if(atom_getlong(argv) != x->state){
		return;
	}

	// Output our data.
	if(s){
		if(s == _sym_bang){
			outlet_bang(x->outlet);
		}else if(s == _sym_int){
			outlet_int(x->outlet, atom_getlong(argv + 1));
		}else if(s == _sym_float){
			outlet_int(x->outlet, atom_getfloat(argv + 1));
		}else if(s == _sym_list){
			outlet_list(x->outlet, NULL, argc - 1, argv + 1);
		}else{
			outlet_anything(x->outlet, s, argc - 1, argv + 1);
		}
	}
}

// If the user sends the stop message, increment the state variable
// to prevent any further messages that may have been scheduled
// from being output.
void ppn_stop(t_ppn *x){
	x->state++;
}

/**************************************************
Object and instance creation functions.
 **************************************************/

// Our object's new instance routine.  This will be called each time our object is 
// instantiated and is where we will set up our inlets and outlets, and any state
// variables we may have.  Our object takes one argument--the delay time in 
// milliseconds.  Note that if this argument were a floating point number instead of
// an integer, we would have to use double, not float.
void *ppn_new(long deltime){
	// our object instance
	t_ppn *x;

	// allocate our object and make sure object_alloc() didn't return NULL
	if(x = (t_ppn *)object_alloc(ppn_class)){
		// Set up a proxy to handle two inlets.  The second argument to proxy_new() is an id
		// that will allow us to identify this particular proxy, and the third argument 
		// is a pointer to a location where the id value of the inlet will be written.
		// Don't access the value of x->in directly--instead use proxy_getinlet() to 
		// determine the inlet number.
		x->proxy = proxy_new((t_object *)x, 1, &x->in);
		// Create an outlet that can handle any type of data and store a pointer to it.
		x->outlet = outlet_new(x, NULL);
		// Set the delay time.  Since we declared the argument to ppn_new() using A_DEFLONG,
		// if no argument is present, it will default to 0. (see main() below)
		x->delay_time_ms = deltime;
		// Set the state.
		x->state = 0;
		// return our object.
		return x;
	}
	// return NULL if our object couldn't be created with object_alloc() for some reason.
	return NULL;
}

// Our object's class initialization routine.  This will only be called the first time your object
// is instantiated.  This is a good place to print the version number of your object since
// it will only print the first time your object is used.
int main(void){
	// Create our class.  Starting with the 6th argument, we can add up to 9 arguments that 
	// will specify the types of arguments that our object will take.  Here, our object will 
	// only take one argument--an integer.  We want that argument to be optional so we use 
	// A_DEFLONG instead of A_LONG.
	t_class *c = class_new("postpone", (method)ppn_new, (method)ppn_free, sizeof(t_ppn), 0L, A_DEFLONG, 0);
	
	// The following function calls register callbacks that we have implemented and that 
	// will be called when our object receives certain messages.  After the name of the message
	// (the 3rd argument), we list the types of arguments that the message can have.  Here we see
	// two special arguments:  A_GIMME which means that the arguments will be passed in as an array
	// to a callback that must have the signature
	//
	// void function(t_myobj *x, t_symbol *msg, short argc, t_atom *argv);
	//
	// and A_CANT which means that that method can't be called from a max patch.
	class_addmethod(c, (method)ppn_bang, "bang", 0);
	class_addmethod(c, (method)ppn_int, "int", A_LONG, 0);
	class_addmethod(c, (method)ppn_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)ppn_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)ppn_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)ppn_assist, "assist", A_CANT, 0);
	// We could set stdinletinfo() as the callback for the ``inletinfo'' message and it would
	// mark every inlet to the right of the leftmost inlet as cold.
	class_addmethod(c, (method)ppn_inletinfo, "inletinfo", A_CANT, 0);
	class_addmethod(c, (method)ppn_stop, "stop", 0);

	// Register our class and store a pointer to it.
	class_register(CLASS_BOX, c);
	ppn_class = c;

	// Initialize a symbol table that we will use in this object.  This gives us access to 
	// the symbols contained in commonsyms.h
	common_symbols_init();

	// Return any int here--it doesn't matter.
	return 0;
}

