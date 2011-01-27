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
NAME: o.if
DESCRIPTION: Sends a bundle through an outlet based on the results of a test
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
#include "ext_hashtab.h"
#include "omax_util.h"
#include "osc_util.h"

typedef t_atom (*two_op_func)(t_atom arg1, t_atom arg2);

enum types{
	OIF_ATOM,
	OIF_FUNC,
	OIF_ADDRESS
};

typedef struct _arg{
	union{
		t_atom atom;
		struct _func *function;
		t_symbol *address;
	} w;	
	int type;
} t_arg;

typedef struct _func{
	two_op_func f;
	t_arg arg1, arg2;
} t_func;

typedef struct _oif{
	t_object ob;
	void *outlets[2];
	void *proxy;
	long inlet;
	int result;
	t_hashtab *ht;
	t_atom *atom_buf;
	int buflen, bufpos;
	t_symbol *function_string;
	t_func function_graph;
} t_oif;

#define ARG_SET_ATOM(a, val) (a)->w.atom = val; (a)->type = OIF_ATOM
#define ARG_SET_FUNC(a, val) (a)->w.function = val; (a)->type = OIF_FUNC
#define ARG_SET_ADDRESS(a, val) (a)->w.address = val; (a)->type = OIF_ADDRESS

#ifdef P2F_DEBUG
#define P2F(a1, a2) post("%s(%f, %f)", __PRETTY_FUNCTION__, atom_getfloat(&(a1)), atom_getfloat(&(a2)))
#else
#define P2F(a1, a2)
#endif

void *oif_class;

void oif_fullPacket(t_oif *x, long len, long ptr);
void oif_cbk(t_osc_msg msg, void *v);
int oif_parse(t_oif *x, char *st, t_func *func);
t_atom oif_funcall(t_oif *x, t_func *func);

t_atom oif_and(t_atom arg1, t_atom arg2);
t_atom oif_or(t_atom arg1, t_atom arg2);
t_atom oif_lt(t_atom arg1, t_atom arg2);
t_atom oif_lte(t_atom arg1, t_atom arg2);
t_atom oif_gt(t_atom arg1, t_atom arg2);
t_atom oif_gte(t_atom arg1, t_atom arg2);
t_atom oif_eq(t_atom arg1, t_atom arg2);
t_atom oif_neq(t_atom arg1, t_atom arg2);

void oif_free(t_oif *x);
void oif_assist(t_oif *x, void *b, long m, long a, char *s);
void *oif_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oif_notify(t_oif *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oif_fullPacket(t_oif *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = osc_util_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = osc_util_flatten(nn, cpy, cpy);

	// extract the messages from the bundle
	hashtab_clear(x->ht);
	x->bufpos = 0;
	osc_util_parseBundleWithCallback(nn, cpy, oif_cbk, (void *)x);

	t_atom a = oif_funcall(x, &(x->function_graph));
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, ptr);
	if(atom_getlong(&a) == 0){
		outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
	}else{
		outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
	}
	//post("%d", atom_getlong(&a));

	/*
	t_symbol **keys = NULL;
	long nkeys;
	hashtab_getkeys(x->ht, &nkeys, &keys);
	int i;
	for(i = 0; i < nkeys; i++){
		post("%s", keys[i]->s_name);
		t_atom *a;
		hashtab_lookup(x->ht, keys[i], (t_object **)&a);
		int j;
		for(j = 0; j < atom_getlong(a); j++){
			postatom(a + j + 1);
		}
	}
	for(i = 0; i < 4; i++){
		postatom(x->atom_buf + i);
	}
	*/
}

void oif_cbk(t_osc_msg msg, void *v){
	t_oif *x = (t_oif *)v;
	long len = msg.argc + 1;
	//t_atom atoms[len];
	if(x->bufpos + len > x->buflen){
		x->atom_buf = (t_atom *)realloc(x->atom_buf, x->buflen * 2 * sizeof(t_atom));
		x->buflen *= 2;
	}
	omax_util_oscMsg2MaxAtoms(&msg, &len, x->atom_buf + x->bufpos);
	//memcpy(x->atom_buf + x->bufpos, atoms + 1, msg.argc * sizeof(t_atom));
	// hashtab stuff
	hashtab_store(x->ht, atom_getsym(x->atom_buf + x->bufpos), (t_object *)(x->atom_buf + x->bufpos));
	atom_setlong(x->atom_buf + x->bufpos, msg.argc);
	x->bufpos += msg.argc + 1;
}

