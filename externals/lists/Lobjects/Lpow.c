// lpow file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2004, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#include "math.h"

#define MAXSIZE 256
#define ASSIST_INLET 1  // used in assist method
#define ASSIST_OUTLET 2


typedef struct lpow
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
} Lpow;


void 	*lpow_new();
void 	lpow_free();
void  	lpow_doLists();
void 	lpow_doBang();
void  	lpow_doInt();
void  	lpow_doFloat();
void 	lpow_doOutput();
void  	lpow_clearList();
void 	lpow_assist();
int 	isint(double b);
int 	check_inlet();
double dopow();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	setup((t_messlist **)&class,(method)lpow_new,(method)lpow_free,(short)sizeof(struct lpow),0L,A_GIMME,0);
	addint((method)lpow_doInt);
	addfloat((method)lpow_doFloat);
	addbang((method)lpow_doBang);
	addmess((method)lpow_doLists, "list",A_GIMME,0);
	addmess((method)lpow_assist, "assist",A_CANT,0);
	addmess((method)lpow_clearList, "clear",0);
	finder_addclass("Lists","Lpow");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Peter Elsea and Regents of the University of California."); // first Lobject used
	s->s_thing = (struct object *)l;}
	return 0;
}

double dopow(a,b)
double a;
double b;
{

	double result;
	result = pow(a,b);
	if(a < 0 && !isint(b)) 
		{
		post("Lpow: Undefined power operation!");
		return(a);
		}
	else return(result);
}

int isint(double b)
{
	return b == (double)((long)b);
}

void lpow_doLists(x,s,argc,argv)
Lpow *x;
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
		x->theList[i] = argv[i];
		}
	for(;i<MAXSIZE;++i)
		{
		x->theList[i].a_type  = A_LONG;
		x->theList[i].a_w.w_long  = 0;
		}
	x->listsize = argc;
	}
	else		// list in left inlet
		{
	for(i=0;i<argc;++i)
		{
		
		x->inList[i] = argv[i];
		if((x->theList[i].a_type != A_LONG) && (x->theList[i].a_type != A_FLOAT))
			x->outList[i] = argv[i];
		else
		switch(argv[i].a_type) // promote types as necessary
		{
			case A_LONG:
			if(x->theList[i].a_type == A_LONG){
			x->outList[i].a_type = A_LONG;
			x->outList[i].a_w.w_long = (long)dopow((double)argv[i].a_w.w_long, (double)x->theList[i].a_w.w_long);
			}else{
			x->outList[i].a_type = A_FLOAT;
			x->outList[i].a_w.w_float = dopow((double)argv[i].a_w.w_long, (double)x->theList[i].a_w.w_float);
			}
			break;
			
			case A_FLOAT:
			x->outList[i].a_type = A_FLOAT;
			if(x->theList[i].a_type == A_FLOAT)
			
			x->outList[i].a_w.w_float = dopow((double)argv[i].a_w.w_float , (double)x->theList[i].a_w.w_float);
			else
			x->outList[i].a_w.w_float = dopow((double)argv[i].a_w.w_float , (double)x->theList[i].a_w.w_long);
			break;
			
			default:
			x->outList[i] = argv[i];
			}
		}
		x->outsize = argc;
		x->insize = argc;
		lpow_doOutput(x);
	}
}


void lpow_doBang(x)  // bang recalculates output based on new input, if any
Lpow *x;

{
	if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				lpow_doInt(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				lpow_doFloat(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		lpow_doLists(x,gensym("bang"),x->insize,x->inList);
}

void  lpow_doInt(x,n)
Lpow *x;
long n;
{
	int i;
	
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
		for(i=0;i<x->listsize;++i)
			{
				if(x->theList[i].a_type == A_SYM){
				SETLONG(&x->outList[i],n);
				continue;
				}
				x->outList[i].a_type = x->theList[i].a_type;
				
				if(x->theList[i].a_type == A_LONG)
					x->outList[i].a_w.w_long = (long)dopow((double)n ,(double) x->theList[i].a_w.w_long);
				else
					x->outList[i].a_w.w_float = dopow((float)n , (double)x->theList[i].a_w.w_float);
			
			}
	x->outsize = x->listsize;
	x->inList[0].a_w.w_long = n;
		x->inList[0].a_type = A_LONG;
		x->insize = 1;
	lpow_doOutput(x);
	}
	
	
}

void  lpow_doFloat(x,n)
Lpow *x;
float n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
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
		for(i=0;i<x->listsize;++i)
			{
				if(x->theList[i].a_type == A_SYM){
				SETLONG(&x->outList[i],n);
				continue;
				}
				x->outList[i].a_type = A_FLOAT;
			
				if(x->theList[i].a_type == A_LONG)
				x->outList[i].a_w.w_float = dopow(n , (float) x->theList[i].a_w.w_long);
				else
				x->outList[i].a_w.w_float =dopow( n , x->theList[i].a_w.w_float);
			
			}
		x->outsize = x->listsize;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
		lpow_doOutput(x);
		}
	
	
}
void lpow_doOutput(x)
Lpow *x;
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

void  lpow_clearList(x)
Lpow *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	x->insize = 1;
	
}

void lpow_assist(x,b,msg,arg,dst)
Lpow *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Raise to the listed power");
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
			strcpy(dst,"Left to the right power");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}



void  *lpow_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lpow  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lpow *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->insize = ac;
	for(i=0;i<ac;++i)
	{
		x->inList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;  // no harm default if bang comes before any input
	}
	switch(ac)
	{
		case 0:
		for(i = 0 ;i<MAXSIZE;++i){
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;  // no harm default
		}
		x->listsize = 1;
		break;
		
		case 1:					
		for(i=0;i<MAXSIZE;++i){  // set up to deal with input of any length
			x->theList[i].a_type = theArgs[0].a_type;
			x->theList[i].a_w.w_long = theArgs[0].a_w.w_long;
		}
		x->listsize = 1;
		break;
			
		default:
		for(i=0;i<ac;++i){     // an initialized list
			x->theList[i].a_type = theArgs[i].a_type;
			x->theList[i].a_w.w_long = theArgs[i].a_w.w_long;
		}
		for(;i<MAXSIZE;++i){  // input longer than initialized should nt be modified.
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;
		}
		x->listsize = ac;
	}
	
	return(x);
}

void lpow_free(x)
Lpow *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lpow *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}