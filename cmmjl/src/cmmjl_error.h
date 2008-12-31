/** @file cmmjl_error.h
	Error handling macros and functions.
	@authors John MacCallum and Andy Schmeder, The Center for New Music and Audio Technologies, University of California, Berkeley.
	\defgroup Error Error reporting
@{
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
/** 	This macro simply calls the cmmjl_error function with the 
	name, file, function, and line of the error so you only need to 
	specify the error code, the reason as a printf-style format string,
	and any additional arguments to be formatted.

	@param 	code		The error code (see t_cmmjl_error)
	@param 	reason_fmt 	A string describing the error encountered. 
				This can be a printf-style format string.
	@param 	args  		Optional arguments to be formatted into the 
		       		reason_fmt string.
	@see	t_cmmjl_error
	@see	cmmjl_errno.h
*/
#define CMMJL_ERROR(code, reason_fmt, args...) \
	cmmjl_error(NAME, FILE, __FUNCTION__, __LINE__, code, reason_fmt, ##args) 
#endif

#ifndef PINFO
/** 	Print info--functions like post, but prepends the object name to the string.

	@param	str	printf-style format string
	@param 	args	arguments to be substituted into the string.
*/
#define PINFO(str, args...) post(NAME ": " str, ##args)
#endif

#ifndef PERROR
/** 	Print an error string with the filename and line number where 
	the error occured.  Has the same signature as post() or printf().

	@param	str	printf-style format string
	@param  args	arguments to be substituted into the string.
*/
#define PERROR(str, args...) error("%s:%s:%s%d: " str, NAME, FILE, __FUNCTION__, __LINE__, ##args)
#endif

#ifdef DEBUG
#ifndef PDEBUG
/** 	Print debugging info--functions like post, but prepends 
	the object name to the string.

	@param	str	printf-style format string
	@param 	args	arguments to be substituted into the string.
*/
#define PDEBUG(str, args...) post(NAME ": " str, ##args)
#endif
#else
#ifndef PDEBUG
/** 	Debugging is disabled--this does nothing. */
#define PDEBUG(str, args...)
#endif
#endif

#ifdef CMMJL_DEBUG
#ifndef CMMJL_PDEBUG
/** 	Print debugging info.  Exactly like PDEBUG, but for internal
	use in debugging the library.

	@param	str	printf-style format string
	@param 	args	arguments to be substituted into the string.
*/
#define CMMJL_PDEBUG(str, args...) post("cmmjl: " str, ##args)
#endif
#else
#ifndef CMMJL_PDEBUG
/** 	Debugging is disabled--this does nothing. */
#define CMMJL_PDEBUG(str, args...)
#endif
#endif

/** 	Error reporting callback.  This can be set to point to 
  	cmmjl_default_error_handler or cmmjl_no_error_handler, using
	cmmjl_set_default_error_handler() and cmmjl_set_no_error_handler()
	respectively, or it can be set to a user-defined callback
	with cmmjl_set_error_handler.
*/
extern void (*cmmjl_error_callback)(const char *objname, 
				    const char *filename, 
				    const char *function, 
				    int line, 
				    int code, 
				    char *reason_fmt);

/**	The error function that should be called to report an error.  
	This function calls vsprintf() to format any optional arguments
	and then calls cmmjl_error_callback.
	
	@param	objname		The name of the object.
	@param	filename	The name of the file in which the error occured.
	@param 	function	The name of the function in which the error occured.
	@param  line		The line number where the error occured.
	@param	code		The error code.
	@param	reason_fmt	A printf-style format string describing the error.
	@param	...		Any optional arguments to be formatted into the 
				error description.

	@see	t_cmmjl_error
	@see	cmmjl_errno.h
*/
void cmmjl_error(const char *objname, 
		 const char *filename, 
		 const char *function, 
		 int line, 
		 int code, 
		 const char *reason_fmt,
		 ...);

/**	The default error handler.  This calls error() (defined in ext.h) to 
	post an error in the following format: 
	objname:filename:function:line: reason (code)

	@param	objname		The name of the object.
	@param	filename	The name of the file in which the error occured.
	@param 	function	The name of the function in which the error occured.
	@param  line		The line number where the error occured.
	@param	code		The error code.
	@param	reason		A printf-style format string describing the error.

	@see	t_cmmjl_error
	@see	cmmjl_errno.h
 */
void cmmjl_default_error_handler(const char *objname, 
				 const char *filename, 
				 const char *function, 
				 int line, 
				 int code, 
				 char *reason);

/**	A dummy function which does nothing.  cmmjl_error_callback points to this
	when error reporting is turned off with cmmjl_set_no_error_handler().

	@param	objname		The name of the object.
	@param	filename	The name of the file in which the error occured.
	@param 	function	The name of the function in which the error occured.
	@param  line		The line number where the error occured.
	@param	code		The error code.
	@param	reason		A printf-style format string describing the error.

	@see	t_cmmjl_error
	@see	cmmjl_errno.h
 */
void cmmjl_no_error_handler(const char *objname, 
			    const char *filename, 
			    const char *function, 
			    int line, 
			    int code, 
			    char *reason);

/** 	This function can be used to set cmmjl_error_callback to a user-defined
	callback.

	@param	cbk	The callback.

	@see	cmmjl_error_callback
*/
void cmmjl_set_error_handler(void (*cbk)(const char *objname,
					 const char *filename,
					 const char *function,
					 int line,
					 int code,
					 char *reason));

/**	This function sets cmmjl_error_callback to point to cmmjl_default_error_handler()

	@see cmmjl_default_error_handler()
 */
void cmmjl_set_default_error_handler(void);

/**	This function turns error reporting off by setting cmmjl_error_callback to 
	a function which does nothing.

	@see	cmmjl_error_callback
*/
void cmmjl_set_no_error_handler(void);

/**	Get the string description associated with the error code.
	
	@param	errno	The error code.

	@returns	The string associated with the error code.
 */
const char *cmmjl_strerror(const t_cmmjl_error errno); /* defined in cmmjl_strerror.c */

#endif

/*@}*/
