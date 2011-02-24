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

#include <stdlib.h>
#include <string.h>
#include "osc.h"
#include "osc_message.h"

t_osc_msg *osc_message_alloc(void){
	t_osc_msg *m = (t_osc_msg *)osc_mem_alloc(sizeof(t_osc_msg));
	if(!m){
		return NULL;
	}
	m->size = 0;
	m->address = NULL;
	m->typetags = m->typetags_start = NULL;
	m->typetag_buffer_pos = 0;
	m->typetag_buffer_size = 0;
	m->argv = m->argv_start = NULL;
	m->argv_buffer_pos = 0;
	m->argv_buffer_size = 0;
	m->next = m->prev = NULL;
	m->free_internal_buffers = 0;
	return m;
}

void osc_message_free(t_osc_msg *m){
	if(m->free_internal_buffers){
		if(m->address){
			osc_mem_free(m->address);
		}
		if(m->typetags){
			osc_mem_free(m->typetags);
		}
		if(m->argv){
			osc_mem_free(m->argv);
		}
	}
	osc_mem_free(m);
}

void osc_message_freeList(t_osc_msg *m){
	while(m){
		t_osc_msg *next = m->next;
		osc_message_free(m);
		m = next;
	}
}

t_osc_err osc_message_deepCopy(t_osc_msg **dest, t_osc_msg *source){
	if(!source){
		return OSC_ERR_MSGTOOSMALL;
	}
	if(!(*dest)){
		*dest = osc_message_alloc();
		if(!(*dest)){
			return OSC_ERR_OUTOFMEM;
		}
	}
	(*dest)->size = source->size;
	(*dest)->address = (char *)osc_mem_alloc(strlen(source->address));
	strcpy((*dest)->address, source->address);
	
	(*dest)->typetags = (char *)osc_mem_alloc(source->typetag_buffer_size);
	memset((*dest)->typetags, '\0', source->typetag_buffer_size);
	memcpy((*dest)->typetags, source->typetags, source->typetag_buffer_pos);
	(*dest)->typetag_buffer_pos = source->typetag_buffer_pos;
	(*dest)->typetag_buffer_size = source->typetag_buffer_size;
	
	(*dest)->argv = (char *)osc_mem_alloc(source->typetag_buffer_size);
	memset((*dest)->argv, '\0', source->typetag_buffer_size);
	memcpy((*dest)->argv, source->argv, source->typetag_buffer_pos);
	(*dest)->argv_buffer_pos = source->argv_buffer_pos;
	(*dest)->argv_buffer_size = source->argv_buffer_size;
	(*dest)->argc = source->argc;

	(*dest)->argv_start = (*dest)->argv;
	(*dest)->typetags_start = (*dest)->typetags;
	
	(*dest)->next = source->next;
	(*dest)->prev = source->prev;
	
	(*dest)->free_internal_buffers = 1;
	return OSC_ERR_NONE;
}

t_osc_err osc_message_parseMessage(int n, char *buf, t_osc_msg *osc_msg){
	if(!osc_msg){
		return OSC_ERR_NOBUNDLE;
	}
	char *ptr = buf;
	// this call could contain a message that's inside a bundle in which case the
	// size will be the first 4 bytes of buf
	// or it could be a naked message in which case n will probably be valid.
	if(n <= 0){
		// assume the first 4 bytes are the size
		n = ntoh32(*((uint32_t *)ptr));
		if(n > 1024 && *ptr == '/'){
			// what are the chances?  better bail out
			return OSC_ERR_MALFORMEDADDRESS;
		}
		ptr += 4;
	}

	osc_msg->address = NULL;
	osc_msg->typetags = NULL;
	osc_msg->argv = NULL;

	osc_msg->size = n;
	osc_msg->address = ptr;
	while(*ptr++){}
	while((ptr - buf) % 4){
		ptr++;
	}
	if(ptr - buf == n){
		return OSC_ERR_NONE;
	}
	// typetags are required
	if(*ptr != ','){
		return OSC_ERR_NONE;
	}
	// we set the typetags pointer to point to the , so that the first call to osc_util_incrementArg will set it to the first
	// actual argument so that you can just go while(osc_util_incrementArg(...)){}
	osc_msg->typetags = ptr;
	osc_msg->typetags_start = ptr;
	ptr++;

	while(*ptr){
		ptr++;
	}
	osc_msg->argc = ptr - osc_msg->typetags - 1;
	if(osc_msg->argc == 0){
		return OSC_ERR_NONE;
	}
	ptr++;
	while((ptr - buf) % 4){
		ptr++;
	}
	osc_msg->argv = ptr;
	osc_msg->argv_start = ptr;
	return OSC_ERR_NONE;
}

