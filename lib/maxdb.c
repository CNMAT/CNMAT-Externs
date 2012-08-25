#include "maxdb.h"
#include "ext_obex.h" 

// private
int maxdb_inc_refcount(t_maxdb *db);
int maxdb_dec_refcount(t_maxdb *db);
int maxdb_reset_refcount(t_maxdb *db);
void maxdb_remove_notification(t_maxdb *db, t_object *object);

// implementation

struct _maxdb_notification_obj{
	t_object *ob;
	t_maxdb_notification_cb cb;
	struct _maxdb_notification_obj *next;
};

t_symbol *maxdb_mangle_name(const char *prefix, t_symbol *name){
	char buf[128];
	sprintf(buf, "maxdb_%s_%s", prefix, name->s_name);
	return gensym(buf);
}

t_maxdb *maxdb_refer(t_maxdb *old_db, t_symbol *new_mangled_name){
	if(old_db){
		maxdb_dec_refcount(old_db);
		if(maxdb_get_refcount(old_db) <= 0){
			maxdb_destroy(old_db);
		}
	}
	
	if(new_mangled_name->s_thing){
		t_maxdb *db = (t_maxdb *)(new_mangled_name->s_thing);
		maxdb_inc_refcount(db);
		return db;
	}else{
		return NULL;
	}
}

void maxdb_unrefer(t_object *x, t_maxdb *db){
	if(!db){
		return;
	}

	maxdb_lock((t_maxdb *)db);
	maxdb_remove_notification(db, x);
	t_symbol *name = db->mangled_name;
	maxdb_dec_refcount(db);
	if(maxdb_get_refcount(db) <= 0){
		if(name){
			name->s_thing = NULL;
		}
		maxdb_unlock((t_maxdb *)db);
		maxdb_destroy((t_maxdb *)db);
	}else{
		maxdb_unlock((t_maxdb *)db);
	}
}

void maxdb_destroy(t_maxdb *db){
	t_maxdb_dtor dtor = db->dtor;
	t_symbol *mangled_name = db->mangled_name;
	t_maxdb_notification_obj *n = db->notify;
	while(n){
		//n->cb(n->ob, ps_maxdb_destroy, db);
		t_maxdb_notification_obj *next = n->next;
		free(n);
		n = next;
	}
	critical_free(db->lock);
	if(mangled_name){
		mangled_name->s_thing = NULL;
	}
	if(dtor){
		dtor(db->context, db);
	}
	free(db);
}

t_maxdb *maxdb_alloc(t_symbol *mangled_name, size_t sizeofdb, t_maxdb_dtor dtor, void *context){
	ps_maxdb_destroy = gensym("maxdb_destroy");
	ps_maxdb_remove = gensym("maxdb_remove");
	ps_maxdb_insert = gensym("maxdb_insert");
	t_maxdb *db = NULL;
	if(mangled_name){
		maxdb_refer(NULL, mangled_name);
	}

	if(!db){
		db = (t_maxdb *)malloc(sizeofdb);
		if(!db){
			return NULL;
		}
		db->refcount = 1;
		critical_new(&(db->lock));
		db->mangled_name = mangled_name;
		db->notify = NULL;
		db->dtor = dtor;
		db->context = context;
		if(mangled_name){
			mangled_name->s_thing = (void *)db;
		}
	}
	return db;
}

int maxdb_inc_refcount(t_maxdb *db){
	db->refcount++;
	return db->refcount;
}

int maxdb_dec_refcount(t_maxdb *db){
	db->refcount--;
	return db->refcount;
}

int maxdb_get_refcount(t_maxdb *db){
	return db->refcount;
}

int maxdb_reset_refcount(t_maxdb *db){
	db->refcount = 0;
	return db->refcount;
}

void maxdb_lock(t_maxdb *db){
	critical_enter(db->lock);
}

void maxdb_unlock(t_maxdb *db){
	critical_exit(db->lock);
}

void maxdb_set_mangled_name(t_maxdb *db, t_symbol *mangled_name){
	db->mangled_name = mangled_name;
}

t_symbol *maxdb_get_mangled_name(t_maxdb *db){
	return db->mangled_name;
}

void maxdb_add_notification(t_maxdb *db, t_object *object, t_maxdb_notification_cb cb){
	t_maxdb_notification_obj *n = (t_maxdb_notification_obj *)malloc(sizeof(t_maxdb_notification_obj));
	if(!n){
		return;
	}
	t_maxdb_notification_obj *nn = db->notify;
	while(nn){
		if(nn->ob == object){
			return;
		}
		nn = nn->next;
	}
	n->ob = object;
	n->cb = cb;
	n->next = db->notify;
	db->notify = n;
}

void maxdb_remove_notification(t_maxdb *db, t_object *object){
	t_maxdb_notification_obj *n = db->notify;
	t_maxdb_notification_obj *prev = NULL;
	while(n){
		if(object == n->ob){
			if(prev){
				prev->next = n->next;
			}else{
				db->notify = n->next;
			}
			free(n);
			break;
		}
		prev = n;
		n = n->next;
	}
}

void maxdb_notify(t_object *sender, t_maxdb *db, t_symbol *notification_type, void *data){
	t_maxdb_notification_obj *n = db->notify;
	while(n){
		if(sender != n->ob){
			n->cb(n->ob, notification_type, data, sender);
		}
		n = n->next;
	}
}
