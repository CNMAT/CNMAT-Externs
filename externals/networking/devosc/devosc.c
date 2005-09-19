/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2005
The Regents of the University of California (Regents).  

Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: devosc
DESCRIPTION: Read devosc data from /dev/osc and output as a binary "fullpacket" message
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2005
VERSION 0.0: Initial version reads from /dev/random instead, just to learn how to make the right system calls.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                
        */

#include "version.h"
#include "ext.h"
// #include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>
// #include <FSp_fopen.h>


void *devosc_class;
Symbol *ps_FullPacket, *ps_OSCBlob;

typedef struct devosc_struct {
	struct object O_ob;
	void *O_outlet;	
	short	O_debug;
	int errorreporting;	  // Does this object report errors in the Max window?
	int fd;
} devosc;


void *devosc_new(long arg);
void devosc_free(devosc *x);
void devosc_assist(devosc *x, void *b, long m, long a, char *s);
void devosc_version (devosc *x);
void devosc_debug (devosc *x);
void devosc_errorreporting(devosc *x, int yesno);
void devosc_bang (devosc *x);

void devosc_sendData(devosc *x, short size, char *data);



// This is from David Z:
// example which gets a pointer to the Mach-O function "open" from the Carbon framework.
typedef int ((*pp_open) (const char *pathname, int flags));  // function definition typedef
pp_open pointer_to_open; // declare function pointer variable to hold function pointer from bundle
typedef size_t ((*pp_read) (int fd, void *buf, size_t count));  // function definition typedef
pp_read pointer_to_read; // declare function pointer variable to hold function pointer from bundle
typedef int ((*pp_close) (int fd));  // function definition typedef
pp_close pointer_to_close; // declare function pointer variable to hold function pointer from bundle




void main (void) {
	// Also from David Z.:
	CFBundleRef bundle	= CFBundleGetBundleWithIdentifier(CFSTR("com.apple.Carbon")); // open bundle
	pointer_to_open = (pp_open)CFBundleGetFunctionPointerForName(bundle, CFSTR("open"));
	pointer_to_read = (pp_read)CFBundleGetFunctionPointerForName(bundle, CFSTR("read"));
	pointer_to_close = (pp_close)CFBundleGetFunctionPointerForName(bundle, CFSTR("close"));
	
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
		
	setup((t_messlist **)&devosc_class, (method) devosc_new, (method) devosc_free,
		  (short)sizeof(devosc),0L,A_DEFLONG,0);

	addmess((method)devosc_assist, "assist",	A_CANT,0);
	addmess((method)devosc_version, "version", 	0);

	addbang((method)devosc_bang);
	ps_FullPacket = gensym("FullPacket");
}

	
void *devosc_new(long arg) {
	devosc *x;
	int fd;
	
	
	fd = (*pointer_to_open)("/dev/random", O_RDONLY);
	if (fd == -1) {
		char *msg = strerror(errno);
		error("devosc: Couldn't open() device: %s", msg);
		return 0;
	}

	x = (devosc *) newobject(devosc_class);

	x->fd = fd;
	/* Create the outlets in right to left order */
	x->O_outlet = outlet_new(x, 0L);

	x->O_debug = false;

	return (x);
}

void devosc_free(devosc *x) {
	int result = (*pointer_to_close)(x->fd);
	if (result != 0) {
		char *msg = strerror(errno);
		error("devosc: Couldn't close() device: %s", msg);
	}
}
void devosc_assist(devosc *x, void *b, long m, long a, char *s) {
}

void devosc_version (devosc *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}

void devosc_debug (devosc *x) {
	x->O_debug = !x->O_debug;
	
	if (x->O_debug)
		post("devosc: debug on");
	else
		post("devosc: debug off");
}

void devosc_errorreporting(devosc *x, int yesno) {
	x->errorreporting = yesno;
	
	if (yesno) { 
		post("devosc: turning on error reporting.");
	} else {
		post("devosc: turning off eror reporting.  What you don't know can't hurt you.");
	}
}



void devosc_bang (devosc *x) {
	char buf[1];
	if ((*pointer_to_read)(x->fd, buf, 1) != 1) {
		char *msg = strerror(errno);
		error("devosc: read() didn't return 1: %s", msg);
      } else {
        outlet_int(x->O_outlet, buf[0]);
      }
}

void devosc_sendData(devosc *x, short size, char *data) {
	Atom arguments[2];
	
	if (x->O_debug) {
		post("devosc: Sending buffer (%ld bytes)", (long) size);
	}
	
	SETLONG(&arguments[0], (long) size);
	SETLONG(&arguments[1], (long) data);
	outlet_anything(x->O_outlet, ps_FullPacket, 2, arguments);

}





/********************************************************************************/

#ifdef RIDICULOUS
#define MAX_FILENAME_LEN 256
#define MAX_FULLPATH_LEN 2000

FILE *ridiculous_fopen(char *filename);
FILE *ridiculous_fopen(char *filename) {
	/* This is stupidly copied from open-sdif-file.c; the code should be factored properly. */
	
	char filenamecopy[MAX_FILENAME_LEN];
	char fullpath[MAX_FULLPATH_LEN];
	short result, pathID;
	long filetype;
	PATH_SPEC ps;	
	OSErr err;
	FILE *f;

	
	strncpy(filenamecopy, filename, MAX_FILENAME_LEN);
	
	result = locatefile_extended(filenamecopy, &pathID, &filetype, 0, 0);

	
	if (result != 0) {
		error("couldn't locate \"%s\" in Max's search path (result %ld)", 
			 filename, result);
		return NULL;
	}
#ifdef WIN_VERSION
#error Windows sucks
#else
#define PATH_SPEC_MEANS_FSSPEC
#ifdef PATH_SPEC_MEANS_FSSPEC
	result = path_tospec(pathID, filenamecopy, &ps);
	if (result != 0) {
		error("couldn't make PATH_SPEC from file %s (path_tospec returned %ld)",
			 filenamecopy, result);
		return NULL;
	}
	
	f = FSp_fopen (&ps, "rb");

	if (f == NULL) {
		error("FSp_fopen returned NULL; can't open %s", filename);
		return NULL;
	} 
    return f;
#else 	
#error What do I do with a PATH_SPEC?	
#endif /* PATH_SPEC_MEANS_FSSPEC */
#endif /* WIN_VERSION */
	
}

#endif

