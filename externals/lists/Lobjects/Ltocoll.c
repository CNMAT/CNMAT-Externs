// ltocoll file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct ltocoll
{
	struct object l_ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	int putItHere;
	int startwith;
	int endwith;
	void *outL;
	void *outR;
} Ltocoll;


void *ltocoll_new();
void ltocoll_free();
void  ltocoll_List();
void  ltocoll_anything();
void  ltocoll_howMany();
void  ltocoll_setnext();
void  ltocoll_reset();
void ltocoll_dumpList();
void ltocoll_assist();
void ltocoll_constant();
void ltocoll_Fconstant();
void ltocoll_symbol();
void ltocoll_sym();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ltocoll_new,(method)ltocoll_free,(short)sizeof(struct ltocoll),0L,A_GIMME,0);
	addint((method)ltocoll_constant);
	addfloat((method)ltocoll_Fconstant);
	addinx((method)ltocoll_setnext, 1);
	addbang((method)ltocoll_dumpList);
	addmess((method)ltocoll_symbol,"symbol", A_GIMME,0);
	addmess((method)ltocoll_anything,"anything", A_GIMME,0);
	addmess((method)ltocoll_dumpList,"dump",0);
	addmess((method)ltocoll_List, "list",A_GIMME,0);
	addmess((method)ltocoll_reset, "reset",A_GIMME,0);
	addmess((method)ltocoll_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Ltocoll");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void ltocoll_reset(x)
Ltocoll *x;
{
	x->putItHere = x->startwith;
}

void ltocoll_setnext(x,n)
Ltocoll *x;
long n;
{	
	if (n > -1)
		x->putItHere = n;
}

void ltocoll_sym(x,s)
Ltocoll *x;
Symbol *s;
{
	x->outList[1].a_type = A_SYM;
	x->outList[1].a_w.w_sym = s;
	x->outsize = 2;
	ltocoll_dumpList(x);
}


void ltocoll_constant(x,n)
Ltocoll *x;
long n;
{
	x->outList[1].a_type = A_LONG;
	x->outList[1].a_w.w_long = n;
	x->outsize = 2;
	ltocoll_dumpList(x);
}

void  ltocoll_Fconstant(x,n)
Ltocoll *x;
double n;
{
	x->outList[1].a_type = A_FLOAT;
	x->outList[1].a_w.w_float = n;
	x->outsize = 2;
	ltocoll_dumpList(x);
}

void ltocoll_List(x,s,argc,argv)
Ltocoll *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	for(i=0;i < argc;++i)
		{
		x->outList[i+1].a_type = argv[i].a_type;
		x->outList[i+1].a_w = argv[i].a_w;
		}
		x->outsize = argc+1;
		ltocoll_dumpList(x);
}

void ltocoll_anything(x,s,argc,argv)
Ltocoll *x;
Symbol *s;
int argc;
Atom *argv;
{	
	int i;
	if(argc >= MAXSIZE - 2) argc = MAXSIZE - 2;	
	x->outList[1].a_type = A_SYM;
	x->outList[1].a_w.w_sym = s;
	for(i=0;i < argc;++i)
		x->outList[i+2] = argv[i];
	x->outsize = argc+2;
	ltocoll_dumpList(x);
}

void ltocoll_symbol(x,s,argc,argv)
Ltocoll *x;
Symbol *s;
int argc;
Atom *argv;
{	
	if(argc > 1)
		ltocoll_List(x,s,argc,argv);
	else
		ltocoll_sym(x,argv[0].a_w.w_sym);
}


void ltocoll_dumpList(x)
Ltocoll *x;
{
	outlet_int(x->outR, (long)x->putItHere);
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = (long)x->putItHere;
	++x->putItHere;
	if(x->putItHere > x->endwith)
		x->putItHere = x->startwith;	
	outlet_list(x->outL,0L,x->outsize,x->outList);
}


void ltocoll_assist(x,b,msg,arg,dst)
Ltocoll *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to format for Coll");
			break;
			
			default:
			strcpy(dst,"Address to place data");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Message for Coll.");//forgot something
			break;
			
			default:
			strcpy(dst,"Address");
			break;
		}
	}
}

void  *ltocoll_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{
	Ltocoll  *x;
	int i;
	
	
	x = (Ltocoll *)newobject(class);
	intin(x,1);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
	}	
	x->outsize  =  MAXSIZE;
	x->putItHere = 0;
	x->startwith = 0;
	x->endwith = 250;
	
	switch(argc)
	{
		case 0:
			break;
			
		case 1:
			
			if(argv[0].a_type== A_LONG)
				x->endwith = argv[0].a_w.w_long;
			break;
			
		default:
			if(argv[0].a_type== A_LONG)
				x->startwith = argv[0].a_w.w_long;
			if(argv[1].a_type== A_LONG)
				x->endwith = argv[1].a_w.w_long;
				
	}
	x->putItHere = x->startwith;
	return(x);
}

void ltocoll_free(x)
Ltocoll *x;
{
}