enum{
	TR_IDLE,
	TR_SELECTED,
	TR_EDITName,
	TR_SELECTEvent,
	TR_EDITEvent,
	TR_ADDNewEvent,					/* transient state */
	TR_DRAGMoveEvent,				/* transient state */
	TR_DRAGSelectEvent,				/* transient state */
	TR_STRETCHEvent,				/* transient state */
	TR_DORMANT,
	TR_PLAY
};

#define	TR_EDITModes	3			/* track edit states */
#define	TRACK_EDIT		0
#define	TRACK_ARRANGE	1
#define	TRACK_LOCKED	2

#include "linkThing.h"

typedef	struct loopPt{
	long	l_begining;				/* begining mark */
	long	l_end;					/* end mark */
	long	l_repetionCount;		/* number of repetitions */
	struct	loopPt *l_nextLoop;		/* next loop structure */
} loopPt, *LoopPt;

typedef	struct tlTrack {
	struct		object tr_ob;
	Symbol		*tr_name;			/* track name */
	long		tr_number;			/* track number */
	
	Boolean		tr_function;		/* am I a function track? */
	long		tr_sample;			/* what is my sample interval? */
	Symbol		*tr_tag;			/* data tag */
	long		tr_dataMin,tr_dataMax;	/* for scaling of output data */
	float		tr_dataScale;		/* precomputed scaling factor */
	long		tr_outletIndex;		/* outlet index to retreive later */
	void		*tr_outlet;			/* pointer to timeline outlet */
									/* outlet is assigned by popup menu on track pane */
	
	short		tr_mode;			/* track mode */
	short		tr_saveMode;

	struct timeline *tr_parent;
	struct windpub *tr_wind;			/* window inherited from parent */
	Boolean		tr_inColor;			/* are we in color? */
/*
	RGBColor	tr_backColor;		== background color ==
	RGBColor	tr_foreColor;		== foreground color for postview mode ==
*/
	Boolean		tr_preview;			/* preview mode? */
	Boolean		tr_mute;			/* are events active in play back? */
	Boolean		tr_solo;
	
	Boolean		tr_spooled;			/* has a redraw been requested? */
	Boolean		tr_active;			/* is this an active track? */
	Boolean		tr_high;			/* is it highlighted? */
	Boolean		tr_visible;
	Rect		tr_textPane;
	Rect		tr_nameBox;
	Rect		tr_numberBox;
	Rect		tr_actionIconBox;
	Rect		tr_padlockBox;
	Rect		tr_editBox;
/*
	RgnHandle	tr_editRgn;
*/
	short		*tr_divider;
	
	long		tr_selStart;		/* start of selection */
	long		tr_selEnd;			/* end of selection */
	Rect		tr_selectionBox;	/* for selecting events */
	long		tr_selCount;		/* count of selected events */
	RgnHandle	tr_selRgn;			/* selection region for dragging */
	Point		tr_selDelta;		/* how much has region moved since last time? */
/*
	RgnHandle	tr_downRgn;			-- region before move --
*/
	long		tr_selBufStartTime;	/* selection buffer start time */
	
	long		tr_downTime;		/* mouse down time */
	Point		tr_selDown;			/* selection point offsets */
	Point		tr_prevDown;		/* previous mouse down point */
	Point		tr_mouseDown;		/* mouse down point */
	Boolean		tr_autoScrolling;	/* am I auto scrolling? */
	Boolean		tr_nameInited;		/* has name TERec been initialized */
	short		tr_vol;				/* location of action class */
	Symbol		*tr_actionName;		/* name of action class */
	method		tr_actionIcon;		/* action class icon */
	short		tr_actionType;		/* action type */
	struct		messageList *tr_messageList;	/* list of messages track deals with */
	Object		*tr_action;			/* the action class instance */
	
	short		tr_editMode;		/* current edit mode */
	Boolean		tr_editable;		/* edits allowed? */

	short		tr_prevOffset;
	short		tr_vOffset;			/* track vertical offset */
	short		tr_height;			/* track height */
	short		tr_expandedHeight;	/* track height when expanded, if collapsed */

	struct		event *tr_nextEvent;	/* the next event to play */
	struct		event *tr_event;		/* events of some sort */
	struct		event *tr_targetEvent;	/* event that receives keys */

	long		*tr_currentTime;
	long		tr_oldPix;
	Boolean		tr_showTimeBar;		/* turn on and off time bar display */
	long		*tr_timeOffset;
	long		*tr_displayTime;
	
	LoopPt		tr_loops;			/* loop points for track */
	LoopPt		tr_currentLoop;		/* which loop are we in the middle of? */
	
	short		tr_updflags;	
	Boolean		tr_invalid;
	long		tr_updStart;		/* update region */
	long		tr_updEnd;
	Rect		tr_updRect;
	struct		event *tr_updateList;	/* list of events to update */

	struct		linkThing tr_linkThing;	/* end event stuff */

	TEHandle	tr_teh;				/* text edit handle */
	
	void		*tr_saveThing;
	struct		tlTrack	*tr_upd;	/* timeline update list */
	struct		tlTrack	*tr_next;	/* pointer to next track (if linked list) */
	
	void		*tr_extraThing;		/* reserved for later use */
} tlTrack, *TLTrack;

