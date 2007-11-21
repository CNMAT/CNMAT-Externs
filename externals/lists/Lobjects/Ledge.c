// ledge file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256

typedef struct ledge
{
	struct object ob;
	
	Atom outList[MAXSIZE];
	int outsize;
	void *outL;
	long lastvalue;
	long space;
	long mark;
} Ledge;


void *ledge_new();
void ledge_free();
void  ledge_List();
void  ledge_howMany();
void ledge_dumpList();
void ledge_assist();
void ledge_constant();
void ledge_Fconstant();

void *class;

int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ledge_new,(method)ledge_free,(short)sizeof(struct ledge),0L,A_GIMME,0);
	addint((method)ledge_constant);
	addfloat((method)ledge_Fconstant);
	addbang((method)ledge_dumpList);
	addmess((method)ledge_List, "list",A_GIMME,0);
	addmess((method)ledge_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Ledge");
	

s = gensym("goslugsgo");
l = gensym("lobjectsbypqe");
if(s->s_thing != (struct object *)l){
post("Lobjects for Max © Regents of the University of California.");
s->s_thing = (struct object *)l;}
return 0;
}



void ledge_constant(x,n)
Ledge *x;
long n;
{
	
	
	
	if(n == x->mark )
	{
		if(x->lastvalue == x->space)
		outlet_int(x->outL,1L);
	}
	x->lastvalue = n;
	
}

void ledge_Fconstant(x,f)
Ledge *x;
float f;
{
	
	long n;
	
	n = (long) f;
	if(n == x->mark )
	{
		if(x->lastvalue == x->space)
		outlet_int(x->outL,1L);
	}
	x->lastvalue = n;
	
}

void ledge_List(x,s,argc,argz)
Ledge *x;
Symbol *s;
int argc;
Atom *argz;
{
	int i,j;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x->lastvalue = x->space;
	j = 0;
	if(x->mark == x->space)// we are looking for any transitions
	{
		if(argz[0].a_w.w_long == x->mark){
			x->outList[j].a_w.w_long = 0;
			++j;
			}
		for(i=1;i < argc  ;++i){
			if((argz[i].a_w.w_long != argz[i-1].a_w.w_long)){
					x->outList[j].a_w.w_long = i;
					++j;
			}
		}
		if(argz[argc-1].a_w.w_long == x->mark){
			x->outList[j].a_w.w_long = argc;
			++j;
		
		}
	}
	else
	{
		for(i=0;i < argc;++i)
			{
				switch(argz[i].a_type)
				{
				case A_LONG:
					if(argz[i].a_w.w_long == x->mark)
					{
						if(x->lastvalue == x->space){
							x->outList[j].a_w.w_long = i;
							++j;
						}
					}	
					x->lastvalue = argz[i].a_w.w_long;
					
					break;
					
				case A_FLOAT:
				if(((long)argz[i].a_w.w_float) == x->mark)
					{
						if(x->lastvalue == x->space){
							x->outList[j].a_w.w_long = i;
							++j;
						}
					}	
					x->lastvalue = (long)argz[i].a_w.w_float;
					break;
				}
				
			}
	}
	x->outsize = j;
	ledge_dumpList(x);
	x->lastvalue = x->space;
	
}



void ledge_dumpList(x)
Ledge *x;
{
	
	if(x->outsize){
	if(x->outsize > 1)
		outlet_list(x->outL,0L,x->outsize,x->outList);
	else
		outlet_int(x->outL,x->outList[0].a_w.w_long);
	}
	
}


void ledge_assist(x,b,msg,arg,dst)
Ledge *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to test");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"positon of transitions");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *ledge_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Ledge  *x;
	int i;
	
	x = (Ledge *)newobject(class);
	x->space = 0;
	x->mark = 1;
	if(ac)
		x->space = theArgs[0].a_w.w_long;
	if(ac > 1) 
		x->mark = theArgs[1].a_w.w_long;
	x->outL= outlet_new(x,0L);
	for( i = 0; i<MAXSIZE; ++i){
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	x->outsize = 0;
	x->lastvalue = x->space;
	
	
	return(x);
}

void ledge_free(x)
Ledge *x;
{
}

