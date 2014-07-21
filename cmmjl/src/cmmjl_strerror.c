#include "cmmjl_error.h"

const char *cmmjl_strerror(unsigned long long err){
	switch(err){
	case CMMJL_SUCCESS:
		return "success";
	case CMMJL_FAILURE:
		return "failure";
	case CMMJL_ENULLPTR:
		return "NULL pointer";
	case CMMJL_EBADTYPE:
		return "unknown data type";
	case CMMJL_ENODATA:
		return "no data";
	case CMMJL_ENOOBJ:
		return "couldn't find internal t_cmmjl_obj";
	case CMMJL_ENOFUNC:
		return "no such function";
	case CMMJL_ENOMEM:
		return "out of memory";
	case CMMJL_OSC_ENO4BYTE:
		return "OSC packet is not a multiple of 4 bytes long";
	case CMMJL_OSC_EUNDRFLW:
		return "data in OSC packet is too small";
	case CMMJL_OSC_EOVRFLW:
		return "data in OSC packet is too big";
	case CMMJL_OSC_EBNDLNO4:
		return "OSC bundle size is not a multiple of 4 bytes long";
	case CMMJL_OSC_EBADBNDL:
		return "OSC bad bundle size";
	case CMMJL_OSC_EBADMSG:
		return "OSC bad message name";
	case CMMJL_OSC_ETYPTAG:
		return "unrecognized OSC type tag";
	case CMMJL_OSC_EARGS:
		return "too many arguments in OSC message";
	case CMMJL_OSC_EBADALIGN:
		return "bad string alignment in OSC packet";
	case CMMJL_OSC_ELIB:
		return "the OSC lib encountered an error while processing a packet";
	case CMMJL_OSC_EMATCH:
		return "the OSC lib encountered an error while attempting a match";
	case CMMJL_OSC_ELATE:
		return "OSC packet arrived too late for scheduling";
	case CMMJL_OSC_EMAXDEL:
		return "OSC packet was scheduled too far into the future";
	}
}
