#include "ext.h"
#include "ext_obex.h"
#include "OSC-client.h"

typedef enum{
	CNMAT_success = 0
}CNMAT_result;

CNMAT_result CNMAT_MMJ_common_init(t_class *c);
void CNMAT_OSC_packet(t_class *x, long size, long address);
OSCTimeTag getOSCTimeTag(char *data);
long getOSCBundleSize(char *data);
void *get4bytes(char *data);
void *getNullTerminatedBytes(int len, char *data);
char *CNMAT_getErrorString(CNMAT_result r);
