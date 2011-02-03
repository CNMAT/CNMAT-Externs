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
NAME: o.spew
DESCRIPTION: Spew out the contents of an OSC bundle as Max messages
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
#include "ext_critical.h"
#include "omax_util.h"
#include "osc_match.h"

#define MAX_NUM_MESSAGES 1024

#define PF __PRETTY_FUNCTION__

typedef struct _ospew_message{
	t_osc_msg msg;
	int outlet_num;
	int full_match;
	int offset;
	struct _ospew_message *next, *prev;
} t_ospew_message;

// local workspace to avoid having to synchronize
// or use critical regions
typedef struct _ospew_wksp{
	t_symbol **args;
	int numArgs;
	t_ospew_message *messages;
	t_ospew_message *message_buf;
	int numMessages, message_buf_len;
} t_ospew_wksp;

typedef struct _ospew{
	t_object ob;
	void **outlets;
	t_critical lock;
	t_symbol **args;
	int numArgs;
	// we have an array of preallocated messages that we'll use, but we'll
	// organize them in a linked list as we accumulate them so that we can 
	// just rip through that list when it's time to output them later
	t_ospew_message *messages;
	t_ospew_message *message_buf;
	int numMessages, message_buf_len;
	int max_message; // set this to note that the event originated as a max message and not a FullPacket
} t_ospew;

void *ospew_class;

void ospew_fullPacket(t_ospew *x, long len, long ptr);
void ospew_fp(t_ospew *x, long len, char *ptr, t_ospew_message *m);
void ospew_cbk(t_osc_msg msg, void *context);
void ospew_insert_msg(t_ospew_wksp *x, t_ospew_message *msg);
void ospew_anything(t_ospew *x, t_symbol *msg, short argc, t_atom *argv);
void ospew_free(t_ospew *x);
void ospew_assist(t_ospew *x, void *b, long m, long a, char *s);
void *ospew_new(t_symbol *msg, short argc, t_atom *argv);


void ospew_fullPacket(t_ospew *x, long len, long ptr){
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
	//osc_util_printBundle(len, cpy, printf);
	t_ospew_wksp wksp;
	t_symbol *args[x->numArgs];
	t_ospew_message mm[128];
	memset(mm, '\0', 128 * sizeof(t_ospew_message));
	wksp.message_buf = mm;
	wksp.messages = NULL;
	wksp.numMessages = 0;
	wksp.message_buf_len = 128;
	critical_enter(x->lock);
	memcpy(args, x->args, x->numArgs * sizeof(t_symbol *));
	wksp.args = args;
	wksp.numArgs = x->numArgs;
	critical_exit(x->lock);
	osc_util_parseBundleWithCallback(nn, cpy, ospew_cbk, (void *)&wksp);

	ospew_fp(x, nn, cpy, wksp.messages);

	x->max_message = 0;
}

/*
void ospew_fp_bundlePartialMatches(t_ospew *x, long len, char *ptr, t_ospew_message *m){
	if(!m){
		return;
	}
	t_atom *argv = (t_atom *)sysmem_newptr(128 * sizeof(t_atom));
	long argc = 0;
	printf("len = %d\n", len);
	char buf[len], *bufp = buf;
	memset(buf, '\0', len);
	memcpy(buf, ptr, 16);
	bufp += 16;
	int last_outlet_num = -1;
	//printf("*******************************************\n");
	while(m->next){
		m = m->next;
	}
	while(m){
		//printf("m = %p\n", m);
		if(last_outlet_num != m->outlet_num && last_outlet_num >= 0 && bufp - buf > 16){
			t_atom out[2];
			atom_setlong(out, bufp - buf);
			atom_setlong(out + 1, (long)buf);
			outlet_anything(x->outlets[last_outlet_num], gensym("FullPacket"), 2, out);
			bufp = buf + 16;
			memset(bufp, '\0', len - 16);
		}
		if(m->full_match){
			omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
			if(argc == 1){
				outlet_bang(x->outlets[m->outlet_num]);
			}else{
				outlet_atoms(x->outlets[m->outlet_num], argc - 1, argv + 1);
			}
		}else if(x->max_message){
			omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
			outlet_anything(x->outlets[m->outlet_num], gensym(atom_getsym(argv)->s_name + m->offset), argc - 1, argv + 1);
		}else{
			char *size = bufp;
			bufp += 4;
			strcpy(bufp, m->msg.address + m->offset);
			bufp += strlen(m->msg.address + m->offset);
			bufp++;
			while((bufp - size) % 4){
				bufp++;
			}
			if(m->msg.typetags){
				memcpy(bufp, m->msg.typetags, m->msg.size - (m->msg.typetags - m->msg.address));
				bufp += m->msg.size - (m->msg.typetags - m->msg.address);
			}else{
				*bufp++ = ',';
				*bufp++ = '\0';
				*bufp++ = '\0';
				*bufp++ = '\0';				
			}
			*((uint32_t *)size) = hton32(bufp - size - 4);
		}
		last_outlet_num = m->outlet_num;
		m = m->prev;
	}
	if(bufp - buf > 16){
		t_atom out[2];
		atom_setlong(out, bufp - buf);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(x->outlets[last_outlet_num], gensym("FullPacket"), 2, out);
	}
	sysmem_freeptr(argv);
}
*/

void ospew_fp(t_ospew *x, long len, char *ptr, t_ospew_message *m){
	if(!m){
		return;
	}
	t_atom *argv = (t_atom *)sysmem_newptr(128 * sizeof(t_atom));
	long argc;
	while(m->next){
		m = m->next;
	}
	while(m){
		omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
		outlet_anything(x->outlets[m->outlet_num], atom_getsym(argv), argc - 1, argv + 1);
		m = m->prev;
	}
	sysmem_freeptr(argv);
}

