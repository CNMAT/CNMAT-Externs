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
  NAME: o.var
  DESCRIPTION: A variable to store OSC bundles
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2011
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 1.0: using libo
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_critical.h"
#include "osc.h"
#include "osc_mem.h"
#include "osc_bundle_s.h"
#include "omax_util.h"

typedef struct _ovar{
	t_object ob;
	void *outlet;
	void *proxy;
	long inlet;
	long len;
	char *bndl;
	long buflen;
	t_critical lock;
} t_ovar;

void *ovar_class;

void ovar_fullPacket(t_ovar *x, long len, long ptr);
void ovar_clear(t_ovar *x);
void ovar_anything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv);

void ovar_bang(t_ovar *x);

void ovar_free(t_ovar *x);
void ovar_assist(t_ovar *x, void *b, long m, long a, char *s);
void *ovar_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err ovar_notify(t_ovar *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_symbol *ps_FullPacket;

void ovar_doFullPacket(t_ovar *x, long len, long ptr, long inlet){
	if(inlet == 1){
		critical_enter(x->lock);
		if(len > x->buflen){
			x->bndl = osc_mem_resize(x->bndl, len);
			if(!(x->bndl)){
				object_error((t_object *)x, "ran out of memory!\n");
				return;
			}
			x->buflen = len;
		}
		memcpy(x->bndl, (char *)ptr, len);
		x->len = len;
		critical_exit(x->lock);
	}else{
#if (defined UNION || defined INTERSECTION || defined DIFFERENCE)
		critical_enter(x->lock);
		long copylen = x->len;
		char copy[copylen];
		memcpy(copy, x->bndl, copylen);
		critical_exit(x->lock);
		long bndllen = 0;
		char *bndl = NULL;
#ifdef UNION
		osc_bundle_s_union(len, (char *)ptr, copylen, copy, &bndllen, &bndl);
#elif defined INTERSECTION
		osc_bundle_s_intersection(len, (char *)ptr, copylen, copy, &bndllen, &bndl);
#elif defined DIFFERENCE
		osc_bundle_s_difference(len, (char *)ptr, copylen, copy, &bndllen, &bndl);
#endif
		t_atom out[2];
		atom_setlong(out, bndllen);
		atom_setlong(out + 1, (long)bndl);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
#else // o.var
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, ptr);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
#endif
	}
}

void ovar_fullPacket(t_ovar *x, long len, long ptr){
	ovar_doFullPacket(x, len, ptr, proxy_getinlet((t_object *)x));
}

void ovar_clear(t_ovar *x){
	critical_enter(x->lock);
	osc_mem_free(x->bndl);
	critical_exit(x->lock);
}

void ovar_anything(t_ovar *x, t_symbol *msg, int argc, t_atom *argv){
	t_symbol *address = NULL;
	if(msg){
		if(*(msg->s_name) != '/'){
			object_error((t_object *)x, "OSC address must begin with a '/'");
			return;
		}
		address = msg;
	}else{
		if(atom_gettype(argv) == A_SYM){
			if(*(atom_getsym(argv)->s_name) != '/'){
				object_error((t_object *)x, "OSC address must begin with a '/'");
				return;
			}
			address = atom_getsym(argv);
			argv++;
			argc--;
		}
	}
	if(!address){
		object_error((t_object *)x, "no OSC address found");
		return;
	}

	int len = omax_util_get_bundle_size_for_atoms(address, argc, argv);
	char buf[len];
	memset(buf, '\0', len);
	omax_util_encode_atoms(buf + 16, address, argc, argv);
	strncpy(buf, "#bundle\0", 8);
	ovar_doFullPacket(x, len, (long)buf, proxy_getinlet((t_object *)x));
}

void ovar_bang(t_ovar *x){
	if(proxy_getinlet((t_object *)x) == 1){
		return;
	}
#if (defined UNION || defined INTERSECTION || defined DIFFERENCE)
	char emptybndl[] = {'#', 'b', 'u', 'n', 'd', 'l', 'e', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
	ovar_fullPacket(x, sizeof(emptybndl), (long)emptybndl);
#else
	critical_enter(x->lock);
	long len = x->len;
	char bndl[len];
	memcpy(bndl, x->bndl, len);
	critical_exit(x->lock);
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)bndl);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
#endif
}

void ovar_assist(t_ovar *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
#ifdef UNION
		sprintf(s,"FullPacket: Union of the left and right inlets");
#elif defined INTERSECTION
		sprintf(s,"FullPacket: Intersection of the left and right inlets");
#elif defined DIFFERENCE
		sprintf(s,"FullPacket: Difference between the left and right inlets");
#else
		sprintf(s, "FullPacket: Stored bundle");
#endif
	}else{
		switch(a){	
		case 0:
#ifdef UNION
			sprintf(s,"FullPacket: Compute union and output");
#elif defined INTERSECTION
			sprintf(s,"FullPacket: Compute intersection and output");
#elif defined DIFFERENCE
			sprintf(s,"FullPacket: Compute difference and output");
#else
			sprintf(s, "FullPacket: Copy and output bundle");
#endif
			break;
		case 1:
			sprintf(s, "FullPacket: Store a bundle without output");
			break;
		}
	}
}

void ovar_free(t_ovar *x){	
	object_free(x->proxy);
	ovar_clear(x);
	critical_free(x->lock);
}

void *ovar_new(t_symbol *msg, short argc, t_atom *argv){
	t_ovar *x;
	if(x = (t_ovar *)object_alloc(ovar_class)){
		x->outlet = outlet_new((t_object *)x, "FullPacket");
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		critical_new(&(x->lock));
		x->len = 0;
		x->buflen = 0;
		x->bndl = NULL;

		//attr_args_process(x, argc, argv);

#if !defined UNION && !defined INTERSECTION && !defined DIFFERENCE
		int nargs = attr_args_offset(argc, argv);
		if(nargs){
			if(atom_gettype(argv)){
				if(osc_error_validateAddress(atom_getsym(argv)->s_name)){
					object_error((t_object *)x, "arguments must begin with a valid OSC address");
					return NULL;
				}
			}else{
				object_error((t_object *)x, "arguments must begin with a valid OSC address");
				return NULL;
			}
		}
#endif
	}
		   	
	return x;
}

int main(void){
#ifdef UNION
	t_class *c = class_new("o.union", (method)ovar_new, (method)ovar_free, sizeof(t_ovar), 0L, A_GIMME, 0);
#elif defined INTERSECTION
	t_class *c = class_new("o.intersection", (method)ovar_new, (method)ovar_free, sizeof(t_ovar), 0L, A_GIMME, 0);
#elif defined DIFFERENCE
	t_class *c = class_new("o.difference", (method)ovar_new, (method)ovar_free, sizeof(t_ovar), 0L, A_GIMME, 0);
#else
	t_class *c = class_new("o.var", (method)ovar_new, (method)ovar_free, sizeof(t_ovar), 0L, A_GIMME, 0);
#endif
	class_addmethod(c, (method)ovar_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)ovar_assist, "assist", A_CANT, 0);
	//class_addmethod(c, (method)ovar_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)ovar_bang, "bang", 0);
	class_addmethod(c, (method)ovar_anything, "anything", A_GIMME, 0);

	class_addmethod(c, (method)ovar_clear, "clear", 0);

	class_register(CLASS_BOX, c);
	ovar_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");

	version(0);
	return 0;
}

t_max_err ovar_notify(t_ovar *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}
