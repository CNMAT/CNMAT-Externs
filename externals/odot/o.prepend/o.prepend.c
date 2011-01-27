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
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc_util.h"
#include "osc_match.h"

typedef struct _oppnd{
	t_object ob;
	void *outlet;
	t_symbol *sym_to_match, *sym_to_prepend;
	char *buffer;
	int bufferLen;
	int bufferPos;
	int shouldPrependAll;
} t_oppnd;

void *oppnd_class;

void oppnd_fullPacket(t_oppnd *x, long len, long ptr);
void oppnd_cbk(t_osc_msg msg, void *v);
void oppnd_set(t_oppnd *x, t_symbol *msg, int argc, t_atom *argv);
void oppnd_anything(t_oppnd *x, t_symbol *msg, short argc, t_atom *argv);
void oppnd_free(t_oppnd *x);
void oppnd_assist(t_oppnd *x, void *b, long m, long a, char *s);
void *oppnd_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void oppnd_fullPacket(t_oppnd *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	char buffer[len * 8];
	memset(buffer, '\0', len * 8);
	x->buffer = buffer;
	x->bufferLen = len * 8;
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
	osc_util_parseBundleWithCallback(nn, cpy, oppnd_cbk, (void *)x);
	/*
	int i; 
	post("x->bufferPos = %d", x->bufferPos);
	for(i = 0; i < x->bufferPos; i++){
		post("%d %x %c", i, x->buffer[i], x->buffer[i]);
	}
	*/
	t_atom out[2];
	atom_setlong(&(out[0]), x->bufferPos);
	atom_setlong(&(out[1]), (long)buffer);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void oppnd_cbk(t_osc_msg msg, void *v){
	t_oppnd *x = (t_oppnd *)v;
	int ret;
	int didmatch = 0;

	if(x->shouldPrependAll){
		char buf[strlen(msg.address) + strlen(x->sym_to_prepend->s_name) + 1];
		sprintf(buf, "%s%s", x->sym_to_prepend->s_name, msg.address);
		x->bufferPos += osc_util_rename(x->buffer, x->bufferLen, x->bufferPos, &msg, buf);
		didmatch++;
	}else{
		//if((ret = osc_match(x, msg.address, x->sym_to_match->s_name)) == -1){
		int po, ao;
		ret = osc_match(msg.address, x->sym_to_match->s_name, &po, &ao);
		if(ret == 3 || ret == 1){
			char buf[strlen(msg.address) + strlen(x->sym_to_prepend->s_name) + 1];
			sprintf(buf, "%s%s", x->sym_to_prepend->s_name, msg.address);
			x->bufferPos += osc_util_rename(x->buffer, x->bufferLen, x->bufferPos, &msg, buf);
			didmatch++;
		}
	}
	if(didmatch == 0){
		*((long *)(x->buffer + x->bufferPos)) = hton32(msg.size);
		x->bufferPos += 4;
		memcpy(x->buffer + x->bufferPos, msg.address, msg.size);
		x->bufferPos += msg.size;
	}
}

void oppnd_set(t_oppnd *x, t_symbol *msg, int argc, t_atom *argv){
	if(argc > 0){
		switch((*argv).a_type){
		case A_FLOAT:
			{
				char buf[256];
				sprintf(buf, "%f", (*argv).a_w.w_float);
				x->sym_to_prepend = gensym(buf);
			}
			break;
		case A_LONG:
			{
				char buf[256];
				sprintf(buf, "%ld", (*argv).a_w.w_long);
				x->sym_to_prepend = gensym(buf);
			}
			break;
		case A_SYM:
			x->sym_to_prepend = (*argv).a_w.w_sym;
			break;
		}
	}
}

void oppnd_anything(t_oppnd *x, t_symbol *msg, short argc, t_atom *argv){
	char buf[strlen(msg->s_name) + strlen(x->sym_to_prepend->s_name)];
	sprintf(buf, "%s%s", x->sym_to_prepend->s_name, msg->s_name);
	outlet_anything(x->outlet, gensym(buf), argc, argv);
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
		if(argc > 1){
			x->sym_to_match = atom_getsym(argv);
			x->shouldPrependAll = 0;
			x->sym_to_prepend = atom_getsym(argv + 1);
		}else{
			x->sym_to_prepend = atom_getsym(argv);
			x->sym_to_match = NULL;
			x->shouldPrependAll = 1;
		}
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.prepend", (method)oppnd_new, (method)oppnd_free, sizeof(t_oppnd), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oppnd_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oppnd_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oppnd_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oppnd_anything, "anything", A_GIMME, 0);

	class_addmethod(c, (method)oppnd_set, "set", A_GIMME, 0);
    
	class_register(CLASS_BOX, c);
	oppnd_class = c;

	ps_FullPacket = gensym("FullPacket");
	common_symbols_init();
	return 0;
}
