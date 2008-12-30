#include "cmmjl_error.h"

const char *cmmjl_strerror(const int cmmjl_errno){
	switch(cmmjl_errno){
	case CMMJL_SUCCESS:
		return "success";
	case CMMJL_FAILURE:
		return "failure";
	case CMMJL_ENULLPTR:
		return "NULL pointer";
	case CMMJL_OSC_ENO4BYTE:
		return "OSC packet is not a multiple of 4 bytes long";
	case CMMJL_OSC_EUNDRFLW:
		return "OSC packet is too small";
	case CMMJL_OSC_EOVRFLW:
		return "OSC packet is too big";
	case CMMJL_OSC_EBNDLNO4:
		return "OSC bundle size is not a multiple of 4 bytes long";
	case CMMJL_OSC_EBADBNDL:
		return "OSC bad bundle size";
	case CMMJL_OSC_EBADMSG:
		return "OSC bad message name";
	}
}
