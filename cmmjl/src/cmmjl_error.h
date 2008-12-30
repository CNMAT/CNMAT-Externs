/** @file cmmjl_error.h
	Error handling macros, functions, and codes.
	@authors John MacCallum and Andy Schmeder, The Center for New Music and Audio Technologies, University of California, Berkeley.
*/
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

Written by John MacCallum and Andy Schmeder, The Center for New Music and 
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

#ifndef __CMMJL_ERROR_H__
#define __CMMJL_ERROR_H__

#include "ext.h"
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef __CMMJL_ERRNO_H__
#include "cmmjl_errno.h"
#endif

#ifndef FILE
#define FILE basename(__FILE__)
#endif

#ifndef CMMJL_ERROR
#define CMMJL_ERROR(code, reason_fmt, args...) \
	cmmjl_error(NAME, FILE, __FUNCTION__, __LINE__, code, reason_fmt, ##args) 
#endif

#ifndef PINFO
/** Print info--functions like post, but prepends the object name to the string.
	@param	str	printf-style format string
	@param 	...	arguments to be substituted into the string.
*/
#define PINFO(str, args...) post(NAME ": " str, ##args)
#endif

#ifndef PERROR
/** Print an error string with the filename and line number where the error occured.  Has the same signature as post() or printf().
	@param	str	printf-style format string
	@param  ...	arguments to be substituted into the string.
*/
#define PERROR(str, args...) error(NAME ": ERROR at %s:%d: " str, FILE, __LINE__, ##args)
#endif

#ifdef DEBUG
#ifndef PDEBUG
/** Print debugging info--functions like post, but prepends the object name to the string.
	@param	str	printf-style format string
	@param 	...	arguments to be substituted into the string.
*/
#define PDEBUG(str, args...) post(NAME ": " str, ##args)
#endif
#else
#ifndef PDEBUG
/** Debugging is disabled--this does nothing. */
#define PDEBUG(str, args...)
#endif
#endif

#ifdef CMMJL_DEBUG
#ifndef CMMJL_PDEBUG
/** Print debugging info.  Exactly like PDEBUG, but for internal use in debugging the library.
	@param	str	printf-style format string
	@param 	...	arguments to be substituted into the string.
*/
#define CMMJL_PDEBUG(str, args...) post("cmmjl: " str, ##args)
#endif
#else
#ifndef CMMJL_PDEBUG
/** Debugging is disabled--this does nothing. */
#define CMMJL_PDEBUG(str, args...)
#endif
#endif

void (*cmmjl_error_callback)(const char *objname, 
			     const char *filename, 
			     const char *function, 
			     int line, 
			     int code, 
			     char *st);
void cmmjl_default_error_handler(const char *objname, 
				 const char *filename, 
				 const char *function, 
				 int line, 
				 int code, 
				 char *st);
void cmmjl_error(const char *objname, 
		 const char *filename, 
		 const char *function, 
		 int line, 
		 int code, 
		 const char *st,
		 ...);

const char *cmmjl_strerror(const int cmmjl_errno); /* defined in cmmjl_strerror.c */

#endif
