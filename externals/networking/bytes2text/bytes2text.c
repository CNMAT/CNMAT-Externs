/*
Copyright (c) 1999.  The Regents of the University of California (Regents).
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

 bytes2text.c

AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1999-2006
SVN_REVISION: $LastChangedRevision$
DESCRIPTION: Interpret a binary buffer (e.g., from a received UDP packet) as an ASCII string, add null termination, and print it in the Max window.
VERSION 0.0: initial version
  
 */
 

/* the required include files */
#include "ext.h"
#include <SetUpA4.h>
#include <A4Stuff.h>
// #include <ctype.h>

/* structure definition of your object */
typedef struct bytes2text
{
	Object b_ob;				// required header
	void *b_outlet;
} bytes2text;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *bytes2text_class;

/* prototypes  */

void *bytes2text_new(Symbol *s, short argc, Atom *argv);
void bytes2text_assist (bytes2text *x, void *box, long msg, long arg, char *dstString);
void bytes2text_convert(bytes2text *x, long nbytes, long pointerAsLong);
void do_convert(bytes2text *x, long nbytes, char *buf);

/* initialization routine */

void main(fptr *f)
{
	long oldA4;
	
	// this code is a 68K-only macro
	oldA4 = SetCurrentA4();			// get correct value of A4
	RememberA4();					// store inside code resource
	// this is not necessary (but harmless) on PowerPC
	FNS = f;	
	
	version(0);

	/* tell Max about your class. The cast to short is important for 68K */
	setup(&bytes2text_class, bytes2text_new, 0L, (short)sizeof(bytes2text), 0L, A_GIMME, 0);

	/* bind your methods to symbols */
	addmess((method)version, "version", 0);
	addmess((method)bytes2text_assist, "assist", A_CANT, 0);
	
	addmess((method)bytes2text_convert, "FullPacket", A_LONG, A_LONG, 0);
	addmess((method)bytes2text_convert, "OTTCP_delim", A_LONG, A_LONG, 0);
	addmess((method)bytes2text_convert, "OTTCP_nbytes", A_LONG, A_LONG, 0);
		
	// restore old value of A4 (68K only)
	RestoreA4(oldA4);
}



/* instance creation routine */

void *bytes2text_new(Symbol *s, short argc, Atom *argv)
{
	bytes2text *x;
	
	EnterCallback();
	
	x = newobject(bytes2text_class);		// get memory for a new object & initialize
	x->b_outlet = listout(x);
	
	ExitCallback();
	return (x);
}



void bytes2text_assist (bytes2text *x, void *box, long msg, long arg, char *dstString) {
	EnterCallback();
	assist_string(3009, msg, arg, 1, 2, dstString);
	ExitCallback();
}

#define LONGEST_REASONABLE_BUFFER 256

void bytes2text_convert(bytes2text *x, long nbytes, long pointerAsLong) {
	EnterCallback();

	post("bytes2text_convert: %ld %ld", nbytes, pointerAsLong);

	if (nbytes == 0) {
		outlet_bang(x->b_outlet);
	} else if (nbytes < 0) {
		post("¥ bytes2text: Bad buffer size: %ld", nbytes);
	} else if (nbytes > LONGEST_REASONABLE_BUFFER) {
		post("¥ bytes2text: Unreasonably long buffer size %ld (max is %ld)", 
			 nbytes, LONGEST_REASONABLE_BUFFER);
	} else {
		do_convert(x, nbytes, (void *) pointerAsLong);
	}

	ExitCallback();
}

void do_convert(bytes2text *x, long nbytes, char *buf) {
	int i;
	char nullTerminated[LONGEST_REASONABLE_BUFFER+1];
	
	for (i = 0; i < nbytes; ++i) {
		/* if (!isprint(buf[i])) {
			post("¥ bytes2text: buf[%ld] == %ld, which is unprintable");
			return;
		} */
		nullTerminated[i] = buf[i];
	}

	nullTerminated[nbytes] = '\0';
	
	post("your bytes: %s", nullTerminated);
}
	
