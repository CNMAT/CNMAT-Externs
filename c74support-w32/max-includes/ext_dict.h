// ext_dict.h -- key-value pair support used by plug-in parameter objects

#ifndef _DICT_H_
#define _DICT_H_

#ifdef __cplusplus
extern "C" {
#endif

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

typedef void *t_dict;		// a linklist of dictentries

void *dict_new(t_atom *a);
void dict_appendatom2(void *dict, t_symbol *key, t_atom *value);
void dict_appendatom(void *dict, char *key, t_atom *value);
void dict_appendlong(void *dict, char *key, long val);
void dict_appendfloat(void *dict, char *key, double val);
void dict_appendsym(void *dict, char *key, t_symbol *sym);
short dict_getlong(void *dict, char *match, long *value);
short dict_getfloat(void *dict, char *match, double *value);
short dict_getsym(void *dict, char *match, t_symbol **value);
short dict_getatom(void *dict, char *match, t_atom *data);
short dict_getatom2(void *dict, t_symbol *match, t_atom *data);
short dict_entryisarray(void *dict, t_symbol *match);
short dict_entryisdict(void *dict, t_symbol *match);
long dict_getentrycount(void *dict);

// wrapper for an atombuf

void dictarray_new(long count, t_atom *values, t_atom *out);
void dictarray_setsym(t_atom *da, long index, t_symbol *val);
void dictarray_setlong(t_atom *da, long index, long val);
void dictarray_setatom(t_atom *da, long index, t_atom *val);
short dictarray_getsym(t_atom *da, long index, t_symbol **val);
short dictarray_getlong(t_atom *da, long index, long *val);
short dictarray_gettype(t_atom *da, long index);
short dictarray_getatom(t_atom *da, long index, t_atom *val);
short dictarray_getlength(t_atom *da, long *len);


#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif // _DICT_H_
