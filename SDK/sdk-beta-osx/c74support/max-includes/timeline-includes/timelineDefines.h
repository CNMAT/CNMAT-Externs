	/* timelineDefines.h */

#define	TIMELRES	3096
#define	TIMELOPEN	3096
#define	TIMELCLOS	3097
#define	TIMELGREY	3098

#define	HCURDiv		4000
#define VCURDiv		4001

/* color palette indices */

#include "colors.h"

/* used for window titles */
#define	NOBRACKETS	0
#define BRACKETS	1

#define	TIMESTRIPHEIGHT	50			/* height of the time ruler area */
#define	XRULEROFFSET	5			/* ruler offsets into timeline window */
#define	YRULEROFFSET	11			/* ruler offset from bottom of time strip */

/* global lines */

#define TRANSPORTliney	22
#define TIMERULERliney	51
#define TRACKstart		51

#define CLOCKpicindex	4206
#define CURSORpicindex	4207

#define PATCHICONpicindex	4220

/* transport line coordinates */

#define CLOCKBOXleft	134
#define CLOCKBOXtop		5
#define CLOCKBOXright	146
#define CLOCKBOXbottom	17

#define TIMEBOXleft 151
#define TIMEBOXtop	5
#define TIMEBOXright	213
#define TIMEBOXbottom 16

#define CURSORBOXleft	221
#define CURSORBOXtop	5
#define CURSORBOXright	234
#define CURSORBOXbottom	17
#define CURSORBOXtextx	

#define CURSORDISPleft	241
#define CURSORDISPtop	5
#define CURSORDISPright	287
#define CURSORDISPbottom 17
#define CURSORDISPtextx 242
#define CURSORDISPtexty 14

#define CURSORSELleft	290
#define CURSORSELtop	5
#define CURSORSELright	356
#define CURSORSELbottom 17
#define CURSORSELtextx 291
#define CURSORSELtexty 14

#define TRANSPORTpicindex	4210

/* ruler strip coords */

#define TRACKMENUleft		5
#define TRACKMENUright		47
#define TRACKMENUtop		26
#define TRACKMENUbottom		47
#define TRACKMENUpicindex	4202

#define DISPLAYMENUleft		51
#define DISPLAYMENUright	92
#define DISPLAYMENUtop		26
#define DISPLAYMENUbottom	47
#define DISPLAYMENUpicindex	4204

#define FONTHEIGHTOFFSET	9
#define FONTWIDTHOFFSET		5
#define	FONTWidth			6

#define TS_NUMtop			23
#define TS_NUMtexty			32
#define TS_NUMtextx(x)		((x)->tl_divider-FONTWIDTHOFFSET)
#define TS_NUMmajordiv		40
#define TS_NUMminordiv		4
#define TS_NUMliney			42
#define TS_NUMbottom		33
#define TSmajorY			8
#define TSminorY			6

#define TIMEHANDLEtop		36
#define	TIMEHANDLEbottom	40
#define TIMEHANDLEwidth		8
#define TIMEHANDLEpicindex	4200

#define TIMESELECTtop		43
#define	TIMESELECTbottom	(TRACKstart)

/* track-specific cooordinates */

#define	ACTIONICONBOXleft 19
#define	ACTIONICONBOXtop 3
#define	ACTIONICONBOXwidth 16
#define	ACTIONICONBOXheight 16

#define	MUTEBOXwidth 12
#define	MUTEBOXheight 12

#define	NAMEBOXleft		38
#define	NAMEBOXtop		4
#define NAMEBOXheight	16
#define NAMEBOXtextx	42
#define NAMEBOXtexty	15

#define	NUMBERBOXleft	2
#define	NUMBERBOXtop	2
#define	NUMBERBOXwidth 15
#define NUMBERBOXheight 16
#define NUMBERBOXtextx	5
#define NUMBERBOXtexty	15

/* zooming */

#define NumZooms 10

/* autoscroll messages */

#define TAScalcold 1
#define TASerase 2 
#define TASdraw 3
#define TASrecalc 4
#define TAStrack 5

#define	LEFTEdge(win)	((win)->w_x1)
#define	TOPEdge(win)	((win)->w_y1)
#define	RIGHTEdge(win)	((win)->w_x2)
#define	BOTTOMEdge(win)	((win)->w_y2)

#define	WINDWIDTH(win)	(RIGHTEdge(win)-LEFTEdge(win))
#define	WINDHEIGHT(win)	(BOTTOMEdge(win)-TOPEdge(win))

#define MINDIVIDER		(102)

#define	MSEC24FPS		(1000L/24L)
#define	MSEC30FPS		(1000L/30L)
#define	DEFTEMPO		(120L)
#define	MSEC24PPQ		(1000L/48L)

#define	PIXPERMS(x)		(long)(x->tl_pixPerMS/x->tl_timeScale)
#define	PIXPERMAJ(x)	(long)(x->tl_pixPerMS*x->tl_msPerMaj/x->tl_timeScale)
#define	PIXPERMIN(x)	(long)(x->tl_pixPerMS*x->tl_msPerMaj/(x->tl_timeScale*x->tl_minorPerMaj))

#define	TIDefauleTRACKHEIGHT	21

/* indexes of the beginning of an "event" message stored in a timeline file */

#define EM_DATATYPE 0
#define EM_EDITOR 1
#define EM_TRACK 1
#define EM_MESSAGE 2
#define EM_START 3
#define EM_DURATION 4
#define EM_TOP 5
#define EM_BOTTOM 6

enum{
	TL_IDLE,
	TL_EDIT,						/* editing something */
	TL_SETTime,
	TL_DRAGTime,					/* transient state */
	TL_SELECTTrack,
	TL_MAKENewTrack,				/* transient state */
	TL_PLAY,
	TL_RECORD
};

enum {
	TIMEDisplayMSec,
	TIMEDisplayMIDIClk,
	TIMEDisplayFPS
};

enum {
	acINTERNAL,
	acRESOURCE,
	acPATCH
};

/*	timeline qflags */
#define TLQAll			(0xFFFF)
#define	TLQTimeRuler	(0x01<<0)
#define	TLQTimeRulerNumbers	(0x01<<1)
#define	TLQTimeBar		(0x01<<2)
#define	TLQTimeDisplay	(0x01<<3)
#define TLQMenuArea		(0x01<<4)
#define	TLQTrackMenu	(0x01<<5)
#define	TLQRecalcTracks	(0x01<<6)
#define	TLQTrackBottom	(0x01<<7)
#define	TLQBackground	(0x01<<8)
#define	TLQLater		(0x01<<9)
#define	TLQTimeSelection (0x01<<10)

/*	track qflags */
#define TRQAll			(0xFFFF)
#define TRQNumber		(0x01<<0)
#define TRQName			(0x01<<1)
#define TRQAction		(0x01<<2)
#define	TRQHilite		(0x01<<3)
#define TRQBackAction	(0x01<<4)
#define	TRQAllData		(0x01<<5)
#define	TRQRedraw		(0x01<<6)
#define TRQBottom		(0x01<<7)
#define	TRQMute			(0x01<<8)
#define	TRQPreview		(0x01<<9)
#define	TRQSolo			(0x01<<10)
#define TRQBottomDivider (0x01<<11)
#define TRQAllAction	(TRQBackAction|TRQNumber|TRQName|TRQAction|TRQHilite|TRQMute|TRQPreview|TRQSolo|TRQBottomDivider)

/* track finding stuff */
#define onTRACKDIVIDER	1
#define inTRACK			2
#define inOUTERSPACE	3
#define	inEDIT			4

#define TLMESSsize		60
