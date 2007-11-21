// lline file for Max object
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

typedef struct lline
{
	struct object ob;
	void * theClock;
	short floatmode;
	Atom theList[MAXSIZE];
	int listsize;
	int listTimePointer;
	int workingThroughList;
	double target;
	double oldTarget;
	double oldTime;
	double currentValue;
	double initialValue;
	double increment;
	long numSteps;
	double interval;
	double transitionTime;
	double leftover;
	double granularity;
	double slope;
	void *outL;
	void *outR;
} Lline;


void *lline_new();
void lline_free();
void  lline_Int();
void  lline_doInt();
void  lline_Float();
void  lline_doFloat();
void  lline_doList();
void  lline_stop();
void  lline_clear();
void  lline_bang();
void  lline_setGran();
void lline_setTime();
void lline_set();
void  lline_doTick();
void  lline_doFinalTick();
void  lline_doFloatTick();
void lline_assist();

void *class;

int main( void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lline_new,(method)lline_free,(short)sizeof(struct lline),0L,A_GIMME,0);
	addint((method)lline_Int);
	addinx((method)lline_setTime,1);
	addinx((method)lline_setGran,2);
	addfloat((method)lline_Float);
	addmess((method)lline_doList, "list",A_GIMME,0);
	addbang((method)lline_bang);
	addmess((method)lline_set, "set",A_GIMME,0);
	addmess((method)lline_assist, "assist",A_CANT,0);
	addmess((method)lline_stop, "stop",0);
	addmess((method)lline_clear, "clear",0);
	//finder_addclass("Lists","Lline");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void  lline_Int(x,n)
Lline *x;
long n;
{
	x->oldTarget = (float)n;
	x->transitionTime =x->oldTime ;
	x->listsize = 0;
	
	lline_doInt(x,n);
}

void  lline_doInt(x,n)
Lline *x;
long n;
{
	double difference;
	if(x->floatmode){
		lline_doFloat(x,(float)n);
		return;
		}
	
	lline_stop(x);
	x->target = (double)n;											// 150

	if(x->target == x->currentValue){
		x->numSteps = 1;
		clock_fdelay(x->theClock,x->transitionTime);
		return;
	}
	if(x->transitionTime == 0){
		x->currentValue = x->target;
		outlet_bang(x->outR);
		outlet_int(x->outL, (long) n);
		return;
	}
	
	x->initialValue = x->currentValue;  					// 15
	x->slope = x->target - x->currentValue;							// 135
	difference = x->slope< 0 ? -x->slope : x->slope;		// 135
	
	if(difference * x->granularity < x->transitionTime){  // no
		x->interval = x->transitionTime / difference;
		x->increment = x->slope / difference;
		if(difference)
			x->numSteps = difference;
		else
			x->numSteps = 1;
	} else{
		
		x->interval = x->granularity;						// 20
		x->numSteps = x->transitionTime /x->interval;			// 50
		x->increment = x->slope / x->numSteps;					//2.7
	}
	//post("increment %ld  current %ld", x->increment,x->currentValue);
	
	clock_fdelay(x->theClock,x->interval);
	
}


void  lline_Float(x,n)
Lline *x;
double n;
{
	x->oldTarget = n;
	x->transitionTime =x->oldTime ;
	x->listsize = 0;
	lline_doFloat(x,n);
}

void  lline_doFloat(x,n)
Lline *x;
double n;
{
	
	double difference;
	if(x->floatmode == 0){
	lline_doInt(x,(long)n);
	return;}
	lline_stop(x);
	
	if((x->transitionTime == 0)||(x->currentValue == n)){
		x->currentValue = n;
		outlet_bang(x->outR);
		outlet_float(x->outL, n);
		return;
	}
	
	x->target = n;
	x->initialValue = x->currentValue;
	difference = n - x->currentValue;
	if(difference < 0)
		x->slope = -1;
		else
		x->slope = 1;
	
	x->interval = x->granularity;
	x->numSteps = (float)x->transitionTime / x->granularity;
	x->increment = difference / x->numSteps;
	clock_fdelay(x->theClock,x->interval);
	
}


void lline_doList(x,s,argc,argv)
Lline *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	double lastTime = x->oldTime;
	double lastTarget = x->oldTarget;
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->theList[i].a_w.w_float = (float)argv[i].a_w.w_long;
			if(i % 2)
			lastTime = x->theList[i].a_w.w_float;
			else
			lastTarget= x->theList[i].a_w.w_float;
			break;
			
			case A_FLOAT:
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			if(i % 2)
			lastTime = x->theList[i].a_w.w_float;
			else
			lastTarget= x->theList[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(i % 2)
			x->theList[i].a_w.w_float = lastTime;
			else
			x->theList[i].a_w.w_float = lastTarget;
			break;
			
			}
		}
	x->listsize = argc;
	if(x->listsize > 2)
		x->workingThroughList = 1;
	else
		x->workingThroughList = 0;
	
	x->listTimePointer = 1;
	x->transitionTime = x->theList[1].a_w.w_float;
	lline_doFloat(x,x->theList[0].a_w.w_float);
}


