// zplane user interface object
// ©1998 Richard Dudas, CNMAT

#define __EXT_USER	// this is not explained in the writing msp externals doc
					// necessary if you're making an MSP user interface object

#include "ext.h"
#include "ext_common.h"
#include "ext_user.h"
#include "ext_wind.h"
#include "z_dsp.h"
#include <Quickdraw.h>
#include <QDOffscreen.h>
#include <math.h>

#define RES_ID		16662
#define R_PI		3.14159265358979323846
#define R_TWOPI		6.28318530717958647692

#define DEFWIDTH 	256			// default width-height of user object in pixels
#define MINWIDTH 	64			// minimum width-height of user object in pixels
#define MAXWIDTH 	1024		// maximum width-height of user object in pixels

typedef struct _zplane
{
	t_pxbox		x_box;
	Patcher		*x_patcher;
	Rect 		x_rect; // old rect size
	void		*x_qelem;
	float		x_sr; //sampling rate
	
	GWorldPtr	x_offscreen; 	// instance-specific offscreen
	Rect		x_offrect;		// rect for defining offscreen space
	long		x_boxsize;

	float x_a0;		// gain
	float x_a1;		// feedforward 1
	float x_a2;		// feedforward 2
	float x_b1;		// feedback 1
	float x_b2;		// feedback 2
	
	Rect x_pole[2];
	Rect x_zero[2]; // rects for click
	int x_dragged;

	RGBColor	x_color;	// each instance can have its own color
	RGBColor	x_shadow;	// each instance can have its own color
	RGBColor	x_lines;	// each instance can have its own color
	
	void		*x_out0;	// float outlet.
	void		*x_out1;	// float outlet.
	void		*x_out2;	// float outlet.
	void		*x_out3;	// float outlet.
	void		*x_out4;	// float outlet.
} t_zplane;
	 			

void zplane_dsp(t_zplane *x, t_signal **sp, short *count);

void zplane_qfn(t_zplane *x);
void zplane_redraw(t_zplane *x);
void zplane_update(t_zplane *x);
void zplane_newcolor(t_zplane *x, long r, long g, long b);
void zplane_click(t_zplane *x, Point pt, short modifiers);
void zplane_drag(t_zplane *x, Point pt, short but);

void zplane_float(t_zplane *x, double f);
void zplane_ft1(t_zplane *x, double f);
void zplane_ft2(t_zplane *x, double f);
void zplane_ft3(t_zplane *x, double f);
void zplane_ft4(t_zplane *x, double f);

void zplane_bang(t_zplane *x);
void zplane_output(t_zplane *x);

void zplane_psave(t_zplane *x, void *w);
void zplane_restore(t_zplane *x);
void zplane_assist(t_zplane *x, void *b, long m, long a, char *s);
void zplane_free(t_zplane *x);
void *zplane_real(Patcher *p, int cx, int cy, short w, short h);
void *zplane_new(Symbol *s, int ac, Atom *av);
void *zplane_menufun(void *p, long x, long y, long font);

void AllocateOffscreen(t_zplane *x);
void DrawZero(t_zplane *x, Rect *r, double xx, double yy);
void DrawPole(t_zplane *x, Rect *r, double xx, double yy);

void *zplane_class;

Symbol *bindMeBaby; // symbol #X used for restore.




/*==========================================================================*/

void main(void)
{	
	setup(&zplane_class, zplane_new, (method)zplane_free, (short)sizeof(t_zplane), zplane_menufun, A_GIMME, 0);
	addmess((method)zplane_dsp, "dsp", A_CANT, 0);
	addfloat((method)zplane_float);
	addftx((method)zplane_ft1, 1);
	addftx((method)zplane_ft2, 2);
	addftx((method)zplane_ft3, 3);
	addftx((method)zplane_ft4, 4);
	addbang((method)zplane_bang);

	
	addmess((method)zplane_restore, "done", 	0);
	addmess((method)zplane_newcolor, "color", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)zplane_click, "click", 	A_CANT, 0);
	addmess((method)zplane_update, "update", 	A_CANT, 0);
	addmess((method)zplane_assist, "assist",	A_CANT, 0);
	addmess((method)zplane_psave, "psave", 	A_CANT, 0);
	bindMeBaby = gensym("#X");			/* symbol that we will use for restore */
	
	rescopy('STR#',RES_ID);

	dsp_initboxclass();
	
	post("Z-Plane Object v0.1 Richard Dudas",0);
	post ("copyright © 1998 Regents of the University of California. All Rights Reserved",0); 
	
}

