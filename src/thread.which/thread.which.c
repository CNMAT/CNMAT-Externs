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
  NAME: thread.which
  DESCRIPTION: Report the current executing thread
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2012
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define NAME "thread.which"
#define DESCRIPTION "Report the current executing thread"
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2012,13"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include <pthread.h>
#include "ext_systhread.h"
//#include <execinfo.h>

#define LEFT_OUTLET 0
#define RIGHT_OUTLET 1

typedef struct _threadwhich{
	t_object ob;
	void *outlets[2];
} t_threadwhich;

void *threadwhich_class;

void threadwhich_bang(t_threadwhich *x);
void threadwhich_long(t_threadwhich *x, long l);
void threadwhich_float(t_threadwhich *x, double f);
void threadwhich_list(t_threadwhich *x, t_symbol *msg, long argc, t_atom *argv);
void threadwhich_anything(t_threadwhich *x, t_symbol *msg, long argc, t_atom *argv);
void threadwhich_free(t_threadwhich *x);
void threadwhich_assist(t_threadwhich *x, void *b, long m, long a, char *s);
void *threadwhich_new(t_symbol *msg, short argc, t_atom *argv);

void threadwhich_outputThread(t_threadwhich *x)
{
	if(systhread_ismainthread()){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("main"), 0, NULL);
	}else if(systhread_istimerthread()){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("timer"), 0, NULL);
	}else{
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("other"), 0, NULL);
	}
}

void threadwhich_bang(t_threadwhich *x){
	threadwhich_outputThread(x);
	outlet_bang(x->outlets[LEFT_OUTLET]);
}

void threadwhich_long(t_threadwhich *x, long l){
	threadwhich_outputThread(x);
	outlet_int(x->outlets[LEFT_OUTLET], l);
}

void threadwhich_float(t_threadwhich *x, double f){
	threadwhich_outputThread(x);
	outlet_float(x->outlets[LEFT_OUTLET], f);
}

void threadwhich_list(t_threadwhich *x, t_symbol *msg, long argc, t_atom *argv){
	threadwhich_outputThread(x);
	outlet_list(x->outlets[LEFT_OUTLET], NULL, argc, argv);
}

void threadwhich_anything(t_threadwhich *x, t_symbol *msg, long argc, t_atom *argv){
	threadwhich_outputThread(x);
	outlet_anything(x->outlets[LEFT_OUTLET], msg, argc, argv);
}

void threadwhich_assist(t_threadwhich *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void threadwhich_free(t_threadwhich *x){
}

void *threadwhich_new(t_symbol *msg, short argc, t_atom *argv){
	t_threadwhich *x;
	if(x = (t_threadwhich *)object_alloc(threadwhich_class)){
		x->outlets[RIGHT_OUTLET] = outlet_new(x, NULL);
		x->outlets[LEFT_OUTLET] = outlet_new(x, NULL);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("thread.which", (method)threadwhich_new, (method)threadwhich_free, sizeof(t_threadwhich), 0L, A_GIMME, 0);
	class_addmethod(c, (method)threadwhich_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)threadwhich_bang, "bang", 0);
	class_addmethod(c, (method)threadwhich_long, "int", 0);
	class_addmethod(c, (method)threadwhich_float, "float", 0);
	class_addmethod(c, (method)threadwhich_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)threadwhich_anything, "anything", A_GIMME, 0);

	class_register(CLASS_BOX, c);
	threadwhich_class = c;

	common_symbols_init();

	version_post_copyright();
	return 0;
}
