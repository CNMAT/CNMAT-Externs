// Copyright (c) 2004. The Regents of the
// University of California (Regents). All Rights Reserved.

// Permission to use, copy, modify, and distribute this software and its
// documentation for educational, research, and not-for-profit purposes,
// without fee and without a signed licensing agreement, is hereby
// granted, provided that the above copyright notice, this paragraph and
// the following two paragraphs appear in all copies, modifications, and
// distributions. Contact The Office of Technology Licensing, UC
// Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
// (510) 643-7201, for commercial licensing opportunities.
// Created by Peter Kassakian, Department of Electrical Engineering and 
// Computer Science, University of California, Berkeley.  

// IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
// SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
// ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
// REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

// REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".  REGENTS HAS NO
// OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
// MODIFICATIONS.

/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: matlabcommunicate
DESCRIPTION: Bridge between Max/MSP and Matlab using the Matlab Engine
AUTHORS: Peter Kassakian, Matt Wright
COPYRIGHT_YEARS: 2005
VERSION 1.0: Initial hacking by Matt
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "z_dsp.h"
#include "engine.h"

#define MAXORDER 1000
#define STRING_CAPACITY 1000
#define BIGGEST_LIST 5000
#define Z_NO_INPLACE 1

void *theobject_class;

typedef struct _theobject
{
  t_pxobject x_obj;
  float x_array[MAXORDER];  // This shouldn't be here.
  void *x_outlet;
  float x_result;
  Engine *x_engine;
  char matlabTextOutput[STRING_CAPACITY+1];
  int engineOpen;
  int verbose;
} t_theobject;

Symbol *ps_ssh;


void *theobject_new(Symbol *s, short argc, Atom *argv);
t_int *theobject_perform(t_int *w); 
void theobject_dsp(t_theobject *x, t_signal **sp, short *count);
void theobject_sum(t_theobject *x);
void theobject_get(t_theobject *x, Symbol *variable);
void theobject_eval(t_theobject *x, t_symbol *message, short argc, t_atom *argv);
void theobject_clear(t_theobject *x, t_symbol *message, short argc, t_atom *argv);
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv);
void theobject_free(t_theobject *x);
void theobject_closeEngine(t_theobject *x);
static int MaxListToString(char *s, int capacity, short argc, t_atom *argv);
void theobject_verbose(t_theobject *x, long yesno);
void matlabbridge_version (t_theobject *x);

void shoehorn(t_theobject *x, const mxArray *array_ptr);
static void analyze_int8(t_theobject *x, const mxArray *array_ptr);
static void analyze_uint8(t_theobject *x, const mxArray *array_ptr);
static void analyze_int16(t_theobject *x, const mxArray *array_ptr);
static void analyze_uint16(t_theobject *x, const mxArray *array_ptr);
static void analyze_int32(t_theobject *x, const mxArray *array_ptr);
static void analyze_uint32(t_theobject *x, const mxArray *array_ptr);
static void analyze_single(t_theobject *x, const mxArray *array_ptr);
static void analyze_double(t_theobject *x, const mxArray *array_ptr);


