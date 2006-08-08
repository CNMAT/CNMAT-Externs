/* dereffer MAX object

SVN_REVISION: $LastChangedRevision$
DESCRIPTION:  Convert binary buffers (e.g., from received UDP packets) which are presumed to consist only of single null-terminated ASCII strings to Max symbols.

   Brian Whitman, WPI 4/22/99

   For use with Matt Wright's OTUDP object / CNMAT

   Dereferences pointer message and creates three outlets:

   a bang,

   an int (size),

   and a symbol (message)

   

   email: bwhitman@crudites.org



  */



/* the required include files */

#include "ext.h"

#include <SetUpA4.h>

#include <A4Stuff.h>

#include <string.h>

#include <stdio.h>

#include <stdlib.h>



/* structure definition of your object */



typedef struct myobject

{

	Object m_ob;				// required header

	void *m_out;  			// integer outlet (size of string)

	void *m_out_b;				// bang outlet (we got a message!)

	void *m_out_s;				// string outlet (the message)

	long m_value;				// example field: store a value

} MyObject;



/* global necessary for 68K function macros to work */

fptr *FNS;



/* globalthat holds the class definition */

void *myobject_class;



/* prototypes for your functions */



void myobject_int(MyObject *x, long n);

void myobject_FullPacket(MyObject *x, long size, long ref);

void myobject_bang(MyObject *x);

void myobject_set(MyObject *x, long n);

void *myobject_new(long n);



/* initialization routine */



void main(fptr *f)

{

	long oldA4;

	

	// this code is a 68K-only macro

	oldA4 = SetCurrentA4();			// get correct value of A4

	RememberA4();					// store inside code resource

	// this is not necessary (but harmless) on PowerPC

	FNS = f;	

	

	/* tell Max about your class. The cast to short is important for 68K */

	setup(&myobject_class, myobject_new,0L, (short)sizeof(MyObject), 0L, A_DEFLONG, 0);

	/* bind your methods to symbols */

	addbang((method)myobject_bang);

	addint((method)myobject_int);

	addmess((method)myobject_FullPacket, "FullPacket", A_LONG, A_LONG, 0);

	addmess((method)myobject_set, "set", A_LONG, 0);

	/* list object in the new object list */

	finder_addclass("Data","dereffer");

	// restore old value of A4 (68K only)

	RestoreA4(oldA4);

}



/* example methods */



void myobject_bang(MyObject *x)

{

	EnterCallback();	// necessary on 68K for methods that use global variables

						// or Max functions. #defined to nothing on PowerPC

						

	outlet_int(x->m_out,x->m_value);	// send stored value out an outlet

	

	ExitCallback();		// undoes EnterCallback

}



void myobject_int(MyObject *x, long n)

{

	EnterCallback();

	

	x->m_value = n+3;						// store a value inside the object

	outlet_int(x->m_out,x->m_value);	// and send it out

	

	ExitCallback();

}



void myobject_FullPacket(MyObject *x, long size, long ref)

{

	

	Atom a;



	EnterCallback();

	a.a_type=A_SYM;

	

	a.a_w.w_sym=gensym((char *) ref); // here is where any arguments would go,

						              // but for VAMP's purposes at least.

							          // "the argument is the message."



	x->m_value = size;

	outlet_int(x->m_out,x->m_value);

	outlet_bang(x->m_out_b);

	outlet_anything(x->m_out_s,gensym("VAMP"),1, &a); 

	// so in the VAMP external (whenever that happens), there needs just be a 

	// void my_object_VAMP(MyObject *x, symbol * message)...



	ExitCallback();

}

	

	



void myobject_set(MyObject *x, long n)

{

	// no need for EnterCallback/ExitCallback, since globals/Max functions are not

	// accessed

	x->m_value = n;		// just store incoming value without outputting it

}



/* instance creation routine */



void *myobject_new(long n)

{

	MyObject *x;

	

	EnterCallback();

	x = newobject(myobject_class);		// get memory for a new object & initialize

	x->m_value = n;						// store value (default is 0)

	x->m_out = intout(x);				// create an int outlet

	x->m_out_b = bangout(x);

	x->m_out_s = outlet_new(x,0L)

	ExitCallback();

	return (x);							// return newly created object to caller

}



	

	



	
