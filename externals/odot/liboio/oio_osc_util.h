#ifndef __OIO_OSC_UTIL_H__
#define __OIO_OSC_UTIL_H__

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include "oio_err.h"

t_oio_err oio_osc_util_printMessage(int n, char *buf, int (*f)(const char *,...));
t_oio_err oio_osc_util_printBundle(int n, char *buf, int (*f)(const char *,...));
void oio_osc_util_makenice(char *buf);

#endif //__OIO_OSC_UTIL_H__
