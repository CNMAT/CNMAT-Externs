#include "libarray.h"

static unsigned int ar_instance_counter = 0;

void ar_initlib(void){
	_sym_array = gensym("array");
	_sym_hashtab = gensym("hashtab");
	_sym_linklist = gensym("linklist");
	_sym_refcount = gensym("refcount");
}

void ar_freeall(t_hashtab *ht, t_linklist *ll){
	if(!ht || !ll){
		return;
	}
	t_symbol **keys = NULL;
	long numKeys = 0;
	long i;
	t_atombuf *ab;
	hashtab_getkeys(ht, &numKeys, &keys);
	for(i = 0; i < numKeys; i++){
		hashtab_lookup(ht, keys[i], (t_object **)(&ab));
		if(ab){
			atombuf_free(ab);
		}
		hashtab_chuckkey(ht, keys[i]);
		linklist_chuckobject(ll, keys[i]);
	}
}

t_symbol *ar_make_iname(t_symbol *name, t_symbol *type){
	char iname_char[64];
	sprintf(iname_char, "cnmat_%s_%s", type->s_name, name->s_name);
	post("%s", iname_char);
	return gensym(iname_char);
}

t_symbol *ar_encode_key(t_atom *a){
	if(a[0].a_type == A_FLOAT){
		return ar_encode_key_from_float(a[0].a_w.w_float);
	}else if(a[0].a_type == A_LONG){
		return ar_encode_key_from_int(a[0].a_w.w_long);
	}else{
		return ar_encode_key_from_symbol(a[0].a_w.w_sym);
	}
}

t_symbol *ar_encode_key_from_float(float f){
	char buf[9];
	sprintf(buf, "f%x", *((unsigned int *)(&f)));
	return gensym(buf);
}

t_symbol *ar_encode_key_from_int(long l){
	char buf[9];
	sprintf(buf, "d%x", (unsigned int)l);
	return gensym(buf);
}

t_symbol *ar_encode_key_from_symbol(t_symbol *s){
	char buf[128];
	sprintf(buf, "s%s", s->s_name);
	return gensym(buf);
}

void ar_decode_key(t_symbol *s, t_atom *a){
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

long ar_inc_refcount(t_symbol *name){
	t_symbol *refcount = ar_make_iname(name, _sym_refcount);
	long count = 1;
	if(!(refcount->s_thing)){
		refcount->s_thing = (void *)count;
	}else{
		count = ((long)(refcount->s_thing));
		count++;
		refcount->s_thing = (void *)count;
	}
	return count;
}

long ar_dec_refcount(t_symbol *name){
	t_symbol *refcount = ar_make_iname(name, _sym_refcount);
	long count = ((long)(refcount->s_thing));
	count--;
	refcount->s_thing = (void *)count;
	return count;
}

t_symbol *ar_make_def_name(){
	unsigned int n = ++ar_instance_counter;
	char buf[11];
	sprintf(buf, "0x%08x", n);
	return gensym(buf);
}
