#include "ext.h"
#include "z_dsp.h"
#include <math.h>
/*
to do:

thinking that it's best ot define object with <fftsize> and <overlap>
where <overlap> is 2 4 or 8.  can't think of where you'd want a different overlap.

right now crashes if not 2 overlap!  oops - need to fix this too.
(although seems to be fixed for all fft sizes


*/

#include "fft_mayer.proto.h"

#define FFT_MIN_POINTS	16
#define FFT_DEFAULT_POINTS 512
#define FFT_MAX_POINTS	4096

#define USING_PHASE

typedef struct _fft
{
	t_pxobject x_obj;

	float *x_realinptr;	// input fill ptr into realin
	float *x_imaginptr;	// input fill ptr into imagin
	float *x_realoutptr;	// output ptr into realout
	float *x_imagoutptr;	// output ptr into imagout
	float *x_realin;	// where transform is done
	float *x_imagin;	// where transform is done
	float *x_realout;	// where transform is done
	float *x_imagout;	// where transform is done
	float *x_window;	// hanning window
	
	long x_fftsize;		// size
	long x_hop;			// sample count before doing another FFT
	long x_start;		// flag that allows phase to be calculated without a global counter
	float x_1overpts;	// for inverse FFT scaling (not rreally needed for fft)
	
	void *x_outlet; // right symbolic connection outlet
	void *x_outlet1; // right symbolic connection outlet
} t_fft;


void *fft_class;

t_symbol *ps_rfft, *ps_rifft;

Symbol *ps_fftbuffconnect;

int errno;

long ilog2(long n);
t_int *fft_perform(t_int *w);
void fft_dsp(t_fft *x, t_signal **sp, short *count);
void fft_tick(t_fft *x);
void fft_free(t_fft *x);
void fft_bang(t_fft *x);
void *fft_new(t_symbol *s, short ac, t_atom *av);
void fft_assist(t_fft *x, void *b, long m, long a, char *s);

void main(void)
{
	setup(&fft_class, fft_new, (method)fft_free, (short)sizeof(t_fft), 
		0L, A_GIMME, 0);
	addmess((method)fft_dsp, "dsp", A_CANT, 0);
	addmess((method)fft_assist, "assist", A_CANT, 0);
	addbang((method)fft_bang);
	ps_rfft = gensym("fft≈");
	ps_rifft = gensym("ifft≈");
	ps_fftbuffconnect = gensym("fftbuffconnect");
	rescopy('STR#',3210);
	dsp_initclass();
}

long ilog2(long n)
{
	long ret = -1;
	while (n) {
		n >>= 1;
		ret++;
	}
	return (ret);
}

