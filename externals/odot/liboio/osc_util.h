#ifndef __OSC_UTIL_H__
#define __OSC_UTIL_H__
#include "osc_byteorder.h"

typedef struct _osc_msg{
	uint32_t size;
	char *address;
	char *typetags;
	char *argv;
	int argc;
} t_osc_msg;

int osc_util_parseMessage(int n, char *buf, t_osc_msg *osc_msg);
int osc_util_getArgCount(t_osc_msg *msg);
int osc_util_getMsgCount(int n, char *osc_bndl);
int osc_util_parseBundle(int n, char *osc_bndl, t_osc_msg *msg);
int osc_util_parseBundleWithCallback(int n, char *osc_bndl, void (*f)(t_osc_msg, void *), void *context);
int osc_util_incrementArg(t_osc_msg *msg);
void osc_util_printBundle(int len, char *bundle, int (*p)(const char *, ...));
void osc_util_printBundleCbk(t_osc_msg msg, void *context);
void osc_util_printMsg(t_osc_msg *msg, int (*p)(const char *, ...));
long osc_util_bundle_naked_message(long n, char *ptr, char *out);
long osc_util_flatten(long n, char *ptr, char *out);
int osc_util_rename(char *buffer, 
		    int bufferLen, 
		    int bufferPos, 
		    t_osc_msg *msg, 
		    char *newAddress);
size_t osc_sizeof(unsigned char typetag, char *data);

#endif //__OSC_UTIL_H__
