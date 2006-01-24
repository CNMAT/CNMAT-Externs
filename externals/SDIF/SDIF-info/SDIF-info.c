/*
Copyright (c) 2002-2004.  The Regents of the University of California
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
University of California, Berkeley. Maintenance by Ben "Jacobs".

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
NAME: SDIF-info
DESCRIPTION: report overall info about an SDIF-buffer
AUTHORS: Matt Wright and Ben "Jacobs"
COPYRIGHT_YEARS: 2002,03,04,05,06
VERSION 0.0.1: 04/05/2004 -  (bj): updated to use sdif-buf.c
VERSION 0.0.2: 06/22/2004 -  (bj): cleanup
VERSION 0.0.3: 1/24/6 (mw):  Uses new version info system
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"

#include <string.h>
#include <float.h>
#include <limits.h>
#include "ext.h"

/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#include <stdio.h>

#include "SDIF-buffer.h"  //  includes "sdif.h", "sdif-mem.h", "sdif-buf.h"
#define VERY_SMALL ((sdif_float64) -(DBL_MAX))

/* #include <assert.h> */

#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}

/* My class definiton */

typedef struct _SDIFinfo {
	struct object t_ob;
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;
 	void *t_out;
} SDIFinfo;

/* prototypes for my functions */
void *SDIFinfo_new(Symbol *s, short argc, Atom *argv);
void *my_getbytes(int numBytes);
void my_freebytes(void *bytes, int size);
static void LookupMyBuffer(SDIFinfo *x);
static void SDIFinfo_set(SDIFinfo *x, Symbol *bufName);
static void SDIFinfo_bang(SDIFinfo *x);

void SDIFinfo_version(SDIFinfo *x);


/* global that holds the class definition */
void *SDIFinfo_class;

Symbol *ps_name, *ps_filename, *ps_streamID, *ps_frameType, *ps_minTime, *ps_maxTime, 
	   *ps_numFrames, *ps_SDIF_buffer_lookup, *ps_noFileName;

void main(fptr *fp)
{
  SDIFresult r;
  
	SDIFinfo_version(0);
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&SDIFinfo_class, (method) SDIFinfo_new, 0,
			(short)sizeof(SDIFinfo), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
	addmess((method)SDIFinfo_set, "set", A_SYM, 0);	
	addmess((method)SDIFinfo_version, "version", 0);
	addbang((method)SDIFinfo_bang);
	
  //  initialize SDIF libraries
	if (r = SDIF_Init()) {
		ouchstring("%s: Couldn't initialize SDIF library! %s", 
		           NAME,
		           SDIF_GetErrorString(r));
    return;
	}
	
	if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
		post("¥ %s: Couldn't initialize SDIF memory utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
    return;
	}
		
	if (r = SDIFbuf_Init()) {
		post("¥ %s: Couldn't initialize SDIF buffer utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
		return;
	}

	ps_name = gensym("/name");
	ps_filename = gensym("/filename");
	ps_streamID = gensym("/streamID");
	ps_frameType = gensym("/frameType");
	ps_minTime = gensym("/minTime");
	ps_maxTime = gensym("/maxTime");
	ps_numFrames = gensym("/numFrames");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	ps_noFileName = gensym("---no-filename---");
	
	/* list object in the new object list */
	finder_addclass("Data","SDIF-info");		
}

void *SDIFinfo_new(Symbol *dummy, short argc, Atom *argv) {
	SDIFinfo *x;
	int i;
	
	// post("SDIFinfo_new: %s, %ld args", s->s_name, (long) argc);
	
	x = newobject(SDIFinfo_class);
	x->t_buffer = 0;
	x->t_buf = NULL;
	x->t_out = listout(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			post("¥ SDIF-info: argument must be name of an SDIF-buffer");
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}

	return (x);
}

static void *my_getbytes(int numBytes) {
	if (numBytes > SHRT_MAX) {
			return 0;
	}
	return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
	freebytes(bytes, (short) size);
}

static void SDIFinfo_version(SDIFinfo *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
    post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California.");
}

static void LookupMyBuffer(SDIFinfo *x) {
#ifdef THE_S_THING_KLUDGE
	void *thing;
	
	thing = x->t_bufferSym->s_thing;
	
	if (thing) {
		if (ob_sym(thing) = ps_SDIFbuffer) {
			x->t_buffer = thing;
			return;
		}
	}
	x->t_buffer = 0;
#else
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		x->t_buffer = 0;
	} else {
		x->t_buffer = (*f)(x->t_bufferSym);
	}
#endif

  //  get access to the SDIFbuf_Buffer instance
  if (x->t_buffer)
    x->t_buf = (x->t_buffer->BufferAccessor)(x->t_buffer);
  else
    x->t_buf = NULL;
}

static void SDIFinfo_set(SDIFinfo *x, Symbol *bufName) {
	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("¥ SDIF-info: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
}


static void SDIFinfo_bang(SDIFinfo *x) {
	Atom outputArgs[1];	
	sdif_float64 tMin, tMax;
	SDIFmem_Frame f;
	char frameTypeString[5];
	Symbol *frameTypeSym;
	
	if (x->t_bufferSym == 0) {
		post("¥ SDIF-info: no SDIF buffer name specified");
		return;
	}
	
	LookupMyBuffer(x);

	if (x->t_buffer == 0) {
		post("¥ SDIF-info: \"%s\" is not an SDIF buffer.", x->t_bufferSym->s_name);
		return;
	}


	/* /name */
	SETSYM(outputArgs, x->t_buffer->s_myname);
	outlet_anything(x->t_out, ps_name, 1, outputArgs);

	/* /filename */
	{
		if (x->t_buffer->fileName == 0) {
			SETSYM(outputArgs, ps_noFileName);
		} else {
			Symbol *filenameSym = gensym(x->t_buffer->fileName);
			SETSYM(outputArgs, filenameSym);
		}
		outlet_anything(x->t_out, ps_filename, 1, outputArgs);
	}
	
	/* /streamID */
	SETLONG(outputArgs, x->t_buffer->streamID);
	outlet_anything(x->t_out, ps_streamID, 1, outputArgs);
	
	/* /frameType */
	if(f = SDIFbuf_GetFirstFrame(x->t_buf))
	{
	post("Frame %p, type %c%c%c%c", f, f->header.frameType[0], f->header.frameType[1], f->header.frameType[2], f->header.frameType[3]);
  	SDIF_Copy4Bytes(frameTypeString, f->header.frameType);
  	frameTypeString[4] = '\0';
  	frameTypeSym = gensym(frameTypeString);
	
  	SETSYM(outputArgs, frameTypeSym);
  	outlet_anything(x->t_out, ps_frameType, 1, outputArgs);
    }
	
	/* /minTime and /maxTime */
	SDIFbuf_GetMinTime(x->t_buf, &tMin);
	SETFLOAT(outputArgs, (float) tMin);
	outlet_anything(x->t_out, ps_minTime, 1, outputArgs);
	
	SDIFbuf_GetMaxTime(x->t_buf, &tMax);
	SETFLOAT(outputArgs, (float) tMax);
	outlet_anything(x->t_out, ps_maxTime, 1, outputArgs);
	
	/* /numFrames */
	{
		long numFrames = 0;
		SDIFmem_Frame f;
		
		f = (*(x->t_buffer->FrameLookup))(x->t_buffer, (sdif_float64) VERY_SMALL, 1);
	
		while (f != 0) {
			f = f->next;
			++numFrames;
		}
	
		SETLONG(outputArgs, numFrames);
		outlet_anything(x->t_out, ps_numFrames, 1, outputArgs);
	}
}
