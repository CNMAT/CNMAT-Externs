// linvert file for Max object
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


typedef struct linvert
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
} Linvert;


void 	*linvert_new();
void 	linvert_free();
void  	linvert_doLists();
void 	doInvert(Atom * divisor,Atom * theAtom,Atom * result);
void 	linvert_doBang();
void  	linvert_doInt();
void  	linvert_doFloat();
void 	linvert_doOutput();
void  	linvert_clearList();
void 	linvert_assist();
int 	check_inlet();
long	doRem();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)linvert_new,(method)linvert_free,(short)sizeof(struct linvert),0L,A_GIMME,0);
	addint((method)linvert_doInt);
	addfloat((method)linvert_doFloat);
	addbang((method)linvert_doBang);
	addmess((method)linvert_doLists, "list",A_GIMME,0);
	addmess((method)linvert_assist, "assist",A_CANT,0);
	addmess((method)linvert_clearList, "clear",0);
	finder_addclass("Lists","Linvert");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Peter Elsea and Regents of the University of California."); // first Lobject used
	s->s_thing = (struct object *)l;}
	return 0;
}

void linvert_doLists(x,s,argc,argv)
Linvert *x;
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
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long  = 1;
		}
	x->listsize = argc;
	}
	else		// list in left inlet
		{
	for(i=0;i<argc;++i)
		{
		
		x->inList[i] = argv[i];
		 doInvert(&argv[i],&x->theList[i],&x->outList[i]);
		}
		x->outsize = argc;
		x->insize = argc;
		linvert_doOutput(x);
	}
}

void doInvert(Atom * divisor,Atom * theAtom,Atom * result)
{
	if((divisor->a_type == A_FLOAT && divisor->a_w.w_float == 0) 
		|| (divisor->a_type == A_LONG && divisor->a_w.w_long == 0) ) {
		result->a_type = theAtom->a_type;
		result->a_w = theAtom->a_w;
		post("Linvert: divide by zero");
		return;
	}
	if((divisor->a_type == A_SYM)||(theAtom->a_type == A_SYM)) {
		result->a_type = divisor->a_type;
		result->a_w = divisor->a_w;
		return;
	}
	switch(theAtom->a_type){
		case A_LONG :
		if(divisor->a_type == A_LONG){
		result->a_type = A_LONG;
		result->a_w.w_long = theAtom->a_w.w_long / divisor->a_w.w_long;
		} else {
		result->a_type = A_FLOAT;
		result->a_w.w_float = (float) theAtom->a_w.w_long / divisor->a_w.w_float;
		}
		return;
		
		case A_FLOAT :
		result->a_type = A_FLOAT;
		if(divisor->a_type == A_LONG){
		result->a_w.w_float = theAtom->a_w.w_float / (float)divisor->a_w.w_long;
		} else {
		result->a_w.w_float = theAtom->a_w.w_float / divisor->a_w.w_float;
		}
		return;
		
		default:
		result->a_type = theAtom->a_type;
		result->a_w = theAtom->a_w;
		return;
	}

}


void linvert_doBang(x) 
Linvert *x;
{
		if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				linvert_doInt(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				linvert_doFloat(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		linvert_doLists(x,gensym("bang"),x->insize,x->inList);
}

void  linvert_doInt(x,n)
Linvert *x;
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
				doInvert(&tempAtom, &x->theList[i],&x->outList[i]);
			}
	x->outsize = x->listsize;
	x->inList[0].a_w.w_long = n;
	x->inList[0].a_type = A_LONG;
	x->insize = 1;
	linvert_doOutput(x);
	}
}

void  linvert_doFloat(x,n)
Linvert *x;
double n;
{
	int i;
	Atom tempAtom;
	if(check_inlet(x))		// constant is in right inlet
	{
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float =  n;
			}
		x->listsize = 1;
	}
	else
		{
		tempAtom.a_type = A_FLOAT;
		tempAtom.a_w.w_float = n;
		for(i=0;i<x->listsize;++i)
			{
				
				doInvert(&tempAtom, &x->theList[i],&x->outList[i]);
			}
		x->outsize = x->listsize;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
		linvert_doOutput(x);
		}
	
	
}
void linvert_doOutput(x)
Linvert *x;
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

void  linvert_clearList(x)
Linvert *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 1;
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 1;
		}
	
	x->listsize = 1;
	x->insize = 1;
	
}

void linvert_assist(x,b,msg,arg,dst)
Linvert *x;
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
			strcpy(dst,"Stored data divided by input");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}



void  *linvert_new(s,ac,argv)
Symbol *s;
int ac;
Atom *argv;
{

	Linvert  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Linvert *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->insize = 0;
	for(i=0;i< MAXSIZE;++i)
	{
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 1;  // no harm default if bang comes before any input
	}
	switch(ac)
	{
		case 0:
		for(i = 0 ;i<MAXSIZE;++i){
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 1;  
			}
			x->listsize = 1;
		break;
		
		case 1:					
		for(i=0;i<MAXSIZE;++i){  // set up to deal with input of any length
			x->theList[i] = argv[0];
				
		}
		x->listsize = 1;
		break;
			
		default:
		for(i=0;i<ac;++i){     // an initialized list
			x->theList[i] = argv[i];
		}
		for(;i<MAXSIZE;++i){  
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 1;  
		}
		x->listsize = ac;
	}
	
	
	x->outsize = 0;
	return(x);
}

void linvert_free(x)
Linvert *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Linvert *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}