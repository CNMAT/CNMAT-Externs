/* 32bit -	read a float as an int and vice versa 				*/
/* 			Spring 1996, Richard Dudas - CW 68K/PPC Summer 96 	*/
// removed 68k-specific junk and updated code Oct 02

#include "ext.h"
#include "ext_common.h"

#define RES_ID 14995

typedef struct fconv
{
	t_object f_ob;
	int f_last;  		// 0 for int, 1 for float 
	double f_float;
	long f_long;
	void *f_outlet;
} t_fconv;

void *fconv_class;

void fconv_bang(t_fconv *x);
void fconv_int(t_fconv *x, long n);
void fconv_float(t_fconv *x, double n);
void fconv_assist(t_fconv *x, void *b, long m, long a, char *s);
void *fconv_new(long n);

/*==========================================================================*/

int main(void)
{	
	setup((t_messlist **)&fconv_class, (method)fconv_new, 0L, (short)sizeof(t_fconv), 0L, A_DEFLONG, 0);
	
	addbang((method)fconv_bang);
	addfloat((method)fconv_float);
	addint((method)fconv_int);
	addmess((method)fconv_assist,"assist",A_CANT,0);
	
	return 0;
}

// bang message is useful for debugging
void fconv_bang(t_fconv *x)
{
	if(x->f_last) {
		outlet_int(x->f_outlet, x->f_long);
		//post("float %f -> long %ld", x->f_float, x->f_long);
	}
	else {
		outlet_float(x->f_outlet, x->f_float);
		//post("long %ld -> float %f", x->f_long, x->f_float);
	}
}

void fconv_int(t_fconv *x, long n)
{
	float *ptf;
	
	ptf = (float*)&n;
	x->f_last = 0;
	x->f_long = n;
	x->f_float = *ptf;
	
	outlet_float(x->f_outlet, x->f_float);
}

void fconv_float(t_fconv *x, double n)
{
	long *pt;
	float nn;
	
	nn = (float)n;
	pt = (long*)&nn;
	x->f_last = 1;
	x->f_float = n;
	x->f_long = *pt;
	
	outlet_int(x->f_outlet, x->f_long);
}

void fconv_assist(t_fconv *x, void *b, long m, long a, char *s)
{
	assist_string(RES_ID,m,0L,1,2,s,a);
}

void *fconv_new(long n)
{
	t_fconv *x;

	x = (t_fconv *)newobject(fconv_class);
	
	x->f_outlet = floatout(x);
	
	x->f_last = 0;
	x->f_long = 0;
	x->f_float = 0.0;
	
	return(x);
}

