/* SDIF-istft~  dudas hack

SVN_REVISION: $LastChangedRevision$
DESCRIPTION: Unfinished ancient inverse STFT object by Richard Dudas, reading from SDIF to a signal output.  This sort of became MSP's pfft~ object.

*/

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#include "fft_mayer.proto.h"
//#include "fft_adrian.h"

#define RES_ID 15668
#define FFT_MIN_POINTS	16
#define FFT_DEFAULT_POINTS 512
#define FFT_MAX_POINTS	4096

typedef struct _t_complex {
	float re;
	float im;
} t_complex;

typedef struct _istft
{
	t_pxobject x_obj;

	float *x_inptr;		// input fill ptr into realin
	float *x_outptr;	// output ptr into realout
	float *x_in;		// where transform is done
	float *x_out;		// where transform is done
	float *x_out2;		// overlap-added output buffer
	float *x_window;	// hanning window
	//float *x_twiddle;	// hanning window
	
	long x_points;		// size
	long x_hop;			// sample count before doing another FFT
	long x_start;		// flag that allows phase to be calculated without a global counter
	float x_1overpts;	// for inverse FFT scaling
	int x_fftbufferreceived;
	
	t_complex *x_sdifbuf; 	// eventually an sdif buffer received
	Symbol *x_sdifbufname;	// name of received sdif buffer
	long x_sdifbufsize;		// size of received sdifbuf
	
	void *x_outlet; 	// symbolic connection (signal) outlet

} t_istft;

void *istft_class;
Symbol *ps_sdifstftbuffconnect;


long ilog2(long n);
t_int *istft_perform(t_int *w);
void istft_dsp(t_istft *x, t_signal **sp, short *count);
void istft_isdifstftbuffconnect(t_istft *x, Symbol *s, int ac, Atom *av);
void istft_tick(t_istft *x);
void istft_free(t_istft *x);
void istft_bang(t_istft *x);
void *istft_new(t_symbol *s, short ac, t_atom *av);
void istft_assist(t_istft *x, void *b, long m, long a, char *s);

void main(void)
{
	setup(&istft_class, istft_new, (method)istft_free, (short)sizeof(t_istft), 
		0L, A_GIMME, 0);
	addmess((method)istft_dsp, "dsp", A_CANT, 0);
	addmess((method)istft_isdifstftbuffconnect, "sdifstftbuffconnect", A_GIMME, 0);
	addmess((method)istft_assist, "assist", A_CANT, 0);
	//addbang((method)istft_bang);
	ps_sdifstftbuffconnect = gensym("sdifstftbuffconnect");
	rescopy('STR#',RES_ID);
	dsp_initclass();
}



