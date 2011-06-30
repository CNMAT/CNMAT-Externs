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
VERSION 0.1: Addresses to match can now have patterns
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_critical.h"
#include "osc.h"
#include "omax_util.h"

#define PF __PRETTY_FUNCTION__

typedef struct _oroute_message{
	t_osc_msg msg;
	int outlet_num;
	int full_match;
	int offset;
	struct _oroute_message *next, *prev;
} t_oroute_message;

// local workspace to avoid having to synchronize
// or use critical regions
typedef struct _oroute_wksp{
	t_symbol **args;
	int numArgs;
	t_oroute_message *messages;
	t_oroute_message *message_buf;
	int numMessages, message_buf_len;
	int *haswildcard;
	t_osc_msg oscschemalist;
} t_oroute_wksp;

typedef struct _oroute{
	t_object ob;
	void **outlets;
	t_critical lock;
	t_symbol **args;
	int *haswildcard;
	int numArgs;
	// we have an array of preallocated messages that we'll use, but we'll
	// organize them in a linked list as we accumulate them so that we can 
	// just rip through that list when it's time to output them later
	int max_message; // set this to note that the event originated as a max message and not a FullPacket
	t_osc_msg oscschemalist;
} t_oroute;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_fp_bundle_partial_matches(t_oroute *x, long len, char *ptr, t_oroute_message *m);
void oroute_fp(t_oroute *x, long len, char *ptr, t_oroute_message *m);
void oroute_cbk(t_osc_msg msg, void *context);
void oroute_insert_msg(t_oroute_wksp *x, t_oroute_message *msg);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_oscschemalist;


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
	/*
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = osc_util_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}
	nn = osc_util_flatten(nn, cpy, cpy);
	*/
	//osc_util_printBundle(len, cpy, printf);
	t_oroute_wksp wksp;
	t_symbol *args[x->numArgs];
	//t_oroute_message mm[128];
#define DEF_NUM_MSGS 128
	t_oroute_message *mm = (t_oroute_message *)osc_mem_alloc(DEF_NUM_MSGS * sizeof(t_oroute_message));
	int haswildcard[x->numArgs];
	memset(mm, '\0', DEF_NUM_MSGS * sizeof(t_oroute_message));
	wksp.message_buf = mm;
	wksp.messages = NULL;
	wksp.numMessages = 0;
	wksp.message_buf_len = DEF_NUM_MSGS;
	wksp.oscschemalist = x->oscschemalist;
	critical_enter(x->lock);
	memcpy(args, x->args, x->numArgs * sizeof(t_symbol *));
	memcpy(haswildcard, x->haswildcard, x->numArgs * sizeof(int));
	wksp.haswildcard = haswildcard;
	wksp.args = args;
	wksp.numArgs = x->numArgs;
	critical_exit(x->lock);
	osc_bundle_getMessagesWithCallback(nn, cpy, oroute_cbk, (void *)&wksp);

#ifdef SPEW
	oroute_fp(x, nn, cpy, wksp.messages);
#else
	oroute_fp_bundle_partial_matches(x, nn, cpy, wksp.messages);
#endif
	osc_mem_free(wksp.message_buf);
	x->max_message = 0;
}

