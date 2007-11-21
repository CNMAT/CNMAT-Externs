// not file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

// note: compile in two versions: as lnot and !
// this requires changing the info.plist as well as project settings
// product name 

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct not
{
	struct object ob;
	//void *proxy;
	//long inletNumber;
	long outsize;
	Atom theList[MAXSIZE];
	int numOuts;
	void *outL;
} Not;


void *not_new();
void not_free();
void  not_doInt();
void  not_dofloat();
void  not_doList();
void not_assist();
char *strcpy();
int check_inlet();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;

	setup((t_messlist **)&class,(method)not_new,(method)not_free,(short)sizeof(struct not),0L,A_GIMME,0);
	addint((method)not_doInt);
	addfloat((method)not_dofloat);
	addmess((method)not_doList, "list",A_GIMME,0);
	addmess((method)not_assist, "assist",A_CANT,0);
	
	finder_addclass("Math","Not");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}




void  not_doInt(x,n)
Not *x;
long n;
{
	outlet_int(x->outL,! n);
}

void  not_dofloat(x,n)
Not *x;
double n;
{
	outlet_int(x->outL,!(long) n);
}

void  not_doList(x,s,argc,argv)
Not *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	int i;
	if(argc >= MAXSIZE) argc = MAXSIZE;
	
		for( i = 0; i < argc; ++i)
		{
			switch(argv[i].a_type)
			{
			
				case  A_LONG:
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = ! argv[i].a_w.w_long ;
				break;
			
				case  A_FLOAT:
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = ! (long)argv[i].a_w.w_float ;
				break;
				
				default:
				x->theList[i].a_type = A_SYM;
				if((argv[i].a_w.w_sym == gensym("false")) ||
				(argv[i].a_w.w_sym == gensym("FALSE")) ||
				(argv[i].a_w.w_sym == gensym("NULL")) ||
				(argv[i].a_w.w_sym == gensym("nil")) )
					x->theList[i].a_w.w_sym = gensym("true");
				else
					x->theList[i].a_w.w_sym = gensym("false");
				
			
			}
		}
		x->outsize = argc;
		outlet_list(x->outL,0L,x->outsize,x->theList);
}



void not_assist(x,b,msg,arg,dst)
Not *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Int");
			break;
			
			default:
			strcpy(dst,"set lsb to start with");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			default:
			strcpy(dst,"Not the input");
			break;
		}
	}
}


void  *not_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Not  *x;
	int i;
	
	if(ac > 8) ac = 8;
	x = (Not *)newobject(class);
	x->outL= outlet_new(x,0L);
	for(i = 0; i<MAXSIZE;++i)
	{
		x->theList[i].a_type = A_LONG;
	}
	
	return(x);
}

void not_free(x)
Not *x;
{
	
}


