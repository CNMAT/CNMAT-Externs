/* SDIF-stft~ dudas hack



-----------------------
to do:

thinking that it's best ot define object with <fftsize> and <overlap>
where <overlap> is 2 4 or 8.  can't think of where you'd want a different overlap.

right now crashes if not 2 overlap!  oops - need to fix this too.
(although seems to be fixed for all fft sizes

*/

#include "ext.h"
#include "z_dsp.h"
#include <math.h>


#include "fft_mayer.proto.h"
//#include "fft_adrian.h"

#define RES_ID 15667

#define stft_MIN_POINTS	16
#define stft_DEFAULT_POINTS 512
#define stft_MAX_POINTS	4096

typedef struct _t_complex {
	float re;
	float im;
} t_complex;

typedef struct _stft
{
	t_pxobject x_obj;

	float *x_inptr;		// input fill ptr into realin
	float *x_outptr;	// output ptr into realout
	float *x_in;		// where transform is done
	float *x_out;		// where transform is done
	float *x_window;	// hanning window
	//float *x_twiddle;	// twiddle table for adrian's fft
	
	long x_fftsize;		// size
	long x_hop;			// sample count before doing another FFT
	long x_start;		// flag that allows phase to be calculated without a global counter
	float x_1overpts;	// for inverse FFT scaling (not rreally needed for fft)
	
	
	t_complex *x_sdifbuf; 	// eventually an sdif buffer
	Symbol *x_sdifbufname;	// name of sdif buffer
	long x_sdifbufsize;		// size of sdifbuf for memory allocation
	
	void *x_outlet; 	// symbolic connection (signal) outlet
} t_stft;


void *stft_class;
Symbol *ps_sdifstftbuffconnect;


long ilog2(long n);
t_int *stft_perform(t_int *w);
void stft_dsp(t_stft *x, t_signal **sp, short *count);
void stft_tick(t_stft *x);
void stft_free(t_stft *x);
void stft_bang(t_stft *x);
void *stft_new(t_symbol *s, short ac, t_atom *av);
void stft_assist(t_stft *x, void *b, long m, long a, char *s);

void main(void)
{
	setup(&stft_class, stft_new, (method)stft_free, (short)sizeof(t_stft), 
		0L, A_GIMME, 0);
	addmess((method)stft_dsp, "dsp", A_CANT, 0);
	addmess((method)stft_assist, "assist", A_CANT, 0);
	addbang((method)stft_bang);

	ps_sdifstftbuffconnect = gensym("sdifstftbuffconnect");
	rescopy('STR#',RES_ID);
	dsp_initclass();
}

//quick test for power of 2:
// b = ((n&(n-1))==0);
 
long ilog2(long n)
{
	long ret = -1;
	while (n) {
		n >>= 1;
		ret++;
	}
	return (ret);
}

