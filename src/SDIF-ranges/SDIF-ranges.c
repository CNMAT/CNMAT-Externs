/*
Copyright (c) 2004.  The Regents of the University of California
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
NAME: SDIF-ranges
DESCRIPTION: Find ranges of data in an SDIF-buffer
AUTHORS: Matt Wright 
COPYRIGHT_YEARS: 2004,05,06
DRUPAL_NODE: /patch/4011
SVN_REVISION: $LastChangedRevision$
VERSION 0.0.2: 050103 mw Added column-range message, updated to new versioning system
VERSION 0.0.3: updated to new new versioning system
VERSION 0.1: Compiles for CFM and MachO
VERSION 0.1.1: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   	
  To-do:  ranges for a given row across all frames
*/

#include "./version.h" // Make sure not to get ../SDIF-buffer/version.h

// #include <string.h>
// #include <float.h>
// #include <stdlib.h>
// #include <math.h>
// #include <limits.h>
#include "ext.h"

/* Undo ext.h's macro versions of some of stdio.h: */
/* #undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf */

#include "version.c"

#include <stdio.h>
#include <limits.h>  // for SHRT_MAX
#include "SDIF-buffer.h"  //  includes sdif.h, sdif-mem.h, sdif-buf.h

/* #include <assert.h> */
#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}

#ifdef WIN_VERSION
#ifdef STUCK_WITH_MICROSOFT_COMPILER
#include <z_dsp.h>
extern int isnan(double d) {
	return IS_NAN_DOUBLE(d);
}
#endif
#endif

/* My class definiton */

typedef struct _SDIFranges {
	struct object t_ob;
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;           //  provides API to manipulate buffer contents
 	void *t_out;
} SDIFranges;


/* global that holds the class definition */
static void *SDIFranges_class;
static Symbol *ps_SDIF_buffer_lookup, *ps_column_mins, *ps_column_maxes, *ps_maxcolumns, *ps_column_range;

/* prototypes for my functions */
void *SDIFranges_new(Symbol *s, short argc, Atom *argv);
void SDIFranges_free(SDIFranges *x);
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
static void LookupMyBuffer(SDIFranges *x);
static void SDIFranges_set(SDIFranges *x, Symbol *bufName);
void SDIFranges_GetMaxNumColumns(SDIFranges *x, Symbol *matrixTypeSym);
static int doGetMaxNumColumns(SDIFranges *x, char *matrixType, sdif_int32 *answer);
void SDIFranges_GetColumnRanges(SDIFranges *x, Symbol *matrixTypeSym);
void SDIFranges_GetColumnRange(SDIFranges *x, long column, Symbol *matrixTypeSym);


void main(fptr *fp)
{
  SDIFresult r;
  
	version(0);
		
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&SDIFranges_class, (method)SDIFranges_new, (method)SDIFranges_free,
			(short)sizeof(SDIFranges), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
    addmess((method)version, "version", 0);
	addmess((method)SDIFranges_set, "set", A_SYM, 0);	
	addmess((method)SDIFranges_GetMaxNumColumns, "max-columns", A_DEFSYM, 0);
	addmess((method)SDIFranges_GetColumnRanges, "column-ranges", A_DEFSYM, 0);
	addmess((method)SDIFranges_GetColumnRange, "column-range", A_LONG, A_DEFSYM, 0);



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
	
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	ps_column_mins = gensym("/column-mins");
	ps_column_maxes = gensym("/column-maxes");
	ps_maxcolumns = gensym("/max-num-columns");
	ps_column_range = gensym("/column-range");


	/* list object in the new object list */
	finder_addclass("Data", NAME);
	
}

void *SDIFranges_new(Symbol *dummy, short argc, Atom *argv) {
	SDIFranges *x;
	int i;
	
	// post("SDIFranges_new: %s, %ld args", s->s_name, (long) argc);
	
	x = newobject(SDIFranges_class);
	x->t_buffer = 0;
	x->t_out = listout(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			error(NAME ": argument must be name of an SDIF-buffer");
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}
	
	x->t_buf = NULL;

	return (x);
}

void SDIFranges_free(SDIFranges *x)
{
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

static void LookupMyBuffer(SDIFranges *x) {
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		x->t_buffer = 0;
	} else {
		x->t_buffer = (*f)(x->t_bufferSym);
	}

  //  get access to the SDIFbuf_Buffer instance
  if (x->t_buffer)
    x->t_buf = (x->t_buffer->BufferAccessor)(x->t_buffer);
  else
    x->t_buf = NULL;
}

static void SDIFranges_set(SDIFranges *x, Symbol *bufName) {
	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("¥ " NAME ": warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}

}

static int resolveBufferAndMatrixType(SDIFranges *x, Symbol *matrixTypeSym, char *matrixType) {
    int i;

	if (x->t_bufferSym == 0) {
		post("SDIFranges: no SDIF buffer name specified");
		return 0;
	}
	
	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("¥ SDIFranges: \"%s\" is not an SDIF buffer.", x->t_bufferSym->s_name);
		return 0;
	}
		
    if (matrixTypeSym->s_name[0] == '\0') {
    	// Use "main matrix": the one whose type is the same as the frame type
	    SDIFmem_Frame f;
	  
		if((f = SDIFbuf_GetFirstFrame(x->t_buf)) == NULL) {
			post(NAME ":  SDIF-buffer %s is empty",  x->t_bufferSym->s_name);
			return 0;
		} else {
	  		SDIF_Copy4Bytes(matrixType, f->header.frameType);
	  	}
	 } else {
	 	// Look at user-supplied matrix type
	    for (i = 1; i < 4; ++i) {
	   		if (matrixTypeSym->s_name[i] == '\0') {
	   			error(NAME ": error: maxcolumns' matrix type argument \"%s\" is less than 4 characters.",
	   				 matrixTypeSym->s_name);
				return 0;
			}
		}
		
	    SDIF_Copy4Bytes(matrixType, matrixTypeSym->s_name);

		if (matrixTypeSym->s_name[4] != '\0') {
			post("¥ " NAME ": warning: truncating maxcolumns' matrix type argument to \"%c%c%c%c\".",
				 matrixType[0], matrixType[1], matrixType[2], matrixType[3]);
	    }
	 }
	 return 1;
}


