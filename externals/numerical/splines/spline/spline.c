// cubic spline interpolation

#include "ext.h"
#include "ext_common.h"

#define MAXSIZE 256

typedef struct t_spline
{
	Object x_ob;
	
	float x_x[MAXSIZE];
	float x_y[MAXSIZE];
	float x_y2[MAXSIZE];
	float x_u[MAXSIZE];
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

void spline_eval(t_spline *obj);
void spline_float(t_spline *x, double f);
void spline_list(t_spline *x, Symbol *s, int ac, Atom *av);
void spline_deriv(t_spline *x, Symbol *s, int ac, Atom *av);
void *spline_new(long n); 
void spline_output(t_spline *x);

void main(fptr *f)
{		
	setup(&spline_class, spline_new, 0L, (short)sizeof(t_spline), 0L, A_DEFLONG, 0);
	addfloat((method)spline_float);
    addmess((method)spline_list, "list", A_GIMME, 0);
    addmess((method)spline_deriv, "deriv", A_GIMME, 0);
}

void spline_eval(t_spline *obj)
{
	float *x = obj->x_x;
	float *y = obj->x_y;
	float *y2 = obj->x_y2;
	float *u = obj->x_u;
	int n = obj->x_maxlen;
	
	float yp1 = obj->x_yp1; // these should eventually be user-specified
	float ypn = obj->x_ypn;
	
	int i,k;
	float p,qn,sig,un;

	if (yp1 > 0.99e30)	// The lower boundary condition is set either to be "natural" 
		y2[0]=u[0]=0.0;
	else { 				// or else to have a specified first derivative.
		y2[0] = -0.5;
		u[0]=(3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
	}
	
	// This is the decomposition loop of the tridiagonal al gorithm. 
	// y2 and u are used for tem porary storage of the decomposed factors.
	for (i=1;i<n-1;i++) { 
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	
	if (ypn > 0.99e30)	// The upper boundary condition is set either to be "natural" 
		qn=un=0.0;
	else { 				// or else to have a specified first derivative.
		qn=0.5;
		un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
	}
	
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
	for (k=n-2;k>=0;k--) // This is the backsubstitution loop of the tridiagonal algorithm. 
		y2[k]=y2[k]*y2[k+1]+u[k];

}

void spline_float(t_spline *obj, double x)
{
	float *xa = obj->x_x;
	float *ya = obj->x_y;
	float *y2a = obj->x_y2;
	int n = obj->x_maxlen;
	
	int klo,khi,k;
	float h,b,a;

	// We will find the right place in the table by means of bisection. This is optimal 
	//   if sequential calls to this routine are at random values of x. If sequential calls 
	//   are in order, and closely spaced, one would do better to store previous values 
	//   of klo and khi and test if they remain appropriate on the next call.
	klo=1; 
	khi=n;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (xa[k-1] > x) khi=k;
		else klo=k;
	} // klo and khi now bracket the input value of x.

	klo--;
	khi--;
	
	h=xa[khi]-xa[klo];
	if (h == 0.0) post("Bad xa input to routine splint",0); // The xa's must be distinct. 

	a=(xa[khi]-x)/h;
	b=(x-xa[klo])/h; //Cubic spline polynomial is now evaluated.
	obj->x_val=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
	
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
	
	if(x->x_maxlen) spline_eval(x);
}

void spline_deriv(t_spline *x, Symbol *s, int ac, Atom *av)
{
	if (x->x_inletnum == 0) {
		if (ac > 1 && av[1].a_type == A_FLOAT)
			x->x_ypn = av[1].a_w.w_float;
		else if (ac > 1 &&  av[1].a_type == A_LONG)
			x->x_ypn = (float)av[1].a_w.w_long;
		else
			x->x_ypn = 0.0;
			
		if (ac > 0 && av[1].a_type == A_FLOAT)
			x->x_yp1 = av[1].a_w.w_float;
		else if (ac > 0 &&  av[1].a_type == A_LONG)
			x->x_yp1 = (float)av[1].a_w.w_long;
		else
			x->x_yp1 = 0.0;
			
		if(x->x_maxlen) spline_eval(x);
	}
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
		x->x_y2[i] = 0.;
		x->x_u[i] = 0.;
	}
	x->x_xlen = x->x_ylen = x->x_maxlen = 0;
	x->x_yp1 = 0.;
	x->x_ypn = 0.;
	
	return (x);
}



