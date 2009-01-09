#include "cmmjl.h"
#include "cmmjl_profile.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_hashtab.h"

int cmmjl_entrance_count_inc(void *x, const char *func){
	t_object *val;
	t_object *count;
	long c;
	if(!_cmmjl_obj_tab){
		return -1;
	}
	hashtab_lookup(_cmmjl_obj_tab, x, &val);
	if(!val){
		return -1;
	}
	hashtab_lookup(((t_cmmjl_obj *)val)->entrance_count_tab, (void *)func, &count);
	c = (long)count;
	c += 1;
	hashtab_store_safe(((t_cmmjl_obj *)val)->entrance_count_tab, (void *)func, (t_object *)c);
	return c;
}

int cmmjl_entrance_count_dec(void *x, const char *func){
	t_object *val;
	t_object *count;
	long c;
	if(!_cmmjl_obj_tab){
		return -1;
	}
	hashtab_lookup(_cmmjl_obj_tab, x, &val);
	if(!val){
		return -1;
	}
	hashtab_lookup(((t_cmmjl_obj *)val)->entrance_count_tab, (void *)func, &count);
	c = (long)count;
	c -= 1;
	hashtab_store_safe(((t_cmmjl_obj *)val)->entrance_count_tab, (void *)func, (t_object *)c);
	return c;
}

int cmmjl_entrance_count_get(void *x, const char *func){
	t_object *val;
	t_object *count;
	if(!_cmmjl_obj_tab){
		return -1;
	}
	hashtab_lookup(_cmmjl_obj_tab, x, &val);
	if(!val){
		return -1;
	}
	hashtab_lookup(((t_cmmjl_obj *)val)->entrance_count_tab, (void *)func, &count);
	return (long)count;
}
