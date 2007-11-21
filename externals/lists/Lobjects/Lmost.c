// lmost file for Max object
// by Peter Elsea,  University of Calmostornia, Santa Cruz
// copyright © 2002, Regents of the University of Calmostornia
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lmost
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	long inletNumber;
	
	Atom outList[MAXSIZE];
	Atom trueList[MAXSIZE];
	Atom falseList[MAXSIZE];
	long outsize;
	void *out;
	void *outR;
} Lmost;


void *lmost_new();
void lmost_free();
void  lmost_doList();
void lmost_anything();
void  lmost_doLong();
void  lmost_doFloat();
void lmost_dumpList();
void  lmost_clearList();
void lmost_assist();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lmost_new,(method)lmost_free,(short)sizeof(struct lmost),0L,A_GIMME,0);
	addint((method)lmost_doLong);
	addfloat((method)lmost_doFloat);
	addbang((method)lmost_dumpList);
	addmess((method)lmost_doList, "list",A_GIMME,0);
	addmess((method)lmost_anything, "anything",A_GIMME,0);
	addmess((method)lmost_assist, "assist",A_CANT,0);
	//finder_addclass("Lists","Lmost");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of Calmostornia.");
	s->s_thing = (struct object *)l;
	}
	return 0;
}

void lmost_doList(x,s,argc,argv)
Lmost *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,mostsofar;
	short thisone = 0;
	long duplicates[MAXSIZE];
	mostsofar = 0;
	for(i = 0; i < argc; ++i)
	 	duplicates[i] = 0;
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i = 0; i < argc; ++i){
		for(j = 0; j < argc; ++j)
			if(argv[j].a_w.w_long == argv[i].a_w.w_long)
				++ duplicates[i];
	}
	for(i = 0; i < argc; ++i){
		if(duplicates[i] > mostsofar){
			mostsofar = duplicates[i];
			
			thisone = i;
		}
	}
	outlet_int(x->outR,mostsofar);
	x->outList[0] = argv[thisone];
	lmost_dumpList(x);
}

void lmost_anything(x,s,argc,argv)
Lmost *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	if(argc >= MAXSIZE) argc = MAXSIZE-1;
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
		tempList[i +1] = argv[i];
	lmost_doList(x,s,argc + 1,tempList);
}



void  lmost_doLong(x,n)
Lmost *x;
long n;
{
	outlet_int(x->out,n);
}



void  lmost_doFloat(x,n)
Lmost *x;
double n;
{
	outlet_float(x->out,n);
}

void lmost_dumpList(x)
Lmost *x;
{
	if(x->outsize){
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym , 0, NIL);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	}
}


void lmost_assist(x,b,msg,arg,dst)
Lmost *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List ");
			break;
			
			case 1:
			strcpy(dst,"List of results for true");
			break;
			
			default:
			strcpy(dst,"List of results for false");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"most common item");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
}


void  *lmost_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lmost  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lmost *)newobject(class);
	x->outR= outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	x->outsize = 1;
	
	return(x);
}

void lmost_free(x)
Lmost *x;
{
}