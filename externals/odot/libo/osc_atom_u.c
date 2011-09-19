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
#include <stdint.h>
#include <inttypes.h>
#include "osc.h"
#include "osc_mem.h"
#include "osc_byteorder.h"
#include "osc_bundle_u.h"
#include "osc_atom_u.h"
#include "osc_atom_u.r"

t_osc_atom_u *osc_atom_u_alloc(void){
	t_osc_atom_u *a = osc_mem_alloc(sizeof(t_osc_atom_u));
	memset(a, '\0', sizeof(t_osc_atom_u));
	return a;
}

void osc_atom_u_free(t_osc_atom_u *a){
	if(!a){
		return;
	}
	if(a->typetag == 's'){
		if(a->w.s){
			osc_mem_free(a->w.s);
		}
	}else if(a->typetag == '#'){
		osc_bundle_u_free(a->w.bndl);
	}
	osc_mem_free(a);
}

void osc_atom_u_copy(t_osc_atom_u **dest, t_osc_atom_u *src){
	if(!src){
		return;
	}
	t_osc_atom_u *aa = *dest;
	if(!aa){
		aa = osc_mem_alloc(sizeof(t_osc_atom_u));
	}
	aa->next = aa->prev = NULL;
	aa->typetag = src->typetag;
	aa->alloc = src->alloc;
	switch(aa->typetag){
	case 's':
		{
			aa->w.s = osc_mem_alloc(strlen(src->w.s) + 1);
			strcpy(aa->w.s, src->w.s);
		}
		break;
	default:
		aa->w = src->w;
	}
	*dest = aa;
}

void osc_atom_u_setShouldFreePtr(t_osc_atom_u *a, int bool){
	if(a){
		a->alloc = bool;
	}
}

void osc_atom_u_clear(t_osc_atom_u *a){
	if(!a){
		return;
	}
	if(a->alloc && a->typetag == 's' && a->w.s){
		osc_mem_free(a->w.s);
	}
	a->alloc = 0;
}

char osc_atom_u_getTypetag(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	return a->typetag;
}

float osc_atom_u_getFloat(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'f': // 32-bit IEEE 754 float
		return a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (float)a->w.d;
	case 'i': // signed 32-bit int
		return (float)a->w.i;
	case 'h': // signed 64-bit int
		return (float)a->w.h;
	case 'I': // unsigned 32-bit int
		return (float)a->w.I;
	case 'H': // unsigned 64-bit int
		return (float)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return strtof(a->w.s, &endp);
		}
	case 'T': // true
		return 1.f;
	case 'F': // false
		return 0.f;
	case 'N': // NULL
		return 0.f;
	}
	return 0.f;
}

double osc_atom_u_getDouble(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'd': // 64-bit IEEE 754 double
		return a->w.d;
	case 'f': // 32-bit IEEE 754 float
		return (double)a->w.f;
	case 'i': // signed 32-bit int
		return (double)a->w.i;
	case 'h': // signed 64-bit int
		return (double)a->w.h;
	case 'I': // unsigned 32-bit int
		return (double)a->w.I;
	case 'H': // unsigned 64-bit int
		return (double)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return strtod(a->w.s, &endp);
		}
	case 'T': // true
		return 1.f;
	case 'F': // false
		return 0.f;
	case 'N': // NULL
		return 0.f;
	}
	return 0.f;
}

int32_t osc_atom_u_getInt32(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'f': // 32-bit IEEE 754 float
		return (int32_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (int32_t)a->w.d;
	case 'h': // signed 64-bit int
		return (int32_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (int32_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int32_t)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return (int32_t)strtol(a->w.s, &endp, 0);
		}
	case 'T': // true
		return 1;
	case 'F': // false
		return 0;
	case 'N': // NULL
		return 0;
	}
	return 0;
}

int64_t osc_atom_u_getInt64(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'h': // signed 64-bit int
		return (int64_t)a->w.h;
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'd': // 64-bit IEEE 754 double
		return (int64_t)a->w.d;
	case 'f': // 32-bit IEEE 754 float
		return (int64_t)a->w.f;
	case 'I': // unsigned 32-bit int
		return (int64_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int64_t)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return (int64_t)strtoll(a->w.s, &endp, 0);
		}
	case 'T': // true
		return 1;
	case 'F': // false
		return 0;
	case 'N': // NULL
		return 0;
	}
	return 0;
}

