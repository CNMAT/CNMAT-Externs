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
  NAME: o.var
  DESCRIPTION: A variable to store OSC bundles
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
#include "omax_util.h"
#include "osc_util.h"

enum{
	OVAR_NONE,
	OVAR_DIFFERENCE,
	OVAR_INTERSECTION,
	OVAR_UNION
};

typedef struct _ovar{
	t_object ob;
	void *outlet;
	void *proxy;
	long inlet;
	char **buffers;
	int nbuffers;
	int *buffer_len;
	int *buffer_pos;
	int operation;
	t_hashtab *ht1, *ht2;
} t_ovar;

void *ovar_class;

void ovar_fullPacket(t_ovar *x, long len, long ptr);
void ovar_doFullPacket(t_ovar *x, long len, long ptr, long operation);
void ovar_cbk(t_osc_msg msg, void *v);
long ovar_hashtab_compute_bundle_size(t_ovar *x, t_hashtab *ht);
void ovar_hashtab_to_bundle(t_ovar *x, t_hashtab *ht, char *buf);
void ovar_clear(t_ovar *x);
void ovar_anything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);
void ovar_doanything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv, long operation);

void ovar_store(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);
void ovar_union(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);
void ovar_difference(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);
void ovar_intersection(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);

void ovar_bang(t_ovar *x);