/*--------------------------------------------------------------------------*/

void zplane_dsp(t_zplane *x, t_signal **sp, short *count)
{
	x->x_sr = sys_getsr();
	qelem_set(x->x_qelem);
}

/*==========================================================================*/

void zplane_qfn(t_zplane *x)		// a queue function, redraws the foreground
{
	GrafPort *gp;
	gp = patcher_setport(x->x_box.z_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {				// check if drawing is allowed
			zplane_redraw(x);						// redraw zplane
		}
		SetPort(gp);
	}
}

void zplane_update(t_zplane *x)
{
	short oh,ow,nh,nw;

	ow = x->x_rect.right-x->x_rect.left;
	oh = x->x_rect.bottom-x->x_rect.top;
	nw = x->x_box.z_box.b_rect.right-x->x_box.z_box.b_rect.left;
	nh = x->x_box.z_box.b_rect.bottom-x->x_box.z_box.b_rect.top;
	
	if(nh != oh || nw != ow) {
		nw = MIN(MAX(nw, MINWIDTH), MAXWIDTH); // constrain to min and max size
		nh = MIN(MAX(nh, MINWIDTH), MAXWIDTH);
		box_size(&x->x_box.z_box, nw, nh);
		x->x_rect = x->x_box.z_box.b_rect;
		x->x_boxsize = nw;
		
		DisposeGWorld((GWorldPtr) x->x_offscreen);
		AllocateOffscreen(x);
	}
	zplane_redraw(x);
}

/*--------------------------------------------------------------------------*/

void zplane_redraw(t_zplane *x) // offscreen drawing method!
{	
	GWorldPtr	offscreen;
	GrafPtr		gp, oldPort, offPort;
	GDHandle	oldDevice;
	Rect		r, temprect;
	RGBColor	myColor, myShadow, myLines;
	
	RGBColor	myBlack = {0, 0, 0};
	RGBColor	myLtGrey = {54613, 54613, 54613}; // 5/6 white
	RGBColor	myMedGrey = {49151, 49151, 49151}; // 3/4 white
	RGBColor	myGrey = {43691, 43691, 43691}; // 2/3 white
	RGBColor	myDkGrey = {32768, 32768, 32768}; // 1/2 white
	RGBColor	myRed = {65535, 0, 0};
	
	float sr = x->x_sr;

	double a0 = x->x_a0;
	double a1 = x->x_a1;
	double a2 = x->x_a2;
	double b1 = x->x_b1;
	double b2 = x->x_b2;
	
	double n, x0, x1, y0, y1, x2, x3, y2, y3;
	
	long BoxSize = x->x_boxsize;
	float BoxDiv = (float)x->x_boxsize/6.0f;
	
		
	gp = patcher_setport(x->x_box.z_box.b_patcher);	//set grafport to patcher

	if (gp)
	{
		offscreen = x->x_offscreen;
		myColor =  x->x_color;
		myShadow = x->x_shadow;
		myLines = x->x_lines;
		
		r = x->x_box.z_box.b_rect;
		SetRect(&temprect, (short)BoxDiv, (short)BoxDiv, (short)(5.0f*BoxDiv), (short)(5.0f*BoxDiv));

		if (a0 == 0)
			x0 = x1 = y0 = y1 = 0;
		else {
			n = a1*a1 - 4*a0*a2;
			if (n >= 0) {
				x0 = (-a1 + sqrt(n)) / 2*a0;
				x1 = (-a1 - sqrt(n)) / 2*a0;
				y0 = y1 = 0;
			}
			else {
				x0 = x1 = -a1 / 2.*a0;
				y0 = sqrt(-n) / 2.*a0;
				y1 = -y0;
			}
		}
		n = b1*b1 - 4*b2;
		if (n >= 0) {
			x2 = (-b1 + sqrt(n)) / 2;
			x3 = (-b1 - sqrt(n)) / 2;
			y2 = y3 = 0;
		}
		else {
			x2 = x3 = -b1 / 2;
			y2 = sqrt(-n) / 2;
			y3 = -y2;
		}
		

		// prepare offscreen for drawing
		GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
		SetGWorld((CGrafPtr)offscreen, NIL);
		
		offPort = (GrafPort *)offscreen;
		
		PenMode(srcCopy);
		
		RGBForeColor(&myMedGrey);	
		PaintRect(&offPort->portRect);
		RGBForeColor(&myLtGrey);	
		PaintOval(&temprect);
		
		RGBForeColor(&myDkGrey);

		MoveTo((short)BoxDiv, 0);
		LineTo((short)BoxDiv, BoxSize);
		MoveTo((short)(2.0f*BoxDiv), 0);
		LineTo((short)(2.0f*BoxDiv), BoxSize);
		MoveTo((short)(4.0f*BoxDiv), 0);
		LineTo((short)(4.0f*BoxDiv), BoxSize);
		MoveTo((short)(5.0f*BoxDiv), 0);
		LineTo((short)(5.0f*BoxDiv), BoxSize);
		
		MoveTo(0, (short)BoxDiv);
		LineTo(BoxSize, (short)BoxDiv);
		MoveTo(0, (short)(2.0f*BoxDiv));
		LineTo(BoxSize, (short)(2.0f*BoxDiv));
		MoveTo(0, (short)(4.0f*BoxDiv));
		LineTo(BoxSize, (short)(4.0f*BoxDiv));
		MoveTo(0, (short)(5.0f*BoxDiv));
		LineTo(BoxSize, (short)(5.0f*BoxDiv));

		RGBForeColor(&myRed);
		MoveTo(BoxSize/2, 0);
		LineTo(BoxSize/2, BoxSize);
		MoveTo(0, BoxSize/2);
		LineTo(BoxSize, BoxSize/2);
		
		RGBForeColor(&myBlack);
		FrameOval(&temprect);
	
		//RGBForeColor(&myRed);
		PenSize(2, 2);
		DrawZero(x, &x->x_zero[0], x0, y0);
		DrawZero(x, &x->x_zero[1], x1, y1);
		DrawPole(x, &x->x_pole[0], x2, y2);
		DrawPole(x, &x->x_pole[1], x3, y3);
		
		PenSize(1, 1);
		RGBForeColor(&myBlack);
		FrameRect(&offPort->portRect);	
		
		PenNormal();
		
		// Then copy the image back on screen.
		SetGWorld((CGrafPtr)oldPort, oldDevice);
		CopyBits(&offPort->portBits, &gp->portBits, 
			&offPort->portRect, &r, srcCopy, NIL);
		
		// un-prepare offscreen
		SetPort(gp);
	}
}

