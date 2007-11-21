// LtoTab file for Max object
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

typedef struct toTab
{
	struct object t_ob;
	long tableSize;
	//long next;
	void *t_outL;
	void *t_outR;
} ToTab;


void *toTab_new();
void toTab_free();
void toTab_listToTab();
void toTab_assist();
void toTab_one();
void toTab_oneFloat();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)toTab_new,0L,(short)sizeof(struct toTab),0L,A_DEFLONG,0);
	addmess((method)toTab_listToTab, "list",A_GIMME,0);
	addmess((method)toTab_assist, "assist",A_CANT,0);
	addint((method)toTab_one);
	addfloat((method)toTab_oneFloat);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void toTab_listToTab(x,s,argc,argv)
ToTab *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type) 
			{
			case A_LONG:
			
			outlet_int(x->t_outR,argv[i].a_w.w_long);
			outlet_int(x->t_outL,(long)i);
			break;
			
			case A_FLOAT:
			
			outlet_float(x->t_outR,argv[i].a_w.w_float);
			outlet_int(x->t_outL,(long)i);
			break;
			
			default:
			//post("Invalid List Member");
			break;
			}
		}
		
}

void toTab_one(x,n)
ToTab *x;
long n;
{

	long i;
	for(i = 0; i < x->tableSize; ++i){
	outlet_int(x->t_outR,n);
	outlet_int(x->t_outL,i);
	}
	
}

void toTab_oneFloat(x,n)
ToTab *x;
double n;
{
	long i;
	for(i = 0; i < x->tableSize; ++i){
	outlet_float(x->t_outR,n);
	outlet_int(x->t_outL,i);
	}
}

void toTab_assist(x,b,msg,arg,dst)
ToTab *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to stuff in table");
			break;
			
			default:
			strcpy(dst,"List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Connect to table left");//forgot something
			break;
			
			default:
			strcpy(dst,"Connect to table right");
			break;
		}
	}
	
}
void  *toTab_new(theArg)
long theArg;
{

	ToTab  *x;
	x = (ToTab *)newobject(class);
	
	if(theArg > 0 && theArg <= 256)
		x->tableSize = theArg;
	else
		x->tableSize = 1;
	//post("%ld", theArg);
	x->t_outR = outlet_new(x,0L);
	x->t_outL = outlet_new(x,0L);
	
	return(x);
}

void toTab_free(x)
ToTab *x;
{
	
	

}