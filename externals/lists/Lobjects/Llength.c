// length file for Max object
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


typedef struct length
{
	struct object ob;
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	void *out;
	void *message;
} Length;


void *length_new();
void length_free();
void  length_measureList();

void  length_setConstant();
void  length_floatConstant();
void  length_dumpList();
void  length_assist();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)length_new,(method)length_free,(short)sizeof(struct length),0L,A_GIMME,0);
	addint((method)length_setConstant);
	addfloat((method)length_floatConstant);
	addbang((method)length_dumpList);
	addmess((method)length_measureList, "list",A_GIMME,0);
	addmess((method)length_measureList, "anything",A_GIMME,0);
	addmess((method)length_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Length");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
	
}

void length_measureList(x,s,argc,argv)
Length *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x->listsize = argc;
	outlet_int(x->out,argc);
}



void  length_setConstant(x,n)
Length *x;
long n;
{
	x->listsize = 1;
	outlet_int(x->out,1L);
}



void  length_floatConstant(x,n)
Length *x;
double n;
{
	x->listsize = 1;
	outlet_int(x->out,1L);
}

void length_dumpList(x)
Length *x;
{
	outlet_int(x->out,x->listsize);
}


void length_assist(x,b,msg,arg,dst)
Length *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to measure");
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
			strcpy(dst,"Length of List");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
}


void  *length_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Length  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Length *)newobject(class);
	x->out= outlet_new(x,0L);
	x->listsize = 0;
	return(x);
}

void length_free(x)
Length *x;
{
}