/*
DESCRIPTION: Richard Dudas' "big event-based fft", implemented for a Gibson demo and never used or maintained thereafter.
*/

//fugger~ object - big event-based fft
// Fft Used for Gibson Guitar Effect Realisation

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#include "fft_mayer.proto.h"

#define RES_ID 12345
#define MAXSIZE 2048

#define HANNING_W(i,ac) ((1.- cos((i*6.283185)/(ac -1.)))/2.)

void *fugger_class;

typedef struct _fugger
{
    t_pxobject	x_obj;
    
    long		x_bufindex;		// current buffer index
    long		x_bufsize;	// in samples
    t_float		*x_inputbuf;
    
    long 		x_fftbufsize;
	t_float 	*x_fftbuffer;
	t_float 	*x_windbuffer;
    
    long 		x_nextfftsize; // this will be the fft size used
    
    int		x_blksize;
    float	x_sr;
    
    void	*x_outlet;
} t_fugger;


Symbol *ps_fuggerlist;

long ilog2(long n);
void fugger_bang(t_fugger *x);
void fugger_int(t_fugger *x, long n);
t_int *fugger_perform(t_int *w);
void fugger_dsp(t_fugger *x, t_signal **sp, short *count);
void fugger_assist(t_fugger *x, void *b, long m, long a, char *s);
void fugger_free(t_fugger *x);
void *fugger_new(Symbol *s, int ac, Atom *av);


