// lpeak file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define MAXSIZE 256

typedef struct lpeak
{
	struct object ob;
	void *proxy;
	long inletNumber;
	long dummy;
	Atom peakValue;
	long Place;
	int outsize;
	int threshold;
	double floatThreshold;
	void *outL;
	void *outR;
} Lpeak;

void *lpeak_new();
void lpeak_free();
void  lpeak_List();
void  lpeak_doList();
void lpeak_dumpList();
void assist();
void lpeak_constant();
void lpeak_Fconstant();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lpeak_new,(method)lpeak_free,(short)sizeof(struct lpeak),0L,A_GIMME,0);
	addint((method)lpeak_constant);
	addfloat((method)lpeak_Fconstant);
	addbang((method)lpeak_dumpList);
	addmess((method)lpeak_List, "list",A_GIMME,0);
	addmess((method)assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lpeak_constant(x,n)
Lpeak *x;
long n;
{
	if(check_inlet(x)){
	x->threshold = n;
	x->floatThreshold = (double) x->threshold;
	x->outsize = 0;
	}
	else
	{
		x->peakValue.a_type = A_LONG;
		x->peakValue.a_w.w_long = n;
		x->outsize = 1;
		
		x->Place = 0;
		outlet_int(x->outR,n);  // right is the value
		
		outlet_int(x->outL,0L); // left is the position
	}
	x->inletNumber = 0;
}

void lpeak_Fconstant(x,n)
Lpeak *x;
double n;
{
	if(check_inlet(x)){
	x->floatThreshold = n;
	x->threshold = (long) x->floatThreshold;
	x->outsize = 0;
	}
	{
		x->peakValue.a_type = A_FLOAT;
		x->peakValue.a_w.w_float = n;
		x->outsize = 1;
		
		x->peakValue.a_type = A_LONG;
		x->Place = 0;
		outlet_float(x->outR,n);
		outlet_int(x->outL,0L);
	}
	x->inletNumber = 0;
}

void lpeak_List(x,s,argc,argv)
Lpeak *x;
Symbol *s;
int argc;
Atom *argv;
{
	long highestSoFar;
	long where;
	double FhighestSoFar;
	int i,foundone;
	
		highestSoFar = x->threshold;
		FhighestSoFar = x->floatThreshold;
		where = 0;
		foundone = 0;
		for(i=0;i<argc;++i)   // find the higest member, put its location in where
			{
				switch(argv[i].a_type)
				{
				case A_LONG:
					if(argv[i].a_w.w_long > highestSoFar)
					{
					highestSoFar = argv[i].a_w.w_long;
					FhighestSoFar = (double)highestSoFar;
					where = i;
					foundone = 1;
					}
					break;
					
				case A_FLOAT:
				if(argv[i].a_w.w_float > FhighestSoFar)
					{
					FhighestSoFar = argv[i].a_w.w_float;
					highestSoFar = (long)FhighestSoFar;
					where = i;
					foundone = 1;
					}
					break;
					
				} // end of switch
			} // end of i loop
		x->peakValue= argv[where];
		x->Place = where;
		x->outsize = foundone;
	lpeak_dumpList(x);
}

void lpeak_dumpList(x)
Lpeak *x;
{
	
		if(x->outsize){
		
			switch(x->peakValue.a_type)
			{
			 case A_LONG:
				outlet_int(x->outR,x->peakValue.a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outR,x->peakValue.a_w.w_float);
				break;
			
			}
			
				outlet_int(x->outL,x->Place);
		}
}

void assist(x,b,msg,arg,dst)
Lpeak *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to Process");
			break;
			
			default:
			strcpy(dst,"Threshold");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Position of peak value");
			break;
			
			default:
			strcpy(dst,"Peak value");
			break;
		}
	}
}


void  *lpeak_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lpeak  *x;
	
	x = (Lpeak *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->inletNumber = 0;
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	
	x->outsize = 0;
	if(argc){
		switch(argv[0].a_type)
		{
			case A_FLOAT:
			
			x->floatThreshold = argv[0].a_w.w_float;
			x->threshold = (long) x->floatThreshold;
			break;
			
			case A_LONG:
			
			x->threshold = argv[0].a_w.w_long;
			x->floatThreshold = (double) x->threshold;
			break;
		}
	}
	else{
		x->threshold = -2147483647;
		x->floatThreshold = (double) x->threshold;
	}
		
	
	return(x);
}

void lpeak_free(x)
Lpeak *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lpeak *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}