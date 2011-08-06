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
#include "osc_message_u.h"
#include "osc_message_u.r"
#include "osc_atom_u.h"
#include "osc_atom_u.r"
#include "osc_bundle_u.h"
#include "osc_bundle_u.r"
#include "osc_bundle_iterator_u.h"

t_osc_bndl_u *osc_bundle_u_alloc(void){
	t_osc_bndl_u *b = osc_mem_alloc(sizeof(t_osc_bndl_u));
	memset(b, '\0', sizeof(t_osc_bndl_u));
}

int osc_bundle_u_getMsgCount(t_osc_bndl_u *bndl){
	return bndl->msgcount;
}

t_osc_err osc_bundle_u_getMessagesWithCallback(t_osc_bndl_u *bndl, void (*f)(t_osc_msg_u*, void *), void *context){
	t_osc_bndl_it_u *it = osc_bndl_it_u_get(bndl);
	while(osc_bndl_it_u_hasNext(it)){
		t_osc_msg_u *m = osc_bndl_it_u_next(it);
		f(m, context);
	}
	osc_bndl_it_u_destroy(it);
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_u_lookupAddress(t_osc_bndl_u *bndl, char *address, int *nmatches, t_osc_msg_u ***m, int fullmatch){
	int matchbuflen = 16, n = 0;
	t_osc_msg_u **matches = osc_mem_alloc(matchbuflen * sizeof(t_osc_msg_u *));
	t_osc_bndl_it_u *it = osc_bndl_it_u_get(bndl);
	while(osc_bndl_it_u_hasNext(it)){
		if(n >= matchbuflen){
			matches = osc_mem_resize(matches, (matchbuflen + 16) * sizeof(t_osc_msg_u *));
			if(!matches){
				return OSC_ERR_OUTOFMEM;
			}
			matchbuflen += 16;
		}
		t_osc_msg_u *current_message = osc_bndl_it_u_next(it);
		int po, ao;
		int r = osc_match(address, osc_message_u_getAddress(current_message), &po, &ao);
		if(fullmatch){
			if(r != (OSC_MATCH_ADDRESS_COMPLETE | OSC_MATCH_PATTERN_COMPLETE)){
				continue;
			}
		}else{
			if(r == 0 || (((r & OSC_MATCH_PATTERN_COMPLETE) == 0) && address[po] != '/')){
				continue;
			}
		}
		osc_message_u_copy(matches + n++, current_message);
	}
	osc_bndl_it_u_destroy(it);
	*nmatches = n;
	*m = matches;
	return OSC_ERR_NONE;
}

t_osc_err osc_bundle_u_addMsg(t_osc_bndl_u *bndl, t_osc_msg_u *msg){
	if(!(bndl->msghead)){
		bndl->msghead = msg;
		bndl->msgtail = msg;
		msg->prev = msg->next = NULL;
		return OSC_ERR_NONE;
	}
	msg->prev = bndl->msgtail;
	msg->next = NULL;
	bndl->msgtail->next = msg;
	return OSC_ERR_NONE;
}

extern t_osc_err osc_message_u_doFormat(t_osc_msg_u *m, long *buflen, long *bufpos, char **buf);

void osc_bundle_u_formatBundleCbk(t_osc_msg_u *msg, void *context){
	struct context {long *buflen; long *bufpos; char **buf;};
	struct context *c = (struct context *)context;
	osc_message_u_doFormat(msg, c->buflen, c->bufpos, c->buf);
	*(c->bufpos) += sprintf(*(c->buf) + *(c->bufpos), "\n");
}

t_osc_err osc_bundle_u_doFormat(t_osc_bndl_u *bndl, long *buflen, long *bufpos, char **buf){
	struct context {long *buflen; 
		long *bufpos; 
		char **buf;} context = {buflen, bufpos, buf};
	osc_bundle_u_getMessagesWithCallback(bndl, osc_bundle_u_formatBundleCbk, (void *)&context);
}

t_osc_err osc_bundle_u_format(t_osc_bndl_u *bndl, long *buflen, char **buf){
	long mybuflen = 0, mybufpos = 0;
	if(*buflen > 0){
		if(*buf){
			mybuflen = *buflen;
		}
	}
	t_osc_err e = osc_bundle_u_doFormat(bndl, &mybuflen, &mybufpos, buf);
	// don't return the actual buffer length since it may be longer than the number of bytes used
	*buflen = mybufpos;
	return e;
}

