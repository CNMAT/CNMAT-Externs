// lrun file for Max object
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

typedef struct lrun
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	long startWith;
	long endWith;
	long interval;
	long lastOut;
	int outsize;
	void *outL;
} Lrun;


void *lrun_new();
void lrun_free();
void  lrun_doLists();
void lrun_makeAnother();
void  lrun_setConstant();
void  lrun_floatConstant();
void lrun_dumpList();
void  lrun_clearList();
void lrun_assist();
void  lrun_reset();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lrun_new,(method)lrun_free,(short)sizeof(struct lrun),0L,A_GIMME,0);
	addint((method)lrun_setConstant);
	addfloat((method)lrun_floatConstant);
	addbang((method)lrun_makeAnother);
	addmess((method)lrun_doLists, "list",A_GIMME,0);
	addmess((method)lrun_assist, "assist",A_CANT,0);
	//addmess((method)lrun_clearList, "clear",0);
	addmess((method)lrun_reset, "reset",0);
	finder_addclass("Lists","Lrun");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lrun_doLists(x,s,argc,argv)
Lrun *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k;
	
	
	j = 0;
	k = 0;
	for(i= 1; i < argc; ++i)
	{
		if(argv[i].a_w.w_long > argv[i-1].a_w.w_long)
		{
			for(j = argv[i-1].a_w.w_long; j < argv[i].a_w.w_long; j +=x->interval)
			{
				x->outList[k++].a_w.w_long = j;
				if(k >= MAXSIZE)
					break;
			}
		}
		if(argv[i].a_w.w_long < argv[i-1].a_w.w_long)
		{
			for(j = argv[i-1].a_w.w_long; j > argv[i].a_w.w_long; j -=x->interval)
			{
				x->outList[k++].a_w.w_long = j;
				if(k >= MAXSIZE)
					break;
			}
		}
		if(argv[i].a_w.w_long == argv[i-1].a_w.w_long)
		{
			x->outList[k++].a_w.w_long =argv[i].a_w.w_long; 
			if(k >= MAXSIZE)
					break;
		}
	}
	if(k >= MAXSIZE)
		k = MAXSIZE;
	else
		x->outList[k++].a_w.w_long = argv[i-1].a_w.w_long;
	x->outsize = k;
	if(check_inlet(x) == 0)  // list in left inlet
	lrun_dumpList(x);
	
	
	
}

void  lrun_clearList(x)
Lrun *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	
	x->outsize = 0;
	
}



void  lrun_dumpList(x)
Lrun *x;
{
	
	
	
	if(x->outsize)
	{
		if(x->outsize > 1)
			outlet_list(x->outL,0L,(short)x->outsize,x->outList);
		else
			outlet_int(x->outL,x->outList[0].a_w.w_long);
		x->lastOut = x->outList[x->outsize-1].a_w.w_long;
	}

	
}

void  lrun_makeAnother(x)
Lrun *x;
{
	
	
	
	if(x->outsize > 1)
	{
		if(x->outList[x->outsize-1].a_w.w_long > x->outList[x->outsize-2].a_w.w_long)
			{
			x->lastOut += x->interval;
			outlet_int(x->outL,x->lastOut);
			}
			
		if(x->outList[x->outsize-1].a_w.w_long < x->outList[x->outsize-2].a_w.w_long)
			{
			x->lastOut -= x->interval;
			outlet_int(x->outL,x->lastOut);
			}
		if(x->outList[x->outsize-1].a_w.w_long == x->outList[x->outsize-2].a_w.w_long)
			outlet_int(x->outL,x->lastOut);

	}

	
}

