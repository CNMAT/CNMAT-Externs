/* 
	jit.platform.h

	Copyright 2001-2004 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
	
*/

#ifndef __JIT_PLATFORM_H__
#define __JIT_PLATFORM_H__

#ifndef WIN_VERSION 
#define MAC
#endif

#ifdef WIN_VERSION
#define JIT_EX_DATA_DECL __declspec(dllexport)
#ifdef WIN_JITLIB
#define JIT_EX_DATA extern __declspec(dllexport)
#else
#define JIT_EX_DATA extern __declspec(dllimport)
#endif //WIN_JITLIB
#else
#define JIT_EX_DATA_DECL
#define JIT_EX_DATA extern
#endif // WIN_VERSION

#define C74_MAX

#ifdef MAC
#include "jit.mac.h"
#else 
#define PREFER_POST_INC
//#include "jit.win.h"
#endif

#ifdef C74_MAX
#include "jit.max.h"
#endif

//optimization
#include "jit.altivec.h"

//endian issues
#include "jit.byteorder.h"

//thread related
#include "jit.critical.h"

//speed macros
#ifdef PREFER_POST_INC
#define FAST_INC_SETUP(x)			// nothing
#define FAST_INC(x)			((x)++)
#define FAST_INC_DEREF(x)	(*((x)++))
#define FAST_DEC_SETUP(x)			// nothing
#define FAST_DEC(x)			((x)--)
#define FAST_DEC_DEREF(x)	(*((x)--))
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
