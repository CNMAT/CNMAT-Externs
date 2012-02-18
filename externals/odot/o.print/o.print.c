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
NAME: o.print
DESCRIPTION: Print the contents of an OSC bundle to the Max window
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2011
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "../odot_version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"
#include "osc_bundle_s.h"
#include "omax_util.h"

typedef struct _oprint{
	t_object ob;
	int print_msgsize;
	int print_typetags;
	t_symbol *myname;
} t_oprint;

void *oprint_class;

void oprint_fullPacket(t_oprint *x, long len, long ptr);
void oprint_cbk(t_osc_msg msg, void *v);
void oprint_int(t_oprint *x, long l);
void oprint_float(t_oprint *x, double f);
void oprint_anything(t_oprint *x, t_symbol *msg, int argc, t_atom *argv);
void oprint_list(t_oprint *x, t_symbol *msg, int argc, t_atom *argv);
void oprint_free(t_oprint *x);
void oprint_assist(t_oprint *x, void *b, long m, long a, char *s);
void *oprint_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oprint_notify(t_oprint *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oprint_fullPacket(t_oprint *x, long len, long ptr){
	osc_bundle_s_wrap_naked_message(len, ptr);
	long buflen = 0;
	char *buf = NULL;
	osc_bundle_s_format(len, (char *)ptr, &buflen, &buf);
	if(buflen == 0){
		post("<empty bundle>");
	}
	// the Max window doesn't respect newlines, so we have to do them manually
	char *start = buf;
	int i;
	for(i = 0; i < buflen; i++){
		if(buf[i] == '\n'){
			long n = ((buf + i) - start);
			char line[n + 1];
			memcpy(line, start, n);
			line[n] = '\0';
			post("%s: %s\n", x->myname->s_name, line);
			start = buf + i + 1;
		}
	}
}

void oprint_anything(t_oprint *x, t_symbol *msg, int argc, t_atom *argv)
{
	char *buf = NULL;
	long len = 0;
	atom_gettext(argc, argv, &len, &buf, 0);
	if(msg){
		post("%s: %s %s", x->myname->s_name, msg->s_name, buf);
	}else{
		post("%s: %s", x->myname->s_name, buf);
	}
	if(buf){
		sysmem_freeptr(buf);
	}
}

void oprint_list(t_oprint *x, t_symbol *msg, int argc, t_atom *argv)
{
	oprint_anything(x, NULL, argc, argv);
}

void oprint_int(t_oprint *x, long l)
{
	t_atom a;
	atom_setlong(&a, l);
	oprint_anything(x, NULL, 1, &a);
}

void oprint_float(t_oprint *x, double f)
{
	t_atom a;
	atom_setfloat(&a, f);
	oprint_anything(x, NULL, 1, &a);
}

void oprint_assist(t_oprint *x, void *b, long m, long a, char *s){
	if(m == ASSIST_INLET){
		switch (a) {	
		case 0:
			sprintf(s,"FullPacket");
			break;
		}
	}
}

void oprint_free(t_oprint *x){
}

void *oprint_new(t_symbol *msg, short argc, t_atom *argv){
	t_oprint *x;
	if(x = (t_oprint *)object_alloc(oprint_class)){
		x->myname = gensym("o.print");
		if(attr_args_offset(argc, argv) > 0 && argc > 0){
			char buf[128];
			switch(atom_gettype(argv)){
			case A_FLOAT:
				sprintf(buf, "%f", atom_getfloat(argv));
				x->myname = gensym(buf);
				break;
			case A_LONG:
				sprintf(buf, "%ld", atom_getlong(argv));
				x->myname = gensym(buf);
				break;
			case A_SYM:
				x->myname = atom_getsym(argv);
				break;
			}
		}
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.print", (method)oprint_new, (method)oprint_free, sizeof(t_oprint), 0L, A_GIMME, 0);
	class_addmethod(c, (method)oprint_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oprint_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oprint_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oprint_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oprint_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oprint_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)oprint_int, "int", A_LONG, 0);
	class_addmethod(c, (method)oprint_float, "float", A_FLOAT, 0);

	CLASS_ATTR_LONG(c, "printtypetags", 0, t_oprint, print_typetags);
	CLASS_ATTR_LONG(c, "printsize", 0, t_oprint, print_msgsize);

	class_register(CLASS_BOX, c);
	oprint_class = c;

	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
	ODOT_PRINT_VERSION;
	return 0;
}

t_max_err oprint_notify(t_oprint *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	/*
	t_symbol *attrname;
        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		}
	}
	*/
	return MAX_ERR_NONE;
}
