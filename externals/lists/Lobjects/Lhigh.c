// lhigh file for Max object
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

typedef struct lhigh
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;	
	int howMany;
	void *outL;
} Lhigh;


void *lhigh_new();
void lhigh_free();
void  lhigh_List();
void  lhigh_howMany();
void lhigh_dumpList();
void lhigh_assist();
void lhigh_constant();
void lhigh_Fconstant();
void 	generic_doBang();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lhigh_new,(method)lhigh_free,(short)sizeof(struct lhigh),0L,A_DEFLONG,0);
	addint((method)lhigh_constant);
	addfloat((method)lhigh_Fconstant);
	addinx((method)lhigh_howMany,1);
	addbang((method)generic_doBang);
	addmess((method)lhigh_List, "list",A_GIMME,0);
	addmess((method)lhigh_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lhigh");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void lhigh_constant(x,n)
Lhigh *x;
long n;
{
	
	
	
	if(n > 0)
	{
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
		x->outsize = 1;
		outlet_list(x->outL,0L,1,x->outList);
	}
	
}

void lhigh_Fconstant(x,n)
Lhigh *x;
double n;
{
	
	
	
	if(n > 0)
	{
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = n;
		x->outsize = 1;
		outlet_list(x->outL,0L,1,x->outList);
	}
	
}

void generic_doBang(x) 
Lhigh *x;
{
		lhigh_List(x,gensym("bang"),x->insize,x->inList);

}

void lhigh_List(x,s,argc,argv)
Lhigh *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	long howManyFound;
	long highestSoFar;
	double FhighestSoFar;
	long where=0;
	Atom argr[MAXSIZE];
	
	if(s == gensym("list")){
		for(i=0;i < argc;++i)
			x->inList[i] = argv[i];
		x->insize = argc;
	}
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	howManyFound = 0;
	if(argc <= x->howMany)  // only one pass through list is necessary, set to 1 or 0
	{
	for(i=0;i < argc;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
			switch(argv[i].a_type)
			{
			case A_LONG:
				if(argv[i].a_w.w_long)
				{
				x->outList[i].a_w.w_long = 1;
				}
				break;
				
			case A_FLOAT:
			if(argv[i].a_w.w_float)
				{
				x->outList[i].a_w.w_long = 1;
				}
				break;
			}
			
		}
	}
	else  // go through once per output member
	{
	for(i=0;i < argc;++i)
		{
		argr[i].a_type = argv[i].a_type;
		argr[i].a_w = argv[i].a_w;
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	howManyFound = 0;
	for(j=0;j<x->howMany;++j)
		{
		highestSoFar = 0;
		FhighestSoFar = 0;
		for(i=0;i<argc;++i)   // find the higest member, put its location in where
			{
					switch(argr[i].a_type)
					{
					case A_LONG:
						if(argr[i].a_w.w_long > highestSoFar)
						{
						highestSoFar = argr[i].a_w.w_long;
						FhighestSoFar = (double)highestSoFar;
						where = i;
						}
						break;
						
					case A_FLOAT:
					if(argr[i].a_w.w_float > FhighestSoFar)
						{
						FhighestSoFar = argr[i].a_w.w_float;
						highestSoFar = (long)FhighestSoFar;
						where = i;
						
						}
						break;
						
					}
			} // end of i loop
		if((highestSoFar == 0) & (FhighestSoFar == 0)) break;

			switch(argr[where].a_type) // put highest value in outlist and clear it in arg.
				{
				case A_LONG:
					x->outList[where].a_type = A_LONG;
					x->outList[where].a_w.w_long = argr[where].a_w.w_long;
					argr[where].a_w.w_long = 0;
					
					break;
					
				case A_FLOAT:
					x->outList[where].a_type = A_FLOAT;
					x->outList[where].a_w.w_float = argr[where].a_w.w_float;
					argr[where].a_w.w_float = 0;
					break;
				}
		
		++howManyFound;
		} // end of j loop
		if(howManyFound == 0)
			{
			
			return;
			}
		}
		x->outsize = argc;
		outlet_list(x->outL,0L,x->outsize,x->outList);
	
}



void  lhigh_howMany(x,n)
Lhigh *x;
long n;
{

	
	if(n < 1)
		{
		n = 1;
		}
	x->howMany = n;
	
}



void lhigh_dumpList(x)
Lhigh *x;
{
	
		outlet_list(x->outL,0L,x->outsize,x->outList);
			
	
}


void lhigh_assist(x,b,msg,arg,dst)
Lhigh *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to process");
			break;
			
			default:
			strcpy(dst,"Number of high values to keep");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List with lower values set to 0");//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *lhigh_new(arg1)
long arg1;
{

	Lhigh  *x;	
	if(arg1 <= 0)
		arg1 = 1;
		
	if(arg1 > MAXSIZE) arg1 = MAXSIZE;
	x = (Lhigh *)newobject(class);
	intin(x,1);
	x->outL= outlet_new(x,0L);
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = 0;
	x->outsize = 1;
	x->insize = 0;
	x->howMany = arg1;
	
	
	return(x);
}

void lhigh_free(x)
Lhigh *x;
{
	
	

}

