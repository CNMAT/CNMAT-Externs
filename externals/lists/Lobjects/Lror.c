// lror file for Max object
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


typedef struct lror
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
} Lror;


void *lror_new();
void lror_free();
void  lror_rorList();
void  lror_rorAnything();
void  lror_int();
void  lror_Constant();
void  lror_FConstant();
void lror_reror();
void  lror_clearList();
void lror_setLength();
void lror_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lror_new,(method)lror_free,(short)sizeof(struct lror),0L,A_GIMME,0);
	addint((method)lror_int);
	addfloat((method)lror_FConstant);
	addbang((method)lror_reror);
	addmess((method)lror_rorList, "list",A_GIMME,0);
	addmess((method)lror_rorAnything, "anything",A_GIMME,0);
	addmess((method)lror_assist, "assist",A_CANT,0);
	addmess((method)lror_clearList, "clear",0);
	addmess((method)lror_setLength, "length", A_DEFLONG,0);
	finder_addclass("Lists","Lror");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lror_rorList(x,s,argc,argv)
Lror *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int there;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
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
				while(there >= argc) there -= argc;
				while(there < 0) there +=argc;
				x->outList[i].a_type = argv[there].a_type;
				x->outList[i].a_w = argv[there].a_w;
		}
		x->outsize = argc;
		outlet_list(x->out,0L,argc,x->outList);
	}
	
}

void lror_rorAnything(x,s,argc,argv)
Lror *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	if(argc > MAXSIZE -1) argc = MAXSIZE - 1;
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for( i = 0; i < argc ; ++i)
		tempList[i + 1] = argv[i];
	lror_rorList(x,s,argc + 1,tempList);
}

void  lror_clearList(x)
Lror *x;
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
void lror_setLength(x,n)
Lror *x;
long n;
{
	if(n <= 0) n = 1;
	x->listsize = n;
}

void  lror_Constant(x,n)
Lror *x;
long n;
{
	int there,i;
	

	for(i=0;i<x->listsize;++i)
		{
		there = (i - (int)n);
		while(there >= x->listsize) there -= x->listsize;
		while(there < 0) there +=x->listsize;
		x->outList[i].a_type = x->theList[there].a_type;
		x->outList[i].a_w = x->theList[there].a_w;
		}
		x->outsize = x->listsize;
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}

void  lror_int(x,n)
Lror *x;
long n;
{
	x->steps = (int)n;		  // n always sets steps
	x->bangcount = 0;
	if(check_inlet(x) == 0) // n is in left and should rotate and output theList
	{
	x->bangcount = 1;
	lror_Constant(x,n);
	}
	
}

void  lror_FConstant(x,n)
Lror *x;
double n;
{	
	x->steps = (int)n;		  // n always sets steps
	x->bangcount = 0;
	if(check_inlet(x) == 0) // n is in left and should rotate and output theList
	{
	x->bangcount = 1;
	lror_Constant(x,(long)n);
	}
	
}



void lror_reror(x)
Lror *x;
{
	
	if(x->steps)
		++x->bangcount;
		lror_Constant(x,x->bangcount * x->steps);
}


void lror_assist(x,b,msg,arg,dst)
Lror *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to rotate (right), or >> n");
			break;
			
			default:
			strcpy(dst,"Steps to rotate or list to store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Rotated List");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *lror_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lror  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lror *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->inletNumber = 0;
	
	switch(ac)
	{
	case 0:
	x->steps = 0;
	
	case 1:					// note: deliberate fall through
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
		x->steps = theArgs[0].a_w.w_long;
		x->theList[0].a_w.w_long = 1;
		x->listsize = 12;
		break;
		
	default:
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
	x->steps = 0;
	x->listsize = ac;
	}
	
	
	return(x);
}

void lror_free(x)
Lror *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lror *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}