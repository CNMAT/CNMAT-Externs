// #include "typedefs.h"
#include "ext.h"
#include "ext_user.h"
#include "ext_anim.h"
#include "ext_wind.h"
#include "ext_strings.h"
#include <Palettes.h>
#include <SetUpA4.h>
#include "ext_common.h"	// changed from common.h
#include <A4Stuff.h>

#include <math.h>	// zk 6/96

/* 11/1/93 SDE:

	1)  added local int message to turn off drawing during mouseclick
			local 1 (the default) -- Always draws when being clicked
			local 0 			  -- Never draw when being clicked
	2)  added LineSegment message, with optional color argument.
	3)  added optional color argument to Paint & Frame Oval and Rect
	
*/


/* DDZ changed <> to "" for max-specific files (12/14/92) */

int	errno;

// #include <math.c>

#define MINMAX(a,b,c)			MIN(c,MAX(a,b))

#define MONACO			(4)
#define FONTSIZE		(9)

#define	CHAR_WIDTH		6
#define	CHAR_HEIGHT		11
#define	CHAR_LEADING	0

#define BACKSPACE		(8)
#define CR				(13)
#define LEFT_ARROW		(28)
#define RIGHT_ARROW		(29)
#define UP_ARROW		(30)
#define DOWN_ARROW		(31)

#define	BORDER_WIDTH	(2)
#define	BORDER_HEIGHT	(2)

#define myRes 3150

typedef struct LCD
{
	struct	box lcd_box;
	
	Boolean	lcd_debug;
	
	short		lcd_row;
	short		lcd_col;	/* cursor position */
	short		lcd_pIndex;	/* palette color index */
	PaletteHandle	lcd_palette;
	short		lcd_penMode;
	PenState	lcd_penState;
	
	Boolean	lcd_shiftClick;
	RgnHandle	lcd_region;
	
	short		font;		/* font number */
	short		f_size;		/* font size */
	short		f_width;	/* font width */
	short		f_height;	/* font height */
	short		f_ascent;
	short		f_descent;
	short		f_lineSpace;/* font line spacing */
	
	short		cols;	/* maximum for display size width */
	short		rows;	/* maximum for display size height */
	
	Rect	lcd_oldRect;	/* old one for clearing and update */
	Point	lcd_where;
	
	double	reson_sampleRate;
	double	reson_spectScale;
	double	reson_trimRatio;
	
	char	*screen;		/* screen buffer for later */
	
	void	*lcd_outlet;	/* outlet for LCD */
	
	short		local; /* if set, draw when being clicked SDE */
	
	short fRefNum;
	char fn[64];
	void *file_outlet;
} Lcd;

