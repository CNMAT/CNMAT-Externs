/* 
	jit.mac.h

	Copyright 2001-2004 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com
	
*/

#ifndef __JIT_MAC_H__
#define __JIT_MAC_H__

#include <MacTypes.h>
#include <devices.h> 
#include <MacWindows.h> 
#include <controls.h> 
#include <Quickdraw.h>
#include <QDOffscreen.h>
#include <Aliases.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE 	1
#endif
#ifndef FALSE
#define FALSE 	0
#endif

#if (TARGET_API_MAC_CARBON)
#define StripAddress(theAddress) ((Ptr)(theAddress))
#endif

OSErr jit_mac_gestalt(OSType selector, long *response);

#ifdef __cplusplus
}
#endif

#endif //__JIT_MAC_H__