void  lline_setGran(x,n)
Lline *x;
long n;
{
	x->granularity = n;
}

void lline_setTime(x,n)
Lline *x;
long n;
{
	x->transitionTime = n;
	x->oldTime = n;
}

void lline_set(x,s,argc,argv)
Lline *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(argc){ 
		if(argv[0].a_type == A_LONG){
			x->currentValue = argv[0].a_w.w_long;
			x->initialValue = argv[0].a_w.w_long;
			x->floatmode = 0;
			}
		if(argv[0].a_type == A_FLOAT){
			x->currentValue = argv[0].a_w.w_float;
			x->initialValue = x->currentValue;
			x->floatmode = 1;
		}
	}
	
	
	if(argc > 1 && argv[1].a_type == A_LONG)
		x->transitionTime = argv[1].a_w.w_long;
	if(argc > 2 && argv[2].a_type == A_LONG)
		x->granularity = argv[2].a_w.w_long;
}

void  lline_doTick(x)
Lline *x;
{
	double result = x->currentValue + x->increment;
	if(--x->numSteps <= 0){
		x->currentValue = x->target;
		if(x->workingThroughList==0)
			outlet_bang(x->outR);
		if(x->floatmode)
			outlet_float(x->outL,x->target);
		else
			outlet_int(x->outL,(long)x->target);
		
		if(x->workingThroughList){
			x->listTimePointer += 2 ;
			if(x->listTimePointer + 2 >= x->listsize) // this is the last time through
				x->workingThroughList = 0;
			x->transitionTime = x->theList[x->listTimePointer].a_w.w_float;
			lline_doFloat(x,x->theList[x->listTimePointer-1].a_w.w_float);
		}
		
	}else{
		x->currentValue = result;
		if(x->floatmode)
		outlet_float(x->outL,result);
		else
		outlet_int(x->outL,(long)result);
		clock_fdelay(x->theClock,x->interval);
	}
}

void  lline_stop(x)
Lline *x;
{
	clock_unset(x->theClock);
}

void  lline_clear(x)
Lline *x;
{
	x->listsize = 0;
}

void  lline_bang(x)
Lline *x;
{
	
	clock_unset(x->theClock);
	
	if(x->listsize ){
		if(x->listsize > 2)
			x->workingThroughList = 1;
		else
			x->workingThroughList = 0;
		
		x->listTimePointer = 1;
		x->transitionTime = x->theList[1].a_w.w_float;
		lline_doFloat(x,x->theList[0].a_w.w_float);
	}
	else{
		x->currentValue = x->initialValue;
		if(x->floatmode){
			outlet_float(x->outL,x->currentValue);
			lline_doFloat(x,x->oldTarget);
		}else{
			outlet_int(x->outL,(long)x->currentValue);
			lline_doInt(x,(long)x->oldTarget);
		}
	}
}



void lline_assist(x,b,msg,arg,dst)
Lline *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"target");
			break;
			
			case 1:
			strcpy(dst,"Transition time");
			break;
			
			default:
			strcpy(dst,"Granularity");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Line");
			break;
			
			default:
			strcpy(dst,"bang at end");
			break;
		}
	}
	
}


void  *lline_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lline  *x;	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lline *)newobject(class);
	intin(x, 2); // for time
	intin(x, 1); // for granularity
	x->outR = outlet_new(x,0L);
	x->outL = outlet_new(x,0L);
	
	x->theClock = clock_new(x,(method)lline_doTick);
	x->floatmode = 0;
	x->currentValue = 0;
	x->initialValue = 0;
	x->target = 0;
	x->oldTarget = 1;
	x->workingThroughList = 0;
	x->listsize = 0;
	if(ac){ 
		if(theArgs[0].a_type == A_LONG){
			x->currentValue = (double)theArgs[0].a_w.w_long;
			}
		if(theArgs[0].a_type == A_FLOAT){
			x->currentValue = theArgs[0].a_w.w_float;
			
			x->floatmode = 1;
		}
	}
	x->initialValue = x->currentValue;
	
	if(ac > 1 && theArgs[1].a_type == A_LONG)
			x->transitionTime = (double)theArgs[1].a_w.w_long;
		else
			x->transitionTime = 1000;
	
	if(ac > 2 && theArgs[2].a_type == A_LONG)
			x->granularity = (double)theArgs[2].a_w.w_long;
		else
			x->granularity = 20;
	x->oldTime = x->transitionTime;
	x->initialValue = x->currentValue;
	return(x);
}

void lline_free(x)
Lline *x;
{
	freeobject(x->theClock);
}