t_int *stft_perform(t_int *w)
{
	t_stft *x = (t_stft *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *outreal = (float *)(w[3]);
	int n = w[4];
	int i, m, np, count;
	long pts = x->x_fftsize;
	long hop = x->x_hop;
	float *b;
	float *ei = x->x_in + pts; // indicates end pointer of buffer
	float *eo = x->x_out + pts;	
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
	while (count--) { // do big loop just once if sigvect < fftsize

		b = x->x_inptr;  // copy signal into input buffer
		BlockMove(inreal,b, np * sizeof(float));
		b += np; 	// increment temp input pointer by sigvs
					// we will increment input pointer later
		
		if (b == ei) { // if temp input pointer is at end
			float *real = x->x_out;
			float *freal = x->x_out;
			t_complex *frame = x->x_sdifbuf;
			long wpts = pts;
			long fpts = x->x_sdifbufsize -1;
			
			// copy input buffer into oputput buffer and fft the outbuffer
			BlockMove(x->x_in,x->x_out,pts * sizeof(float));
			
			while (wpts--) 
				*real++ *= *wind++; // do windowing
	
			realfft(pts, x->x_out); // perform mayer FFT	
			//fftRealfast(pts, x->x_out, x->x_twiddle); // REALFAST FFT		
			*outreal = 1.0; //send just a click out
			
			// fill sdifbuf
			for (i=1; i< fpts; i++) {
				x->x_sdifbuf[i].re = x->x_out[i];
				x->x_sdifbuf[i].im = x->x_out[i+fpts];
			}
			x->x_sdifbuf[0].re = x->x_out[0];		
			x->x_sdifbuf[fpts].re = x->x_out[fpts];
			x->x_sdifbuf[0].im = x->x_sdifbuf[fpts].im = 0.;
		
			// shift and reset i/o buffer pointers
			BlockMove(x->x_in+hop,x->x_in,(pts-hop) * sizeof(float));
			x->x_inptr = x->x_in +pts-hop;

		} else if (b) { // otherwise just increment input pointer
			x->x_inptr += np;
		}
	}
exit:
	return (w + 5);
}


void stft_dsp(t_stft *x, t_signal **sp, short *count)
{
	stft_bang(x);
	
	x->x_start = 1;
	x->x_inptr = x->x_in + x->x_hop;
	
	if (x->x_hop % sys_getblksize())
		error("SDIF-stft~: phase (%ld) must be multiple of %ld",x->x_hop,sys_getblksize());
	else {
		dsp_add(stft_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
	}
}

void stft_bang(t_stft *x)
{
	Atom at[2];
	
	at[0].a_type = A_SYM;
	at[0].a_w.w_sym = (Symbol *)x->x_sdifbufname;
	
	at[1].a_type = A_LONG;
	at[1].a_w.w_long = x->x_sdifbufsize;
	
	ps_sdifstftbuffconnect->s_thing = (Object *)x->x_sdifbuf;
	outlet_anything(x->x_outlet, ps_sdifstftbuffconnect, 2, at);
	
}


void stft_free(t_stft *x)
{
	if (x->x_in)
		t_freebytes(x->x_in,x->x_fftsize * sizeof(float));
	if (x->x_out)
		t_freebytes(x->x_out,x->x_fftsize * sizeof(float));
	if (x->x_window)
		t_freebytes(x->x_window,x->x_fftsize * sizeof(float));	
	if (x->x_sdifbuf)
		t_freebytes(x->x_sdifbuf,x->x_sdifbufsize * sizeof(t_complex));
	//if (x->x_twiddle) 
	//	t_freebytes(x->x_twiddle, CMAX * x->x_fftsize * sizeof(float));
	dsp_free((t_pxobject *)x);

}


void stft_assist(t_stft *x, void *b, long m, long a, char *s)
{
		assist_string(RES_ID,m,a,1,3,s);
}

void *stft_new(Symbol *s, short ac, Atom *av)
{
	t_stft *x;
	float *buf;
	long i;
	long bs = sys_getblksize();
	
	long fftsize, hop;
	
	x = (t_stft *)newobject(stft_class);
	
	if ((ac > 0) && (av[0].a_type == A_SYM)) {
		x->x_sdifbufname = av[0].a_w.w_sym;
	}
	else {
		x->x_sdifbufname = 0L;
		post("SDIF-stft~: need a SDIF buffer name as 1st arg",0);
	}
		
	if ((ac > 1) && (av[1].a_type == A_LONG))
		fftsize = av[1].a_w.w_long;
	else
		fftsize = stft_DEFAULT_POINTS;
		
	if (fftsize != (1 << ilog2(fftsize))) {
		fftsize = 1 << ilog2(fftsize);
		error("SDIF-stft~: power of two required for fft size - using %ld", fftsize);
	}
	
	if ((ac > 2) && (av[2].a_type == A_LONG))
		hop = av[2].a_w.w_long;
	else
		hop = 2;
	
	if ( hop != 2 ) {
		error("SDIF-stft~: overlap must be 2 or 4 right now - setting to 2");
		hop = 2;
	}
	
	if (fftsize < stft_MIN_POINTS)
		error("SDIF-stft~: minimum size %ld", fftsize = stft_MIN_POINTS);
	else if (fftsize > stft_MAX_POINTS)
		error("SDIF-stft~: maximum size %ld", fftsize = stft_MAX_POINTS);

	hop = fftsize / hop; // hop is now defined in samples instead of x overlap
	
	// this is not very good well done because overlap should be 2 4 or 8
	if (hop < bs){
		hop = bs;
		error("SDIF-stft~: HOP must be multiple of %ld, setting to %ld", bs,hop);
	}
	
	x->x_fftsize = fftsize;
	x->x_sdifbufsize = (fftsize/2) +1;
	x->x_hop = hop;
	
	dsp_setup((t_pxobject *)x,1);
	x->x_outlet = outlet_new(x, "signal");
	
	x->x_obj.z_misc = Z_NO_INPLACE;
	x->x_in = t_getbytes(fftsize * sizeof(float));
	x->x_out = t_getbytes(fftsize * sizeof(float));
	x->x_window = t_getbytes(fftsize * sizeof(float));
	x->x_sdifbuf = t_getbytes(x->x_sdifbufsize * sizeof(t_complex));
	//x->x_twiddle = t_getbytes(CMAX * fftsize * sizeof(float));
	
	for (i=0; i < fftsize; i++) {
		x->x_in[i] = 0;
		x->x_out[i] = 0;
	}
	for (i=0; i < fftsize; i++) { 
		// hanning window
		x->x_window[i] = sqrt(0.5 * (1. + cos(3.14159 + 3.14159 * 2.* i/fftsize)));
	}
	x->x_inptr = x->x_in;
	x->x_outptr = x->x_out;

	x->x_1overpts = 1. / x->x_fftsize;
	
	return (x);
}

