// lif file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2002, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lif
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	long inletNumber;
	
	Atom outList[MAXSIZE];
	Atom trueList[MAXSIZE];
	Atom falseList[MAXSIZE];
	long outsize;
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;
	void *out;
	//void *outR;
	//void *message;
} Lif;


void 	*lif_new();
void 	lif_free();
void  	lif_doList();
void 	lif_doBang();
void  	lif_doLong();
void  	lif_doFloat();
void 	lif_dumpList();
void  	lif_clearList();
void 	lif_assist();
char 	*strcpy();
int 	check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lif_new,(method)lif_free,(short)sizeof(struct lif),0L,A_GIMME,0);
	addint((method)lif_doLong);
	addfloat((method)lif_doFloat);
	addbang((method)lif_doBang);
	addmess((method)lif_doList, "list",A_GIMME,0);
	addmess((method)lif_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lif");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
}

void lif_doList(x,s,argc,argv)
Lif *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	switch(check_inlet(x)){
		case 0:
		for(i = 0; i < argc; ++i){
		x->inList[i] = argv[i];
		x->outList[i] = argv[i].a_w.w_long ? x->trueList[i] : x->falseList[i];
		}
		x->outsize = argc;
		x->insize = argc;
		lif_dumpList(x);
		break;
		
		case 1:
		for(i = 0; i < argc; ++i){
		x->trueList[i] = argv[i];
		}
		break;
		
		case 2:
		for(i = 0; i < argc; ++i){
		x->falseList[i] = argv[i];
		}
		break;
	}
}


void lif_doBang(x)
Lif *x;
{
	int i;
	
	for(i = 0; i < x->insize; ++i)
		x->outList[i] = x->inList[i].a_w.w_long ? x->trueList[i] : x->falseList[i];
	x->outsize = x->insize;
	lif_dumpList(x);
}




void  lif_doLong(x,n)
Lif *x;
long n;
{
	int i;
	
	switch(check_inlet(x)){
		case 0:
		x->outList[0]= n ? x->trueList[0] : x->falseList[0];
		x->outsize = 1;
		x->inList[0].a_w.w_long = n;
		x->inList[0].a_type = A_LONG;
		x->insize = 1;
		lif_dumpList(x);
		break;
	
		case 1:
		for(i = 0; i < MAXSIZE; ++ i){
			SETLONG(&x->trueList[i],n);
		}
		break;
		
		case 2:
		for(i = 0; i < MAXSIZE; ++ i){
			SETLONG(&x->falseList[i],n);
			
		}
		break;
	
	}
}



void  lif_doFloat(x,n)
Lif *x;
double n;
{
	int i;
	
	switch(check_inlet(x)){
		case 0:
		x->outList[0]= n ? x->trueList[0] : x->falseList[0];
		x->outsize = 1;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->insize = 1;
		lif_dumpList(x);
		break;
	
		case 1:
		for(i = 0; i < MAXSIZE; ++ i){
			SETFLOAT(&x->trueList[i],n);
		}
		break;
		
		case 2:
		for(i = 0; i < MAXSIZE; ++ i){
			SETFLOAT(&x->falseList[i],n);
			
		}
		break;
	
	}
	
	
}

void lif_dumpList(x)
Lif *x;
{
	
	
	if(x->outsize){
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym , 0, NIL);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	}
	
}


void lif_assist(x,b,msg,arg,dst)
Lif *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List of tests");
			break;
			
			case 1:
			strcpy(dst,"List of results for true");
			break;
			
			default:
			strcpy(dst,"List of results for false");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"result of test");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
}


void  *lif_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lif  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lif *)newobject(class);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	
	x->out= outlet_new(x,0L);
	switch(ac){
		case 0:
		for(i = 0; i < MAXSIZE; ++ i){
			SETLONG(&x->trueList[i],1);
			SETLONG(&x->falseList[i],0);
		}
		break;
		
		case 1:
		for(i = 0; i < MAXSIZE; ++ i){
			x->trueList[i] = theArgs[0];
			SETLONG(&x->falseList[i],0);
		}
		break;
		
		case 2:
		for(i = 0; i < MAXSIZE; ++ i){
			x->trueList[i] = theArgs[0];
			x->falseList[i] = theArgs[1];
		}
		break;
	}
	x->insize = 0;
	return(x);
}

void lif_free(x)
Lif *x;
{
	freeobject(x->proxy1);
	freeobject(x->proxy2);
}

int check_inlet(x)
Lif *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}