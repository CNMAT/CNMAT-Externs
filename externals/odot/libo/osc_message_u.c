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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osc.h"
#include "osc_message_u.h"
#include "osc_message_u.r"
#include "osc_atom_u.h"
#include "osc_atom_u.r"
#include "osc_message_iterator_u.h"

t_osc_msg_u *osc_message_u_alloc(){
	t_osc_msg_u *m = (t_osc_msg_u *)osc_mem_alloc(sizeof(t_osc_msg_u));
	if(!m){
		return NULL;
	}
	osc_message_u_initMsg(m);
	return m;
}

void osc_message_u_free(t_osc_msg_u *m){
	if(m->address){
		osc_mem_free(m->address);
	}
	t_osc_atom_u *a = m->arghead;
	while(a){
		t_osc_atom_u *next = a->next;
		osc_mem_free(a);
		a = next;
	}
}

void osc_message_u_initMsg(t_osc_msg_u *m){
	memset(m, '\0', sizeof(t_osc_msg_u));
}

void osc_message_u_copy(t_osc_msg_u **dest, t_osc_msg_u *src){
	if(!(*dest)){
		*dest = osc_message_u_alloc();
	}
	**dest = *src;
}

uint32_t osc_message_u_getSize(t_osc_msg_u *m){
	return m->size;
}

char *osc_message_u_getAddress(t_osc_msg_u *m){
	return m->address;
}

t_osc_err osc_message_u_setAddress(t_osc_msg_u *m, char *address){
	if(m->address){
		int len = strlen(m->address);
		if(len < strlen(address)){
			m->address = osc_mem_resize(m->address, len + 1);
		}
		strcpy(m->address, address);
	}else{
		char *buf = osc_mem_alloc(strlen(address) + 1);
		strcpy(buf, address);
		m->address = buf;
	}
}

int osc_message_u_getArgCount(t_osc_msg_u *m){
	return m->argc;
}

void osc_message_u_getArg(t_osc_msg_u *m, int n, t_osc_atom_u **atom){
	int nn = osc_message_u_getArgCount(m);
	if(nn <= n){
		return;
	}
	t_osc_atom_u *a = NULL;
	int i;
	if(nn / n < .5){
		a = m->arghead;
		for(i = 0; i < n; i++){
			a = a->next;
		}
	}else{
		a = m->argtail;
		for(i = m->argc; i >= n; i--){
			a = a->prev;
		}
	}
	*atom = a;
}

