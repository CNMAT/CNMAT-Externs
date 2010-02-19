/*
 *  obstring.h
 *
 *  Copyright 2006 Cycling '74. All rights reserved.
 *
 */

#ifndef __OBSTRING_H__
#define __OBSTRING_H__

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif


/** 
	The string object. This struct is provided for debugging convenience, 
	but should be considered opaque and is subject to change without notice. 

	@ingroup string
*/
typedef struct _string
{
	t_object		s_obj;
	char			*s_text; 
} t_string;  

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif


/**
	Create a new string object.
	@ingroup string
	@param	psz		Pointer to a C-string that will be copied to memory internal to this string object instance.
	@return			The new string object instance pointer.
*/
t_string* string_new(const char *psz);


/**
	Create a new string object.
	@ingroup string
	@param	x		The string object instance.
	@return			A pointer to the internally maintained C-string.
*/
const char* string_getptr(t_string *x);


// TODO: This object has a number of additional methods exposed via Max's messaging interface


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__OBSTRING_H__