void main(void)
{
    setup(&fugger_class, fugger_new, (method)fugger_free, (short)sizeof(t_fugger), 0L, A_GIMME, 0);
    addmess((method)fugger_dsp, "dsp", A_CANT, 0);
    addbang((method)fugger_bang);
    addint((method)fugger_int);
    addmess((method)fugger_assist,"assist",A_CANT,0);
    
    dsp_initclass();
    
    ps_fuggerlist = gensym("fuggerlist");
    
    rescopy('STR#',RES_ID);
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

void fugger_bang(t_fugger *x)
{
	Atom at[2];
	
	int i;
	long fftsize = x->x_nextfftsize;
	long countdown = x->x_nextfftsize;
	long size = x->x_bufsize;
	long index = x->x_bufindex;
	long begin = index - fftsize;
	
	float *fug = x->x_fftbuffer;
	float *wind = x->x_windbuffer;
	float *buf = x->x_inputbuf; // + x->x_bufindex - size;
	
	t_int *myintbuf = (t_int *)buf; 
	t_int *myintfug = (t_int *)fug;
	
	set_zero(fug, fftsize);
	
	if (begin >= 0) {
		//post("begin is positive", 0);
		myintbuf = (t_int *)buf + begin;
		for (i = 0; i < fftsize; i++) 
			myintfug[i] = myintbuf[i];
	}
	else if (begin < 0) {
		//post("going from %ld in %ld steps", (size + begin), -begin);
		myintbuf = (t_int *)buf + (size + begin);
		for (i = 0; i < (-begin); i++) 
			myintfug[i] = myintbuf[i];
		//post("going from 0 in %ld steps", index);	
		myintbuf = (t_int *)buf;
		myintfug = (t_int *)fug - begin;
		for (i = 0; i < index; i++) 
			myintfug[i] = myintbuf[i]; 
	}
	
	// window fft buffer
	while (countdown--) 
		*fug++ *= *wind++;
			
	realfft(fftsize, x->x_fftbuffer);
	//post("fugger first val: %f", x->x_fftbuffer[0]);
	
	SETLONG(&at[0], fftsize);
	SETLONG(&at[1], (long)x->x_fftbuffer);
	
	outlet_anything(x->x_outlet, ps_fuggerlist, 2, at);
}

void fugger_int(t_fugger *x, long n)
{
	int i;
	float *wind = x->x_windbuffer;
	
	if (n < 2) {
		error("fugger: fft size cannot be less than two");
		n = 2;
		post("        correcting fft size: %ld", n);
	}
	
	if (n != (1 << ilog2(n)) ) {
		error("fugger: power of two required for fft size");
		n = 1 << ilog2(n);
		post("        correcting fft size: %ld", n);
	}

	if (n > x->x_fftbufsize) {
		error("fugger: fft size cannot be greater than max fft size");
		n = x->x_fftbufsize;
		post("        correcting fft size: %ld", n);
	}
	
	for (i = 0; i < n; i++) 
		*wind++ = HANNING_W(i, n);
			
	//post("fugger: fft size %ld", n);
	x->x_nextfftsize = n;
}

t_int *fugger_perform(t_int *w)		// if one sig inlet is used
{
	int i;
	
	t_float *in = (t_float *)(w[1]);
	t_fugger *x = (t_fugger *)(w[2]);
	int n = (int)(w[3]);
	t_float *buf = x->x_inputbuf;
	
	long index = x->x_bufindex;

	t_int *myintin = (t_int *)in; // use RISC chip's int registers for copy
	t_int *myintbuf = (t_int *)(buf+index);
	
	if (x->x_obj.z_disabled)
		goto out;
	if (!x->x_inputbuf)
		goto out;
		
	// do DSP here

	for (i = 0; i < n; i++)
		myintbuf[i] = myintin[i];
		
	if (index + n == x->x_bufsize)
		x->x_bufindex = 0;
	else
		x->x_bufindex += n;
	 
out:
    return (w+4);					// return pointer to NEXT object in dsp chain
}

// the dsp method that calls the above routines:

void fugger_dsp(t_fugger *x, t_signal **sp, short *count)  
{
	x->x_sr =  sys_getsr();
	x->x_blksize =  sys_getblksize();
	
	dsp_add(fugger_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}


void fugger_assist(t_fugger *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

void fugger_free(t_fugger *x)
{	
	if (x->x_inputbuf)
		t_freebytes(x->x_inputbuf,x->x_bufsize * sizeof(float));
		
	if (x->x_fftbuffer)
		t_freebytes(x->x_fftbuffer,x->x_fftbufsize * sizeof(float));	
		
	if (x->x_windbuffer)
		t_freebytes(x->x_windbuffer,x->x_fftbufsize * sizeof(float));	
		
	dsp_free((t_pxobject *)x);
}

void *fugger_new(Symbol *s, int ac, Atom *av)
{
	long size, fftsize;
	double sr, maxms;
	
    t_fugger *x = (t_fugger *)newobject(fugger_class);
    dsp_setup((t_pxobject *)x,1);
    x->x_outlet = outlet_new((t_pxobject *)x, 0L);
    
    if (ac>0) {
    	if (av[0].a_type == A_LONG)
    		maxms = (double)av[0].a_w.w_long;
    	else if (av[0].a_type == A_FLOAT)
    		maxms = av[0].a_w.w_float;
    	else
    		maxms = 1;
    }
    if (maxms < 1) maxms = 1; // set minimum 1 ms sample buffer corrected to 2048 samples
    
    x->x_nextfftsize = 1024;
    if (ac>1) {
    	if (av[1].a_type == A_LONG)
    		x->x_nextfftsize = av[1].a_w.w_long;
    	else if (av[1].a_type == A_FLOAT)
    		x->x_nextfftsize = (long)av[1].a_w.w_float;
    }
    
    x->x_sr = sr = sys_getsr();
	x->x_blksize =  sys_getblksize();
	x->x_bufindex = 0;
	
// allocate buffers ---------------------------------------
    size = maxms * sr * 0.001;
  	//post("size before correction %ld",size);
   	size += (-size) & (SYS_MAXBLKSIZE - 1);
 	//post("     after correction %ld = 2048 * %ld",size, size/2048);
   	x->x_bufsize = size;
   	
   	fftsize = 1 << ilog2(size);
	post("fugger: maximum fft size %ld points", fftsize);
	
	x->x_inputbuf = t_getbytes(x->x_bufsize * sizeof(float));
	if (!x->x_inputbuf) 
		error("fugger~: out of memory allocating input buffer");
    else
    	set_zero(x->x_inputbuf, x->x_bufsize);
    	
    x->x_fftbufsize = fftsize;
	x->x_fftbuffer = t_getbytes(x->x_fftbufsize * sizeof(float));
	if (!x->x_fftbuffer) 
		error("fugger~: out of memory allocating fft buffer");
    else
    	set_zero(x->x_fftbuffer, x->x_fftbufsize);

	x->x_windbuffer = t_getbytes(x->x_fftbufsize * sizeof(float));
	if (!x->x_windbuffer) 
		error("fugger~: out of memory allocating wind buffer");
    else
    	set_zero(x->x_windbuffer, x->x_fftbufsize);
    
    fugger_int(x, x->x_nextfftsize); // make windowing buffer
    return (x);
}
