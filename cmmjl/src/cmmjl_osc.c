/*
  Copyright (c) 1996-2008.  The Regents of the University of California (Regents).
  All Rights Reserved.

  Permission to use, copy, modify, and distribute this software and its
  documentation for educational, research, and not-for-profit purposes, without
  fee and without a signed licensing agreement, is hereby granted, provided that
  the above copyright notice, this paragraph and the following two paragraphs
  appear in all copies, modifications, and distributions.  Contact The Office of
  Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
  CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

  Written by Matt Wright, John MacCallum, and Andy Schmeder, The Center for New Music and
  Audio Technologies, University of California, Berkeley.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
  ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
  REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
  DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
  REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
  ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "cmmjl.h"
#include "cmmjl_osc.h"
#include "cmmjl_commonsymbols.h"
#include "cmmjl_error.h"
#include <libgen.h>
#include "jpatcher_api.h"
//#include <iostream>
//#include <boost/regex.hpp>

t_max_err cmmjl_osc_address_get(void *x, t_object *attr, long *argc, t_atom **argv){
	if(!(*argc) || !(*argv)){
		*argc = 1;
		*argv = (t_atom *)calloc(*argc, sizeof(t_atom));
		if(!argv){
			*argc = 0;
			PERROR("%s", cmmjl_strerror(CMMJL_ENOMEM));
			return MAX_ERR_OUT_OF_MEM;
		}
	}
	t_symbol *sym = cmmjl_osc_getAddress(x);
	if(!sym){
		return MAX_ERR_GENERIC;
	}
	atom_setsym(*argv, sym);
	return MAX_ERR_NONE;
}

t_max_err cmmjl_osc_address_set(void *x, t_object *attr, long argc, t_atom *argv){
	if(argc && argv){
		cmmjl_osc_setAddress(x, atom_getsym(argv));
	}else{
		cmmjl_osc_setAddress(x, gensym(""));
	}
	cmmjl_osc_saveAddressWithPatcher(x, true);
	return MAX_ERR_NONE;
}

t_symbol *cmmjl_osc_getAddress(void *x){
	if(!_cmmjl_obj_tab){
		return NULL;
	}
	t_object *ob;
	t_max_err e;
	if(e = hashtab_lookup(_cmmjl_obj_tab, x, &ob)){
		return NULL;
	}
	return ((t_cmmjl_obj *)ob)->osc_address;
}

void cmmjl_osc_setAddress(void *x, t_symbol *address){
	if(!_cmmjl_obj_tab){
		return;
	}
	t_object *ob;
	hashtab_lookup(_cmmjl_obj_tab, x, &ob);
	((t_cmmjl_obj *)ob)->osc_address = address;
}

void cmmjl_osc_setDefaultAddress(void *x){
	char buf[256];
	long instance = cmmjl_obj_instance_get(x);
	cmmjl_osc_makeDefaultAddress(x, instance, buf);
	cmmjl_osc_setAddress(x, gensym(buf));
	cmmjl_osc_saveAddressWithPatcher(x, false);
}

void cmmjl_osc_makeDefaultAddress(void *x, long instance, char *buf){
	char buf2[256], tmp[64];
	t_patcher *p, *pp;
	t_box *b;
	t_symbol *name;
	char *ptr;
	int index;
	object_obex_lookup(x, gensym("#P"), (t_object **)&p);
	if(!p){
		return;
	}
	name = jpatcher_get_name(p);
	if(name){
		if(strcmp(name->s_name, "")){
			// if this is a filename, get rid of the extension
			if(ptr = strrchr(name->s_name, '.')){
				memset(tmp, '\0', 64);
				index = name->s_name - ptr;
				if(index < 0) index = -index;
				memcpy(tmp, name->s_name, index);
				sprintf(buf2, "/%s", tmp);
			}else{
				sprintf(buf2, "/%s", name->s_name);
			}
		}
	}else{
		// maybe we didn't wait long enough...
	}
	b = jpatcher_get_box(p);
	if(b){
		while(p){
			pp = jpatcher_get_parentpatcher(p);
			if(pp){
				name = jpatcher_get_name(pp);
				if(name){
					if(strcmp(name->s_name, "")){
						if(ptr = strrchr(name->s_name, '.')){
							memset(tmp, '\0', 64);
							index = name->s_name - ptr;
							if(index < 0) index = -index;
							memcpy(tmp, name->s_name, index);
							sprintf(buf, "/%s%s", tmp, buf2);
						}else{
							sprintf(buf, "/%s%s", name->s_name, buf2);
						}
					}
				}
			}
			p = pp;
			strcpy(buf2, buf);
		}
	}


}

void cmmjl_osc_saveAddressWithPatcher(void *x, bool b){
	long ac;
	t_atom a, *aa;
	t_patcher *p;
	object_attr_attr_getvalueof(x, ps_OSCaddress, gensym("save"), &ac, &aa);
	if(aa->a_type == A_LONG){
		if(aa->a_w.w_long != (long)b){
			atom_setlong(&a, b ? 1 : 0);
			object_attr_attr_setvalueof(x, ps_OSCaddress, gensym("save"), 1, &a);
			object_obex_lookup(x, gensym("#P"), (t_object **)&p);
			if(p){
				jpatcher_set_dirty(p, true);
			}
		}
	}
}

int cmmjl_osc_init_bundle(int len, char *ptr, char *timetag){
	if(len < 16){
		return 0;
	}
	ptr[0] = '#';
	ptr[1] = 'b';
	ptr[2] = 'u';
	ptr[3] = 'n';
	ptr[4] = 'd';
	ptr[5] = 'l';
	ptr[6] = 'e';
	ptr[7] = '\0';
	if(timetag == NULL){
		*((long long *)(ptr + 8)) = 0x100000000000000LL;
	}else{
		memcpy(ptr + 8, timetag, 8);
	}
	return 16;
}

int cmmjl_osc_get_msg_length(char *address, char *tt, int argc, char *argv){
	int addressLen = strlen(address);
	addressLen++;
	while(addressLen % 4){
		addressLen++;
	}
	int ttLen = strlen(tt);
	ttLen++;
	while(ttLen % 4){
		ttLen++;
	}
	int argLen = 0;
	int i;
	for(i = 0; i < argc; i++){
		if(tt[i + 1] == 'f' || tt[i + 1] == 'i'){
			argLen += 4;
		}else{
			int n = strlen(argv + argLen);
			argLen += n + 1;
			while(argLen % 4){
				argLen++;
			}
		}
	}
	return addressLen + ttLen + argLen;
}

int cmmjl_osc_get_msg_length_max(t_symbol *msg, short argc, t_atom *argv){
	int addressLen = strlen(msg->s_name);
	addressLen++;
	while(addressLen % 4){
		addressLen++;
	}
	int ttLen = argc + 2;
	while(ttLen % 4){
		ttLen++;
	}
	int argLen = 0;
	int i;
	for(i = 0; i < argc; i++){
		if(argv[i].a_type == A_FLOAT || argv[i].a_type == A_LONG){
			argLen += 4;
		}else{
			argLen += strlen(atom_getsym(argv + i)->s_name);
			argLen++;
			while(argLen % 4){
				argLen++;
			}
		}
	}
	return addressLen + ttLen + argLen;
}

int cmmjl_osc_make_bundle_from_atoms(long argc, t_atom *argv, int *len, char *buffer){
	int i;
	if(atom_gettype(argv) != A_SYM){
		return 0;
	}
	if(*(atom_getsym(argv)->s_name) != '/'){
		return 0;
	}
	int numargs = 0;
	char typetags[argc];
	char argbuf[*len];
	char *bufptr = buffer, *argbufptr = argbuf;
	char *sizeptr = buffer;
	bufptr += 4; // skip over the size
        for(i = 0; i < argc; i++){
		int type;
		if((type = atom_gettype(argv + i)) == A_SYM){
			t_symbol *sym = atom_getsym(argv + i);
			if(*(sym->s_name) == '/'){
				// OSC address
				if(numargs){
					*bufptr++ = ',';
					memcpy(bufptr, typetags, numargs);
					bufptr += numargs;
					*bufptr++ = '\0';
					while((bufptr - buffer) % 4){
						*bufptr++ = '\0';
					}
					memcpy(bufptr, argbuf, argbufptr - argbuf);
					bufptr += (argbufptr - argbuf);
					*((long *)sizeptr) = htonl((bufptr - sizeptr) - 4);
					sizeptr = bufptr;
					bufptr += 4;
				}
				memset(argbuf, '\0', *len);
				memset(typetags, '\0', argc);
				argbufptr = argbuf;

				numargs = 0;
				strcpy(bufptr, sym->s_name);
				bufptr += strlen(sym->s_name) + 1;
				while((bufptr - buffer) % 4){
					*bufptr++ = '\0';
				}
			}else{
				typetags[numargs] = 's';
				strcpy(argbufptr, sym->s_name);
				argbufptr += strlen(sym->s_name) + 1;
				while((argbufptr - buffer) % 4){
					argbufptr++;
				}
				numargs++;
			}
		}else{
			switch(type){
			case A_FLOAT:
				*((long *)argbufptr) = htonl(*((long *)&(argv[i].a_w.w_float)));
				typetags[numargs] = 'f';
				break;
			case A_LONG:
				*((long *)argbufptr) = htonl(argv[i].a_w.w_long);
				typetags[numargs] = 'i';
				break;
			}
			numargs++;
			argbufptr += 4;
		}
        }
	if(numargs){
		*bufptr++ = ',';
		memcpy(bufptr, typetags, numargs);
		bufptr += numargs;
		*bufptr++ = '\0';
		while((bufptr - buffer) % 4){
			*bufptr++ = '\0';
		}
		memcpy(bufptr, argbuf, argbufptr - argbuf);
		bufptr += (argbufptr - argbuf);
		*((long *)sizeptr) = htonl((bufptr - sizeptr) - 4);
	}
	return bufptr - buffer;
}

int cmmjl_osc_make_bundle(int numAddresses,
			  t_symbol **addresses, 
			  int *numArgs,
			  char **typetags, 
			  t_atom **args, 
			  int *len, 
			  char *buffer)
{
	if(strncmp(buffer, "#bundle", 8)){
		cmmjl_osc_init_bundle(*len, buffer, NULL);
	}
	int i, j;
	char *ptr = &(buffer[16]);
	for(i = 0; i < numAddresses; i++){
		char *sizeptr = ptr;
		ptr += 4; // skip over size
		*len = cmmjl_osc_check_pos_and_resize(buffer, *len, ptr);
		int addlen = strlen(addresses[i]->s_name);
		memcpy(ptr, addresses[i]->s_name, addlen);
		ptr += addlen;
		ptr += 4 - (addlen % 4);
		*len = cmmjl_osc_check_pos_and_resize(buffer, *len, ptr);
		*ptr = ',';
		char *tt = ++ptr;
		ptr += numArgs[i];
		ptr += 4 - ((numArgs[i] + 1) % 4);
		*len = cmmjl_osc_check_pos_and_resize(buffer, *len, ptr);
		for(j = 0; j < numArgs[i]; j++){
			*tt = typetags[i][j];
			switch(*tt){
			case 'i':
				*((long *)ptr) = htonl(atom_getlong(args[i] + j));
				ptr += 4;
				break;
			case 'f':
				{
					float f = atom_getfloat(args[i] + j);
					*((long *)ptr) = htonl(*((long *)(&f)));
					ptr += 4;
				}
				break;
			case 's':
				{
					t_symbol *s = atom_getsym(args[i] + j);
					int slen = strlen(s->s_name);
					memcpy(ptr, s->s_name, slen);
					ptr += slen;
					ptr += 4 - (slen % 4);
				}
				break;
			case 'I':
				break;
			}
			tt++;
			*len = cmmjl_osc_check_pos_and_resize(buffer, *len, ptr);
		}
		*((long *)sizeptr) = htonl(ptr - sizeptr - 4);
	}
	return ptr - buffer;
}

int cmmjl_osc_check_pos_and_resize(char *buf, int len, char *pos){
	if(pos - buf >= len){
		realloc(buf, len * 2);
		return len * 2;
	}
	return len;
}

int cmmjl_osc_add_to_bundle(int len, char *ptr, t_cmmjl_osc_message *msg){
	*((long *)ptr) = htonl(msg->size);
	memcpy(ptr + 4, msg->address, msg->size);
	return msg->size + 4; // the contents + 4 bytes for the size
}

int cmmjl_osc_rename(char *buffer, 
		    int bufferLen, 
		    int bufferPos, 
		    t_cmmjl_osc_message *msg, 
		    char *newAddress){
	int start = bufferPos;
	int len = strlen(newAddress);
	int bp = bufferPos;
	len += 4 - (len % 4);
	*((long *)(buffer + bp)) = htonl(msg->size + (len - (msg->typetags - msg->address)));
	bp += 4;
	memcpy(buffer + bp, newAddress, strlen(newAddress));
	bp += len;
	len = msg->size - (msg->typetags - msg->address);
	memcpy(buffer + bp, msg->typetags, len);
	bp += len;
	return bp - start;
}

long cmmjl_osc_bundle_naked_message(long n, char *ptr, char *out){
	if(!strncmp(ptr, "#bundle\0", 8)){
		return -1;
	}
	if(!out){
		out = malloc(n + 20);
	}
	char buf[n + 20];
	buf[0] = '#';
	buf[1] = 'b';
	buf[2] = 'u';
	buf[3] = 'n';
	buf[4] = 'd';
	buf[5] = 'l';
	buf[6] = 'e';
	buf[7] = '\0';
	*((long long *)(buf + 8)) = 0x000000001;
	*((long *)(buf + 16)) = htonl(n);
	memcpy(buf + 20, ptr, n);
	memcpy(out, buf, n + 20);
	return n + 20;
}

long cmmjl_osc_flatten(long n, char *ptr, char *out){
	if(strncmp(ptr, "#bundle\0", 8)){
		return -1;
	}
	if(!out){
		out = malloc(n);
	}
	int i = 0, j = 0;
	memcpy(out, ptr, 16);
	i = j = 16;
	while(i < n){
		if(!strncmp(ptr + i, "#bundle\0", 8)){
			i += 16; // skip over #bundle\0, timetag, and size
			j -= 4;
		}else{
			*(out + (j++)) = *(ptr + i++);
		}
	}
	memset(out + j, '\0', n - j);
	return j;
}

int cmmjl_osc_copy_max_messages(t_symbol *msg, short argc, t_atom *argv, int len, char *buf){
	int pos = 0;
	int n = strlen(msg->s_name);
	int i;
	memcpy(buf + pos, msg->s_name, n);
	pos += n + 1;
	while(pos % 4){
		pos++;
	}
	n = argc;
	buf[pos++] = ',';
	for(i = 0; i < argc; i++){
		switch(argv[i].a_type){
		case A_FLOAT:
			buf[pos++] = 'f';
			break;
		case A_LONG:
			buf[pos++] = 'i';
			break;
		case A_SYM:
			buf[pos++] = 's';
			break;
		}
	}
	pos++;
	while(pos % 4){
		pos++;
	}
	for(i = 0; i < argc; i++){
		switch(argv[i].a_type){
		case A_FLOAT:
			{
				float f = atom_getfloat(argv + i);
				*((long *)(buf + pos)) = htonl(*(long *)(&f));
				pos += 4;
			}
			break;
		case A_LONG:
			*((long *)(buf + pos)) = htonl(atom_getlong(argv + i));
			pos += 4;
			break;
		case A_SYM:
			n = strlen(atom_getsym(argv + i)->s_name);
			memcpy(buf + pos, atom_getsym(argv + i)->s_name, n);
			pos += n + 1;
			while(pos % 4){
				pos++;
			}
			break;
		}
	}
	return pos;
}

void cmmjl_osc_fullPacket(void *x, long n, long ptr){
	//cmmjl_osc_parseFullPacket(x, (char *)ptr, n, true, cmmjl_osc_sendMsg);
	t_cmmjl_error (*parser)(void *, long, long, void (*)(void *, t_symbol *, int, t_atom *));
	t_cmmjl_error e;
	if(cmmjl_osc_should_schedule(x)){
		cmmjl_osc_schedule(x, n, ptr);
	}else{
		parser = cmmjl_obj_osc_parser_get(x);
		if(e = (*parser)(x, n, ptr, cmmjl_obj_osc_parser_cb_get(x))){
			// cmmjl_osc_parseFullPacket posts its own errors so just return
			return;
		}
	}
}

void cmmjl_osc_sendMsg(void *x, t_symbol *msg, int argc, t_atom *argv){
	t_linklist *ll = cmmjl_obj_osc_address_methods_get(x);
	t_symbol *m = gensym(basename(msg->s_name));
	char *osc_address;
	int i;
	method func;
	void *r;

	if(msg == ps_OSCTimeTag){
		return;
	}
	if(!ll){
		return;
	}
	osc_address = linklist_getindex(ll, 0);
	for(i = 0; i < linklist_getsize(ll); i++){
		if(!osc_address){
			post("no OSC address--breaking");
			break;
		}
		if(!cmmjl_osc_match(x, msg->s_name, osc_address)){
			func = zgetfn((t_object *)x, m);
			if(func){
				r = typedmess(x, m, argc, argv);
				//return;
			}
		}else{
		}
		linklist_next(ll, osc_address, (void **)&osc_address);
	}
	CMMJL_ERROR(x, CMMJL_ENOFUNC, "couldn't send message %s to object", m->s_name);
}

t_cmmjl_error cmmjl_osc_parseFullPacket(void *x, 
					long n, 
					long ptr, 
					void (*cbk)(void*, t_symbol*, int, t_atom*))
{
	t_cmmjl_obj *o = cmmjl_obj_get(x);
	if(!x){
		return CMMJL_ENOOBJ;
	}
	cmmjl_osc_parse(x, n, (char *)ptr, true, o->osc_parser_cb);
	return CMMJL_SUCCESS;
}


t_cmmjl_error cmmjl_osc_extract_messages(long n, 
					 char *buf,
					 bool topLevel,
					 void (*cbk)(t_cmmjl_osc_message msg, void *v), 
					 void *v)
{
	long size, messageLen, i;
	char *messageName;
	char *args;
	int t;
	t_cmmjl_error err;
	if(!cbk){
		return 0;
	}

	if ((n % 4) != 0) {
		//CMMJL_ERROR(x, CMMJL_OSC_ENO4BYTE, 
		//"packet size (%d) is not a multiple of 4 bytes: dropping", n);
		return CMMJL_OSC_ENO4BYTE;
	}
    
	if(n <= 0) {
		//CMMJL_ERROR(x, CMMJL_OSC_EUNDRFLW,
		//"bad OSC packet length: %d", n);
		return CMMJL_OSC_EUNDRFLW;
	}

	if(buf == NULL) {
		//CMMJL_ERROR(x, CMMJL_ENULLPTR, "OSC packet pointer is NULL");
		return CMMJL_ENULLPTR;
	}
             
	if ((n >= 8) && (strncmp(buf, "#bundle", 8) == 0)) {
		/* This is a bundle message. */
		if (n < 16) {
			//CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, 
			//"bundle is too small (%d bytes) for time tag", n);
			return CMMJL_OSC_EBADBNDL;
		}

		if (topLevel) {
			/*
			Atom timeTagLongs[2];
			SETLONG(&timeTagLongs[0], ntohl(*((long *)(buf+8))));
			SETLONG(&timeTagLongs[1], ntohl(*((long *)(buf+12))));
			cbk(x, ps_OSCTimeTag, 2, timeTagLongs);
			*/
		}

		i = 16; /* Skip "#bundle\0" and time tag */
		while((i+sizeof(long)) < n) { // next operation will take four bytes -aws
			size = ntohl(*((long *) (buf + i)));
			if ((size % 4) != 0) {
				//CMMJL_ERROR(x, CMMJL_OSC_EBNDLNO4, 
				//"bundle size (%d) is not a multiple of 4", size);
				return CMMJL_OSC_EBNDLNO4;
			}
			if ((size + i + 4) > n) {
				//CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, 
				//"bad OSC bundle size %d, only %d bytes left in entire bundle", 
				//size, n - i - 4);
				return CMMJL_OSC_EBADBNDL;
			}
	    
			/* Recursively handle element of bundle */
			t = cmmjl_osc_extract_messages(size, buf+i+4, false, cbk, v);
			if(t != 0) {
				//CMMJL_ERROR(x, CMMJL_FAILURE, 
				//"recursive processing of OSC packet failed.  Bailing out.");
				return CMMJL_FAILURE;
			}
			i += 4 + size;
		}
		if (i != n) {
			//CMMJL_ERROR(x, CMMJL_FAILURE, 
			//"failed to process entire packet (%d of %d bytes)", i, n);
			return CMMJL_FAILURE;
		}
	} else {
		/* This is not a bundle message */
		//messageName = buf;
		t_cmmjl_osc_message msg;
		msg.size = n;
		msg.address = buf;
		if(err = cmmjl_osc_dataAfterAlignedString(buf, buf+n, &args)){
			//CMMJL_ERROR(x, err, cmmjl_strerror(err));
			return err;
		}
		//messageLen = args-messageName;	    
		msg.typetags = args;

		if(err = cmmjl_osc_dataAfterAlignedString(msg.typetags, buf+n, &args)){
			//CMMJL_ERROR(x, err, cmmjl_strerror(err));
			return err;
		}
		msg.argv = args;
		args--;
		while(*args == '\0'){
			args--;
		}
		msg.argc = args - msg.typetags;
		cbk(msg, v);
	}
    
	if (topLevel) {
		//outlet_bang(x->O_outlet1);
	}
    
	return CMMJL_SUCCESS;
	/*
	  ParseOSCPacket_Error:
    
	  if (topLevel) {
	  //outlet_bang(x->O_outlet1);
	  }
	  return 1;

	*/
}

