// lmatch file for Max object
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


typedef struct lmatch
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	long constant;
	int incount;
	int listsize;
	int outsize;
	int outtype;
	int noWrap;
	void *out;
	void *outL;
} Lmatch;


void *lmatch_new();
void lmatch_free();
void  lmatch_doLists();
void  lmatch_anything();
void  lmatch_doConstant();
void  lmatch_doFloatConstant();
void lmatch_dumpList();
void  lmatch_clearList();
void lmatch_assist();
void lmatch_wrapon();
void lmatch_wrapoff();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lmatch_new,(method)lmatch_free,(short)sizeof(struct lmatch),0L,A_GIMME,0);
	addint((method)lmatch_doConstant);
	addfloat((method)lmatch_doFloatConstant);
	addbang((method)lmatch_dumpList);
	addmess((method)lmatch_doLists, "list",A_GIMME,0);
	addmess((method)lmatch_anything, "anything",A_GIMME,0);
	addmess((method)lmatch_assist, "assist",A_CANT,0);
	addmess((method)lmatch_clearList, "clear",0);
	addmess((method)lmatch_wrapon, "wrapon", 0);
	addmess((method)lmatch_wrapoff, "wrapoff", 0);
	//finder_addclass("Lists","Lmatch");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lmatch_doLists(x,s,argc,argv)
Lmatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,there, end;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
	{
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			
			x->theList[i].a_type = argv[i].a_type;
			x->theList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_type = argv[i].a_type;
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i].a_type = argv[i].a_type;
			x->theList[i].a_w.w_sym = argv[i].a_w.w_sym;
			}
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = argc;
	}
	
	else		// do matching
	{
	if(argc <= x->listsize)
	{
	end = x->listsize - x->noWrap * argc;
	for(j=0;j < end;++j)
		{
		for(i=0; i < argc; ++i)
			{
			there = i + j;
			if(there >= x->listsize)  there -= x->listsize;
			if(argv[i].a_type == A_SYM) continue;
			if(x->theList[there].a_type == A_SYM) continue;
				if(argv[i].a_type != x->theList[there].a_type)
					break;
			
			if(argv[i].a_type == A_LONG)
				{
					if(argv[i].a_w.w_long != x->theList[there].a_w.w_long)
						break;
				}
				
			if(argv[i].a_type == A_FLOAT)
				{
					if(argv[i].a_w.w_float != x->theList[there].a_w.w_float)
						break;
				}
			}
		if(i == argc)   //success
			{
			outlet_int(x->outL,(long)j);
						return;
			}
		}
	}
		for(i=0; i < argc; ++i)
			switch(argv[i].a_type)
			{ 
			case A_LONG:
			
			x->outList[i].a_type = argv[i].a_type;
			x->outList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->outList[i].a_type = argv[i].a_type;
			x->outList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i].a_type = argv[i].a_type;
			x->theList[i].a_w.w_sym = argv[i].a_w.w_sym;
			}
		x->outsize = argc;
		outlet_list(x->out,0L,argc,x->outList);
	}
}

void lmatch_anything(x,s,argc,argv)
Lmatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	
	if(argc){
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
		tempList[i+1] = argv[i];
	
	lmatch_doLists(x,s,argc+1,tempList);
	}else{   // search for a single symbol
		for(i = 0; i < x->listsize;++i){
			//postatom(&x->theList[i]);
			if(x->theList[i].a_type == A_SYM)
				if(x->theList[i].a_w.w_sym == s){
					outlet_int(x->outL, i);
					return;
				}
		}
	outlet_anything(x->out,s,0,NIL);
	}
}

void  lmatch_clearList(x)
Lmatch *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = 1;
	
	}

void lmatch_wrapon(x)
Lmatch *x;
{
	x->noWrap = 0;
	}

void lmatch_wrapoff(x)
Lmatch *x;
{
	x->noWrap = 1;
	}

void  lmatch_doConstant(x,n)
Lmatch *x;
long n;
{
	int i;
	
	if(x->inletNumber)		// constant is in right inlet, replaces all values
	{

	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = n;
		}
	x->listsize = 1;
	}
	
	else			// search for constant
	{
	for(i=0;i<x->listsize;++i)
		{
			if(x->theList[i].a_type != A_LONG)
				continue;
			
			if(n == x->theList[i].a_w.w_long)	
				{
				outlet_int(x->outL,(long)i);
								return;
				}
		}
		x->outList[0].a_type = A_LONG;
			x->outList[0].a_w.w_long = n;
		x->outsize = 1;
		outlet_list(x->out,0L,1,x->outList);
	}
}


void  lmatch_doFloatConstant(x,n)
Lmatch *x;
double n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet, replaces all values
		{

			for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = n;
			}
		x->listsize = 1;
		}
	else 
		{
	for(i=0;i<x->listsize;++i)
		{
			if(x->theList[i].a_type != A_FLOAT)
				continue;
			
			if(n == x->theList[i].a_w.w_float)	
				{
				outlet_int(x->outL,(long)i);
								return;
				}
		}
		x->outList[0].a_type = A_FLOAT;
			x->outList[0].a_w.w_float = n;
		x->outsize = 1;
		outlet_list(x->out,0L,1,x->outList);
	}
		
	}

void lmatch_dumpList(x)
Lmatch *x;
{
	
		outlet_list(x->out,0L,x->outsize,x->outList);
			
	}


void lmatch_assist(x,b,msg,arg,dst)
Lmatch *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to look for.");
			break;
			
			default:
			strcpy(dst,"List to search.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Position of  list");//forgot something
			break;
			
			default:
			strcpy(dst,"List if not found");
			break;
		}
	}
}

void  *lmatch_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lmatch  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lmatch *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->outL= intout(x);
	x->inletNumber = 0;
	x->noWrap = 0;
	
	switch(ac)
	{
	case 0:
	theArgs[0].a_type = A_LONG;
	theArgs[0].a_w.w_long = 0;
	
	case 1:					// note: deliberate fall through
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = theArgs[0].a_type;
		x->theList[i].a_w.w_long = theArgs[0].a_w.w_long;
		}
		x->listsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->theList[i].a_type = theArgs[i].a_type;
		x->theList[i].a_w.w_long = theArgs[i].a_w.w_long;
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

void lmatch_free(x)
Lmatch *x;
{
	
	freeobject(x->proxy);
	
}

int check_inlet(x)
Lmatch *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}