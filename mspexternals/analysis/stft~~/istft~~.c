#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#include "fft_mayer.proto.h"

#define FFT_MIN_POINTS	16
#define FFT_DEFAULT_POINTS 512
#define FFT_MAX_POINTS	4096

#define USING_PHASE

typedef struct _ifft
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
	float *x_realout2;	// overlap-added output buffer
	float *x_imagout2;	// overlap-added output buffer
	float *x_window;	// hanning window
	
	long x_points;		// size
	long x_hop;	// sample count before doing another FFT
	long x_start;		// flag that allows phase to be calculated without a global counter
	float x_1overpts;	// for inverse FFT scaling
	int x_fftbufferreceived[2];
	
	void *x_outlet; // right symbolic connection outlet
	void *x_outlet1;
} t_ifft;


void *ifft_class;

t_symbol *ps_rfft, *ps_rifft;

Symbol *ps_fftbuffconnect;

int errno;

long ilog2(long n);
t_int *ifft_perform(t_int *w);
void ifft_dsp(t_ifft *x, t_signal **sp, short *count);
void ifft_ifftbuffconnect(t_ifft *x, Symbol *s, int ac, Atom *av);
void ifft_tick(t_ifft *x);
void ifft_free(t_ifft *x);
void ifft_bang(t_ifft *x);
void *ifft_new(t_symbol *s, short ac, t_atom *av);
void ifft_assist(t_ifft *x, void *b, long m, long a, char *s);

void main(void)
{
	setup(&ifft_class, ifft_new, (method)ifft_free, (short)sizeof(t_ifft), 
		0L, A_GIMME, 0);
	addmess((method)ifft_dsp, "dsp", A_CANT, 0);
	addmess((method)ifft_ifftbuffconnect, "fftbuffconnect", A_GIMME, 0);
	addmess((method)ifft_assist, "assist", A_CANT, 0);
	//addbang((method)ifft_bang);
	ps_rfft = gensym("fft≈");
	ps_rifft = gensym("ifft≈");
	ps_fftbuffconnect = gensym("fftbuffconnect");
	rescopy('STR#',3210);
	dsp_initclass();
}