t_cmmjl_error cmmjl_osc_parse(void *x, 
			      long n, 
			      char *buf,
			      bool topLevel,
			      void (*cbk)(void *x, t_symbol *sym, int argc, t_atom *argv))
{
	long size, messageLen, i;
	char *messageName;
	char *args;
	int t;
	t_cmmjl_error err;
	if(!cbk){
		cbk = cmmjl_post_gimme;
	}

	if ((n % 4) != 0) {
		CMMJL_ERROR(x, CMMJL_OSC_ENO4BYTE, 
			    "packet size (%d) is not a multiple of 4 bytes: dropping", n);
		return CMMJL_OSC_ENO4BYTE;
	}
    
	if(n <= 0) {
		CMMJL_ERROR(x, CMMJL_OSC_EUNDRFLW,
			    "bad OSC packet length: %d", n);
		return CMMJL_OSC_EUNDRFLW;
	}

	/* your object is passing n in, so it should check this before the function is called.
	   if(n > x->b.size) {
	   post("OTUDP: OpenSoundControl n (%d) exceeds buffer size (%d)", n, x->b.size);
	   goto ParseOSCPacket_Error;
	   }
	*/
    
	if(buf == NULL) {
		CMMJL_ERROR(x, CMMJL_ENULLPTR, "OSC packet pointer is NULL");
		return CMMJL_ENULLPTR;
	}
             
	if ((n >= 8) && (strncmp(buf, "#bundle", 8) == 0)) {
		/* This is a bundle message. */
		if (n < 16) {
			CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, 
				    "bundle is too small (%d bytes) for time tag", n);
			return CMMJL_OSC_EBADBNDL;
		}

		if (topLevel) {
			Atom timeTagLongs[2];
			SETLONG(&timeTagLongs[0], ntohl(*((long *)(buf+8))));
			SETLONG(&timeTagLongs[1], ntohl(*((long *)(buf+12))));
			cbk(x, ps_OSCTimeTag, 2, timeTagLongs);
		}

		i = 16; /* Skip "#bundle\0" and time tag */
		while((i+sizeof(long)) < n) { // next operation will take four bytes -aws
			size = ntohl(*((long *) (buf + i)));
			if ((size % 4) != 0) {
				CMMJL_ERROR(x, CMMJL_OSC_EBNDLNO4, 
					    "bundle size (%d) is not a multiple of 4", size);
				return CMMJL_OSC_EBNDLNO4;
			}
			if ((size + i + 4) > n) {
				CMMJL_ERROR(x, CMMJL_OSC_EBADBNDL, 
					    "bad OSC bundle size %d, only %d bytes left in entire bundle", 
					    size, n - i - 4);
				return CMMJL_OSC_EBADBNDL;
			}
	    
			/* Recursively handle element of bundle */
			t = cmmjl_osc_parse(x, size, buf+i+4, false, cbk);
			if(t != 0) {
				CMMJL_ERROR(x, CMMJL_FAILURE, 
					    "recursive processing of OSC packet failed.  Bailing out.");
				return CMMJL_FAILURE;
			}
			i += 4 + size;
		}
		if (i != n) {
			CMMJL_ERROR(x, CMMJL_FAILURE, 
				    "failed to process entire packet (%d of %d bytes)", i, n);
			return CMMJL_FAILURE;
		}
	} else {
		/* This is not a bundle message */
		messageName = buf;
		if(err = cmmjl_osc_dataAfterAlignedString(messageName, buf+n, &args)){
			CMMJL_ERROR(x, err, cmmjl_strerror(err));
			return err;
		}
		
		messageLen = args-messageName;	    
		cmmjl_osc_formatMessage(x, messageName, (void *)args, n-messageLen, cbk);
	}
    
	if (topLevel) {
		//outlet_bang(x->O_outlet1);
	}
    
	return CMMJL_SUCCESS;
	/*
	  ParseOSCPacket_Error:
    
	  if (topLevel) {
	  //outlet_bang(x->O_outlet1);
	  }
	  return 1;

	*/
}

