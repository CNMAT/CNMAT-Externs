/*
Copyright (c) 1999, 2000-07.  The Regents of the University of California
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
University of California, Berkeley. Interpolation support by Ben "Jacobs".

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
NAME: SDIF-tuples
DESCRIPTION: Extract "tuples" of data (a list of values for each column) from an SDIF-buffer and concatenate them to form a Max list.
AUTHORS: Matt Wright and Ben "Jacobs" 
PUBLICATION: ICMC99 paper | http://www.cnmat.berkeley.edu/ICMC99/papers/msp+sdif/icmc99-msp+sdif-short.pdf
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06,07
DRUPAL_NODE: /patch/4010
SVN_REVISION: $LastChangedRevision$
VERSION 0.2: includes reltime
VERSION 0.3: max_rows, only one complaint if empty
VERSION 0.3.1: "tuples time" now accepts int or float
VERSION 0.3.2: Does the right thing if no SDIF buffers exist at all
VERSION 0.4: uses -(DBL_MAX) instead of -9999999999999.9
VERSION 0.4b: compiled with CW 7.0
VERSION 0.5.0: added interpolation support (bj, 2004/04/01)
VERSION 0.5.1: cleanup (bj, 2004/06/22)
VERSION 0.5.2: Made an "interp" message to set interpolation mode like the other parameters (mw, 12/30/04)
VERSION 0.5.3: Made integers work (non-interpolating)
VERSION 0.6: Uses new version system
VERSION 0.6.1: Force Package Info Generation
VERSION 1.0: Controllable by a virtual time signal input. tellmeeverything. Better help patch.
VERSION 1.1: Works for Max 5.
VERSION 2.0: Common code has been forked out.
VERSION 2.0.1: Added support for obex and attributes
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 Todo:
    Make "matrix" able to be part of the tuples message or a typed-in argument
    Don't clone a perfectly good matrix in the non-interpolating case just so you can free it later
    more interpolation features
    More logical error reporting in Max window
*/


// make sure not to get ../SDIF-buffer/version.h
#include "./version.h" 

#define MAX_NUM_COLUMNS 100
#define BIGGEST_OUTPUT_LIST 3000

#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "ext.h"
#include "ext_obex.h"
#include "commonsyms.h"

/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf


#include "version.c"

#include "CNMAT_MMJ_SDIF.h"

#include <z_dsp.h>

/* private types */

#ifdef WIN_VERSION
#ifdef STUCK_WITH_MICROSOFT_COMPILER
#include <z_dsp.h>
extern int isnan(double d) {
	return IS_NAN_DOUBLE(d);
}
#endif
#endif

/* My class definiton */

typedef struct _SDIFtuples {
	t_pxobject t_ob;
	void *obex;
	CNMAT_MMJ_SDIF_buffer t_b;
 	Boolean t_errorreporting;
 	Boolean t_complainedAboutEmptyBufferAlready;
 	void *t_out;
	SDIFinterp_Interpolator t_it;   //  interpolator matrix
	Boolean t_itValid;              //  is interpolator matrix still valid?

	/* State for signal control */
	float t_output_interval;
	int t_output_interval_samps;
	int t_samps_until_output;
	float t_sr;
 	
 	/* State for what I'm supposed to output */
 	long t_concatenate;		/* One big list or one list per row? */
 	sdif_float64 t_time;	/* Which frame? */
 	Boolean t_reltime;			/* if nonzero, t_time is from 0 to 1 and should be scaled by length */
 	int t_direction;		/* Frame in which direction from t_time? */
 	int t_num_columns;		/* How many cols to output?  If 0, all of them */
 	int t_columns[MAX_NUM_COLUMNS];  /* Which ones? (1-based) */
 	InterpMode t_interp;       /* Interpolation mode */
	// Getting rid of "main matrix mode".  Now, when the user calles the matrix function with no argument, 
	// it simply finds the "main matrix" and puts that type in the t_matrixType field.
 	//Boolean t_mainMatrix;	  /* If true, output the main matrix in the frame */
 	char t_matrixType[4];   /* Type of matrix to output */
	char t_matrixTypeCount; /* this is necessary for obex */
	Boolean t_mainMatrix;
 	int t_max_rows;			/* Max # of rows to output */
} SDIFtuples;


