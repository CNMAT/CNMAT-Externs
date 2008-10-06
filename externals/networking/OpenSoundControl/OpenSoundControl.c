/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 1996,97,98,99,2000,01,02,03,04,05
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
NAME: OpenSoundControl
DESCRIPTION: Format Max data to <A href="http://www.cnmat.berkeley.edu/OpenSoundControl">OpenSoundControl</a> protocol and vice versa
AUTHORS: Matt Wright, Andy Schmeder
COPYRIGHT_YEARS: 1996,97,98,99,2000,01,02,03,04,05
PUBLICATION: ICMC 97 paper | http://www.cnmat.berkeley.edu/ICMC97/papers-html/OpenSoundControl.html
PUBLICATION: NIME 03 paper | http://www.cnmat.berkeley.edu/Research/NIME2003/NIME03_Wright.pdf
PUBLICATION: Community web portal | http://www.opensoundcontrol.org
DRUPAL_NODE: /patch/4059
SVN_REVISION: $LastChangedRevision$
VERSION 1.2: was still version 1.1 on 68K; 1.3 is the same for both.
VERSION 1.4: has 3 outlets
VERSION 1.5: uses FullPacket instead of gimme, so no subverting of argc/argv
VERSION 1.6: supports the evil "gimme" for (68K) backwards compatibility
VERSION 1.7: Supports SuperCollider-style type tags
VERSION 1.8: has errorreporting mode, compiles under CW7 and Max 4 SDK
VERSION 1.9:: Cleaned up and fixed copyright for open-sourcing
VERSION 1.9.1: rudimentary blob support
VERSION 1.9.2: Builds CFM and MachO from the same code
VERSION 1.9.3: Same as 1.9.2
VERSION 1.9.4: Fixed severe type tag bug and severe byte-order bug (for receiving) and built for Windows
VERSION 1.9.5: Rebuilt for CFM (had to change where it got ntohl())
VERSION 1.9.6: Implements assistance strings again
VERSION 1.9.7: Force Package Info Generation
VERSION 1.9.8: Fixed byte-order bug with time tags
VERSION 1.9.9: Another attempt to fix time tag byte-order bug
VERSION 1.9.10: Handle time tags in arguments (andy@cnmat)
VERSION 1.9.11: Implement usable blob support
VERSION 1.9.12: Fix crash for zero, negative and excessive packet lengths
VERSION 1.9.13: Remove legacy ouchstring and broken htm_error_string
VERSION 1.9.14: Fix more bad packet crashes, always do toplevel bang even on bad packet
VERSION 1.9.15: Handle bad packets correctly when nested in bundles
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		
	Note: all conversions to network byte order for outgoing packets happen in OSC-client.c,
	whereas conversions to host byte order for incoming packes happen in this file.
	
   */

#include "version.h"
#include "ext.h"
#include "version.c"
#include "OSC-client.h"

#ifdef WIN_VERSION
// To get ntohl() on Windows
// #include <asm/byteorder.h>
// #include </usr/include/asm/byteorder.h>
#include </usr/include/w32api/winsock2.h>
#endif

void *OSC_class;
Symbol *ps_gimme, *ps_OSCTimeTag, *ps_FullPacket, *ps_OSCBlob;

typedef struct openSoundControl {
	struct object O_ob;
	void *O_outlet1;	// stuff for UDP or bang when done parsing a packet
	void *O_outlet2;	// messages in a packet
	void *O_outlet3;	// time tag from a packet
	short	O_debug;
	short O_resetAllTheWayMode;
	OSCTimeTag O_timeTagToUse;  // Stored in host byte order
	OSCbuf b;
	int writeTypeStrings; // nonzero if this object writes messages with type strings
	int readTypeStrings;  // if zero, never interpret first argument as a type string
	int errorreporting;	  // Does this object report errors in the Max window?
} OSC;


void *OSC_new(long arg);
void OSC_assist(OSC *x, void *b, long m, long a, char *s);
void OSC_debug (OSC *x);
void OSC_errorreporting(OSC *x, int yesno);
void OSC_reset (OSC *x);
void OSC_resetAllTheWayMode (OSC *x);
void OSC_doReset (OSC *x);
void OSC_send (OSC *x);
void OSC_anything (OSC *x, Symbol *s, short argc, Atom *argv);
void OSC_openBundleCB (OSC *x);
void OSC_closeBundleCB (OSC *x);
void OSC_bang (OSC *x);
void OSC_readtypestrings(OSC *x, int yesno);
void OSC_writetypestrings(OSC *x, int yesno);
void OSC_printcontents (OSC *x);
void OSC_NewTimeTag(OSC *x, long seconds, long fraction);
void OSC_ParseFullPacket(OSC *x, long size, long bufptr);
void OSC_ParsePartialPacket(OSC *x, long size, long bufptr);
void OSC_ParseEvilGimme(OSC *x, Symbol *s, short not_really_argc, Atom *not_really_argv);

