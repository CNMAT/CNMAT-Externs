/* xgui - a blank user interface object (ppc only -- for Max 4)
		1999-2001 dudas hack
		
	this serves as a basic example of a very simple UI object including:
		new-style object creation function for UI objects (just _new and _menu methods)
		use of lockout_set to make interrupt-safe output when user clicks/drags on UI object
		compatibility with the preset object
		an offscreen canvas for smooth updating after the object is redrawn (no flicker!)
		old- and new-style color schemes can coexist with a bit of trickery
		correct RGB conversions between (0-65535) and (0-255) using mult/div 257 NOT bit-shift!
*/

#include "ext.h"
#include "ext_common.h"
#include "ext_user.h"
#include "ext_wind.h"
#include <Quickdraw.h>
#include <QDOffscreen.h>

#define RES_ID		10101		// ID of our STR# and SICN resources

#define MAXWIDTH 	512			// maximum width and height of user object in pixels
#define MAXHEIGHT	256			//    (defines maximum offscreen canvas allocation)
#define MINWIDTH 	64			// minimum width and height
#define MINHEIGHT	32			
#define DEFWIDTH 	256			// default width and height
#define DEFHEIGHT	128	
#define RADIUS		4			// radius of the dot that we draw in the UI object

typedef struct xgui
{
	t_box		x_box;
	Rect 		x_rect;			// for comparing with x_box.b_rect
	GWorldPtr	x_offscreen; 	// offscreen for this insstance
	Rect		x_offrect;		// rect for defining offscreen space
	void		*x_qelem;		// queue element for redraw
	
	RGBColor	x_brgb;			// background color
	RGBColor	x_frgb;			// foreground color
	Point		x_point;		// mouse-click-point
	
	void		*x_outlet;		// outlet.
} t_xgui;

void xgui_qfn(t_xgui *x);
void xgui_update(t_xgui *x);
void xgui_allocoffscreen(t_xgui *x);
void xgui_redraw(t_xgui *x);
void xgui_color(t_xgui *x, long color);
void xgui_brgb(t_xgui *x, long r, long g, long b);
void xgui_frgb(t_xgui *x, long r, long g, long b);
void xgui_click(t_xgui *x, Point pt, short modifiers);
void xgui_drag(t_xgui *x, Point pt, short but);
void xgui_output(t_xgui *x);
void xgui_bang(t_xgui *x);
void xgui_list(t_xgui *x, t_symbol *s, int argc, t_atom *argv);
void xgui_psave(t_xgui *x, void *w);
void xgui_preset(t_xgui *x);
void xgui_assist(t_xgui *x, void *b, long m, long a, char *s);
void xgui_free(t_xgui *x);
void *xgui_new(t_symbol *s, short argc, t_atom *argv);
void *xgui_menu(void *p, long x, long y, long font);

void *xgui_class;
t_symbol *ps_preset; // our preset is bound to "_preset"

/*==========================================================================*/

void main(void)
{	
	setup((t_messlist **)&xgui_class, (method)xgui_new, (method)xgui_free, (short)sizeof(t_xgui), (method)xgui_menu, A_GIMME, 0);
	addbang((method)xgui_bang);
	addmess((method)xgui_list, "list", 	A_GIMME, 0);
	addmess((method)xgui_color, "color", 	A_LONG, 0);
	addmess((method)xgui_frgb, "frgb", 	A_LONG, A_LONG, A_LONG, 0);
	addmess((method)xgui_brgb, "brgb", 	A_LONG, A_LONG, A_LONG, 0);
	addmess((method)xgui_click,	"click", 	A_CANT, 0);
	addmess((method)xgui_update, "update", 	A_CANT, 0);
	addmess((method)xgui_assist, "assist",	A_CANT, 0);
	addmess((method)xgui_psave, "psave", 	A_CANT, 0);
	addmess((method)xgui_preset, "preset", 	0);
	
	rescopy(FOUR_CHAR_CODE('STR#'),RES_ID);
	ps_preset = gensym("_preset");		// preset is bound to this symbol
}

/*==========================================================================*/

void xgui_qfn(t_xgui *x)		// a queue function, redraws the foreground
{
	GrafPtr gp;
	gp = patcher_setport(x->x_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {		// check if drawing is allowed
			xgui_redraw(x);					// redraw xgui
			box_enddraw((t_box *)x);		// our drawing is finished (New for Max 4)
		}
		SetPort(gp);
	}
}

