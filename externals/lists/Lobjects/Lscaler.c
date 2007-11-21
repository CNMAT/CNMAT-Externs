// lscaler file for Max object
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


typedef struct lscaler
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outMax[MAXSIZE];
	Atom inMax[MAXSIZE];
	Atom outList[MAXSIZE];
	float scalers[MAXSIZE];
	int listsize;
	int outsize;
	Atom inList[MAXSIZE]; 	// copy of input
	int insize;
	void *out;
} Lscaler;


void *lscaler_new();
void lscaler_free();
void  lscaler_doLists();
void 	lscaler_doBang();
void lscaler_setlist();
void  lscaler_doConstant();
void  lscaler_floatConstant();
void lscaler_dumpList();
void  lscaler_set();
void  lscaler_clear();
void lscaler_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lscaler_new,(method)lscaler_free,(short)sizeof(struct lscaler),0L,A_GIMME,0);
	addint((method)lscaler_doConstant);
	addfloat((method)lscaler_floatConstant);
	addbang((method)lscaler_doBang);
	addmess((method)lscaler_doLists, "list",A_GIMME,0);
	addmess((method)lscaler_set, "set",A_GIMME,0);
	addmess((method)lscaler_assist, "assist",A_CANT,0);
	addmess((method)lscaler_clear, "clear",0);
	finder_addclass("Lists","Lscaler");
	

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	
}

void lscaler_doLists(x,s,argc,argv)
Lscaler *x;
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
			if(argv[i].a_w.w_long == 0) continue;
			switch(argv[i].a_type)
			{ 
				case A_LONG:
					x->scalers[i] = x->outMax[i].a_w.w_float / (float) argv[i].a_w.w_long;
					x->inMax[i].a_w.w_float = (float) argv[i].a_w.w_long;
					break;
					
				case A_FLOAT:
					x->scalers[i] = x->outMax[i].a_w.w_float /  argv[i].a_w.w_float;
					x->inMax[i].a_w.w_float = argv[i].a_w.w_float;
					break;
					
				default:
					x->scalers[i] = 1;
			}
		}
		for(i=argc;i<MAXSIZE;++i){
			x->inMax[i].a_w.w_float = 127;
			x->scalers[i] = x->outMax[i].a_w.w_float / 127;
		}
		x->listsize = argc;
	}
	else		// list in left inlet
	{
		for(i=0;i<argc;++i){
			x->inList[i].a_type = argv[i].a_type;
			x->inList[i].a_w = argv[i].a_w;
		}
		x->insize = argc;
		lscaler_doBang(x);
	}
}

void 	lscaler_doBang(x)
Lscaler *x;
{
	int i;
	double tempf = 0;
	for(i=0;i<x->insize;++i)
	{
		if(x->inList[i].a_type == A_FLOAT)
			tempf = x->inList[i].a_w.w_float;
		if(x->inList[i].a_type == A_LONG)
			tempf = (float)x->inList[i].a_w.w_long;	
		switch(x->outList[i].a_type) // match output type
		{
			case A_LONG:
			x->outList[i].a_w.w_long = (long)(tempf * x->scalers[i]);
			break;
			
			case A_FLOAT:
			x->outList[i].a_w.w_float = tempf * x->scalers[i];
			break;
			
			default:
			x->outList[i].a_w.w_long = 0;
		}
	}
	x->outsize = x->insize;
	lscaler_dumpList(x);
}

void lscaler_set(x,s,argc,argv)
Lscaler *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;

	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i = 0; i < argc; ++i){
		x->outList[i].a_type = argv[i].a_type;
		if(argv[i].a_type == A_LONG)
			x->outMax[i].a_w.w_float = (float)argv[i].a_w.w_long;
		if(argv[i].a_type == A_FLOAT)
			x->outMax[i].a_w.w_float = argv[i].a_w.w_float;
		
		x->scalers[i] = x->outMax[i].a_w.w_float /  x->inMax[i].a_w.w_float;
	}
	for(; i < MAXSIZE;++i){
		x->outMax[i].a_w.w_float = 127;
		x->scalers[i] = 1;
	x->listsize = argc;
	}
}


