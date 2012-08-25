/**************************************************
 hash table
 **************************************************/
#include "maq_hashtab.h"

struct _maq_hashtab_elem{
	void *key;
	void *obj;
	struct _maq_hashtab_elem *next;
};

struct _maq_hashtab{
	t_maq_hashtab_elem **slots;
	long nslots;
};

long primes[] = {
	541,
	1181,
	2371,
	4759,
	9521,
	19037,
	38083,
	76157,
	152287
};

void maq_hashtab_store_elem(t_maq_hashtab *ht, void *key, MAQ_HASHTYPE hash, t_maq_hashtab_elem *e);

MAQ_HASHTYPE maq_hash(void *key){
#ifdef __LP64__
	// http://www.concentric.net/~ttwang/tech/inthash.htm
	register MAQ_HASHTYPE k = (MAQ_HASHTYPE)key;
	k = (~k) + (k << 21); // k = (k << 21) - k - 1;
	k = k ^ (k >> 24);
	k = (k + (k << 3)) + (k << 8); // k * 265
	k = k ^ (k >> 14);
	k = (k + (k << 2)) + (k << 4); // k * 21
	k = k ^ (k >> 28);
	k = k + (k << 31);
	return k;
#else
	// Knuth
	register uint32 k;
	k = (uint32) key;
	return (k >> 3) * 2654435761UL;
#endif
}

void maq_hashtab_rehash(t_maq_hashtab *ht){
	int i;
	for(i = 0; i < sizeof(primes) / sizeof(long); i++){
		if(ht->nslots > primes[i]){
			long oldnslots = ht->nslots;
			t_maq_hashtab_elem **oldslots = ht->slots;	
			ht->slots = (t_maq_hashtab_elem **)calloc(oldnslots, sizeof(t_maq_hashtab_elem *));
			ht->nslots = primes[i];
			int j;
			for(j = 0; j < ht->nslots; j++){
				t_maq_hashtab_elem *e = oldslots[i];
				while(e){
					t_maq_hashtab_elem *next = e->next;
					maq_hashtab_store_elem(ht, e->key, maq_hash(e->key), e);
					e = next;
				}
			}

		}
	}
}

t_maq_hashtab *maq_hashtab_new(){
	t_maq_hashtab *ht = (t_maq_hashtab *)malloc(sizeof(t_maq_hashtab));
	if(!ht){
		return NULL;
	}
	ht->slots = (t_maq_hashtab_elem **)calloc(sizeof(t_maq_hashtab_elem *), MAQ_HASHTAB_DEFAULT_NUM_SLOTS);
	ht->nslots = MAQ_HASHTAB_DEFAULT_NUM_SLOTS;
	return ht;
}

void maq_hashtab_store(t_maq_hashtab *ht, void *key, void *obj){
	t_maq_hashtab_elem *e = (t_maq_hashtab_elem *)malloc(sizeof(t_maq_hashtab_elem));
	if(!e){
		return;
	}
	e->key = key;
	e->obj = obj;
	e->next = NULL;
	MAQ_HASHTYPE h = maq_hash(key);
	maq_hashtab_store_elem(ht, key, h, e);
}

void maq_hashtab_store_elem(t_maq_hashtab *ht, void *key, MAQ_HASHTYPE hash, t_maq_hashtab_elem *e){
	t_maq_hashtab_elem *ee = ht->slots[hash % ht->nslots];
	while(ee){
		if(ee->key == key){
			return;
		}
		ee = ee->next;
	}
	e->next = ht->slots[hash % ht->nslots];
	ht->slots[hash % ht->nslots] = e;
}

void *maq_hashtab_lookup(t_maq_hashtab *ht, void *key){
	MAQ_HASHTYPE h = maq_hash(key);
	t_maq_hashtab_elem *e = ht->slots[h % ht->nslots];
	while(e){
		if(e->key == key){
			return e->obj;
		}
		e = e->next;
	}
	return NULL;
}

void *maq_hashtab_remove(t_maq_hashtab *ht, void *key){
	MAQ_HASHTYPE h = maq_hash(key);
	t_maq_hashtab_elem *e = ht->slots[h % ht->nslots];
	t_maq_hashtab_elem *prev = NULL;
	while(e){
		if(e->key == key){
			void *obj = e->obj;
			if(prev){
				prev->next = e->next;
			}else{
				ht->slots[h % ht->nslots] = e->next;
			}
			free(e);
			return obj;
		}
		prev = e;
		e = e->next;
	}
	return NULL;
}

void maq_hashtab_clear(t_maq_hashtab *ht){
	int i;
	for(i = 0; i < ht->nslots; i++){
		t_maq_hashtab_elem *e = ht->slots[i];
		while(e){
			t_maq_hashtab_elem *next = e->next;
			free(e);
			e = next;
		}
	}
	memset(ht->slots, '\0', ht->nslots * sizeof(t_maq_hashtab_elem *));
}

void maq_hashtab_destroy(t_maq_hashtab *ht){
	maq_hashtab_clear(ht);
	free(ht->slots);
	free(ht);
}