static int doGetMaxNumColumns(SDIFranges *x, char *matrixType, sdif_int32 *answer) {
	SDIFresult r = SDIFbuf_GetMaxNumColumns(x->t_buf, matrixType, answer);
	
	if (r==ESDIF_END_OF_DATA) {
		post(NAME ":  SDIF-buffer %s is empty",  x->t_bufferSym->s_name);
		return 0;
	} else if (r==ESDIF_BAD_MATRIX_DATA_TYPE) {
		post(NAME ":  SDIF-buffer %s has no matrices of type %c%c%c%c",
			 x->t_bufferSym->s_name, matrixType[0], matrixType[1], matrixType[2], matrixType[3]);
	    return 0;
	}
	return 1;
}


void SDIFranges_GetMaxNumColumns(SDIFranges *x, Symbol *matrixTypeSym) {
    char matrixType[4];
	sdif_int32 answer;
    
    if (resolveBufferAndMatrixType(x, matrixTypeSym, matrixType)) {
		if (doGetMaxNumColumns(x, matrixType, &answer)) {
			Atom outputArgs[1];	
			SETLONG(outputArgs, answer);
			outlet_anything(x->t_out, ps_maxcolumns, 1, outputArgs);
		}
	}
}

#ifdef WIN_VERSION
// Workaround kludge
#define getbytes16 getbytes
#define freebytes16 freebytes
#endif

void SDIFranges_GetColumnRanges(SDIFranges *x, Symbol *matrixTypeSym) {
    char matrixType[4];
	sdif_int32 numCols;
	int i;
    
    if (resolveBufferAndMatrixType(x, matrixTypeSym, matrixType)) {
		if (doGetMaxNumColumns(x, matrixType, &numCols)) {
			Atom *outputArgs = (Atom *) getbytes(numCols * sizeof(Atom));
			sdif_float64 *mins = (sdif_float64 *) getbytes16(numCols * sizeof(sdif_float64));
			sdif_float64 *maxes = (sdif_float64 *) getbytes16(numCols * sizeof(sdif_float64));
			
			if (maxes == 0) {
				error(NAME ": out of memory");
				return;
			}
			SDIFbuf_GetColumnRanges(x->t_buf, matrixType, numCols, mins, maxes);
			
			for (i = 0; i<numCols; ++i) {
				SETFLOAT(outputArgs+i, (float) mins[i]);
			}
			outlet_anything(x->t_out, ps_column_mins, numCols, outputArgs);
			
			for (i = 0; i<numCols; ++i) {
				SETFLOAT(outputArgs+i, (float) maxes[i]);
			}
			outlet_anything(x->t_out, ps_column_maxes, numCols, outputArgs);
			
			freebytes(outputArgs, numCols * sizeof(Atom));
			freebytes16((char *)mins, numCols * sizeof(sdif_float64));
			freebytes16((char *)maxes, numCols * sizeof(sdif_float64));
		}
	}
}

void SDIFranges_GetColumnRange(SDIFranges *x, long columnArg, Symbol *matrixTypeSym) {
    char matrixType[4];
	sdif_int32 numCols;
	int i;
	SDIFresult r;
	long column = columnArg -1;  /* Max user numbers columns from 1; internally it's zero-origin. */
    
    if (columnArg <= 0) {
    	error(NAME ": column_range: illegal column number %ld (must be >= 1)", columnArg);
    	return;
    } 
    
    if (!resolveBufferAndMatrixType(x, matrixTypeSym, matrixType)) {
    	error("Couldn't resolve SDIF-buffer and/or matrix type.");
    	return;
    }
    
	if (!doGetMaxNumColumns(x, matrixType, &numCols)) {
	    error("Couldn't get max num columns");
	 }
	 
    // post("* numCols %ld, want column %ld", numCols, column);
	if (column >= numCols) {
		error(NAME 
			 ": SDIF-buffer %s has only %ld columns; can't	give column-range of column %ld",
			 x->t_bufferSym->s_name, numCols, columnArg);
		return;
	} else {
		Atom outputArgs[3];
		sdif_float64 min, max;

		if (r = SDIFbuf_GetColumnRange(x->t_buf, matrixType, column, &min, &max)) {
			post("¥ %s: Couldn't get column range:  %s", 
	             NAME, SDIF_GetErrorString(r));
            return;
		}
		
		post("**  min %f, max %f", (float) min, (float) max);
		SETLONG(outputArgs, columnArg);
		SETFLOAT(outputArgs+1, (float) min);
		SETFLOAT(outputArgs+2, (float) max);
		
		outlet_anything(x->t_out, ps_column_range, 3, outputArgs);
	
	}
}

