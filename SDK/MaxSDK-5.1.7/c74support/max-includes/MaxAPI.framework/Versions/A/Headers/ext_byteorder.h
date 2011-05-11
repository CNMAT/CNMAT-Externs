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

// To be documented
#define BYTEORDER_SWAPF32_PTR(x) 			byteorder_swap_pointer_32((unsigned char*)(x))
#define BYTEORDER_SWAPF64_PTR(x) 			byteorder_swap_pointer_64((unsigned char*)(x))
#define BYTEORDER_SWAPF32_PTR_TO_PTR(x) 	byteorder_swap_pointer_32_copy((unsigned char*)(x))
#define BYTEORDER_SWAPF64_PTR_TO_PTR(x) 	byteorder_swap_pointer_64_copy((unsigned char*)(x))
#define BYTEORDER_SWAPF32_FROM_PTR(x) 		byteorder_swap_pointer_32_to_float32((unsigned char*)(x))
#define BYTEORDER_SWAPF64_FROM_PTR(x) 		byteorder_swap_pointer_64_to_float64((unsigned char*)(x))


// Undocumented, use the above macros.
// OBSOLETE. don't use. not safe to pass byteswapped floats in registers
float swapf32(float f);
double swapf64(double f);

// these functions are inplace
void byteorder_swap_pointer_16(unsigned char *p);
void byteorder_swap_pointer_32(unsigned char *p);
void byteorder_swap_pointer_64(unsigned char *p);
// these function copy from one pointer to another (src!=dst)
void byteorder_swap_pointer_16_copy(unsigned char *src, unsigned char *dst);
void byteorder_swap_pointer_32_copy(unsigned char *src, unsigned char *dst);
void byteorder_swap_pointer_64_copy(unsigned char *src, unsigned char *dst);
// these functions return floating point values from a byteswapped pointer
float byteorder_swap_pointer_32_to_float32(unsigned char *p);
double byteorder_swap_pointer_64_to_float64(unsigned char *p);


#if C74_BIG_ENDIAN
#define BYTEORDER_LSBW16(x) 				BYTEORDER_SWAPW16(x)
#define BYTEORDER_LSBW32(x) 				BYTEORDER_SWAPW32(x)
#define BYTEORDER_LSBF32(x) 				BYTEORDER_SWAPF32(x)
#define BYTEORDER_LSBF64(x) 				BYTEORDER_SWAPF64(x)
#define BYTEORDER_LSBF32_PTR(x) 			BYTEORDER_SWAPF32_PTR((unsigned char *)(x))
#define BYTEORDER_LSBF64_PTR(x) 			BYTEORDER_SWAPF64_PTR((unsigned char *)(x))
#define BYTEORDER_LSBF32_FROM_PTR(x) 		BYTEORDER_SWAPF32_FROM_PTR((unsigned char *)(x))
#define BYTEORDER_LSBF64_FROM_PTR(x) 		BYTEORDER_SWAPF64_FROM_PTR((unsigned char *)(x))
#define BYTEORDER_LSBF32_PTR_TO_PTR(x,y) 	BYTEORDER_SWAPF32_PTR_TO_PTR((unsigned char *)(x),(unsigned char *)(y))
#define BYTEORDER_LSBF64_PTR_TO_PTR(x,y) 	BYTEORDER_SWAPF64_PTR_TO_PTR((unsigned char *)(x),(unsigned char *)(y))
#define BYTEORDER_MSBW16(x) 				(x)
#define BYTEORDER_MSBW32(x) 				(x)
#define BYTEORDER_MSBF32(x)					(x)
#define BYTEORDER_MSBF64(x) 				(x)
#define BYTEORDER_MSBF32_PTR(x) 		
#define BYTEORDER_MSBF64_PTR(x) 		
#define BYTEORDER_MSBF32_FROM_PTR(x) 		(*(float *)(x))
#define BYTEORDER_MSBF64_FROM_PTR(x) 		(*(double *)(x))
#define BYTEORDER_MSBF32_PTR_TO_PTR(x,y) 	((*(float *)(y))=(*(float *)(x)))
#define BYTEORDER_MSBF64_PTR_TO_PTR(x,y) 	((*(double *)(y))=(*(double *)(x)))
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


// To be documented.
#define BYTEORDER_LSBF32_PTR(x) 		
#define BYTEORDER_LSBF64_PTR(x) 		
#define BYTEORDER_LSBF32_FROM_PTR(x) 		(*(float *)(x))
#define BYTEORDER_LSBF64_FROM_PTR(x) 		(*(double *)(x))
#define BYTEORDER_LSBF32_PTR_TO_PTR(x,y) 	((*(float *)(y))=(*(float *)(x)))
#define BYTEORDER_LSBF64_PTR_TO_PTR(x,y) 	((*(double *)(y))=(*(double *)(x)))
#define BYTEORDER_MSBF32_PTR(x) 			BYTEORDER_SWAPF32_PTR((unsigned char *)(x))
#define BYTEORDER_MSBF64_PTR(x) 			BYTEORDER_SWAPF64_PTR((unsigned char *)(x))
#define BYTEORDER_MSBF32_FROM_PTR(x) 		BYTEORDER_SWAPF32_FROM_PTR((unsigned char *)(x))
#define BYTEORDER_MSBF64_FROM_PTR(x) 		BYTEORDER_SWAPF64_FROM_PTR((unsigned char *)(x))
#define BYTEORDER_MSBF32_PTR_TO_PTR(x,y) 	BYTEORDER_SWAPF32_PTR_TO_PTR((unsigned char *)(x),(unsigned char *)(y))
#define BYTEORDER_MSBF64_PTR_TO_PTR(x,y) 	BYTEORDER_SWAPF64_PTR_TO_PTR((unsigned char *)(x),(unsigned char *)(y))

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
