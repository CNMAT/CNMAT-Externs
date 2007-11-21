// lswitch  an object to spread bangs around   Oct 1994   
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

typedef struct lswitch
{
	struct object b_ob;
	void *proxy[32];
	long inletNumber;
	long activeInlet;
	long controlInlet;
	void *out;
	int numProxies;
} Lswitch;

void *lswitch_new();
void lswitch_free();
void lswitch_bang();
void lswitch_int();
void lswitch_float();
void lswitch_List();
void lswitch_sym();
void lswitch_assist();
void lswitch_setLength();
void lswitch_reset();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class, (method)lswitch_new, (method)lswitch_free, (short)sizeof(Lswitch), 0L, A_GIMME, 0);
	addbang((method)lswitch_bang);
	addint((method)lswitch_int);
	addfloat((method)lswitch_float);
	addmess((method)lswitch_List, "list",A_GIMME,0);
	addmess((method)lswitch_assist, "assist", A_CANT,0);
	addmess((method)lswitch_sym, "anything",A_GIMME, 0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void lswitch_int(x,n)
Lswitch *x;
long n;
{
	if(check_inlet(x) == x->controlInlet)
	{
		if(n >= 0 && n < x->controlInlet)  
			x->activeInlet = n;
		
	}
	else
		if(check_inlet(x) == x->activeInlet)
			outlet_int(x->out,n);
	
	
}

void lswitch_float(x,n)
Lswitch *x;
double n;
{
	
	if(check_inlet(x) == x->activeInlet)
		outlet_float(x->out,n);
	
	
}

void lswitch_List(x,s,argc,argv)
Lswitch *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(check_inlet(x) == x->activeInlet)
	outlet_list(x->out,0L,argc,argv);
}

void lswitch_sym(x,s,argc,argv)
Lswitch *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	
	if(check_inlet(x) == x->activeInlet)
	outlet_anything(x->out,s,argc,argv);
	
	
	
}

void lswitch_bang(x)
Lswitch *x;
{
	if(check_inlet(x) == x->activeInlet)
	outlet_bang(x->out);
	
}




void lswitch_assist(x,b,msg,arg,dst)
Lswitch *x;
void *b;
long  msg,arg;
char *dst;
{
	char theString[16];
	
	
		theString[0] = 'I';
		theString[1] = 'n';
		theString[2] = 'l';
		theString[3] = 'e';
		theString[4] = 't';
		theString[5] = ' ';
		theString[6] = ' ';
		theString[7] = 0;
		theString[8] = 0;
		
	if(msg == ASSIST_INLET)
	{
		if(arg == x->controlInlet)
			strcpy(dst,"Select active input");
		else{
			if(arg < 10)
				theString[6] = (arg % 10) + 48;
			else{
				theString[7] = (arg % 10) + 48;
				theString[6] = (arg / 10) + 48;
			}
			strcpy(dst,theString);
		
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		strcpy(dst,"Selected Data.");
	}
}


void *lswitch_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lswitch *x;
	long i;
	
	x = (Lswitch *)newobject(class);
	
	x->activeInlet = 0;
	x->out = outlet_new(x,0L);
	if(ac )
	{
		x->controlInlet = theArgs[0].a_w.w_long;
		if(x->controlInlet > 32) x->controlInlet = 32;
		x->numProxies = x->controlInlet;
		
		for(i = x->numProxies; i > 0 ; --i)
		{
		x->proxy[i-1] = proxy_new(x,i,&x->inletNumber);	
		
		}
		
	}
	else
	{
		x->proxy[1] = proxy_new(x,2L,&x->inletNumber);
		x->proxy[0] = proxy_new(x,1L,&x->inletNumber);
		x->numProxies = 2;
		x->controlInlet = 2;
	}
	
	if(ac > 1) x->activeInlet = theArgs[1].a_w.w_long;
	
	
	return(x);
}

void lswitch_free(x)
Lswitch *x;
{
	int i;
	
	for( i = 0; i < x->numProxies; ++i)
		freeobject(x->proxy[i]);
}

int check_inlet(x)
Lswitch *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}