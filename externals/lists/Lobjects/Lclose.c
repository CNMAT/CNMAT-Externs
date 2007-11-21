// lclose file for Max object
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

typedef struct lclose
{
	struct object ob;
	void *proxy;
	Atom theList[MAXSIZE];
	Atom valueList[MAXSIZE];
	Atom outList[MAXSIZE];
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;	
	double lookForThis;
	void *outL;
	void *outR;
	int listsize;
	int outsize;
	long inletNumber;
} Lclose;


void *lclose_new();
void lclose_free();
void  lclose_List();
void lclose_doAgain();
void lclose_assist();
void lclose_constant();
void lclose_Fconstant();
long findClosest();
double absoluteDifference();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lclose_new,(method)lclose_free,(short)sizeof(struct lclose),0L,A_GIMME,0);
	addint((method)lclose_constant);
	addfloat((method)lclose_Fconstant);
	addbang((method)lclose_doAgain);
	addmess((method)lclose_List, "list",A_GIMME,0);
	addmess((method)lclose_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void lclose_constant(x,n)
Lclose *x;
long n;
{
	
	long result;
	x->inList[0].a_w.w_long = n;
		x->inList[0].a_type = A_LONG;
		x->insize = 1;
	
	result = findClosest(x,(double)n);
	if(x->theList[result].a_type == A_LONG)
		outlet_int(x->outR,x->theList[result].a_w.w_long);
	else
		outlet_float(x->outR,x->theList[result].a_w.w_float);
	outlet_int(x->outL,result);
	
}

void lclose_Fconstant(x,n)
Lclose *x;
double n;
{
	long result;
	x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
	result = findClosest(x,n);
	if(x->theList[result].a_type == A_LONG)
		outlet_int(x->outR,x->theList[result].a_w.w_long);
	else
		outlet_float(x->outR,x->theList[result].a_w.w_float);
	outlet_int(x->outL,result);
	
	
}

long findClosest(x,n)
Lclose *x;
double n;
{
	
	int i;
	int closestSoFar;
	double smallestDifferenceSoFar;
	double difference;
	
	closestSoFar = 0;
	smallestDifferenceSoFar = 1000000000;
	x->lookForThis = n;
	for(i=0;i < x->listsize;++i)
	{
		switch(x->theList[i].a_type)
		{
			case A_LONG:
			difference = absoluteDifference(n,(double)x->theList[i].a_w.w_long);
			break;
			
		case A_FLOAT:
			difference = absoluteDifference(n,x->theList[i].a_w.w_float);
			break;
			
		default :
		continue;			
		}
		if(difference < smallestDifferenceSoFar){
			closestSoFar = i;
			smallestDifferenceSoFar = difference;
		}
	}
	return(closestSoFar);
}



double absoluteDifference(a, b)
double a;
double b;
{
	double difference;
	difference = b - a;
	if(difference < 0)
		difference =  - difference;
	return(difference);
}


void lclose_List(x,s,argc,argv)
Lclose *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc >MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x)) //list in right
	{
		for(i = 0; i < argc; ++i)
			x->theList[i] = argv[i];
		x->listsize = argc;
	} 
	else // list in left
	{
		for(i = 0; i < argc; ++i) 
		{
			x->inList[i] = argv[i];
		
			switch(argv[i].a_type)
			{
				case A_LONG :
				x->outList[i].a_w.w_long = findClosest(x,(double)argv[i].a_w.w_long);
				break;
				
				case A_FLOAT :
				x->outList[i].a_w.w_long = findClosest(x,argv[i].a_w.w_float);
				break;
			
			}
		}
		x->outsize = argc;
		x->insize = argc;
		for(i=0;i<argc;++i)
			x->valueList[i] = x->theList[x->outList[i].a_w.w_long];
		outlet_list(x->outR,0L,x->outsize,x->valueList);
		outlet_list(x->outL,0L,x->outsize,x->outList);
	}
	
}




void lclose_doAgain(x)
Lclose *x;
{
	if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				lclose_constant(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				lclose_Fconstant(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		lclose_List(x,gensym("bang"),x->insize,x->inList);
			
	
}


void lclose_assist(x,b,msg,arg,dst)
Lclose *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Value to search for");
			break;
			
			default:
			strcpy(dst,"Store List");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Position of closest value");
			break;
			
			default:
			strcpy(dst,"Closest Value");
			break;
		}
	}
	
	
}

void  *lclose_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lclose  *x;
	int i;	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x = (Lclose *)newobject(class);
	x->proxy = proxy_new(x,1,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	if(argc)
	for(i = 0; i < argc;++i) 
		x->theList[i]= argv[i];
	else
		SETLONG(&x->theList[0],0);
	x->listsize = argc;
	
	for(i = 0; i < MAXSIZE; ++i)
		x->outList[i].a_type = A_LONG;
		
	x->insize = 0;
	x->outsize = 0;
	
	return(x);
}

void lclose_free(x)
Lclose *x;
{
	
	freeobject(x->proxy);
	

}

int check_inlet(x)
Lclose *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}