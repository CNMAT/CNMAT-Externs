// lbondo file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lbondo
{
	struct object ob;
	void *proxy[32];
	void *theClock;
	short numInlets;
	long inletNumber;
	Atom theList[MAXSIZE];
	int listsize;
	int startSize;
	long delay;
	void *outL;
	//void *outR;
} Lbondo;


void *lbondo_new();
void lbondo_free();
void  lbondo_storeList();
void  lbondo_setSymbol();
void lbondo_setList();
void  lbondo_setConstant();
void  lbondo_put();
void  lbondo_floatConstant();
void  lbondo_setDelay();
void lbondo_dumpList();
void  lbondo_clearList();
void lbondo_assist();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbondo_new,(method)lbondo_free,(short)sizeof(struct lbondo),0L,A_GIMME,0);
	addint((method)lbondo_setConstant);
	addfloat((method)lbondo_floatConstant);
	addbang((method)lbondo_dumpList);
	addmess((method)lbondo_storeList, "list",A_GIMME,0);
	addmess((method)lbondo_setList, "set",A_GIMME,0);
	addmess((method)lbondo_put, "put",A_GIMME,0);
	addmess((method)lbondo_setSymbol, "anything",A_GIMME,0);
	addmess((method)lbondo_setDelay, "delay",A_DEFLONG,0);
	addmess((method)lbondo_assist, "assist",A_CANT,0);
	addmess((method)lbondo_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbondo_storeList(x,s,argc,argv)
Lbondo *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long inletNumber = proxy_getinlet((t_object *)x);
	long length = argc + inletNumber;
	if(length > MAXSIZE) 
	  argc = MAXSIZE - inletNumber;
	
	for(i= 0;i< argc;++i)		// list in any inlet
		{
			x->theList[ i + inletNumber] = argv[i] ;
		}
	if(argc + inletNumber > x->listsize) 
		x->listsize = argc + inletNumber;
	if(x->delay) {
		clock_unset(x->theClock);
		clock_delay(x->theClock, x->delay);
	}
	else 
		lbondo_dumpList(x);
	inletNumber = 0;
	
}

void lbondo_setSymbol(x,s,argc,argv)
Lbondo *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long inletNumber = proxy_getinlet((t_object *)x);
	x->theList[inletNumber].a_type = A_SYM;
	x->theList[inletNumber].a_w.w_sym = s;
	if(argc + inletNumber +1 > MAXSIZE) 
	  argc = MAXSIZE - inletNumber;
	
	for(i= 0;i< argc;++i)		// list in any inlet
		{
			x->theList[ i + inletNumber +1] = argv[i] ;
		}
	if(argc + inletNumber +1 > x->listsize) 
		x->listsize = argc + inletNumber +1;
	if(x->delay) {
		clock_unset(x->theClock);
		clock_delay(x->theClock, x->delay);
	}
	else 
		lbondo_dumpList(x);
	inletNumber = 0;
	
}

void lbondo_put(x,s,argc,argv)
Lbondo *x;
Symbol *s;
int argc;
Atom *argv;
{	
	if((argc > 1) && (argv[1].a_type == A_LONG))
	 {
		if((argv[1].a_w.w_long >= 0) && (argv[1].a_w.w_long < x->listsize))
		x->theList[ argv[1].a_w.w_long] = argv[0] ;
	}
	if(x->delay) {
		clock_unset(x->theClock);
		clock_delay(x->theClock, x->delay);
	}
	else 
		lbondo_dumpList(x);
	
}

void lbondo_setList(x,s,argc,argv)
Lbondo *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc  > MAXSIZE) argc = MAXSIZE;
	  
	
	for(i= 0;i< argc;++i)		
		{
			x->theList[ i] = argv[i] ;
		}
	 x->listsize = argc;
	
	
}



void  lbondo_clearList(x)
Lbondo *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = x->startSize;
	
}


void  lbondo_setConstant(x,n)
Lbondo *x;
long n;
{
	
	long inletNumber = proxy_getinlet((t_object *)x);
	x->theList[inletNumber].a_type = A_LONG;
	x->theList[inletNumber].a_w.w_long = n;
	if(x->delay) {
		clock_unset(x->theClock);
		clock_delay(x->theClock, x->delay);
	}
	else 
		lbondo_dumpList(x);
	
}

void  lbondo_floatConstant(x,n)
Lbondo *x;
float n;
{
	long inletNumber = proxy_getinlet((t_object *)x);
	x->theList[inletNumber].a_type = A_FLOAT;
	x->theList[inletNumber].a_w.w_float = n;
	if(x->delay) {
		clock_unset(x->theClock);
		clock_delay(x->theClock, x->delay);
	}
	else 
		lbondo_dumpList(x);
}
void  lbondo_setDelay(x,n)
Lbondo *x;
long n;
{
	if(n > 0)
		x->delay = n;
}
void lbondo_dumpList(x)
Lbondo *x;
{
	if(x->listsize){
		if(x->listsize == 1){
			switch(x->theList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->theList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->theList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->outL,x->theList[0].a_w.w_sym,0,NIL);
			}
		}
		else
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	}
}


void lbondo_assist(x,b,msg,arg,dst)
Lbondo *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		
			strcpy(dst,"Insert in list and output");
		
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		
			strcpy(dst,"List");
		
	}
	
	
}


void  *lbondo_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lbondo  *x;
	int i;
	
	x = (Lbondo *)newobject(class);
	if(argc) 
		x->numInlets = argv[0].a_w.w_long;
	else
		x->numInlets = 2;
	if(x->numInlets > 32) x->numInlets = 32;
	if(x->numInlets < 2) x->numInlets = 2;
	for(i = x->numInlets -2; i >= 0 ; --i)
	x->proxy[i] = proxy_new(x,(long) i+1,&x->inletNumber);
	
	x->outL= outlet_new(x,0L);
	x->theClock = clock_new(x, (method) lbondo_dumpList);
	if(argc > 1)
		x->delay = argv[1].a_w.w_long;
	else
		x->delay = 0;
		
	if(argc > 2)
	x->listsize = argv[2].a_w.w_long;
	else
	x->listsize = x->numInlets;
	x->startSize = x->listsize;
	if(x->listsize > MAXSIZE) x->listsize = MAXSIZE;
	for(i = 0; i < MAXSIZE; ++i)
	{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
	} 	
	
	
	return(x);
}

void lbondo_free(x)
Lbondo *x;
{
	int i;
	for(i = x->numInlets -2; i >= 0 ; --i)
		freeobject(x->proxy[i]);
	freeobject( x->theClock);
}