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
COPYRIGHT_YEARS: 2000-06
DRUPAL_NODE: /patch/4024
SVN_REVISION: $LastChangedRevision$
VERSION 0.1b: Earliest version I could find
VERSION 0.2: Using new version system
VERSION 0.2.1: Force Package Info Generation
VERSION 0.3: Added support for many more a_type possibilities found in ext_mess.h
VERSION 0.4: Added support for binary OSC packets sent as "FullPacket" messages (i.e., from the OpenSoundControl object)
VERSION 0.4.1: Added min and max OSC Packet sizes as a heuristic protection against crashing from non-OSC input.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  
 */
#define NAME "printit"
#define DESCRIPTION "Really print everything about what comes in the inlet"
#define AUTHORS "Matt Wright"
#define COPYRIGHT_YEARS "2000-06,2012"

 
 

#include "version.h"
#include "ext.h"
#include "ext_obex.h"


#include "../../../CNMAT-OSC/dumpOSC/printOSCpacket.h"

/* structure definition of your object */

typedef struct printit
{
	Object o_ob;				// required header
	Symbol *my_name;
  int min_OSCPacket_size, max_OSCPacket_size;
} printit;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
t_class *printit_class;

/* prototypes  */

void printit_bang(printit *x);
void printit_float(printit *x, double d);
void printit_int(printit *x, long n);
void printit_list(printit *x, Symbol *s, short argc, Atom *argv);
void printit_anything(printit *x, Symbol *s, short argc, Atom *argv);
void *printit_new(Symbol *s);
void printit_assist (printit *x, void *box, long msg, long arg, char *dstString);

Symbol *ps_emptysymbol;
Symbol *ps_printit;
Symbol *ps_FullPacket;


/* initialization routine */

int main(void)
{
  version_post_copyright();
	/* tell Max about your class. The cast to short is important for 68K */
	printit_class = class_new("printit", (method) printit_new, 0L, (short)sizeof(printit), 
		  0L, A_DEFSYM, 0);


	/* bind your methods to symbols */
	class_addmethod(printit_class, (method)printit_bang, "bang", 0);
	class_addmethod(printit_class, (method)printit_int, "int", A_LONG, 0);
	class_addmethod(printit_class, (method)printit_float, "float", A_FLOAT, 0);
	class_addmethod(printit_class, (method)printit_list, "list", A_GIMME, 0);	
	class_addmethod(printit_class, (method)printit_anything, "anything", A_GIMME, 0);
	class_addmethod(printit_class, (method)printit_assist, "assist", A_CANT, 0);
	class_addmethod(printit_class, (method)version, "version", 0);

	ps_emptysymbol = gensym("");
	ps_printit = gensym("printit");
	ps_FullPacket = gensym("FullPacket");

	class_register(CLASS_BOX, printit_class);
	return 0;
}



/* instance creation routine */

void *printit_new(Symbol *s)
{
	printit *x;	
	
	x = object_alloc(printit_class);		// get memory for a new object & initialize
	if(!x){
		return NULL;
	}
	if (s == ps_emptysymbol) {
		x->my_name = ps_printit;
	} else {
		x->my_name = s;
	}

	// A bundle containing a single message with a very short
	// address and a single argument is 32 bytes:
	x->min_OSCPacket_size = 32;

	x->max_OSCPacket_size = 2048;

	return (x);
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
		object_post((t_object *)x, "¥ printit_assist: unrecognized message %ld", msg);
	}
}

void printit_bang(printit *x)
{
	object_post((t_object *)x, "%s: received a bang.", x->my_name->s_name);
}

void printit_float(printit *x, double d)
{
	float f = (float) d;
	object_post((t_object *)x, "%s: received a float: %f (which was passed on the stack as a double: %f)", x->my_name->s_name, f, d);
}

void printit_int(printit *x, long n)
{
	object_post((t_object *)x, "%s: received an int: %ld", x->my_name->s_name, n);
}

static void print_args(printit *x, short argc, Atom *argv) {
  int i;

  for (i = 0; i < argc; ++i) {
    if (argv[i].a_type == A_SYM) {
      post(" SYMBOL \"%s\" (%p, s_thing %p)",
	   argv[i].a_w.w_sym->s_name, argv[i].a_w.w_sym, argv[i].a_w.w_sym->s_thing);
    } else if (argv[i].a_type == A_LONG) {
      object_post((t_object *)x, " LONG   %ld", argv[i].a_w.w_long);
    } else if (argv[i].a_type == A_FLOAT) {
      object_post((t_object *)x, " FLOAT  %f", argv[i].a_w.w_float);
    } else if (argv[i].a_type == A_SEMI) {
      object_post((t_object *)x, " A_SEMI (semicolon: \";\") - its own special undocumented Max data type");
    } else if (argv[i].a_type == A_COMMA) {
      object_post((t_object *)x, " A_COMMA (comma: \",\") - its own special undocumented Max data type");
    } else if (argv[i].a_type == A_DOLLAR) {
      object_post((t_object *)x, " A_DOLLAR (dollar sign: \"$\") - its own special undocumented Max data type");
    } else if (argv[i].a_type == A_NOTHING) {
      object_post((t_object *)x, " A_NOTHING (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_OBJ) {
      object_post((t_object *)x, " A_OBJ (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_DEFLONG) {
      object_post((t_object *)x, " A_DEFLONG (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_DEFFLOAT) {
      object_post((t_object *)x, " A_DEFFLOAT (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_DEFSYM) {
      object_post((t_object *)x, " A_DEFSYM (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_GIMME) {
      object_post((t_object *)x, " A_GIMME (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_CANT) {
      object_post((t_object *)x, " A_CANT (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_DOLLSYM) {
      object_post((t_object *)x, " A_DOLLSYM (whatever that means---let Matt Wright know if you find out!)");
    } else if (argv[i].a_type == A_GIMMEBACK) {
      object_post((t_object *)x, " A_GIMMEBACK (whatever that means---let Matt Wright know if you find out!)");
    } else {
      object_error((t_object *)x, "unrecognized argument type %d!", argv[i].a_type);
    }
  }
}

void printit_anything(printit *x, Symbol *s, short argc, Atom *argv) {
	post("%s: received MESSAGE \"%s\" (%p, s_thing %p) with %d argument(s):", 
	     x->my_name->s_name, s->s_name, s, s->s_thing, argc);

	print_args(x, argc, argv);
	
	if (s == ps_FullPacket && argc == 2 && argv[0].a_type == A_LONG && argv[1].a_type == A_LONG) {
	  int size = argv[0].a_w.w_long;
	  char *bufptr = (char *) argv[1].a_w.w_long;
	  object_post((t_object *)x, "It looks like an OSC packet");
	  if (size < x->min_OSCPacket_size) {
	    post("but the size is only %ld bytes, so I won't try to print it.",
		 size);
	  } else if (size > x->max_OSCPacket_size) {
	    post("but the size (%ld) looks too big, so I won't try to print it",
		 size);
	  } else {
		PrintOSCPacket(bufptr, size);
	  }
	}
}

void printit_list(printit *x, Symbol *s, short argc, Atom *argv) {
	object_post((t_object *)x, "%s: received LIST with %d argument(s):", x->my_name->s_name, argc);
	print_args(x, argc, argv);
}
