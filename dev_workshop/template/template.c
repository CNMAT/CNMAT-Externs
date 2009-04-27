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
NAME: template
DESCRIPTION: A template for Max objects
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

typedef struct _temp{
	t_object ob;
	void *outlet;
} t_temp;

static t_class *temp_class;

void temp_bang(t_temp *x);
void temp_int(t_temp *x, long n);
void temp_float(t_temp *x, double xx);
void temp_list(t_temp *x, t_symbol *msg, short argc, t_atom *argv);
void temp_anything(t_temp *x, t_symbol *msg, short argc, t_atom *argv);
void temp_free(t_temp *x);
void temp_assist(t_temp *x, void *b, long m, long a, char *s);
void temp_inletinfo(t_temp *x, void *b, long index, char *t);
void *temp_new(void);

void temp_bang(t_temp *x){
	post("Hello World!");
}

void temp_int(t_temp *x, long n){

}

void temp_float(t_temp *x, double f){

}

void temp_list(t_temp *x, t_symbol *msg, short argc, t_atom *argv){

}

void temp_anything(t_temp *x, t_symbol *msg, short argc, t_atom *argv){

}

void temp_free(t_temp *x){
}

void temp_assist(t_temp *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	}
}


/**************************************************
Object and instance creation functions.
 **************************************************/

void *temp_new(void){
	t_temp *x;

	if(x = (t_temp *)object_alloc(temp_class)){
		x->outlet = outlet_new(x, NULL);
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("template", (method)temp_new, (method)temp_free, sizeof(t_temp), 0L, 0);

	class_addmethod(c, (method)temp_bang, "bang", 0);
	class_addmethod(c, (method)temp_int, "int", A_LONG, 0);
	class_addmethod(c, (method)temp_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)temp_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)temp_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)temp_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);

	class_register(CLASS_BOX, c);
	temp_class = c;

	common_symbols_init();

	return 0;
}

