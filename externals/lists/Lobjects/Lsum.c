// lsum file for Max object
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

typedef struct lsum
{
	struct object ob;
	void *proxy;
	long dummy;
	void *outL;
} Lsum;


void *lsum_new();
void  lsum_doLists();
void lsum_assist();
void pass_floats();
void pass_ints();

void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsum_new,0L,(short)sizeof(struct lsum),0L,A_GIMME,0);
	
	addmess((method)lsum_doLists, "list",A_GIMME,0);
	addmess((method)lsum_assist, "assist",A_CANT,0);
	addint((method)pass_ints);
	addfloat((method)pass_floats);
	finder_addclass("Lists","Lsum");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lsum_doLists(x,s,argc,argv)
Lsum *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,count;
	float temp;
	float total;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	total = 0;
	count = 0;
	for(i=0; i < argc; ++i)
			{
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				total += temp = (float)argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				total += temp = argv[i].a_w.w_float;
				break;
				
				default:
				temp = 0;
				}
			if(temp) ++ count;
			}
	if(total == (long) total)
		outlet_int(x->outL, (long)total);
	else
		outlet_float(x->outL,total);

}

void pass_ints(x,n)
Lsum *x;
long n;
{
	outlet_int(x->outL,n);
}

void pass_floats(x,n)
Lsum *x;
float n;
{
	outlet_float(x->outL,n);
}


void lsum_assist(x,b,msg,arg,dst)
Lsum *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to add up");
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
			strcpy(dst,"Sum of members");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *lsum_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lsum  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsum *)newobject(class);
	x->outL= outlet_new(x,0L);
	
	return(x);
}