void oroute_fp_bundle_partial_matches(t_oroute *x, long len, char *ptr, t_oroute_message *m){
	if(!m){
		return;
	}
	long argc = 0;
	//char buf[len], *bufp = buf;
	char buf[len];
	char *bufp = buf;

	memset(buf, '\0', len);
	memcpy(buf, ptr, 16);
	bufp += 16;
	int last_outlet_num = -1;
	//printf("*******************************************\n");
	int counter = 0;
	int maxnumatoms = m->msg.argc;
	t_oroute_message *last = m;
	while(m){
		if(m->msg.argc > maxnumatoms){
			maxnumatoms = m->msg.argc;
		}
		last = m;
		m = m->next;
		//post("%d: %p <- %p -> %p, address: %s", counter++, m->prev, m, m->next, m->msg.address);
	}
	maxnumatoms++;
	m = last;
	t_atom argv[maxnumatoms];
	counter = 0;
	while(m){
		if(last_outlet_num != m->outlet_num && last_outlet_num >= 0 && bufp - buf > 16){
			t_atom out[2];
			atom_setlong(out, bufp - buf);
			atom_setlong(out + 1, (long)buf);
			outlet_anything(x->outlets[last_outlet_num], gensym("FullPacket"), 2, out);
			bufp = buf + 16;
			memset(bufp, '\0', len - 16);
		}
		if(m->full_match){
#ifdef SELECT
			char *size = bufp;
			bufp += 4;
			strcpy(bufp, m->msg.address);
			bufp += strlen(m->msg.address);
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
#else
			omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
			if(argc == 1){
				outlet_bang(x->outlets[m->outlet_num]);
			}else{
				outlet_atoms(x->outlets[m->outlet_num], argc - 1, argv + 1);
			}
#endif
		}else if(x->max_message){
			omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
#ifdef SELECT
			outlet_anything(x->outlets[m->outlet_num], gensym(atom_getsym(argv)->s_name), argc - 1, argv + 1);
#else
			outlet_anything(x->outlets[m->outlet_num], gensym(atom_getsym(argv)->s_name + m->offset), argc - 1, argv + 1);
#endif
		}else{
			char *size = bufp;
			bufp += 4;
#ifdef SELECT
			strcpy(bufp, m->msg.address);
			bufp += strlen(m->msg.address);
#else
			strcpy(bufp, m->msg.address + m->offset);
			bufp += strlen(m->msg.address + m->offset);
#endif
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
}

void oroute_fp(t_oroute *x, long len, char *ptr, t_oroute_message *m){
	if(!m){
		return;
	}
	long argc;
	int maxnumatoms = m->msg.argc;
	t_oroute_message *last = m;
	while(m){
		if(m->msg.argc > maxnumatoms){
			maxnumatoms = m->msg.argc;
		}
		last = m;
		m = m->next;
	}
	m = last;
	t_atom argv[maxnumatoms];
	while(m){
		omax_util_oscMsg2MaxAtoms(&(m->msg), &argc, argv);
		outlet_anything(x->outlets[m->outlet_num], atom_getsym(argv), argc - 1, argv + 1);
		m = m->prev;
	}
}

void oroute_cbk(t_osc_msg msg, void *context){
	if(!(msg.address)){
		return;
	}
	t_oroute_wksp *x = (t_oroute_wksp *)context;
	int i;
	int match = 0;
	for(i = 0; i < x->numArgs; i++){
		int po, ao;
		int ret;
		if(x->haswildcard[i]){
			ret = osc_match(x->args[i]->s_name, msg.address, &ao, &po);
			ret = ((ret & 1) << 1) | ((ret & 2) >> 1);
		}else{
			ret = osc_match(msg.address, x->args[i]->s_name, &po, &ao);
		}

		//post("pattern = %s, address = %s, ret = %d", msg.address, x->args[i]->s_name, ret);
		int star_at_end = 0;
		if(msg.address[po] == '*' && msg.address[po + 1] == '\0'){
			star_at_end = 1;
		}
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && ((ret & OSC_MATCH_PATTERN_COMPLETE) || ((msg.address[po] == '/') || star_at_end == 1))){
			if(x->numMessages + 1 > x->message_buf_len){
				x->message_buf = (t_oroute_message *)osc_mem_resize(x->message_buf, x->message_buf_len + DEF_NUM_MSGS);
				x->message_buf_len += DEF_NUM_MSGS;
			}
			x->message_buf[x->numMessages].msg = msg;
			x->message_buf[x->numMessages].full_match = 0;
			x->message_buf[x->numMessages].offset = po;
			if(ret == 3 || star_at_end == 1){
				x->message_buf[x->numMessages].full_match = 1;
			}
			x->message_buf[x->numMessages].outlet_num = i;
			oroute_insert_msg(x, &(x->message_buf[x->numMessages]));
			match = 1;
			x->numMessages++;
		}else{
		}
	}
	if(!match){
		if(x->numMessages + 1 > x->message_buf_len){
			x->message_buf = (t_oroute_message *)osc_mem_resize(x->message_buf, x->message_buf_len + DEF_NUM_MSGS);
			x->message_buf_len += DEF_NUM_MSGS;
		}
		if(!strcmp(msg.address, ps_oscschemalist->s_name)){
			x->message_buf[x->numMessages].msg = x->oscschemalist;
		}else{
			x->message_buf[x->numMessages].msg = msg;
		}
		x->message_buf[x->numMessages].full_match = 0;
		x->message_buf[x->numMessages].offset = 0;
		x->message_buf[x->numMessages].outlet_num = x->numArgs;
		oroute_insert_msg(x, &(x->message_buf[x->numMessages]));
		x->numMessages++;
	}
}

void oroute_insert_msg(t_oroute_wksp *x, t_oroute_message *message){
	t_oroute_message *msg = message;
	t_oroute_message *m = x->messages;
	t_oroute_message *prev = NULL;
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

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
	if(!msg){
		object_error((t_object *)x, "doesn't look like an OSC message");
		return;
	}
	if(msg->s_name[0] != '/'){
		object_error((t_object *)x, "OSC addresses must begin with a /");
		return;
	}
	int len = omax_util_get_bundle_size_for_atoms(msg, argc, argv);
	char buffer[len];
	memset(buffer, '\0', len);
	t_atom av[argc + 1];
	atom_setsym(av, msg);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	strncpy(buffer, "#bundle\0", 8);
	*((long long *)(buffer + 8)) = hton64(1ll);
	len = omax_util_encode_atoms(buffer + 16, msg, argc, argv);
	x->max_message = 1;

	oroute_fullPacket(x, len + 16, (long)buffer);

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

void oroute_set(t_oroute *x, long index, t_symbol *sym){
	if(index < 1 || index > x->numArgs){
		object_error((t_object *)x, "index (%d) out of bounds", index);
		return;
	}
	x->args[index - 1] = sym;
	x->haswildcard[index - 1] = 0;
	int i;
	for(i = 0; i < strlen(x->args[index - 1]->s_name); i++){
		switch(x->args[index - 1]->s_name[i]){
		case '*':
		case '[':
		case '{':
		case '?':
			x->haswildcard[index - 1] = 1;
		}
	}
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		if(a == x->numArgs){
			sprintf(s, "Unmatched messages (delegation)");
		}else{
			sprintf(s, "Messages that match %s", x->args[a]->s_name);
		}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"OSC bundle (FullPacket) or Max message");
			break;
		}
	}
}

