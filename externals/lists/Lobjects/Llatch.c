// llatch file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2006, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct llatch
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom outList[MAXSIZE];
	long outsize;
	float currentValue;
	float lastValue;
	int latched;
	int floatMode;
	void *outR;
	void *outL;
} Llatch;


void *llatch_new();
void llatch_free();
void  llatch_doLists();
void  llatch_anything();
void  llatch_doConstant();
void  llatch_doFloatConstant();
void llatch_dumpList();
void  llatch_clearLatch();
void  llatch_setLatch();
void llatch_assist();
void llatch_doBang();
int searchTheList();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)llatch_new,(method)llatch_free,(short)sizeof(struct llatch),0L,A_GIMME,0);
	addint((method)llatch_doConstant);
	addfloat((method)llatch_doFloatConstant);
	addbang((method)llatch_doBang);
	addmess((method)llatch_doLists, "list",A_GIMME,0);
	//addmess((method)llatch_anything, "anything",A_GIMME,0);
	addmess((method)llatch_assist, "assist",A_CANT,0);
	addmess((method)llatch_clearLatch, "clear",0);
	addmess((method)llatch_setLatch, "latch",A_GIMME,0);
	finder_addclass("Lists","Llatch");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void llatch_doLists(x,s,argc,argv)
Llatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	float n;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(argv[0].a_type == A_FLOAT)
		n = argv[0].a_w.w_float;
	else
		n = (float)argv[0].a_w.w_long;
	
	if(check_inlet(x))  // list behaves like first element is data
	{
	for(i=0;i<argc;++i)
		x->outList[i] = argv[i];
	x->outsize = argc;
	if(!x->latched){
			x->latched = 1;
			outlet_int(x->outR,0);
			}
	x->currentValue = n;
	llatch_dumpList(x);
	}
	
	else		// do matching
	{
		
		if(x->latched){
			if((n == x->currentValue) || 
					((n < x->currentValue) && (x->currentValue <= x->lastValue)) ||
						((n > x->currentValue) && (x->currentValue >= x->lastValue))){
				x->latched = 0;
				outlet_int(x->outR,1);
				}
		}
		x->lastValue = n;
		if(!x->latched){
			x->currentValue = n;
			for(i=0;i<argc;++i)
				x->outList[i] = argv[i];
			x->outsize = argc;
			llatch_dumpList(x);
		}
	
	}
}

void  llatch_clearLatch(x)
Llatch *x;
{
	x->latched = 0;
	outlet_int(x->outR,1);
	
}
	
void llatch_setLatch(x,s,argc,argv)
Llatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	x->latched = 1;
	if(argc){
		if(argv[0].a_type == A_FLOAT)
			x->currentValue = argv[0].a_w.w_long;
		else 
		x->currentValue = (float)argv[0].a_w.w_long;
	
	} else
		x->currentValue = 0;
	x->latched = 1;
	outlet_int(x->outR,0);
}


void  llatch_doConstant(x,n)
Llatch *x;
long n;
{
	
	llatch_doFloatConstant(x,(float)n);
}


void  llatch_doFloatConstant(x,n)
Llatch *x;
double n;
{
	x->outsize = 0;
	if(x->inletNumber)		// data is in right inlet, sets latch
	{
		if(!x->latched){
			x->latched = 1;
			outlet_int(x->outR,0);
			}
		x->currentValue = n;
		if(x->floatMode)
		outlet_float(x->outL,x->currentValue);
	else
		outlet_int(x->outL,(long)x->currentValue);
	}
	
	else		// check for latch clearing
	{
		if(x->latched){
			if((n == x->currentValue) || 
					((n < x->currentValue) && (x->currentValue <= x->lastValue)) ||
						((n > x->currentValue) && (x->currentValue >= x->lastValue))){
				x->latched = 0;
				outlet_int(x->outR,1);
				}
			
		}
		x->lastValue = n;
		if(!x->latched){
			x->currentValue = n;
			if(x->floatMode)
				outlet_float(x->outL,x->currentValue);
			else
				outlet_int(x->outL,(long)x->currentValue);
		}
	}
}

void llatch_doBang(x)
Llatch *x;
{
	if(x->outsize ){
		llatch_dumpList(x);
		return;
	}
	
	if(x->floatMode)
		outlet_float(x->outL,x->currentValue);
	else
		outlet_int(x->outL,(long)x->currentValue);
}

void llatch_dumpList(x)
Llatch *x;
{
	if(x->outsize < 1) return;
	if(x->outsize > MAXSIZE) return;
	if(x->outsize == 1)
	{
		switch(x->outList[0].a_type) {
			case A_LONG:
			outlet_int(x->outL,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT :
			outlet_float(x->outL,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM :
			outlet_anything(x->outL,x->outList[0].a_w.w_sym, 0, 0);
			break;
		}
	}else
		if(x->outList[0].a_type == A_SYM)
			outlet_anything(x->outL,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
		else
			outlet_list(x->outL,0L,x->outsize,x->outList);
}


void llatch_assist(x,b,msg,arg,dst)
Llatch *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			sprintf(dst,"Data from controller");
			break;
			
			default:
			sprintf(dst,"Data to set latch.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			sprintf(dst,"Data from controller if latch is off");//forgot something
			break;
			
			default:
			sprintf(dst,"1 if controller active, 0 if latched");
			break;
		}
	}
}

void  *llatch_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Llatch  *x;
	
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Llatch *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->inletNumber = 0;
	x->latched = 1;
	x->outsize = 0;
	x->lastValue = 0;
	
	switch(ac)
	{
	case 0:
	x->currentValue = 0;
	x->lastValue = 0;
	x->floatMode = 0;
		break;

		
	default:
		if(theArgs[0].a_type == A_FLOAT){
			x->floatMode = 1;
			x->currentValue = theArgs[0].a_w.w_float;
		} else 
		x->currentValue = (float)theArgs[0].a_w.w_long;
		
		
	}
	
		return(x);
}

void llatch_free(x)
Llatch *x;
{
	
	freeobject(x->proxy);
	
}

int check_inlet(x)
Llatch *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}