void main(void)
{
  setup((t_messlist **)&theobject_class, (method)theobject_new, (method)theobject_free, (short)sizeof(t_theobject), 0L, A_GIMME, 0);
  // dsp_initclass();
  // addmess((method)theobject_dsp, "dsp", A_CANT, 0);
  addmess((method)theobject_clear,"clear", A_GIMME, 0);
  addmess((method)theobject_sum,"sum", 0);
  addmess((method)theobject_closeEngine,"closeEngine", 0);
  addmess((method)theobject_get, "get", A_SYM, 0);
  addmess((method)theobject_eval, "eval", A_GIMME, 0);
  // addbang((method)theobject_bang);
  addmess((method)theobject_list, "list", A_GIMME, 0);
  addmess((method)theobject_verbose, "verbose", A_LONG, 0);
  addmess((method)matlabbridge_version, "version", 0);
  
  post(NAME " object version " VERSION " by " AUTHORS ".");
  post("Copyright ¨ " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
  
  ps_ssh = gensym("ssh");
}

t_int *theobject_perform(t_int *w)
{
  t_float *in1, *out1;
  t_theobject *x;
  long i, N;
  
  in1 = (t_float *)(w[1]);
  out1 = (t_float *)(w[2]);
  N = (int)(w[3]);
  x = (t_theobject *)(w[4]);

  for (i=0; i<N; i++) {
    out1[i] = in1[i];
  }
  return (w+5);
}

void theobject_dsp(t_theobject *x, t_signal **sp, short *count)
{		
  dsp_add(theobject_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n, x);
}

void theobject_sum(t_theobject *x) {
  char s[STRING_CAPACITY];
  mxArray *result;
  double *datptr ;

  if (!x->engineOpen || x->x_engine == NULL) {
	error("matlabcommunicate: sum: Matlab engine is not open.");
	return;
  }

  strcpy(s, "y=sum(x);");

  if (x->verbose) {
	post(s);
  }
  engEvalString(x->x_engine, s);
  result = engGetVariable(x->x_engine, "y");
  datptr = mxGetPr(result);
  x->x_result = *datptr;
  mxDestroyArray(result);
  outlet_float(x->x_outlet, x->x_result);
}


void theobject_get(t_theobject *x, Symbol *variable) {
	if (x->verbose) post("%s = ");
}


void theobject_eval(t_theobject *x, t_symbol *message, short argc, t_atom *argv) {
  char s[STRING_CAPACITY];
  mxArray *ans;
  
  if (!x->engineOpen || x->x_engine == NULL) {
	error("matlabcommunicate: eval: Matlab engine is not open.");
	return;
  }
  
  if (MaxListToString(s, STRING_CAPACITY, argc, argv) == 0) {
	error("matlabcommunicate: Couldn't make string to be evaluated.");
  } else {
    if (x->verbose) post("to Matlab: \"%s\"", s);
  }

  engEvalString(x->x_engine, s);
  // Skip ">>\n" that Matlab prints to stdout
  if (x->verbose) post ("%s", x->matlabTextOutput+4);

  if (strncmp(x->matlabTextOutput+4, "ans =", 5) == 0) {
	  if ((ans = engGetVariable(x->x_engine,"ans")) == NULL)
		   error("Matlab printed \"ans =\", but there is no variable ans.  (?)");
	  else {
		  // post("ans is class %s", mxGetClassName(ans));
		  shoehorn(x, ans);
		  mxDestroyArray(ans);
	  }
  }
}
    
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv) {
  int i;
  char s[STRING_CAPACITY], t[STRING_CAPACITY];
  
  
  if (!x->engineOpen || x->x_engine == NULL) {
	error("matlabcommunicate: list: Matlab engine is not open.");
	return;
  }
 
  for (i=0; i<argc; i++) {
	    if (argv[i].a_type == A_LONG) {
			x->x_array[i] = (float) argv[i].a_w.w_long;
		} else if (argv[i].a_type == A_FLOAT) {
			x->x_array[i] = argv[i].a_w.w_float;
		} else {
			post("¥ matlabcommunicate: ignoring nonnumeric list element.");
			x->x_array[i] = 0.0f;
		}
  }
 
  s[0] = '\0';
  strcpy(s,"x = [");
  for (i=0; i<argc-1; i++){
    sprintf(t, "%f, ", x->x_array[i]);
    strcat(s, t);
  }
  sprintf(t, "%f", x->x_array[argc-1]);
  strcat(s, t);
  strcpy(t, "];");
  strcat(s, t);
  engEvalString(x->x_engine, s);
  if (x->verbose) post(s);
}

void theobject_clear(t_theobject *x, t_symbol *message, short argc, t_atom *argv)
{
  int i;
  for (i=0; i<MAXORDER; i++){
    x->x_array[i] = 0;
  } 
}




