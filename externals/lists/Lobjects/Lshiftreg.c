// lshiftreg file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lshiftreg
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theLeftover;
	Atom outList[MAXSIZE];
	int listsize;
	
	int outsize;
	void *outR;
	void *out;
} Lshiftreg;


void *lshiftreg_new();
void lshiftreg_free();
void  lshiftreg_List();
void  lshiftreg_anything();
void  lshiftreg_Int();
void  lshiftreg_Float();
void 	shiftTheList();
void lshiftreg_dumpList();
void  lshiftreg_clearList();
void lshiftreg_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lshiftreg_new,(method)lshiftreg_free,(short)sizeof(struct lshiftreg),0L,A_GIMME,0);
	addint((method)lshiftreg_Int);
	addfloat((method)lshiftreg_Float);
	addbang((method)lshiftreg_dumpList);
	addmess((method)lshiftreg_List, "list",A_GIMME,0);
	addmess((method)lshiftreg_anything, "anything",A_GIMME,0);
	addmess((method)lshiftreg_assist, "assist",A_CANT,0);
	addmess((method)lshiftreg_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lshiftreg_List(x,s,argc,argv)
Lshiftreg *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;

	if(check_inlet(x))  // list in right inlet replaces stored list
	{
		for(i = 0; i < argc; ++i)
			x->outList[i] = argv[i];
		x->outsize = argc;
	}
	else		// list in left inlet is shifted into first of out list
	{
		for(i = 0; i < argc; ++i){
			shiftTheList(x);
			x->outList[0] = argv[i];
		}
		
		lshiftreg_dumpList(x);
	}
	
}

void lshiftreg_anything(x,s,argc,argv)
Lshiftreg *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc >= MAXSIZE) argc = MAXSIZE-1;

	if(check_inlet(x))  // command in right inlet
	{
		x->outList[0].a_type = A_SYM;
		x->outList[0].a_w.w_sym = s;
		for(i=0;i< argc;++i)
			{
				x->outList[i + 1] = argv[i];
			}
		
		x->outsize = argc + 1;
		
	}
	else		// list in left inlet
	{
		shiftTheList(x);
		x->outList[0].a_type = A_SYM;
		x->outList[0].a_w.w_sym = s;
		for(i=0;i < argc ; ++i)
			{
				shiftTheList(x);
				x->outList[0] = argv[i];
				
			}
		lshiftreg_dumpList(x);
	}
	
}

void  lshiftreg_clearList(x)
Lshiftreg *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	
	
}


void  lshiftreg_Int(x,n)
Lshiftreg *x;
long n;
{
	
	if(check_inlet(x))		// constant is in right inlet
	{
		
		lshiftreg_clearList(x);
		x->outsize = n;
		
		x->inletNumber = 0;
	}
	else    // constant in left
	{
		shiftTheList(x);
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
	
		lshiftreg_dumpList(x);
	}
	
	
}


void  lshiftreg_Float(x,n)
Lshiftreg *x;
float n;
{
	
	if(check_inlet(x))		// constant is in right inlet
	{
		
		
	}
	else    // constant in left
	{
		shiftTheList(x);
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = n;
	
		lshiftreg_dumpList(x);
	}
	
}

void shiftTheList(x)
Lshiftreg *x;
{
	int i;
	x->theLeftover = x->outList[x->outsize-1];
	for( i = x->outsize-1; i>0  ; --i) {
		x->outList[i] = x->outList[i-1];
	}
	switch(x->theLeftover.a_type){
			case A_LONG:
			outlet_int(x->outR,x->theLeftover.a_w.w_long);
			break;
			
			case A_FLOAT:
			outlet_float(x->outR, x->theLeftover.a_w.w_float);
			break;
			
			case A_SYM:
			outlet_anything(x->outR,x->theLeftover.a_w.w_sym , 0L, nil);
			break;
		
		}
}


void lshiftreg_dumpList(x)
Lshiftreg *x;
{
	
	
	if(x->listsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void lshiftreg_assist(x,b,msg,arg,dst)
Lshiftreg *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"insert at beginning of list");
			break;
			
			default:
			strcpy(dst,"list to store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"list shifted to the right");
			break;
			
			default:
			strcpy(dst,"items shifted off list");
			break;
		}
	}
}

void  *lshiftreg_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lshiftreg  *x;
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x = (Lshiftreg *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	
	switch(argc)
	{
		case 0:
		x->outsize = 12;
		for(i=0;i < 12;++i){
			x->outList[i].a_type = A_LONG;
			x->outList[i].a_w.w_long = 0;
		}
		break;
		
		case 1:
		x->outsize = argv[0].a_w.w_long;
		for(i=0;i < x->outsize;++i){
			x->outList[i].a_type = A_LONG;
			x->outList[i].a_w.w_long = 0;
		}
		break;
		
		default:
		x->outsize = argc;
		for(i=0;i < argc;++i)
			x->outList[i] = argv[i];
		break;
	
		
	}
	return(x);
}

void lshiftreg_free(x)
Lshiftreg *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lshiftreg *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}