void oroute_free(t_oroute *x){
	critical_free(x->lock);
	if(x->args){
		free(x->args);
	}
	if(x->haswildcard){
		free(x->haswildcard);
	}
	// the address is 4 bytes into the block of memory we allocated in the new routine.
	if(x->oscschemalist.address){
		free(x->oscschemalist.address - 4);
	}
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv){
	t_oroute *x;
	int i;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		int numArgs = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((numArgs + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(numArgs * sizeof(t_symbol *));
		x->haswildcard = (int *)calloc(numArgs, sizeof(int));
		x->numArgs = numArgs;
		critical_new(&(x->lock));
		x->outlets[numArgs] = outlet_new(x, "FullPacket"); // unmatched outlet
		int schemalist_len = 24 + numArgs + 2;
		while(schemalist_len % 4){
			schemalist_len++;
		}
		for(i = 0; i < numArgs; i++){
			x->outlets[numArgs - 1 - i] = outlet_new(x, NULL);
			if(atom_gettype(argv + i) != A_SYM){
				object_error((t_object *)x, "argument %d is not an OSC address", i);
				return NULL;
			}
			x->args[i] = atom_getsym(argv + i);
			int j;
			int len = strlen(x->args[i]->s_name);
			schemalist_len += len + 1;
			while(schemalist_len % 4){
				schemalist_len++;
			}
			for(j = 0; j < len; j++){
				switch(x->args[i]->s_name[j]){
				case '*':
				case '[':
				case '{':
				case '?':
					x->haswildcard[i] = 1;
					break;
				}
			}
		}
		char *schemalist = (char *)calloc(sizeof(char), schemalist_len);
		memset(schemalist, '\0', schemalist_len);
		omax_util_encode_atoms(schemalist, ps_oscschemalist, numArgs, argv);
		osc_message_parseMessage(-1, schemalist, &(x->oscschemalist));
		
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
#ifdef SELECT
	char *name = "o.select";
#elif SPEW
	char *name = "o.spew";
#else
	char *name = "o.route";
#endif
	t_class *c = class_new(name, (method)oroute_new, (method)oroute_free, sizeof(t_oroute), 0L, A_GIMME, 0);
    	osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oroute_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oroute_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oroute_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oroute_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oroute_set, "set", A_LONG, A_SYM, 0);

	//CLASS_ATTR_LONG(c, "bundle_partial_matches", 0, t_oroute, bundle_partial_matches);
    
	class_register(CLASS_BOX, c);
	oroute_class = c;

	ps_oscschemalist = gensym("/osc/schema/list");

	common_symbols_init();
	return 0;
}
