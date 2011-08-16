/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009-11, The Regents of
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
NAME: o.pack
DESCRIPTION: Pack messages together into an OSC bundle
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009-11
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 1.0: One inlet per address
VERSION 1.1: renamed o.pack (from o.build) 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "omax_util.h"
#include "osc.h"

//#define MAX_NUM_ARGS 64

typedef struct _opack{
	t_object ob;
	void *outlet;
	t_symbol **addresses;
	int numAddresses;
	t_atom **args;
	int *numargs;
	int *arglen;
	long inlet;
	void **proxy;
	//long max_num_args;
} t_opack;

void *opack_class;

void opack_outputBundle(t_opack *x);
int opack_checkPosAndResize(char *buf, int len, char *pos);
void opack_anything(t_opack *x, t_symbol *msg, short argc, t_atom *argv);
void opack_int(t_opack *x, long l);
void opack_float(t_opack *x, double f);
void opack_free(t_opack *x);
void opack_assist(t_opack *x, void *b, long m, long a, char *s);
void *opack_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void opack_outputBundle(t_opack *x){
	int i, j;
	int nbytes = 16;
	for(i = 0; i < x->numAddresses; i++){
		nbytes += 4; // size
		// address
		nbytes += strlen(x->addresses[i]->s_name);
		nbytes++;
		while(nbytes % 4){
			nbytes++;
		}
		nbytes++; // ,
		nbytes += x->numargs[i];
		nbytes++;
		while(nbytes % 4){
			nbytes++;
		}
		for(j = 0; j < x->numargs[i]; j++){
			switch(atom_gettype(x->args[i] + j)){
			case A_LONG:
			case A_FLOAT:
				nbytes += 4;
				break;
			case A_SYM:
				nbytes += strlen(atom_getsym(x->args[i] + j)->s_name);
				nbytes++;
				while(nbytes % 4){
					nbytes++;
				}
				break;
			}
		}
	}
	char buffer[nbytes];
	memset(buffer, '\0', nbytes);
	strncpy(buffer, "#bundle\0", 8);
	int pos = 16;
	// insert timetag!
	for(i = 0; i < x->numAddresses; i++){
		pos += omax_util_encode_atoms(buffer + pos, x->addresses[i], x->numargs[i], x->args[i]);
	}

	t_atom out[2];
	atom_setlong(&(out[0]), pos);
	atom_setlong(&(out[1]), (long)buffer);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void opack_list(t_opack *x, t_symbol *msg, short argc, t_atom *argv){
	opack_anything(x, NULL, argc, argv);
}

void opack_anything(t_opack *x, t_symbol *msg, short argc, t_atom *argv){
	int inlet = proxy_getinlet((t_object *)x);
	int shouldOutput = (inlet == 0);
#ifdef PAK
	shouldOutput = 1;
#endif
	int numargs = argc;
	if(msg){
		numargs++;
	}
	if(numargs > x->arglen[inlet]){
		if(x->args[inlet] == NULL){
			//x->args[inlet] = (t_atom *)sysmem_newptr(sizeof(t_atom) * numargs);
			x->args[inlet] = (t_atom *)malloc(sizeof(t_atom) * numargs);
			if(!x->args[inlet]){
				object_error((t_object *)x, "out of memory!");
				return;
			}
		}else{
			//x->args[inlet] = (t_atom *)sysmem_resizeptr(x->args[inlet], sizeof(t_atom) * numargs);
			x->args[inlet] = (t_atom *)realloc(x->args[inlet], sizeof(t_atom) * numargs);
			if(!x->args[inlet]){
				object_error((t_object *)x, "out of memory!");
				return;
			}
		}
		/*
		numargs = MAX_NUM_ARGS;
		if(x->args[inlet] == NULL){
			object_error((t_object *)x, "Out of memory--Max will be crashing soon...");
			return;
		}
		*/
		x->arglen[inlet] = numargs;
	}
	if(msg){
		atom_setsym(x->args[inlet], msg);
		memcpy(x->args[inlet] + 1, argv, argc * sizeof(t_atom));
	}else{
		memcpy(x->args[inlet], argv, argc * sizeof(t_atom));
	}
	x->numargs[inlet] = numargs;
	if(shouldOutput){
		opack_outputBundle(x);
	}
}

void opack_int(t_opack *x, long l){
	t_atom a;
	atom_setlong(&a, l);
	opack_anything(x, NULL, 1, &a);
}

void opack_float(t_opack *x, double f){
	t_atom a;
	atom_setfloat(&a, (float)f);
	opack_anything(x, NULL, 1, &a);
}

void opack_bang(t_opack *x){
	//opack_anything(x, NULL, 0, NULL);
	opack_outputBundle(x);
}

void opack_assist(t_opack *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"OSC bundle");
	else {
		sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void opack_free(t_opack *x){
	if(x->addresses){
		free(x->addresses);
	}
	if(x->args){
		int i;
		for(i = 0; i < x->numAddresses; i++){
			if(x->args[i]){
				free(x->args[i]);
			}
		}
		free(x->args);
	}
	if(x->numargs){
		free(x->numargs);
	}
	if(x->arglen){
		free(x->arglen);
	}
	if(x->proxy){
		int i;
		for(i = 1; i < x->numAddresses; i++){
			if(x->proxy[i]){
				sysmem_freeptr(x->proxy[i]);
			}
		}
		free(x->proxy);
	}
}

void *opack_new(t_symbol *msg, short argc, t_atom *argv){
	t_opack *x;
	if(x = (t_opack *)object_alloc(opack_class)){
		if(argc == 0){
			object_error((t_object *)x, "you must supply at least 1 argument");
			return NULL;
		}
		//x->max_num_args = MAX_NUM_ARGS;
		if(atom_gettype(argv) == A_LONG){
			//x->max_num_args = atom_getlong(argv);
			object_error((t_object *)x, "o.pack no longer takes an integer argument to specify the list length of each inlet.");
			object_error((t_object *)x, "The internal buffers will expand as necessary.");
			argv++;
			argc--;

		}
		if(atom_gettype(argv) != A_SYM){
			object_error((t_object *)x, "the first argument must be an OSC address");
			return NULL;
		}
		if(atom_getsym(argv)->s_name[0] != '/' && atom_getsym(argv)->s_name[0] != '#'){
			object_error((t_object *)x, "the first argument must be an OSC string that begins with a slash (/)");
			return NULL;
		}
	
		t_atom *addresses[argc];
		int numargs[argc];
		int count = 0;
		int i;
		for(i = 0; i < argc; i++){
			numargs[i] = 0;
			if(atom_gettype(argv + i) == A_SYM){
				if(atom_getsym(argv + i)->s_name[0] == '/' || atom_getsym(argv + i)->s_name[0] == '#'){
					addresses[count++] = argv + i;
				}else{
					numargs[count - 1]++;
				}
			}else{
				numargs[count - 1]++;
			}
		}
		x->numAddresses = count;
		x->addresses = (t_symbol **)malloc(count * sizeof(t_symbol *));
		x->args = (t_atom **)malloc(count * sizeof(t_atom *));
		x->numargs = (int *)malloc(count * sizeof(int));
		memset(x->numargs, '\0', count * sizeof(int));
		x->arglen = (int *)malloc(count * sizeof(int));
		memset(x->arglen, '\0', count * sizeof(int));
		for(i = 0; i < count; i++){
			x->addresses[i] = atom_getsym(addresses[i]);
			x->args[i] = (t_atom *)malloc(numargs[i] * sizeof(t_atom));
			x->arglen[i] = numargs[i];
			//x->args[i] = (t_atom *)malloc(x->max_num_args * sizeof(t_atom));
			//x->arglen[i] = x->max_num_args;
			x->numargs[i] = numargs[i];
			memcpy(x->args[i], addresses[i] + 1, numargs[i] * sizeof(t_atom));
		}

		x->proxy = (void **)malloc(count * sizeof(void *));
		for(i = 1; i < count; i++){
			x->proxy[i] = proxy_new((t_object *)x, count - i, &(x->inlet));
		}
		x->outlet = outlet_new(x, "FullPacket");
	}
		   	
	return(x);
}

int main(void){
	char *name;
#ifdef PAK
	name = "o.pak";
#else
	name = "o.pack";
#endif
	t_class *c = class_new(name, (method)opack_new, (method)opack_free, sizeof(t_opack), 0L, A_GIMME, 0);
    	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	//class_addmethod(c, (method)opack_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)opack_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)opack_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)opack_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)opack_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)opack_int, "int", A_LONG, 0);
	class_addmethod(c, (method)opack_bang, "bang", 0);

	class_register(CLASS_BOX, c);
	opack_class = c;

	ps_FullPacket = gensym("FullPacket");

#ifdef PAK
	class_alias(c, gensym("o.bild"));
#else
	class_alias(c, gensym("o.build"));
#endif

	common_symbols_init();
	return 0;
}
