// lround file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lround
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	int outType;
	double precision;
	void *out;
} Lround;


void *lround_new();
void lround_free();
void  lround_List();
void  lround_Constant();
void  lround_FloatConstant();
void lround_dumpList();
void lround_assist();
double roundoff();
char *strcpy();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;

	setup((t_messlist **)&class,(method)lround_new,(method)lround_free,(short)sizeof(struct lround),0L,A_GIMME,0);
	addint((method)lround_Constant);
	addfloat((method)lround_FloatConstant);
	addbang((method)lround_dumpList);
	addmess((method)lround_List, "list",A_GIMME,0);
	addmess((method)lround_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lround");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

double roundoff(x,n)
Lround *x;
double n;
{
	
	long temp;
	double result;
	long odd;
	
	if(n >= 0)
	{
	n = n * x->precision;
	temp = (long)n;
	odd = temp % 2;
	n = n - (double)temp;
	if(n > 0.5) ++temp;
	if(n == 0.5) temp += odd;
	result = (double)temp / x->precision;
	}
	else
	{
	n = - n;
	n = n * x->precision;
	temp = (long)n;
	odd = temp % 2;
	n = n - (double)temp;
	if(n > 0.5) ++temp;
	if(n == 0.5) temp += odd;
	result = (double)temp / x->precision;
	result = - result;
	}
	
	return(result);

}

void lround_List(x,s,argc,argv)
Lround *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long temp;
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	for(i=0;i<argc;++i)
		{
			
				switch(argv[i].a_type)
				{
				case A_LONG:
				temp = (long)roundoff(x,(double)argv[i].a_w.w_long);
				if(x->outType == 0)
					{
					x->outList[i].a_type = A_LONG;
					x->outList[i].a_w.w_long = temp;
					}
				else
					{
					x->outList[i].a_type = A_FLOAT;
					x->outList[i].a_w.w_float = (double)temp;
					}
					break;
					
				case A_FLOAT:
				if(x->outType == 0)
					{
					x->outList[i].a_type = A_LONG;
					x->outList[i].a_w.w_long = (long)roundoff(x,argv[i].a_w.w_float);
					}
					else
					{
					x->outList[i].a_type = A_FLOAT;
					x->outList[i].a_w.w_float = roundoff(x,argv[i].a_w.w_float);
					}
					break;
					
				default:
					post("Invalid List Member in Lround");
				}
		}
		x->outsize = argc;
		outlet_list(x->out,0L,argc,x->outList);
	
	
}

void  lround_Constant(x,n)
Lround *x;
long n;
{
	long temp;
	
	
	temp = (long)roundoff(x,(double)n);
	if(x->outType == 0)
		{
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = temp;
		}
	else
		{
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = (double)temp;
		}
		x->outsize = 1;
		lround_dumpList(x);
		
	
	
}


void  lround_FloatConstant(x,n)
Lround *x;
double n;
{
	double tempf;
	
	
	tempf = roundoff(x,n);
	if(x->outType == 0)
		{
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = (long)tempf;
		}
	else
		{
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = tempf;
		}
	x->outsize = 1;
	lround_dumpList(x);
		
	
}

void lround_dumpList(x)
Lround *x;
{
	
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
				else
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
			
	
}




void lround_assist(x,b,msg,arg,dst)
Lround *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Numbers to round off");
			break;
			
			default:
			strcpy(dst,"List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Rounded Values");//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *lround_new(s,argc,theArgs)
Symbol *s;
int argc;
Atom *theArgs;
{

	Lround  *x;
	int i;
	int arg;
	
	if(argc)
		arg = (int)theArgs[0].a_w.w_long;
	else
		arg = 0;
	
	if(arg > 8) arg = 8;
	if(arg < -8) arg = -8;
	x = (Lround *)newobject(class);
	x->outType = argc;
	x->out= outlet_new(x,0L);
	x->outsize = 1;
	x->precision = 1;
	if(arg > 0)
	{
	for(i=0;i < arg;++i)
		{
		x->precision = 10 * x->precision;
		}
	}
	if(arg < 0)
	for(i=arg;i < 0;++i)
	{
		{
		x->precision =  x->precision / 10;
		}
	}
	
	
	return(x);
}

void lround_free(x)
Lround *x;
{
	
	

}