void xgui_update(t_xgui *x)
{
	short ow = x->x_rect.right - x->x_rect.left;
	short oh = x->x_rect.bottom - x->x_rect.top;
	short nw = x->x_box.b_rect.right-x->x_box.b_rect.left;
	short nh = x->x_box.b_rect.bottom-x->x_box.b_rect.top;
	
	if(nh != oh || nw != ow) {
		nw = CLIP(nw, MINWIDTH, MAXWIDTH); // constrain to min and max size
		nh = CLIP(nh, MINHEIGHT, MAXHEIGHT);
		box_size(&x->x_box, nw, nh);
		x->x_rect = x->x_box.b_rect;
		
		xgui_allocoffscreen(x); // (existing offsceen is disposed inside)
	}
	xgui_redraw(x);
}

/* ========================================================================== */

void xgui_allocoffscreen(t_xgui *x) 
{
	OSErr		err;
	GrafPtr		curPort, offPort;
	GDHandle	curDevice;
	Rect 		offrect;
	
	// dispose any existing offscreen first
	if (x->x_offscreen) 
		DisposeGWorld(x->x_offscreen);
		
	// define the offscreen canvas area
	offrect = x->x_box.b_rect;
	OffsetRect(&offrect, -offrect.left, -offrect.top);
	x->x_offrect = offrect; // save offrect coordinates in object struct
	
	// create 16-bit color offscreen canvas
	err = NewGWorld(&x->x_offscreen, 16, &offrect, NIL, NIL, 0);
	if (err)
		post("error %ld creating offscreen", err);
	else { 
		// initialize offscreen for drawing
		GetGWorld((CGrafPtr *)&curPort, &curDevice); // save current graphics port
		SetGWorld((CGrafPtr)x->x_offscreen, NIL); // set port to offscreen canvas
		EraseRect(&offrect); // erase offscreen canvas
		SetGWorld((CGrafPtr)curPort, curDevice);	// restore current graphics port	
	}
}

/*--------------------------------------------------------------------------*/

void xgui_redraw(t_xgui *x) // offscreen drawing method!
{	
	GWorldPtr	offscreen = x->x_offscreen;
	GrafPtr		curPort, offPort;
	GDHandle	curDevice;
	Rect		r = x->x_box.b_rect;
	Rect		offrect = x->x_offrect;
	Rect		dot;
	Point 		pt = x->x_point;
	
	RGBColor	myBlack = {0, 0, 0};
	RGBColor	brgb = x->x_brgb;
	RGBColor	frgb = x->x_frgb;
	
	// in case old-fashioned color selection has changed (if user just chose "Color..." from menu)
	if (x->x_box.b_color != 0) 
		xgui_color(x,x->x_box.b_color);
	
	SetRect(&dot, pt.h-RADIUS, pt.v-RADIUS, pt.h+RADIUS, pt.v+RADIUS); // set dot rect from point
	
	// save the current graphics port and set the drawing port to the offscreen
	GetGWorld((CGrafPtr *)&curPort, &curDevice);
	SetGWorld((CGrafPtr)offscreen, NIL);
	offPort = (GrafPtr)offscreen;
	
	// draw our stuff on the offscreen canvas
	PenMode(srcCopy);
	
	RGBForeColor(&brgb);	
	PaintRect(&offrect);
	RGBForeColor(&frgb);	
	PaintOval(&dot);
	RGBForeColor(&myBlack);	
	FrameRect(&offrect);
	
	PenNormal();
	
	// now restore previous drawing port and copy the offscreen image back on-screen
	SetGWorld((CGrafPtr)curPort, curDevice);
	CopyBits(GetPortBitMapForCopyBits((CGrafPtr)offPort), GetPortBitMapForCopyBits((CGrafPtr)curPort), &offrect, &r, srcCopy, NIL);
}

/*--------------------------------------------------------------------------*/

// old-style color message (16 indexed colors)
void xgui_color(t_xgui *x, long color)
{
	RGBColor rgb;
	
	color = CLIP(color, 0, 15); // constrains colors to 0-15 zone
	box_getcolor((t_box *)x, color,  &rgb);
	x->x_box.b_color = 0; // we do this so both color schemes work together (see xgui_redraw)
	
	xgui_brgb(x, rgb.red/257, rgb.green/257, rgb.blue/257); // send max-style rgb values to xgui_brgb()
}

