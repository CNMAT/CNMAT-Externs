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
VERSION 1.0: Uses flex and bison to do the lexing/parsing
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"

#ifdef OIF
#undef NAME
#define NAME "o.if"
#endif

#ifndef WIN_VERSION
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "omax_util.h"
#include "omax_expr.h"
#include "omax_parser.h"
#include "osc.h"
#include "osc_mem.h"
#include "osc_atom_u.h"

#include <libgen.h>
#ifdef WIN_VERSION
#include <windows.h>
#else
#include <dlfcn.h>
#endif

extern int yyparse (int argc, t_atom *argv, int *argp, t_omax_expr **f);
extern void omax_scanner_scan_atom_array(int, t_atom *, int *, t_atom **);

typedef struct _oexpr{
	t_object ob;
#ifdef OIF
	void *outlets[2];
#else
	void *outlet;
	t_symbol *address;
	int addresslen;
#endif
	t_omax_expr *function_graph;
	t_atom *argvlex;
	int argclex;

	t_object *ed;
	char *cbuf;
	int cbuf_size, cbuf_len;
	t_symbol *path_to_maxsdk;
} t_oexpr;

void *oexpr_class;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr);
void oexpr_free(t_oexpr *x);
void oexpr_assist(t_oexpr *x, void *b, long m, long a, char *s);
void *oexpr_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr){
#ifdef OIF
	int argc = 0;
	t_osc_atom_ar_u *argv = NULL;
	t_atom out[2];
	// we don't actually want to do this copy here.  we need to 
	// have another version of omax_expr_funcall that doesn't do 
	// assignment
	char *copy = (char *)osc_mem_alloc(len);
	memcpy(copy, (char *)ptr, len);
	int ret = omax_expr_funcall(x->function_graph, &len, &copy, &argv);
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)copy);
	if(ret){
		outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
	}else{
		int i;
		for(i = 0; i < argc; i++){
			if(osc_atom_u_getDouble(osc_atom_array_u_get(argv, i)) == 0){
				outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
				goto out;
			}
		}
		outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
	}
 out:
	if(argv){
		osc_mem_free(argv);
	}
	if(copy){
		free(copy);
	}
#else
	// we need to make a copy incase the expression contains assignment that will
	// alter the bundle.
	// the copy needs to use memory allocated with osc_mem_alloc in case the 
	// bundle has to be resized during assignment
	char *copy = (char *)osc_mem_alloc(len);
	memcpy(copy, (char *)ptr, len);
	int argc = 0;
	//t_atom64 *argv = NULL;
	t_osc_atom_ar_u *argv = NULL;
	t_atom out[2];
	//int ret = omax_expr_funcall(x->function_graph, &len, &copy, &argc, &argv);
	int ret = omax_expr_funcall(x->function_graph, &len, &copy, &argv);
	if(ret){
		atom_setlong(out, len);
		atom_setlong(out + 1, ptr);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}else{
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)copy);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
		/*
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
		int size = osc_message_getSize(newm);
		char newm_ser[size + 4];
		memset(newm_ser, '\0', size + 4);
		osc_message_serialize(newm, newm_ser);
		osc_bundle_lookupAddress_s(len, copy, x->address->s_name, &m, 1);
		char bndl_out[len + size + 4];
		memset(bndl_out, '\0', len + size + 4);
		if(m){
			int firstpart = (m->address - 4) - copy;
			memcpy(bndl_out, copy, firstpart);
			memcpy(bndl_out + firstpart, newm_ser, size + 4);
			memcpy(bndl_out + firstpart + size + 4, copy + firstpart + m->size + 4, len - (firstpart + m->size + 4));
			atom_setlong(out, len + size - m->size);
			atom_setlong(out + 1, (long)bndl_out);
			outlet_anything(x->outlet, ps_FullPacket, 2, out);
			osc_message_free(m);
		}else{
			memcpy(bndl_out, copy, len);
			memcpy(bndl_out + len, newm_ser, size + 4);
			atom_setlong(out, len + size + 4);
			atom_setlong(out + 1, (long)bndl_out);
			outlet_anything(x->outlet, ps_FullPacket, 2, out);
		}
		osc_message_free(newm);
		*/
	}
	if(argv){
		osc_atom_array_u_free(argv);
	}
	if(copy){
		osc_mem_free(copy);
	}
#endif
}

