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
NAME: o.split
DESCRIPTION: Split the contents of an OSC bundle
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

typedef struct _osplit{
	t_object ob;
	void **outlets;
	char **buffers;
	int *buffer_len;
	int *buffer_pos;
	double min, max;
	int (*left_test)(double, double);
	int (*right_test)(double, double);
	int test_all_args;
} t_osplit;

void *osplit_class;

void osplit_fullPacket(t_osplit *x, long len, long ptr);
void osplit_cbk(t_cmmjl_osc_message msg, void *v);
int osplit_gt(double f1, double f2);
int osplit_gte(double f1, double f2);
int osplit_lt(double f1, double f2);
int osplit_lte(double f1, double f2);
void osplit_anything(t_osplit *x, t_symbol *msg, short argc, t_atom *argv);
void osplit_free(t_osplit *x);
void osplit_assist(t_osplit *x, void *b, long m, long a, char *s);
void *osplit_new(t_symbol *msg, short argc, t_atom *argv);

void osplit_fullPacket(t_osplit *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	char buffers[x->numranges + 1][len * 2];
	int i;
	for(i = 0; i < x->numranges + 1; i++){
		memset(buffers[i], '\0', len * 2);
		x->buffers[i] = buffers[i];
		x->buffer_len[i] = len * 2;
		x->buffer_pos[i] = 0;
	}

	int nn = len;
	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	for(i = 0; i < x->numranges + 1; i++){
		memcpy(buffers[i], cpy, 16);
		x->buffer_pos[i] = 16;
	}

	// extract the messages from the bundle
	cmmjl_osc_extract_messages(nn, cpy, true, osplit_cbk, (void *)x);

	/*
	int i; 
	post("x->bufferPos = %d", x->bufferPos);
	for(i = 0; i < x->bufferPos; i++){
		post("%d %x %c", i, x->buffer[i], x->buffer[i]);
	}
	*/

	for(i = x->numranges; i >= 0; i--){
		t_atom out[2];
		atom_setlong(&(out[0]), x->buffer_pos[i]);
		atom_setlong(&(out[1]), (long)buffers[i]);
		outlet_anything(x->outlets[i], ps_FullPacket, 2, out);
	}

}

void osplit_cbk(t_cmmjl_osc_message msg, void *v){
	t_osplit *x = (t_osplit *)v;
	int i, r;
	int ret;
	int didmatch = 0;
	int argpos = 0;
	int num_args_to_test = 1;
	if(x->test_all_args){
		num_args_to_test = msg.argc;
	}
	t_cmmjl_osc_atom atoms[msg.argc];
	cmmjl_osc_get_data(&msg, atoms);
	for(i = 0; i < num_args_to_test; i++){

		switch(msg.typetags[i + 1]){
		case 'i':
			{
				long l = ntohl(*((long *)(msg.argv + argpos)));
				float val = (float)l;
				post("%s long val = %f", msg.address, val);
				if(val >= x->ranges[r].min && val <= x->ranges[r].max){
					//post("%s: %f <= %f <= %f", msg.address, x->ranges[r].min, val, x->ranges[r].max);
					rangematches[r]++;
					//x->buffer_pos[r] += cmmjl_osc_add_to_bundle(x->buffer_len[r], x->buffers[r] + x->buffer_pos[r], &msg);
				}
				argpos += 4;
			}
			break;
		case 'f':
			{
				long l = ntohl(*((long *)(msg.argv + argpos)));
				float val = *((float *)(&l));
				//post("%s float val = %f", msg.address, val);
				if(val >= x->ranges[r].min && val <= x->ranges[r].max){
					//post("%s: %f <= %f <= %f", msg.address, x->ranges[r].min, val, x->ranges[r].max);
					rangematches[r]++;
					//x->buffer_pos[r] += cmmjl_osc_add_to_bundle(x->buffer_len[r], x->buffers[r] + x->buffer_pos[r], &msg);
				}
				argpos += 4;
			}
			break;
		default:
			{
				int len = strlen(msg.argv + argpos) + 1;
				while(len % 4){len++;}
				argpos += len;
			}
			break;
		}
	}

	for(r = 0; r < x->numranges; r++){
		//post("%d %d", rangematches[r], msg.argc);
		if(rangematches[r] == msg.argc){
			didmatch++;
			x->buffer_pos[r] += cmmjl_osc_add_to_bundle(x->buffer_len[r], x->buffers[r] + x->buffer_pos[r], &msg);
		}
	}

	if(didmatch == 0){
		x->buffer_pos[x->numranges] += cmmjl_osc_add_to_bundle(x->buffer_len[x->numranges], x->buffers[x->numranges] + x->buffer_pos[x->numranges], &msg);
	}
}

int osplit_gt(double f1, double f2){
	return f1 > f2;
}

int osplit_gte(double f1, double f2){
	return f1 >= f2;
}

int osplit_lt(double f1, double f2){
	return f1 < f2;
}

int osplit_lte(double f1, double f2){
	return f1 <= f2;
}

void osplit_anything(t_osplit *x, t_symbol *msg, short argc, t_atom *argv){
}

void osplit_assist(t_osplit *x, void *b, long m, long a, char *s){
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

void osplit_free(t_osplit *x){

}

void *osplit_new(t_symbol *msg, short argc, t_atom *argv){
	if(argc % 2 != 0){
		error("o.rename: multiple of 2 arguments required");
		return NULL;
	}
	t_osplit *x;
	int i;
	if(x = (t_osplit *)object_alloc(osplit_class)){
		cmmjl_init(x, NAME, 0);
		if(argc % 2){
			error("o.split: multiple of 2 arguments required.");
			return NULL;
		}
		x->numranges = argc / 2;
		x->outlets = (void **)calloc(x->numranges + 1, sizeof(void *));
		x->buffers = (char **)calloc(x->numranges + 1, sizeof(char *));
		x->buffer_len = (int *)calloc(x->numranges + 1, sizeof(int *));
		x->buffer_pos = (int *)calloc(x->numranges + 1, sizeof(int *));
		x->ranges = (t_range *)calloc(x->numranges, sizeof(int *));

		for(i = 0; i < x->numranges; i++){
			x->outlets[x->numranges - i] = outlet_new(x, "FullPacket");
			// skip buffers since they will be allocated on the stack
			x->ranges[i] = (t_range){atom_getfloat(argv + (i * 2)), atom_getfloat(argv + ((i * 2) + 1))};
		}
	        x->outlets[0] = outlet_new(x, "FullPacket");
		attr_args_process(x, argc, argv);
	}
		   	
	return x;
}

int main(void){
	t_class *c = class_new("o.split", (method)osplit_new, (method)osplit_free, sizeof(t_osplit), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)osplit_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)osplit_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)osplit_assist, "assist", A_CANT, 0);

	CLASS_ATTR_LONG(c, "test_all_args", 0, t_osplit, test_all_args);
    
	class_register(CLASS_BOX, c);
	osplit_class = c;

	common_symbols_init();
	return 0;
}
