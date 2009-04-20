#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_hashtab.h"
#include "ext_linklist.h"

t_symbol *_sym_array, *_sym_hashtab, *_sym_linklist, *_sym_refcount;

void ar_initlib(void);
void ar_freeall(t_hashtab *ht, t_linklist *ll);
t_symbol *ar_make_iname(t_symbol *name, t_symbol *type);
t_symbol *ar_encode_key(t_atom *a);
t_symbol *ar_encode_key_from_float(float f);
t_symbol *ar_encode_key_from_int(long l);
t_symbol *ar_encode_key_from_symbol(t_symbol *s);
void ar_decode_key(t_symbol *s, t_atom *a);
long ar_inc_refcount(t_symbol *name);
long ar_dec_refcount(t_symbol *name);
t_symbol *ar_make_def_name();
