// cubic spline interpolation

#include "ext.h"
#include "ext_common.h"
#include <math.h>

#define MAXSIZE 256

typedef struct t_spline
{
	Object x_ob;
	
	float x_x[MAXSIZE];
	float x_y[MAXSIZE];
	float x_c[MAXSIZE];
	float x_d[MAXSIZE];
	int x_xlen;
	int x_ylen;
	int x_maxlen;
	
	float x_yp1; // first derivative values
	float x_ypn;
	
	float x_val;
	
    long  x_inletnum;
	void *x_proxy;
	
	void *x_out;
	
} t_spline;

void *spline_class;

void spline_float(t_spline *x, double f);
void spline_list(t_spline *x, Symbol *s, int ac, Atom *av);
void *spline_new(long n); 
void spline_output(t_spline *x);

void main(fptr *f)
{		
	setup(&spline_class, spline_new, 0L, (short)sizeof(t_spline), 0L, A_DEFLONG, 0);
	addfloat((method)spline_float);
    addmess((method)spline_list, "list", A_GIMME, 0);
}



void spline_float(t_spline *obj, double x)
{

	int i,m,ns=0;
	float den,dif,dift,ho,hp,w;
	float y;
	
	float *xa = obj->x_x;
	float *ya = obj->x_y;
	float *c = obj->x_c;
	float *d = obj->x_d;
	int n = obj->x_maxlen;

	dif=fabs(x-xa[0]);

	for (i=0;i<n;i++) {		// Here we find the index ns of the closest table entry,
		if ( (dift=fabs(x-xa[i])) < dif) {
			ns=i;
			dif=dift;
		}
		c[i]=ya[i];			//and initialize the tableau of c's and d's.
		d[i]=ya[i];
	}
	y=ya[ns--]; 			//This is the initial approximation to y.
	
	for (m=1;m<=n;m++) { 		// For each column of the tableau,
		for (i=1;i<=n-m;i++) { 	// we loop over the current c's and d's and update them. 
			ho=xa[i-1]-x;
			hp=xa[(i-1)+(m-1)]-x;
			w=c[(i-1)+1]-d[i-1];
			if ( (den=ho-hp) == 0.0) post("Error in routine polint", 0);
			// This error can occur only if two input xa's are (to within roundoff) identical.
			den=w/den;
			d[i-1]=hp*den; // Here the c's and d's are updated.
			c[i-1]=ho*den;
		}
		y += (2*(ns+1) < (n-m) ? c[ns] : d[(ns--)-1]);
	}
	obj->x_val = y;
	
	outlet_float(obj->x_out, obj->x_val);
	
}

void spline_list(t_spline *x, Symbol *s, int ac, Atom *av)
{
	int i;
	if (x->x_inletnum == 0) {
		for (i=0; i<ac; i++) {
			if (av[i].a_type == A_FLOAT)
				x->x_x[i] = av[i].a_w.w_float;
			else if (av[i].a_type == A_LONG)
				x->x_x[i] = (float)av[i].a_w.w_long;
			else
				x->x_x[i] = 0.0;
		}
		x->x_xlen = ac;
	}
	else if (x->x_inletnum == 1) {
		for (i=0; i<ac; i++) {
			if (av[i].a_type == A_FLOAT)
				x->x_y[i] = av[i].a_w.w_float;
			else if (av[i].a_type == A_LONG)
				x->x_y[i] = (float)av[i].a_w.w_long;
			else
				x->x_y[i] = 0.;
		}
		x->x_ylen = ac;
	}
	x->x_maxlen = MIN(x->x_xlen, x->x_ylen);
	
	//if(x->x_maxlen) spline_eval(x);
}


void *spline_new(long n)
{
	int i;
	t_spline *x;

	x = (t_spline *)newobject(spline_class);
	
	x->x_proxy = proxy_new(x, 1L, &x->x_inletnum); // right proxy inlet
	x->x_out = floatout(x);
	
	for(i=0; i<MAXSIZE; i++) {
		x->x_x[i] = 0.;
		x->x_y[i] = 0.;
		x->x_c[i] = 0.;
		x->x_d[i] = 0.;
	}
	x->x_xlen = x->x_ylen = x->x_maxlen = 0;
	x->x_yp1 = 0.;
	x->x_ypn = 0.;
	
	return (x);
}



