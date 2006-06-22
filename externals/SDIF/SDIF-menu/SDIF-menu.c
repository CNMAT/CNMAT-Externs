/*
Copyright (c) 1999,2000,2001.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Richard Dudas and Matt Wright, The Center for New
Music and Audio Technologies, University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
*/

/* 
DESCRIPTION: Richard Dudas' obsolete OS9-only PPC-only GUI for SDIF stream selection.  Use sdif-menu2.js instead.
*/

/* SDIF-menu -  a gui object for SDIF thread selection
	
	(c) CNMAT 1999
	ppc only Dudas hack - gzdip!
	using some of Matt's SDIF code (modified)
	Version 0.4 by Matt: solves fopen binary bug, less verbose in Max window
	Version 0.5 by Matt: Uses %g to print time tags so hugely negative ones don't crash Max
	Version 0.6 by Matt: Removed "navcrap" printing to Max window
	
	
	To Do:
		- Correctly handle "cancel" without losing memory
		- Indicate when there are more streams not shown

*/

#define VERSION "SDIF-menu 0.6" 

#include "ext.h"
#include "ext_common.h"
#include "ext_strings.h"
#include "ext_user.h"
#include "ext_wind.h"

#include <stdio.h>
#include "sdif.h"

#include <Quickdraw.h>
#include <QDOffscreen.h>
#include <math.h>
#include <Navigation.h>
#include <FSp_fopen.h>


#define RES_ID		19999


#define MAXWIDTH 	384			// maximum width and height of user object in pixels
#define MAXHEIGHT	512			//    (defines maximum offscreen canvas allocation)
#define MINWIDTH 	256			// minimum width and height
#define MINHEIGHT	128			
#define DEFWIDTH 	256			// default width and height
#define DEFHEIGHT	128	

#define TITLEBAR	20
#define READBWIDTH	60
#define TYPE_COLUMN		40
#define TIME_COLUMN		60
#define FRAMES_COLUMN	50

#define MAX_STREAMS 100  // Most SDIF streams any file should have


#undef CLIP
#define CLIP(a, b, c) MAX(MIN(a, c), b)


typedef struct sdifmenu
{
	Box			x_box;
	Patcher		*x_patcher;
	Rect 		x_rect;
	short		x_boxwidth;		// these are currently fixed at BOXWIDTH, BOXHEIGHT
	short		x_boxheight;
	
	GWorldPtr	x_offscreen; 	// offscreen for this insstance
	Rect		x_offrect;		// rect for defining offscreen space

	void		*x_qelem;
	
	int			x_color; 		// color of object
	//RGBColor	x_rgbcolor;
	RGBColor	x_rgbcolor1;	// ultralight
	RGBColor	x_rgbcolor2;	// light
	RGBColor	x_rgbcolor3;	// regular
	RGBColor	x_rgbcolor4;	// dark
	RGBColor	x_rgbcolor5;	// ultradark
	
	
	Rect 	x_readbutton;	// read file button at top of title bar
	int		x_hilitebutton; // highlight the button?
	void	*x_klok; 	// clock for button hilite
	
	int		x_selection; //which stream is selected for output
	
	int		x_column1;
	int		x_column2;
	int		x_column3;
	int		x_column4;
	int		x_column5;
	
	Str255			x_filename;
	int				x_ns; // number of streams seen
	sdif_int32 		x_streamID[MAX_STREAMS];
	char 			x_frameType[MAX_STREAMS][4];
	sdif_float64 	x_starttime[MAX_STREAMS];
	sdif_float64 	x_endtime[MAX_STREAMS];
	sdif_int32		x_numframes[MAX_STREAMS];
	
	void		*x_out0;	// outlet.
} t_sdifmenu;
	 			



void sdifmenu_qfn(t_sdifmenu *x);
void sdifmenu_redraw(t_sdifmenu *x);
void sdifmenu_update(t_sdifmenu *x);
void sdifmenu_color(t_sdifmenu *x, long color);
void sdifmenu_setcolor(t_sdifmenu *x, long r, long g, long b);
void sdifmenu_click(t_sdifmenu *x, Point pt);
void sdifmenu_drag(t_sdifmenu *x, Point pt, short but);


void sdifmenu_tick(t_sdifmenu *x);
void sdifmenu_bang(t_sdifmenu *x);
void sdifmenu_clear(t_sdifmenu *x);
void sdifmenu_int(t_sdifmenu *x, long n);
void sdifmenu_calccolumns(t_sdifmenu *x);
void sdifmenu_output(t_sdifmenu *x);

