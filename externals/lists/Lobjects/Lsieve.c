// lsieve file for Max object
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

typedef struct lsieve
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	void *out;
	void *outR;
	void *outL;
} Lsieve;


void *lsieve_new();
void lsieve_free();
void  lsieve_doLists();
void  lsieve_anything();
void  lsieve_doConstant();
void  lsieve_floatConstant();
void lsieve_dumpList();
void  lsieve_clearList();
void lsieve_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsieve_new,(method)lsieve_free,(short)sizeof(struct lsieve),0L,A_GIMME,0);
	addint((method)lsieve_doConstant);
	addfloat((method)lsieve_floatConstant);
	addbang((method)lsieve_dumpList);
	addmess((method)lsieve_doLists, "list",A_GIMME,0);
	addmess((method)lsieve_anything, "anything",A_GIMME,0);
	addmess((method)lsieve_assist, "assist",A_CANT,0);
	addmess((method)lsieve_clearList, "clear",0);
	finder_addclass("Lists","Lsieve");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lsieve_doLists(x,s,argc,argv)
Lsieve *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j, k, found;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet
	{
	for(i=0;i<argc;++i)
		{
		if(argv[i].a_type == A_LONG)
			{ 
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			}
			else
			x->theList[i] = argv[i];
			
		}
		
	x->listsize = argc;
	}
	else		// list in left inlet
		{
		k = 0;
		
	for(i=0;i<argc;++i)
		{
		found = 0;
		switch(argv[i].a_type) 
			{
				case A_LONG:
			for(j = 0;j < x->listsize;++j)
				{
				//post("j=%d ",j);
				if(x->theList[j].a_type == A_SYM)
					continue;
				if( (double)argv[i].a_w.w_long == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_FLOAT:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type == A_SYM)
					continue;
				if(argv[i].a_w.w_float == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_SYM:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type != A_SYM)
					continue;
				if(argv[i].a_w.w_sym == x->theList[j].a_w.w_sym)
					{
					found = 1;
					break;
					}
				}
			break;
			}
		if(found == 1)
			{
			x->outList[k] = argv[i];
			++k;
			//post("k=%d ",k);
			}
		found = 0;
		
		} //end of i loop
		if(k)
			{
			x->outsize = k;
			x->out = x->outL;
			}
		else
			{
			for(i=0;i<argc;++i)
				{
				x->outList[i].a_type = argv[i].a_type;
				x->outList[i].a_w = argv[i].a_w;
				}
			x->outsize = argc;
			x->out = x->outR;
			}
		lsieve_dumpList(x);
	}
	
}

void lsieve_anything(x,s,argc,argv)
Lsieve *x;
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
	lsieve_doLists(x,s,argc + 1,tempList);
}

void  lsieve_clearList(x)
Lsieve *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_SYM;
		x->theList[i].a_w.w_sym = 0;
		}
	
	x->listsize = 0;

	
}


void  lsieve_doConstant(x,n)
Lsieve *x;
long n;
{
	int i;
	int found;
	
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = (double)n;
	x->listsize = 1;
	}
	else
		{
	for(i=0;i<x->listsize;++i)
		{	
		if(x->theList[i].a_type == A_SYM)
					continue;
		if( n == (long)x->theList[i].a_w.w_float)
			{
			found = 1;
			break;
			}
		}
		x->outsize = 1;
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
		if(found == 1)
			x->out = x->outL;
		else
			x->out = x->outR;
		lsieve_dumpList(x);
		}
	
	
}

void  lsieve_floatConstant(x,n)
Lsieve *x;
double n;
{
	int i,found;
	
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = n;
	x->listsize = 1;
	}
	else
		{
	for(i=0;i<x->listsize;++i)
		{
		if(x->theList[i].a_type == A_SYM)
					continue;
		if( n == x->theList[i].a_w.w_float)
			{
			found = 1;
			break;
			}
		}
		x->outsize = 1;
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = n;
		if(found == 1)
			x->out = x->outL;
		else
			x->out = x->outR;
		lsieve_dumpList(x);
		}
	
	
}

void lsieve_dumpList(x)
Lsieve *x;
{
	if(x->outsize == 0)
		return;
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym,0,0);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void lsieve_assist(x,b,msg,arg,dst)
Lsieve *x;
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
			strcpy(dst,"values to allow");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"list of allowed items");
			break;
			
			default:
			strcpy(dst,"list if nothing passed");
			break;
		}
	}
	
}


void  *lsieve_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lsieve  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsieve *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	if(ac == 0){
	theArgs[0].a_type = A_SYM;
	theArgs[0].a_w.w_sym = 0;
	x->listsize = 0;
	}
	else{
		for(i=0;i<ac;++i)
			switch(theArgs[i].a_type){
				case A_LONG:
					x->theList[i].a_type = A_FLOAT;
					x->theList[i].a_w.w_float = (double)theArgs[i].a_w.w_long;
				break;
					
				case A_FLOAT:
					x->theList[i].a_type = A_FLOAT;
					x->theList[i].a_w.w_float = theArgs[i].a_w.w_float;
					break;
				
				case A_SYM:
					x->theList[i] = theArgs[i];
					break;
			}
	x->listsize = ac;
	}
	return(x);
}

void lsieve_free(x)
Lsieve *x;
{
	
	freeobject(x->proxy);
	

}
int check_inlet(x)
Lsieve *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}