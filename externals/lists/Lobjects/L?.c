// lif file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1997, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

/*
this is a wrapper for three expr objects.
it emulats the C conditional operation    expr1 ? expr2 : expr3

build two versions L? and Lcond

*/

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lif
{
	struct object ob;
	long inletNumber;
	Atom outList[MAXSIZE];
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
	Atom expr1_inputs[9];
	Atom expr2_inputs[9];
	Atom expr3_inputs[9];
	short numProxyInlets;
	short noOpFlag;
	void *outL;
	void *proxy[9];
} Lif;


void *lif_new();
void lif_free();
void  lif_doLists();
void  lif_doInt();
void  lif_doFloat();
void  lif_bang();
void  lif_clearList();
void lif_assist();
int check_inlet();

void *class;

int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lif_new,(method)lif_free,(short)sizeof(struct lif),0L,A_GIMME,0);
	addint((method)lif_doInt);
	addfloat((method)lif_doFloat);
	addbang((method)lif_bang);
	//addmess((method)lif_doLists, "list",A_GIMME,0);
	addmess((method)lif_assist, "assist",A_CANT,0);
	alias("Lcond");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Regents of the University of California.");
		s->s_thing = (struct object *)l;}
	return 0;
}

void lif_doLists(x,s,argc,argv)
Lif *x;
Symbol *s;
int argc;
Atom *argv;
{}





void  lif_bang(x)
Lif *x;
{
	t_atom result;
	expr_eval(x->expr1,9,x->expr1_inputs,&result);
	if(result.a_w.w_long)
		expr_eval(x->expr2,9,x->expr2_inputs,&result);
	else
		if(x->noOpFlag)
			return;
		else
			expr_eval(x->expr3,9,x->expr3_inputs,&result);
	
	if(result.a_type == A_LONG)
		outlet_int(x->outL, result.a_w.w_long);
	else
		if(result.a_type == A_FLOAT)
		outlet_float(x->outL, result.a_w.w_float);
}


void  lif_doInt(x,n)
Lif *x;
long n;
{
	int which;	
	
	//which= proxy_getinlet((t_object *)x);  // OSX only
	which = check_inlet(x);  // classic only
	
	if(x->expr1_inputs[which].a_type == A_LONG)
		x->expr1_inputs[which].a_w.w_long = n;
	else if(x->expr1_inputs[which].a_type == A_FLOAT)
		x->expr1_inputs[which].a_w.w_float = (double)n;
		
	if(x->expr2_inputs[which].a_type == A_LONG)
		x->expr2_inputs[which].a_w.w_long = n;
	else if(x->expr2_inputs[which].a_type == A_FLOAT)
		x->expr2_inputs[which].a_w.w_float = (double)n;
		
	if(x->expr3_inputs[which].a_type == A_LONG)
		x->expr3_inputs[which].a_w.w_long = n;
	else if(x->expr3_inputs[which].a_type == A_FLOAT)
		x->expr3_inputs[which].a_w.w_float = (double)n;
	if(which == 0)
	{
		lif_bang(x);
	}
	
	
}

void  lif_doFloat(x,n)
Lif *x;
double n;
{	
	int which;	
	//which= proxy_getinlet((t_object *)x);  // OSX only
	which = check_inlet(x); // Clasic only
	
	if(x->expr1_inputs[which].a_type == A_LONG)
		x->expr1_inputs[which].a_w.w_long = (long)n;
	else if(x->expr1_inputs[which].a_type == A_FLOAT)
		x->expr1_inputs[which].a_w.w_float = n;
		
	if(x->expr2_inputs[which].a_type == A_LONG)
		x->expr2_inputs[which].a_w.w_long = (long)n;
	else if(x->expr2_inputs[which].a_type == A_FLOAT)
		x->expr2_inputs[which].a_w.w_float = n;
		
	if(x->expr3_inputs[which].a_type == A_LONG)
		x->expr3_inputs[which].a_w.w_long = (long)n;
	else if(x->expr3_inputs[which].a_type == A_FLOAT)
		x->expr3_inputs[which].a_w.w_float = n;
	if(which == 0)
	{
		lif_bang(x);
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
			case 0: strcpy(dst,"$i1 or $f1"); break;
			case 1: strcpy(dst,"$i2 or $f2"); break;
			case 2: strcpy(dst,"$i3 or $f3"); break;
			case 3: strcpy(dst,"$i4 or $f4"); break;
			case 4: strcpy(dst,"$i5 or $f5"); break;
			case 5: strcpy(dst,"$i6 or $f6"); break;
			case 6: strcpy(dst,"$i7 or $f7"); break;
			case 7: strcpy(dst,"$i8 or $f8"); break;
			case 8: strcpy(dst,"$i9 or $f9"); break;
			
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Result of Conditional");
	}
}


void  *lif_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lif  *x;
	long i,k;
	Atom argv[MAXSIZE];
	short qFound, cFound;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lif *)newobject(class);
	x->outL= outlet_new(x,0L);
	
	x->inletNumber = 0;
	x->expr1 =0;
	x->expr2 =0;
	x->expr3 =0;
	x->noOpFlag = 0;
	for(i = 0; i < 9; ++i){
		x->expr1_inputs[i].a_w.w_long = 0;
		x->expr1_inputs[i].a_type = 0;
		x->expr2_inputs[i].a_w.w_long = 0;
		x->expr2_inputs[i].a_type = 0;
		x->expr3_inputs[i].a_w.w_long = 0;
		x->expr3_inputs[i].a_type = 0;
	}
	k = 0;
	qFound=0;
	cFound = 0;
	for(i = 0;i < ac;i++)
	{
		
		if(theArgs[i].a_w.w_sym == gensym("?")){
		x->expr1 = expr_new(k,argv,x->expr1_inputs);
		k = 0;
		qFound = 1;
		
		continue;
		}
		if(theArgs[i].a_w.w_sym == gensym(":")){
		x->expr2 = expr_new(k,argv,x->expr2_inputs);
		k = 0;
		cFound =1;
		continue;
		}
		argv[k++] = theArgs[i];
		//postatom(&argv[k-1]);
	}
	x->expr3 = expr_new(k,argv,x->expr3_inputs);
	if(!qFound) post("L? ? needed to seperate expressions");
	if(!cFound){
		x->noOpFlag = 1;
		x->expr2 = x->expr3;
		x->expr3 = 0;
		for(i = 0; i < 9; ++i)
			x->expr2_inputs[i] = x->expr3_inputs[i];
	}
	for(i = 8; i > 0 ; --i)
		if((x->expr1_inputs[i].a_type)||
			(x->expr2_inputs[i].a_type)||
				(x->expr3_inputs[i].a_type))
					break;
	x->numProxyInlets = i;
	for(i = x->numProxyInlets; i > 0 ; --i)
		x->proxy[i-1] = proxy_new(x,i,&x->inletNumber);
	
	return(x);
}

void lif_free(x)
Lif *x;
{
	int i;
	if(x->expr1)
	freeobject((t_object*)x->expr1);
	if(x->expr2)
	freeobject((t_object*)x->expr2);
	if(x->expr3)
	freeobject((t_object*)x->expr3);
	for(i=0;i<x->numProxyInlets;++i)
	freeobject(x->proxy[i]);
	

}

int check_inlet(x)
Lif *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}

