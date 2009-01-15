/** @file cmmjl_obj.h
	Object extensions.  Here we have data structures and functions
	for working with extra instance data.
	@author John MacCallum
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

#ifndef __CMMJL_OBJ_H__
#define __CMMJL_OBJ_H__

#include "cmmjl_error.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_hashtab.h"
#include "ext_linklist.h"

#ifndef CMMJL_CREATE_INFO_OUTLET
/** 	Pass this to cmmjl_init to create a default info outlet */
#define CMMJL_CREATE_INFO_OUTLET true
#endif

#ifndef CMMJL_DONT_CREATE_INFO_OUTLET
/** 	Pass this to cmmjl_init to suppress creation of an info outlet */
#define CMMJL_DONT_CREATE_INFO_OUTLET false
#endif

extern t_hashtab *_cmmjl_obj_tab;
extern t_hashtab *_cmmjl_instance_count;

#ifndef CMMJL_CLASS_ADDMETHOD
/**	This macro can be used to add a message to your object and
	at the same time add an alias which has the same name with
	a slash prepended to it for use with OSC-style messages.
	For example, if you want your object to respond to a message
	"foo", it will also respond to "/foo".  This macro has exactly
	the same signature as class_addmethod() found in the Max SDK
	and can be used in place of it.

	@param	c	Your object's class--this should be instantiated
			by calling class_new().
	@param	func	The function that will be called in response to
			the message.
	@param  msg	The message name.
	@param	args	The function signature.
*/
#define CMMJL_CLASS_ADDMETHOD(c, func, msg, args...)	\
	do{					\
	char buf[64];				\
	sprintf(buf, "/%s", msg);		\
	class_addmethod(c, func, msg, ##args);	\
	class_addmethod(c, func, buf, ##args); \
	} while(0)
#endif

/** 	Internal object used to store any data that will be used by the lib. */
typedef struct _cmmjl_obj{
	void (*error)(const char *objname, 
		      const char *file, 
		      const char *func, 
		      int line, 
		      t_cmmjl_error code, 
		      char *reason_fmt); /**< The object's error handler */
	void *info_outlet; /**< A pointer to the object's info_outlet */
	t_hashtab *entrance_count_tab; /**< Hashtab to keep track of entrance counts in functions */
	t_symbol *osc_address; /**< the object's varname (scripting name) as an OSC address. */
	t_linklist *osc_address_methods; /**< a list of all the OSC messages this obj understands*/
	long instance; /**< a unique number identifying which instance this obj is */
} t_cmmjl_obj;

t_hashtab *_cmmjl_obj_tab;
t_hashtab *_cmmjl_instance_count;

/**	Get the t_cmmjl_obj associated with your object (x).
	@param	x	Your object.
	@returns	A pointer to the t_cmmjl_obj associated with your object.
*/
t_cmmjl_obj *cmmjl_obj_get(void *x);

/** 	@cond
	Initializes an object used by the library to store things like the error
	handler, the info outlet, etc.

	@param	x		The object
	@param 	o		The t_cmmjl_obj object
	@param	name		The name of the object
	@param 	instance	The instance number

	@returns	An error or CMMJL_SUCCESS
*/
t_cmmjl_error cmmjl_obj_init(void *x, 
			     t_cmmjl_obj *o, 
			     bool shouldCreateInfoOutlet, 
			     const char *name,
			     long instance);
/* 	@endcond */


/**	Returns a pointer to the info outlet.
	@param 	x	Your object.
	@returns	A pointer to the info outlet.
*/
void *cmmjl_obj_info_outlet_get(void *x);

/**	Set the info outlet.
	@param	x	Your object.
	@param	outlet	A pointer to the outlet.
*/
void cmmjl_obj_info_outlet_set(void *x, void *outlet);

/**	Get the instance number of your object.  
	@param	x	Your object
	@returns	The instance number or -1 if there was an error.
*/
long cmmjl_obj_getInstance(void *x);

#endif // __CMMJL_OBJ_H__