int osc_message_rename(int len, char *buffer, char *new_address, char **new_buffer){
	int new_address_len = strlen(new_address);
	int new_address_len_nullpadded = new_address_len;
	new_address_len_nullpadded++;
	while(new_address_len_nullpadded % 4){
		new_address_len_nullpadded++;
	}
	int old_address_len_nullpadded = strlen(buffer + 4);
	old_address_len_nullpadded++;
	while(old_address_len_nullpadded % 4){
		old_address_len_nullpadded++;
	}
	int newlen = len - old_address_len_nullpadded + new_address_len_nullpadded;
	char newbuf[newlen];
	memset(newbuf, '\0', newlen);
	char *ptr = newbuf + 4;
	memcpy(ptr, new_address, new_address_len);
	ptr += new_address_len;
	ptr++;
	while((ptr - newbuf) % 4){
		ptr++;
	}
	memcpy(ptr, buffer + old_address_len_nullpadded + 4, len - old_address_len_nullpadded - 4);
	*((uint32_t *)newbuf) = ntoh32(newlen);
	if(!(*new_buffer)){
		*new_buffer = (char *)osc_mem_alloc(newlen);
	}
	memcpy(*new_buffer, newbuf, newlen);
	return newlen;
}

uint32_t osc_message_getSize(char *buf){
	if(!buf){
		return 0;
	}
	return ntoh32(*((uint32_t *)buf));
}

int osc_message_getArgCount(t_osc_msg *msg){
	if(!msg){
		return 0;
	}
	if(!(msg->typetags)){
		return 0;
	}
	if(msg->typetags){
		return strlen(msg->typetags) - 1;
	}
	return 0;
}

int osc_message_incrementArg(t_osc_msg *msg){
	if(!msg->typetags || msg->argc == 0){
		return 0;
	}
	char tt = *(msg->typetags);
	if(tt == ','){
		msg->typetags += 1;
		return 1;
	}
	msg->typetags += 1;
	msg->argc--;
	if(msg->typetags == '\0' || msg->argc == 0){
		return 0;
	}
	int size = osc_sizeof(tt, msg->argv);
	msg->argv += size;
	return 1;
}

void osc_message_resetArgs(t_osc_msg *msg){
	msg->typetags = msg->typetags_start;
	msg->argv = msg->argv_start;
}

void osc_message_printMsg(t_osc_msg *msg, int (*p)(const char *, ...)){
	p("\t%d ", msg->size);
	p("%s ", msg->address);
	while(osc_message_incrementArg(msg)){
		switch(*(msg->typetags)){
		case 'i':
			p("%d ", ntoh32(*((int32_t *)msg->argv)));
			break;
		case 'f':
			{
				uint32_t l = ntoh32(*((int32_t *)msg->argv));
				p("%f ", *((float *)&l));
			}
			break;
		case 'h':
		case 't':
			p("%lld ", ntoh64(*((int64_t *)msg->argv)));
			break;
		case 'd':
			{
				uint64_t l = ntoh64(*((int64_t *)msg->argv));
				p("%f ", *((double *)&l));
			}
			break;
		case 's':
		case 'S':
			p("%s ", msg->argv);
			break;
		case 'c':
			p("%c ", *(msg->argv));
			break;
		case 'T':
			p("True ");
			break;
		case 'F':
			p("False ");
			break;
		case 'N':
			p("Nil ");
			break;
		case 'I':
			p("Infinitum ");
			break;
		case 'b':
			{
				int j, n = osc_sizeof(*(msg->typetags), msg->argv);
				p("blob (%d bytes): ", n);
				for(j = 0; j < n; j++){
					p("%d ", msg->argv[j]);
				}
			}
			break;
		}
	}
	p("\n");
}
