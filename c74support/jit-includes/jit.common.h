/* 
	jit.common.h

	Copyright 2001-2004 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
		
*/

#ifndef __JIT_COMMON_H__
#define __JIT_COMMON_H__

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#include "jit.platform.h"
#include "jit.error.h"
#include "jit.file.h"
#include "jit.math.h"
#include "jit.symbols.h"
#include "jit.gworld.h"
#include "jit.qt.codec.h"
#include "jit.critical.h"

#ifndef TRUE
#define TRUE 	1
#endif
#ifndef FALSE
#define FALSE 	0
#endif

#define JIT_ATTR_GET_OPAQUE			0x00000001
#define JIT_ATTR_SET_OPAQUE			0x00000002
#define JIT_ATTR_GET_OPAQUE_USER	0x00000100
#define JIT_ATTR_SET_OPAQUE_USER	0x00000200
#define JIT_ATTR_GET_DEFER			0x00010000
#define JIT_ATTR_GET_USURP			0x00020000
#define JIT_ATTR_GET_DEFER_LOW		0x00040000
#define JIT_ATTR_GET_USURP_LOW		0x00080000
#define JIT_ATTR_SET_DEFER			0x01000000
#define JIT_ATTR_SET_USURP			0x02000000
#define JIT_ATTR_SET_DEFER_LOW		0x04000000
#define JIT_ATTR_SET_USURP_LOW		0x08000000

//t_jit_matrix_info flags
#define JIT_MATRIX_DATA_HANDLE		0x00000002
#define JIT_MATRIX_DATA_REFERENCE	0x00000004 	//data is someone else's
#define JIT_MATRIX_DATA_FLAGS_USE	0x00008000 	//necessary if using handle/reference data flags when creating
												//jit_matrix, however, it is never stored in matrix

#define JIT_MATRIX_MAX_DIMCOUNT		32 			//maximum dimension count
#define JIT_MATRIX_MAX_PLANECOUNT	32 			//maximum plane count

#define JIT_MATRIX_CONVERT_CLAMP	0x00000001  //not currently used
#define JIT_MATRIX_CONVERT_INTERP	0x00000002
#define JIT_MATRIX_CONVERT_SRCDIM	0x00000004
#define JIT_MATRIX_CONVERT_DSTDIM	0x00000008

typedef unsigned long 	ulong;
typedef unsigned int 	uint;
typedef unsigned short 	ushort;
typedef unsigned char 	uchar;

#include "jit.op.h"
#include "jit.linklist.h"

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

//for passing on the stack in method calls
typedef struct _stack_splat
{
	char b[64];
} t_stack_splat;

//common attr struct
typedef struct _jit_attr
{
	t_jit_object	ob;
	t_symbol		*name;
	t_symbol		*type;	
	long			flags;  		//public/private get/set methods
	method			get;    		//override default get method
	method			set;    		//override default set method
	void			*filterget;		//filterobject for get method
	void			*filterset; 	//filterobject for set method
	void			*reserved;		//for future use
	t_symbol		*regname;
} t_jit_attr;		

typedef struct _jit_matrix_info
{
	long			size;			// in bytes (0xFFFFFFFF=UNKNOWN)
	t_symbol		*type;			// primitive type
	long			flags;			// my data?, handle?
	long			dimcount;		
	long			dim[JIT_MATRIX_MAX_DIMCOUNT];		
	long			dimstride[JIT_MATRIX_MAX_DIMCOUNT]; // in bytes(array of size dimcount)
	long			planecount;		// # planes
} t_jit_matrix_info;


typedef struct _matrix_conv_info
{
	long 	flags;
	long	planemap[JIT_MATRIX_MAX_PLANECOUNT];
	long	srcdimstart[JIT_MATRIX_MAX_DIMCOUNT];		
	long	srcdimend[JIT_MATRIX_MAX_DIMCOUNT];		
	long	dstdimstart[JIT_MATRIX_MAX_DIMCOUNT];		
	long	dstdimend[JIT_MATRIX_MAX_DIMCOUNT];		
} t_matrix_conv_info;

long jit_method_true(void *x);
long jit_method_false(void *x);

void *jit_class_new(char *name, method mnew, method mfree, long size, ...);
t_jit_err jit_class_free(void *x);
t_jit_err jit_class_register(void *x);
t_jit_err jit_class_addmethod(void *x, method m, char *name, ...);
t_jit_err jit_class_addattr(void *x,t_jit_object *attr);
t_jit_err jit_class_addadornment(void *xx,t_jit_object *o);
void *jit_class_adornment_get(void *xx,t_symbol *classname);
t_symbol *jit_class_nameget(void *x);
void *jit_class_findbyname(t_symbol *classname);
long jit_object_classname_compare(void *x, t_symbol *name);
method jit_class_method(void *x, t_symbol *methodname);
void *jit_class_attrlist_get(void *x);
void *jit_class_attr_get(void *x, t_symbol *attrname);

void *jit_object_alloc(void *c);
void *jit_object_new(t_symbol *classname, ...);
t_jit_err jit_object_free(void *x);
void *jit_object_attrlist_get(void *x);
void *jit_object_method(void *x, t_symbol *s, ...);
method jit_object_getmethod(void *x, t_symbol *s);
t_symbol *jit_object_classname(void *x);
void * jit_object_register(void *x, t_symbol *s);
void *jit_object_findregistered(t_symbol *s);
t_symbol *jit_object_findregisteredbyptr(void *x);
t_jit_err jit_object_unregister(void *x);
void *jit_object_attach(t_symbol *s, void *x);
t_jit_err jit_object_detach(t_symbol *s, void *x);
t_jit_err jit_object_notify(void *x, t_symbol *s, void *data);
void *jit_object_class(void *x);
void *jit_robject_findbyptr(void *x);

