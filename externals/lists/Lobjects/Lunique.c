// lunique file for Max object
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


typedef struct lunique
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outList[MAXSIZE];
	long allow;
	
	int outsize;
	void *out;
	void *outR;
	void *message;
} Lunique;


void *lunique_new();
void lunique_free();
void  lunique_doList();
void lunique_anything();
void  lunique_doLong();
void  lunique_doFloat();
void lunique_dumpList();
void  lunique_clearList();
void lunique_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lunique_new,(method)lunique_free,(short)sizeof(struct lunique),0L,A_GIMME,0);
	addint((method)lunique_doLong);
	addfloat((method)lunique_doFloat);
	addbang((method)lunique_dumpList);
	addmess((method)lunique_doList, "list",A_GIMME,0);
	addmess((method)lunique_anything, "anything",A_GIMME,0);
	addmess((method)lunique_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lunique_doList(x,s,argc,argv)
Lunique *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k;
	short found;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	found = 0;
	k = 0;
	for(i=0;i<argc;++i)		// list in either inlet
		{
		found = 0;
		for(j = 0; j < i; ++j){
			if(argv[j].a_w.w_long == argv[i].a_w.w_long)
				if(argv[j].a_type == argv[i].a_type){
					++found;
					
					}}
		if(found < x->allow)
			x->outList[k++] = argv[i];
		}
	x->outsize = k;
	lunique_dumpList(x);
	
}

void lunique_anything(x,s,argc,argv)
Lunique *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	if(argc >= MAXSIZE) argc = MAXSIZE-1;
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
		tempList[i +1] = argv[i];
	lunique_doList(x,s,argc + 1,tempList);
}


void  lunique_doLong(x,n)
Lunique *x;
long n;
{
	if(check_inlet(x))
		x->allow = n;
	else 
		outlet_int(x->out,n);
}
void  lunique_doFloat(x,n)
Lunique *x;
double n;
{
	outlet_float(x->out,n);
}

void lunique_dumpList(x)
Lunique *x;
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


void lunique_assist(x,b,msg,arg,dst)
Lunique *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List input");
			break;
			
			default:
			strcpy(dst,"Store List");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List with duplicates removed");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
}

void  *lunique_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lunique  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lunique *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	
	x->out= outlet_new(x,0L);
	if(ac)
		x->allow = theArgs[0].a_w.w_long;
	else
		x->allow = 1;
	return(x);
}

void lunique_free(x)
Lunique *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lunique *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}