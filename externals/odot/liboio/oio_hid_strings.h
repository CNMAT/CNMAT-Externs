#ifndef __OIO_HID_USAGE_STRINGS_H__
#define __OIO_HID_USAGE_STRINGS_H__

#include "oio_obj.h"
#include "oio_err.h"
#include <IOKit/hid/IOHIDLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>

CFStringRef oio_hid_strings_getUsagePageString(t_oio *oio, uint32_t usage_page);
CFStringRef oio_hid_strings_getUsageString(t_oio *oio, uint32_t usage_page, uint32_t usage);
uint32_t oio_hid_strings_getUsagePage(t_oio *oio, char *usage_page);
uint32_t oio_hid_strings_getUsage(CFDictionaryRef dict, char *usage);
t_oio_err oio_hid_strings_readUsageFile(t_oio *oio, const char *filename);
void oio_hid_usage_strings(uint32_t usage_page, uint32_t usage, char *usage_page_string, char *usage_string);

#endif // __OIO_HID_USAGE_STRINGS_H__
