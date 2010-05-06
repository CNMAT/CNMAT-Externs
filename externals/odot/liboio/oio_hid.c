#include "oio_hid.h"
#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <pthread.h>
#include "oio_mem.h"
#include "oio_hid_util.h"
#include "oio_hid_osc.h"

CFMutableDictionaryRef oio_hid_dict;

t_oio_err oio_hid_run(t_oio *oio);
void *oio_hid_runloop(void *context);

// callbacks
void oio_hid_connectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
void oio_hid_disconnectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
void oio_hid_valueCallback(void *context, IOReturn result, void *sender, IOHIDValueRef value);
void oio_hid_dispatch(t_oio_hid_callbackList *callback_list, long n, char *osc);

void oio_hid_enumerateDevices(t_oio *oio);
void oio_hid_addDevice(t_oio *oio, IOHIDDeviceRef device);

void oio_hid_postKey(IOHIDDeviceRef device, CFStringRef key);

t_oio_err oio_hid_run(t_oio *oio){
	pthread_t th;
	pthread_create(&th, NULL, oio_hid_runloop, (void *)oio);
	pthread_detach(th);
	return OIO_ERR_NONE;
}

void *oio_hid_runloop(void *context){
	t_oio *oio = (t_oio *)context;
	t_oio_hid *hid = oio->hid;

	IOHIDManagerScheduleWithRunLoop(hid->hidmanager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	CFRunLoopRun();
	pthread_exit(NULL);
}

t_oio_err oio_hid_getDeviceNames(t_oio *oio, int *n, char ***names){
	t_oio_hid *hid = oio->hid;
	CFMutableDictionaryRef dict = hid->device_hash;
	int nn = CFDictionaryGetCount(dict);
	CFStringRef keys[nn];
	CFNumberRef vals[nn];
	CFDictionaryGetKeysAndValues(dict, (const void **)&keys, (const void **)&vals);
	*n = nn;
	char **ptr;
	ptr = (char **)oio_mem_alloc(nn, sizeof(char *));
	int i;
	for(i = 0; i < nn; i++){
		ptr[i] = oio_mem_alloc(256, sizeof(char));
		CFStringGetCString(keys[i], ptr[i], 256, kCFStringEncodingUTF8);
	}
	*names = ptr;
	return OIO_ERR_NONE;
}

void oio_hid_connectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device){
	t_oio *oio = (t_oio *)context;
	oio_hid_addDevice(oio, device);
	IOHIDDeviceOpen(device, 0L);
}

void oio_hid_valueCallback(void *context, IOReturn result, void *sender, IOHIDValueRef value){
	t_oio *oio = (t_oio *)context;
	IOHIDDeviceRef device = (IOHIDDeviceRef)sender;
	io_service_t s = IOHIDDeviceGetService(device);
	char buf[256];
	if(oio_hid_util_getDeviceProductID(device, 256, buf)){
		return;
	}
	t_oio_hid_dev *d = oio->hid->devices;
	while(d){
		if(s == IOHIDDeviceGetService(d->device)){
			long len;
			char *oscbuf;
			PP("");
			oio_hid_osc_encode(&len, &oscbuf, device, value);
			oio_hid_dispatch(d->input_value_callbacks, strlen(d->name), d->name);
			break;
		}
		d = d->next;
	}
}

void oio_hid_dispatch(t_oio_hid_callbackList *callback_list, long n, char *osc){
	t_oio_hid_callbackList *cb = callback_list;
	while(cb){
		cb->f(n, osc);
		cb = cb->next;
	}
}

void oio_hid_enumerateDevices(t_oio *oio){
	IOHIDManagerRef hidmanager = oio->hid->hidmanager;

	CFSetRef set = IOHIDManagerCopyDevices(hidmanager);
	int n = (int)CFSetGetCount(set);

	if(n == 0){
		return;
	}
	const void *devices[n];
	CFSetGetValues(set, devices);
	int i;
	for(i = 0; i < n; i++){
		oio_hid_addDevice(oio, (IOHIDDeviceRef)devices[i]);
	}
}

