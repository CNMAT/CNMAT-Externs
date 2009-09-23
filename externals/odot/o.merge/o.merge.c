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
NAME: o.merge
DESCRIPTION: Merge 2 or more OSC bundles together
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
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_osc.h"

typedef struct _omerge{
	t_object ob;
	void *outlet;
	void **proxies;
	long inlet;
	int num_inlets;
	char **buffers;
	int *buffer_pos;
	int buffer_len;
} t_omerge;

void *omerge_class;

void omerge_fullPacket(t_omerge *x, long len, long ptr);
void omerge_cbk(t_cmmjl_osc_message msg, void *v);
void omerge_anything(t_omerge *x, t_symbol *msg, short argc, t_atom *argv);
void omerge_free(t_omerge *x);
void omerge_assist(t_omerge *x, void *b, long m, long a, char *s);
void *omerge_new(t_symbol *msg, short argc, t_atom *argv);

void omerge_fullPacket(t_omerge *x, long len, long ptr){
	int inlet = proxy_getinlet((t_object *)x);
	if(len > x->buffer_len){
		error("o.merge: len is greater than the maximum buffer size of %d bytes", x->buffer_len);
		return;
	}

	memcpy(x->buffers[inlet], (char *)ptr, len);
	x->buffer_pos[inlet] = len;
	if(strncmp((char *)ptr, "#bundle\0", 8)){
		x->buffer_pos[inlet] = cmmjl_osc_bundle_naked_message(len, x->buffers[inlet], x->buffers[inlet]);
		if(x->buffer_pos[inlet] < 0){
			error("problem bundling naked message");
			return;
		}
	}

	// flatten any nested bundles
	x->buffer_pos[inlet] = cmmjl_osc_flatten(x->buffer_pos[inlet], x->buffers[inlet], x->buffers[inlet]);

	//memcpy(x->buffers[inlet], (char *)ptr, len);
	if(inlet != 0){
		return;
	}

	char buf[x->num_inlets * x->buffer_len];
	int bufpos = 0;
	if(x->buffer_pos[0] > 0){
		memcpy(buf, x->buffers[0], x->buffer_pos[0]);
		bufpos += x->buffer_pos[0];
	}
	int i;
	for(i = 1; i < x->num_inlets; i++){
		if(x->buffer_pos[i] > 0){
			memcpy(buf + bufpos, x->buffers[i] + 16, x->buffer_pos[i] - 16);
			bufpos += x->buffer_pos[i] - 16;
		}
	}
	t_atom out[2];
	atom_setlong(&(out[0]), bufpos);
	atom_setlong(&(out[1]), (long)buf);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);

	/*
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	char buffer[len * 2];
	memset(buffer, '\0', len * 2);
	x->buffer = buffer;
	x->bufferLen = len * 2;
	memcpy(cpy, (char *)ptr, len);
	long nn = len;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	memcpy(buffer, cpy, 16);
	x->bufferPos = 16;

	// extract the messages from the bundle
	cmmjl_osc_extract_messages(nn, cpy, true, omerge_cbk, (void *)x);

	*/
	/*
	int i; 
	post("x->bufferPos = %d", x->bufferPos);
	for(i = 0; i < x->bufferPos; i++){
		post("%d %x %c", i, x->buffer[i], x->buffer[i]);
	}
	*/
	/*
	t_atom out[2];
	atom_setlong(&(out[0]), x->bufferPos);
	atom_setlong(&(out[1]), (long)buffer);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
	*/
}

/*
void omerge_cbk(t_cmmjl_osc_message msg, void *v){
	t_omerge *x = (t_omerge *)v;
	int i;
	int ret;
	int didmatch = 0;
	if(x->bufferPos + msg.size > x->bufferLen){
		realloc(x->buffer, x->bufferLen * 2);
	}
	for(i = 0; i < x->numAddresses; i++){
		//post("testing %s against %s", msg.address, x->addresses_in[i]->s_name);
		if((ret = cmmjl_osc_match(x, msg.address, x->addresses_in[i]->s_name)) == -1){
			//post("matchd %d characters", ret);
			x->bufferPos += cmmjl_osc_rename(x->buffer, x->bufferLen, x->bufferPos, &msg, x->addresses_out[i]->s_name);
			didmatch++;
		}
		//post("ret = %d", ret);
	}
	if(didmatch == 0){
		*((long *)(x->buffer + x->bufferPos)) = htonl(msg.size);
		x->bufferPos += 4;
		memcpy(x->buffer + x->bufferPos, msg.address, msg.size);
		x->bufferPos += msg.size;
	}
}
*/

/*
int omerge_rename(char *buffer, 
		    int bufferLen, 
		    int bufferPos, 
		    t_cmmjl_osc_message *msg, 
		    char *newAddress){
	int start = bufferPos;
	int len = strlen(newAddress);
	len++;
	len += 4 - (len % 4);
	*((long *)(buffer + bufferPos)) = htonl(msg->size + (len - (msg->typetags - msg->address)));
	bufferPos += 4;
	memcpy(buffer + bufferPos, newAddress, strlen(newAddress));
	bufferPos += len;
	len = msg->size - (msg->typetags - msg->address);
	memcpy(buffer + bufferPos, msg->typetags, len);
	bufferPos += len;
	return bufferPos - start;
}
*/

void omerge_anything(t_omerge *x, t_symbol *msg, short argc, t_atom *argv){
}

void omerge_assist(t_omerge *x, void *b, long m, long a, char *s){
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

void omerge_free(t_omerge *x){
}

void *omerge_new(t_symbol *msg, short argc, t_atom *argv){
	if(argc % 2 != 0){
		error("o.rename: multiple of 2 arguments required");
		return NULL;
	}
	t_omerge *x;
	int i;
	if(x = (t_omerge *)object_alloc(omerge_class)){
		cmmjl_init(x, NAME, 0);
		x->outlet = outlet_new(x, "FullPacket");
		x->num_inlets = 2;
		x->buffer_len = 4096;
		attr_args_process(x, argc, argv);
		x->proxies = (void **)calloc(x->num_inlets - 1, sizeof(void *));
		x->buffers = (char **)calloc(x->num_inlets, sizeof(char *));
		x->buffer_pos = (int *)calloc(x->num_inlets, sizeof(char *));
		int i;
		for(i = 0; i < x->num_inlets - 1; i++){
			x->proxies[i] = proxy_new((t_object *)x, x->num_inlets - i - 1, &(x->inlet));
		}
		for(i = 0; i < x->num_inlets; i++){
			x->buffers[i] = (char *)calloc(x->buffer_len, sizeof(char));
		}
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.merge", (method)omerge_new, (method)omerge_free, sizeof(t_omerge), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)omerge_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)omerge_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)omerge_assist, "assist", A_CANT, 0);
    
	CLASS_ATTR_LONG(c, "num_inlets", 0, t_omerge, num_inlets);
	CLASS_ATTR_LONG(c, "buffer_len", 0, t_omerge, buffer_len);

	class_register(CLASS_BOX, c);
	omerge_class = c;

	common_symbols_init();
	return 0;
}