void  lscaler_clear(x)
Lscaler *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
			x->outList[i].a_type = A_LONG;
			x->scalers[i] = 1;
			x->outMax[i].a_w.w_float = 127;
			x->inMax[i].a_w.w_float = 127;
		}
	x->listsize = 1;	
}


void  lscaler_doConstant(x,n)
Lscaler *x;
long n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet
	{
		if(n != 0)
			for(i=0;i<MAXSIZE;++i)
			{
				x->inMax[i].a_w.w_float = (float) n;
				x->scalers[i] = x->outMax[i].a_w.w_float / (float) n;
			}
	}
		else    // constant in left
		{
			for(i=0;i<x->listsize;++i)
			{
				if(x->outList[i].a_type == A_LONG)
					x->outList[i].a_w.w_long = (long) ((float) n * x->scalers[i]);
				else
					x->outList[i].a_w.w_float = ((float) n * x->scalers[i]);
			}
			x->outsize = x->listsize;
			lscaler_dumpList(x);
		}
}

void  lscaler_floatConstant(x,n)
Lscaler *x;
float n;
{
	int i;
	if(check_inlet(x))		// constant is in right inlet
	{
	if(n != 0.0)
	for(i=0;i<MAXSIZE;++i)
		{
		x->inMax[i].a_w.w_float =  n;
		x->scalers[i] = x->outMax[i].a_w.w_float /  n;
		}
	}
	else
		{
		for(i=0;i<x->listsize;++i)
			{
				if(x->outList[i].a_type == A_FLOAT)
					x->outList[i].a_w.w_float =  n * x->scalers[i];
				else
					x->outList[i].a_w.w_long = (long)( n * x->scalers[i]);
			
			}
		x->outsize = x->listsize;
		lscaler_dumpList(x);
		}
}
void lscaler_dumpList(x)
Lscaler *x;
{
	if(x->outsize == 0) return;
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


void lscaler_assist(x,b,msg,arg,dst)
Lscaler *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to Scale");
			break;
			
			default:
			strcpy(dst,"Input Maxima");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List of scaled values");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}


void  *lscaler_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lscaler  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lscaler *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	
	switch(ac)
	{
		case 0:  // no args set default
			for(i=0;i<MAXSIZE;++i)
			{
				x->outList[i].a_type = A_LONG;
				x->outMax[i].a_type = A_FLOAT;
				x->outMax[i].a_w.w_float = 127;
			}
			x->listsize = 1;
			break;
			
		case 1:	// 1 arg all members are treated the same				
			
			for(i=0;i<MAXSIZE;++i)
			{
				x->outList[i].a_type =theArgs[0].a_type;
				x->outMax[i].a_type = A_FLOAT;
				if(theArgs[0].a_type == A_LONG)
					x->outMax[i].a_w.w_float = (float)theArgs[0].a_w.w_long;
				else
					x->outMax[i].a_w.w_float = theArgs[0].a_w.w_float;
			}
			x->listsize = 1;
			break;
			
		default:  // more than 1 arg
			for(i=0;i<ac;++i)
			{
				x->outList[i].a_type = theArgs[i].a_type;
				x->outMax[i].a_type = A_FLOAT;
				if(theArgs[i].a_type == A_LONG)
					x->outMax[i].a_w.w_float = (float)theArgs[i].a_w.w_long;
				else
					x->outMax[i].a_w.w_float = theArgs[i].a_w.w_float;
			}
			for(;i<MAXSIZE;++i)  // default for long lists
			{
				x->outList[i].a_type = A_LONG;
				x->outMax[i].a_type = A_FLOAT;
				x->outMax[i].a_w.w_float = 127;
			}
			x->listsize = ac;
	}
	for(i = 0 ;i < MAXSIZE; ++i){
		
		x->inMax[i].a_type = A_FLOAT;
		x->inMax[i].a_w.w_float = 127;
	x->scalers[i] = x->outMax[i].a_w.w_float/x->inMax[i].a_w.w_float ;
	}
	x->insize = 0;
	return(x);
}

void lscaler_free(x)
Lscaler *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lscaler *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}