void OSC_accumulateMessage(OSC *x, char *messageName, short argc, Atom *argv);
int OSC_stringSubstitution(char *target, char *format, short *argcp, Atom **argvp);
void OSC_sendBuffer(OSC *x);
void OSC_sendData(OSC *x, short size, char *data);
void OSC_formatData(OSC *x, char *messageName, short argc, Atom *argv);

#ifdef __MWERKS__
#define DONT_HAVE_STRING_LIBRARY
/* KLUDGE!!! 
If we're on metrowerks we must be compiling for PPC which is big-endian, so since I don't know
where to link in the real ntohl() I'll define this evil macro: */
#define ntohl(x) (x)
#endif

#ifdef DONT_HAVE_STRING_LIBRARY
void strcpy(char *s1, char *s2);
#endif

void main (fptr *f) {	
        version(0);
		
	setup((t_messlist **)&OSC_class, (method) OSC_new,0L,(short)sizeof(OSC),0L,A_DEFLONG,0);

#ifdef SANITY_CHECK
	post("*** sizeof(int4byte) = %ld", (long) sizeof(int4byte));
	post("*** sizeof(long) = %ld", (long) sizeof(long));
#endif

	addmess((method)OSC_assist, "assist",	A_CANT,0);
	addmess((method)version, "version", 	0);
	addmess((method)OSC_debug, "debug", 	0);
	addmess((method)OSC_errorreporting, "errorreporting", 	A_LONG, 0);
	addmess((method)OSC_reset, "reset", 	0);
	addmess((method)OSC_resetAllTheWayMode, "resetallthewaymode", 	0);

	addmess((method)OSC_send, "send", 	0);

	addmess((method)OSC_anything, "anything",	A_GIMME,0);
	addmess((method)OSC_openBundleCB, "openbundle", 	0);
	addmess((method)OSC_closeBundleCB, "closebundle", 	0);
	addmess((method)OSC_openBundleCB, "[", 	0);
	addmess((method)OSC_closeBundleCB, "]", 	0);

	addbang((method)OSC_bang);
	addmess((method)OSC_printcontents, "printcontents", 0);
	addmess((method)OSC_NewTimeTag, "OSCTimeTag", A_LONG, A_LONG, 0);
	addmess((method)OSC_ParseFullPacket, "FullPacket", A_LONG, A_LONG, 0);
	addmess((method)OSC_ParsePartialPacket, "PartialPacket", A_LONG, A_LONG, 0);
	addmess((method)OSC_ParseEvilGimme, "gimme", A_GIMME, 0);
	addmess((method)OSC_readtypestrings, "readtypestrings", A_LONG, 0);
	addmess((method)OSC_writetypestrings, "writetypestrings", A_LONG, 0);
	

	finder_addclass("Devices","OpenSoundControl");
	//	rescopy('STR#',3009);
	ps_gimme = gensym("gimme");
	ps_OSCTimeTag = gensym("OSCTimeTag");
	ps_FullPacket = gensym("FullPacket");
	ps_OSCBlob = gensym("OSCBlob");
}

#define DEFAULT_BUFFER_SIZE 1024


void *OSC_new(long arg) {
	OSC *x;
	char *buf;
	
	x = (OSC *) newobject(OSC_class);

	/* Create the outlets in right to left order */
	x->O_outlet3 = outlet_new(x, "OSCTimeTag");
	x->O_outlet2 = outlet_new(x, 0L);
	x->O_outlet1 = outlet_new(x, 0L);

	if (arg == 0) {
		arg = DEFAULT_BUFFER_SIZE;
	}
	if (arg < 50) {
		post("OpenSoundControl: buffer size changed to 50.");
		arg = 50;
	}
	if (arg > 32000) {
		post("OpenSoundControl: buffer size changed to 32000.");
		arg = 32000;
	}
	buf = getbytes((short) arg);
	if (buf == 0) {
		post("OpenSoundControl: couldn't allocate %ld bytes for buffer.", arg);
		return 0;
	}

	x->O_debug = false;
	x->O_resetAllTheWayMode = false;
	x->O_timeTagToUse = OSCTT_Immediately();
	// Kludge to patch around lack of endianness-awareness in OSC-timetag.c:
	// OSC_NewTimeTag(x, 0, 1);
	x->writeTypeStrings = 1;
	x->readTypeStrings = 1;
	
	OSC_initBuffer(&(x->b), arg, buf);
#ifdef LAME
	post("*** Obj %p, buf %p, Buffer %p, size %ld", x, &(x->b), 
		 (x->b).buffer, (long) OSC_packetSize(&(x->b)));
#endif
	
	OSC_doReset(x);
	
	return (x);
}