/*--------------------------------------------------------------------------*/
void zplane_newcolor(t_zplane *x, long r, long g, long b)
{
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	
	x->x_color.red = r << 8;
	x->x_color.green = g << 8;
	x->x_color.blue = b << 8;
	
	x->x_shadow.red = r-40 << 8;
	x->x_shadow.green = g-40 << 8;
	x->x_shadow.blue = b-40 << 8;	
	
	x->x_lines.red = r-80 << 8;
	x->x_lines.green = g-80 << 8;
	x->x_lines.blue = b-80 << 8;
	
	qelem_set(x->x_qelem);
}

void zplane_click(t_zplane *x, Point pt, short modifiers)
{ 
	int isit;
	float sro2 = x->x_sr/2;
		
	x->x_dragged = 0;
	isit = PtInRect(pt, &x->x_zero[0]);
	if (isit) 
		x->x_dragged = 1;
	isit = PtInRect(pt, &x->x_zero[1]);
	if (isit)
		x->x_dragged = 2;
	isit = PtInRect(pt, &x->x_pole[0]);
	if (isit)
		x->x_dragged = 3;
	isit = PtInRect(pt, &x->x_pole[1]);
	if (isit) 
		x->x_dragged = 4;
	
	qelem_set(x->x_qelem);
		
	wind_drag((void *)zplane_drag, x, pt);
}

