
// loop file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256

typedef struct loop {
struct object ob;
void *proxy1;
void *proxy2;
void *outL;
void *outR;
void *theQ;
void *nowBang;
long inletNumber;
int count;
int start;
int end;
int state;
} Loop;

void *class;

void *loop_new(), doList(), doInt(), doFloat(), doBang();
void loop_assist();
void loop_free();
void setStart();
void setEnd();
void pause();
void stop();
void start();
void restart();
void doCount();
void stepLoop();
void bangRight();
int check_inlet();


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup ((t_messlist **) &class, (method)loop_new, 0L,(short) sizeof(Loop), 
													0L, A_GIMME,0);
	addmess((method)doList, "list",A_GIMME,0);
	addmess ((method)pause, "pause",0);
	addmess ((method)stop, "stop",0);
	addmess ((method)start, "start",0);
	addmess ((method)restart, "continue",0);
	addint((method)doInt);
	addfloat((method)doFloat);
	addbang((method)doBang);
	addmess((method)loop_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void doList (x,s,argc,argv)
Loop *x;
Symbol *s;
int argc;
Atom *argv;
{
	switch(argc)
	{
	case 0:
	break;
	
	case 1:
	if(argv[0].a_type == A_LONG)
		doInt(x,argv[0].a_w.w_long);
	else
		{
		if(argv[0].a_type == A_FLOAT)
			doFloat(x,argv[0].a_w.w_long);
		}
	break;
	
	default:
	x->start = argv[0].a_w.w_long;
	x->end = argv[1].a_w.w_long;
	}
	if(check_inlet(x) == 0)
		doBang(x);
	x->inletNumber = 0;
	
}

void doFloat (x,n)
Loop *x;
double n;
{	
	doInt(x,(long)n);
	
	
}

void doInt (x,n)
Loop *x;
long n;
{
	switch(check_inlet(x))
	{
	case 0:
	x->state = 1;
	x->start = n;
	x->count = n;
	stepLoop(x);
	break;
	
	case 1:
	x->start = n;
	break;
	
	case 2:
	x->end = n;
	break;
	}
	x->inletNumber = 0;
	
}

void doBang(x)
Loop *x; 
{
	switch(check_inlet(x))
	{
	case 0:				//setup to start
	x->state = 1;
	x->count = x->start;
	// deliberate fall through
	
	case 2:      // count once
	if(x->state)
		if(x->count < x->end)
			{
			//post("count  %d",x->count);
			qelem_set(x->theQ);
			}
			else
			{
			qelem_set(x->nowBang);
			x->state = 0;
			}
	}
	x->inletNumber = 0;
	
}


void stepLoop(x)
Loop *x;
{
	if(x->count < x->end)
			{
			//post("count  %d",x->count);
			qelem_set(x->theQ);
			}
			else
			{
			qelem_set(x->nowBang);
			x->state = 0;
			}

}


void doCount(x)
Loop *x;
{
	outlet_int(x->outL, (long)x->count++);
	
}

void bangRight(x)
Loop *x;
{

	
	outlet_bang(x->outR);
	
}

void pause(x)
Loop *x;
{
	
	x->state = 0;
	
}

void stop(x)
Loop *x;
{
	
	if(x->state)
		outlet_bang(x->outR);
	x->state = 0;
	x->count = x->start;
	
	
}

void start(x)
Loop *x;
{
	
	x->count = x->start;
	x->state = 1;
	stepLoop(x);
	
}

void restart(x)
Loop *x;
{
	
	x->state = 1;
	stepLoop(x);
	
}

void loop_assist(x,b,msg,arg,dst)
Loop *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"bang to start");
			break;
			
			case 1:
			strcpy(dst,"Start index");
			break;
			
			default:
			strcpy(dst,"End index & bang for looping");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Loop index");
			break;
			
			default:
			strcpy(dst,"Bang when done");
			break;
		}
	}
}

void *loop_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Loop *x;	
	
	x = (Loop *)newobject(class);
	x->theQ = qelem_new(x,(method)doCount);
	x->nowBang = qelem_new(x,(method)bangRight);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->state = 0;
	x->inletNumber = 0;
	switch(ac)
		{
			case 0:
			x->start = 0;
			x->end = 0;
			break;
			
			case 1:
			x->start = 0;
			x->end = theArgs[0].a_w.w_long;
			break;
			
			default:
			x->start = theArgs[0].a_w.w_long;
			x->end = theArgs[1].a_w.w_long;
			break;
		}
	
	
	return (x);
}

void loop_free(x)
Loop *x;
{
	
	freeobject(x->proxy1);
	freeobject(x->proxy2);
	qelem_free(x->theQ);
	

}

int check_inlet(x)
Loop *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}