t_cmmjl_error cmmjl_osc_formatMessage(void *x, 
				      char *address, 
				      void *v, 
				      long n, 
				      void (*cbk)(void *x, t_symbol *msg, int argc, t_atom *argv))
{
	int i, j, k;
	float *floats;
	long *ints;
	char *chars;
	char *string, *nextString, *typeTags, *thisType;
	//unsigned char *p;
	char *p;
	Symbol *addressSymbol, *argSymbol;

	if(!cbk){
		CMMJL_ERROR(x, CMMJL_FAILURE,
			    "you must pass a callback to cmmjl_osc_formatMessage!");
		return CMMJL_FAILURE;
	}

#define MAXARGS 5000

	Atom args[MAXARGS];
	int numArgs = 0;
	Boolean tooManyArgs = false;
	t_cmmjl_error err;

	addressSymbol = gensym(address);

	/* Go through the arguments a word at a time */
	floats = v;
	ints = v;
	chars = v;

	//if (readTypeStrings && chars[0] == ',' && cmmjl_osc_isNiceString(chars, chars+n)) {
	if (chars[0] == ',' && !cmmjl_osc_isNiceString(chars, chars+n)) {
		/* Interpret the first string argument as a type string */
				
		typeTags = chars;
		if(err = cmmjl_osc_dataAfterAlignedString(chars, chars+n, &p)){
			CMMJL_ERROR(x, err, cmmjl_strerror(err));
			return err;
		}
		
		for (thisType = typeTags+1; *thisType != 0; ++thisType) {
  		   
			switch (*thisType) {
			case 'i': case 'r': case 'm': case 'c':
				SETLONG(&args[numArgs], ntohl(*((int *) p)));
				p += 4;
				break;

			case 'f': 
				{ // Pretend the 32 bits are an int so I can call ntohl()
					long bytesAsInt = ntohl(*((int *) p));
					SETFLOAT(&args[numArgs], *((float *) (& bytesAsInt)));
				}
				p += 4;
				break;

			case 't':
				/* handle typetags in args as they are in bundles */
				/* Could see if the data fits in a 32-bit int and output it like that if so... */
				SETSYM(&args[numArgs], ps_OSCTimeTag);
				numArgs++;
				SETLONG(&args[numArgs], ntohl(*((int *) p)));
				numArgs++;
				p += 4;
				SETLONG(&args[numArgs], ntohl(*((int *) p)));
				p += 4;
				break;
             
			case 'h': 
				/* 64-bit int: interpret as zero since Max doesn't have long ints */
				/* Could see if the data fits in a 32-bit int and output it like that if so... */
				SETLONG(&args[numArgs], 0);
				p += 8;
				break;

			case 'd':
				/* 64-bit float: interpret as zero since Max doesn't have doubles */
				/* Could see if the data fits in a 32-bit float and output it like that if so... */
				SETFLOAT(&args[numArgs], 0.0);
				p += 8;
				break;

			case 's': case 'S':
				if (cmmjl_osc_isNiceString(p, typeTags+n)) {
					SETSYM(&args[numArgs], gensym("¥Bogus_String"));
				} else {
					SETSYM(&args[numArgs], gensym(p));
					if(err = cmmjl_osc_dataAfterAlignedString(p, typeTags+n, &p)){
						CMMJL_ERROR(x, err, cmmjl_strerror(err));
						return err;
					}
				}
				break;
	            
			case 'b':
				{
					// output symbol OSCBlob, int size, int data0 ... int dataN
					int size = ntohl(*((int *) p));
                
					if (p+4+size > chars+n) {
						CMMJL_ERROR(x, CMMJL_OSC_EOVRFLW, 
							    "blob size %ld is too big for packet", 
							    size);
						return CMMJL_OSC_EOVRFLW;
					}
					SETSYM(&args[numArgs], ps_OSCBlob); numArgs++;
                
					if(numArgs + 1 + size > MAXARGS) {
						CMMJL_ERROR(x, CMMJL_OSC_EOVRFLW, 
							    "blob size too big for encoding");
						return CMMJL_OSC_EOVRFLW;
					}
					SETLONG(&args[numArgs], size); numArgs++;

					for(j = 0; j < size; j++) {
						SETLONG(&args[numArgs], ((long) (*(p + 4 + j)))); numArgs++;
					}
                
					numArgs--; // increments again at end of loop
                
					p += cmmjl_osc_effectiveBlobLength(size);

				}
				break;

			case 'T': 
				/* "True" value comes out as the int 1 */
				SETLONG(&args[numArgs], 1);
				/* Don't touch p */
				break;
	           	
			case 'F': 
				/* "False" value comes out as the int 0 */
				SETLONG(&args[numArgs], 0);
				/* Don't touch p */
				break;
	           	            
			case 'N': 
				/* Empty lists in max?  I wish!  How about the symbol "nil"? */
				SETSYM(&args[numArgs], gensym("nil"));
				/* Don't touch p */
				break;
	           	
			case 'I': 
				/* Infinita in Max?  Ha!  How about the symbol "Infinitum"? */
				SETSYM(&args[numArgs], gensym("Infinitum"));
				/* Don't touch p */
				break;


			default:
				CMMJL_ERROR(x, CMMJL_OSC_ETYPTAG, 
					    "Unrecognized type tag %c", *thisType);
				return CMMJL_OSC_ETYPTAG;
			}
			++numArgs;
		}
	} else {
		/* Use type-guessing heuristics */

		for (i = 0; i<n/4; ) {
			if (numArgs >= MAXARGS) {
				CMMJL_ERROR(x, CMMJL_OSC_EARGS, 
					    "message has more than %ld args; droping extra ones", 
					    (long)MAXARGS);
				return CMMJL_OSC_EARGS;
			}

			string = &chars[i*4];
			if  (ints[i] >= -1000 && ints[i] <= 1000000) {
				SETLONG(&args[numArgs], ntohl(ints[i]));
				i++;
			} else if (floats[i] >= -1000.f && floats[i] <= 1000000.f &&
				   (floats[i]<=0.0f || floats[i] >= __FLT_MIN__)) {
				// Pretend the 32 bits are an int so I can call ntohl()
				long bytesAsInt = ntohl(ints[i]);
				SETFLOAT(&args[numArgs], *((float *) (&bytesAsInt)));
				i++;
			} else if (!cmmjl_osc_isNiceString(string, chars+n)) {
				if(err = cmmjl_osc_dataAfterAlignedString(string, chars+n, &nextString)){
					CMMJL_ERROR(x, err, cmmjl_strerror(err));
					return err;
				}
				argSymbol = gensym(string);
				SETSYM(&args[numArgs], argSymbol);
				i += (nextString-string) / 4;
			} else {
				// Assume int if nothing looks good.
				SETLONG(&args[numArgs], ntohl(ints[i]));
				i++;
			}
			numArgs++;
		}
	}
	cbk(x, addressSymbol, numArgs, args);
	return CMMJL_SUCCESS;
}

