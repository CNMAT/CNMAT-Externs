// MW 4/4/97
#define MAC		
#define MAXObject
/* #define GRAPHICS  
   left out graphics because it's not clear whether a NeuralNet's 
   myWind field should be (Wind *) or (WindPub *).  MW 4/4/97 */

#ifdef MAC
#ifdef never
#include <color.h>
#endif
#include <Dialogs.h>
#include <Palettes.h>
// #include <Pascal.h>
#include <Resources.h>
#include <StandardFile.h>
#include <ToolUtils.h>
#include <Types.h>
#endif /* MAC */

#include <string.h>
#include <stdlib.h>

#undef BUFSIZ

#include "common.h"
#include "typedefs.h"


#ifndef SCOPE
#ifdef PPC
#define RememberA4()	(0)
#define RememberA0()	(0)
#define SetUpA4()		(0)
#define RestoreA4()		(0)
#else
#endif /* PPC */
#define SCOPE
#endif /* SCOPE */


#ifdef MAXObject
#include "ext.h"
#include "ext_user.h"
#include "ext_wind.h"
#include "ext_anim.h"
#define	BUFSIZ	(512)

#else /* not MAXObject */

#ifndef MAC
typedef int     Boolean;
#define false   (0)
#define true    (!false)
#endif /* MAC */

#include <stdio.h>
#endif /* MAXOBJECT */


#ifndef nil
#define nil ((void *)0)
#endif

#ifndef SCOPE
#define SCOPE
#endif

#include "nn.h"

#include "function.h"
#include "netError.h"

/* resource numbering */
#ifdef MAXObject
#define	myRes	4000
#define	myRes1	4001

SCOPE fptr	*FNS;
SCOPE void *class;
#endif /* MAXObject */

#ifndef MAXObject
#define freebytes(x,y)	free(x)
#define getbytes(x)		malloc((size_t)(x))
#define	post			printf("\nMax: ");printf
#define lockout_set
#define	SETFLOAT
#else
#ifdef MAC
SCOPE int numPaletteColors;
SCOPE Boolean hasColorQD;
SCOPE Pattern blk,wht;
#endif
#endif

#include "prototypes.h"
#include "math.h"
