// gspline - a graphic spline curve interpolator and display

#define __EXT_USER	// this is not explained in the writing msp externals doc
					// necessary if you're making an MSP user interface object

#include "ext.h"
#include "ext_common.h"
#include "ext_user.h"
#include "ext_wind.h"
#include "z_dsp.h"
#include "ext_strings.h"
#include <Quickdraw.h>
#include <QDOffscreen.h>
#include <math.h>

#define RES_ID		12344

#define MAXSIZE 256

#define MAXWIDTH 	1024		// maximum width and height of user object in pixels
#define MAXHEIGHT	512			//    (defines maximum offscreen canvas allocation)
#define MINWIDTH 	128			// minimum width and height
#define MINHEIGHT	64			
#define DEFWIDTH 	256			// default width and height
#define DEFHEIGHT	128	

#define CUNTHAIR 0.0000000001		// standard construction worker's measurment

typedef struct gspline
{
	t_pxbox		x_box;
	Patcher		*x_patcher;
	void		*x_qelem;
	
	Rect 		x_rect;			// ui object box's rect
	short		x_boxwidth;
	short		x_boxheight;
	
	GWorldPtr	x_offscreen; 	// global offscreen for all instances
	Rect		x_offrect;		// rect for defining offscreen space
	
	double 		x_sr;
	long		x_mindomain;	// display ranges and related stuff
	long		x_maxdomain;
	float		x_amprange;
	int			x_logamp;
	
	int			x_color; 		// color of object
	RGBColor	x_brgb;
	RGBColor	x_brgbsh;		// shadow for grid lines
	RGBColor	x_frgb;
	
	float x_x[MAXSIZE];			// points (freq, amp) for spline
	float x_y[MAXSIZE];
	float x_y2[MAXSIZE];
	float x_u[MAXSIZE];
	int x_xlen;
	int x_ylen;
	int x_maxlen;
	float x_yp1; 				// first derivative values
	float x_ypn;
	
	float x_xinterp[MAXSIZE];		// interpolate loactions in
	float x_yinterp[MAXSIZE];		// interpolated values out
	Atom x_yiatom[MAXSIZE];			// Atom version of previous for output
	int x_interplen;
	
	int x_autoderiv;
	
	float x_xcentroid;		// spectral and amplitudinal centroids
	float x_ycentroid;

    long  		x_inletnum;
	void 		*x_proxy[2];	// proxy inlets
	
	void		*x_outlet[3];	// outlets
} t_gspline;
	 			


void gspline_dsp(t_gspline *x, t_signal **sp, short *count);

void gspline_qfn(t_gspline *x);
void gspline_redraw(t_gspline *x);
void gspline_update(t_gspline *x);


void gspline_color(t_gspline *x, long color);
void gspline_brgb(t_gspline *x, long r, long g, long b);
void gspline_frgb(t_gspline *x, long r, long g, long b);

void gspline_domain(t_gspline *x, long a, long b);
void gspline_range(t_gspline *x, double f);
void gspline_logamp(t_gspline *x);
void gspline_linamp(t_gspline *x);
void gspline_setautoderiv(t_gspline *x);

void gspline_output(t_gspline *x);
void gspline_centroids(t_gspline *x);
void gspline_bang(t_gspline *x);
void gspline_list(t_gspline *x, Symbol *s, int ac, Atom *av);
void gspline_int(t_gspline *x, long n);
void gspline_float(t_gspline *x, double f);
void gspline_eval(t_gspline *obj);
void gspline_deriv(t_gspline *x, Symbol *s, int ac, Atom *av);
void gspline_autoderiv(t_gspline *x);
float gspline_interp(t_gspline *obj, float x);

void gspline_save(t_gspline *x, void *w);
void gspline_restore(t_gspline *x);
void gspline_assist(t_gspline *x, void *b, long m, long a, char *s);
void gspline_free(t_gspline *x);
void *gspline_real(Patcher *p, short bx, short by, short w, short h);
void *gspline_new(Symbol *s, int ac, Atom *av);
void *gspline_menufun(void *p, long x, long y, long font);

void AllocateOffscreen(t_gspline *x);

void *gspline_class;

