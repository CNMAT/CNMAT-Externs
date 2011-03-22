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
NAME: o.accumulate
DESCRIPTION: Accumulate OSC messages and bundles
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
#include "osc.h"
#include "omax_util.h"

typedef struct _oaccum{
	t_object ob;
	void *outlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
} t_oaccum;

void *oaccum_class;

void oaccum_fullPacket(t_oaccum *x, long len, long ptr);
void oaccum_anything(t_oaccum *x, t_symbol *msg, int argc, t_atom *argv);
void oaccum_cbk(t_osc_msg msg, void *v);
void oaccum_bang(t_oaccum *x);
void oaccum_free(t_oaccum *x);
void oaccum_assist(t_oaccum *x, void *b, long m, long a, char *s);
void *oaccum_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err oaccum_notify(t_oaccum *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void oaccum_fullPacket(t_oaccum *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;

	// if the OSC packet contains a single message, turn it into a bundle
	/*
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = osc_util_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}
	*/
	
	// flatten any nested bundles
	//nn = osc_util_flatten(nn, cpy, cpy);

	if(x->buffer_pos + nn > x->buffer_len){
		/*
		x->buffer = sysmem_resizeptr(x->buffer, x->buffer_pos + nn);
		if(!x->buffer){
			object_error((t_object *)x, "Out of memory...sayonara max...");
			return;
		}
		x->buffer_len = x->buffer_pos + nn;
		*/
		oaccum_bang(x);
	}

	if(x->buffer_pos == 0){
		memcpy(x->buffer, cpy, nn);
		x->buffer_pos += nn;
	}else{
		memcpy(x->buffer + x->buffer_pos, cpy + 16, nn - 16);
		x->buffer_pos += nn - 16;
	}	

	// extract the messages from the bundle
	//osc_util_parseBundleWithCallback(nn, cpy, oaccum_cbk, (void *)x);
}

void oaccum_anything(t_oaccum *x, t_symbol *msg, int argc, t_atom *argv){
	int len;
	len = omax_util_get_bundle_size_for_atoms(msg, argc, argv);
	char buf[len];
	memset(buf, '\0', len);
	omax_util_encode_atoms(buf + 16, msg, argc, argv);
	strncpy(buf, "#bundle\0", 8);
	oaccum_fullPacket(x, len, (long)buf);
}

void oaccum_bang(t_oaccum *x){
	if(x->buffer_pos > 16){
		t_atom out[2];
		atom_setlong(out, x->buffer_pos);
		char outbuf[x->buffer_pos];
		memcpy(outbuf, x->buffer, x->buffer_pos);
		x->buffer_pos = 0;
		atom_setlong(out + 1, (long)outbuf);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
}

void oaccum_assist(t_oaccum *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"OSC bundles to accumulate.");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"An OSC bundle with all accumulated messages.");
			break;
		}
	}
}

void oaccum_free(t_oaccum *x){
	if(x->buffer){
		free(x->buffer);
	}
}

void *oaccum_new(t_symbol *msg, short argc, t_atom *argv){
	t_oaccum *x;
	if(x = (t_oaccum *)object_alloc(oaccum_class)){
		x->outlet = outlet_new((t_object *)x, NULL);
		x->buffer_len = 1024;
		if(argc){
			if(atom_gettype(argv) == A_LONG){
				x->buffer_len = atom_getlong(argv);
			}
		}
		x->buffer = (char *)malloc(x->buffer_len * sizeof(char));
		memset(x->buffer, '\0', x->buffer_len);
		x->buffer_pos = 0;
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.accumulate", (method)oaccum_new, (method)oaccum_free, sizeof(t_oaccum), 0L, A_GIMME, 0);
    	osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oaccum_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oaccum_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oaccum_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oaccum_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oaccum_bang, "bang", 0);

	class_register(CLASS_BOX, c);
	oaccum_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");
	version(0);
	return 0;
}

t_max_err oaccum_notify(t_oaccum *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
