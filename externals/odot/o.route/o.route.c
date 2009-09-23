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
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_osc.h"
//#include "cmmjl/cmmjl_osc_obj.h"

typedef struct _oroute{
	t_object ob;
	void **outlets;
	t_symbol **args;
	int numArgs;
	t_cmmjl_osc_message *matched, *unmatched;
	int *numCharsMatched;
	int *matched_outlets;
	int numMatched, numUnmatched;
	int bundlePartialMatches;
} t_oroute;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_doFullPacket(t_oroute *x, long len, long ptr, int shouldOutputBundle);
void oroute_foo(t_cmmjl_osc_message msg, void *v);
void oroute_cbk(void *xx, t_symbol *msg, int argc, t_atom *argv);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

void oroute_fullPacket(t_oroute *x, long len, long ptr){
	oroute_doFullPacket(x, len, ptr, 1);
}

void oroute_doFullPacket(t_oroute *x, long len, long ptr, int shouldOutputBundle){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;
	//int k;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		/*
		post("making bundle from naked message");
		for(k = 0; k < nn; k++){
			post("bundled: %d %c 0x%x", k, cpy[k], cpy[k]);
		}
		*/
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	/*
	for(k = 0; k < nn; k++){
		post("bundled: %d %c 0x%x", k, cpy[k], cpy[k]);
	}
	*/
	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	// init the number of matched and unmatched items and extract the messages from the bundle
	x->numMatched = x->numUnmatched = 0;
	cmmjl_osc_extract_messages(nn, cpy, true, oroute_foo, (void *)x);

	int i, j = 0;

	// pack all unmatched messages into a new bundle and output the pointer to it out the right outlet
	if(x->numUnmatched > 0){
		char out[len];
		j += cmmjl_osc_init_bundle(len, out, cpy + 8);
		for(i = 0; i < x->numUnmatched; i++){
			j += cmmjl_osc_add_to_bundle(len - j, out + j, &(x->unmatched[i]));
		}

		t_atom out_atoms[2];
		atom_setlong(&(out_atoms[0]), j);
		atom_setlong(&(out_atoms[1]), (long)out);
		outlet_anything(x->outlets[x->numArgs], gensym("FullPacket"), 2, out_atoms);
	}

	// output all of the matches in right to left outlet order
	for(j = x->numArgs - 1; j >= 0; j--){
		for(i = 0; i < x->numMatched; i++){
			//atom_setsym(&(out_atoms[0]), gensym(x->matched[i].address));
			if(x->matched_outlets[i] == j){
				t_atom out_atoms[x->matched[i].argc];
				cmmjl_osc_args2atoms(x->matched[i].typetags, x->matched[i].argv, out_atoms);

				t_symbol *sym = _sym_list;
				if(x->numCharsMatched[i] > 0){
					// this is a partial match
					//post("partial match");
					sym = gensym(x->matched[i].address + x->numCharsMatched[i]);
					if(x->bundlePartialMatches){
						//post("bundling partial match");
						//int len = cmmjl_osc_get_msg_length(sym->s_name, x->matched[i].typetags, x->matched[i].argc, x->matched[i].argv);
						int len = x->matched[i].size;
						char buf[len];
						memcpy(buf, x->matched[i].address + x->numCharsMatched[i], len);
						t_atom out[2];
						atom_setlong(out, len);
						atom_setlong(out + 1, (long)buf);
						outlet_anything(x->outlets[x->matched_outlets[i]], ps_FullPacket, 2, out);
					}else{
						outlet_anything(x->outlets[x->matched_outlets[i]], sym, x->matched[i].argc, out_atoms);
					}
				}else{
					// this is a full match
					if(x->matched[i].argc > 1){
						outlet_anything(x->outlets[x->matched_outlets[i]], sym, x->matched[i].argc, out_atoms);
					}else{
						outlet_atoms(x->outlets[x->matched_outlets[i]], x->matched[i].argc, out_atoms);
					}
				}

				if(x->matched[i].argc > 1){

				}else{

				}
			}
		}
	}
}

void oroute_foo(t_cmmjl_osc_message msg, void *v){
	t_oroute *x = (t_oroute *)v;
	int i;
	int ret;
	int didmatch = 0;
	for(i = 0; i < x->numArgs; i++){
		if((ret = cmmjl_osc_match(x, msg.address, x->args[i]->s_name)) != 0 || !strcmp(msg.address, x->args[i]->s_name)){
			x->matched_outlets[x->numMatched] = i;	
			x->numCharsMatched[x->numMatched] = ret;
			x->matched[x->numMatched++] = msg;
			didmatch += 1;
		}
	}
	if(didmatch == 0){
		x->unmatched[x->numUnmatched++] = msg;
	}
}

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
	int len = cmmjl_osc_get_msg_length_max(msg, argc, argv);
	if(len == 0){
		error("o.route: problem formatting bundle");
		return;
	}
	char buf[len];
	memset(buf, '\0', len);
	cmmjl_osc_copy_max_messages(msg, argc, argv, len, buf);

	oroute_doFullPacket(x, len, (long)buf, 0);
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
		cmmjl_init(x, NAME, 0);
		int numArgs = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((numArgs + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(numArgs * sizeof(t_symbol *));
		x->numArgs = numArgs;
		x->outlets[numArgs] = outlet_new(x, "FullPacket"); // unmatched outlet
		for(i = 0; i < numArgs; i++){
			x->outlets[numArgs - 1 - i] = outlet_new(x, NULL);
			x->args[i] = atom_getsym(argv + i);
			x->matched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			x->numCharsMatched = (int *)malloc(1024 * sizeof(int));
			x->unmatched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			x->matched_outlets = (int *)malloc(1024 * sizeof(int));
			x->numMatched = 0;
			x->numUnmatched = 0;
		}
		x->bundlePartialMatches = 0;
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
