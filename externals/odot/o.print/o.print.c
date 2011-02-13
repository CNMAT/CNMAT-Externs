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

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "omax_util.h"
#include "liboio/osc_util.h"
#include "liboio/osc_match.h"

typedef struct _oprint{
	t_object ob;
	int print_msgsize;
	int print_typetags;
} t_oprint;

void *oprint_class;

void oprint_fullPacket(t_oprint *x, long len, long ptr);
void oprint_cbk(t_osc_msg msg, void *v);
void oprint_int(t_oprint *x, long l);
void oprint_float(t_oprint *x, double f);
void oprint_anything(t_oprint *x, t_symbol *msg, short argc, t_atom *argv);
void oprint_list(t_oprint *x, t_symbol *msg, short argc, t_atom *argv);
void oprint_free(t_oprint *x);
void oprint_assist(t_oprint *x, void *b, long m, long a, char *s);
void *oprint_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oprint_notify(t_oprint *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oprint_fullPacket(t_oprint *x, long len, long ptr){
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
	post("[ 0x%x", ntoh64(*((uint64 *)(cpy + 8))));
	osc_util_parseBundleWithCallback(nn, cpy, oprint_cbk, (void *)x);
	post("]");
}

void oprint_cbk(t_osc_msg msg, void *v){
	t_oprint *x = (t_oprint *)v;
	long len = msg.argc;
	t_atom atoms[len + 1];
	omax_util_oscMsg2MaxAtoms(&msg, &len, atoms);
	//char buf[1024];
	char *buf = sysmem_newptr(1024);
	int bufsize = 1024;
	int bufpos = 0;
	if(x->print_msgsize){
		bufpos += sprintf(buf, "\t\t(%d) %s", msg.size, msg.address);
	}else{
		bufpos += sprintf(buf, "\t\t%s", msg.address);
	}
	if(x->print_typetags){
		bufpos += sprintf(buf + bufpos, " %s", msg.typetags);
	}
	int i;
	for(i = 1; i < len + 1; i++){
		if(bufsize - bufpos < 64){
			buf = sysmem_resizeptr(buf, bufsize + 256);
			bufsize += 256;
		}
		switch(atom_gettype(atoms + i)){
		case A_LONG:
			bufpos += sprintf(buf + bufpos, " %ld", atom_getlong(atoms + i));
			break;
		case A_FLOAT:
			bufpos += sprintf(buf + bufpos, " %f", atom_getfloat(atoms + i));
			break;
		case A_SYM:
			bufpos += sprintf(buf + bufpos, " %s", atom_getsym(atoms + i)->s_name);
			break;
		}
	}
	post("%s", buf);
	sysmem_freeptr(buf);
}

void oprint_assist(t_oprint *x, void *b, long m, long a, char *s){
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

void oprint_free(t_oprint *x){
}

void *oprint_new(t_symbol *msg, short argc, t_atom *argv){
	t_oprint *x;
	if(x = (t_oprint *)object_alloc(oprint_class)){
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

	CLASS_ATTR_LONG(c, "printtypetags", 0, t_oprint, print_typetags);
	CLASS_ATTR_LONG(c, "printsize", 0, t_oprint, print_msgsize);

	class_register(CLASS_BOX, c);
	oprint_class = c;

	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
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
