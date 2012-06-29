/*
 *
 *		Represents a context within a container of some form (window, patcher view, etc.)
 *
 */

#ifndef JIT_GL_CONTEXT_VIEW_H
#define JIT_GL_CONTEXT_VIEW_H

#include "jit.common.h"
#include "jit.gl.h"

/*
	The render target is the object that creates a jit_gl_context_view.  It manages its lifetime.  The 
	jit_gl_render object drawing to this destination will get a notification about destruction.  A 
	jit_gl_render object does not create a jit_gl_context_view, it only gets one via the destination 
	name.
*/


// return codes for renderer
typedef long t_jit_gl_context_status;
#define JIT_GL_VIEW_AVAILABLE		0
#define JIT_GL_VIEW_UNAVAILABLE		1
#define JIT_GL_VIEW_ERROR			2


typedef long t_jit_gl_context_modifier;
#define JIT_GL_VIEW_COMMAND_KEY		(1<<0)
#define JIT_GL_VIEW_SHIFT_KEY		(1<<1)
#define JIT_GL_VIEW_CAPS_LOCK		(1<<2)
#define JIT_GL_VIEW_ALT_KEY			(1<<3)
#define JIT_GL_VIEW_CONTROL_KEY		(1<<4)
	


typedef struct _jit_pt {
	long	x;
	long	y;
} t_jit_pt;

typedef struct _jit_rect {
	long	x;
	long	y;
	long	width;
	long	height;
} t_jit_rect;

/**
 * t_jit_gl_context_view object struct.
 *
 * Manages an OpenGL context within a rectangle.  Objects that use a t_jit_gl_context_view 
 * to manage an OpenGL context should attach themselves to the object for its lifetime
 * and implement an "update" method in order to handle modifications to the 
 * t_jit_gl_context_view that may require a rebuild or further response within the embedding 
 * object.
 *
 * @ingroup gl
 * 
 */
typedef struct _jit_gl_context_view {
	t_object			ob;					///< jitter object
	long				rebuild;			///< rebuild flag
	t_jit_gl_context	context;			///< OpenGL context
	t_symbol			*shared_context;	///< shared context name
	t_wind_mouse_info	mouse_info;			///< data for mouse events
	long				doublebuffer;		///< double buffer flag
	long				depthbuffer;		///< depth buffer flag
	long				stereo;				///< active stereo flag
	t_jit_rect			frame;				///< frame of context
	long				fsaa;				///< FSAA flag
	long				sync;				///< V-sync flag
	long				idlemouse;			///< Idlemouse flag (events on mouse move)
	void				*target;			///< target object we're controlled by
	long				targettype;			///< target type we're controlled by
	t_symbol			*name;				///< name of the view
	long				reshaping;			///< flag for breaking cycles on reshape notification
	long				ownerreshape;		///< flag for if the owner handles reshaping the context
} t_jit_gl_context_view;


t_jit_err jit_gl_context_view_init(void);
t_jit_gl_context_view * jit_gl_context_view_new();
void jit_gl_context_view_free(t_jit_gl_context_view *x);

// internal methods (these exist but aren't available in the header)
// t_jit_gl_context_status jit_gl_context_view_create(t_jit_gl_context_view *x);

// render destination interface
t_jit_err jit_gl_context_view_rebuild(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_destroy(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_update(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_clear(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_attach(t_jit_gl_context_view *x, void *target);
t_jit_err jit_gl_context_view_detach(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_reshape(t_jit_gl_context_view *x);
t_jit_err jit_gl_context_view_set_frame(t_jit_gl_context_view *x, t_jit_rect *rect);
t_jit_err jit_gl_context_view_get_frame(t_jit_gl_context_view *x, t_jit_rect *rect);
t_jit_gl_context jit_gl_context_view_getcontext(t_jit_gl_context_view *x);
void * jit_gl_context_view_gettarget(t_jit_gl_context_view *x);
void jit_gl_context_view_handle_mouse_event(t_jit_gl_context_view *x, t_symbol *s, long mousedown, t_jit_pt pt, t_jit_gl_context_modifier modifiers);
t_jit_err  jit_gl_context_view_get_mouse_info(t_jit_gl_context_view *x, t_wind_mouse_info **minfo);
t_jit_err jit_gl_context_view_setname(t_jit_gl_context_view *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_context_view_setflag(t_jit_gl_context_view *x, void *attr, long argc, t_atom *argv);
t_jit_err jit_gl_context_view_setshared_context(t_jit_gl_context_view *x, void *attr, long argc, t_atom *argv);

// common to render destination and renderer interface
t_jit_gl_context_status jit_gl_context_view_make_current(t_jit_gl_context_view *x);

// renderer interface
t_jit_gl_context_status jit_gl_context_view_swap(t_jit_gl_context_view *x);

#endif