// add the device if it doesn't already exist
void oio_hid_addDevice(t_oio *oio, IOHIDDeviceRef device){
	t_oio_hid *hid = oio->hid;
	CFMutableDictionaryRef dict = hid->device_hash;
	char buf[256], mangled[256];
	t_oio_err ret;
	if(ret = oio_hid_util_getDeviceProductID(device, 256, buf)){
		return;
	}
	strcpy(mangled, buf);
	int i = 0;
	while(i < 128){
		CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, mangled, kCFStringEncodingUTF8);
		// if the key is not in the dictionary, add it
		if(!CFDictionaryContainsKey((CFDictionaryRef)(dict), key)){
			t_oio_hid_dev *dev = (t_oio_hid_dev *)oio_mem_alloc(1, sizeof(t_oio_hid_dev));
			strcpy(dev->name, mangled);
			dev->device = device;
			dev->port = IOHIDDeviceGetService(device);
			dev->prev = NULL;
			dev->input_value_callbacks = NULL;
			dev->disconnect_callbacks = NULL;
			dev->connect_callbacks = NULL;
			if(hid->devices){
				hid->devices->prev = dev;
			}
			dev->next = hid->devices;
			hid->devices = dev;
			{
				CFNumberRef val = CFNumberCreate(kCFAllocatorDefault, kCFNumberLongType, (void *)(&dev));
				CFDictionaryAddValue(dict, key, val);
				CFRelease(key);
				CFRelease(val);
			}
			break;
		}else{
			// if the key is in the dictionary, check to see if the device 
			// matches the one we have and bail if it does (don't need it 
			// in there twice)
			const void *val = CFDictionaryGetValue(dict, key);
			if(val){
				long ptr;
				CFNumberGetValue((CFNumberRef)val, kCFNumberLongType, &ptr);
				if(ptr != 0){
					t_oio_hid_dev *dev = (t_oio_hid_dev *)ptr;
					io_service_t s1 = IOHIDDeviceGetService(device);
					io_service_t s2 = IOHIDDeviceGetService(dev->device);
					if(s1 == s2){
						break;
					}
				}
			}
		}
		sprintf(mangled, "%s %d", buf, ++i);
	}
}

t_oio_err oio_hid_registerValueCallback(t_oio *oio, const char *name, t_oio_hid_callback f){
	t_oio_hid *hid = oio->hid;
	CFMutableDictionaryRef dict = hid->device_hash;
	CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
	if(CFDictionaryContainsKey(dict, key)){
		const void *val;
		val = CFDictionaryGetValue(dict, key);
		if(val){
			long ptr;
			CFNumberGetValue((CFNumberRef)val, kCFNumberLongType, &ptr);
			if(ptr != 0){
				t_oio_hid_dev *dev = (t_oio_hid_dev *)ptr;
				if(dev->input_value_callbacks == NULL){
					IOHIDDeviceRegisterInputValueCallback(dev->device, oio_hid_valueCallback, oio);
				}
				t_oio_hid_callbackList *cb = (t_oio_hid_callbackList *)oio_mem_alloc(1, sizeof(t_oio_hid_callbackList));
				cb->f = f;
				cb->next = dev->input_value_callbacks;
				dev->input_value_callbacks = cb;
			}
		}
	}else{
		OIO_ERROR(OIO_ERR_DNF);
	}
	CFRelease(key);
	return OIO_ERR_NONE;
}

void oio_hid_alloc(t_oio *oio){
	t_oio_hid *hid = (t_oio_hid *)oio_mem_alloc(1, sizeof(t_oio_hid));
	hid->devices = NULL;
	hid->device_hash = CFDictionaryCreateMutable(kCFAllocatorDefault, 128, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	hid->hidmanager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	IOHIDManagerSetDeviceMatching(hid->hidmanager, NULL);

	IOHIDManagerRegisterDeviceMatchingCallback(hid->hidmanager, oio_hid_connectCallback, (void *)oio);

	oio->hid = hid;

	oio_hid_run(oio);
	oio_hid_enumerateDevices(oio);
}
