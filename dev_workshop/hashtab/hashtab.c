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
NAME: hashtab
DESCRIPTION: A hashtable for Max
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

/* Description of the object:

 */

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h" 	// Hashtable header


typedef struct _htab{
	t_object ob;
	void *outlet;
	t_symbol *name;  // The name of our hashtable
	t_hashtab *hashtab; // A pointer to our object's hashtable
} t_htab;

static t_class *htab_class;

void htab_bang(t_htab *x);
void htab_int(t_htab *x, long n);
void htab_float(t_htab *x, double xx);
void htab_list(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_anything(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_free(t_htab *x);
void htab_assist(t_htab *x, void *b, long m, long a, char *s);
void htab_inletinfo(t_htab *x, void *b, long index, char *t);
void *htab_new(t_symbol *name, long size);

void htab_bang(t_htab *x){

}

void htab_int(t_htab *x, long n){

}

void htab_float(t_htab *x, double f){

}

void htab_list(t_htab *x, t_symbol *msg, short argc, t_atom *argv){

}

void htab_anything(t_htab *x, t_symbol *msg, short argc, t_atom *argv){
	if(argc == 0){
		// get the data associated with the message
		
	}else{
	}
}

void htab_free(t_htab *x){
}

void htab_assist(t_htab *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "Store or recall data");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "Data that has been recalled");
			break;
		}
		break;
	}
}


/**************************************************
Object and instance creation functions.
 **************************************************/

void *htab_new(t_symbol *name, long size){
	t_htab *x;

	if(x = (t_htab *)object_alloc(htab_class)){
		t_hashtab *ht;
		x->outlet = outlet_new(x, NULL);
		if(name){
			x->name = name;
		}else{
			name = gensym("");
		}
		if(ht = (t_hashtab *)(name->s_thing)){
			x->hashtab = ht;
		}else{
			x->hashtab = (t_hashtab *)hashtab_new(size);
		}
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("hashtab", (method)htab_new, (method)htab_free, sizeof(t_htab), 0L, A_DEFSYM, A_DEFLONG, 0);

	class_addmethod(c, (method)htab_bang, "bang", 0);
	class_addmethod(c, (method)htab_int, "int", A_LONG, 0);
	class_addmethod(c, (method)htab_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)htab_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)htab_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)htab_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);

	class_register(CLASS_BOX, c);
	htab_class = c;

	common_symbols_init();

	return 0;
}