t_int *fft_perform(t_int *w)
{
	t_fft *x = (t_fft *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *inimag = (float *)(w[3]);
	float *outreal = (float *)(w[4]);
	float *outimag = (float *)(w[5]);
	int n = w[6];
	int m,np,count;
	long pts = x->x_fftsize;
	long hop = x->x_hop;
	float *b;
	float *ei = x->x_realin + pts; // indicates end pointer of buffer
	float *eo = x->x_realout + pts;	
	//float mult = x->x_1overpts;
	float *wind = x->x_window;
	
	if (x->x_obj.z_disabled)
		goto exit;
	if (n > pts) {  // if sigvect > fftsize
		np = pts;
		count = n / pts;
	} else {
		np = n;
		count = 1;
	}
	m = np;
	set_zero(outreal, n);
	set_zero(outimag, n);
	while (count--) { // do big loop just once if sigvect < fftsize

		b = x->x_realinptr;  // copy signal into input buffer
		BlockMove(inreal,b, np * sizeof(float));
		BlockMove(inimag,x->x_imaginptr, np * sizeof(float));
		b += np; // increment temp input pointer by sigvs
		// we will increment input pointer later
		
		if (b == ei) { // if temp input pointer is at end
			float *real = x->x_realout, *imag = x->x_imagout;
			long wpts = pts;
			
			// copy input buffer into oputput buffer and fft the outbuffer
			BlockMove(x->x_realin,x->x_realout,pts * sizeof(float));
			BlockMove(x->x_imagin,x->x_imagout,pts * sizeof(float));
			// do windowing
			while (wpts--) {
				*real++ *= *wind;
				*imag++ *= *wind++;
			}
			fft(pts, x->x_realout, x->x_imagout); // perform FFT
			
			*outreal = *outimag = 1.0; //send just a click out
			
			// shift and reset i/o buffer pointers
			BlockMove(x->x_realin+hop,x->x_realin,(pts-hop) * sizeof(float));
			BlockMove(x->x_imagin+hop,x->x_imagin,(pts-hop) * sizeof(float));
			x->x_realinptr = x->x_realin +pts-hop;
			x->x_imaginptr = x->x_imagin +pts-hop;

		} else if (b) { // otherwise just increment input pointer
			x->x_realinptr += np;
			x->x_imaginptr += np;
		}
	}
exit:
	return (w + 7);
}


void fft_dsp(t_fft *x, t_signal **sp, short *count)
{
	fft_bang(x);
	
	x->x_start = 1;
	x->x_realinptr = x->x_realin + x->x_hop;
	x->x_imaginptr = x->x_imagin + x->x_hop;
	
	if (x->x_hop % sys_getblksize())
		error("fft≈: phase (%ld) must be multiple of %ld",x->x_hop,sys_getblksize());
	else {
		dsp_add(fft_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);
	}
}

void fft_bang(t_fft *x)
{
	Atom at;
	//float **fftbuff[2];
	float *outreal = x->x_realout;
	float *outimag = x->x_imagout;
	
	//(float *)fftbuff[0] = (float *)outreal;
	//(float *)fftbuff[1] = (float *)outimag;
	
	at.a_type = A_LONG;
	at.a_w.w_long = x->x_fftsize;
	
	ps_fftbuffconnect->s_thing = (Object *)outimag;
	outlet_anything(x->x_outlet1, ps_fftbuffconnect, 1, &at);
	ps_fftbuffconnect->s_thing = (Object *)outreal;
	outlet_anything(x->x_outlet, ps_fftbuffconnect, 1, &at);
	
}


void fft_free(t_fft *x)
{
	if (x->x_realin)
		t_freebytes(x->x_realin,x->x_fftsize * sizeof(float));
	if (x->x_imagin)
		t_freebytes(x->x_imagin,x->x_fftsize * sizeof(float));
	if (x->x_realout)
		t_freebytes(x->x_realout,x->x_fftsize * sizeof(float));
	if (x->x_imagout)
		t_freebytes(x->x_imagout,x->x_fftsize * sizeof(float));
	if (x->x_window)
		t_freebytes(x->x_window,x->x_fftsize * sizeof(float));
	dsp_free((t_pxobject *)x);

}


void fft_assist(t_fft *x, void *b, long m, long a, char *s)
{
		assist_string(3210,m,a,1,3,s);
}

void *fft_new(Symbol *s, short ac, Atom *av)
{
	t_fft *x;
	float *buf;
	long i;
	long bs = sys_getblksize();
	
	long fftsize, hop;
	
	if ((ac > 0) && (av[0].a_type == A_LONG))
		fftsize = av[0].a_w.w_long;
	else
		fftsize = FFT_DEFAULT_POINTS;
		
	if (fftsize != (1 << ilog2(fftsize))) {
		fftsize = 1 << ilog2(fftsize);
		error("fft≈: power of two required for fft size - using %ld", fftsize);
	}
	
	if ((ac > 1) && (av[1].a_type == A_LONG))
		hop = av[1].a_w.w_long;
	else
		hop = 2;
	
	if ( hop != 2 ) {
		error("fft≈: overlap must be 2 or 4 right now - setting to 2");
		hop = 2;
	}
	
	if (fftsize < FFT_MIN_POINTS)
		error("fft≈: minimum size %ld", fftsize = FFT_MIN_POINTS);
	else if (fftsize > FFT_MAX_POINTS)
		error("fft≈: maximum size %ld", fftsize = FFT_MAX_POINTS);

	hop = fftsize / hop; // hop is now defined in samples instead of x overlap
	
	// this is not very good well done because overlap should be 2 4 or 8
	if (hop < bs){
		hop = bs;
		error("fft≈: HOP must be multiple of %ld, setting to %ld", bs,hop);
	}
	
	x = (t_fft *)newobject(fft_class);
	x->x_fftsize = fftsize;
	x->x_hop = hop;
	
	dsp_setup((t_pxobject *)x,2);
	x->x_obj.z_misc = Z_NO_INPLACE;
	x->x_realin = t_getbytes(fftsize * sizeof(float));
	x->x_imagin = t_getbytes(fftsize * sizeof(float));
	x->x_realout = t_getbytes(fftsize * sizeof(float));
	x->x_imagout = t_getbytes(fftsize * sizeof(float));
	x->x_window = t_getbytes(fftsize * sizeof(float));
	
	for (i=0; i < fftsize; i++) {
		x->x_realin[i] = 0;
		x->x_realout[i] = 0;
		x->x_imagin[i] = 0;
		x->x_imagout[i] = 0;
	}
	for (i=0; i < fftsize; i++) { 
		// hanning window with sqrt for two overlap
		x->x_window[i] = sqrt(0.5 * (1. + cos(3.14159 + 3.14159 * 2.* i/fftsize)));
	}
	x->x_realinptr = x->x_realin;
	x->x_realoutptr = x->x_realout;
	x->x_imaginptr = x->x_imagin;
	x->x_imagoutptr = x->x_imagout;

	x->x_1overpts = 1. / x->x_fftsize;
	
	//x->x_outlet2 = outlet_new((t_pxobject *)x, 0L);
	x->x_outlet1 = outlet_new(x, "signal");
	x->x_outlet = outlet_new(x, "signal");
	
	return (x);
}

