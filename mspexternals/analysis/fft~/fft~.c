#include "ext.h"
#include "z_dsp.h"
#include "math.h"

#include "fft_mayer.proto.h"
//#include "fft_duhamel.proto.h"

#define FFT_MIN_POINTS	16
#define FFT_DEFAULT_POINTS 512
#define FFT_MAX_POINTS	4096

#define USING_PHASE

typedef struct _fft
{
	t_pxobject f_obj;
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
	long f_phase;		// phase of FFT start (relative to global counter)
	long f_interval;	// sample count before doing another FFT
	long f_inverse;		// do IFFT
	long f_start;		// flag that allows phase to be calculated without a global counter
	void *f_clock;
	void *f_bangout;
	float f_1overpts;	// for inverse FFT scaling
} t_fft;


void *fft_class;
long fts_mode;

t_symbol *ps_fft, *ps_ifft;

int errno;

long ilog2(long n);
t_int *fft_complex_perform(t_int *w);
t_int *fft_real_perform(t_int *w);
void fft_dsp(t_fft *x, t_signal **sp, short *count);
void fft_tick(t_fft *x);
void fft_free(t_fft *x);
void *fft_donew(long points, long interval, long phase, long inverse);
void *fft_new(t_symbol *s, short argc, t_atom *argv);
void d2fcopy(double *src, float *dst, long n);
void f2dcopy(float *src, double *dst, long n);
void fft_assist(t_fft *x, void *b, long m, long a, char *s);

void main(void)
{
	setup(&fft_class, fft_new, (method)fft_free, (short)sizeof(t_fft), 
		0L, A_GIMME, 0);
	addmess((method)fft_dsp, "dsp", A_CANT, 0);
	addmess((method)fft_assist, "assist", A_CANT, 0);
	ps_fft = gensym("fft~");
	ps_ifft = gensym("ifft~");
	rescopy('STR#',3210);
	dsp_initclass();
	fts_mode = (long)dsp_fts_mode;
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

t_int *fft_complex_perform(t_int *w)
{
	t_fft *x = (t_fft *)(w[1]);
	float *inreal = (float *)(w[2]);
	float *inimag = (float *)(w[3]);
	float *outreal = (float *)(w[4]);
	float *outimag = (float *)(w[5]);
	float *outsync = (float *)(w[6]);
	int n = w[7],m,np,count;
	long pts = x->f_points, interval = x->f_interval;
	float *b, *ei = x->f_realin + pts, *eo = x->f_realout + pts;	// just do the real buf as the test
	float mult = x->f_1overpts;
	
	if (x->f_obj.z_disabled)
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
#ifdef USING_PHASE
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
#endif
		if (x->f_countdown) {
			x->f_countdown--;
			b = 0;
		} else {
			b = x->f_realinptr;
			BlockMove(inreal,b, np * sizeof(float));
			BlockMove(inimag,x->f_imaginptr, np * sizeof(float));
			b += np;
		}
		if (x->f_realoutptr != eo) {
			double q = (int)(x->f_realoutptr - x->f_realout);
			BlockMove(x->f_realoutptr,outreal,np * sizeof(float));
			BlockMove(x->f_imagoutptr,outimag,np * sizeof(float));
			x->f_realoutptr += np;
			x->f_imagoutptr += np;
			if (!fts_mode) {
				while (m--) {
					*outsync++ = q;
					q += 1.0;
				}
			}
		} else {
			if (!fts_mode) {
				while (m--)
					*outsync++ = 0.;
			}
		}
		
		if (b == ei) {
			// this step can be eliminated if signals were unique
			BlockMove(x->f_realin,x->f_realout,pts * sizeof(float));
			BlockMove(x->f_imagin,x->f_imagout,pts * sizeof(float));
			if (x->f_inverse) {
				float *real = x->f_realout, *imag = x->f_imagout;
				float mult = x->f_1overpts;
				ifft(pts, real, imag);
				while (pts--) {
					*real++ *= mult;
					*imag++ *= mult;
				}
			} else
				fft(pts, x->f_realout, x->f_imagout);
			// "pts" is no longer valid at this point
			x->f_realoutptr = x->f_realout;
			x->f_imagoutptr = x->f_imagout;
			x->f_realinptr = x->f_realin;
			x->f_imaginptr = x->f_imagin;
			x->f_countdown = (interval - x->f_points) / np;
			if (fts_mode)
				clock_delay(x->f_clock, 0L);
		} else if (b) {
			x->f_realinptr += np;
			x->f_imaginptr += np;
		}
	}
out:
	return (w + 8);
}

