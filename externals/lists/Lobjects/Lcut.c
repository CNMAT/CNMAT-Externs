// lcut file for Max object
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


typedef struct lcut
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	Atom inList[MAXSIZE];
	int listsize;
	int outsize;
	int insize;
	double threshold;
	void *out;
} Lcut;


void *lcut_new();
void lcut_free();
void  lcut_doLists();
void  lcut_redoLists();
void  lcut_doConstant();
void  lcut_doFloatConstant();
void lcut_cutList();
void lcut_dumpList();
void  lcut_clearList();
void lcut_assist();

int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lcut_new,(method)lcut_free,(short)sizeof(struct lcut),0L,A_GIMME,0);
	addint((method)lcut_doConstant);
	addfloat((method)lcut_doFloatConstant);
	addbang((method)lcut_redoLists);
	addmess((method)lcut_doLists, "list",A_GIMME,0);
	addmess((method)lcut_assist, "assist",A_CANT,0);
	addmess((method)lcut_clearList, "clear",0);
	finder_addclass("Lists","Lcut");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lcut_doLists(x,s,argc,argv)
Lcut *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
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
			
			default:
			post("Invalid List Member");
			}
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = argc;
	}
	
	else{		// do matching useing theList[0]
	for(i=0;i<argc;++i)
		x->inList[i] = argv[i];
	x->insize = argc;
	lcut_cutList(x,argc,argv,x->threshold);
	
	}
}

void  lcut_redoLists(x)
Lcut *x;
{
	lcut_cutList(x,x->insize,x->inList,x->threshold);

}

void  lcut_clearList(x)
Lcut *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
}


void lcut_cutList(x,argc,argv,n)
Lcut *x;
int argc;
Atom *argv;
double n;
{
	int i;
	
	for(i=0; i < argc; ++i)
	{
		switch(argv[i].a_type)
		{ 
			case A_LONG:
			if((double)argv[i].a_w.w_long >= n )
				{
				x->outList[i].a_type = argv[i].a_type;
				x->outList[i].a_w.w_long = argv[i].a_w.w_long;
				}
				else
				{
				x->outList[i].a_type = A_LONG;
				x->outList[i].a_w.w_long = 0;
				}
			break;
			
			case A_FLOAT:
			if(argv[i].a_w.w_float >= n)
				{
				x->outList[i].a_type = argv[i].a_type;
				x->outList[i].a_w.w_float = argv[i].a_w.w_float;
				}
				else
				{
				x->outList[i].a_type = A_FLOAT;
				x->outList[i].a_w.w_float = 0;
				}
			break;
			
			default:
			break;
		}
	}
		
		x->outsize = argc;
		outlet_list(x->out,0L,argc,x->outList);
}
	
	
void  lcut_doConstant(x,n)
Lcut *x;
long n;
{
	if(check_inlet(x))
	x->threshold = n;
	else
	lcut_cutList(x,x->listsize,x->theList,(double)n);
}


void  lcut_doFloatConstant(x,n)
Lcut *x;
double n;
{
	if(check_inlet(x))
	x->threshold = n;
	else
	lcut_cutList(x,x->listsize,x->theList,n);
}

void lcut_dumpList(x)
Lcut *x;
{
	outlet_list(x->out,0L,x->outsize,x->outList);
}


void lcut_assist(x,b,msg,arg,dst)
Lcut *x;
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
			strcpy(dst,"Threshold");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Alpha cut list (members < threshold set to 0)");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void  *lcut_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lcut  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lcut *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->inletNumber = 0;
	
	switch(ac)
	{
	case 0:
	theArgs[0].a_type = A_FLOAT;
	theArgs[0].a_w.w_long = 0;
	x->threshold = 0.5;
	break;
	
	case 1:					
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
		x->listsize = 1;
		if(theArgs[0].a_type == A_FLOAT)
			x->threshold = theArgs[0].a_w.w_float;
		else
			x->threshold = (double)theArgs[0].a_w.w_long;
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

void lcut_free(x)
Lcut *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lcut *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}