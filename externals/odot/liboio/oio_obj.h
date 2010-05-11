#ifndef __OIO_OBJ_H__
#define __OIO_OBJ_H__

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <libgen.h>
#include "oio_err.h"

typedef enum _oio_dev_type{
	OIO_DEV_DNF,
	OIO_DEV_HID,
	OIO_DEV_MIDI,
	OIO_DEV_SERIAL
} t_oio_dev_type;

#define OIO_BYTE_SWAP64(x) \
	((((x) & 0xff00000000000000LL) >> 56) | \
	 (((x) & 0x00ff000000000000LL) >> 40) | \
	 (((x) & 0x0000ff0000000000LL) >> 24) | \
	 (((x) & 0x000000ff00000000LL) >> 8)  | \
	 (((x) & 0x00000000ff000000LL) << 8)  | \
	 (((x) & 0x0000000000ff0000LL) << 24) | \
	 (((x) & 0x000000000000ff00LL) << 40) | \
	 (((x) & 0x00000000000000ffLL) << 56))

#define OIO_BYTE_SWAP32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >> 8) | \
	 (((x) & 0x0000ff00) << 8) | \
	 (((x) & 0x000000ff) << 24))

#ifdef BYTE_ORDER
#define OIO_BYTE_ORDER BYTE_ORDER
#else
#ifdef __BYTE_ORDER
#define OIO_BYTE_ORDER __BYTE_ORDER
#endif
#endif

#ifdef LITTLE_ENDIAN
#define OIO_LITTLE_ENDIAN LITTLE_ENDIAN
#else
#ifdef __LITTLE_ENDIAN
#define OIO_LITTLE_ENDIAN __LITTLE_ENDIAN
#endif
#endif

#if OIO_BYTE_ORDER == OIO_LITTLE_ENDIAN
#define hton32(x) OIO_BYTE_SWAP32(x)
#define ntoh32(x) OIO_BYTE_SWAP32(x)
#define hton64(x) OIO_BYTE_SWAP64(x)
#define ntoh64(x) OIO_BYTE_SWAP64(x)
#else
#define hton32(x) (x)
#define ntoh32(x) (x)
#define hton64(x) (x)
#define ntoh64(x) (x)
#endif

#define PP(s, ...) printf("%s[%d]:%s(): "s"\n", basename(__FILE__), __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

typedef struct _oio{
	struct _oio_hid *hid;
} t_oio;

typedef void (*t_oio_callback)(t_oio *, long, char*, void *);

t_oio *oio_obj_alloc(void);
t_oio_err oio_obj_sendOSC(t_oio *oio, int n, char *buf);
CFPropertyListRef oio_obj_getPlist(const char *filepath);

#endif // __OIO_OBJ_H__
