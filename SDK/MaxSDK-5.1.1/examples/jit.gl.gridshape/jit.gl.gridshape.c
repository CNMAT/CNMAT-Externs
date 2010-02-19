// jit.gl.gridshape.c
// draw 3d gridshapes. 
//
// author: jkc
// Copyright 2002-2005 - Cycling '74
 

#include "jit.common.h"
#include "jit.gl.h"
#include "ext_strings.h"
#include <math.h>
#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

typedef struct _jit_gl_gridshape 
{
	t_object			ob;
	void				*ob3d;
	t_jit_glchunk		*chunk;
	long				recalc;
	t_symbol			*shape;
	long				dim[2];
	float				rad_minor; // for torus
	char				gridmode;
	char				displaylist;
	GLuint				dlref; 
} t_jit_gl_gridshape;

void *_jit_gl_gridshape_class;

t_jit_err jit_gl_gridshape_init(void) ;
t_jit_gl_gridshape *jit_gl_gridshape_new(t_symbol * dest_name);
void jit_gl_gridshape_free(t_jit_gl_gridshape *x);

t_jit_err jit_gl_gridshape_shape(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_gridshape_dim(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_gridshape_rad_minor(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_gridshape_displaylist(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_gridshape_rebuild_geometry(t_jit_gl_gridshape *x);
t_jit_err jit_gl_gridshape_draw(t_jit_gl_gridshape *x);
t_jit_err jit_gl_gridshape_dest_changed(t_jit_gl_gridshape *x);
void jit_gl_gridshape_get_color(t_jit_gl_gridshape *x, float *red, float *green, float *blue, float *alpha);

t_jit_err jit_gl_gridshape_recalc(t_jit_gl_gridshape *x);
t_jit_err jit_gl_gridshape_dest_closing(t_jit_gl_gridshape *x);

void calc_sphere(t_jit_gl_gridshape *x);
void calc_cylinder(t_jit_gl_gridshape *x);
void calc_torus(t_jit_gl_gridshape *x);
void calc_opencylinder(t_jit_gl_gridshape *x);
void calc_circle(t_jit_gl_gridshape *x);
void calc_cube(t_jit_gl_gridshape *x);
void calc_opencube(t_jit_gl_gridshape *x);
void calc_plane(t_jit_gl_gridshape *x);

void color_surface(t_jit_gl_gridshape *x);

void draw_grid(t_jit_gl_gridshape *x, t_jit_object *matrix, GLenum mode);

t_symbol *ps_circle,*ps_sphere,*ps_torus,*ps_cylinder,*ps_opencylinder,*ps_cube,*ps_opencube,*ps_plane;
t_symbol *ps_matrixoutput;


// --------------------------------------------------------------------------------
//

t_jit_err jit_gl_gridshape_init(void) 
{
	long attrflags=0;
	long ob3d_flags=0;
	t_jit_object *attr;
	void * ob3d;
	
	_jit_gl_gridshape_class = jit_class_new("jit_gl_gridshape", (method)jit_gl_gridshape_new, (method)jit_gl_gridshape_free,
		sizeof(t_jit_gl_gridshape),A_DEFSYM,0L);
	
	// set up object extension for 3d object, customized with flags
	ob3d = jit_ob3d_setup(_jit_gl_gridshape_class, calcoffset(t_jit_gl_gridshape, ob3d), ob3d_flags);

	// add attributes
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	// shape
	attr = jit_object_new(_jit_sym_jit_attr_offset,"shape",_jit_sym_symbol,attrflags,
		(method)0L,(method)jit_gl_gridshape_shape,calcoffset(t_jit_gl_gridshape, shape));	
	jit_class_addattr(_jit_gl_gridshape_class,attr);	
	attr = jit_object_new(_jit_sym_jit_attr_offset_array,"dim",_jit_sym_long,2,attrflags,
		(method)0L,(method)jit_gl_gridshape_dim,0/*fix*/,calcoffset(t_jit_gl_gridshape,dim));
	jit_attr_addfilterset_clip(attr,2,0,TRUE,FALSE);
	jit_class_addattr(_jit_gl_gridshape_class,attr);	
	attr = jit_object_new(_jit_sym_jit_attr_offset,"rad_minor",_jit_sym_float32,attrflags,
		(method)0L,(method)jit_gl_gridshape_rad_minor,calcoffset(t_jit_gl_gridshape, rad_minor));	
	jit_class_addattr(_jit_gl_gridshape_class,attr);	
	attr = jit_object_new(_jit_sym_jit_attr_offset,"displaylist",_jit_sym_char,attrflags,
		(method)0L,(method)jit_gl_gridshape_displaylist,calcoffset(t_jit_gl_gridshape, displaylist));	
	jit_class_addattr(_jit_gl_gridshape_class,attr);	
	attr = jit_object_new(_jit_sym_jit_attr_offset,"gridmode",_jit_sym_char,attrflags,
		(method)0L,(method)0L,calcoffset(t_jit_gl_gridshape, gridmode));	
	jit_class_addattr(_jit_gl_gridshape_class,attr);	
	
	// define our OB3D draw method.  called in automatic mode by 
	// jit.gl.render or otherwise through ob3d when banged. this 
	// method is A_CANT because our draw setup needs to happen 
	// in the ob3d beforehand to initialize OpenGL state 
	jit_class_addmethod(_jit_gl_gridshape_class, (method)jit_gl_gridshape_draw, "ob3d_draw", A_CANT, 0L);
	
	// define our dest_closing and dest_changed methods. 
	// these methods are called by jit.gl.render when the 
	// destination context closes or changes: for example, when 
	// the user moves the window from one monitor to another. Any 
	// resources your object keeps in the OpenGL machine 
	// (e.g. textures, display lists, vertex shaders, etc.) 
	// will need to be freed when closing, and rebuilt when it has 
	// changed. 
	jit_class_addmethod(_jit_gl_gridshape_class, (method)jit_gl_gridshape_dest_closing, "dest_closing",	A_CANT, 0L);
	jit_class_addmethod(_jit_gl_gridshape_class, (method)jit_gl_gridshape_dest_changed, "dest_changed", A_CANT, 0L);
	
	// rebuild_geometry is called if texture units or other 
	// attributes change requiring a rebuild
	jit_class_addmethod(_jit_gl_gridshape_class, (method)jit_gl_gridshape_rebuild_geometry, "rebuild_geometry", 0L);

	// must register for ob3d	
	jit_class_addmethod(_jit_gl_gridshape_class, (method)jit_object_register, 			"register",		A_CANT, 0L);

	jit_class_register(_jit_gl_gridshape_class);

	ps_circle 			= gensym("circle");
	ps_sphere 			= gensym("sphere");
	ps_torus 			= gensym("torus");
	ps_cylinder 		= gensym("cylinder");
	ps_opencylinder 	= gensym("opencylinder");
	ps_cube 			= gensym("cube");
	ps_opencube 		= gensym("opencube");
	ps_plane 			= gensym("plane");
	ps_matrixoutput 	= gensym("matrixoutput");

	return JIT_ERR_NONE;
}


t_jit_gl_gridshape *jit_gl_gridshape_new(t_symbol * dest_name)
{
	t_jit_gl_gridshape *x;

	// make jit object
	if (x = (t_jit_gl_gridshape *)jit_object_alloc(_jit_gl_gridshape_class)) 
	{
		// create and attach ob3d
		jit_ob3d_new(x, dest_name);
		
		// set instance variable defaults
		x->shape = ps_sphere;
		x->dim[0] = 20;
		x->dim[1] = 20;	
		x->rad_minor = 0.5;
		x->recalc = 1;
		x->displaylist = 0;
		x->dlref = 0;
		x->gridmode = 0;

		// allocate glchunk matrix used to store and draw geometry
		x->chunk = jit_glchunk_grid_new(_jit_sym_gl_quad_grid, 12, x->dim[0], x->dim[1]);
	} 
	else 
	{
		x = NULL;
	}	
	return x;
}

t_jit_err jit_gl_gridshape_shape(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv)
{
	if (argc&&argv) {
		x->shape = jit_atom_getsym(argv);
		x->recalc = 1;		
	}
	
	return JIT_ERR_NONE;
}


t_jit_err jit_gl_gridshape_dim(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv)
{
	if (argc&&argv&&x->chunk&&x->chunk->m_vertex) {
		t_jit_matrix_info info;

		jit_object_method(x->chunk->m_vertex,_jit_sym_dim, argc, argv);
		jit_object_method(x->chunk->m_vertex,_jit_sym_getinfo,&info);
		x->dim[0] = info.dim[0];
		x->dim[1] = info.dim[1];
		x->recalc = 1;		
	}
	
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_rad_minor(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv)
{
	if (argc&&argv) {
		x->rad_minor = jit_atom_getfloat(argv);
		if (x->shape==ps_torus)
			x->recalc = 1;		
	}
	
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_displaylist(t_jit_gl_gridshape *x, void *attr, long argc, t_atom *argv)
{
	if (argc&&argv) {
		x->displaylist = jit_atom_getlong(argv);
		x->recalc = 1;		
	}
	
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_recalc(t_jit_gl_gridshape *x)
{
	if (x->chunk&&x->chunk->m_vertex) {
		if (x->shape==ps_plane) {
			calc_plane(x);
		} else if (x->shape==ps_cube) {
			calc_cube(x);
		} else if (x->shape==ps_opencube) {
			calc_opencube(x);
		} else if (x->shape==ps_circle) {
			calc_circle(x);
		} else if (x->shape==ps_opencylinder) {
			calc_opencylinder(x);
		} else if (x->shape==ps_torus) {
			calc_torus(x);
		} else if (x->shape==ps_cylinder) {
			calc_cylinder(x);
		} else {
			calc_sphere(x);			
		}
	}
	
	return JIT_ERR_NONE;
}

void jit_gl_gridshape_free(t_jit_gl_gridshape *x)
{
	
	if (x->chunk)
		jit_glchunk_delete(x->chunk);
	if (jit_ob3d_set_context(x)==JIT_ERR_NONE) {
		if (x->dlref)
			glDeleteLists(x->dlref,1);
	}
	jit_ob3d_free(x);
}

t_jit_err jit_gl_gridshape_dest_closing(t_jit_gl_gridshape *x)
{
	// destination is closing, free our context dependent resources
	if (x->dlref) {
		glDeleteLists(x->dlref,1);
		x->dlref=0;
		x->recalc=1;
	}
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_dest_changed(t_jit_gl_gridshape *x)
{
	// destination has changed, requires rebuilding context dependent resources
	if (x->displaylist) 
		x->recalc=1;
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_rebuild_geometry(t_jit_gl_gridshape *x)
{
	// some attribute has changed which requires recomputing geometry
	x->recalc = 1;
	return JIT_ERR_NONE;
}

t_jit_err jit_gl_gridshape_draw(t_jit_gl_gridshape *x)
{
	t_jit_err result = JIT_ERR_NONE;
	GLenum prim;
	
	prim = (x->gridmode) ? GL_TRIANGLE_STRIP : GL_QUAD_STRIP;
	
	if (x->recalc) {
		jit_gl_gridshape_recalc(x);
		if (x->displaylist) 
		{
			t_jit_gl_context ctx;
			
			// cache/restore context in case in capture mode
			ctx = jit_gl_get_context();			
			jit_ob3d_set_context(x);
			
			if (x->dlref) {
				glDeleteLists(x->dlref,1);
				x->dlref = 0;
			}
			if (x->dlref=glGenLists(1)) {
				glNewList(x->dlref, GL_COMPILE);
				if (x->chunk&&x->chunk->m_vertex) 
					draw_grid(x,x->chunk->m_vertex, prim);
				glEndList();
			}
			
			jit_gl_set_context(ctx);			
		}		
		x->recalc = 0;
	}
	
	// draw our chunk of OpenGL geometry. 
	if (x->chunk&&x->chunk->m_vertex) {
		if (!jit_attr_getlong(x,ps_matrixoutput)) {
			if (x->displaylist&&x->dlref)
				glCallList(x->dlref);
			else 
				draw_grid(x,x->chunk->m_vertex, prim);
		} else{
			color_surface(x);
			result = jit_ob3d_draw_chunk(x->ob3d, x->chunk); //output matrix
		}
	}	
	
	return result;
}

void jit_gl_gridshape_get_color(t_jit_gl_gridshape *x, float *red, float *green, float *blue, float *alpha)
{
	long ac=0;
	t_atom *av=NULL;
	
	jit_object_method(x,gensym("getcolor"),&ac,&av);
	if ((ac==4)&&av) {
		*red 	= jit_atom_getfloat(av);
		*green 	= jit_atom_getfloat(av+1);
		*blue 	= jit_atom_getfloat(av+2);
		*alpha 	= jit_atom_getfloat(av+3);
		jit_freebytes(av,ac*sizeof(t_atom));
	}
}


void calc_sphere(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0]-1;
	double majorstep = (M_PI/(double)nummajor);
	double minorstep = (2.0*M_PI/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);
		
	for (i=0;i<=nummajor;i++) {
		double a = i*majorstep;
		double r = jit_math_sin(a);
		double z = jit_math_cos(a);
		double c,x,y;
	
		p = (float *)(bp + i*rowstride);

		for (j=0;j<=numminor;j++) {
			c = j*minorstep;
			x = r*jit_math_cos(c);
			y = r*jit_math_sin(c);

			//vertex
			*p++ = x; 
			*p++ = y; 
			*p++ = z; 
			//texture
			*p++ = j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = x; 
			*p++ = y; 
			*p++ = z; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}

void calc_opencylinder(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0]-1;
	double majorstep = (2./(double)nummajor);
	double minorstep = (2.0*M_PI/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);
		
	for (i=0;i<=nummajor;i++) {
		double z = 1. - i*majorstep;
		double c,x,y;
	
		p = (float *)(bp + i*rowstride);

		for (j=0;j<=numminor;j++) {
			c = j*minorstep;
			x = jit_math_cos(c);
			y = jit_math_sin(c);

			//vertex
			*p++ = x; 
			*p++ = y; 
			*p++ = z; 
			//texture
			*p++ = j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = x; 
			*p++ = y; 
			*p++ = 0.; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}

void calc_cylinder(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-3;
	int numminor = x->dim[0]-1;
	double majorstep = (2./(double)nummajor);
	double minorstep = (2.0*M_PI/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	if (x->dim[1]<3)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);

	p = (float *)bp;
	//top
	for (j=0;j<=numminor;j++) {
		//vertex
		*p++ = 0; 
		*p++ = 0; 
		*p++ = 1; 
		//texture
		*p++ = j*numminor_inv; 
		*p++ = 0;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = 1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
	//body	
	for (i=0;i<=nummajor;i++) {
		double z = 1. - i*majorstep;
		double c,x,y;
	
		p = (float *)(bp + (i+1)*rowstride);

		for (j=0;j<=numminor;j++) {
			c = j*minorstep;
			x = jit_math_cos(c);
			y = jit_math_sin(c);

			//vertex
			*p++ = x; 
			*p++ = y; 
			*p++ = z; 
			//texture
			*p++ = j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = x; 
			*p++ = y; 
			*p++ = 0.; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
	//bottom
	p = (float *)(bp + (nummajor+2)*rowstride);
	for (j=0;j<=numminor;j++) {
		//vertex
		*p++ = 0; 
		*p++ = 0; 
		*p++ = -1; 
		//texture
		*p++ = j*numminor_inv; 
		*p++ = 1;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = -1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
}

void calc_cube(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-5;
	int numminor = x->dim[0];
	double majorstep = (2./(double)nummajor);
	double minorstep;
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)(numminor-1);
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
	double tx,ty,vx,vy,nx,ny;
	long numface0,numface1,numface2,numface3;
	long tface0,tface1,tface2;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	if (x->dim[1]<3)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);

	numface0 = numminor/4;
	tface0 = numface0;

	numface1 = numminor/2 - tface0;
	tface1 = tface0+numface1;

	numface2 = (3*numminor)/4 - tface1;
	tface2 = tface1+numface2;

	numface3 = numminor - tface2;


	p = (float *)bp;
	//top
	for (j=0;j<numminor;j++) {
		//vertex
		*p++ = 0; 
		*p++ = 0; 
		*p++ = 1; 
		//texture
		*p++ = 0.5; 
		*p++ = 0.5;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = 1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
	p = (float *)(bp + rowstride);
	for (j=0;j<numminor;j++) {
		if (j<tface0) {
			tx=(float)j/(float)(numface0-1);	
			vx=1;
			vy=(2.*tx)-1.;	
			ty=1-tx; //swap
			tx=1;
		} else if (j<tface1) {
			tx=(float)(j-tface0)/(float)(numface1-1);
			vx=1.-(2.*tx);	
			vy=1;
			tx=1-tx;
			ty=0;
		} else if (j<tface2) {
			tx=(float)(j-tface1)/(float)(numface2-1);
			vx=-1;	
			vy=1.-(2.*tx);	
			ty=tx; //swap
			tx=0;
		} else {	
			tx=(float)(j-tface2)/(float)(numface3-1);
			vx=(2.*tx)-1.;	
			vy=-1;
			ty=1;
		}
		//vertex
		*p++ = vx;
		*p++ = vy; 
		*p++ = 1; 
		//texture
		*p++ = tx; 
		*p++ = ty;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = 1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
	
	//body	
	for (i=0;i<=nummajor;i++) {
		double vz = 1. - i*majorstep;
	
		p = (float *)(bp + (i+2)*rowstride);
		
		for (j=0;j<numminor;j++) {
			if (j<tface0) {
				tx=(float)j/(float)(numface0-1);	
				vx=1;
				vy=(2.*tx)-1.;	
				nx=1;
				ny=0;
			} else if (j<tface1) {
				tx=(float)(j-tface0)/(float)(numface1-1);
				vx=1.-(2.*tx);	
				vy=1;
				nx=0;
				ny=1;
			} else if (j<tface2) {
				tx=(float)(j-tface1)/(float)(numface2-1);
				vx=-1;	
				vy=1.-(2.*tx);	
				nx=-1;
				ny=0;
			} else {	
				tx=(float)(j-tface2)/(float)(numface3-1);
				vx=(2.*tx)-1.;	
				vy=-1;
				nx=0;
				ny=-1;
			}
			//vertex
			*p++ = vx; 
			*p++ = vy; 
			*p++ = vz; 
			//texture
			//*p++ = j*numminor_inv; 
			*p++ = tx; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = nx; 
			*p++ = ny; 
			*p++ = 0; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
	//bottom
	p = (float *)(bp + (nummajor+3)*rowstride);
	for (j=0;j<numminor;j++) {
		if (j<tface0) {
			tx=(float)j/(float)(numface0-1);	
			vx=1;
			vy=(2.*tx)-1.;	
			ty=1-tx; //swap
			tx=1;
		} else if (j<tface1) {
			tx=(float)(j-tface0)/(float)(numface1-1);
			vx=1.-(2.*tx);	
			vy=1;
			tx=1-tx;
			ty=0;
		} else if (j<tface2) {
			tx=(float)(j-tface1)/(float)(numface2-1);
			vx=-1;	
			vy=1.-(2.*tx);	
			ty=tx; //swap
			tx=0;
		} else {	
			tx=(float)(j-tface2)/(float)(numface3-1);
			vx=(2.*tx)-1.;	
			vy=-1;
			ty=1;
		}
		//vertex
		*p++ = vx; 
		*p++ = vy; 
		*p++ = -1; 
		//texture
		*p++ = tx; 
		*p++ = ty;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = -1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
	p = (float *)(bp + (nummajor+4)*rowstride);
	for (j=0;j<numminor;j++) {
		//vertex
		*p++ = 0; 
		*p++ = 0; 
		*p++ = -1; 
		//texture
		*p++ = 0.5; 
		*p++ = 0.5;
		//normals
		*p++ = 0; 
		*p++ = 0; 
		*p++ = -1; 
		//color rgba
		*p++ = red;
		*p++ = green;
		*p++ = blue;
		*p++ = alpha;
	}
}

void calc_opencube(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0];
	double majorstep = (2./(double)nummajor);
	double minorstep;
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)(numminor-1);
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
	double tx,ty,vx,vy,nx,ny;
	long numface0,numface1,numface2,numface3;
	long tface0,tface1,tface2;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	if (x->dim[1]<3)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);

	numface0 = numminor/4;
	tface0 = numface0;

	numface1 = numminor/2 - tface0;
	tface1 = tface0+numface1;

	numface2 = (3*numminor)/4 - tface1;
	tface2 = tface1+numface2;

	numface3 = numminor - tface2;


	p = (float *)bp;
	
	//body	
	for (i=0;i<=nummajor;i++) {
		double vz = 1. - i*majorstep;
	
		p = (float *)(bp + i*rowstride);
		
		for (j=0;j<numminor;j++) {
			if (j<tface0) {
				tx=(float)j/(float)(numface0-1);	
				vx=1;
				vy=(2.*tx)-1.;	
				nx=1;
				ny=0;
			} else if (j<tface1) {
				tx=(float)(j-tface0)/(float)(numface1-1);
				vx=1.-(2.*tx);	
				vy=1;
				nx=0;
				ny=1;
			} else if (j<tface2) {
				tx=(float)(j-tface1)/(float)(numface2-1);
				vx=-1;	
				vy=1.-(2.*tx);	
				nx=-1;
				ny=0;
			} else {	
				tx=(float)(j-tface2)/(float)(numface3-1);
				vx=(2.*tx)-1.;	
				vy=-1;
				nx=0;
				ny=-1;
			}
			//vertex
			*p++ = vx; 
			*p++ = vy; 
			*p++ = vz; 
			//texture
			*p++ = tx; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = nx; 
			*p++ = ny; 
			*p++ = 0; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}


void calc_circle(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0]-1;
	double majorstep = (1./(double)nummajor);
	double minorstep = (2.0*M_PI/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);
		
	for (i=0;i<=nummajor;i++) {
		double r = 1. - i*majorstep;
		double c,x,y;
	
		p = (float *)(bp + i*rowstride);

		for (j=0;j<=numminor;j++) {
			c = 2.0*M_PI - j*minorstep;
			x = r*jit_math_cos(c);
			y = r*jit_math_sin(c);

			//vertex
			*p++ = x; 
			*p++ = y; 
			*p++ = 0; 
			//texture
			*p++ = j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = 0; 
			*p++ = 0; 
			*p++ = 1; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}

void calc_plane(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0]-1;
	double majorstep = (2.0/(double)nummajor);
	double minorstep = (2.0/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);
		
	for (i=0;i<=nummajor;i++) {
		double x,y;
		y = i*majorstep-1.;

		p = (float *)(bp + i*rowstride);
		
		for (j=0;j<=numminor;j++) {
			x = 1.-j*minorstep;

			//vertex
			*p++ = x; 
			*p++ = y; 
			*p++ = 0; 
			//texture
			*p++ = 1.-j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = 0; 
			*p++ = 0; 
			*p++ = 1; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}


void calc_torus(t_jit_gl_gridshape *x)
{
	int nummajor = x->dim[1]-1;
	int numminor = x->dim[0]-1;
	double majorstep = (2.0*M_PI/(double)nummajor);
	double minorstep = (2.0*M_PI/(double)numminor);
	double nummajor_inv = 1./(double)nummajor;
	double numminor_inv = 1./(double)numminor;
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;
	double radminor=x->rad_minor;
		
	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);
		
	for (i=0;i<=nummajor;i++) {
		double a = i*majorstep;
		double x = jit_math_cos(a);
		double y = jit_math_sin(a);
		double c,b,r,z;
	
		p = (float *)(bp + i*rowstride);

		for (j=0;j<=numminor;j++) {
 		    b = j*minorstep;
 			c = jit_math_cos(b);
      		r = radminor*c + 1.;
      		z = jit_math_sin(b);

			//vertex
			*p++ = x*r; 
			*p++ = y*r; 
			*p++ = z*radminor; 
			//texture
			*p++ = j*numminor_inv; 
			*p++ = i*nummajor_inv;
			//normals
			*p++ = x*c; 
			*p++ = y*c; 
			*p++ = z; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
		}
	}
}

void color_surface(t_jit_gl_gridshape *x)
{
	float *p;
	char *bp=NULL;
	void *m;
	int i,j,rowstride;
	t_jit_matrix_info info;
	float red,green,blue,alpha;

	if (!x->chunk)
		return;

	if (!(m=x->chunk->m_vertex))
		return;
	
	jit_object_method(m,_jit_sym_getinfo,&info);
	jit_object_method(m,_jit_sym_getdata,&bp);
	
	if (!bp)
		return;
	
	rowstride = info.dimstride[1];
	
	jit_gl_gridshape_get_color(x,&red,&green,&blue,&alpha);			
	
	p = (float *)bp;
	if ((p[8]==red)&&(p[9]==green)&&(p[10]==blue)&&(p[11]==alpha))			
		return;
	for (i=0;i<info.dim[1];i++) {
		p = (float *)(bp + i*rowstride);
		for (j=0;j<info.dim[0];j++) {   	
		    p += 8; 
			//color rgba
			*p++ = red;
			*p++ = green;
			*p++ = blue;
			*p++ = alpha;
			
		}
    }
}


//_____________________________________________________
// util

#define _I_X	0
#define _I_Y	1
#define _I_Z	2
#define _I_S	3
#define _I_T	4
#define _I_NX	5
#define _I_NY	6
#define _I_NZ	7
#define _I_R	8
#define _I_G	9
#define _I_B	10
#define _I_A	11

//not drawing per vertex color in this object

void draw_grid(t_jit_gl_gridshape *x,t_jit_object *matrix, GLenum mode)
{
	float *p,*p2;
	char *bp=NULL;
	long i,j,rowstride,width,height,planecount,up=0;
	t_jit_matrix_info info;
	t_jit_gl_drawinfo drawinfo;

	if(jit_gl_drawinfo_setup(x,&drawinfo)) return;

	if (!matrix) return;

	jit_object_method(matrix,_jit_sym_getinfo,&info);
	jit_object_method(matrix,_jit_sym_getdata,&bp);

	if (!bp) return;
	
	planecount	= info.planecount;
	rowstride 	= info.dimstride[1];
	height 		= info.dim[1]-1;
	width 		= info.dim[0];
		
	for (i=0;i<height;i++) {
		p = (float *)(bp + i*rowstride);
		p2 = (float *)(bp + (i+1)*rowstride);
		
		glBegin(mode);
		switch (planecount) {		
		case 16:
		case 15:
		case 14:
		case 13:
		case 12:
			for (j=0;j<width;j++) {
				
				glNormal3f(p[_I_NX],p[_I_NY],p[_I_NZ]);
//				glColor4f(p[_I_R],p[_I_G],p[_I_B],p[_I_A]);		
				jit_gl_texcoord2f(&drawinfo,p[_I_S],p[_I_T]); 			
				glVertex3f(p[_I_X],p[_I_Y],p[_I_Z]);

				glNormal3f(p2[_I_NX],p2[_I_NY],p2[_I_NZ]);
//				glColor4f(p2[_I_R],p2[_I_G],p2[_I_B],p2[_I_A]);
				jit_gl_texcoord2f(&drawinfo,p2[_I_S],p2[_I_T]); 			
				glVertex3f(p2[_I_X],p2[_I_Y],p2[_I_Z]);
				
				p+=planecount;
				p2+=planecount;	    	
			}
			break;
		case 11:
		case 10:
		case 9:
		case 8:
			for (j=0;j<width;j++) {
			
				glNormal3f(p[_I_NX],p[_I_NY],p[_I_NZ]);
				jit_gl_texcoord2f(&drawinfo,p[_I_S],p[_I_T]); 			
				glVertex3f(p[_I_X],p[_I_Y],p[_I_Z]);

				glNormal3f(p2[_I_NX],p2[_I_NY],p2[_I_NZ]);
				glTexCoord2f(p2[_I_S],p2[_I_T]);
				jit_gl_texcoord2f(&drawinfo,p2[_I_S],p2[_I_T]); 			
				glVertex3f(p2[_I_X],p2[_I_Y],p2[_I_Z]);

				p+=planecount;
				p2+=planecount;	    	
			}
			break;
		case 7:
		case 6:
		case 5:
			for (j=0;j<width;j++) {
			
				jit_gl_texcoord2f(&drawinfo,p[_I_S],p[_I_T]); 			
				glVertex3f(p[_I_X],p[_I_Y],p[_I_Z]);

				glTexCoord2f(p2[_I_S],p2[_I_T]);
				jit_gl_texcoord2f(&drawinfo,p2[_I_S],p2[_I_T]); 			
				glVertex3f(p2[_I_X],p2[_I_Y],p2[_I_Z]);

				p+=planecount;
				p2+=planecount;	    	
			}
			break;
		case 4:
		case 3:
			for (j=0;j<width;j++) {
			
				glVertex3f(p[_I_X],p[_I_Y],p[_I_Z]);

				glVertex3f(p2[_I_X],p2[_I_Y],p2[_I_Z]);

				p+=planecount;
				p2+=planecount;	    	
			}
			break;
		}
		glEnd();
	}
}
