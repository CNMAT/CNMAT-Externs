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
NAME: o.expr
DESCRIPTION: C-like expressions that operate on OSC bundles
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
#include "omax_expr.h"
#include "omax_parser.h"
#include "osc.h"

extern int yyparse (int argc, t_atom *argv, int *argp, t_omax_expr **f);
extern void omax_scanner_scan_atom_array(int, t_atom *, int *, t_atom **);

typedef struct _oexpr{
	t_object ob;
	void *outlet;
	t_omax_expr *function_graph;
	t_symbol *address;
	int addresslen;
} t_oexpr;

void *oexpr_class;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr);
void oexpr_free(t_oexpr *x);
void oexpr_assist(t_oexpr *x, void *b, long m, long a, char *s);
void *oexpr_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oexpr_notify(t_oexpr *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr){
	int argc = 0;
	t_atom64 *argv = NULL;
	t_atom out[2];
	if(omax_expr_funcall(x->function_graph, len, (char *)ptr, &argc, &argv)){
		atom_setlong(out, len);
		atom_setlong(out + 1, ptr);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}else{
		t_osc_msg *m = NULL;
		t_osc_msg *newm = osc_message_alloc();
		newm->free_internal_buffers = 1;
		osc_message_setAddress(newm, x->address->s_name);
		float argvf[argc];
		int i;
		for(i = 0; i < argc; i++){
			//argvf[i] = (float)argv[i];
			argvf[i] = atom64_getfloat(argv + i);
		}
		char tt[argc];
		memset(tt, 'f', argc);
		osc_message_addData(newm, argc, tt, argc * sizeof(float), (char *)argvf);
		int size = osc_message_getSerializedSize(newm);
		char newm_ser[size + 4];
		memset(newm_ser, '\0', size + 4);
		osc_message_serialize(newm, newm_ser);
		osc_bundle_lookupAddressSerialized(len, (char *)ptr, x->address->s_name, &m, 1);
		char bndl_out[len + size + 4];
		memset(bndl_out, '\0', len + size + 4);
		if(m){
			int firstpart = (m->address - 4) - ((char *)ptr);
			memcpy(bndl_out, (char *)ptr, firstpart);
			memcpy(bndl_out + firstpart, newm_ser, size + 4);
			memcpy(bndl_out + firstpart + size + 4, ((char *)ptr) + firstpart + m->size + 4, len - (firstpart + m->size + 4));
			atom_setlong(out, len + size - m->size);
			atom_setlong(out + 1, (long)bndl_out);
			outlet_anything(x->outlet, ps_FullPacket, 2, out);
			osc_message_free(m);
		}else{
			memcpy(bndl_out, (char *)ptr, len);
			memcpy(bndl_out + len, newm_ser, size + 4);
			atom_setlong(out, len + size + 4);
			atom_setlong(out + 1, (long)bndl_out);
			outlet_anything(x->outlet, ps_FullPacket, 2, out);
		}
		osc_message_free(newm);
	}
	if(argv){
		osc_mem_free(argv);
	}
}

void oexpr_postConstants(t_oexpr *x){
	int i;
	for(i = 0; i < sizeof(omax_expr_constsym) / sizeof(t_omax_expr_const_rec); i++){
		post("%s: %s (%f)", omax_expr_constsym[i].name, omax_expr_constsym[i].desc, omax_expr_constsym[i].val);
	}
}

void oexpr_postFunctions(t_oexpr *x){
	int i;
	for(i = 0; i < sizeof(omax_expr_funcsym) / sizeof(t_omax_expr_rec); i++){
		post("%s: %s", omax_expr_funcsym[i].name, omax_expr_funcsym[i].desc);
	}
}

void oexpr_assist(t_oexpr *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}else{
		switch (a) {	
		case 0:
			break;
		}
	}
}

void oexpr_free(t_oexpr *x){
	omax_expr_free(x->function_graph);
}


void *oexpr_new(t_symbol *msg, short argc, t_atom *argv){
	t_oexpr *x;
	if(x = (t_oexpr *)object_alloc(oexpr_class)){
		x->outlet = outlet_new((t_object *)x, "FullPacket");
		//x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		if(argc){
			int argclex = 0;
			t_atom *argvlex = NULL;
			omax_scanner_scan_atom_array(attr_args_offset(argc, argv), argv, &argclex, &argvlex);
			/*
			int i;
			for(i = 0; i < argclex; i++){
				postatom(argvlex + i);
			}
			post("**************************************************");
			*/
			int counter = 0;
			yyparse(argclex, argvlex, &counter, &(x->function_graph));
			if(argvlex){
				osc_mem_free(argvlex);
			}
		}
		x->address = gensym("/result");
		x->addresslen = strlen(x->address->s_name);
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.expr", (method)oexpr_new, (method)oexpr_free, sizeof(t_oexpr), 0L, A_GIMME, 0);
	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oexpr_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oexpr_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oexpr_notify, "notify", A_CANT, 0);

	class_addmethod(c, (method)oexpr_postFunctions, "post-functions", 0);
	class_addmethod(c, (method)oexpr_postConstants, "post-constants", 0);

	CLASS_ATTR_SYM(c, "as", 0, t_oexpr, address);

	class_register(CLASS_BOX, c);
	oexpr_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");
	return 0;
}

t_max_err oexpr_notify(t_oexpr *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
