// later file for Max object
// by Peter Elsea,  University of Calrunrnia, Santa Cruz
// copyright © 1994, Regents of the University of Calrunrnia
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define ARRAYSIZE 1024
#define ARRAYMASK 1023

typedef struct later
{
	struct object ob;
	void *proxy;
	long inletNumber;
	long numOuts;
	Atom theArray[ARRAYSIZE];
	//Atom theList[MAXSIZE];
	//Atom outList[MAXSIZE];
	long interval[32];
	long nextIn;
	void *out[32];
	void *outR;
} Later;


void *later_new();
void later_free();
void  later_doLists();
//void later_setlist();
void  later_setConstant();
void  later_floatConstant();
void  later_symbol();
void  later_doBang();
void later_dumpList();
void  later_clearList();
void stepAndOutput();
void later_assist();

int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)later_new,(method)later_free,(short)sizeof(struct later),0L,A_GIMME,0);
	addint((method)later_setConstant);
	addfloat((method)later_floatConstant);
	addbang((method)later_doBang);
	addmess((method)later_doLists, "list",A_GIMME,0);
	addmess((method)later_symbol, "anything",A_GIMME,0);
	addmess((method)later_assist, "assist",A_CANT,0);
	addmess((method)later_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void later_doLists(x,s,argc,argv)
Later *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	if(check_inlet(x)){
		for(i = 0; i < argc; ++i){
			if(i == x->numOuts) break;
			if(argv[i].a_type == A_LONG)
				x->interval[i] = argv[i].a_w.w_long;
			}
	}
	else{
		x->theArray[x->nextIn] = argv[0];
		stepAndOutput(x);
	}
}

void  later_clearList(x)
Later *x;
{
	int i;
	x->nextIn = 0;
	for(i = 0; i < ARRAYSIZE; ++i)
		x->theArray[i].a_type = A_NOTHING; // A_NOTHING is for internal use only
	
}


void  later_setConstant(x,n)
Later *x;
long n;
{
	if(check_inlet(x))	
	{
		if(n > -2 && n < ARRAYSIZE)
			x->interval[0] = n ;
	}
	else
	{
		x->theArray[x->nextIn].a_type = A_LONG;
		x->theArray[x->nextIn].a_w.w_long = n;
		stepAndOutput(x);
	}
}

void  later_floatConstant(x,n)
Later *x;
double n;
{
	x->theArray[x->nextIn].a_type = A_FLOAT;
	x->theArray[x->nextIn].a_w.w_float = n;
	stepAndOutput(x);
}

void  later_symbol(x,s,argc,argv)
Later *x;
Symbol *s;
int argc;
Atom *argv;
{
	x->theArray[x->nextIn].a_type = A_SYM;
	x->theArray[x->nextIn].a_w.w_sym = s;
	stepAndOutput(x);
}

void  later_doBang(x)
Later *x;
{
	x->theArray[x->nextIn].a_type = A_NOTHING;
	stepAndOutput(x);
}

void stepAndOutput(x)
Later *x;
{
	long nextout, base;
	int i;
	
	base = x->nextIn;
	++ x->nextIn;
	if(x->nextIn > ARRAYMASK) x->nextIn = 0;	
	
	for(i= x->numOuts-1; i >= 0; --i){
		if(x->interval[i] < 0) continue;
		nextout = (base - x->interval[i]) & ARRAYMASK;
		switch(x->theArray[nextout].a_type) {
			case A_NOTHING:
			break;
			
			case A_LONG:
			outlet_int(x->out[i],x->theArray[nextout].a_w.w_long);
			break;
			
			case A_FLOAT :
			outlet_float(x->out[i],x->theArray[nextout].a_w.w_float);
			break;
			
			case A_SYM :
			outlet_anything(x->out[i],x->theArray[nextout].a_w.w_sym, 0, 0);
			break;
		}
	}
}


void later_assist(x,b,msg,arg,dst)
Later *x;
void *b;
long  msg,arg;
char *dst;
{
	char theString[32] = "Item delayed    ticks.\0";
	if(x->interval[arg] / 10) theString[13] = (x->interval[arg] % 100 ) / 10 +48; 
	theString[14] = x->interval[arg] % 10 +48;
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Item to release later.");
			break;
			
			case 1:
			strcpy(dst,"how much later.");
			break;
			
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,theString);
	}
}


void  *later_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Later  *x;
	int i;
	
	x = (Later *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->numOuts = ac;
	if(ac < 1){ x->numOuts = 1; x->interval[0] = -1;}
	if(ac >32) x->numOuts = 32;
	//x->outR = outlet_new(x,0L); // diagnostic
	for(i= x->numOuts-1; i >= 0; --i)
		{
			x->out[i] = outlet_new(x,0L); // number outlets left to right
		}
	
	for(i = 0; i < ac; ++i){
		x->interval[i] = -1;
		if(theArgs[i].a_type != A_LONG)
			continue;
		if(theArgs[i].a_w.w_long < ARRAYSIZE) 
			x->interval[i] = theArgs[i].a_w.w_long;
	}
	x->nextIn = 0;
	for(i = 0; i < ARRAYSIZE; ++i)
		x->theArray[i].a_type = A_NOTHING; // A_NOTHING is for internal use only
	return(x);
}

void later_free(x)
Later *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Later *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}