Symbol *bindMeBaby; // symbol #X used for restore.


/*==========================================================================*/

void main(void)
{	
	setup(&gspline_class, gspline_new, (method)gspline_free, (short)sizeof(t_gspline), gspline_menufun, A_GIMME, 0);
	addmess((method)gspline_dsp, "dsp", A_CANT, 0);
	addbang((method)gspline_bang);
	addfloat((method)gspline_float);
	addint((method)gspline_int);
    addmess((method)gspline_list, "list", A_GIMME, 0);
    addmess((method)gspline_deriv, "deriv", A_GIMME, 0);
    
	addmess((method)gspline_restore, "done", 	0);
	addmess((method)gspline_color, "color", 	A_DEFLONG, 0);
	addmess((method)gspline_brgb, "brgb", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)gspline_frgb, "frgb", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)gspline_domain, "domain", 	A_DEFLONG, A_DEFLONG, 0);
	addmess((method)gspline_range, "range", 	A_DEFFLOAT, 0);
	addmess((method)gspline_update, "update", 	A_CANT, 0);
	addmess((method)gspline_assist, "assist",	A_CANT, 0);
	
	addmess((method)gspline_setautoderiv, "autoderiv", 0);
	addmess((method)gspline_logamp, "logamp", 0);
	addmess((method)gspline_linamp, "linamp", 0);
	addmess((method)gspline_save, "psave", 	A_CANT, 0);

	bindMeBaby = gensym("#X");			// symbol that we will use for restore
	
	rescopy('STR#',RES_ID);

	dsp_initboxclass();
	
}

/*--------------------------------------------------------------------------*/

void gspline_dsp(t_gspline *x, t_signal **sp, short *count)
{
	x->x_sr = sys_getsr();
	
	x->x_maxdomain = MIN(x->x_maxdomain, x->x_sr);
	
	qelem_set(x->x_qelem);
}



/*==========================================================================*/

void gspline_qfn(t_gspline *x)		// a queue function, redraws the foreground
{
	GrafPort *gp;
	gp = patcher_setport(x->x_box.z_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {		// check if drawing is allowed
			gspline_redraw(x);					// redraw gspline
		}
		SetPort(gp);
	}
}

void gspline_update(t_gspline *x)
{
	short oh,ow,nh,nw;
	
	ow = x->x_boxwidth;
	oh = x->x_boxheight;
	nw = x->x_box.z_box.b_rect.right-x->x_box.z_box.b_rect.left;
	nh = x->x_box.z_box.b_rect.bottom-x->x_box.z_box.b_rect.top;
	
	if(nh != oh || nw != ow) {
		nw = MIN(MAX(nw, MINWIDTH), MAXWIDTH); // constrain to min and max size
		nh = MIN(MAX(nh, MINHEIGHT), MAXHEIGHT);
		box_size(&x->x_box.z_box, nw, nh);
		x->x_boxwidth = nw;
		x->x_boxheight = nh;
	}
	gspline_redraw(x);
}

/*--------------------------------------------------------------------------*/

