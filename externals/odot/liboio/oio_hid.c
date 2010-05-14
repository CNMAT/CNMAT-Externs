#include "oio_hid.h"
#include <pthread.h>
#include "oio_mem.h"
#include "oio_hid_util.h"
#include "oio_hid_osc.h"
#include "oio_osc_util.h"
#include "oio_hid_strings.h"
#include <mach/mach_time.h>

CFMutableDictionaryRef oio_hid_dict;

t_oio_err oio_hid_run(t_oio *oio);
void *oio_hid_runloop(void *context);

// callbacks
void oio_hid_connectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
void oio_hid_disconnectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
void oio_hid_valueCallback(void *context, IOReturn result, void *sender, IOHIDValueRef value);
void oio_hid_dispatch(t_oio *oio, t_oio_hid_callbackList *callback_list, long n, char *osc);

void oio_hid_sendToDevice(IOHIDDeviceRef device, IOHIDElementRef element, uint64_t value);

void oio_hid_enumerateDevices(t_oio *oio);
t_oio_hid_dev *oio_hid_addDevice(t_oio *oio, IOHIDDeviceRef device);
void oio_hid_removeDevice(t_oio *oio, IOHIDDeviceRef device);

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
		if(keys[i]){
			ptr[i] = oio_mem_alloc(256, sizeof(char));
			CFStringGetCString(keys[i], ptr[i], 256, kCFStringEncodingUTF8);
		}
	}
	*names = ptr;
	return OIO_ERR_NONE;
}

void oio_hid_connectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device){
	t_oio *oio = (t_oio *)context;
	t_oio_hid_dev *dev = oio_hid_addDevice(oio, device);
	(int)IOHIDDeviceOpen(device, 0L);
	if(dev){
		oio_hid_dispatch(oio, oio->hid->connect_callbacks, strlen(dev->name), dev->name);
	}
}

void oio_hid_disconnectCallback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device){
	t_oio *oio = (t_oio *)context;
	t_oio_hid_dev *dev;
	oio_hid_util_getDeviceByDevice(oio, device, &dev);
	if(dev){
		oio_hid_dispatch(oio, oio->hid->disconnect_callbacks, strlen(dev->name), dev->name);
	}
	oio_hid_removeDevice(oio, device);
}

void oio_hid_valueCallback(void *context, IOReturn result, void *sender, IOHIDValueRef value){
	t_oio *oio = (t_oio *)context;
	t_oio_hid_dev *device;
	if(!(oio_hid_util_getDeviceByDevice(oio, (IOHIDDeviceRef)sender, &device))){
		long len;
		char *oscbuf = NULL;
		oio_hid_osc_encode(oio, &len, &oscbuf, device, value);
		oio_hid_dispatch(oio, device->input_value_callbacks, len, oscbuf);
		oio_mem_free(oscbuf);
	}
}

void oio_hid_dispatch(t_oio *oio, t_oio_hid_callbackList *callback_list, long n, char *osc){
	t_oio_hid_callbackList *cb = callback_list;
	while(cb){
		cb->f(oio, n, osc, cb->context);
		cb = cb->next;
	}
}

t_oio_err oio_hid_sendOSCBundleToDevice(t_oio *oio, int n, char *bundle){
	if(strncmp(bundle, "#bundle\0", 8)){
		return OIO_ERR_OSCBNDL;
	}
	char *ptr = bundle + 8;
	uint64_t timestamp = ntoh64(*((uint64_t *)ptr));
	ptr += 8;
	while(ptr - bundle < n){
		int size = ntoh32(*((uint32_t *)ptr));
		ptr += 4;
		char *address = ptr;
		while(*ptr++ != ','){}
		char typetag = *ptr;
		if(*ptr = '\0'){
			return OIO_ERR_OSCBNDL;
		}
		int num_data;
		while(*ptr++){
			num_data++;
		}

		while((ptr - bundle) % 4){
			ptr++;
		}
		uint64_t val;
		//int n = oio_osc_util_getUInt64(typetag, ptr, &val);
		oio_hid_sendValueToDevice(oio, address, timestamp, val);
		ptr += n;
	}
}

