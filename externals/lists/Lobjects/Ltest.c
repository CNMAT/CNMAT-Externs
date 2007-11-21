// ltest file for Max object
// by Peter Elsea,  University of Caltestornia, Santa Cruz
// copyright © 2002, Regents of the University of Caltestornia
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

#define EQUALS 0
#define GREATERTHAN 1
#define LESSTHAN 2
#define GREATERTHANOREQUALS 3
#define LESSTHANOREQUALS 4
#define NOTEQUAL 5

typedef struct ltest
{
	struct object ob;
	void *proxy1;
	//void *proxy2;
	long inletNumber;
	short theTest;
	Atom outList[MAXSIZE];
	Atom theList[MAXSIZE];
	long outsize;
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;	
	void *out;
	//void *outR;
	//void *message;
} Ltest;


void 	*ltest_new();
void 	ltest_free();
void  	ltest_doList();
void 	generic_doBang();
void  	ltest_doLong();
void  	ltest_doFloat();
int    	ltest_doTest(Ltest * x,Atom  first, Atom  second);
int 	test_longs(Ltest * x, long A, long B);
int 	test_floats(Ltest * x, double A, double B);
void 	ltest_dumpList();
void 	ltest_setTest();
void  	ltest_clearList();

void ltest_assist();

int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ltest_new,(method)ltest_free,(short)sizeof(struct ltest),0L,A_GIMME,0);
	addint((method)ltest_doLong);
	addfloat((method)ltest_doFloat);
	addbang((method)generic_doBang);
	addmess((method)ltest_doList, "list",A_GIMME,0);
	addmess((method)ltest_setTest, "anything",A_GIMME,0);
	addmess((method)ltest_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Ltest");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of Caltestornia.");
	s->s_thing = (struct object *)l;
	}
	return 0;
}

void ltest_doList(x,s,argc,argv)
Ltest *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) 
		argc = MAXSIZE;
	if(check_inlet(x)){
	
		for(i=0; i < argc; ++ i)
			x->theList[i] = argv[i];
		for(; i < MAXSIZE; ++i)
			 SETLONG(&x->theList[i],0);
	}
	else
	{
		for(i = 0; i < argc; ++i){
			x->inList[i] = argv[i];
			x->outList[i].a_w.w_long = ltest_doTest(x,argv[i],x->theList[i]);
			}
		x->outsize = argc;
		x->insize = argc;
		ltest_dumpList(x);
	}
	
}

void generic_doBang(x) 
Ltest *x;
{
	int i;
	for(i = 0; i < x->insize; ++i)
		x->outList[i].a_w.w_long = ltest_doTest(x,x->inList[i],x->theList[i]);
	
		x->outsize = x->insize;
		ltest_dumpList(x);
}


int ltest_doTest(Ltest * x,Atom  first, Atom  second)
{
		if((first.a_type == A_LONG) && (second.a_type == A_LONG))
			return(test_longs(x,first.a_w.w_long, second.a_w.w_long));
			
		if((first.a_type == A_LONG) && (second.a_type == A_FLOAT))
			return(test_longs(x,first.a_w.w_long, (long)second.a_w.w_float));
			
		if((first.a_type == A_FLOAT) && (second.a_type == A_LONG))
			return(test_longs(x,(long)first.a_w.w_float, second.a_w.w_long));
			
		if((first.a_type == A_FLOAT) && (second.a_type == A_FLOAT))
			return(test_floats(x,first.a_w.w_float, second.a_w.w_float));
		
		if((first.a_type == A_SYM) || (second.a_type == A_SYM))
			return(1); // symbols are wild cards an d return true
		return(-1);// error
}

int test_longs(Ltest *x,long A,long B)
{
		switch(x->theTest){
		
		case EQUALS:
		return(A==B);
		
		case GREATERTHAN:
		return(A>B);
		
		case LESSTHAN:
		return(A<B);
		
		case GREATERTHANOREQUALS:
		return(A>=B);
		
		case LESSTHANOREQUALS:
		return(A<=B);
		
		case NOTEQUAL:
		return(A!=B);
		
		}
		return 0;
}



