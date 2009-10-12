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
NAME: o.apply
DESCRIPTION: Apply a funcction to the contents of an OSC packet
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_osc.h"
//#include "cmmjl/cmmjl_osc_obj.h"

typedef struct _oapply{
	t_object ob;
	void *outlets[2];
	void *proxy;
	long inlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
	t_cmmjl_osc_message msg;
} t_oapply;

void *oapply_class;

void oapply_fullPacket(t_oapply *x, long len, long ptr);
void oapply_cbk(t_cmmjl_osc_message msg, void *v);
void oapply_int(t_oapply *x, long l);
void oapply_float(t_oapply *x, double f);
void oapply_anything(t_oapply *x, t_symbol *msg, short argc, t_atom *argv);
void oapply_list(t_oapply *x, t_symbol *msg, short argc, t_atom *argv);
void oapply_free(t_oapply *x);
void oapply_assist(t_oapply *x, void *b, long m, long a, char *s);
void *oapply_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oapply_notify(t_oapply *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void oapply_fullPacket(t_oapply *x, long len, long ptr){
	if(proxy_getinlet((t_object *)x) == 1){
		return;
	}
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	memcpy(x->buffer, cpy, 16);
	x->buffer_pos = 16;

	// extract the messages from the bundle
	cmmjl_osc_extract_messages(nn, cpy, true, oapply_cbk, (void *)x);

	t_atom out[2];
	atom_setlong(out, x->buffer_pos);
	atom_setlong(out + 1, (long)x->buffer);
	outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
	/*
	int i;
	for(i = 0; i < x->buffer_pos; i++){
		post("%d %c 0x%x", i, x->buffer[i], x->buffer[i]);
	}
	*/
}

void oapply_cbk(t_cmmjl_osc_message msg, void *v){
	t_oapply *x = (t_oapply *)v;
	x->msg = msg;
	t_atom atoms[msg.argc];
	cmmjl_osc_get_data_atoms(&msg, atoms);
	outlet_anything(x->outlets[1], gensym(msg.address), msg.argc, atoms);
}

void oapply_int(t_oapply *x, long l){
	if(proxy_getinlet((t_object *)x) == 0){
		return;
	}
	t_atom a;
	atom_setlong(&a, l);
	oapply_list(x, NULL, 1, &a);
}

void oapply_float(t_oapply *x, double f){
	if(proxy_getinlet((t_object *)x) == 0){
		return;
	}
	t_atom a;
	atom_setfloat(&a, f);
	oapply_list(x, NULL, 1, &a);
}

void doapply_int(t_oapply *x, long l){

}

void oapply_anything(t_oapply *x, t_symbol *msg, short argc, t_atom *argv){
	if(msg){
		t_atom a[argc + 1];
		atom_setsym(a, msg);
		memcpy(a + 1, argv, argc * sizeof(t_atom));
		oapply_list(x, NULL, argc + 1, a);
	}else{
		oapply_list(x, NULL, argc, argv);
	}
}

void oapply_list(t_oapply *x, t_symbol *msg, short argc, t_atom *argv){
	if(atom_gettype(argv) == A_SYM){
		t_symbol *address = atom_getsym(argv);
		if(*(address->s_name) == '/'){
			x->buffer_pos += cmmjl_osc_make_bundle_from_atoms(argc, argv, &(x->buffer_len), x->buffer + x->buffer_pos);
			return;
		}
	}
	t_symbol *address = gensym(x->msg.address);
	t_atom av[argc + 1];
	atom_setsym(av, address);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	x->buffer_pos += cmmjl_osc_make_bundle_from_atoms(argc + 1, av, &(x->buffer_len), x->buffer + x->buffer_pos);


	/*
	if(atom_gettype(argv) == A_SYM){
		t_symbol *sym = atom_getsym(argv);
		if(*(sym->s_name) == '/'){
			// full osc address
			char *sizeptr = x->buffer + x->buffer_pos;
			char *bufptr = x->buffer + x->buffer_pos + 4;
			strcpy(bufptr, sym->s_name);
			bufptr += strlen(sym->s_name);
			bufptr++;
			while((bufptr - sizeptr) % 4){
				*bufptr++ = '\0';
			}
			*bufptr++ = ',';
			char *ttptr = bufptr;
			bufptr += (argc - 1);
			while((bufptr - sizeptr) % 4){
				*bufptr++ = '\0';
			}
			int i;
			for(i = 1; i < argc - 1; i++){
				switch(atom_gettype(argv + i)){
				case A_FLOAT:
					*ttptr++ = 'f';
					*((long *)bufptr) = htonl(*((long *)(&(argv[i].a_w.w_float))));
					bufptr += 4;
					break;
				case A_LONG:
					*ttptr++ = 'i';
					*((long *)bufptr) = htonl(atom_getlong(argv + i));
					bufptr += 4;
					break;
				case A_SYM:
					strcpy(bufptr, atom_getsym(argv + i)->s_name);
					bufptr += strlen(bufptr);
					while((bufptr - sizeptr) % 4){
						*bufptr++ = '\0';
					}
					*ttptr++ = 's';
					break;
				}
			}
			*((long *)sizeptr) = htonl(bufptr - sizeptr - 4);
			x->buffer_pos += bufptr - sizeptr;
		}else{
			// just the data
		}
	}
	*/
}

void oapply_assist(t_oapply *x, void *b, long m, long a, char *s){
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

void oapply_free(t_oapply *x){
	object_free(x->proxy);
}

void *oapply_new(t_symbol *msg, short argc, t_atom *argv){
	t_oapply *x;
	if(x = (t_oapply *)object_alloc(oapply_class)){
		cmmjl_init(x, NAME, 0);
		x->outlets[1] = outlet_new((t_object *)x, NULL);
		x->outlets[0] = outlet_new((t_object *)x, "FullPacket");
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		x->buffer = (char *)calloc(4096, sizeof(char));
		x->buffer_len = 4096;
		x->buffer_pos = 0;

		//object_attach_byptr_register(x, x, CLASS_BOX);
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.apply", (method)oapply_new, (method)oapply_free, sizeof(t_oapply), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oapply_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oapply_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oapply_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oapply_int, "int", A_LONG, 0);
	class_addmethod(c, (method)oapply_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)oapply_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oapply_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)oapply_notify, "notify", A_CANT, 0);

	class_register(CLASS_BOX, c);
	oapply_class = c;

	common_symbols_init();
	return 0;
}

t_max_err oapply_notify(t_oapply *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	/*
	t_symbol *attrname;
        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		}
	}
	*/
	return MAX_ERR_NONE;
}
