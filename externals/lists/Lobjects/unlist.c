// unlist file for Max object
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

typedef struct unlist
{
	struct object u_ob;
	void *proxy;
	long inletNumber;
	Atom u_theList[MAXSIZE];
	int u_listsize;
	int u_bangcount;
	int u_outtype;
	int askon;
	int onceon;
	void *u_outL;
	void *u_outR;
} Unlist;


void *unlist_new();
void unlist_free();
void unlist_banglist();
void unlist_loadlist();
void unlist_anything();
void unlist_assist();
void unlist_one();
void unlist_onef();
void unlist_once();
void unlist_access();
void unlist_set();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)unlist_new,0L,(short)sizeof(struct unlist),0L,A_GIMME,0);
	addbang((method)unlist_banglist);
	addint((method)unlist_one);
	addfloat((method)unlist_onef);
	addmess((method)unlist_loadlist, "list",A_GIMME,0);
	addmess((method)unlist_anything, "anything",A_GIMME,0);
	addmess((method)unlist_once, "once",A_GIMME,0);
	addmess((method)unlist_loadlist, "symbol",A_GIMME,0);
	addmess((method)unlist_assist, "assist",A_CANT,0);
	addmess((method)unlist_access, "access",A_GIMME,0);
	addmess((method)unlist_access, "get",A_GIMME,0);
	addmess((method)unlist_set, "set",A_GIMME,0);
	finder_addclass("Lists","unlist");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void unlist_loadlist(x,s,argc,argv)
Unlist *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	x->u_listsize = argc;
	
		for(i=0;i<argc;++i)
			for(i=0;i<argc;++i)
			x->u_theList[i] = argv[i];
	
	if(check_inlet(x) == 0){
			if(x->askon )
			{
			switch(x->u_theList[0].a_type)
				{
				case A_LONG:
				outlet_int(x->u_outL,argv[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				outlet_float(x->u_outL,argv[0].a_w.w_float);
				break;
				
				case A_SYM:
				outlet_anything(x->u_outL,x->u_theList[0].a_w.w_sym,0,NIL);
				break;
				
				}
			x->u_bangcount = 1;
			}
			else
			x->u_bangcount = 0;
		}
		x->inletNumber = 0;
}

void unlist_anything(x,s,argc,argv)
Unlist *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc >= MAXSIZE) argc = MAXSIZE-1;
	x->u_listsize = argc + 1;
	x->u_theList[0].a_type = A_SYM;
	x->u_theList[0].a_w.w_sym = s;
	for(i=0;i<argc;++i)
			x->u_theList[i+1] = argv[i];
		
	
	if(check_inlet(x) == 0){
			if(x->askon )
			{
			switch(x->u_theList[0].a_type)
				{
				case A_LONG:
				outlet_int(x->u_outL,argv[0].a_w.w_long);
				break;
				
				case A_FLOAT:
				outlet_float(x->u_outL,argv[0].a_w.w_float);
				break;
				
				case A_SYM:
				outlet_anything(x->u_outL,x->u_theList[0].a_w.w_sym,0,NIL);
				break;
				
				}
			x->u_bangcount = 1;
			}
			else
			x->u_bangcount = 0;
		}
		x->inletNumber = 0;
}

void unlist_one(x,n)
Unlist *x;
long n;
{
	
	x->u_theList[0].a_type = A_LONG;
	x->u_theList[0].a_w.w_long = n;
	x->u_listsize = 1;
	x->u_bangcount = 0;
	if(x->inletNumber == 0)
		unlist_banglist(x);
	x->inletNumber = 0;
	
}

void unlist_onef(x,n)
Unlist *x;
double n;
{
	
	x->u_theList[0].a_type = A_FLOAT;
	x->u_theList[0].a_w.w_float = n;
	x->u_listsize = 1;
	x->u_bangcount = 0;
	if(x->inletNumber == 0)
		unlist_banglist(x);
	x->inletNumber = 0;
	
}

void unlist_access(x,message, argc, argv)
Unlist *x;
Symbol *message;
int argc;
Atom *argv;
{
	
	x->u_bangcount = argv[0].a_w.w_long;
	unlist_banglist(x);
	x->inletNumber = 0;
	
}

void unlist_set(x,message, argc, argv)
Unlist *x;
Symbol *message;
int argc;
Atom *argv;
{
	
	x->u_bangcount = argv[0].a_w.w_long;
}

void unlist_once(x,message, argc, argv)
Unlist *x;
Symbol *message;
int argc;
Atom *argv;
{
	
	if(argc == 0)
		x->onceon = 1;
	else
		x->onceon = argv[0].a_w.w_long;
}

void unlist_banglist(x)
Unlist *x;
{
	if(x->u_bangcount > x->u_listsize-1)
	{
		{
		if(! x->onceon)
		x->u_bangcount = 0;
		outlet_bang(x->u_outR);
		}
		if(x->askon || x->onceon)return;
	}
	switch(x->u_theList[x->u_bangcount].a_type)
		{
		case A_LONG:
		outlet_int(x->u_outL,x->u_theList[x->u_bangcount].a_w.w_long);
		break;
		
		case A_FLOAT:
		outlet_float(x->u_outL,x->u_theList[x->u_bangcount].a_w.w_float);
		break;
		
		case A_SYM:
		outlet_anything(x->u_outL,x->u_theList[x->u_bangcount].a_w.w_sym,0,NIL);
		break;
		
		}
	++x->u_bangcount;
}

void unlist_assist(x,b,msg,arg,dst)
Unlist *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Bang to unlist");
			break;
			
			default:
			strcpy(dst,"Input list without output");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Members of list");
			break;
			
			default:
			if(x->askon)
				strcpy(dst,"bang when finished");
			else
				strcpy(dst,"bang with last item");
			break;
		}
	}
	
	
}


void  *unlist_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Unlist  *x;
	int i;
	
	x = (Unlist *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->u_outR = bangout(x);
	x->u_outL = outlet_new(x,0L);
	x->u_bangcount=0;
	x->u_listsize = 0;
	x->askon= 1;   // askon = 1 means that list will pause at end when bang is sent
	x->onceon = 0; // onceon = 1 means list will not cycle
		if(ac)
	{
		for(i=0;i<ac;++i)
		{
		x->u_theList[i].a_type = theArgs[i].a_type;
		x->u_theList[i].a_w.w_long = theArgs[i].a_w.w_long;
		}
		for(;i<MAXSIZE;++i)
		{
		x->u_theList[i].a_type = A_LONG;
		x->u_theList[i].a_w.w_long = 0;
		}
	x->askon = 0;
	x->u_listsize = ac;
	}
	x->inletNumber = 0;
	
	return(x);
}

void unlist_free(x)
Unlist *x;
{
	freeobject(x->proxy);
}
int check_inlet(x)
Unlist *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}