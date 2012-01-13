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
NAME: o.collect
DESCRIPTION: Collect OSC messages and bundles
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
#include "osc_mem.h"
#include "osc_bundle_s.h"
#include "omax_util.h"

typedef struct _ocoll{
	t_object ob;
	void *outlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
} t_ocoll;

void *ocoll_class;

void ocoll_fullPacket(t_ocoll *x, long len, long ptr);
void ocoll_anything(t_ocoll *x, t_symbol *msg, int argc, t_atom *argv);
void ocoll_bang(t_ocoll *x);
void ocoll_free(t_ocoll *x);
void ocoll_assist(t_ocoll *x, void *b, long m, long a, char *s);
void *ocoll_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err ocoll_notify(t_ocoll *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void ocoll_fullPacket(t_ocoll *x, long len, long ptr){
	if(len == OSC_HEADER_SIZE){
		// empty bundle
		return;
	}
	if(x->buffer_pos + len > x->buffer_len){
		char *tmp = (char *)realloc(x->buffer, x->buffer_pos + len);
		if(!tmp){
			object_error((t_object *)x, "Out of memory...sayonara max...");
			return;
		}
		x->buffer = tmp;
		memset(x->buffer + x->buffer_pos, '\0', len);
		x->buffer_len = x->buffer_pos + len;
	}

	if(x->buffer_pos == 0){
		memcpy(x->buffer, (char *)ptr, len);
		x->buffer_pos += len;
	}else{
		memcpy(x->buffer + x->buffer_pos, (char *)ptr + 16, len - 16);
		x->buffer_pos += len - 16;
	}	
}

void ocoll_anything(t_ocoll *x, t_symbol *msg, int argc, t_atom *argv){
	int len;
	len = omax_util_get_bundle_size_for_atoms(msg, argc, argv);
	char buf[len];
	memset(buf, '\0', len);
	omax_util_encode_atoms(buf + OSC_HEADER_SIZE, msg, argc, argv);
	osc_bundle_s_setBundleID(buf);
	ocoll_fullPacket(x, len, (long)buf);
}

void ocoll_bang(t_ocoll *x){
	if(x->buffer_pos > 16){
		t_atom out[2];
		atom_setlong(out, x->buffer_pos);
		int len = x->buffer_pos;
		char outbuf[len];
		memcpy(outbuf, x->buffer, len);
		memset(x->buffer, '\0', len);
		x->buffer_pos = 0;
		atom_setlong(out + 1, (long)outbuf);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
}

void ocoll_assist(t_ocoll *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"OSC bundles to collect.");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"An OSC bundle with all collectd messages.");
			break;
		}
	}
}

void ocoll_free(t_ocoll *x){
	if(x->buffer){
		free(x->buffer);
	}
}

void *ocoll_new(t_symbol *msg, short argc, t_atom *argv){
	t_ocoll *x;
	if(x = (t_ocoll *)object_alloc(ocoll_class)){
		x->outlet = outlet_new((t_object *)x, NULL);
		x->buffer_len = 1024;
		if(argc){
			if(atom_gettype(argv) == A_LONG){
				//x->buffer_len = atom_getlong(argv);
				object_error((t_object *)x, "o.collultate no longer takes an argument to specify its internal buffer size.");
				object_error((t_object *)x, "The buffer will expand as necessary.");
			}
		}
		x->buffer = (char *)osc_mem_alloc(x->buffer_len * sizeof(char));
		memset(x->buffer, '\0', x->buffer_len);
		x->buffer_pos = 0;
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.collect", (method)ocoll_new, (method)ocoll_free, sizeof(t_ocoll), 0L, A_GIMME, 0);
	class_addmethod(c, (method)ocoll_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)ocoll_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)ocoll_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)ocoll_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)ocoll_bang, "bang", 0);

	class_register(CLASS_BOX, c);
	ocoll_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");
	ODOT_PRINT_VERSION;
	return 0;
}

t_max_err ocoll_notify(t_ocoll *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
