/* 
	jit.platform.h

	Copyright 2001-2002 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
	
*/

#ifndef __JIT_PLATFORM_H__
#define __JIT_PLATFORM_H__

#define MAC
#define C74_MAX

#ifdef MAC
#include "jit.mac.h"
#include "jit.altivec.h"
#else ifdef WIN32
#define PREFER_POST_INC
#include "jit.win.h"
#endif

#ifdef C74_MAX
#include "jit.max.h"
#endif

//endian issues
#include "jit.byteorder.h"

//speed macros
#ifdef PREFER_POST_INC
#define FAST_INC_SETUP(x)			// nothing
#define FAST_INC(x)			((x)++)
#define FAST_INC_DEREF(x)	((*(x))++)
#define FAST_DEC_SETUP(x)			// nothing
#define FAST_DEC(x)			((x)--)
#define FAST_DEC_DEREF(x)	((*(x))--)
#else
#define FAST_INC_SETUP(x)	(--(x)) 	// predec
#define FAST_INC(x)			(++(x))
#define FAST_INC_DEREF(x)	(*(++(x)))
#define FAST_DEC_SETUP(x)	(++(x))		// preinc
#define FAST_DEC(x)			(--(x))
#define FAST_DEC_DEREF(x)	(*(--(x)))
#endif

//utils
#define calcoffset(x,y) ((long)(&(((x *)0L)->y)))


#endif
