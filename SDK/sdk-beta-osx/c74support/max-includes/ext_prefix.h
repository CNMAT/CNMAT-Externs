#ifndef _EXT_PREFIX_H_
#define _EXT_PREFIX_H_

////////////////////////////////////////////////////////////////////////////////
// macros

#ifndef WIN_VERSION
#define MAC_VERSION 1 // we'll assume that if we aren't requesting the windows version, that we build the mac version(s) of the product
#endif  WIN_VERSION

#ifdef __cplusplus
#define BEGIN_USING_C_LINKAGE \
	extern "C" {
#else
#define BEGIN_USING_C_LINKAGE
#endif __cplusplus

#ifdef __cplusplus
#define END_USING_C_LINKAGE \
	}
#else
#define END_USING_C_LINKAGE
#endif __cplusplus

BEGIN_USING_C_LINKAGE

////////////////////////////////////////////////////////////////////////////////
// Mac Target

#ifdef MAC_VERSION
#ifndef powerc
#pragma d0_pointers on
#endif// WORKITEM_WORKITEM $craighs$ will move this define into a seperate header
#define _K( k )	
#endif MAC_VERSION

////////////////////////////////////////////////////////////////////////////////
// Win32 Target

#ifdef WIN_VERSION

#pragma warning( disable : 4005 ) // macro redefinition
#pragma warning( disable : 4101 ) // unreferenced local
#pragma warning( disable : 4800 ) // forcing value to bool 'true' or 'false'
#pragma warning( disable : 4087 ) // 'function' declared with 'void' parameter list
#pragma warning( disable : 4068 ) // unknown pragma
//#pragma warning( disable : 4731 ) // frame pointer register modified by inline assembly code

// crtl
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>

// windows
#include <windows.h>
#include <richedit.h>
#include <commctrl.h>

// Mac2Win
#include "ext_mac2win.h"
#include "pascal.h"
#include "asiport.h"
#include "asiextrn.h"
#include "WindowM.h"
#include "Gestalt.h"
#include "Serial.h"
#include "ColorPicker.h"
//#include "Movienam.h"
#include "Movies.h"
#include "ImageCompression.h"
#include "qtml.h"

// WORKITEM_WORKITEM $craighs$ will move these defines into a seperate header
#ifndef __cplusplus
#define __MOTO__
#endif  __cplusplus
#define powerc
#define GetDlgItem				MaxGetDlgItem		// conflict with winuser.h
#define ExitCallback()			((void*)0)
#define EnterCallback()			((void*)0)
#define SetCurrentA4()			(0)
#define hintsLoop				(1 << 1)		// in carbon version of Movie.h only
#define _K( k )					(k).bigEndianValue
#define MAX_ASSERT( f )			( assert(f) )
#define topLeft(r)				(((Point *) &(r))[0])
#define botRight(r)				(((Point *) &(r))[1])
#define pascal// WORKITEM_WORKITEM $craighs$ will move these define into a seperate header
extern HINSTANCE				g_external_instance;
extern LPSTR					g_external_path;
extern LPSTR					g_external_path_pstr;
#endif WIN_VERSION

END_USING_C_LINKAGE

#endif _EXT_PREFIX_H_
