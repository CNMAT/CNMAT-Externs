/*
Copyright (c) 2001,2,3,4,5.  
The Regents of the University of California (Regents).
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

 printit.c
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: printit
DESCRIPTION: Really print everything about what comes in the inlet
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2000,01,02,03,04,05
VERSION 0.1b: Earliest version I could find
VERSION 0.2: Using new version system
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  
 */
 
#include "version.h"
 

/* the required include files */
#include "ext.h"

/* structure definition of your object */

typedef struct printit
{
	Object o_ob;				// required header
	Symbol *my_name;
} printit;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *printit_class;

/* prototypes  */

void printit_bang(printit *x);
void printit_float(printit *x, double d);
void printit_int(printit *x, long n);
void printit_list(printit *x, Symbol *s, short argc, Atom *argv);
void printit_anything(printit *x, Symbol *s, short argc, Atom *argv);
void *printit_new(Symbol *s);
void printit_version (printit *x);
void printit_assist (printit *x, void *box, long msg, long arg, char *dstString);

Symbol *ps_emptysymbol;
Symbol *ps_printit;

/* initialization routine */

void main(fptr *f)
{
	/* tell Max about your class. The cast to short is important for 68K */
	setup((t_messlist **)&printit_class, (method) printit_new, 0L, (short)sizeof(printit), 
		  0L, A_DEFSYM, 0);


	/* bind your methods to symbols */
	addbang((method)printit_bang);
	addint((method)printit_int);
	addfloat((method)printit_float);
	addmess((method)printit_list, "list", A_GIMME, 0);	
	addmess((method)printit_anything, "anything", A_GIMME, 0);
	addmess((method)printit_assist, "assist", A_CANT, 0);
	addmess((method)printit_version, "version", 0);

	ps_emptysymbol = gensym("");
	ps_printit = gensym("printit");
	
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
}



/* instance creation routine */

void *printit_new(Symbol *s)
{
	printit *x;	
	
	x = newobject(printit_class);		// get memory for a new object & initialize
	if (s == ps_emptysymbol) {
		x->my_name = ps_printit;
	} else {
		x->my_name = s;
	}

	return (x);
}


void printit_version (printit *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void printit_assist (printit *x, void *box, long msg, long arg, char *dstString) {
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "Anything to print");
	} else if (msg==ASSIST_OUTLET) {
		sprintf(dstString, "No outlet");
	} else {
		post("¥ printit_assist: unrecognized message %ld", msg);
	}
}


void printit_bang(printit *x)
{
	post("%s: received a bang.", x->my_name->s_name);
}

void printit_float(printit *x, double d)
{
	float f = (float) d;
	post("%s: received a float: %f (which was passed on the stack as a double: %d)", x->my_name->s_name, f, d);
}

void printit_int(printit *x, long n)
{
	post("%s: received an int: %ld", x->my_name->s_name, n);
}

void printit_anything(printit *x, Symbol *s, short argc, Atom *argv) {
	int i;
	
	post("%s: received MESSAGE \"%s\" (%p, s_thing %p) with %d argument(s):", 
	     x->my_name->s_name, s->s_name, s, s->s_thing, argc);
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type == A_SYM) {
			post(" SYMBOL \"%s\" (%p, s_thing %p)",
			     argv[i].a_w.w_sym->s_name, argv[i].a_w.w_sym, argv[i].a_w.w_sym->s_thing);
		} else if (argv[i].a_type == A_LONG) {
			post(" LONG   %ld", argv[i].a_w.w_long);
		} else if (argv[i].a_type == A_FLOAT) {
			post(" FLOAT  %f", argv[i].a_w.w_float);
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}

void printit_list(printit *x, Symbol *s, short argc, Atom *argv) {
	int i;
	
	post("%s: received LIST with %d argument(s):", x->my_name->s_name, argc);
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type == A_SYM) {
			post(" SYMBOL \"%s\" (%p, s_thing %p)",
			     argv[i].a_w.w_sym->s_name, argv[i].a_w.w_sym, argv[i].a_w.w_sym->s_thing);
		} else if (argv[i].a_type == A_LONG) {
			post(" LONG   %ld", argv[i].a_w.w_long);
		} else if (argv[i].a_type == A_FLOAT) {
			post(" FLOAT  %f", argv[i].a_w.w_float);
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}