t_cmmjl_error cmmjl_osc_dataAfterAlignedString(char *string, char *boundary, char **result){
	/* The argument is a block of data beginning with a string.  The
	   string has (presumably) been padded with extra null characters
	   so that the overall length is a multiple of STRING_ALIGN_PAD
	   bytes.  Return a pointer to the next byte after the null
	   byte(s).  The boundary argument points to the character after
	   the last valid character in the buffer---if the string hasn't
	   ended by there, something's wrong.

	   If the data looks wrong, return 0, and set htm_error_string */

	int i;
    
	if ((boundary - string) %4 != 0) {
		result = 0;
		return CMMJL_OSC_EBADALIGN;
	}
    
	// note the above test doesn't catch this possibility and the next test dereferences string[0] resulting in possible crash -aws
	if ((boundary - string) == 0) { 
		result = 0;
		return CMMJL_ENODATA;
	}

	for (i = 0; string[i] != '\0'; i++) {
		if (string + i >= boundary) {
			result = 0;
			return CMMJL_OSC_EOVRFLW;
		}
	}

	/* Now string[i] is the first null character */
	i++;

	for (; (i % CMMJL_OSC_STRING_ALIGN_PAD) != 0; i++) {
		if (string + i >= boundary) {
			result = 0;
			return CMMJL_OSC_EOVRFLW;
		}
		if (string[i] != '\0') {
			result = 0;
			return CMMJL_OSC_EBADALIGN;
		}
	}
	*result = string + i;
	return CMMJL_SUCCESS;
}

