// ldiv file for Max object
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


typedef struct ldiv
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
} Ldiv;


void 	*ldiv_new();
void 	ldiv_free();
void  	ldiv_doLists();
void doDiv(Atom * theAtom,Atom * divisor,Atom * result);
void 	ldiv_doBang();
void  	ldiv_doInt();
void  	ldiv_doFloat();
void 	ldiv_doOutput();
void  	ldiv_clearList();
void 	ldiv_assist();
int 	check_inlet();
long	doRem();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	setup((t_messlist **)&class,(method)ldiv_new,(method)ldiv_free,(short)sizeof(struct ldiv),0L,A_GIMME,0);
	addint((method)ldiv_doInt);
	addfloat((method)ldiv_doFloat);
	addbang((method)ldiv_doBang);
	addmess((method)ldiv_doLists, "list",A_GIMME,0);
	addmess((method)ldiv_assist, "assist",A_CANT,0);
	addmess((method)ldiv_clearList, "clear",0);
	finder_addclass("Lists","Ldiv");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Peter Elsea and Regents of the University of California."); // first Lobject used
	s->s_thing = (struct object *)l;}
	return 0;
}

void ldiv_doLists(x,s,argc,argv)
Ldiv *x;
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
		 doDiv(&argv[i],&x->theList[i],&x->outList[i]);
		}
		x->outsize = argc;
		x->insize = argc;
		ldiv_doOutput(x);
	}
}

void doDiv(Atom * theAtom,Atom * divisor,Atom * result)
{
	if((divisor->a_type == A_FLOAT && divisor->a_w.w_float == 0) 
		|| (divisor->a_type == A_LONG && divisor->a_w.w_long == 0) ) {
		result->a_type = theAtom->a_type;
		result->a_w = theAtom->a_w;
		post("Ldiv: divide by zero");
		return;
	}
	if((divisor->a_type == A_SYM)||(theAtom->a_type == A_SYM)) {
		result->a_type = theAtom->a_type;
		result->a_w = theAtom->a_w;
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


void ldiv_doBang(x) 
Ldiv *x;
{
		if(x->insize == 0 ) return;
		if(x->insize == 1)
			switch(x->inList[0].a_type){
				case A_LONG:
				ldiv_doInt(x,x->inList[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				ldiv_doFloat(x,x->inList[0].a_w.w_float);
				break;
			
			}
		else
		ldiv_doLists(x,gensym("bang"),x->insize,x->inList);
}

void  ldiv_doInt(x,n)
Ldiv *x;
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
				doDiv(&tempAtom, &x->theList[i],&x->outList[i]);
			}
	x->outsize = x->listsize;
	x->inList[0].a_w.w_long = n;
	x->inList[0].a_type = A_LONG;
	x->insize = 1;
	ldiv_doOutput(x);
	}
}

void  ldiv_doFloat(x,n)
Ldiv *x;
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
				
				doDiv(&tempAtom, &x->theList[i],&x->outList[i]);
			}
		x->outsize = x->listsize;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
		ldiv_doOutput(x);
		}
	
	
}
void ldiv_doOutput(x)
Ldiv *x;
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

void  ldiv_clearList(x)
Ldiv *x;
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

void ldiv_assist(x,b,msg,arg,dst)
Ldiv *x;
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
			strcpy(dst,"Input divided by stored data");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}



void  *ldiv_new(s,ac,argv)
Symbol *s;
int ac;
Atom *argv;
{

	Ldiv  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Ldiv *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->insize = ac;
	for(i=0;i<ac;++i)
	{
		x->inList[i].a_type = A_LONG;
		x->inList[i].a_w.w_long = 0;  // no harm default if bang comes before any input
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

void ldiv_free(x)
Ldiv *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Ldiv *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}