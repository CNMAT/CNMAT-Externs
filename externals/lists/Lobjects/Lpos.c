// lpos file for Max object
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


typedef struct lpos
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom matchList[MAXSIZE];
	Atom theList[MAXSIZE];
	Atom failedList[MAXSIZE];
	Atom foundList[MAXSIZE];
	Atom wildcard;
	long constant;
	int incount;
	int listsize;
	int failedsize;
	int matchsize;
	int foundsize;
	int noWrap;
	void *out;
	void *outL;
} Lpos;


void *lpos_new();
void lpos_free();
void  lpos_doLists();
void  lpos_anything();
void  lpos_doConstant();
void  lpos_setWildcard();
void  lpos_doFloatConstant();
void lpos_dumpList();
int compareStuff();
void  lpos_clearList();
void lpos_wrapon();
void lpos_wrapoff();
void lpos_assist();
int check_inlet();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lpos_new,(method)lpos_free,(short)sizeof(struct lpos),0L,A_GIMME,0);
	addint((method)lpos_doConstant);
	addfloat((method)lpos_doFloatConstant);
	addbang((method)lpos_dumpList);
	addmess((method)lpos_doLists, "list",A_GIMME,0);
	addmess((method)lpos_setWildcard, "wildcard",A_GIMME,0);
	addmess((method)lpos_anything, "anything",A_GIMME,0);
	addmess((method)lpos_assist, "assist",A_CANT,0);
	addmess((method)lpos_clearList, "clear",0);
	addmess((method)lpos_wrapon, "wrapon", 0);
	addmess((method)lpos_wrapoff, "wrapoff", 0);
	//finder_addclass("Lists","Lpos");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lpos_doLists(x,s,argc,argv)
Lpos *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j, end;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
	{
		for(i=0;i<argc;++i)
			{
				x->theList[i] = argv[i];
			}
		x->listsize = argc;
	}
	else		// do matching
	{
		for(i=0; i < argc; ++i)
			x->matchList[i] = argv[i];
		x->matchsize = argc;
		if(x->noWrap)
			end = x->listsize - argc;
		else
			end = x->listsize;
		j=0;
		x->foundsize = 0;
			for(i=0; i < end; ++i)
			{
				if(compareStuff(x, i))
					x->foundList[j++].a_w.w_long = i;
			}
		x->foundsize = j;
		if(x->foundsize)   //success
			outlet_list(x->outL,0L,x->foundsize,x->foundList);
		else
			outlet_list(x->out,0L,argc,x->matchList);
		
	}
}

void lpos_anything(x,s,argc,argv)
Lpos *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	int end;
	if(check_inlet(x))
	{
		x->theList[0].a_type = A_SYM;
		x->theList[0].a_w.w_sym = s;
		for(i = 0; i < argc; ++i)
			x->theList[i+1] = argv[i];
		x->listsize = argc +1;
	}
	else
	{
		x->matchList[0].a_type = A_SYM;
		x->matchList[0].a_w.w_sym = s;
		for(i = 0; i < argc; ++i)
			x->matchList[i+1] = argv[i];
		x->matchsize = argc +1;
		if(x->noWrap)
			end = x->listsize - x->matchsize;
		else
			end = x->listsize;
		
		j=0;
		x->foundsize = 0;
			for(i=0; i < end; ++i)
			{
				if(compareStuff(x, i))
					x->foundList[j++].a_w.w_long = i;
			}
		x->foundsize = j;
		if(x->foundsize)   //success
			outlet_list(x->outL,0L,x->foundsize,x->foundList);
		else
			outlet_anything(x->out,s,argc,argv);
	}
}

void lpos_setWildcard(x,s,argc,argv)
Lpos *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(argc)
	x->wildcard = argv[0];
	else
	x->wildcard.a_type = A_NOTHING;
}

void  lpos_clearList(x)
Lpos *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->listsize = 1;
	
	}
	
void lpos_wrapon(x)
Lpos *x;
{
	x->noWrap = 0;
	}

void lpos_wrapoff(x)
Lpos *x;
{
	x->noWrap = 1;
	}

void  lpos_doConstant(x,n)
Lpos *x;
long n;
{
	int i,j;
	if(check_inlet(x))		// constant is in right inlet, 
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		x->listsize = 1;
	}
	else			// search for constant
	{
		x->matchList[0].a_type = A_LONG;
		x->matchList[0].a_w.w_long = n;
		x->matchsize = 1;
		j=0;
		x->foundsize = 0;
		for(i=0; i < x->listsize; ++i)
		{
			if(compareStuff(x, i))
				x->foundList[j++].a_w.w_long = i;
		}
		x->foundsize = j;
		if(x->foundsize)   //success
			outlet_list(x->outL,0L,x->foundsize,x->foundList);
		else
			outlet_list(x->out,0L,1L,x->matchList);
	}
}


