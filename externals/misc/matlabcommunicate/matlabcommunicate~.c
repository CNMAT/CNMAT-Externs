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

#define MAXORDER 200
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
} t_theobject;

void *theobject_new(double val);
t_int *theobject_perform(t_int *w); 
void theobject_dsp(t_theobject *x, t_signal **sp, short *count);
void theobject_bang(t_theobject *x);
void theobject_clear(t_theobject *x, t_symbol *message, short argc, t_atom *argv);
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv);
void theobject_free(t_theobject *x);

void main(void)
{
  setup((t_messlist **)&theobject_class, (method)theobject_new, (method)theobject_free, (short)sizeof(t_theobject), 0L, A_DEFFLOAT, 0);
  dsp_initclass();
  addmess((method)theobject_dsp, "dsp", A_CANT, 0);
  addmess((method)theobject_clear,"clear", A_GIMME, 0);
  addbang((method)theobject_bang);
  addmess((method)theobject_list, "list", A_GIMME, 0);
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

void theobject_bang(t_theobject *x)
{
  int i;
  char s[MAXORDER], t[MAXORDER];
  mxArray *result;
  double *datptr ;

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
  post(s);
  engEvalString(x->x_engine, s);
  strcpy(s, "y=sum(x);");
  post(s);
  engEvalString(x->x_engine, s);
  result = engGetVariable(x->x_engine, "y");
  datptr = mxGetPr(result);
  x->x_result = *datptr;
  mxDestroyArray(result);
  outlet_float(x->x_floatout, x->x_result);
}
    
void theobject_list(t_theobject *x, t_symbol *msg, short argc, t_atom *argv)
{
  int i;
  for (i=0; i<argc; i++){
    x->x_array[i] = argv[i].a_w.w_float;
  }
  x->x_len = argc;
  post("list received");
}

void theobject_clear(t_theobject *x, t_symbol *message, short argc, t_atom *argv)
{
  int i;
  for (i=0; i<MAXORDER; i++){
    x->x_array[i] = 0;
  } 
}


void *theobject_new(double val)
{
  int i;
  
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
  if ((x->x_engine = engOpen("\0"))==0)
    post("Couldnt load matlab");
  else post("Matlab engine loaded successfully");  

  return (x);
}
void theobject_free(t_theobject *x)
{
  if(engClose(x->x_engine)==0)
    post("Matlab engine closed successfully");

  dsp_free(&x->x_obj);
}
  
