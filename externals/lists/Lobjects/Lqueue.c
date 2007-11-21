// lqueue file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1996, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"


#define QWRAP 2047
#define QSIZE 2048
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lqueue
{
	struct object ob;
	void *clock;
	Atom theQ[QSIZE];
	int nextout, nextslot, clockOff;
	long delay;
	void *outR;
	void *outL;
} Lqueue;


void *lqueue_new();
void lqueue_free();
void  lqueue_List();
void  lqueue_doInt();
void  lqueue_doFloat();
void  lqueue_Int();
void  lqueue_Float();
void  lqueue_Sym();
void  lqueue_anything();
void lqueue_timeout();
void lqueue_setdel();
void lqueue_clear();
void lqueue_assist();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lqueue_new,(method)lqueue_free,(short)sizeof(struct lqueue),0L,A_GIMME,0);
	addint((method)lqueue_doInt);
	addinx((method)lqueue_setdel,1);
	addfloat((method)lqueue_doFloat);
	addmess((method)lqueue_List, "list",A_GIMME,0);
	addmess((method)lqueue_anything, "anything",A_GIMME,0);
	addmess((method)lqueue_assist, "assist",A_CANT,0);
	addmess((method)lqueue_clear, "clear",0);
	finder_addclass("Notes","Lqueue");
	

s = gensym("goslugsgo");
l = gensym("lobjectsbypqe");
if(s->s_thing != (struct object *)l){
post("Lobjects for Max © Regents of the University of California.");
s->s_thing = (struct object *)l;}
return 0;
}

void lqueue_timeout(x)
Lqueue *x;
{
	long n;
	if(x->nextslot == x->nextout) // timer was started by a single item
	{
		x->clockOff = 1;
		outlet_bang(x->outR);  
		return;
	}
	switch(x->theQ[x->nextout].a_type){
	 	case A_LONG:
			n = x->theQ[x->nextout].a_w.w_long;
			//post("int  %ld",n);
			outlet_int(x->outL,n);
			break;
		case A_FLOAT:
			outlet_float(x->outL,x->theQ[x->nextout].a_w.w_float);
			break;
		case A_SYM:
			outlet_anything(x->outL,x->theQ[x->nextout].a_w.w_sym,0,NIL);
			break;
		}
	++x->nextout;
	x->nextout &= QWRAP;
	if(x->nextslot == x->nextout)
	{
		x->clockOff = 1;
		outlet_bang(x->outR);
	}
	else
	{
		x->clockOff = 0;
		clock_delay(x->clock,x->delay);
	}
	
}

void lqueue_clear(x)
Lqueue *x;
{
	clock_unset(x->clock);
	x->nextslot = x->nextout;
	x->clockOff = 1;
	//outlet_bang(x->outR);

}

void  lqueue_doInt(x,n)
Lqueue *x;
long n;
{
	if(x->clockOff){
		outlet_int(x->outL,n);
		x->clockOff = 0;
		clock_delay(x->clock,x->delay);
	}else
		lqueue_Int(x,n);
}
	
	
void  lqueue_Int(x,n)
Lqueue *x;
long n;
{
	
	x->theQ[x->nextslot].a_type = A_LONG;
	x->theQ[x->nextslot].a_w.w_long = n;
	++x->nextslot;
	x->nextslot &= QWRAP;
	if(x->nextslot == x->nextout)
		post("Lqueue Overflow!");
	if(x->clockOff){
		clock_delay(x->clock,x->delay);
		x->clockOff =0;
		}
}


void  lqueue_Sym(x,s)
Lqueue *x;
Symbol * s;
{
	x->theQ[x->nextslot].a_type = A_SYM;
	x->theQ[x->nextslot].a_w.w_sym = s;
	++x->nextslot;
	x->nextslot &= QWRAP;
	if(x->nextslot == x->nextout)
		post("Lqueue Overflow!");
	if(x->clockOff){
		clock_delay(x->clock,x->delay);
		x->clockOff =0;
		}
}

void  lqueue_doFloat(x,n)
Lqueue *x;
float n;
{
	if(x->clockOff){
		outlet_float(x->outL,n);
		x->clockOff = 0;
		clock_delay(x->clock,x->delay);
	}else
		lqueue_Float(x,n);
}

void  lqueue_Float(x,n)
Lqueue *x;
float n;
{
	x->theQ[x->nextslot].a_type = A_FLOAT;
	x->theQ[x->nextslot].a_w.w_float = n;
	++x->nextslot;
	x->nextslot &= QWRAP;
	if(x->nextslot == x->nextout)
		post("Lqueue Overflow!");
	if(x->clockOff){
		clock_delay(x->clock,x->delay);
		x->clockOff =0;
		}
}

void lqueue_setdel(x,n)
Lqueue *x;
long n;
{
	
	if(n > 0)
		x->delay = n;
	
}

void  lqueue_List(x,s,ac,theArgs)
Lqueue  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int i=0;
	if(x->clockOff){
		i = 1;
		switch(theArgs[0].a_type){
			case A_LONG:
				outlet_int(x->outL,theArgs[0].a_w.w_long);
				break;
			case A_FLOAT:
				outlet_float(x->outL,theArgs[0].a_w.w_float);
				break;
			case A_SYM:
				outlet_anything(x->outL,theArgs[0].a_w.w_sym,0,NIL);
				break;
		}
	}
	for(;i<ac;++i)
	{
		switch(theArgs[i].a_type){
			case  A_LONG:
				lqueue_Int(x,theArgs[i].a_w.w_long);
				break;
			case A_FLOAT:
				lqueue_Float(x,theArgs[i].a_w.w_float);
				break;
			case A_SYM:
				lqueue_Sym(x,theArgs[i].a_w.w_sym);
				break;
		}
	}
}

void  lqueue_anything(x,s,ac,theArgs)
Lqueue  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int i;
	if(x->clockOff)
		outlet_anything(x->outL,s,0,NIL);
	else
		lqueue_Sym(x,s);
	for(i=0;i<ac;++i)
	{
		if(theArgs[i].a_type == A_LONG)
			lqueue_Int(x,theArgs[i].a_w.w_long);
		if(theArgs[i].a_type == A_FLOAT)
			lqueue_Float(x,theArgs[i].a_w.w_float);
		if(theArgs[i].a_type == A_SYM)
			lqueue_Sym(x,theArgs[i].a_w.w_sym);
	}
}

void lqueue_assist(x,b,msg,arg,dst)
Lqueue *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Numbers to slow");
			break;
			
			default:
			strcpy(dst,"Delay between output");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"data at steady rate");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
}


void  *lqueue_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lqueue  *x;
	x = (Lqueue *)newobject(class);
	intin(x,1);
	x->outR = bangout(x);
	x->outL = outlet_new(x,0);
	x->clock = clock_new(x,(method)lqueue_timeout);
	x->nextout = 0;
	x->nextslot = 0;
	x->delay = 5;
	x->clockOff = 1;
	if(ac)
	{
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0)		
		x->delay = theArgs[0].a_w.w_long;		
	}
	return(x);
}

void lqueue_free(x)
Lqueue *x;
{
	
	clock_free(x->clock);
}