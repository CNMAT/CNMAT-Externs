
// jit.window.h
//
// author: randall jones
// Copyright 2001-2004 - Cycling '74

#ifndef __JIT_WINDOW_H__
#define __JIT_WINDOW_H__

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

#ifndef WIN_VERSION //win_todo
#include "agl.h"
#include "glut.h"
#endif
#include "jit.common.h"

#ifdef __cplusplus
	extern "C" {
#endif 

#ifndef __max_ext_wind_h
#define __max_ext_wind_h
        #include "ext_wind.h"
#endif

// t_jit_window flags
#define JIT_WINDOW_USING_GL                     1 << 0
#define JIT_WINDOW_NO_ACCEL                     1 << 1

#define MIN_WIND_DIM    9
#define MAX_WIND_COORD  8192

void jit_window_dragcheck(void *x);

t_jit_err jit_window_init(void);

#ifdef __cplusplus
}
#endif

#endif //__JIT_WINDOW_H__
