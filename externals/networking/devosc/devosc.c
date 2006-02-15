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
VERSION 0.1: Seems to work.  Added "open" and "close" messages, made packet size variable.
VERSION 0.2: Added "errorreporing" message.
SVN_REVISION: $LastChangedRevision$
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/


#include "version.h"
#include "ext.h"
#include <fcntl.h>


// #include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <errno.h>


void *devosc_class;
Symbol *ps_FullPacket, *ps_OSCBlob;


#define DEVICE_FILENAME "/dev/osc"

#define DEFAULT_RIMASBOX_OSC_PACKET_SIZE 188  /* Size (in bytes) of packet Rimas currently sends. */
#define MAX_RIMASBOX_OSC_PACKET_SIZE 4096


typedef struct devosc_struct {
	struct object O_ob;
	void *O_outlet;	
	short	O_debug;
	int errorreporting;	  // Does this object report errors in the Max window?
	int fd;
	int packetsize;
	char osc_packet[MAX_RIMASBOX_OSC_PACKET_SIZE];

} devosc;


void *devosc_new(long arg);
void devosc_open(devosc *x);
void devosc_close(devosc *x);
void devosc_free(devosc *x);
void devosc_assist(devosc *x, void *b, long m, long a, char *s);
void devosc_version (devosc *x);
void devosc_debug (devosc *x);
void devosc_errorreporting(devosc *x, int yesno);
void devosc_packetsize(devosc *x, long size);
void devosc_bang (devosc *x);
void read_rimasbox_packet(devosc *x);
void read_one_byte(devosc *x);

void devosc_sendData(devosc *x, short size, char *data);



// This is from David Z:
// example which gets a pointer to the Mach-O function "open" from the Carbon framework.
typedef int ((*pp_open) (const char *pathname, int flags));  // function definition typedef
pp_open pointer_to_open; // declare function pointer variable to hold function pointer from bundle
typedef size_t ((*pp_read) (int fd, void *buf, size_t count));  // function definition typedef
pp_read pointer_to_read; // declare function pointer variable to hold function pointer from bundle
typedef int ((*pp_close) (int fd));  // function definition typedef
pp_close pointer_to_close; // declare function pointer variable to hold function pointer from bundle
typedef int ((*pp_fcntl) (int fd, int cmd, int arg));  // function definition typedef
pp_fcntl pointer_to_fcntl; // declare function pointer variable to hold function pointer from bundle
typedef int ((*pp_stat) (const char *path, struct stat *sb));  // function definition typedef
pp_stat pointer_to_stat; // declare function pointer variable to hold function pointer from bundle




void main (void) {
	// Also from David Z.:
	CFBundleRef bundle	= CFBundleGetBundleWithIdentifier(CFSTR("com.apple.Carbon")); // open bundle
	pointer_to_open = (pp_open)CFBundleGetFunctionPointerForName(bundle, CFSTR("open"));
	pointer_to_read = (pp_read)CFBundleGetFunctionPointerForName(bundle, CFSTR("read"));
	pointer_to_close = (pp_close)CFBundleGetFunctionPointerForName(bundle, CFSTR("close"));
	pointer_to_fcntl = (pp_fcntl)CFBundleGetFunctionPointerForName(bundle, CFSTR("fcntl"));
	pointer_to_stat = (pp_stat)CFBundleGetFunctionPointerForName(bundle, CFSTR("stat"));
	
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
		
	setup((t_messlist **)&devosc_class, (method) devosc_new, (method) devosc_free,
		  (short)sizeof(devosc),0L,A_DEFLONG,0);

	addmess((method)devosc_assist, "assist",	A_CANT,0);
	addmess((method)devosc_version, "version", 	0);
	addmess((method)devosc_open, "open", 	0);
	addmess((method)devosc_close, "close", 	0);
	addmess((method)devosc_packetsize, "packetsize", 	A_LONG, 0);
	addmess((method)devosc_errorreporting, "errorreporting", 	A_LONG, 0);	

	addbang((method)devosc_bang);
	ps_FullPacket = gensym("FullPacket");
}

	
void *devosc_new(long arg) {
	devosc *x;

	post("arg is %ld", arg);
	
	x = (devosc *) newobject(devosc_class);

	x->fd = -1;
	
	/* Create the outlets in right to left order */
	x->O_outlet = outlet_new(x, 0L);

	x->O_debug = false;

	x->packetsize = DEFAULT_RIMASBOX_OSC_PACKET_SIZE;
	
	if (arg != 0) {
		devosc_packetsize(x, arg);
	}

	devosc_open(x);
	
	return (x);
}

