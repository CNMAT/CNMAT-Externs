// spectrogram user interface object
// ©1998, 1999 Richard Dudas, CNMAT

/* to do:

make re-sizable (larger offscreen allocated)
set display range along freq axis
add rgb color
make amplitude range display more coherent (log or db option?)

*/

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

#define RES_ID		16661
#define R_PI		3.14159265358979323846
#define R_TWOPI		6.28318530717958647692

#define BOXWIDTH 	256			// width of user object in pixels
#define BOXHEIGHT	128			// height of user object in pixels

typedef struct spectrogram
{
	t_pxbox		f_box;
	Patcher		*f_patcher;
	Rect 		f_rect;
	void		*f_qelem;
	void 		*f_klok;
	int 		f_pronto; // is an fft buffer ready for displaying
	long 		f_timegrain;
	
	float		f_sr; //sampling rate
	
	
	int			f_logscale; // is it a log scale
	int			f_phasespect; // display the phase spectrum
	float		f_range; // range of the amplitude spectrum display
	
	long f_countdown;	// vector count
	float *f_realinptr;	// input fill ptr into realin
	float *f_imaginptr;	// input fill ptr into imagin
	float *f_realoutptr;	// output ptr into realout
	float *f_imagoutptr;	// output ptr into imagout
	float *f_realin;	// where transform is done
	float *f_imagin;	// where transform is done
	float *f_realout;	// where transform is done
	float *f_imagout;	// where transform is done
	long f_points;		// size
	float f_1overpts; // fft scaling
	long f_phase;		// phase of FFT start (relative to global counter)
	long f_interval;	// sample count before doing another FFT
	long f_start;		// flag that allows phase to be calculated without a global counter

	RGBColor	f_color;	// each instance can have its own color
	RGBColor	f_shadow;	// each instance can have its own color
	RGBColor	f_lines;	// each instance can have its own color

} t_spectrogram;
	 			

void spectrogram_dsp(t_spectrogram *x, t_signal **sp, short *count);
t_int *spectrogram_perform(t_int *w);

void spectrogram_qfn(t_spectrogram *x);
void spectrogram_tick(t_spectrogram *x);
void spectrogram_redraw(t_spectrogram *x);
void spectrogram_update(t_spectrogram *x);
void spectrogram_newcolor(t_spectrogram *x, long r, long g, long b);

void spectrogram_bang(t_spectrogram *x);
void spectrogram_log(t_spectrogram *x);
void spectrogram_lin(t_spectrogram *x);
void spectrogram_phase(t_spectrogram *x);
void spectrogram_amp(t_spectrogram *x);
void spectrogram_range(t_spectrogram *x, double f);


void spectrogram_save(t_spectrogram *x, void *w);
void spectrogram_restore(t_spectrogram *x);
void spectrogram_assist(t_spectrogram *x, void *b, long m, long a, char *s);
void spectrogram_free(t_spectrogram *x);
void *spectrogram_real(Patcher *p, int cx, int cy);
void *spectrogram_new(Symbol *s, int ac, Atom *av);
void *spectrogram_menufun(void *p, long x, long y, long font);

void MyCreateGWorld(void);

void *spectrogram_class;
long spectrogram_number_of_instances; // keep track of number of instances for offscreen
Symbol *bindMeBaby; // symbol #X used for restore.

GWorldPtr	gMyNewGWorld; 	// global offscreen for all instances
Rect		gOffRect;		// rect for defining offscreen space

/*==========================================================================*/

void main(void)
{	
	setup(&spectrogram_class, spectrogram_new, (method)spectrogram_free, (short)sizeof(t_spectrogram), spectrogram_menufun, A_GIMME, 0);
	addmess((method)spectrogram_dsp, "dsp", A_CANT, 0);

	addbang((method)spectrogram_bang);
	addmess((method)spectrogram_log, "log", 	0);
	addmess((method)spectrogram_lin, "lin", 	0);
	addmess((method)spectrogram_phase, "phase", 	0);
	addmess((method)spectrogram_amp, "amp", 	0);
	addmess((method)spectrogram_range, "range", A_FLOAT, 0);

	
	addmess((method)spectrogram_restore, "done", 	0);
	addmess((method)spectrogram_newcolor, "color", 	A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);
	addmess((method)spectrogram_update, "update", 	A_CANT, 0);
	addmess((method)spectrogram_assist, "assist",	A_CANT, 0);
	addmess((method)spectrogram_save, "psave", 	A_CANT, 0);
	bindMeBaby = gensym("#X");			/* symbol that we will use for restore */
	
	rescopy('STR#',RES_ID);
	gMyNewGWorld = NIL;
	spectrogram_number_of_instances = 0; 
	dsp_initboxclass();
	
}