#if 0
  /* Some bad old code for ssh */
  if ((argc > 0)  && argv[0].a_type == A_SYM && argv[0].a_w.w_sym == ps_ssh) {
	post("You want ssh mode.");
	
	if (argc == 1) {
		error("matlabcommunicate: ssh argument needs a subsequent host name argument.");
		return 0;
	}

	if (argv[1].a_type != A_SYM) {
		error("matlabcommunicate: bad hostname after ssh argument");
		return 0;
	}

	sprintf(s, "ssh %s \"/bin/csh -c 'setenv DISPLAY %s:0; matlab'\"", 
			argv[1].a_w.w_sym->s_name, argv[1].a_w.w_sym->s_name);
#endif  



void *theobject_new(Symbol *symb, short argc, Atom *argv) {
  int i;
  char s[STRING_CAPACITY];
    
  t_theobject *x = (t_theobject *)newobject(theobject_class);
  x->x_engine = NULL;
  x->engineOpen = 0;
  
  // dsp_setup((t_pxobject *)x,1);                 // The input signal
  // x->x_obj.z_misc |= Z_NO_INPLACE; 
  
  x->x_outlet = outlet_new((t_object *)x, 0L);
  // outlet_new((t_object *)x, "signal");        // A signal outlet

  x->x_result = 0;
  for (i=0; i<MAXORDER; i++){
    x->x_array[i] = 0;
  }
  
  
  if (MaxListToString(s, STRING_CAPACITY, argc, argv) == 0) {
      post("Couldn't convert arguments to string; ");
	  post("will attempt to start Matlab Engine normally");
	  s[0] = '\0';
  } else if (argc == 0) {
    post("Trying to start Matlab Engine normally");
  } else {
	post("Here's how I'm going to try to start the Matlab Engine:");
	post(" %s", s);	
  }

  /* Open the matlab engine */
  if ((x->x_engine = engOpen(s))==0) {
    post("Couldn't load matlab");
  } else {
	post("Matlab engine loaded successfully");
	x->engineOpen = 1;

    x->matlabTextOutput[STRING_CAPACITY] = '\0';
    engOutputBuffer(x->x_engine, x->matlabTextOutput, STRING_CAPACITY);
  }

  x->verbose = 1;
  return (x);
}

void theobject_free(t_theobject *x) {
  // dsp_free(&x->x_obj);

  theobject_closeEngine(x);
}


void theobject_closeEngine(t_theobject *x) {
  if (!x->engineOpen) {
	post("Engine already closed; can't close it again.");
	return;
  }
  
  if(engClose(x->x_engine)==0) {
	post("Matlab engine closed successfully");
	x->engineOpen = 0;
  } else {
	post("Failed to close Matlab engine.");
  }  
}


static int MaxListToString(char *s, int capacity, short argc, t_atom *argv) {
	int i, a, written, remain;
	i = 0;
	s[0] = '\0';
	
	for (a = 0; a<argc; ++a) {
		remain = capacity - i - 1;
		
		if (argv[a].a_type == A_SYM) {
			written = snprintf(s+i, remain, "%s ", argv[a].a_w.w_sym->s_name);
		} else if (argv[a].a_type == A_LONG) {
			written = snprintf(s+i, remain, "%ld ", argv[a].a_w.w_long);
		} else if (argv[a].a_type == A_FLOAT) {
			written = snprintf(s+i, remain, "%f ", argv[a].a_w.w_float);
		} else {
			error("matlabcommunicate: unrecognized Max data type %ld !", argv[i].a_type);
			return 0;
		}

		if (written >= remain) {
			post("¥ matlabcommunicate: warning: truncated string.");
			s[capacity-1] = '\0';
			return 0;
		}
        i += written;
	}
	return 1;
}


void theobject_verbose(t_theobject *x, long yesno) {
	x->verbose = yesno;
	post("matlabcommunicate: verbose %s", yesno ? "on" : "off");
}

void matlabbridge_version (t_theobject *x) {
  post(NAME " version " VERSION ", by " AUTHORS ". ");
  post("Compiled " __TIME__ " " __DATE__);
}



/* This code for dealing with an mxArray comes from the example provided by
Matlab in extern/examples/mex/explore.c, adapted for Max. */

/* shoehorn figures out the size, and category 
   of the input array_ptr, and then displays all this information. */ 
void shoehorn(t_theobject *x, const mxArray *array_ptr) {
  const char    *name;
  const char    *class_name;
  const int     *dims;
  char    *shape_string;
  char    *temp_string;
  int      c;
  int      number_of_dimensions; 
  int      length_of_shape_string;
  mxClassID  category;

  int scalar = 0, list = 0;
  
  /* Display the mxArray's Dimensions; for example, 5x7x3.  
     If the mxArray's Dimensions are too long to fit, then just
     display the number of dimensions; for example, 12-D. */ 
  number_of_dimensions = mxGetNumberOfDimensions(array_ptr);
  dims = mxGetDimensions(array_ptr);
  
  // post("%ld-dimensional array", number_of_dimensions);
  if (number_of_dimensions > 2) {
	post("sorry; Max can't handle more than 2 dimensions.");
	return;
  } else if (number_of_dimensions == 2) {
	if (dims[0] == 1 && dims[1] == 1) {
		// post("A \"two dimensional\" 1x1 matrix is really just a scalar");
		scalar = 1;
	} else if (dims[0] == 1 || dims[1] == 1) {
	    // post("A \"two dimensional\" %dx%d matrix is really just a list", dims[0], dims[1]);
		list = 1;
	} else {
		post("A %dx%d 2D array would fit in an SDIF matrix.", dims[0], dims[1]);
		error("Too bad that doesn't work yet.");
		return;
	}
  } else {
	/* 1D */
	if (dims[0] == 1) {
		scalar = 1;
	} else {
		list = 1;
	}
  }

  // if (list) post("can be interpreted as a Max list");
  // if (scalar) post("can be interpreted as a Max scalar");
  
  /* Display the mxArray's class (category). */
  class_name = mxGetClassName(array_ptr);
  // post("Class Name: %s%s\n", class_name, mxIsSparse(array_ptr) ? " (sparse)" : "");

  if (mxIsSparse(array_ptr)) {
	post("Sorry, can't handle sparse arrays yet.");
	return;
  }
  
  if (mxIsComplex(array_ptr)) {
	post("Sorry, can't handle complex arrays yet.");
	return;
  }

  category = mxGetClassID(array_ptr);
  switch (category) {
	  case mxCHAR_CLASS:    {
		post("sorry; can't handle Matlab strings yet.");
		return;
		// analyze_string(array_ptr);    
	  } break;
	  case mxSTRUCT_CLASS:  {
		post("sorry; can't handle Matlab structures yet.");
		return;
		// analyze_structure(array_ptr);    
	  } break;
	  case mxCELL_CLASS:    {
		post("sorry; can't handle Matlab cell arrays yet.");
		return;
		// analyze_cell(array_ptr);    
	  } break;
	  case mxUNKNOWN_CLASS: {
	    error("Matlab returned something of \"Unknown class\"; don't know what to do.");
		return;
	  } break;
	  default: {
		  /* A numeric array */
		  switch (category)  {
			 case mxINT8_CLASS:   analyze_int8(x, array_ptr);   break; 
			 case mxUINT8_CLASS:  analyze_uint8(x, array_ptr);  break;
			 case mxINT16_CLASS:  analyze_int16(x, array_ptr);  break;
			 case mxUINT16_CLASS: analyze_uint16(x, array_ptr); break;
			 case mxINT32_CLASS:  analyze_int32(x, array_ptr);  break;
			 case mxUINT32_CLASS: analyze_uint32(x, array_ptr); break;
			 case mxSINGLE_CLASS: analyze_single(x, array_ptr); break; 
			 case mxDOUBLE_CLASS: analyze_double(x, array_ptr); break;
			 default:
				error("Unrecognized numeric type has fallen through the cracks.");
				return;
		  }
	  } break;
	}
}


static void analyze_double(t_theobject *x, const mxArray *array_ptr) {
  double *pr, *pi; 
  int     total_num_of_elements, index; 
  Atom output[BIGGEST_LIST];
  float f;
  
  pr = mxGetPr(array_ptr);
  pi = mxGetPi(array_ptr);
  total_num_of_elements = mxGetNumberOfElements(array_ptr);
  
  if (total_num_of_elements > BIGGEST_LIST) {
	post("¥ " NAME ": warning: matlab returned %ld-element list; truncating to %ld",
		 total_num_of_elements, BIGGEST_LIST);
	total_num_of_elements = BIGGEST_LIST;
  }
  
  if (total_num_of_elements == 1) {
	f = (float) *pr;
	outlet_float(x->x_outlet, f);
  } else {
	  /* We flatten the list without too much care */
	  for (index=0; index<total_num_of_elements; index++)  {
		f = (float) *pr++;
		SETFLOAT(output+index, f);
	  }
	  outlet_list (x->x_outlet, 0L, total_num_of_elements, output);
  }
}



static void analyze_int8(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle int8 data yet.");
}

static void analyze_uint8(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle uint8 data yet.");
}

static void analyze_int16(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle int16 data yet.");
}

static void analyze_uint16(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle uint16 data yet.");
}

static void analyze_int32(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle int32 data yet.");
}

static void analyze_uint32(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle uint32 data yet.");
}

static void analyze_single(t_theobject *x, const mxArray *array_ptr) {
    post("sorry, can't handle float32 data yet.");
}


#if 0

/* Pass analyze_string a pointer to a char mxArray.  Each element
   in a char mxArray holds one 2-byte character (an mxChar); 
   analyze_string displays the contents of the input char mxArray
   one row at a time.  Since adjoining row elements are NOT stored in 
   successive indices, analyze_string has to do a bit of math to
   figure out where the next letter in a string is stored. */ 
static void
analyze_string(const mxArray *string_array_ptr)
{
  char *buf;
  int   number_of_dimensions; 
  const int  *dims;
  int   buflen, d, page, total_number_of_pages, elements_per_page;
  
  /* Allocate enough memory to hold the converted string. */ 
  buflen = mxGetNumberOfElements(string_array_ptr) + 1;
  buf = mxCalloc(buflen, sizeof(char));
  
  /* Copy the string data from string_array_ptr and place it into buf. */ 
  if (mxGetString(string_array_ptr, buf, buflen) != 0)
    mexErrMsgTxt("Could not convert string data.");
  
  /* Get the shape of the input mxArray. */
  dims = mxGetDimensions(string_array_ptr);
  number_of_dimensions = mxGetNumberOfDimensions(string_array_ptr);
  
  elements_per_page = dims[0] * dims[1];
  /* total_number_of_pages = dims[2] x dims[3] x ... x dims[N-1] */ 
  total_number_of_pages = 1;
  for (d=2; d<number_of_dimensions; d++) 
    total_number_of_pages *= dims[d];
  
  for (page=0; page < total_number_of_pages; page++) {
    int row;
    /* On each page, walk through each row. */ 
    for (row=0; row<dims[0]; row++)  {
      int column;     
      int index = (page * elements_per_page) + row;
      mexPrintf("\t");
      display_subscript(string_array_ptr, index);
      mexPrintf(" ");
      
      /* Walk along each column in the current row. */ 
      for (column=0; column<dims[1]; column++) {
	mexPrintf("%c",buf[index]);
	index += dims[0];
      }
      mexPrintf("\n");

    }
    
  } 
}
#endif
