/*
Copyright (c) 1999, 2000, 2001, 2002.  The Regents of the University of California (Regents).
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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: tt
DESCRIPTION: type-sane trigger: How Matt believes Max's "trigger" object ought to work.  Has an "a"nything argument which outputs whatever type was received.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.0: Matt's initial version.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
  
 */
 

#define TT_VERSION "0.0"



/* the required include files */
#include "ext.h"

/* structure definition of your object */
#define MAX_NUM 256

typedef struct tt
{
	Object o_ob;				// required header
	int o_num;					// Number of arguments
	Atom o_arguments[MAX_NUM];
	void *o_outlets[MAX_NUM];
} tt;

Symbol *ps_a, *ps_l, *ps_s, *ps_i, *ps_f, *ps_b, *ps_list, *ps_emptysymbol;


/* globalthat holds the class definition */
void *tt_class;

/* prototypes  */

void tt_bang(tt *x);
void tt_float(tt *x, float f);
void tt_int(tt *x, long n);
void tt_doanything(tt *x, Symbol *s, short argc, Atom *argv);
void tt_anything(tt *x, Symbol *s, short argc, Atom *argv);
void tt_list(tt *x, Symbol *s, short argc, Atom *argv);

void *tt_new(Symbol *s, short argc, Atom *argv);
void tt_version (tt *x);
void tt_assist (tt *x, void *box, long msg, long arg, char *dstString);
static char *NextSlashOrNull(char *p);
static void StrCopyUntilSlash(char *target, const char *source);


/* initialization routine */

void main(fptr *f)
{
	/* tell Max about your class. The cast to short is important for 68K */
	setup((t_messlist **)&tt_class, (method)tt_new,0L, (short)sizeof(tt), 0L, A_GIMME, 0);
	/* bind your methods to symbols */
	
	addbang((method)tt_bang);
	addint((method)tt_int);
	addfloat((method)tt_float);
	addmess((method)tt_anything, "anything", A_GIMME, 0);
	addmess((method)tt_list, "list", A_GIMME, 0);
	addmess((method)tt_assist, "assist", A_CANT, 0);
	addmess((method)tt_version, "version", 0);

	ps_list = gensym("list");
	ps_a = gensym("a");
	ps_l = gensym("l");
	ps_s = gensym("s");
	ps_i = gensym("i");
	ps_f = gensym("f");
	ps_b = gensym("b");
	ps_emptysymbol = gensym("");
	
	post("tt object version " TT_VERSION " by Matt Wright. ");
	post("Copyright © 20002 Regents of the University of California. All Rights Reserved.");
}



/* instance creation routine */

void *tt_new(Symbol *s, short argc, Atom *argv)
{
	tt *x;
	int i, n;
	
	x = newobject(tt_class);		// get memory for a new object & initialize

	if (argc > MAX_NUM) {
		post("¥ tt: too many outlets requested. (max %ld)", MAX_NUM);
		return 0;
	}

	x->o_num = argc;
	for (i = 0; i < argc; ++i) {
		x->o_arguments[i] = argv[i];
	}
	
	
	/* Have to create the outlets in reverse order */
		
	for (i = x->o_num-1; i >= 0; --i) {
		x->o_outlets[i] = listout(x);
	}
		
	return (x);
}


void tt_version (tt *x) {
	post("OSC-route Version " TT_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
}

/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void tt_assist (tt *x, void *box, long msg, long arg, char *dstString) {	
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "Message to be fanned to multiple outlets");
	} else if (msg==ASSIST_OUTLET) {
		if (arg < 0 || arg >= x->o_num) {
			post("¥ tt_assist: No outlet corresponds to arg %ld!", arg);
		} else {
			if (x->o_arguments[arg].a_type == A_LONG) {
				sprintf(dstString, "argument %ld: %ld", arg+1, x->o_arguments[arg].a_w.w_long);
			} else if (x->o_arguments[arg].a_type == A_FLOAT)  {
				sprintf(dstString, "argument %ld: %f", arg+1, x->o_arguments[arg].a_w.w_float);
			} else if (x->o_arguments[arg].a_type == A_SYM) {
				sprintf(dstString, "argument %ld: %s", arg+1, x->o_arguments[arg].a_w.w_sym->s_name);
			} else {
			post("¥ tt_assist: unrecognized argument type!");
			}
		}
	} else {
		post("¥ tt_assist: unrecognized message %ld", msg);
	}
}


