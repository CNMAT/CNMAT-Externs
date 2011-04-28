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
NAME: o.build
DESCRIPTION: Build up an OSC packet
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 1.0: One inlet per address
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

typedef struct _obuild{
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
} t_obuild;

void *obuild_class;

void obuild_outputBundle(t_obuild *x);
int obuild_checkPosAndResize(char *buf, int len, char *pos);
void obuild_anything(t_obuild *x, t_symbol *msg, short argc, t_atom *argv);
void obuild_int(t_obuild *x, long l);
void obuild_float(t_obuild *x, double f);
void obuild_free(t_obuild *x);
void obuild_assist(t_obuild *x, void *b, long m, long a, char *s);
void *obuild_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void obuild_outputBundle(t_obuild *x){
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

void obuild_list(t_obuild *x, t_symbol *msg, short argc, t_atom *argv){
	obuild_anything(x, NULL, argc, argv);
}

void obuild_anything(t_obuild *x, t_symbol *msg, short argc, t_atom *argv){
	int inlet = proxy_getinlet((t_object *)x);
	int shouldOutput = (inlet == 0);
#ifdef BILD
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
		obuild_outputBundle(x);
	}
}

void obuild_int(t_obuild *x, long l){
	t_atom a;
	atom_setlong(&a, l);
	obuild_anything(x, NULL, 1, &a);
}

void obuild_float(t_obuild *x, double f){
	t_atom a;
	atom_setfloat(&a, (float)f);
	obuild_anything(x, NULL, 1, &a);
}

void obuild_bang(t_obuild *x){
	//obuild_anything(x, NULL, 0, NULL);
	obuild_outputBundle(x);
}

void obuild_assist(t_obuild *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"OSC bundle");
	else {
		sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void obuild_free(t_obuild *x){
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

void *obuild_new(t_symbol *msg, short argc, t_atom *argv){
	t_obuild *x;
	if(x = (t_obuild *)object_alloc(obuild_class)){
		if(argc == 0){
			object_error((t_object *)x, "you must supply at least 1 argument");
			return NULL;
		}
		//x->max_num_args = MAX_NUM_ARGS;
		if(atom_gettype(argv) == A_LONG){
			//x->max_num_args = atom_getlong(argv);
			object_error((t_object *)x, "o.build no longer takes an integer argument to specify the list length of each inlet.");
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
				if(atom_getsym(argv + i)->s_name[0] == '/'){
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
#ifdef BILD
	name = "o.bild";
#else
	name = "o.build";
#endif
	t_class *c = class_new(name, (method)obuild_new, (method)obuild_free, sizeof(t_obuild), 0L, A_GIMME, 0);
    	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	//class_addmethod(c, (method)obuild_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)obuild_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)obuild_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)obuild_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)obuild_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)obuild_int, "int", A_LONG, 0);
	class_addmethod(c, (method)obuild_bang, "bang", 0);

	class_register(CLASS_BOX, c);
	obuild_class = c;

	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
	return 0;
}
