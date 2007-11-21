// lsearch file for Max object
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


typedef struct lsearch
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	Atom inList[MAXSIZE];
	long constant;
	int insize;
	int listsize;
	int outsize;
	int outtype;
	int wrap;
	void *outR;
	void *outL;
} Lsearch;


void *lsearch_new();
void lsearch_free();
void  lsearch_doLists();
void  lsearch_anything();
void  lsearch_doConstant();
void  lsearch_doFloatConstant();
void lsearch_dumpList();
void lsearch_doBang();
void  lsearch_clearList();
void lsearch_assist();
void lsearch_wrap();
void lsearch_wrapon();
void lsearch_wrapoff();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsearch_new,(method)lsearch_free,(short)sizeof(struct lsearch),0L,A_GIMME,0);
	addint((method)lsearch_doConstant);
	addfloat((method)lsearch_doFloatConstant);
	addbang((method)lsearch_doBang);
	addmess((method)lsearch_doLists, "list",A_GIMME,0);
	addmess((method)lsearch_anything, "anything",A_GIMME,0);
	addmess((method)lsearch_assist, "assist",A_CANT,0);
	addmess((method)lsearch_clearList, "clear",0);
	addmess((method)lsearch_wrap, "wrap", A_DEFLONG,0);
	addmess((method)lsearch_wrapon, "wrapon", 0);
	addmess((method)lsearch_wrapoff, "wrapoff", 0);
	//finder_addclass("Lists","Lsearch");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}
void lsearch_doBang(x)
Lsearch *x;
{
	lsearch_doLists(x,0,x->insize,x->inList);
}

void lsearch_doLists(x,s,argc,argv)
Lsearch *x;
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
		x->listsize = argc;
	}
	else		// do matching
	{
		for(i = 0; i < argc;++i)
		x->inList[i] = argv[i];
		x->insize = argc;
		
		x->outsize = 0;
		if(argc > x->listsize)
			lsearch_dumpList(x);
		else
		{
			if(x->wrap)
				end = x->listsize;
			else
				end = x->listsize - argc+1;
			for(j=0;j < end;++j) // j is front end of search target
			{
				for(i=0; i < argc; ++i)
				{
					there = i + j;
					if(there >= x->listsize)  there -= x->listsize;
					if(argv[i].a_type != x->theList[there].a_type)
						break;
					if(argv[i].a_w.w_long != x->theList[there].a_w.w_long)
						break;
				}
				if(i == argc)   //success
				{
					x->outList[x->outsize++].a_w.w_long = j;
				}
			}
		}
		lsearch_dumpList(x);
	}
}
void lsearch_anything(x,s,argc,argv)
Lsearch *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	x->inList[0].a_type = A_SYM;
	x->inList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
			x->inList[i+1] = argv[i];
	x->insize = argc+1;
	lsearch_doLists(x,s,x->insize,x->inList);
}

void  lsearch_clearList(x)
Lsearch *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = 0;
}
void lsearch_wrap(x,n)
Lsearch *x;
long n;
{
	x->wrap = n;
}
void lsearch_wrapon(x)
Lsearch *x;
{
	x->wrap = 1;
}

void lsearch_wrapoff(x)
Lsearch *x;
{
	x->wrap = 0;
}

void  lsearch_doConstant(x,n)
Lsearch *x;
long n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet, replaces all values
	{

	x->theList[0].a_type = A_LONG;
	x->theList[0].a_w.w_long = n;
	x->listsize = 1;
	}
	
	else			// search for constant
	{
	x->inList[0].a_type = A_LONG;
		x->inList[0].a_w.w_long = n;
		x->insize = 1;
	x->outsize = 0;
	for(i=0;i<x->listsize;++i)
		{
			if(x->theList[i].a_type != A_LONG)
				continue;
			if(n == x->theList[i].a_w.w_long)	
				x->outList[x->outsize++].a_w.w_long = i;
		}
		lsearch_dumpList(x);
	}
}


void  lsearch_doFloatConstant(x,n)
Lsearch *x;
double n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet, replaces all values
	{
		x->theList[0].a_type = A_FLOAT;
		x->theList[0].a_w.w_float = n;
		x->listsize = 1;
	}
	else 
	{
		x->inList[0].a_type = A_FLOAT;
		x->inList[0].a_w.w_float = n;
		x->insize = 1;
		x->outsize = 0;
		for(i=0;i<x->listsize;++i)
		{
			if(x->theList[i].a_type != A_FLOAT)
				continue;
			if(n == x->theList[i].a_w.w_float)	
				x->outList[x->outsize++].a_w.w_long = i;
		}
	}
	lsearch_dumpList(x);
}
		


void lsearch_dumpList(x)
Lsearch *x;
{
	if(x->outsize ==0)
		outlet_int(x->outL,0L);
	else{
		if(x->outsize == 1)
			outlet_int(x->outR,x->outList[0].a_w.w_long);
		else
			outlet_list(x->outR,0L,x->outsize,x->outList);
		outlet_int(x->outL,1L);
	}
}


void lsearch_assist(x,b,msg,arg,dst)
Lsearch *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"data to look for.");
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
			strcpy(dst,"1 if found, 0 if not");
			break;
			
			default:
			strcpy(dst,"List of locations of data");
			break;
		}
	}
}

void  *lsearch_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	
	Lsearch  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsearch *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= intout(x);
	x->inletNumber = 0;
	x->wrap = 0;
	x->insize = 0;
	for(i=0;i<MAXSIZE;++i)
		x->outList[i].a_type = A_LONG;
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

void lsearch_free(x)
Lsearch *x;
{
	
	freeobject(x->proxy);
	
}

int check_inlet(x)
Lsearch *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}