void sdifmenu_psave(t_sdifmenu *x, void *w);
void sdifmenu_restore(t_sdifmenu *x);
void sdifmenu_assist(t_sdifmenu *x, void *b, long m, long a, char *s);
void sdifmenu_free(t_sdifmenu *x);
void *sdifmenu_real(Patcher *p, short bx, short by, short w, short h);
void *sdifmenu_new(Symbol *s, int ac, Atom *av);
void *sdifmenu_menufun(void *p, long x, long y, long font);

void AllocateOffscreen(t_sdifmenu *x);
void LineSegment(short a, short b, short c, short d);
void SetRGBColor(RGBColor *rgb, long r, long g, long b);
void ShadedRect(Rect *r, RGBColor *rgb1, RGBColor *rgb2, RGBColor *rgb3, int th);


void sdifmenu_read(t_sdifmenu *x, Symbol *s, int ac, Atom *av);
void do_streamlist(t_sdifmenu *x, FILE *f, char *name);
static FILE *sdifmenu_opensdif(t_sdifmenu *x, char *filename);
void sdifmenu_navcrap(t_sdifmenu *x);
pascal void worldsLamestEventProc(NavEventCallbackMessage callBackSelector,
						NavCBRecPtr callBackParms,
						NavCallBackUserData callBackUD);

void *sdifmenu_class;
Symbol *bindMeBaby; // symbol #X used for restore.

RGBColor 	myPurple	= {38400,25600,51200};
RGBColor	myBlack 	= {0, 0, 0};
RGBColor	myWhite 	= {65535, 65535, 65535};

/*==========================================================================*/

void main(void)
{	
	setup((t_messlist **)&sdifmenu_class, (method) sdifmenu_new, (method)sdifmenu_free, 
		  (short)sizeof(t_sdifmenu), (method) sdifmenu_menufun, A_GIMME, 0);
	addbang((method)sdifmenu_bang);
	addint((method)sdifmenu_int);
	addmess((method)sdifmenu_restore, "done", 	0);
	addmess((method)sdifmenu_read, "read", A_GIMME, 0);
	addmess((method)sdifmenu_clear, "clear", 0);
	addmess((method)sdifmenu_color, "color", 	A_DEFLONG, 0);
	addmess((method)sdifmenu_click,	"click", 	A_CANT, 0);
	addmess((method)sdifmenu_update, "update", 	A_CANT, 0);
	addmess((method)sdifmenu_assist, "assist",	A_CANT, 0);
	addmess((method)sdifmenu_psave, "psave", 	A_CANT, 0);

	
	bindMeBaby = gensym("#X");			/* symbol that we will use for restore */
	
	rescopy('STR#',RES_ID);
	post(VERSION,0);
	post("Copyright © 1999,2000,01,02 Regents of the University of California.");
	
}


/*==========================================================================*/

void sdifmenu_qfn(t_sdifmenu *x)		// a queue function, redraws the foreground
{
	GrafPort *gp;
	gp = patcher_setport(x->x_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {		// check if drawing is allowed
			sdifmenu_redraw(x);					// redraw sdifmenu
		}
		SetPort(gp);
	}
}

void sdifmenu_update(t_sdifmenu *x)
{
	short oh,ow,nh,nw;
	
	ow = x->x_boxwidth;
	oh = x->x_boxheight;
	nw = x->x_box.b_rect.right-x->x_box.b_rect.left;
	nh = x->x_box.b_rect.bottom-x->x_box.b_rect.top;
	
	if(nh != oh || nw != ow) {
		nw = MIN(MAX(nw, MINWIDTH), MAXWIDTH); // constrain to min and max size
		nh = MIN(MAX(nh, MINHEIGHT), MAXHEIGHT);
		box_size(&x->x_box, nw, nh);
		x->x_rect = x->x_box.b_rect;
		x->x_boxwidth = nw;
		x->x_boxheight = nh;
		sdifmenu_calccolumns(x);
		
		DisposeGWorld((GWorldPtr) x->x_offscreen);
		AllocateOffscreen(x);
	}
	sdifmenu_redraw(x);
}

/*--------------------------------------------------------------------------*/