void gspline_redraw(t_gspline *x) // offscreen drawing method!
{	
	GWorldPtr	myNewGWorld= x->x_offscreen;
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	Rect		r = x->x_box.z_box.b_rect;
	Rect		offr, graphr;
	short		font, size;
	
	RGBColor	myBlack = {0, 0, 0};
	RGBColor	myGrey = {32767, 32767, 32767};
	RGBColor	myWhite = {65535, 65535, 65535};
	RGBColor	frgb = x->x_frgb;
	RGBColor	brgb = x->x_brgb;
	RGBColor	brgbsh = x->x_brgbsh;
	
	double 		sr = x->x_sr;
	short 		w = x->x_boxwidth;
	short 		h = x->x_boxheight;
	short 		gw = w-20;
	short 		gh = h-16;
	
	float arange = x->x_amprange;
	long mind = x->x_mindomain;
	long maxd = x->x_maxdomain;
	float interploc, amp;
	
	unsigned char numstr[12];
	
	int i,j;
	short locw, loch;
	
	Rect	cdot; // centroid oval
	
	offr.left = 0;
	offr.top = 0;
	offr.right = w-1;
	offr.bottom = h-1;
	
	
	graphr.left = 20;
	graphr.top = 0;
	graphr.right = w-1;
	graphr.bottom = gh+1;
	
	// prepare offscreen for drawing
	GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
	LockPixels(GetGWorldPixMap((GWorldPtr) myNewGWorld));
	SetGWorld((CGrafPtr)myNewGWorld, NIL);
	offPort = (GrafPort *)myNewGWorld;
	
	font = myNewGWorld->txFont;
	size = myNewGWorld->txSize;
	
	PenMode(srcCopy);
	// clear display area (draw blank background and frame)
	RGBForeColor(&myWhite);	
	PaintRect(&offr);
	RGBForeColor(&brgb);	
	PaintRect(&graphr);
	
	RGBForeColor(&brgbsh);
	if(x->x_logamp) {
		MoveTo(20,gh/5);
		LineTo(w, gh/5);
		MoveTo(20,gh*3/7);
		LineTo(w, gh*3/7);		
		MoveTo(20,gh*5/8);
		LineTo(w, gh*5/8);
	}else{
		MoveTo(20,gh/2);
		LineTo(w, gh/2);
		MoveTo(20,gh*3/4);
		LineTo(w, gh*3/4);		
		MoveTo(20,gh/4);
		LineTo(w, gh/4);
	}
	 
	RGBForeColor(&myBlack);
	TextFont(4); //monaco
	TextSize(8);
	if(x->x_logamp) {
		MoveTo(3,12);
		DrawString("\p dB");
		MoveTo(3,gh/5);
		DrawString("\p -6");
		MoveTo(3,gh*3/7);
		DrawString("\p-12");
		MoveTo(3,gh*5/8);
		DrawString("\p-24");
	}else{
		MoveTo(3,12);
		DrawString("\p dB");
		MoveTo(3,gh/4);
		DrawString("\p -3");
		MoveTo(3,gh/2);
		DrawString("\p -6");
		MoveTo(3,gh*3/4);
		DrawString("\p-12");
	}
		
	MoveTo(4,h-5);
	DrawString("\pHz:");
	
	MoveTo(20,h-5);
	NumToString(mind, numstr);
	DrawString(numstr);
	
	MoveTo(w-30,h-5);
	NumToString(maxd, numstr);
	DrawString(numstr);
	

	// -------------------------------------------------

	
	if (x->x_maxlen) {
		//partials
		RGBForeColor(&myBlack);	
		for(i=0; i<x->x_maxlen; i++) {
			locw = (short)((x->x_x[i] - (float)mind)/ (float)(maxd-mind) *(float)gw)+20;
			loch = gh - (short)(x->x_y[i] *(float)gh);
			MoveTo(locw, gh);
			LineTo(locw, loch);
		}
		// interp partials (grey)
		if (x->x_interplen) {
			RGBForeColor(&myGrey);	
			for(i=0; i<x->x_interplen; i++) {
				locw = (short)((x->x_xinterp[i] - (float)mind)/ (float)(maxd-mind) *(float)gw)+20;
				loch = gh - (short)(x->x_yinterp[i] *(float)gh);
				MoveTo(locw, gh);
				LineTo(locw, loch);
			}
		}
		//curve
		RGBForeColor(&frgb);		
		MoveTo(20, gh);
		for(i=0; i<gw; i++) {
			
			interploc = ((float)i/gw)*(float)(maxd-mind) + (float)mind;
			amp = gspline_interp(x, interploc);
			amp = MIN(MAX(amp,0), 1);
			locw = i+20;
			loch = gh - (short)(amp * (float)gh);
			
			LineTo(locw, loch);
		}
		//centroid
		locw = (short)((x->x_xcentroid - (float)mind)/ (float)(maxd-mind) *(float)gw)+20;
		loch = gh - (short)(x->x_ycentroid *(float)gh);
		cdot.left = locw-4;
		cdot.top = loch-4;
		cdot.right = locw+4;
		cdot.bottom = loch+4;
		FrameOval(&cdot);
	}
	
	
	
	RGBForeColor(&myBlack);	
	FrameRect(&offr);
	FrameRect(&graphr);
	PenNormal();
	TextFont(font);
	TextSize(size);
	//RGBForeColor(&myBlack); // always set color to black at end
	
	// Then copy the image back on screen and un-prepare offscreen
	SetGWorld((CGrafPtr)oldPort, oldDevice);
	CopyBits(&offPort->portBits, &oldPort->portBits, &offr, &r, srcCopy, NIL);
	UnlockPixels(GetGWorldPixMap((GWorldPtr) myNewGWorld));
	
}

