// labs file for Max object
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


typedef struct labs
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	void *outL;
} Labs;


void *labs_new();
void labs_free();
void  labs_List();
void labs_dumpList();
void labs_assist();
void labs_doFloat();
void labs_doInt();

int check_inlet();
void *class;

int main(void)
{

	Symbol * s;
	Symbol * l;

	setup((t_messlist **)&class,(method)labs_new,(method)labs_free,(short)sizeof(struct labs),0L,A_DEFLONG,0);
	addint((method)labs_doInt);
	addfloat((method)labs_doFloat);
	addbang((method)labs_dumpList);
	addmess((method)labs_List, "list",A_GIMME,0);
	addmess((method)labs_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Labs");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void labs_doInt(x,n)
Labs *x;
long n;
{
	long absn;
	
		if(n < 0) 
			{
			 absn = 0 - n;
			}
		else
			absn = n;
		outlet_int(x->outL, absn);
}

void labs_doFloat(x,n)
Labs *x;
double n;
{
	double absn;
	
		if(n < 0) 
			{
			 absn = 0 - n;
			}
		else
			absn = n;
		outlet_float(x->outL,absn);
}

void labs_List(x,s,argc,argv)
Labs *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x->outsize = argc;
	for(i=0;i<argc;++i)
		{
		x->outList[i].a_type = argv[i].a_type;
		switch(argv[i].a_type) 
			{
			case A_LONG:

			x->outList[i].a_w.w_long = argv[i].a_w.w_long;
			if(x->outList[i].a_w.w_long < 0)
				x->outList[i].a_w.w_long = -x->outList[i].a_w.w_long;

			break;
			
			case A_FLOAT:
			
			x->outList[i].a_w.w_float = argv[i].a_w.w_float;
			if(x->outList[i].a_w.w_float <0)
			x->outList[i].a_w.w_float = -x->outList[i].a_w.w_float;
			break;
			
			default:
			//post("Invalid List Member");
			x->outList[i].a_w.w_sym = argv[i].a_w.w_sym;
			}
		}
		outlet_list(x->outL,0L,x->outsize,x->outList);
}



void labs_dumpList(x)
Labs *x;
{
	
	outlet_list(x->outL,0L,x->outsize,x->outList);
}


void labs_assist(x,b,msg,arg,dst)
Labs *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"data to process");
			break;
			
			default:
			strcpy(dst,"List to store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Absolute values.");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *labs_new()
{

	Labs  *x;
	int i;
	
	x = (Labs *)newobject(class);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	x->outsize  = 12;
	
	
	
	return(x);
}

void labs_free(x)
Labs *x;
{
}