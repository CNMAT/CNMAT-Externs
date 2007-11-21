// linfer file for Max object
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

typedef struct linfer
{
	struct object l_ob;
	void *l_proxy;
	long dummy;
	long inletNumber;
	Atom l_theList[MAXSIZE];
	Atom l_outList[MAXSIZE];
	double l_constant;
	int l_listsize;
	int l_outsize;
	int l_outtype;
	void *l_outL;
} Linfer;

void *linfer_new();
void linfer_free();
void  linfer_doLists();
void  linfer_doConstant();
void  linfer_doFloatConstant();
void  linfer_clearList();
void linfer_assist();
void testlist();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)linfer_new,(method)linfer_free,(short)sizeof(struct linfer),0L,A_GIMME,0);
	addint((method)linfer_doConstant);
	addfloat((method)linfer_doFloatConstant);
	addmess((method)linfer_doLists, "list",A_GIMME,0);
	addmess((method)linfer_assist, "assist",A_CANT,0);
	addmess((method)linfer_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void linfer_doLists(x,s,argc,argv)
Linfer *x;
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
		x->l_theList[i].a_type = A_FLOAT;
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->l_theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->l_theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			default:
			post("Invalid List Member");
			}
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		x->l_theList[i].a_type = A_FLOAT;
		x->l_theList[i].a_w.w_float = 0;
		}
	x->l_listsize = argc;
	}
	
	else		// do interpolation
	{
	testlist(x,argc,argv,x->l_constant);
	}
	
	
}

void testlist(x,argc,argv,n)
Linfer *x;
int argc;
Atom *argv;
double n;
{
	int i,j, firstnonzero, lastnonzero;
	double result, fraction, difference;
	Atom testList[MAXSIZE];
	
	
		for(i=0; i < argc; ++i)  // copy list to floats
			{
				testList[i].a_type = A_FLOAT;
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				
				testList[i].a_w.w_float = (double)argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				testList[i].a_w.w_float = argv[i].a_w.w_float;
				break;
				
				default:
				post("Invalid List Member");
				}
			}
		if(n > argc-1)
				{
				outlet_float(x->l_outL,testList[argc-1].a_w.w_float);
				
					return;
				}
			if(n < 0)
				{
				outlet_float(x->l_outL,testList[0].a_w.w_float);
				
					return;
				}
			firstnonzero = lastnonzero = -1;// look for boundries
			for(i=0; i < argc; ++i) 
			{
			if(testList[i].a_w.w_float)
				{
				if(firstnonzero == -1)
					firstnonzero = i;
				lastnonzero = i;
				}
			}
			
			if(firstnonzero == -1)  // do not evaluate empty sets
				{
				
					return;
				}
				
			if(n < (double)firstnonzero || n > (double)lastnonzero)  // return zero for alphacut region
				{
				outlet_float(x->l_outL,0);
				
				return;
				}
			
			i = (long)n;  // point at member below position.
			j = (long)n +1;
					
			if((long)n==n)	// first look for integer position
				{
				if(testList[i].a_w.w_float || n == 0 || n == argc-1)
					{
					outlet_float(x->l_outL,testList[i].a_w.w_float);
					
					return;
					}
				}
				
			while(testList[i].a_w.w_float == 0)
				{
				
				if(i <= firstnonzero - 1)
					break;
				if(i == 0)
					break;
				--i;
				}
				
			while(testList[j].a_w.w_float == 0)
				{
				
				if(j >= lastnonzero)
					break;
				if(j == argc-1)
					break;
				++j;
				}
			
			fraction = n - i;
			difference = testList[j].a_w.w_float - testList[i].a_w.w_float;
			result = fraction * difference;
			result /= (j-i);
			result += testList[i].a_w.w_float;
			outlet_float(x->l_outL,result);

	
}

void  linfer_clearList(x)
Linfer *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->l_theList[i].a_type = A_LONG;
		x->l_theList[i].a_w.w_long = 0;
		}
	
	x->l_listsize = 1;
	
	
}

void  linfer_doConstant(x,n)
Linfer *x;
long n;
{	
	
	if(check_inlet(x))		// constant is in right inlet, replaces all values
	{
	x->l_constant = (double)n;
	}
	
	else			// search for constant
	{
	testlist(x,x->l_listsize,x->l_theList,(double)n);
	}
	
	
}


void  linfer_doFloatConstant(x,n)
Linfer *x;
double n;
{

	if(check_inlet(x))		// constant is in right inlet, replaces all values
		{
		x->l_constant = n;
		}
	else 
		{
	testlist(x,x->l_listsize,x->l_theList,n);
		}
		
	
}




void linfer_assist(x,b,msg,arg,dst)
Linfer *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to find value in or position to query.");
			break;
			
			default:
			strcpy(dst,"List or position to store.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Interpolated value at position, ignoring 0s");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void  *linfer_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Linfer  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Linfer *)newobject(class);
	x->l_proxy = proxy_new(x,1L,&x->inletNumber);
	x->l_outL= outlet_new(x,0L);
	x->inletNumber = 0;
	
	switch(ac)
	{
	case 0:
	theArgs[0].a_type = A_FLOAT;
	theArgs[0].a_w.w_float = 0;
	
	case 1:					// note: deliberate fall through
	for(i=0;i<MAXSIZE;++i)
		{
		x->l_theList[i].a_type =A_FLOAT;
		x->l_theList[i].a_w.w_float = 0;
		}
		if(theArgs[0].a_type == A_FLOAT)
			x->l_constant = theArgs[0].a_w.w_float;
		else
			x->l_constant = (double)theArgs[0].a_w.w_long;
		x->l_listsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->l_theList[i].a_type = theArgs[i].a_type;
		x->l_theList[i].a_w.w_long = theArgs[i].a_w.w_long;
		}
		for(;i<MAXSIZE;++i)
		{
		x->l_theList[i].a_type = A_LONG;
		x->l_theList[i].a_w.w_long = 0;
		}
	x->l_listsize = ac;
	x->l_constant = 0;
	}
	
	
	return(x);
}

void linfer_free(x)
Linfer *x;
{
	freeobject(x->l_proxy);
}

int check_inlet(x)
Linfer *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}