// lmask file for Max object
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


typedef struct lmask
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int masksize;
	int outsize;
	int zeroExists;
	void *out;
} Lmask;


void *lmask_new();
void lmask_free();
void  lmask_List();
void  lmask_anything();
void  lmask_Int();
void  lmask_Float();
void lmask_dumpList();
void  lmask_clearList();
void lmask_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lmask_new,(method)lmask_free,(short)sizeof(struct lmask),0L,A_GIMME,0);
	addint((method)lmask_Int);
	addfloat((method)lmask_Float);
	addbang((method)lmask_dumpList);
	addmess((method)lmask_List, "list",A_GIMME,0);
	addmess((method)lmask_anything, "anything",A_GIMME,0);
	addmess((method)lmask_assist, "assist",A_CANT,0);
	addmess((method)lmask_clearList, "clear",0);
	finder_addclass("Lists","Lmask");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lmask_List(x,s,argc,argv)
Lmask *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	if(argc > MAXSIZE) argc = MAXSIZE;

	if(check_inlet(x))  // list in right inlet
	{
		lmask_clearList(x);
		for(i=0;i<argc;++i)
		{
			x->theList[i] = argv[i];
		}
		
		if((argv[argc-1].a_type == A_SYM) && argc)
			x->listsize = MAXSIZE;
		else
			x->listsize = argc;
		x->masksize = argc;
		x->inletNumber = 0;
	}
	else		// list in left inlet
	{
		
		for(i=0;i < argc ; ++i)
			{
				if(x->theList[i].a_type == A_SYM)
					x->outList[i] = argv[i];
				else
					x->outList[i] =x->theList [i];
			}
		if(argc < x->listsize)
			x->outsize = argc;
		else
			x->outsize = x->listsize;
		lmask_dumpList(x);
	}
	
}

void lmask_anything(x,s,argc,argv)
Lmask *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc >= MAXSIZE) argc = MAXSIZE-1;

	if(check_inlet(x))  // command in right inlet
	{
		lmask_clearList(x);
		if(argc)
		{
			for(i=0;i< argc;++i)
			{
				x->theList[i + 1] = argv[i];
			}
			
			if(argv[argc-1].a_type == A_SYM)
					x->listsize = MAXSIZE;
				else
					x->listsize = argc + 1;
		}
		else
			x->listsize = MAXSIZE;
		x->masksize = argc + 1;
		x->inletNumber = 0;
	}
	else		// list in left inlet
	{
		
		for(i=0;i < argc ; ++i)
			{
				if(x->theList[i].a_type == A_SYM)
					x->outList[i] = argv[i];
				else
					x->outList[i] =x->theList [i];
			}
		if(argc < x->listsize)
			x->outsize = argc;
		else
			x->outsize = x->listsize;
		outlet_anything(x->out,s,x->outsize,x->outList);
	}
}

void  lmask_clearList(x)
Lmask *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_SYM;
		}
	x->listsize = MAXSIZE;
	x->masksize = 1;
	}


void  lmask_Int(x,n)
Lmask *x;
long n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
		
		lmask_clearList(x);
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		
		x->listsize = 1;
		x->masksize = 1;
		x->inletNumber = 0;
	}
	else    // constant in left
	{
		for(i=0;i < x->masksize ; ++i)
			{
				if(x->theList[i].a_type == A_SYM)
				{
					x->outList[i].a_type = A_LONG;
					x->outList[i].a_w.w_long = n;
					
				}
				else
					x->outList[i] =x->theList [i];
			}
		x->outsize = x->masksize;
		lmask_dumpList(x);
	}
	
	
}

void  lmask_Float(x,n)
Lmask *x;
double n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
		lmask_clearList(x);
		x->theList[0].a_type = A_FLOAT;
		x->theList[0].a_w.w_float = n;
		
		x->listsize = 1;
		x->masksize = 1;
		x->inletNumber = 0;
		
	}
	else    // constant in left
	{
		for(i=0;i < x->masksize ; ++i)
			{
				if(x->theList[i].a_type == A_SYM)
				{
					x->outList[i].a_type = A_FLOAT;
					x->outList[i].a_w.w_float = n;
					
				}
				else
					x->outList[i] =x->theList [i];
			}
		x->outsize = x->masksize;
		lmask_dumpList(x);
	}
	
}
void lmask_dumpList(x)
Lmask *x;
{
	if(x->listsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	}


void lmask_assist(x,b,msg,arg,dst)
Lmask *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"list to process");
			break;
			
			default:
			strcpy(dst,"Mask: symbols pass input");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"list with desired items masked out");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
	
}

void  *lmask_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lmask  *x;
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x = (Lmask *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	for(i=0;i < MAXSIZE;++i)
			x->theList[i].a_type = A_SYM;
		x->listsize = MAXSIZE;
		x->masksize = 1;
	if(argc)
	{
		x->masksize = argc;
		for(i=0;i < argc;++i)
			x->theList[i] = argv[i];
		if(argv[argc-1].a_type == A_SYM)
			x->listsize = MAXSIZE;
		else
			x->listsize = argc + 1;
	}
	return(x);
}

void lmask_free(x)
Lmask *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lmask *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}