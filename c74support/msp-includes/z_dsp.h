// z_dsp.h -- the main header file for all DSP objects copyright 1997-2003 Cycling '74

// DSP services:

#ifndef _Z_DSP_H
#define _Z_DSP_H

#ifndef SAMPLE_TYPEDEF
typedef int t_int;
typedef float t_sample, t_float;
#define SAMPLE_TYPEDEF
#endif

#include "z_altivec.h"

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
	SYS_MAXBLKSIZE = 2048,	// a good number for a maximum signal vector size
	SYS_MAXSIGS = 250		// number of signal inlets you can have in an object
};

// global variables

#ifdef SYSOBJ

#ifdef MAC_VERSION
#define EXT_SYSOBJ		extern
#endif //MAC_VERSION

#ifdef WIN_VERSION
#ifdef WIN_MAL_VERSION
#define EXT_SYSOBJ		extern __declspec(dllexport)
#else
#define EXT_SYSOBJ		extern __declspec(dllimport)
#endif WIN_MAL_VERSION
#endif // WIN_VERSION

EXT_SYSOBJ float **sys_soundout;
EXT_SYSOBJ float **sys_soundin;
EXT_SYSOBJ long sys_blksize;
EXT_SYSOBJ t_int *dsp_chain;
EXT_SYSOBJ long sys_hasadc;
EXT_SYSOBJ short dac_chanused[];
EXT_SYSOBJ short adc_chanused[];
EXT_SYSOBJ int dsp_chainsize;
EXT_SYSOBJ long sys_ioblksize;
EXT_SYSOBJ double sys_sr;
EXT_SYSOBJ long sys_isr;

#endif // SYSOBJ

// macro loop for checking for NAN/INF

// note: this may be platform-dependent

#ifdef MAC_VERSION

#define NAN_MASK 0x7F800000

#define NAN_CHECK(n,o) \
while (--n) { if ((*++(o) & NAN_MASK) == NAN_MASK) *(o) = 0; }

#define IS_DENORM_FLOAT(v)		((((*(unsigned long *)&(v))&0x7f800000)==0)&&((v)!=0.f))
#define IS_DENORM_DOUBLE(v)		((((((unsigned long *)&(v))[0])&0x7fe00000)==0)&&((v)!=0.))			

#define IS_NAN_FLOAT(v)			(((*(unsigned long *)&(v))&0x7f800000)==0x7f800000)
#define IS_NAN_DOUBLE(v)		(((((unsigned long *)&(v))[0])&0x7fe00000)==0x7fe00000) 

#define IS_DENORM_NAN_FLOAT(v)		(IS_DENORM_FLOAT(v)||IS_NAN_FLOAT(v))
#define IS_DENORM_NAN_DOUBLE(v)		(IS_DENORM_DOUBLE(v)||IS_NAN_DOUBLE(v))			

#define FIX_DENORM_FLOAT(v)		
#define FIX_DENORM_DOUBLE(v)	

#define FIX_DENORM_NAN_FLOAT(v)		
#define FIX_DENORM_NAN_DOUBLE(v)

#endif

#ifdef WIN_VERSION

#define NAN_MASK 0x7F800000

#define NAN_CHECK(n,o) \
while (--n) { if ((*++(o) & NAN_MASK) == NAN_MASK) *(o) = 0; }

#define DENORM_WANT_FIX		1

#define IS_DENORM_FLOAT(v)		((((*(unsigned long *)&(v))&0x7f800000)==0)&&((v)!=0.f))
#define IS_DENORM_DOUBLE(v)		((((((unsigned long *)&(v))[1])&0x7fe00000)==0)&&((v)!=0.))			

#define IS_NAN_FLOAT(v)			(((*(unsigned long *)&(v))&0x7f800000)==0x7f800000)
#define IS_NAN_DOUBLE(v)		(((((unsigned long *)&(v))[1])&0x7fe00000)==0x7fe00000) 

#define IS_DENORM_NAN_FLOAT(v)		(IS_DENORM_FLOAT(v)||IS_NAN_FLOAT(v))
#define IS_DENORM_NAN_DOUBLE(v)		(IS_DENORM_DOUBLE(v)||IS_NAN_DOUBLE(v))			

#define FIX_DENORM_FLOAT(v)		((v)=IS_DENORM_FLOAT(v)?0.f:(v))
#define FIX_DENORM_DOUBLE(v)	((v)=IS_DENORM_DOUBLE(v)?0.f:(v))

#define FIX_DENORM_NAN_FLOAT(v)		((v)=IS_DENORM_NAN_FLOAT(v)?0.f:(v))
#define FIX_DENORM_NAN_DOUBLE(v)	((v)=IS_DENORM_NAN_DOUBLE(v)?0.:(v))

#endif


// header for all DSP objects. Provides a proxy.

// z_misc flags:

#define Z_NO_INPLACE 1			// flag indicating the object doesn't want signals in place
#define Z_PUT_LAST 2			// when list of ugens is resorted, put this object at end
#define Z_PUT_FIRST 4			// when list of ugens is resorted, put this object at beginning

typedef void *t_proxy;

typedef struct t_pxobject {
	struct object z_ob;
	long z_in;
	t_proxy *z_proxy;
	long z_disabled;
	short z_count;
	short z_misc;
} t_pxobject;

#ifdef __EXT_USER

typedef struct t_pxbox {
	struct box z_box;
	long z_in;
	t_proxy *z_proxy;
	long z_disabled;
	short z_count;
	short z_misc;
} t_pxbox;

#else

typedef void *t_pxbox;

#endif

// what is bound to the symbol "_conninfo"

typedef struct t_conninfo {
	struct object *c_ob;
	long c_no;
} t_conninfo;

// the signal data structure

