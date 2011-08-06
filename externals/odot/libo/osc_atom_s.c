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
#include "osc_atom_s.h"
#include "osc_atom_s.r"

t_osc_atom_s *osc_atom_s_alloc(char typetag, char *ptr){
	t_osc_atom_s *a = osc_mem_alloc(sizeof(t_osc_atom_s));
	osc_atom_s_set(a, typetag, ptr);
	return a;
}

void osc_atom_s_set(t_osc_atom_s *a, char typetag, char *ptr){
	a->typetag = typetag;
	a->data = ptr;
}

char osc_atom_s_getTypetag(t_osc_atom_s *a){
	return a->typetag;
}

char *osc_atom_s_getData(t_osc_atom_s *a){
	return a->data;
}

float osc_atom_s_getFloat(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (float)(ntoh32(*((int32_t *)(a->data))));
	case 'h': // signed 64-bit int
		return (float)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return (float)(ntoh32(*((uint32_t *)(a->data))));
	case 'H': // unsigned 64-bit int
		return (float)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return *((float *)&i);
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (float)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return strtof(a->data, &endp);
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

double osc_atom_s_getDouble(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (double)(ntoh32(*((int32_t *)(a->data))));
	case 'h': // signed 64-bit int
		return (double)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return (double)(ntoh32(*((uint32_t *)(a->data))));
	case 'H': // unsigned 64-bit int
		return (double)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (double)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return *((double *)&i);
		}
	case 's': // string
		{
			char *endp = NULL;
			return strtod(a->data, &endp);
		}
	case 'T': // true
		return 1.;
	case 'F': // false
		return 0.;
	case 'N': // NULL
		return 0.;
	}
	return 0.;
}

int32_t osc_atom_s_getInt32(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return ntoh32(*((int32_t *)(a->data)));
	case 'h': // signed 64-bit int
		return (int32_t)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return ntoh32(*((uint32_t *)(a->data)));
	case 'H': // unsigned 64-bit int
		return (int32_t)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			// this looks nasty, but it's correct.
			// interpret the bits as uint32 so that we can
			// call ntoh32, then interpret them as float, then
			// cast to int32
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (int32_t)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (int32_t)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return (int32_t)strtol(a->data, &endp, 0);
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

int64_t osc_atom_s_getInt64(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return ntoh32(*((int32_t *)(a->data)));
	case 'h': // signed 64-bit int
		return ntoh64(*((int64_t *)(a->data)));
	case 'I': // unsigned 32-bit int
		return (int64_t)(ntoh32(*((uint32_t *)(a->data))));
	case 'H': // unsigned 64-bit int
		return (int64_t)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (int64_t)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (int64_t)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return (int64_t)strtoll(a->data, &endp, 0);
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

uint32_t osc_atom_s_getUInt32(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (uint32_t)(ntoh32(*((int32_t *)(a->data))));
	case 'h': // signed 64-bit int
		return (uint32_t)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return ntoh32(*((uint32_t *)(a->data)));
	case 'H': // unsigned 64-bit int
		return (uint32_t)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (uint32_t)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (uint32_t)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return (uint32_t)strtoul(a->data, &endp, 0);
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

uint64_t osc_atom_s_getUInt64(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (uint64_t)(ntoh32(*((int32_t *)(a->data))));
	case 'h': // signed 64-bit int
		return (uint64_t)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return (uint64_t)(ntoh32(*((uint32_t *)(a->data))));
	case 'H': // unsigned 64-bit int
		return ntoh64(*((uint64_t *)(a->data)));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (uint64_t)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (uint64_t)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return (uint64_t)strtoull(a->data, &endp, 0);
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

int osc_atom_s_getInt(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		return (int)(ntoh32(*((int32_t *)(a->data))));
	case 'h': // signed 64-bit int
		return (int)(ntoh64(*((int64_t *)(a->data))));
	case 'I': // unsigned 32-bit int
		return (int)(ntoh32(*((uint32_t *)(a->data))));
	case 'H': // unsigned 64-bit int
		return (int)(ntoh64(*((uint64_t *)(a->data))));
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			return (int)(*((float *)&i));
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			return (int)(*((double *)&i));
		}
	case 's': // string
		{
			char *endp = NULL;
			return (int)strtol(a->data, &endp, 0);
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

char *osc_atom_s_getString(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i': // signed 32-bit int
		{
			int32_t i = ntoh32(*((int32_t *)(a->data)));
			int n = snprintf(NULL, 0, "%"PRId32, i);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRId32, i);
			return buf;
		}
	case 'h': // signed 64-bit int
		{
			int64_t i = ntoh64(*((int64_t *)(a->data)));
			int n = snprintf(NULL, 0, "%"PRId64, i);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRId64, i);
			return buf;
		}
	case 'I': // unsigned 32-bit int
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			int n = snprintf(NULL, 0, "%"PRIu32, i);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRIu32, i);
			return buf;
		}
	case 'H': // unsigned 64-bit int
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			int n = snprintf(NULL, 0, "%"PRIu64, i);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%"PRIu64, i);
			return buf;
		}
	case 'f': // 32-bit IEEE 754 float
		{
			uint32_t i = ntoh32(*((uint32_t *)(a->data)));
			float f = *((float *)&i);
			int n = snprintf(NULL, 0, "%f", f);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%g", f);
			return buf;
		}
	case 'd': // 64-bit IEEE 754 double
		{
			uint64_t i = ntoh64(*((uint64_t *)(a->data)));
			double f = *((double *)&i);
			int n = snprintf(NULL, 0, "%f", f);
			char *buf = osc_mem_alloc(n + 1);
			sprintf(buf, "%g", f);
			return buf;
		}
	case 's': // string
		{
			int n = strlen(a->data);
			char *buf = osc_mem_alloc(n + 1);
			strncpy(buf, a->data, n + 1);
			return buf;
		}
	case 'T': // true
		{
			char *buf = osc_mem_alloc(5);
			strncpy(buf, "true\0", 5); // i know it's null terminated--just being explicit...
			return buf;
		}
	case 'F': // false
		{
			char *buf = osc_mem_alloc(6);
			strncpy(buf, "false\0", 6); // i know it's null terminated--just being explicit...
			return buf;
		}
	case 'N': // NULL
		;
		// fall through
	}
	char *buf = osc_mem_alloc(1);
	buf[0] = '\0';
	return buf;
}

