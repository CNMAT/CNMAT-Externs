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
NAME: o.explode
DESCRIPTION: explode an OSC message into a bunch of messages
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

typedef struct _oex{
	t_object ob;
	void *outlets;

	char *buffer;
	int bufferLen;
	int bufferPos;
} t_oex;

void *oex_class;

void oex_fullPacket(t_oex *x, long len, long ptr);
void oex_cbk(t_cmmjl_osc_message msg, void *v);
void oex_anything(t_oex *x, t_symbol *msg, short argc, t_atom *argv);
void oex_free(t_oex *x);
void oex_assist(t_oex *x, void *b, long m, long a, char *s);
void *oex_new(t_symbol *msg, short argc, t_atom *argv);

void oex_fullPacket(t_oex *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;
	char buffer[len * 2];
	x->buffer = buffer;
	x->bufferLen = len * 2;
	x->bufferPos = 0;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	cmmjl_osc_extract_messages(nn, cpy, true, oex_cbk, (void *)x);

}

void oex_cbk(t_cmmjl_osc_message msg, void *v){
	t_oex *x = (t_oex *)v;
	int i;
	int ret;
	for(i = 0; i < x->numArgs; i++){
		if((ret = cmmjl_osc_match(x, msg.address, x->args[i]->s_name)) != 0 || !strcmp(msg.address, x->args[i]->s_name)){
			x->matched_outlets[x->numMatched] = i;	
			x->numCharsMatched[x->numMatched] = ret;
			x->matched[x->numMatched++] = msg;
		}
	}
}

void oex_anything(t_oex *x, t_symbol *msg, short argc, t_atom *argv){
}

void oex_assist(t_oex *x, void *b, long m, long a, char *s){
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

void oex_free(t_oex *x){
	if(x->args){
		free(x->args);
	}
}

void *oex_new(t_symbol *msg, short argc, t_atom *argv){
	t_oex *x;
	int i;
	if(x = (t_oex *)object_alloc(oex_class)){
		cmmjl_init(x, NAME, 0);
		x->outlet = outlet_new(x->outlet, NULL);
		for(i = 0; i < argc; i++){

		}
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.explode", (method)oex_new, (method)oex_free, sizeof(t_oex), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oex_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oex_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oex_assist, "assist", A_CANT, 0);
    
	class_register(CLASS_BOX, c);
	oex_class = c;

	common_symbols_init();
	return 0;
}
