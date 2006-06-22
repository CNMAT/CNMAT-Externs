/*
Copyright (c) 2000.  The Regents of the University of California (Regents).
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

 bytes2ints.c

DESCRIPTION:  Convert binary buffers (e.g., from received UDP packets) to max lists where each byte of the buffer is a separate Max int.
  
 */
 

#define BYTES2INTS_VERSION "0.0"
#define DEFAULT_MAX_LIST_LENGTH 256


/* the required include files */
#include "ext.h"
#include <SetUpA4.h>
#include <A4Stuff.h>
// #include <ctype.h>

/* structure definition of your object */
typedef struct bytes2ints
{
	Object b_ob;				// required header
	void *b_outlet;
	Atom *b_list;
	long b_listlen;			// capacity of b_list
} bytes2ints;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *bytes2ints_class;

/* prototypes  */

void *bytes2ints_new(long listlen);
void bytes2ints_free(bytes2ints *x);
void bytes2ints_version (bytes2ints *x);
void bytes2ints_assist (bytes2ints *x, void *box, long msg, long arg, char *dstString);
void bytes2ints_convert(bytes2ints *x, long nbytes, long pointerAsLong);
void do_convert(bytes2ints *x, long nbytes, char *buf);

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
	setup(&bytes2ints_class, bytes2ints_new, (method) bytes2ints_free, 
		  (short)sizeof(bytes2ints), 0L, A_DEFLONG, 0);

	/* bind your methods to symbols */
	addmess((method)bytes2ints_assist, "assist", A_CANT, 0);
	addmess((method)bytes2ints_version, "version", 0);
	
	addmess((method)bytes2ints_convert, "FullPacket", A_LONG, A_LONG, 0);
	addmess((method)bytes2ints_convert, "OTTCP_delim", A_LONG, A_LONG, 0);
	addmess((method)bytes2ints_convert, "OTTCP_nbytes", A_LONG, A_LONG, 0);
		
	post("bytes2ints object version " BYTES2INTS_VERSION " by Matt Wright. ");
	post("Copyright © 2000 Regents of the University of California. All Rights Reserved.");

	// restore old value of A4 (68K only)
	RestoreA4(oldA4);
}



/* instance creation routine */

void *bytes2ints_new(long listlen) {
	bytes2ints *x;
	
	EnterCallback();
	
	post("bytes2ints_new(%ld)", listlen);
	
	if (listlen <= 0) {
		listlen = DEFAULT_MAX_LIST_LENGTH;
	}
	
	x = newobject(bytes2ints_class);		// get memory for a new object & initialize
	x->b_outlet = listout(x);
	x->b_list = (Atom *) getbytes((short) (listlen * sizeof(Atom)));
	x->b_listlen = listlen;
	
	ExitCallback();
	return (x);
}


void bytes2ints_free(bytes2ints *x) {
	freebytes(x->b_list, (short) (x->b_listlen * sizeof(Atom)));
}

void bytes2ints_version(bytes2ints *x) {
	EnterCallback();
	post("bytes2ints Version " BYTES2INTS_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
	ExitCallback();
}

void bytes2ints_assist (bytes2ints *x, void *box, long msg, long arg, char *dstString) {
	EnterCallback();
	assist_string(3019, msg, arg, 1, 2, dstString);
	ExitCallback();
}

void bytes2ints_convert(bytes2ints *x, long nbytes, long pointerAsLong) {
	EnterCallback();

	if (nbytes == 0) {
		outlet_bang(x->b_outlet);
	} else if (nbytes < 0) {
		post("¥ bytes2ints: Bad buffer size: %ld", nbytes);
	} else {
		if (nbytes > x->b_listlen) {
			post("¥ bytes2ints: Warning: only outputting first %ld of %ld bytes", 
				 x->b_listlen, nbytes);
			nbytes = x->b_listlen;
		}
		do_convert(x, nbytes, (void *) pointerAsLong);
	}

	ExitCallback();
}

void do_convert(bytes2ints *x, long nbytes, char *buf) {
	int i;
	long l;
	
	for (i = 0; i < nbytes; ++i) {
		SETLONG(x->b_list+i, (long) buf[i]);
	}

	outlet_list(x->b_outlet, 0L, nbytes, x->b_list);
}
	
