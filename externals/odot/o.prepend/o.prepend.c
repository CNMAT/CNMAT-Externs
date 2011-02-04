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
NAME: o.prepend
DESCRIPTION: Prepend a string to the addresses in a bundle
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
version 1.0: Rewritten to only take one argument (the symbol to be prepended) which can be overridden by a symbol at the beginning of a mesage
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "libo/osc_util.h"
#include "libo/osc_match.h"
#include "omax_util.h"

typedef struct _oppnd{
	t_object ob;
	void *outlet;
	t_symbol *sym_to_prepend;
	char *buffer;
	int bufferLen;
	int bufferPos;
} t_oppnd;

struct context{
	char *buffer;
	int bufferLen;
	int bufferPos;
	t_symbol *sym_to_prepend;
};

void *oppnd_class;

void oppnd_fullPacket(t_oppnd *x, long len, long ptr);
void oppnd_doFullPacket(t_oppnd *x, long len, long ptr, t_symbol *sym_to_prepend);
void oppnd_cbk(t_osc_msg msg, void *v);
void oppnd_set(t_oppnd *x, t_symbol *sym_to_prepend);
void oppnd_anything(t_oppnd *x, t_symbol *msg, short argc, t_atom *argv);
void oppnd_free(t_oppnd *x);
void oppnd_assist(t_oppnd *x, void *b, long m, long a, char *s);
void *oppnd_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void oppnd_fullPacket(t_oppnd *x, long len, long ptr){
	oppnd_doFullPacket(x, len, ptr, x->sym_to_prepend);
}

void oppnd_doFullPacket(t_oppnd *x, long len, long ptr, t_symbol *sym_to_prepend){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	char buffer[len * 8];
	memset(buffer, '\0', len * 8);

	struct context c = {buffer + 16, len * 8, 16, sym_to_prepend};
	//x->buffer = buffer;
	//x->bufferLen = len * 8;
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

	memcpy(buffer, cpy, 16);
	x->bufferPos = 16;

	// extract the messages from the bundle
	//cmmjl_osc_extract_messages(nn, cpy, true, oppnd_cbk, (void *)x);
	osc_util_parseBundleWithCallback(nn, cpy, oppnd_cbk, (void *)&c);

	/*
	int i; 
	post("x->bufferPos = %d", c.bufferPos);
	for(i = 0; i < c.bufferPos; i++){
		post("%d %x %c", i, c.buffer[i], c.buffer[i]);
	}
	*/
	
	t_atom out[2];
	atom_setlong(&(out[0]), c.bufferPos);
	atom_setlong(&(out[1]), (long)buffer);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void oppnd_cbk(t_osc_msg msg, void *v){
	//t_oppnd *x = (t_oppnd *)v;
	struct context *c = (struct context *)v;

	char buf[strlen(msg.address) + strlen(c->sym_to_prepend->s_name) + 1];
	sprintf(buf, "%s%s", c->sym_to_prepend->s_name, msg.address);
	c->bufferPos += osc_util_rename(c->buffer, &msg, buf);
	/*
	*((long *)(x->buffer + x->bufferPos)) = hton32(msg.size);
	x->bufferPos += 4;
	memcpy(x->buffer + x->bufferPos, msg.address, msg.size);
	x->bufferPos += msg.size;
	*/
}

void oppnd_set(t_oppnd *x, t_symbol *sym_to_prepend){
	x->sym_to_prepend = sym_to_prepend;
}

void oppnd_anything(t_oppnd *x, t_symbol *msg, short argc, t_atom *argv){
	if(atom_gettype(argv) == A_SYM){
		if(atom_getsym(argv) == ps_FullPacket){
			oppnd_doFullPacket(x, atom_getlong(argv + 1), atom_getlong(argv + 2), msg);
			return;
		}else{
			t_symbol *sym_to_prepend = atom_getsym(argv);
			char buf[strlen(msg->s_name) + strlen(sym_to_prepend->s_name)];
			sprintf(buf, "%s%s", msg->s_name, sym_to_prepend->s_name);
			t_symbol *address = gensym(buf);
			int len = omax_util_get_bundle_size_for_atoms(gensym(buf), argc - 1, argv + 1);
			char oscbuf[len];
			memset(oscbuf, '\0', len);
			strncpy(oscbuf, "#bundle\0", 8);
			omax_util_encode_atoms(oscbuf + 16, address, argc - 1, argv + 1);
			t_atom out[2];
			atom_setlong(out, len);
			atom_setlong(out + 1, (long)oscbuf);
			outlet_anything(x->outlet, ps_FullPacket, 2, out);
			//outlet_anything(x->outlet, gensym(buf), argc - 1, argv + 1);
		}
	}else{
		char buf[strlen(msg->s_name) + strlen(x->sym_to_prepend->s_name)];
		sprintf(buf, "%s%s", x->sym_to_prepend->s_name, msg->s_name);
		t_symbol *address = gensym(buf);
		int len = omax_util_get_bundle_size_for_atoms(gensym(buf), argc, argv);
		char oscbuf[len];
		memset(oscbuf, '\0', len);
		strncpy(oscbuf, "#bundle\0", 8);
		omax_util_encode_atoms(oscbuf + 16, address, argc, argv);
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)oscbuf);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
}

void oppnd_assist(t_oppnd *x, void *b, long m, long a, char *s){
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

void oppnd_free(t_oppnd *x){
}

void *oppnd_new(t_symbol *msg, short argc, t_atom *argv){
	t_oppnd *x;
	if(x = (t_oppnd *)object_alloc(oppnd_class)){
		x->outlet = outlet_new(x, "FullPacket");
		x->sym_to_prepend = atom_getsym(argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.prepend", (method)oppnd_new, (method)oppnd_free, sizeof(t_oppnd), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oppnd_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oppnd_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oppnd_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oppnd_anything, "anything", A_GIMME, 0);

	class_addmethod(c, (method)oppnd_set, "set", A_SYM, 0);
    
	class_register(CLASS_BOX, c);
	oppnd_class = c;

	ps_FullPacket = gensym("FullPacket");
	common_symbols_init();
	return 0;
}
