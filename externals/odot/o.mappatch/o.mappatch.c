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
NAME: o.mappatch
DESCRIPTION: Map a patch onto the contents of an OSC bundle
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 1.0: New name
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "omax_util.h"
#include "osc.h"
#include "osc_bundle_s.h"
#include "osc_bundle_iterator_s.h"
#include "osc_message_s.h"

typedef struct _omap{
	t_object ob;
	void *outlets[2];
	void *proxy;
	long inlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
	t_osc_msg_s *msg;
} t_omap;

void *omap_class;

void omap_fullPacket(t_omap *x, long len, long ptr);
void omap_cbk(t_osc_msg msg, void *v);
void omap_int(t_omap *x, long l);
void omap_float(t_omap *x, double f);
void omap_anything(t_omap *x, t_symbol *msg, short argc, t_atom *argv);
void omap_list(t_omap *x, t_symbol *msg, short argc, t_atom *argv);
void omap_free(t_omap *x);
void omap_assist(t_omap *x, void *b, long m, long a, char *s);
void *omap_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err omap_notify(t_omap *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void omap_fullPacket(t_omap *x, long len, long ptr){
	if(proxy_getinlet((t_object *)x) == 1){
		// I hope you meant to do this...
		memcpy(x->buffer + x->buffer_pos, ((char *)ptr) + 16, len - 16);
		x->buffer_pos += len - 16;
		return;
	}
	memset(x->buffer, '\0', x->buffer_len);
	memcpy(x->buffer, (char *)ptr, 16);
	x->buffer_pos = 16;

	//osc_bundle_getMessagesWithCallback(len, (char *)ptr, omap_cbk, (void *)x);
	t_osc_bndl_it_s *it = osc_bndl_it_s_get(len, (char *)ptr);
	while(osc_bndl_it_s_hasNext(it)){
		t_osc_msg_s *msg = osc_bndl_it_s_next(it);
		x->msg = msg;
		//long len = osc_message_s_getArgCount(msg);
		long len = omax_util_getNumAtomsInOSCMsg(msg);
		t_atom atoms[len];
		omax_util_oscMsg2MaxAtoms(msg, atoms);
		outlet_anything(x->outlets[1], atom_getsym(atoms), len - 1, atoms + 1);
	}

	t_atom out[2];
	//lock
	int buffer_pos = x->buffer_pos;
	x->buffer_pos = 0;
	char buffer[buffer_pos];
	memcpy(buffer, x->buffer, buffer_pos);
	//unlock
	atom_setlong(out, buffer_pos);
	atom_setlong(out + 1, (long)buffer);
	outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
}
/*
void omap_cbk(t_osc_msg msg, void *v){
	t_omap *x = (t_omap *)v;
	x->msg = msg;
	long len = msg.argc;
	t_atom atoms[len + 1];
	//cmmjl_osc_get_data_atoms(&msg, atoms);
	omax_util_oscMsg2MaxAtoms(&msg, &len, atoms);
	outlet_anything(x->outlets[1], atom_getsym(atoms), msg.argc, atoms + 1);
}
*/

void omap_int(t_omap *x, long l){
	if(proxy_getinlet((t_object *)x) == 0){
		return;
	}
	t_atom a;
	atom_setlong(&a, l);
	omap_list(x, NULL, 1, &a);
}

void omap_float(t_omap *x, double f){
	if(proxy_getinlet((t_object *)x) == 0){
		return;
	}
	t_atom a;
	atom_setfloat(&a, f);
	omap_list(x, NULL, 1, &a);
}

void domap_int(t_omap *x, long l){

}

void omap_anything(t_omap *x, t_symbol *msg, short argc, t_atom *argv){
	if(msg){
		t_atom a[argc + 1];
		atom_setsym(a, msg);
		memcpy(a + 1, argv, argc * sizeof(t_atom));
		omap_list(x, NULL, argc + 1, a);
	}else{
		omap_list(x, NULL, argc, argv);
	}
}

void omap_list(t_omap *x, t_symbol *msg, short argc, t_atom *argv){
	if(proxy_getinlet((t_object *)x) != 1){
		// not sure what to do with this...
		return;
	}
	if(atom_gettype(argv) == A_SYM){
		t_symbol *address = atom_getsym(argv);
		if(*(address->s_name) == '/'){
			x->buffer_pos += omax_util_encode_atoms(x->buffer + x->buffer_pos, address, argc - 1, argv + 1);
			return;
		}
	}
	t_symbol *address = gensym(osc_message_s_getAddress(x->msg));
	t_atom av[argc + 1];
	atom_setsym(av, address);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	x->buffer_pos += omax_util_encode_atoms(x->buffer + x->buffer_pos, address, argc, argv);
}

void omap_assist(t_omap *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		switch(a){
		case 0:
			sprintf(s, "FullPacket: results of the mapping");
			break;
		case 1:
			sprintf(s, "OSC-style max messages contained in the bundle");
			break;
		}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"FullPacket");
			break;
		case 1:
			sprintf(s, "Messages from the original bundle to be included in the new bundle");
			break;
		}
	}
}

void omap_free(t_omap *x){
	object_free(x->proxy);
}

void *omap_new(t_symbol *msg, short argc, t_atom *argv){
	t_omap *x;
	if(x = (t_omap *)object_alloc(omap_class)){
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
	t_class *c = class_new("o.mappatch", (method)omap_new, (method)omap_free, sizeof(t_omap), 0L, A_GIMME, 0);
	class_addmethod(c, (method)omap_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)omap_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)omap_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)omap_int, "int", A_LONG, 0);
	class_addmethod(c, (method)omap_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)omap_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)omap_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)omap_notify, "notify", A_CANT, 0);

	class_register(CLASS_BOX, c);
	omap_class = c;

	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
	return 0;
}

t_max_err omap_notify(t_omap *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	/*
	t_symbol *attrname;
        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		}
	}
	*/
	return MAX_ERR_NONE;
}
