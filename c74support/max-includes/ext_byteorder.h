#ifndef _BYTEORDER_H_
#define _BYTEORDER_H_

#ifdef WIN_VERSION
#define LITTLE_ENDIAN
#else
#define BIG_ENDIAN 
#endif

#define BYTEORDER_SWAPW16(x) ((short)(((((unsigned short)(x))>>8)&0x00ff)+((((unsigned short)(x))<<8)&0xff00)))
#define BYTEORDER_SWAPW32(x) ((long)(((((unsigned long)(x))>>24L)&0x000000ff)+((((unsigned long)(x))>>8L)&0x0000ff00)+ \
									((((unsigned long)(x))<<24L)&0xff000000)+((((unsigned long)(x))<<8L)&0x00ff0000)))
#define BYTEORDER_SWAPF32 byteorder_swapf32
#define BYTEORDER_SWAPF64 byteorder_swapf64

#ifdef BIG_ENDIAN
#define BYTEORDER_LSBW16(x) 		BYTEORDER_SWAPW16(x)
#define BYTEORDER_LSBW32(x) 		BYTEORDER_SWAPW32(x)
#define BYTEORDER_LSBF32(x) 		BYTEORDER_SWAPF32(x)
#define BYTEORDER_LSBF64(x) 		BYTEORDER_SWAPF64(x)
#define BYTEORDER_MSBW16(x) 		(x)
#define BYTEORDER_MSBW32(x) 		(x)
#define BYTEORDER_MSBF32(x)			(x)
#define BYTEORDER_MSBF64(x) 		(x)
#else
#define BYTEORDER_LSBW16(x) 		(x)
#define BYTEORDER_LSBW32(x) 		(x)
#define BYTEORDER_LSBF32(x) 		(x)
#define BYTEORDER_LSBF64(x) 		(x)
#define BYTEORDER_MSBW16(x) 		BYTEORDER_SWAPW16(x)
#define BYTEORDER_MSBW32(x) 		BYTEORDER_SWAPW32(x)
#define BYTEORDER_MSBF32(x) 		BYTEORDER_SWAPF32(x)
#define BYTEORDER_MSBF64(x) 		BYTEORDER_SWAPF64(x)
#endif

#endif // _BYTEORDER_H