void  lrun_reset(x)
Lrun *x;
{	
	if(x->outsize > 1)
	{
		if(x->outList[x->outsize-1].a_w.w_long > x->outList[x->outsize-2].a_w.w_long)
			{
			x->lastOut = x->startWith - x->interval;
			}
			
		if(x->outList[x->outsize-1].a_w.w_long < x->outList[x->outsize-2].a_w.w_long)
			{
			x->lastOut = x-> startWith + x->interval;
			}
		if(x->outList[x->outsize-1].a_w.w_long == x->outList[x->outsize-2].a_w.w_long)
			x->lastOut = x->startWith;
	}
	else
	{
		x->outList[0].a_w.w_long = -2;		//set up for counting bangs
		x->outList[1].a_w.w_long = -1;
		x->outsize = 2;
		x->lastOut = x->startWith - x->interval;
	}
	
	
}

void  lrun_setConstant(x,n)
Lrun *x;
long n;
{
	int j, k;
	
	k = 0;
	switch(check_inlet(x))	
	{	
		case 1:   // middle inlet
		x->startWith = n;
		break;
		
		case 2:  // right inlet
		if(n < 0)
			n= -n;
		if(n == 0)
			n = 1;
		x->interval = n;
		break;
		
		case 0:  // left inlet
		if(n > x->startWith)
		{
			for(j =x->startWith ; j < n; j += x->interval)
			{	x->outList[k++].a_w.w_long = j;
				if(k >= MAXSIZE)
				break;
			}
		}
		if(n < x->startWith)
		{
			for(j = x->startWith; j > n ; j -= x->interval)
			{
				x->outList[k++].a_w.w_long = j;
				if(k >= MAXSIZE)
				break;
			}
		}
		
		if(k >= MAXSIZE)
			k = MAXSIZE;
		else
			x->outList[k++].a_w.w_long =n;
		x->outsize = k;
		lrun_dumpList(x);
		
	}
	
	
}

void  lrun_floatConstant(x,n)
Lrun *x;
double n;
{
		lrun_setConstant(x,(long)n);
	
}




void lrun_assist(x,b,msg,arg,dst)
Lrun *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Set end and trigger output.");
			break;
			
			case 1:
			strcpy(dst,"Start value.");
			break;
			
			case 2:
			strcpy(dst,"Interval (absolute).");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"List of integers from start to end.");
	}
	
	
}


void  *lrun_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lrun  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lrun *)newobject(class);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	x->startWith = 0;
	x->endWith = 0;
	x->interval = 1;
	switch(ac)
	{
		case 0:
		break;
		
		case 1:
		if(theArgs[0].a_type == A_LONG)
			x->startWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->startWith = (long)theArgs[0].a_w.w_float;

		break;
		
		case 2:
		if(theArgs[0].a_type == A_LONG)
			x->startWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->startWith = (long)theArgs[0].a_w.w_float;
		if(theArgs[1].a_type == A_LONG)
			x->interval = theArgs[1].a_w.w_long;
		if(theArgs[1].a_type == A_FLOAT)
			x->interval = (long)theArgs[1].a_w.w_float;
		
		break;
		
		default:
		if(theArgs[0].a_type == A_LONG)
			x->startWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->startWith = (long)theArgs[0].a_w.w_float;
			
		if(theArgs[1].a_type == A_LONG)
			x->interval = theArgs[1].a_w.w_long;
		if(theArgs[1].a_type == A_FLOAT)
			x->interval = (long)theArgs[1].a_w.w_float;	
			
			
		if(theArgs[2].a_type == A_LONG)
			x->endWith = theArgs[2].a_w.w_long;
		if(theArgs[2].a_type == A_FLOAT)
			x->endWith = (long)theArgs[2].a_w.w_float;
		break;
		
	}
	if(x->interval < 0)
		x->interval = -x->interval;
	if(x->interval == 0)
		x->interval = 1;
	for(i = 0;i < MAXSIZE; ++i)
		x->outList[i].a_type = A_LONG;
	x->outList[0].a_w.w_long = -2;		//set up for counting bangs
	x->outList[1].a_w.w_long = -1;
	x->outsize = 2;
	x->lastOut = x->startWith - x->interval;
	
	return(x);
}

void lrun_free(x)
Lrun *x;
{
	
	freeobject(x->proxy1);
	freeobject(x->proxy2);
	

}
int check_inlet(x)
Lrun *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}