static Symbol *ps_emptysymbol, *ps_concatenate, *ps_time, *ps_reltime, *ps_direction, *ps_columns, *ps_interp, *ps_max_rows;


/* global that holds the class definition */
void *SDIFtuples_class;


/* prototypes for my functions */
void *SDIFtuples_new(Symbol *s, short argc, Atom *argv);
void SDIFtuples_free(SDIFtuples *x);
//void SDIFtuples_outputinterval(SDIFtuples *x, Symbol *dummy, short argc, t_atom *argv);
void SDIFtuples_dsp(SDIFtuples *x, t_signal **sp, short *count);
t_int *SDIFbuffer_perform(t_int *w);
static void SDIFtuples_set(SDIFtuples *x, Symbol *bufName);
static void SDIFtuples_errorreporting(SDIFtuples *x, long yesno);
static void SDIFtuples_time(SDIFtuples *x, double t);
/*
static void SDIFtuples_concatenate(SDIFtuples *x, long yesno);
static void SDIFtuples_reltime(SDIFtuples *x, double t);
static void SDIFtuples_direction(SDIFtuples *x, long d);
*/
static void SDIFtuples_columns(SDIFtuples *x, void *s, short argc, Atom *argv);

/*
static void SDIFtuples_max_rows(SDIFtuples *x, long n);
static void SDIFtuples_interptype(SDIFtuples *x, long interptype);
*/
void SDIFtuples_matrix_attr_set(SDIFtuples *x, void *attr, long argc, t_atom *argv);
static void SDIFtuples_matrix(SDIFtuples *x, Symbol *matrixType);
static void SDIFtuples_tuples(SDIFtuples *x, Symbol *s, short argc, Atom *argv);
void SDIFtuples_print(SDIFtuples *x);
void SDIFtuples_tellmeeverything(SDIFtuples *x);
void SDIFtuples_reportError(SDIFresult r, const char *st);

