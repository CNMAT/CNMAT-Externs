#ifndef __EXT_USER
#define __EXT_USER

typedef struct box
{
	struct object b_ob;

	short b_inlet:1;			/* this box is an inlet */
	short b_outlet:1;			/* this box is an outlet */
	short b_textdirty:1;		/* text has been changed */
	short b_drawfirstin:1;	/* draw inlet for the real object */
	short b_outline:1;		/* draw the box */
	short b_drawn:1;			/* has been drawn */
	short b_spooled:1;		/* is spooled for drawing */
	short b_growy:1;			/* can grow in y direction */
	unsigned short b_inset:3;	/* top-and-bottom inset for text */
	short b_selected:1;		/* on if selected */
	unsigned short b_color:4;	/* index into palette (default 0) */
	unsigned short b_fsize;	/* font size */
	short b_fontindex;		/* font index (get from window) */
	short b_growboth:1;		/* can grow independently in both directions */
	short b_ignorelockclick:1; /* ignore clicks if patcher is locked (be "transparent") */
	short b_hidden:1;			/* hide box on lock */
	short b_nogrow:1;			/* don't grow at all */
	short b_hilited:1;		/* hilited, typeable object */
	short b_pending:1;		/* typing is pending */
	short b_hiliteable:1;		/* hiliteable object */
	short b_savehilite:1;		/* save hilite state when window is not active */
	short b_saveselect:1;		/* save select state when window is not active */
	short b_undo:2;			/* undo bit */
	short b_bogus:1;			/* box's bogus bit--patcher creates its inlets/outlets on connect */
	short b_checkinvis:1;		/* check when invizzing */
	short b_visible:1;		/* for vnewobj patchers, is box visible? */
	short b_drawinlast:1;		/* used by vnewobj, inlet, outlet to draw inlets/outlets */
	short b_dynamic:1;
	short b_remote:1;			/* remote box */
	short b_ignoreclick:1;	/* user setting for ignoring a lock click */
	short b_background:1;	// part of background
	short b_transparent:1;
	short b_clipaware:1;
	short b_unused:4;		/* extra space for future bits */
	short b_temp;					/* temporary space */
	struct box *b_upd;		/* used in saving */
	Rect b_rect;					/* location */
	struct box *b_next;				/* next box in patcher */
	struct patcher *b_patcher;		/* owning patcher */
	void *b_firstin;				/* first inlet if nonzero */
	void *b_binbuf;					/* contained message in binary */
	union
	{
		struct outlet *b_vpout;		/* patchoutlet in this patcher's visobj */
		struct inlet *b_vpin;		/* patchinlet in this patcher's visobj */
		struct textbox *b_teh;		/* text editor handle */
	} b_un;
} Box, t_box;

typedef struct patcher
{
	struct object p_ob;
	struct windpub *p_wind;		/* window object */
	void *p_vnewobj;			/* containing box if any */
	void *p_instr;				/* associated instrument if any */
						/* graphing */
	struct box *p_boxupd;		/* list of boxes to redwraw */
	struct patchline *p_lineupd;/* lines to redraw */
	long p_whenupd;				/* when to redraw */
						/* selection and editing */
	void *p_updclock;			/* update clock */
	struct box *p_seltext;		/* if != 0, box to send keys to */
						/* other */
	struct box *p_box;			/* the boxes */
	struct patchline *p_line;	/* the lines */
	Rect p_span;				/* rectangle spanned by all boxes */
	Point p_scroll;				/* last known scroll values */
	void *p_pushed;				/* patcher we pushed to name this one "Pat" */
	struct box *p_hilited;		/* "operational selection" */
	void *p_parent;				/* parent object (eg. pr-type thing) */
	short p_nselect;				/* number of selected boxes */	
	short p_vol;					/* filesys volume to look up classes in */
	char p_lock;				/* on if the patcher is locked */
	char p_dirtybar;			/* on if the edit bar needs drawing */
	char p_noedit;				/* can't unlock this patcher */
	char p_enabled;				/* is MIDI enabled? */
	short p_pasted:1;				/* just did a paste (don't edit text) */
	short p_freed:1;				/* being freed, no need to redraw */
	short p_remote:1;				/* has remote version */
	short p_host:1;				/* has host version */
	short p_traceactive:1;		/* making patcher active during trace */
#ifndef NOTIMELINE
	short p_tl:1;					/* timeline subpatcher? */
	short p_unused:2;
#else
	short p_unused:3;				/* unused flags */
#endif
	char p_cursor;				/* index of the user interface object of the cursor */
	char p_loadbanged;			/* have I been loadbanged already? */
	char p_color;				/* is this patcher in color (mult devices only) == 4 for 16, 8 otherwise */
	void *p_hiout;				/* hilited outlet */
	struct box *p_hibox;		/* hilited outlet's box */
	void *p_okhook;				/* object to send okclose message to */
	long p_search;				/* search info field */
	void *p_assoc;				/* object (eg. lib) associated with this patcher */
	void *p_undo;				/* undo object */
	short p_deffont;				/* default font for new objects */
	short p_defsize;				/* default font size for new objects */
	void *p_collective;			/* collective record if this patcher comes from one */
} t_patcher, Patcher;

/* box flags */

#define F_DRAWFIRSTIN	1		/* draw first inlet */
#define F_GROWY			2		/* can grow in y direction by dragging */
#define F_NODRAWBOX		4		/* don't draw the box  */
#define F_MOVING		8
#define F_DRAWN 		16
#define F_GROWBOTH		32		/* can grow independently in both x and y */
#define F_IGNORELOCKCLICK 64    /* box should ignore a click if patcher is locked */
#define F_NOGROW		128
#define F_HILITE		256
#define F_DRAWINLAST	512		/* draw inlets after update method */
#define F_TRANSPARENT	1024
#define F_SAVVY			2048
#define F_BACKGROUND	4096
#define F_NOFLOATINSPECTOR 8192	// no floating inspector window

// clipregion message

#define CLIPRGN_REGION 0
#define CLIPRGN_RECT 1
#define CLIPRGN_EMPTY 2
#define CLIPRGN_COMPLEX 3

/* other constants */

#define POX 10	/* patcher outlet width and height */
#define POY 3
#define SIZE1 15	/* frequently used control dimensions */
#define SIZE2 50
#define UNIT 256
#define USHIFT 8

#endif