int oexpr_postFunctionGraph_r(t_omax_expr *fg, char *buf){
	char *ptr = buf;
	ptr += sprintf(ptr, "(%s ", fg->rec->name);
	t_omax_expr_arg *f_argv = fg->argv;
	while(f_argv){
		switch(f_argv->type){
		case OMAX_ARG_TYPE_ATOM:
			{
				t_atom a = f_argv->arg.atom;
				switch(atom_gettype(&a)){
				case A_LONG:
					ptr += sprintf(ptr, "%ld ", atom_getlong(&a));
					break;
				case A_FLOAT:
					ptr += sprintf(ptr, "%f ", atom_getfloat(&a));
					break;
				case A_SYM:
					ptr += sprintf(ptr, "%s ", atom_getsym(&a)->s_name);
					break;
				}
			}
			break;
		case OMAX_ARG_TYPE_OSCADDRESS:
			ptr += sprintf(ptr, "%s ", f_argv->arg.osc_address);
			break;
		case OMAX_ARG_TYPE_EXPR:
			ptr += oexpr_postFunctionGraph_r(f_argv->arg.expr, ptr);
			break;
		}
		f_argv = f_argv->next;
	}
	ptr--;
	ptr += sprintf(ptr, ") ");
	return ptr - buf;
}

void oexpr_postFunctionGraph(t_omax_expr *fg){
	char buf[256];
	char *ptr = buf;
	t_omax_expr *f = fg;
	while(f){
		oexpr_postFunctionGraph_r(f, ptr);
		post("%s", buf);
		f = f->next;
	}
}

void oexpr_bang(t_oexpr *x){
	char buf[16];
	strncpy(buf, "#bundle\0", 8);
	oexpr_fullPacket(x, 16, (long)buf);
}

void oexpr_anything(t_oexpr *x, t_symbol *msg, int argc, t_atom *argv){
	if(msg == gensym("post-argvlex")){
		int i;
		for(i = 0; i < x->argclex; i++){
			postatom(x->argvlex + i);
		}
	}else if(msg == gensym("post-functiongraph")){
		oexpr_postFunctionGraph(x->function_graph);
	}else{
		object_error((t_object *)x, "doesn't respond to message %s", msg->s_name);
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
		if(omax_expr_funcsym[i].numargs < 0){
			post("%s(): %s", omax_expr_funcsym[i].name, omax_expr_funcsym[i].desc);
		}else if(omax_expr_funcsym[i].numargs == 0){
			post("%s(...): %s", omax_expr_funcsym[i].name, omax_expr_funcsym[i].desc);
		}else{
			char buf[256];
			char *ptr = buf;
			ptr += sprintf(ptr, "%s(", omax_expr_funcsym[i].name);
			int j;
			for(j = 0; j < omax_expr_funcsym[i].numargs; j++){
				ptr += sprintf(ptr, "arg%d ", j + 1);
			}
			*(--ptr) = '\0';
			post("%s): %s", buf, omax_expr_funcsym[i].desc);
		}
	}
}

/**************************************************
editor and compilation
**************************************************/

int oexpr_write_template(t_oexpr *x, char *buf){
	if(!buf){
		return 0;
	}
	int len = 0;
	//len += sprintf(buf + len, "#include \"ext.h\"\n");

	return len;
}

void oexpr_edclose(t_oexpr *x, char **text, long size){
	printf("%s\n", __func__);
	if(size + 1 > x->cbuf_len){
		char *tmp = (char *)osc_mem_resize(x->cbuf, (size + 1) * sizeof(char));
		if(tmp){
			x->cbuf = tmp;
			x->cbuf_size = size + 1;
		}else{
			object_error((t_object *)x, "out of memory!");
			return;
		}
	}
	strncpy(x->cbuf, *text, size);
	x->cbuf[size] = '\0';
	x->cbuf_len = size + 1;

	x->ed = NULL;
}

void oexpr_okclose(t_oexpr *x, char *s, short *result){
	*result = 3;
}

void oexpr_open_editor(t_oexpr *x){
	// this should be done only if the file was actually modified--otherwise, just use the buffer

	if(x->cbuf_len == 0){
		x->cbuf_len = oexpr_write_template(x, x->cbuf);
	}

	if(x->ed){
		object_method(x->ed, gensym("settext"), x->cbuf, gensym("utf-8"));
	}else{
		x->ed = (t_object *)object_new(CLASS_NOBOX, gensym("jed"), (t_object *)x, 0);
		object_method(x->ed, gensym("settext"), x->cbuf, gensym("utf-8"));
	}

	object_attr_setchar(x->ed, gensym("visible"), 1);
}