int main(int dummy, char **dummy2) {
	SDIFresult r;
  
	version(0);
	
	/* tell Max about my class. The cast to short is important for 68K */
	//setup((t_messlist **)&SDIFtuples_class, (method)SDIFtuples_new, (method)SDIFtuples_free,
	//(short)sizeof(SDIFtuples), 0L, A_GIMME, 0);
	t_class *c;
	t_object *attr;
	long attrflags = 0;

	c = class_new("SDIF-tuples", (method)SDIFtuples_new, (method)SDIFtuples_free, sizeof(SDIFtuples),
			       (method)0L, A_GIMME, 0);

	class_obexoffset_set(c, calcoffset(SDIFtuples, obex));
	common_symbols_init();
	
	/* bind my methods to symbols */
	class_addmethod(c, (method)version, "version", 0);
	class_addmethod(c, (method)SDIFtuples_dsp, "dsp", A_CANT, 0);

	//dsp_initclass();
	class_dspinit(c);

	//class_addmethod(c, (method)SDIFtuples_outputinterval, "outputinterval", A_GIMME, 0);
	class_addmethod(c, (method)SDIFtuples_set, "set", A_SYM, 0);	
	class_addmethod(c, (method)SDIFtuples_errorreporting, "errorreporting", A_LONG, 0);
	class_addmethod(c, (method)SDIFtuples_print, "print", 0);
	class_addmethod(c, (method)SDIFtuples_tellmeeverything, "tellmeeverything", 0L);
	//class_addmethod(c, (method)SDIFtuples_concatenate, "concatenate", A_LONG, 0);
	class_addmethod(c, (method)SDIFtuples_time, "time", A_FLOAT, 0);
	//class_addmethod(c, (method)SDIFtuples_reltime, "reltime", A_FLOAT, 0);
	//class_addmethod(c, (method)SDIFtuples_direction, "direction", A_LONG, 0);
	//class_addmethod(c, (method)SDIFtuples_columns, "columns", A_GIMME, 0);
	//class_addmethod(c, (method)SDIFtuples_max_rows, "max_rows", A_LONG, 0);
	//class_addmethod(c, (method)SDIFtuples_interptype, "interp", A_LONG, 0);	
	class_addmethod(c, (method)SDIFtuples_tuples, "tuples", A_GIMME, 0);
	//class_addmethod(c, (method)SDIFtuples_matrix, "matrix", A_DEFSYM, 0);

	class_addmethod(c, (method)object_obex_dumpout, "dumpout", A_CANT, 0); 
	class_addmethod(c, (method)object_obex_quickref, "quickref", A_CANT, 0); 

	attr = attr_offset_new("buffer_name", _sym_symbol, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_b.t_bufferSym));
	class_addattr(c, attr);
	attr = attr_offset_new("reltime", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_reltime));
	class_addattr(c, attr);
	attr = attr_offset_array_new("matrix", 
				     _sym_char, 
				     4, 
				     attrflags, 
				     (method)0L, 
				     (method)SDIFtuples_matrix_attr_set,
				     calcoffset(SDIFtuples, t_matrixTypeCount), 
				     calcoffset(SDIFtuples, t_matrixType));
	//attr = attr_offset_new("matrix", _sym_symbol, attrflags, (method)0L, (method)SDIFtuples_matrix_attr_set, calcoffset(SDIFtuples, t_matrixType));
	class_addattr(c, attr);
	attr = attr_offset_new("outputinterval", _sym_float32, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_output_interval));
	class_addattr(c, attr);
	attr = attr_offset_new("direction", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_direction));
	class_addattr(c, attr);
	attr = attr_offset_array_new("columns", 
				     _sym_long, 				     
				     MAX_NUM_COLUMNS, 
				     attrflags, 
				     (method)0L, 
				     (method)SDIFtuples_columns,
				     calcoffset(SDIFtuples, t_num_columns), 
				     calcoffset(SDIFtuples, t_columns));
	class_addattr(c, attr);
	attr = attr_offset_new("max_rows", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_max_rows));
	class_addattr(c, attr);
	attr = attr_offset_new("interp", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_interp));
	class_addattr(c, attr);
	attr = attr_offset_new("concatenate", _sym_long, attrflags, (method)0L, (method)0L, calcoffset(SDIFtuples, t_concatenate));
	class_addattr(c, attr);

	class_register(CLASS_BOX, c);
	SDIFtuples_class = c;

	if(CNMAT_MMJ_SDIF_init(SDIFtuples_reportError) != ESDIF_SUCCESS) return 0;

	ps_emptysymbol  = gensym("");
	ps_concatenate = gensym("concatenate");
	ps_time = gensym("time");
	ps_reltime = gensym("reltime");
	ps_direction = gensym("direction");
	ps_columns = gensym("columns");
	ps_interp = gensym("interp");
	ps_max_rows = gensym("max_rows");

	return 0;
}

void *SDIFtuples_new(Symbol *dummy, short argc, Atom *argv) {
	SDIFtuples *x = NULL;
	int i;
	
	// post("SDIFtuples_new: %s, %ld args", s->s_name, (long) argc);
	
	//x = newobject(SDIFtuples_class);
	if(x = (SDIFtuples *)object_alloc(SDIFtuples_class)){
		dsp_setup((t_pxobject *)x,1); // One signal input
		object_obex_store(x, _sym_dumpout, outlet_new(x, NULL)); // right-most (dump) outlet
		x->t_out = listout(x); // main outlet
	
		if (argc >= 1) {
			// First argument is name of SDIF-buffer
			if (argv[0].a_type != A_SYM) {
				error("SDIF-tuples: argument must be name of an SDIF-buffer");
			} else {
				// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
				x->t_b.t_bufferSym = argv[0].a_w.w_sym;
			}
		}

		x->t_errorreporting = FALSE;
		x->t_complainedAboutEmptyBufferAlready = FALSE;
		x->t_b.t_buffer = 0;
		x->t_b.t_buf = NULL;
		x->t_it = NULL;
		x->t_itValid = FALSE;

		x->t_matrixTypeCount = 4;
		x->t_matrixType[0] = '1';
		x->t_matrixType[1] = 'T';
		x->t_matrixType[2] = 'R';
		x->t_matrixType[3] = 'C';
		x->t_mainMatrix = TRUE;
		x->t_concatenate = TRUE;
		x->t_time = VERY_SMALL;
		x->t_reltime = FALSE;
		x->t_direction = 1;
		x->t_interp = INTERP_MODE_NONE;
		x->t_max_rows = 1000000;
	
		x->t_num_columns = 0;
		for (i = 0; i < MAX_NUM_COLUMNS; ++i) {
			x->t_columns[i] = i;
		}

		x->t_output_interval = 10.0f; // Default 10ms

		attr_args_process(x, argc, argv);

	}
	return x;


}

