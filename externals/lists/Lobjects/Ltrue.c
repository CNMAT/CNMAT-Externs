// ltrue file for Max object
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

typedef struct ltrue
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	int testme;
	int testOnBang;
	void *out;
	void *outR;
} Ltrue;

void *ltrue_new();
void ltrue_free();
void  ltrue_doLists();
void ltrue_dobang();
void ltrue_setlist();
void  ltrue_setConstant();
void  ltrue_floatConstant();
void ltrue_dumpList();
void  ltrue_clearList();
void ltrue_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ltrue_new,(method)ltrue_free,(short)sizeof(struct ltrue),0L,A_GIMME,0);
	addint((method)ltrue_setConstant);
	addfloat((method)ltrue_floatConstant);
	addbang((method)ltrue_dobang);
	addmess((method)ltrue_doLists, "list",A_GIMME,0);
	addmess((method)ltrue_assist, "assist",A_CANT,0);
	addmess((method)ltrue_clearList, "clear",0);
	finder_addclass("Lists","Ltrue");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void ltrue_doLists(x,s,argc,argv)
Ltrue *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x) == 1)  // list in right inlet
	{
	x->listsize = argc;
	
	for(i=0;i<argc;++i)
		{
		x->theList[i].a_type = A_LONG;
		switch(argv[i].a_type)
			{ 
			case A_FLOAT:
			if(argv[i].a_w.w_float == 0)
			x->theList[i].a_w.w_long = 0;
			else
			x->theList[i].a_w.w_long = 1;
			break;
			
			case A_LONG:
			if(argv[i].a_w.w_long == 0)
				x->theList[i].a_w.w_long = 0;
			else
				x->theList[i].a_w.w_long = 1;
			break;
			
			case A_SYM:
			x->theList[i].a_w.w_long = 0;
			}
		}
		for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	}
	else
	{
	if(argv[x->testme].a_w.w_long == 1)
		outlet_bang(x->out);
	else
		{
		for(i = 0; i < MAXSIZE; ++i)
			{
			x->outList[i].a_type = argv[i].a_type;
			x->outList[i].a_w = argv[i].a_w;
			}
		x->outsize = argc;
		outlet_list(x->outR,0L,x->outsize,x->outList);
		}
	}
	
}

void  ltrue_clearList(x)
Ltrue *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->outsize = 1;
	
}


void  ltrue_setConstant(x,n)
Ltrue *x;
long n;
{	
	if(check_inlet(x))		// constant is in right inlet
		{
		if(n > MAXSIZE) x->testme = MAXSIZE;
		if(n < 0) x->testme = 0;
		x->testme = n;
		}
	else    // constant in left
		{
		x->testOnBang = n;
		if((n >= 0) & (n < MAXSIZE))
			if(x->theList[n].a_w.w_long == 1)
				outlet_bang(x->out);
			else
				outlet_int(x->outR,n);
		}
}

void  ltrue_floatConstant(x,n)
Ltrue *x;
double n;
{
	ltrue_setConstant(x,(long)n);
}

void ltrue_dobang(x)
Ltrue *x;
{
	++ x->testOnBang;
	if(x->testOnBang >= x->listsize) x->testOnBang  = 0;
	if(x->theList[x->testOnBang].a_w.w_long == 1)
				outlet_bang(x->out);
}

void ltrue_dumpList(x)
Ltrue *x;
{
	if(x->outsize == 0)
		return;
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->outR,x->outList[0].a_w.w_long);
				else
				outlet_float(x->outR,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
}


void ltrue_assist(x,b,msg,arg,dst)
Ltrue *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Location to test");
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
			strcpy(dst,"bang if non zero");
			break;
			
			default:
			strcpy(dst,"Input if untrue");
			break;
		}
	}
}


void  *ltrue_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Ltrue  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Ltrue *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	x->listsize = ac;
	x->outsize = 0;
	x->testOnBang = 0;
	switch(ac)
	{
	case 0:
	x->testme = 0;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
		break;
	
	case 1:					
	if(theArgs[0].a_type == A_FLOAT)
		x->testme = (int)theArgs[0].a_w.w_float;
		else
		x->testme = (int)theArgs[0].a_w.w_long;
	if(x->testme > MAXSIZE) x->testme = MAXSIZE;
	if(x->testme < 0 ) x->testme = 0;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
		break;
		
	default:
	x->testme = 0;
		for(i=0;i<ac;++i)
		{
		x->theList[i].a_type = A_LONG;
		switch(theArgs[i].a_type)
			{ 
			case A_FLOAT:
			if(theArgs[i].a_w.w_float == 0)
			x->theList[i].a_w.w_long = 0;
			else
			x->theList[i].a_w.w_long = 1;
			break;
			
			case A_LONG:
			if(theArgs[i].a_w.w_long == 0)
				x->theList[i].a_w.w_long = 0;
			else
				x->theList[i].a_w.w_long = 1;
			break;
			
			case A_SYM:
			x->theList[i].a_w.w_long = 0;
			}
		}
		for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}	
	}
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = x->theList[i].a_type;
		x->outList[i].a_w = x->theList[i].a_w;
		}
	return(x);
}

void ltrue_free(x)
Ltrue *x;
{	
	freeobject(x->proxy);
}

int check_inlet(x)
Ltrue *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}