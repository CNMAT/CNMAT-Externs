// lmean file for Max object
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


typedef struct lave
{
	struct object ob;
	void *proxy;
	long dummy;
	int countZeros;
	void *outL;
} Lave;


void *lave_new();
void lave_free();
void  lave_doLists();
void lave_assist();
void pass_ints();
void pass_floats();
void countzeros();

void *class;


int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lave_new,(method)lave_free,(short)sizeof(struct lave),0L,A_GIMME,0);
	addint((method)pass_ints);
	addfloat((method)pass_floats);
	addmess((method)lave_doLists, "list",A_GIMME,0);
	addmess((method)countzeros, "countzeros",A_DEFLONG,0);
	addmess((method)lave_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lave");
	

s = gensym("goslugsgo");
l = gensym("lobjectsbypqe");
if(s->s_thing != (struct object *)l){
post("Lobjects for Max © Regents of the University of California.");
s->s_thing = (struct object *)l;}
return 0;
}

void pass_ints(x,n)
Lave *x;
long n;
{
	outlet_int(x->outL,n);
}

void pass_floats(x,n)
Lave *x;
float n;
{
	outlet_float(x->outL,n);
}

void countzeros(x,n)
Lave *x;
long n;
{
	x->countZeros = n;
}

void lave_doLists(x,s,argc,argv)
Lave *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,count;
	float total,mean,n;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	total = 0;
	count = 0;
	n = 0;
	for(i=0; i < argc; ++i)
			{
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				total += n = (float)argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				total += n = argv[i].a_w.w_float;
				break;
				
				default:
				post("Strange List Member in Lave");
				}
			if(n || x->countZeros) ++count;
			}
	if(count)
		{
		mean = total / count;
		outlet_float(x->outL,mean);
		}
	
	
}


void lave_assist(x,b,msg,arg,dst)
Lave *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to average");
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
			if(x->countZeros)
			strcpy(dst,"Average of all values");
			else
			strcpy(dst,"Average of non-zero values");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *lave_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lave  *x;
	
	
	if(ac > 1) ac = 1;
	x = (Lave *)newobject(class);
	x->outL= outlet_new(x,0L);
	if(ac && (theArgs[0].a_w.w_long == 0))
		x->countZeros = true;
	else
		x->countZeros = false;
	
	return(x);
}

void lave_free(x)
Lave *x;
{
	
	

}