void sdifmenu_redraw(t_sdifmenu *x) // offscreen drawing method!
{	
	GWorldPtr	offscreen;
	GrafPtr		curPort, offPort;
	GDHandle	curDevice;
	Rect		r, offr, myrect, tempr;
	PenState ps;
	
	short font, size;
	int i;
	Str255 dest;
	int ns = x->x_ns;
	int sel = x->x_selection;
	
	RGBColor	rgb = x->x_rgbcolor3;
			
	offscreen = x->x_offscreen;
	font = offscreen->txFont;
	size = offscreen->txSize;
	
	r = x->x_box.b_rect;
	offr = x->x_offrect;
	//SetRect (&temprect, 0, 0, x->x_boxwidth, x->x_boxheight);
	
	// prepare offscreen for drawing
	GetGWorld((CGrafPtr *)&curPort, &curDevice);
	SetGWorld((CGrafPtr)offscreen, NIL);
	offPort = (GrafPort *)offscreen;
	
	PenMode(srcCopy);
	
	RGBForeColor(&rgb);	
	PaintRect(&offr);
	RGBForeColor(&myBlack);
	
	GetPenState(&ps);
	PenNormal();
	PenSize(1,1);
	
	SetRect (&tempr, 20, 60, 70, 90);
	
	//the button
	if (x->x_hilitebutton)
	{
		SetRect (&x->x_readbutton, 0, 0, READBWIDTH, TITLEBAR);
		ShadedRect(&x->x_readbutton, &x->x_rgbcolor4, &x->x_rgbcolor3, &x->x_rgbcolor5, 2);
		
		RGBForeColor(&myBlack);
		TextFont(4); //monaco
		TextSize(9);
		MoveTo(10,15);
		DrawString("\pRead...");
	
	}
	else {
		SetRect (&x->x_readbutton, 0, 0, READBWIDTH, TITLEBAR);
		ShadedRect(&x->x_readbutton, &x->x_rgbcolor3, &x->x_rgbcolor5, &x->x_rgbcolor1, 2);
		
		RGBForeColor(&myBlack);
		TextFont(4); //monaco
		TextSize(9);
		MoveTo(8,14);
		DrawString("\pRead...");
	}
	
	//the currently open filename space
	SetRect (&myrect, READBWIDTH, 0, x->x_boxwidth, TITLEBAR);
	ShadedRect(&myrect, &x->x_rgbcolor3, &x->x_rgbcolor2, &x->x_rgbcolor4, 2);
	 
	RGBForeColor(&myBlack);
	TextFont(4); //monaco
	TextSize(9);
	MoveTo(READBWIDTH+8,14);
	DrawString(x->x_filename);



	//the column headers
	SetRect (&myrect, 0, TITLEBAR, x->x_boxwidth, TITLEBAR*2);
	ShadedRect(&myrect, &x->x_rgbcolor3, &x->x_rgbcolor2, &x->x_rgbcolor4, 1);

	RGBForeColor(&myBlack);
	TextFont(4); //monaco
	TextSize(9);
	MoveTo(8,TITLEBAR+14);
	DrawString("\pStream");
	MoveTo(x->x_column1+8,TITLEBAR+14);
	DrawString("\pType");	
	MoveTo(x->x_column2+8,TITLEBAR+14);
	DrawString("\pStart");	
	MoveTo(x->x_column3+8,TITLEBAR+14);
	DrawString("\pEnd");	
	MoveTo(x->x_column4+8,TITLEBAR+14);
	DrawString("\pFrames");
	
	// the main listing space
	SetRect (&myrect, 0, TITLEBAR*2, x->x_boxwidth, x->x_boxheight);
	ShadedRect(&myrect, &x->x_rgbcolor2, &x->x_rgbcolor1, &x->x_rgbcolor3, 1);
	


	//light horizontal lines
	RGBForeColor(&x->x_rgbcolor1);
	/*for(i=TITLEBAR*3; i< x->x_boxheight; i+=TITLEBAR) 
		LineSegment(2, i, x->x_boxwidth-2, i);*/	
	for(i=0; i<x->x_ns; i++){
		LineSegment(2, (i*TITLEBAR)+(TITLEBAR*3), x->x_boxwidth-2, (i*TITLEBAR)+(TITLEBAR*3));
	}
	
	// selected item highlighted
	if ((sel > -1) && (sel<ns)){
		SetRect (&myrect, 0, (sel*TITLEBAR)+TITLEBAR*2, x->x_boxwidth, ((sel+1)*TITLEBAR)+TITLEBAR*2);
		ShadedRect(&myrect, &x->x_rgbcolor1, &myWhite, &x->x_rgbcolor2, 1);
	}
	
	// the stream menu
	RGBForeColor(&myBlack);
	TextFont(4); //monaco
	TextSize(9);
	for(i=0; i<x->x_ns; i++){
		MoveTo(8, (TITLEBAR*i)+(TITLEBAR*2)+14);
		NumToString(x->x_streamID[i], (unsigned char *)dest);
		DrawString(dest);
		MoveTo(x->x_column1+8, (TITLEBAR*i)+(TITLEBAR*2)+14);
		sprintf((char *)dest, "%c%c%c%c", x->x_frameType[i][0], x->x_frameType[i][1], x->x_frameType[i][2], x->x_frameType[i][3]);
		CtoPstr((char *)dest);
		DrawString(dest);
		MoveTo(x->x_column2+8, (TITLEBAR*i)+(TITLEBAR*2)+14);
		sprintf((char *)dest, "%6.3g", x->x_starttime[i]);
		CtoPstr((char *)dest);
		DrawString(dest);
		MoveTo(x->x_column3+8, (TITLEBAR*i)+(TITLEBAR*2)+14);
		sprintf((char *)dest, "%6.3g", x->x_endtime[i]);
		CtoPstr((char *)dest);
		DrawString(dest);
		MoveTo(x->x_column4+8, (TITLEBAR*i)+(TITLEBAR*2)+14);
		NumToString(x->x_numframes[i], (unsigned char *)dest);
		DrawString(dest);
	}
	// the vertical column separators
	RGBForeColor(&x->x_rgbcolor5);
	LineSegment(x->x_column1, TITLEBAR+1, x->x_column1, x->x_boxheight-1);
	LineSegment(x->x_column2, TITLEBAR+1, x->x_column2, x->x_boxheight-1);
	LineSegment(x->x_column3, TITLEBAR+1, x->x_column3, x->x_boxheight-1);
	LineSegment(x->x_column4, TITLEBAR+1, x->x_column4, x->x_boxheight-1);
	
	SetPenState(&ps);
	PenNormal();
	
	// Then copy the image back on screen and un-prepare offscreen
	SetGWorld((CGrafPtr)curPort, curDevice);
	CopyBits(&offPort->portBits, &curPort->portBits, &offr, &r, srcCopy, NIL);
	
}

