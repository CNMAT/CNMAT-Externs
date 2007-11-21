// lbang file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lbang
{
	struct object ob;
	Atom theList[MAXSIZE];
	int listsize;
	Symbol * theSym;
	void *outR;
	void *outL;
	struct lbang * c_next;
} Lbang;


void *lbang_new();
void lbang_free();
void lbang_tester();
void lbang_dumpList();
void lbang_bangNow(Lbang *x,t_symbol *s,  short argc, t_atom * argv);
void lbang_assist();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbang_new,(method)lbang_free,(short)sizeof(struct lbang),0L,A_GIMME,0);
	addbang((method)lbang_dumpList);
	addint((method)lbang_dumpList);
	addmess((method)lbang_dumpList, "loadbang",0);
	addmess((method)lbang_dumpList, "dblclick",0);
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void lbang_dumpList(x)
Lbang *x;
{
	
	defer_low(x,(method)lbang_bangNow,0,0, 0);
	
}

void lbang_bangNow(Lbang *x,t_symbol *s,  short argc, t_atom * argv)
{
	
	if(x->listsize){
		if(x->listsize == 1){
			switch(x->theList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->theList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->theList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->outL,x->theList[0].a_w.w_sym,0,NIL);
			}
		}
		else
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	}
	else
		outlet_bang(x->outL);  // no args
}


void lbang_assist(x,b,msg,arg,dst)
Lbang *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"bang to trigger ouput");
			break;
			
			default:
			strcpy(dst,"");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Args sent on load");
			break;
			
			default:
			strcpy(dst,"Deferred bang.");
			break;
		}
	}
	
	
}


void  *lbang_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lbang  *x;
	
	int i;
	
	
	x = (Lbang *)newobject(class);
	x->outL= outlet_new(x,0L);
	x->theSym = gensym("Lbang");
	
	for(i = 0; i < argc; ++i)
		{
			x->theList[i] = argv[i];
		} 
		x->listsize = argc;
	
	
	return(x);
}

void lbang_free(x)
Lbang *x;
{
	
	

}

