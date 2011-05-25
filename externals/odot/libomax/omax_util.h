/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2009-11, The Regents of
  the University of California (Regents). 
  Permission to use, copy, modify, distribute, and distribute modified versions
  of this software and its documentation without fee and without a signed
  licensing agreement, is hereby granted, provided that the above copyright
  notice, this paragraph and the following two paragraphs appear in all copies,
  modifications, and distributions.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
  HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#ifndef __OMAX_UTIL_H__
#define __OMAX_UTIL_H__

#include "osc.h"
#include "ext.h"
#include "ext_obex.h"

#define __USE_OMAX_ATOM_SETTERS__

#ifdef __USE_OMAX_ATOM_SETTERS__
#define atom_setlong(ap, x) *(ap) = (t_atom){A_LONG, (union word)((long)(x))}
#define atom_setfloat(ap, x) *(ap) = (t_atom){A_FLOAT, (union word)((float)(x))}
#define atom_setsym(ap, x) *(ap) = (t_atom){A_SYM, (union word)(x)}
#endif

#define __USE_OMAX_ATOM_GETTERS__

#ifdef __USE_OMAX_ATOM_GETTERS__
inline long omax_atom_getlong(t_atom *ap);
inline float omax_atom_getfloat(t_atom *ap);
inline t_symbol *omax_atom_getsym(t_atom *ap);
#define atom_getlong(ap) omax_atom_getlong(ap)
#define atom_getfloat(ap) omax_atom_getfloat(ap)
#define atom_getsym(ap) omax_atom_getsym(ap)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define A64_LONG 1
#define A64_FLOAT 2
#define A64_SYM 3
#define A64_PTR 4

typedef struct _atom64{
	union _word{
		int64_t w_long;
		double w_float;
		t_symbol *w_sym;
		void *w_ptr;
	} a64_word;
	int type;
} t_atom64;

void atom64_setlong(t_atom64 *a, int64_t l);
void atom64_setfloat(t_atom64 *a, double d);
void atom64_setsym(t_atom64 *a, t_symbol *s);
void atom64_setptr(t_atom64 *a, void *p);

int64_t atom64_getlong(t_atom64 *a);
double atom64_getfloat(t_atom64 *a);
t_symbol *atom64_getsym(t_atom64 *a);
void *atom64_getptr(t_atom64 *a);

/**
 * An object for holding OSC-related instance data
 */
typedef struct _omax{
	void *continuation_outlet;
	void *info_outlet;
} t_omax;

#define OMAX_METHOD 0
#define OMAX_PARAMETER 1
//#define OMAX_SETTER 1
//#define OMAX_GETTER 2

// from sdk:
/*
typedef struct messlist
{
	struct symbol *m_sym;		///< Name of the message
	method m_fun;				///< Method associated with the message
	char m_type[MAXARG + 1];	///< Argument type information
} Messlist, t_messlist;
 */

typedef struct _omax_method{
	int type;
	//t_messlist m;
	//method m;
	t_symbol *sym;
	void *thing;
} t_omax_method;

//hashtab_store(omax_class_getHashtab(class_nameget(c)->s_name), omax_class_makeMethodNameOSC(class_nameget(c)->s_name, name), (t_object *)m); }

#define OMAX_ADDMETHOD(c, functionpointer, name, ...)\
	OMAX_ADDMETHOD_OSCNAME(c, functionpointer, name, omax_class_makeMethodNameOSC(class_nameget(c)->s_name, name)->s_name, __VA_ARGS__)

#define OMAX_ADDMETHOD_OSCNAME(c, functionpointer, name, oscname, ...)		\
	{ class_addmethod(c, (method)functionpointer, name, __VA_ARGS__); \
	t_omax_method *m = (t_omax_method *)sysmem_newptr(sizeof(t_omax_method)); \
	m->type = OMAX_METHOD;						\
	m->sym = gensym(name);						\
	t_messlist *messlist = (t_messlist *)sysmem_newptr(sizeof(t_messlist));	\
	messlist->m_fun = functionpointer;				\
	messlist->m_sym = gensym(name);					\
	char tmp[MAXARG + 1] = {__VA_ARGS__};				\
	memcpy(messlist->m_type, tmp, MAXARG + 1);			\
	m->thing = messlist;\
	omax_class_addToSchemaList(c, oscname);\
	hashtab_store(omax_class_getHashtab(class_nameget(c)->s_name), gensym(oscname), (t_object *)m); }

