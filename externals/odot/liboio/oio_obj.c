#include "oio_obj.h"
#include "oio_mem.h"
#include "oio_hid.h"
#include <mach/mach_time.h>


t_oio_err oio_obj_sendOSCMessage(t_oio *oio, int n, char *buf, uint64_t timestamp);
t_oio_dev_type oio_obj_getDeviceType(t_oio *oio, char *name);

t_oio *oio_obj_alloc(void){
	t_oio *oio = (t_oio *)oio_mem_alloc(1, sizeof(t_oio));
	oio_hid_alloc(oio);
	return oio;
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
	return OIO_DEV_DNF;
}

CFPropertyListRef oio_obj_getPlist(const char *filepath){
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
		CFPropertyListRef plist = CFPropertyListCreateWithData(kCFAllocatorDefault, data, kCFPropertyListImmutable, NULL, NULL);
		//CFShow(plist);
		CFRelease(data);
		return plist;
	}
	return NULL;
}
