/* 
	jit.altivec.h

	Copyright 2001-2004 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
		
*/

#ifndef __JIT_ALTIVEC_H__
#define __JIT_ALTIVEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN_VERSION
#include <altivec.h>

#define AltivecGetPrefetchConstant(blockSizeInVectors,blockCount,blockStride) \
	((blockSizeInVectors << 24) & 0x1F000000) | ((blockCount << 16) && 0x00FF0000) | (blockStride & 0xFFFF)

#endif //WIN_VERSION

JIT_EX_DATA long _jit_altivec;

void jit_altivec_init(void);
long jit_altivec_capable(void);

#ifdef __cplusplus
}
#endif

#endif // __JIT_ALTIVEC_H__
