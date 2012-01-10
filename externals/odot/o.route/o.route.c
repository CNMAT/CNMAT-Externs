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

#include "../odot_version.h"
#include "ext.h"
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
#include "osc_rset.h"
#include "osc_query.h"

typedef struct _oroute{
	t_object ob;
	void *delegation_outlet;
	void **outlets;
	t_critical lock;
	char **selectors;
	int num_selectors;
	char **unique_selectors;
	int num_unique_selectors;
	int max_message; // set this to note that the event originated as a max message and not a FullPacket
	char *schema;
	long schemalen;
	struct _oroute_context *context_array;
	void **proxy;
	long inlet;
} t_oroute;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_dispatch_rset(t_oroute *x, t_osc_rset *rset);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void oroute_set(t_oroute *x, long index, t_symbol *sym);
void oroute_doSet(t_oroute *x, long index, t_symbol *sym);
void oroute_makeSchema(t_oroute *x);
void oroute_spewBundle(void *outlet, long len, char *bndl);
void oroute_outputBundle(void *outlet, long len, char *bndl);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_oscschemalist, *ps_FullPacket;

void oroute_fullPacket(t_oroute *x, long len, long ptr)
{
	if(x->num_selectors > 0){
		t_osc_rset *rset = NULL;
		int strip_matched_portion_of_address = 1;
#if (defined SELECT) || (defined SPEW)
		strip_matched_portion_of_address = 0;
#endif
		osc_query_select(x->num_unique_selectors,
				 x->unique_selectors,
				 len,
				 (char *)ptr,
				 strip_matched_portion_of_address,
				 &rset);
		if(rset){
			oroute_dispatch_rset(x, rset);
			osc_rset_free(rset);
		}
	}else{
#ifdef SPEW
		oroute_spewBundle(x->delegation_outlet, len, (char *)ptr);
#else
		oroute_outputBundle(x->outlets[0], len, (char *)ptr);
#endif
	}
}

void oroute_dispatch_rset(t_oroute *x, t_osc_rset *rset)
{
	t_osc_bndl_s *unmatched = osc_rset_getUnmatched(rset);
	if(unmatched){
		oroute_outputBundle(x->delegation_outlet,
				    osc_bundle_s_getLen(unmatched),
				    osc_bundle_s_getPtr(unmatched));
	}
	int i;
	for(i = 0; i < x->num_selectors; i++){
		char *selector = x->selectors[i];
		t_osc_rset_result *res = osc_rset_select(rset, selector);
		if(res){
			t_osc_bndl_s *partial_matches = osc_rset_result_getPartialMatches(res);
			t_osc_bndl_s *complete_matches = osc_rset_result_getCompleteMatches(res);
#ifdef SELECT
			if(partial_matches || complete_matches){
				long len = osc_bundle_s_getLen(partial_matches) +
					osc_bundle_s_getLen(complete_matches);
				char concat[len];
				len = osc_bundle_s_concat(osc_bundle_s_getLen(partial_matches),
							  osc_bundle_s_getPtr(partial_matches),
							  osc_bundle_s_getLen(complete_matches),
							  osc_bundle_s_getPtr(complete_matches),
							  concat);
				oroute_outputBundle(x->outlets[i], len, concat);
			}
#else
			if(partial_matches){
#ifdef SPEW
				oroute_spewBundle(x->outlets[i],
						  osc_bundle_s_getLen(partial_matches),
						  osc_bundle_s_getPtr(partial_matches));
#else
				oroute_outputBundle(x->outlets[i],
						    osc_bundle_s_getLen(partial_matches),
						    osc_bundle_s_getPtr(partial_matches));
#endif
			}
			if(complete_matches){
#ifdef SPEW
				oroute_spewBundle(x->outlets[i],
						  osc_bundle_s_getLen(complete_matches),
						  osc_bundle_s_getPtr(complete_matches));
#else
				t_osc_bndl_it_s *it = osc_bndl_it_s_get(osc_bundle_s_getLen(complete_matches),
									osc_bundle_s_getPtr(complete_matches));
				while(osc_bndl_it_s_hasNext(it)){
					t_osc_msg_s *msg = osc_bndl_it_s_next(it);
					int num_atoms = omax_util_getNumAtomsInOSCMsg(msg);
					t_atom a[num_atoms];
					omax_util_oscMsg2MaxAtoms(msg, a);
					outlet_atoms(x->outlets[i], num_atoms - 1, a + 1);
				}
				osc_bndl_it_s_destroy(it);
#endif
			}
#endif
		}
	}
}

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
	long inlet = proxy_getinlet((t_object *)x);
	if(inlet > 0){
		oroute_doSet(x, inlet, msg);
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
	for(i = 0; i < x->num_selectors; i++){
		//int outletnum = x->num_selectors - i - 1;
		int outletnum = i;
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
			outlet_anything(x->outlets[outletnum], msg, argc, argv);
			match++;
		}
#else // route
		if((ret & OSC_MATCH_ADDRESS_COMPLETE) && ((ret & OSC_MATCH_PATTERN_COMPLETE))){
			// complete match
			if(argc){
				//outlet_list(x->outlets[i], NULL, argc, argv);
				outlet_atoms(x->outlets[outletnum], argc, argv);
			}else{
				outlet_bang(x->outlets[outletnum]);
			}
			match++;
		}else if(po > 0 && ((msg->s_name[po] == '/') || star_at_end == 1)){
			// partial match
			t_symbol *ss = gensym(msg->s_name + po);
			outlet_anything(x->outlets[outletnum], ss, argc, argv);
			match++;
		}else{
		}
#endif
	}
	if(!match){
		outlet_anything(x->delegation_outlet, msg, argc, argv);
	}
}

