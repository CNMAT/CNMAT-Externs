// lequ file for Max object
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

typedef struct lequ
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom inList[MAXSIZE];
	int listsize;
	int insize;
	void *outL;
	void *outR;
} Lequ;


void *lequ_new();
void lequ_free();
void  lequ_doLists();
void  lequ_doBang();
void  lequ_doInt();
void  lequ_doFloat();
void  lequ_clearList();
void lequ_assist();
int check_inlet();
void *class;

int main(void)
{

	Symbol * s;
	Symbol * l;

	
	setup((t_messlist **)&class,(method)lequ_new,(method)lequ_free,(short)sizeof(struct lequ),0L,A_GIMME,0);
	addint((method)lequ_doInt);
	addfloat((method)lequ_doFloat);
	addbang((method)lequ_doBang);
	addmess((method)lequ_doLists, "list",A_GIMME,0);
	addmess((method)lequ_assist, "assist",A_CANT,0);
	addmess((method)lequ_clearList, "clear",0);
	finder_addclass("Lists","L==");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lequ_doLists(x,s,argc,argv)
Lequ *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,howmanymatch, found;
	double result;
	double relativeLength;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet
	{
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i].a_type = A_SYM;
			x->theList[i].a_w.w_sym = 0;
			}
		}
		
		for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
		
	x->listsize = argc;
	}
	else		// list in left inlet
		{
		if(argc > x->listsize)
			relativeLength = (double)x->listsize / (double)argc;
		else
			relativeLength = (double)argc / (double)x->listsize;
		outlet_float(x->outR,relativeLength);
		howmanymatch = 0;
		
	for(i=0;i<argc;++i)
		{
		found = 0;
		x->inList[i] = argv[i];
		if(x->theList[i].a_type == A_SYM)
				{
				++howmanymatch;
				continue;
				}
		switch(argv[i].a_type) 
			{
				case A_LONG:
				if( (double)argv[i].a_w.w_long == x->theList[i].a_w.w_float)
					{
					found = 1;
					}
			break;
			
			case A_FLOAT:
				if(argv[i].a_w.w_float == x->theList[i].a_w.w_float)
					{
					found = 1;
					}
			break;
			
			default:
			found = 1;
			}
		if(found)
			{
			++howmanymatch;
			}
		
		} //end of i loop
		x->insize = argc;
		result = (double)howmanymatch/(double)argc;
		outlet_float(x->outL,result);
	}
}
	
void lequ_doBang(x)
Lequ *x;
{
	if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				lequ_doInt(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				lequ_doFloat(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		lequ_doLists(x,gensym("bang"),x->insize,x->inList);
	}


void  lequ_clearList(x)
Lequ *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	
	x->listsize = 1;

	}


void  lequ_doInt(x,n)
Lequ *x;
long n;
{
	
	int found;
	double relativeLength;
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = (double)n;
	x->listsize = 1;
	}
	else
	{
	x->inList[0].a_w.w_long = n;
	x->inList[0].a_type = A_LONG;
	x->insize = 1;
	relativeLength = 1 / (double) x->listsize;
	outlet_float(x->outR,relativeLength);
	
	switch(x->theList[0].a_type){
	
		case A_LONG:
		if(n == x->theList[0].a_w.w_long)
			outlet_float(x->outL,relativeLength);
		else
			outlet_float(x->outL,0);
		break;
		
		case A_FLOAT:
		if((float)n == x->theList[0].a_w.w_float)
			outlet_float(x->outL,relativeLength);
		else
			outlet_float(x->outL,0);
		break;
		
		default :
		 	outlet_float(x->outL,relativeLength);
	}
	}
}

void  lequ_doFloat(x,n)
Lequ *x;
double n;
{
	int found;
		double relativeLength;
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = n;
	x->listsize = 1;
	}
	else
		{
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;	
			relativeLength = 1 / (double)x->listsize;
	outlet_float(x->outR,relativeLength);
	switch(x->theList[0].a_type){
	
		case A_LONG:
		if((long)n == x->theList[0].a_w.w_long)
			outlet_float(x->outL,relativeLength);
		else
			outlet_float(x->outL,0);
		break;
		
		case A_FLOAT:
		if(n == x->theList[0].a_w.w_float)
			outlet_float(x->outL,relativeLength);
		else
			outlet_float(x->outL,0);
		break;
		
		default :
		 	outlet_float(x->outL,relativeLength);
	}
	
	}

}

void lequ_assist(x,b,msg,arg,dst)
Lequ *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to test");
			break;
			
			default:
			strcpy(dst,"List to store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Fuzzy equality of lists");
			break;
			
			default:
			strcpy(dst,"length comparison");
			break;
		}
	}
	
}

void  *lequ_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lequ  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lequ *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	
	switch(ac)
	{
	case 0:
	theArgs[0].a_type = A_SYM;
	theArgs[0].a_w.w_sym = 0;
	x->listsize = 1;
	
	case 1:					// note: deliberate fall through
	x->theList[0].a_type = A_FLOAT;
		if(theArgs[0].a_type == A_LONG)
			x->theList[0].a_w.w_float = (double)theArgs[0].a_w.w_long;
		else
			x->theList[0].a_w.w_float = theArgs[0].a_w.w_float;
	
		x->listsize = 1;
		break;
		
	default:
	for(i=0;i<ac;++i)
		{
		switch(theArgs[i].a_type)
			{ 
			case A_LONG:
			
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = (double)theArgs[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = theArgs[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i].a_type = A_SYM;
			x->theList[i].a_w.w_sym = 0;
			}
		}
		
		for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	x->listsize = ac;
	}
	
		return(x);
}

void lequ_free(x)
Lequ *x;
{
	
	freeobject(x->proxy);
	
}

int check_inlet(x)
Lequ *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}