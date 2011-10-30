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
#include "osc_match.h"
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
	char **selectors;
	int nselectors;
	t_osc_vtable *vtab;
	int max_message; // set this to note that the event originated as a max message and not a FullPacket
	char *schema;
	long schemalen;
	struct _oroute_context *context_array;
	void **proxy;
	long inlet;
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
void oroute_set(t_oroute *x, long index, t_symbol *sym);
void oroute_makeSchema(t_oroute *x);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_oscschemalist, *ps_FullPacket;

void oroute_fullPacket(t_oroute *x, long len, long ptr){
#if (defined SELECT) || (defined SPEW)
	if(x->nselectors > 0){
		osc_dispatch_selectors(x->vtab, x->nselectors, x->selectors, len, (char *)ptr, 0);
	}else{
		osc_dispatch(x->vtab, len, (char *)ptr, 0);
	}
#else
	osc_dispatch_selectors(x->vtab, x->nselectors, x->selectors, len, (char *)ptr, 1);
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
	long inlet = proxy_getinlet((t_object *)x);
	if(inlet > 0){
		oroute_set(x, inlet, msg);
		return;
	}
	if(!msg){
		object_error((t_object *)x, "doesn't look like an OSC message");
		return;
	}
	if(msg->s_name[0] != '/'){
		object_error((t_object *)x, "OSC addresses must begin with a /");
		return;
	}
	int i;
	int match = 0;
	for(i = 0; i < x->nselectors; i++){
		char *s = x->selectors[i];
		int ret, ao, po;
		ret = osc_match(msg->s_name, s, &po, &ao);
		// if the match failed because a trailing star at the end of the pattern didn't match,
		// say /foo/* and /foo, we'll call it a match
		int star_at_end = 0;
		if(msg->s_name[po] == '*' && msg->s_name[po + 1] == '\0'){
			star_at_end = 1;
		}
#if defined SELECT || defined SPEW
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && ((ret & OSC_MATCH_PATTERN_COMPLETE)) ||
		   (po > 0 && ((msg->s_name[po] == '/') || star_at_end == 1))){
			outlet_anything(x->outlets[i], msg, argc, argv);
			match++;
		}
#else // route
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && ((ret & OSC_MATCH_PATTERN_COMPLETE))){
			// complete match
			if(argc){
				outlet_list(x->outlets[i], NULL, argc, argv);
			}else{
				outlet_bang(x->outlets[i]);
			}
			match++;
		}else if(po > 0 && ((msg->s_name[po] == '/') || star_at_end == 1)){
			// partial match
			t_symbol *ss = gensym(msg->s_name + po);
			outlet_anything(x->outlets[i], ss, argc, argv);
			match++;
		}else{
		}
#endif
	}
	if(!match){
		outlet_anything(x->outlets[x->nselectors], msg, argc, argv);
	}
}

void oroute_set(t_oroute *x, long index, t_symbol *sym){
	if(index < 1 || index > x->nselectors){
		object_error((t_object *)x, "index (%d) out of bounds", index);
		return;
	}
	osc_vtable_renameEntry(x->vtab, x->selectors[x->nselectors - (index)], sym->s_name);
	x->selectors[x->nselectors - (index - 1)] = sym->s_name;
	oroute_makeSchema(x);
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		if(a == x->nselectors){
			sprintf(s, "Unmatched messages (delegation)");
		}else{
			sprintf(s, "Messages that match %s", x->selectors[x->nselectors - a - 1]);
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
	if(x->selectors){
		free(x->selectors);
	}
	/*
	if(x->haswildcard){
		free(x->haswildcard);
	}
	*/
	// the address is 4 bytes into the block of memory we allocated in the new routine.
	if(x->vtab){
		osc_vtable_free(x->vtab);
	}
	if(x->context_array){
		free(x->context_array);
	}
	if(x->schema){
		osc_mem_free(x->schema);
	}
}

void oroute_makeSchema(t_oroute *x){
	t_osc_bndl_u *bndl = osc_bundle_u_alloc();
	t_osc_msg_u *msg = osc_message_u_alloc();
	osc_message_u_setAddress(msg, "/osc/schema/list");

	int i;
	for(i = 0; i < x->nselectors; i++){
		osc_message_u_appendString(msg, x->selectors[i]);
	}
	osc_bundle_u_addMsg(bndl, msg);
	osc_bundle_u_serialize(bndl, &(x->schemalen), &(x->schema));
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv){
	t_oroute *x;
	int i;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		int nselectors = attr_args_offset(argc, argv);
		x->outlets = (void **)malloc((nselectors + 1) * sizeof(void *));
		x->selectors = (char **)malloc(nselectors * sizeof(char *));
		x->context_array = (t_oroute_context *)malloc((nselectors + 1) * sizeof(t_oroute_context));
		x->nselectors = nselectors;
		critical_new(&(x->lock));
		x->outlets[nselectors] = outlet_new(x, "FullPacket"); // unmatched outlet
		x->proxy = (void **)malloc((x->nselectors) * sizeof(void));

		x->vtab = osc_vtable_alloc(nselectors);
		x->context_array[nselectors].x = x;
		x->context_array[nselectors].outlet = x->outlets[nselectors];
		osc_vtable_setDelegationMethod(x->vtab, oroute_delegation_callback, (void *)(x->context_array + nselectors));
		for(i = 0; i < nselectors; i++){
			x->outlets[nselectors - 1 - i] = outlet_new(x, NULL);
			x->proxy[i] = proxy_new((t_object *)x, argc - i, &(x->inlet));
			if(atom_gettype(argv + i) != A_SYM){
				object_error((t_object *)x, "argument %d is not an OSC address", i);
				return NULL;
			}
			x->context_array[i].x = x;
			x->context_array[i].outlet = x->outlets[nselectors - 1 - i];
			osc_vtable_addEntry(x->vtab, atom_getsym(argv + (nselectors - 1 - i))->s_name, oroute_dispatch_callback, (void *)(x->context_array + i));
			x->selectors[x->nselectors - i - 1] = atom_getsym(argv + i)->s_name;
		}
		x->schema = NULL;
		x->schemalen = 0;
		oroute_makeSchema(x);
		
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
