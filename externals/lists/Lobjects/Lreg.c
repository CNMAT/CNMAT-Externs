// lreg file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ARRAYSIZE 128
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lreg
{
	struct object ob;
	void *proxy;
	long inletNumber;
	long theArray[ARRAYSIZE];
	Atom theList[MAXSIZE];
	int listsize;
	int storeNext;
	int mode;
	void *outL;
	void *outR;
} Lreg;


void *lreg_new();
void lreg_free();
void  lreg_storeList();
void lreg_setlist();
void  lreg_setConstant();
void  lreg_doConstant();
void  lreg_setMember();
void  lreg_floatConstant();
void lreg_dumpList();
void  lreg_clear();
void lreg_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lreg_new,(method)lreg_free,(short)sizeof(struct lreg),0L,A_GIMME,0);
	addint((method)lreg_setConstant);
	addfloat((method)lreg_floatConstant);
	addbang((method)lreg_dumpList);
	addmess((method)lreg_storeList, "list",A_GIMME,0);
	addmess((method)lreg_assist, "assist",A_CANT,0);
	addmess((method)lreg_clear, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lreg_storeList(x,s,argc,argv)
Lreg *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  
	
	for(i=0;i<argc;++i)		// list in either inlet
		{
			if(argv[i].a_type ==A_LONG)
			lreg_doConstant(x,argv[i].a_w.w_long);
		}
		
	if(!check_inlet(x))
		lreg_dumpList(x);
	x->inletNumber = 0;
	
}



void  lreg_clear(x)
Lreg *x;
{
	int i;
	
	
	for(i=0;i<ARRAYSIZE;++i)
		{
		x->theArray[i] = 0;
		}
	
	x->listsize = 0;
	
}


void  lreg_setConstant(x,n)
Lreg *x;
long n;
{
	
	lreg_doConstant(x,n);
	if(!check_inlet(x))
		lreg_dumpList(x);
	x->inletNumber = 0;
	
}

void  lreg_doConstant(x,n)
Lreg *x;
long n;
{
	
	
	if(n < 0 || n > 127)
	return;
	if(check_inlet(x))		// constant is in right inlet
		x->storeNext = n;
	else
	{
		
		if(x->storeNext)  // set the array location indicated by n
		{
			++x->theArray[n];
		}
		else  // clear or decrement the array location
		{
			if(x->mode)
			{
				--x->theArray[n];
				if(x->theArray[n] < 0)
					x->theArray[n] = 0;
			}
			else
				x->theArray[n] = 0;
		}
		
	}
}

void  lreg_floatConstant(x,n)
Lreg *x;
double n;
{
	
	
	lreg_doConstant(x,(long)n);
	if(!check_inlet(x))
		lreg_dumpList(x);
	x->inletNumber = 0;
	
}
void lreg_dumpList(x)
Lreg *x;
{
	int i;
	
	x->listsize = 0;
	for(i=0; i < ARRAYSIZE;++i)
	{
		if(x->theArray[i])
		{
			x->theList[x->listsize++].a_w.w_long = i;
		}
	}
	if(x->listsize)
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	else
		outlet_bang(x->outR);
	
}


void lreg_assist(x,b,msg,arg,dst)
Lreg *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data (0-127)to store or delete");
			break;
			
			default:
			strcpy(dst,"1 to store 0 to delete");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List");
			break;
			
			default:
			strcpy(dst,"bang when Lreg empty");
			break;
		}
	}
}

void  *lreg_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lreg  *x;
	int i;
	x = (Lreg *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->storeNext = 0;
	x->listsize = 0;
	if(argc)
		x->mode = argv[0].a_w.w_long;
	else
		x->mode = 0;
	for(i = 0; i < MAXSIZE; ++i)
	{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
	} 	
	for(i = 0; i < ARRAYSIZE; ++i)
	{
		x->theArray[i] = 0;
	}
	return(x);
}

void lreg_free(x)
Lreg *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lreg *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}