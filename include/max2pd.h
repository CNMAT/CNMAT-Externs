#ifndef __M2P_H__
#define __M2P_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "m_pd.h"
#include "m_imp.h"
#include "string.h"
    
/*
//#ifdef __inline
#undef __inline
#define __inline static __inline
//#endif
*/
    
    /*
//#ifdef FORCE_INLINE
#undef FORCE_INLINE
#define FORCE_INLINE
//#endif
*/
    
#ifndef PI
    /** The pi constant.				@ingroup msp	*/
#define PI 3.14159265358979323846
#endif
#ifndef TWOPI
    /** Twice the pi constant.			@ingroup msp	*/
#define TWOPI		6.28318530717958647692
#endif  // TWOPI
#ifndef PIOVERTWO
    /** Half of the pi constant.		@ingroup msp	*/
#define PIOVERTWO	1.57079632679489661923
#endif  // PIOVERTWO
        
#define t_pxobject t_object
#define sysmem_freeptr free
    
#define t_max_err void*
#define MAX_ERR_NONE 0
#define MAX_ERR_GENERIC 0
    
#define critical_enter(x)
#define critical_exit(x)
#define critical_free(x)
#define critical_new(x)
#define t_critical void*
    
#define object_post(x, st, ...) post(st, ##__VA_ARGS__)
#define object_error(x, st, ...) post(st, ##__VA_ARGS__)
#define object_warn(x, st, ...) post(st, ##__VA_ARGS__)

#define A_SYM   A_SYMBOL
#define A_LONG  A_CANT
#define OMAX_DICT_DICTIONARY(o, x, st)

#define object_alloc(class) pd_new(class)
    
#define clock_fdelay clock_delay
#define method t_method    
    
    
    //added for HISS
    
#define t_atom_long long
#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)
#define ATTR_FUN(a, str) STRINGIZE(a) "_" str
    
//all "attrs" need custom methods in pd
#define CLASS_ATTR_LONG(c, str, flags, name, var)\
    class_addmethod(c, (t_method)ATTR_FUN(name,str), gensym(str), A_DEFFLOAT, 0);
    
// varize attrs need size member as well as structmemer
#define CLASS_ATTR_DOUBLE_VARSIZE(c,attrname,flags,structname,structmember,sizemember,maxsize) \
    class_addmethod(c, (t_method)ATTR_FUN(structname,attrname), gensym(attrname), A_GIMME, 0);


#define CLASS_ATTR_ACCESSORS(c, str, getter, setter)\
    class_addmethod(c, setter, gensym(str), A_GIMME, 0); //(no getter)
    
#define CLASS_ATTR_ATOM_VARSIZE(c,attrname,flags,structname,structmember,sizemember,maxsize) \
    class_addmethod(c, (t_method)ATTR_FUN(structname,attrname), gensym(attrname), A_GIMME, 0);

    
#define CLASS_ATTR_ENUMINDEX(...) //<< this could be added later to give feedback about settings, maybe make this another method as an enum for long value (which is the case with the max version)

#define CLASS_ATTR_LABEL(...)
#define CLASS_ATTR_STYLE_LABEL(...)

#define CLASS_ATTR_FILTER_CLIP(...) //<< this needs to be done manually in name_str function
#define CLASS_ATTR_FILTER_MIN(...)
    
#define CLASS_ATTR_CLEAR(...)
#define CLASS_ATTR_ATOM(...) //??
#define CLASS_STICKY_ATTR(...)
#define CLASS_STICKY_ATTR_CLEAR(...)

#define listout(x) outlet_new(&x->x_obj, &s_list)

    /**
     Sends a type-checked message to an object.
     
     @ingroup obj
     
     @param 	x		The object that will receive the message
     @param 	s		The message selector
     @param 	ac		Count of message arguments in <tt>av</tt>
     @param 	av		Array of t_atoms; the message arguments
     @param 	rv		Return value of function, if available
     
     @return 		This function returns the error code #MAX_ERR_NONE if successful,
     or one of the other error codes defined in #e_max_errorcodes if unsuccessful.
     
     @remark 		If the receiver object can respond to the message, object_method_typed() returns the result in <tt>rv</tt>. Otherwise, <tt>rv</tt> will contain an #A_NOTHING atom.
     */
    void object_method_typed(t_object *x, t_symbol *s, long ac, t_atom *av, t_atom *rv);
    void object_method(t_object *x, t_symbol *s);

    t_symbol *ob_sym(t_symbol *s);
    void atom_setfloat(t_atom *atom, t_float f);
    void atom_setlong(t_atom *atom, long l);
    void atom_setsym(t_atom *atom, t_symbol *s);
    t_int atom_getlong(t_atom *atom);
    t_symbol *atom_getsym(t_atom *atom);
    t_atomtype atom_gettype(t_atom *atom);
    
    void outlet_int(void *outlet, int i);
    void outlet_atoms(void *out, short argc, t_atom *argv);
    void printargs(int argc, t_atom *argv);
    
/*
t_symbol *ob_sym(t_symbol *s)
{
    t_class *b = (t_class *)s->s_thing;
    
    if (b && b->c_name)
        return b->c_name;
    else
        return NULL;
}
    
void atom_setfloat(t_atom *atom, t_float f)
{
	SETFLOAT(atom, f);
}

void atom_setlong(t_atom *atom, long l)
{
	SETFLOAT(atom, (float)l);
}

void atom_setsym(t_atom *atom, t_symbol *s)
{
	SETSYMBOL(atom, s);
}

t_int atom_getlong(t_atom *atom)
{
	return atom_getint(atom);
}

t_symbol *atom_getsym(t_atom *atom)
{
	return atom_getsymbol(atom);
}

t_atomtype atom_gettype(t_atom *atom)
{
	return atom->a_type;
}
    
void outlet_atoms(void *out, short argc, t_atom *argv)
{
	if (argc == 1) {
		if (argv->a_type == A_FLOAT)
			outlet_float((t_outlet *)out,argv->a_w.w_float);
		else if (argv->a_type == A_SYM)
			outlet_anything((t_outlet *)out,argv->a_w.w_symbol,0,0);
	} else {
		if (argv->a_type == A_FLOAT)
			outlet_list((t_outlet *)out,&s_list, argc, argv);
		else if (argv->a_type == A_SYM)
			outlet_anything((t_outlet *)out,argv->a_w.w_symbol,argc-1,argv+1);
	}
}

void outlet_int(void *outlet, int i)
{
	outlet_float((t_outlet *)outlet, (float)i);
}
        
void printargs(int argc, t_atom *argv)
{
	int i;
	for( i = 0; i < argc; i++)
		{
			switch ((argv+i)->a_type) {
			case A_FLOAT:
				post("%s A_FLOAT argv[%d] %f", __func__, i, atom_getfloat(argv+i));
				break;
			case A_SYMBOL:
				post("%s A_SYMBOL argv[%d] %s", __func__, i, atom_getsymbol(argv+i)->s_name);
				break;
			default:
				break;
			}
		}
}
*/
#ifdef __cplusplus 
}
#endif
#endif