/*--------------------------------------------------------------------------*/


void gspline_color(t_gspline *x, long color)
{
	RGBColor rgb;
	
	color = color%16; // constrains colors to 0-15 zone
	box_getcolor((Box *)x, color,  &rgb);
	
	x->x_color = color;
	gspline_brgb(x, rgb.red>>8, rgb.green>>8, rgb.blue>>8);
}

void gspline_brgb(t_gspline *x, long r, long g, long b)
{
	x->x_brgb.red = MIN(MAX(r<<8,0), 65535);
	x->x_brgb.green = MIN(MAX(g<<8,0), 65535);
	x->x_brgb.blue = MIN(MAX(b<<8,0), 65535);
	
	
	x->x_brgbsh.red = MIN(MAX((r<<8)-8192,0), 65535);
	x->x_brgbsh.green = MIN(MAX((g<<8)-8192,0), 65535);
	x->x_brgbsh.blue = MIN(MAX((b<<8)-8192,0), 65535);
	
	
	x->x_color = 0;

	qelem_set(x->x_qelem);
}

void gspline_frgb(t_gspline *x, long r, long g, long b)
{
	x->x_frgb.red = MIN(MAX(r<<8,0), 65535);
	x->x_frgb.green = MIN(MAX(g<<8,0), 65535);
	x->x_frgb.blue = MIN(MAX(b<<8,0), 65535);
	
	x->x_color = 0;

	qelem_set(x->x_qelem);
}

/*--------------------------------------------------------------------------*/

void gspline_domain(t_gspline *x, long a, long b)
{
	a = MIN(MAX(a, 0), x->x_sr/2);
	b = MIN(MAX(b, 0), x->x_sr/2);
	x->x_mindomain = MIN(a, b);
	x->x_maxdomain = MAX(a, b);
	
	qelem_set(x->x_qelem);
}
void gspline_range(t_gspline *x, double f)
{
	x->x_amprange = MAX(0, f);
	qelem_set(x->x_qelem);
}
void gspline_logamp(t_gspline *x)
{	
	x->x_logamp = 1;
	qelem_set(x->x_qelem);
}
void gspline_linamp(t_gspline *x)
{	
	x->x_logamp = 0;
	qelem_set(x->x_qelem);
}

void gspline_setautoderiv(t_gspline *x)
{	
	x->x_autoderiv = 1;
	if(x->x_maxlen) gspline_eval(x);
	qelem_set(x->x_qelem);
}

/*--------------------------------------------------------------------------*/

void gspline_bang(t_gspline *x)
{	
	
}

void gspline_list(t_gspline *x, Symbol *s, int ac, Atom *av)
{
	int i;
	ac = MIN(ac, MAXSIZE);
	if (x->x_inletnum == 0) {
		for (i=0; i<ac; i++) {
			if (av[i].a_type == A_FLOAT)
				x->x_xinterp[i] = av[i].a_w.w_float;
			else if (av[i].a_type == A_LONG)
				x->x_xinterp[i] = (float)av[i].a_w.w_long;
			else
				x->x_xinterp[i] = 0.0;
		}
		x->x_interplen = ac;
		gspline_output(x);
	}
	if (x->x_inletnum == 1) {
		for (i=0; i<ac; i++) {
			if (av[i].a_type == A_FLOAT)
				x->x_x[i] = av[i].a_w.w_float;
			else if (av[i].a_type == A_LONG)
				x->x_x[i] = (float)av[i].a_w.w_long;
			else
				x->x_x[i] = 0.0;
		}
		x->x_xlen = ac;
		x->x_maxlen = MIN(x->x_xlen, x->x_ylen);
		if (x->x_maxlen < 2) x->x_maxlen = 0;
		if(x->x_maxlen) gspline_eval(x);
	}
	else if (x->x_inletnum == 2) {
		for (i=0; i<ac; i++) {
			if (av[i].a_type == A_FLOAT)
				x->x_y[i] = av[i].a_w.w_float;
			else if (av[i].a_type == A_LONG)
				x->x_y[i] = (float)av[i].a_w.w_long;
			else
				x->x_y[i] = 0.;
		}
		x->x_ylen = ac;
		x->x_maxlen = MIN(x->x_xlen, x->x_ylen);
		if (x->x_maxlen < 2) x->x_maxlen = 0;
		if(x->x_maxlen) gspline_eval(x);
	}
	
	qelem_set(x->x_qelem);
	
}

