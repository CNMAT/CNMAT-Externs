
#include "max2pd.h"

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

void object_method_typed(t_object *x, t_symbol *s, long ac, t_atom *av, t_atom *rv)
{
    //    t_gotfn *z = (t_gotfn *)zgetfn(&x->te_g.g_pd, s);
//    t_atom *a = (z)(x, );

    // NOTE: no return value here~
//#define    object_method_typed ((t_object *)b, gensym("sizeinsamps"), 1L, temp_atom, temp_atom + 1);
   pd_typedmess(&x->te_g.g_pd, s, (int)ac, av);
}

void object_method(t_object *x, t_symbol *s)
{
    pd_typedmess(&x->te_g.g_pd, s, 0, 0);
}

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