// new RGB color messages (0-255 range correctly converted to 0-65535 with multiplication by 257)
void xgui_brgb(t_xgui *x, long r, long g, long b)
{
	x->x_brgb.red = CLIP(r*257, 0, 65535);
	x->x_brgb.green = CLIP(g*257, 0, 65535);
	x->x_brgb.blue = CLIP(b*257, 0, 65535);
	
	qelem_set(x->x_qelem);
}

void xgui_frgb(t_xgui *x, long r, long g, long b)
{
	x->x_frgb.red = CLIP(r*257, 0, 65535);
	x->x_frgb.green = CLIP(g*257, 0, 65535);
	x->x_frgb.blue = CLIP(b*257, 0, 65535);
	
	qelem_set(x->x_qelem);
}

// if the user clicks on out object's rect...
void xgui_click(t_xgui *x, Point pt, short modifiers)
{ 
	Rect r = x->x_box.b_rect;
	short lock;

	x->x_point.h = pt.h-r.left;
	x->x_point.v = pt.v-r.top;

	lock = lockout_set(1);	// inhibit interupt-level processing while outputting as a result of click&drag
		xgui_output(x);
	lockout_set(lock);		// restore lockout flag to previous value
	
	qelem_set(x->x_qelem);
	wind_drag((void *)xgui_drag, x, pt);
}

void xgui_drag(t_xgui *x, Point pt, short but)
{
	Rect r = x->x_box.b_rect;
	short lock;

	if (but) {
		//if (PtInRect(pt,&x->x_box.b_rect)) { // if we wanted no output while the mouse was out of bounds
		pt.h = CLIP(pt.h, r.left, r.right);
		pt.v = CLIP(pt.v, r.top, r.bottom);
		
		x->x_point.h = pt.h-r.left;
		x->x_point.v = pt.v-r.top;
		
		lock = lockout_set(1);	// inhibit interupt-level processing while outputting as a result of click&drag
			xgui_output(x);
		lockout_set(lock);		// restore lockout flag to previous value
		
		qelem_set(x->x_qelem);
		//}
	}
}
/*--------------------------------------------------------------------------*/

// respond to typed messages (bang and list)

void xgui_bang(t_xgui *x)
{	
	xgui_output(x);
	qelem_set(x->x_qelem);
}

void xgui_list(t_xgui *x, t_symbol *s, int argc, t_atom *argv)
{	
	// set point from list
	if (argc > 0) {
		if (argv[0].a_type == A_LONG) x->x_point.h = argv[0].a_w.w_long; 
		else if (argv[0].a_type == A_FLOAT) x->x_point.h = (short)argv[0].a_w.w_float; 
		else x->x_point.h = 0; 
	}
	if (argc > 1) {
		if (argv[1].a_type == A_LONG) x->x_point.v = argv[1].a_w.w_long; 
		else if (argv[1].a_type == A_FLOAT) x->x_point.v = (short)argv[1].a_w.w_float; 
		else x->x_point.v = 0; 
	}
	
	xgui_output(x);
	qelem_set(x->x_qelem);
}

void xgui_output(t_xgui *x)
{
	Atom argv[2];
	
	argv[0].a_type = A_LONG;
	argv[0].a_w.w_long = (long)x->x_point.h;
	argv[1].a_type = A_LONG;
	argv[1].a_w.w_long = (long)x->x_point.v;
	
	outlet_list(x->x_outlet, 0L, 2, argv);
}

/*==========================================================================*/

// save our UI object's location and appearance with the patcher...

void xgui_psave(t_xgui *x, void *w)
{
	Rect r = x->x_box.b_rect;
	t_atom argv[16];
	short inc = 0;
	
	SETSYM(argv,gensym("#P"));
	if (x->x_box.b_hidden) {	// i.e. if it's hidden when the patcher is locked
		SETSYM(argv+1,gensym("hidden"));
		inc = 1;
	}
	SETSYM(argv+1+inc,gensym("user"));
	SETSYM(argv+2+inc,ob_sym(x));
	SETLONG(argv+3+inc,r.left);
	SETLONG(argv+4+inc,r.top);
	SETLONG(argv+5+inc,r.right - r.left);	// width
	SETLONG(argv+6+inc,r.bottom - r.top);	// height
	
	// rgb colors in 0-65535 range correctly converted to 0-255 range with division by 257
	SETLONG(argv+7+inc,x->x_brgb.red/257);
	SETLONG(argv+8+inc,x->x_brgb.green/257);
	SETLONG(argv+9+inc,x->x_brgb.blue/257);
	SETLONG(argv+10+inc,x->x_frgb.red/257);
	SETLONG(argv+11+inc,x->x_frgb.green/257);
	SETLONG(argv+12+inc,x->x_frgb.blue/257);
	binbuf_insert(w,0L,13+inc,argv);
}

