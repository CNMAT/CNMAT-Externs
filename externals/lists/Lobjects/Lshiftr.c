// lshiftr file for Max object
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


typedef struct lshiftr
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int steps;
	int bangcount;
	int listsize;
	int outsize;
	int outtype;
	void *out;
} Lshiftr;


void *lshiftr_new();
void lshiftr_free();
void  lshiftr_doLists();
void  lshiftr_anything();
void lshiftr_setlist();
void  lshiftr_Constant();
void  lshiftr_FConstant();
void lshiftr_reshift();
void  lshiftr_clearList();
void lshiftr_setLength();
void lshiftr_assist();
void  lshiftr_int();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lshiftr_new,(method)lshiftr_free,(short)sizeof(struct lshiftr),0L,A_GIMME,0);
	addint((method)lshiftr_int);
	addfloat((method)lshiftr_FConstant);
	addbang((method)lshiftr_reshift);
	addmess((method)lshiftr_doLists, "list",A_GIMME,0);
	addmess((method)lshiftr_anything, "anything",A_GIMME,0);
	addmess((method)lshiftr_assist, "assist",A_CANT,0);
	addmess((method)lshiftr_clearList, "clear",0);
	addmess((method)lshiftr_setLength, "length", A_DEFLONG,0);
	finder_addclass("Lists","Lshiftr");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lshiftr_doLists(x,s,argc,argv)
Lshiftr *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int there;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(x->inletNumber)  // list in right inlet replaces stored list.
	{
	x->bangcount = 0;
	for(i=0;i<argc;++i)
		{
			x->theList[i].a_type = argv[i].a_type;
			x->theList[i].a_w = argv[i].a_w;
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = argc;
	}
	else  // list is in left and needs to be rotated
	{
	for(i=0;i<argc;++i)
		{
				there = (i - x->steps);
				if(there >= argc) there = argc-1;
				if(there < 0) there = 0;
				x->outList[i].a_type = argv[there].a_type;
				x->outList[i].a_w = argv[there].a_w;
		}
		x->outsize = argc;
		outlet_list(x->out,0L,argc,x->outList);
	}
	
}

void lshiftr_anything(x,s,argc,argv)
Lshiftr *x;
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
	lshiftr_doLists(x,s,argc + 1,tempList);
}

void  lshiftr_clearList(x)
Lshiftr *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->theList[0].a_w.w_long = 1;
	x->listsize = 12;
	x->steps = 0;
	x->bangcount = 0;
	
}
void lshiftr_setLength(x,n)
Lshiftr *x;
long n;
{
	
	
	if(n == 0) n = 1;
	x->listsize = n;
	
	
}
void  lshiftr_Constant(x,n)
Lshiftr *x;
long n;
{
	int there,i;
	

	for(i=0;i<x->listsize;++i)
		{
				there = (i - (int)n);
				while(there >= x->listsize) there = x->listsize-1;
				while(there < 0) there = 0;
				x->outList[i].a_type = x->theList[there].a_type;
				x->outList[i].a_w = x->theList[there].a_w;
		}
		x->outsize = x->listsize;
		outlet_list(x->out,0L,x->outsize,x->outList);

	
}

void  lshiftr_int(x,n)
Lshiftr *x;
long n;
{			
	x->steps = (int)n;
	x->bangcount = 0;
  if(x->inletNumber == 0)	// n is in left and should rotate and output theList
  		{
  		x->bangcount = 1;
		lshiftr_Constant(x,n);
		}
	
}

void  lshiftr_FConstant(x,n)
Lshiftr *x;
double n;
{	
	x->steps = (int)n;
	x->bangcount = 0;
  if(x->inletNumber == 0)	// n is in left and should rotate and output theList
  	{
  	x->bangcount = 1;
	lshiftr_Constant(x,(long)n);
	}
	
}

void lshiftr_reshift(x)
Lshiftr *x;
{
	
	if(x->steps)
	++x->bangcount;
		lshiftr_Constant(x,x->bangcount * x->steps);
			
	
}


void lshiftr_assist(x,b,msg,arg,dst)
Lshiftr *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to shift (right)");
			break;
			
			default:
			strcpy(dst,"steps to shift");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Shifted list");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *lshiftr_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lshiftr  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lshiftr *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->inletNumber = 0;
	x->steps = 1;
	x->bangcount = 0;
	x->listsize = 12;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->theList[0].a_w.w_long = 1;
	
	switch(ac)
	{
	case 0:
	break;
	
	case 1:					// one arg sets step
	
		if(theArgs[0].a_type == A_LONG)
			x->steps = theArgs[0].a_w.w_long;
		break;
		
	default:  // list sets internal list
		for(i=0;i<ac;++i)
		{
		x->theList[i].a_type = theArgs[i].a_type;
		x->theList[i].a_w = theArgs[i].a_w;
		}
		for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = ac;
	}
	
	
	return(x);
}

void lshiftr_free(x)
Lshiftr *x;
{
	
	freeobject(x->proxy);
	

}

int check_inlet(x)
Lshiftr *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}