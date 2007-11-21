// lfind file for Max object
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

typedef struct lfind
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
	void *outL;
} Lfind;


void *lfind_new();
void lfind_free();
void  lfind_doLists();
void  lfind_doConstant();
void  lfind_doFloatConstant();
void  lfind_clearList();
void lfind_assist();
void testlist();
int check_inlet();

void *lfind_class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&lfind_class,(method)lfind_new,(method)lfind_free,(short)sizeof(struct lfind),0L,A_GIMME,0);
	addint((method)lfind_doConstant);
	addfloat((method)lfind_doFloatConstant);
	addmess((method)lfind_doLists, "list",A_GIMME,0);
	addmess((method)lfind_assist, "assist",A_CANT,0);
	addmess((method)lfind_clearList, "clear",0);
	//finder_addclass("Lists","Lfind");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lfind_doLists(x,s,argc,argv)
Lfind *x;
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
			switch(argv[i].a_type){
			case A_LONG:
			x->theList[i].a_w.w_float = (float)argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			default:
			x->theList[i].a_w.w_float = 0;
			break;
			}
		}
	for(i=argc;i<MAXSIZE;++i) //Dummy values for rest of list
		{
		//x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	x->listsize = argc;
	}
	
	else		// do matching
	{
	testlist(x,argc,argv,x->theList[0].a_w.w_float);
	}
	
	
}

void testlist(x,argc,argv,n)
Lfind *x;
int argc;
Atom *argv;
float n;
{
	int i,justunder, justover;
	float result, highestsofar;
	Atom temp[MAXSIZE];
	
		justunder = justover = -1;
			for(i=0; i < argc; ++i)
			{
			temp[i].a_type = A_FLOAT;
				switch(argv[i].a_type)
				{ 
				case A_LONG:
				
				temp[i].a_w.w_float = (float)argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				temp[i].a_w.w_float = argv[i].a_w.w_float;
				break;
				
				default:
				temp[i].a_w.w_float = 0;
				}
			}
			
			for(i = 0; i < argc; ++i)  // first look for exact match
				{
				if(temp[i].a_w.w_float==n)
					{
					outlet_float(x->outL,(float)i);
					return;
					}
				}
				
			highestsofar = -2147483640;
			for(i = 0; i < argc; ++i) // find location just below value
				{
				if(temp[i].a_w.w_float < n)
					{
					if(temp[i].a_w.w_float >= highestsofar)
						{
						highestsofar = temp[i].a_w.w_float;
						justunder = i;
						}
					}
				else
					break;
				}
			if(justunder == -1)  // everything in list is greater than n
				{
				for(i = argc-1; i >= 0; --i)
					if(temp[i].a_w.w_float < n)
					{
					if(temp[i].a_w.w_float >= highestsofar)
						{
						highestsofar = temp[i].a_w.w_float;
						justunder = i;
						}
					}
				else
					break;
				}
			if(justunder == -1)
				{
				
					return;
				}
			
			for (i = justunder; i < argc; ++i) // find location just above value
				{
				if(temp[i].a_w.w_float > n)
					{
					justover = i;
					break;
					}
				}
			if(justover == -1)
				{
				for(i = justunder; i >= 0; --i)
					{
					if(temp[i].a_w.w_float > n)
						{
						justover = i;
						break;
						}
					}
				}
			if(justover == -1)
				{
				
					return;
				}
			if(justover > justunder)
				{
				result = n-temp[justunder].a_w.w_float;
				result /= temp[justover].a_w.w_float - temp[justunder].a_w.w_float;
				result *=(float)(justover-justunder);
				result += (float)justunder;
				outlet_float(x->outL,result);
				//post("ov = %ld un = %ld re = %f",(long)justover, (long)justunder, result);
				
				return;
				}
			else   // declining curve
				{
				result = temp[justover].a_w.w_float-n;
				result /= temp[justover].a_w.w_float - temp[justunder].a_w.w_float;
				result *=  (float)(justunder-justover);
				result += (float)justover;
				outlet_float(x->outL,result);
				//post("ov = %ld un = %ld re = %f",(long)justover, (long)justunder, result);
				
				return;
				}
	
}

void  lfind_clearList(x)
Lfind *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	
	
}

void  lfind_doConstant(x,n)
Lfind *x;
long n;
{
	int i;
	
	
	if(check_inlet(x))		// constant is in right inlet, replaces all values
	{

	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = (float)n;
		}
	x->listsize = 1;
	}
	
	else			// search for constant
	{
	testlist(x,x->listsize,x->theList,(float)n);
	}
}


void  lfind_doFloatConstant(x,n)
Lfind *x;
float n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet, replaces all values
		{

			for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = n;
			}
		x->listsize = 1;
		}
	else 
		{
	testlist(x,x->listsize,x->theList,n);
		}
}




void lfind_assist(x,b,msg,arg,dst)
Lfind *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to search or target to search for");
			break;
			
			default:
			strcpy(dst,"List to Store or target to search for");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Position of target in list");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void  *lfind_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lfind  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lfind *)newobject(lfind_class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	x->inletNumber = 0;
	for(i = 0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = 0;
		}
	switch(ac)
	{
	case 0:
		x->listsize = 1;
		break;
	
	case 1:					
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i] = theArgs[0];
		}
		x->listsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->theList[i] = theArgs[i];
		x->listsize = ac;
		}
	}
	
	
	return(x);
}

void lfind_free(x)
Lfind *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lfind *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}