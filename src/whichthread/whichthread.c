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
  NAME: whichthread
  DESCRIPTION: Report the current executing thread
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2012
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define NAME "whichthread"
#define DESCRIPTION "Report the current executing thread"
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2012,13"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include <pthread.h>
#include "ext_systhread.h"
#include <execinfo.h>

#define LEFT_OUTLET 0
#define RIGHT_OUTLET 1

typedef struct _whichthread{
	t_object ob;
	void *outlets[2];
} t_whichthread;

void *whichthread_class;

void whichthread_bang(t_whichthread *x);
void whichthread_long(t_whichthread *x, long l);
void whichthread_float(t_whichthread *x, double f);
void whichthread_list(t_whichthread *x, t_symbol *msg, long argc, t_atom *argv);
void whichthread_anything(t_whichthread *x, t_symbol *msg, long argc, t_atom *argv);
void whichthread_free(t_whichthread *x);
void whichthread_assist(t_whichthread *x, void *b, long m, long a, char *s);
void *whichthread_new(t_symbol *msg, short argc, t_atom *argv);

void whichthread_outputThread(t_whichthread *x)
{
	if(systhread_ismainthread()){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("main"), 0, NULL);
	}else if(systhread_istimerthread()){
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("timer"), 0, NULL);
	}else{
		outlet_anything(x->outlets[RIGHT_OUTLET], gensym("other"), 0, NULL);
	}
}

void whichthread_bang(t_whichthread *x){
	whichthread_outputThread(x);
	outlet_bang(x->outlets[LEFT_OUTLET]);
}

void whichthread_long(t_whichthread *x, long l){
	whichthread_outputThread(x);
	outlet_int(x->outlets[LEFT_OUTLET], l);
}

void whichthread_float(t_whichthread *x, double f){
	whichthread_outputThread(x);
	outlet_float(x->outlets[LEFT_OUTLET], f);
}

void whichthread_list(t_whichthread *x, t_symbol *msg, long argc, t_atom *argv){
	whichthread_outputThread(x);
	outlet_list(x->outlets[LEFT_OUTLET], NULL, argc, argv);
}

void whichthread_anything(t_whichthread *x, t_symbol *msg, long argc, t_atom *argv){
	whichthread_outputThread(x);
	outlet_anything(x->outlets[LEFT_OUTLET], msg, argc, argv);
}

void whichthread_assist(t_whichthread *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void whichthread_free(t_whichthread *x){
}

void *whichthread_new(t_symbol *msg, short argc, t_atom *argv){	
	t_whichthread *x;
	if(x = (t_whichthread *)object_alloc(whichthread_class)){
		x->outlets[RIGHT_OUTLET] = outlet_new(x, NULL);
		x->outlets[LEFT_OUTLET] = outlet_new(x, NULL);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("whichthread", (method)whichthread_new, (method)whichthread_free, sizeof(t_whichthread), 0L, A_GIMME, 0);
	class_addmethod(c, (method)whichthread_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)whichthread_bang, "bang", 0);
	class_addmethod(c, (method)whichthread_long, "int", 0);
	class_addmethod(c, (method)whichthread_float, "float", 0);
	class_addmethod(c, (method)whichthread_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)whichthread_anything, "anything", A_GIMME, 0);

	class_register(CLASS_BOX, c);
	whichthread_class = c;

	common_symbols_init();

	version_post_copyright();
	return 0;
}