int osc_atom_s_getBool(t_osc_atom_s *a){
	switch(a->typetag){
	case 'i':
		return !(ntoh32(*((int32_t *)a->data)) == 0);
	case 'h':
		return !(ntoh64(*((int64_t *)a->data)) == 0);
	case 'I':
		return !(ntoh32(*((uint32_t *)a->data)) == 0);
	case 'H':
		return !(ntoh64(*((uint64_t *)a->data)) == 0);
	case 'f':
		{
			uint32_t i = ntoh32(*((uint32_t *)a->data));
			return !(*((float *)&i) == 0);
		}
	case 'd':
		{
			uint64_t i = ntoh64(*((uint64_t *)a->data));
			return !(*((double *)&i) == 0);
		}
	case 's':
		// not sure what to do here...
		if(a->data){
			if(!strcmp(a->data, "true") || !strcmp(a->data, "True") || 
			   !strcmp(a->data, "TRUE") || (a->data[0] == 't' && a->data[1] == '\0') || 
			   (a->data[0] == 'T' && a->data[1] == '\0')){
				return 1;
			}else if(!strcmp(a->data, "false") || !strcmp(a->data, "False") || 
				 !strcmp(a->data, "FALSE") || (a->data[0] == 'f' && a->data[1] == '\0') || 
				 (a->data[0] == 'F' && a->data[1] == '\0')){
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
		return 0;
	}
	return 0;
}

void osc_atom_s_setFloat(t_osc_atom_s *a, float v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(sizeof(float));
	}
	a->typetag = 'f';
	*((float *)(a->data)) = hton32(*((uint32_t *)&v));
}

void osc_atom_s_setDouble(t_osc_atom_s *a, double v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(sizeof(double));
	}
	a->typetag = 'd';
	*((double *)(a->data)) = hton64(*((uint64_t *)&v));
}