void  lpos_doFloatConstant(x,n)
Lpos *x;
double n;
{
	int i,j;
	if(check_inlet(x))		// float constant is in right inlet, 
	{
		x->theList[0].a_type = A_FLOAT;
		x->theList[0].a_w.w_float = n;
		x->listsize = 1;
	}
	else			// search for float constant
	{
		x->matchList[0].a_type = A_FLOAT;
		x->matchList[0].a_w.w_float = n;
		x->matchsize = 1;
		j=0;
		x->foundsize = 0;
		for(i=0; i < x->listsize; ++i)
		{
			if(compareStuff(x, i))
				x->foundList[j++].a_w.w_long = i;
		}
		x->foundsize = j;
		if(x->foundsize)   //success
			outlet_list(x->outL,0L,x->foundsize,x->foundList);
		else
			outlet_list(x->out,0L,1L,x->matchList);
	}
}

void lpos_dumpList(x)
Lpos *x;
{
	int i,j,end;
	end = x->listsize - x->matchsize ;
		j=0;
		x->foundsize = 0;
			for(i=0; i <= end; ++i)
			{
				if(compareStuff(x, i))
					x->foundList[j++].a_w.w_long = i;
			}
		x->foundsize = j;
		if(x->foundsize)   //success
			outlet_list(x->outL,0L,x->foundsize,x->foundList);
		else
			outlet_list(x->out,0L,x->matchsize,x->matchList);
}

int compareStuff(x,n)
Lpos * x;
int n;
{
	int i;
	if(x->matchsize < 1) return( 0);
	
	for(i = 0; i < x->matchsize; ++i){
		if(x->matchList[i].a_type == x->wildcard.a_type){
			
			switch(x->matchList[i].a_type){
				case A_LONG :
				if(x->matchList[i].a_w.w_long == x->wildcard.a_w.w_long)
					continue;
				break;
				case A_FLOAT :
				if(x->matchList[i].a_w.w_float == x->wildcard.a_w.w_float)
					continue;
				break;
				case A_SYM :
				if(x->matchList[i].a_w.w_sym == x->wildcard.a_w.w_sym)
					continue;
				break;
			}
		}
		
		if(x->matchList[i].a_type != x->theList[(i + n) % x->listsize].a_type)
			return(0);
		
		
		switch(x->matchList[i].a_type){
			case A_LONG :
			if(x->matchList[i].a_w.w_long != x->theList[(i + n) % x->listsize].a_w.w_long)
				return(0);
			break;
			case A_FLOAT :
			if(x->matchList[i].a_w.w_float != x->theList[(i + n) % x->listsize].a_w.w_float)
				return(0);
			break;
			case A_SYM :
			if(x->matchList[i].a_w.w_sym != x->theList[(i + n) % x->listsize].a_w.w_sym)
				return(0);
			break;
		}
	}
	return(1);
}

void lpos_assist(x,b,msg,arg,dst)
Lpos *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Target to look for.");
			break;
			
			default:
			strcpy(dst,"List to search.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Positions of target list");//forgot something
			break;
			
			default:
			strcpy(dst,"target if not found");
			break;
		}
	}
}

void  *lpos_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lpos  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lpos *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->outL= intout(x);
	x->inletNumber = 0;
	x->wildcard.a_type = A_NOTHING;
	x->noWrap = 0;
	switch(ac)
	{
	case 0:
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = 0;
		x->listsize = 0;
		break;
	
	case 1:					
		x->theList[0].a_type = theArgs[0].a_type;
		x->theList[0].a_w.w_long = theArgs[0].a_w.w_long;
		x->listsize = 1;
	break;
		
	default:
		for(i=0;i<ac;++i)
		{
			x->theList[i].a_type = theArgs[i].a_type;
			x->theList[i].a_w.w_long = theArgs[i].a_w.w_long;
		}
		
		x->listsize = ac;
	}
	for(i=0;i<MAXSIZE;++i)
		{
		x->foundList[i].a_type = A_LONG;
		}
	x->foundsize = 0;
	x->matchsize = 0;
	x->failedsize = 0;
		return(x);
}

void lpos_free(x)
Lpos *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lpos *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}