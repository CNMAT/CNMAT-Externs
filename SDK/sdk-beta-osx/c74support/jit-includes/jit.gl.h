// jit.gl.h
// 
//
// author: randall jones
// © 2002 cycling '74


#ifndef __JIT_GL_H__
#define __JIT_GL_H__

#include "agl.h"
#include "glu.h"
#include "jit.common.h"
#include <math.h>

// --------------------------------------------------------------------------------
// konstants

#define		JIT_GL_RENDER_REG_STR	"jit_r3nd3r_"
#define		JIT_CLASS3D_FINDER		"this_is_the_jit_class3d"
#define		PI	3.141592653589793

// --------------------------------------------------------------------------------
// utility headers

long jit_gl_available(void);
void jit_gl_init(void);
void jit_gl_renderer_set(void * p);
void * jit_gl_renderer_get();
void jit_gl_ob3d_set(void * p);
void * jit_gl_ob3d_get();
void jit_gl_set_floats(float *pf, int count, ...);
int jit_gl_get_blend_mode(int index);
long jit_gl_report_error (char *prefix);

// --------------------------------------------------------------------------------
// misc

// mouse info struct for window and pwindow
typedef struct 
{
   Atom			mouseatoms[8];	// h, v, (up/down), cmdKey, shiftKey, alphaLock, option, control.
   int			argc;
   t_symbol 	* mousesymbol;
}  t_wind_mouse_info;


// --------------------------------------------------------------------------------
// geometry stuff

typedef GLfloat t_point_2d[2];		// point

typedef GLfloat t_point_3d[3];		// point

typedef GLfloat t_vec_3d[3];		// vector

typedef GLfloat t_quaternion[4];	// quaternion (rotation)

typedef GLfloat t_rotation[4];		// angle/axis style rotation.

typedef GLfloat t_color[4];			// RGBA

typedef struct 
{
 	GLfloat	u[3];
 	GLfloat	v[3];
}  	t_line_3d;						// line or line segment

// --------------------------------------------------------------------------------
// jit_glchunk is a public structure to store one gl-command's-worth of data, in a format
// which can be passed easily to glDrawRangeElements.  

typedef struct _jit_glchunk
{
	t_symbol	*	prim;			// GL_TRI_STRIP, GL_TRIANGLES, etc. 
	t_jit_object *	m_vertex;		// jit_matrix of xyzst... data. 
	t_symbol *		m_vertex_name;	
	t_jit_object *	m_index;		// optional 1d jit_matrix of vertex indices. 
	t_symbol *		m_index_name;	
	unsigned long	m_flags;
}
t_jit_glchunk;

t_jit_glchunk * jit_glchunk_new(t_symbol * prim, int planes, int vertices, int indices);
t_jit_glchunk * jit_glchunk_grid_new(t_symbol * prim, int planes, int width, int height);
void jit_glchunk_delete(t_jit_glchunk * x);

// --------------------------------------------------------------------------------
// flags for chunk creation

#define JIT_GL_CHUNK_IGNORE_TEXTURES		1 << 0
#define JIT_GL_CHUNK_IGNORE_NORMALS			1 << 1
#define JIT_GL_CHUNK_IGNORE_COLORS			1 << 2
#define JIT_GL_CHUNK_IGNORE_EDGES			1 << 3

// --------------------------------------------------------------------------------
// ob3d stuff

// flags -- default: all flags off.

#define JIT_OB3D_NO_ROTATION_SCALE			1 << 0
#define JIT_OB3D_NO_POLY_VARS				1 << 1
#define JIT_OB3D_NO_BLEND					1 << 2
#define JIT_OB3D_NO_TEXTURE					1 << 3
#define JIT_OB3D_NO_MATRIXOUTPUT			1 << 4
#define JIT_OB3D_AUTO_ONLY					1 << 5
#define JIT_OB3D_DOES_UI					1 << 6
#define JIT_OB3D_NO_DEPTH					1 << 7
#define JIT_OB3D_NO_ANTIALIAS				1 << 8
#define JIT_OB3D_NO_FOG						1 << 9
#define JIT_OB3D_NO_LIGHTING_MATERIAL		1 << 10
#define JIT_OB3D_HAS_LIGHTS					1 << 11
#define JIT_OB3D_HAS_CAMERA					1 << 12
#define JIT_OB3D_IS_RENDERER				1 << 13
#define JIT_OB3D_NO_COLOR					1 << 14

// --------------------------------------------------------------------------------
// ob3d headers - setup


typedef struct _jit_class3d
{
	long				oboffset; 		// instance byte offset to the ob3d struct.
	long				flags;
	// extensible
} t_jit_class3d;


void *jit_ob3d_setup(void * jit_class, long oboffset, long ob3d_flags);
t_jit_err jit_ob3d_set(void *x, void *p);
void *jit_ob3d_get(void *x);
void *jit_ob3d_new(void *x, t_symbol * dest_name);
void jit_ob3d_free(void *x);
t_jit_err jit_ob3d_set_context(void *x);
t_jit_err jit_ob3d_draw_chunk(void *ob3d, t_jit_glchunk * chunk);

void max_ob3d_setup(void);

// attach jit object bearing an ob3d to a max object and its outlet.
void max_jit_ob3d_attach(void *x, t_jit_object *jit_ob, void *outlet);
void max_jit_ob3d_detach(void *x);
t_jit_err max_jit_ob3d_assist(void *x, void *b, long m, long a, char *s);

void * ob3d_jitob_get(void *v);
void * ob3d_next_get(void *v);
long ob3d_auto_get(void *v);
long ob3d_ui_get(void *v);
void * ob3d_outlet_get(void *v);

t_jit_err ob3d_draw_begin(void *ob3d);
t_jit_err ob3d_draw_end(void *ob3d);
t_jit_err ob3d_draw_preamble(void *ob3d);
void jit_gl_set_context(AGLContext p);
void * jit_gl_current_ob3d_set(void * p);
void * jit_gl_current_ob3d_get();


#endif // __JIT_GL_H__
