#ifndef __OIO_OBJ_H__
#define __OIO_OBJ_H__

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <libgen.h>
#include "oio_err.h"

#define PP(s, ...) printf("%s[%d]:%s(): "s"\n", basename(__FILE__), __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define DEV_NAME(d) ((t_oio_generic_device *)d)->name
#define DEV_NEXT(d) ((t_oio_generic_device *)d)->next
#define DEV_PREV(d) ((t_oio_generic_device *)d)->prev

typedef enum _oio_dev_type{
	OIO_DEV_DNF,
	OIO_DEV_HID,
	OIO_DEV_MIDI,
	OIO_DEV_SERIAL
} t_oio_dev_type;

typedef struct _oio_generic_device{
	char name[64];
	struct _oio_generic_device *next, *prev;
} t_oio_generic_device;

typedef struct _oio{
	struct _oio_hid *hid;
	struct _oio_midi *midi;
} t_oio;

typedef void (*t_oio_callback)(t_oio *, long, char*, void *);

void oio_obj_alloc(t_oio_callback hid_connect_callback, 
		     void *hid_connect_context, 
		     t_oio_callback hid_disconnect_callback, 
		     void *hid_disconnect_context, 
		     char *hid_usage_plist, 
		     char *hid_cookie_plist,
		     t_oio_callback midi_notify_callback,
		   void *midi_notify_context,
		   t_oio **oio);
void oio_obj_run(t_oio *oio);
/*
t_oio *oio_obj_alloc(t_oio_callback hid_connect_callback, 
		     void *hid_connect_context, 
		     t_oio_callback hid_disconnect_callback, 
		     void *hid_disconnect_context, 
		     char *hid_usage_plist, 
		     char *hid_cookie_plist,
		     t_oio_callback midi_notify_callback,
		     void *midi_notify_context);
*/
t_oio_err oio_obj_sendOSC(t_oio *oio, int n, char *buf);
CFPropertyListRef oio_obj_getPlist(char *filepath);
t_oio_err oio_obj_getDevicesByName(t_oio *oio, 
				   char *name, 
				   t_oio_generic_device *device_list, 
				   CFMutableDictionaryRef hash, 
				   int *num_devices, 
				   t_oio_generic_device ***matched_devices);

#endif // __OIO_OBJ_H__
