/*
Copyright (c) 2000-2004.  The Regents of the University of California
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
NAME: SDIF-listpoke
DESCRIPTION: Write data from a Max list into an SDIF-buffer
AUTHORS: Matt Wright 
COPYRIGHT_YEARS: 2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.1   021120 - Added "numcolumns" method
VERSION 0.1.1 040405 (bj) - updated to use sdif-buf.c
VERSION 0.1.2 040622 (bj) - cleanup
VERSION 0.2: Uses new version info system
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "./version.h" // make sure not to get ../SDIF-buffer/version.h




#define MAX_NUM_COLUMNS 100


/* the required include files */

#include <string.h>
// #include <float.h>
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

#include <stdio.h>
#include "SDIF-buffer.h"
#define VERY_SMALL ((sdif_float64) -9999999999999.9)

/* #include <assert.h> */

#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}
							             
#define asfloat(x)  (((x).a_type == A_FLOAT) ? ((x).a_w.w_float) : ((float) ((x).a_w.w_long)))


/* My class definiton */

typedef struct _SDIFlistpoke {
	struct object t_ob;
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;
 	void *t_out;
 	int t_errorreporting;
 	int t_complainedAboutEmptyBufferAlready;
 	
 	/* State for where I'm supposed to write my data in the SDIF-buffer */
 	sdif_float64 t_time;	/* Which frame? */
 	int t_num_columns;		/* How many cols? */
 	int t_mainMatrix;		/* If true, write to the main matrix in the frame */
 	char t_matrixType[4];	/* Otherwise, type of matrix to write to */
 	sdif_int32 t_streamID;	/* Which streamID? (For when an SDIF-buffer has multiple streams) */
} SDIFlistpoke;


Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup, *ps_emptysymbol, *ps_concatenate,
	*ps_time, *ps_reltime, *ps_direction, *ps_numcolumns, *ps_interp, *ps_max_rows;


/* global that holds the class definition */
void *SDIFlistpoke_class;

/* prototypes for my functions */
void *SDIFlistpoke_new(Symbol *s, short argc, Atom *argv);
static void LookupMyBuffer(SDIFlistpoke *x);
static void SDIFlistpoke_set(SDIFlistpoke *x, Symbol *bufName);
static void SDIFlistpoke_errorreporting(SDIFlistpoke *x, long yesno);
static void SDIFlistpoke_time(SDIFlistpoke *x, double t);
static void SDIFlistpoke_numcolumns(SDIFlistpoke *x, long n);
static void SDIFlistpoke_matrixtype(SDIFlistpoke *x, Symbol *matrixType);
static void SDIFlistpoke_listpoke(SDIFlistpoke *x, Symbol *s, short argc, Atom *argv);
static void SDIFlistpoke_newmatrix(SDIFlistpoke *x, Symbol *s, short argc, Atom *argv);
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);




static void *my_getbytes(int numBytes) {
	return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
	freebytes(bytes, (short) size);
}


