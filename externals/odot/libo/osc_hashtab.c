/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2011, The Regents of
the University of California (Regents). 
Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

/** 	\file osc_hashtab.c
	\author John MacCallum

*/
#include "osc_hashtab.h"
#include <stdint.h>
#include "osc_mem.h"
#include <stdlib.h>
#include <string.h>

struct _osc_hashtab_elem{
	void *key;
	void *obj;
	struct _osc_hashtab_elem *next;
};

struct _osc_hashtab{
	t_osc_hashtab_elem **slots;
	long nslots;
};

long primes[] = {
	17,
	37,
	67,
	137,
	271,
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

void osc_hashtab_store_elem(t_osc_hashtab *ht, char *key, OSC_HASHTYPE hash, t_osc_hashtab_elem *e);

uint32_t osc_hash(int len, char *key){
#define __ELFHASH__
#if defined __ELFHASH__
	OSC_HASHTYPE hash = 0;
	OSC_HASHTYPE x    = 0;
	OSC_HASHTYPE i    = 0;

	for(i = 0; i < len; key++, i++){
		hash = (hash << 4) + (*key);
		if((x = hash & 0xF0000000L) != 0){
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}

	return hash;
#elif defined __DJBHASH__
	OSC_HASHTYPE hash = 5381;
	OSC_HASHTYPE i    = 0;

	for(i = 0; i < len; key++, i++){
		hash = ((hash << 5) + hash) + (*key);
	}

	return hash;
#endif
}

uint32_t osc_hashtab_getPrime(uint32_t n){
	int i;
	for(i = 0; i < sizeof(primes) / sizeof(long); i++){
		if(n < primes[i]){
			return primes[i];
		}
	}
	return primes[i];
}

void osc_hashtab_rehash(t_osc_hashtab *ht){
	int i;
	for(i = 0; i < sizeof(primes) / sizeof(long); i++){
		if(ht->nslots > primes[i]){
			long oldnslots = ht->nslots;
			t_osc_hashtab_elem **oldslots = ht->slots;	
			ht->slots = (t_osc_hashtab_elem **)osc_mem_alloc(primes[i] * sizeof(t_osc_hashtab_elem *));
			memset(ht->slots, '\0', primes[i] * sizeof(t_osc_hashtab_elem *));
			ht->nslots = primes[i];
			int j;
			for(j = 0; j < oldnslots; j++){
				t_osc_hashtab_elem *e = oldslots[j];
				while(e){
					t_osc_hashtab_elem *next = e->next;
					osc_hashtab_store_elem(ht, e->key, osc_hash(strlen(e->key), e->key), e);
					e = next;
				}
			}
			osc_mem_free(oldslots);
		}
	}
}

t_osc_hashtab *osc_hashtab_new(int nslots){
	if(nslots <= 0){
		nslots = OSC_HASHTAB_DEFAULT_NUM_SLOTS;
	}
	nslots = osc_hashtab_getPrime(nslots);
	t_osc_hashtab *ht = (t_osc_hashtab *)osc_mem_alloc(sizeof(t_osc_hashtab));
	if(!ht){
		return NULL;
	}
	ht->slots = (t_osc_hashtab_elem **)osc_mem_alloc(sizeof(t_osc_hashtab_elem *) * nslots);
	memset(ht->slots, '\0', sizeof(t_osc_hashtab_elem *) * nslots);
	ht->nslots = nslots;
	return ht;
}

void osc_hashtab_store(t_osc_hashtab *ht, int keylen, char *key, void *obj){
	t_osc_hashtab_elem *e = (t_osc_hashtab_elem *)osc_mem_alloc(sizeof(t_osc_hashtab_elem));
	if(!e){
		return;
	}
	e->key = key;
	e->obj = obj;
	e->next = NULL;
	OSC_HASHTYPE h = osc_hash(keylen, key);
	osc_hashtab_store_elem(ht, key, h, e);
}

void osc_hashtab_store_elem(t_osc_hashtab *ht, char *key, OSC_HASHTYPE hash, t_osc_hashtab_elem *e){
	t_osc_hashtab_elem *ee = ht->slots[hash % ht->nslots];
	while(ee){
		if(ee->key == key){
			return;
		}
		ee = ee->next;
	}
	e->next = ht->slots[hash % ht->nslots];
	ht->slots[hash % ht->nslots] = e;
}

void *osc_hashtab_lookup(t_osc_hashtab *ht, int keylen, char *key){
	OSC_HASHTYPE h = osc_hash(keylen, key);
	t_osc_hashtab_elem *e = ht->slots[h % ht->nslots];
	while(e){
		if(e->key == key){
			return e->obj;
		}
		e = e->next;
	}
	return NULL;
}

void *osc_hashtab_remove(t_osc_hashtab *ht, int keylen, char *key){
	OSC_HASHTYPE h = osc_hash(keylen, key);
	t_osc_hashtab_elem *e = ht->slots[h % ht->nslots];
	t_osc_hashtab_elem *prev = NULL;
	while(e){
		if(e->key == key){
			void *obj = e->obj;
			if(prev){
				prev->next = e->next;
			}else{
				ht->slots[h % ht->nslots] = e->next;
			}
			osc_mem_free(e);
			return obj;
		}
		prev = e;
		e = e->next;
	}
	return NULL;
}

void osc_hashtab_clear(t_osc_hashtab *ht){
	int i;
	for(i = 0; i < ht->nslots; i++){
		t_osc_hashtab_elem *e = ht->slots[i];
		while(e){
			t_osc_hashtab_elem *next = e->next;
			osc_mem_free(e);
			e = next;
		}
	}
	memset(ht->slots, '\0', ht->nslots * sizeof(t_osc_hashtab_elem *));
}

void osc_hashtab_destroy(t_osc_hashtab *ht){
	osc_hashtab_clear(ht);
	osc_mem_free(ht->slots);
	osc_mem_free(ht);
}
