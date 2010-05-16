#ifndef __OIO_OBJ_H__
#define __OIO_OBJ_H__

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <libgen.h>
#include "oio_err.h"

typedef enum _oio_dev_type{
	OIO_DEV_DNF,
	OIO_DEV_HID,
	OIO_DEV_MIDI,
	OIO_DEV_SERIAL
} t_oio_dev_type;

#define PP(s, ...) printf("%s[%d]:%s(): "s"\n", basename(__FILE__), __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

typedef struct _oio{
	struct _oio_hid *hid;
} t_oio;

typedef void (*t_oio_callback)(t_oio *, long, char*, void *);

t_oio *oio_obj_alloc(t_oio_callback hid_connect_callback, 
		     void *hid_connect_context, 
		     t_oio_callback hid_disconnect_callback, 
		     void *hid_disconnect_context, 
		     const char *hid_usage_plist, 
		     const char *hid_cookie_plist);
t_oio_err oio_obj_sendOSC(t_oio *oio, int n, char *buf);
CFPropertyListRef oio_obj_getPlist(const char *filepath);

#endif // __OIO_OBJ_H__
