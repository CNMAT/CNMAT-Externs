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

#define HTAB_DEFAULT_SLOTS 512;

typedef struct _htab{
	t_object ob;
	void *outlet;
	t_symbol *name;  // The name of our hashtable
	t_symbol *iname; // The internal name of our hashtable
	t_hashtab *hashtab; // A pointer to our object's hashtable
} t_htab;

static t_class *htab_class;

void htab_count(t_htab *x);
void htab_store(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_lookup(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_keys(t_htab *x);
void htab_hashtab(t_htab *x, long ptr);
void htab_clear(t_htab *x);
void htab_dump(t_htab *x);
void htab_bang(t_htab *x);
void htab_int(t_htab *x, long n);
void htab_float(t_htab *x, double xx);
void htab_list(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_anything(t_htab *x, t_symbol *msg, short argc, t_atom *argv);
void htab_free(t_htab *x);
void htab_assist(t_htab *x, void *b, long m, long a, char *s);
t_symbol *htab_make_iname(t_symbol *name);
t_symbol *htab_make_key(t_atom *a);
t_symbol *htab_make_key_from_float(float f);
t_symbol *htab_make_key_from_int(long l);
t_symbol *htab_make_key_from_symbol(t_symbol *s);
void htab_get_key(t_symbol *s, t_atom *a);
void htab_set(t_htab *x, t_symbol *name);
void *htab_new(t_symbol *name, long size);

t_symbol *_sym_hashtab;

void htab_count(t_htab *x){
	if(!x->hashtab){
		return;
	}
	outlet_int(x->outlet, hashtab_getsize(x->hashtab));
}

void htab_store(t_htab *x, t_symbol *msg, short argc, t_atom *argv){
	if(!x->hashtab){
		return;
	}
	if(argc < 2){
		return;
	}

	t_atombuf *ab = (t_atombuf *)atombuf_new(argc - 1, argv + 1);
	hashtab_store(x->hashtab, htab_make_key(argv), (t_object *)ab);
}

void htab_lookup(t_htab *x, t_symbol *msg, short argc, t_atom *argv){
	if(!x->hashtab){
		return;
	}
	if(argc < 1){
		return;
	}
	t_atombuf *ab;
	hashtab_lookup(x->hashtab, htab_make_key(argv), (t_object **)(&ab));
	if(ab){
		outlet_list(x->outlet, NULL, ab->a_argc, ab->a_argv);
	}
}

void htab_keys(t_htab *x){
	if(!x->hashtab){
		return;
	}
	t_symbol **keys = NULL;
	long numKeys = 0;
	long i;
	hashtab_getkeys(x->hashtab, &numKeys, &keys);
	for(i = 0; i < numKeys; i++){
		t_atom a;
		htab_get_key(keys[i], &a);
		if(a.a_type == A_FLOAT){
			outlet_float(x->outlet, a.a_w.w_float);
		}else if(a.a_type == A_LONG){
			outlet_int(x->outlet, a.a_w.w_long);
		}else{
			outlet_anything(x->outlet, a.a_w.w_sym, 0, NULL);
		}
	}
}

void htab_hashtab(t_htab *x, long ptr){
	t_hashtab *ht = (t_hashtab *)ptr;
	t_symbol **keys = NULL;
	long numKeys = 0;
	long i;
	hashtab_getkeys(ht, &numKeys, &keys);
	if(!x->hashtab){
		x->hashtab = hashtab_new(0);
	}
	htab_clear(x);
	for(i = 0; i < numKeys; i++){
		t_atombuf *ab1, *ab2;
		hashtab_lookup(ht, keys[i], (t_object **)(&ab1));
		ab2 = (t_atombuf *)atombuf_new(ab1->a_argc, ab1->a_argv);
		hashtab_store(x->hashtab, keys[i], (t_object *)ab2);
	}
}

void htab_clear(t_htab *x){
	if(!x->hashtab){
		return;
	}
	t_symbol **keys = NULL;
	long numKeys = 0;
	long i;
	t_atombuf *ab;
	hashtab_getkeys(x->hashtab, &numKeys, &keys);
	for(i = 0; i < numKeys; i++){
		hashtab_lookup(x->hashtab, keys[i], (t_object **)(&ab));
		if(ab){
			atombuf_free(ab);
		}
		hashtab_chuckkey(x->hashtab, keys[i]);
	}
}

void htab_dump(t_htab *x){
	if(!x->hashtab){
		return;
	}
	t_symbol **keys = NULL;
	long numKeys = 0;
	long i;
	t_atombuf *ab;
	hashtab_getkeys(x->hashtab, &numKeys, &keys);
	for(i = 0; i < numKeys; i++){
		hashtab_lookup(x->hashtab, keys[i], (t_object **)(&ab));
		outlet_anything(x->outlet, keys[i], ab->a_argc, ab->a_argv);
	}
}

void htab_bang(t_htab *x){
	if(!x->hashtab){
		return;
	}
	t_atom a;
	atom_setlong(&a, (long)x->hashtab);
	outlet_anything(x->outlet, _sym_hashtab, 1, &a);
}

void htab_int(t_htab *x, long n){
}

void htab_float(t_htab *x, double f){
}

void htab_list(t_htab *x, t_symbol *msg, short argc, t_atom *argv){
}

void htab_anything(t_htab *x, t_symbol *msg, short argc, t_atom *argv){
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

t_symbol *htab_make_iname(t_symbol *name){
	char *name_char = name->s_name;
	char iname_char[64];
	sprintf(iname_char, "cnmat_hashtab_%s", name_char);
	return gensym(iname_char);
}

t_symbol *htab_make_key(t_atom *a){
	if(a[0].a_type == A_FLOAT){
		return htab_make_key_from_float(a[0].a_w.w_float);
	}else if(a[0].a_type == A_LONG){
		return htab_make_key_from_int(a[0].a_w.w_long);
	}else{
		return htab_make_key_from_symbol(a[0].a_w.w_sym);
	}
}

t_symbol *htab_make_key_from_float(float f){
	char buf[9];
	sprintf(buf, "f%x", *((unsigned int *)(&f)));
	return gensym(buf);
}

t_symbol *htab_make_key_from_int(long l){
	char buf[9];
	sprintf(buf, "d%x", (unsigned int)l);
	return gensym(buf);
}

t_symbol *htab_make_key_from_symbol(t_symbol *s){
	char buf[128];
	sprintf(buf, "s%s", s->s_name);
	return gensym(buf);
}

void htab_get_key(t_symbol *s, t_atom *a){
	char *cp = s->s_name;
	if(cp[0] == 'f'){
		float f;
		sscanf(cp + 1, "%x", (unsigned int *)(&f));
		atom_setfloat(a, f);
	}else if(cp[0] == 'd'){
		unsigned long l;
		sscanf(cp + 1, "%x", (unsigned int *)(&l));
		atom_setlong(a, (long)l);
	}else{
		atom_setsym(a, gensym(cp + 1));
	}
}

void htab_set(t_htab *x, t_symbol *name){
	if((t_hashtab *)name->s_thing){
		x->name = name;
		x->iname = htab_make_iname(name);
		x->hashtab = (t_hashtab *)(x->iname->s_thing);
	}else{
		error("hashtab: There is no hashtable called %s", name->s_name);
		return;
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
			x->iname = htab_make_iname(name);
			if(ht = (t_hashtab *)(name->s_thing)){
				x->hashtab = ht;
			}else{
				x->hashtab = (t_hashtab *)hashtab_new(size);
				x->iname->s_thing = (t_object *)x->hashtab;
			}
		}else{
			x->name = _sym_emptytext;
			x->iname = _sym_emptytext;
		}
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("hashtab", (method)htab_new, (method)htab_free, sizeof(t_htab), 0L, A_DEFSYM, A_DEFLONG, 0);

	class_addmethod(c, (method)htab_count, "count", 0);
	class_addmethod(c, (method)htab_store, "store", A_GIMME, 0);
	class_addmethod(c, (method)htab_lookup, "lookup", A_GIMME, 0);
	class_addmethod(c, (method)htab_keys, "keys", 0);
	class_addmethod(c, (method)htab_set, "set", A_SYM, 0);
	class_addmethod(c, (method)htab_hashtab, "hashtab", A_LONG, 0);
	class_addmethod(c, (method)htab_clear, "clear", 0);
	class_addmethod(c, (method)htab_dump, "dump", 0);

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

	_sym_hashtab = gensym("hashtab");

	return 0;
}