int oif_parse(t_oif *x, char *st, t_func *func){
	if(*st != '('){
		object_error((t_object *)x, "test string must start with \"(\"");
		return -1;
	}
	char *ptr = st + 1;

	//static int count;
	//count++;
	//post("function %d, arg1: %s", count, ptr);
	if(*ptr == '('){
		// function
		t_func *f = (t_func *)sysmem_newptr(sizeof(t_func));
		ARG_SET_FUNC(&(func->arg1), f);
		int ret = oif_parse(x, ptr, f);
		if(ret < 0){
			return ret;
		}
		ptr += ret;
		/*
		while(*ptr != ')' && *ptr != '\0'){
			ptr++;
		}
		//ptr += 2;
		while(*ptr != ' ' && *ptr != '\0'){
			ptr++;
		}
		ptr++;
		*/
	}else if(*ptr == '/'){
		// address
		char *start = ptr;
		while(*ptr != ' ' && *ptr != '\0'){
			ptr++;
		}
		char buf[ptr - start + 1];
		memcpy(buf, start, (ptr - start));
		buf[ptr - start] = '\0';
		ARG_SET_ADDRESS(&(func->arg1), gensym(buf));
		ptr++;
	}else{
		// value
		char *endptr;
		float f = strtof(ptr, &endptr);
		t_atom a;
		atom_setfloat(&a, f);
		ARG_SET_ATOM(&(func->arg1), a);
		while(*ptr != ' '){
			ptr++;
		}
		ptr++;
	}


	// check for ')' here just in case there are 2 sets of parens around
	// this part.  if we find that, set f to oif_eq and arg2 to arg1 so that
	// it will succeed no matter what
	
	// op
	if(*ptr == '&' && *(ptr + 1) == '&'){
		func->f = oif_and;
		ptr += 3;
	}else if(*ptr == '|' && *(ptr + 1) == '|'){
		func->f = oif_or;
		ptr += 3;
	}else if(*ptr == '<'){
		func->f = oif_lt;
		ptr += 2;
	}else if(!strncmp(ptr, "<=", 2)){
		func->f = oif_lte;
		ptr += 3;
	}else if(*ptr == '>'){
		func->f = oif_gt;
		ptr += 2;
	}else if(!strncmp(ptr, ">=", 2)){
		func->f = oif_gte;
		ptr += 3;
	}else if(!strncmp(ptr, "==", 2)){
		func->f = oif_eq;
		ptr += 3;
	}else if(!strncmp(ptr, "!=", 2)){
		func->f = oif_neq;
		ptr += 3;
	}else{
		object_error((t_object *)x, "unrecognized operation %s", ptr);
		return -1;
	}
	
	// arg2
	//post("function %d, arg2: %s", count, ptr);
	if(*ptr == '('){
		// function
		t_func *f = (t_func *)sysmem_newptr(sizeof(t_func));
		ARG_SET_FUNC(&(func->arg2), f);
		int ret = oif_parse(x, ptr, f);
		if(ret < 0){
			return ret;
		}
		ptr += ret;
		/*
		while(*ptr != ')' && *ptr != '\0'){
			ptr++;
		}
		//ptr += 2;
		while(*ptr != ' ' && *ptr != '\0'){
			ptr++;
		}
		ptr++;
		*/
	}else if(*ptr == '/'){
		// address
		char *start = ptr;
		while(*ptr != ')' && *ptr != '\0'){
			ptr++;
		}
		char buf[ptr - start + 1];
		memcpy(buf, start, (ptr - start));
		buf[ptr - start] = '\0';
		ARG_SET_ADDRESS(&(func->arg2), gensym(buf));
	}else{
		// value
		char *endptr;
		float f = strtof(ptr, &endptr);
		t_atom a;
		atom_setfloat(&a, f);
		ARG_SET_ATOM(&(func->arg2), a);
		while(*ptr != ' '){
			ptr++;
		}
		ptr++;
	}
	//count--;
	return ptr - st;
}

char *oif_add_parens(t_oif *x, char *st){
}