t_int *fft_real_perform(t_int *w)
{
	t_fft *x = (t_fft *)(w[1]);
	float *in = (float *)(w[2]);
	float *out = (float *)(w[3]);
	float *outsync = (float *)(w[4]);
	int n = w[5],m,np,count;
	long pts = x->f_points, interval = x->f_interval;
	float *b, *ei = x->f_realin + pts, *eo = x->f_realout + pts;
	float mult = x->f_1overpts;
	
	if (x->f_obj.z_disabled)
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
#ifdef USING_PHASE
		if (x->f_start) {
			long phase = x->f_phase % interval;
			if (phase < pts)
				x->f_realoutptr = x->f_realout + phase;
			else
				x->f_realoutptr = x->f_realout + pts;
			if (pts + phase >= interval) {
				x->f_countdown = 0;
				x->f_realinptr = x->f_realin + (pts + phase - interval);
			} else {
				x->f_countdown = (interval - phase - pts) / np;
				x->f_realinptr = x->f_realin;
			}
			x->f_start = 0;
		}
#endif
		if (x->f_countdown) {
			x->f_countdown--;
			b = 0;
		} else {
			b = x->f_realinptr;
			BlockMove(in,b, np * sizeof(float));
			b += np;
		}
		if (x->f_realoutptr != eo) {
			double q = (int)(x->f_realoutptr - x->f_realout);
			BlockMove(x->f_realoutptr,out,np * sizeof(float));
			x->f_realoutptr += np;
			if (!fts_mode) {
				while (m--) {
					*outsync++ = q;
					q += 1.0;
				}
			}
		} else {
			if (!fts_mode) {
				while (m--)
					*outsync++ = 0.;
			}
		}
		if (b == ei) {
			BlockMove(x->f_realin,x->f_realout,pts * sizeof(float));
			if (x->f_inverse) {
				float *real = x->f_realout;
				float mult = x->f_1overpts;
				realifft(pts, real);
				while (pts--)
					*real++ *= mult;
			} else
				realfft(pts, x->f_realout);
			x->f_realoutptr = x->f_realout;
			x->f_realinptr = x->f_realin;
			x->f_countdown = (interval - x->f_points) / np;
			if (fts_mode)
				clock_delay(x->f_clock, 0L);
		} else if (b) {
			x->f_realinptr += np;
		}
	}
out:
	return (w + 6);
}

void fft_dsp(t_fft *x, t_signal **sp, short *count)
{
	// need to calculate phase
	x->f_start = 1;
#ifndef USING_PHASE
	x->f_countdown = 0;
	x->f_realinptr = x->f_realin;
	x->f_realoutptr = x->f_realout;
	x->f_imaginptr = x->f_imagin;
	x->f_imagoutptr = x->f_imagout;
#endif
	if (x->f_phase % sys_getblksize())
		error("fft~: phase (%ld) must be multiple of %ld",x->f_phase,sys_getblksize());
	else {
		if (count[1] || count[3])	// imaginary inputs, use complex fft
			dsp_add(fft_complex_perform, 7, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
				fts_mode? 0 : sp[4]->s_vec, sp[0]->s_n);
		else
			dsp_add(fft_real_perform, 5, x, sp[0]->s_vec, sp[2]->s_vec, fts_mode? 0 : sp[4]->s_vec, sp[0]->s_n);
	}
}
	
void fft_tick(t_fft *x)
{
	outlet_bang(x->f_bangout);
}

void fft_free(t_fft *x)
{
	if (x->f_realin)
		t_freebytes(x->f_realin,x->f_points * sizeof(float));
	if (x->f_imagin)
		t_freebytes(x->f_imagin,x->f_points * sizeof(float));
	if (x->f_realout)
		t_freebytes(x->f_realout,x->f_points * sizeof(float));
	if (x->f_imagout)
		t_freebytes(x->f_imagout,x->f_points * sizeof(float));
	dsp_free((t_pxobject *)x);
	if (fts_mode)
		freeobject(x->f_clock);
}

void fft_assist(t_fft *x, void *b, long m, long a, char *s)
{
	if (fts_mode)
		assist_string(3210,m,a,1,6,s);
	else
		assist_string(3210,m,a,1,3,s);
}

void *fft_donew(long points, long interval, long phase, long inverse)
{
	t_fft *x;
	float *buf;
	long bad = 0,i;
	long bs = sys_getblksize();
	
	if (!points)
		points = FFT_DEFAULT_POINTS;
	if (!interval)
		interval = points;
	if (points != (1 << ilog2(points)) || interval != (1 << ilog2(interval))) {
		error("fft: power of two required for size and interval");
		points = 1 << ilog2(points);
		interval = 1 << ilog2(interval);
	}
	if (points < FFT_MIN_POINTS)
		error("fft~: minimum size %ld", points = FFT_MIN_POINTS);
	else if (points > FFT_MAX_POINTS)
		error("fft~: maximum size %ld", points = FFT_MAX_POINTS);
	if (points > interval) {
		error("fft~: interval must be at least one frame, setting to %ld",points);
		interval = points;
	}
	phase %= interval;
	if (phase % bs) {
		if (phase > bs)
			phase -= (phase % bs);
		else
			phase = bs;
		error("fft~: phase must be multiple of %ld, setting to %ld", bs,phase);
	}
	x = (t_fft *)newobject(fft_class);
	x->f_points = points;
	x->f_interval = interval;
	x->f_phase = phase;
	x->f_inverse = inverse;
	dsp_setup((t_pxobject *)x,2);
	x->f_obj.z_misc = Z_NO_INPLACE;
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
	if (!fts_mode)
		outlet_new(x, "signal");
	else {
		x->f_clock = clock_new(x, (method)fft_tick);
		x->f_bangout = bangout((t_object *)x);
	}
	x->f_1overpts = 1. / x->f_points;
	outlet_new(x, "signal");
	outlet_new(x, "signal");
	return (x);
}

void *fft_new(t_symbol *s, short argc, t_atom *argv)
{
	long npoints = 0, period = 0, phase = 0, inverse = s == ps_ifft;
	
	if (argc) {
		if (argv->a_type == A_LONG)
			npoints = argv->a_w.w_long;
		argv++;
		argc--;
	}
	if (argc) {
		if (argv->a_type == A_LONG)
			period = argv->a_w.w_long;
		argv++;
		argc--;
	}
	if (argc && argv->a_type == A_LONG)
		phase = argv->a_w.w_long;
				
	return (fft_donew(npoints, period, phase, inverse));
}
