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

/** 	\file osc_bundle_u.h
	\author John MacCallum
	\brief Utilities for manipulating userialized OSC bundles


*/

#ifndef __OSC_BUNDLE_U_H__
#define __OSC_BUNDLE_U_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "osc_message_u.h"
#include "osc_error.h"

typedef struct _osc_bundle_u t_osc_bundle_u, t_osc_bndl_u;

t_osc_bndl_u *osc_bundle_u_alloc(void);
int osc_bundle_u_getMsgCount(t_osc_bndl_u *bndl);
t_osc_err osc_bundle_u_getMessagesWithCallback(t_osc_bndl_u *bndl, void (*f)(t_osc_msg_u*, void *), void *context);
t_osc_err osc_bundle_u_lookupAddress(t_osc_bndl_u *bndl, char *address, int *nmatches, t_osc_msg_u ***m, int fullmatch);
t_osc_err osc_bundle_u_format(t_osc_bndl_u *bndl, long *buflen, char **buf);

#ifdef __cplusplus
}
#endif

#endif // __OSC_BUNDLE_U_H__