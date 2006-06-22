/*
DESCRIPTION: Ancient spectral display object by Richard Dudas, superceded by objects now bundled with MSP
*/

// dugger - a display object for fugger
// Display Used for Gibson Guitar Effect Realisation

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

#define RES_ID		12346

#define MAXWIDTH 	1024		// maximum width and height of user object in pixels
#define MAXHEIGHT	512			//    (defines maximum offscreen canvas allocation)
#define MINWIDTH 	128			// minimum width and height
#define MINHEIGHT	64			
#define DEFWIDTH 	256			// default width and height
#define DEFHEIGHT	128	

#define CUNTHAIR 0.0000000001	// standard construction worker's measurment

typedef struct dugger
{
	t_pxbox		x_box;
	Patcher		*x_patcher;
	//Rect 		x_rect;
	short		x_boxwidth;		// these are currently fixed at BOXWIDTH, BOXHEIGHT
	short		x_boxheight;
	
	GWorldPtr	x_offscreen; 	// global offscreen for all instances
	Rect		x_offrect;		// rect for defining offscreen space

	void		*x_qelem;
	
	double 		x_sr;
	int			x_logamp;		// log amp scale?
	int			x_pointmode; 	// draw points or lines instead of bars?
	long		x_mindomain;
	long		x_maxdomain;
	float		x_amprange;
	
	int			x_color; 		// color of object
	RGBColor	x_brgb;
	RGBColor	x_brgbsh;		// shadow for grid lines
	RGBColor	x_frgb;
	
	int x_fuggerReceived;		// should we redraw the whole fft graph?
	int x_suggerReceived;		// should we redraw the whole signal graph?
	int x_clearDisplay;			// should we redraw an empty graph?
	
	long  		x_fuggerbufsize;
	t_float		*x_fuggerbuf;

	void		*x_outlet;	// outlet.
} t_dugger;
	 			
Symbol *ps_fuggerconnect;

void dugger_dsp(t_dugger *x, t_signal **sp, short *count);

void dugger_qfn(t_dugger *x);
void dugger_redraw(t_dugger *x);
void dugger_update(t_dugger *x);

void dugger_color(t_dugger *x, long color);
void dugger_brgb(t_dugger *x, long r, long g, long b);
void dugger_frgb(t_dugger *x, long r, long g, long b);

void dugger_fuggerlist(t_dugger *x, Symbol *s, int ac, Atom *av);
void dugger_suggerlist(t_dugger *x, Symbol *s, int ac, Atom *av);

void dugger_output(t_dugger *x);
void dugger_bang(t_dugger *x);
void dugger_domain(t_dugger *x, long a, long b);
void dugger_range(t_dugger *x, double f);
void dugger_logamp(t_dugger *x);
void dugger_linamp(t_dugger *x);

void dugger_pointmode(t_dugger *x);
void dugger_barmode(t_dugger *x);
void dugger_linemode(t_dugger *x);

void dugger_save(t_dugger *x, void *w);
void dugger_restore(t_dugger *x);
void dugger_assist(t_dugger *x, void *b, long m, long a, char *s);
void dugger_free(t_dugger *x);
void *dugger_real(Patcher *p, short bx, short by, short w, short h);
void *dugger_new(Symbol *s, int ac, Atom *av);
void *dugger_menufun(void *p, long x, long y, long font);

void AllocateOffscreen(t_dugger *x);

void *dugger_class;

Symbol *bindMeBaby; // symbol #X used for restore.


/*==========================================================================*/

void main(void)
{	
	setup(&dugger_class, dugger_new, (method)dugger_free, (short)sizeof(t_dugger), dugger_menufun, A_GIMME, 0);
	addmess((method)dugger_dsp, "dsp", A_CANT, 0);
	addmess((method)dugger_fuggerlist, "fuggerlist", A_GIMME, 0);
	addmess((method)dugger_suggerlist, "suggerlist", A_GIMME, 0);
	addbang((method)dugger_bang);
	addmess((method)dugger_restore, "done", 	0);
	addmess((method)dugger_color, "color", 	A_DEFLONG, 0);
	addmess((method)dugger_brgb, "brgb", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)dugger_frgb, "frgb", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)dugger_domain, "domain", 	A_DEFLONG, A_DEFLONG, 0);
	addmess((method)dugger_range, "range", 	A_DEFFLOAT, 0);
	addmess((method)dugger_update, "update", 	A_CANT, 0);
	addmess((method)dugger_assist, "assist",	A_CANT, 0);
	
	addmess((method)dugger_pointmode, "pointmode", 0);
	addmess((method)dugger_barmode, "barmode", 0);
	addmess((method)dugger_linemode, "linemode", 0);
	
	addmess((method)dugger_logamp, "logamp", 0);
	addmess((method)dugger_linamp, "linamp", 0);
	addmess((method)dugger_save, "psave", 	A_CANT, 0);

	bindMeBaby = gensym("#X");			// symbol that we will use for restore
	
	rescopy('STR#',RES_ID);

	dsp_initboxclass();
	
}