int test_floats(Ltest *x,double A, double B)
{
		switch(x->theTest){
		
		case EQUALS:
		return(A==B);
		
		case GREATERTHAN:
		return(A>B);
		
		case LESSTHAN:
		return(A<B);
		
		case GREATERTHANOREQUALS:
		return(A>=B);
		
		case LESSTHANOREQUALS:
		return(A<=B);
		
		case NOTEQUAL:
		return(A!=B);
		
		}
	return 0;
}

void  ltest_doLong(x,n)
Ltest *x;
long n;
{
	int i;
	Atom testme;
	testme.a_type = A_LONG;
	testme.a_w.w_long = n;
	if(check_inlet(x))
		for(i = 0; i < MAXSIZE; ++i)
			SETLONG(&x->theList[i],n);
	else{
	outlet_int(x->out, ltest_doTest(x,testme,x->theList[0])) ;
	x->inList[0].a_w.w_long = n;
	x->inList[0].a_type = A_LONG;
	x->insize = 1;
	}
	
}



void  ltest_doFloat(x,n)
Ltest *x;
double n;
{
	int i;
	Atom testme;
	testme.a_type = A_FLOAT;
	testme.a_w.w_float = n;
	if(check_inlet(x))
		for(i = 0; i < MAXSIZE; ++i)
			SETFLOAT(&x->theList[i],n);
	else{
	outlet_int(x->out, ltest_doTest(x,testme,x->theList[0])) ;
	x->inList[0].a_w.w_float = n;
	x->inList[0].a_type = A_FLOAT;
	x->insize = 1;
	}
}

void ltest_dumpList(x)
Ltest *x;
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

void ltest_setTest(x,s,argc,argv)
Ltest *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	int i;
		if(s == gensym("==")) x->theTest = EQUALS;
		if(s == gensym(">")) x->theTest = GREATERTHAN;
		if(s == gensym("<")) x->theTest = LESSTHAN;
		if(s == gensym(">=")) x->theTest = GREATERTHANOREQUALS;
		if(s == gensym("<=")) x->theTest = LESSTHANOREQUALS;
		if(s == gensym("!=")) x->theTest = NOTEQUAL;
		if(argc){
			if(argc == 1)
				for(i=0; i < MAXSIZE; ++i)
				 x->theList[i] = argv[i];
				 else{
				for(i = 0; i < argc; ++i)
					x->theList[i] = argv[i];
				for(; i < MAXSIZE; ++i)
					SETLONG(&x->theList[i],0);
			}
		}
}


void ltest_assist(x,b,msg,arg,dst)
Ltest *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to test");
			break;
			
			case 1:
			strcpy(dst,"Data to test against");
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


void  *ltest_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Ltest  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Ltest *)newobject(class);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->out = outlet_new(x,0L);
	x->theTest = EQUALS;
	for(i=0; i < MAXSIZE; ++i){
		 x->outList[i].a_type = A_LONG;
		 x->theList[i].a_type = A_LONG;
		 x->theList[i].a_w.w_long = 0;
		 x->outList[i].a_w.w_long = 0;
	 }
	 x->outsize = 3;
	if(ac && theArgs[0].a_type == A_SYM){
		if(theArgs[0].a_w.w_sym == gensym("==")) x->theTest = EQUALS;
		if(theArgs[0].a_w.w_sym == gensym(">")) x->theTest = GREATERTHAN;
		if(theArgs[0].a_w.w_sym == gensym("<")) x->theTest = LESSTHAN;
		if(theArgs[0].a_w.w_sym == gensym(">=")) x->theTest = GREATERTHANOREQUALS;
		if(theArgs[0].a_w.w_sym == gensym("<=")) x->theTest = LESSTHANOREQUALS;
		if(theArgs[0].a_w.w_sym == gensym("!=")) x->theTest = NOTEQUAL;
	
		for(i = 1; i < ac; ++i){
			x->theList[i-1] = theArgs[i];
		}
		
	}
	else if(ac){
		if(ac == 1)
		for(i=0; i < MAXSIZE; ++i)
			x->theList[i] = theArgs[0];
		else
		for(i=0; i < ac; ++i)
			x->theList[i] = theArgs[i];
	}
	
	x->insize = 0;
	
	return(x);
}

void ltest_free(x)
Ltest *x;
{
	freeobject(x->proxy1);
}

int check_inlet(x)
Ltest *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}