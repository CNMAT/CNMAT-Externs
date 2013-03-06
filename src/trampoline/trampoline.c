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
  NAME: trampoline
  DESCRIPTION: Trampoline tail call optimization for Max
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2011
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 1.0: Handles FullPackets properly
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define NAME "trampoline"
#define DESCRIPTION "Trampoline tail call optimization for Max"
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2011"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include <execinfo.h>

enum{
	T_ANYTHING,
	T_LIST,
	T_FLOAT,
	T_INT,
	T_BANG,
	T_FULLPACKET
};

typedef struct _trampoline{
	t_object ob;
	void **outlets;
	int noutlets;
	int which_outlet;
	void **proxies;
	long inlet;
	int outlet_type;
	t_symbol *msg;
	int argc;
	t_atom *argv;
	int argv_size;
	long fullpacket_len, fullpacket_size;
	char *fullpacket;
	int should_continue;
	int calling_outlet;
} t_trampoline;

void *trampoline_class;

t_symbol *ps_FullPacket;

void trampoline_anything(t_trampoline *x, t_symbol *msg, long argc, t_atom *argv);
void trampoline_list(t_trampoline *x, t_symbol *msg, long argc, t_atom *argv);
void trampoline_int(t_trampoline *x, long l);
void trampoline_float(t_trampoline *x, double f);
void trampoline_bang(t_trampoline *x);
void trampoline_fullPacket(t_trampoline *x, long len, long ptr);
void trampoline_execute(t_trampoline *x, int outlet);
void trampoline_free(t_trampoline *x);
void trampoline_assist(t_trampoline *x, void *b, long m, long a, char *s);
void *trampoline_new(t_symbol *msg, short argc, t_atom *argv);


