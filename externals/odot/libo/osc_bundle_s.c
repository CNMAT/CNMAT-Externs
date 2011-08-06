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
#include <inttypes.h>
#include "osc.h"
#include "osc_message_s.h"
#include "osc_atom_s.h"
#include "osc_bundle_s.h"
#include "osc_bundle_iterator_s.h"

t_osc_err osc_bundle_s_getMsgCount(int len, char *buf, int *count){
	*count = 0;
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

t_osc_err osc_bundle_s_getMessagesWithCallback(int len, char *buf, void (*f)(t_osc_msg_s*, void *), void *context){
	int ret;
	if(ret = osc_error_bundleSanityCheck(len, buf)){
		return ret;
	}
	t_osc_bndl_it_s *it = osc_bndl_it_s_get(len, buf);
	while(osc_bndl_it_s_hasNext(it)){
		t_osc_msg_s *m = osc_bndl_it_s_next(it);
		f(m, context);
	}
	osc_bndl_it_s_destroy(it);
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_s_lookupAddress(int len, char *buf, char *address, int *nmatches, t_osc_msg_s ***m, int fullmatch){
	int matchbuflen = 16, n = 0;
	t_osc_msg_s **matches = osc_mem_alloc(matchbuflen * sizeof(t_osc_msg_s *));
	t_osc_bndl_it_s *it = osc_bndl_it_s_get(len, buf);
	while(osc_bndl_it_s_hasNext(it)){
		if(n >= matchbuflen){
			matches = osc_mem_resize(matches, (matchbuflen + 16) * sizeof(t_osc_msg_s *));
			if(!matches){
				return OSC_ERR_OUTOFMEM;
			}
			matchbuflen += 16;
		}
		t_osc_msg_s *current_message = osc_bndl_it_s_next(it);
		int po, ao;
		int r = osc_match(address, osc_message_s_getAddress(current_message), &po, &ao);
		if(fullmatch){
			if(r != (OSC_MATCH_ADDRESS_COMPLETE | OSC_MATCH_PATTERN_COMPLETE)){
				continue;
			}
		}else{
			if(r == 0 || (((r & OSC_MATCH_PATTERN_COMPLETE) == 0) && address[po] != '/')){
				continue;
			}
		}
		//t_osc_msg_s *mm = osc_message_s_alloc();
		osc_message_s_copy(matches + n++, current_message);
		/*
		if(last == NULL){
			*m = mm;
			last = mm;
		}else{
			last->next = mm;
			mm->prev = last;
			last = mm;
		}
		*/
		//cont:
		//ptr += msgsize;
	}
	osc_bndl_it_s_destroy(it);
	*nmatches = n;
	*m = matches;
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_s_addSerializedMessage(long *len, char **bndl, long msglen, char *msg){
	char *tmp = (char *)osc_mem_resize(*bndl, *len + msglen + 4);
	if(!tmp){
		return OSC_ERR_OUTOFMEM;
	}
	memcpy(tmp + *len, msg, msglen + 4);
	*len = *len + msglen + 4;
	*bndl = tmp;
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_s_setBundleID(char *buf){
	if(!buf){
		return OSC_ERR_NOBUNDLE;
	}
	strncpy(buf, OSC_IDENTIFIER, OSC_IDENTIFIER_SIZE);
}

int osc_bundle_s_strcmpID(char *buf){
	return strncmp(buf, OSC_ID, OSC_ID_SIZE);
}

#ifdef OSC_2_0
t_osc_timetag osc_bundle_s_getTimetag(int len, char *buf){
}
#else
t_osc_timetag osc_bundle_s_getTimetag(int len, char *buf){
}
#endif

extern t_osc_err osc_message_s_doFormat(t_osc_msg_s *m, long *buflen, long *bufpos, char **buf);

void osc_bundle_s_formatBundleCbk(t_osc_msg_s *msg, void *context){
	struct context {long *buflen; long *bufpos; char **buf;};
	struct context *c = (struct context *)context;
	osc_message_s_doFormat(msg, c->buflen, c->bufpos, c->buf);
}

t_osc_err osc_bundle_s_doFormat(long len, char *bndl, long *buflen, long *bufpos, char **buf){
	struct context {long *buflen; 
		long *bufpos; 
		char **buf;} context = {buflen, bufpos, buf};
	osc_bundle_s_getMessagesWithCallback(len, bndl, osc_bundle_s_formatBundleCbk, (void *)&context);
}

t_osc_err osc_bundle_s_format(long len, char *bndl, long *buflen, char **buf){
	long mybuflen = 0, mybufpos = 0;
	if(*buflen > 0){
		if(*buf){
			mybuflen = *buflen;
		}
	}
	t_osc_err e = osc_bundle_s_doFormat(len, bndl, &mybuflen, &mybufpos, buf);
	// don't return the actual buffer length since it may be longer than the number of bytes used
	*buflen = mybufpos;
	return e;
}

