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

#ifndef __OSC_ERROR_H__
#define __OSC_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint64_t t_osc_err;

#define OSC_ERR_NONE 0
#define OSC_ERR_BUNDLETOOSMALL 0x1
#define OSC_ERR_NOBUNDLEID 0x2
#define OSC_ERR_MSGTOOSMALL 0x4
#define OSC_ERR_MALFORMEDADDRESS 0x8
#define OSC_ERR_NOBUNDLE 0x10
#define OSC_ERR_OUTOFMEM 0x11
#define OSC_ERR_NULLPTR 0x12

char *osc_error_string(t_osc_err err);
t_osc_err osc_error_bundleSanityCheck(int len, char *bundle);
t_osc_err osc_error_msgSanityCheck(char *msg);
t_osc_err osc_error_validateAddress(char *address);

#ifdef __cplusplus
}
#endif

#endif // __OSC_ERROR_H__