void SDIFtuples_free(SDIFtuples *x)
{
	if(x->t_it)
		SDIFinterp_Free(x->t_it);
	dsp_free(&(x->t_ob));
}

static void SDIFtuples_set(SDIFtuples *x, Symbol *bufName) {
	x->t_b.t_buffer = 0;
	x->t_b.t_bufferSym = bufName;

	LookupMyBuffer(&(x->t_b));
	if (x->t_b.t_buffer == 0) {
		error("SDIF-tuples: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
	x->t_complainedAboutEmptyBufferAlready = FALSE;

}


static void SDIFtuples_time(SDIFtuples *x, double t) {
	x->t_time = (sdif_float64) t;
	x->t_reltime = FALSE;
}

static void SDIFtuples_errorreporting(SDIFtuples *x, long yesno) {
	x->t_errorreporting = yesno;
	verbose_error_reporting(TRUE);
}
/*
static void SDIFtuples_concatenate(SDIFtuples *x, long yesno) {
	post("SDIFtuples_concatenate: %ld", yesno);
	x->t_concatenate = yesno;
}

static void SDIFtuples_reltime(SDIFtuples *x, double t) {
	x->t_time = (sdif_float64) t;
	x->t_reltime = TRUE;
}

static void SDIFtuples_direction(SDIFtuples *x, long d) {
	x->t_direction = d;
}

*/

static void SDIFtuples_columns(SDIFtuples *x, void *dummy, short argc, Atom *argv) {
	int i;

	//First make sure all arguments are OK 
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_LONG) {
			error("SDIF-tuples: all arguments to columns must be ints!");
			return;
		}
		if (argv[i].a_w.w_long <= 0) {
			error("SDIF-tuples: column number %d is no good.", argv[i].a_w.w_long);
			return;
		}
	}

	x->t_num_columns = argc;
	for (i = 0; i < argc; ++i) {
		x->t_columns[i] = argv[i].a_w.w_long;
	}
	

}

void SDIFtuples_matrix_attr_set(SDIFtuples *x, void *attr, long argc, t_atom *argv){
	if(argc == 0){
		SDIFtuples_matrix(x, ps_emptysymbol);
		return;
	}
	if(argv[0].a_type != A_SYM){
		error("SDIFtuples: Matrix type must be a 4 char string");
		return;
	}
	//t_symbol *matrixType = argv[0].a_w.w_sym;
	SDIFtuples_matrix(x, argv[0].a_w.w_sym);
}

static void SDIFtuples_matrix(SDIFtuples *x, Symbol *matrixType) {
	if (matrixType == ps_emptysymbol) {
		x->t_mainMatrix = TRUE;
		/*
		LookupMyBuffer(&(x->t_b));
		SDIFmem_Frame fr;
		fr = SDIFbuf_GetFirstFrame(x->t_b.t_buf);
		if (fr == NULL) {
			if (x->t_errorreporting) {
				error("SDIF-tuples: buffer %s is empty!", x->t_b.t_bufferSym->s_name);
			}
			return;
		}
		*/
  		//SDIF_Copy4Bytes(x->t_matrixType, fr->header.frameType);
		char *mt = CNMAT_MMJ_SDIF_getMainMatrixType(&(x->t_b));
  		SDIF_Copy4Bytes(x->t_matrixType, mt);
	} else {		
		//Here's where I could implement user matrix names that aren't just the 4 bytes. 
		x->t_mainMatrix = FALSE;
		if (strlen(matrixType->s_name) != 4) {
			error("SDIF-tuples: matrix %s is not 4 letters!", matrixType->s_name);
			return;
		}
		SDIF_Copy4Bytes(x->t_matrixType, matrixType->s_name);
	}
}

