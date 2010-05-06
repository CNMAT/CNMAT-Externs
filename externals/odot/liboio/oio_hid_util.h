#ifndef __OIO_HID_UTIL_H__
#define __OIO_HID_UTIL_H__

#include "oio_err.h"
#include <IOKit/hid/IOHIDLib.h>

t_oio_err oio_hid_util_getDeviceProductID(IOHIDDeviceRef device, long bufsize, char *buf);
void oio_hid_util_dumpDeviceInfo(IOHIDDeviceRef device);
void oio_hid_util_dumpElementInfo(IOHIDElementRef element);
void oio_hid_util_postDeviceKey(IOHIDDeviceRef device, CFStringRef key);
void oio_hid_util_postElementKey(IOHIDElementRef element, CFStringRef key);

#endif // __OIO_HID_UTIL_H__