t_cmmjl_error cmmjl_osc_isNiceString(char *string, char *boundary)  {
	/* Arguments same as cmmjl_osc_dataAfterAlignedString().  Is the given "string"
	   really a string?  I.e., is it a sequence of isprint() characters
	   terminated with 1-4 null characters to align on a 4-byte boundary? */

	int i;

	if ((boundary - string) % 4 != 0) {
		return CMMJL_OSC_EBADALIGN;
	}
    
	if ((boundary - string) == 0) { 
		return CMMJL_ENODATA;
	}

	for (i = 0; string[i] != '\0'; i++) {
		if (!isprint(string[i])) return FALSE;
		if (string + i >= boundary) return FALSE;
	}

	/* If we made it this far, it's a null-terminated sequence of printing characters 
	   in the given boundary.  Now we just make sure it's null padded... */

	/* Now string[i] is the first null character */
	i++;
	for (; (i % CMMJL_OSC_STRING_ALIGN_PAD) != 0; i++) {
		if (string[i] != '\0') return FALSE;
	}

	return CMMJL_SUCCESS;
}

bool cmmjl_osc_isFinalPathSegment(char *path){
	return !strcmp(dirname(path), "/");
}

int cmmjl_osc_effectiveBlobLength(int blobDataSize){
	int len = 4 + blobDataSize;  /* int32 size count. */
	/* Round up len to next multiple of STRING_ALIGN_PAD to account for alignment padding */
	if ((len % CMMJL_OSC_STRING_ALIGN_PAD) != 0) {
		len += CMMJL_OSC_STRING_ALIGN_PAD - (len % CMMJL_OSC_STRING_ALIGN_PAD);
	}
	return len;
}

