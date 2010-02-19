#ifndef _BYTEORDER_H_
#define _BYTEORDER_H_

#ifdef MAC_VERSION
#if TARGET_RT_LITTLE_ENDIAN
#define C74_LITTLE_ENDIAN 1
#define C74_BIG_ENDIAN 0
#else
#define C74_LITTLE_ENDIAN 0
#define C74_BIG_ENDIAN 1
#endif

#else // WIN_VERSION

/**
	A macro that indicates whether or not the current architecture uses Litte-endian byte ordering
	(such as is used on an i386 processor).
	Note that this macro is always defined; it will be either a 0 or a 1.
	@ingroup byteorder
*/
#define C74_LITTLE_ENDIAN 1

/**
	A macro that indicates whether or not the current architecture uses Big-endian byte ordering
	(such as is used on a PPC processor).
	Note that this macro is always defined; it will be either a 0 or a 1.
	@ingroup byteorder
*/
#define C74_BIG_ENDIAN 0

#endif // WIN_VERSION


/**
	Switch the byte ordering of a short integer.
	@ingroup	byteorder
	@param	x	A short integer.
	@return		A short integer with the byte-ordering swapped.
*/
#define BYTEORDER_SWAPW16(x) ((short)(((((unsigned short)(x))>>8)&0x00ff)+((((unsigned short)(x))<<8)&0xff00)))

/**
	Switch the byte ordering of an integer.
	@ingroup	byteorder
	@param	x	An integer.
	@return		An integer with the byte-ordering swapped.
*/
#define BYTEORDER_SWAPW32(x) ((long)(((((unsigned long)(x))>>24L)&0x000000ff)+((((unsigned long)(x))>>8L)&0x0000ff00)+ \
									((((unsigned long)(x))<<24L)&0xff000000)+((((unsigned long)(x))<<8L)&0x00ff0000)))
/**
	Switch the byte ordering of a float.
	@ingroup	byteorder
	@param	x	A float.
	@return		A float with the byte-ordering swapped.
*/
#define BYTEORDER_SWAPF32 byteorder_swapf32

/**
	Switch the byte ordering of a double.
	@ingroup	byteorder
	@param	x	A double.
	@return		A double.
*/
#define BYTEORDER_SWAPF64 byteorder_swapf64


// Undocumented, use the above macros.
float byteorder_swapf32(float f);
double byteorder_swapf64(double f);


#if C74_BIG_ENDIAN
#define BYTEORDER_LSBW16(x) 		BYTEORDER_SWAPW16(x)
#define BYTEORDER_LSBW32(x) 		BYTEORDER_SWAPW32(x)
#define BYTEORDER_LSBF32(x) 		BYTEORDER_SWAPF32(x)
#define BYTEORDER_LSBF64(x) 		BYTEORDER_SWAPF64(x)
#define BYTEORDER_MSBW16(x) 		(x)
#define BYTEORDER_MSBW32(x) 		(x)
#define BYTEORDER_MSBF32(x)			(x)
#define BYTEORDER_MSBF64(x) 		(x)
#else

/**
	Switch the byte ordering of a short integer from the native swapping to Little-endian (Least Significant Byte).
	If the current environment is already Little-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A short integer.
	@return		A short integer with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_LSBW16(x) 		(x)

/**
	Switch the byte ordering of an integer from the native swapping to Little-endian (Least Significant Byte).
	If the current environment is already Little-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	An integer.
	@return		An integer with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_LSBW32(x) 		(x)

/**
	Switch the byte ordering of a float from the native swapping to Little-endian (Least Significant Byte).
	If the current environment is already Little-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A float.
	@return		A float with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_LSBF32(x) 		(x)

/**
	Switch the byte ordering of a double from the native swapping to Little-endian (Least Significant Byte).
	If the current environment is already Little-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A double.
	@return		A double with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_LSBF64(x) 		(x)

/**
	Switch the byte ordering of a short integer from the native swapping to Big-endian (Most Significant Byte).
	If the current environment is already Big-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A short integer.
	@return		A short integer with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_MSBW16(x) 		BYTEORDER_SWAPW16(x)

/**
	Switch the byte ordering of an integer from the native swapping to Big-endian (Most Significant Byte).
	If the current environment is already Big-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	An integer.
	@return		An integer with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_MSBW32(x) 		BYTEORDER_SWAPW32(x)

/**
	Switch the byte ordering of a float from the native swapping to Big-endian (Most Significant Byte).
	If the current environment is already Big-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A float.
	@return		A float with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_MSBF32(x) 		BYTEORDER_SWAPF32(x)

/**
	Switch the byte ordering of a double from the native swapping to Big-endian (Most Significant Byte).
	If the current environment is already Big-endian, then the returned value is not byteswapped.
	
	@ingroup	byteorder
	@param	x	A double.
	@return		A double with the byte-ordering swapped if neccessary.
*/
#define BYTEORDER_MSBF64(x) 		BYTEORDER_SWAPF64(x)

#endif


#if C74_LITTLE_ENDIAN
#define STR_TO_FOURCC(x)	((x) = \
							(((unsigned long) ((x) & 0x000000FF)) << 24) | \
							(((unsigned long) ((x) & 0x0000FF00)) << 8)  | \
							(((unsigned long) ((x) & 0x00FF0000)) >> 8)  | \
							(((unsigned long) ((x) & 0xFF000000)) >> 24))\
							
#else
/*
	Swap the byte ordering, if neccessary, for a Four Character Code.
	The bytes are swapped in-place, thus there is no return value.
	
	@ingroup byteorder
	@param	x	An integer (32bit) containing the four char code.  
				If byte-swapping is needed, it will be byte-swapped in-place.
*/
#define STR_TO_FOURCC(x)	(x)
#endif // C74_LITTLE_ENDIAN


#endif // _BYTEORDER_H