t_atom oif_funcall(t_oif *x, t_func *func){
	t_atom arg1, arg2;
	t_atom *arg1p = NULL, *arg2p = NULL;
	int narg1 = 1, narg2 = 1;
	t_atom a;
	atom_setlong(&a, 0);
	switch(func->arg1.type){
	case OIF_ATOM:
		arg1 = func->arg1.w.atom;
		arg1p = &arg1;
		break;
	case OIF_FUNC:
		arg1 = oif_funcall(x, func->arg1.w.function);
		arg1p = &arg1;
		break;
	case OIF_ADDRESS:
		hashtab_lookup(x->ht, func->arg1.w.address, (t_object **)&arg1p);
		if(!arg1p){
			object_error((t_object *)x, "couldn't find data for OSC address %s", func->arg1.w.address->s_name);
			return a;
		}
		narg1 = atom_getlong(arg1p);
		arg1p++;
		break;
	}

	switch(func->arg2.type){
	case OIF_ATOM:
		arg2 = func->arg2.w.atom;
		arg2p = &arg2;
		break;
	case OIF_FUNC:
		arg2 = oif_funcall(x, func->arg2.w.function);
		arg2p = &arg2;
		break;
	case OIF_ADDRESS:
		hashtab_lookup(x->ht, func->arg2.w.address, (t_object **)&arg2p);
		if(!arg2p){
			object_error((t_object *)x, "couldn't find data for OSC address %s", func->arg2.w.address->s_name);
			return a;
		}
		narg2 = atom_getlong(arg2p);
		arg2p++;
		break;
	}

	if(narg1 == narg2 == 1){
		return func->f(*arg1p, *arg2p);
	}else if(narg1 == 1){
		int i;
		t_atom res;
		for(i = 0; i < narg2; i++){
			res = func->f(*arg1p, *arg2p++);
			if(atom_getlong(&res) == 0){
				break;
			}
		}
		return res;
		
	}else if(narg2 == 1){
		int i;
		t_atom res;
		for(i = 0; i < narg1; i++){
			res = func->f(*arg1p++, *arg2p);
			if(atom_getlong(&res) == 0){
				break;
			}
		}
		return res;
		
	}else{
		int min = narg1 < narg2 ? narg1 : narg2;
		int i;
		t_atom res;
		for(i = 0; i < min; i++){
			res = func->f(*arg1p++, *arg2p++);
			if(atom_getlong(&res) == 0){
				break;
			}
		}
		return res;
	}
}

t_atom oif_and(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) && atom_getfloat(&arg2)));
	return a;
}

t_atom oif_or(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) || atom_getfloat(&arg2)));
	return a;
}

t_atom oif_lt(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) < atom_getfloat(&arg2)));
	return a;
}

t_atom oif_lte(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) <= atom_getfloat(&arg2)));
	return a;
}

t_atom oif_gt(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) > atom_getfloat(&arg2)));
	return a;
}

t_atom oif_gte(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) >= atom_getfloat(&arg2)));
	return a;
}

t_atom oif_eq(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) == atom_getfloat(&arg2)));
	return a;
}

t_atom oif_neq(t_atom arg1, t_atom arg2){
	P2F(arg1, arg2);
	t_atom a;
	atom_setlong(&a, (atom_getfloat(&arg1) != atom_getfloat(&arg2)));
	return a;
}

void oif_assist(t_oif *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void oif_free(t_oif *x){
	object_free(x->proxy);
	if(x->atom_buf){
		free(x->atom_buf);
	}
}

void *oif_new(t_symbol *msg, short argc, t_atom *argv){
	t_oif *x;
	if(x = (t_oif *)object_alloc(oif_class)){
		x->outlets[1] = outlet_new((t_object *)x, "FullPacket");
		x->outlets[0] = outlet_new((t_object *)x, "FullPacket");
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		x->atom_buf = (t_atom *)calloc(1024, sizeof(t_atom));
		x->buflen = 1024;
		x->bufpos = 0;

		x->ht = hashtab_new(0);
		hashtab_flags(x->ht, OBJ_FLAG_DATA);

		if(argc){
			if(atom_gettype(argv) != A_SYM){
				object_error((t_object *)x, "expected a symbol as the first argument");
				return NULL;
			}
			if(*(atom_getsym(argv)->s_name) != '('){
				object_error((t_object *)x, "argument should begin with a \"(\"");
				return NULL;
			}
			x->function_string = atom_getsym(argv);
			oif_parse(x, atom_getsym(argv)->s_name, &(x->function_graph));
		}
		
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.if", (method)oif_new, (method)oif_free, sizeof(t_oif), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oif_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oif_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oif_notify, "notify", A_CANT, 0);

	class_register(CLASS_BOX, c);
	oif_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");
	return 0;
}

t_max_err oif_notify(t_oif *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