//memory functions
void *jit_getbytes(long size);
void *jit_freebytes(void *ptr,long size);
void *jit_handle_new(long size);
void jit_handle_free(void **handle);
long jit_handle_size_get(void **handle);
t_jit_err jit_handle_size_set(void **handle, long size);
long jit_handle_lock(void **handle, long lock);
void jit_copy_bytes(void *dest, const void *src, long bytes);
long jit_freemem(void);
char *jit_newptr(long size);
void jit_disposeptr(char *ptr);


//atom functions
t_jit_err jit_atom_setlong(t_atom *a, long b);
t_jit_err jit_atom_setfloat(t_atom *a, double b);
t_jit_err jit_atom_setsym(t_atom *a, t_symbol *b);				
t_jit_err jit_atom_setobj(t_atom *a, void *b);
long jit_atom_getlong(t_atom *a);
double jit_atom_getfloat(t_atom *a);
t_symbol *jit_atom_getsym(t_atom *a);
void *jit_atom_getobj(t_atom *a);
long jit_atom_getcharfix(t_atom *a);
//the following are useful for setting the values _only_ if there is an arg
//rather than setting it to 0 or _jit_sym_nothing
long jit_atom_arg_getlong(long *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getfloat(float *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getdouble(double *c, long idx, long ac, t_atom *av);
long jit_atom_arg_getsym(t_symbol **c, long idx, long ac, t_atom *av);

//matrix info utils
t_jit_err jit_matrix_info_default(t_jit_matrix_info *info);
long jit_matrix_info_typesize(t_jit_matrix_info *minfo); 

//mop utils
t_jit_err jit_mop_single_type(void *x, t_symbol *s);
t_jit_err jit_mop_single_planecount(void *x, long c);
t_jit_err jit_mop_methodall(void *x, t_symbol *s, ...);
t_jit_err jit_mop_input_nolink(void *mop, long c);
t_jit_err jit_mop_output_nolink(void *mop, long c);
t_jit_err jit_mop_ioproc_copy_adapt(void *mop, void *mop_io, void *matrix);
t_jit_err jit_mop_ioproc_copy_trunc(void *mop, void *mop_io, void *matrix);
t_jit_err jit_mop_ioproc_copy_trunc_zero(void *mop, void *mop_io, void *matrix);

//attr functions
long max_jit_attr_args_offset(short ac, t_atom *av);
void max_jit_attr_args(void *x, short ac, t_atom *av);
//for easy access of simple attributes
long jit_attr_getlong(void *x, t_symbol *s);
t_jit_err jit_attr_setlong(void *x, t_symbol *s, long c);
float jit_attr_getfloat(void *x, t_symbol *s);
t_jit_err jit_attr_setfloat(void *x, t_symbol *s, float c);
t_symbol *jit_attr_getsym(void *x, t_symbol *s);
t_jit_err jit_attr_setsym(void *x, t_symbol *s, t_symbol *c);
long jit_attr_getlong_array(void *x, t_symbol *s, long max, long *vals);
t_jit_err jit_attr_setlong_array(void *x, t_symbol *s, long count, long *vals);
long jit_attr_getchar_array(void *x, t_symbol *s, long max, uchar *vals);
t_jit_err jit_attr_setchar_array(void *x, t_symbol *s, long count, uchar *vals);
long jit_attr_getfloat_array(void *x, t_symbol *s, long max, float *vals);
t_jit_err jit_attr_setfloat_array(void *x, t_symbol *s, long count, float *vals);
long jit_attr_getdouble_array(void *x, t_symbol *s, long max, double *vals);
t_jit_err jit_attr_setdouble_array(void *x, t_symbol *s, long count, double *vals);
long jit_attr_getsym_array(void *x, t_symbol *s, long max, t_symbol **vals);
t_jit_err jit_attr_setsym_array(void *x, t_symbol *s, long count, t_symbol **vals);

//attr filters util
t_jit_err jit_attr_addfilterset_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterset_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterget_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterget_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilter_clip(void *x, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilter_clip_scale(void *x, double scale, double min, double max, long usemin, long usemax);
t_jit_err jit_attr_addfilterset_proc(void *x, method proc);
t_jit_err jit_attr_addfilterget_proc(void *x, method proc);

//more util functions
void jit_rand_setseed(long n);
long jit_rand(void);
t_symbol *jit_symbol_unique();
void jit_error_code(void *x,t_jit_err v); //interrupt safe
void jit_error_sym(void *x,t_symbol *s); //interrupt safe
void jit_post_sym(void *x,t_symbol *s);  //interrupt safe

//util macros
#define CLIP(x,a,b) (x)=(x)<(a)?(a):(x)>(b)?(b):(x)
#define CLAMP(x,a,b) (x)<(a)?(a):(x)>(b)?(b):(x)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ABS(x) ((x)<0?-(x):(x))
#define	SET_FLAG(word, flag)	((word) |= (flag))
#define	CLEAR_FLAG(word, flag)	((word) &= ~(flag))
#define GET_FLAG(word, flag)	((word) & (flag))

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __JIT_COMMON_H__