int cmmjl_osc_get_tree_length(int len, char *msg){
	int i;
	int count = 0;
	for(i = 0; i < len; i++){
		if(msg[i] == '/'){
			count++;
		}
	}
	if(msg[len - 1] == '/'){
		count--;
	}
	return count;
}

void cmmjl_osc_address_to_array(int len, char *msg, char **ar, int *lengths){
	int i;
	int count = 0;
	int plen = 0;
	for(i = 0; i < len - 1; i++){
		if(msg[i] == '/'){
			if(i > 0){
				lengths[count - 1] = plen - 1;
				plen = 0;
			}
			ar[count++] = msg + i + 1;
		}
		plen++;
	}
	lengths[count - 1] = plen;
}

void cmmjl_osc_atom2maxatom(t_cmmjl_osc_atom *a, t_atom *ma){
	switch(a->a_type){
	case CMMJL_CHAR:
		atom_setlong(ma, (long)(a->a_w.w_char));
		break;
	case CMMJL_UCHAR:
		atom_setlong(ma, (long)(a->a_w.w_uchar));
		break;
	case CMMJL_SHORT:
		atom_setlong(ma, (long)(a->a_w.w_short));
		break;
	case CMMJL_USHORT:
		atom_setlong(ma, (long)(a->a_w.w_ushort));
		break;
	case CMMJL_LONG:
		atom_setlong(ma, (long)(a->a_w.w_long));
		break;
	case CMMJL_ULONG:
		atom_setlong(ma, (long)(a->a_w.w_ulong));
		break;
	case CMMJL_LLONG:
		atom_setlong(ma, (long)(a->a_w.w_llong));
		break;
	case CMMJL_ULLONG:
		atom_setlong(ma, (long)(a->a_w.w_ullong));
		break;
	case CMMJL_FLOAT:
		atom_setfloat(ma, (float)(a->a_w.w_float));
		break;
	case CMMJL_DOUBLE:
		atom_setfloat(ma, (float)(a->a_w.w_double));
		break;
	case CMMJL_LDOUBLE:
		atom_setfloat(ma, (float)(a->a_w.w_ldouble));
		break;
	case CMMJL_PTR:
		switch(a->osc_typetag){
		case 'b':
			atom_setlong(ma, (long)(a->a_w.w_ptr));
			break;
		case 's':
			atom_setsym(ma, gensym(a->a_w.w_ptr));
			break;
		}
		break;	
	case CMMJL_TIMETAG:
		// maybe a string here?
		//atom_setlong(ma, (long)(a->a_w.w_char));
		break;
	}
}

