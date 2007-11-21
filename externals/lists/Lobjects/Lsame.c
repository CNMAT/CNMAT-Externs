// lsame file for Max object
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

typedef struct lsame
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Symbol * theSymbol;
	int listsize;
	int outsize;
	void *out;
} Lsame;


void *lsame_new();
void lsame_free();
void  lsame_doLists();
void  lsame_doSymbols();
void lsame_setlist();
void  lsame_setConstant();
void  lsame_floatConstant();
void lsame_dumpList();
void  lsame_clearList();
void lsame_assist();
int check_inlet();
void *class;

int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsame_new,(method)lsame_free,(short)sizeof(struct lsame),0L,A_GIMME,0);
	addint((method)lsame_setConstant);
	addfloat((method)lsame_floatConstant);
	addbang((method)lsame_dumpList);
	addmess((method)lsame_doLists, "list",A_GIMME,0);
	addmess((method)lsame_doSymbols, "anything",A_GIMME,0);
	addmess((method)lsame_assist, "assist",A_CANT,0);
	addmess((method)lsame_clearList, "clear",0);
	finder_addclass("Lists","Lsame");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
	
}

void lsame_doLists(x,s,argc,argv)
Lsame *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  
	if(check_inlet(x)){
	for(i=0;i<argc;++i)		// list in right inlet
		{
		x->theList[i] = argv[i];
		}
		
	x->listsize = argc;
	}else {
		if(x->listsize != argc)
			outlet_int(x->out, 0L);
		else {
			for(i = 0; i < argc; ++i){
				if(argv[i].a_type != x->theList[i].a_type ||
					argv[i].a_w.w_long != x->theList[i].a_w.w_long){
				outlet_int(x->out,0L);
				return;
				}
			outlet_int(x->out,1L);
			}
		}
	}

}

void lsame_doSymbols(x,s,argc,argv)
Lsame *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  
	if(check_inlet(x)){
	x->theSymbol = s;
	for(i=0;i<argc;++i)		// list in right inlet
		{
		x->theList[i] = argv[i];
		}
		
	x->listsize = argc;
	}
	else 
	{
		if(s != x->theSymbol){
			outlet_int(x->out, 0L);
			return;
		}
		
		if(x->listsize != argc){
			outlet_int(x->out, 0L);
			return;
		}
			//post("checking args %ld", argc);
			for(i = 0; i < argc; ++i){
				if(argv[i].a_type != x->theList[i].a_type ||
					argv[i].a_w.w_long != x->theList[i].a_w.w_long){
				outlet_int(x->out,0L);
				return;
			}
		
		}
		outlet_int(x->out,1L);
	}
	
}

void  lsame_clearList(x)
Lsame *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	
}


void  lsame_setConstant(x,n)
Lsame *x;
long n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = n;
		}
	x->listsize = 1;
	}
	else
		{
		if(x->listsize != 1){
			outlet_int(x->out,0L);
			return;
			}
		if(x->theList[0].a_type != A_LONG){
			outlet_int(x->out,0L);
			return;
			}
		if(n != x->theList[0].a_w.w_long){
			outlet_int(x->out,0L);
			return;
			}
		
		outlet_int(x->out,1L);
			
		}
	
	
}

void  lsame_floatConstant(x,n)
Lsame *x;
double n;
{
	int i;
	
	if(check_inlet(x))		// constant is in right inlet
	{
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		x->theList[i].a_w.w_float = n;
		}
	x->listsize = 1;
	}
	else
		{
		if(x->listsize != 1){
			outlet_int(x->out,0L);
			return;
			}
		if(x->theList[0].a_type != A_FLOAT){
			outlet_int(x->out,0L);
			return;
			}
		if(n != x->theList[0].a_w.w_float){
			outlet_int(x->out,0L);
			return;
			}
		
		outlet_int(x->out,1L);
			
		}
}

void lsame_dumpList(x)
Lsame *x;
{
	
	
	
}


void lsame_assist(x,b,msg,arg,dst)
Lsame *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Message to compare");
			break;
			
			default:
			strcpy(dst,"Store any thing to compare");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"1 if same, 0 if not");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
}


void  *lsame_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lsame  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsame *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	
	x->out= outlet_new(x,0L);
	
	switch(ac)
	{
		case 0:
		theArgs[0].a_type = A_NOTHING;
		theArgs[0].a_w.w_long = 0;
		x->listsize = 1;
		break;
		
		case 1:		
		if(theArgs[0].a_type == A_SYM){
			x->theSymbol = theArgs[0].a_w.w_sym;
			x->listsize = 0;
		}
		else	{
			x->theList[0] = theArgs[0];
			x->listsize = 1;
		}
		break;
		
	default:
		if(theArgs[0].a_type == A_SYM){
			x->theSymbol = theArgs[0].a_w.w_sym;
			for(i=1;i<ac;++i)
				x->theList[i-1] = theArgs[i];
			x->listsize = ac-1;
		}
		else
		{
			for(i=0;i < ac;++i)
				x->theList[i] = theArgs[i];
			x->listsize = ac;
		}
	}
	
	
	return(x);
}

void lsame_free(x)
Lsame *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lsame *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}