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
NAME: o.route
DESCRIPTION: OSC-route for full packets
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
#include "omax_util.h"
#include "osc_match.h"

#define MAX_NUM_MESSAGES 1024

typedef struct _oroute_message{
	t_osc_msg msg;
	int outlet_num;
	int full_match;
	int offset;
	struct _oroute_message *next, *prev;
} t_oroute_message;

typedef struct _oroute{
	t_object ob;
	void **outlets;
	t_symbol **args;
	int numArgs;
	// we have an array of preallocated messages that we'll use, but we'll
	// organize them in a linked list as we accumulate them so that we can 
	// just rip through that list when it's time to output them later
	t_oroute_message *messages;
	t_oroute_message *message_buf;
	int numMessages, message_buf_len;
	//t_cmmjl_osc_message *matched, *unmatched;
	//int *numCharsMatched;
	//int *matched_outlets;
	int numMatched, numUnmatched;
	int bundlePartialMatches;
} t_oroute;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_fp_bundlePartialMatches(t_oroute *x, long len, char *ptr);
void oroute_fp(t_oroute *x, long len, char *ptr);
void oroute_cbk(t_osc_msg msg, void *context);
void oroute_insert_msg(t_oroute *x, t_oroute_message *msg);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);


void oroute_fullPacket(t_oroute *x, long len, long ptr){
	/*
	int i;
	for(i = 0; i < len; i++){
		printf("%d: %c (0x%x)\n", i, ((char *)ptr)[i], ((char *)ptr)[i]);
	}
	*/
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = osc_util_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}
	nn = osc_util_flatten(nn, cpy, cpy);
	x->messages = NULL;
	x->numMessages = 0;
	osc_util_parseBundleWithCallback(nn, cpy, oroute_cbk, (void *)x);
	if(x->bundlePartialMatches){
		oroute_fp_bundlePartialMatches(x, nn, cpy);
	}else{
		oroute_fp(x, nn, cpy);
	}
}


void oroute_fp_bundlePartialMatches(t_oroute *x, long len, char *ptr){
	t_oroute_message *m = x->messages;
	t_atomarray *a;
	t_atom *argv;
	long argc;
	char buf[len], *bufp = buf;
	memset(buf, '\0', len);
	memcpy(buf, ptr, 16);
	bufp += 16;
	int last_outlet_num = -1;
	while(m){
		if(last_outlet_num != m->outlet_num && last_outlet_num >= 0 && bufp - buf > 16){
			t_atom out[2];
			atom_setlong(out, bufp - buf);
			atom_setlong(out + 1, (long)buf);
			outlet_anything(x->outlets[m->outlet_num], gensym("FullPacket"), 2, out);
			bufp = buf + 16;
			memset(bufp, '\0', len - 16);
		}
		if(m->full_match){
			a = omax_util_oscMsg2MaxAtoms(&(m->msg));
			atomarray_getatoms(a, &argc, &argv);
			outlet_list(x->outlets[m->outlet_num], NULL, argc - 1, argv + 1);
			atomarray_clear(a);
		}else{
			char *size = bufp;
			bufp += 4;
			strcpy(bufp, m->msg.address + m->offset);
			bufp += strlen(m->msg.address + m->offset);
			bufp++;
			while((bufp - buf) % 4){
				bufp++;
			}
			memcpy(bufp, m->msg.typetags, m->msg.size - (m->msg.typetags - m->msg.address));
			bufp += m->msg.size - (m->msg.typetags - m->msg.address);
			*((uint32_t *)size) = hton32(bufp - size - 4);
		}
		last_outlet_num = m->outlet_num;
		m = m->next;
	}
	if(bufp - buf > 16){
		t_atom out[2];
		atom_setlong(out, bufp - buf);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(x->outlets[last_outlet_num], gensym("FullPacket"), 2, out);
	}
}

void oroute_fp(t_oroute *x, long len, char *ptr){
	t_oroute_message *m = x->messages;
	t_atomarray *a = NULL;
	t_atom *argv;
	long argc;
	while(m){
	        a = omax_util_oscMsg2MaxAtoms(&(m->msg));
		if(m->full_match){
			atomarray_getatoms(a, &argc, &argv);
			outlet_list(x->outlets[m->outlet_num], NULL, argc - 1, argv + 1);
			atomarray_clear(a);
		}else{
			atomarray_getatoms(a, &argc, &argv);
			if(argc){
				outlet_anything(x->outlets[m->outlet_num], gensym(((atom_getsym(argv)->s_name) + m->offset)), argc - 1, argv + 1);
				atomarray_clear(a);
			}
		}
		m = m->next;
	}
}