#define OMAX_ADDPARAM(c, name, datatype, flags, structname, structmember, setter, getter) \
	{ t_attr *attr = (t_attr *)attr_offset_new(name, USESYM(datatype), (flags), (method)getter, \
					 (method)setter, calcoffset(structname, structmember));	\
			class_addattr((c), (t_object *)attr);			\
		t_omax_method *m = (t_omax_method *)sysmem_newptr(sizeof(t_omax_method)); \
		m->type = OMAX_PARAMETER;				\
		m->sym = gensym(name);					\
		m->thing = attr;					\
		omax_class_addToSchemaList(c, omax_class_makeParamNameOSC(class_nameget(c)->s_name, name)->s_name);\
		hashtab_store(omax_class_getHashtab(class_nameget(c)->s_name), omax_class_makeParamNameOSC(class_nameget(c)->s_name, name), (t_object *)m); }

/*
#define OMAX_ADDPARAM(c, name, datatype, flags, structname, structmember, setter, getter) \
	{ class_addattr((c), attr_offset_new(name, USESYM(datatype), (flags), (method)getter, \
					     (method)setter, calcoffset(structname, structmember))); \
		t_omax_method *m = (t_omax_method *)sysmem_newptr(sizeof(t_omax_method)); \
		m->type = OMAX_SETTER;					\
		long get; t_object *attr = NULL;			\
		m->m.m_fun = class_attr_method(c, gensym(name), (void **)(&attr), &get); \
		m->m.m_sym = gensym(name);				\
		hashtab_store(omax_class_getHashtab(class_nameget(c)->s_name), omax_class_makeSetterNameOSC(class_nameget(c)->s_name, name), (t_object *)m); \
		m = (t_omax_method *)sysmem_newptr(sizeof(t_omax_method)); \
		m->type = OMAX_GETTER;					\
		get = 0; attr = NULL;					\
		char buf[128];						\
		sprintf(buf, "get%s", name);				\
		m->m.m_fun = class_attr_method(c, gensym(buf), (void **)(&attr), &get); \
		m->m.m_sym = gensym(name);				\
		hashtab_store(omax_class_getHashtab(class_nameget(c)->s_name), omax_class_makeGetterNameOSC(class_nameget(c)->s_name, name), (t_object *)m); }
*/	

t_class *omax_class_new(char *name, method ctor, method dtor, size_t structsize, size_t osc_ob_offset, method fullpacket_method);
void omax_init(t_object *x);
void omax_addnotification(t_class *c, method fp);
void omax_FullPacket(t_object *x, long len, long ptr);
void omax_FullPacketCallback(t_osc_msg msg, void *context);
void omax_class_addToSchemaList(t_class *c, const char *address);
void omax_schemaList(t_object *ob, t_symbol *msg, int argc, t_atom *argv);
void omax_object_ioReport(t_object *x, t_symbol *msg, int argc, t_atom *argv);
void omax_object_createIOReport(t_object *x, t_symbol *msg, int argc, t_atom *argv, int *buflen, char **buf);
void omax_outputState(t_object *x);

t_max_err omax_notify(t_object *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

t_hashtab *omax_class_getHashtab(char *classname);
t_symbol *omax_class_makeHashtabName(char *classname);

t_symbol *omax_class_makeMethodNameOSC(char *classname, char *methodname);
t_symbol *omax_class_makeParamNameOSC(char *classname, char *paramname);
t_symbol *omax_class_makeSetterNameOSC(char *classname, char *methodname);
t_symbol *omax_class_makeGetterNameOSC(char *classname, char *methodname);
t_symbol *omax_class_makeAccessorNameOSC(char *classname, char *methodname, char *type);
t_symbol *omax_class_makeSetterName(char *methodname);
t_symbol *omax_class_makeGetterName(char *methodname);

t_omax *omax_object_getOmaxObj(t_object *ob);
void *omax_object_getContinuationOutlet(t_object *ob);
void *omax_object_getInfoOutlet(t_object *ob);
method omax_object_getNotificationCallback(t_object *ob);

void omax_util_oscMsg2MaxAtoms(t_osc_msg *msg, long *ac, t_atom *av);
int osc_util_make_bundle(int numAddresses,
			  t_symbol **addresses, 
			  int *numArgs,
			  char **typetags, 
			  t_atom **args, 
			  int *len, 
			 char *buffer);
int osc_util_check_pos_and_resize(char *buf, int len, char *pos);
int omax_util_get_bundle_size_for_atoms(t_symbol *address, int argc, t_atom *argv);
int osc_util_make_bundle_from_atoms(long argc, t_atom *argv, int *len, char *buffer);

char omax_util_typetagForAtom(t_atom *a);
int omax_util_encode_atoms(char *buf, t_symbol *address, int argc, t_atom *argv);

#ifdef __cplusplus
}
#endif

#endif //__OMAX_UTIL_H__
