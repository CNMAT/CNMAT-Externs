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


#include "ext.h"
#include "z_dsp.h"
#include "engine.h"

#define MAXORDER 1000
#define STRING_CAPACITY 1000
#define Z_NO_INPLACE 1

void *theobject_class;

typedef struct _theobject
{
  t_pxobject x_obj;
  float x_array[MAXORDER];
  int x_len;
  void *x_floatout;
  float x_result;
  Engine *x_engine;
  int verbose;
} t_theobject;

Symbol *ps_ssh;


void *theobject_new(Symbol *s, short argc, Atom *argv);
t_int *theobject_perform(t_int *w); 
void theobject_dsp(t_theobject *x, t_signal **sp, short *count);
void theobject_sum(t_theobject *x);
void theobject_convalot(t_theobject *x, long size);
void theobject_get(t_theobject *x, Symbol *variable);
void theobject_eval(t_theobject *x, t_symbol *message, short argc, t_atom *argv);
void theobject_clear(t_theobject *x, t_symbol *message, short argc, t_atom *argv);
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv);
void theobject_free(t_theobject *x);
static int MaxListToString(char *s, int capacity, short argc, t_atom *argv);
void theobject_verbose(t_theobject *x, long yesno);

void main(void)
{
  setup((t_messlist **)&theobject_class, (method)theobject_new, (method)theobject_free, (short)sizeof(t_theobject), 0L, A_GIMME, 0);
  dsp_initclass();
  addmess((method)theobject_dsp, "dsp", A_CANT, 0);
  addmess((method)theobject_clear,"clear", A_GIMME, 0);
  addmess((method)theobject_sum,"sum", 0);
  addmess((method)theobject_convalot, "convalot", A_LONG, 0);
  addmess((method)theobject_get, "get", A_SYM, 0);
  addmess((method)theobject_eval, "eval", A_GIMME, 0);
  // addbang((method)theobject_bang);
  addmess((method)theobject_list, "list", A_GIMME, 0);
  post("Matlabcommunicate main()");
  
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

  strcpy(s, "y=sum(x);");

  if (x->verbose) {
	post(s);
  }
  engEvalString(x->x_engine, s);
  result = engGetVariable(x->x_engine, "y");
  datptr = mxGetPr(result);
  x->x_result = *datptr;
  mxDestroyArray(result);
  outlet_float(x->x_floatout, x->x_result);
}


void theobject_get(t_theobject *x, Symbol *variable) {
	if (x->verbose) post("%s = ");
}


void theobject_convalot(t_theobject *x, long size) {
  int i;
  char s[STRING_CAPACITY], t[STRING_CAPACITY];
  mxArray *result;
  double *datptr ;

  sprintf(s, "convout=conv(x,[1:%ld]);", size);
  if (x->verbose) post(s);
  engEvalString(x->x_engine, s);

  sprintf(s, "result=convout(%ld);", size);
  if (x->verbose) post(s);
  engEvalString(x->x_engine, s);

  result = engGetVariable(x->x_engine, "result");
  datptr = mxGetPr(result);
  x->x_result = *datptr;
  mxDestroyArray(result);
  outlet_float(x->x_floatout, x->x_result);
}

void theobject_eval(t_theobject *x, t_symbol *message, short argc, t_atom *argv) {
  char s[STRING_CAPACITY];
  
  if (MaxListToString(s, STRING_CAPACITY, argc, argv) == 0) {
	error("matlabcommunicate: Couldn't make string to be evaluated.");
  } else {
    if (x->verbose) post("to Matlab: \"%s\"", s);
  }

  engEvalString(x->x_engine, s);  
}
    
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv) {
  int i;
  char s[STRING_CAPACITY], t[STRING_CAPACITY];
 
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
  x->x_len = argc;
 
   s[0] = '\0';
  strcpy(s,"x = [");
  for (i=0; i<x->x_len-1; i++){
    sprintf(t, "%f, ", x->x_array[i]);
    strcat(s, t);
  }
  sprintf(t, "%f", x->x_array[x->x_len-1]);
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


void *theobject_new(Symbol *s, short argc, Atom *argv) {
  int i;
  char s[STRING_CAPACITY];
  
  post("entering matlabcommunicate new()");

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
#if 0

	sprintf(s, "ssh %s \"/bin/csh -c 'setenv DISPLAY %s:0; matlab'\"", 
			argv[1].a_w.w_sym->s_name, argv[1].a_w.w_sym->s_name);
#endif  
	MaxListToString(s, STRING_CAPACITY, argc, argv);

	post("Here's how I'm going to try to start Matlab:");
	post("   %s", s);	
  } else {
  
    if (MaxListToString(s, STRING_CAPACITY, argc, argv) == 0) {
      post("Couldn't convert arguments to string.");
    } else {
	  post("Args as string: \"%s\"", s);
    }
  }
  
  t_theobject *x = (t_theobject *)newobject(theobject_class);
  dsp_setup((t_pxobject *)x,1);                 // The input signal
  x->x_obj.z_misc |= Z_NO_INPLACE; 
  x->x_floatout = outlet_new((t_object *)x, 0L);
  outlet_new((t_object *)x, "signal");        // A signal outlet

  x->x_len = 1;
  x->x_result = 0;
  for (i=0; i<MAXORDER; i++){
    x->x_array[i] = 0;
  }
  /* Open the matlab engine */
  if ((x->x_engine = engOpen(s))==0) {
    post("Couldn't load matlab");
  } else {
	post("Matlab engine loaded successfully");  
  }
  
  x->verbose = 1;
  return (x);
}

void theobject_free(t_theobject *x) {
  dsp_free(&x->x_obj);

  if(engClose(x->x_engine)==0) {
    post("Matlab engine closed successfully");
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