void OSC_assist(OSC *x, void *b, long m, long a, char *dst) {
	if (m == ASSIST_INLET) {
		if (a == 0) {
			strcpy(dst, "Max msgs; OSC packets");
		} else {
			error("OSC_assist: unrecognized inlet number %ld", a);
		}
	} else if (m == ASSIST_OUTLET) {
		if (a == 0) {
			strcpy(dst, "OSC packets; bang after msgs in OSC packet");
		} else if (a == 1) {
			strcpy(dst, "OSC msgs decoded from OSC packet");
		} else if (a == 2) {
			strcpy(dst, "Time tag (list of two ints) from OSC packet");
		} else {
			error("OSC_assist: unrecognized outlet number %ld", a);
		}
	} else {
		error("Unexpected msg %ld in OSC_assist", m);
	}
}

void OSC_debug (OSC *x) {
	x->O_debug = !x->O_debug;
	
	if (x->O_debug)
		post("OpenSoundControl: debug on");
	else
		post("OpenSoundControl: debug off");
}

void OSC_errorreporting(OSC *x, int yesno) {
	x->errorreporting = yesno;
	
	if (yesno) {
		post("OpenSoundControl: turning on error reporting.");
	} else {
		post("OpenSoundControl: turning off eror reporting.  What you don't know can't hurt you.");
	}
}


void OSC_resetAllTheWayMode (OSC *x) {
	x->O_resetAllTheWayMode = ! x->O_resetAllTheWayMode;
	
	if (x->O_debug)
		post("OpenSoundControl: resetallthewaymode %s", 
			 x->O_resetAllTheWayMode ? "on" : "off");
}


void OSC_doReset (OSC *x) {
	OSC_resetBuffer(&(x->b));
	if (!x->O_resetAllTheWayMode) {
		OSC_openBundle(&(x->b), x->O_timeTagToUse);
	}
}
			

void OSC_reset (OSC *x) {
	if (x->O_debug) {
		post("OpenSoundControl: resetting buffer to empty state");
	}
	OSC_doReset(x);
}

void OSC_send (OSC *x) {
	if (x->O_debug) {
		post("OpenSoundControl: sending buffer (not resetting)");
	}
	OSC_sendBuffer(x);
}


void OSC_bang (OSC *x) {
	if (x->O_debug) {
		post("OpenSoundControl: BANG! Sending buffer and resetting");
	}
	OSC_sendBuffer(x);
	OSC_doReset(x);
}


void OSC_readtypestrings(OSC *x, int yesno) {
	x->readTypeStrings = yesno;
}


void OSC_writetypestrings(OSC *x, int yesno) {
	x->writeTypeStrings = yesno;
}


void OSC_openBundleCB (OSC *x) {
	if (OSC_openBundle(&(x->b), x->O_timeTagToUse)) {
		if (x->errorreporting) {
			post("OpenSoundControl: problem opening bundle:");
			post("   %s", OSC_errorMessage);
			post("Throwing away entire buffer.");
		}
		OSC_doReset(x);	
	} else {
		if (x->O_debug)
			post("OpenSoundControl: opened bundle.  (Bundle depth now %ld)",
				 (long) x->b.bundleDepth);
	}
}

void OSC_closeBundleCB (OSC *x) {
	if (OSC_closeBundle(&(x->b))) {
		if (x->errorreporting) {
			post("OpenSoundControl: problem closing bundle:");
			post("   %s", OSC_errorMessage);
			post("Throwing away entire buffer.");
		}
		OSC_doReset(x);	
	} else {
		if (x->O_debug)
			post("OpenSoundControl: closed bundle.  (Bundle depth now %ld)",
				 (long) x->b.bundleDepth);
	}
}


void OSC_sendBuffer(OSC *x) {
	OSCbuf *buf = &(x->b);
	if (OSC_isBufferEmpty(buf)) {
		if (x->O_debug) {
			post("OpenSoundControl: buffer empty, not sending anything");
		}
		return;
	}
	if (!OSC_isBufferDone(buf)) {
		// Maybe the problem is unclosed bundles
		OSC_closeAllBundles(buf);
		if (!OSC_isBufferDone(buf)) {
			if (x->errorreporting) {
				post("OpenSoundControl: buffer not ready to send");
			}
			return;
		}
	}
#ifdef LAME
	post("*** OSC_sendBuffer.  x is %p, buf is %p, size %ld, packet is %p, getPacket returned %p",
		 x, buf, (long) OSC_packetSize(buf), buf->buffer, OSC_getPacket(buf));
#endif
	OSC_sendData(x, (short) OSC_packetSize(buf), OSC_getPacket(buf));
}

void OSC_sendData(OSC *x, short size, char *data) {
	Atom arguments[2];
	
	if (x->O_debug) {
		post("OpenSoundControl: Sending buffer (%ld bytes)", (long) size);
	}
	
	SETLONG(&arguments[0], (long) size);
	SETLONG(&arguments[1], (long) data);
	outlet_anything(x->O_outlet1, ps_FullPacket, 2, arguments);

#ifdef OLD_EVIL_WAY
	/* The cast from char *data to (struct atom *) is what made
	   this object so dangerous. */
	outlet_anything(x->O_outlet1, ps_gimme, size, (struct atom *)data);
#endif
}

