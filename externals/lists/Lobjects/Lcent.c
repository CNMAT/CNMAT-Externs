// lcent file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2004, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.	

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
typedef struct lcent
{
	struct object l_ob;
	//void *l_proxy;
	long dummy;
	void *l_outL;
} Lcent;


void *lcent_new();
void lcent_free();
void  lcent_doLists();
void ldefault_constant();
void ldefault_Fconstant();
void ldefault();
void lcent_assist();

void *class;
int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lcent_new,(method)lcent_free,(short)sizeof(struct lcent),0L,A_GIMME,0);
	addint((method)ldefault_constant);
	addfloat((method)ldefault_Fconstant);
	addmess((method)ldefault, "anything",A_GIMME,0);
	addmess((method)lcent_doLists, "list",A_GIMME,0);
	addmess((method)lcent_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void ldefault(x,s,argc,argv)
Lcent *x;
Symbol *s;
int argc;
Atom *argv;
{
	outlet_float(x->l_outL,0);
}

void ldefault_constant(x,n)
Lcent *x;
long n;
{
	outlet_float(x->l_outL,0);
}

void ldefault_Fconstant(x,n)
Lcent *x;
double n;
{
	outlet_float(x->l_outL,0);
}

void lcent_doLists(x,s,argc,argv)
Lcent *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	float total,moment, temp;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	total = 0;
	moment = 0;
	for(i=0; i < argc; ++i)
			{
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				temp = (float)argv[i].a_w.w_long;
				total += temp;
				break;
				
				case A_FLOAT:
				temp = argv[i].a_w.w_float;
				total += temp;
				break;
				
				default:
				temp = 0;
				}
				moment += (float)i * temp;
			}
		if(total ==0)
			{
			
			return;
			}

		
		moment = moment / total;
		
	outlet_float(x->l_outL,moment);
	
}


void lcent_assist(x,b,msg,arg,dst)
Lcent *x;
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
			strcpy(dst,"Weighted center of list.");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void  *lcent_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lcent  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lcent *)newobject(class);
	x->l_outL= outlet_new(x,0L);
	
	return(x);
}

void lcent_free(x)
Lcent *x;
{
	
	

}