/*--------------------------------------------------------------------------*/

void dugger_dsp(t_dugger *x, t_signal **sp, short *count)
{
	x->x_sr = sys_getsr();
	
	x->x_maxdomain = MIN(x->x_maxdomain, x->x_sr);
	
	qelem_set(x->x_qelem);
}

/*==========================================================================*/

void dugger_qfn(t_dugger *x)		// a queue function, redraws the foreground
{
	GrafPort *gp;
	gp = patcher_setport(x->x_box.z_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {		// check if drawing is allowed
			dugger_redraw(x);					// redraw dugger
		}
		SetPort(gp);
	}
}

void dugger_update(t_dugger *x)
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
		
		x->x_clearDisplay = 1; // if size has changed paint a blank graph
		
		// reallocate newly sized offscreen
		DisposeGWorld((GWorldPtr) x->x_offscreen);
		AllocateOffscreen(x);
	}
	x->x_fuggerReceived = 0; // otherwise just update what's already drawn on the offscreen
	x->x_suggerReceived = 0; // otherwise just update what's already drawn on the offscreen
	dugger_redraw(x);
}

/*--------------------------------------------------------------------------*/

void dugger_redraw(t_dugger *x) // offscreen drawing method!
{	
	GWorldPtr	offscreen= x->x_offscreen;
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	Rect		r = x->x_box.z_box.b_rect;
	Rect		offr, graphr;
	short		font, size;
	
	RGBColor	myBlack = {0, 0, 0};
	RGBColor	myWhite = {65535, 65535, 65535};
	RGBColor	frgb = x->x_frgb;
	RGBColor	brgb = x->x_brgb;
	RGBColor	brgbsh = x->x_brgbsh;
	
	double 		sr = x->x_sr;
	short 		w = x->x_boxwidth;
	short 		h = x->x_boxheight;
	short 		gw = w-20;
	short 		gh = h-16;
	
	int 		pointmode = x->x_pointmode;
	
	float arange = 1./x->x_amprange;
	
	long fftsize = x->x_fuggerbufsize/2; // realfft = demiframe+1 real & demiframe-1 imag
	long sugsize = x->x_fuggerbufsize;
	float mult = 1./(float)fftsize;
	
	long mind = x->x_mindomain/(sr/2)*fftsize;
	long maxd = x->x_maxdomain/(sr/2)*fftsize;
	
	unsigned char numstr[12];
	
	float spacing = fftsize/(maxd-mind);
	
	int i,j;
	float re, im, amp;
	short locw, loch;
	
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
	SetGWorld((CGrafPtr)offscreen, NIL);
	offPort = (GrafPort *)offscreen;
	
	font = offscreen->txFont;
	size = offscreen->txSize;
	
	PenMode(srcCopy);
	if(x->x_clearDisplay) {
		// draw blank background and frame
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
		NumToString(x->x_mindomain, numstr);
		DrawString(numstr);
		
		MoveTo(w-30,h-5);
		NumToString(x->x_maxdomain, numstr);
		DrawString(numstr);
		
		x->x_clearDisplay = 0;
	}
		
	// draw whatever it is that needs to be drawn if fugger is attached
	if(x->x_fuggerReceived) {
	
		// draw background and frame
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
		NumToString(x->x_mindomain, numstr);
		DrawString(numstr);
		
		MoveTo(w-30,h-5);
		NumToString(x->x_maxdomain, numstr);
		DrawString(numstr);
		
		RGBForeColor(&frgb);

		if(x->x_logamp) {
			/*re = x->x_fuggerbuf[0]*mult;
			amp = re*re;
			if(amp < CUNTHAIR)
				amp = CUNTHAIR;
			amp = (10*log10(amp))/100.+ 1.;
			
			locw = 20;
			loch = gh - (short)((float)amp * (float)gh);
			
			MoveTo(locw, gh);
			LineTo(locw, loch);*/
			
			MoveTo(20, gh);
			for(i=mind; i<maxd; i++) { 
				re = x->x_fuggerbuf[i]*mult;
				im = x->x_fuggerbuf[i+fftsize]*mult;
				amp = re*re+im*im;
				if(amp < CUNTHAIR)
					amp = CUNTHAIR;
				amp = ((10*log10(amp))/100.+ 1.)*arange;
				locw = (short)((float)(i-mind)/(float)(maxd-mind) * (float)gw)+20;
				loch = gh - (short)((float)amp * (float)gh);
				
				if (pointmode == 1)
					MoveTo(locw, loch);
				else if (pointmode == 0)
					MoveTo(locw, gh);
				LineTo(locw, loch);

			}
			
			/*re = x->x_fuggerbuf[fftsize]*mult;
			amp = re*re;
			if(amp < CUNTHAIR)
				amp = CUNTHAIR;
			amp = (10*log10(amp))/100.+ 1.;
			locw = gw+20;
			loch = gh - (short)((float)amp * (float)gh);
			
			MoveTo(locw, gh);
			LineTo(locw, loch);*/
		}
		else {
			/*re = x->x_fuggerbuf[0]*mult;
			amp = sqrt(re*re);
			locw = 20;
			loch = gh - (short)((float)amp * (float)gh);
			
			MoveTo(locw, gh);
			LineTo(locw, loch);*/
				
				
			MoveTo(20, gh);
			for(i=mind; i<maxd; i++) {
				re = x->x_fuggerbuf[i]*mult;
				im = x->x_fuggerbuf[i+fftsize]*mult;
				amp = sqrt(re*re+im*im) * arange;
				locw = (short)((float)(i-mind)/(float)(maxd-mind) * (float)gw)+20;
				loch = gh - (short)((float)amp * (float)gh);
				
				if (pointmode == 1)
					MoveTo(locw, loch);
				else if (pointmode == 0)
					MoveTo(locw, gh);
				LineTo(locw, loch);
			}
			
			/*re = x->x_fuggerbuf[fftsize]*mult;
			amp = sqrt(re*re);
			locw = gw+20;
			loch = gh - (short)((float)amp * (float)gh);
			
			MoveTo(locw, gh);
			LineTo(locw, loch);*/
		}
	}
	
	if(x->x_suggerReceived) {
	
		// draw background and frame
		RGBForeColor(&myWhite);	
		PaintRect(&offr);
		RGBForeColor(&brgb);	
		PaintRect(&graphr);
	
		RGBForeColor(&brgbsh);
		MoveTo(20,gh/2);
		LineTo(w, gh/2);
		MoveTo(20,gh*3/4);
		LineTo(w, gh*3/4);		
		MoveTo(20,gh/4);
		LineTo(w, gh/4);
		 
		RGBForeColor(&myBlack);
		TextFont(4); //monaco
		TextSize(8);

		MoveTo(3,12);
		DrawString("\p  1");
		MoveTo(3,gh/4);
		DrawString("\p .5");
		MoveTo(3,gh/2);
		DrawString("\p  0");
		MoveTo(3,gh*3/4);
		DrawString("\p-.5");
		
		
		MoveTo(4,h-5);
		DrawString("\pms:");
		
		MoveTo(20,h-5);
		NumToString(0, numstr);
		DrawString(numstr);
		
		MoveTo(w-30,h-5);
		NumToString((int)(sugsize*1000/sr+0.5), numstr);
		DrawString(numstr);
		
		RGBForeColor(&frgb);	
				
		MoveTo(20, gh/2);
		for(i=0; i<sugsize; i++) {
			amp = x->x_fuggerbuf[i] * arange;
			locw = (short)((float)(i)/(float)(sugsize) * (float)gw)+20;
			loch = gh/2 - (short)((float)amp * (float)gh/2);
			
			if (pointmode == 1)
				MoveTo(locw, loch);
			else if (pointmode == 0)
				MoveTo(locw, gh/2);
			LineTo(locw, loch);
		}
		
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
	
}

/*--------------------------------------------------------------------------*/


void dugger_color(t_dugger *x, long color)
{
	RGBColor rgb;
	
	color = color%16; // constrains colors to 0-15 zone
	box_getcolor((Box *)x, color,  &rgb);
	
	x->x_color = color;
	dugger_brgb(x, rgb.red>>8, rgb.green>>8, rgb.blue>>8);
}

void dugger_brgb(t_dugger *x, long r, long g, long b)
{
	x->x_brgb.red = MIN(MAX(r<<8,0), 65535);
	x->x_brgb.green = MIN(MAX(g<<8,0), 65535);
	x->x_brgb.blue = MIN(MAX(b<<8,0), 65535);
	
	x->x_brgbsh.red = MIN(MAX((r<<8)-8192,0), 65535);
	x->x_brgbsh.green = MIN(MAX((g<<8)-8192,0), 65535);
	x->x_brgbsh.blue = MIN(MAX((b<<8)-8192,0), 65535);
	
	x->x_color = 0;
	x->x_fuggerReceived = 0;
	x->x_suggerReceived = 0;
	x->x_clearDisplay = 1;
	qelem_set(x->x_qelem);
}

void dugger_frgb(t_dugger *x, long r, long g, long b)
{
	x->x_frgb.red = MIN(MAX(r<<8,0), 65535);
	x->x_frgb.green = MIN(MAX(g<<8,0), 65535);
	x->x_frgb.blue = MIN(MAX(b<<8,0), 65535);
	
	x->x_color = 0;
	x->x_fuggerReceived = 0;
	x->x_suggerReceived = 0;
	x->x_clearDisplay = 1;
	qelem_set(x->x_qelem);
}

/*--------------------------------------------------------------------------*/

void dugger_bang(t_dugger *x)
{	
	if(x->x_fuggerReceived) {
		dugger_output(x);
		qelem_set(x->x_qelem);
	}
}

void dugger_domain(t_dugger *x, long a, long b)
{
	a = MIN(MAX(a, 0), x->x_sr/2);
	b = MIN(MAX(b, 0), x->x_sr/2);
	x->x_mindomain = MIN(a, b);
	x->x_maxdomain = MAX(a, b);
}

void dugger_range(t_dugger *x, double f)
{
	x->x_amprange = MAX(0, f);
}

void dugger_logamp(t_dugger *x)
{	
	x->x_logamp = 1;
}

void dugger_linamp(t_dugger *x)
{	
	x->x_logamp = 0;
}

void dugger_pointmode(t_dugger *x)
{	
	x->x_pointmode = 1;
}

void dugger_barmode(t_dugger *x)
{	
	x->x_pointmode = 0;
}

void dugger_linemode(t_dugger *x)
{	
	x->x_pointmode = 2;
}

void dugger_fuggerlist(t_dugger *x, Symbol *s, int ac, Atom *av)
{
	// just some rudimentary verification that it's a real fuggerlist
	if(ac >= 2) {
		if (av[0].a_type == A_LONG && av[1].a_type == A_LONG) {
			x->x_fuggerbufsize = av[0].a_w.w_long;
			//post("size = %ld",x->x_fuggerbufsize);

			(float *)x->x_fuggerbuf = (float *)av[1].a_w.w_long;
			//post("buf val: %f",x->x_fuggerbuf[0]);
			x->x_fuggerReceived = 1;
		}
		else x->x_fuggerReceived = 0;
	}
	else {
		x->x_fuggerReceived = 0;
	}
	qelem_set(x->x_qelem);
}

void dugger_suggerlist(t_dugger *x, Symbol *s, int ac, Atom *av)
{
	// just some rudimentary verification that it's a real fuggerlist
	if(ac >= 2) {
		if (av[0].a_type == A_LONG && av[1].a_type == A_LONG) {
			x->x_fuggerbufsize = av[0].a_w.w_long;
			//post("size = %ld",x->x_fuggerbufsize);

			(float *)x->x_fuggerbuf = (float *)av[1].a_w.w_long;
			//post("buf val: %f",x->x_fuggerbuf[0]);
			x->x_suggerReceived = 1;
		}
		else x->x_suggerReceived = 0;
	}
	else {
		x->x_suggerReceived = 0;
	}
	qelem_set(x->x_qelem);
}

void dugger_output(t_dugger *x)
{
	Atom at[256];
	long temp;
	int i;
	float re, im, amp;
	
	temp = x->x_fuggerbufsize/2;
	if (temp>256) temp = 256;
	
	/*for(i=0; i<temp; i++) {
		re = x->x_fuggerbuf->buffer[i];
		im = x->x_fuggerbuf->buffer[i+temp];
		amp = sqrt(re*re+im*im);
		SETFLOAT(&at[i], amp);
	}*/
	
	for(i=0; i<temp; i++) {
		re = x->x_fuggerbuf[i];
		//im = x->x_fuggerbuf->buffer[i+temp];
		//amp = sqrt(re*re+im*im);
		SETFLOAT(&at[i], re);
	}
	
	outlet_anything(x->x_outlet, gensym("list"), temp, at);
}

/*==========================================================================*/

void dugger_save(t_dugger *x, void *w)
{
	Rect *r;
	
	r = &x->x_box.z_box.b_rect;

	if (x->x_box.z_box.b_hidden)
		binbuf_vinsert(w,"ssssllll", gensym("#P"),
									gensym("hidden"),
									gensym("user"),
									gensym("dugger"),
									(long)(r->left),
									(long)(r->top),
									(long)(r->right - r->left), 
									(long)(r->bottom - r->top) );
	else 
		binbuf_vinsert(w,"sssllll",	gensym("#P"),
									gensym("user"), 
									gensym("dugger"),
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
		
	if (x->x_pointmode == 2)
		binbuf_vinsert(w,"ss",bindMeBaby,gensym("linemode"));
	else if (x->x_pointmode == 1)
		binbuf_vinsert(w,"ss",bindMeBaby,gensym("pointmode"));
	else
		binbuf_vinsert(w,"ss",bindMeBaby,gensym("barmode"));
		
	binbuf_vinsert(w,"ss",bindMeBaby,gensym("done") );
}

void dugger_restore(t_dugger *x)
{
	// to remove the current binding to #X when restore is finished
	bindMeBaby->s_thing = 0;
	qelem_set(x->x_qelem);
}
	
/* ========================================================================== */

void dugger_assist(t_dugger *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

/*--------------------------------------------------------------------------*/

void dugger_free(t_dugger *x)
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

void *dugger_real(Patcher *p, short bx, short by, short w, short h)
{
	t_dugger *x;
	short bw, bh;
	
	x = (t_dugger*)newobject(dugger_class);
	
	dsp_setupbox((t_pxbox *)x, 0); // default inlet
	x->x_outlet = outlet_new(x, "list");
	
	x->x_patcher = p;
	
	bw = MIN(MAX(w, MINWIDTH), MAXWIDTH); // constrain to min and max size
	bh = MIN(MAX(h, MINHEIGHT), MAXHEIGHT);
	box_new((Box *)x, p, F_DRAWFIRSTIN | F_NODRAWBOX | F_GROWBOTH | F_DRAWINLAST, bx, by, bx+bw, by+bh);
	
	x->x_box.z_box.b_firstin = x;
	//x->x_rect = x->x_box.z_box.b_rect;
	x->x_boxwidth = bw;
	x->x_boxheight = bh;
	
	x->x_sr = sys_getsr();
	
	x->x_qelem = qelem_new(x, (method)dugger_qfn);

	bindMeBaby->s_thing = (Object *)x;	// to restore saved state
	
	
	AllocateOffscreen(x);

	dugger_brgb(x, 220, 255, 200);
	dugger_frgb(x, 255, 0, 0);
	dugger_domain(x, 0, x->x_sr/2);
	x->x_amprange = 1.0;
	x->x_logamp = 0;
	x->x_pointmode = 0;
	
	x->x_clearDisplay = 1;
	x->x_fuggerReceived = 0;
	x->x_suggerReceived = 0;
	
	box_ready((Box *)x);
	return(x);
}

/*--------------------------------------------------------------------------*/

void *dugger_new(Symbol *s, int ac, Atom *av)
{
	void *p, *res;
	
	p = av[0].a_w.w_obj;
	av++;
	ac--;
	res = patchbox(p, dugger_real, ac, av, DEFWIDTH);
	
	return (res);
}

/*--------------------------------------------------------------------------*/

void *dugger_menufun(void *p, long x, long y, long font)
{
	return(dugger_real(p, x, y, DEFWIDTH, DEFHEIGHT));
}

/* ========================================================================== */

void AllocateOffscreen(t_dugger *x) 
{
	OSErr		err;
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	
	x->x_offrect = x->x_box.z_box.b_rect;
	OffsetRect(&x->x_offrect, -x->x_offrect.left, -x->x_offrect.top);
	
	// create 8-bit color offscreen
	err = NewGWorld(&x->x_offscreen, 8, &x->x_offrect, NIL, NIL, 0);
	if (err)
		post("dugger: error %ld creating offscreen", err);
	else {
	
		// prepare offscreen for drawing
		GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
		SetGWorld((CGrafPtr)x->x_offscreen, NIL);
		
		// initilaize canvas
		offPort = (GrafPort *)x->x_offscreen;
		EraseRect(&offPort->portRect);
		
		// un-prepare offscreen
		SetGWorld((CGrafPtr)oldPort, oldDevice);	
		//post("offscreen created",0);
	}
}

