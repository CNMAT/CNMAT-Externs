#ifndef __MAXDB_H__
#define __MAXDB_H__

#include "ext.h"
#include "ext_critical.h"

typedef void (*t_maxdb_notification_cb)(t_object *context, t_symbol *notification_type, void *data, t_object *sender);
typedef struct _maxdb_notification_obj t_maxdb_notification_obj;

typedef struct _maxdb{
	int refcount;
	t_critical lock;
	t_symbol *mangled_name;
	t_maxdb_notification_obj *notify;
	void (*dtor)(void *context, struct _maxdb *);
	void *context;
} t_maxdb;

t_symbol *ps_maxdb_destroy, *ps_maxdb_remove, *ps_maxdb_insert;

typedef void (*t_maxdb_dtor)(void *, t_maxdb *);

t_symbol *maxdb_mangle_name(const char *prefix, t_symbol *name);
t_maxdb *maxdb_refer(t_maxdb *old_db, t_symbol *new_mangled_name);
void maxdb_unrefer(t_object *x, t_maxdb *db);
void maxdb_add_notification(t_maxdb *db, t_object *object, t_maxdb_notification_cb cb);
void maxdb_remove_notification(t_maxdb *db, t_object *object);
void maxdb_notify(t_object *sender, t_maxdb *db, t_symbol *notification_type, void *data);
void maxdb_destroy(t_maxdb *db);
// looks up database for mangled_name and returns it if one exists, otherwise
// allocates a new db and returns it
t_maxdb *maxdb_alloc(t_symbol *mangled_name, size_t sizeofdb, t_maxdb_dtor dtor, void *context);
void maxdb_lock(t_maxdb *db);
void maxdb_unlock(t_maxdb *db);
void maxdb_set_mangled_name(t_maxdb *db, t_symbol *mangled_name);
t_symbol *maxdb_get_mangled_name(t_maxdb *db);
int maxdb_get_refcount(t_maxdb *db);

#endif // __MAXDB_H__