void cmmjl_osc_get_data_atoms(t_cmmjl_osc_message *msg, t_atom *atoms){
	t_cmmjl_osc_atom oa[msg->argc];
	cmmjl_osc_get_data(msg, oa);
	int i;
	for(i = 0; i < msg->argc; i++){
		cmmjl_osc_atom2maxatom(oa + i, atoms + i);
	}
}

void cmmjl_osc_get_data(t_cmmjl_osc_message *msg, t_cmmjl_osc_atom *a){
	int i;
	char *tt = msg->typetags + 1;
	int argoffset = 0;
	for(i = 0; i < msg->argc; i++){
		switch(*tt++){
		case 'i':
			argoffset += cmmjl_osc_atom_setlong(a + i, msg->argv + argoffset);
			cmmjl_osc_atom_settypetag(a + i, 'i');
			break;
		case 'f':
			argoffset += cmmjl_osc_atom_setfloat(a + i, msg->argv + argoffset);
			cmmjl_osc_atom_settypetag(a + i, 'f');
			break;
		case 's':
			argoffset += cmmjl_osc_atom_setptr(a + i, msg->argv + argoffset);
			cmmjl_osc_atom_settypetag(a + i, 's');
			break;
		case 'b':
			argoffset += cmmjl_osc_atom_setptr(a + i, msg->argv + argoffset + 4); // blob has a 4 byte length at the beg.
			argoffset += 4; // length
			cmmjl_osc_atom_settypetag(a + i, 'b');
			break;
		case 'T':
			{
				char c = 1;
				cmmjl_osc_atom_setchar(a + i, &c);
				cmmjl_osc_atom_settypetag(a + i, 'T');
			}
			break;
		case 'F':
			{
				char c = 0;
				cmmjl_osc_atom_setchar(a + i, &c);
				cmmjl_osc_atom_settypetag(a + i, 'F');
			}
			break;
		case 'N':

			break;
		case 'I':

			break;
		case 't':

			break;

		}
	}
}

int cmmjl_osc_atom_setchar(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_char = *ptr;
	a->a_type = CMMJL_CHAR;
	a->nbytes = sizeof(char);
	a->npaddingbytes = 0;
	return a->nbytes;
}

