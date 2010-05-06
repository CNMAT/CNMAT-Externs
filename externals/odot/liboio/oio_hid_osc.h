#ifndef __OIO_HID_OSC_H__
#define __OIO_HID_OSC_H__

#include <IOKit/hid/IOHIDLib.h>
#include "oio_obj.h"
#include "oio_err.h"

t_oio_err oio_hid_osc_encode(long *len, char **oscbuf, IOHIDDeviceRef device, IOHIDValueRef value);


#endif // __OIO_HID_OSC_H__