void oexpr_dblclick(t_oexpr *x){
	oexpr_open_editor(x);
}

/**************************************************
**************************************************/

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
	if(x->argvlex){
		osc_mem_free(x->argvlex);
	}
}

t_max_err oexpr_notify(t_oexpr *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}

t_max_err oexpr_cbuf_set(t_oexpr *x, t_object *attr, long argc, t_atom *argv){
	if(argc == 0){
		return 1;
	}
	if(x->cbuf_size < argc){
		x->cbuf = osc_mem_resize(x->cbuf, argc);
		x->cbuf_size = argc;
	}
	int i;
	for(i = 0; i < argc; i++){
		x->cbuf[i] = (char)atom_getlong(argv + i);
	}
	x->cbuf_len = argc;
	return 0;
}

t_max_err oexpr_cbuf_get(t_oexpr *x, t_object *attr, long *argc, t_atom **argv){
	char alloc = 0;
	atom_alloc_array(x->cbuf_len, argc, argv, &alloc);
	int i;
	for(i = 0; i < x->cbuf_len; i++){
		atom_setlong((*argv) + i, x->cbuf[i]);
	}
	return 0;
}


void *oexpr_new(t_symbol *msg, short argc, t_atom *argv){
	t_oexpr *x;
	if(x = (t_oexpr *)object_alloc(oexpr_class)){
#ifdef OIF
		x->outlets[1] = outlet_new((t_object *)x, "FullPacket");
		x->outlets[0] = outlet_new((t_object *)x, "FullPacket");
#else
		x->outlet = outlet_new((t_object *)x, "FullPacket");
		x->address = gensym("/result");
#endif
		if(argc){
			omax_scanner_scan_atom_array(attr_args_offset(argc, argv), argv, &(x->argclex), &(x->argvlex));
			x->function_graph = NULL;
			omax_expr_parse(&(x->function_graph), x->argclex, x->argvlex);
		}
		attr_args_process(x, argc, argv);
#ifndef OIF
		x->addresslen = strlen(x->address->s_name);
#endif

		x->cbuf_size = 4096;
		x->cbuf = (char *)osc_mem_alloc(x->cbuf_size);
		x->cbuf_len = 0;
	}
		   	
	return(x);
}

int main(void){
#ifdef OIF
	char *buf = "o.if";
#else
	char *buf = "o.expr";
#endif
	t_class *c = class_new(buf, (method)oexpr_new, (method)oexpr_free, sizeof(t_oexpr), 0L, A_GIMME, 0);
	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oexpr_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oexpr_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oexpr_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oexpr_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oexpr_bang, "bang", 0);

	class_addmethod(c, (method)oexpr_postFunctions, "post-functions", 0);
	class_addmethod(c, (method)oexpr_postConstants, "post-constants", 0);
	/*
	class_addmethod(c, (method)oexpr_edclose, "edclose", A_CANT, 0); 
	//class_addmethod(c, (method)oexpr_edsave, "edsave", A_CANT, 0); 
	class_addmethod(c, (method)oexpr_dblclick, "dblclick", A_CANT, 0);
	class_addmethod(c, (method)oexpr_okclose, "okclose", A_CANT, 0);  
	*/

#ifndef OIF
	//CLASS_ATTR_SYM(c, "as", 0, t_oexpr, address);
#endif

	/*
	CLASS_ATTR_SYM(c, "maxsdk", 0, t_oexpr, path_to_maxsdk);
	CLASS_ATTR_SAVE(c, "maxsdk", 0);

	CLASS_ATTR_CHAR_ARRAY(c, "cbuf", 0, t_oexpr, cbuf, 1);
	CLASS_ATTR_ACCESSORS(c, "cbuf", oexpr_cbuf_get, oexpr_cbuf_set);
	CLASS_ATTR_SAVE(c, "cbuf", 0);
	*/
	class_register(CLASS_BOX, c);
	oexpr_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");

#ifdef WIN_VERSION
	srand(GetTickCount());
#else
	srand(mach_absolute_time());
#endif

	version(0);
	return 0;
}