static void SDIFtuples_max_rows(SDIFtuples *x, long n) {
	x->t_max_rows = n;
}

static void SDIFtuples_interptype(SDIFtuples *x, long interptype) {
	x->t_interp = interptype;
}


static void SetAtomFromMatrix(Atom *a, SDIFmem_Matrix m, sdif_int32 column, sdif_int32 row) {
	if (m->header.matrixDataType == SDIF_INT32) {
		SETLONG(a, SDIFutil_GetMatrixCell_int32(m, column, row));
	} else {
		SETFLOAT(a, SDIFutil_GetMatrixCell(m, column, row));
	}
}


static void SDIFtuples_tuples(SDIFtuples *x, Symbol *dummy, short argc, Atom *argv) {
	Boolean concatenate;
	sdif_float64 time;
	Boolean reltime;
	int direction;
	InterpMode interp;
	int num_columns;
	int columns[MAX_NUM_COLUMNS];
	int max_rows;
	int i, j;
	Atom outputArgs[BIGGEST_OUTPUT_LIST];
	short numArgs;
	SDIFmem_Matrix m;
	//	char desiredType[4];  XXX need to parse "matrix" arguments to tuples too!
	
	
	/* First, store my remembered parameters locally */
	concatenate = x->t_concatenate;
	time = x->t_time;
	reltime = x->t_reltime;
	direction = x->t_direction;
	interp = x->t_interp;
	num_columns = x->t_num_columns;
	for (i = 0; i < num_columns; ++i) {
 		columns[i] = x->t_columns[i];
 	}
 	max_rows = x->t_max_rows;
 	
	/* Now parse my arguments to replace these values */

	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_SYM) {
			error("SDIF-tuples: bad arguments");
			return;
		}
		
		if (argv[i].a_w.w_sym == ps_concatenate) {
			if (i+1 < argc && argv[i+1].a_type == A_LONG) {
				concatenate = argv[i+1].a_w.w_long;
				++i;
			} else {
				error("SDIF-tuples: bad argument after concatenate");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_interp) {
			if (i+1 >= argc) {
				error("SDIF-tuples needs an argument after \"interp\" to specify interpolation type. ");
				return;
			} 

			if (argv[i+1].a_type == A_LONG) {
				int arg = argv[i+1].a_w.w_long;
				if(arg >= 0 && arg < INTERP_MODE_END)
					interp = arg;
				else {
					error("SDIF-tuples: unknown interp mode specified (%d)", arg);
					return;
				}
				++i;
			} else {
				error("SDIF-tuples: bad argument after interp");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_time) {
			if (i+1 >= argc) {
				error("SDIF-tuples: need an argument after time");
				return;
			} else if (argv[i+1].a_type == A_FLOAT) {
				time = (sdif_float64) argv[i+1].a_w.w_float;
				reltime = FALSE;
				++i;
			} else if (argv[i+1].a_type == A_LONG) {
				time = (sdif_float64) argv[i+1].a_w.w_long;
				reltime = FALSE;
				++i;
			} else {
				error("SDIF-tuples: bad argument after time");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_reltime) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_FLOAT)) {
				time = (sdif_float64) argv[i+1].a_w.w_float;
				reltime = TRUE;
				++i;
			} else {
				error("SDIF-tuples: bad argument after reltime");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_direction) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_LONG)) {
				direction = argv[i+1].a_w.w_long;
				++i;
			} else {
				error("SDIF-tuples: bad argument after direction");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_max_rows) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_LONG)) {
				max_rows = argv[i+1].a_w.w_long;
				++i;
			} else {
				error("SDIF-tuples: bad argument after max_rows");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_columns) {
			++i;
			for (j = 0; i < argc && argv[i].a_type == A_LONG; ++j, ++i) {
				columns[j] = argv[i].a_w.w_long;
			}
			--i; // What we finally found wasn't a column number
			num_columns = j;
			if (num_columns == 0) {
				error("SDIF-tuples: need at least one column!");
				return;
			}	
		} else {
			error("SDIF-tuples: unrecognized argument %s", argv[i].a_w.w_sym->s_name);
			return;
		}
	}
		
	LookupMyBuffer(&(x->t_b));
	
	if (x->t_b.t_buffer == 0) {
		error("SDIF-tuples: no buffer!");
		return;
	}
	
	if (reltime) {
		sdif_float64 tMax;
	  
		SDIFbuf_GetMaxTime(x->t_b.t_buf, &tMax);
		time *= tMax;
	}


	//  get the matrix (perform interpolation if requested)
	if(x->t_mainMatrix) SDIFtuples_matrix(x, ps_emptysymbol);

	if(x->t_interp == INTERP_MODE_NONE){
		if(!(m = GetMatrix(&(x->t_b), x->t_matrixType, time, direction)))
			return;
	}else{
		if(!(m = GetMatrixWithInterpolation(&(x->t_b), 
						    x->t_matrixType, 
						    time, 
						    columns, 
						    num_columns, 
						    x->t_interp, 
						    &(x->t_it), 
						    &(x->t_itValid))))
			return;
	}
	
	numArgs = 0;
	for (i = 0; (i < m->header.rowCount) && (i < max_rows); i++) {
		if (num_columns == 0) {
			/* Output all the columns */
			if (numArgs + m->header.columnCount >= BIGGEST_OUTPUT_LIST) {
				error("SDIF-tuples: output list too big.");
				break;
			}
			for (j = 0; j < m->header.columnCount; ++j) {
				SetAtomFromMatrix(&(outputArgs[numArgs++]), m, j, i);
			}
		} else {
			/* Output the desired columns */
			if (numArgs + num_columns >= BIGGEST_OUTPUT_LIST) {
				error("SDIF-tuples: output list too big.");
				break;
			}
			for (j = 0; j<num_columns; ++j) {
				/* User's column numbers are 1-based, so sub 1 for array lookup: */
				SetAtomFromMatrix(&(outputArgs[numArgs++]), m, columns[j] - 1, i);
			}
		}
		if (!concatenate) {
			outlet_list(x->t_out, 0L, numArgs, outputArgs);
			numArgs = 0;
		}
	}
	
	if (numArgs > 0) {
		outlet_list(x->t_out, 0L, numArgs, outputArgs);
	}
	
	//  release matrix memory
	SDIFmem_FreeMatrix(m);
}