#define LONGEST_MESSAGE_NAME 1024

void OSC_anything (OSC *x, Symbol *s, short argc, Atom *argv) {	
	char mess[LONGEST_MESSAGE_NAME];
	
	if (!OSC_stringSubstitution(mess, s->s_name, &argc, &argv)) {
		if (x->O_debug) {
			int i;
			post("OpenSoundControl: Adding message %s with %ld args:", mess, (long) argc);
			for (i = 0; i < argc; i++) {
				postatom(argv+i);
			}
		}

		OSC_accumulateMessage(x,mess,argc,argv);
	}

	if (x->O_debug) {
		post("OpenSoundControl: Now buffer has %ld bytes in it.",
			 (long) OSC_packetSize(&(x->b)));
	}
}

int OSC_stringSubstitution(char *target, char *format, short *argcp, Atom **argvp) {
	char *in, *p;
	int i, num;
	
	for (in = format, i=0; *in != '\0'; in++) {
		if (i >= LONGEST_MESSAGE_NAME) {
			post("OpenSoundControl: message address longer than LONGEST_MESSAGE_NAME (%ld): dropping.",
				 (long) LONGEST_MESSAGE_NAME);
			return 1;
		}
		if (*in != '%') {
			target[i++] = *in;
		} else {
			switch (in[1]) {
				case '\0':
					post("OpenSoundControl: message string ends with a single %%: dropping.");
					return 1;
					
				case '%':
					target[i++] = '%';
					in++;
					break;
					
				case 's':
					if (*argcp == 0) {
						post("OpenSoundControl: message string with %%s doesn't have enough arguments: dropping.");
						return 1;
					}
					if ((*argvp)->a_type != A_SYM) {
						post("OpenSoundControl: Dropping message. Argument corresponding to %%s isn't a string:");
						postatom(*argvp);
						return 1;
					}
					p = (*argvp)->a_w.w_sym->s_name;
					while (*p != 0) {
						target[i++] = *p++;
						if (i >= LONGEST_MESSAGE_NAME) {
							post("OpenSoundControl: with %%s substitution, message name string too long: dropping.");
							return 1;
						}
					}
					in++;
					(*argcp)--;
					(*argvp)++;
					break;
					
				case 'f':
					if (*argcp == 0) {
						post("OpenSoundControl: message string with %%f doesn't have enough arguments: dropping.");
						return 1;
					}
					if ((*argvp)->a_type != A_FLOAT) {
						post("OpenSoundControl: Dropping message: Argument corresponding to %%f isn't a float:");
						postatom(*argvp);
						return 1;
					}
					num = sprintf(target+i, "%f", (*argvp)->a_w.w_float);
					if (num < 0) {
						post("OpenSoundControl: error formatting float for %%f arg: dropping.");
						return 1;
					}
					i += num;
					/* If i is too big we'll get an error the next time the loop comes around. */

					in++;
					(*argcp)--;
					(*argvp)++;
					break;


				case 'd':
					if (*argcp == 0) {
						post("OpenSoundControl: message string with %%d doesn't have enough arguments: dropping.");
						return 1;
					}
					if ((*argvp)->a_type != A_LONG) {
						post("OpenSoundControl: Dropping message: Argument corresponding to %%d isn't an int:");
						postatom(*argvp);
						return 1;
					}
					num = sprintf(target+i, "%ld", (*argvp)->a_w.w_long);
					if (num < 0) {
						post("OpenSoundControl: error formatting int for %%d arg: dropping.");
						return 1;
					}
					in++;
					i += num;
					/* If i is too big we'll get an error the next time the loop comes around. */

					(*argcp)--;
					(*argvp)++;
					break;

				case 'c':
					if (*argcp == 0) {
						post("OpenSoundControl: message string with %%c doesn't have enough arguments: dropping.");
						return 1;
					}
					if ((*argvp)->a_type != A_LONG) {
						post("OpenSoundControl: Dropping message.  Argument corresponding to %%c isn't an int:");
						postatom(*argvp);
						return 1;
					}
					num = sprintf(target+i, "%c", (*argvp)->a_w.w_long);
					if (num < 0) {
						post("OpenSoundControl: error formatting char for %%c arg: dropping.");
						return 1;
					}
					in++;
					i += num;
					/* If i is too big we'll get an error the next time the loop comes around. */

					(*argcp)--;
					(*argvp)++;
					break;

				default:
					post("OpenSoundControl: don't know what to do with \"%%%c\" in message format string: dropping",
						 (int) in[1]);
					return 1;
			} /* switch */
		}
	}
	target[i] = '\0';
	return 0;
}

