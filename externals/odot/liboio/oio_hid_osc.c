#include "oio_hid_osc.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include "oio_mem.h"
#include "oio_hid_util.h"
#include "oio_hid_usage_strings.h"
#include "oio_osc_util.h"

void oio_hid_osc_encodeRaw(char *name, uint32_t usage_page, uint32_t usage, uint32_t cookie, uint64_t val, int *n, char *buf);
void oio_hid_osc_encodeGeneric(char *name, char *usage_page, char *usage, uint32_t cookie, uint64_t val, int *n, char *buf);;

t_oio_err oio_hid_osc_encode(long *len, char **oscbuf, t_oio_hid_dev *device, IOHIDValueRef value){
	if(!(*oscbuf)){
		*oscbuf = oio_mem_alloc(1024, sizeof(char));
	}
	int oscbuf_size = 1024;

	IOHIDElementRef element = IOHIDValueGetElement(value);
	uint64_t timestamp = IOHIDValueGetTimeStamp(value);
	uint64_t val = IOHIDValueGetIntegerValue(value);
	uint32_t usage = IOHIDElementGetUsage(element);
	uint32_t usage_page = IOHIDElementGetUsagePage(element);
	uint32_t type = IOHIDElementGetType(element);
	uint32_t cookie = (uint32_t)IOHIDElementGetCookie(element);

	char *ptr = *oscbuf;
	strncpy(ptr, "#bundle", 8);
	ptr += 8;
	*((uint64_t *)ptr) = hton64(timestamp);
	ptr += 8;

	int n;
	char buf[256];
	char usage_string[256], usage_page_string[256];
	usage_string[0] = '\0';
	usage_page_string[0] = '\0';
	oio_hid_usage_strings(usage_page, usage, usage_page_string, usage_string);

	oio_hid_osc_encodeRaw(device->name, usage_page, usage, cookie, val, &n, buf);
	if(n + (ptr - *oscbuf) > oscbuf_size){
		*oscbuf = oio_mem_resize(*oscbuf, oscbuf_size * 2);
		oscbuf_size * 2;
	}
	memcpy(ptr, buf, n);
	ptr += n;

	oio_hid_osc_encodeGeneric(device->name, usage_page_string, usage_string, cookie, val, &n, buf);
	if((n - 4) + (ptr - *oscbuf) > oscbuf_size){
		*oscbuf = oio_mem_resize(*oscbuf, oscbuf_size * 2);
		oscbuf_size * 2;
	}
	memcpy(ptr, buf, n);
	ptr += n;
	//oio_osc_util_printBundle(ptr - oscbuf, oscbuf, printf);
	//oio_osc_util_printMessage(n, buf, printf);
	*len = (ptr - *oscbuf);
	return OIO_ERR_NONE;
}

void oio_hid_osc_encodeRaw(char *name, uint32_t usage_page, uint32_t usage, uint32_t cookie, uint64_t val, int *n, char *buf){
	char *ptr = buf + 4;

	ptr += sprintf(ptr, "/%s/usage-page/%d/usage/%d/cookie/%u", name, usage_page, usage, cookie);
	*ptr++ = '\0';
	while((ptr - buf) % 4){
		*ptr++ = '\0';
	}
	*ptr++ = ',';
	*ptr++ = 'h';
	*ptr++ = '\0';
	*ptr++ = '\0';
	*((uint64_t *)ptr) = hton64(val);
	ptr += 8;
	*((uint32_t *)buf) = hton32(ptr - buf - 4);

	*n = ptr - buf;
}

void oio_hid_osc_encodeGeneric(char *name, char *usage_page, char *usage, uint32_t cookie, uint64_t val, int *n, char *buf){
	char *ptr = buf + 4;
	char *unknown = "<unknown>";
	char *up = usage_page, *uu = usage;
	if(*usage_page == '\0'){
		up = unknown;
	}
	if(*usage == '\0'){
		uu = unknown;
	}

	ptr += sprintf(ptr, "/%s/%s/%u", name, uu, cookie);
	*ptr++ = '\0';
	while((ptr - buf) % 4){
		*ptr++ = '\0';
	}
	*ptr++ = ',';
	*ptr++ = 'h';
	*ptr++ = '\0';
	*ptr++ = '\0';
	*((uint64_t *)ptr) = hton64(val);
	ptr += 8;
	*((uint32_t *)buf) = hton32(ptr - buf - 4);

	*n = ptr - buf;
}

void oio_hid_osc_encodeSpecific(t_oio_hid_dev *dev, uint32_t usage_page, uint32_t usage, uint32_t cookie, uint64_t val, int *n, char *buf){

}