void ifft_ifftbuffconnect(t_ifft *x, Symbol *s, int ac, Atom *av)
{	
	int i = x->x_obj.z_in;
	
	float *fftbuff = (float *)ps_fftbuffconnect->s_thing;
	if (i==0)
		x->x_realin = fftbuff;
	else if (i==1)
		x->x_imagin = fftbuff;
	x->x_fftbufferreceived[i] = 1;
	post("ifft≈: fftbuffconnect[%ld] received! %ld",i, av[0].a_w.w_long);
	
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

t_int *ifft_perform(t_int *w)
{
	t_ifft *x = (t_ifft *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *inimag = (float *)(w[3]);
	float *outreal = (float *)(w[4]);
	float *outimag = (float *)(w[5]);
	int n = w[6];
	int m,np,count;
	long pts = x->x_points;
	long hop = x->x_hop;
	float *b;
	float *ei = x->x_realin + pts;
	float *eo = x->x_realout + pts;	// just do the real buf as the test
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

		b = x->x_realinptr; // copy signal into input buffer
		//BlockMove(inreal,b, np * sizeof(float));
		//BlockMove(inimag,x->x_imaginptr, np * sizeof(float));
		b += np; // increment temp input pointer by sigvs
		// we will increment input pointer later
		
		if (x->x_realoutptr != eo) { // if out position not at end move fft'd sig into out
			double q = (int)(x->x_realoutptr - x->x_realout);
			BlockMove(x->x_realoutptr,outreal,np * sizeof(float));
			BlockMove(x->x_imagoutptr,outimag,np * sizeof(float));
			x->x_realoutptr += np;
			x->x_imagoutptr += np;
		}
		
		if (b == ei) {// if temp input pointer is at end
			float *real = x->x_realout, *imag = x->x_imagout;
			float *real2 = x->x_realout2, *imag2 = x->x_imagout2;
			float mult = x->x_1overpts;
			long wpts = pts;
			// (this step can be eliminated if signals were unique)
			// copy input buffer into oputput buffer and fft the outbuffer
			BlockMove(x->x_realin,x->x_realout2,pts * sizeof(float));
			BlockMove(x->x_imagin,x->x_imagout2,pts * sizeof(float));

			// shift overlapp-add output buffers and zero remaining space
			BlockMove(x->x_realout+hop,x->x_realout,(pts-hop) * sizeof(float));
			BlockMove(x->x_imagout+hop,x->x_imagout,(pts-hop) * sizeof(float));
			set_zero(x->x_realout+(pts-hop), hop);
			set_zero(x->x_imagout+(pts-hop), hop);
			
			ifft(pts, real2, imag2);
			while (wpts--) {
				*real++ += *real2++ * mult * *wind;
				*imag++ += *imag2++ * mult * *wind++;
			}
			
			x->x_realoutptr = x->x_realout;
			x->x_imagoutptr = x->x_imagout;
			x->x_realinptr = x->x_realin+pts-hop;
			x->x_imaginptr = x->x_imagin+pts-hop;

		} else if (b) {  // otherwise increment input pointer
			x->x_realinptr += np;
			x->x_imaginptr += np;
		}
	}
out:
	return (w + 7);
}


void ifft_dsp(t_ifft *x, t_signal **sp, short *count)
{
	// only add to chain if it has been connected to a valid fft buffer pointer!
	if (x->x_fftbufferreceived[0] && x->x_fftbufferreceived[1]) {
		post("ifft≈: adding to dsp chain",0);

		x->x_start = 1;

		x->x_realinptr = x->x_realin + x->x_hop;
		x->x_imaginptr = x->x_imagin + x->x_hop;
		x->x_realoutptr = x->x_realout;
		x->x_imagoutptr = x->x_imagout;

		if (x->x_hop % sys_getblksize())
			error("ifft≈: hop (%ld) must be multiple of %ld",x->x_hop,sys_getblksize());
		else {
			dsp_add(ifft_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);
		}
	}
}





void ifft_free(t_ifft *x)
{
	if (x->x_realin)
		t_freebytes(x->x_realin,x->x_points * sizeof(float));
	if (x->x_imagin)
		t_freebytes(x->x_imagin,x->x_points * sizeof(float));
	if (x->x_realout)
		t_freebytes(x->x_realout,x->x_points * sizeof(float));
	if (x->x_imagout)
		t_freebytes(x->x_imagout,x->x_points * sizeof(float));
	if (x->x_realout2)
		t_freebytes(x->x_realout2,x->x_points * sizeof(float));
	if (x->x_imagout2)
		t_freebytes(x->x_imagout2,x->x_points * sizeof(float));
	if (x->x_window)
		t_freebytes(x->x_window,x->x_points * sizeof(float));
	dsp_free((t_pxobject *)x);

}


void ifft_assist(t_ifft *x, void *b, long m, long a, char *s)
{
		assist_string(3210,m,a,1,3,s);
}

void *ifft_new(t_symbol *s, short ac, t_atom *av)
{
	t_ifft *x;
	float *buf;
	long bad = 0,i;
	long bs = sys_getblksize();
	
	long fftsize, hop;
	
	if ((ac > 0) && (av[0].a_type == A_LONG))
		fftsize = av[0].a_w.w_long;
	else
		fftsize = FFT_DEFAULT_POINTS;
	
	if (fftsize != (1 << ilog2(fftsize))) {
		error("ifft≈: power of two required for fftsize");
		fftsize = 1 << ilog2(fftsize);
	}
		
	if ((ac > 1) && (av[1].a_type == A_LONG))
		hop = av[1].a_w.w_long;
	else
		hop = 2;
	
	if ( hop != 2 ) {
		error("ifft≈: overlap must be 2 or 4 right now - setting to 2");
		hop = 2;
	}

	if (fftsize < FFT_MIN_POINTS)
		error("ifft≈: minimum size %ld", fftsize = FFT_MIN_POINTS);
	else if (fftsize > FFT_MAX_POINTS)
		error("ifft≈: maximum size %ld", fftsize = FFT_MAX_POINTS);

	hop = fftsize / hop; // hop is now defined in samples instead of x overlap 
	
	// this is not very good well done because overlap should be 2 4 or 8
	if (hop < bs){
		hop = bs;
		error("ifft≈: HOP must be multiple of %ld, setting to %ld", bs,hop);
	}
	
	x = (t_ifft *)newobject(ifft_class);
	x->x_points = fftsize;
	x->x_hop = hop;


	dsp_setup((t_pxobject *)x,2);
	x->x_obj.z_misc = Z_NO_INPLACE;
	x->x_realin = t_getbytes(fftsize * sizeof(float));
	x->x_imagin = t_getbytes(fftsize * sizeof(float));
	x->x_realout = t_getbytes(fftsize * sizeof(float));
	x->x_imagout = t_getbytes(fftsize * sizeof(float));
	x->x_realout2 = t_getbytes(fftsize * sizeof(float));
	x->x_imagout2 = t_getbytes(fftsize * sizeof(float));
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

	x->x_1overpts = 1. / x->x_points;
	x->x_fftbufferreceived[0] = x->x_fftbufferreceived[1] = 0;
	
	x->x_outlet1 = outlet_new(x, "signal");
	x->x_outlet = outlet_new(x, "signal");
	
	return (x);
}