// lchange file for Max object
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

typedef struct lchange
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
	short lock;
	void *out;
	void *outL;
} Lchange;


void *lchange_new();
void lchange_free();
void  lchange_doLists();
void  lchange_setLock();
void  lchange_reset();
void  lchange_doConstant();
void  lchange_doFloatConstant();
void lchange_dumpList();
void  lchange_clearList();
void lchange_assist();
int check_inlet();

void *class;

int main( void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lchange_new,(method)lchange_free,(short)sizeof(struct lchange),0L,A_GIMME,0);
	addint((method)lchange_doConstant);
	addfloat((method)lchange_doFloatConstant);
	addbang((method)lchange_dumpList);
	addmess((method)lchange_doLists, "list",A_GIMME,0);
	addmess((method)lchange_assist, "assist",A_CANT,0);
	addmess((method)lchange_clearList, "clear",0);
	addmess((method)lchange_setLock, "lock",A_DEFLONG,0);
	addmess((method)lchange_reset, "reset",A_DEFLONG,0);
	//finder_addclass("Lists","Lchange");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	
}

void lchange_doLists(x,s,argc,argv)
Lchange *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,Same;
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
	{
	for(i=0;i<argc;++i)
	{
		x->theList[i] = argv[i];		
	}
	x->listsize = argc;
	}	
	else		// find if input matches stored list
	{
		Same = 0;
		if(argc == x->listsize)
		{
			Same = 1;
			for(i=0; i < x->listsize; ++i)
			{
				if(argv[i].a_type != x->theList[i].a_type){
					Same = 0;
					break;
					}
				if(argv[i].a_w.w_long != x->theList[i].a_w.w_long){
					Same = 0;
					break;
				}					
			}
		}
		if(Same == 0)
		{
			if(!x->lock){
			for(i=0;i<argc;++i)
				x->theList[i] = argv[i];
			x->listsize = argc;
			}
			x->outsize = argc;
			outlet_list(x->outL,0L,argc,argv);			
		}
		else
		{
			for(i=0; i < argc; ++i)
				x->outList[i] = argv[i];				
			x->outsize = argc;
			outlet_list(x->out,0L,argc,x->outList);
		}
	}
	x->inletNumber = 0;	
}

void  lchange_clearList(x)
Lchange *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_NOTHING;
		x->theList[i].a_w.w_long = 0;
		}	
	x->listsize = 1;
	x->inletNumber = 0;	
}

void  lchange_setLock(x,n)
Lchange *x;
long n;
{
	x->lock = n;
}

void  lchange_reset(x,n)
Lchange *x;
long n;
{
	x->theList[0].a_type = A_NOTHING;
	x->lock = 0;
	x->listsize = 0;
}

void  lchange_doConstant(x,n)
Lchange *x;
long n;
{	
	if(check_inlet(x))		// constant is in right inlet, set list = constant
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		x->listsize = 1;
	}	
	else			// if constant matches stored constant,  send it out the right
	{
		if((x->theList[0].a_type == A_LONG) && (x->listsize == 1) && (n == x->theList[0].a_w.w_long))
			outlet_int(x->out,n);
		else  // copy it and send it ot the left
		{
			if(!x->lock){
				x->theList[0].a_type = A_LONG;
				x->theList[0].a_w.w_long = n;
				x->listsize = 1;
			}
			outlet_int(x->outL,n);
		}
	}
	x->inletNumber = 0;	
}

void  lchange_doFloatConstant(x,n)
Lchange *x;
double n;
{
	if(check_inlet(x))		// constant is in right inlet, set list = constant
	{
		x->theList[0].a_type = A_FLOAT;
		x->theList[0].a_w.w_long = n;
		x->listsize = 1;
	}	
	else			// if constant matches stored constant,  send it out the right
	{
		if((x->theList[0].a_type == A_FLOAT) && (x->listsize == 1) && (n == x->theList[0].a_w.w_float))
			outlet_float(x->out,n);
		else
		{
			if(!x->lock){
				x->theList[0].a_type = A_FLOAT;
				x->theList[0].a_w.w_float = n;
				x->listsize = 1;
			}
			outlet_float(x->outL,n);
		}
	}
	x->inletNumber = 0;	
}

void lchange_dumpList(x)
Lchange *x;
{
	if(x->listsize == 0)
		return;
	outlet_list(x->outL,0L,x->listsize,x->theList);
	x->inletNumber = 0;
}

void lchange_assist(x,b,msg,arg,dst)
Lchange *x;
void *b;
long  msg,arg;
char *dst;
{

	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to test");
			break;
			
			default:
			strcpy(dst,"list to test against");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"list if different ");
			break;
			
			default:
			strcpy(dst,"list if same");
			break;
		}
	}
}

void  *lchange_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lchange  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lchange *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->inletNumber = 0;
	if(ac){
	for(i=0;i<ac;++i)
		x->theList[i] = theArgs[i];
		x->lock = 1;
	}else{
		x->theList[0].a_type = A_NOTHING;
		x->lock = 0;
	}
	x->listsize = ac;	
	return(x);
}

void lchange_free(x)
Lchange *x;
{	
	freeobject(x->proxy);
}

int check_inlet(x)
Lchange *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}