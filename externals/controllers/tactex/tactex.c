/********************************************************************
 *                                                                  *
 *  (c) Tactex Controls Inc., 1999                                  *
 *  All rights reserved.                                            *
 *                                                                  *
 *  File:    main.c                                                 *
 *                                                                  *
 ********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mtc.h"

#define xBase 	20
#define yBase 	350
#define yLine 	15

#define yStrTop	(yBase)
#define yStrMax	(yBase + 12*yLine)
#define xStrMax	400


extern void ViewInit(int, int, int, int);
extern void ViewDrawBorder(WindowPtr, Boolean);
extern void ViewUpdateAll(WindowPtr, WORD*);



/* local function prototypes */
static void 	init(void);
static void 	connect(Str255, Str255);
static void 	disconnect(void);
static void     showVersion(void);
static void 	showRx(void);
static Boolean 	mouseDownHdlr(Point);
static Boolean 	menuSelectHdlr(Point);
static Boolean	keyDownHdlr(EventRecord);
static void		showString(Str255);
static void 	newLine(void);

/* local variables */
static WindowPtr 	gWinPtr;
static Boolean		gNormalOn = false;
/*static*/ MTCHandle    gMTC = NULL;
static Boolean 		gConnected = false;
static Boolean		gDisplayNorm = true;
static WORD			*gPress;

/* global variables */
Boolean main_ShowNumbers = false;
Boolean main_DisplayXY = false;


/* ************************************************************************* */	
static void connect(Str255 in, Str255 out)
	{
	MTCCreate 	mtcCreate;
	MTCConfig	mtcConfig;
	MTCErr		err;
	Str255		str;
	
	if (!gConnected) {
		showString("\pTrying to connect to MTC through: ");				
		showString(in);
		showString(out);
		
		mtcCreate.inputPort = (char *)in;
		mtcCreate.outputPort = (char *)out;
		mtcCreate.mappingFile = "mapping.txt";
		mtcCreate.normalFile = "normal.txt";
		gMTC = MTC_New(mtcCreate);
		
		if (gMTC) {
			showString("\pCommunications initialized. ");
	
			/* allocate memory to store the pressure data */
			MTC_GetConfig(gMTC, &mtcConfig);
			gPress = malloc(mtcConfig.nTaxels * sizeof(WORD));
			if (!gPress)
				showString("\pMemory allocation failed...unable to display pressure.");
			
			/* display the MTC firmware version */
			strcpy((char *)(str + 1), mtcConfig.unitDescriptor);
			str[0] = strlen(mtcConfig.unitDescriptor);
			showString(str);
					
			/* init the view module */
			ViewInit(5, 5, 390, 340);
			ViewDrawBorder(gWinPtr, false);
		
			/* start the data flow */
			if (!MTC_StartSendingData(gMTC, MTCCompNone, 1))
				showString("\pMTC did not respond to StartSendingData");
			
			gConnected = true;
			}
		
		else {
			err = MTC_GetLastError(NULL);
			strcpy((char *)(str + 1), MTC_GetErrorString(err));
			str[0] = strlen(MTC_GetErrorString(err));
			showString(str);
			}
		}
	
	else
		showString("\pAlready connected.");
		
	return;
	}



static void disconnect(void) 
	{
	if (gConnected) {
		if (MTC_Delete(gMTC))
			showString("\pConnection to MTC closed.");
		else
			showString("\p¥error closing connection¥");
		}
	
	gMTC = NULL;
	gConnected = false;
	free(gPress);
	
	return;
	}



void showRx(void)
	{
	BOOL			dataAvail;
	MTCErr			e;
	Str255			errStr;
	
	/* Report any errors */
	do {
		e = MTC_GetLastError(gMTC);
		if (e) {
			strcpy((char *)(errStr + 1), MTC_GetErrorString(e));
			errStr[0] = strlen(MTC_GetErrorString(e));
			showString(errStr);
			}
		}
	while (e);		

	/* If new data is available, display it. */
	if (gConnected && gPress) {
		if (gDisplayNorm)
			dataAvail = MTC_GetNormalizedData(gMTC, gPress);
		else
			dataAvail = MTC_GetRawData(gMTC, gPress);
		
		if (dataAvail)
			ViewUpdateAll(gWinPtr, gPress);
		}
			
	return;
	}



static void showVersion(void)
	{
	Str255 v;
	
	MTC_GetAPIVersion((char *)(v + 1));
	v[0] = strlen((char *)(v + 1));
	
	showString(v);
	
	return;
	}


				
				
/* ************************************************************************* */	
static void newLine(void)
	{
	RGBColor 	c;
	Rect 		r;
	static int 	x = 5, y = yStrTop;
			
	/* increment the output line, clear the screen and reset. */
	y += yLine;

	if (y > yStrMax) {
		/* clear the screen */
		r.top = yStrTop - yLine;
		r.left = x;
		r.right = xStrMax;
		r.bottom = yStrMax + yLine;
		c.green = c.blue = c.red = 0xFFFF;
		RGBForeColor(&c);
		PaintRect(&r);
			
		/* reset the color */
		c.green = c.blue = c.red = 0;
		RGBForeColor(&c);
			
		y = yStrTop;
		}

	MoveTo(x, y);
	
	return;
	}

static void showString(Str255 s)
	{
	SetPort(gWinPtr);
	newLine();
	DrawString(s);
	return;
	}
	


/* ************************************************************************* */	



/* Handle the mouse-down events.  This may invlove calling menuSelectHdlr(). */
