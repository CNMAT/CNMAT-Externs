/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009-ll, The Regents of
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

#include "osc_bundle.h"
#include <stdio.h>
#include <string.h>
#include "osc.h"
#include "osc_message.h"

t_osc_err osc_bundle_makeBundle(int len, char *buf, t_osc_bundle *bundle){
	return osc_bundle_getMessages(len, buf, &(bundle->num_messages), &(bundle->messages));
}

t_osc_bundle *osc_bundle_alloc(void){
	t_osc_bundle *b = (t_osc_bundle *)osc_mem_alloc(sizeof(t_osc_bundle));
	b->messages = NULL;
	b->num_messages = 0;
}

void osc_bundle_freeBundle(t_osc_bundle *bundle){
	if(bundle->messages){
		t_osc_msg *m = bundle->messages;
		while(m){
			t_osc_msg *next = m->next;
			osc_message_free(m);
			m = next;
		}
	}
}

t_osc_err osc_bundle_getMsgCount(int len, char *buf, int *count){
	t_osc_err ret;
	if(ret = osc_error_bundleSanityCheck(len, buf)){
		return ret;
	}
	char *ptr = buf + OSC_HEADER_SIZE;
	while((ptr - buf) < len){
		int size = ntoh32(*((uint32_t *)ptr));
		*count += 1;
		ptr += size + 4;
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_getMessages(int len, char *buf, int *num_messages, t_osc_msg **messages){
	t_osc_err ret;
	if(ret = osc_error_bundleSanityCheck(len, buf)){
		return ret;
	}
	char *ptr = buf;
	ptr += OSC_HEADER_SIZE;
	t_osc_msg *last = NULL;
	*num_messages = 0;
	while((ptr - buf) < len){
		int size = ntoh32(*((uint32_t *)ptr));

		t_osc_msg *m = osc_message_alloc();
		t_osc_err e = osc_message_parseMessage(0, ptr, m);
		if(last == NULL){
			*messages = m;
			last = m;
		}else{
			last->next = m;
			m->prev = last;
			last = m;
		}
		*num_messages += 1;
		ptr += size + 4;
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_getMessagesWithCallback(int len, char *buf, void (*f)(t_osc_msg, void *), void *context){
	int ret;
	if(ret = osc_error_bundleSanityCheck(len, buf)){
		return ret;
	}
	char *ptr = buf;
	int i;
	ptr += OSC_HEADER_SIZE;
	while((ptr - buf) < len){
		uint32_t size = osc_message_getSize(ptr);
		t_osc_msg m;
		osc_message_parseMessage(size, ptr + 4, &m);
		f(m, context);
		ptr += size + 4;
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_setBundleID(char *buf){
	if(!buf){
		return OSC_ERR_NOBUNDLE;
	}
	strncpy(buf, OSC_IDENTIFIER, OSC_IDENTIFIER_SIZE);
}

int osc_bundle_strcmpID(char *buf){
	return strncmp(buf, OSC_ID, OSC_ID_SIZE);
}

#ifdef OSC_2_0
t_osc_timetag osc_bundle_getTimetag(int len, char *buf){
}
#else
t_osc_timetag osc_bundle_getTimetag(int len, char *buf){
}
#endif

int osc_bundle_bundleNakedMessage(long n, char *ptr, char **out){
	if(!osc_bundle_strcmpID(ptr)){
		return n;
	}
	if(!(*out)){
		*out = (char *)osc_mem_alloc(n + 20);
	}
	char buf[n + OSC_HEADER_SIZE + 4];
	osc_bundle_setBundleID(buf);
#ifndef OSC_2_0
	*((uint64_t *)(buf + OSC_ID_SIZE)) = 0x000000001; // now
#endif
	*((uint32_t *)(buf + OSC_HEADER_SIZE)) = hton32(n);
	memcpy(buf + OSC_HEADER_SIZE + 4, ptr, n);
	memcpy(*out, buf, n + OSC_HEADER_SIZE + 4);
	return n + OSC_HEADER_SIZE + 4;
}

int osc_bundle_flatten(long n, char *ptr, char **out){
	// out could be the same as ptr for an inplace operation, so do everything locally and then copy
	if(osc_bundle_strcmpID(ptr)){
		char *cpy;
		osc_bundle_bundleNakedMessage(n, ptr, &cpy);
		ptr = cpy;
	}
	if(!(*out)){
		*out = (char *)osc_mem_alloc(n);
	}
	char localptr[n];
	int i = 0, j = 0;
	memcpy(localptr, ptr, OSC_HEADER_SIZE);
	i = j = OSC_HEADER_SIZE;
	while(i < n){
		if(!osc_bundle_strcmpID(ptr + i)){
			i += OSC_HEADER_SIZE;
			j -= 4;
		}else{
			*(localptr + (j++)) = *(ptr + i++);
		}
	}
	memset(localptr + j, '\0', n - j);
	memset(*out, '\0', n);
	memcpy(*out, localptr, j);
	return j;
}

t_osc_err osc_bundle_printBundle(int len, char *buf, int (*p)(const char *, ...)){
	t_osc_err ret;
	if(ret = osc_error_bundleSanityCheck(len, buf)){
		return ret;
	}
	char timetag_string[64];
	timetag_string[0] = '\0';
	//ret = osc_timetag_format(osc_bundle_getTimetag(buf), timetag_string);
	p("[ %s timetag: %s\n", OSC_ID, timetag_string);
	//p("[ %s\n", OSC_ID);
	osc_bundle_getMessagesWithCallback(len, buf, osc_bundle_printBundleCbk, (void *)p);
	p("]\n");
	return OSC_ERR_NONE;
}

void osc_bundle_printBundleCbk(t_osc_msg msg, void *context){
	osc_message_printMsg(&msg, context);
}

t_osc_err osc_bundle_lookupAddressSerialized(int len, char *buf, char *address, t_osc_msg **m, int fullmatch){
	char *ptr = buf + OSC_HEADER_SIZE;
	t_osc_msg *last = NULL;
	while(ptr - buf < len){
		int msgsize = osc_message_getSize(ptr);
		ptr += 4;
		int po, ao;
		int r = osc_match(address, ptr, &po, &ao);
		if(fullmatch){
			if(r != (OSC_MATCH_ADDRESS_COMPLETE | OSC_MATCH_PATTERN_COMPLETE)){
				goto cont;	
			}
		}else{
			if(r == 0 || (((r & OSC_MATCH_PATTERN_COMPLETE) == 0) && address[po] != '/')){
				goto cont;
			}
		}
		t_osc_msg *mm = osc_message_alloc();
		t_osc_err e = osc_message_parseMessage(msgsize, ptr, mm);
		if(last == NULL){
			*m = mm;
			last = mm;
		}else{
			last->next = mm;
			mm->prev = last;
			last = mm;
		}
	cont:
		ptr += msgsize;
	}
}

t_osc_err osc_bundle_lookupAddress(t_osc_bundle *bundle, char *address, t_osc_msg **m, int fullmatch){
	t_osc_msg *mm = bundle->messages;
	t_osc_msg *last = NULL;
	while(mm){
		int po, ao;
		int r = osc_match(address, mm->address, &po, &ao);
		if(fullmatch){
			if(r != (OSC_MATCH_ADDRESS_COMPLETE | OSC_MATCH_PATTERN_COMPLETE)){
				goto cont;	
			}
		}else{
			if(r == 0 || (((r & OSC_MATCH_PATTERN_COMPLETE) == 0) && address[po] != '/')){
				goto cont;
			}
		}
		t_osc_msg *mmm = osc_message_alloc();
		t_osc_err e = osc_message_deepCopy(&mmm, mm);
		if(e){
			return e;
		}
		if(last == NULL){
			*m = mmm;
			last = mmm;
		}else{
			last->next = mmm;
			mm->prev = last;
			last = mmm;
		}
	cont:
		mm = mm->next;
	}
}

t_osc_err osc_bundle_addMessage(t_osc_bundle *bundle, t_osc_msg *message){
	t_osc_msg *m = bundle->messages;
	if(!m){
		bundle->messages = message;
		bundle->num_messages = 1;
		return OSC_ERR_NONE;
	}
	while(m->next){
		m = m->next;
	}
	m->next = message;
	message->prev = m;
	message->next = NULL;
	bundle->num_messages++;
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_getSerializedLen(t_osc_bundle *bundle, long *len){
	t_osc_msg *m = bundle->messages;
	*len = OSC_HEADER_SIZE;
	while(m){
		*len += 4 + m->size;
		m = m->next;
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_serialize(t_osc_bundle *bundle, long *len, char **buffer){
	t_osc_err ret;
	if(ret = osc_bundle_getSerializedLen(bundle, len)){
		return ret;
	}
	*buffer = (char *)osc_mem_alloc(*len);
	osc_bundle_serializeWithBuffer(bundle, *buffer);
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_serializeWithBuffer(t_osc_bundle *bundle, char *buffer){
	t_osc_err ret;
	char *ptr = buffer;
	osc_bundle_setBundleID(ptr);
	ptr += OSC_HEADER_SIZE;
	t_osc_msg *m = bundle->messages;
	while(m){
		ptr += osc_message_serialize(m, ptr);
		m = m->next;
	}
	return OSC_ERR_NONE;
}