t_oio_err oio_hid_sendValueToDevice(t_oio *oio, const char *osc_string, uint64_t timestamp, uint64_t val){
	t_oio_hid_dev **dev;
	int n;
	if(oio_hid_util_getDevicesByOSCPattern(oio, osc_string, &n, &dev)){
		OIO_ERROR(OIO_ERR_DNF);
		return OIO_ERR_DNF;
	}
	const char *ptr = osc_string;
	// skip over the product name
	ptr++;
	while(*ptr++ != '/'){}
	// check to see if the next element of the address the instance number
	char *p = NULL;
	while(1){
		strtoul(ptr, &p, 0);
		if(ptr == p){
			// not a number--must be a part of the name
			while(*ptr++ != '/'){}
		}else{
			ptr = p;
			break;
		}
	}
	// ptr now contains the end of the string which should correspond to something that we can look up in the cookie table
	// or a cookie number

	int i;
	for(i = 0; i < n; i++){
		IOHIDDeviceRef device = dev[i]->device;
		uint32_t vid = -1, pid = -1;
		CFNumberRef cookie;
	        uint32_t c = strtoul(ptr + 1, &p, 0);
		if(p == (ptr + 1)){
			//oio_hid_util_getDeviceVendorID(device, &vid);
			//oio_hid_util_getDeviceProductID(device, &pid);
			c = oio_hid_strings_getCookie(oio, dev[i]->vendor_id, dev[i]->product_id, ptr);
		}
		cookie = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &c);

		CFMutableDictionaryRef mdict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFDictionarySetValue(mdict, CFSTR(kIOHIDElementCookieKey), cookie);
		CFArrayRef elements = IOHIDDeviceCopyMatchingElements(dev[i]->device, mdict, 0L);
		if(elements){
			int j;
			for(j = 0; j < CFArrayGetCount(elements); j++){
				IOHIDElementRef element = (IOHIDElementRef)CFArrayGetValueAtIndex(elements, j);
				IOHIDValueRef value = IOHIDValueCreateWithIntegerValue(kCFAllocatorDefault, element, mach_absolute_time(), val);
				IOHIDDeviceSetValue(dev[i]->device, element, value);
			}
		}
	}
	return OIO_ERR_NONE;
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
t_oio_hid_dev *oio_hid_addDevice(t_oio *oio, IOHIDDeviceRef device){
	t_oio_hid *hid = oio->hid;
	CFMutableDictionaryRef dict = hid->device_hash;
	char buf[256], pstring[256], dstring[256], mangled[256], *ptr = buf;
	memset(buf, '\0', 256);
	t_oio_err ret;
	uint32_t pid = -1, vid = -1;
	ret = oio_hid_util_getDeviceProductID(device, &pid);
	ret = oio_hid_util_getDeviceVendorID(device, &vid);

	if(ret = oio_hid_util_getDeviceProduct(device, 256, pstring)){
		OIO_ERROR(ret);
		return NULL;
	}else{
		oio_osc_util_makenice(pstring);
		ptr += sprintf(ptr, "/%s", pstring);
	}

	CFStringRef name = NULL;
	if(name = oio_hid_strings_getDeviceNameString(oio, vid, pid)){
		if(CFStringGetCString(name, dstring, 256, kCFStringEncodingUTF8) == true){
			// check to see if there is a / at the beg of the string
	      		ptr += sprintf(ptr, "%s", dstring);
		}
	}else{
	}
	//oio_osc_util_makenice(buf);
	int i = 1;
	t_oio_hid_dev *dev = NULL;
	while(i < 128){
		sprintf(mangled, "%s/%d", buf, i++);
		CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, mangled, kCFStringEncodingUTF8);
		// if the key is not in the dictionary, add it
		if(!CFDictionaryContainsKey((CFDictionaryRef)(dict), key)){
			dev = (t_oio_hid_dev *)oio_mem_alloc(1, sizeof(t_oio_hid_dev));
			strcpy(dev->name, mangled);
			dev->device = device;
			dev->prev = NULL;
			dev->input_value_callbacks = NULL;
			dev->product_id = pid;
			dev->vendor_id = vid;

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
					dev = (t_oio_hid_dev *)ptr;
					if(dev->device == device){
						break;
					}
				}
			}
		}
	}
	return dev;
}

void oio_hid_removeDevice(t_oio *oio, IOHIDDeviceRef device){
	t_oio_hid_dev *d;
	if(!(oio_hid_util_getDeviceByDevice(oio, device, &d))){
		if(d->prev){
			d->prev->next = d->next;
		}else{
			oio->hid->devices = d->next;
		}
		if(d->next){
			d->next->prev = d->prev;
		}

		t_oio_hid_callbackList *cb = d->input_value_callbacks;
		t_oio_hid_callbackList *next;
		while(cb){
			next = cb->next;
			oio_mem_free(cb);
			cb = next;
		}
		CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, d->name, kCFStringEncodingUTF8);
		CFDictionaryRemoveValue(oio->hid->device_hash, key);
		CFRelease(key);
		oio_mem_free(d);
	}
}

