/*
Copyright (c) 1999.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/


/* 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: SDIF-matrixlist
DESCRIPTION: list all the matrix types found in the given stream.  This never worked; use the "framelist" message to SDIF-buffer.
AUTHORS: Matt Wright 
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.1:  First version
VERSION 0.1.1: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

/*
 SDIF-matrixlist.c -- the SDIF-matrixlist object
 by Matt Wright, 7/8/99 
 A Max SDIF selector object - 
 -- */

#include "./version.h" // make sure not to get ../SDIF-buffer/version.h

#include "ext.h"

/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#include "version.c"

#include "SDIF-buffer.h"

/* #include <assert.h> */

#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}

/* My class definiton */

typedef struct _SDIFmatrixlist {
	struct object t_ob;
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
 	void *t_out;
 	
} SDIFmatrixlist;


/* global necessary for 68K function macros to work */
fptr *FNS;

Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup;

/* global that holds the class definition */
void *SDIFmatrixlist_class;

/* prototypes for my functions */
void *SDIFmatrixlist_new(Symbol *s, short argc, Atom *argv);
static void LookupMyBuffer(SDIFmatrixlist *x);
void SDIFmatrixlist_listmatrices(SDIFmatrixlist *x);
void SDIFmatrixlist_printmatrices(SDIFmatrixlist *x);

void main(fptr *fp)
{
	long oldA4;
	
	// this code is a 68K-only macro
	oldA4 = SetCurrentA4();			// get correct value of A4
	RememberA4();					// store inside code resource
	// this is not necessary (but harmless) on PowerPC
	FNS = fp;	
	
	post("SDIF-matrixlist version " VERSION " by Matt Wright");
	post("Copyright © 1999,2000-06 Regents of the University of California.");
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup(&SDIFmatrixlist_class, SDIFmatrixlist_new, 0,
			(short)sizeof(SDIFmatrixlist), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
	addmess((method)SDIFmatrixlist_listmatrices, "listmatrices", 0);
	addmess((method)SDIFmatrixlist_printmatrices, "printmatrices", 0);


	/* list object in the new object list */
	finder_addclass("Data","SDIF-matrixlist");
	
	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");


	{
		char buf[10];
		Symbol *f, *b;
		sprintf(buf, "foo");
		f = gensym(buf);
		sprintf(buf, "bar");
		b = gensym(buf);
		post("I hope this is foo: %s", f->s_name);
	}

	// restore old value of A4 (68K only)
	RestoreA4(oldA4);
}

void *SDIFmatrixlist_new(Symbol *s, short argc, Atom *argv) {
	SDIFmatrixlist *x;
	int i;
	
	EnterCallback();
	post("SDIFmatrixlist_new: %s, %ld args", s->s_name, (long) argc);
	
	x = newobject(SDIFmatrixlist_class);
	x->t_buffer = 0;
	x->t_out = listout(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			post("¥ SDIF-matrixlist: argument must be name of an SDIF-buffer");
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}

	ExitCallback();
	return (x);
}

static void LookupMyBuffer(SDIFmatrixlist *x) {
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	x->t_buffer = (*f)(x->t_bufferSym);
}

#define MOST_MATRIX_TYPES 100

void SDIFmatrixlist_listmatrices(SDIFmatrixlist *x) {
	char types[MOST_MATRIX_TYPES][4];
	int i, num;
	Atom result[MOST_MATRIX_TYPES];
	
	num = FindAllMatrixTypes(x, types);
	
	for (i = 0; i < num; ++i) {
		char buf[5];
		result[i].a_type = A_SYM;
		/* xxx */
	}
}
	
void SDIFmatrixlist_printmatrices(SDIFmatrixlist *x) {
	char types[MOST_MATRIX_TYPES][4];
	int i, num;
	
	EnterCallback();
	num = FindAllMatrixTypes(x, types);
	post("Matrix types in SDIF-buffer %s:", x->t_bufferSym->s_name);
	for (i = 0; i < num; ++i) {
		post("  %c%c%c%c", types[i][0], types[i][1], types[i][2], types[i][3])
	}
	
	ExitCallback();
} 

int FindAllMatrixTypes(SDIFmatrixlist *x, char *(types[4])) {
	LookupMyBuffer(x);
	
	xxx Need lookup next frame after -infinity, not before.