void main() {
	SDIFresult r;
	
	
	version(0);
	
	r = SDIFmem_Init(my_getbytes, my_freebytes);
	
	if (r) {
		ouchstring("Couldn't initialize SDIF memory utilities! %s", SDIF_GetErrorString(r));
	}	
	
	setup((t_messlist **)&SDIFlistpoke_class, (method)SDIFlistpoke_new, 0,
			(short)sizeof(SDIFlistpoke), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
        addmess((method)version, "version", 0);
	addmess((method)SDIFlistpoke_set, "set", A_SYM, 0);	
	addmess((method)SDIFlistpoke_errorreporting, "errorreporting", A_LONG, 0);
	addmess((method)SDIFlistpoke_time, "time", A_FLOAT, 0);
	addmess((method)SDIFlistpoke_numcolumns, "numcolumns", A_LONG, 0);
	addmess((method)SDIFlistpoke_listpoke, "listpoke", A_GIMME, 0);
	addmess((method)SDIFlistpoke_listpoke, "list", A_GIMME, 0);
	addmess((method)SDIFlistpoke_newmatrix, "newmatrix", A_GIMME, 0);
	addmess((method)SDIFlistpoke_matrixtype, "matrixtype", A_DEFSYM, 0);

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

	/* list object in the new object list */
	finder_addclass("Data", NAME);
	
	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	ps_emptysymbol  = gensym("");
	ps_concatenate = gensym("concatenate");
	ps_time = gensym("time");
	ps_reltime = gensym("reltime");
	ps_direction = gensym("direction");
	ps_numcolumns = gensym("numcolumns");
	ps_interp = gensym("interp");
	ps_max_rows = gensym("max_rows");
	
}

void *SDIFlistpoke_new(Symbol *dummy, short argc, Atom *argv) {
	SDIFlistpoke *x;
	
	// post("SDIFlistpoke_new: %s, %ld args", s->s_name, (long) argc);
	
	x = newobject(SDIFlistpoke_class);
	x->t_errorreporting = 0;
	x->t_buffer = 0;
	x->t_out = bangout(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			post("¥ SDIF-tuples: argument must be name of an SDIF-buffer");
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}

	x->t_mainMatrix = 1;
	x->t_time = 0.0;
    x->t_num_columns = 1;
    
	return (x);
}

static void LookupMyBuffer(SDIFlistpoke *x) {
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

static void SDIFlistpoke_set(SDIFlistpoke *x, Symbol *bufName) {

	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("¥ SDIF-tuples: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
	x->t_complainedAboutEmptyBufferAlready = 0;
}

static void SDIFlistpoke_errorreporting(SDIFlistpoke *x, long yesno) {
	x->t_errorreporting = yesno;
}


static void SDIFlistpoke_time(SDIFlistpoke *x, double t) {
	x->t_time = (sdif_float64) t;
}

static void SDIFlistpoke_numcolumns(SDIFlistpoke *x, long n) {
	if (n <= 0) {
		post("¥ SDIF-listpoke: numcolumns: number must be non-negative");
	} else {
		x->t_num_columns = n;
	}
}


static void SDIFlistpoke_matrixtype(SDIFlistpoke *x, Symbol *matrixType) {
	if (matrixType == ps_emptysymbol) {
		x->t_mainMatrix = 1;
	} else {		
		/* Here's where I could implement user matrix names that aren't just the 4 bytes. */
		if (strlen(matrixType->s_name) != 4) {
			post("¥ SDIF-tuples: matrix %s is not 4 letters!", matrixType->s_name);
			return;
		}
		x->t_mainMatrix = 0;
		SDIF_Copy4Bytes(x->t_matrixType, matrixType->s_name);
	}
}


static void SDIFlistpoke_newmatrix(SDIFlistpoke *x, Symbol *s, short argc, Atom *argv) {
	sdif_float64 time, oldtime;
	int numrows, numcols, oldnumcols, oldmainmatrix;
	char oldmatrixtype[4];
	char *type;
	
	if (argc < 5) {
		badargs:
		post("¥ SDIF-listpoke: newmatrix args: frametime, matrixtype, #rows, #cols, data...");
		return;
	}
	
	if (argv[0].a_type == A_SYM) {
		post("¥ SDIF-listpoke: newmatrix: frame time must be a number");
		return;
	}
	time = asfloat(argv[0]);


	if (argv[1].a_type != A_SYM ) {
		post("¥ SDIF-listpoke: newmatrix: matrix type must be a symbol");
		return;
	}
	
	type = argv[1].a_w.w_sym->s_name;
	if (type[0] == '\0' || type[1] == '\0' || type[2] == '\0' || type[3] == '\0' || type[4] != '\0') {
		post("¥ SDIF-listpoke: newmatrix: illegal type \"%s\" is not 4 characters.", type);
		return;
	}

	
	if (argv[2].a_type != A_LONG) {
		post("¥ SDIF-listpoke: newmatrix: #rows must be an int.");
		return;
	}
	numrows = argv[2].a_w.w_long;

	if (numrows <= 0) {
		post("¥ SDIF-listpoke: newmatrix: #rows must be >= 1");
		return;
	}

	if (argv[3].a_type != A_LONG) {
		post("¥ SDIF-listpoke: newmatrix: #columns must be an int.");
		return;
	}
	numcols = argv[3].a_w.w_long;
	
	if (numcols <= 0) {
		post("¥ SDIF-listpoke: newmatrix: #columns must be >= 1");
		return;
	}

	
	if (numrows*numcols != argc-4) {
		post("¥ SDIF-listpoke: newmatrix: %ld rows times %ld columns is %ld,", 
			  numrows, numcols, numrows*numcols);
		post("   but there are %ld elements of matrix data.  Dropping.", argc-4);
		return;
	}
	
	
	/* Store old instance variables  */
	oldtime = x->t_time;
	oldnumcols = x->t_num_columns;
	oldmainmatrix = x->t_mainMatrix;
	SDIF_Copy4Bytes(oldmatrixtype, x->t_matrixType);
	
	
	/*temporarily overwrite them */
	x->t_time = time;
	x->t_num_columns = numcols;
	x->t_mainMatrix = 0;
	SDIF_Copy4Bytes(x->t_matrixType, type);
	
	/* Store the new matrix via listpoke */
	SDIFlistpoke_listpoke(x, s, argc-4, argv+4);
	
	/* Restore instance variables */
	x->t_time = oldtime;
	x->t_num_columns = oldnumcols;
	x->t_mainMatrix = oldmainmatrix;
	SDIF_Copy4Bytes(x->t_matrixType, oldmatrixtype);
}	


static void SDIFlistpoke_listpoke(SDIFlistpoke *x, Symbol *dummy, short argc, Atom *argv) {
	int i;
	SDIFmem_Frame f;
	SDIFmem_Matrix m;
	float *mdata;
	char myFrameType[4];
	char myMatrixType[4];
	SDIFresult r;


	// post("* SDIFlistpoke_listpoke: x %p, argc %ld, argv %p", x, argc, argv);
	
 	
	/* Check that arguments are all numbers */
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_FLOAT && argv[i].a_type != A_LONG) {
			post("¥ SDIF-listpoke: only numbers allowed in list.");
			return;
		}
	}
	
	/* Check that the number of arguments is a multiple of x->t_num_columns */
	if (argc % x->t_num_columns != 0) {
		post("¥ SDIF-listpoke: %d element list doesn't go into %ld columns.  Ignoring.",
			 argc, x->t_num_columns);
		return;
	}

	/* Get the frame */
	LookupMyBuffer(x);
	
	if (x->t_buffer == 0) {
		post("¥ SDIF-listpoke: no buffer!");
		return;
	}

  //  decide the frame type
  if(f = SDIFbuf_GetFirstFrame(x->t_buf))
    SDIF_Copy4Bytes(myFrameType, f->header.frameType);
  else
    //  we are about to create first frame in the buffer
    //  (give it the same type as the matrix we are adding)
    SDIF_Copy4Bytes(myFrameType, x->t_matrixType);
  
  //  decide the matrix type
	if (x->t_mainMatrix) {
		SDIF_Copy4Bytes(myMatrixType, myFrameType);
	} else {
		SDIF_Copy4Bytes(myMatrixType, x->t_matrixType);
	}

	/* post("** myMatrixType: %c%c%c%c", myMatrixType[0], myMatrixType[1],
		 myMatrixType[2], myMatrixType[3]); */
	
	f = (*(x->t_buffer->FrameLookup))(x->t_buffer, x->t_time, 0);
	
	if (f == 0) {
		/* There was no frame at the given time */
		// post("** There was no frame at that time");
		f = SDIFmem_CreateEmptyFrame();
		
		if (f == 0) {
			post("¥ SDIF-listpoke: out of memory for new frame in SDIF-buffer!");
			return;
		}
		
		SDIF_Copy4Bytes(f->header.frameType, myFrameType);
		f->header.time = x->t_time;
		f->header.streamID = x->t_buffer->streamID;
		
		i = (*(x->t_buffer->FrameInsert))(f, x->t_buffer);
		if (i) {
			post("¥ SDIF-listpoke: FrameInsert returned %d", i);
		}

				
	} else {
		/* There was already a frame at the given time */

		for (m = f->matrices; m != 0; m = m->next) {
			if (SDIF_Char4Eq(myMatrixType, m->header.matrixType)) {
				post("SDIF-listpoke: deleting old matrix %c%c%c%c at time %f",
					 m->header.matrixType[0], m->header.matrixType[1], 
					 m->header.matrixType[2], m->header.matrixType[3], x->t_time);
				r = SDIFmem_RemoveMatrix(f, m);
				if (r != ESDIF_SUCCESS) {
					post("¥ SDIF-listpoke: Problem removing matrix: %s", SDIF_GetErrorString(r));
				}
			}
		}
	}

	/* Now we know f is a frame in x->t_buffer, at the right time, with no matrix of 
	   the type we want to write into, and that we have to call SDIFmem_RepairFrameHeader(f) */

	m = SDIFmem_CreateEmptyMatrix();

	if (m == 0) {
		post("¥ SDIF-listpoke: out of memory for new matrix in SDIF-buffer!");
		SDIFmem_FreeFrame(f);
		return;
	}
	
	SDIF_Copy4Bytes(m->header.matrixType, myMatrixType);
	m->header.matrixDataType = SDIF_FLOAT32;
	
	m->header.columnCount = x->t_num_columns;
	m->header.rowCount = argc / x->t_num_columns;  // We checked above that it divides evenly

	// post("** about to getbytes() for the matrix data");

	m->data = getbytes(argc * sizeof(float));
	if (m->data == 0) {
		post("¥ SDIF-listpoke: out of memory for matrix data in SDIF-buffer!");
		SDIFmem_RemoveMatrix(f, m);
		SDIFmem_FreeMatrix(m);
		SDIFmem_RepairFrameHeader(f);	
		return;
	}

	mdata = m->data;
	for (i = 0; i < argc; ++i) {
		mdata[i] = asfloat(argv[i]);
	}	

	/*
	{
		SDIFmem_Matrix p;
		for (p = f->matrices; p != NULL; p = p->next) {
			post("Matrix: %p, type %c%c%c%c", p, 
				 p->header.matrixType[0], p->header.matrixType[1], p->header.matrixType[2],
				 p->header.matrixType[3]);
		}
	}
	*/
	
	
	// post("** about to SDIFmem_AddMatrix");
	SDIFmem_AddMatrix(f, m);

	// post("** about to SDIFmem_RepairFrameHeader");
	SDIFmem_RepairFrameHeader(f);	
}
