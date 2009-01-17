/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and
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

#include "ext.h"
#include "ext_obex.h"
#include "ext_hashtab.h"
#include "cmmjl.h"
#include "cmmjl_errno.h"
#include "cmmjl_error.h"
#include <stdarg.h>
#include <stdio.h>

extern t_hashtab *_cmmjl_obj_tab;

void cmmjl_error(void *x,
		 const char *objname, 
		 const char *filename, 
		 const char *function, 
		 int line, 
		 t_cmmjl_error code, 
		 const char *reason_fmt,
		 ...)
{
	if(!_cmmjl_obj_tab){
		return;
	}
	t_object *val;
	hashtab_lookup(_cmmjl_obj_tab, x, &val);
	va_list ap;
	va_start(ap, reason_fmt);
	char buf[256];
	vsprintf(buf, reason_fmt, ap);
	va_end(ap);
	((t_cmmjl_obj *)val)->error(objname, filename, function, line, code, buf);
}

void cmmjl_default_error_handler(const char *objname, 
				 const char *filename, 
				 const char *function, 
				 int line, 
				 t_cmmjl_error code, 
				 char *reason)
{
	if(code & CMMJL_EMASK_ALL_REG){
		error("%s: %s: %s: %d: %s (0x%llx)", objname, filename, function, line, reason, code);
	}
}

void cmmjl_verbose_error_handler(const char *objname, 
				 const char *filename, 
				 const char *function, 
				 int line, 
				 t_cmmjl_error code, 
				 char *reason)
{
	error("%s: %s: %s: %d: %s (0x%llx)", objname, filename, function, line, reason, code);
}

void cmmjl_no_error_handler(const char *objname,
			    const char *filename,
			    const char *function,
			    int line,
			    t_cmmjl_error code,
			    char *reason)
{}

void *cmmjl_get_error_handler(void *x){
	if(!_cmmjl_obj_tab){
		return NULL;
	}
	t_object *o;
	t_max_err err;
	if(err = hashtab_lookup(_cmmjl_obj_tab, x, &o)){
		return NULL;
	}
	return ((t_cmmjl_obj *)o)->error;
}

t_cmmjl_error cmmjl_set_error_handler(void *x,
				      void (*cbk)(const char *objname,
						  const char *filename,
						  const char *function,
						  int line,
						  t_cmmjl_error code,
						  char *reason))
{
	if(!_cmmjl_obj_tab){
		return CMMJL_ENOOBJ;
	}
	t_object *o;
	t_max_err err;
	if(err = hashtab_lookup(_cmmjl_obj_tab, x, &o)){
		return CMMJL_ENOOBJ;
	}
	((t_cmmjl_obj *)o)->error = cbk;
}

t_cmmjl_error cmmjl_set_default_error_handler(void *x){
	cmmjl_set_error_handler(x, cmmjl_default_error_handler);
}

t_cmmjl_error cmmjl_set_verbose_error_handler(void *x){
	cmmjl_set_error_handler(x, cmmjl_verbose_error_handler);
}

t_cmmjl_error cmmjl_set_no_error_handler(void *x){
	cmmjl_set_error_handler(x, cmmjl_no_error_handler);
}
