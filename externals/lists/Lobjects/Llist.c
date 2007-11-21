// llist file for Max object
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


typedef struct llist
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outList[MAXSIZE];
	
	int listsize;
	int outsize;
	void *out;
	void *outR;
	void *message;
} Llist;


void *llist_new();
void llist_free();
void  llist_storeList();
void  llist_anything();
void llist_setlist();
void  llist_setConstant();
void  llist_setMember();
void  llist_floatConstant();
void llist_dumpList();
void  llist_clearList();
void llist_assist();
void  llist_getMember();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)llist_new,(method)llist_free,(short)sizeof(struct llist),0L,A_GIMME,0);
	addint((method)llist_setConstant);
	addfloat((method)llist_floatConstant);
	addbang((method)llist_dumpList);
	addmess((method)llist_getMember, "get",A_DEFLONG,0);
	addmess((method)llist_storeList, "list",A_GIMME,0);
	addmess((method)llist_anything, "anything",A_GIMME,0);
	addmess((method)llist_setMember, "set",A_GIMME,0);
	addmess((method)llist_assist, "assist",A_CANT,0);
	addmess((method)llist_clearList, "clear",0);
	finder_addclass("Lists","Llist");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
	
}

void llist_storeList(x,s,argc,argv)
Llist *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i=0;i<argc;++i)		// list in either inlet
		{
		x->outList[i] = argv[i];
		}
		
	x->outsize = argc;
	
	if(check_inlet(x) == 0){	// list in left inlet
		
		
		llist_dumpList(x);
		}
	
}

void llist_anything(x,s,argc,argv)
Llist *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc >= MAXSIZE) argc = MAXSIZE - 1;
	 x->outList[0].a_type = A_SYM;
	 x->outList[0].a_w.w_sym = s;
	
	for(i=0;i<argc;++i)		// list in either inlet
		x->outList[i+ 1] = argv[i];
	x->outsize = argc + 1;
	if(check_inlet(x) == 0)	// list in left inlet
		llist_dumpList(x);
}

void llist_setMember(x,s,argc,argv)
Llist *x;
Symbol *s;
int argc;
Atom *argv;
{
	long i;
	#ifdef TEST
	  x->message = s;
	  #endif
	if(argc > 1 && argv[0].a_type == A_LONG)
	{
		i =argv[0].a_w.w_long;
		if(i >= 0 && i < x->outsize)
			x->outList[i] = argv[1];
	}
	else
	if(argc == 1 && argv[0].a_type == A_LONG)
	{
		i =argv[0].a_w.w_long;
		if(i >= 0 && i < x->outsize){
			x->outList[i].a_type = A_LONG;
			x->outList[i].a_w.w_long = 0;
		}
	}
}

void  llist_clearList(x)
Llist *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i){
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
	}
	x->outsize = 0;
}


void  llist_setConstant(x,n)
Llist *x;
long n;
{
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = n;
	x->outsize = 1;
	if(check_inlet(x) == 0)		// constant is in left inlet
		llist_dumpList(x);
}

void  llist_getMember(x,n)
Llist *x;
long n;
{
	
	if(n < x->outsize && n > -1){
		switch(x->outList[n].a_type)
		{ 	case   A_LONG:
			outlet_int(x->out,x->outList[n].a_w.w_long);
			break;
			
			case A_FLOAT:
			outlet_float(x->out,x->outList[n].a_w.w_float);
			break;
			
			case A_SYM:
			outlet_anything(x->out,x->outList[n].a_w.w_sym , 0, NIL);
			break;
		}
	}	
}

void  llist_floatConstant(x,n)
Llist *x;
double n;
{
	x->outList[0].a_type = A_FLOAT;
	x->outList[0].a_w.w_float = n;
	x->outsize = 1;
	if(check_inlet(x) == 0)		// constant is in left inlet
		llist_dumpList(x);
}

void llist_dumpList(x)
Llist *x;
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


void llist_assist(x,b,msg,arg,dst)
Llist *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Output list and store");
			break;
			
			default:
			strcpy(dst,"Store List");
			break;
		}
	}
	else
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"List");
	
}


void  *llist_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Llist  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Llist *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	
	switch(ac)
	{
	case 0:
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
		x->outsize = 0;
		break;
	break;
	
	case 1:					
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i] = theArgs[0];
		}
		x->outsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->outList[i] = theArgs[i];
		}
		for(;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->outsize = ac;
	}
	return(x);
}

void llist_free(x)
Llist *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Llist *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}