uint32_t osc_atom_u_getUInt32(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'I': // unsigned 32-bit int
		return (uint32_t)a->w.I;
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'f': // 32-bit IEEE 754 float
		return (uint32_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (uint32_t)a->w.d;
	case 'h': // signed 64-bit int
		return (uint32_t)a->w.h;
	case 'H': // unsigned 64-bit int
		return (uint32_t)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return (uint32_t)strtoul(a->w.s, &endp, 0);
		}
	case 'T': // true
		return 1;
	case 'F': // false
		return 0;
	case 'N': // NULL
		return 0;
	}
	return 0;
}

uint64_t osc_atom_u_getUInt64(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'H': // unsigned 64-bit int
		return (uint64_t)a->w.H;
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (uint64_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (uint64_t)a->w.I;
	case 'f': // 32-bit IEEE 754 float
		return (uint64_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (uint64_t)a->w.d;
	case 's': // string
		{
			char *endp = NULL;
			return (uint64_t)strtoull(a->w.s, &endp, 0);
		}
	case 'T': // true
		return 1;
	case 'F': // false
		return 0;
	case 'N': // NULL
		return 0;
	}
	return 0;
}

int osc_atom_u_getInt(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'f': // 32-bit IEEE 754 float
		return (int)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (int)a->w.d;
	case 'h': // signed 64-bit int
		return (int)a->w.h;
	case 'I': // unsigned 32-bit int
		return (int)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int)a->w.H;
	case 's': // string
		{
			char *endp = NULL;
			return (int)strtol(a->w.s, &endp, 0);
		}
	case 'T': // true
		return 1;
	case 'F': // false
		return 0;
	case 'N': // NULL
		return 0;
	}
	return 0;
}

char *osc_atom_u_getStringPtr(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	if(a->typetag == 's'){
		return a->w.s;
	}else{
		return NULL;
	}
}

int osc_atom_u_getString(t_osc_atom_u *a, char **out){
	if(!a){
		return 0;
	}
	switch(a->typetag){
	case 's': // string
		{
			int n = strlen(a->w.s);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			strcpy(*out, a->w.s);
			return n;
		}
	case 'i': // signed 32-bit int
		{
			int n = snprintf(NULL, 0, "%"PRId32, a->w.i);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%"PRId32, a->w.i);
			return n;
		}
	case 'f': // 32-bit IEEE 754 float
		{
			int n = snprintf(NULL, 0, "%f", a->w.f);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%f", a->w.f);
			return n;
		}
	case 'd': // 64-bit IEEE 754 double
		{
			int n = snprintf(NULL, 0, "%f", a->w.f);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%f", a->w.f);
			return n;
		}
	case 'h': // signed 64-bit int
		{
			int n = snprintf(NULL, 0, "%"PRId64, a->w.h);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%"PRId64, a->w.h);
			return n;
		}
	case 'I': // unsigned 32-bit int
		{
			int n = snprintf(NULL, 0, "%"PRIu32, a->w.I);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%"PRIu32, a->w.I);
			return n;
		}
	case 'H': // unsigned 64-bit int
		{
			int n = snprintf(NULL, 0, "%"PRIu64, a->w.H);
			if(!(*out)){
				*out = osc_mem_alloc(n + 1);
			}
			sprintf(*out, "%"PRIu64, a->w.H);
			return n;
		}
	case 'T': // true
		{
			if(!(*out)){
				*out = osc_mem_alloc(5);
			}
			strncpy(*out, "true\0", 5);
			return 4;
		}
	case 'F': // false
		{
			if(!(*out)){
				*out = osc_mem_alloc(6);
			}
			strncpy(*out, "false\0", 6);
			return 5;
		}
	case 'N': // NULL
		;
	}
	if(!(*out)){
		*out = osc_mem_alloc(1);
	}
	*out[0] = '\0';
	return 0;
}

int osc_atom_u_getBool(t_osc_atom_u *a){
	if(!a){
		return '\0';
	}
	switch(a->typetag){
	case 'T':
		return 1;
	case 'F':
	case 'N':
		return 0;
	case 'i':
		return !(a->w.i == 0);
	case 'h':
		return !(a->w.h == 0);
	case 'I':
		return !(a->w.I == 0);
	case 'H':
		return !(a->w.H == 0);
	case 'f':
		return !(a->w.f == 0);
	case 'd':
		return !(a->w.d == 0);
	case 's':
		// not sure what to do here...
		if(a->w.s){
			if(!strcmp(a->w.s, "true") || !strcmp(a->w.s, "True") || 
			   !strcmp(a->w.s, "TRUE") || (a->w.s[0] == 't' && a->w.s[1] == '\0') || 
			   (a->w.s[0] == 'T' && a->w.s[1] == '\0')){
				return 1;
			}else if(!strcmp(a->w.s, "false") || !strcmp(a->w.s, "False") || 
				 !strcmp(a->w.s, "FALSE") || (a->w.s[0] == 'f' && a->w.s[1] == '\0') || 
				 (a->w.s[0] == 'F' && a->w.s[1] == '\0')){
				return 0;
			}
		}else{
			return 0;
		}
	case '#':
		// uh...
		return 0;
	}
	return 0;
}

