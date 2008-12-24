#include "cmmjl_error.h"

static char *cmmjl_error_strings[] = {
	// Library error strings
	"Operation was successful",
	"Illegal error code",
	// OSC error strings
	"Operation was successful",
	"OSC packet size is not a multiple of 4 bytes",
	"Bad OSC Packet size",
	"Requested buffer position exceeds buffer size",
	"Buffer pointer is NULL",
	"Bundle message is too small for a time tag",
	"Bad size count in bundle",
	"Error in encapsulated message.  Recursive call to cnmat_osc_parseFullPacket() returned an error",
	"Failed to process the entire OSC packet",
	"Bad message name string",
	"Illegal error code",
	// SDIF error strings
	"Operation was successful",
	"Illegal error code"
};

char *cmmjl_error_getString(t_cmmjl_error e){
	return cmmjl_error_strings[e];
}