void devosc_free(devosc *x) {
	devosc_close(x);
}


void devosc_open(devosc *x) {
	int fd;

	devosc_close(x);
	
	fd = (*pointer_to_open)(DEVICE_FILENAME, O_RDONLY);
	// Should be:	fd = (*pointer_to_open)(DEVICE_FILENAME, O_RDONLY | O_NONBLOCK);
	if (fd <= -1) {
		char *msg = strerror(errno);
		if (errno == 0) {
			// For some reason errno is zero when the file doesn't exist
			struct stat sb;
			if ((*pointer_to_stat)(DEVICE_FILENAME, &sb) == 0) {
				error("devosc: open() failed with errno==0, but stat() seems happy. ???");
			} else {
				char *msg = strerror(errno);
				error("devosc: open() failed with errno==0, but stat() says \"%s\".", msg);
			}
		} else {
			error("devosc: Couldn't open() device: %s", msg);
		}
		return;
	}
	x->fd = fd;

#if 0
	// Make I/O nonblocking the hard way
	if ((*pointer_to_fcntl)(fd, F_SETFL, O_NONBLOCK) == -1) {
		char *msg = strerror(errno);
		error("devosc: Couldn't fcntl() device to set it to nonblocking: %s", msg);
		return;
	}
#endif
}

void devosc_close(devosc *x) {
	if (x->fd == -1) return;
	
	if ( (*pointer_to_close)(x->fd) != 0) {
		char *msg = strerror(errno);
		error("devosc: Couldn't close() device: %s", msg);
	} else {
		x->fd = -1;
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


void devosc_packetsize(devosc *x, long size) {
	if (size <= 0) {
		error("devosc: unreasonable packet size %ld; not changing", size);
	} else if (size > MAX_RIMASBOX_OSC_PACKET_SIZE) {
		error("devosc: requested packet size %ld is bigger than MAX_RIMASBOX_OSC_PACKET_SIZE");
	} else {
		x->packetsize = size;
	}
}

void devosc_bang (devosc *x) {
	read_rimasbox_packet(x);
}



void read_rimasbox_packet(devosc *x) {
	int bytes_read;
	
	if (x->fd == -1) {
		if (x->errorreporting) error("Can't read; device not open.");
		return;
	}

	bytes_read = (*pointer_to_read)(x->fd, x->osc_packet, x->packetsize);
	
	if (bytes_read == x->packetsize) {
		// success
		devosc_sendData(x, x->packetsize, x->osc_packet);
	} else if (bytes_read == 0) {
		if (x->errorreporting) 
			error("devosc: read() saw end-of-file.  OSC never ends!  What's going on?");
	} else {
		// An error
		if (errno == EAGAIN) {
			// This "error" just means no new data ready, so do nothing.
		} else {
			char *msg = strerror(errno);
			if (x->errorreporting) error("devosc: read() failed: %s", msg);
		}
	}
}


void read_one_byte(devosc *x) {
	char buf[1];

	if (x->fd == -1) {
		if (x->errorreporting) error("Can't read; device not open.");
		return;
	}

	if ((*pointer_to_read)(x->fd, buf, 1) != 1) {
		char *msg = strerror(errno);
		if (x->errorreporting) error("devosc: read() didn't return 1: %s", msg);
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
