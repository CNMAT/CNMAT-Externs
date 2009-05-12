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

	name = object_classname(x);
	sprintf(buf, "%s/%s/%d", buf2, name->s_name, instance);

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

	// get rid of me and just use the length passed to the function
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

