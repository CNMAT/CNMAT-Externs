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
University of California, Berkeley. Maintenance by Ben Jacobs.

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
#define NAME "SDIF-info"
#define DESCRIPTION "report overall info about an SDIF-buffer"
#define AUTHORS "Matt Wright and Ben Jacobs"
#define COPYRIGHT_YEARS "2002-06,12,13"


/*

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: SDIF-info
DESCRIPTION: report overall info about an SDIF-buffer
AUTHORS: Matt Wright and Ben Jacobs
COPYRIGHT_YEARS: 2002,03,04,05,06
PUBLICATION: ICMC 99 paper | http://www.cnmat.berkeley.edu/ICMC99/papers/msp+sdif/icmc99-msp+sdif-short.pdf
DRUPAL_NODE: /patch/4014
SVN_REVISION: $LastChangedRevision$
VERSION 0.0.1: 04/05/2004 -  (bj): updated to use sdif-buf.c
VERSION 0.0.2: 06/22/2004 -  (bj): cleanup
VERSION 0.0.3: 1/24/6 (mw):  Uses new version info system
VERSION 0.0.4: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h" // make sure not to get ../SDIF-buffer/version.h

#include <string.h>
#include <float.h>
#include <limits.h>
#include "ext.h"
#include "ext_obex.h"


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
void *SDIFinfo_new(t_symbol *s, short argc, t_atom *argv);
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
static void LookupMyBuffer(SDIFinfo *x);
static void SDIFinfo_set(SDIFinfo *x, t_symbol *bufName);
static void SDIFinfo_bang(SDIFinfo *x);
void SDIFinfo_tellmeeverything(SDIFinfo *x);

/* global that holds the class definition */
t_class *SDIFinfo_class;

t_symbol *ps_name, *ps_filename, *ps_streamID, *ps_frameType, *ps_minTime, *ps_maxTime, 
	   *ps_numFrames, *ps_SDIF_buffer_lookup, *ps_noFileName;

int main(void)
{
  SDIFresult r;
  
	version_post_copyright();
	
	/* tell Max about my class. The cast to short is important for 68K */
	SDIFinfo_class = class_new("SDIF-info", (method) SDIFinfo_new, 0,
			(short)sizeof(SDIFinfo), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
	class_addmethod(SDIFinfo_class, (method)version, "version", 0);
	class_addmethod(SDIFinfo_class, (method)SDIFinfo_set, "set", A_SYM, 0);	
	class_addmethod(SDIFinfo_class, (method)SDIFinfo_bang, "bang", 0);
	class_addmethod(SDIFinfo_class, (method)SDIFinfo_tellmeeverything, "tellmeeverything", 0);
	
  //  initialize SDIF libraries
    if ((r = SDIF_Init())) {
		ouchstring("%s: Couldn't initialize SDIF library! %s", 
		           NAME,
		           SDIF_GetErrorString(r));
    return 0;
	}
	
    if ((r = SDIFmem_Init(my_getbytes, my_freebytes))) {
		post("� %s: Couldn't initialize SDIF memory utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
    return 0;
	}
		
    if ((r = SDIFbuf_Init())) {
		post("� %s: Couldn't initialize SDIF buffer utilities! %s", 
		     NAME,
		     SDIF_GetErrorString(r));
		return 0;
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
	//finder_addclass("Data","SDIF-info");

	class_register(CLASS_BOX, SDIFinfo_class);
	return 0;
}

void *SDIFinfo_new(t_symbol *dummy, short argc, t_atom *argv) {
	SDIFinfo *x;
	//int i;
	
	// post("SDIFinfo_new: %s, %ld args", s->s_name, (long) argc);
	
	x = object_alloc(SDIFinfo_class);
	if(!x){
		return NULL;
	}
	x->t_buffer = 0;
	x->t_buf = NULL;
	x->t_out = outlet_new(x, NULL);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			object_post((t_object *)x, "� SDIF-info: argument must be name of an SDIF-buffer");
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

static void SDIFinfo_set(SDIFinfo *x, t_symbol *bufName) {
	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		object_post((t_object *)x, "� SDIF-info: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
}


static void SDIFinfo_bang(SDIFinfo *x) {
	t_atom outputArgs[1];	
	sdif_float64 tMin, tMax;
	SDIFmem_Frame f;
	char frameTypeString[5];
	t_symbol *frameTypeSym;
	
	if (x->t_bufferSym == 0) {
		object_post((t_object *)x, "� SDIF-info: no SDIF buffer name specified");
		return;
	}
	
	LookupMyBuffer(x);

	if (x->t_buffer == 0) {
		object_post((t_object *)x, "� SDIF-info: \"%s\" is not an SDIF buffer.", x->t_bufferSym->s_name);
		return;
	}


	/* /name */
	atom_setsym(outputArgs, x->t_buffer->s_myname);
	outlet_anything(x->t_out, ps_name, 1, outputArgs);

	/* /filename */
	{
		if (x->t_buffer->fileName == 0) {
			atom_setsym(outputArgs, ps_noFileName);
		} else {
			t_symbol *filenameSym = gensym(x->t_buffer->fileName);
			atom_setsym(outputArgs, filenameSym);
		}
		outlet_anything(x->t_out, ps_filename, 1, outputArgs);
	}
	
	/* /streamID */
	atom_setlong(outputArgs, x->t_buffer->streamID);
	outlet_anything(x->t_out, ps_streamID, 1, outputArgs);
	
	/* /frameType */
    if((f = SDIFbuf_GetFirstFrame(x->t_buf)))
	{
		//post("Frame %p, type %c%c%c%c", f, f->header.frameType[0], f->header.frameType[1], f->header.frameType[2], f->header.frameType[3]);
  	SDIF_Copy4Bytes(frameTypeString, f->header.frameType);
  	frameTypeString[4] = '\0';
  	frameTypeSym = gensym(frameTypeString);
	
  	atom_setsym(outputArgs, frameTypeSym);
  	outlet_anything(x->t_out, ps_frameType, 1, outputArgs);
    }
	
	/* /minTime and /maxTime */
	SDIFbuf_GetMinTime(x->t_buf, &tMin);
	atom_setfloat(outputArgs, (float) tMin);
	outlet_anything(x->t_out, ps_minTime, 1, outputArgs);
	
	SDIFbuf_GetMaxTime(x->t_buf, &tMax);
	atom_setfloat(outputArgs, (float) tMax);
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
	
		atom_setlong(outputArgs, numFrames);
		outlet_anything(x->t_out, ps_numFrames, 1, outputArgs);
	}
}

void SDIFinfo_tellmeeverything(SDIFinfo *x){
	version_post_copyright();
	object_post((t_object *)x, "buffer name: %s", x->t_bufferSym->s_name);
}
