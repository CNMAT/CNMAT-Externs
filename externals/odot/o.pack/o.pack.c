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
NAME: o.pack
DESCRIPTION: Pack up an OSC packet
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

#define MAX_ARGS 64

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
		while(nbytes % 4){
			nbytes++;
		}
		// args
		for(j = 0; j < x->numargs[i]; j++){
			switch(atom_gettype(x->args[i] + j)){
			case A_LONG:
			case A_FLOAT:
				nbytes += sizeof(double); // just to be safe...
				break;
			case A_SYM:
				nbytes += strlen(atom_getsym(x->args[i] + j)->s_name);
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
	
	/*
	int len = osc_util_make_bundle(x->numAddresses, 
					x->addresses, 
					x->numArgs, 
					x->typetags, 
					x->args, 
					&(x->bufsize), 
					buffer);
	*/
	/*
	int i;
	for(i = 0; i < len; i++){
		post("%d %c 0x%x", i, buffer[i], buffer[i]);
	}
	*/
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
	if(msg){
		argc++;
	}
	if(argc > x->arglen[inlet]){
		x->args[inlet] = (t_atom *)sysmem_resizeptr(x->args[inlet], sizeof(t_atom) * argc);
		if(x->args[inlet] == NULL){
			object_error((t_object *)x, "Out of memory--Max will be crashing soon...");
			return;
		}
	}
	if(msg){
		atom_setsym(x->args[inlet], msg);
		memcpy(x->args[inlet] + 1, argv, argc * sizeof(t_atom));
	}else{
		memcpy(x->args[inlet], argv, argc * sizeof(t_atom));
	}
	x->numargs[inlet] = argc;
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
		if(argc == 0){
			object_error((t_object *)x, "you must supply at least 1 argument");
			return NULL;
		}

		if(atom_getsym(argv)->s_name[0] != '/'){
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
		x->addresses = (t_symbol **)sysmem_newptr(count * sizeof(t_symbol *));
		x->args = (t_atom **)sysmem_newptr(count * sizeof(t_atom *));
		x->numargs = (int *)sysmem_newptr(count * sizeof(int));
		x->arglen = (int *)sysmem_newptr(count * sizeof(int));
		for(i = 0; i < count; i++){
			x->addresses[i] = atom_getsym(addresses[i]);
			if(numargs[i] > 0){
				x->args[i] = (t_atom *)sysmem_newptr(numargs[i] * sizeof(t_atom));
				x->arglen[i] = numargs[i];
				x->numargs[i] = numargs[i];
				memcpy(x->args[i], addresses[i] + 1, numargs[i] * sizeof(t_atom));
			}
		}
		/*
		int isaddress[argc];
		x->bufsize = 16;
		memset(isaddress, '\0', argc * sizeof(int));
		for(i = 0; i < argc; i++){
			if(argv[i].a_type == A_SYM){
				if(atom_getsym(argv + i)->s_name[0] == '/'){
					++(x->numAddresses);
					isaddress[i] = 1;
				}
			}
		}
		x->addresses = (t_symbol **)malloc(x->numAddresses * sizeof(t_symbol *));
		x->typetags = (char **)calloc(x->numAddresses, sizeof(char *));
		x->args = (t_atom **)malloc(x->numAddresses * sizeof(t_atom *));
		x->numArgs = (int *)calloc(x->numAddresses, sizeof(int));
		int j = -1;
		for(i = 0; i < argc; i++){
			if(isaddress[i]){
				j++;
				x->addresses[j] = atom_getsym(argv + i);
				int len = strlen(x->addresses[j]->s_name);
				len++;
				len += (4 - (len % 4));
				x->bufsize += 4; // size
				x->bufsize += len;

			}else{
				x->numArgs[j]++;
			}
		}

		for(i = 0; i < x->numAddresses; i++){
			if(x->numArgs[i] == 0){
				x->numArgs[i] = 1;
			}
			x->typetags[i] = calloc(x->numArgs[i], sizeof(char));
			memset(x->typetags[i], 'f', x->numArgs[i]);
			x->args[i] = (t_atom *)calloc(x->numArgs[i], sizeof(t_atom));
			x->bufsize += 4 - ((x->numArgs[i] + 2) % 4);
		}

		j = -1;
		int numInlets = 0;
		i = 0;
		while(i < argc){
			if(isaddress[i]){
				j++;
				i++;
			}else{
				int k = 0;
				while(!(isaddress[i])){
					t_atom a = argv[i];
					switch(a.a_type){
					case A_LONG:
						x->typetags[j][k] = 'i';
						x->args[j][k] = a;
						x->bufsize += 4;
						break;
					case A_FLOAT:
						x->typetags[j][k] = 'f';
						x->args[j][k] = a;
						x->bufsize += 4;
						break;
					case A_SYM:
						{
							int len;
							t_symbol *s = atom_getsym(&a);
							if((len = strlen(atom_getsym(&a)->s_name)) == 1){
								switch(s->s_name[0]){
								case 'i':
									atom_setlong(&a, 0);
									x->typetags[j][k] = 'i';
									x->args[j][k] = a;
									x->bufsize += 4;
									break;
								case 'f':
									atom_setfloat(&a, 0.);
									x->typetags[j][k] = 'f';
									x->args[j][k] = a;
									x->bufsize += 4;
									break;
								case 's':
									x->typetags[j][k] = 's';
									x->args[j][k] = a;
									x->bufsize += 32;
									break;
								case 'I':
									x->typetags[j][k] = 'I';
									break;
								default:
									x->typetags[j][k] = 's';
									x->args[j][k] = a;
									x->bufsize += 32;
									break;
								}
							}else{
								x->typetags[j][k] = 's';
								x->args[j][k] = a;
								x->bufsize += 32;
							}
					        }
						break;
					}
					k++;
					i++;
					if(i >= argc){
						break;
					}
					numInlets++;
				}
			}
		}
		*/

		x->proxy = (void **)sysmem_newptr(count * sizeof(void *));
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

	common_symbols_init();
	return 0;
}
