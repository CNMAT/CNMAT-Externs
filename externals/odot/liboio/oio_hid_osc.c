#include "oio_hid_osc.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include "oio_mem.h"
#include "oio_hid_util.h"
#include "oio_hid_usage_strings.h"



t_oio_err oio_hid_osc_encode(long *len, char **oscbuf, IOHIDDeviceRef device, IOHIDValueRef value){
	IOHIDElementRef element = IOHIDValueGetElement(value);
	uint64_t timestamp = IOHIDValueGetTimeStamp(value);
	uint64_t val = IOHIDValueGetIntegerValue(value);
	uint32_t usage = IOHIDElementGetUsage(element);
	uint32_t usage_page = IOHIDElementGetUsagePage(element);
	uint32_t type = IOHIDElementGetType(element);
	uint32_t cookie = (uint32_t)IOHIDElementGetCookie(element);
	//PP("type: %u 0x%x, usage: %u 0x%x, usage page: %u 0x%x, cookie: %u 0x%x, val: %llu 0x%llx", type, type, usage, usage, usage_page, usage_page, cookie, cookie, val, val);
	char usage_string[256], usage_page_string[256];
	oio_hid_usage_strings(usage, usage_page, usage_string, usage_page_string);
	PP("%s: %s", usage_string, usage_page_string);
	//oio_hid_util_dumpElementInfo(element);
	return OIO_ERR_NONE;
}
