//sugger~ object - big event-based signal buffer display
// Signal Used for Gibson Guitar Effect Realisation

#include "ext.h"
#include "z_dsp.h"
#include <math.h>

#define RES_ID 12347
#define MAXSIZE 2048

void *sugger_class;


typedef struct _sugger
{
    t_pxobject	x_obj;
    
    long		x_bufindex;		// current buffer index
    long		x_bufsize;	// in samples
    t_float		*x_inputbuf;
    
    long 		x_sigbufsize;
	t_float 	*x_sigbuffer;
    
    long 		x_nextsigsize;
    
    int		x_blksize;
    float	x_sr;
    
    void	*x_outlet;
} t_sugger;


Symbol *ps_suggerlist;

void sugger_bang(t_sugger *x);
void sugger_int(t_sugger *x, long n);
t_int *sugger_perform(t_int *w);
void sugger_dsp(t_sugger *x, t_signal **sp, short *count);
void sugger_assist(t_sugger *x, void *b, long m, long a, char *s);
void sugger_free(t_sugger *x);
void *sugger_new(double maxms);


void main(void)
{
    setup(&sugger_class, sugger_new, (method)sugger_free, (short)sizeof(t_sugger), 0L, A_DEFFLOAT, 0);
    addmess((method)sugger_dsp, "dsp", A_CANT, 0);
    addbang((method)sugger_bang);
    addint((method)sugger_int);
    addmess((method)sugger_assist,"assist",A_CANT,0);
    
    dsp_initclass();
    
    ps_suggerlist = gensym("suggerlist");
    
    rescopy('STR#',RES_ID);
}



void sugger_bang(t_sugger *x)
{
	Atom at[2];
	
	int i;
	long sigsize = x->x_nextsigsize;
	long size = x->x_bufsize;
	long index = x->x_bufindex;
	long begin = index - sigsize;
	
	float *sug = x->x_sigbuffer;
	float *buf = x->x_inputbuf; // + x->x_bufindex - size;
	
	t_int *myintbuf = (t_int *)buf; 
	t_int *myintsug = (t_int *)sug;
	
	set_zero(sug, sigsize);
	
	if (begin >= 0) {
		//post("begin is positive", 0);
		myintbuf = (t_int *)buf + begin;
		for (i = 0; i < sigsize; i++) 
			myintsug[i] = myintbuf[i];
	}
	else if (begin < 0) {
		//post("going from %ld in %ld steps", (size + begin), -begin);
		myintbuf = (t_int *)buf + (size + begin);
		for (i = 0; i < (-begin); i++) 
			myintsug[i] = myintbuf[i];
		//post("going from 0 in %ld steps", index);	
		myintbuf = (t_int *)buf;
		myintsug = (t_int *)sug - begin;
		for (i = 0; i < index; i++) 
			myintsug[i] = myintbuf[i]; 
	}
	
	SETLONG(&at[0], sigsize);
	SETLONG(&at[1], (long)x->x_sigbuffer);
	
	outlet_anything(x->x_outlet, ps_suggerlist, 2, at);
}




void sugger_int(t_sugger *x, long n)
{
	if (n < 2) {
		error("sugger: sigbuf size cannot be less than two");
		n = 2;
		post("        correcting sigbuf size: %ld", n);
	}

	if (n > x->x_sigbufsize) {
		error("sugger: sigbuf size cannot be greater than max sigbuf size");
		n = x->x_sigbufsize;
		post("        correcting sigbuf size: %ld", n);
	}
	
	//post("sugger: sigbuf size %ld", n);
	x->x_nextsigsize = n;

}

t_int *sugger_perform(t_int *w)		// if one sig inlet is used
{
	int i;
	
	t_float *in = (t_float *)(w[1]);
	t_sugger *x = (t_sugger *)(w[2]);
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

void sugger_dsp(t_sugger *x, t_signal **sp, short *count)  
{
	x->x_sr =  sys_getsr();
	x->x_blksize =  sys_getblksize();
	
	dsp_add(sugger_perform, 3, sp[0]->s_vec, x, sp[0]->s_n);
}


void sugger_assist(t_sugger *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,a,1,2,s);
}

void sugger_free(t_sugger *x)
{	
	if (x->x_inputbuf)
		t_freebytes(x->x_inputbuf,x->x_bufsize * sizeof(float));
		
	if (x->x_sigbuffer)
		t_freebytes(x->x_sigbuffer,x->x_sigbufsize * sizeof(float));
		
	dsp_free((t_pxobject *)x);
}

void *sugger_new(double maxms)
{
	long size, sigsize;
	double sr;
	
    t_sugger *x = (t_sugger *)newobject(sugger_class);
    dsp_setup((t_pxobject *)x,1);
    x->x_outlet = outlet_new((t_pxobject *)x, 0L);
    
    if (maxms < 1) maxms = 1; // set minimum 1 ms sample buffer corrected to 2048 samples
    
    x->x_sr = sr = sys_getsr();
	x->x_blksize =  sys_getblksize();
	
	x->x_bufindex = 0;
	
// allocate buffers ---------------------------------------
    size = maxms * sr * 0.001;
    sigsize = size; // use uncorrected size as default
  	//post("size before correction %ld",size);
   	size += (-size) & (SYS_MAXBLKSIZE - 1);
 	//post("     after correction %ld = 2048 * %ld",size, size/2048);
   	x->x_bufsize = size;
	x->x_sigbufsize = sigsize;
	x->x_nextsigsize = sigsize;

	x->x_inputbuf = t_getbytes(x->x_bufsize * sizeof(float));
	if (!x->x_inputbuf) 
		error("sugger~: out of memory allocating input buffer");
    else
    	set_zero(x->x_inputbuf, x->x_bufsize);
    	
	
	x->x_sigbuffer = t_getbytes(x->x_sigbufsize * sizeof(float));
	if (!x->x_sigbuffer) 
		error("sugger~: out of memory allocating sigbuf buffer");
    else
    	set_zero(x->x_sigbuffer, x->x_sigbufsize);

	
    return (x);
}