/*--------------------------------------------------------------------------*/


void sdifmenu_color(t_sdifmenu *x, long color)
{
	RGBColor rgb;
	unsigned short r, g, b;
	
	color = color%16; // constrains colors to 0-15 zone
	x->x_color = color;
	box_getcolor((Box *)x, color,  &rgb);
	
	//rgb = myPurple; // joke for Matt
	x->x_color = color;
	r = rgb.red;
	g = rgb.green;
	b = rgb.blue;
	
	SetRGBColor(&x->x_rgbcolor1, r+16384, g+16384, b+16384);
	SetRGBColor(&x->x_rgbcolor2, r+8192, g+8192, b+8192);
	SetRGBColor(&x->x_rgbcolor3, r, g, b);
	SetRGBColor(&x->x_rgbcolor4, r-8192, g-8192, b-8192);
	SetRGBColor(&x->x_rgbcolor5, r-16384, g-16384, b-16384);

	
	qelem_set(x->x_qelem);
}

void sdifmenu_click(t_sdifmenu *x, Point pt)
{ 
	Rect r = x->x_box.b_rect;
	int temp;
	char savelock;
	
	pt.h -= r.left;
	pt.v -= r.top;
	
	if (PtInRect(pt, &x->x_readbutton)) {
		x->x_hilitebutton = 1;
		//clock_delay(x->x_klok, 200);
		//sdifmenu_navcrap(x);
		wind_drag((void *)sdifmenu_drag, x, pt);
	}
	
	if ((pt.v - TITLEBAR*2)>0) {
		temp = (pt.v -TITLEBAR*2)/TITLEBAR;
		if (temp < x->x_ns) {
			x->x_selection = temp;
			//post("selection %d", x->x_selection);
			savelock = lockout_set(1);
			sdifmenu_output(x);
			lockout_set(savelock);
		}
	}
	
	qelem_set(x->x_qelem);
	//sdifmenu_output(x);
}



void sdifmenu_drag(t_sdifmenu *x, Point pt, short but)
{
	Rect r = x->x_box.b_rect;
	int state;
	
	pt.h -= r.left; // for some reason the real pt is passed to the drag method
	pt.v -= r.top;  //  and not the pt argument given in  sdifmenu_click

	state = PtInRect(pt, &x->x_readbutton);
	
	if (state != x->x_hilitebutton) {
		x->x_hilitebutton = state;
		qelem_set(x->x_qelem);
	} 
	
	if (!but && state) {
		x->x_hilitebutton = 0;
		sdifmenu_navcrap(x);
		qelem_set(x->x_qelem);

	}
}
/*--------------------------------------------------------------------------*/

void sdifmenu_tick(t_sdifmenu *x)
{	
	x->x_hilitebutton = 0;
	clock_unset(x->x_klok);
	
	qelem_set(x->x_qelem);
}

