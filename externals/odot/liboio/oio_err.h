#ifndef __OIO_ERR_H__
#define __OIO_ERR_H__

#include <libgen.h>

typedef unsigned char t_oio_err;
#define OIO_ERR_NONE 0
#define OIO_ERR_DNF 1
#define OIO_ERR_MEM 2
#define OIO_ERR_NOHID 3
#define OIO_ERR_CFTYPE 4

typedef void (*t_oio_error_handler)(char *, unsigned long, t_oio_err, char *);

#define OIO_ERROR(errno) oio_error(basename(__FILE__), __LINE__, errno, oio_err_getString(errno))

char *oio_err_getString(t_oio_err errno);
void oio_error(char *filename, unsigned long linenum, unsigned char errno, char *errstr);
void oio_err_setErrorHandler(t_oio_error_handler eh);

#endif //__OIO_ERR_H__