t_oio_err oio_hid_registerValueCallback(t_oio *oio, const char *name, t_oio_hid_callback f, void *context){
	t_oio_hid_dev **dev;
	int n;
	if(oio_hid_util_getDevicesByName(oio, name, &n, &dev)){
		OIO_ERROR(OIO_ERR_DNF);
		return OIO_ERR_DNF;
	}
	int i;
	for(i = 0; i < n; i++){
		if(dev[i]->input_value_callbacks == NULL){
			IOHIDDeviceRegisterInputValueCallback(dev[i]->device, oio_hid_valueCallback, oio);
		}
		t_oio_hid_callbackList *cb = (t_oio_hid_callbackList *)oio_mem_alloc(1, sizeof(t_oio_hid_callbackList));
		cb->f = f;
		cb->context = context;
		cb->next = dev[i]->input_value_callbacks;
		dev[i]->input_value_callbacks = cb;
	}
	if(dev){
		oio_mem_free(dev);
	}
	return OIO_ERR_NONE;
}

t_oio_err oio_hid_registerConnectCallback(t_oio *oio, t_oio_hid_callback f, void *context){
	t_oio_hid *hid = oio->hid;
	t_oio_hid_callbackList *cb = (t_oio_hid_callbackList *)oio_mem_alloc(1, sizeof(t_oio_hid_callbackList));
	cb->f = f;
	cb->context = context;
	cb->next = hid->connect_callbacks;
	hid->connect_callbacks = cb;

	return OIO_ERR_NONE;
}

t_oio_err oio_hid_registerDisconnectCallback(t_oio *oio, t_oio_hid_callback f, void *context){
	t_oio_hid *hid = oio->hid;
	t_oio_hid_callbackList *cb = (t_oio_hid_callbackList *)oio_mem_alloc(1, sizeof(t_oio_hid_callbackList));
	cb->f = f;
	cb->context = context;
	cb->next = hid->disconnect_callbacks;
	hid->disconnect_callbacks = cb;

	return OIO_ERR_NONE;
}

t_oio_err oio_hid_usageFile(t_oio *oio, const char *filename){
	t_oio_err ret;
	if(ret = oio_hid_strings_readUsageFile(oio, filename)){
		OIO_ERROR(ret);
	}
	return ret;
}

t_oio_err oio_hid_cookieFile(t_oio *oio, const char *filename){
	t_oio_err ret;
	if(ret = oio_hid_strings_readCookieFile(oio, filename)){
		OIO_ERROR(ret);
	}
	return ret;
}

void oio_hid_alloc(t_oio *oio, 
		   t_oio_hid_callback hid_connect_callback, 
		   void *hid_connect_context, 
		   t_oio_hid_callback hid_disconnect_callback, 
		   void *hid_disconnect_context, 
		   const char *hid_usage_plist, 
		   const char *hid_cookie_plist){
	t_oio_hid *hid = (t_oio_hid *)oio_mem_alloc(1, sizeof(t_oio_hid));
	oio->hid = hid;
	hid->devices = NULL;
	hid->disconnect_callbacks = NULL;
	hid->connect_callbacks = NULL;
	hid->device_hash = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	hid->cookie_strings_dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if(hid_usage_plist){
		oio_hid_usageFile(oio, hid_usage_plist);
	}
	if(hid_cookie_plist){
		oio_hid_cookieFile(oio, hid_cookie_plist);
	}
	if(hid_connect_callback){
		oio_hid_registerConnectCallback(oio, hid_connect_callback, hid_connect_context);
	}
	if(hid_connect_callback){
		oio_hid_registerDisconnectCallback(oio, hid_disconnect_callback, hid_disconnect_context);
	}
	hid->hidmanager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	IOHIDManagerSetDeviceMatching(hid->hidmanager, NULL);

	IOHIDManagerRegisterDeviceMatchingCallback(hid->hidmanager, oio_hid_connectCallback, (void *)oio);
	IOHIDManagerRegisterDeviceRemovalCallback(hid->hidmanager, oio_hid_disconnectCallback, (void *)oio);

	oio_hid_enumerateDevices(oio);
	oio_hid_run(oio);
}