void gspline_float(t_gspline *x, double f)
{
	if (x->x_inletnum == 0) {
		x->x_xinterp[0] = (float)f;
		x->x_interplen = 1;
		gspline_output(x);
	}
	qelem_set(x->x_qelem);
}
void gspline_int(t_gspline *x, long n)
{
	if (x->x_inletnum == 0) {
		x->x_xinterp[0] = (float)n;
		x->x_interplen = 1;
		gspline_output(x);
	}
	qelem_set(x->x_qelem);
}

void gspline_eval(t_gspline *obj)
{
	float *x = obj->x_x;
	float *y = obj->x_y;
	float *y2 = obj->x_y2;
	float *u = obj->x_u;
	int n = obj->x_maxlen;
	
	float yp1, ypn;

	int i,k;
	float p,qn,sig,un;
	
	gspline_centroids(obj); // needed for autoderiv
	
	if (obj->x_autoderiv) // by default this is on
		gspline_autoderiv(obj);
		
	yp1 = obj->x_yp1; // these should eventually be user-specified
	ypn = obj->x_ypn;

	if (yp1 > 0.99e30)	// The lower boundary condition is set either to be "natural" 
		y2[0]=u[0]=0.0;
	else { 				// or else to have a specified first derivative.
		y2[0] = -0.5;
		u[0]=(3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
	}
	
	// This is the decomposition loop of the tridiagonal al gorithm. 
	// y2 and u are used for tem porary storage of the decomposed factors.
	for (i=1;i<n-1;i++) { 
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	
	if (ypn > 0.99e30)	// The upper boundary condition is set either to be "natural" 
		qn=un=0.0;
	else { 				// or else to have a specified first derivative.
		qn=0.5;
		un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
	}
	
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
	for (k=n-2;k>=0;k--) // This is the backsubstitution loop of the tridiagonal algorithm. 
		y2[k]=y2[k]*y2[k+1]+u[k];

}

float gspline_interp(t_gspline *obj, float x)
{
	float *xa = obj->x_x;
	float *ya = obj->x_y;
	float *y2a = obj->x_y2;
	int n = obj->x_maxlen;
	
	
	int klo,khi,k;
	float h,b,a;
	float val; // result to return

	// We will find the right place in the table by means of bisection.
	klo=1; 
	khi=n;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (xa[k-1] > x) khi=k;
		else klo=k;
	} // klo and khi now bracket the input value of x.

	klo--;
	khi--;
	
	h=xa[khi]-xa[klo];
	if (h == 0.0) //post("Bad xa input to gspline_interp",0); // The xa's must be distinct. 
		val = 0.; //cannot post from redraw so making errors default to 0.
	else {
		a=(xa[khi]-x)/h;
		b=(x-xa[klo])/h; //Cubic spline polynomial is now evaluated.
		val=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
	}
	return (val);
	
}

void gspline_autoderiv(t_gspline *x)
{
	float dx, dy;
	int n = x->x_maxlen-1;
	
	/*
	dx = x->x_x[n] - x->x_x[0];
	x->x_yp1 = x->x_ypn = (x->x_y[n] / dx) - (x->x_y[0] / dx);
	*/
	
	//x->x_yp1 = x->x_ypn = x->x_ycentroid / x->x_xcentroid;

	x->x_yp1 = x->x_ypn = (x->x_y[n] / x->x_xcentroid) - (x->x_y[0] / x->x_xcentroid);
}

void gspline_deriv(t_gspline *x, Symbol *s, int ac, Atom *av)
{
	if (x->x_inletnum == 0) {
		x->x_autoderiv = 0;
		if (ac > 1 && av[1].a_type == A_FLOAT)
			x->x_ypn = av[1].a_w.w_float;
		else if (ac > 1 &&  av[1].a_type == A_LONG)
			x->x_ypn = (float)av[1].a_w.w_long;
		else
			x->x_ypn = 0.0;
			
		if (ac > 0 && av[0].a_type == A_FLOAT)
			x->x_yp1 = av[0].a_w.w_float;
		else if (ac > 0 &&  av[0].a_type == A_LONG)
			x->x_yp1 = (float)av[0].a_w.w_long;
		else
			x->x_yp1 = 0.0;
			
		if(x->x_maxlen) gspline_eval(x);
		qelem_set(x->x_qelem);
	}
}
void gspline_output(t_gspline *x)
{
	int i;
	float val;

	for(i=0; i<x->x_interplen; i++) {
		val = gspline_interp(x, x->x_xinterp[i]);
		x->x_yinterp[i] = val;
		SETFLOAT(&x->x_yiatom[i], val);
	}
	outlet_anything(x->x_outlet[0], gensym("list"), x->x_interplen, x->x_yiatom);
}


void gspline_centroids(t_gspline *x)
{	
	int i;

	float sumxy, sumx, sumy;
	
	sumxy = sumx = sumy = 0.;
	
	for (i = 0; i < x->x_maxlen; i++) {
		sumxy += x->x_x[i] * x->x_y[i];
		sumx += x->x_x[i];
		sumy += x->x_y[i];
	}
	x->x_xcentroid = sumxy / sumy;
	x->x_ycentroid = sumxy / sumx;
	
	outlet_float(x->x_outlet[2], x->x_ycentroid);
	outlet_float(x->x_outlet[1], x->x_xcentroid);
	
}

/*==========================================================================*/

void gspline_save(t_gspline *x, void *w)
{
	Rect *r;
	
	r = &x->x_box.z_box.b_rect;

	if (x->x_box.z_box.b_hidden)
		binbuf_vinsert(w,"ssssllll", gensym("#P"),
									gensym("hidden"),
									gensym("user"),
									gensym("gspline"),
									(long)(r->left),
									(long)(r->top),
									(long)(r->right - r->left), 
									(long)(r->bottom - r->top) );
	else 
		binbuf_vinsert(w,"sssllll",	gensym("#P"),
									gensym("user"), 
									gensym("gspline"),
									(long)(r->left),
									(long)(r->top),
									(long)(r->right - r->left), 
									(long)(r->bottom - r->top) );

	binbuf_vinsert(w,"ssl",bindMeBaby,gensym("color"),
									(long)(x->x_color));
									
	binbuf_vinsert(w,"sslll",bindMeBaby,gensym("brgb"),
									(long)(x->x_brgb.red>>8),
									(long)(x->x_brgb.green>>8),
									(long)(x->x_brgb.blue>>8));
									
	binbuf_vinsert(w,"sslll",bindMeBaby,gensym("frgb"),
									(long)(x->x_frgb.red>>8),
									(long)(x->x_frgb.green>>8),
									(long)(x->x_frgb.blue>>8));
									
	binbuf_vinsert(w,"ssll",bindMeBaby,gensym("domain"),
									(long)(x->x_mindomain),
									(long)(x->x_maxdomain));
	binbuf_vinsert(w,"ssf",bindMeBaby,gensym("range"),
									(float)(x->x_amprange));
									
	if (x->x_logamp)
		binbuf_vinsert(w,"ss",bindMeBaby,gensym("logamp"));
	else
		binbuf_vinsert(w,"ss",bindMeBaby,gensym("linamp"));
		
		
	binbuf_vinsert(w,"ss",bindMeBaby,gensym("done") );
}

void gspline_restore(t_gspline *x)
{
	// to remove the current binding to #X when restore is finished
	bindMeBaby->s_thing = 0;
	qelem_set(x->x_qelem);
}
	
/* ========================================================================== */

void gspline_assist(t_gspline *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,4,s);
}

