// lrem file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2004, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1  // used in assist method
#define ASSIST_OUTLET 2


typedef struct lrem
{
	struct object ob;
	void *proxy;
	long inletNumber;  // needed for classic
	Atom theList[MAXSIZE];  // stored list
	int listsize;
	Atom outList[MAXSIZE];  // list that will be output
	int outsize;
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;	
	void *out;  // just one outlet for these
} Lrem;


void 	*lrem_new();
void 	lrem_free();
void  	lrem_doLists();
void 	lrem_doBang();
void  	lrem_doInt();
void  	lrem_doFloat();
void 	lrem_doOutput();
void  	lrem_clearList();
void 	lrem_assist();
int 	check_inlet();
long	doRem();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	setup((t_messlist **)&class,(method)lrem_new,(method)lrem_free,(short)sizeof(struct lrem),0L,A_GIMME,0);
	addint((method)lrem_doInt);
	addfloat((method)lrem_doFloat);
	addbang((method)lrem_doBang);
	addmess((method)lrem_doLists, "list",A_GIMME,0);
	addmess((method)lrem_assist, "assist",A_CANT,0);
	addmess((method)lrem_clearList, "clear",0);
	finder_addclass("Lists","Lrem");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Peter Elsea and Regents of the University of California."); // first Lobject used
	s->s_thing = (struct object *)l;}
	return 0;
}

void lrem_doLists(x,s,argc,argv)
Lrem *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet
	{
	for(i=0;i<argc;++i)
	{
		switch(argv[i].a_type){
		case A_LONG:
		x->theList[i].a_w.w_long = argv[i].a_w.w_long; // rem is for ints only
		break;
		
		case A_FLOAT:
		x->theList[i].a_w.w_long = (long)argv[i].a_w.w_float;
		break;
		
		default:
		x->theList[i].a_w.w_long = 0;  // zeros mark no op
		}
	}
	for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_w.w_long  = 0;
		}
	x->listsize = argc;
	}
	else		// list in left inlet
		{
	for(i=0;i<argc;++i)
		{
		
		x->inList[i] = argv[i];
		x->outList[i].a_w.w_long = doRem(&argv[i],x->theList[i].a_w.w_long);
		}
		x->outsize = argc;
		x->insize = argc;
		lrem_doOutput(x);
	}
}


void lrem_doBang(x) 
Lrem *x;
{
		if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				lrem_doInt(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				lrem_doFloat(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		lrem_doLists(x,gensym("bang"),x->insize,x->inList);
}

void  lrem_doInt(x,n)
Lrem *x;
long n;
{
	int i;
	Atom tempAtom;
	if(check_inlet(x))		// constant is in right inlet
	{
		
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = n;
			}
		x->listsize = 1;
	}
	else    // constant in left
	{
		tempAtom.a_type = A_LONG;
		tempAtom.a_w.w_long = n;
		for(i=0;i<x->listsize;++i)
			{
				x->inList[i].a_w.w_long  = n;
				x->outList[i].a_w.w_long = doRem(&tempAtom, x->theList[i].a_w.w_long);
			}
	x->outsize = x->listsize;
	x->inList[0].a_w.w_long = n;
	x->inList[0].a_type = A_LONG;
	x->insize = 1;
	lrem_doOutput(x);
	}
}

void  lrem_doFloat(x,n)
Lrem *x;
double n;
{
	int i;
	Atom tempAtom;
	if(check_inlet(x))		// constant is in right inlet
	{
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_w.w_long =(long) n;
			}
		x->listsize = 1;
	}
	else
		{
		tempAtom.a_type = A_LONG;
		tempAtom.a_w.w_long = (long)n;
		for(i=0;i<x->listsize;++i)
			{
				x->inList[i].a_w.w_long  = (long)n;
				x->outList[i].a_w.w_long = doRem(&tempAtom, x->theList[i].a_w.w_long);
			}
		x->outsize = x->listsize;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
		lrem_doOutput(x);
		}
}

void lrem_doOutput(x)
Lrem *x;
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

void  lrem_clearList(x)
Lrem *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 1;
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	x->insize = 1;
	
}

void lrem_assist(x,b,msg,arg,dst)
Lrem *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Divide by stored list");
			break;
			
			default:
			strcpy(dst,"store this");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Remainders of L/R");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}



void  *lrem_new(s,ac,argv)
Symbol *s;
int ac;
Atom *argv;
{

	Lrem  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lrem *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->insize = 0;
	for(i=0;i<MAXSIZE;++i)
	{
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 1;  // no harm default if bang comes before any input
		x->theList[i].a_type = A_LONG;
	}
	switch(ac)
	{
		case 0:
		for(i = 0 ;i<MAXSIZE;++i)
			x->theList[i].a_w.w_long = 1;  
			x->listsize = 1;
		break;
		
		case 1:					
		for(i=0;i<MAXSIZE;++i){  // set up to deal with input of any length
			switch(argv[0].a_type){
				case A_LONG:
				x->theList[i].a_w.w_long = argv[0].a_w.w_long;
				break;
				
				case A_FLOAT:
				x->theList[i].a_w.w_long = (long)argv[0].a_w.w_float;
				break;
				
				default:
				x->theList[i].a_w.w_long = 1;  
				}
		}
			x->listsize = 1;
		break;
			
		default:
		for(i=0;i<ac;++i){     // an initialized list
			switch(argv[i].a_type){
				case A_LONG:
				x->theList[i].a_w.w_long = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				x->theList[i].a_w.w_long = (long)argv[i].a_w.w_float;
				break;
				
				default:
				x->theList[i].a_w.w_long = 1;  
				}
			}
		for(;i<MAXSIZE;++i){  // input longer than initialized should not be modified.
			x->theList[i].a_w.w_long = 1;  
		}
		x->listsize = ac;
	}
	
	for(i=0;i<MAXSIZE;++i)
	{
		x->outList[i].a_type = A_LONG;
	}
	x->outsize = 0;
	return(x);
}

void lrem_free(x)
Lrem *x;
{
	freeobject(x->proxy);
}

long doRem(Atom * theAtom, long n)
{
	if(n == 0){
	post("Lrem: divide by 0");
	 return 0;
	 }
	
	switch(theAtom->a_type){
	case A_LONG:
	return(theAtom->a_w.w_long % n);
	break;
	
	case A_FLOAT:
	return((long)theAtom->a_w.w_float % n);
	break;
	
	default :
	return 0;
	
	}

}

int check_inlet(x)
Lrem *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}