/* Signal-rate control ********************/

void SDIFtuples_outputinterval(SDIFtuples *x, Symbol *dummy, short argc, t_atom *argv) {
	if (argc != 1) {
		error("SDIF-tuples: outputinterval should have exactly one argument (you passed %ld)",
		      argc);
	} else if (argv[0].a_type == A_LONG) {
		x->t_output_interval = (float) argv[0].a_w.w_long;
	} else if (argv[0].a_type == A_FLOAT) {
		x->t_output_interval = argv[0].a_w.w_float;    
	} else {
		error("SDIF-tuples: outputinterval's argument must be a number.");
	}

	x->t_samps_until_output = x->t_output_interval_samps = 
		(int) (x->t_sr * x->t_output_interval * 0.001f);
}

void SDIFtuples_dsp(SDIFtuples *x, t_signal **sp, short *count) {
	x->t_sr = sp[0]->s_sr;
	x->t_samps_until_output = x->t_output_interval_samps = 
		(int) (x->t_sr * x->t_output_interval * 0.001f);

	if (count[0]) {
		// Signal inlet is connected
		dsp_add(SDIFbuffer_perform, 3, x, sp[0]->s_n, sp[0]->s_vec);
	} else {
		// Don't add anything
	}
}

t_int *SDIFbuffer_perform(t_int *w) {
	SDIFtuples *x = (SDIFtuples *)(w[1]);  // object
	int size = w[2]; // vector size
	t_float *vtime_in = (t_float *)(w[3]);

	if (x->t_samps_until_output >= size) {
		x->t_samps_until_output -= size;
	} else {
		x->t_time = (sdif_float64) vtime_in[x->t_samps_until_output];
		SDIFtuples_tuples(x, 0, 0, 0);
		x->t_samps_until_output = x->t_output_interval_samps;
	}

	return w+4;
}

