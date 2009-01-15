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
#include "cmmjl_obj.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h"
#include "ext_linklist.h"

t_cmmjl_error cmmjl_obj_init(void *x, 
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

	o->instance = instance;

	if(object_attr_get(x, ps_OSCaddress)){
		// We'll just blindly set this in the default manner since 
		// if the name has been saved with the patcher, Max will 
		// override this name
		char buf1[256];
		char buf2[256];
		cmmjl_osc_makeDefaultAddress(x, instance, buf1);
		o->osc_address = gensym(buf1);
		//cmmjl_osc_setDefaultAddress(x);

		OBJ_ATTR_SAVE(x, "OSCaddress", 0L);

		o->osc_address_methods = (t_linklist *)linklist_new();
		// make sure the items will never be freed
		linklist_flags(o->osc_address_methods, OBJ_FLAG_DATA);
		int i = 0;
		// we're relying on the object being the first element of the struct
		while(((t_object *)x)->o_messlist[i].m_sym){
			if(((t_object *)x)->o_messlist[i].m_sym->s_name[0] == '/'){
				sprintf(buf2, "%s%s", buf1, ((t_object *)x)->o_messlist[i].m_sym->s_name);
				linklist_append(o->osc_address_methods, buf2);
			}
			i++;
		}
	}
	return CMMJL_SUCCESS;
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

void cmmjl_obj_info_outlet_set(void *x, void *outlet){
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

long cmmjl_obj_getInstance(void *x){
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!o){
		return -1;
	}
	return o->instance;
}
