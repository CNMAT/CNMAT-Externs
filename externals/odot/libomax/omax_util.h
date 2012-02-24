/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2009-11, The Regents of
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

/** 	\file omax_util.h
	\author John MacCallum

*/

#ifndef __OMAX_UTIL_H__
#define __OMAX_UTIL_H__

#include "osc.h"
#include "ext.h"
#include "ext_obex.h"
#include "osc_message.h"
#include "osc_message_s.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////
// to be removed
//////////////////////////////////////////////////
// shouldn't need this
char omax_util_typetagForAtom(t_atom *a);
// or this
//int osc_util_check_pos_and_resize(char *buf, int len, char *pos);
// not encoding to serialized bundles any more--encode to unserialized and then serialize
//int omax_util_get_bundle_size_for_atoms(t_symbol *address, int argc, t_atom *argv);
// this is still in use by a few things mainly (exclusively?) in this file, but should be removed soon
//void omax_util_oscMsg2MaxAtoms_old(t_osc_msg *msg, long *ac, t_atom *av);
// these two are very old and probably safe to remove
//int osc_util_make_bundle(int numAddresses,
//t_symbol **addresses, 
//int *numArgs,
//char **typetags, 
//t_atom **args, 
//int *len, 
//char *buffer);

//////////////////////////////////////////////////
// to be replaced
//////////////////////////////////////////////////
// this should be replaced by the unserialized functions below.
int omax_util_encode_atoms(char *buf, t_symbol *address, int argc, t_atom *argv);

//////////////////////////////////////////////////
// new
//////////////////////////////////////////////////

void omax_util_outletOSC(void *outlet, long len, char *ptr);
void omax_util_maxFullPacketToOSCAtom_u(t_osc_atom_u **osc_atom, t_atom *len, t_atom *ptr);
void omax_util_maxAtomToOSCAtom_u(t_osc_atom_u **osc_atom, t_atom *max_atom);
t_osc_err omax_util_maxAtomsToOSCMsg_u(t_osc_msg_u **msg, t_symbol *address, int argc, t_atom *argv);
int omax_util_getNumAtomsInOSCMsg(t_osc_msg_s *msg);
void omax_util_oscMsg2MaxAtoms(t_osc_msg_s *m, t_atom *av);

int omax_util_liboErrorHandler(const char * const errorstr);

#ifdef __cplusplus
}
#endif

#endif 
