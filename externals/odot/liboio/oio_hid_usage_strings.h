#ifndef __OIO_HID_USAGE_STRINGS_H__
#define __OIO_HID_USAGE_STRINGS_H__

#include <IOKit/hid/IOHIDLib.h>

void oio_hid_usage_strings(uint32_t usage, uint32_t usage_page, char *usage_string, char *usage_page_string);

#endif // __OIO_HID_USAGE_STRINGS_H__