void setUpPalette(Lcd *x, RGBColor *oldColor, PaletteHandle *oldPalette);
void restorePalette(Lcd *x, RGBColor *oldColor, PaletteHandle *oldPalette);
void LCD_makepalette(void);
void LCD_ascii(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_doascii(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_assist(Lcd *x, void *b, long m, long a, char *s);
void LCD_clear_area(Lcd *x);
void LCD_local (Lcd *x, long loco);
void LCD_click(Lcd *x, Point where, short modifiers);
void LCD_color(Lcd *x, long color);
void LCD_debug(Lcd *x);
void LCD_drag(Lcd *x, Point pt, short button_down);
void LCD_drawTxt(Lcd *x, char *stng);
void LCD_calcfont(Lcd *x);
void LCD_font(Lcd *x, long font, long size);
void LCD_dofont(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_frameOval(Lcd *x, long left, long top, long right, long bottom, long color);
void LCD_doframeOval(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_frameRect(Lcd *x, long left, long top, long right, long bottom, long color);
void LCD_doframeRect(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_free(Lcd *x);
void LCD_Line(Lcd *x, long deltaX, long deltaY);
void LCD_doLine(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_LineTo(Lcd *x, long deltaX, long deltaY);
void LCD_doLineTo(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_Move(Lcd *x, long deltaX, long deltaY);
void LCD_doMove(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_MoveTo(Lcd *x, long deltaX, long deltaY);
void LCD_doMoveTo(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_linesegment(Lcd *x, long fromx, long fromy, long tox, long toy, long color);
void LCD_dolinesegment(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_paintOval(Lcd *x, long left, long top, long right, long bottom, long color);
void LCD_dopaintOval(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_paintRect(Lcd *x, long left, long top, long right, long bottom, long color);
void LCD_dopaintRect(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_penMode(Lcd *x, long mode);
void LCD_pos(Lcd *x, long row, long col);
void LCD_posToRC(Lcd *x, Point pt);
void LCD_resize(Lcd *x);
double sqrt(double a);
double brep(double b);
void LCD_reson(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_doreson(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_resonSampleRate(Lcd *x, double sampleRate);
void LCD_resonSpect(Lcd *x, double spectScale);
void LCD_resonTrim(Lcd *x, double trimRatio);
void LCD_save(Lcd *x, void *w);
void LCD_update(Lcd *x);
void LCD_write(Lcd *x, Symbol *s, short argc, Atom *argv);
void LCD_dowrite(Lcd *x, Symbol *s, short argc, Atom *argv);
void *rLCD(void *patcher, short x, short y, short w, short s);
void *LCD_new(Symbol *sym, short argc, Atom *argv);
void *LCD_menu(void *p, long x, long y, long font);


void LCD_doreadpict(Lcd *x);
void LCD_picthandle(Lcd *x, long ph);
void LCD_readpict (Lcd *x, Symbol *s, short ac, Atom *av);
void LCD_drawpict (Lcd *x);

Symbol *ps_picthandle,*ps_pictname;

fptr *FNS;
void *lcd_class;

short numPaletteColors;
Boolean hasColorQD;
Pattern blk,wht;
PaletteHandle myPalette;

// sde
PicHandle picthandle;
short pictgood; // flag if this is good or not

RGBColor	BLACK = {0,0,0};

#define	BUFSIZ	512

void setUpPalette(Lcd *x, RGBColor *oldColor, PaletteHandle *oldPalette)
{
	if (hasColorQD) {
		if (numPaletteColors>2){
			GetForeColor(oldColor);
			*oldPalette = GetPalette(&x->lcd_box.b_patcher->p_wind->w_gp);
			SetPalette(&x->lcd_box.b_patcher->p_wind->w_gp,x->lcd_palette,FALSE);
			PmForeColor(x->lcd_pIndex);
		}
	}
	GetPenState(&x->lcd_penState);
	PenMode(x->lcd_penMode);
	PenSize(1,1);
}

void restorePalette(Lcd *x, RGBColor *oldColor, PaletteHandle *oldPalette)
{
	if (hasColorQD) {
		if (numPaletteColors>2){
			SetPalette(&x->lcd_box.b_patcher->p_wind->w_gp,*oldPalette,FALSE);
			RGBForeColor(oldColor);
		}
	}
	SetPenState(&x->lcd_penState);
}

void main(fptr *f)
{
	CInfoRec ct;
	
	long oldA4;
	oldA4 = SetCurrentA4();
	RememberA4();
	FNS = f;

	setup(&lcd_class, (void *)LCD_new, (method) LCD_free, (short)sizeof(Lcd),
		(method)LCD_menu, A_GIMME, 0);
	
	addmess((method)LCD_ascii,"ascii",A_GIMME,0);
	addmess((method)LCD_update,"clear",0);
	addmess((method)LCD_click,"click",A_CANT,0);
	addmess((method)LCD_color,"color",A_LONG,0);
	addmess((method)LCD_debug,"debug",0);
	addmess((method)LCD_font,"font",A_DEFLONG,A_DEFLONG,0);
	addmess((method)LCD_frameOval,"frameOval",A_LONG,A_LONG,A_LONG,A_LONG,A_DEFLONG,0L,0);
	addmess((method)LCD_frameRect,"frameRect",A_LONG,A_LONG,A_LONG,A_LONG,A_DEFLONG,0L,0);
	addmess((method)LCD_Line,"Line",A_LONG,A_LONG,0L,0);
	addmess((method)LCD_LineTo,"LineTo",A_LONG,A_LONG,0L,0);
	addmess((method)LCD_Move,"Move",A_LONG,A_LONG,0L,0);
	addmess((method)LCD_MoveTo,"MoveTo",A_LONG,A_LONG,0L,0);
	addmess((method)LCD_linesegment,"LineSegment",A_LONG,A_LONG,A_LONG,A_LONG,A_DEFLONG,0);
	addmess((method)LCD_paintOval,"PaintOval",A_LONG,A_LONG,A_LONG,A_LONG,A_DEFLONG,0L,0);
	addmess((method)LCD_paintRect,"PaintRect",A_LONG,A_LONG,A_LONG,A_LONG,A_DEFLONG,0L,0);
	addmess((method)LCD_penMode,"penMode",A_LONG,0L,0);
	addmess((method)LCD_pos,"pos",A_LONG,A_LONG,0);
	addmess((method)LCD_save,"psave",A_CANT,0);
	addmess((method)LCD_reson,"reson",A_GIMME,0);
	addmess((method)LCD_resonSampleRate,"resonfreqmax",A_FLOAT,0L,0);
	addmess((method)LCD_resonSpect,"resonSpect",A_FLOAT,0L,0);
	addmess((method)LCD_resonTrim,"resonTrim",A_FLOAT,0L,0);
	addmess((method)LCD_update,"update",A_CANT, 0);
	addmess((method)LCD_write,"write",A_GIMME,0);
	addmess((method)LCD_assist,"assist",A_CANT, 0);
	
	addmess((method)LCD_local,"local",A_LONG,0);
	addmess((method)LCD_readpict,"readpict",A_GIMME,0);
	addmess((method)LCD_drawpict,"drawpict",0);
	addmess((method)LCD_picthandle,"picthandle",A_LONG,0);

	GetIndPattern(&blk,sysPatListID,1);
	GetIndPattern(&wht,sysPatListID,20);
	colorinfo(&ct);
	hasColorQD = ct.c_hasColorQD;
	numPaletteColors = ct.c_depth>=8? 256 : (ct.c_depth==4? 16 : 2);
	
	if (numPaletteColors<=16)
		hasColorQD=0; // sde 4-bit mode is screwey, ignore it.
		
#ifdef debug	
	post("hasColor %d",hasColorQD);
	post("numColors %d",numPaletteColors);
#endif	
	post("LCD Object by Michael Lee, Steve Ellison, David Zicarelli");
	post("copyright © 1991-97 Regents of the University of California. All Rights Reserved.");
	
	LCD_makepalette();		/* DDZ made palette in main(), avoids rescopy */
	rescopy('STR#',myRes);	/* DDZ added assistance */

	ps_picthandle=gensym("picthandle");
	ps_pictname=gensym("pictname");

	RestoreA4(oldA4);
}

/* ------------------------- LCD_makepalette ------------------- */

void LCD_makepalette(void)
{
	Handle myClut;
	
	if (hasColorQD) {
		myClut = Get1Resource('clut', myRes);
		
		if (!myClut) {
		    post("Color Problem");
		    myPalette = GetNewPalette(0);
		}
		/* DDZ changed palette from "courteous" to "tolerant" 12/14/92 */
		else    
			myPalette = NewPalette(numPaletteColors,(CTabHandle)myClut,pmTolerant,0x5000);
	} else
		myPalette = 0;
}

/* ------------------------- LCD_ascii ------------------------- */



void LCD_ascii(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	EnterCallback();
	defer(x,(method)LCD_doascii,s,argc,argv);
	ExitCallback();
}

void LCD_doascii(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	short i;
	char	stng[256];
	
	EnterCallback();
	
	stng[1] = '\0';
	for (i=0;i<argc;i++) {

		switch(argv[i].a_type) {
			case A_LONG:
				switch (stng[0] = (char)argv[i].a_w.w_long) {
					case BACKSPACE:
						stng[0] = 32;
						x->lcd_col--;
						x->lcd_col = MINMAX(x->lcd_col,0,x->cols-1);
						LCD_drawTxt(x,stng);
						x->lcd_col--;
						x->lcd_col = MINMAX(x->lcd_col,0,x->cols-1);
						break;
					case CR:
						x->lcd_row++;
						x->lcd_row = MINMAX(x->lcd_row,0,x->rows-1);
						x->lcd_col = 0;
						break;
					case LEFT_ARROW:
						x->lcd_col--;
						x->lcd_col = MINMAX(x->lcd_col,0,x->cols-1);
						break;
					case RIGHT_ARROW:
						x->lcd_col++;
						x->lcd_col = MINMAX(x->lcd_col,0,x->cols-1);
						break;
					case UP_ARROW:
						x->lcd_row--;
						x->lcd_row = MINMAX(x->lcd_row,0,x->rows-1);
						break;
					case DOWN_ARROW:
						x->lcd_row++;
						x->lcd_row = MINMAX(x->lcd_row,0,x->rows-1);
						break;
					default:
						LCD_drawTxt(x,stng);
						break;
				}						
				break;
			case A_SYM:
			case A_FLOAT:
				break;
		}
	}
	ExitCallback();
}

/* ------------------------- LCD_assist ----------------------------- */

void LCD_assist(Lcd *x, void *b, long m, long a, char *s)
{
	EnterCallback();
	assist_string(myRes,m,a,1,2,s);
	ExitCallback();
}

/* ------------------------- LCD_clear_area ------------------------- */

void LCD_clear_area(Lcd *x)      /* erase the area occupied by the LCD */
{	
	Rect	r;
	
	r = x->lcd_oldRect;
	InsetRect(&r,1,1);
	EraseRect(&r);
	x->lcd_row = 0;
	x->lcd_col = 0;
}

/* ------------------------- LCD_local ------------------------- */

void LCD_local (Lcd *x, long loco)
{
	EnterCallback();
	x->local=(short)loco;
	ExitCallback();
}

/* ------------------------- LCD_click ------------------------- */

void LCD_click(Lcd *x, Point where, short modifiers)
{
	short	i;
	Rect	r;
	Atom	aList[2];
	short		way;
	
	EnterCallback();
	
#ifdef debug
	post("¥LCD_click - start");
#endif
	r = x->lcd_box.b_rect;
	x->lcd_shiftClick = modifiers&shiftKey;

	if (x->local)
		LCD_MoveTo(x,(long)(where.h-r.left),(long)(where.v-r.top));

	LCD_posToRC(x,where);
	wind_drag((void *)LCD_drag,x,where);

	SETLONG(&aList[0],(long)(where.h-r.left));
	SETLONG(&aList[1],(long)(where.v-r.top));
	/* lockouts added by DDZ 12/14/92 */
	way = lockout_set(1);
	outlet_list(x->lcd_outlet,0L,2,aList);
	lockout_set(way);
	ExitCallback();
}	/* ms_click */

/* ------------------------- LCD_color ------------------------- */

void LCD_color(Lcd *x, long color)
{
	EnterCallback();
	x->lcd_pIndex = (short)color & (numPaletteColors-1);
	ExitCallback();
}

/* ------------------------- LCD_debug ------------------------- */

void LCD_debug(Lcd *x)
{
	EnterCallback();
	x->lcd_debug = !x->lcd_debug;
	if (x->lcd_debug)
		post("LCD debug on");
	else
		post("LCD debug off");
	
	ExitCallback();
}

/* ------------------------- LCD_drag ------------------------- */

void LCD_drag(Lcd *x, Point pt, short button_down)
{
	Rect	r;
	short		slider;
	Atom	aList[2];
	short 	way;
	
	EnterCallback();
	
#ifdef debug
	post("¥LCD_drag - start");
#endif
	r = x->lcd_box.b_rect;
	
	if (x->local)
	{
		if (button_down) {
			LCD_MoveTo(x,(long)(x->lcd_where.h),(long)(x->lcd_where.v));
			LCD_LineTo(x,(long)(pt.h-r.left),(long)(pt.v-r.top));
		}
		else
			x->lcd_shiftClick = false;
	}
	
	LCD_posToRC(x,pt);

	SETLONG(&aList[0],(long)(pt.h-r.left));
	SETLONG(&aList[1],(long)(pt.v-r.top));
	/* lockouts added by DDZ 12/14/92 */
	way = lockout_set(1);
	outlet_list(x->lcd_outlet,0L,2,aList);
	lockout_set(way);
	ExitCallback();
}

/* ------------------------- LCD_drawTxt ------------------------ */

void LCD_drawTxt(Lcd *x, char *stng)     /* drawn in response to update event */
{
	GrafPort *gp;
	short		chars;
	Rect	r,area;
	PaletteHandle pH;
	RGBColor fColor;
	RgnHandle cur;
	short		font,size;
	
	gp = patcher_setport(x->lcd_box.b_patcher);
	if (gp) {
		font = gp->txFont;
		size = gp->txSize;
		r = x->lcd_box.b_rect;
		if (x->f_width == -1) { /* window not visible before */
			if (!x->font && !x->f_size) {
				x->font = font;
				x->f_size = size;
			}
			LCD_calcfont(x);
			LCD_resize(x);
		}
		TextFont(x->font);
		TextSize(x->f_size);

		if ((x->lcd_row+1)<=x->rows) {
			if (!box_nodraw((void *)x)) {
				/* chars = x->cols-x->lcd_col;  DDZ removed this stuff */
				if (/* chars > 0 */ 1) {
#ifdef never
					MoveTo(	(x->lcd_col*x->f_width)+r.left+BORDER_WIDTH,
							(x->lcd_row+1)*x->f_height+(x->lcd_row)*x->f_lineSpace+r.top-x->f_descent);

					/* erase area where we will write */
					area.top = (x->lcd_row*x->f_height)+r.top+BORDER_HEIGHT;
					area.bottom = ((x->lcd_row+1)*x->f_height)+r.top+BORDER_HEIGHT;
					area.left = (x->lcd_col*x->f_width)+r.left+BORDER_WIDTH;
					area.right = ((x->lcd_col+chars)*x->f_width)+r.left+BORDER_WIDTH;
#else
					MoveTo(x->lcd_where.h+r.left+BORDER_WIDTH,x->lcd_where.v+r.top);
#endif
					/* chars = MIN(chars,(short)strlen(stng)); */
					chars = strlen(stng);  /* DDZ just made it draw any string */
					
					cur = NewRgn();
					GetClip(cur);
					SetClip(x->lcd_region);
#ifdef never
					EraseRect(&area);
#endif
					setUpPalette(x,&fColor,&pH);

					DrawText(stng,0,chars);
					x->lcd_where.h += TextWidth(stng,0,chars);
					restorePalette(x,&fColor,&pH);
					SetClip(cur);
					DisposeRgn(cur);
					x->lcd_col += chars;		/* advance cursor */
					if (x->lcd_col > x->cols-1 )
						x->lcd_col--;			/* make it stick to last col */
				}
			}
		}
		TextFont(font);
		TextSize(size);

		SetPort(gp);
	}
}

/* ------------------------- LCD_font ------------------------ */

void LCD_calcfont(Lcd *x)
{
	FontInfo	info;
	
	GetFontInfo(&info);

	x->f_width = info.widMax;
	x->f_ascent = info.ascent;
	x->f_descent = info.descent;
	x->f_height = info.ascent+info.descent;
	x->f_lineSpace = info.leading;
}

void LCD_font(Lcd *x, long font, long size)
{
	Atom a[2];
	
	EnterCallback();
	SETLONG(a,font);
	SETLONG(a+1,size);
	defer(x,(method)LCD_dofont,0L,2,a);
	ExitCallback();
}
	
void LCD_dofont(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	long font, size;			/* check patcher font settings */
	GrafPtr		sp,pp;
	short			sFont,sSize;
	
	EnterCallback();
	
	font = argv->a_w.w_long;
	size = (argv+1)->a_w.w_long;
	x->font = (short)font;
	x->f_size = (short)size;
	if (sp=patcher_setport(x->lcd_box.b_patcher)){
		GetPort(&pp);
		if (!font&&!size){
			x->font = pp->txFont;
			x->f_size = pp->txSize;			
		}
		else{
			sFont = pp->txFont;
			sSize = pp->txSize;
			TextFont(font);
			TextSize(size);
		}
		
#ifdef debug
post("font:%d size:%d",x->font,x->f_size);
#endif
		LCD_calcfont(x);
		
		LCD_resize(x);
		
		if (font||size){
			TextFont(sFont);
			TextSize(sSize);
		}
		SetPort(sp);
	} else { /* note that this needs to be calced later */
		x->f_width = -1;
		x->f_ascent = -1;
		x->f_descent = -1;
		x->f_height = -1;
		x->f_lineSpace = -1;
	}
	ExitCallback();
}

/* ------------------------- LCD_frameOval ------------------------ */

void LCD_frameOval(Lcd *x, long left, long top, long right, long bottom, long color)
{	
	Atom a[5];
	
	EnterCallback();
	SETLONG(a,left);
	SETLONG(a+1,top);
	SETLONG(a+2,right);
	SETLONG(a+3,bottom);
	SETLONG(a+4,color);
	defer(x,(method)LCD_doframeOval,0L,5,a);
	ExitCallback();
}

void LCD_doframeOval(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	long left, top, right, bottom, color;
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	Rect	 b,r;
	
	EnterCallback();
	
	// I'm suspicious of (argv++)->thing in Code Warrior
	left = argv->a_w.w_long;
	top = (argv+1)->a_w.w_long;
	right = (argv+2)->a_w.w_long;
	bottom = (argv+3)->a_w.w_long;
	color = (argv+4)->a_w.w_long;
#ifdef debug
	post("frameOval");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {

			if (color)  // sde 11/1
				x->lcd_pIndex = (short)color & (numPaletteColors-1);

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);
			
			r = x->lcd_box.b_rect;
			b.left = r.left + (short)left;
			b.top  = r.top + (short)top;
			b.right = r.left + (short)right;
			b.bottom = r.top + (short)bottom;
			FrameOval(&b);

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_frameRect ------------------------ */

void LCD_frameRect(Lcd *x, long left, long top, long right, long bottom, long color)
{	
	Atom a[5];
	
	EnterCallback();
	SETLONG(a,left);
	SETLONG(a+1,top);
	SETLONG(a+2,right);
	SETLONG(a+3,bottom);
	SETLONG(a+4,color);
	defer(x,(method)LCD_doframeRect,0L,5,a);
	ExitCallback();
}

void LCD_doframeRect(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	Rect	 b,r;
	long left, top, right, bottom, color;
	
	EnterCallback();
	left = argv->a_w.w_long;
	top = (argv+1)->a_w.w_long;
	right = (argv+2)->a_w.w_long;
	bottom = (argv+3)->a_w.w_long;
	color = (argv+4)->a_w.w_long;
	
#ifdef debug
	post("frameRect");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {

			if (color)  // sde 11/1
				x->lcd_pIndex = (short)color & (numPaletteColors-1);

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);
			
			r = x->lcd_box.b_rect;
			b.left = r.left + (short)left;
			b.top  = r.top + (short)top;
			b.right = r.left + (short)right;
			b.bottom = r.top + (short)bottom;
			FrameRect(&b);

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_free ------------------------ */

void LCD_free(Lcd *x)
{
	EnterCallback();
	
	if (x->screen) freebytes(x->screen,(short)sizeof(char)*(x->cols*x->rows));	
	if (x->lcd_region)
		DisposeRgn(x->lcd_region);
	box_free((void *)x);

	ExitCallback();
}

/* ------------------------- LCD_Line ------------------------ */

void LCD_Line(Lcd *x, long deltaX, long deltaY)
{	
	Atom a[2];
	
	EnterCallback();
	SETLONG(a,deltaX);
	SETLONG(a+1,deltaY);
	defer(x,(method)LCD_doLine,0L,2,a);
	ExitCallback();
}

void LCD_doLine(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	long deltaX, deltaY;
	
	EnterCallback();
	deltaX = argv->a_w.w_long;
	deltaY = (argv+1)->a_w.w_long;
#ifdef debug
	post("Line");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);

			LCD_MoveTo(x,(long)(x->lcd_where.h),(long)(x->lcd_where.v));
			Line((short)deltaX,(short)deltaY);
			x->lcd_where.h += (short)deltaX;
			x->lcd_where.v += (short)deltaY;

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_LineTo ------------------------ */

void LCD_LineTo(Lcd *x, long deltaX, long deltaY)
{	
	Atom a[2];
	
	EnterCallback();
	SETLONG(a,deltaX);
	SETLONG(a+1,deltaY);
	defer(x,(method)LCD_doLineTo,0L,2,a);
	ExitCallback();
}

void LCD_doLineTo(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	long deltaX, deltaY;
	
	EnterCallback();
	deltaX = argv->a_w.w_long;
	deltaY = (argv+1)->a_w.w_long;
#ifdef debug
	post("LineTo");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);

			LCD_MoveTo(x,(long)(x->lcd_where.h),(long)(x->lcd_where.v));
			LineTo(x->lcd_box.b_rect.left+1+(short)deltaX,x->lcd_box.b_rect.top+1+(short)deltaY);
			x->lcd_where.h = (short)deltaX;
			x->lcd_where.v = (short)deltaY;

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_Move ------------------------ */

void LCD_Move(Lcd *x, long deltaX, long deltaY)
{	
	Atom a[2];
	
	EnterCallback();
	SETLONG(a,deltaX);
	SETLONG(a+1,deltaY);
	defer(x,(method)LCD_doMove,0L,2,a);
	ExitCallback();
}

void LCD_doMove(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	GrafPort *gp;
	long deltaX, deltaY;

	EnterCallback();
	deltaX = argv->a_w.w_long;
	deltaY = (argv+1)->a_w.w_long;
	
#ifdef debug
	post("Move");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			Move((short)deltaX,(short)deltaY);
			x->lcd_where.h += (short)deltaX;
			x->lcd_where.v += (short)deltaY;
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_MoveTo ------------------------ */

void LCD_MoveTo(Lcd *x, long deltaX, long deltaY)
{	
	Atom a[2];
	
	EnterCallback();
	SETLONG(a,deltaX);
	SETLONG(a+1,deltaY);
	defer(x,(method)LCD_doMoveTo,0L,2,a);
	ExitCallback();
}

void LCD_doMoveTo(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	GrafPort *gp;
	long deltaX, deltaY;
	
	EnterCallback();
	deltaX = argv->a_w.w_long;
	deltaY = (argv+1)->a_w.w_long;
	
#ifdef debug
	post("MoveTo");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			MoveTo(x->lcd_box.b_rect.left+1+(short)deltaX,x->lcd_box.b_rect.top+1+(short)deltaY);
			x->lcd_where.h = (short)deltaX;
			x->lcd_where.v = (short)deltaY;
		}
		SetPort(gp);
	}
	ExitCallback();
}

void LCD_linesegment(Lcd *x, long fromx, long fromy, long tox, long toy, long color)
{	
	Atom a[5];
	
	EnterCallback();
	SETLONG(a,fromx);
	SETLONG(a+1,fromy);
	SETLONG(a+2,tox);
	SETLONG(a+3,toy);
	SETLONG(a+4,color);
	defer(x,(method)LCD_dolinesegment,0L,5,a);
	ExitCallback();
}

void LCD_dolinesegment(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	long fromx,fromy,tox,toy,color;
	short save;
	
	EnterCallback();
	
	fromx = argv->a_w.w_long;
	fromy = (argv+1)->a_w.w_long;
	tox = (argv+2)->a_w.w_long;
	toy = (argv+3)->a_w.w_long;
	color = (argv+4)->a_w.w_long;
	
#ifdef debug
	post("Segment");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			// save=lockout_set(1);
			MoveTo(x->lcd_box.b_rect.left+1+(short)fromx,x->lcd_box.b_rect.top+1+(short)fromy);
			x->lcd_where.h = (short)fromx;
			x->lcd_where.v = (short)fromy;
			
			if (color)
				x->lcd_pIndex = (short)color & (numPaletteColors-1);

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);

			setUpPalette(x,&fColor,&pH);

		//	LCD_MoveTo(x,(long)(x->lcd_where.h),(long)(x->lcd_where.v));
			
			LineTo(x->lcd_box.b_rect.left+1+(short)tox,x->lcd_box.b_rect.top+1+(short)toy);
			x->lcd_where.h = (short)tox;
			x->lcd_where.v = (short)toy;

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
			// lockout_set(save);

		}
		SetPort(gp);
	}
	ExitCallback();
}


/* ------------------------- LCD_paintOval ------------------------ */

void LCD_paintOval(Lcd *x, long left, long top, long right, long bottom, long color)
{	
	Atom a[5];
	
	EnterCallback();
	SETLONG(a,left);
	SETLONG(a+1,top);
	SETLONG(a+2,right);
	SETLONG(a+3,bottom);
	SETLONG(a+4,color);
	defer(x,(method)LCD_dopaintOval,0L,5,a);
	ExitCallback();
}

void LCD_dopaintOval(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	Rect	 b,r;
	long left, top, right, bottom, color;
	
	EnterCallback();
	left = argv->a_w.w_long;
	top = (argv+1)->a_w.w_long;
	right = (argv+2)->a_w.w_long;
	bottom = (argv+3)->a_w.w_long;
	color = (argv+4)->a_w.w_long;
	
#ifdef debug
	post("paintOval");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			
			if (color)  // sde 11/1
				x->lcd_pIndex = (short)color & (numPaletteColors-1);

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);
			r = x->lcd_box.b_rect;
			b.left = r.left + (short)left;
			b.top  = r.top + (short)top;
			b.right = r.left + (short)right;
			b.bottom = r.top + (short)bottom;
			PaintOval(&b);

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_paintRect ------------------------ */

void LCD_paintRect(Lcd *x, long left, long top, long right, long bottom, long color)
{	
	Atom a[5];
	
	EnterCallback();
	SETLONG(a,left);
	SETLONG(a+1,top);
	SETLONG(a+2,right);
	SETLONG(a+3,bottom);
	SETLONG(a+4,color);
	defer(x,(method)LCD_dopaintRect,0L,5,a);
	ExitCallback();
}

void LCD_dopaintRect(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	Rect	 b,r;
	long left, top, right, bottom, color;
	
	EnterCallback();
	left = argv->a_w.w_long;
	top = (argv+1)->a_w.w_long;
	right = (argv+2)->a_w.w_long;
	bottom = (argv+3)->a_w.w_long;
	color = (argv+4)->a_w.w_long;
	
#ifdef debug
	post("paintRect");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {

			if (color)  // sde 11/1
				x->lcd_pIndex = (short)color & (numPaletteColors-1);

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);
			r = x->lcd_box.b_rect;
			b.left = r.left + (short)left;
			b.top  = r.top + (short)top;
			b.right = r.left + (short)right;
			b.bottom = r.top + (short)bottom;
			PaintRect(&b);

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_penMode ------------------------- */

void LCD_penMode(Lcd *x, long mode)
{
	EnterCallback();
	if (mode < patCopy)
		mode += patCopy;
	x->lcd_penMode = (short)mode;
	ExitCallback();
}

/* ------------------------- LCD_pos ------------------------- */

void LCD_pos(Lcd *x, long row, long col)
{
	EnterCallback();
	x->lcd_row = MINMAX((short)row,0,x->rows-1);
	x->lcd_col = MINMAX((short)col,0,x->cols-1);
	ExitCallback();
}

/* ------------------------- LCD_posToRC ------------------------- */

void LCD_posToRC(Lcd *x, Point pt)
{
	Rect	r;
	short		hor,ver;

	r = x->lcd_box.b_rect;
	
	x->lcd_row = (short)(ceil(1.0*(pt.v-r.top-(2*BORDER_HEIGHT))/x->f_height))-1;
	x->lcd_col = (short)(ceil(1.0*(pt.h-r.left-(2*BORDER_WIDTH))/x->f_width));

	x->lcd_row = MINMAX(x->lcd_row,0,x->rows-1);
	x->lcd_col = MINMAX(x->lcd_col,0,x->cols-1);
	
	x->lcd_where.h = pt.h-r.left;
	x->lcd_where.v = pt.v-r.top;
}

/* ------------------------- LCD_resize ------------------------- */

void LCD_resize(Lcd *x)
{
	Rect	r;
	short		hor,ver;

#ifdef debug
	post("LCD_Resize");
#endif

	r = x->lcd_box.b_rect;
	
	hor = x->cols = FixRound(FixRatio(((r.right-r.left-(2*BORDER_WIDTH))),x->f_width));
	ver = x->rows = FixRound(FixRatio(((r.bottom-r.top-(2*BORDER_HEIGHT))),x->f_height));

#ifdef FORCEDResize
	x->lcd_box.b_rect.right = r.left + (2*BORDER_WIDTH) + hor*x->f_width;
	x->lcd_box.b_rect.bottom = r.top + (2*BORDER_HEIGHT) + ver*x->f_height;
#endif

	if (x->lcd_region)
		DisposeRgn(x->lcd_region);
	x->lcd_oldRect = r = x->lcd_box.b_rect;
	InsetRect(&r,1,1);
	x->lcd_region = NewRgn();
	RectRgn(x->lcd_region,&r);  /* DDZ simpler way to make a rectangular region */
}

/* ------------------------- LCD_reson ------------------------- */

double sqrt(double a);

double brep(double b)
{
	return (0.1)/sqrt(MINMAX(b,0.1,40.0));
}

void LCD_reson(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	EnterCallback();
	defer(x,(method)LCD_doreson,s,argc,argv);
	ExitCallback();
}

void LCD_doreson(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	short i,x1,y1,y2,height,width;
	double	freq,ampl,band;
	Rect	r;
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	Rect	 b;
	double xscale;
	double bscale;
	double yscale;				
	
	EnterCallback();

	if((argc%3)!=0)
	{
		post("bad number of resonance parameters %d", argc);
	} else {
		gp = patcher_setport(x->lcd_box.b_patcher);

		if (gp) {
			if (!box_nodraw((void *)x)) {
				cur = NewRgn();
				GetClip(cur);
				SetClip(x->lcd_region);
				setUpPalette(x,&fColor,&pH);
				
				r = x->lcd_box.b_rect;
				width	= r.right-r.left;
				height	= r.bottom-r.top;
				xscale=width/(x->reson_sampleRate);
				bscale = (0.1*height);
				yscale = height/x->reson_spectScale;
				for (i=0;i<argc;i++)
				{
					freq = (double)argv[i].a_w.w_float;
					ampl =argv[++i].a_w.w_float;
					if(ampl<0.0f)
						ampl = -ampl;
					ampl = 20.0*log10(ampl);
					band = (double)argv[++i].a_w.w_float;
					if(freq>0.0 && freq<(x->reson_sampleRate)	&& band>0.0)
					{
						x1 = (freq*xscale);
						{
							float ydisp = ampl*yscale;
							if(ydisp>(r.bottom-r.top))
								y1 = r.bottom -r. top;
							else
								y1= ydisp;
						}
						y2 = bscale*3.1415926/sqrt(MINMAX(band,0.1,40.0));
				
						if (x->lcd_debug)
							post("x1: %d y1: %d y2: %d",x1,y1,y2);
								
						MoveTo(r.left+x1,r.top+y1);
						Line(0,y2);
					}
				}
		
				restorePalette(x,&fColor,&pH);
				SetClip(cur);
				DisposeRgn(cur);
			}
			SetPort(gp);
		}
	
	}
	ExitCallback();
}

/* ------------------------- LCD_resonSampleRate ------------------------- */

void LCD_resonSampleRate(Lcd *x, double sampleRate)
{
	EnterCallback();
	x->reson_sampleRate = sampleRate;
	ExitCallback();
}

/* ------------------------- LCD_resonSpect ------------------------- */

void LCD_resonSpect(Lcd *x, double spectScale)
{
	EnterCallback();
	x->reson_spectScale = spectScale;
	ExitCallback();
}

/* ------------------------- LCD_resonTrim ------------------------- */

void LCD_resonTrim(Lcd *x, double trimRatio)
{
	EnterCallback();
	x->reson_trimRatio = trimRatio;
	ExitCallback();
}

/* ------------------------- LCD_save ------------------------- */

void LCD_save(Lcd *x, void *w)
{
	Rect *r;
	
	EnterCallback();
	r = &x->lcd_box.b_rect;
	
	if (x->lcd_box.b_hidden) {
		binbuf_vinsert(w,"ssssllll", gensym("#P"),gensym("hidden"),gensym("user"),
			gensym("LCD"), (long)(r->left), (long)(r->top),
				(long)(r->right - r->left), (long)(r->bottom - r->top));
	} else {
		binbuf_vinsert(w,"sssllll", gensym("#P"),gensym("user"), 
			gensym("LCD"), (long)(r->left), (long)(r->top),
				(long)(r->right - r->left), (long)(r->bottom - r->top));
	}
	ExitCallback();		
}

/* ------------------------- LCD_update ------------------------- */

void LCD_update(Lcd *x)     /* drawn in response to clear or update event */
{
	GrafPort *gp;
	
	EnterCallback();
	
#ifdef debug
	post("LCD_update");
#endif
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp) {
		if (!box_nodraw((void *)x)) {
			LCD_clear_area(x);
			LCD_resize(x);
		}
		SetPort(gp);
	}
	ExitCallback();
}

/* ------------------------- LCD_write ------------------------- */

void LCD_write(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	EnterCallback();
	defer(x,(method)LCD_dowrite,s,argc,argv);
	ExitCallback();
}

void LCD_dowrite(Lcd *x, Symbol *s, short argc, Atom *argv)
{
	short i;
	char	stng[256];
	
	EnterCallback();
	
	for (i=0;i<argc;i++) {
		if (i)
			LCD_drawTxt(x," ");	/* advance the cursor */
			
		switch(argv[i].a_type) {
			case A_LONG:
				sprintf(stng,"%ld",argv[i].a_w.w_long);
				break;
			case A_SYM:
				sprintf(stng,"%s",argv[i].a_w.w_sym->s_name);
				break;
			case A_FLOAT:
				sprintf(stng,"%lf",argv[i].a_w.w_float);
				break;
			default:
				stng[0] = 0;
				break;
		}
		LCD_drawTxt(x,stng);
	}
	ExitCallback();
}

/* ------------------------- rLCD ------------------------- */

void *rLCD(void *patcher, short x, short y, short w, short s)
{
	ResType		theType;
	Lcd			*p;
	FontInfo	info;

	p = (Lcd *)newobject(lcd_class);

	if (s < 1) s = 1;
	box_new((Box *)p, patcher, F_DRAWFIRSTIN | F_GROWBOTH, x, y, x+w,y+s);

	p->lcd_box.b_firstin = (void *)p;  /* it's not really an inlet */
	p->lcd_row = 0;
	p->lcd_col = 0;
	p->lcd_region = 0L;  /* DDZ moved to prevent dispose of bad region handle
							(LCD_font calls LCD_resize) */
	LCD_font(p,0L,0L);	/* DDZ changed to add 0,0 as arguments (needed) 12/14/92 */
	
	p->lcd_shiftClick = false;
	p->file_outlet=outlet_new(p,0L);
	p->lcd_outlet = outlet_new(p,0L);

	p->lcd_palette = myPalette;
	p->lcd_pIndex = numPaletteColors-1;
	p->lcd_penMode = patCopy;
	
	p->screen = 0L;
	
	p->reson_sampleRate = 20000.0;
	p->reson_spectScale = -100.0;
	p->reson_trimRatio = 1.0;
	
	p->local=1; // sde want to draw mouseclicks by default

	p->lcd_debug = false;
	p->lcd_oldRect = p->lcd_box.b_rect;
	/* DDZ, the statement above needs to be done in case the 
		window is not visible, because otherwise, oldRect
		can be set to some huge size and erase the entire
		patcher window
	*/
	// sde
	picthandle = (PicHandle)newhandle(4L);// create a dummy so we have something to dispose
	pictgood=0;
	
	{
		GrafPort *gp;
		
		gp = patcher_setport(p->lcd_box.b_patcher);	
		if (gp) {
			LCD_resize(p);
			SetPort(gp);
		}
	}
	
	box_ready((Box *)p);

	return (p);
}

/* ------------------------- LCD_new ------------------------- */

/* creating LCD with values from binbuf */

void *LCD_new(Symbol *sym, short argc, Atom *argv)
{
	void *patcher;
	short	x,y,w,s;
	void *res;
	
	EnterCallback();
	patcher = argv[0].a_w.w_obj;
 	x = (short)argv[1].a_w.w_long;
	y = (short)argv[2].a_w.w_long;
	w = (short)argv[3].a_w.w_long;
	s = (short)argv[4].a_w.w_long;
	
	res = rLCD(patcher,x,y,w,s);
	
	ExitCallback();
	return (res);
}

/* ------------------------- LCD_menu ------------------------- */

/* creating LCD with default values */

void *LCD_menu(void *p, long x, long y, long font)
{
	void *res;
	
	EnterCallback();
	res = rLCD(p,(short)x,(short)y, 16*CHAR_WIDTH+2*BORDER_WIDTH, 2*CHAR_HEIGHT+2*BORDER_HEIGHT);
	ExitCallback();
	return (res);
}

// picture stuff added by Steve Ellison

void LCD_doreadpict(Lcd *x)      /*read in a 'PICT' selected by the user*/
{ 

	Point	wher;		
	SFReply	reply;	
	SFTypeList	myFileTypes;	
	short	numFileTypes;
	OSErr	err;
	QDProcsPtr	savedProcs;
	long	longCount,myEOF,filePos;
	short vol,refNum;
	long length;
	Atom a;
	short i;
	
	GrafPort *thePort;
	Symbol *file;
	
	EnterCallback();
	
	wher.h = 20;
	wher.v = 20;
	numFileTypes = 1;		/*display 'PICT's*/
	myFileTypes[0] = 'PICT';

	if (!x->fn[0])
	{
		SFGetFile(wher,"\p",0L,numFileTypes,myFileTypes,0L,&reply);
		vol=reply.vRefNum;
		if (reply.good)
		{
			for (i=0;i<=reply.fName[0];i++)
				x->fn[i]=reply.fName[i];
			PtoCstr((Byte *)x->fn);
			file=gensym(x->fn);
			SETSYM(&a,file);
			outlet_anything (x->file_outlet,ps_pictname,1,&a);
			CtoPstr(x->fn);
		}
	}
	else
	{
		reply.good=!locatefiletype(x->fn,&vol,'PICT',0L);
		if (reply.good)
		{
			CtoPstr (x->fn);
		}
	}
	
	if (reply.good)
	{

		err = FSOpen((Byte *)x->fn,vol,&refNum);
			
		if (!err) {
			SetFPos(refNum,fsFromStart,512L);
			GetEOF(refNum,&length);
			length -= 512;
			disposhandle ((char **)picthandle); // free the memory
			picthandle = (PicHandle)newhandle(length);
			FSRead(refNum,&length,*picthandle);
			FSClose(refNum);
			post("pict: reading pict size %ld",length);
			SETLONG (&a,(long)picthandle);
			pictgood=1;
			outlet_anything(x->file_outlet,ps_picthandle,1,&a);
		}
	}
		
	ExitCallback();
} 

void LCD_picthandle(Lcd *x, long ph)      /*read in a 'PICT' specified by a handle*/
{ 

	Atom a;
	EnterCallback();
	
	picthandle = (PicHandle)ph;

	SETLONG (&a,ph);
	outlet_anything(x->file_outlet,ps_picthandle,1,&a);
		
	ExitCallback();
} 

void LCD_readpict (Lcd *x, Symbol *s, short ac, Atom *av)
{
	short i;
	
	EnterCallback();
	if (ac==1 && av->a_type==A_SYM)
		strcpy(x->fn,av->a_w.w_sym->s_name);
	else
		x->fn[0] = 0;
	defer(x,(method)LCD_doreadpict,0L,0,0L);
	ExitCallback();
}

void LCD_drawpict (Lcd *x)
{
	PaletteHandle pH;
	RGBColor fColor;
	GrafPort *gp;
	RgnHandle cur;
	PicHandle thePicture;
	
	EnterCallback();
	
	gp = patcher_setport(x->lcd_box.b_patcher);

	if (gp &&picthandle&&pictgood) {
		if (!box_nodraw((void *)x)) {

			cur = NewRgn();
			GetClip(cur);
			SetClip(x->lcd_region);
			setUpPalette(x,&fColor,&pH);
  
  	//		thePicture = GetPicture(1111);

			DrawPicture(picthandle,&x->lcd_box.b_rect);

		//	DrawPicture(thePicture,&x->lcd_box.b_rect);

			restorePalette(x,&fColor,&pH);
			SetClip(cur);
			DisposeRgn(cur);
		}
		SetPort(gp);
	}
	ExitCallback();
}
