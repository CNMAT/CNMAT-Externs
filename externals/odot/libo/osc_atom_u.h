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

/** 	\file osc_atom_u.h
	\author John MacCallum

*/

#ifndef __OSC_ATOM_U_H__
#define __OSC_ATOM_U_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include "osc_error.h"

typedef struct _osc_atom_u t_osc_atom_u;

t_osc_atom_u *osc_atom_u_alloc(void);
char osc_atom_u_getTypetag(t_osc_atom_u *a);

float osc_atom_u_getFloat(t_osc_atom_u *a);
double osc_atom_u_getDouble(t_osc_atom_u *a);
int32_t osc_atom_u_getInt32(t_osc_atom_u *a);
int64_t osc_atom_u_getInt64(t_osc_atom_u *a);
uint32_t osc_atom_u_getUint32(t_osc_atom_u *a);
uint64_t osc_atom_u_getUint64(t_osc_atom_u *a);
int osc_atom_u_getInt(t_osc_atom_u *a);
char *osc_atom_u_getString(t_osc_atom_u *a);
int osc_atom_u_getBool(t_osc_atom_u *a);

void osc_atom_u_setFloat(t_osc_atom_u *a, float v);
void osc_atom_u_setDouble(t_osc_atom_u *a, double v);
void osc_atom_u_setInt32(t_osc_atom_u *a, int32_t v);
void osc_atom_u_setInt64(t_osc_atom_u *a, int64_t v);
void osc_atom_u_setUInt64(t_osc_atom_u *a, uint64_t v);
void osc_atom_u_setUInt64(t_osc_atom_u *a, uint64_t v);
void osc_atom_u_setString(t_osc_atom_u *a, char *v);
void osc_atom_u_setBool(t_osc_atom_u *a, int v);
void osc_atom_u_setTrue(t_osc_atom_u *a);
void osc_atom_u_setFalse(t_osc_atom_u *a);
void osc_atom_u_setNull(t_osc_atom_u *a);

size_t osc_atom_u_sizeof(t_osc_atom_u *a);

t_osc_err osc_atom_u_format(t_osc_atom_u *a, long *buflen, char **buf);

#ifdef __cplusplus
}
#endif

#endif // __OSC_ATOM_U_H__
