
#ifndef _EXT_SPEC_H_
#define _EXT_SPEC_H_

#if WINVER
#define MENU_FIXUP_CALL			/* */
#define USE_QD_STRUCT			1
#define CLOSE_WINDOW_DECLARATION	/* */
#define CLOSE_WINDOW_CALL(wPtr)		TRUE
#define USE_QUIT_CHECK			0
#define QUIT_CHECK_DECLARATION		/* */
#define QUIT_CHECK_CALL()		TRUE
#define FORCE_UPDATE( wPtr )		/* */	// was ForceUpdate(wPtr)
#include "ASIWin3X.h"
#endif

#ifndef H_NO_MACHEADERS

#if WINVER
#define H_MacTypes 1
#define H_QuickDraw 1
#define H_MenuMgr 1
#define H_MemoryMgr 1
#define H_WindowMgr 1
#define H_ControlMgr 1
#define H_TextEdit 1
#define H_EventMgr 1
#define H_DialogMgr 1
#define H_ListMgr 1
#define H_ResourceMgr 1
#define H_FileMgr 1
#define H_HFS 1
#define H_ToolboxUtil 1

#define H_SegmentLdr 1
#define H_DeviceMgr 1
#define H_IntlPkg 1
#define H_OSUtil 1
#define H_ScrapMgr 1
#define H_SysErr 1

#if 0
#define H_PackageMgr 1
#define H_StdFilePkg 1
#endif

#endif		// #if WINVER
#endif		// #ifndef H_NO_MACHEADERS

#include "OSTDefs.H"

#define IDM_UNDO	0x0200
#define IDM_CUT		0x0202
#define IDM_COPY	0x0203
#define IDM_PASTE	0x0204
#define IDM_CLEAR	0x0205

#endif _EXT_SPEC_H_