void ospew_cbk(t_osc_msg msg, void *context){
	if(!(msg.address)){
		return;
	}
	t_ospew_wksp *x = (t_ospew_wksp *)context;
	int i;
	int match = 0;
	for(i = 0; i < x->numArgs; i++){
		int po, ao;
		int ret = osc_match(msg.address, x->args[i]->s_name, &po, &ao);

		int star_at_end = 0;
		if(msg.address[po] == '*' && msg.address[po + 1] == '\0'){
			star_at_end = 1;
		}
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && (ret & OSC_MATCH_PATTERN_COMPLETE)){
			x->message_buf[x->numMessages].msg = msg;
			x->message_buf[x->numMessages].full_match = 0;
			x->message_buf[x->numMessages].offset = po;
			if(ret == 3 || star_at_end == 1){
				x->message_buf[x->numMessages].full_match = 1;
			}
			x->message_buf[x->numMessages].outlet_num = i;
			ospew_insert_msg(x, &(x->message_buf[x->numMessages]));
			match = 1;
			x->numMessages++;
		}else{
		}
	}
	if(!match){
		x->message_buf[x->numMessages].msg = msg;
		x->message_buf[x->numMessages].full_match = 0;
		x->message_buf[x->numMessages].offset = 0;
		x->message_buf[x->numMessages].outlet_num = x->numArgs;
		ospew_insert_msg(x, &(x->message_buf[x->numMessages]));
		x->numMessages++;
	}
}

void ospew_insert_msg(t_ospew_wksp *x, t_ospew_message *message){
	t_ospew_message *msg = message;
	t_ospew_message *m = x->messages;
	t_ospew_message *prev = NULL;
	msg->next = NULL;
	msg->prev = NULL;

	if(!m){
		x->messages = msg;
		return;
	}
	if(m->outlet_num >= msg->outlet_num){
		msg->next = x->messages;
		m->prev = msg;
		x->messages = msg;
		return;
	}
	while(m){
		if(msg->outlet_num <= m->outlet_num){
			break;
		}
		prev = m;
		m = m->next;
	}
	if(!m){
		m = prev;
		m->next = msg;
		msg->prev = m;
	}else{
		//post("%s %d <= %d %s", msg->msg.address, msg->outlet_num, m->outlet_num, m->msg.address);
		msg->next = m;
		msg->prev = m->prev;
		if(m->prev){
			m->prev->next = msg;
		}
		m->prev = msg;
	}
}

void ospew_anything(t_ospew *x, t_symbol *msg, short argc, t_atom *argv){
	if(!msg){
		object_error((t_object *)x, "doesn't look like an OSC message");
		return;
	}
	if(msg->s_name[0] != '/'){
		object_error((t_object *)x, "OSC addresses must begin with a /");
		return;
	}
	char *buffer = (char *)sysmem_newptr(1024);
	memset(buffer, '\0', 1024);
	t_atom av[argc + 1];
	atom_setsym(av, msg);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	long len = 1024;
	//len = osc_util_make_bundle_from_atoms(argc + 1, av, &len, buffer);
	strncpy(buffer, "#bundle\0", 8);
	*((long long *)(buffer + 8)) = hton64(1ll);
	len = omax_util_encode_atoms(buffer + 16, msg, argc, argv);
	x->max_message = 1;

	ospew_fullPacket(x, len + 16, (long)buffer);

	/*
	int len = cmmjl_osc_get_msg_length_max(msg, argc, argv);
	if(len == 0){
		error("o.spew: problem formatting bundle");
		return;
	}
	char buf[len];
	memset(buf, '\0', len);
	cmmjl_osc_copy_max_messages(msg, argc, argv, len, buf);

	ospew_doFullPacket(x, len, (long)buf, 0);
	*/
}

void ospew_set(t_ospew *x, long index, t_symbol *sym){
	if(index < 1 || index > x->numArgs){
		object_error((t_object *)x, "index (%d) out of bounds", index);
		return;
	}
	x->args[index - 1] = sym;
}

void ospew_assist(t_ospew *x, void *b, long m, long a, char *s){
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

void ospew_free(t_ospew *x){
	critical_free(x->lock);
	if(x->args){
		free(x->args);
	}
}

void *ospew_new(t_symbol *msg, short argc, t_atom *argv){
	t_ospew *x;
	int i;
	if(x = (t_ospew *)object_alloc(ospew_class)){
		int numArgs = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((numArgs + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(numArgs * sizeof(t_symbol *));
		x->numArgs = numArgs;
		//x->message_buf = (t_ospew_message *)sysmem_newptr(MAX_NUM_MESSAGES * sizeof(t_ospew_message));
		x->messages = NULL;
		critical_new(&(x->lock));
		x->outlets[numArgs] = outlet_new(x, "FullPacket"); // unmatched outlet
		for(i = 0; i < numArgs; i++){
			x->outlets[numArgs - 1 - i] = outlet_new(x, NULL);
			x->args[i] = atom_getsym(argv + i);
		}

		//attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.spew", (method)ospew_new, (method)ospew_free, sizeof(t_ospew), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)ospew_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)ospew_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)ospew_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)ospew_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)ospew_set, "set", A_LONG, A_SYM, 0);

    
	class_register(CLASS_BOX, c);
	ospew_class = c;

	common_symbols_init();
	return 0;
}