void osc_atom_u_setFloat(t_osc_atom_u *a, float v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.f = v;
	a->typetag = 'f';
}

void osc_atom_u_setDouble(t_osc_atom_u *a, double v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.d = v;
	a->typetag = 'd';
}

void osc_atom_u_setInt32(t_osc_atom_u *a, int32_t v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.i = v;
	a->typetag = 'i';
}

void osc_atom_u_setInt64(t_osc_atom_u *a, int64_t v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.h = v;
	a->typetag = 'h';
}

void osc_atom_u_setUInt32(t_osc_atom_u *a, uint32_t v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.I = v;
	a->typetag = 'I';
}

void osc_atom_u_setUInt64(t_osc_atom_u *a, uint64_t v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.H = v;
	a->typetag = 'H';
}

void osc_atom_u_setString(t_osc_atom_u *a, char *v){
	if(!a){
		return;
	}
	if(a->w.s && a->typetag == 's' && a->alloc){
		long len = strlen(v) + 1;
		a->w.s = osc_mem_resize(a->w.s, len);
		strlcpy(a->w.s, v, len);
	}else{
		a->w.s = osc_mem_alloc(strlen(v) + 1);
		strcpy(a->w.s, v);
		a->typetag = 's';
		a->alloc = 1;
	}
}

void osc_atom_u_setStringPtr(t_osc_atom_u *a, char *v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.s = v;
	a->typetag = 's';
}

void osc_atom_u_setBool(t_osc_atom_u *a, int v){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	if(v == 0){
		osc_atom_u_setFalse(a);
	}else{
		osc_atom_u_setTrue(a);
	}
}

void osc_atom_u_setTrue(t_osc_atom_u *a){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->typetag = 'T';
}

void osc_atom_u_setFalse(t_osc_atom_u *a){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->typetag = 'F';
}

void osc_atom_u_setNull(t_osc_atom_u *a){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->typetag = 'N';
}

void osc_atom_u_setBndl(t_osc_atom_u *a, t_osc_bndl_u *bndl){
	if(!a){
		return;
	}
	osc_atom_u_clear(a);
	a->w.bndl = bndl;
	a->typetag = '#';
}

size_t osc_atom_u_sizeof(t_osc_atom_u *a){
	if(!a){
		return 0;
	}
	switch(a->typetag){
	case 'i':
	case 'I':
	case 'f':
		return 4;
	case 'h':
	case 'H':
	case 'd':
		return 8;
	case 's':
		return strlen(a->w.s);
	case 'T':
	case 'F':
	case 'N':
		return 0;
	case '#':
		return ntoh32(*((uint32_t *)(a->w.bndl)));
	}
	return 0;
}