void sdifmenu_bang(t_sdifmenu *x)
{	
	
	sdifmenu_output(x);
	qelem_set(x->x_qelem);
	
}

void sdifmenu_clear(t_sdifmenu *x)
{	
	
	x->x_ns = 0; // zero streams until a file is read
	x->x_selection = -1; // no selection
	strcpy((char*)x->x_filename, "<no open file>");
	CtoPstr((char *)x->x_filename);
	qelem_set(x->x_qelem);
	
}

void sdifmenu_int(t_sdifmenu *x, long n)
{	
	if ((n <= x->x_ns) && (n>=0)) {
		x->x_selection = n-1; // items in menu are numbered from 1 in max, from 0 in code
		if (n) sdifmenu_output(x);
		qelem_set(x->x_qelem);
	}
}

void sdifmenu_calccolumns(t_sdifmenu *x) // calc column width
{	
	

	x->x_column1 = x->x_boxwidth - (TYPE_COLUMN+TIME_COLUMN+TIME_COLUMN+FRAMES_COLUMN);
	x->x_column2 = x->x_column1+TYPE_COLUMN;
	x->x_column3 = x->x_column2+TIME_COLUMN;
	x->x_column4 = x->x_column3+TIME_COLUMN;
	x->x_column5 = x->x_column4+FRAMES_COLUMN;// = boxwidth
	//post("column values %ld %ld %ld %ld %ld", x->x_column1, x->x_column2, x->x_column3, x->x_column4, x->x_column5);
	qelem_set(x->x_qelem);
	
}

void sdifmenu_output(t_sdifmenu *x)
{
	Atom at[2];
	int ns = x->x_ns;
	int sel = x->x_selection;
	char dest[255];
	
	if (ns && (sel<ns)){
		
		strcpy(dest, (char *)x->x_filename);
		PtoCstr((unsigned char *)dest);
		at[0].a_type = A_SYM;
		at[0].a_w.w_sym = gensym(dest);
		at[1].a_type = A_LONG;
		at[1].a_w.w_long = (long)x->x_streamID[sel];
		
		outlet_list(x->x_out0, 0L, 2, at);
	}
}




/*==========================================================================*/

void sdifmenu_psave(t_sdifmenu *x, void *w)
{
	Rect *r;
	
	r = &x->x_box.b_rect;

	if (x->x_box.b_hidden)
		binbuf_vinsert(w,"ssssllll", gensym("#P"),
									gensym("hidden"),
									gensym("user"),
									gensym("SDIF-menu"),
									(long)(r->left),
									(long)(r->top),
									(long)(r->right - r->left), 
									(long)(r->bottom - r->top) );
	else 
		binbuf_vinsert(w,"sssllll",	gensym("#P"),
									gensym("user"), 
									gensym("SDIF-menu"),
									(long)(r->left),
									(long)(r->top),
									(long)(r->right - r->left), 
									(long)(r->bottom - r->top) );

	binbuf_vinsert(w,"ssl",bindMeBaby,gensym("color"),
									(long)(x->x_color));
									
	binbuf_vinsert(w,"ss",bindMeBaby,gensym("done") );
}

void sdifmenu_restore(t_sdifmenu *x)
{
	// to remove the current binding to #X when restore is finished
	bindMeBaby->s_thing = 0;
	qelem_set(x->x_qelem);
}
	
/* ========================================================================== */

void sdifmenu_assist(t_sdifmenu *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,3,s);
}

/*--------------------------------------------------------------------------*/

void sdifmenu_free(t_sdifmenu *x)
{
	qelem_free(x->x_qelem);
	clock_free(x->x_klok);
	box_free((void *)x);

	if (x->x_offscreen)
		DisposeGWorld((GWorldPtr) x->x_offscreen);

		

}

/*--------------------------------------------------------------------------*/