/* Responding to input */


void tt_bang(tt *x) {
	int i;
	
	for (i = x->o_num-1; i >= 0; --i) {
		if (x->o_arguments[i].a_type == A_LONG) {
			outlet_int(x->o_outlets[i], x->o_arguments[i].a_w.w_long);
		} else if (x->o_arguments[i].a_type == A_FLOAT) {
			outlet_float(x->o_outlets[i], x->o_arguments[i].a_w.w_float);
		} else if (x->o_arguments[i].a_type == A_SYM) {
			Symbol *argSym = x->o_arguments[i].a_w.w_sym;
			/* The symbols "a", "b", "i", "f", "l", and "s" are special. */
			if (argSym == ps_a || argSym == ps_b) {
				outlet_bang(x->o_outlets[i]);
			} else if (argSym == ps_i) {
				outlet_int(x->o_outlets[i], 0);
			} else if (argSym == ps_f) {
				outlet_float(x->o_outlets[i], 0.0f);
			} else if (argSym == ps_s) {
				/* output the empty symbol */
				outlet_anything(x->o_outlets[i], ps_emptysymbol, 0, 0);
			} else if (argSym == ps_l) {
				/* output a list containing the long 0 as the only element */
				Atom atoms[1];
				SETLONG(&(atoms[0]), 0);
				outlet_list(x->o_outlets[i], 0L, 1, atoms);
			} else {
				/* This argument is a non-special symbol, so output it as a constant */
				outlet_anything(x->o_outlets[i], argSym, 0, 0);
			}
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}

void tt_float(tt *x, float f) {
	int i;
	
	for (i = x->o_num-1; i >= 0; --i) {
		if (x->o_arguments[i].a_type == A_LONG) {
			outlet_int(x->o_outlets[i], x->o_arguments[i].a_w.w_long);
		} else if (x->o_arguments[i].a_type == A_FLOAT) {
			outlet_float(x->o_outlets[i], x->o_arguments[i].a_w.w_float);
		} else if (x->o_arguments[i].a_type == A_SYM) {
			Symbol *argSym = x->o_arguments[i].a_w.w_sym;
			/* The symbols "a", "b", "i", "f", "l", and "s" are special. */
			if (argSym == ps_a) {
				outlet_float(x->o_outlets[i], f);
			} else if (argSym == ps_b) {
				outlet_bang(x->o_outlets[i]);
			} else if (argSym == ps_i) {
				/* Convert this float to an int */
				outlet_int(x->o_outlets[i], (int) f);
			} else if (argSym == ps_f) {
				/* Output this float directly */
				outlet_float(x->o_outlets[i], f);
			} else if (argSym == ps_s) {
				/* output the empty symbol */
				outlet_anything(x->o_outlets[i], ps_emptysymbol, 0, 0);
			} else if (argSym == ps_l) {
				/* output a list containing the given float as the only element */
				Atom atoms[1];
				SETFLOAT(&(atoms[0]), f);
				outlet_list(x->o_outlets[i], 0L, 1, atoms);
			} else {
				/* This argument is a non-special symbol, so output it as a constant */
				outlet_anything(x->o_outlets[i], argSym, 0, 0);
			}
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}

void tt_int(tt *x, long n) {
	int i;
	
	for (i = x->o_num-1; i >= 0; --i) {
		if (x->o_arguments[i].a_type == A_LONG) {
			outlet_int(x->o_outlets[i], x->o_arguments[i].a_w.w_long);
		} else if (x->o_arguments[i].a_type == A_FLOAT) {
			outlet_float(x->o_outlets[i], x->o_arguments[i].a_w.w_float);
		} else if (x->o_arguments[i].a_type == A_SYM) {
			Symbol *argSym = x->o_arguments[i].a_w.w_sym;
			/* The symbols "a", "b", "i", "f", "l", and "s" are special. */
			if (argSym == ps_a) {
				outlet_int(x->o_outlets[i], n);
			} else if (argSym == ps_b) {
				outlet_bang(x->o_outlets[i]);
			} else if (argSym == ps_i) {
				/* Output this int directly */
				outlet_int(x->o_outlets[i], n);
			} else if (argSym == ps_f) {
				/* Convert this int to a float */
				outlet_float(x->o_outlets[i], (float) n);
			} else if (argSym == ps_s) {
				/* output the empty symbol */
				outlet_anything(x->o_outlets[i], ps_emptysymbol, 0, 0);
			} else if (argSym == ps_l) {
				/* output a list containing the given int as the only element */
				Atom atoms[1];
				SETLONG(&(atoms[0]), n);
				outlet_list(x->o_outlets[i], 0L, 1, atoms);
			} else {
				/* This argument is a non-special symbol, so output it as a constant */
				outlet_anything(x->o_outlets[i], argSym, 0, 0);
			}
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}


void tt_list(tt *x, Symbol *s, short argc, Atom *argv) {
	int i;

	if (argc == 0) {
		/* Treat an empty list like a bang: */
		tt_bang(x);
		return;
	} 
	
	for (i = x->o_num-1; i >= 0; --i) {
		if (x->o_arguments[i].a_type == A_LONG) {
			outlet_int(x->o_outlets[i], x->o_arguments[i].a_w.w_long);
		} else if (x->o_arguments[i].a_type == A_FLOAT) {
			outlet_float(x->o_outlets[i], x->o_arguments[i].a_w.w_float);
		} else if (x->o_arguments[i].a_type == A_SYM) {
			Symbol *argSym = x->o_arguments[i].a_w.w_sym;
			/* The symbols "a", "b", "i", "f", "l", and "s" are special. */
			if (argSym == ps_a || argSym == ps_s || argSym == ps_l) {
				outlet_list(x->o_outlets[i], 0L, argc, argv);
			} else if (argSym == ps_b) {
				outlet_bang(x->o_outlets[i]);
			} else if (argSym == ps_i) {
				/* Output first element of list as an int */
				if (argv[0].a_type == A_LONG) {
					outlet_int(x->o_outlets[i], argv[0].a_w.w_long);
				} else if (argv[0].a_type == A_FLOAT) {
					outlet_int(x->o_outlets[i], (int) argv[0].a_w.w_float);
				} else {
					outlet_int(x->o_outlets[i], 0);
				}
			} else if (argSym == ps_f) {
				/* Output first element of list as a float */
				if (argv[0].a_type == A_LONG) {
					outlet_float(x->o_outlets[i], (float) argv[0].a_w.w_long);
				} else if (argv[0].a_type == A_FLOAT) {
					outlet_float(x->o_outlets[i], argv[0].a_w.w_float);
				} else {
					outlet_float(x->o_outlets[i], 0.0);
				}
			} else {
				/* This argument is a non-special symbol, so output it as a constant */
				outlet_anything(x->o_outlets[i], argSym, 0, 0);
			}
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}

void tt_anything(tt *x, Symbol *s, short argc, Atom *argv) {
	int i;

	for (i = x->o_num-1; i >= 0; --i) {
		if (x->o_arguments[i].a_type == A_LONG) {
			outlet_int(x->o_outlets[i], x->o_arguments[i].a_w.w_long);
		} else if (x->o_arguments[i].a_type == A_FLOAT) {
			outlet_float(x->o_outlets[i], x->o_arguments[i].a_w.w_float);
		} else if (x->o_arguments[i].a_type == A_SYM) {
			Symbol *argSym = x->o_arguments[i].a_w.w_sym;
			/* The symbols "a", "b", "i", "f", "l", and "s" are special. */
			if (argSym == ps_a || argSym == ps_s || argSym == ps_l) {
				outlet_anything(x->o_outlets[i], s, argc, argv);
			} else if (argSym == ps_b) {
				outlet_bang(x->o_outlets[i]);
			} else if (argSym == ps_i) {
				/* This "list" begins with the symbol s, so just output 0 */
				outlet_int(x->o_outlets[i], 0);
			} else if (argSym == ps_f) {
				/* This "list" begins with the symbol s, so just output 0.0 */
				outlet_float(x->o_outlets[i], 0.0f);
			} else {
				/* This argument is a non-special symbol, so output it as a constant */
				outlet_anything(x->o_outlets[i], argSym, 0, 0);
			}
		} else {
			post(" ¥ unrecognized argument type!");
		}
	}
}

