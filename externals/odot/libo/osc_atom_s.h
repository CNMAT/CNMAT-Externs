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

/** 	\file osc_atom_s.h
	\author John MacCallum

*/

#ifndef __OSC_ATOM_S_H__
#define __OSC_ATOM_S_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include "osc_error.h"
#include "osc_byteorder.h"

typedef struct _osc_atom_s t_osc_atom_s;

t_osc_atom_s *osc_atom_s_alloc(char typetag, char *ptr);
void osc_atom_s_set(t_osc_atom_s *a, char typetag, char *ptr);
char osc_atom_s_getTypetag(t_osc_atom_s *a);
char *osc_atom_s_getData(t_osc_atom_s *a);

float osc_atom_s_getFloat(t_osc_atom_s *a);
double osc_atom_s_getDouble(t_osc_atom_s *a);
int32_t osc_atom_s_getInt32(t_osc_atom_s *a);
int64_t osc_atom_s_getInt64(t_osc_atom_s *a);
uint32_t osc_atom_s_getUInt32(t_osc_atom_s *a);
uint64_t osc_atom_s_getUInt64(t_osc_atom_s *a);
int osc_atom_s_getInt(t_osc_atom_s *a);
char *osc_atom_s_getString(t_osc_atom_s *a);
int osc_atom_s_getBool(t_osc_atom_s *a);

void osc_atom_s_setFloat(t_osc_atom_s *a, float v);
void osc_atom_s_setDouble(t_osc_atom_s *a, double v);
void osc_atom_s_setInt32(t_osc_atom_s *a, int32_t v);
void osc_atom_s_setInt64(t_osc_atom_s *a, int64_t v);
void osc_atom_s_setUInt32(t_osc_atom_s *a, uint32_t v);
void osc_atom_s_setUInt64(t_osc_atom_s *a, uint64_t v);
void osc_atom_s_setString(t_osc_atom_s *a, char *v);
void osc_atom_s_setTrue(t_osc_atom_s *a);
void osc_atom_s_setFalse(t_osc_atom_s *a);
void osc_atom_s_setNull(t_osc_atom_s *a);

size_t osc_atom_s_sizeof(t_osc_atom_s *a);

t_osc_err osc_atom_s_format(t_osc_atom_s *a, long *buflen, char **buf);

#ifdef __cplusplus
}
#endif

#endif // __OSC_ATOM_S_H__
