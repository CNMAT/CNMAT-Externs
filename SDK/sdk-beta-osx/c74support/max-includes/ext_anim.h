/* anim.h */
#include <Palettes.h>

#define ANIM

#ifdef ANIM

typedef struct offscreen {
	struct object *o_ob;
	PixMapHandle offPix;
	Handle offMem;
	BitMap offBits;
	BitMap curBits;
	PixMapHandle curPix;
	Rect imageClip,screenRect;
	Rect temp;
	GrafPtr dest;
	Boolean realityChecked;
	RgnHandle saveVisRgn,wholeRgn,mRgn,invisRgn,visCopy;
	struct sprt *spriteList;
	struct sprt *lastSprite;		/* used eventually... */
#ifdef _Offscreen_
	GWorldPtr world;
#else
	Ptr world;
#endif
	CGrafPtr savePort;
	GDHandle saveDevice;
} OffScreen;

typedef struct colorinfo {
	short c_hasColorQD;
	short c_depth;
	short c_has32bitQD;
	short c_inColor;
	short c_curDevH;
	short c_curDevV;
} CInfoRec;

typedef struct gwind {
	struct object g_ob;
	struct wind *g_wind;
	short g_style;
	Point g_last;
	PaletteHandle g_palette;
	char g_inited;
	char g_mousein;				/* mouse in window flag */
	struct symbol *g_sym;		/* associated symbol (name) */
	struct object *g_assoc;		/* associated graphic object */
	OffScreen *g_off;			/* offscreen buffer */
	struct gwind *g_next;		/* next graphics window in the list */
} GWind, t_gwind;

typedef struct sprite {
	struct object s_ob;
	GrafPtr s_dest;				/* screen dest */
	Rect s_rect;				/* rectangle */
	BitMapHandle s_mask;		/* mask */
	RgnHandle s_rgn;			/* mask rgn */
	short s_number;				/* sprite number (priority) */
	char s_drawn;				/* is it drawn */
	char s_change;				/* message to sprite proc to go to its "next" frame */
	void *(*s_proc)();			/* procedure that draws */
	long s_frame;				/* current frame, used by s_proc */
	long s_misc;				/* used by s_proc */
	void *s_assoc;				/* an associated object */
	OffScreen *s_owner;			/* owning system */
	struct sprite *s_prev;		/* link */
	struct sprite *s_next;		/* link */
} t_sprite;

#endif