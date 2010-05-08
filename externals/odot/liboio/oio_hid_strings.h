#ifndef __OIO_HID_USAGE_STRINGS_H__
#define __OIO_HID_USAGE_STRINGS_H__

#include <IOKit/hid/IOHIDLib.h>

void oio_hid_usage_strings(uint32_t usage_page, uint32_t usage, char *usage_page_string, char *usage_string);

#endif // __OIO_HID_USAGE_STRINGS_H__
