// logic file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and logicice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define MAXINLETS 32

enum  opcode {op_and,op_or,op_xor,op_nand,op_nor,op_not};

typedef struct logic
{
	struct object ob;
	void *proxy[MAXINLETS];
	long inletNumber;
	long outState;
	int inputState[MAXINLETS];
	int numberOfInlets;
	enum opcode theOpcode;
	void *outL;
} Logic;


void *logic_new();
void logic_free();
void  logic_doInt();
void  logic_dofloat();
void  logic_doBang();
void  logic_doList();
void logic_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)logic_new,(method)logic_free,(short)sizeof(struct logic),0L,A_GIMME,0);
	addint((method)logic_doInt);
	addfloat((method)logic_dofloat);
	addbang((method)logic_doBang);
	addmess((method)logic_doList, "list",A_GIMME,0);
	addmess((method)logic_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void  logic_doBang(x)
Logic *x;
{

	
	outlet_int(x->outL,x->outState);
	x->inletNumber = 0;
	
	
}

void  logic_doInt(x,n)
Logic *x;
long n;
{
	
	int i;
	int outState = 0;
	
	x->inputState[check_inlet(x)] = n && 1;
	switch(x->theOpcode)
	{
		case op_and : case op_nand :
		for( i = 0; i < x->numberOfInlets; ++i)
		{
			outState = x->inputState[i];
			if(outState == 0)
				break;
		}
		break;
		
		case op_or : case op_nor :
		for( i = 0; i < x->numberOfInlets; ++i)
		{
			outState = x->inputState[i];
			if(outState == 1)
				break;
		}
		break;
		
		case op_xor : 
		outState = 0;
		for( i = 0; i < x->numberOfInlets; ++i)
		{
			outState += x->inputState[i];
			
		}
		if(outState > 1)
				outState = 0;
		break;
		
		case op_not : 
		outState = 0;
		for( i = 0; i < x->numberOfInlets; ++i)
		{
			outState += x->inputState[i];
			if(outState) break;
		}
		outState = ! outState;
		break;
		
	}
	if(x->theOpcode == op_nand || x->theOpcode == op_nor)
		outState = (! outState) & 1;
	if(x->theOpcode == op_xor)
		if(outState > 1) outState = 0;
	x->outState = outState;
	outlet_int(x->outL,x->outState);
	
	x->inletNumber = 0;
	
}

void  logic_dofloat(x,n)
Logic *x;
double n;
{
	if(n == 0.0)
		logic_doInt(x,0);
	else
		logic_doInt(x,1);
}

void  logic_doList(x,s,argc,argv)
Logic *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	int i;
	int outState;
	
	outState = 0;
		if(argc >= MAXSIZE) argc = MAXSIZE;
	switch(x->theOpcode)
	{
		case op_and : case op_nand :
		for( i = 0; i < argc; ++i)
		{
			outState = argv[i].a_w.w_long && 1;
			if(outState == 0)
				break;
		}
		break;
		
		case op_or : case op_nor :
		for( i = 0; i < argc; ++i)
		{
			outState = argv[i].a_w.w_long && 1;
			if(outState == 1)
				break;
		}
		break;
		
		case op_xor : 
		for( i = 0; i < argc; ++i)
		{
			outState += argv[i].a_w.w_long && 1;
			if(outState > 1)
				break;
		}
		break;
		
		case op_not : 
		for( i = 0; i < argc; ++i)
		{
			outState += argv[i].a_w.w_long && 1;
			if(outState > 1)
				break;
		}
		if(outState > 1) outState = 0;
		break;
		
	}
	if(x->theOpcode == op_nand || x->theOpcode == op_nor)
		outState = (! outState) && 1;
	if(x->theOpcode == op_xor)
		if(outState > 1) outState = 0;
	x->outState = outState;
	outlet_int(x->outL,x->outState);
	
	x->inletNumber = 0;
	
}



void logic_assist(x,b,msg,arg,dst)
Logic *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"logic 1 or 0");
			break;
			
			default:
			strcpy(dst,"Logic 1 or 0");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (x->theOpcode)
		{
			
			//op_and,op_or,op_xor,op_nand,op_nor
			case op_and:
			strcpy(dst,"Logic AND of the inputs");
			break;
			
			case op_or:
			strcpy(dst,"Logic OR of the inputs");
			break;
			
			case op_nand:
			strcpy(dst,"Logic NAND of the inputs");
			break;
			
			case op_xor:
			strcpy(dst,"Logic XOR of the inputs");
			break;
			
			case op_nor:
			strcpy(dst,"Logic NOR of the inputs");
			break;
			
			case op_not:
			strcpy(dst,"Logic NOT of the input");
			break;
		}
	}
	
	
}


void  *logic_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Logic  *x;
	int i;
	
	
	x = (Logic *)newobject(class);
	if(ac > 1 && theArgs[1].a_type == A_LONG)
		x->numberOfInlets = theArgs[1].a_w.w_long;
	else
		x->numberOfInlets = 2;
	if(x->numberOfInlets > MAXINLETS)
		x->numberOfInlets = MAXINLETS;
	if(x->numberOfInlets < 2)
		x->numberOfInlets = 2;
	
	
	
	x->theOpcode = op_and;
	x->outState = 0;
	if(theArgs[0].a_type == A_SYM)
	{
		if(theArgs[0].a_w.w_sym == gensym("OR"))
		{
			x->theOpcode = op_or;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("or"))
		{
			x->theOpcode = op_or;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("Or"))
		{
			x->theOpcode = op_or;
			goto finish;;
		}
		if(theArgs[0].a_w.w_sym == gensym("NAND"))
		{
			x->theOpcode = op_nand;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("nand"))
		{
			x->theOpcode = op_nand;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("Nand"))
		{
			x->theOpcode = op_nand;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("NOR"))
		{
			x->theOpcode = op_nor;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("nor"))
		{
			x->theOpcode = op_nor;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("Nor"))
		{
			x->theOpcode = op_nor;
			x->outState = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("XOR"))
		{
			x->theOpcode = op_xor;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("xor"))
		{
			x->theOpcode = op_xor;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("Xor"))
		{
			x->theOpcode = op_xor;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("not"))
		{
			x->theOpcode = op_not;
			x->outState = 1;
			x->numberOfInlets = 1;
			goto finish;
		}
		if(theArgs[0].a_w.w_sym == gensym("Not"))
		{
			x->theOpcode = op_not;
			x->outState = 1;
			x->numberOfInlets = 1;
			goto finish;
		}
	
	}
	finish :	for(i = x->numberOfInlets-1; i > 0; --i)
	{
		x->proxy[i-1] = proxy_new(x,(long)i,&x->inletNumber);
		x->inputState[i]= 0;
	}
	x->outL= outlet_new(x,0L);
	return(x);
}

void logic_free(x)
Logic *x;
{
	int i;
	
	for(i=0; i < x->numberOfInlets-1; ++i)
		freeobject(x->proxy[i]);
	

}

int check_inlet(x)
Logic *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}