t_osc_err osc_message_u_appendAtom(t_osc_msg_u *m, t_osc_atom_u *a){
	m->argc++;
	if(!(m->arghead)){
		a->prev = a->next = NULL;
		m->arghead = m->argtail = a;
		return OSC_ERR_NONE;
	}
	m->argtail->next = a;
	a->prev = m->argtail;
	a->next = NULL;
	m->argtail = a;
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependAtom(t_osc_msg_u *m, t_osc_atom_u *a){
	m->argc++;
	if(!(m->arghead)){
		a->prev = a->next = NULL;
		m->arghead = m->argtail = a;
		return OSC_ERR_NONE;
	}
	a->next = m->arghead;
	m->arghead->prev = a;
	m->arghead = a;
	a->prev = NULL;
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertAtom(t_osc_msg_u *m, t_osc_atom_u *a, int pos){
	if(m->arghead == NULL){
		m->arghead = m->argtail = a;
		a->next = a->prev = NULL;
		goto out;
	}
	if(pos >= m->argc){
		if(m->argtail){
			m->argtail->next = a;
		}
		a->prev = m->argtail;
		m->argtail = a;
		goto out;
	}
	if(pos == 0){
		a->next = m->arghead;
		if(m->arghead){
			m->arghead->prev = a;
		}
		a->prev = NULL;
		m->arghead = a;
		goto out;
	}
	t_osc_atom_u *aa = NULL;
	int i;
	if((float)pos / m->argc < .5){
		aa = m->arghead;
		for(i = 0; i < pos; i++){
			aa = aa->next;
		}
	}else{
		aa = m->argtail;
		for(i = m->argc; i > pos + 1; i--){
			aa = aa->prev;
		}
	}
	if(aa->prev){
		aa->prev->next = a;
	}
	a->prev = aa->prev;
	a->next = aa;
	aa->prev = a;
 out:
	m->argc++;
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendInt32(t_osc_msg_u *m, int32_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt32(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendInt64(t_osc_msg_u *m, int64_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt64(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendUInt32(t_osc_msg_u *m, uint32_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt32(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendUInt64(t_osc_msg_u *m, uint64_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt64(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendFloat(t_osc_msg_u *m, float v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFloat(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendDouble(t_osc_msg_u *m, double v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setDouble(a, v);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendString(t_osc_msg_u *m, char *v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	int len = strlen(v);
	char *buf = osc_mem_alloc(len + 1);
	strlcpy(buf, v, len + 1);
	osc_atom_u_setString(a, buf);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendBool(t_osc_msg_u *m, int v){
	if(v == 0){
		osc_message_u_appendFalse(m);
	}else{
		osc_message_u_appendTrue(m);
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendTrue(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setTrue(a);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendFalse(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFalse(a);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_appendNull(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setNull(a);
	osc_message_u_appendAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependInt32(t_osc_msg_u *m, int32_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt32(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependInt64(t_osc_msg_u *m, int64_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt64(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependUInt32(t_osc_msg_u *m, uint32_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt32(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependUInt64(t_osc_msg_u *m, uint64_t v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt64(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependFloat(t_osc_msg_u *m, float v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFloat(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependDouble(t_osc_msg_u *m, double v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setDouble(a, v);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependString(t_osc_msg_u *m, char *v){
	t_osc_atom_u *a = osc_atom_u_alloc();
	int len = strlen(v);
	char *buf = osc_mem_alloc(len + 1);
	strlcpy(buf, v, len + 1);
	osc_atom_u_setString(a, buf);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependBool(t_osc_msg_u *m, int v){
	if(v == 0){
		osc_message_u_prependFalse(m);
	}else{
		osc_message_u_prependTrue(m);
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependTrue(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setTrue(a);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependFalse(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFalse(a);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_prependNull(t_osc_msg_u *m){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setNull(a);
	osc_message_u_prependAtom(m, a);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertInt32(t_osc_msg_u *m, int32_t v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt32(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertInt64(t_osc_msg_u *m, int64_t v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setInt64(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertUInt32(t_osc_msg_u *m, uint32_t v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt32(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertUInt64(t_osc_msg_u *m, uint64_t v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setUInt64(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertFloat(t_osc_msg_u *m, float v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFloat(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertDouble(t_osc_msg_u *m, double v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setDouble(a, v);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertString(t_osc_msg_u *m, char *v, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	int len = strlen(v);
	char *buf = osc_mem_alloc(len + 1);
	strlcpy(buf, v, len + 1);
	osc_atom_u_setString(a, buf);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertBool(t_osc_msg_u *m, int v, int pos){
	if(v == 0){
		osc_message_u_insertFalse(m, pos);
	}else{
		osc_message_u_insertTrue(m, pos);
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertTrue(t_osc_msg_u *m, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setTrue(a);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertFalse(t_osc_msg_u *m, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setFalse(a);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}

t_osc_err osc_message_u_insertNull(t_osc_msg_u *m, int pos){
	t_osc_atom_u *a = osc_atom_u_alloc();
	osc_atom_u_setNull(a);
	osc_message_u_insertAtom(m, a, pos);
	return OSC_ERR_NONE;
}


extern t_osc_err osc_atom_u_doFormat(t_osc_atom_u *a, long *buflen, long *bufpos, char **buf);
t_osc_err osc_message_u_doFormat(t_osc_msg_u *m, long *buflen, long *bufpos, char **buf){
	if((*buflen - *bufpos) < 256){
		*buf = osc_mem_resize(*buf, *buflen + 1024);
		if(!(*buf)){
			return OSC_ERR_OUTOFMEM;
		}
		*buflen += 1024;
	}
	*bufpos += sprintf(*buf + *bufpos, "%s ", osc_message_u_getAddress(m));
	t_osc_msg_it_u *it = osc_msg_it_u_get(m);
	while(osc_msg_it_u_hasNext(it)){
		t_osc_atom_u *a = osc_msg_it_u_next(it);
		osc_atom_u_doFormat(a, buflen, bufpos, buf);
	}
}

t_osc_err osc_message_u_format(t_osc_msg_u *m, long *buflen, char **buf){
	long mybuflen = 0, mybufpos = 0;
	t_osc_err e = osc_message_u_doFormat(m, &mybuflen, &mybufpos, buf);
	*buflen = mybufpos;
	return e;
}
