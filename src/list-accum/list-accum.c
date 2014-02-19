/*
Copyright (c) 2000,01,02,03,04,05,06,07.  The Regents of the University of California (Regents).
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
NAME: list-accum
DESCRIPTION: "Accumulate" a list by adding elements gradually.  Much like "zl group" except it can output arbitrary-length lists.
AUTHORS: Michael Lee and Matt Wright
COPYRIGHT_YEARS: 1996,98,98,992000,01,02,03,04,05,06
DRUPAL_NODE: /patch/4065
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Mike Lee's original version
VERSION 0.2: Modified to work w/ PPC Max on 9/25/96 by Matt Wright.  Also made it accept "symbol" lists, not just lists of numbers.
VERSION 0.3: Modified 8/12/05 by Matt Wright to compile again 
VERSION 0.4: Accepts single floats or integers
VERSION 0.5: Uses sysmem_newptr instead of getbytes(); works with more than 4096
VERSION 0.5.1: Force Package Info Generation
VERSION 0.6: MacCallum added a second outlet to output the length of the list.  Also changed main to return an int and take 0 args.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define NAME "list-accum"
#define DESCRIPTION "Accumulate a list by adding elements gradually.  Much like zl group except it can output arbitrary-length lists."
#define AUTHORS "Michael Lee and Matt Wright"
#define COPYRIGHT_YEARS "1996,98,99,2000-06,12,13"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"

struct listAccum *listAccumNew(long);
void listAccumFree(struct listAccum *);
void listAccumBang(struct listAccum *);
void listAccumDebug(struct listAccum *);
void listAccumList(struct listAccum *, struct symbol *, int, struct atom *);
void listAccumInt(struct listAccum *, int i);
void listAccumFloat(struct listAccum *, double d);
void listAccum_anything (struct listAccum *x, t_symbol *s, short argc, t_atom *argv);


//fptr *FNS;
t_class *class;

struct listAccum {
	struct object l_ob;
	void   *l_outlet;
	void   *l_outlet2; //to output the length of the list
	
	long	l_numAtoms;
	long	l_atomsInBuf;
	t_atom	*l_atoms;
	
	int	debug;
};


/*  -----------------  Constructor/Destructor ----------------- */

struct listAccum *listAccumNew(long n) {
	struct listAccum *x;
	
	if (n <= 0) {
		post("listAccum: Argument must be positive.");
		return 0;
	}
	
	x = (struct listAccum *)object_alloc(class);
	if(!x){
		return NULL;
	}
	x->l_outlet2 = intout(x);
	x->l_outlet = outlet_new(x,0L);
	x->l_numAtoms = n;
	x->l_atoms	= (t_atom *) sysmem_newptr((long)(sizeof(t_atom)*x->l_numAtoms));
	x->l_atomsInBuf = 0;
	
	x->debug = false;
	
	return (x);
}

void listAccumFree(struct listAccum *x)
{
  //freebytes(x->l_atoms,(short)(sizeof(t_atom)*x->l_numAtoms));
  sysmem_freeptr(x->l_atoms);
}

/* ------------------------- listAccumBang ------------------------ */


void listAccumBang(struct listAccum *x)
{
	if (x->debug)
		post("listAccum bang");
	
	if (x->l_atomsInBuf){
		outlet_int(x->l_outlet2, x->l_atomsInBuf);
		outlet_list(x->l_outlet,0L,(short)x->l_atomsInBuf,x->l_atoms);
	}
	x->l_atomsInBuf = 0;
}

/* ------------------------- listAccumDebug ------------------------ */

void listAccumDebug(struct listAccum *x)
{
	x->debug = !x->debug;
	
	if (x->debug)
		post("listAccum debug on");
	else
		post("listAccum debug off");
	
}

/* ------------------------- listAccumList ------------------------ */

void	listAccumList(struct listAccum *x, struct symbol *s, int argc, struct atom *argv)
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


void listAccumInt(struct listAccum *x, int i) {
  atom_setlong(&(x->l_atoms[x->l_atomsInBuf]), i);
  x->l_atomsInBuf++;
  if (x->debug) post("listAccum: added int %ld, that makes %ld atoms so far", i, x->l_atomsInBuf);
  if (x->l_atomsInBuf>=x->l_numAtoms) listAccumBang(x);
}

void listAccumFloat(struct listAccum *x, double d) {
  float f = (float) d;
  atom_setfloat(&(x->l_atoms[x->l_atomsInBuf]), f);
  x->l_atomsInBuf++;
  if (x->debug) post("listAccum: added float %, that makes %ld atoms so far", f, x->l_atomsInBuf);
  if (x->l_atomsInBuf>=x->l_numAtoms) listAccumBang(x);
}

 

void listAccumFloat(struct listAccum *, double d);

/* -----------------------  listAccum_anything ----------------------- */

void listAccum_anything (struct listAccum *x, t_symbol *s, short argc, t_atom *argv) {
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


int main(void) 
{		
	class = class_new("list-accum", (method) listAccumNew, (method)listAccumFree,
		  (short) sizeof(struct listAccum), 0L, A_LONG, 0);

	class_addmethod(class, (method)listAccumBang, "bang", 0);
	class_addmethod(class, (method)listAccumInt, "int", A_LONG, 0);
	class_addmethod(class, (method)listAccumFloat, "float", A_FLOAT, 0);
	class_addmethod(class, (method)listAccumDebug, "debug", 0);
	class_addmethod(class, (method)listAccumList, "list",A_GIMME, 0);
 	class_addmethod(class, (method)listAccum_anything,	"anything",	A_GIMME, 0);
 	class_addmethod(class, (method)version,	"version", 0);
	
	version_post_copyright();
	class_register(CLASS_BOX, class);
	return 0;
}
