// lswap file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lswap
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	int zeroExists;
	void *out;
} Lswap;


void *lswap_new();
void lswap_free();
void  lswap_List();
void  lswap_anything();
void  lswap_Int();
void  lswap_Float();
void lswap_dumpList();
void  lswap_clearList();
void lswap_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lswap_new,(method)lswap_free,(short)sizeof(struct lswap),0L,A_GIMME,0);
	addint((method)lswap_Int);
	addfloat((method)lswap_Float);
	addbang((method)lswap_dumpList);
	addmess((method)lswap_List, "list",A_GIMME,0);
	addmess((method)lswap_anything, "anything",A_GIMME,0);
	addmess((method)lswap_assist, "assist",A_CANT,0);
	addmess((method)lswap_clearList, "clear",0);
	finder_addclass("Lists","Lswap");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lswap_List(x,s,argc,argv)
Lswap *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k, here,there;
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(argc == 1) // trap lists of length 1
		{
		switch(argv[0].a_type)
			{
			case A_LONG:
			lswap_Int(x, argv[0].a_w.w_long);
			break;	

			case A_FLOAT:
			lswap_Float(x,argv[0].a_w.w_float);
			break;		
			}
		
		return;
		}
	if(check_inlet(x))  // list in right inlet
	{
	j = 0;
	x->zeroExists = 0;
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->theList[j].a_type = A_LONG;
			x->theList[j++].a_w.w_long = argv[i].a_w.w_long;
			if(argv[i].a_w.w_long == 0)
				x->zeroExists = 1;
			break;
			
			case A_FLOAT:
			x->theList[j].a_type = A_LONG;
			x->theList[j++].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(argv[i-1].a_type != A_SYM)
				x->theList[j++].a_type = A_SYM;
			if(i == argc-1 && i > 1)
				{
				--j;
				k = 1;
				for(; j < MAXSIZE; ++j)
					{
					x->theList[j].a_type = A_LONG;
					x->theList[j].a_w.w_long = x->theList[j-1].a_w.w_long +
							 x->theList[k].a_w.w_long - x->theList[k-1].a_w.w_long;
					if(x->theList[j].a_w.w_long < 0)
					{
					x->theList[j].a_w.w_long = 256;
					post("Warning: negative series in Lswap");
					break;
					}
					++k;
					if(k == argc - 1) k = 1;
					}
				}
			break;
			
			default:
			post("Invalid List Member");
			}
		}
		
	x->listsize = j;
	x->inletNumber = 0;
	}
	else		// list in left inlet
	{
		for(i = 0; i < argc && i < x->listsize; ++i)
			{
			x->outList[i].a_type = A_SYM;
			x->outList[i].a_w.w_sym = 0;
			}
		j = 0;
		for(i=0;i < x->listsize ; ++i)
			{
				if(x->theList[i].a_type == A_LONG)
				{
					here = x->theList[i].a_w.w_long;
					if(here < 0)
						here += argc;
					if(here < argc)  // otherwise it's out of range for this input
						x->outList[j++] = argv[here];
				}
				if(x->theList[i].a_type == A_SYM) // symbol indicates range
				{
					if(x->theList[i-1].a_w.w_long > -1) // previous is 0 or positive
						here = x->theList[i-1].a_w.w_long;
					else
						here = argc + x->theList[i-1].a_w.w_long; // previous is negative, count back from end
					
					if(x->theList[i+1].a_w.w_long > -1) // next is 0 or positive
						there = x->theList[i+1].a_w.w_long;
					else
						there = argc + x->theList[i+1].a_w.w_long; // next is negative, count back from end
					
					if(here < 0) here = 0;  // fix anything out of range for this input
					if(here > argc) here = argc;
					if(there < 0) there = 0;
					if(there > argc) there = argc;
					
					if(here > there)
						{
							for(k = here - 1; k > there; --k){
								x->outList[j].a_type = argv[k].a_type;
								x->outList[j++].a_w = argv[k].a_w;
								}
						}
					if(here < there)
						{
							for(k = here + 1; k < there; ++k){
								x->outList[j].a_type = argv[k].a_type;
								x->outList[j++].a_w = argv[k].a_w;
								}
						}
					if(here == there) ++i;  // we've already seen this one
				}
			}
		x->outsize = j;
		lswap_dumpList(x);
	}
	
}

