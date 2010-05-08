/*
To do:

callbacks that we register for must include a context parameter--the max wrapper has to get its object back when the callback happens.
the callbacks (value, connect, etc) should also get the t_oio obj (why not?)

 */

#include <stdlib.h>
#include "oio.h"
#include "oio_osc_util.h"

void print_devices(t_oio *oio);
void value_callback(t_oio *oio, long n, char *ptr, void *context);
void connect_callback(t_oio *oio, long n, char *ptr, void *context);
void disconnect_callback(t_oio *oio, long n, char *ptr, void *context);

int main(int argc, char **argv){
	/*
	CFDictionaryRef plist = (CFDictionaryRef)oio_osc_util_getPlist("/Users/john/suck.plist");
	CFShow(plist);
	char buf[32];
	sprintf(buf, "%d", 1356);
	CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, buf, kCFStringEncodingUTF8);
	const void *ptr;
	ptr = CFDictionaryGetValue(plist, key);
	CFShow((CFDictionaryRef)ptr);
	*/

	t_oio *oio = oio_obj_alloc();
	print_devices(oio);

	/*
	char line[256];
	char *p = line;
	while((*p++ = getchar()) != '\n'){
	}
	p--;
	*p = '\0';
	p = line;
	if(p){
		oio_hid_registerValueCallback(oio, p, value_callback);
	}
	*/

	//oio_hid_registerValueCallback(oio, "PLAYSTATION(R)3-Controller", value_callback);
	oio_hid_registerValueCallback(oio, "Apple-Internal-Keyboard---Trackpad", value_callback, NULL);
	oio_hid_registerValueCallback(oio, "Apple-Internal-Keyboard---Trackpad-2", value_callback, NULL);
	oio_hid_registerValueCallback(oio, "Apple-Internal-Keyboard---Trackpad-3", value_callback, NULL);
	oio_hid_registerValueCallback(oio, "Apple-Internal-Keyboard---Trackpad-4", value_callback, NULL);

	/*
	oio_hid_registerValueCallback(oio, "Apple-Keyboard", value_callback);
	oio_hid_registerValueCallback(oio, "Apple-Keyboard-2", value_callback);
	*/
	oio_hid_registerConnectCallback(oio, connect_callback, NULL);
	oio_hid_registerDisconnectCallback(oio, disconnect_callback, NULL);


	while(1){
		sleep(1);
	}

	return 0;
}

void print_devices(t_oio *oio){
	int i, n;
	char **names;
	oio_hid_getDeviceNames(oio, &n, &names);
	for(i = 0; i < n; i++){
		PP("%d: %s", i, names[i]);
		oio_mem_free(names[i]);
	}
	oio_mem_free(names);
}

void value_callback(t_oio *oio, long n, char *ptr, void *context){
	PP("%ld %p", n, ptr);
	oio_osc_util_printBundle(n, ptr, printf);
	oio_obj_sendOSC(oio, n, ptr);
}

void connect_callback(t_oio *oio, long n, char *ptr, void *context){
	PP("%s was connected", ptr);
}

void disconnect_callback(t_oio *oio, long n, char *ptr, void *context){
	PP("%s was disconnected", ptr);
}
