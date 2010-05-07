#ifndef __OIO_HID_H__
#define __OIO_HID_H__

#include <IOKit/hid/IOHIDLib.h>
#include "oio_obj.h"
#include "oio_err.h"

typedef t_oio_callback t_oio_hid_callback;

typedef struct _oio_hid_callbackList{
	//void (*f)(long n, char *ptr);
	t_oio_hid_callback f;
	struct _oio_hid_callbackList *next;
} t_oio_hid_callbackList;

typedef struct _oio_hid_dev{
	char name[64];
	IOHIDDeviceRef device;
	struct _oio_hid_dev *next;
	struct _oio_hid_dev *prev;
	t_oio_hid_callbackList *input_value_callbacks;
} t_oio_hid_dev;

typedef struct _oio_hid{
	t_oio_hid_dev *devices;
	CFMutableDictionaryRef device_hash;
	IOHIDManagerRef hidmanager;
	t_oio_hid_callbackList *connect_callbacks;
	t_oio_hid_callbackList *disconnect_callbacks;
} t_oio_hid;

t_oio_err oio_hid_getDeviceNames(t_oio *oio, int *n, char ***names);
t_oio_err oio_hid_registerValueCallback(t_oio *oio, const char *name, t_oio_hid_callback f);
t_oio_err oio_hid_registerConnectCallback(t_oio *oio, t_oio_hid_callback f);
t_oio_err oio_hid_registerDisconnectCallback(t_oio *oio, t_oio_hid_callback f);
void oio_hid_alloc(t_oio *oio);

#endif // __OIO_HID_H__