t_osc_err osc_atom_u_doSerialize(t_osc_atom_u *a, long *buflen, long *bufpos, char **buf){
	if(!a){
		return OSC_ERR_NOBUNDLE;
	}
	if((*buflen - *bufpos) < 64){
		*buf = osc_mem_resize(*buf, *buflen + 64);
		if(!(*buf)){
			return OSC_ERR_OUTOFMEM;
		}
		memset(*buf + *buflen, '\0', 64);
		(*buflen) += 64;
	}
	switch(osc_atom_u_getTypetag(a)){
	case 'i':
	case 'I':
		*((uint32_t *)(*buf + *bufpos)) = hton32(a->w.i);
		(*bufpos) += 4;
		break;
	case 'f':
		{
			float f = a->w.f;
			*((uint32_t *)((*buf) + (*bufpos))) = hton32(*((uint32_t *)(&f)));
			(*bufpos) += 4;

		}
		break;
	case 'd':
		{
			double d = a->w.d;
			*((uint64_t *)((*buf) + (*bufpos))) = hton64(*((uint64_t *)(&d)));
			(*bufpos) += 8;

		}
		break;
	case 's':
		*bufpos += sprintf(*buf + *bufpos, "%s", a->w.s);
		(*bufpos)++;
		while((*bufpos) % 4){
			(*bufpos)++;
		}
		break;
	case '#':
		{
			long bndlstart = *bufpos;
			(*bufpos) += 4;
			extern t_osc_err osc_bundle_u_doSerialize(t_osc_bndl_u *bndl, long *buflen, long *bufpos, char **buf);
			osc_bundle_u_doSerialize(a->w.bndl, buflen, bufpos, buf);
			*((uint32_t *)(*buf + bndlstart)) = hton32((*bufpos - bndlstart) - 4);
		}
		break;
	case 'h':
	case 'H':
		*((uint64_t *)(*buf + *bufpos)) = hton64(a->w.h);
		(*bufpos) += 8;
		break;
// nothing to do for T, F, or N
	case 'b':
		{
			/*
			int j, n = osc_sizeof(*(m->typetags), a->w.);
			*bufpos += sprintf(*buf + *bufpos, "blob (%d bytes): ", n);
			for(j = 0; j < n; j++){
				*bufpos += sprintf(*buf + *bufpos, "%d ", a->w.[j]);
			}
			*/
		}
		break;
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_atom_u_serialize(t_osc_atom_u *a, long *buflen, char **buf){
	if(!a){
		return OSC_ERR_NOBUNDLE;
	}
	long mybuflen = *buflen, mybufpos = 0;
	t_osc_err e = osc_atom_u_doSerialize(a, &mybuflen, &mybufpos, buf);
	*buflen = mybufpos;
	return e;
}

t_osc_err osc_atom_u_doFormat(t_osc_atom_u *a, long *buflen, long *bufpos, char **buf){
	if(!a){
		return OSC_ERR_NOBUNDLE;
	}
	if((*buflen - *bufpos) < 64){
		*buf = osc_mem_resize(*buf, *buflen + 64);
		if(!(*buf)){
			return OSC_ERR_OUTOFMEM;
		}
		*buflen += 64;
	}
	if(osc_atom_u_getTypetag(a) == '#'){
		*bufpos += sprintf(*buf + *bufpos, "[\n");
		extern t_osc_err osc_bundle_u_doFormat(t_osc_bndl_u *bndl, long *buflen, long *bufpos, char **buf);
		osc_bundle_u_doFormat(a->w.bndl, buflen, bufpos, buf);
		*bufpos += sprintf(*buf + *bufpos, "]");
	}else{
		char *b = *buf + *bufpos;
		(*bufpos) += osc_atom_u_getString(a, &b);
		(*buf)[(*bufpos)++] = ' ';
		(*buf)[(*bufpos)] = '\0';
	}
	/*
	switch(osc_atom_u_getTypetag(a)){
	case 'i':
		*bufpos += sprintf(*buf + *bufpos, "%"PRId32" ", a->w.i);
		break;
	case 'f':
		*bufpos += sprintf(*buf + *bufpos, "%f ", a->w.f);
		break;
	case 'd':
		*bufpos += sprintf(*buf + *bufpos, "%f ", a->w.d);
		break;
	case 's':
		*bufpos += sprintf(*buf + *bufpos, "%s ", a->w.s);
		break;
	case '#':
		{
			*bufpos += sprintf(*buf + *bufpos, "[\n");
			extern t_osc_err osc_bundle_u_doFormat(t_osc_bndl_u *bndl, long *buflen, long *bufpos, char **buf);
			osc_bundle_u_doFormat(a->w.bndl, buflen, bufpos, buf);
			*bufpos += sprintf(*buf + *bufpos, "]");
		}
	case 'T':
		*bufpos += sprintf(*buf + *bufpos, "true ");
		break;
	case 'F':
		*bufpos += sprintf(*buf + *bufpos, "false ");
		break;
	case 'h':
		*bufpos += sprintf(*buf + *bufpos, "%"PRId64" ", a->w.h);
		break;
	case 'I':
		*bufpos += sprintf(*buf + *bufpos, "%"PRIu32" ", a->w.I);
		break;
	case 'H':
		*bufpos += sprintf(*buf + *bufpos, "%"PRIu64" ", a->w.H);
		break;
	case 'N':
		*bufpos += sprintf(*buf + *bufpos, "NULL ");
		break;
	case 'b':
		{
			int j, n = osc_sizeof(*(m->typetags), a->w.);
			*bufpos += sprintf(*buf + *bufpos, "blob (%d bytes): ", n);
			for(j = 0; j < n; j++){
				*bufpos += sprintf(*buf + *bufpos, "%d ", a->w.[j]);
			}
		}
		break;
	}
	*/
	return OSC_ERR_NONE;
}

t_osc_err osc_atom_u_format(t_osc_atom_u *a, long *buflen, char **buf){
	if(!a){
		return OSC_ERR_NOBUNDLE;
	}
	long mybuflen = *buflen, mybufpos = 0;
	t_osc_err e = osc_atom_u_doFormat(a, &mybuflen, &mybufpos, buf);
	*buflen = mybufpos;
	return e;
}

