#ifndef __MAQ_HASHTAB_H__
#define __MAQ_HASHTAB_H__

#define MAQ_HASHTYPE32 uint32_t
#define MAQ_HASHTYPE64 uint64_t

#ifdef __LP64__
#define MAQ_HASHTYPE MAQ_HASHTYPE64
#else
#define MAQ_HASHTYPE MAQ_HASHTYPE32
#endif

#define MAQ_HASHTAB_DEFAULT_NUM_SLOTS 541 // big to avoid a rehash

typedef struct _maq_hashtab_elem t_maq_hashtab_elem;
typedef struct _maq_hashtab t_maq_hashtab;

t_maq_hashtab *maq_hashtab_new();
void maq_hashtab_store(t_maq_hashtab *ht, void *key, void *obj);
void *maq_hashtab_lookup(t_maq_hashtab *ht, void *key);
void *maq_hashtab_remove(t_maq_hashtab *ht, void *key);
void maq_hashtab_clear(t_maq_hashtab *ht);
void maq_hashtab_destroy(t_maq_hashtab *ht);

#endif // __MAQ_HASHTAB_H__
