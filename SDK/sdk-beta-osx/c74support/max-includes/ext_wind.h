/*************************/
/* Copyright 1988 IRCAM. */
/*************************/

typedef struct windpub
{
	struct object w_ob;
	short w_x1;					/* location of window */
	short w_x2;
	short w_y1;
	short w_y2;
	short w_xoffset;				/* scroll offsets */
	short w_yoffset;
	short w_scrollgrain;			/* scroll grain in pixels */
	short w_refcount;				/* reference count */
	char w_vis;					/* visible */
	char w_titled;				/* has a title */
	char w_grow;				/* has a grow region */
	char w_close;				/* has a close region */
	char w_scrollx;				/* has an x scroll region */
	char w_scrolly;				/* has a y scroll region */
	char w_dirty;				/* dirty flag (can save) */
	char w_scratch;				/* scratch flag -- don't complain on close */
	char w_bin;					/* binary save */
	char w_font;				/* text font */
	char w_fsize;				/* font size */
	char w_fontindex;			/* font index (0 = 9 points, 7 = 24 points) */
#if !TARGET_API_MAC_CARBON
	GrafPort w_gp;				/* grafport */
#endif
} t_windpub, Windpub;

typedef struct wind
{
	struct object w_ob;
	short w_x1;					/* location of window */
	short w_x2;
	short w_y1;
	short w_y2;
	short w_xoffset;				/* scroll offsets */
	short w_yoffset;
	short w_scrollgrain;			/* scroll grain in pixels */
	short w_refcount;				/* reference count */
	char w_vis;					/* visible */
	char w_titled;				/* has a title */
	char w_grow;					/* has a grow region */
	char w_close;				/* has a close region */
	char w_scrollx;				/* has an x scroll region */
	char w_scrolly;				/* has a y scroll region */
	char w_dirty;				/* dirty flag (can save) */
	char w_scratch;				/* scratch flag -- don't complain on close */
	char w_bin;					/* binary save */
	char w_font;				/* text font */
	char w_fsize;				/* font size */
	char w_fontindex;			/* font index (0 = 9 points, 7 = 24 points)  */
#if !TARGET_API_MAC_CARBON
	WindowRecord w_wind;
#endif
		/* (end of public region) */
	short w_vsmax;				/* vertical scroll max */
	ControlHandle w_vscroll;
	short w_hsmax;				/* horizontal scroll max */
	ControlHandle w_hscroll;
	void *w_assoc;				/* associated object */
	method w_idle;				/* window idle function */
	char w_name[80];			/* filename = window title */
	short w_vol;					/* file volume number (working directory) */
	short  w_proc;				/* window proc id (0 = normal) */
	char w_keeptitle;			/* set window title on saveas */
	char w_canon;				/* slot in canonical list of locations */
	char w_silentgrow;			/* has invisible grow region -- don't draw icon */
	char w_color;				/* try to make color window if you can */
	char w_bits;				/* number of bits (i.e. 2 for b&w) */
	char w_divscrollx;			/* divided horiz scroll bar */
	char w_zoom;				/* has zoom rect */
	short w_realfont;				/* real font in 16 bits */
	short w_hsleft;				/* left of scroll bar */
	Rect w_oldsize;
	short w_oldproc;
	char w_select;				/* always select on click */
	char w_frame;				/* special 'frame' mode */
	long w_flags;				/* used by thispatcher window message */
	WindowPtr w_wptr;			// DDZ 6/11/98, contains pre-existing window or ptr to w_wind
	long w_local;				// window's memory stored locally
	Rect w_growbounds;			// optional grow bounds for a window
	char w_helper;				// is a helper (used by the Windows menu)
} t_wind, Wind;

	/* flags for window object */
#define WVIS		01		/* visible */
#define WGROW		02		/* grow region */
#define WSCROLLX	04		/* x scrollbar */
#define WSCROLLY	010		/* y scrollbar */
#define WCLOSE		020		/* close region */
#define WKEEPT		040		/* keep window title away from saveas */
#define WSGROW		0100 	/* silent grow region */
#define WCOLOR		0200 	/* color window */
#define WPATCHPROC	0400 	/* patcher wdef */
#define WSHADOWPROC 01000 	/* shadow wdef */
#define WDIVSCROLLX 02000 	/* divided horizontal scroll bar (min 140 pixels) */
#define WZOOM		04000	/* has a zoom box */
#define WSELECT		010000	/* always select on click (disobey all windows active) */
#define WFROZEN     020000  /* prevent "thispatcher window" messages */
#define WFLOATING	040000	// floating window (OS 8.6 or higher)

#define W_WIDTH(w) (w->w_x2 - w->w_x1)
#define W_HEIGHT(w) (w->w_y2 - w->w_y1)
#define SBARWIDTH	15
#define DIVBARMIN	130

#define ISMAXWINDOW(window) ((window->windowKind == userKind) && (window->refCon))

/* event codes returned from patcher WDEF */

#define inLock 9		/* lock on/off */
#define inRock 10		/* midi on/off */
#define inDock 11		/* dock on (eventually) */

#define aaYes 1
#define aaNo 2
#define aaCancel 3

#define C_ARROW		1
#define C_WATCH		2
#define C_IBEAM		3
#define C_HAND		4
#define C_CROSS		5
#define C_PENCIL	6
#define C_GROW		8
