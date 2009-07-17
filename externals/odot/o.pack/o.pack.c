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
DESCRIPTION: Pack up an OSC packet
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

typedef struct _opack{
	t_object ob;
	void *outlet;
	t_symbol **addresses;
	char **typetags;
	int numAddresses;
	long inlet;
	void *proxy;
} t_opack;

void *opack_class;

void opack_anything(t_opack *x, t_symbol *msg, short argc, t_atom *argv);
void opack_free(t_opack *x);
void opack_assist(t_opack *x, void *b, long m, long a, char *s);
void *opack_new(t_symbol *msg, short argc, t_atom *argv);

void opack_anything(t_opack *x, t_symbol *msg, short argc, t_atom *argv){

}

void opack_assist(t_opack *x, void *b, long m, long a, char *s){
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

void opack_free(t_opack *x){
	if(x->addresses){
		free(x->addresses);
	}
}

void *opack_new(t_symbol *msg, short argc, t_atom *argv){
	t_opack *x;
	if(x = (t_opack *)object_alloc(opack_class)){
		cmmjl_init(x, NAME, 0);
		int i;
		int type[argc];
		x->numAddresses = 0;
		for(i = 0; i < argc; i++){
			if(atom_getsym(argv + i)->s_name[0] == '/'){
				type[i] = 1;
				++(x->numAddresses);
			}else{
				type[i] = 0;
			}
		}
		x->addresses = (t_symbol **)malloc(x->numAddresses * sizeof(t_symbol *));
		x->typetags = (char **)calloc(x->numAddresses, sizeof(char *));
		int j = 0;
		for(i = 0; i < argc; i++){
			if(type[i]){
				x->addresses[j] = atom_getsym(argv + i);
				post("address %s", x->addresses[j]->s_name);
				j++;
			}else{
				t_symbol *tt = atom_getsym(argv + i);
				x->typetags[j] = malloc(strlen(tt->s_name));
				strcpy(x->typetags[j], tt->s_name);
				post("typetags %s", x->typetags[j]);
			}
		}
		for(i = 0; i < x->numAddresses - 1; i++){
			x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		}
		x->outlet = outlet_new(x, "FullPacket");
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.pack", (method)opack_new, (method)opack_free, sizeof(t_opack), 0L, A_GIMME, 0);
    
	//class_addmethod(c, (method)opack_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)opack_assist, "assist", A_CANT, 0);
    
	class_register(CLASS_BOX, c);
	opack_class = c;

	common_symbols_init();
	return 0;
}