void osc_atom_s_setInt32(t_osc_atom_s *a, int32_t v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(4);
	}
	a->typetag = 'i';
	*((int32_t *)(a->data)) = hton32(v);
}

void osc_atom_s_setInt64(t_osc_atom_s *a, int64_t v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(8);
	}
	a->typetag = 'h';
	*((int64_t *)(a->data)) = hton64(v);
}

void osc_atom_s_setUInt32(t_osc_atom_s *a, uint32_t v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(4);
	}
	a->typetag = 'I';
	*((uint32_t *)(a->data)) = hton32(v);
}

void osc_atom_s_setUInt64(t_osc_atom_s *a, uint64_t v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(8);
	}
	a->typetag = 'H';
	*((uint64_t *)(a->data)) = hton64(v);
}

void osc_atom_s_setString(t_osc_atom_s *a, char *v){
	if(a->data == NULL){
		a->data = osc_mem_alloc(strlen(v) + 1);
	}
	a->typetag = 's';
	strcpy(a->data, v);
}

void osc_atom_s_setTrue(t_osc_atom_s *a){
	a->typetag = 'T';
}

void osc_atom_s_setFalse(t_osc_atom_s *a){
	a->typetag = 'F';
}

void osc_atom_s_setNull(t_osc_atom_s *a){
	a->typetag = 'N';
}

size_t osc_atom_s_sizeof(t_osc_atom_s *a){
	return osc_sizeof(a->typetag, a->data);
}

t_osc_err osc_atom_s_doFormat(t_osc_atom_s *a, long *buflen, long *bufpos, char **buf){
	if((*buflen - *bufpos) < 64){
		*buf = osc_mem_resize(*buf, *buflen + 64);
		if(!(*buf)){
			return OSC_ERR_OUTOFMEM;
		}
		*buflen += 64;
	}
	char *data = osc_atom_s_getData(a);
	switch(osc_atom_s_getTypetag(a)){
	case 'i':
		*bufpos += sprintf(*buf + *bufpos, "%"PRId32" ", (int32_t)ntoh32(*((int32_t *)data)));
		break;
	case 'h':
		*bufpos += sprintf(*buf + *bufpos, "%"PRId64" ", (int64_t)ntoh64(*((int64_t *)data)));
		break;
	case 'I':
		*bufpos += sprintf(*buf + *bufpos, "%"PRIu32" ", (uint32_t)ntoh32(*((uint32_t *)data)));
		break;
	case 'H':
		*bufpos += sprintf(*buf + *bufpos, "%"PRIu64" ", (uint64_t)ntoh64(*((uint64_t *)data)));
		break;
	case 'f':
		{
			uint32_t l = ntoh32(*((uint32_t *)data));
			*bufpos += sprintf(*buf + *bufpos, "%f ", *((float *)&l));
		}
		break;
	case 'd':
		{
			uint64_t l = ntoh64(*((int64_t *)data));
			*bufpos += sprintf(*buf + *bufpos, "%f ", *((double *)&l));
		}
		break;
	case 's':
		*bufpos += sprintf(*buf + *bufpos, "%s ", data);
		break;
	case 'c':
		*bufpos += sprintf(*buf + *bufpos, "%c ", *(data));
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
			int j, n = osc_sizeof(*(m->typetags), data);
			*bufpos += sprintf(*buf + *bufpos, "blob (%d bytes): ", n);
			for(j = 0; j < n; j++){
				*bufpos += sprintf(*buf + *bufpos, "%d ", data[j]);
			}
			*/
		}
		break;
	case '#':
		{
			*bufpos += sprintf(*buf + *bufpos, "[\n");
			osc_bundle_formatBndl(ntoh32(*((uint32_t *)data)), data + 4, buflen, bufpos, buf);
			*bufpos += sprintf(*buf + *bufpos, "]");
		}
	}
	return OSC_ERR_NONE;
}

t_osc_err osc_atom_s_format(t_osc_atom_s *a, long *buflen, char **buf){
	long mybuflen = 0, mybufpos = 0;
	t_osc_err e = osc_atom_s_doFormat(a, &mybuflen, &mybufpos, buf);
	*buflen = mybufpos;
	return e;
}