void SDIFtuples_tellmeeverything(SDIFtuples *x) {
	int i;

	post("SDIF-tuples object:");
	post(" buffer %s", x->t_b.t_bufferSym->s_name);
	post(" this object %s errors", x->t_errorreporting ? "reports" : "does not report");
	post(" input signal (if any) is sampled every %f ms (%ld samples)",
	     x->t_output_interval, x->t_output_interval_samps);
	post("Current output settings:");

	if (x->t_mainMatrix) {
		post(" output the \"main\" matrix (i.e., the one whose type is the same as the frame type)");
	} else {
		post(" output the %c%c%c%c matrix",
		     x->t_matrixType[0], x->t_matrixType[1], x->t_matrixType[2], x->t_matrixType[3]);
	}

	post(" output no more than %ld rows", x->t_max_rows);

	post(" output %ld columns:", x->t_num_columns);
	for (i=0; i<x->t_num_columns; ++i) {
		post("   %ld", x->t_columns[i]);
	}

	if (x->t_concatenate) {
		post(" concatenate 1: combine all rows into a single output list");
	} else {
		post(" concatenate 0: Output each row as a separate output list");
	}
	// There's a bug with this function 9/2008
	//object_attr_getdump(x, gensym("concatenate"), NULL, NULL);
			 
	post(" %s time: %f", x->t_reltime ? "Relative" : "Absolute",
	     (float) x->t_time);

	switch (x->t_interp) {
	case INTERP_MODE_NONE: 
		post(" No time axis interpolation. So you'll get one of the frames from the SDIF file:");
		if (x->t_direction < 0) {
			post("  direction -1: search backwards from requested time");
		} else if (x->t_direction == 0) {
			post("  direction 0: Output nothing unless the SDIF file has a frame exactly at the requested time");
		} else {
			post("  direction 1: search forwards from requested time");
		}
		break;
	case INTERP_MODE_LINEAR: 
		post(" Linear interpolation along the time axis.");
		break;
	case INTERP_MODE_LAGRANGE2:
		post(" Lagrange interpolation (degree 2)");
		break;
	case INTERP_MODE_LAGRANGE3:
		post(" Lagrange interpolation (degree 3)");
		break;
	default:
		post(" Unrecognized interpolation mode!  (This is a bug.)");
		break;
	}
}


/* Printing stuff ***************************/

void SDIFtuples_print(SDIFtuples *x) {
	SDIFmem_Frame f;
	
	if (x->t_b.t_bufferSym == 0) {
		post("SDIFtuples: no SDIF buffer name specified");
	} else {
		LookupMyBuffer(&(x->t_b));
		if (x->t_b.t_buffer == 0) {
			error("SDIFtuples: \"%s\" is not an SDIF buffer.", x->t_b.t_bufferSym->s_name);
		} else {
		  sdif_float64 tMin, tMax;
		  
			// post("* ob_sym(x->t_b.t_buffer)->s_name: %s", ob_sym(x->t_b.t_buffer)->s_name);
			
			post("SDIFtuples: SDIF-buffer \"%s\"", x->t_b.t_buffer->s_myname->s_name);
			if(f = SDIFbuf_GetFirstFrame(x->t_b.t_buf)) {
	  			post("   Stream ID %ld, Frame Type %c%c%c%c", x->t_b.t_buffer->streamID,
  					f->header.frameType[0], f->header.frameType[1], f->header.frameType[2], f->header.frameType[3]);
				SDIFbuf_GetMinTime(x->t_b.t_buf, &tMin);
				SDIFbuf_GetMaxTime(x->t_b.t_buf, &tMax);
				post("   Min time %g, Max time %g", tMin, tMax);
			
				// post("Calling FrameLookup(VERY_SMALL, 1)");
				// f = (*(x->t_buffer->FrameLookup))(x->t_buffer, (sdif_float64) VERY_SMALL, 1);
				// post("It returned %p", f);
			
				for (/* f already set */; f != NULL; f = f->next) {
					PrintOneFrame(f);
				}
			} else {
				post("No frames.");
			}
		}
	}
}

void SDIFtuples_reportError(SDIFresult r, const char *st){
	if(r) error("SDIFtuples: %s: %s", st, SDIF_GetErrorString(r));
	else error("SDIFtuples: %s", st);
}

