#include "CNMAT_MMJ_common.h"

static char *CNMAT_resultStrings[] = {
	"Operation was SUCCESSful"
};

CNMAT_result CNMAT_MMJ_common_init(t_class *c){
	class_addmethod(c, (method)CNMAT_OSC_packet, "FullPacket", A_LONG, A_LONG, 0);
	return CNMAT_SUCCESS;
}

void CNMAT_OSC_packet(t_class *x, long size, long address){
	/*
	char *data = (char *)address;
	int pos = 0;
	OSCTimeTag timetag;
	char *message;
	if(!strncmp("#bundle", data, 8)){
		pos += 8; // skip over #bundle keyword
		timetag = getOSCTimeTag(data + pos);
		pos += 8; // time tag is 8 bytes
		long s = getOSCBundleSize(data + pos);
		pos += 4; // size is 4 bytes
		post("bundle size = %d", s);
		int i;
		for(i = 0; i < size; i++){
			if(data[i] == 0x0) post("%d ***", i);
			else post("%d %c %d", i, data[i], (int)data[i]);
		}
		message = (char *)getNullTerminatedBytes(s, data + pos);
		int len = strlen(message);
		len = len + (4 - (len % 4));
		pos += len;
		//post("message = %s", message);
	}else{

	}
*/
	if(object_method(x, gensym("nOsc"), 32) == 0) post("didn't work");
	else post("worked!");
}

OSCTimeTag getOSCTimeTag(char *data){
	OSCTimeTag tt;
	tt.seconds = ntohl(*((long *)(data)));
	tt.fraction = ntohl(*((long *)(data + 4)));
	return tt;
}

void *get4bytes(char *data){
	long l = ntohl(*((long *)data));
	return (void *)l;
}

void *getNullTerminatedBytes(int len, char *data){
	int counter = 0;
	int i;
	for(i = 0; i < len; i++){
		if(data[i] == 0x0) break;
		else ++counter;
	}
	char *d = (char *)calloc(counter, sizeof(char));
	memcpy(&d, data, sizeof(char) * counter);
	post("counter = %d", counter);
	return (void *)d;
}

long getOSCBundleSize(char *data){
	long size = ntohl(*((long *)data));
	return size;
}
/*
long getOSCLong(char *data){
	long l = ntohl(*((long *)data));
	return l;
}

float getOSCFloat(char *data){
	float f = ntohl(*((float *)data));
	return f;
}

char *getOSCString(char *data){

}
*/

char *CNMAT_getErrorString(CNMAT_result r){
	return CNMAT_resultStrings[r];
}
