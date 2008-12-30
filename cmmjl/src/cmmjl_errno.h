#ifndef __CMMJL_ERRNO_H__
#define __CMMJL_ERRNO_H__
#endif

/** Error codes used by the cmmjl */
typedef enum _cmmjl_error{
	CMMJL_SUCCESS = 0, /**< success */
	CMMJL_FAILURE, /**< failure */
	CMMJL_ENULLPTR, /**< null pointer */
	CMMJL_OSC_ENO4BYTE = 0x100, /**< OSC packet is not a multiple of 4 bytes long */
	CMMJL_OSC_EUNDRFLW, /**< OSC packet is too small */
	CMMJL_OSC_EOVRFLW, /**< OSC packet is too big */
	CMMJL_OSC_EBNDLNO4, /**< OSC bundle size is not a multiple of 4 bytes long */
	CMMJL_OSC_EBADBNDL, /**< OSC bad bundle size */
	CMMJL_OSC_EBADMSG /**< OSC bad message name */
} t_cmmjl_error;
