/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2011, The Regents of
  the University of California (Regents). 
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
  NAME: backtrace
  DESCRIPTION: Backtrace tail call optimization for Max
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2011
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include <pthread.h>
#include "ext_systhread.h"
#include <execinfo.h>

#define LEFT_OUTLET 0
#define RIGHT_OUTLET 1

#define BACKTRACE_ARRAY_LEN 512
typedef struct _backtrace{
	t_object ob;
	void *outlets[2];
} t_backtrace;

void *backtrace_class;

void backtrace_bang(t_backtrace *x);
void backtrace_long(t_backtrace *x, long l);
void backtrace_float(t_backtrace *x, double f);
void backtrace_list(t_backtrace *x, t_symbol *msg, long argc, t_atom *argv);
void backtrace_anything(t_backtrace *x, t_symbol *msg, long argc, t_atom *argv);
void backtrace_free(t_backtrace *x);
void backtrace_assist(t_backtrace *x, void *b, long m, long a, char *s);
void *backtrace_new(t_symbol *msg, short argc, t_atom *argv);

void backtrace_bang(t_backtrace *x){
	void *bt[BACKTRACE_ARRAY_LEN];
	int n = backtrace(bt, BACKTRACE_ARRAY_LEN);
	char **syms = backtrace_symbols(bt, n);
	int i;
	for(i = 0; i < n; i++){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym(syms[i]), 0, NULL);
	}
	outlet_bang(x->outlets[LEFT_OUTLET]);
}

void backtrace_long(t_backtrace *x, long l){
	void *bt[BACKTRACE_ARRAY_LEN];
	int n = backtrace(bt, BACKTRACE_ARRAY_LEN);
	char **syms = backtrace_symbols(bt, n);
	int i;
	for(i = 0; i < n; i++){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym(syms[i]), 0, NULL);
	}
	outlet_int(x->outlets[LEFT_OUTLET], l);
}

void backtrace_float(t_backtrace *x, double f){
	void *bt[BACKTRACE_ARRAY_LEN];
	int n = backtrace(bt, BACKTRACE_ARRAY_LEN);
	char **syms = backtrace_symbols(bt, n);
	int i;
	for(i = 0; i < n; i++){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym(syms[i]), 0, NULL);
	}
	outlet_float(x->outlets[LEFT_OUTLET], f);
}

void backtrace_list(t_backtrace *x, t_symbol *msg, long argc, t_atom *argv){
	void *bt[BACKTRACE_ARRAY_LEN];
	int n = backtrace(bt, BACKTRACE_ARRAY_LEN);
	char **syms = backtrace_symbols(bt, n);
	int i;
	for(i = 0; i < n; i++){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym(syms[i]), 0, NULL);
	}
	outlet_list(x->outlets[LEFT_OUTLET], NULL, argc, argv);
}

void backtrace_anything(t_backtrace *x, t_symbol *msg, long argc, t_atom *argv){
	void *bt[BACKTRACE_ARRAY_LEN];
	int n = backtrace(bt, BACKTRACE_ARRAY_LEN);
	char **syms = backtrace_symbols(bt, n);
	int i;
	for(i = 0; i < n; i++){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym(syms[i]), 0, NULL);
	}
	outlet_anything(x->outlets[LEFT_OUTLET], msg, argc, argv);
}

void backtrace_assist(t_backtrace *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void backtrace_free(t_backtrace *x){
}

void *backtrace_new(t_symbol *msg, short argc, t_atom *argv){	
	t_backtrace *x;
	if(x = (t_backtrace *)object_alloc(backtrace_class)){
		x->outlets[RIGHT_OUTLET] = outlet_new(x, NULL);
		x->outlets[LEFT_OUTLET] = outlet_new(x, NULL);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("backtrace", (method)backtrace_new, (method)backtrace_free, sizeof(t_backtrace), 0L, A_GIMME, 0);
	class_addmethod(c, (method)backtrace_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)backtrace_bang, "bang", 0);
	class_addmethod(c, (method)backtrace_long, "int", 0);
	class_addmethod(c, (method)backtrace_float, "float", 0);
	class_addmethod(c, (method)backtrace_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)backtrace_anything, "anything", A_GIMME, 0);

	class_register(CLASS_BOX, c);
	backtrace_class = c;

	common_symbols_init();
	return 0;
}