void zplane_drag(t_zplane *x, Point pt, short but)
{
	Rect r;
	Point p;
	float sro2 = x->x_sr/2;
	
	long BoxSize = x->x_boxsize;
	long BoxDiv = BoxSize / 6;
	double coordx, coordy;
	
	p.h = pt.h - x->x_box.z_box.b_rect.left;
	p.v = pt.v - x->x_box.z_box.b_rect.top;

	if (but) {
		if (PtInRect(pt,&x->x_box.z_box.b_rect)) {
			if (x->x_dragged == 4) {
				coordx = -(p.h - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				coordy = -(p.v - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				x->x_b1 = -(coordx+coordy);
				x->x_b2 = (coordx*coordx)+(coordy*coordy);
				//post("drag: %f %f", coordx, coordy);
			}
			if (x->x_dragged == 3) {
				coordx = -(p.h - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				coordy = -(p.v - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				x->x_b1 = -(coordx+coordy);
				x->x_b2 = coordx*coordx+coordy*coordy;
				//post("drag: %f %f", coordx, coordy);
			}
			if (x->x_dragged == 2) {
				coordx = -(p.h - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				coordy = -(p.v - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				x->x_a1 = -(coordx+coordy);
				x->x_a2 = coordx*coordx+coordy*coordy;
				x->x_a0 = 1.;
				//post("drag: %f %f", coordx, coordy);
			}
			if (x->x_dragged == 1) {
				coordx = -(p.h - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				coordy = -(p.v - (float)BoxSize/2.0f) / (float)(2.0f*BoxDiv);
				x->x_a1 = -(coordx+coordy);
				x->x_a2 = coordx*coordx+coordy*coordy;
				x->x_a0 = 1.;
				//post("drag: %f %f", coordx, coordy);
			}
			// do something here
			qelem_set(x->x_qelem);
			zplane_output(x);
		}
	}

}
/*--------------------------------------------------------------------------*/

void zplane_float(t_zplane *x, double f)
{	
	x->x_a0 = f;
	zplane_output(x);
	qelem_set(x->x_qelem);
}
void zplane_ft1(t_zplane *x, double f)
{	
	x->x_a1 = f;
	zplane_output(x);
	qelem_set(x->x_qelem);
}
void zplane_ft2(t_zplane *x, double f)
{	
	x->x_a2 = f;
	zplane_output(x);
	qelem_set(x->x_qelem);
}
void zplane_ft3(t_zplane *x, double f)
{	
	x->x_b1 = f;
	zplane_output(x);
	qelem_set(x->x_qelem);
}
void zplane_ft4(t_zplane *x, double f)
{	
	x->x_b2 = f;
	zplane_output(x);
	qelem_set(x->x_qelem);
}

/*==========================================================================*/

void zplane_bang(t_zplane *x)
{
	qelem_set(x->x_qelem); // the qelem_set is in the int method
}

void zplane_output(t_zplane *x)
{	
	Atom at[5];
	
	SETFLOAT(&at[0], x->x_a0);
	SETFLOAT(&at[1], x->x_a1);
	SETFLOAT(&at[2], x->x_a2);
	SETFLOAT(&at[3], x->x_b1);
	SETFLOAT(&at[4], x->x_b2);
	outlet_list(x->x_out0, 0L, 5, at);
}

/*==========================================================================*/

void zplane_psave(t_zplane *x, void *w)
{
	Rect *r;
	
	r = &x->x_box.z_box.b_rect;

	if (x->x_box.z_box.b_hidden)
		binbuf_vinsert(w,"sssslll",	gensym("#P"),
									gensym("hidden"),
									gensym("user"),
									gensym("zplane~"),
									(long)(r->left),
									(long)(r->top),	
									(long)(x->x_boxsize) );
	else 
		binbuf_vinsert(w,"ssslll",	gensym("#P"),
									gensym("user"), 
									gensym("zplane~"),
									(long)(r->left),
									(long)(r->top),
									(long)(x->x_boxsize) );
									
	binbuf_vinsert(w,"sslll",bindMeBaby,gensym("color"),
									(long)(x->x_color.red >>8),
									(long)(x->x_color.green>>8),
									(long)(x->x_color.blue>>8) );

									
	binbuf_vinsert(w,"ss",bindMeBaby,gensym("done") );
}

void zplane_restore(t_zplane *x)
{
	// just to remove the current binding to #X when restore is finished
	bindMeBaby->s_thing = 0;
	
	qelem_set(x->x_qelem);
}
	
/* ========================================================================== */

void zplane_assist(t_zplane *x, void *b, long m, long a, char *s)
{
	//assist_string(RES_ID,m,a,1,3,s);
}

/*--------------------------------------------------------------------------*/

void zplane_free(t_zplane *x)
{
	dsp_freebox((t_pxbox *)x);
	qelem_free(x->x_qelem);
	box_free((void *)x);

	if (x->x_offscreen) {
		DisposeGWorld((GWorldPtr) x->x_offscreen);
		//post("offscreen disposed",0);
	}
	
}

/*--------------------------------------------------------------------------*/

void *zplane_real(Patcher *p, int cx, int cy, short w, short h)
{
	int i;
	short flags;
	t_zplane *x;
	
	x = (t_zplane*)newobject(zplane_class);
	dsp_setupbox((t_pxbox *)x, 0); // a0
	floatin((t_pxbox *)x, 4); // b2
	floatin((t_pxbox *)x, 3); // b1
	floatin((t_pxbox *)x, 2); // a2
	floatin((t_pxbox *)x, 1); // a1
	x->x_out4 = floatout(x);
	x->x_out3 = floatout(x);
	x->x_out2 = floatout(x);
	x->x_out1 = floatout(x);
	x->x_out0 = listout(x);
	
	x->x_patcher		= p;

	x->x_boxsize = w;
	SetRect(&x->x_rect, cx, cy, cx + w, cy + w);

	flags = F_DRAWFIRSTIN | F_GROWY | F_DRAWINLAST | F_NODRAWBOX;
	
	box_new((Box *)x, p, flags,
				x->x_rect.left,
				x->x_rect.top,
				x->x_rect.right,
				x->x_rect.bottom);
			
	x->x_box.z_box.b_firstin = x;
	
	x->x_rect = x->x_box.z_box.b_rect;
	x->x_qelem = qelem_new(x, (method)zplane_qfn);

	bindMeBaby->s_thing = (Object *)x;	// to restore saved state

	AllocateOffscreen(x);
	
	zplane_newcolor(x, 224, 224, 224);	// grey for default background
	
	x->x_sr = sys_getsr();
	
	x->x_a0 = 1.;
	x->x_a1 = 0.;
	x->x_a2 = 0.;
	x->x_b1 = 0.;
	x->x_b2 = 0.;
	
	box_ready((void *)x);
	return(x);
}

/*--------------------------------------------------------------------------*/

void *zplane_new(Symbol *s, int ac, Atom *av)
{
	void *p, *res;
	
	p = av[0].a_w.w_obj;
	av++;
	ac--;
	res = patchbox(p, zplane_real, ac, av, DEFWIDTH);
	
	return (res);
}

/*--------------------------------------------------------------------------*/

void *zplane_menufun(void *p, long x, long y, long font)
{
	return(zplane_real(p, x, y, DEFWIDTH, DEFWIDTH));
}

/* ========================================================================== */


void AllocateOffscreen(t_zplane *x)
{
	OSErr		err;
	GrafPtr		curPort, offPort;
	GDHandle	curDev;
	
	x->x_offrect = x->x_box.z_box.b_rect;
	OffsetRect(&x->x_offrect, -x->x_offrect.left, -x->x_offrect.top);
	
	// create 16-bit color offscreen
	err = NewGWorld(&x->x_offscreen, 16, &x->x_offrect, NIL, NIL, 0);
	if (err)
		post("error %ld creating offscreen", err);
	else {
		// prepare offscreen for drawing
		GetGWorld((CGrafPtr *)&curPort, &curDev);
		SetGWorld((CGrafPtr)x->x_offscreen, NIL);
	
		// initilaize canvas
		offPort = (GrafPort *)x->x_offscreen;
		EraseRect(&offPort->portRect);
	
		// un-prepare offscreen
		SetGWorld((CGrafPtr)curPort, curDev);	
	}
}
 
void DrawZero(t_zplane *x, Rect *r, double xx, double yy)
{
	long BoxSize = x->x_boxsize;
	long BoxDiv = BoxSize / 6;
	double coordx, coordy;
	
	coordx = -xx * (double)(2*BoxDiv) + (double)BoxSize/2;
	coordy = -yy * (double)(2*BoxDiv) + (double)BoxSize/2;

	r->left = coordx-4;
	r->top = coordy-4;
	r->right = coordx+4;
	r->bottom = coordy+4;
	
	FrameOval(r);
	r->left += x->x_box.z_box.b_rect.left;
	r->top += x->x_box.z_box.b_rect.top;
	r->right += x->x_box.z_box.b_rect.left;
	r->bottom += x->x_box.z_box.b_rect.top;
}
void DrawPole(t_zplane *x, Rect *r, double xx, double yy)
{
	long BoxSize = x->x_boxsize;
	long BoxDiv = BoxSize / 6;
	double coordx, coordy;
	
	coordx = -xx * (double)(2*BoxDiv) + (double)BoxSize/2;
	coordy = -yy * (double)(2*BoxDiv) + (double)BoxSize/2;
	
	r->left = coordx-3;
	r->top = coordy-3;
	r->right = coordx+3;
	r->bottom = coordy+3;
	
	MoveTo(r->left, r->top);
	LineTo(r->right, r->bottom);
	MoveTo(r->left, r->bottom);
	LineTo(r->right, r->top);
	
	r->left += x->x_box.z_box.b_rect.left-1;
	r->top += x->x_box.z_box.b_rect.top-1;
	r->right += x->x_box.z_box.b_rect.left+1;
	r->bottom += x->x_box.z_box.b_rect.top+1;
}
	