#include "patcher2.h"

#define	MAX_TLOUTLETS	20

#ifdef TIMERECORD
typedef struct tiRecordBuf {
	long	time;
	long	track;
	long	value;
} tiRecordBuf, *TiRecordBuf;
#endif

typedef struct timeline {
	struct patcher tl_p;
	
	void		**tl_outlet;			/* timeline outlets */
	long		tl_outletCount;			/* number of outlets */
	Symbol		*tl_instanceName;		/* instance name */
	
	short		tl_mode;				/* interface focus mode */
	short		tl_playMode;
	short		tl_saveMode;
	short		tl_vol;					/* used for searching for timeline templates */
	
	long		tl_timeSelectionBegin;
	long		tl_timeSelectionEnd;

	struct windpub *tl_wind;			/* our beloved window */
	long		tl_hScrollMax;			/* maximum scroll bar value in horizontal */
	long		tl_vScrollMax;			/* maximum scroll bar value in vertical */
	Point		tl_offset;				/* window offsets of where I think I am */
	Point		tl_mouseDown;
	
	Point		tl_oldIdle;

	long		tl_trackCount;			/* number of tracks in our timeline */
	struct		tlTrack	*tl_track;		/* pointer to track data structures */
										/* can either implement as a list or an
									   	array of tracks */
	long		tl_soloCount;
	
#ifdef TIMERECORD
	long		tl_recordBufferSize;
	TiRecordBuf	tl_recordBuffer;
	TiRecordBuf	tl_recordPtr;
	long		tl_recordCount;
	long		tl_recordTime;
#endif

	long		tl_loopTo;				/* if looping, what the end point is */
	void		*tl_clock;				/* the clock */
	long		tl_currentTime;			/* current time marker */
	long		tl_timeOffset;			/* time base offset in mSecs */
	short		tl_oldPix;				/* for drawing time handle */
	short		tl_oldXor;				/* for drawing time handle */
	long		tl_maxTime;				/* time of last event */
	long		tl_pageScroll;			/* scroll amount in units of MS */
	
	long		tl_firstTrack;			/* first track to display */
	long		tl_displayTime;			/* display time marker */
	Rect		tl_whatToInvert;		/* for max-drag button routine */
	Rect		tl_timePane;			/* time display box */
	Rect		tl_menuPane;	
	Rect		tl_timeStrip;			/* time display strip */
	short		tl_trackBottom;
	short		tl_divider;				/* x location of action/editor divider */
	
	Boolean		tl_saveWithPatcher;
	Boolean		tl_inColor;
	Boolean		tl_showMute;
	Boolean		tl_autoPlayScroll;
	float		tl_pixPerMS;			/* pixels per millisecond */
	long		tl_timeScale;			/* time ruler scaling */
	long		tl_zoomLevel;
	long		tl_timeFormat;			/* time display format */
	long		tl_unitsPerSec;			/* units per second */
	long		tl_msPerMaj;			/* number of fundamental time units per major tick */
	long		tl_minorPerMaj;			/* number of minor tick marks between majors */
	short		tl_majorTickLength;		/* length of minute markers */
	short		tl_minorTickLength;		/* length of minors */
	
	void		*tl_selBuffer;
	struct		tlTrack	*tl_targetTrack;/* target of keyboard input */

	void		*tl_saveThing;

	short		tl_clockMode;
	short		tl_updflags;
	void		*tl_updclock;
	long		tl_whenupd;
	struct		tlTrack *tl_updateList;
	Symbol		*tl_setclock;
	void		*tl_newobj;				/* pointer to box */
	void		*tl_proxy;				/* required by obscure limitation */
	long		tl_inReceive;			/* for proxy */
	char		tl_message[TLMESSsize];	/* posted message to update */
} timeLine,*Timeline;
