/*
Copyright (c) 2002.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

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

 SNNS-wrapper.c

DESCRIPTION: Turn an SNNS-generated network-forward-pass C file into a Max object.
SVN_REVISION: $LastChangedRevision$
 
 To change the SNNS file, do this:
 	1) Add the new .c file to the project
 	2) Under Edit / SNNSPPC Settings, go to the "PPC Target" panel and change the "File Name"
 	   to whatever you want your external to be called.
    3) Search for all the comments in this file that say "HEY!" and change the code.
 
 */
 

#define SNNS_VERSION "0.0"

/* the required include files */
#include "ext.h"


/* HEY!  put your .h file here: */
#include "test.h"

/* HEY!  put the numbers of inputs and outputs here: */
#define NUM_INPUTS 2
#define NUM_OUTPUTS 60


/* structure definition of your object */

typedef struct SNNS
{
	Object o_ob;				// required header
	void *outlet;
	float inputFloats[NUM_INPUTS];
	float outputFloats[NUM_OUTPUTS];
	Atom outputMaxList[NUM_OUTPUTS];
} SNNS;


void *SNNS_class;


/* prototypes  */
void SNNS_float(SNNS *x, float f);
void SNNS_int(SNNS *x, long n);
void SNNS_list(SNNS *x, Symbol *s, short argc, Atom *argv);
void SNNS_anything(SNNS *x, Symbol *s, short argc, Atom *argv);
void *SNNS_new(Symbol *s);
void SNNS_version (SNNS *x);
void SNNS_assist (SNNS *x, void *box, long msg, long arg, char *dstString);



/* initialization routine */

void main(fptr *f)
{
	setup((t_messlist **)&SNNS_class, (method)SNNS_new,0L, (short)sizeof(SNNS), 0L, 0);
	/* bind your methods to symbols */
	addmess((method)SNNS_anything, "anything", A_GIMME, 0);
	addmess((method)SNNS_list, "list", A_GIMME, 0);
	addmess((method)SNNS_assist, "assist", A_CANT, 0);
	addmess((method)SNNS_version, "version", 0);
	addint((method)SNNS_int);
	addfloat((method)SNNS_float);
	
	
	
	post("SNNS wrapping object version " SNNS_VERSION " by Matt Wright. ");
	post("Copyright © 2002 Regents of the University of California. All Rights Reserved.");

}



/* instance creation routine */

void *SNNS_new(Symbol *s)
{
	SNNS *x;
	int i;
		
	x = newobject(SNNS_class);		// get memory for a new object & initialize
	
	x->outlet = listout(x);
	
	for (i = 0; i < NUM_OUTPUTS; ++i) {
		x->outputMaxList[i].a_type = A_FLOAT;
		x->outputMaxList[i].a_w.w_float = -99999.9;
	}
	
	return (x);
}


void SNNS_version (SNNS *x) {
	post("SNNS wrapper version " SNNS_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void SNNS_assist (SNNS *x, void *box, long msg, long arg, char *dstString) {	
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "List of network input values");
	} else if (msg==ASSIST_OUTLET) {
		sprintf(dstString, "List of network output values");
	} else {
		post("¥ SNNS_assist: unrecognized message %ld", msg);
	}
}


#define ATOM_AS_FLOAT(a)  (((a).a_type == A_LONG) ? ((float) (a).a_w.w_long) : ((a).a_w.w_float))

void SNNS_list(SNNS *x, Symbol *s, short argc, Atom *argv) {
	int i;
	
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type == A_SYM) {
			post("¥ SNNS: error: symbols are not allowed in the input list.");
			return;
		}
	}	
	
	if (argc < NUM_INPUTS) {
		post("¥ SNNS: warning: network has %ld inputs but you sent only %ld numbers.",
		     NUM_INPUTS, argc);
		post("  Setting extra network inputs to zero.");
		for (i = 0; i < argc; ++i) {
			x->inputFloats[i] = ATOM_AS_FLOAT(argv[i]);
		}
		for (i = argc; i < NUM_INPUTS; ++i) {
			x->inputFloats[i] = 0.0f;
		}
	} else {
		if (argc > NUM_INPUTS) {
			post("¥ SNNS: warning: input list has %ld numbers, but network has ", argc);
			post("  only %ld inputs.  Ignoring extra list elements.", NUM_INPUTS);
		}
		for (i = 0; i < NUM_INPUTS; ++i) {
			x->inputFloats[i] = ATOM_AS_FLOAT(argv[i]);
		}
	}

	// post("*** Input list:");
	// for (i = 0; i < NUM_INPUTS; ++i) {
	//  	post("   %f", x->inputFloats[i]);
	// }	

	// post("*** calling network...");

	/* HEY!  Make sure the right function is being called here */
	te(x->inputFloats, x->outputFloats, 0);

	// post("*** done calling network.");
		
	for (i = 0; i < NUM_OUTPUTS; ++i) {
		x->outputMaxList[i].a_w.w_float = x->outputFloats[i];
	}
	
	outlet_list(x->outlet, 0L, NUM_OUTPUTS, x->outputMaxList);
}
	
	
void SNNS_anything(SNNS *x, Symbol *s, short argc, Atom *argv) {
	SNNS_list(x, s, argc, argv);
}

void SNNS_int(SNNS *x, long n) {
	Atom a[1];
	
	a[0].a_type = A_FLOAT;
	a[0].a_w.w_float = (float) n;
	
	SNNS_list(x, 0, 1, a);
}


void SNNS_float(SNNS *x, float f) {
	Atom a[1];
	
	a[0].a_type = A_FLOAT;
	a[0].a_w.w_float = f;
	
	SNNS_list(x, 0, 1, a);
}
