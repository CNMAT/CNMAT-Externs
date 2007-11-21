// linterp file for Max object
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


typedef struct linterp
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	double constant;
	int listsize;
	int outsize;
	int outtype;
	void *outL;
} Linterp;


void *linterp_new();
void linterp_free();
void  linterp_doLists();
void  linterp_doConstant();
void  linterp_doFloatConstant();
void  linterp_clearList();
void linterp_assist();
void testlist();
void lror_List();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)linterp_new,(method)linterp_free,(short)sizeof(struct linterp),0L,A_GIMME,0);
	addint((method)linterp_doConstant);
	addfloat((method)linterp_doFloatConstant);
	addmess((method)linterp_doLists, "list",A_GIMME,0);
	addmess((method)lror_List, "rotate",A_LONG,0);
	addmess((method)linterp_assist, "assist",A_CANT,0);
	addmess((method)linterp_clearList, "clear",0);
	finder_addclass("Lists","Linterp");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void linterp_doLists(x,s,argc,argv)
Linterp *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
	{
	//post("rightlist");
	for(i=0;i<argc;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			default:
			post("Linterp:Inapropriate List Member");
			x->theList[i].a_w.w_float = 0;
			}
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	x->listsize = argc;
	}
	
	else		// do interpolation
	{
	testlist(x,argc,argv,x->constant);
	}
}

void testlist(x,argc,argv,n)
Linterp *x;
int argc;
Atom *argv;
double n;
{
	int i;
	double result, fraction, difference;
	Atom temp[MAXSIZE];
		
		if(n > argc-1) return;
		if(n < 0) return;
		
			for(i=0; i < argc; ++i)
			{
			temp[i].a_type = A_FLOAT;
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				temp[i].a_w.w_float = (double)argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				temp[i].a_w.w_float = argv[i].a_w.w_float;
				break;
				
				default:
				temp[i].a_w.w_float = 0;
				post("Inapropriate List Member in Linterp");
				}
			}
			
			i = (long)n;  // point at member below position.
			if((long)n==n)	// first look for integer position
				{
				outlet_float(x->outL,temp[i].a_w.w_float);
				return;
				}
			fraction = n - i;
			difference = temp[i+1].a_w.w_float - temp[i].a_w.w_float;
			result = fraction * difference +temp[i].a_w.w_float;
			outlet_float(x->outL,result);
}

void  linterp_clearList(x)
Linterp *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = 1;
}

void lror_List(x,n)
Linterp *x;
long n;
{
	int i;
	int there;
	Atom temp[MAXSIZE];
	
	if(n > MAXSIZE) n = MAXSIZE;
	for(i= 0; i < x->listsize;++i)
		{
				there = (i - n);
				while(there >= x->listsize) there -= x->listsize;
				while(there < 0) there +=x->listsize;
				//post("before %d,  %d",x->theList[there].a_w.w_long, temp[i].a_w.w_long);
				switch(x->theList[there].a_type)
				{ 
				case A_LONG:
				SETLONG (temp+i,x->theList[there].a_w.w_long);
				break;
				
				case A_FLOAT:
				SETFLOAT (temp+i, x->theList[there].a_w.w_float);
				break;
				
				}
				//post("during %d,  %d",x->theList[there].a_w.w_long, temp[i].a_w.w_long);
		}
	for(i= 0; i < x->listsize;++i)
		{
					switch(temp[i].a_type)
				{ 
				case A_LONG:
				SETLONG (x->theList+i,temp[i].a_w.w_long);
				break;
				
				case A_FLOAT:
				SETFLOAT (x->theList+i, temp[i].a_w.w_float);
				break;
				
				}
				//post("after %d,  %d",x->theList[i].a_w.w_long, temp[i].a_w.w_long);
		}
	
}

void  linterp_doConstant(x,n)
Linterp *x;
long n;
{
	if(check_inlet(x))		// constant is in right inlet, replaces all values
		x->constant = (double)n;
	else			// search for constant
		testlist(x,x->listsize,x->theList,(double)n);
}


void  linterp_doFloatConstant(x,n)
Linterp *x;
double n;
{
	
	if(check_inlet(x))		// constant is in right inlet, replaces all values
		x->constant = n;
	else 
		testlist(x,x->listsize,x->theList,n);
}

void linterp_assist(x,b,msg,arg,dst)
Linterp *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to process or position to query");
			break;
			
			default:
			strcpy(dst,"Position to query or List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Interpolated value at position");//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void  *linterp_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Linterp  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Linterp *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	x->inletNumber = 0;
	
	switch(ac)
	{
	case 0:
	theArgs[0].a_type = A_FLOAT;
	theArgs[0].a_w.w_float = 0;
	
	case 1:					// note: deliberate fall through
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type =A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
		if(theArgs[0].a_type == A_FLOAT)
		x->constant = theArgs[0].a_w.w_float;
		else
		x->constant = (double)theArgs[0].a_w.w_long;
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
	x->constant = 0;
	}
	return(x);
}

void linterp_free(x)
Linterp *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Linterp *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}