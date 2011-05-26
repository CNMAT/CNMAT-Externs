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
VERSION 1.0: Uses flex and bison
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h"
#include "omax_util.h"
#include "omax_expr.h"
#include "omax_parser.h"
#include "osc.h"

extern int yyparse (int argc, t_atom *argv, int *argp, t_omax_expr **f);
extern void omax_scanner_scan_atom_array(int, t_atom *, int *, t_atom **);

typedef struct _oif{
	t_object ob;
	void *outlets[2];
	t_omax_expr *function_graph;
} t_oif;

void *oif_class;

void oif_fullPacket(t_oif *x, long len, long ptr);
void oif_free(t_oif *x);
void oif_assist(t_oif *x, void *b, long m, long a, char *s);
void *oif_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oif_notify(t_oif *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oif_fullPacket(t_oif *x, long len, long ptr){
	int argc = 0;
	t_atom64 *argv = NULL;
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, ptr);
	if(omax_expr_funcall(x->function_graph, len, (char *)ptr, &argc, &argv)){
		outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
	}else{
		int i;
		for(i = 0; i < argc; i++){
			if(atom64_getfloat(argv + i) == 0){
				outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
				return;
			}
		}
		outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
	}
}

void oif_postConstants(t_oif *x){
	int i;
	for(i = 0; i < sizeof(omax_expr_constsym) / sizeof(t_omax_expr_const_rec); i++){
		post("%s: %s (%f)", omax_expr_constsym[i].name, omax_expr_constsym[i].desc, omax_expr_constsym[i].val);
	}
}

void oif_postFunctions(t_oif *x){
	int i;
	for(i = 0; i < sizeof(omax_expr_funcsym) / sizeof(t_omax_expr_rec); i++){
		post("%s: %s", omax_expr_funcsym[i].name, omax_expr_funcsym[i].desc);
	}
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
	omax_expr_free(x->function_graph);
}

void *oif_new(t_symbol *msg, short argc, t_atom *argv){
	t_oif *x = NULL;
	if(x = (t_oif *)object_alloc(oif_class)){
		x->outlets[1] = outlet_new((t_object *)x, "FullPacket");
		x->outlets[0] = outlet_new((t_object *)x, "FullPacket");
		if(argc){
			int argclex = 0;
			t_atom *argvlex = NULL;
			omax_scanner_scan_atom_array(argc, argv, &argclex, &argvlex);
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
			attr_args_process(x, argc, argv);
		}
	}
	return x;
}

int main(void){
	t_class *c = class_new("o.if", (method)oif_new, (method)oif_free, sizeof(t_oif), 0L, A_GIMME, 0);
    	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oif_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oif_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oif_notify, "notify", A_CANT, 0);

	class_addmethod(c, (method)oif_postFunctions, "post-functions", 0);
	class_addmethod(c, (method)oif_postConstants, "post-constants", 0);

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
