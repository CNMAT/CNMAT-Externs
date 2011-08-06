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

/** 	\file osc_bundle_s.h
	\author John MacCallum
	\brief Utilities for manipulating serialized OSC bundles


*/

#ifndef __OSC_BUNDLE_S_H__
#define __OSC_BUNDLE_S_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "osc_message_s.h"
#include "osc_error.h"

t_osc_err osc_bundle_s_getMsgCount(int len, char *buf, int *count);
t_osc_err osc_bundle_s_getMessagesWithCallback(int len, char *buf, void (*f)(t_osc_msg_s*, void *), void *context);
t_osc_err osc_bundle_s_lookupAddress(int len, char *buf, char *address, int *nmatches, t_osc_msg_s ***m, int fullmatch);
t_osc_err osc_bundle_s_setBundleID(char *buf);
int osc_bundle_s_strcmpID(char *buf);
t_osc_err osc_bundle_s_format(long len, char *bndl, long *buflen, char **buf);

#ifdef __cplusplus
}
#endif

#endif // __OSC_BUNDLE_S_H__
