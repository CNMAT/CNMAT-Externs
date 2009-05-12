/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and
Audio Technologies, University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "cmmjl.h"
#include "cmmjl_osc.h"
#include "cmmjl_obj.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h"
#include "ext_linklist.h"

t_cmmjl_error cmmjl_obj_init(void *x, 
			     t_cmmjl_obj *o, 
			     unsigned long flags,
			     const char *name,
			     long instance)
{
	//t_atom a;
	o->error = cmmjl_default_error_handler;
	if(flags & CMMJL_CREATE_INFO_OUTLET){
		o->info_outlet = outlet_new(x, (char *)name);
	}else{
		o->info_outlet = (char *)name;
	}
	o->entrance_count_tab = (t_hashtab *)hashtab_new(0);

	o->instance = instance;

	o->osc_address = NULL;
	o->osc_address_methods = NULL;
	o->osc_parser = NULL;
	o->osc_parser_cb = NULL;

	if(object_attr_get(x, ps_OSCaddress)){
		o->osc_parser = cmmjl_osc_parseFullPacket;
		o->osc_parser_cb = cmmjl_osc_sendMsg;
		o->osc_should_schedule = flags & CMMJL_OSC_SCHEDULER_ON;

		OBJ_ATTR_SAVE(x, "OSCaddress", 0L);

		cmmjl_osc_schedule_init(x, o, flags);

		//atom_setlong(&a, 0);
		//object_attr_attr_setvalueof(x, ps_OSCaddress, gensym("save"), 1, &a);

		// Now schedule the rest of the initialization for 50ms from now since
		// we need to be sure that all objs have been instantiated before we 
		// start making our OSC address.
		schedule_defer(x, (method)cmmjl_obj_init_del, 50, NULL, 0, NULL);
	}
	return CMMJL_SUCCESS;
}

void cmmjl_obj_init_del(void *x, t_symbol *sym, short argc, t_atom *argv){
	//post("cmmjl_obj_init_del");
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(o->osc_address){
		//post("address = %s", o->osc_address);
		// We already have an address presumably set by Max due to it 
		// being saved with the patcher.
		return;
	}
	char buf[256];
	cmmjl_osc_makeDefaultAddress(x, cmmjl_obj_instance_get(x), buf);
	//post("buf = %s", buf);
	o->osc_address = gensym(buf);

	// cmmjl_osc_saveAddressWithPatcher() would do this, but it will set the patcher's 
	// "dirty" bit which we don't want at this point.
	t_atom a;
	atom_setlong(&a, 0);
	object_attr_attr_setvalueof(x, ps_OSCaddress, gensym("save"), 1, &a);
	//cmmjl_osc_saveAddressWithPatcher(x, false);

	o->osc_address_methods = (t_linklist *)linklist_new();
	linklist_flags(o->osc_address_methods, OBJ_FLAG_MEMORY);
	int i = 0;
	char *buf_ptr;
	// we're relying on the object being the first element of the struct
	while(((t_object *)x)->o_messlist[i].m_sym){
		if(((t_object *)x)->o_messlist[i].m_sym->s_name[0] == '/'){
			sprintf(buf, "%s%s", o->osc_address->s_name, ((t_object *)x)->o_messlist[i].m_sym->s_name);
			buf_ptr = (char *)calloc(sizeof(char), strlen(buf) + 1);
			strcpy(buf_ptr, buf);
			linklist_append(o->osc_address_methods, buf_ptr);
		}
		i++;
	}
}

t_cmmjl_obj *cmmjl_obj_get(void *x){
	if(!_cmmjl_obj_tab){
		return NULL;
	}
	t_max_err e;
	t_cmmjl_obj *o;
	if(e = hashtab_lookup(_cmmjl_obj_tab, x, (t_object **)&o)){
		return NULL;
	}
	return o;
}

void *cmmjl_obj_info_outlet_get(void *x){
	if(!_cmmjl_obj_tab){
		return NULL;
	}
	t_object *o;
	t_max_err err;
	if(err = hashtab_lookup(_cmmjl_obj_tab, x, &o)){
		return NULL;
	}
	return ((t_cmmjl_obj *)o)->info_outlet;
}

t_cmmjl_error cmmjl_obj_info_outlet_set(void *x, void *outlet){
	if(!_cmmjl_obj_tab){
		return CMMJL_ENOOBJ;
	}
	t_object *o;
	t_max_err err;
	if(err = hashtab_lookup(_cmmjl_obj_tab, x, &o)){
		return CMMJL_FAILURE;
	}
	((t_cmmjl_obj *)o)->info_outlet = outlet;
	return CMMJL_SUCCESS;
}

long cmmjl_obj_instance_get(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return -1;
	}
	return o->instance;
}

void cmmjl_obj_instance_mark_used(void *x, long instance){
	cmmjl_obj_instance_mark_val(x, instance, 1);
}

void cmmjl_obj_instance_mark_free(void *x, long instance){
	cmmjl_obj_instance_mark_val(x, instance, 0);
}

void cmmjl_obj_instance_mark_val(void *x, long instance, long val){
	t_symbol *name = object_classname(x);
	long *c;
	hashtab_lookup(_cmmjl_instance_count, name, (t_object **)&c);
	c[instance] = val;
}

void *cmmjl_obj_osc_parser_get(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return NULL;
	}
	return o->osc_parser;
}

t_cmmjl_error cmmjl_obj_osc_parser_set(void *x, 
				       t_cmmjl_error(*cb)(void*,long,long,
							  void(*)(void*,t_symbol*,int,t_atom*)))
{
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return CMMJL_ENOOBJ;
	}
	o->osc_parser = cb;
	return CMMJL_SUCCESS;
}

void *cmmjl_obj_osc_parser_cb_get(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return NULL;
	}
	return o->osc_parser_cb;
}

t_cmmjl_error cmmjl_obj_osc_parser_cb_set(void *x, void (*cb)(void*,t_symbol*,int,t_atom*)){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return CMMJL_ENOOBJ;
	}
	o->osc_parser_cb = cb;
	return CMMJL_SUCCESS;
}

t_linklist *cmmjl_obj_osc_address_methods_get(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return NULL;
	}
	return o->osc_address_methods;
}

t_cmmjl_error cmmjl_obj_osc_address_methods_add(void *x, char *address){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return CMMJL_ENOOBJ;
	}
	linklist_append(o->osc_address_methods, gensym(address));
}
