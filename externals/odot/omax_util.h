#ifndef __OMAX_UTIL_H__
#define __OMAX_UTIL_H__

#include "osc_util.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_atomarray.h"

//t_atomarray *omax_util_oscMsg2MaxAtoms(t_osc_msg *msg);
void omax_util_oscMsg2MaxAtoms(t_osc_msg *msg, long *ac, t_atom **av);

#endif //__OMAX_UTIL_H__
