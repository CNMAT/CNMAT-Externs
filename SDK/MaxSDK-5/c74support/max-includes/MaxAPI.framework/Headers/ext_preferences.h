#ifndef _EXT_PREFERENCES_H_
#define _EXT_PREFERENCES_H_

BEGIN_USING_C_LINKAGE

t_max_err preferences_getatomforkey(t_symbol *key, t_atom *value);
t_symbol *preferences_getsym(char *name);
void preferences_setsym(char *name, t_symbol *value);
long preferences_getlong(char *name);
void preferences_setlong(char *name, long value);
long preferences_getchar(char *name);
void preferences_setchar(char *name, long value);
t_max_err preferences_getatoms(char *name, long *argc, t_atom **argv);
t_max_err preferences_setatoms(char *name, long argc, t_atom *argv);
void *preferences_define(char *name, char *type, char *label, char *style, char *category, long attrflags, method get, method set, long flags);
void *preferences_class_define(t_class *c, char *name, char *type, char *label, char *style, char *category, long attrflags, method get, method set, long flags);
void *preferences_defineoption(char *name, char *label, char *category, char *ptr, method notify, long flags);
void *preferences_class_defineoption(t_class *c, char *name, char *label, char *category, char *ptr, method notify, long flags);
t_max_err preferences_writedictionary(t_dictionary *d, char *filename);
t_max_err preferences_readdictionary(char *filename, t_dictionary **d);

END_USING_C_LINKAGE

enum {
	PREFERENCES_FLAGS_INVISIBLE = 1,
	PREFERENCES_FLAGS_DONTSAVE = 2
};


#endif // _EXT_PREFERENCES_H_
