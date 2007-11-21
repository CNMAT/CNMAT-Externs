// lbuf file for Max object
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
typedef struct lbuf
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	void *out;
} Lbuf;

void *lbuf_new();
void lbuf_free();
void  lbuf_doLists();
void  lbuf_doAll();
void lbuf_setlist();
void  lbuf_setConstant();
void  lbuf_floatConstant();
void lbuf_dumpList();
void  lbuf_clearList();
void lbuf_assist();
void lbuf_restore();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbuf_new,(method)lbuf_free,(short)sizeof(struct lbuf),0L,A_GIMME,0);
	addint((method)lbuf_setConstant);
	addfloat((method)lbuf_floatConstant);
	addbang((method)lbuf_dumpList);
	addmess((method)lbuf_doLists, "list",A_GIMME,0);
	addmess((method)lbuf_doAll, "anything",A_GIMME,0);
	addmess((method)lbuf_assist, "assist",A_CANT,0);
	addmess((method)lbuf_clearList, "clear",0);
	addmess((method)lbuf_restore, "restore",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbuf_doLists(x,s,argc,argv)
Lbuf *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x) == 0)  // list in left inlet
	{
		lbuf_dumpList(x);
	}
	
	for(i=0;i<argc;++i)
		{
			x->outList[i] = argv[i];
		}
		
	x->outsize = argc;

	
}

void lbuf_doAll(x,s,argc,argv)
Lbuf *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x) == 0)  // list in left inlet
	{
		lbuf_dumpList(x);
	}
	x->outList[0].a_type = A_SYM;
	x->outList[0].a_w.w_sym = s;
	for(i=0;i<argc;++i)
		{
			x->outList[i+1] = argv[i];
		
		}
		
	x->outsize = argc+1;

	
}


void  lbuf_clearList(x)
Lbuf *x;
{	
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = 0;
	
	x->outsize = 1;
	
}

void lbuf_restore(x)
Lbuf *x;
{
	int i;
	
	for(i=0;i<x->listsize;++i)
		{
		x->outList[i].a_type = x->theList[i].a_type;
		x->outList[i].a_w = x->theList[i].a_w;
		}
	
	x->outsize = x->listsize;
}


void  lbuf_setConstant(x,n)
Lbuf *x;
long n;
{	
	if(check_inlet(x))		// constant is in right inlet
	{
	
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
		x->listsize = 1;
	}
	else    // constant in left
		{
			if(n < 0) n = 0;
			if(n >= x->outsize) n = x->outsize -1;
			switch(x->outList[n].a_type)
			{
			 	case A_LONG:
				outlet_int(x->out,x->outList[n].a_w.w_long);
				break;
				
				case A_FLOAT:
				outlet_float(x->out,x->outList[n].a_w.w_float);
				break;
				
				case A_SYM:
				outlet_list(x->out,0L,1L,&x->outList[n]);
				break;
			}
		}
	
	
}

void  lbuf_floatConstant(x,n)
Lbuf *x;
double n;
{
	lbuf_setConstant(x,(long)n);
}

void lbuf_dumpList(x)
Lbuf *x;
{
	if(x->outsize == 0) return;
	if(x->outsize == 1)
	{
		switch(x->outList[0].a_type) {
			case A_LONG:
			outlet_int(x->out,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT :
			outlet_float(x->out,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM :
			outlet_anything(x->out,x->outList[0].a_w.w_sym, 0, 0);
			break;
		}
	}else
		if(x->outList[0].a_type == A_SYM)
			outlet_anything(x->out,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
		else
			outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void lbuf_assist(x,b,msg,arg,dst)
Lbuf *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to store  or member to access");
			break;
			
			default:
			strcpy(dst,"List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"current member or previous list");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}


void  *lbuf_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lbuf  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lbuf *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	
	switch(ac)
	{
	case 0:
		x->listsize = 0;
		x->outsize = 0;
		break;
	
	case 1:					
		x->outList[0] = theArgs[0];
		x->theList[0] = theArgs[0];
			
		x->listsize = 1;
		x->outsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->outList[i] = theArgs[i]	;
		x->theList[i] = theArgs[i]	;
		}
		x->listsize = ac;
		x->outsize = ac;
		break;
	
	}
	
	return(x);
}

void lbuf_free(x)
Lbuf *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lbuf *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}