void OSC_accumulateMessage(OSC *x, char *messageName, short argc, Atom *argv) {
	/* Translate the message and add it to the internal buffer.  */
	

	OSC_formatData(x, messageName, argc, argv);

}

#define MAX_ARGS_TO_OSC_MSG 1024


void OSC_formatData (OSC *x, char *messageName, short argc, Atom *argv) {
	/* Format the given Max data into buf. */
	   
	int i;	
  int j;
  int k;
  int size;
  
	OSCbuf *buf = &(x->b);
	char typeString[MAX_ARGS_TO_OSC_MSG+2]; /* Space for comma and null */
	
	if (OSC_writeAddress(buf, messageName)) goto err;


	if (x->writeTypeStrings) {
		/* Write type string */
		typeString[0] = ',';

		j = 0;
		for (i=0; i < argc; i++) {
      
			switch (argv[i].a_type) {
				case A_LONG:
					typeString[j+1] = 'i';
					break;
					
				case A_FLOAT:
					typeString[j+1] = 'f';
					break;
					
				case A_SYM:
          // might be an OSCTimeTag + two longs, or just an ordinary string...
          if(strcmp(argv[i].a_w.w_sym->s_name, "OSCTimeTag") == 0 && i+1 < argc && argv[i+1].a_type == A_LONG && i+2 < argc && argv[i+2].a_type == A_LONG) {
              typeString[j+1] = 't';
              i++; i++; // skip the next two args...
          } else if(strcmp(argv[i].a_w.w_sym->s_name, "OSCBlob") == 0 && i+1 < argc && argv[i+1].a_type == A_LONG) {
              typeString[j+1] = 'b';
              i++; // skip past OSCBlob
              i += argv[i].a_w.w_long; // skip by size
          } else {
  					typeString[j+1] = 's';
          }
					break;
					
				default:
					error("OpenSoundControl: unrecognized argument type");
					break;
			}
      
      j++;
		}
		typeString[j+1] = '\0';

		if (OSC_writeStringArg(buf, typeString)) goto err;
	}

	for (i=0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				if (OSC_writeIntArg(buf, argv[i].a_w.w_long)) goto err;
				break;
				
			case A_FLOAT:
				if (OSC_writeFloatArg(buf, argv[i].a_w.w_float)) goto err;
				break;
				
			case A_SYM:
        if(strcmp(argv[i].a_w.w_sym->s_name, "OSCTimeTag") == 0 && i+1 < argc && argv[i+1].a_type == A_LONG && i+2 < argc && argv[i+2].a_type == A_LONG) {
          if (OSC_writeTimeTagArg(buf, argv[i+1].a_w.w_long, argv[i+2].a_w.w_long)) goto err;
          i++; i++;
        } else if(strcmp(argv[i].a_w.w_sym->s_name, "OSCBlob") == 0 && i+1 < argc && argv[i+1].a_type == A_LONG && i + argv[i+1].a_w.w_long < argc) {
          size = argv[i+1].a_w.w_long;
          if (OSC_writeIntArg(buf, argv[i+1].a_w.w_long)) goto err;
          i += 2;
          for(k = 0; k < size; k++) {
            buf->bufptr[k] = (unsigned char)(argv[i+k].a_w.w_long);
          }
          i += k - 1;
          while(k % 4 != 0) {
            buf->bufptr[k] = 0;
            k++;
          }
          buf->bufptr += k;
        } else {
   	      if (OSC_writeStringArg(buf, argv[i].a_w.w_sym->s_name)) goto err;
        }

				break;
				
			default:
				error("OpenSoundControl: unrecognized argument type");
				break;
		}
	}
	return;

	err:
	if (x->errorreporting) {
		post("OpenSoundControl: problem adding message to buffer:");
		post("   %s", OSC_errorMessage);
		post("Throwing away entire buffer.");
	}
	OSC_doReset(x);	
}

#ifdef DONT_HAVE_STRING_LIBRARY
void strcpy(char *s1, char *s2) {
	while (*s1++ = *s2++);
}
#endif

#define PRINTABLE(c) ((char) (c>= 0x20 && c <= 0x7e ? c : 'û'))
#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)

