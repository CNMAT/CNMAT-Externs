#ifndef _EXT_H_
#define _EXT_H_

#define C74_MAX_SDK_VERSION 0x0600

#if C74_NO_CONST == 0
#define C74_CONST const
#else
#define C74_CONST
#endif

#include "ext_prefix.h"	/* this header must always be first */

BEGIN_USING_C_LINKAGE

#include "ext_mess.h"

typedef short (*fretint)(void *, ...);	/* kludge to cast to function returning int */
typedef short (*eachdomethod)(void *, ...);
typedef long (*exprmethod)(void *, ...);
typedef long (*fptr)(void *, ...);
typedef void *(*methodptr)(void *, ...);

#define clock_free freeobject
#define binbuf_free freeobject
#define wind_free freeobject

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

/**
	This macro being defined means that getbytes and sysmem APIs for memory management are unified.
	This is correct for Max 5, but should be commented out when compiling for old max targets.
	@ingroup memory
*/
#define MM_UNIFIED

#include "ext_types.h"
#include "ext_maxtypes.h"
#include "ext_byteorder.h"

#include "ext_proto.h"

END_USING_C_LINKAGE

#endif /* _EXT_H_ */