void oroute_set(t_oroute *x, long index, t_symbol *sym){
	oroute_doSet(x, index, sym);
}

void oroute_doSet(t_oroute *x, long index, t_symbol *sym){
	if(index < 1 || index > x->num_selectors){
		object_error((t_object *)x, "index (%d) out of bounds", index);
		return;
	}
	x->selectors[x->num_selectors - index] = sym->s_name;
	oroute_makeSchema(x);
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s)
{
	if(m == ASSIST_OUTLET){
		if(a == x->num_selectors){
			sprintf(s, "Unmatched messages (delegation)");
		}else{
			sprintf(s, "Messages that match %s", x->selectors[x->num_selectors - a - 1]);
		}
	}else{
		if(a == 0){
			sprintf(s, "OSC bundle or Max message");
		}else{
			sprintf(s, "Change the selector %s", x->selectors[x->num_selectors - a]);
		}
	}
}

void oroute_free(t_oroute *x)
{
	critical_free(x->lock);
	if(x->selectors){
		free(x->selectors);
	}
	if(x->schema){
		osc_mem_free(x->schema);
	}
}

void oroute_makeSchema(t_oroute *x)
{
	t_osc_bndl_u *bndl = osc_bundle_u_alloc();
	t_osc_msg_u *msg = osc_message_u_alloc();
	osc_message_u_setAddress(msg, "/osc/schema/list");

	int i;
	for(i = 0; i < x->num_selectors; i++){
		osc_message_u_appendString(msg, x->selectors[i]);
	}
	osc_bundle_u_addMsg(bndl, msg);
	osc_bundle_u_serialize(bndl, &(x->schemalen), &(x->schema));
}

void oroute_spewBundle(void *outlet, long len, char *bndl)
{
	t_osc_bndl_it_s *it = osc_bndl_it_s_get(len, bndl);
	while(osc_bndl_it_s_hasNext(it)){
		t_osc_msg_s *msg = osc_bndl_it_s_next(it);
		int argc = osc_message_s_getArgCount(msg);
		int natoms = omax_util_getNumAtomsInOSCMsg(msg);
		t_atom atoms[natoms];
		omax_util_oscMsg2MaxAtoms(msg, atoms);
		t_symbol *address = atom_getsym(atoms);
		outlet_anything(outlet, address, natoms - 1, atoms + 1);
	}
	osc_bndl_it_s_destroy(it);
}

void oroute_outputBundle(void *outlet, long len, char *bndl)
{
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)bndl);
	outlet_anything(outlet, ps_FullPacket, 2, out);
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv)
{
	t_oroute *x;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		critical_new(&(x->lock));
		x->delegation_outlet = outlet_new(x, "FullPacket"); // unmatched outlet
		x->outlets = (void **)malloc(argc * sizeof(void *));
		x->proxy = (void **)malloc(argc * sizeof(void *));
		x->selectors = (char **)malloc(argc * sizeof(char *));
		x->num_selectors = argc;

		char *unique_selectors[argc];
		int num_unique_selectors = 0;
		int i;
		for(i = 0; i < argc; i++){
			x->outlets[i] = outlet_new(x, NULL);
			x->proxy[i] = proxy_new((t_object *)x, argc - i, &(x->inlet));
			if(atom_gettype(argv + i) != A_SYM){
				object_error((t_object *)x, "argument %d is not an OSC address", i);
				return NULL;
			}

			char *selector = atom_getsym(argv + i)->s_name;
			x->selectors[x->num_selectors - i - 1] = selector;

			int match = 0;
			int j;
			for(j = 0; j < num_unique_selectors; j++){
				if(!strcmp(unique_selectors[j], selector)){
					match++;
					break;
				}
			}
			if(!match){
				unique_selectors[num_unique_selectors++] = selector;
			}
		}
		x->unique_selectors = (char **)malloc(num_unique_selectors * sizeof(char *));
		memcpy(x->unique_selectors, unique_selectors, num_unique_selectors * sizeof(char **));
		x->num_unique_selectors = num_unique_selectors;

		x->schema = NULL;
		x->schemalen = 0;
		oroute_makeSchema(x);
		
		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void)
{
#ifdef SELECT
	char *name = "o.select";
#elif SPEW
	char *name = "o.spew";
#else
	char *name = "o.route";
#endif
	t_class *c = class_new(name, (method)oroute_new, (method)oroute_free, sizeof(t_oroute), 0L, A_GIMME, 0);
	class_addmethod(c, (method)oroute_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oroute_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)oroute_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oroute_set, "set", A_LONG, A_SYM, 0);

	class_register(CLASS_BOX, c);
	oroute_class = c;

	ps_FullPacket = gensym("FullPacket");
	ps_oscschemalist = gensym("/osc/schema/list");

	common_symbols_init();
	ODOT_PRINT_VERSION;
	return 0;
}
