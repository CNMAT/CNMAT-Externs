
// jit.window.h
//
// author: randall jones
// © 2001 cycling '74

#include <Devices.h>
#include <Dialogs.h>
#include <DriverServices.h>
#include <Events.h>
#include <LowMem.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Windows.h>
#include <MacWindows.h>

#include <math.h>
#include <stdio.h>

#include "agl.h"
#include "glut.h"
#include "jit.common.h"

#ifndef __max_ext_wind_h
#define __max_ext_wind_h
        #include "ext_wind.h"
#endif

/*
#ifndef __max_z_dsp_h
#define __max_z_dsp_h
        #include "z_dsp.h"
#endif
*/

// t_jit_window flags
#define JIT_WINDOW_USING_GL                     1 << 0
#define JIT_WINDOW_NO_ACCEL                     1 << 1


#define MIN_WIND_DIM    9
#define MAX_WIND_COORD  8192

t_jit_err jit_window_get_gl_context(void *v, AGLContext *p_context); 
t_jit_err jit_window_get_rect(void *v, Rect *p_rect);
t_jit_err jit_window_get_mouse_info(void *v, t_wind_mouse_info **h_info);

void jit_window_dragcheck(void *x);


t_jit_err jit_window_init(void);
