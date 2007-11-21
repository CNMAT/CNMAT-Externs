// lifo file for Max object
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

typedef struct lifo
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	long nextspot;
	int outsize;
	void *out;
} Lifo;


void *lifo_new();
void lifo_free();
void  lifo_doLists();
void  lifo_doLong();
void  lifo_doFloat();
void  lifo_pop();
void  lifo_doAnything();
void lifo_dumpList();
void  lifo_clearList();
void lifo_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lifo_new,(method)lifo_free,(short)sizeof(struct lifo),0L,A_GIMME,0);
	addint((method)lifo_doLong);
	addfloat((method)lifo_doFloat);
	addbang((method)lifo_dumpList);
	addmess((method)lifo_doLists, "list",A_GIMME,0);
	addmess((method)lifo_pop, "pop",A_DEFLONG,0);
	addmess((method)lifo_doAnything, "anything",A_GIMME,0);
	addmess((method)lifo_assist, "assist",A_CANT,0);
	addmess((method)lifo_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lifo_doLists(x,s,argc,argv)
Lifo *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(x->nextspot >=0)
		for(i= argc - 1; i >= 0; --i)
		{
			x->theList[x->nextspot--] = argv[i];
			if(x->nextspot < 0) break;
		}
	if(check_inlet(x) == 0)
		lifo_dumpList(x);
}

void lifo_doAnything(x,s,argc,argv)
Lifo *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(x->nextspot >=0)
		for(i= argc - 1; i >= 0; --i)
		{
			x->theList[x->nextspot--] = argv[i];
			if(x->nextspot < 0) break;
		}
	
	if(x->nextspot >=0){
		x->theList[x->nextspot].a_type = A_SYM;
		x->theList[x->nextspot].a_w.w_sym = s;
		-- x->nextspot;
	}
	
	if(check_inlet(x) == 0)
		lifo_dumpList(x);
}

void  lifo_clearList(x)
Lifo *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->nextspot = MAXSIZE - 1;
}


void  lifo_dumpList(x)
Lifo *x;
{
	int i;
	
	++ x->nextspot;// when list is full nextspot will be -1
	x->outsize = MAXSIZE - x->nextspot; 
	
	for(i = 0; i < x->outsize; ++ i)
		x->outList[i] = x->theList[i + x->nextspot];
	
	
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym, 0, 0);
			}
		else
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
			else
				outlet_list(x->out,0L,x->outsize,x->outList);
	x->nextspot = MAXSIZE - 1;  // dumping clears list
}

void  lifo_doLong(x,n)
Lifo *x;
long n;
{
	x->theList[x->nextspot].a_type = A_LONG;
	x->theList[x->nextspot].a_w.w_long = n;
	-- x->nextspot;
	if(check_inlet(x) == 0)
		lifo_dumpList(x);
}

void  lifo_pop(x,n)
Lifo *x;
long n;
{
	int i;
	for(i= 0;i<n;++i){
		++ x->nextspot;
		if(x->nextspot >= MAXSIZE){
			x->nextspot = MAXSIZE-1;
			 break;
		 }
		x->outList[i] = x->theList[x->nextspot];
	}
	x->outsize = n;
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym, 0, 0);
			}
		else
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->out,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
			else
				outlet_list(x->out,0L,x->outsize,x->outList);
}

void  lifo_doFloat(x,n)
Lifo *x;
double n;
{
	x->theList[x->nextspot].a_type = A_FLOAT;
	x->theList[x->nextspot].a_w.w_float = n;
	-- x->nextspot;
	if(check_inlet(x) == 0)
		lifo_dumpList(x);
}

void lifo_assist(x,b,msg,arg,dst)
Lifo *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Prepend to list and trigger output.");
			break;
			
			case 1:
			strcpy(dst,"Prepend to list.");
			break;
			
			case 2:
			strcpy(dst,"");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"list, last in first out.");
	}
}


void  *lifo_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lifo  *x;
	x = (Lifo *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->nextspot = MAXSIZE -1;
	
	return(x);
}

void lifo_free(x)
Lifo *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lifo *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}