void ovar_free(t_ovar *x);
void ovar_assist(t_ovar *x, void *b, long m, long a, char *s);
void *ovar_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err ovar_notify(t_ovar *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void ovar_fullPacket(t_ovar *x, long len, long ptr){
	ovar_doFullPacket(x, len, ptr, x->operation);
}

void ovar_doFullPacket(t_ovar *x, long len, long ptr, long operation){
	char cpy[len + 16];
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

	int inlet = proxy_getinlet((t_object *)x);

	hashtab_clear(x->ht2);
	// extract the messages from the bundle
	osc_util_parseBundleWithCallback(nn, cpy, ovar_cbk, (void *)x);

	if(inlet == 0){

		switch(operation){
		case OVAR_NONE:
			hashtab_clear(x->ht1);
		case OVAR_UNION:
			break;
		case OVAR_DIFFERENCE:
			{
				t_symbol **keys = NULL;
				long nkeys;
				hashtab_getkeys(x->ht1, &nkeys, &keys);
				int i;
				for(i = 0; i < nkeys; i++){
					t_atom *val = NULL;
					hashtab_lookup(x->ht2, keys[i], (t_object **)(&val));
					if(val){
						hashtab_delete(x->ht1, keys[i]);
						hashtab_delete(x->ht2, keys[i]);
					}
				}
				if(keys){
					sysmem_freeptr(keys);
				}
				keys = NULL;

				hashtab_getkeys(x->ht2, &nkeys, &keys);
				for(i = 0; i < nkeys; i++){
					t_atom *val = NULL;
					hashtab_lookup(x->ht1, keys[i], (t_object **)(&val));
					if(val){
						hashtab_delete(x->ht1, keys[i]);
						hashtab_delete(x->ht2, keys[i]);
					}
				}
				if(keys){
					sysmem_freeptr(keys);
				}
				keys = NULL;
			}
			break;
		case OVAR_INTERSECTION:
			{
				t_symbol **keys = NULL;
				long nkeys;
				hashtab_getkeys(x->ht1, &nkeys, &keys);
				int i;
				for(i = 0; i < nkeys; i++){
					t_atom *val = NULL;
					hashtab_lookup(x->ht2, keys[i], (t_object **)(&val));
					if(!val){
						hashtab_delete(x->ht1, keys[i]);
					}
				}
				if(keys){
					sysmem_freeptr(keys);
				}
				keys = NULL;

				hashtab_getkeys(x->ht2, &nkeys, &keys);
				for(i = 0; i < nkeys; i++){
					t_atom *val = NULL;
					hashtab_lookup(x->ht1, keys[i], (t_object **)(&val));
					if(!val){
						hashtab_delete(x->ht2, keys[i]);
					}
				}
				if(keys){
					sysmem_freeptr(keys);
				}
				keys = NULL;
			}
			break;
		}
	}


	t_symbol **keys = NULL;
	long nkeys;
	hashtab_getkeys(x->ht2, &nkeys, &keys);
	int i;
	for(i = 0; i < nkeys; i++){
		t_atom *val;
		hashtab_lookup(x->ht2, keys[i], (t_object **)(&val));
		int len = atom_getlong(val) + 1;
		t_atom *newval = (t_atom *)sysmem_newptr(len * sizeof(t_atom));
		memcpy(newval, val, len * sizeof(t_atom));
		hashtab_store(x->ht1, keys[i], (t_object *)newval);
	}
	if(keys){
		sysmem_freeptr(keys);
	}
	
	if(inlet == 0){
		int len = ovar_hashtab_compute_bundle_size(x, x->ht1);
		char buf[len];
		memset(buf, '\0', len);
		ovar_hashtab_to_bundle(x, x->ht1, buf);
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
}

void ovar_cbk(t_osc_msg msg, void *v){
	t_ovar *x = (t_ovar *)v;

	// omax_util_oscMsg2MaxAtoms() will stick the address in the first element
	// of the atom array, so allocate 1 more than the number of args
	//printf("%d\n", msg.argc);
	//printf("i want %d bytes\n", (msg.argc + 2) * sizeof(t_atom));
	t_atom *atoms = (t_atom *)sysmem_newptr((msg.argc + 2) * sizeof(t_atom));
	long len = msg.argc;

	// this will turn the osc mesage (a char array) into an array of atoms
	omax_util_oscMsg2MaxAtoms(&msg, &len, atoms + 1);
	atom_setlong(atoms, len);

	// the length in the first atom is the length of the arguments plus the address
	hashtab_store(x->ht2, atom_getsym(atoms + 1), (t_object *)atoms);
}

long ovar_hashtab_compute_bundle_size(t_ovar *x, t_hashtab *ht){
	t_symbol **keys = NULL;
	long nkeys;
	hashtab_getkeys(ht, &nkeys, &keys);
	long len = 16;
	int i;
	for(i = 0; i < nkeys; i++){
		t_atom *val;
		hashtab_lookup(ht, keys[i], (t_object **)&val);
		if(!val){
			continue;
		}
		len += 4; // size
		// address
		len += strlen(atom_getsym(val + 1)->s_name);
		len++;
		while(len % 4){
			len++;
		}
		// typetags
		len += 2 + atom_getlong(val);
		while(len % 4){
			len++;
		}
		int j;
		for(j = 0; j < atom_getlong(val) - 1; j++){
			t_atom *v = val + j + 2;
			switch(atom_gettype(v)){
			case A_FLOAT:
			case A_LONG:
				len += 4;
				break;
			case A_SYM:
				len += strlen(atom_getsym(v)->s_name);
				len++;
				while(len % 4){
					len++;
				}
				break;
			}
		}
	}
	
	if(keys){
		sysmem_freeptr(keys);
	}
	return len;
}

void ovar_hashtab_to_bundle(t_ovar *x, t_hashtab *ht, char *buf){
	char *ptr = buf;
	strncpy(ptr, "#bundle\0", 8);
	ptr += 16;

	t_symbol **keys = NULL;
	long nkeys;
	hashtab_getkeys(ht, &nkeys, &keys);
	int i;
	for(i = 0; i < nkeys; i++){
		t_atom *val = NULL;
		hashtab_lookup(ht, keys[i], (t_object **)&val);
		ptr += osc_util_make_bundle_from_atoms(atom_getlong(val), val + 1, &i, ptr);
	}
	
	if(keys){
		sysmem_freeptr(keys);
	}
}

void ovar_clear(t_ovar *x){
	hashtab_clear(x->ht1);
	hashtab_clear(x->ht2);
}

void ovar_anything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){
	ovar_doanything(x, msg, argc, argv, x->operation);
}

void ovar_doanything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv, long operation){
	t_symbol *address = NULL;
	if(msg){
		if(*(msg->s_name) != '/'){
			object_error((t_object *)x, "OSC address must begin with a '/'");
			return;
		}
		address = msg;
	}else{
		if(atom_gettype(argv) == A_SYM){
			if(*(atom_getsym(argv)->s_name) != '/'){
				object_error((t_object *)x, "OSC address must begin with a '/'");
				return;
			}
			address = atom_getsym(argv);
			argv++;
			argc--;
		}
	}
	if(!address){
		object_error((t_object *)x, "no OSC address found");
		return;
	}

	int len = omax_util_get_bundle_size_for_atoms(address, argc, argv);
	char buf[len];
	memset(buf, '\0', len);
	omax_util_encode_atoms(buf + 16, address, argc, argv);
	strncpy(buf, "#bundle\0", 8);
	ovar_doFullPacket(x, len, (long)buf, operation);
}

void ovar_store(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){
	if(atom_gettype(argv) == A_SYM){
		if(atom_getsym(argv) == ps_FullPacket){
			ovar_doFullPacket(x, atom_getlong(argv + 1), atom_getlong(argv + 2), OVAR_NONE);
			return;
		}else{
			ovar_doanything(x, NULL, argc, argv, OVAR_NONE);
		}
	}
}

void ovar_union(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){
	if(atom_gettype(argv) == A_SYM){
		if(atom_getsym(argv) == ps_FullPacket){
			ovar_doFullPacket(x, atom_getlong(argv + 1), atom_getlong(argv + 2), OVAR_UNION);
			return;
		}else{
			ovar_doanything(x, NULL, argc, argv, OVAR_UNION);
		}
	}
}

