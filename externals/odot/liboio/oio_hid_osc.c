#include "oio_hid_osc.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include "oio_mem.h"
#include "oio_hid_util.h"
#include "oio_hid_usage_strings.h"

void oio_hid_osc_encodeGeneric(char *name, uint32_t usage_page, uint32_t usage, uint32_t cookie, uint64_t val, int *n, char *buf);

t_oio_err oio_hid_osc_encode(long *len, char **oscbuf, t_oio_hid_dev *device, IOHIDValueRef value){
	IOHIDElementRef element = IOHIDValueGetElement(value);
	uint64_t timestamp = IOHIDValueGetTimeStamp(value);
	uint64_t val = IOHIDValueGetIntegerValue(value);
	uint32_t usage = IOHIDElementGetUsage(element);
	uint32_t usage_page = IOHIDElementGetUsagePage(element);
	uint32_t type = IOHIDElementGetType(element);
	uint32_t cookie = (uint32_t)IOHIDElementGetCookie(element);
	//PP("type: %u 0x%x, usage: %u 0x%x, usage page: %u 0x%x, cookie: %u 0x%x, val: %llu 0x%llx", type, type, usage, usage, usage_page, usage_page, cookie, cookie, val, val);
	switch(type){
	case kIOHIDElementTypeInput_Misc:

		break;
	}
	//char usage_string[256], usage_page_string[256];
	//oio_hid_usage_strings(usage_page, usage, usage_page_string, usage_string);
	//PP("%s: %s", usage_string, usage_page_string);
	//PP("***********************************************************************");
	//oio_hid_util_dumpElementInfo(element);
	int n;
	char buf[1024];
	oio_hid_osc_encodeGeneric(device->name, usage_page, usage, cookie, val, &n, buf);
	char *ptr = buf;
	printf("%d ", ntoh32(*((uint32_t *)buf)));
	ptr += 4;
	printf(" ");
	while(*ptr != ','){
		printf("%c", *ptr++);
	}
	ptr++;
	printf(" %c ", *ptr);
	ptr += 3;
	printf("%llu\n", ntoh64(*((uint64_t *)ptr)));
	return OIO_ERR_NONE;
}

void oio_hid_osc_encodeGeneric(char *name, uint32_t usage_page, uint32_t usage, uint32_t cookie, uint64_t val, int *n, char *buf){
	char usage_string[256], usage_page_string[256];
	usage_string[0] = '\0';
	usage_page_string[0] = '\0';
	oio_hid_usage_strings(usage_page, usage, usage_page_string, usage_string);
	char *ptr = buf + 4;

	// put <unknown> or something in there when we don't have a valid usage page
	ptr += sprintf(ptr, "/hid/%s/%s/%d", usage_page_string, usage_string, cookie);
	while((ptr - buf) % 4){
		ptr++;
	}
	*ptr++ = ',';
	*ptr++ = 'h';
	*ptr++ = '\0';
	*ptr++ = '\0';
	*((uint64_t *)ptr) = hton64(val);
	ptr += 8;
	*((uint32_t *)buf) = hton32(ptr - buf + 4);

	*n = ptr - buf;
}
