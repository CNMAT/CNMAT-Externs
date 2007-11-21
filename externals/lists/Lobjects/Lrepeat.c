// lrepeat file for Max object
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

typedef struct lrepeat
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outList[MAXSIZE];
	int repeats;
	int outsize;
	void *out;
} Lrepeat;


void *lrepeat_new();
void lrepeat_free();
void  lrepeat_doLists();
void  lrepeat_setConstant();
void  lrepeat_floatConstant();
void lrepeat_dumpList();
void  lrepeat_clearList();
void lrepeat_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lrepeat_new,(method)lrepeat_free,(short)sizeof(struct lrepeat),0L,A_GIMME,0);
	addint((method)lrepeat_setConstant);
	addfloat((method)lrepeat_floatConstant);
	addbang((method)lrepeat_dumpList);
	addmess((method)lrepeat_doLists, "list",A_GIMME,0);
	addmess((method)lrepeat_assist, "assist",A_CANT,0);
	addmess((method)lrepeat_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lrepeat_doLists(x,s,argc,argv)
Lrepeat *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	k=0;
	for(j=0; j<x->repeats;++j)
	{
		for(i = 0; i < argc; ++i)
		{
			x->outList[k++] = argv[i];
			if(k >= MAXSIZE)
			break;
		}
		if(k >= MAXSIZE)
			break;
	}
	if(k >= MAXSIZE)
		x->outsize = MAXSIZE;
	else
		x->outsize = k;
	x->inletNumber = 0;
	lrepeat_dumpList(x);
	
}


void  lrepeat_clearList(x)
Lrepeat *x;
{	
	
	
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = 0;
	
	x->outsize = 1;
	x->inletNumber = 0;
	
}




void  lrepeat_setConstant(x,n)
Lrepeat *x;
long n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
	
		x->repeats = n;
		if(x->repeats > MAXSIZE)
			x->repeats = MAXSIZE;
		
	}
	else    // constant in left
	{
		for (i = 0; i < x->repeats; ++i)
		{
			x->outList[i].a_type = A_LONG;
			x->outList[i].a_w.w_long = n;
		}
		x->outsize = x->repeats;
		lrepeat_dumpList(x);
	}
	x->inletNumber = 0;
	
}

void  lrepeat_floatConstant(x,n)
Lrepeat *x;
float n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
	
		x->repeats = (long) n;
		if(x->repeats > MAXSIZE)
			x->repeats = MAXSIZE;
		
	}
	else    // constant in left
	{
		for (i = 0; i < x->repeats; ++i)
		{
			x->outList[i].a_type = A_FLOAT;
			x->outList[i].a_w.w_float = n;
		}
		x->outsize = x->repeats;
		lrepeat_dumpList(x);
	}
	x->inletNumber = 0;
	
}

void lrepeat_dumpList(x)
Lrepeat *x;
{
	
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
				else
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	x->inletNumber = 0;
	
}


void lrepeat_assist(x,b,msg,arg,dst)
Lrepeat *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List, first is replaced by rest.");
			break;
			
			case 1:
			strcpy(dst,"Starting List.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"List with items replaced.");
	}
}



void  *lrepeat_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lrepeat  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lrepeat *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->outsize = 0;
	if(ac && theArgs[0].a_type == A_LONG)
		x->repeats = theArgs[0].a_w.w_long;
	else
		x->repeats = 2;
		
	
	return(x);
}

void lrepeat_free(x)
Lrepeat *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lrepeat *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}