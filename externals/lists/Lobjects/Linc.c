// linc file for Max object
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

typedef struct linc
{
	struct object ob;
	void *proxy;
	long inletNumber;
	//Atom theList[MAXSIZE];
	//Atom outList[MAXSIZE];
	long value;
	long initialvalue;
	long highlimit;
	long lowlimit;
	int justCleared;
	int stepsize;
	void *out;
} Linc;


void *linc_new();
void linc_free();
void  linc_inc();
void linc_dec();
void  linc_setConstant();
void  linc_floatConstant();
void  linc_clear();
void  linc_set();
void  linc_min();
void  linc_max();
void linc_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)linc_new,(method)linc_free,(short)sizeof(struct linc),0L,A_GIMME,0);
	addint((method)linc_setConstant);
	addfloat((method)linc_floatConstant);
	addmess((method)linc_inc, "inc",A_GIMME,0);
	addmess((method)linc_dec, "dec",A_GIMME,0);
	addmess((method)linc_set, "set",A_DEFLONG,0);
	addmess((method)linc_min, "min",A_DEFLONG,0);
	addmess((method)linc_max, "max",A_DEFLONG,0);
	addmess((method)linc_assist, "assist",A_CANT,0);
	addmess((method)linc_clear, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void linc_inc(x,s,argc,argv)
Linc *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	
	
	if(! x->justCleared)
	{
		if(argc && argv[0].a_type == A_LONG) 
			x->value += argv[0].a_w.w_long;
		else
			x->value += x->stepsize;
	}
	if(x->value > x->highlimit || x->value < x->lowlimit )
		x->value = x->highlimit;
	outlet_int(x->out,x->value);
	x->justCleared = 0;
	
}

void linc_dec(x,s,argc,argv)
Linc *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	
	
	if(! x->justCleared)
	{
		if(argc && argv[0].a_type == A_LONG) 
			x->value -= argv[0].a_w.w_long;
		else
			x->value -= x->stepsize;
	}
	if(x->value > x->highlimit || x->value < x->lowlimit)
		x->value = x->lowlimit;
	outlet_int(x->out,x->value);
	x->justCleared = 0;
	
}

void  linc_clear(x)
Linc *x;
{
	
	
	
	x->value = x->initialvalue;
	x->justCleared = 1;
	
}

void  linc_set(x,n)
Linc *x;
long n;
{
	
	
	
	x->value = n;
	x->justCleared = 1;
	
}

void  linc_min(x,n)
Linc *x;
long n;
{
	
	
	
	x->lowlimit = n;
	
}

void  linc_max(x,n)
Linc *x;
long n;
{
	
	
	
	x->highlimit = n;
	
}


void  linc_setConstant(x,n)
Linc *x;
long n;
{
	
	
	if(check_inlet(x))
	{
		x->stepsize = n;
		x->inletNumber = 0;
	}
	else
	{
		x->value = n;
		outlet_int(x->out,x->value);
		x->justCleared = 0;
	}
	
}

void  linc_floatConstant(x,n)
Linc *x;
double n;
{	
	if(! check_inlet(x))
	{
		x->value = (long)n;
		outlet_int(x->out,x->value);
		x->justCleared = 0;
	}
	
}



void linc_assist(x,b,msg,arg,dst)
Linc *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"inc,dec or int to set");
			break;
			
			default:
			strcpy(dst,"stepsize");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"current value");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
}


void  *linc_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Linc  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Linc *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->value = 0;
	x->initialvalue = 0;
	x->stepsize = 1;
	x->justCleared = 1;
	x->highlimit = 0x7fffffff;
	x->lowlimit = 0x80000000;
	
	switch(ac)
	{
	case 0:
	break;
	
	case 1:					
	if(theArgs[0].a_type == A_LONG)
		x->stepsize = theArgs[0].a_w.w_long;
		break;
		
	case 4:
	if(theArgs[3].a_type == A_LONG)
		x->highlimit = theArgs[3].a_w.w_long;
	case 3:
	if(theArgs[2].a_type == A_LONG)
		x->lowlimit = theArgs[2].a_w.w_long;// note: deliberate fall through
	case 2:
	if(theArgs[0].a_type == A_LONG)
	{
		x->value = theArgs[0].a_w.w_long;
		x->initialvalue = x->value;
	}
	if(theArgs[1].a_type == A_LONG)
		x->stepsize = theArgs[1].a_w.w_long;
		break;
	}
	return(x);
}

void linc_free(x)
Linc *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Linc *x;
{
	return x->inletNumber;  // classic only
//	return proxy_getinlet((t_object *)x);  // OS X only
}