/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009, The Regents of
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
NAME: postpone
DESCRIPTION: Delay anything.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

typedef struct _ppn{
	t_object ob;
	void *outlet;
	long in;
	void *proxy;
	long delay_time;
	long state;
} t_ppn;

static t_class *ppn_class;

void *ppn_new();
void ppn_bang(t_ppn *x);
void ppn_int(t_ppn *x, long n);
void ppn_float(t_ppn *x, double xx);
void ppn_list(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_anything(t_ppn *x, t_symbol *msg, short argc, t_atom *argv);
void ppn_free(t_ppn *x);
void ppn_assist(t_ppn *x, void *b, long m, long a, char *s);
void ppn_callback(t_ppn *x, t_symbol *s, short argc, t_atom *argv);

int main(void){
	t_class *c;
	c = class_new("postpone", (method)ppn_new, (method)ppn_free, sizeof(t_ppn), 0L, 0); 
	
	class_addmethod(c, (method)ppn_bang, "bang", 0);
	class_addmethod(c, (method)ppn_int, "int", A_LONG, 0);
	class_addmethod(c, (method)ppn_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)ppn_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)ppn_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)ppn_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);

	ppn_class = c;

	common_symbols_init();

	return 0;
}

void *ppn_new(){
	t_ppn *x;

	if(x = (t_ppn *)object_alloc(ppn_class)){
		x->proxy = proxy_new((t_object *)x, 1, &x->in);
		x->outlet = outlet_new(x, NULL);
		x->delay_time = 0;
		x->state = 0;
		return x;
	}
	return NULL;
}

void ppn_bang(t_ppn *x){
	t_atom a;
	atom_setlong(&a, x->state);
	schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time, _sym_bang, 1, &a);
}

void ppn_int(t_ppn *x, long n){
	t_atom a[2];
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		atom_setlong(&(a[0]), x->state);
		atom_setlong(&(a[1]), n);
		schedule_delay((t_object *)x, (method)ppn_callback, x->delay_time, 0, 2, a);
		break;
	case 1:
		x->delay_time = n;
		break;
	}
}

void ppn_float(t_ppn *x, double f){
	switch(proxy_getinlet((t_object *)x)){
	case 0:

		break;
	case 1:
		x->delay_time = (long)f;
		break;
	}
}

void ppn_list(t_ppn *x, t_symbol *msg, short argc, t_atom *argv){
}

void ppn_anything(t_ppn *x, t_symbol *msg, short argc, t_atom *argv){
}

void ppn_free(t_ppn *x){
}

void ppn_assist(t_ppn *x, void *b, long m, long a, char *s){
}

void ppn_callback(t_ppn *x, t_symbol *s, short argc, t_atom *argv){
	int state = 0;
	if(argc == 0){
		error("postpone: something really bad happened");
		return;
	}
	state = atom_getlong(argv);
	if(state != x->state){
		return;
	}
	if(s){
		if(s == _sym_bang){
			outlet_bang(x->outlet);
		}else if(s == _sym_int){
			outlet_int(x->outlet, atom_getlong(argv + 1));
		}else if(s == _sym_float){
			outlet_int(x->outlet, atom_getfloat(argv + 1));
		}else if(s == _sym_list){
			outlet_list(x->outlet, argc - 1, argv + 1);
		}else{
			//outlet_list(x->outlet, 
		}
	}
}