char cmmjl_osc_atom_getchar(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (char)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (char)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (char)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (char)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (char)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (char)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (char)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (char)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (char)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (char)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (char)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setuchar(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_uchar = *((unsigned char *)ptr);
	a->a_type = CMMJL_UCHAR;
	a->nbytes = sizeof(unsigned char);
	a->npaddingbytes = 0;
	return a->nbytes;
}

unsigned char cmmjl_osc_atom_getuchar(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (unsigned char)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (unsigned char)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (unsigned char)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (unsigned char)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (unsigned char)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (unsigned char)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (unsigned char)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (unsigned char)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (unsigned char)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (unsigned char)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (unsigned char)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setshort(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_short = ntohs(*((short *)ptr));
	a->a_type = CMMJL_SHORT;
	a->nbytes = sizeof(short);
	a->npaddingbytes = 0;
	return a->nbytes;
}

short cmmjl_osc_atom_getshort(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (short)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (short)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (short)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (short)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (short)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (short)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (short)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (short)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (short)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (short)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (short)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}


int cmmjl_osc_atom_setushort(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_ushort = ntohs(*((unsigned short *)ptr));
	a->a_type = CMMJL_USHORT;
	a->nbytes = sizeof(unsigned short);
	a->npaddingbytes = 0;
	return a->nbytes;
}

unsigned short cmmjl_osc_atom_getushort(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (unsigned short)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (unsigned short)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (unsigned short)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (unsigned short)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (unsigned short)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (unsigned short)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (unsigned short)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (unsigned short)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (unsigned short)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (unsigned short)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (unsigned short)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setlong(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_long = ntohl(*((long *)ptr));
	a->a_type = CMMJL_LONG;
	a->nbytes = sizeof(long);
	a->npaddingbytes = 0;
	return a->nbytes;
}

long cmmjl_osc_atom_getlong(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (long)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (long)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (long)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (long)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (long)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (long)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (long)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (long)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (long)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (long)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (long)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setulong(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_ulong = ntohl(*((unsigned long *)ptr));
	a->a_type = CMMJL_ULONG;
	a->nbytes = sizeof(unsigned long);
	a->npaddingbytes = 0;
	return a->nbytes;
}

unsigned long cmmjl_osc_atom_getulong(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (unsigned long)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (unsigned long)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (unsigned long)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (unsigned long)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (unsigned long)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (unsigned long)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (unsigned long)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (unsigned long)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (unsigned long)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (unsigned long)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (unsigned long)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}


int cmmjl_osc_atom_setllong(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_llong = ntohll(*((long long *)ptr));
	a->a_type = CMMJL_LLONG;
	a->nbytes = sizeof(long long);
	a->npaddingbytes = 0;
	return a->nbytes;
}

long long cmmjl_osc_atom_getllong(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (long long)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (long long)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (long long)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (long long)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (long long)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (long long)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (long long)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (long long)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (long long)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (long long)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (long long)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}


int cmmjl_osc_atom_setullong(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_ullong = ntohll(*((unsigned long long *)ptr));
	a->a_type = CMMJL_ULLONG;
	a->nbytes = sizeof(unsigned long long);
	a->npaddingbytes = 0;
	return a->nbytes;
}

unsigned long long cmmjl_osc_atom_getullong(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (unsigned long long)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (unsigned long long)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (unsigned long long)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (unsigned long long)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (unsigned long long)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (unsigned long long)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (unsigned long long)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (unsigned long long)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (unsigned long long)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (unsigned long long)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (unsigned long long)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}


int cmmjl_osc_atom_setfloat(t_cmmjl_osc_atom *a, char *ptr){
	long l = htonl(*((long *)ptr));
	a->a_w.w_float = *((float *)&l);
	a->a_type = CMMJL_FLOAT;
	a->nbytes = sizeof(float);
	a->npaddingbytes = 0;
	return a->nbytes;
}

float cmmjl_osc_atom_getfloat(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (float)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (float)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (float)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (float)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (float)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (float)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (float)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (float)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (float)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (float)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (float)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setdouble(t_cmmjl_osc_atom *a, char *ptr){
	long long l = htonll(*((long *)ptr));
	a->a_w.w_double = *((double *)&l);
	a->a_type = CMMJL_DOUBLE;
	a->nbytes = sizeof(double);
	a->npaddingbytes = 0;
	return a->nbytes;
}

double cmmjl_osc_atom_getdouble(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (double)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (double)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (double)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (double)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (double)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (double)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (double)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (double)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (double)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (double)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (double)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setldouble(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_ldouble = *((long double *)ptr);
	a->a_type = CMMJL_LDOUBLE;
	a->nbytes = sizeof(long double);
	a->npaddingbytes = 0;
	return a->nbytes;
}

long double cmmjl_osc_atom_getldouble(t_cmmjl_osc_atom *a){
	switch(a->a_type){
	case CMMJL_CHAR:
		return (long double)(a->a_w.w_char);
	case CMMJL_UCHAR:
		return (long double)(a->a_w.w_uchar);
	case CMMJL_SHORT:
		return (long double)(a->a_w.w_short);
	case CMMJL_USHORT:
		return (long double)(a->a_w.w_ushort);
	case CMMJL_LONG:
		return (long double)(a->a_w.w_long);
	case CMMJL_ULONG:
		return (long double)(a->a_w.w_ulong);
	case CMMJL_LLONG:
		return (long double)(a->a_w.w_llong);
	case CMMJL_ULLONG:
		return (long double)(a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return (long double)(a->a_w.w_float);
	case CMMJL_DOUBLE:
		return (long double)(a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return (long double)(a->a_w.w_ldouble);
	default:
		return 0;
	}
}

int cmmjl_osc_atom_setptr(t_cmmjl_osc_atom *a, char *ptr){
	a->a_w.w_ptr = ptr;
	a->a_type = CMMJL_PTR;
	a->nbytes = 0;
	a->npaddingbytes = 0;
	int i = 0;
	while(*(ptr + i) != '\0'){
		//post("%d %c 0x%x", i, ptr[i], ptr[i]);
		i++;
		a->nbytes++;
	}
	i++;
	a->npaddingbytes++;
	while(i % 4){
		//post("%d %c 0x%x", i, ptr[i], ptr[i]);
		i++;
		a->npaddingbytes++;
	}
	//post("%d bytes", a->nbytes + a->npaddingbytes);
	return a->nbytes + a->npaddingbytes;
}

char *cmmjl_osc_atom_getptr(t_cmmjl_osc_atom *a){
	return a->a_w.w_ptr;;
}

void cmmjl_osc_atom_settypetag(t_cmmjl_osc_atom *a, char tt){
	a->osc_typetag = tt;
}

char cmmjl_osc_atom_gettypetag(t_cmmjl_osc_atom *a){
	return a->osc_typetag;
}

int cmmjl_osc_atom_tostring(t_cmmjl_osc_atom *a, char *buf){
	switch(a->a_type){
	case CMMJL_CHAR:
		return sprintf(buf, "%c", a->a_w.w_char);
	case CMMJL_UCHAR:
		return sprintf(buf, "%c", a->a_w.w_uchar);
	case CMMJL_SHORT:
		return sprintf(buf, "%hd", a->a_w.w_short);
	case CMMJL_USHORT:
		return sprintf(buf, "%hud", a->a_w.w_ushort);
	case CMMJL_LONG:
		return sprintf(buf, "%ld", a->a_w.w_long);
	case CMMJL_ULONG:
		return sprintf(buf, "%lud", a->a_w.w_ulong);
	case CMMJL_LLONG:
		return sprintf(buf, "%lld", a->a_w.w_llong);
	case CMMJL_ULLONG:
		return sprintf(buf, "%llud", a->a_w.w_ullong);
	case CMMJL_FLOAT:
		return sprintf(buf, "%f", a->a_w.w_float);
	case CMMJL_DOUBLE:
		return sprintf(buf, "%f", a->a_w.w_double);
	case CMMJL_LDOUBLE:
		return sprintf(buf, "%Lf", a->a_w.w_ldouble);
	case CMMJL_PTR:
		return sprintf(buf, "%p", a->a_w.w_ptr);
	case CMMJL_TIMETAG:
		// should format in some human readable form
		return 0;
	}
	return 0;
}
