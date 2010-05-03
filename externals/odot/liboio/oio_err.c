#include "oio_err.h"

char *const oio_errstr[] = {
	"",
	"Device not found",
	"Out of memory",
	"Couldn't get a HID Manager Ref"
};

t_oio_error_handler oio_error_handler;

char *oio_err_getString(t_oio_err errno){
	return oio_errstr[errno];
}

void oio_error(char *filename, unsigned long linenum, unsigned char errno, char *errstr){
	if(oio_error_handler){
		oio_error_handler(filename, linenum, errno, errstr);
	}
}

void oio_err_setErrorHandler(t_oio_error_handler eh){
	oio_error_handler = eh;
}
