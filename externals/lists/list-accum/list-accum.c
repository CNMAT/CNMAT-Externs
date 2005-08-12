/* listAccum.c

   Modified to work w/ PPC Max on 9/25/96 by Matt Wright.
   Also made it accept "symbol" lists, not just lists of numbers.
   
 * Copyright (c) 1994 Regents of the University of California.
 * All rights reserved.
 * The name of the University may not be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE
 * IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.

 Modified 8/12/05 by Matt Wright to compile again 
 
*/

#include "ext.h"

struct listAccum *listAccumNew(long);
void *listAccumFree(struct listAccum *);
void *listAccumBang(struct listAccum *);
void *listAccumDebug(struct listAccum *);
void *listAccumList(struct listAccum *, struct symbol *, int, struct atom *);
void listAccum_anything (struct listAccum *x, Symbol *s, short argc, Atom *argv);


fptr *FNS;
void *class;

struct listAccum {
	struct object l_ob;
	void   *l_outlet;
	
	long	l_numAtoms;
	long	l_atomsInBuf;
	Atom	*l_atoms;
	
	Boolean	debug;
};


/*  -----------------  Constructor/Destructor ----------------- */

struct listAccum *listAccumNew(long n) {
	struct listAccum *x;
	int i;
	
	if (n <= 0) {
		post("listAccum: Argument must be positive.");
		return 0;
	}
	
	x = (struct listAccum *)newobject(class);
	x->l_outlet = outlet_new(x,0L);
	x->l_numAtoms = n;
	x->l_atoms	= (Atom *) getbytes((short)(sizeof(Atom)*x->l_numAtoms));
	x->l_atomsInBuf = 0;
	
	x->debug = false;
	
	return (x);
}

void *listAccumFree(struct listAccum *x)
{
	freebytes(x->l_atoms,(short)(sizeof(Atom)*x->l_numAtoms));
}

/* ------------------------- listAccumBang ------------------------ */


void *listAccumBang(struct listAccum *x)
{
	if (x->debug)
		post("listAccum bang");
	
	if (x->l_atomsInBuf)
		outlet_list(x->l_outlet,0L,(short)x->l_atomsInBuf,x->l_atoms);
	x->l_atomsInBuf = 0;
}

/* ------------------------- listAccumDebug ------------------------ */

void *listAccumDebug(struct listAccum *x)
{
	x->debug = !x->debug;
	
	if (x->debug)
		post("listAccum debug on");
	else
		post("listAccum debug off");
	
}

/* ------------------------- listAccumList ------------------------ */

void	*listAccumList(struct listAccum *x, struct symbol *s, int argc, struct atom *argv)
{
	int i;
	for (i=0;i<argc;i++) {
		x->l_atoms[x->l_atomsInBuf] = argv[i];
		x->l_atomsInBuf++;
		if (x->debug)
			post("listAccum: %ld atoms accumulated",x->l_atomsInBuf);
		if (x->l_atomsInBuf>=x->l_numAtoms) {
			listAccumBang(x);
		}
	}
}

/* -----------------------  listAccum_anything ----------------------- */

void listAccum_anything (struct listAccum *x, Symbol *s, short argc, Atom *argv) {
	int i;

	/* First, deal with symbol that's the message name */
		
	x->l_atoms[x->l_atomsInBuf].a_type = A_SYM;
	x->l_atoms[x->l_atomsInBuf].a_w.w_sym = s;
	x->l_atomsInBuf++;
	
	if (x->debug)
		post("listAccum: %ld atoms accumulated",x->l_atomsInBuf);
	
	if (x->l_atomsInBuf >= x->l_numAtoms) {
		listAccumBang(x);
	}
	
	/* Now, deal with the list of arguments */
	for (i=0; i<argc; i++) {
		x->l_atoms[x->l_atomsInBuf] = argv[i];
		x->l_atomsInBuf++;
		if (x->debug)
			post("listAccum: %ld atoms accumulated",x->l_atomsInBuf);
		if (x->l_atomsInBuf>=x->l_numAtoms) {
			listAccumBang(x);
		}
	}
	
}


main(fptr *f) {		
	setup((t_messlist **)&class, (method) listAccumNew, (method)listAccumFree,
		  (short) sizeof(struct listAccum), 0L, A_LONG, 0);

	addbang((method)listAccumBang);
	addmess((method)listAccumDebug, "debug", 0);
	addmess((method)listAccumList, "list",A_GIMME, 0);
 	addmess((method)listAccum_anything,	"anything",	A_GIMME, 0);

	finder_addclass("Data","listAccum");
	
	post("listAccum: A max list accumulator.  By Michael Lee and Matt Wright.");
	post("Copyright © 1996-2005 Regents of the University of California.  All rights reseved.");
}