#include "oio_hid_osc.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include "oio_mem.h"
#include "oio_hid_util.h"



t_oio_err oio_hid_osc_encode(long *len, char **oscbuf, IOHIDDeviceRef device, IOHIDValueRef value){
	IOHIDElementRef element = IOHIDValueGetElement(value);
	uint64_t timestamp = IOHIDValueGetTimeStamp(value);
	uint64_t val = IOHIDValueGetIntegerValue(value);
	uint32_t usage = IOHIDElementGetUsage(element);
	uint32_t usage_page = IOHIDElementGetUsagePage(element);
	PP("val = %llu", val);
	oio_hid_util_dumpElementInfo(element);
	return OIO_ERR_NONE;
}