void oroute_cbk(t_osc_msg msg, void *context){
	if(!(msg.address)){
		return;
	}
	t_oroute *x = (t_oroute *)context;
	int i;
	int match = 0;
	for(i = 0; i < x->numArgs; i++){
		int po, ao;
		int ret = osc_match(msg.address, x->args[i]->s_name, &po, &ao);
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && ((ret & OSC_MATCH_PATTERN_COMPLETE) || (msg.address[po] == '/'))){
			x->message_buf[x->numMessages].msg = msg;
			x->message_buf[x->numMessages].full_match = 0;
			x->message_buf[x->numMessages].offset = po;
			if(ret == 3){
				x->message_buf[x->numMessages].full_match = 1;
			}
			x->message_buf[x->numMessages].outlet_num = i;
			oroute_insert_msg(x, &(x->message_buf[x->numMessages]));
			match = 1;
			x->numMessages++;
		}
	}
	if(!match){
		x->message_buf[x->numMessages].msg = msg;
		x->message_buf[x->numMessages].full_match = 0;
		x->message_buf[x->numMessages].offset = 0;
		x->message_buf[x->numMessages].outlet_num = x->numArgs;
		oroute_insert_msg(x, &(x->message_buf[x->numMessages]));
		x->numMessages++;
	}
	/*
	t_oroute *x = (t_oroute *)v;
	int i;
	int ret;
	int didmatch = 0;
	for(i = 0; i < x->numArgs; i++){
		char *arg = x->args[i]->s_name;
		int l1 = strlen(msg.address);
		int l2 = strlen(arg);
		ret = cmmjl_osc_match(x, msg.address, arg);
		if(ret != 0){
			if(((ret < l1 && ret < l2) || (ret == l1 && ret < l2)) && ret != -1){
				continue;
			}
			x->matched_outlets[x->numMatched] = i;	
			x->numCharsMatched[x->numMatched] = ret;
			x->matched[x->numMatched++] = msg;
			didmatch += 1;
		}
	}
	if(didmatch == 0){
		x->unmatched[x->numUnmatched++] = msg;
	}
	*/
}

void oroute_insert_msg(t_oroute *x, t_oroute_message *message){
	t_oroute_message *msg = message;
	t_oroute_message *m = x->messages;
	t_oroute_message *prev = NULL;
	msg->next = NULL;
	msg->prev = NULL;
	if(!m){
		x->messages = msg;
		return;
	}
	while(m){
		if(m->outlet_num <= msg->outlet_num){
			break;
		}
		prev = m;
		m = m->next;
	}
	if(!m){
		m = msg;
		msg = prev;
	}
	msg->next = m;
	msg->prev = NULL;
	if(m){
		msg->prev = m->prev;
		if(!(m->prev)){
			x->messages = msg;
		}
		m->prev = msg;
	}
	//post("%p <-- %p --> %p", msg->prev, msg, msg->next);
}

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
	/*
	int len = cmmjl_osc_get_msg_length_max(msg, argc, argv);
	if(len == 0){
		error("o.route: problem formatting bundle");
		return;
	}
	char buf[len];
	memset(buf, '\0', len);
	cmmjl_osc_copy_max_messages(msg, argc, argv, len, buf);

	oroute_doFullPacket(x, len, (long)buf, 0);
	*/
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s){
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

void oroute_free(t_oroute *x){
	if(x->args){
		free(x->args);
	}
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv){
	t_oroute *x;
	int i;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		//cmmjl_init(x, NAME, 0);
		int numArgs = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((numArgs + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(numArgs * sizeof(t_symbol *));
		x->numArgs = numArgs;
		x->message_buf = (t_oroute_message *)sysmem_newptr(MAX_NUM_MESSAGES * sizeof(t_oroute_message));
		x->messages = NULL;
		x->outlets[numArgs] = outlet_new(x, "FullPacket"); // unmatched outlet
		for(i = 0; i < numArgs; i++){
			x->outlets[numArgs - 1 - i] = outlet_new(x, NULL);
			x->args[i] = atom_getsym(argv + i);
			//x->matched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			//x->numCharsMatched = (int *)malloc(1024 * sizeof(int));
			//x->unmatched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			//x->matched_outlets = (int *)malloc(1024 * sizeof(int));
			//x->numMatched = 0;
			//x->numUnmatched = 0;
		}
		x->bundlePartialMatches = 1;
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.route", (method)oroute_new, (method)oroute_free, sizeof(t_oroute), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oroute_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oroute_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oroute_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oroute_anything, "anything", A_GIMME, 0);

	CLASS_ATTR_LONG(c, "bundlePartialMatches", 0, t_oroute, bundlePartialMatches);
    
	class_register(CLASS_BOX, c);
	oroute_class = c;

	common_symbols_init();
	return 0;
}
