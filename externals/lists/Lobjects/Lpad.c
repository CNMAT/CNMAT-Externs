// lpad file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1997, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lpad
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	void *proxy3;
	long inletNumber;
	Atom fillWith;
	Atom outList[MAXSIZE];
	long startWith;
	long leading;
	int outsize;
	void *outL;
} Lpad;


void *lpad_new();
void lpad_free();
void  lpad_doLists();
void  lpad_setConstant();
void  lpad_floatConstant();
void lpad_dumpList();
void  lpad_clearList();
void  lpad_leading();
void lpad_assist();
void  lpad_reset();
void  lpad_check();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lpad_new,(method)lpad_free,(short)sizeof(struct lpad),0L,A_GIMME,0);
	addint((method)lpad_setConstant);
	addfloat((method)lpad_floatConstant);
	addbang((method)lpad_dumpList);
	addmess((method)lpad_doLists, "list",A_GIMME,0);
	addmess((method)lpad_assist, "assist",A_CANT,0);
	addmess((method)lpad_clearList, "clear",0);
	addmess((method)lpad_leading, "leading",A_DEFLONG,0);
	addmess((method)lpad_reset, "reset",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lpad_doLists(x,s,argc,argv)
Lpad *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k,startHere;
	if(x->leading)
		startHere = x->outsize - argc;
	else
		startHere = x->startWith;
	j = 0;
	k = 0;
	for(k = 0; k < startHere;++k)
		x->outList[k] = x->fillWith;
	for(i= 0; i < argc; ++i)
	{
		x->outList[k++] = argv[i];
		if(k == x->outsize)
			break;
	}
	for(; k < x->outsize;++k)
		x->outList[k] = x->fillWith;
	lpad_dumpList(x);
	x->inletNumber = 0;
	
	
}

void  lpad_reset(x)
Lpad *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i]= x->fillWith;
		}
}

void  lpad_clearList(x)
Lpad *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_w.w_long = 0;
		}
	
}

void  lpad_dumpList(x)
Lpad *x;
{
	if(x->outsize)
	{
		if(x->outsize > 1)
			outlet_list(x->outL,0L,(short)x->outsize,x->outList);
		else
			switch(x->outList[0].a_type)
			{
			case A_LONG:
			outlet_int(x->outL,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT:
			outlet_float(x->outL,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM:
			outlet_list(x->outL,0L,1,x->outList);
			break;
			}
	}
}

void  lpad_leading(x,n)
Lpad *x;
long n;
{
	x->leading = n;
}
void  lpad_setConstant(x,n)
Lpad *x;
long n;
{
	int i;
	
	
	switch(check_inlet(x))	
	{	
		case 1:   
			x->startWith = n;
			break;
			
		case 2:  
			x->fillWith.a_type = A_LONG;
			x->fillWith.a_w.w_long = n;
			break;
			
		case 3:   
			x->outsize = n;
			break;
			
		case 0:  // left inlet
			for( i = 0; i < x->outsize; ++i)
				x->outList[i] = x->fillWith;
			if(x->leading){
				x->outList[x->outsize -1].a_type = A_LONG;
				x->outList[x->outsize -1].a_w.w_long = n;
			}else{
				x->outList[x->startWith].a_type = A_LONG;
				x->outList[x->startWith].a_w.w_long = n;
			}
			lpad_dumpList(x);
			
	}
	lpad_check(x);
	x->inletNumber = 0;
	
}

void  lpad_floatConstant(x,n)
Lpad *x;
float n;
{
	int i;
	
	
	switch(check_inlet(x))	
	{	
		case 1:   
		x->startWith = (long)n;
		break;
		
		case 2:  
		x->fillWith.a_type = A_FLOAT;
		x->fillWith.a_w.w_float = n;
		break;
		
		case 3:   
		x->outsize = (long)n;
		break;
		
		case 0:  // left inlet
			for( i = 0; i < x->outsize; ++i)
				x->outList[i] = x->fillWith;
			if(x->leading){
				x->outList[x->outsize -1].a_type = A_FLOAT;
				x->outList[x->outsize -1].a_w.w_long = n;
			}else{
				x->outList[x->startWith].a_type = A_FLOAT;
				x->outList[x->startWith].a_w.w_long = n;
			}
			lpad_dumpList(x);
		
	}
	lpad_check(x);
	x->inletNumber = 0;
	
}




void lpad_assist(x,b,msg,arg,dst)
Lpad *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to pad and output");
			break;
			
			case 1:
			strcpy(dst,"Place list here.");
			break;
			
			case 2:
			strcpy(dst,"Value to pad with.");
			break;
			
			case 3:
			strcpy(dst,"Length of output.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Padded List.");
	}
}

void  lpad_check(x)
Lpad *x;
{
	if(x->startWith < 0)
		x->startWith = 0;
	if(x->startWith > MAXSIZE)
		x->startWith = MAXSIZE;
	if(x->outsize > MAXSIZE)
		x->outsize = MAXSIZE;
}


void  *lpad_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lpad  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lpad *)newobject(class);
	x->proxy3 = proxy_new(x,3L,&x->inletNumber);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	x->startWith = 0;
	x->leading = 0;
	x->outsize = MAXSIZE;
	x->fillWith.a_type = A_LONG;
	x->fillWith.a_w.w_long = 0;
	switch(ac)
	{
		case 0:
		break;
		
		case 1:
		if(theArgs[0].a_type == A_LONG)
			x->outsize = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->outsize = (long)theArgs[0].a_w.w_float;

		break;
		
		case 2:
		x->fillWith = theArgs[0];
		if(theArgs[1].a_type == A_LONG)
			x->outsize = theArgs[1].a_w.w_long;
		if(theArgs[1].a_type == A_FLOAT)
			x->outsize = (long)theArgs[1].a_w.w_float;
		
		break;
		
		default:
		if(theArgs[0].a_type == A_LONG)
			x->startWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->startWith = (long)theArgs[0].a_w.w_float;
			
		x->fillWith = theArgs[1];	
			
		if(theArgs[2].a_type == A_LONG)
			x->outsize = theArgs[2].a_w.w_long;
		if(theArgs[2].a_type == A_FLOAT)
			x->outsize = (long)theArgs[2].a_w.w_float;
		break;
		
	}
	lpad_check(x);
	for(i = 0; i<MAXSIZE; ++ i)
		x->outList[i] = x->fillWith;
	
	return(x);
}

void lpad_free(x)
Lpad *x;
{
	
	freeobject(x->proxy1);
	freeobject(x->proxy2);
	freeobject(x->proxy3);
}

int check_inlet(x)
Lpad *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}