/*--------------------------------------------------------------------------*/

void gspline_free(t_gspline *x)
{
	if (x->x_offscreen) {
		DisposeGWorld((GWorldPtr) x->x_offscreen);
		//post("offscreen disposed",0);
	}
	qelem_free(x->x_qelem);
	dsp_freebox((t_pxbox *)x);
	box_free((void *)x);
}

/*--------------------------------------------------------------------------*/

void *gspline_real(Patcher *p, short bx, short by, short w, short h)
{
	t_gspline *x;
	short bw, bh;
	
	x = (t_gspline*)newobject(gspline_class);
	x->x_patcher = p;
	
	x->x_proxy[1] = proxy_new(x, 2L, &x->x_inletnum); // right proxy inlet
	x->x_proxy[0] = proxy_new(x, 1L, &x->x_inletnum); // right proxy inlet
	x->x_inletnum = 0;
	
	dsp_setupbox((t_pxbox *)x, 0); // no sig inlets
	x->x_outlet[2] = floatout(x);
	x->x_outlet[1] = floatout(x);
	x->x_outlet[0] = outlet_new(x, "list");
	
	bw = MIN(MAX(w, MINWIDTH), MAXWIDTH); // constrain to min and max size
	bh = MIN(MAX(h, MINHEIGHT), MAXHEIGHT);
	box_new((Box *)x, p, F_DRAWFIRSTIN | F_NODRAWBOX | F_GROWBOTH | F_DRAWINLAST, bx, by, bx+bw, by+bh);
	
	x->x_box.z_box.b_firstin = x;
	x->x_rect = x->x_box.z_box.b_rect;
	x->x_boxwidth = bw;
	x->x_boxheight = bh;
	
	x->x_sr = sys_getsr(); // not really necessary for gspline but nice to have around
	
	x->x_qelem = qelem_new(x, (method)gspline_qfn);

	AllocateOffscreen(x);
	
	gspline_brgb(x, 220, 255, 200);
	gspline_frgb(x, 255, 0, 0);
	gspline_domain(x, 50, 5000);
	x->x_amprange = 1.0;
	x->x_logamp = 0;
	
	x->x_yp1 = 0.;
	x->x_ypn = 0.;
	x-> x_autoderiv = 1; // by default this is on
	
	x->x_xlen = x->x_ylen = x->x_maxlen = x->x_interplen = 0;
	bindMeBaby->s_thing = (Object *)x;	// to restore saved state
	
	box_ready((Box *)x);
	return(x);
}

