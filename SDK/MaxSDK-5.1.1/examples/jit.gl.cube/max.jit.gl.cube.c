// max.jit.gl.cube.c
// sample GL group object which draws a cube.
//
// author: randall jones
// © 2002 cycling '74


#include "jit.common.h"
#include "jit.gl.h"


typedef struct _max_jit_gl_cube 
{
	t_object		ob;
	void			*obex;
} t_max_jit_gl_cube;

t_jit_err jit_gl_cube_init(void); 

void *max_jit_gl_cube_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_gl_cube_free(t_max_jit_gl_cube *x);
t_class *max_jit_gl_cube_class;
		 	

void main(void)
{	
	void *classex, *jitclass;
	
	// standard Max stuff
	jit_gl_cube_init();	
	setup((t_messlist **)&max_jit_gl_cube_class, (method)max_jit_gl_cube_new, (method)max_jit_gl_cube_free, (short)sizeof(t_max_jit_gl_cube), 
		0L, A_GIMME, 0);
	classex = max_jit_classex_setup(calcoffset(t_max_jit_gl_cube, obex));
	jitclass = jit_class_findbyname(gensym("jit_gl_cube"));	
	
	// add getattributes/dumpout/maxjitclassaddmethods/etc to class extension.
    max_jit_classex_standard_wrap(classex, jitclass, 0); 	
    			   
   	// use standard ob3d assist method.
    addmess((method)max_jit_ob3d_assist, "assist", A_CANT,0);  

	// add methods for 3d drawing
    max_ob3d_setup();
}

void max_jit_gl_cube_free(t_max_jit_gl_cube *x)
{
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_obex_free(x);
}

void *max_jit_gl_cube_new(t_symbol *s, long argc, t_atom *argv)
{
	t_max_jit_gl_cube *x;
	void *jit_ob;
	long attrstart;
	t_symbol *dest_name_sym = _jit_sym_nothing;

	if (x = (t_max_jit_gl_cube *)max_jit_obex_new(max_jit_gl_cube_class, gensym("jit_gl_cube"))) 
	{
		//get normal args
		attrstart = max_jit_attr_args_offset(argc,argv);
		if (attrstart&&argv) 
		{
			jit_atom_arg_getsym(&dest_name_sym, 0, attrstart, argv);
		}

		if (jit_ob = jit_object_new(gensym("jit_gl_cube"), dest_name_sym)) 
		{
			max_jit_obex_jitob_set(x, jit_ob);
			max_jit_obex_dumpout_set(x, outlet_new(x,NULL));
			
			// get attribute args. 
			// typical but not strictly necessary because this sample object has no attributes.
			max_jit_attr_args(x, argc, argv);		
			
			// attach the jit object's ob3d to a new outlet for sending drawing messages.	
			// this outlet is used in matrixoutput mode.
			max_jit_ob3d_attach(x, jit_ob, outlet_new(x, "jit_matrix"));
		} 
		else 
		{
			error("jit.gl.cube: could not allocate object");
			freeobject((t_object *)x);
			x = NULL;
		}
	}
	return (x);
}


