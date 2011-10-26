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
VERSION 0.1: Addresses to match can now have patterns
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
#include "osc_bundle_iterator_s.h"
#include "osc_message_s.h"
#include "osc_message_iterator_s.h"
#include "osc_message_u.h"
#include "omax_util.h"
#include "osc_vtable.h"
#include "osc_dispatch.h"

typedef struct _oroute{
	t_object ob;
	void **outlets;
	t_critical lock;
	t_symbol **args;
	int numArgs;
	t_osc_vtable *vtab;
	int max_message; // set this to note that the event originated as a max message and not a FullPacket
	t_osc_msg oscschemalist;
	struct _oroute_context *context_array;
} t_oroute;

typedef struct _oroute_context{
	t_oroute *x;
	void *outlet;
} t_oroute_context;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_oscschemalist, *ps_FullPacket;

void oroute_fullPacket(t_oroute *x, long len, long ptr){
#if (defined SELECT) || (defined SPEW)
	osc_dispatch(x->vtab, len, (char *)ptr, 0);
#else
	osc_dispatch(x->vtab, len, (char *)ptr, 1);
#endif
}

void oroute_dispatch_callback(t_osc_vtable_entry *e,
			      long bndllen,
			      char *bndl,
			      t_osc_bndl_s *partial_matches,
			      t_osc_bndl_s *complete_matches,
			      void *context)
{
#ifdef SELECT
	long len = osc_bundle_s_getLen(partial_matches) + osc_bundle_s_getLen(complete_matches);
	char concat[len];
	len = osc_bundle_s_concat(osc_bundle_s_getLen(partial_matches),
				  osc_bundle_s_getPtr(partial_matches),
				  osc_bundle_s_getLen(complete_matches),
				  osc_bundle_s_getPtr(complete_matches),
				  concat);
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)concat);
	outlet_anything(((t_oroute_context *)context)->outlet, ps_FullPacket, 2, out);
#elif defined SPEW
	t_osc_bndl_s *bndls[2] = {partial_matches, complete_matches};
	int i;
	for(i = 0; i < 2; i++){
		t_osc_bndl_s *b = bndls[i];
		if(b){
			t_osc_bndl_it_s *it = osc_bndl_it_s_get(osc_bundle_s_getLen(b),
								osc_bundle_s_getPtr(b));
			while(osc_bndl_it_s_hasNext(it)){
				t_osc_msg_s *msg = osc_bndl_it_s_next(it);
				int argc = osc_message_s_getArgCount(msg);
				t_atom atoms[argc + 1];
				omax_util_oscMsg2MaxAtoms(msg, atoms);
				t_symbol *address = atom_getsym(atoms);
				outlet_anything(((t_oroute_context *)context)->outlet, address, argc, atoms + 1);
			}
			osc_bndl_it_s_destroy(it);
		}
	}
#else // route
	if(partial_matches){
		t_atom out[2];
		atom_setlong(out, (long)(osc_bundle_s_getLen(partial_matches)));
		atom_setlong(out + 1, (long)(osc_bundle_s_getPtr(partial_matches)));
		outlet_anything(((t_oroute_context *)context)->outlet, ps_FullPacket, 2, out);
	}
	if(complete_matches){
		t_osc_bndl_it_s *it = osc_bndl_it_s_get(osc_bundle_s_getLen(complete_matches),
							osc_bundle_s_getPtr(complete_matches));
		while(osc_bndl_it_s_hasNext(it)){
			t_osc_msg_s *msg = osc_bndl_it_s_next(it);
			int argc = osc_message_s_getArgCount(msg);
			t_atom atoms[argc + 1];
			omax_util_oscMsg2MaxAtoms(msg, atoms);
			outlet_list(((t_oroute_context *)context)->outlet, NULL, argc, atoms + 1);
		}
		osc_bndl_it_s_destroy(it);
	}
#endif
}

void oroute_delegation_callback(long bndllen,
				char *bndl,
				t_osc_bndl_s *unmatched,
				void *context)
{
#ifdef SELECT
	if(unmatched){
		t_atom out[2];
		atom_setlong(out, osc_bundle_s_getLen(unmatched));
		atom_setlong(out + 1, (long)osc_bundle_s_getPtr(unmatched));
		outlet_anything(((t_oroute_context *)context)->outlet, ps_FullPacket, 2, out);
	}
#elif defined SPEW
	if(unmatched){
		t_osc_bndl_it_s *it = osc_bndl_it_s_get(osc_bundle_s_getLen(unmatched),
							osc_bundle_s_getPtr(unmatched));
		while(osc_bndl_it_s_hasNext(it)){
			t_osc_msg_s *msg = osc_bndl_it_s_next(it);
			int argc = osc_message_s_getArgCount(msg);
			t_atom atoms[argc + 1];
			omax_util_oscMsg2MaxAtoms(msg, atoms);
			t_symbol *address = atom_getsym(atoms);
			outlet_anything(((t_oroute_context *)context)->outlet, address, argc, atoms + 1);
		}
		osc_bndl_it_s_destroy(it);
	}
#else // route
	t_atom out[2];
	atom_setlong(out, (long)(osc_bundle_s_getLen(unmatched)));
	atom_setlong(out + 1, (long)(osc_bundle_s_getPtr(unmatched)));
	outlet_anything(((t_oroute_context *)context)->outlet, ps_FullPacket, 2, out);
#endif
}

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
	/*
	if(!msg){
		object_error((t_object *)x, "doesn't look like an OSC message");
		return;
	}
	if(msg->s_name[0] != '/'){
		object_error((t_object *)x, "OSC addresses must begin with a /");
		return;
	}
	int len = omax_util_get_bundle_size_for_atoms(msg, argc, argv);
	char buffer[len];
	memset(buffer, '\0', len);
	t_atom av[argc + 1];
	atom_setsym(av, msg);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	strncpy(buffer, "#bundle\0", 8);
	*((long long *)(buffer + 8)) = hton64(1ll);
	len = omax_util_encode_atoms(buffer + 16, msg, argc, argv);
	x->max_message = 1;

	oroute_fullPacket(x, len + 16, (long)buffer);
	*/
}

