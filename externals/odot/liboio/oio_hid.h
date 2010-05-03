#ifndef __OIO_HID_H__
#define __OIO_HID_H__

#include <IOKit/hid/IOHIDLib.h>
#include "oio_err.h"

typedef struct _oio_hid_dev{
	char name[64];
	unsigned char slot;
	IOHIDDeviceRef device;
} t_oio_hid_dev;

t_oio_err oio_hid_getDeviceNames(int *n, char **names);

#endif // __OIO_HID_H__