/*--------------------------------------------------------------------------*/

void spectrogram_dsp(t_spectrogram *x, t_signal **sp, short *count)
{
	x->f_sr = sys_getsr();
	// need to calculate phase
	x->f_start = 1;

	x->f_countdown = 0;
	x->f_realinptr = x->f_realin;
	x->f_imaginptr = x->f_imagin;

	if (x->f_phase % sys_getblksize())
		error("spectrogram: phase (%ld) must be multiple of %ld",x->f_phase,sys_getblksize());
	else {
		if (count[0] && count[1])
			dsp_add(spectrogram_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
	}
	clock_delay(x->f_klok, 0L);
	qelem_set(x->f_qelem);
}

t_int *spectrogram_perform(t_int *w)
{
	t_spectrogram *x = (t_spectrogram *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *inimag = (float *)(w[3]);
	int n = w[4];
	
	int m,np,count;
	long pts = x->f_points;
	long interval = x->f_interval;
	float *b, *ei = x->f_realin + pts, *eo = x->f_realout + pts;	// just do the real buf as the test
	
	if (x->f_box.z_disabled)
		goto out;
	if (n > pts) {
		np = pts;
		count = n / pts;
	} else {
		np = n;
		count = 1;
	}
	m = np;
	while (count--) {

		if (x->f_start) {
			long phase = x->f_phase % interval;
			if (phase < pts) {
				x->f_realoutptr = x->f_realout + phase;
				x->f_imagoutptr = x->f_imagout + phase;
			} else {
				x->f_realoutptr = x->f_realout + pts;
				x->f_imagoutptr = x->f_imagout + pts;
			}
			if (pts + phase >= interval) {
				x->f_countdown = 0;
				x->f_realinptr = x->f_realin + (pts + phase - interval);
				x->f_imaginptr = x->f_imagin + (pts + phase - interval);
			} else {
				x->f_countdown = (interval - phase - pts) / np;
				x->f_realinptr = x->f_realin;
				x->f_imaginptr = x->f_imagin;
			}
			x->f_start = 0;
		}

		if (x->f_countdown) {
			x->f_countdown--;
			b = 0;
		} else {
			b = x->f_realinptr;
			BlockMove(inreal,b, np * sizeof(float));
			BlockMove(inimag,x->f_imaginptr, np * sizeof(float));
			b += np;
		}
		/*if (x->f_realoutptr != eo) {
			double q = (int)(x->f_realoutptr - x->f_realout);
			BlockMove(x->f_realoutptr,outreal,np * sizeof(float));
			BlockMove(x->f_imagoutptr,outimag,np * sizeof(float));
			x->f_realoutptr += np;
			x->f_imagoutptr += np;
		}*/
		
		if (b == ei) {
			// this step can be eliminated if signals were unique
			BlockMove(x->f_realin,x->f_realout,pts * sizeof(float));
			BlockMove(x->f_imagin,x->f_imagout,pts * sizeof(float));
		
			//instead of an fft ask for the redraw!
			// can't set a qelem from inside dsp perform!
			//qelem_set(x->f_qelem);
			x->f_pronto = 1;
			
			// "pts" is no longer valid at this point
			x->f_realoutptr = x->f_realout;
			x->f_imagoutptr = x->f_imagout;
			x->f_realinptr = x->f_realin;
			x->f_imaginptr = x->f_imagin;
			x->f_countdown = (interval - x->f_points) / np;

		} else if (b) {
			x->f_realinptr += np;
			x->f_imaginptr += np;
		}
	}
out:
	return (w + 5);
}



/*==========================================================================*/

void spectrogram_qfn(t_spectrogram *x)		// a queue function, redraws the foreground
{
	GrafPort *gp;
	gp = patcher_setport(x->f_box.z_box.b_patcher);	// set grafport to patcher

	if (gp) {
		if (!box_nodraw((void *)x)) {				// check if drawing is allowed
			spectrogram_redraw(x);						// redraw spectrogram
		}
		SetPort(gp);
	}
}

void spectrogram_update(t_spectrogram *x)
{
	/*short oh,ow,nh,nw;
	
	ow = x->f_boxwidth;
	oh = x->f_boxheight;
	nw = x->f_box.z_box.b_rect.right-x->f_box.z_box.b_rect.left;
	nh = x->f_box.z_box.b_rect.bottom-x->f_box.z_box.b_rect.top;

	if(nh != oh || nw != ow) {
		nw = MIN(MAX(nw, MINWIDTH), MAXWIDTH); // constrain to min and max size
		nh = MIN(MAX(nh, MINHEIGHT), MAXHEIGHT);
		box_size(&x->f_box.z_box, nw, nh);
		x->f_boxwidth = nw;
		x->f_boxheight = nh;
	}*/
	spectrogram_redraw(x);
}

void spectrogram_tick(t_spectrogram *x)
{	
	long n;
	
	n = sys_getdspstate();
	if (n && x->f_timegrain)
		clock_delay(x->f_klok, (long)x->f_timegrain);
	else
		clock_unset(x->f_klok);
		
	if (x->f_pronto)
		spectrogram_bang(x);
}

/*--------------------------------------------------------------------------*/

void spectrogram_redraw(t_spectrogram *x) // offscreen drawing method!
{	
	GWorldPtr	myNewGWorld;
	GrafPtr		gp, oldPort, offPort;
	GDHandle	oldDevice;
	Rect		r, offr;
	RGBColor	myColor, myShadow, myLines;
	RGBColor	myBlack = {0, 0, 0};
	RGBColor	myGrey = {49151, 49151, 49151}; // 3/4 white
	RGBColor	myRed = {65535, 0, 0};
	
	RgnHandle	myFiltRgn;
	
	int i;
	
	double temp;
	double x0, x1, x2, y1, y2, A, B, C, D, numR, numI, den, R, I, amp;
	long iamp;
	
	int logscale = x->f_logscale;
	int phasespect = x->f_phasespect;
	float amprange = x->f_range;
	
	float sr = x->f_sr;
	float sro2 = sr/2.;
	float mult = x->f_1overpts;

	myNewGWorld = gMyNewGWorld;
	myColor =  x->f_color;
	myShadow = x->f_shadow;
	myLines = x->f_lines;
	
	r = x->f_box.z_box.b_rect;
	 
	offr.left = 0;
	offr.top = 0;
	offr.right = BOXWIDTH;
	offr.bottom = BOXHEIGHT;
	
	// prepare offscreen for drawing
	GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
	LockPixels(GetGWorldPixMap((GWorldPtr) myNewGWorld));
	SetGWorld((CGrafPtr)myNewGWorld, NIL);
	offPort = (GrafPort *)myNewGWorld;
	
	myFiltRgn = NewRgn();	// get handle to empty region 
	
	PenMode(srcCopy);
	
	RGBForeColor(&x->f_color);	
	PaintRect(&offr);

	
	OpenRgn();	// turn on the Region recorder

	
	MoveTo(0, BOXHEIGHT);
	
	for(i=0; i<BOXWIDTH; i++) {
	
		// right now averaging, should search for peak
		// scaling using 1overpts
		R = (x->f_realout[i*4]*x->f_realout[i*4+2])/2. * mult;
		I = (x->f_imagout[i*4]*x->f_imagout[i*4+2])/2. * mult;
		
		if (phasespect) {
			amp = atan2(I,R);
			iamp = BOXHEIGHT/2 - (long)(amp/R_PI*BOXHEIGHT/2);
		}
		else {
			amp =sqrt(R*R + I*I);
			iamp = BOXHEIGHT - (long)(amp*BOXHEIGHT/amprange);
		}
		LineTo(i, iamp);
	}
	LineTo(BOXWIDTH, BOXHEIGHT);
	LineTo(0, BOXHEIGHT);
	
	CloseRgn(myFiltRgn);
	
	
	RGBBackColor(&myShadow);
	FillRgn(myFiltRgn, &qd.black);
	
	
	RGBForeColor(&myLines);
	MoveTo(BOXWIDTH/8, 0);
	LineTo(BOXWIDTH/8, BOXHEIGHT);
	MoveTo(BOXWIDTH/4, 0);
	LineTo(BOXWIDTH/4, BOXHEIGHT);
	MoveTo(3*BOXWIDTH/8, 0);
	LineTo(3*BOXWIDTH/8, BOXHEIGHT);
	MoveTo(5*BOXWIDTH/8, 0);
	LineTo(5*BOXWIDTH/8, BOXHEIGHT);
	MoveTo(3*BOXWIDTH/4, 0);
	LineTo(3*BOXWIDTH/4, BOXHEIGHT);
	MoveTo(7*BOXWIDTH/8, 0);
	LineTo(7*BOXWIDTH/8, BOXHEIGHT);
	
	MoveTo(0, BOXHEIGHT/2);
	LineTo(BOXWIDTH, BOXHEIGHT/2);
		
	RGBForeColor(&myRed);
	MoveTo(BOXWIDTH/2, 0);
	LineTo(BOXWIDTH/2, BOXHEIGHT);
	
	RGBForeColor(&myBlack);
	FrameRgn(myFiltRgn);

	
	RGBForeColor(&myBlack);
	FrameRect(&offr);	
	
	DisposeRgn(myFiltRgn);
	
	PenNormal();
	
	// Then copy the image back on screen and un-prepare offscreen
	SetGWorld((CGrafPtr)oldPort, oldDevice);
	CopyBits(&offPort->portBits, &oldPort->portBits, &offr, &r, srcCopy, NIL);
	UnlockPixels(GetGWorldPixMap((GWorldPtr) myNewGWorld));
		

}

/*--------------------------------------------------------------------------*/
void spectrogram_newcolor(t_spectrogram *x, long r, long g, long b)
{
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	
	x->f_color.red = r << 8;
	x->f_color.green = g << 8;
	x->f_color.blue = b << 8;
	
	x->f_shadow.red = r-40 << 8;
	x->f_shadow.green = g-40 << 8;
	x->f_shadow.blue = b-40 << 8;	
	
	x->f_lines.red = r-80 << 8;
	x->f_lines.green = g-80 << 8;
	x->f_lines.blue = b-80 << 8;
	
	qelem_set(x->f_qelem);
}


/*--------------------------------------------------------------------------*/

void spectrogram_bang(t_spectrogram *x)
{
	x->f_pronto = 0;
	qelem_set(x->f_qelem); // the qelem_set is in the int method
}

void spectrogram_log(t_spectrogram *x)
{	
	x->f_logscale = 1;
	qelem_set(x->f_qelem);
}
void spectrogram_lin(t_spectrogram *x)
{	
	x->f_logscale = 0;
	qelem_set(x->f_qelem);
}
void spectrogram_phase(t_spectrogram *x)
{	
	x->f_phasespect = 1;
	qelem_set(x->f_qelem);
}
void spectrogram_amp(t_spectrogram *x)
{	
	x->f_phasespect = 0;
	qelem_set(x->f_qelem);
}


void spectrogram_range(t_spectrogram *x, double f)
{	
	x->f_range = f;
	qelem_set(x->f_qelem);
}



/*==========================================================================*/

void spectrogram_save(t_spectrogram *x, void *w)
{
	Rect *r;
	
	r = &x->f_box.z_box.b_rect;

	if (x->f_box.z_box.b_hidden)
		binbuf_vinsert(w,"ssssll",	gensym("#P"),
									gensym("hidden"),
									gensym("user"),
									gensym("spectrogram~"),
									(long)(r->left),
									(long)(r->top)	);
	else 
		binbuf_vinsert(w,"sssll",	gensym("#P"),
									gensym("user"), 
									gensym("spectrogram~"),
									(long)(r->left),
									(long)(r->top)	);
									
	binbuf_vinsert(w,"sslll",bindMeBaby,gensym("color"),
									(long)(x->f_color.red >>8),
									(long)(x->f_color.green>>8),
									(long)(x->f_color.blue>>8) );

									
	binbuf_vinsert(w,"ss",bindMeBaby,gensym("done") );
}

void spectrogram_restore(t_spectrogram *x)
{
	// just to remove the current binding to #X when restore is finished
	bindMeBaby->s_thing = 0;
	
	qelem_set(x->f_qelem);
}
	
/* ========================================================================== */

void spectrogram_assist(t_spectrogram *x, void *b, long m, long a, char *s)
{
	//assist_string(RES_ID,m,a,1,3,s);
}

/*--------------------------------------------------------------------------*/

void spectrogram_free(t_spectrogram *x)
{
	if (x->f_realin)
		t_freebytes(x->f_realin,x->f_points * sizeof(float));
	if (x->f_imagin)
		t_freebytes(x->f_imagin,x->f_points * sizeof(float));
	if (x->f_realout)
		t_freebytes(x->f_realout,x->f_points * sizeof(float));
	if (x->f_imagout)
		t_freebytes(x->f_imagout,x->f_points * sizeof(float));
	dsp_freebox((t_pxbox *)x);
	qelem_free(x->f_qelem);
	box_free((void *)x);
	spectrogram_number_of_instances--;
	
	if (! spectrogram_number_of_instances) {
		if (gMyNewGWorld) {
			DisposeGWorld((GWorldPtr) gMyNewGWorld);
			//post("offscreen disposed",0);
		}
	}
}

/*--------------------------------------------------------------------------*/

void *spectrogram_real(Patcher *p, int cx, int cy)
{
	int i;
	long points, interval, phase;
	t_spectrogram *x;
	
	x = (t_spectrogram*)newobject(spectrogram_class);
	dsp_setupbox((t_pxbox *)x, 2);
	
	x->f_patcher		= p;

	x->f_rect.left		= cx;
	x->f_rect.top		= cy;
	x->f_rect.right		= cx + BOXWIDTH; // fixed size
	x->f_rect.bottom	= cy + BOXHEIGHT;

	box_new((Box *)x, p,
	
				  F_DRAWFIRSTIN
				| F_NOGROW,
			
				x->f_rect.left,
				x->f_rect.top,
				x->f_rect.right,
				x->f_rect.bottom
			);
			
	x->f_box.z_box.b_firstin = x;
	box_ready((void *)x);
	x->f_rect = x->f_box.z_box.b_rect;
	x->f_qelem = qelem_new(x, (method)spectrogram_qfn);
	x->f_klok = clock_new(x, (method)spectrogram_tick);

	bindMeBaby->s_thing = (Object *)x;	// to restore saved state
	
	if (! spectrogram_number_of_instances) {
		MyCreateGWorld();
	}
	spectrogram_number_of_instances++;
	
	spectrogram_newcolor(x, 224, 224, 224);	// grey for default background
	
	x->f_sr = sys_getsr();
	x->f_logscale = 0;
	x->f_phasespect = 0;
	x->f_range = 2.;

	points = interval = 1024;
	phase = 512;
	
	x->f_points = points;
	x->f_1overpts = 1.0/points;
	x->f_interval = interval;
	x->f_phase = phase;
	
	x->f_realin = t_getbytes(points * sizeof(float));
	x->f_imagin = t_getbytes(points * sizeof(float));
	x->f_realout = t_getbytes(points * sizeof(float));
	x->f_imagout = t_getbytes(points * sizeof(float));
	
	for (i=0; i < points; i++) {
		x->f_realin[i] = 0;
		x->f_realout[i] = 0;
		x->f_imagin[i] = 0;
		x->f_imagout[i] = 0;
	}
	
	x->f_realinptr = x->f_realin;
	x->f_realoutptr = x->f_realout;
	x->f_imaginptr = x->f_imagin;
	x->f_imagoutptr = x->f_imagout;
	x->f_countdown = 0;
	
	x->f_pronto = 0;
	x->f_timegrain = 20;
	
	return(x);
}

/*--------------------------------------------------------------------------*/

void *spectrogram_new(Symbol *s, int ac, Atom *av)
{
	void *p, *res;
	
	p = av[0].a_w.w_obj;
	av++;
	ac--;
	res = patchbox(p, spectrogram_real, ac, av, SIZE1);
	
	return (res);
}


/*--------------------------------------------------------------------------*/

void *spectrogram_menufun(void *p, long x, long y, long font)
{
	patcher_setport(p);
	if(p)  spectrogram_real(p, x, y);
	
	return (0);
}

/* ========================================================================== */


void MyCreateGWorld(void)
{
	OSErr		err;
	GrafPtr		oldPort, offPort;
	GDHandle	oldDevice;
	
		gOffRect.left = 0;
		gOffRect.top = 0;
		gOffRect.right = BOXWIDTH;
		gOffRect.bottom = BOXHEIGHT;
		
		// create 32-bit color offscreen
		err = NewGWorld(&gMyNewGWorld, 0, &gOffRect, NIL, NIL, 0);
		if (err)
			post("error %ld creating offscreen", err);
		else {
			// prepare offscreen for drawing
			GetGWorld((CGrafPtr *)&oldPort, &oldDevice);
			LockPixels(GetGWorldPixMap((GWorldPtr) gMyNewGWorld));
			SetGWorld((CGrafPtr)gMyNewGWorld, NIL);
		
			// initilaize canvas
			offPort = (GrafPort *)gMyNewGWorld;
			EraseRect(&offPort->portRect);
		
			// un-prepare offscreen
			UnlockPixels(GetGWorldPixMap((GWorldPtr) gMyNewGWorld));
			SetGWorld((CGrafPtr)oldPort, oldDevice);	
		
			//post("offscreen created",0);
		}
}