void oroute_set(t_oroute *x, long index, t_symbol *sym){
	if(index < 1 || index > x->numArgs){
		object_error((t_object *)x, "index (%d) out of bounds", index);
		return;
	}
	x->args[index - 1] = sym;
	/*
	x->haswildcard[index - 1] = 0;
	int i;
	for(i = 0; i < strlen(x->args[index - 1]->s_name); i++){
		switch(x->args[index - 1]->s_name[i]){
		case '*':
		case '[':
		case '{':
		case '?':
			x->haswildcard[index - 1] = 1;
		}
	}
	*/
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		if(a == x->numArgs){
			sprintf(s, "Unmatched messages (delegation)");
		}else{
			sprintf(s, "Messages that match %s", x->args[a]->s_name);
		}
	else {
		switch (a) {	
		case 0:
			sprintf(s,"OSC bundle (FullPacket) or Max message");
			break;
		}
	}
}

void oroute_free(t_oroute *x){
	critical_free(x->lock);
	if(x->args){
		free(x->args);
	}
	/*
	if(x->haswildcard){
		free(x->haswildcard);
	}
	*/
	// the address is 4 bytes into the block of memory we allocated in the new routine.
	if(x->oscschemalist.address){
		free(x->oscschemalist.address - 4);
	}
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv){
	t_oroute *x;
	int i;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		int numArgs = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((numArgs + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(numArgs * sizeof(t_symbol *));
		x->context_array = (t_oroute_context *)malloc((numArgs + 1) * sizeof(t_oroute_context));
		//x->haswildcard = (int *)calloc(numArgs, sizeof(int));
		x->numArgs = numArgs;
		critical_new(&(x->lock));
		x->outlets[numArgs] = outlet_new(x, "FullPacket"); // unmatched outlet
		int schemalist_len = 24 + numArgs + 2;
		while(schemalist_len % 4){
			schemalist_len++;
		}
		x->vtab = osc_vtable_alloc(numArgs);
		x->context_array[numArgs].x = x;
		x->context_array[numArgs].outlet = x->outlets[numArgs];
		osc_vtable_setDelegationMethod(x->vtab, oroute_delegation_callback, (void *)(x->context_array + numArgs));
		for(i = 0; i < numArgs; i++){
			x->outlets[numArgs - 1 - i] = outlet_new(x, NULL);
			if(atom_gettype(argv + i) != A_SYM){
				object_error((t_object *)x, "argument %d is not an OSC address", i);
				return NULL;
			}
			x->context_array[i].x = x;
			x->context_array[i].outlet = x->outlets[numArgs - 1 - i];
			osc_vtable_addEntry(x->vtab, atom_getsym(argv + (numArgs - 1 - i))->s_name, oroute_dispatch_callback, (void *)(x->context_array + i));
			x->args[i] = atom_getsym(argv + i);

			int len = strlen(x->args[i]->s_name);
			schemalist_len += len + 1;
			while(schemalist_len % 4){
				schemalist_len++;
			}
			/*
			for(j = 0; j < len; j++){
				switch(x->args[i]->s_name[j]){
				case '*':
				case '[':
				case '{':
				case '?':
					x->haswildcard[i] = 1;
					break;
				}
			}
			*/
		}
		char *schemalist = (char *)calloc(sizeof(char), schemalist_len);
		memset(schemalist, '\0', schemalist_len);
		omax_util_encode_atoms(schemalist, ps_oscschemalist, numArgs, argv);
		//osc_message_parseMessage(-1, schemalist, &(x->oscschemalist));
		
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
#ifdef SELECT
	char *name = "o.select";
#elif SPEW
	char *name = "o.spew";
#else
	char *name = "o.route";
#endif
	t_class *c = class_new(name, (method)oroute_new, (method)oroute_free, sizeof(t_oroute), 0L, A_GIMME, 0);
    	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oroute_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oroute_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oroute_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oroute_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oroute_set, "set", A_LONG, A_SYM, 0);

	//CLASS_ATTR_LONG(c, "bundle_partial_matches", 0, t_oroute, bundle_partial_matches);
    
	class_register(CLASS_BOX, c);
	oroute_class = c;

	ps_FullPacket = gensym("FullPacket");
	ps_oscschemalist = gensym("/osc/schema/list");

	common_symbols_init();
	return 0;
}
