// asyncfile.h
#ifndef __ASYNCFILE_H__
#define __ASYNCFILE_H__

// operations
#define ASYNCFILE_READ				1
#define ASYNCFILE_WRITE				2
#define ASYNCFILE_CLOSE				3
#define ASYNCFILE_GETEOF			4
#define ASYNCFILE_SETEOF			5

// flags
// no need for position flag, always from start
// no need for want result/callback flag, always set result + call cb if cb ptr valid
// look into chaching hints later
#define ASYNCFILE_DEFAULT			0x0000L
#define ASYNCFILE_SYNC				0x0001L		// simulate synchronous call
#define ASYNCFILE_NO_CACHE			0x0002L		// currently ignored

typedef method t_asyncfile_cb;

typedef struct _asyncfile 
{
	long		this_struct_is_opaque;
} t_asyncfile;

// should we make param struct opaque + use accessors?
typedef struct _asyncfile_params
{
	t_asyncfile		*file;
	long			operation;		// read/write/etc (internal use only)
	long			flags;			// in pb model would set ioResult,(a)sync, caching, position flags
	long			requestcount;
	long			actualcount;
	void			*buf;
	long			position;
	long			positionrfu;	// 64 bit
	t_asyncfile_cb	callback;
	void			*arg;
	long			result;

} t_asyncfile_params;

void asyncfile_init(void);
long asyncfile_create(char *name, short path, long perm, long type, t_asyncfile **f, long flags);
long asyncfile_close(t_asyncfile *f);
long asyncfile_read(t_asyncfile_params *params); 
long asyncfile_write(t_asyncfile_params *params); 
long asyncfile_seteof(t_asyncfile *f, long logeof);
long asyncfile_geteof(t_asyncfile *f, long *logeof);
t_asyncfile_cb asyncfile_callback_new(method m);
void asyncfile_callback_free(t_asyncfile_cb m);

// use following calls to allocate/free/reset asyncfile param structs
// this permits us to add additional struct fields if necessary
t_asyncfile_params *asyncfile_params_new(void);
void asyncfile_params_free(t_asyncfile_params *p);
void asyncfile_params_default(t_asyncfile_params *p);


#endif // __ASYNCFILE_H__
