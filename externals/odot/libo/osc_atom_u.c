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
#include "osc_atom_u.h"
#include "osc_atom_u.r"

t_osc_atom_u *osc_atom_u_alloc(void){
	t_osc_atom_u *a = osc_mem_alloc(sizeof(t_osc_atom_u));
	memset(a, '\0', sizeof(t_osc_atom_u));
	return a;
}

char osc_atom_u_getTypetag(t_osc_atom_u *a){
	return a->typetag;
}

float osc_atom_u_getFloat(t_osc_atom_u *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (float)a->w.i;
	case 'h': // signed 64-bit int
		return (float)a->w.h;
	case 'I': // unsigned 32-bit int
		return (float)a->w.I;
	case 'H': // unsigned 64-bit int
		return (float)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (float)a->w.d;
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
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (double)a->w.i;
	case 'h': // signed 64-bit int
		return (double)a->w.h;
	case 'I': // unsigned 32-bit int
		return (double)a->w.I;
	case 'H': // unsigned 64-bit int
		return (double)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return (double)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return a->w.d;
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
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (int32_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (int32_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int32_t)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return (int32_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (int32_t)a->w.d;
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
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (int64_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (int64_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int64_t)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return (int64_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (int64_t)a->w.d;
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

uint32_t osc_atom_u_getUint32(t_osc_atom_u *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (uint32_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (uint32_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (uint32_t)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return (uint32_t)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (uint32_t)a->w.d;
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

uint64_t osc_atom_u_getUint64(t_osc_atom_u *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (uint64_t)a->w.h;
	case 'I': // unsigned 32-bit int
		return (uint64_t)a->w.I;
	case 'H': // unsigned 64-bit int
		return (uint64_t)a->w.H;
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
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return a->w.i;
	case 'h': // signed 64-bit int
		return (int)a->w.h;
	case 'I': // unsigned 32-bit int
		return (int)a->w.I;
	case 'H': // unsigned 64-bit int
		return (int)a->w.H;
	case 'f': // 32-bit IEEE 754 float
		return (int)a->w.f;
	case 'd': // 64-bit IEEE 754 double
		return (int)a->w.d;
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

char *osc_atom_u_getString(t_osc_atom_u *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		{
			int n = snprintf(NULL, 0, "%"PRId32, a->w.i);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRId32, a->w.i);
			return buf;
		}
	case 'h': // signed 64-bit int
		{
			int n = snprintf(NULL, 0, "%"PRId64, a->w.h);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRId64, a->w.h);
			return buf;
		}
	case 'I': // unsigned 32-bit int
		{
			int n = snprintf(NULL, 0, "%"PRIu32, a->w.I);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRIu32, a->w.I);
			return buf;
		}
	case 'H': // unsigned 64-bit int
		{
			int n = snprintf(NULL, 0, "%"PRIu64, a->w.H);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRIu64, a->w.H);
			return buf;
		}
	case 'f': // 32-bit IEEE 754 float
		{
			int n = snprintf(NULL, 0, "%f", a->w.f);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%f", a->w.f);
			return buf;
		}
	case 'd': // 64-bit IEEE 754 double
		{
			int n = snprintf(NULL, 0, "%f", a->w.f);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%f", a->w.f);
			return buf;
		}
	case 's': // string
		{
			int n = strlen(a->w.s);
			char *buf = osc_mem_alloc(n + 1);
			strcpy(buf, a->w.s);
			return buf;
		}
	case 'T': // true
		{
			char *buf = osc_mem_alloc(5);
			strncpy(buf, "true\0", 5);
			return buf;
		}
	case 'F': // false
		{
			char *buf = osc_mem_alloc(6);
			strncpy(buf, "false\0", 6);
			return buf;
		}
	case 'N': // NULL
		;
	}
	char *buf = osc_mem_alloc(1);
	buf[0] = '\0';
	return buf;
}

int osc_atom_u_getBool(t_osc_atom_u *a){
	switch(a->typetag){
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
	case 'T':
		return 1;
	case 'F':
	case 'N':
		return 0;
	case '#':
		// uh...
		return 0;
	}
	return 0;
}

void osc_atom_u_setFloat(t_osc_atom_u *a, float v){
	a->w.f = v;
	a->typetag = 'f';
}

void osc_atom_u_setDouble(t_osc_atom_u *a, double v){
	a->w.d = v;
	a->typetag = 'd';
}

void osc_atom_u_setInt32(t_osc_atom_u *a, int32_t v){
	a->w.i = v;
	a->typetag = 'i';
}

void osc_atom_u_setInt64(t_osc_atom_u *a, int64_t v){
	a->w.h = v;
	a->typetag = 'h';
}

void osc_atom_u_setUInt32(t_osc_atom_u *a, uint32_t v){
	a->w.I = v;
	a->typetag = 'I';
}

void osc_atom_u_setUInt64(t_osc_atom_u *a, uint64_t v){
	a->w.H = v;
	a->typetag = 'H';
}

void osc_atom_u_setString(t_osc_atom_u *a, char *v){
	a->w.s = v;
	a->typetag = 's';
}

void osc_atom_u_setBool(t_osc_atom_u *a, int v){
	if(v == 0){
		osc_atom_u_setTrue(a);
	}else{
		osc_atom_u_setFalse(a);
	}
}

void osc_atom_u_setTrue(t_osc_atom_u *a){
	a->typetag = 'T';
}

void osc_atom_u_setFalse(t_osc_atom_u *a){
	a->typetag = 'F';
}

void osc_atom_u_setNull(t_osc_atom_u *a){
	a->typetag = 'N';
}

size_t osc_atom_u_sizeof(t_osc_atom_u *a){
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

t_osc_err osc_atom_u_doFormat(t_osc_atom_u *a, long *buflen, long *bufpos, char **buf){
	if((*buflen - *bufpos) < 64){
		*buf = osc_mem_resize(*buf, *buflen + 64);
		if(!(*buf)){
			return OSC_ERR_OUTOFMEM;
		}
		*buflen += 64;
	}
	switch(osc_atom_u_getTypetag(a)){
	case 'i':
		*bufpos += sprintf(*buf + *bufpos, "%"PRId32" ", a->w.i);
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
	case 'f':
		*bufpos += sprintf(*buf + *bufpos, "%f ", a->w.f);
		break;
	case 'd':
		*bufpos += sprintf(*buf + *bufpos, "%f ", a->w.d);
		break;
	case 's':
		*bufpos += sprintf(*buf + *bufpos, "%s ", a->w.s);
		break;
	case 'T':
		*bufpos += sprintf(*buf + *bufpos, "true ");
		break;
	case 'F':
		*bufpos += sprintf(*buf + *bufpos, "false ");
		break;
	case 'N':
		*bufpos += sprintf(*buf + *bufpos, "NULL ");
		break;
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
	case '#':
		{
			*bufpos += sprintf(*buf + *bufpos, "[\n");
			osc_bundle_formatBndl(ntoh32(*((uint32_t *)a->w.bndl)), a->w.bndl + 4, buflen, bufpos, buf);
			*bufpos += sprintf(*buf + *bufpos, "]");
		}
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_atom_u_format(t_osc_atom_u *a, long *buflen, char **buf){
	long mybuflen = 0, mybufpos = 0;
	t_osc_err e = osc_atom_u_doFormat(a, &mybuflen, &mybufpos, buf);
	*buflen = mybufpos;
	return e;
}