// respond to the preset object so it can gather our current state

void xgui_preset(t_xgui *x)
{
	t_atom msg[6];
	void *z;
	
	if (!(z = ps_preset->s_thing))	// if there's nothing bound to the _preset symbol, then exit
		return;
	
	SETOBJ(msg, (t_object *)x); 	// first element is object pointer
	SETSYM(msg+1, ob_sym(x));		// second is object symbol
	SETSYM(msg+2, gensym("list"));	// next is message name, then arguments
	SETLONG(msg+3, x->x_point.h);		
	SETLONG(msg+4, x->x_point.v);
	binbuf_insert(z, 0L, 5, msg);	// hand over our message to the preset
}
	
/* ========================================================================== */

// standard boring assist method

void xgui_assist(t_xgui *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,3,s);
}

/*--------------------------------------------------------------------------*/

// this is where we need to explicitly free-up the memory which we
//		allocated for our offscreen canvas, qelem and box

void xgui_free(t_xgui *x)
{
	if (x->x_offscreen)
		DisposeGWorld((GWorldPtr) x->x_offscreen);
	qelem_free(x->x_qelem);
	box_free((void *)x);
}

/*--------------------------------------------------------------------------*/

// when the UI external is read-in from a patcher file...

void *xgui_new(t_symbol *s, short argc, t_atom *argv)
{
	t_xgui *x;
	void *patcher;
	long bx, by, width, height;
	short bw, bh, flags;
	
	x = (t_xgui*)newobject(xgui_class);
	patcher = argv[0].a_w.w_obj;
	bx = argv[1].a_w.w_long;
	by = argv[2].a_w.w_long;
	width = argv[3].a_w.w_long;
	height = argv[4].a_w.w_long;	
	
	// create outlet (already one inlet by default)
	x->x_outlet = outlet_new(x, "list"); // list outlet
	
	bw = CLIP(width, MINWIDTH, MAXWIDTH); // constrain to min and max size
	bh = CLIP(height, MINHEIGHT, MAXHEIGHT);
	flags = F_DRAWFIRSTIN | F_NODRAWBOX | F_GROWBOTH | F_DRAWINLAST | F_SAVVY;
	box_new((t_box *)x, patcher, flags, bx, by, bx+bw, by+bh);
	x->x_box.b_firstin = (void *)x;
	x->x_rect = x->x_box.b_rect;
	
	// place default dot at center of ui obj rect
	x->x_point.h = bw/2; 
	x->x_point.v = bh/2;
	
	// restore rgb colors (0-255 range correctly converted to 0-65535 with multiplication by 257)
	x->x_brgb.red = argv[5].a_w.w_long*257;
	x->x_brgb.green = argv[6].a_w.w_long*257;
	x->x_brgb.blue = argv[7].a_w.w_long*257;
	x->x_frgb.red = argv[8].a_w.w_long*257;
	x->x_frgb.green = argv[9].a_w.w_long*257;
	x->x_frgb.blue = argv[10].a_w.w_long*257;
	
	x->x_qelem = qelem_new(x, (method)xgui_qfn);

	x->x_offscreen = 0L; // initialize offscreen to 0
	xgui_allocoffscreen(x); // create offscreen
	
	box_ready((t_box *)x);
	return(x);
}

/*--------------------------------------------------------------------------*/

// if the user selects our object from the toolbar/menu, 
//		we need to supply the _new method with default values

void *xgui_menu(void *p, long x, long y, long font)
{
	t_atom argv[16];
	
	SETOBJ(argv,p);
	SETLONG(argv+1,x);
	SETLONG(argv+2,y);
	SETLONG(argv+3,DEFWIDTH);
	SETLONG(argv+4,DEFHEIGHT);
	SETLONG(argv+5,170); // default grey
	SETLONG(argv+6,170);
	SETLONG(argv+7,170);
	SETLONG(argv+8,0); // default black
	SETLONG(argv+9,0);
	SETLONG(argv+10,0);
	return xgui_new(gensym("xgui"),11,argv);
}

