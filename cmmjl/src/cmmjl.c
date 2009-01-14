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

#include "cmmjl_errno.h"
#include "cmmjl_error.h"
#include "cmmjl.h"
#include "cmmjl_commonsymbols.h"

static void cmmjl_reformat_obj_name(char *name);

t_hashtab *_cmmjl_obj_tab;
t_hashtab *_cmmjl_instance_count;

/** 	@cond
	Initializes an object used by the library to store things like the error
	handler, the info outlet, etc.

	@param	x		The object
	@param 	o		The t_cmmjl_obj object
	@param	name		The name of the object
	@param 	instance	The instance number

	@returns	An error or CMMJL_SUCCESS
*/
t_cmmjl_error cmmjl_init_obj(void *x, 
			     t_cmmjl_obj *o, 
			     bool shouldCreateInfoOutlet, 
			     const char *name,
			     long instance);
/* 	@endcond */

t_cmmjl_error cmmjl_init(void *x, const char *name, bool shouldCreateInfoOutlet){
	t_cmmjl_error err;

	// initialize symbol table. This function checks to see if it's 
	// already been done and does nothing if so.
	cmmjl_common_symbols_init();

	// Create a new hashtable if we haven't done that yet.
	if(!_cmmjl_obj_tab){
		_cmmjl_obj_tab = (t_hashtab *)hashtab_new(0);
	}

	// Create a hashtab to keep track of instance counts for all the objects.
	// This is used to create unique OSC addresses for all instances of all objects
	// which use the OSC part of this lib.
	t_symbol *s_name = gensym((char *)name);
	if(!_cmmjl_instance_count){
		_cmmjl_instance_count = (t_hashtab *)hashtab_new(0);
	}
	t_object *count;
	long c;
	hashtab_lookup(_cmmjl_instance_count, s_name, &count);
	c = (long)count;
	c += 1;
	hashtab_store_safe(_cmmjl_instance_count, s_name, (t_object *)c);

	// Create a data structure to hold our internal data and add its 
	// address to our hash table
	t_cmmjl_obj *o = (t_cmmjl_obj *)malloc(sizeof(t_cmmjl_obj));
	if(err = cmmjl_init_obj(x, o, shouldCreateInfoOutlet, name, c)){
		error("cmmjl: couldn't allocate object (%d)", err);
		return err;
	}
	hashtab_store(_cmmjl_obj_tab, x, (t_object *)o);

	return CMMJL_SUCCESS;
}

t_cmmjl_error cmmjl_init_obj(void *x, 
			     t_cmmjl_obj *o, 
			     bool shouldCreateInfoOutlet, 
			     const char *name,
			     long instance)
{
	o->error = cmmjl_default_error_handler;
	if(shouldCreateInfoOutlet){
		o->info_outlet = outlet_new(x, (char *)name);
	}else{
		o->info_outlet = (char *)name;
	}
	o->entrance_count_tab = (t_hashtab *)hashtab_new(0);

	// We'll just blindly set this in the default manner since 
	// if the name has been saved with the patcher, Max will 
	// override this name
	char ad[256];
	char ad2[256];
	sprintf(ad, "/%s/%d", name, instance);
	o->osc_address = gensym(ad);

	o->osc_address_methods = (t_linklist *)linklist_new();
	// make sure the items will never be freed
	linklist_flags(o->osc_address_methods, OBJ_FLAG_DATA);
	int i = 0;
	// we're relying on the object being the first element of the struct
	while(((t_object *)x)->o_messlist[i].m_sym){
		if(((t_object *)x)->o_messlist[i].m_sym->s_name[0] == '/'){
			sprintf(ad2, "%s%s", ad, ((t_object *)x)->o_messlist[i].m_sym->s_name);
			linklist_append(o->osc_address_methods, ad2);
		}
		i++;
	}

	return CMMJL_SUCCESS;
}

void *cmmjl_info_outlet_get(void *x){
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

void cmmjl_info_outlet_set(void *x, void *outlet){
	if(!_cmmjl_obj_tab){
		return;
	}
	t_object *o;
	t_max_err err;
	if(err = hashtab_lookup(_cmmjl_obj_tab, x, &o)){
		return;
	}
	((t_cmmjl_obj *)o)->info_outlet = outlet;
}

void cmmjl_post_gimme(void *x, t_symbol *msg, int argc, t_atom *argv){
	int i;
	t_atom a;
	PINFO("x = %p", x);
	PINFO("msg = %s", msg->s_name);
	PINFO("argc = %d", argc);
	PINFO("argv:");
	for(i = 0; i < argc; i++){
		a = argv[i];
		switch(a.a_type){
		case A_FLOAT:
			PINFO("A_FLOAT: %f", a.a_w.w_float);
			break;
		case A_LONG:
			PINFO("A_LONG: %d", a.a_w.w_long);
			break;
		case A_SYM:
			PINFO("A_SYM: %s", a.a_w.w_sym->s_name);
			break;
		default:
			CMMJL_ERROR(x, CMMJL_EBADTYPE, "Unknown type: %d", a.a_type);
		}
	}
}