void istft_isdifstftbuffconnect(t_istft *x, Symbol *s, int ac, Atom *av)
{	
	if (av[0].a_type == A_SYM && av[1].a_type == A_LONG) {
		x->x_sdifbufname = av[0].a_w.w_sym; 
		x->x_sdifbufsize = av[1].a_w.w_long; 
		
		x->x_sdifbuf = (t_complex *)ps_sdifstftbuffconnect->s_thing;
		x->x_fftbufferreceived = 1;
		
		//post("SDIF-istft~: sdifstftbuf %s %ld received!", x->x_sdifbufname->s_name, x->x_sdifbufsize);
	}
	else
		post("incorrect arguments to sdifstftbuffconnect message!",0);
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

t_int *istft_perform(t_int *w)
{
	t_istft *x = (t_istft *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *outreal = (float *)(w[3]);
	int n = w[4];
	int i, m, np, count;
	long pts = x->x_points;
	long hop = x->x_hop;
	float *b;
	float *ei = x->x_in + pts;
	float *eo = x->x_out + pts;	// just do the real buf as the test
	float mult = x->x_1overpts;
	float *wind = x->x_window;
	
	if (x->x_obj.z_disabled)
		goto out;
	if (n > pts) {  // if sigvect > fftsize
		np = pts;
		count = n / pts;
	} else {
		np = n;
		count = 1;
	}
	m = np;
	
	while (count--) { // do big loop just once if sigvect < fftsize

		b = x->x_inptr; // copy signal into input buffer
		b += np; // increment temp input pointer by sigvs
		// we will increment input pointer later
		
		if (x->x_outptr != eo) { // if out position not at end move fft'd sig into out
			double q = (int)(x->x_outptr - x->x_out);
			BlockMove(x->x_outptr,outreal,np * sizeof(float));
			x->x_outptr += np;
		}
		
		if (b == ei) {// if temp input pointer is at end
			float *real = x->x_out; 
			float *freal = x->x_out; 
			float *frame = (float *)x->x_sdifbuf; 
			float *real2 = x->x_out2;
			float mult = x->x_1overpts;
			long wpts = pts;
			long fpts = x->x_sdifbufsize -1;
			
			// read from sdifbuf
			for (i=1; i<fpts; i++) {
				x->x_out2[i] = x->x_sdifbuf[i].re;
				x->x_out2[i+fpts] = x->x_sdifbuf[i].im;
			}
			x->x_out2[0] = x->x_sdifbuf[0].re;	
			x->x_out2[fpts] = x->x_sdifbuf[fpts].re;		

			// (this step can be eliminated if signals were unique)
			// copy input buffer into oputput buffer and fft the outbuffer
			//BlockMove(x->x_in,x->x_out2,pts * sizeof(float));

			// shift overlapp-add output buffers and zero remaining space
			BlockMove(x->x_out+hop,x->x_out,(pts-hop) * sizeof(float));
			set_zero(x->x_out+(pts-hop), hop);
			
			//ifftRealfast(pts, real2, x->x_twiddle); // REALFAST IFFT
			realifft(pts, real2);
			
			while (wpts--)
				*real++ += *real2++ * mult * *wind++;
			
			x->x_outptr = x->x_out;
			x->x_inptr = x->x_in+pts-hop;

		} else if (b) {  // otherwise increment input pointer
			x->x_inptr += np;
		}
	}
out:
	return (w + 5);
}


void istft_dsp(t_istft *x, t_signal **sp, short *count)
{
	// only add to chain if it has been connected to a valid fft buffer pointer!
	if (x->x_fftbufferreceived) {
		//post("SDIF-istft~: adding to dsp chain",0);

		x->x_start = 1;
		x->x_inptr = x->x_in + x->x_hop;
		x->x_outptr = x->x_out;

		if (x->x_hop % sys_getblksize())
			error("SDIF-istft~: hop (%ld) must be multiple of %ld",x->x_hop,sys_getblksize());
		else {
			dsp_add(istft_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
		}
	}
}





void istft_free(t_istft *x)
{
	if (x->x_in)
		t_freebytes(x->x_in,x->x_points * sizeof(float));
	if (x->x_out)
		t_freebytes(x->x_out,x->x_points * sizeof(float));
	if (x->x_out2)
		t_freebytes(x->x_out2,x->x_points * sizeof(float));
	if (x->x_window)
		t_freebytes(x->x_window,x->x_points * sizeof(float));
	//if (x->x_twiddle) 
	//	t_freebytes(x->x_twiddle, CMAX * x->x_points * sizeof(float));
	dsp_free((t_pxobject *)x);

}


void istft_assist(t_istft *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,3,s);
}

void *istft_new(t_symbol *s, short ac, t_atom *av)
{
	t_istft *x;
	float *buf;
	long bad = 0,i;
	long bs = sys_getblksize();
	
	long fftsize, hop;
	
	if ((ac > 0) && (av[0].a_type == A_LONG))
		fftsize = av[0].a_w.w_long;
	else
		fftsize = FFT_DEFAULT_POINTS;
	
	if (fftsize != (1 << ilog2(fftsize))) {
		error("SDIF-istft~: power of two required for fftsize");
		fftsize = 1 << ilog2(fftsize);
	}
		
	if ((ac > 1) && (av[1].a_type == A_LONG))
		hop = av[1].a_w.w_long;
	else
		hop = 2;
	
	if ( hop != 2 ) {
		error("SDIF-istft~: overlap must be 2 or 4 right now - setting to 2");
		hop = 2;
	}

	if (fftsize < FFT_MIN_POINTS)
		error("SDIF-istft~: minimum size %ld", fftsize = FFT_MIN_POINTS);
	else if (fftsize > FFT_MAX_POINTS)
		error("SDIF-istft~: maximum size %ld", fftsize = FFT_MAX_POINTS);

	hop = fftsize / hop; // hop is now defined in samples instead of x overlap 
	
	// this is not very good well done because overlap should be 2 4 or 8
	if (hop < bs){
		hop = bs;
		error("SDIF-istft~: HOP must be multiple of %ld, setting to %ld", bs,hop);
	}
	
	x = (t_istft *)newobject(istft_class);
	x->x_points = fftsize;
	x->x_hop = hop;

	x->x_sdifbufname = 0L;
	 
	dsp_setup((t_pxobject *)x,1);
	x->x_outlet = outlet_new(x, "signal");
	
	x->x_obj.z_misc = Z_NO_INPLACE;
	x->x_in = t_getbytes(fftsize * sizeof(float));
	x->x_out = t_getbytes(fftsize * sizeof(float));
	x->x_out2 = t_getbytes(fftsize * sizeof(float));
	x->x_window = t_getbytes(fftsize * sizeof(float));
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

	x->x_1overpts = 1. / x->x_points;
	x->x_fftbufferreceived = 0;
	
	return (x);
}
