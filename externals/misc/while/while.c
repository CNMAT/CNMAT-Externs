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
NAME: while
DESCRIPTION: While loop for max
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

typedef struct _while{
	t_object ob;
	void *outlet;
	long inlet;
	void **proxy;
	int condition;
} t_while;

void *while_class;

void while_int(t_while *x, long l);
void while_break(t_while *x);
void while_bang(t_while *x);
void while_free(t_while *x);
void while_assist(t_while *x, void *b, long m, long a, char *s);
void *while_new(t_symbol *msg, short argc, t_atom *argv);

void while_int(t_while *x, long l){
	if(proxy_getinlet((t_object *)x) == 1){
		x->condition = l;
	}
}

void while_break(t_while *x){
	x->condition = 0;
}

void while_bang(t_while *x){
	x->condition = 1;
	do{
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
		outlet_bang(x->outlet);
		if(x->condition == 0){
			break;
		}
	}while(x->condition != 0);
}

void while_assist(t_while *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void while_free(t_while *x){
	if(x->proxy){
		object_free(x->proxy);
	}
}

void *while_new(t_symbol *msg, short argc, t_atom *argv){	
	t_while *x;
	if(x = (t_while *)object_alloc(while_class)){
		x->outlet = bangout(x);
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("while", (method)while_new, (method)while_free, sizeof(t_while), 0L, A_GIMME, 0);
	class_addmethod(c, (method)while_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)while_bang, "bang", 0);
	class_addmethod(c, (method)while_int, "int", A_LONG, 0);
	class_addmethod(c, (method)while_break, "break", 0);

	class_register(CLASS_BOX, c);
	while_class = c;

	common_symbols_init();
	return 0;
}