void trampoline_anything(t_trampoline *x, t_symbol *msg, long argc, t_atom *argv)
{
	x->outlet_type = T_ANYTHING;
	if(argc > x->argv_size){
		x->argv = (t_atom *)sysmem_resizeptr(x->argv, (argc) * sizeof(t_atom));
		if(!(x->argv)){
			object_error((t_object *)x, "out of memory!");
			return;
		}
		x->argv_size = argc;
	}
	x->argc = argc;
	memcpy(x->argv, argv, (argc) * sizeof(t_atom));
	x->msg = msg;
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_list(t_trampoline *x, t_symbol *msg, long argc, t_atom *argv)
{
	x->outlet_type = T_LIST;
	if(argc > x->argv_size){
		x->argv = (t_atom *)sysmem_resizeptr(x->argv, (argc) * sizeof(t_atom));
		if(!(x->argv)){
			object_error((t_object *)x, "out of memory!");
			return;
		}
		x->argv_size = argc;
	}
	x->argc = argc;
	memcpy(x->argv, argv, (argc) * sizeof(t_atom));
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_int(t_trampoline *x, long l)
{
	x->outlet_type = T_INT;
	atom_setlong(x->argv, l);
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_float(t_trampoline *x, double f)
{
	x->outlet_type = T_FLOAT;
	atom_setfloat(x->argv, f);
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_bang(t_trampoline *x)
{
	x->outlet_type = T_BANG;
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_fullPacket(t_trampoline *x, long len, long ptr)
{
	x->outlet_type = T_FULLPACKET;
	if(len > x->fullpacket_size){
		x->fullpacket = (char *)sysmem_resizeptr(x->fullpacket, len * sizeof(t_atom));
		if(!(x->fullpacket)){
			object_error((t_object *)x, "out of memory!");
			return;
		}
		x->fullpacket_size = len;
	}
	x->fullpacket_len = len;
	memcpy(x->fullpacket, (char *)ptr, len);
	trampoline_execute(x, proxy_getinlet((t_object *)x));
}

void trampoline_execute(t_trampoline *x, int outlet)
{
	if(outlet >= x->noutlets){
		object_error((t_object *)x, "%d is greater than the number of outlets", outlet);
		x->should_continue = 0;
		return;
	}
	if(x->calling_outlet){
		x->which_outlet = outlet;
		x->should_continue = 1;
	}else{
		x->should_continue = 1;
		x->which_outlet = outlet;
		while(x->should_continue){
			x->should_continue = 0;
			x->calling_outlet = 1;
			outlet = x->which_outlet;
			switch(x->outlet_type){
			case T_ANYTHING:
				{
					int argc = x->argc;
					t_atom argv[argc];
					memcpy(argv, x->argv, argc * sizeof(t_atom));
					outlet_anything(x->outlets[outlet], x->msg, argc, argv);
				}
				break;
			case T_LIST:
				{
					int argc = x->argc;
					t_atom argv[argc];
					memcpy(argv, x->argv, argc * sizeof(t_atom));
					outlet_list(x->outlets[outlet], NULL, argc, argv);
				}
				break;
			case T_FLOAT:
				outlet_float(x->outlets[outlet], atom_getfloat(x->argv));
				break;
			case T_INT:
				outlet_int(x->outlets[outlet], atom_getlong(x->argv));
				break;
			case T_BANG:
				outlet_bang(x->outlets[outlet]);
				break;
			case T_FULLPACKET:
				{
					t_atom out[2];
					int len = x->fullpacket_len;
					char buf[len];
					memcpy(buf, x->fullpacket, len);
					atom_setlong(out, len);
					atom_setlong(out + 1, (long)buf);
					outlet_anything(x->outlets[outlet], ps_FullPacket, 2, out);
				}
				break;
			}
			x->calling_outlet = 0;
		}
	}
}

void trampoline_assist(t_trampoline *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void trampoline_free(t_trampoline *x)
{
	if(x->outlets){
		free(x->outlets);
	}
	if(x->proxies){
		for(int i = 0; i < x->noutlets - 1; i++){
			object_free((t_object *)(x->proxies[i]));
		}
		free(x->proxies);
	}
	if(x->argv){
		sysmem_freeptr(x->argv);
	}
	if(x->fullpacket){
		sysmem_freeptr(x->fullpacket);
	}
}

void *trampoline_new(t_symbol *msg, short argc, t_atom *argv)
{	
	t_trampoline *x;
	if((x = (t_trampoline *)object_alloc(trampoline_class))){
		int i;
		x->noutlets = 1;
		if(argc){
			x->noutlets = atom_getlong(argv);
		}
		x->outlets = (void **)malloc(x->noutlets * sizeof(void *));
		for(i = x->noutlets - 1; i >= 0; i--){
			x->outlets[i] = outlet_new(x, NULL);
		}
		x->proxies = (void **)malloc((x->noutlets - 1) * sizeof(void *));
		for(i = 0; i < x->noutlets - 1; i++){
			x->proxies[i] = proxy_new((t_object *)x, i + 1, &(x->inlet));
		}
		x->argv_size = 128;
		x->argv = (t_atom *)sysmem_newptr(x->argv_size * sizeof(t_atom));
		x->argc = 0;
		x->fullpacket_size = 256;
		x->fullpacket_len = 0;
		x->fullpacket = (char *)sysmem_newptr(x->fullpacket_size * sizeof(char));
		x->msg = NULL;
		x->should_continue = 0;
		x->calling_outlet = 0;
		x->which_outlet = 0;
	}
		   	
	return(x);
}

int main(void)
{
	t_class *c = class_new("trampoline", (method)trampoline_new, (method)trampoline_free, sizeof(t_trampoline), 0L, A_GIMME, 0);
	class_addmethod(c, (method)trampoline_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)trampoline_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)trampoline_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)trampoline_int, "int", A_LONG, 0);
	class_addmethod(c, (method)trampoline_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)trampoline_bang, "bang", 0);
	class_addmethod(c, (method)trampoline_fullPacket, "FullPacket", A_LONG, A_LONG, 0);

	class_addmethod(c, (method)version, "version", 0);

	class_register(CLASS_BOX, c);
	trampoline_class = c;
	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
	version_post_copyright();
	return 0;
}
