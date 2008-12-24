/** @file cmmjl.h
	Basic common functions and initialization.
*/

#include "cmmjl_error.h"
#include "cmmjl_types.h"

#ifndef __CMMJL_H__
#define __CMMJL_H__

/** Initializes the library.  This must be called before the library is used.  Currently, it sets the cmmjl_error callback to error() (defined in one of the max-includes) and initializes the symbol table.
 */
t_cmmjl_error cmmjl_init(void);

#endif
