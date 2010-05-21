#include "oio_obj.h"
#include "oio_hid.h"
#include "oio_midi.h"
#include "oio_mem.h"
#include <mach/mach_time.h>
#include <stdio.h>
#include "osc_util.h"
#include "osc_match.h"

t_oio_err oio_obj_sendOSCMessage(t_oio *oio, int n, char *buf, uint64_t timestamp);
t_oio_dev_type oio_obj_getDeviceType(t_oio *oio, char *name);

void oio_obj_alloc(t_oio_hid_callback hid_connect_callback, 
		     void *hid_connect_context, 
		     t_oio_hid_callback hid_disconnect_callback, 
		     void *hid_disconnect_context, 
		     char *hid_usage_plist, 
		     char *hid_cookie_plist,
		     t_oio_midi_callback midi_notify_callback,
		   void *midi_notify_context,
		   t_oio **oio){
	*oio = (t_oio *)oio_mem_alloc(1, sizeof(t_oio));
	oio_hid_alloc(*oio, 
		      hid_connect_callback, 
		      hid_connect_context, 
		      hid_disconnect_callback, 
		      hid_disconnect_context, 
		      hid_usage_plist, 
		      hid_cookie_plist);
	oio_midi_alloc(*oio,
		       midi_notify_callback,
		       midi_notify_context);
	return;
}

void oio_obj_run(t_oio *oio){
	oio_hid_run(oio);
}

t_oio_err oio_obj_sendOSC(t_oio *oio, int n, char *buf){
	if(strncmp(buf, "#bundle\0", 8)){
		// not a bundle--just do the message and bail
		return oio_obj_sendOSCMessage(oio, n, buf, mach_absolute_time());
	}
	// bundle
	char *ptr = buf + 8;
	uint64_t timestamp = ntoh64(*((uint64_t *)ptr));
	ptr += 8;
	t_oio_err ret = 0;
	while(ptr - buf < n){
		int size = ntoh32(*((uint32_t *)ptr));
		ptr += 4;
		if(ret = oio_obj_sendOSCMessage(oio, size, ptr, timestamp)){
			return ret;
		}
		ptr += size;
	}
	return OIO_ERR_NONE;
}

t_oio_err oio_obj_sendOSCMessage(t_oio *oio, int n, char *buf, uint64_t timestamp){
	char *address = buf + 1;
	char *typetags = buf + strlen(buf);
	while(*typetags++ != ','){}
	char *data = typetags + strlen(typetags) + 1;
	while((data - buf) % 4){
		data++;
	}
	
	char *ptr = address;
	while(*ptr != '/'){
		ptr++;
	}
	char device_name[128];
	strncpy(device_name, address, ptr - address);
	device_name[ptr - address] = '\0';

	switch(oio_obj_getDeviceType(oio, device_name)){
	case OIO_DEV_HID:

		break;
	}

	return OIO_ERR_NONE;
}

t_oio_dev_type oio_obj_getDeviceType(t_oio *oio, char *name){
	CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
	if(CFDictionaryContainsKey(oio->hid->device_hash, key)){
		return OIO_DEV_HID;
	}
	CFRelease(key);
	return OIO_DEV_DNF;
}

CFPropertyListRef oio_obj_getPlist(char *filepath){
	CFDataRef data = NULL;
	FILE *file = fopen(filepath, "r");
	if(file){
		int result = fseek(file, 0, SEEK_END);
		result = ftell(file);
		rewind(file);
		unsigned char buf[result];
		if(fread(buf, result, 1, file) > 0){
			data = CFDataCreate(NULL, buf, result);
		}
		fclose(file);
	}
	if(data){
		// this is deprecated in 10.6
		CFPropertyListRef plist = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, data, kCFPropertyListImmutable, NULL);
		// use this when we're sure people are on 10.6
		//CFPropertyListRef plist = CFPropertyListCreateWithData(kCFAllocatorDefault, data, kCFPropertyListImmutable, NULL, NULL);
		//CFShow(plist);
		CFRelease(data);
		return plist;
	}
	return NULL;
}

t_oio_err oio_obj_getDevicesByName(t_oio *oio, 
				   char *name, 
				   t_oio_generic_device *device_list, 
				   CFMutableDictionaryRef hash, 
				   int *num_devices, 
				   t_oio_generic_device ***matched_devices){
	char *pattern = name;
	int n = CFDictionaryGetCount(hash);
	*matched_devices = (t_oio_generic_device **)oio_mem_alloc(n, sizeof(t_oio_generic_device *));
	int i = 0;
	t_oio_generic_device *dd = device_list;
	while(dd){
		char *address = DEV_NAME(dd);
		int pattern_offset, address_offset;
		int ret = osc_match(pattern, address, &pattern_offset, &address_offset);
		if(ret & OSC_MATCH_ADDRESS_COMPLETE){
			//if(ret - address == strlen(address)){
			(*matched_devices)[i++] = dd;
		}
		dd = DEV_NEXT(dd);
	}
	*num_devices = i;
	if(i == 0){
		return OIO_ERR_DNF;
	}
	return OIO_ERR_NONE;
	/*
	t_oio_hid *hid = oio->hid;
	if(*name == '/'){
		// probably an OSC address
		return oio_hid_util_getDevicesByOSCPattern(oio, name, num_devices, devices);
	}
	CFMutableDictionaryRef dict = hid->device_hash;
	CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
	if(CFDictionaryContainsKey(dict, key)){
		const void *val;
		val = CFDictionaryGetValue(dict, key);
		if(val){
			long ptr;
			CFNumberGetValue((CFNumberRef)val, kCFNumberLongType, &ptr);
			if(ptr != 0){
				*devices = (t_oio_hid_dev **)oio_mem_alloc(1, sizeof(t_oio_hid_dev *));
				**devices = (t_oio_hid_dev *)ptr;
				*num_devices = 1;
				return OIO_ERR_NONE;
			}
		}
	}
	CFRelease(key);
	*num_devices = 0;
	return OIO_ERR_DNF;
	*/
}