void ovar_difference(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){	
	if(atom_gettype(argv) == A_SYM){
		if(atom_getsym(argv) == ps_FullPacket){			
			ovar_doFullPacket(x, atom_getlong(argv + 1), atom_getlong(argv + 2), OVAR_DIFFERENCE);
			return;
		}else{
			ovar_doanything(x, NULL, argc, argv, OVAR_DIFFERENCE);
		}
	}
}

void ovar_intersection(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){
	if(atom_gettype(argv) == A_SYM){
		if(atom_getsym(argv) == ps_FullPacket){
			ovar_doFullPacket(x, atom_getlong(argv + 1), atom_getlong(argv + 2), OVAR_INTERSECTION);
			return;
		}else{
			ovar_doanything(x, NULL, argc, argv, OVAR_INTERSECTION);
		}
	}
}

void ovar_get_op(t_ovar *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc(argc, argv, &alloc);
	switch(x->operation){
	case OVAR_NONE:
		atom_setsym(*argv, gensym("store"));
		break;
	case OVAR_UNION:
		atom_setsym(*argv, gensym("union"));
		break;
	case OVAR_INTERSECTION:
		atom_setsym(*argv, gensym("intersection"));
		break;
	case OVAR_DIFFERENCE:
		atom_setsym(*argv, gensym("difference"));
		break;
	}
}

void ovar_set_op(t_ovar *x, t_object *attr, long argc, t_atom *argv){
	t_symbol *op = atom_getsym(argv);
	if(op == gensym("store")){
		x->operation = OVAR_NONE;
	}else if(op == gensym("union")){
		x->operation = OVAR_UNION;
	}else if(op == gensym("difference")){
		x->operation = OVAR_DIFFERENCE;
	}else if(op == gensym("intersection")){
		x->operation = OVAR_INTERSECTION;
	}else{
		object_error((t_object *)x, "unrecognized operation %s", atom_getsym(argv)->s_name);
		x->operation = OVAR_NONE;
	}
}

void ovar_bang(t_ovar *x){
	int len = ovar_hashtab_compute_bundle_size(x, x->ht1);
	char buf[len];
	memset(buf, '\0', len);
	ovar_hashtab_to_bundle(x, x->ht1, buf);
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)buf);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void ovar_assist(t_ovar *x, void *b, long m, long a, char *s){
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

void ovar_free(t_ovar *x){
	object_free(x->proxy);
	int i;
	if(x->buffers){
		for(i = 0; i < x->nbuffers; i++){
			if(x->buffers[i]){
				free(x->buffers[i]);
			}
		}
		free(x->buffers);
	}
	if(x->buffer_pos){
		free(x->buffer_pos);
	}
	if(x->buffer_len){
		free(x->buffer_len);
	}
}

void *ovar_new(t_symbol *msg, short argc, t_atom *argv){
	t_ovar *x;
	if(x = (t_ovar *)object_alloc(ovar_class)){
		x->outlet = outlet_new((t_object *)x, "FullPacket");
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		x->nbuffers = 2;
		x->buffers = (char **)calloc(x->nbuffers, sizeof(char*));
		x->buffer_len = (int *)calloc(x->nbuffers, sizeof(int));
		x->buffer_pos = (int *)calloc(x->nbuffers, sizeof(int));
		int i;
		for(i = 0; i < x->nbuffers; i++){
			x->buffers[i] = (char *)calloc(4096, sizeof(char));
			x->buffer_pos[i] = 0;
			x->buffer_len[i] = 4096;
		}

		x->ht1 = hashtab_new(0);
		hashtab_flags(x->ht1, OBJ_FLAG_MEMORY);
		x->ht2 = hashtab_new(0);
		hashtab_flags(x->ht2, OBJ_FLAG_MEMORY);
		
		x->operation = OVAR_NONE;

		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.var", (method)ovar_new, (method)ovar_free, sizeof(t_ovar), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)ovar_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)ovar_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)ovar_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)ovar_bang, "bang", 0);
	class_addmethod(c, (method)ovar_anything, "anything", A_GIMME, 0);

	class_addmethod(c, (method)ovar_store, "store", A_GIMME, 0);
	class_addmethod(c, (method)ovar_union, "union", A_GIMME, 0);
	class_addmethod(c, (method)ovar_difference, "difference", A_GIMME, 0);
	class_addmethod(c, (method)ovar_intersection, "intersection", A_GIMME, 0);

	class_addmethod(c, (method)ovar_clear, "clear", 0);

	CLASS_ATTR_SYM(c, "op", 0, t_ovar, operation);
	CLASS_ATTR_ACCESSORS(c, "op", (method)ovar_get_op, (method)ovar_set_op);

	class_register(CLASS_BOX, c);
	ovar_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");
	return 0;
}

t_max_err ovar_notify(t_ovar *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
