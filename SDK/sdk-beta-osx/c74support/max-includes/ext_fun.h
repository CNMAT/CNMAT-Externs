/*************************/
/* Copyright 1988 IRCAM. */
/*************************/

/*    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++LIST OBJECT DEFINITION    */

typedef struct zll		/* doubly linked list */
{											/* note:  no obj header  */
	long	z_x;
	long	z_y;
	struct zll *z_next;
	struct zll *z_prev;
} Zll;

/*    ++++++++++++++++++++++++++++++++++++++++++++++++++++FUN-BUFFER OBJECT DEFINITION    */

typedef struct funbuff
{
	struct object f_obj;
	void *f_outlet;
	void *f_outlet1;
	void *f_outlet2;
	struct zll f_head;	   	/* head of double linked list of function elements */
	struct zll *f_foot;		/* foot in the door pointer for list  */
	long f_gotoDelta;		/* used by goto and next */
	long f_selectX;			/* selected region start   */
	long f_selectW;			/* selected region width   */
	struct symbol *f_sym;	/* filename */
	long f_y;				/* y-value from inlet */
	char f_yvalid;			/* flag that y has been set since x has */
	char f_embed;			/* flag for embedding funbuff values in patcher */
} Funbuff;

#define DEFSIZE 128		/* default size 128 elements */
#define DEFRANGE 128	/* range 0-127 */
#define ELEM long	/* table element type */

typedef struct tabwind
{
	struct object w_ob;
	struct windpub *w_wind;
	struct symbol *w_sym;
	ELEM **w_tab;
	long w_size;		/* size of actual table */
	long w_range;		/* editable range (y values) */
	long w_xoff;		/* x-value of first point displayed */
	long w_yoff;		/* y-value of top line */
	long w_xshift;		/* x-shift for displaying */
	long w_yshift;		/* y-shift */
	long w_xzoom;		/* x zoom */
	long w_yzoom;		/* y zoom */
	long w_cliprange;	/* range, anded to be 0 mod 1 << w_xzoom */
	long w_dpoints;		/* display points (size >> xshift rounded up) */
	long w_dragx;		/* previous x-value changed by dragging */
	long w_selstart;	/* starting sample of selected region (inclusive) */
	long w_selend;		/* ending sample of selected region (exclusive) */
	long w_saveno;		/* <a> added; serial number of last time saved to prevent duplicates */
	void *w_clock;		/* <a> clock to delay updating */
	void *w_qelem;		/* <a> qelem to queue updating to low pri */
	short w_xdisp;		/* number of points displayed */
	short w_ydisp;		/* number of y-values displayed */	
	char w_signed;		/* flag for signed display */
	char w_clkset;		/* <a> flag indicating clock is already set */
	char w_selmode;		/* tool 0 = select,1 = move,2 = line,3 = pencil */
	long w_xwas;       	/* last x value legended */
	long w_ywas;		/* last x value legended */
	short w_xcenter;		/* last x point legend drawn at */
	short w_ycenter;		/* last y point legend drawn at */
	Point w_scroll;     /* last known scroll values, a la Patcher */
	char w_ylegmode;    /* 0 for Y legend as numbers,
							1 for notes.  w_signed turns this off */
	char w_embed;		/* save it in a patcher */
	char w_crossguide;	/* show cross guide */
	char w_act;			/* activated? */
	long w_vwas;		/* last value legended */
	struct num *w_numrange;	/* range numerical */
	struct num *w_numsize;	/* size numerical */
	struct table *w_reftab;	/* list of reference tables */
	struct object *w_client;	/* send "tabchanged" message when table is changed */
	short w_xscale;		/* scrollbar scaling when values > 16 bits */
	short w_yscale;
} t_tabwind, Tabwind;

