// llimit file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.


#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct llimit
{
	struct object ob;
	void *proxy;
	long inletNumber;
	long upperInt;
	double upperFloat;
	long lowerInt;
	double lowerFloat;
	Atom outList[MAXSIZE];
	int outsize;
	void *out;
} Llimit;


void *llimit_new();
void llimit_free();
void  llimit_accumList();
void llimit_setlist();
void  llimit_setConstantL();
void  llimit_addto();
void  llimit_setConstantF();
void llimit_dumpList();
void  llimit_clearList();
void llimit_assist();
void llimit_flush();
void fixAtomRange();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)llimit_new,(method)llimit_free,(short)sizeof(struct llimit),0L,A_GIMME,0);
	addint((method)llimit_setConstantL);
	addfloat((method)llimit_setConstantF);
	addbang((method)llimit_dumpList);
	addmess((method)llimit_accumList, "list",A_GIMME,0);
	addmess((method)llimit_addto, "addto",A_GIMME,0);
	addmess((method)llimit_assist, "assist",A_CANT,0);
	addmess((method)llimit_clearList, "clear",0);
	addmess((method)llimit_flush, "flush",0);
	finder_addclass("Lists","Llimit");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void llimit_accumList(x,s,argc,argv)
Llimit *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	double tempF;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type) // promote to double if necessary
			{
		case A_LONG:
			
			if(x->outList[i].a_type == A_LONG)
			{
				x->outList[i].a_w.w_long += argv[i].a_w.w_long;
				
			}
			else		//convert input to double if output is double
			{
				x->outList[i].a_w.w_float += (double)argv[i].a_w.w_long;
			}
			break;
			
		case A_FLOAT:
			
			if(x->outList[i].a_type == A_FLOAT)
				x->outList[i].a_w.w_float += argv[i].a_w.w_float;
			
			else 		//convert output to double
				{
				tempF = (double)x->outList[i].a_w.w_long;
				x->outList[i].a_type = A_FLOAT;
				x->outList[i].a_w.w_float = tempF + argv[i].a_w.w_float;
				}
			break;
			
		default:
			//post("Invalid List Member");
			break;
			}
			fixAtomRange(x,&x->outList[i]);
		}
		if(argc > x->outsize) x->outsize = argc;
		if(check_inlet(x)==0) llimit_dumpList(x);
	
	
}

void fixAtomRange(x,theAtom)
Llimit * x;
Atom * theAtom;
{
	if(theAtom->a_type == A_FLOAT)
	{
		if(theAtom->a_w.w_float < x->lowerFloat)
			theAtom->a_w.w_float = x->lowerFloat;
		
		if(theAtom->a_w.w_float > x->upperFloat)
			theAtom->a_w.w_float = x->upperFloat;
	}
	if(theAtom->a_type == A_LONG)
	{
		if(theAtom->a_w.w_long < x->lowerInt)
			theAtom->a_w.w_long = x->lowerInt;
		
		if(theAtom->a_w.w_long > x->upperInt)
			theAtom->a_w.w_long = x->upperInt;
	}

}

void llimit_addto(x,s,argc,argv)
Llimit *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	double tempF;
	
	if((argc > 1) && (argv[0].a_type == A_LONG))
	{
		i = argv[0].a_w.w_long;
		if((i< 0)|| (i > MAXSIZE)) return;
		switch(argv[1].a_type) // promote to double if necessary
			{
				case A_LONG:
			
			if(x->outList[i].a_type == A_LONG)
			{
				x->outList[i].a_w.w_long += argv[1].a_w.w_long;
				
			}
			else		//convert input to double if output is double
			{
				x->outList[i].a_w.w_float += (double)argv[1].a_w.w_long;
			}
			break;
			
			case A_FLOAT:
			
			if(x->outList[i].a_type == A_FLOAT)
				x->outList[i].a_w.w_float += argv[1].a_w.w_float;
			
			else 		//convert output to double
				{
				tempF = (double)x->outList[i].a_w.w_long;
				x->outList[i].a_type = A_FLOAT;
				x->outList[i].a_w.w_float = tempF + argv[1].a_w.w_float;
				}
			break;
			
			default:
			//post("Invalid List Member");
			break;
			}
			fixAtomRange(x,&x->outList[i]);
			if(check_inlet(x)==0) llimit_dumpList(x);
		}
	
	
}
	
void llimit_flush(x)
Llimit *x;
{
	
	
	llimit_dumpList(x);
	llimit_clearList(x);
	
}

void  llimit_clearList(x)
Llimit *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	
	x->outsize = 1;
	
}


void  llimit_setConstantL(x,n)
Llimit *x;
long n;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
			
			if(x->outList[i].a_type == A_LONG)
			x->outList[i].a_w.w_long += n;
			
			else
			{
			x->outList[i].a_w.w_float += (double)n;
			}
			fixAtomRange(x,&x->outList[i]);
		}
	if(check_inlet(x)==0) llimit_dumpList(x);
	
}

void  llimit_setConstantF(x,n)
Llimit *x;
double n;
{
	int i;
	long tempL;

	
	
	for(i=0;i<MAXSIZE;++i)
		{
			if(x->outList[i].a_type == A_FLOAT)
				x->outList[i].a_w.w_float += n;
			else
			{
			tempL = x->outList[i].a_w.w_long;
			x->outList[i].a_type = A_FLOAT;
			x->outList[i].a_w.w_float = n + (double)tempL;
			}
			fixAtomRange(x,&x->outList[i]);
		}
	if(check_inlet(x)==0) llimit_dumpList(x);
	
}

void llimit_dumpList(x)
Llimit *x;
{
	
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
				else
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void llimit_assist(x,b,msg,arg,dst)
Llimit *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Add to list and output");
			break;
			
			default:
			strcpy(dst,"Add to list");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List clipped to bounds");
			break;
			
			default:
			strcpy(dst,"");
			break;
		}
	}
	
	
}


void  *llimit_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Llimit  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Llimit *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->upperInt = 1;
	x->upperFloat = 1;
	x->lowerInt = 0;
	x->lowerFloat = 0;
	x->outsize = 0;
	if(ac > 1)
	{
		if(theArgs[0].a_type == A_FLOAT)
		{
			x->lowerFloat = theArgs[0].a_w.w_float;
			x->lowerInt = (long)x->lowerFloat;
		}
		else if(theArgs[0].a_type == A_LONG)
		{
			x->lowerInt = theArgs[0].a_w.w_long;
			x->lowerFloat = (double)x->lowerInt;
		}
		
		if(theArgs[1].a_type == A_FLOAT)
		{
			x->upperFloat = theArgs[1].a_w.w_float;
			x->upperInt = (long)x->upperFloat;
		}
		else if(theArgs[1].a_type == A_LONG)
		{
			x->upperInt = theArgs[1].a_w.w_long;
			x->upperFloat = (double)x->upperInt;
		}
	
	}
	for(i = 0; i < MAXSIZE; ++ i)
	{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
	}
	
	return(x);
}

void llimit_free(x)
Llimit *x;
{
	
	freeobject(x->proxy);
	

}
int check_inlet(x)
Llimit *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}