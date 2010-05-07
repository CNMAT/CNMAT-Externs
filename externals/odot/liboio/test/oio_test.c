#include <stdlib.h>
#include "oio.h"

void print_devices(t_oio *oio);
void value_callback(long n, char *ptr);
void connect_callback(long n, char *ptr);
void disconnect_callback(long n, char *ptr);

int main(int argc, char **argv){
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

	oio_hid_registerValueCallback(oio, "Apple Internal Keyboard / Trackpad", value_callback);
	oio_hid_registerValueCallback(oio, "Apple Internal Keyboard / Trackpad 1", value_callback);
	oio_hid_registerValueCallback(oio, "Apple Internal Keyboard / Trackpad 2", value_callback);
	oio_hid_registerValueCallback(oio, "Apple Internal Keyboard / Trackpad 3", value_callback);

	/*
	oio_hid_registerValueCallback(oio, "Apple Keyboard", value_callback);
	oio_hid_registerValueCallback(oio, "Apple Keyboard 1", value_callback);
	*/
	oio_hid_registerConnectCallback(oio, connect_callback);
	oio_hid_registerDisconnectCallback(oio, disconnect_callback);

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

void value_callback(long n, char *ptr){
	PP("%ld %s", n, ptr);
}

void connect_callback(long n, char *ptr){
	PP("%s was connected", ptr);
}

void disconnect_callback(long n, char *ptr){
	PP("%s was disconnected", ptr);
}
