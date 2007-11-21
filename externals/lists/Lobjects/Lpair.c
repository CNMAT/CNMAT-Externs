// lpair file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256

typedef struct lpair
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	int listsize;
	int outsize;
	void *outL;
	void *outR;
	int nextout;
} Lpair;


void *lpair_new();
void lpair_free();
void  lpair_pairLists();
void lpair_setlist();
void  lpair_setConstant();
void  lpair_floatConstant();
void lpair_dumpList();
void  lpair_clearList();
void lpair_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lpair_new,(method)lpair_free,(short)sizeof(struct lpair),0L,A_GIMME,0);
	addint((method)lpair_setConstant);
	addfloat((method)lpair_floatConstant);
	addmess((method)lpair_pairLists, "list",A_GIMME,0);
	addmess((method)lpair_assist, "assist",A_CANT,0);
	addmess((method)lpair_clearList, "clear",0);
	finder_addclass("Lists","Lpair");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lpair_pairLists(x,s,argc,argv)
Lpair *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	if(check_inlet(x))  // list in right inlet
	{
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i=0;i<argc;++i)
		{
		x->theList[i] = argv[i];
		}
		
	x->listsize = argc;
	x->nextout = 0;
	}
	else		// list in left inlet
		{
		j = 0;
	for(i=0;i<argc;++i)
		{
		if(i < x->listsize)  j = i; // otherwise j is left pointing at the last member of theList
		
		switch(x->theList[j].a_type)  
			{
				case A_LONG:
				outlet_int(x->outR,x->theList[j].a_w.w_long);
				break;
			
				case A_FLOAT:
				outlet_float(x->outR,x->theList[j].a_w.w_float);
				break;
				
				
				default:
				outlet_list(x->outR,0L,1,&x->theList[j]);
			}
			
		switch(argv[i].a_type)  
			{
				case A_LONG:
				outlet_int(x->outL,argv[i].a_w.w_long);
				break;
			
				case A_FLOAT:
				outlet_float(x->outL,argv[i].a_w.w_float);
				break;
				
				default:
				outlet_list(x->outL,0L,1,&argv[i]);
			}
		
		}
		
	}
	x->nextout = 0;
	
}

void  lpair_clearList(x)
Lpair *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;

	
}


void  lpair_setConstant(x,n)
Lpair *x;
long n;
{	
	if(check_inlet(x))		// constant is in right inlet
	{
	
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		x->nextout = 0;
		x->listsize = 1;
	}
	else
	{
		if(x->nextout >= x->listsize) x->nextout = 0;
		
		switch(x->theList[x->nextout].a_type)  
			{
				case A_LONG:
				outlet_int(x->outR,x->theList[x->nextout].a_w.w_long);
				break;
			
				case A_FLOAT:
				outlet_float(x->outR,x->theList[x->nextout].a_w.w_float);
				break;
				
				default:
				outlet_list(x->outR,0L,1,&x->theList[x->nextout]);
			}
		outlet_int(x->outL,n);
		++ x->nextout;
	}
		
	
}

void  lpair_floatConstant(x,n)
Lpair *x;
double n;
{	
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = n;
	x->nextout = 0;
	x->listsize = 1;
	}
	else
		{
		if(x->nextout == x->listsize) x->nextout = 0;
		switch(x->theList[x->nextout].a_type)  
			{
				case A_LONG:
				outlet_int(x->outR,x->theList[x->nextout].a_w.w_long);
				break;
			
				case A_FLOAT:
				outlet_float(x->outR,x->theList[x->nextout].a_w.w_float);
				break;
				
				default:
				post("Lpair:Inappropriate List Member");
			}
		outlet_float(x->outL,n);
		++ x->nextout;
		}
	
	
}




void lpair_assist(x,b,msg,arg,dst)
Lpair *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List A to iterate");
			break;
			
			default:
			strcpy(dst,"List B to interleave");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List A itered");
			break;
			
			default:
			strcpy(dst,"List B itered in sync with List A");
			break;
		}
	}
}

void  *lpair_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lpair  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lpair *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->nextout = 0;
	switch(ac)
	{
	case 0:
	x->theList[0].a_type = A_LONG;
	x->theList[0].a_w.w_long = 1;
	x->listsize = 1;
		break;
	
	case 1:					// note: deliberate fall through
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = theArgs[0].a_type;
		x->theList[i].a_w.w_long = theArgs[0].a_w.w_long;
		}
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
		x->theList[i].a_w.w_long = 1;
		}
	x->listsize = ac;
	}
	
	
	return(x);
}

void lpair_free(x)
Lpair *x;
{
	
	freeobject(x->proxy);
	

}

int check_inlet(x)
Lpair *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}