void OSC_printcontents (OSC *x) {
	char *m, buf[100], *p;
	int n, i;
	
	m = (x->b).buffer;
	n = OSC_packetSize(&(x->b));
	
	post("OSC_printcontents: buffer %p, size %ld", m, (long) n);

	if (n % 4 != 0) {
		post("Hey, the size isn't a multiple of 4!");
	} else {
	    for (i = 0; i < n; i += 4) {
	    	p = buf;
	    	
	    	p += sprintf(p, "  %x", m[i]);
	    	if (isprint(m[i])) {
	    		p += sprintf(p, "  (%c)", m[i]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+1]);
	    	if (isprint(m[i+1])) {
	    		p += sprintf(p, "  (%c)", m[i+1]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+2]);
	    	if (isprint(m[i+2])) {
	    		p += sprintf(p, "  (%c)", m[i+2]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+3]);
	    	if (isprint(m[i+3])) {
	    		p += sprintf(p, "  (%c)", m[i+3]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	*p = '\0';
	    	post(buf);	    		 
	    }
	}
}
	
	

#ifdef PRECOMPUTE_SIZES
/* In the old days, we used to have to compute the size of our messages
   by hand to see if there was room in the buffer. */
   
int OSC_messageSize(char *messageName, short argc, Atom *argv) {
	int result;
	int i;
	
	/* First, we need space for the messageName */
	result = OSC_effectiveStringLength(messageName);	
	
	/* Now account for the arguments */
	for (i = 0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				result += sizeof(long);
				break;

			case A_FLOAT:
				result += sizeof(float);
				break;
			
			case A_SYM:
				result += OSC_effectiveStringLength(argv[i].a_w.w_sym->s_name);
				break;
			
			default:
				error("OpenSoundControl: unrecognized argument type");
				break;
		}
	}

	return result;
}
#endif


void OSC_NewTimeTag(OSC *x, long seconds, long fraction) {
	OSCTimeTag tt;
	tt.seconds = seconds;
	tt.fraction = fraction;
	
	/* Try changing the one that's already there */
	ChangeOutermostTimestamp(&(x->b), tt);
	
	/* Remember this time stamp for next time too. */
	x->O_timeTagToUse.seconds = seconds;
	x->O_timeTagToUse.fraction = fraction;
}


/*******************************************************************
 Stuff having to do with parsing incoming OSC packets into Max data
 *******************************************************************/

int ParseOSCPacket(OSC *x, char *buf, long n, Boolean topLevel);
static void Smessage(OSC *x, char *address, void *v, long n);
char *DataAfterAlignedString(char *string, char *boundary); 
Boolean IsNiceString(char *string, char *boundary);
#ifdef DONT_HAVE_STRING_LIBRARY
int strncmp(char *s1, char *s2, int n);
#endif

void OSC_ParsePartialPacket(OSC *x, long size, long bufptr) {
	post("Can't parse partial packet.  Forget it.");
}

void OSC_ParseFullPacket(OSC *x, long size, long bufptr) {
	ParseOSCPacket(x, (char *)bufptr, size, true);
}

void OSC_ParseEvilGimme(OSC *x, Symbol *s, short not_really_argc, Atom *not_really_argv) {
	// post("Evil gimme: %d bytes, pointer %p", not_really_argc, not_really_argv);
	ParseOSCPacket(x, (char *) not_really_argv, not_really_argc, true);
}

int ParseOSCPacket(OSC *x, char *buf, long n, Boolean topLevel) {
    long size, messageLen, i;
    char *messageName;
    char *args;
    int t;

    if ((n % 4) != 0) {
    	if (x->errorreporting) {
			post("OTUDP: OpenSoundControl packet size (%d) not a multiple of 4 bytes: dropping", n);
		}
        goto ParseOSCPacket_Error;
    }
    
    if(n < 0 || n == 0) {
        post("OTUDP: OpenSoundControl bad n (%d)", n);
        goto ParseOSCPacket_Error;
    }

    if(n > x->b.size) {
        post("OTUDP: OpenSoundControl n (%d) exceeds buffer size (%d)", n, x->b.size);
        goto ParseOSCPacket_Error;
    }
    
    if(buf == NULL) {
        post("OTUDP: OpenSoundControl got null buffer");
        goto ParseOSCPacket_Error;
    }
             
    if ((n >= 8) && (strncmp(buf, "#bundle", 8) == 0)) {
		/* This is a bundle message. */
		if (n < 16) {
			if (x->errorreporting) {
		   	    post("OTUDP: Bundle message too small (%d bytes) for time tag", n);
			}
            goto ParseOSCPacket_Error;
		}

		if (topLevel) {
			Atom timeTagLongs[2];
			SETLONG(&timeTagLongs[0], ntohl(*((long *)(buf+8))));
			SETLONG(&timeTagLongs[1], ntohl(*((long *)(buf+12))));
			outlet_anything(x->O_outlet3, ps_OSCTimeTag, 2, timeTagLongs);
		}

		i = 16; /* Skip "#bundle\0" and time tag */
		while((i+sizeof(long)) < n) { // next operation will take four bytes -aws
	  	  size = ntohl(*((long *) (buf + i)));
	  	  if ((size % 4) != 0) {
			if (x->errorreporting) {
				post("OTUDP: Bad size count %d in bundle (not a multiple of 4)", size);
			}
            goto ParseOSCPacket_Error;
 	      }
	   	  if ((size + i + 4) > n) {
			 if (x->errorreporting) {
			 	post("OTUDP: Bad size count %d in bundle (only %d bytes left in entire bundle)",
			 	     size, n-i-4);
			 }
             goto ParseOSCPacket_Error;
	      }
	    
	      /* Recursively handle element of bundle */
	      t = ParseOSCPacket(x, buf+i+4, size, false);
          if(t != 0) {
              post("OTUDP: Error in encapsulated message; bailing out on bundle");
              goto ParseOSCPacket_Error;
          }
	      i += 4 + size;
	    }
		if (i != n) {
	  	    post("OTUDP: Failed to process entire packet (%d of %d)", i, n);
            goto ParseOSCPacket_Error;
		}
    } else {
		/* This is not a bundle message */

		messageName = buf;
		args = DataAfterAlignedString(messageName, buf+n);
		if (args == 0) {
			if (x->errorreporting) {
		   		post("OTUDP: Bad message name string");
			}
            goto ParseOSCPacket_Error;
		}
		
		messageLen = args-messageName;	    
		Smessage(x, messageName, (void *)args, n-messageLen);
    }
    
    if (topLevel) {
		outlet_bang(x->O_outlet1);
	}
    
    return 0;

ParseOSCPacket_Error:
    
    if (topLevel) {
		outlet_bang(x->O_outlet1);
	}
    return 1;
    
}

#define SMALLEST_POSITIVE_FLOAT 0.000001f
#define MAXARGS 5000

static void Smessage(OSC *x, char *address, void *v, long n) {
    int i, j, k;
    float *floats;
    long *ints;
    char *chars;
    char *string, *nextString, *typeTags, *thisType, *p;
    Symbol *addressSymbol, *argSymbol;
	Atom args[MAXARGS];
	int numArgs = 0;
	Boolean tooManyArgs = false;
	
	addressSymbol = gensym(address);

    /* Go through the arguments a word at a time */
    floats = v;
    ints = v;
    chars = v;


	if (x->readTypeStrings && chars[0] == ',' && IsNiceString(chars, chars+n)) {
		/* Interpret the first string argument as a type string */
				
		typeTags = chars;
		p = DataAfterAlignedString(chars, chars+n);
		
		
  		 for (thisType = typeTags+1; *thisType != 0; ++thisType) {
  		   
	       switch (*thisType) {
	            case 'i': case 'r': case 'm': case 'c':
	            SETLONG(&args[numArgs], ntohl(*((int *) p)));
	            p += 4;
	            break;

	            case 'f': 
		      { // Pretend the 32 bits are an int so I can call ntohl()
			long bytesAsInt = ntohl(*((int *) p));
			SETFLOAT(&args[numArgs], *((float *) (& bytesAsInt)));
		      }
	            p += 4;
	            break;

              case 't':
              /* handle typetags in args as they are in bundles */
              /* Could see if the data fits in a 32-bit int and output it like that if so... */
              SETSYM(&args[numArgs], ps_OSCTimeTag);
              numArgs++;
              SETLONG(&args[numArgs], ntohl(*((int *) p)));
              numArgs++;
              p += 4;
              SETLONG(&args[numArgs], ntohl(*((int *) p)));
              p += 4;
              break;
             
	            case 'h': 
	            /* 64-bit int: interpret as zero since Max doesn't have long ints */
	            /* Could see if the data fits in a 32-bit int and output it like that if so... */
	            SETLONG(&args[numArgs], 0);
	            p += 8;
	            break;

	            case 'd':
	            /* 64-bit float: interpret as zero since Max doesn't have doubles */
	            /* Could see if the data fits in a 32-bit float and output it like that if so... */
	            SETFLOAT(&args[numArgs], 0.0);
	            p += 8;
	            break;

	            case 's': case 'S':
	            if (!IsNiceString(p, typeTags+n)) {
	            	SETSYM(&args[numArgs], gensym("¥Bogus_String"));
	            } else {
	            	SETSYM(&args[numArgs], gensym(p));
	                p = DataAfterAlignedString(p, typeTags+n);
	            }
	            break;
	            
	            case 'b':
	            {
                // output symbol OSCBlob, int size, int data0 ... int dataN
	            	int size = ntohl(*((int *) p));
                
                if (p+4+size > chars+n) {
                    	error("OpenSoundControl: blob size %ld too big for packet", size);
                    	return;
	            	}
	            	SETSYM(&args[numArgs], ps_OSCBlob); numArgs++;
                
                if(numArgs + 1 + size > MAXARGS) {
                  post("OpenSoundControl: blob size too big for encoding");
                  return;
                }
	            	SETLONG(&args[numArgs], size); numArgs++;

                for(j = 0; j < size; j++) {
                  SETLONG(&args[numArgs], ((long) (*(p + 4 + j)))); numArgs++;
                }
                
                numArgs--; // increments again at end of loop
                
	            	p += OSC_effectiveBlobLength(size);

	            }
	            break;

	            case 'T': 
	            /* "True" value comes out as the int 1 */
	           	SETLONG(&args[numArgs], 1);
	           	/* Don't touch p */
	           	break;
	           	
	            case 'F': 
	            /* "False" value comes out as the int 0 */
	           	SETLONG(&args[numArgs], 0);
	           	/* Don't touch p */
	           	break;
	           	            
	            case 'N': 
	            /* Empty lists in max?  I wish!  How about the symbol "nil"? */
	            SETSYM(&args[numArgs], gensym("nil"));
	            /* Don't touch p */
	           	break;
	           	
	            case 'I': 
	            /* Infinita in Max?  Ha!  How about the symbol "Infinitum"? */
	            SETSYM(&args[numArgs], gensym("Infinitum"));
	            /* Don't touch p */
	           	break;


	            default:
	            if (x->errorreporting) {
		            post("¥ OpenSoundControl: Unrecognized type tag %c", *thisType);
		        }
	            return;
	        }
	        ++numArgs;
	     }
	} else {
		/* Use type-guessing heuristics */

	    for (i = 0; i<n/4; ) {
	    	if (numArgs >= MAXARGS) {
	    		if (x->errorreporting) {
	    			post("OTUDP: message has more than %ld arguments; dropping extra ones",
	    				 (long) MAXARGS);
	    		}
	    		break;
	    	}

			string = &chars[i*4];
			if  (ints[i] >= -1000 && ints[i] <= 1000000) {
				SETLONG(&args[numArgs], ntohl(ints[i]));
			    i++;
			} else if (floats[i] >= -1000.f && floats[i] <= 1000000.f &&
				   (floats[i]<=0.0f || floats[i] >= SMALLEST_POSITIVE_FLOAT)) {
			  // Pretend the 32 bits are an int so I can call ntohl()
			  long bytesAsInt = ntohl(ints[i]);
			  SETFLOAT(&args[numArgs], *((float *) (&bytesAsInt)));
			    i++;
			} else if (IsNiceString(string, chars+n)) {
			    nextString = DataAfterAlignedString(string, chars+n);
			    argSymbol = gensym(string);
			    SETSYM(&args[numArgs], argSymbol);
			    i += (nextString-string) / 4;
			} else {
				// Assume int if nothing looks good.
			    SETLONG(&args[numArgs], ntohl(ints[i]));
			    i++;
			}
			numArgs++;
	    }
    }
	outlet_anything(x->O_outlet2, addressSymbol, numArgs, args);
}


#define STRING_ALIGN_PAD 4

char *DataAfterAlignedString(char *string, char *boundary) 
{
    /* The argument is a block of data beginning with a string.  The
       string has (presumably) been padded with extra null characters
       so that the overall length is a multiple of STRING_ALIGN_PAD
       bytes.  Return a pointer to the next byte after the null
       byte(s).  The boundary argument points to the character after
       the last valid character in the buffer---if the string hasn't
       ended by there, something's wrong.

       If the data looks wrong, return 0, and set htm_error_string */

    int i;
    
    if ((boundary - string) %4 != 0) {
		post("OTUDP: Internal error: DataAfterAlignedString: bad boundary\n");
		return 0;
    }
    
    // note the above test doesn't catch this possibility and the next test dereferences string[0] resulting in possible crash -aws
    if ((boundary - string) == 0) { 
		post("OTUDP: Internal error: DataAfterAlignedString: no data\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (string + i >= boundary) {
		    post("DataAfterAlignedString: Unreasonably long string");
		    return 0;
		}
    }

    /* Now string[i] is the first null character */
    i++;

    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string + i >= boundary) {
		    post("DataAfterAlignedString: Unreasonably long string");
		    return 0;
		}
		if (string[i] != '\0') {
		    post("DataAfterAlignedString: Incorrectly padded string.");
		    return 0;
		}
    }

    return string+i;
}

Boolean IsNiceString(char *string, char *boundary)  {
    /* Arguments same as DataAfterAlignedString().  Is the given "string"
       really a string?  I.e., is it a sequence of isprint() characters
       terminated with 1-4 null characters to align on a 4-byte boundary? */

    int i;

    if ((boundary - string) % 4 != 0) {
		post("OTUDP: Internal error: IsNiceString: bad boundary\n");
		return 0;
    }
    
    if ((boundary - string) == 0) { 
		post("OTUDP: Internal error: DataAfterAlignedString: no data\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (!isprint(string[i])) return FALSE;
		if (string + i >= boundary) return FALSE;
    }

    /* If we made it this far, it's a null-terminated sequence of printing characters 
       in the given boundary.  Now we just make sure it's null padded... */

    /* Now string[i] is the first null character */
    i++;
    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string[i] != '\0') return FALSE;
    }

    return TRUE;
}

#ifdef DONT_HAVE_STRING_LIBRARY
int strncmp(char *s1, char *s2, int n) {
	while (n > 0) {
		if (*s1 != *s2) return *s2 - *s1;
		if (*s1 == 0) return 0;
		s1++; s2++; n--;
	}
}
#endif
