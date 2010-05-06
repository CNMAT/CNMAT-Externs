#ifndef __OIO_OBJ_H__
#define __OIO_OBJ_H__

#define PP(s, ...) printf("%s[%d]:%s(): "s"\n", basename(__FILE__), __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

typedef void (*t_oio_callback)(long, char*);

typedef struct _oio{
	struct _oio_hid *hid;
} t_oio;

t_oio *oio_obj_alloc(void);

#endif // __OIO_OBJ_H__
