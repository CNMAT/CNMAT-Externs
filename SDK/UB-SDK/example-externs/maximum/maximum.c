/* maximum.c -- output the maximum of a group of numbers ------- */

#include "ext.h"

#define MAXSIZE 256

typedef struct _maximum
{
	struct object m_ob;
	t_atom m_args[MAXSIZE];
	long m_count;
	short m_incount;
	short m_outtype;
	void *m_out;
} t_maximum;

void *maximum_class;

void maximum_compare(t_atom *max, t_atom *newone);
void maximum_bang(t_maximum *x);
void maximum_int(t_maximum *x, long n);
void maximum_in1(t_maximum *x, long n);
void maximum_float(t_maximum *x, double f);
void maximum_ft1(t_maximum *x, double f);
void maximum_list(t_maximum *x, t_symbol *s, short ac, t_atom *av);
void maximum_assist(t_maximum *x, void *b, long m, long a, char *s);
void *maximum_new(t_symbol *s, short ac, t_atom *av);


void main()
{
	setup((t_messlist **)&maximum_class, (method)maximum_new,0L, (short)sizeof(t_maximum), 0L, A_GIMME, 0);
	addbang((method)maximum_bang);
	addint((method)maximum_int);
	addinx((method)maximum_in1,1);
	addfloat((method)maximum_float);
	addftx((method)maximum_ft1,1);
	addmess((method)maximum_list,"list",A_GIMME,0);
	addmess((method)maximum_assist,"assist",A_CANT,0);
}

void maximum_compare(t_atom *max, t_atom *newone)
{
	if (max->a_type==A_NOTHING) {
		*max = *newone;
		return;
	}
	if (max->a_type==A_FLOAT) {
		if (newone->a_type==A_FLOAT) {
			if (newone->a_w.w_float > max->a_w.w_float)
				max->a_w.w_float = newone->a_w.w_float;
		} else {
			if ((float)newone->a_w.w_long > max->a_w.w_float)
				max->a_w.w_float = (float)newone->a_w.w_long;
		}
	} else {
		if (newone->a_type==A_LONG) {
			if (newone->a_w.w_long > max->a_w.w_long)
				max->a_w.w_long = newone->a_w.w_long;
		} else {
			if ((long)newone->a_w.w_float > max->a_w.w_long)
				max->a_w.w_long = (long)newone->a_w.w_float;
		}
	}
}

void maximum_bang(t_maximum *x)
{
	short i;
	t_atom theMax;
	long res;
	double fres;
	
	theMax.a_type = A_NOTHING;
	for (i=0; i < x->m_count; i++)
		maximum_compare(&theMax,x->m_args+i);
	if (x->m_outtype==A_LONG) {
		if (theMax.a_type==A_LONG)
			res = theMax.a_w.w_long;
		else
			res = (long)theMax.a_w.w_float;
		outlet_int(x->m_out,res);
	} else {
		if (theMax.a_type==A_FLOAT)
			fres = theMax.a_w.w_float;
		else
			fres = (float)theMax.a_w.w_long;
		outlet_float(x->m_out,fres);
	}
}

void maximum_int(t_maximum *x, long n)
{
	SETLONG(x->m_args,n);
	maximum_bang(x);
}

void maximum_in1(t_maximum *x, long n)
{
	SETLONG(x->m_args+1,n);
	x->m_count = 2;
}


void maximum_float(t_maximum *x, double f)
{
	SETFLOAT(x->m_args,f);
	maximum_bang(x);
}

void maximum_ft1(t_maximum *x, double f)
{
	SETFLOAT(x->m_args+1,f);
	x->m_count = 2;
}

void maximum_list(t_maximum *x, t_symbol *s, short ac, t_atom *av)
{
	short i;
	
	if (ac > MAXSIZE)
		ac = MAXSIZE;
	for (i=0; i < ac; i++,av++) {
		if (av->a_type==A_LONG)
			SETLONG(x->m_args+i,av->a_w.w_long);
		else if (av->a_type==A_FLOAT)
			SETFLOAT(x->m_args+i,av->a_w.w_float);
	}
	x->m_count = ac;
	maximum_bang(x);
}

void maximum_assist(t_maximum *x, void *b, long m, long a, char *s)
{
	if (m==ASSIST_INLET) {
		switch (a) {
			case 0: sprintf(s,"Compares Left and Right Inlets"); break;
			case 1: sprintf(s,"Value to be Compared"); break;
		}
	}
	else {
		sprintf(s,"Maximum of Left and Right Inlets");
	}
}

void *maximum_new(t_symbol *s, short ac, t_atom *av)
{
	t_maximum *x;
	
	x = (t_maximum *)newobject(maximum_class);
	x->m_count = 2;
	if (ac) {
		x->m_args[1] = *av;
		if (av->a_type==A_LONG) {
			x->m_args[0].a_type = x->m_outtype = A_LONG;
			x->m_out = intout(x);
			x->m_args[0].a_w.w_long = 0;
			intin(x,1);
		} else if (av->a_type==A_FLOAT) {
			x->m_args[0].a_type = x->m_outtype = A_FLOAT;
			x->m_out = floatout(x);
			x->m_args[0].a_w.w_float = 0;
			floatin(x,1);
		} else { 
			x->m_outtype = A_LONG;
			intin(x,1);
			x->m_out = intout(x);
			SETLONG(x->m_args+1,0L);
			SETLONG(x->m_args,0L);
		}
	} else {
		x->m_outtype = A_LONG;
		intin(x,1);
		x->m_out = intout(x);
		SETLONG(x->m_args+1,0L);
		SETLONG(x->m_args,0L);
	}
	return x;
}