/*--------------------------------------------------------------------------*/

void *gspline_new(Symbol *s, int ac, Atom *av)
{
	void *p, *res;
	
	p = av[0].a_w.w_obj;
	av++;
	ac--;
	res = patchbox(p, gspline_real, ac, av, DEFWIDTH);
	
	return (res);
}

/*--------------------------------------------------------------------------*/

void *gspline_menufun(void *p, long x, long y, long font)
{
	return(gspline_real(p, x, y, DEFWIDTH, DEFHEIGHT));
}

/* ========================================================================== */

// will probably need to receive pointer to object instance to resize offscreen???
// no! I decided to set a fixed maximum display size, instead.

void AllocateOffscreen(t_gspline *x) 
{
	OSErr		err;
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	
	x->x_offrect.left = 0;
	x->x_offrect.top = 0;
	x->x_offrect.right = MAXWIDTH;
	x->x_offrect.bottom = MAXHEIGHT;
	
	// create 8-bit color offscreen
	err = NewGWorld(&x->x_offscreen, 8, &x->x_offrect, NIL, NIL, 0);
	if (err)
		post("gspline: error %ld creating offscreen", err);
	else {
	
		// prepare offscreen for drawing
		GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
		LockPixels(GetGWorldPixMap((GWorldPtr) x->x_offscreen));
		SetGWorld((CGrafPtr)x->x_offscreen, NIL);
		
		// initilaize canvas
		offPort = (GrafPort *)x->x_offscreen;
		EraseRect(&offPort->portRect);
		
		// un-prepare offscreen
		UnlockPixels(GetGWorldPixMap((GWorldPtr) x->x_offscreen));
		SetGWorld((CGrafPtr)oldPort, oldDevice);	
		//post("offscreen created",0);
	}
}

