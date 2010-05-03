#include "oio_hid.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>

CFMutableDictionaryRef oio_hid_dict;

void oio_hid_connect(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
void oio_hid_value(void *context, IOReturn result, void *sender, IOHIDValueRef value);

t_oio_err oio_hid_getDeviceNames(int *n, char **names){
	IOHIDManagerRef hidmanager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	if(!hidmanager){
		return OIO_ERR_NOHID;
	}
	IOHIDManagerSetDeviceMatching(hidmanager, NULL);
	IOHIDManagerRegisterDeviceMatchingCallback(hidmanager, oio_hid_connect, NULL);
	IOHIDManagerScheduleWithRunLoop(hidmanager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	IOHIDManagerOpen(hidmanager, kIOHIDOptionsTypeNone);
	IOHIDManagerRegisterInputValueCallback(hidmanager, oio_hid_value, NULL);
	CFRunLoopRun();
	return OIO_ERR_NONE;
}

void oio_hid_connect(void *context, IOReturn result, void *sender, IOHIDDeviceRef device){
	printf("%s: context = %p, result = %p, sender = %p, device = %p\n", __PRETTY_FUNCTION__, context, (void *)result, sender, (void *)device);
	IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone);
	CFTypeRef productKey = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
	if(productKey){
		printf("productKey exists\n");
		if(CFStringGetTypeID() == CFGetTypeID(productKey)){
			printf("it's a string\n");
			CFIndex len = (int)CFStringGetLength((CFStringRef)productKey);
			if(len > 0){
				printf("len = %d\n", (int)len);
				char buf[(int)len + 1];
				Boolean r = CFStringGetCString((CFStringRef)productKey, buf, len+1, kCFStringEncodingUTF8);
				printf("r = %d\n", r);
				printf("%s\n", buf);
			}
		}
	}
}

void oio_hid_value(void *context, IOReturn result, void *sender, IOHIDValueRef value){
	//printf("%s(context: %p, result: %p, sender: %p, value: %p).\n",
	//__PRETTY_FUNCTION__, context, (void *) result, sender, (void*) value);
	//printf("timestamp: %llu\n", IOHIDValueGetTimeStamp(value));
}

void oio_hid_init(void){
	oio_hid_dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 128, kCFTypeDictionaryKeyCallBacks, kCFTypeDictionaryValueCallBacks);
}
