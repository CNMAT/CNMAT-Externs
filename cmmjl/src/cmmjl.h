/** @file cmmjl.h
	Basic common functions, defines, and initialization.
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

#include "cmmjl_obj.h"
#include "cmmjl_error.h"
#include "cmmjl_commonsymbols.h"
#include "cmmjl_types.h"
#include "cmmjl_profile.h"

#ifndef __CMMJL_H__
#define __CMMJL_H__

/** 	@cond
	These should be defined in float.h, but just in case...
*/
#ifndef __FLT_MIN__
#define __FLT_MIN__ 1.17549435e-38F
#endif

#ifndef __FLT_MAX__
#define __FLT_MAX__ 3.40282347e+38F
#endif

#ifndef __DBL_MIN__
#define __DBL_MIN__ 2.2250738585072014e-308
#endif

#ifndef __DBL_MAX__
#define __DBL_MAX__ 1.7976931348623157e+308
#endif
/** @endcond */

/** @cond */
#ifndef NAME
#define NAME "cmmjl"
#endif
/** @endcond */


/** 	Initializes the library.  This must be called before the library is used.  

	@param	x	A pointer to your object.
	@param  name	The name of your object.
	@param	flags	A bitfield of flags.  See cmmjl_obj.h

	@returns	An error code or 0 on success.
	@see cmmjl_obj.h
 */
t_cmmjl_error cmmjl_init(void *x, const char *name, unsigned long flags);

/**	Free the resources used by an object

	@param	x	The object
*/
void cmmjl_free(void *x);

/**	Posts the arguments of an A_GIMME message

	@param	x	The object.  This function posts the pointer address.
	@param 	msg	The message.
	@param 	argc	The number of arguments.
	@param	argv	The arguments.
*/
void cmmjl_post_gimme(void *x, t_symbol *msg, int argc, t_atom *argv);

#endif
