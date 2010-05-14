#ifndef __OIO_OSC_UTIL_H__
#define __OIO_OSC_UTIL_H__

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include "oio_err.h"

typedef struct{
	char word[16];
	char typetag;
} t_oio_osc_atom;

typedef struct _oio_osc_msg{
	char address[128];
	int natoms;
	t_oio_osc_atom *atoms;
	struct _oio_osc_msg *next;
} t_oio_osc_msg;

t_oio_err oio_osc_util_printMessage(int n, char *buf, int (*f)(const char *,...));
t_oio_err oio_osc_util_printBundle(int n, char *buf, int (*f)(const char *,...));
void oio_osc_util_makenice(char *buf);
uint64_t oio_osc_util_machAbsoluteToNTP(uint64_t ma);
uint64_t oio_osc_util_NTPToMachAbsolute(uint64_t ntp);
t_oio_err oio_osc_util_parseOSCBundle(int nbytes, char *osc_msg, t_oio_osc_msg **msg);
t_oio_err oio_osc_util_parseOSCMessage(int nbytes, char *osc_msg, t_oio_osc_msg *msg);
t_oio_err oio_osc_util_atomSet(t_oio_osc_atom *atom, void *buf, char typetag, char byteswap);
size_t sizeofosc(unsigned char typetag);

/*
#define OIO_OSC_ATOM_GETINT64(atom, val) {				\
		val = *((uint64_t *)((atom))) & ((uint64_t[]){0xff, 0xffff, 0x0, 0xffffffff, 0x0, 0x0, 0x0, 0xffffffffffffffff})[sizeofosc(((atom))->typetag) - 1]; \
	}
//#define OIO_OSC_ATOM_SETINT64(atom, val) *((uint64_t *)((atom))) = (uint64_t)val
#define OIO_OSC_ATOM_SETVALUE(atom, typetag, value) {\
	memcpy(((atom))->word, ((value)), sizeofosc(((typetag))));	\
	((atom))->word = (sizeofosc(((typetag))) == 8 ? hton64(*((uint64_t *)((atom))->word)) : (sizeofosc(((typetag))) == 4 ? hton32(*((uint32_t *)((atom))->word)) : ((atom))->word)); \
}

#define OIO_OSC_ATOM_GETFLOAT64(atom, val) {	\
		uint64_t v;			\
		OIO_OSC_ATOM_GETINT64(atom, v);	\
		val = *((double *)&v);		\
	}
#define OIO_OSC_ATOM_SETFLOAT64(atom, val) {				\
		double v = (double)val;					\
		OIO_OSC_ATOM_SETINT64(((atom)), (*((uint64_t *)(&((v)))))); \
	}
*/
#endif //__OIO_OSC_UTIL_H__