void lswap_anything(x,s,argc,argv)
Lswap *x;
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
	lswap_List(x,s,argc + 1,tempList);
}

void  lswap_clearList(x)
Lswap *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	
}


void  lswap_Int(x,n)
Lswap *x;
long n;
{	
	if(check_inlet(x))		// constant is in right inlet
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		if(n == -1) 
			x->zeroExists = 1;
		else
			x->zeroExists = !n;
		x->listsize = 1;
		x->inletNumber = 0;
	}
	else    // constant in left
	{
		if(x->zeroExists)
		{
			x->outList[0].a_type = A_LONG;
			x->outList[0].a_w.w_long = n;
			x->outsize = 1;
			lswap_dumpList(x);
		}
	}
	
	
}

void  lswap_Float(x,n)
Lswap *x;
double n;
{	
	if(check_inlet(x))		// constant is in right inlet
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = (long)n;
		if(n == -1) 
			x->zeroExists = 1;
		else
			x->zeroExists = !n;
		x->listsize = 1;
		x->inletNumber = 0;
	}
	else
	{
		if(x->zeroExists)
		{
			x->outList[0].a_type = A_FLOAT;
			x->outList[0].a_w.w_float = n;
			x->outsize = 1;
			lswap_dumpList(x);
		}
	}
	
	
}
void lswap_dumpList(x)
Lswap *x;
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
				outlet_anything(x->out,x->outList[0].a_w.w_sym, 0, 0);
			}
		else
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
			else
				outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void lswap_assist(x,b,msg,arg,dst)
Lswap *x;
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
			strcpy(dst,"Swap template");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Rearranged list");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *lswap_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lswap  *x;
	int i,j,k;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x = (Lswap *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->zeroExists = 0;
	j = 0;
	if(argc)
	{
		for(i=0;i < argc;++i)
		{
			switch(argv[i].a_type)
				{ 
				case A_LONG:
				x->theList[j].a_type = A_LONG;
				x->theList[j++].a_w.w_long = argv[i].a_w.w_long;
				if(argv[i].a_w.w_long == 0) x->zeroExists = 1;
				break;
				
				case A_FLOAT:
				x->theList[j].a_type = A_LONG;
				x->theList[j++].a_w.w_long = (long)argv[i].a_w.w_float;
				break;
				
				case A_SYM:
			if(argv[i-1].a_type != A_SYM)  // syms mean fill in - two will break list
				x->theList[j++].a_type = A_SYM;
			if(i == argc-1 && i > 1)  // symbol at end means take the rest in implied series
				{
				--j;
				k = 1;
				for(; j < MAXSIZE; ++j)
					{
					x->theList[j].a_type = A_LONG;
					x->theList[j].a_w.w_long = x->theList[j-1].a_w.w_long +
							 x->theList[k].a_w.w_long - x->theList[k-1].a_w.w_long;
					if(x->theList[j].a_w.w_long < 0)
					{
					x->theList[j].a_w.w_long = 256;
					post("Warning: negative series in Lswap");
					break;
					}
					++k;
					if(k == argc - 1) k = 1;
					}
				}
			break;
				
				default:
				post("Invalid List Member");
				}
		}
		x->listsize = j;
	}
	else
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = -1;
		x->theList[1].a_type = A_SYM;
		x->theList[2].a_type = A_LONG;
		x->theList[2].a_w.w_long = 0;
		x->listsize = 3;
		x->zeroExists = 1;
	}
	
	
	
	return(x);
}

void lswap_free(x)
Lswap *x;
{
	
	freeobject(x->proxy);
	

}
int check_inlet(x)
Lswap *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}