void *sdifmenu_real(Patcher *p, short bx, short by, short w, short h)
{
	t_sdifmenu *x;
	short bw, bh;
	
	x = (t_sdifmenu*)newobject(sdifmenu_class);
	
	
	x->x_out0 = outlet_new(x, "list");
	
	x->x_patcher = p;
	
	bw = MIN(MAX(w, MINWIDTH), MAXWIDTH); // constrain to min and max size
	bh = MIN(MAX(h, MINHEIGHT), MAXHEIGHT);
	box_new((Box *)x, p, F_DRAWFIRSTIN | F_NODRAWBOX | F_GROWBOTH | F_DRAWINLAST, bx, by, bx+bw, by+bh);
	
	x->x_box.b_firstin = x;
	x->x_rect = x->x_box.b_rect;
	x->x_boxwidth = bw;
	x->x_boxheight = bh;
	
	x->x_qelem = qelem_new(x, (method)sdifmenu_qfn);
	x->x_klok = clock_new(x, (method)sdifmenu_tick);
	x->x_hilitebutton = 0;
	SetRect (&x->x_readbutton, 0, 0, READBWIDTH, TITLEBAR);

	bindMeBaby->s_thing = (Object *)x;	// to restore saved state
	

	AllocateOffscreen(x);
	x->x_ns = 0; // zero streams until a file is read
	strcpy((char*)x->x_filename, "<no open file>");
	CtoPstr((char *)x->x_filename);
	
	sdifmenu_color(x, 0);
	sdifmenu_calccolumns(x);
	x->x_selection = -1;
	
	box_ready((Box *)x);
	return(x);
}

/*--------------------------------------------------------------------------*/

void *sdifmenu_new(Symbol *s, int ac, Atom *av)
{
	void *p, *res;
	
	p = av[0].a_w.w_obj;
	av++;
	ac--;
	res = patchbox(p, (method) sdifmenu_real, ac, av, DEFWIDTH);
	
	return (res);
}

/*--------------------------------------------------------------------------*/

void *sdifmenu_menufun(void *p, long x, long y, long font)
{
	return(sdifmenu_real(p, x, y, DEFWIDTH, DEFHEIGHT));
}

/*==========================================================================*/
/*   graphics shit   */
/*==========================================================================*/

void AllocateOffscreen(t_sdifmenu *x) 
{
	OSErr		err;
	GrafPtr		curPort, offPort;
	GDHandle	curDevice;
	
	x->x_offrect = x->x_box.b_rect;
	OffsetRect(&x->x_offrect, -x->x_offrect.left, -x->x_offrect.top);
	
	// create 16-bit color offscreen
	err = NewGWorld(&x->x_offscreen, 16, &x->x_offrect, NIL, NIL, 0);
	if (err)
		post("SDIF-menu: error %ld creating offscreen", err);
	else {
	
		// prepare offscreen for drawing
		GetGWorld((CGrafPtr *)&curPort, &curDevice);
		SetGWorld((CGrafPtr)x->x_offscreen, NIL);
		
		// initilaize canvas
		offPort = (GrafPort *)x->x_offscreen;
		EraseRect(&offPort->portRect);
		
		// un-prepare offscreen
		SetGWorld((CGrafPtr)curPort, curDevice);	
	}
}

void LineSegment(short a, short b, short c, short d)
{
	MoveTo(a, b);
	LineTo(c, d);
}

void SetRGBColor(RGBColor *rgb, long r, long g, long b)
{
	rgb->red = MIN(MAX(r, 0), 65535);
	rgb->green = MIN(MAX(g, 0), 65535);
	rgb->blue = MIN(MAX(b, 0), 65535);
}

void ShadedRect(Rect *r, RGBColor *rgb1, RGBColor *rgb2, RGBColor *rgb3, int th)
{
	PenState ps;
	int n = th+1;
	
	GetPenState(&ps);

	RGBForeColor(rgb1);
	PaintRect(r);
	
	PenSize(th,th);
	RGBForeColor(rgb2);
	//RGBForeColor(&myWhite);
	MoveTo(r->left+1, r->bottom-n);
	LineTo(r->right-n, r->bottom-n);
	LineTo(r->right-n, r->top+1);
	RGBForeColor(rgb3);
	//RGBForeColor(&myBlack);
	MoveTo(r->left+1, r->bottom-n);
	LineTo(r->left+1, r->top+1);
	LineTo(r->right-n, r->top+1);
	
	RGBForeColor(&myBlack);
	PenSize(1,1);
	FrameRect(r);
	
	SetPenState(&ps);
	
}


/*==========================================================================*/
/*   SDIF file reading shit   */
/*==========================================================================*/


