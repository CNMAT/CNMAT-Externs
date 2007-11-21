// lgate  an object to spread bangs around   Oct 1994   
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

typedef struct lgate
{
	struct object b_ob;
	void *proxy;
	long inletNumber;
	long outGate;
	void *out[32];
	int numOuts;
} Lgate;

void *lgate_new();
void lgate_free();
void lgate_bang();
void lgate_int();
void lgate_float();
void lgate_List();
void lgate_sym();
void lgate_assist();
void lgate_setLength();
void lgate_reset();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class, (method)lgate_new, (method)lgate_free, (short)sizeof(Lgate), 0L, A_GIMME, 0);
	addbang((method)lgate_bang);
	addint((method)lgate_int);
	addfloat((method)lgate_float);
	addmess((method)lgate_List, "list",A_GIMME,0);
	addmess((method)lgate_assist, "assist", A_CANT,0);
	addmess((method)lgate_sym, "anything",A_GIMME, 0);
	//finder_addclass("Control", "lgate");
	

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
}



void lgate_int(x,n)
Lgate *x;
long n;
{	
	if(check_inlet(x))
	{
		if((n < x->numOuts) && (n >= 0))
			x->outGate = n;
	}
	else
		outlet_int(x->out[x->outGate],n);
	
	
}

void lgate_float(x,n)
Lgate *x;
double n;
{	
	if(check_inlet(x))
	{
	// ingore it
	}
	else
	outlet_float(x->out[x->outGate],n);
	
	
}

void lgate_List(x,s,argc,argv)
Lgate *x;
Symbol *s;
int argc;
Atom *argv;
{	
	if(check_inlet(x))
	{
	// ingore it
	}
	else
	outlet_list(x->out[x->outGate],0L,argc,argv);
	
	
	
}

void lgate_sym(x,s,argc,argv)
Lgate *x;
Symbol *s;
int argc;
Atom *argv;
{	
	if(check_inlet(x))
	{
	// ingore it
	}
	else
	outlet_anything(x->out[x->outGate],s,argc,argv);
	
	
	
}

void lgate_bang(x)
Lgate *x;
{	
	outlet_bang(x->out[x->outGate]);
	
}




void lgate_assist(x,b,msg,arg,dst)
Lgate *x;
void *b;
long  msg,arg;
char *dst;
{
	char theString[16];
	
	
		theString[0] = 'O';
		theString[1] = 'u';
		theString[2] = 't';
		theString[3] = ' ';
		theString[4] = '0';
		theString[5] = 0;
		theString[6] = 0;
		
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to pass through.");
			break;
			
			default:
			strcpy(dst,"Outlet to send data to.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		if(arg < 10)
		theString[4] = (arg % 10) + 48;
		else{
		theString[5] = (arg % 10) + 48;
		theString[4] = (arg / 10) + 48;
		}
		strcpy(dst,theString);
		
	}
	
	
}


void *lgate_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lgate *x;
	int i;
	
	x = (Lgate *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outGate = 0;
	
	if(ac )
	{
		x->numOuts = theArgs[0].a_w.w_long;
		if(x->numOuts > 32) x->numOuts = 32;
		for(i = x->numOuts - 1; i >= 0 ; --i)
		{
			x->out[i] = outlet_new(x,0L);
		
		}
		
	}
	else
	{
		x->out[1] = outlet_new(x,0L);
		x->out[0] = outlet_new(x,0L);
		x->numOuts = 2;
		
	}
	 if(ac > 1) x->outGate =theArgs[1].a_w.w_long;
	
	
	return(x);
}

void lgate_free(x)
Lgate *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lgate *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}