#define MAXLOGSIG 13
#define MAXSIGSIZE (1 << MAXLOGSIG)

typedef struct _signal
{
    long s_n;
    t_sample *s_vec;
    float s_sr;
    struct _signal *s_next;
    struct _signal *s_nextused;
    short s_refcount;
    short s_size;					// element size (* s_n == memory)
    char *s_ptr;					// what to free
} t_signal;

typedef struct _dspchain
{
	t_object c_ob;
	t_int *c_chain;
	long c_chainsize;
	long c_callcount;
	long c_usedcount;
	long c_reusedcount;
	long c_freedcount;
	long c_sr;
	long c_bs;
	t_signal *c_usedlist;
	t_signal *c_freelist;
	t_signal *c_zero;
	struct _ugenbox *c_ugenlist;	// temporary variable, allows reentrant compiling
	struct _dspchain *c_prev;
	void *c_patcher;	// top-level parent or 0 if global
	void *c_inlets;		// collection of inlets
	void *c_outlets;	// collection of outlets
	void *c_inputs;		// signal input list (zero before first exec)
	void *c_outputs; 	// signal output list
	// have to determine whether freelist is global or local
	long c_broken;		// object being freed, don't run it
	long c_intype;		// using old signal linklist (0) or array
	long c_outtype;		// using old signal linklist (0) or array
} t_dspchain;

typedef t_int *(*t_perfroutine)(t_int *args);

#ifndef t_vptr
typedef void *t_vptr;
#endif // t_vptr

// useful define

#ifndef PI
#define PI 3.14159265358979323846
#endif
#ifndef TWOPI
#define TWOPI		6.28318530717958647692
#endif  TWOPI
#ifndef PIOVERTWO
#define PIOVERTWO	1.57079632679489661923
#endif  PIOVERTWO

// system access prototypes

#ifdef __cplusplus
extern "C" {
#endif

// messages to the dsp object

void *dspmess(t_symbol *mess);

// access to DSP system variables

int sys_getmaxblksize(void);	// returns current max blk size
int sys_getblksize(void);		// returns current blk size
float sys_getsr(void);			// returns current sampling rate
int sys_getch(void);			// returns current number of channels

int sys_optimize(void);			// returns whether to optimize or not
int sys_altivec(void);			// returns whether machine has vector processing

int sys_getdspstate(void);		// returns whether audio is on or off

// controlling the DSP

void canvas_start_dsp(void);
void canvas_stop_dsp(void);
void dsp_tick(void);			// no longer used

// the dspchain object

t_dspchain *dspchain_start(long bs, long sr);
t_dspchain *dspchain_get(void);
t_dspchain *dspchain_compile(t_patcher *p, long bs, long sr);
t_dspchain *dspchain_compile2(t_patcher *p, t_dspchain *x);
void dspchain_tick(t_dspchain *c);
void canvas_start_onedsp(t_patcher *p, t_dspchain **c, long bs, long sr); 
void canvas_stop_onedsp(t_dspchain *c);

// utility perform routines

void set_zero(float *dst, long n);
t_int *plus_perform(t_int *args);
t_int *sig_perform(t_int *args);
t_int *copy_perform(t_int *args);
t_int *plus2_perform(t_int *w);

// setup routines used by DSP objects

void dsp_add(t_perfroutine f, int n, ...);			// called in dsp method
void dsp_addv(t_perfroutine f, int n, void **vector);	 // called in dsp method

void z_dsp_setup(t_pxobject *x, long nsignals);		// called in new method
#define dsp_setup z_dsp_setup

void z_dsp_free(t_pxobject *x);						// default object free
#define dsp_free z_dsp_free

void z_add_signalmethod(void);						// called in initialization routine
#define dsp_initclass z_add_signalmethod

void z_sysinit(void);
#define dsp_sysinit z_sysinit

void z_box_dsp_setup(t_pxbox *x, long nsignals);		// called in new method
#define dsp_setupbox z_box_dsp_setup

void z_box_dsp_free(t_pxbox *x);						// default object free
#define dsp_freebox z_box_dsp_free

void z_box_add_signalmethod(void);						// called in initialization routine
#define dsp_initboxclass z_box_add_signalmethod

void z_patcher_for_dsp(t_object *x);
#define dsp_patcher z_patcher_for_dsp

void dsp_setpatcher(void *p);

short z_isconnected(t_object *x, t_object *dst, short *index);
#define dsp_isconnected z_isconnected

short z_dsp_setloadupdate(short way);
#define dsp_setloadupdate z_dsp_setloadupdate

void *dsp_setpostprocess(method pm);

// used only by audio driver objects

void sys_setprocessflag(short way);

// lame audio file utility (do not use)

short aiff_parse(char *header, long *offset, long *size, long *nchans, long *ssize,
	long *srate, void *chunk, void *markers);

// memory utilities

void *t_resizebytes(char *old, long oldsize, long newsize);
void *t_getbytes(long size);
void *t_freebytes(void *fatso, long size);

// atom utilities

t_int atom_getintarg(short which, short argc, t_atom *argv);
float atom_getfloatarg(short which, short argc, t_atom *argv);
t_symbol *atom_getsymarg(short which, short argc, t_atom *argv);

#ifdef MAC_VERSION
#define PROXY_GETINLET(x) proxy_getinlet? proxy_getinlet(x) : (((t_pxobject *)x)->z_in)
#endif
#ifdef WIN_VERSION
// On windows we always have this function, and this stops the warning ($rbs$)
#define PROXY_GETINLET(x) proxy_getinlet(x)
#endif

void class_dspinit(t_class *c);
void class_dspinitbox(t_class *c);

#ifdef __cplusplus
}
#endif

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#endif // _Z_DSP_H