static FILE *sdifmenu_opensdif(t_sdifmenu *x, char *filename) 
{
#ifdef ALWAYS_WANT_TO_LOOK_IN_MAX_FOLDER
	return SDIF_OpenRead(filename);
#else
	short vRefNum;
	FSSpec spec;
	OSErr err;
	FILE *f;
	SDIFresult r;

	
	if (locatefiletype(filename, &vRefNum, 0L, 0L) != 0) {
		post("¥ SDIF-menu: couldn't locate alleged SDIF file %s", filename);
		sprintf((char *)x->x_filename, "¥ <couldn't locate: %s>", filename);
		CtoPstr((char *)x->x_filename);
		return NULL;
	}
	
	strcpy((char*)x->x_filename, (char *)filename);
	CtoPstr((char *)x->x_filename);
	
	// Convert to an FSSpec
	err = FSMakeFSSpec (vRefNum, 0, CtoPstr(filename), &spec);
	PtoCstr((unsigned char *) filename);
	if (err != noErr) {
		if (err == nsvErr) {
			post("¥ SDIF-menu: locatefiletype returned a volume ref number that doesn't exist!");
		} else if (err == bdNamErr) {
			post("¥ SDIF-menu: FSMakeFSSpec thought your filename sucked.");
		} else {
			post("¥ SDIF-menu: FSMakeFSSpec returned %d", err);
		}
		return NULL;
	}
	
	f = FSp_fopen (&spec, "rb");
	
	post("** FSp_fopen returned %x", f);
	if (f == NULL) {
		post("¥ SDIF-menu: FSp_fopen returned NULL!");
	} else {
		r = SDIF_BeginRead(f);
		if (r != ESDIF_SUCCESS) {
			post("SDIF-menu: error while beginning to read SDIF file %s:", filename);
			post("%s", SDIF_GetErrorString(r));
			SDIF_CloseRead(f);
			f = NULL;
		}
	}
	
	return f;
#endif
}



void sdifmenu_read(t_sdifmenu *x, Symbol *s, int ac, Atom *av) 
{	
	SDIFresult r;
	Symbol *fileName;
	FILE *f;
	
	x->x_ns = 0; // zero streams until a file is read
	x->x_selection = -1; // no selection
	strcpy((char*)x->x_filename, "<no open file>");
	CtoPstr((char *)x->x_filename);
		
	if (ac && av[0].a_type == A_SYM){
		fileName = av[0].a_w.w_sym;
		//post("sdifmenu_read for file %s", fileName->s_name);
		f = sdifmenu_opensdif(x, fileName->s_name);
		if (f == NULL) {
			goto out;
		} 

		
		do_streamlist(x, f, fileName->s_name);
		if ((r = SDIF_CloseRead(f))) {
			post("SDIF-menu: error closing SDIF file %s:", fileName->s_name);
			post("%s", SDIF_GetErrorString(r));
		}
	}
	else {
		// if no filename is given then use the Navigation services open dialog
		sdifmenu_navcrap(x);
	}
out:
	qelem_set(x->x_qelem);
}

void do_streamlist(t_sdifmenu *x, FILE *f, char *name) {
	SDIFresult r;
	SDIF_FrameHeader fh;
	int result, i, sawStreamAlready;

	x->x_ns = 0;
		
	while ((r = SDIF_ReadFrameHeader(&fh, f)) == ESDIF_SUCCESS) {
		/* post("** Read frame header: ID %d, time %g, type %c%c%c%c", fh.streamID, fh.time,
			 fh.frameType[0], fh.frameType[1], fh.frameType[2], fh.frameType[3]); */
			 
		sawStreamAlready = 0;
		for (i = 0; i < x->x_ns; ++i) {
			if (x->x_streamID[i] == fh.streamID) {
				sawStreamAlready = 1;
				// Already saw this stream, so just make sure type is OK
				if (!SDIF_Char4Eq(fh.frameType, x->x_frameType[i])) {
					post("¥ streamlist: Warning: First frame for stream %ld", fh.streamID);
					post("¥ had type %c%c%c%c, but frame at time %g has type %c%c%c%c",
						 x->x_frameType[i][0], x->x_frameType[i][1],
						 x->x_frameType[i][2], x->x_frameType[i][3],
						 fh.time, fh.frameType[0], fh.frameType[1], fh.frameType[2],
						 fh.frameType[3]);
				}
				break;
			}
		}
				
		if (!sawStreamAlready) {
			if (x->x_ns >= MAX_STREAMS) {
				post(" ¥ streamlist: error: SDIF file has more than %ld streams!", MAX_STREAMS);
				return;
			}
			++(x->x_ns);
			x->x_streamID[i] = fh.streamID;
			SDIF_Copy4Bytes(x->x_frameType[i], fh.frameType);
			x->x_starttime[i] = fh.time;
			x->x_numframes[i] = 0;
		}
		
		x->x_endtime[i] = fh.time;
		++(x->x_numframes[i]);

		if (r = SDIF_SkipFrame(&fh, f)) {
			post("SDIF-menu: error skipping frame in SDIF file %s:", name);
			post("   %s", SDIF_GetErrorString(r));
			SDIF_CloseRead(f);
			goto out;
		}
	}
	
	if (r != ESDIF_END_OF_DATA) {
		post("SDIF-menu: error reading SDIF file %s:", name);
		post("%s", SDIF_GetErrorString(r));
	}
	

out:

/*	post("Here's all yer %d streams:", x->x_ns);
	for (i = 0; i < x->x_ns; ++i) {
		post("  ID %d, type %c%c%c%c, start %g end %g, %d frames",
			 x->x_streamID[i], x->x_frameType[i][0], x->x_frameType[i][1],
			 x->x_frameType[i][2], x->x_frameType[i][3], x->x_starttime[i],
			 x->x_endtime[i], x->x_numframes[i]);
	} */
	
	return;
}


/*==========================================================================*/
/*    Open files via the new Macintosh Navigation services dialog...        */
/*==========================================================================*/


void sdifmenu_navcrap(t_sdifmenu *x) {
	OSErr err;
	struct NavReplyRecord nrr;
	NavCallBackUserData unused;
	long numItems, i;
	AEKeyword theKeyword;
	char *keywordAsString;
	DescType typeIGot;
	FSSpec theSpec;
	Size sizeIGot;
	char myString[65];
	FILE *f;
	
	
	/* post("** Entering sdifmenu_navcrap()"); */
	
	x->x_ns = 0; // zero streams until a file is read
	x->x_selection = -1; // no selection
	strcpy((char*)x->x_filename, "<no open file>");
	CtoPstr((char *)x->x_filename);
	
	
	err = NavGetFile(NULL,  /* Default location is last folder opened */
			  		 &nrr, /* where answer will go */
			  		 NULL, /* Use default dialog options */
			  		 NewNavEventProc(&worldsLamestEventProc), /* So we can move+resize dialog box*/
					 NULL, /* No previews */
					 NULL, /* No filter procedure */
					 NULL, /* I hope a NULL type list means we try to open any type. */
					 unused);
	if (err != noErr) {
		if (err == -128) {
			/* User hit cancel; no need to print an error message */
		} else {
			post("¥ sdifmenu: NavGetFile returned error %ld", (long) err);
		}
	} else {
		if (nrr.validRecord) {
			numItems = 99999;
			err = AECountItems(&(nrr.selection), &numItems);
			if (err != noErr) {
				post("¥ sdifmenu: AECountItems returned error %ld", (long) err);
			} else {
				for (i = 1; i <= numItems; ++i) {
					err = AEGetNthPtr(&(nrr.selection), i, typeFSS, &theKeyword, &typeIGot,
									   &theSpec, sizeof(theSpec), &sizeIGot);
					if (err != noErr) {
						post("¥ sdifmenu: AEGetNthDesc returned error %ld", (long) err);
					} else {
						if (sizeIGot != sizeof(theSpec)) {
							post("¥ sdifmenu: size returned by AEGetNthDesc is %ld;", sizeIGot);
							post("  but sizeof(FSSpec) is only %ld", sizeof(theSpec));
						}
						
						f = FSp_fopen (&theSpec, "rb"); 
						if (f == NULL) {
							post("¥ sdifmenu: FSp_fopen returned NULL!");
						} else {
							SDIF_BeginRead(f);
							// was: do_streamlist(x, f, "The one you chose");
							do_streamlist(x, f, (char *)theSpec.name);
							fclose(f);
						}

						keywordAsString = ((char *) &theKeyword);
						/* post("Item %ld: keyword %c%c%c%c.", i, keywordAsString[0], 
							 keywordAsString[1], keywordAsString[2], keywordAsString[3]);
						
						post("FSSpec: volume %ld, parent ID %ld", (long) theSpec.vRefNum,
							 (long) theSpec.parID);
						
						PtoCstr(theSpec.name);
						post("   name: %s", theSpec.name); */
						
						strcpy((char*)x->x_filename, (char *)theSpec.name);
						// CtoPstr((char *)x->x_filename);
						
					}
				}
			}
		} else {
			post("¥ sdifmenu: validRecord is false");
		}

		NavDisposeReply(&nrr);
	}
	/* post("Exiting sdifmenu_navcrap()"); */
}

pascal void worldsLamestEventProc(NavEventCallbackMessage callBackSelector,
						NavCBRecPtr callBackParms,
						NavCallBackUserData callBackUD)
{
	WindowPtr window = (WindowPtr) (callBackParms->eventData.eventDataParms.event->message);
	switch (callBackSelector) {
		case kNavCBEvent:
		switch (callBackParms->eventData.eventDataParms.event->what) {
			case updateEvt:
			/* MyHandleUpdateEvent(window,
								(EventRecord*)callBackParms->eventData.event); */
			break;
		}
	break;
	}
}

