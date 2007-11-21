// laccum file for Max object
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


typedef struct laccum
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	int listsize;
	Atom outList[MAXSIZE];
	int outsize;
	void *out;
} Laccum;


void *laccum_new();
void laccum_free();
void  laccum_accumList();
void laccum_setlist();
void  laccum_doInt();
void  laccum_doFloat();
void laccum_dumpList();
void  laccum_clearList();
void  laccum_restoreList();
void laccum_assist();
void laccum_flush();
char *strcpy();
int check_inlet();
void *class;

int main(void)
{


	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)laccum_new,(method)laccum_free,(short)sizeof(struct laccum),0L,A_GIMME,0);
	addint((method)laccum_doInt);
	addfloat((method)laccum_doFloat);
	addbang((method)laccum_dumpList);
	addmess((method)laccum_accumList, "list",A_GIMME,0);
	addmess((method)laccum_assist, "assist",A_CANT,0);
	addmess((method)laccum_clearList, "clear",0);
	addmess((method)laccum_restoreList, "restore",0);
	addmess((method)laccum_flush, "flush",0);
	finder_addclass("Lists","Laccum");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void laccum_accumList(x,s,argc,argv)
Laccum *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	double tempF;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type) // promote to double if necessary
			{
				case A_LONG:
			
			if(x->outList[i].a_type == A_LONG)
				x->outList[i].a_w.w_long += argv[i].a_w.w_long;
			
			else		//convert input to double if output is double
			{
			x->outList[i].a_w.w_float += (double)argv[i].a_w.w_long;
			}
			break;
			
			case A_FLOAT:
			
			if(x->outList[i].a_type == A_FLOAT)
				x->outList[i].a_w.w_float += argv[i].a_w.w_float;
			
			else 		//convert output to double
				{
				tempF = (double)x->outList[i].a_w.w_long;
				x->outList[i].a_type = A_FLOAT;
				x->outList[i].a_w.w_float = tempF + argv[i].a_w.w_float;
				}
			break;
			
			default:
			//post("Invalid List Member");
			break;
			}
		}
		if(argc > x->outsize) x->outsize = argc;
		if(check_inlet(x)==0) laccum_dumpList(x);
	
	
}

void laccum_flush(x)
Laccum *x;
{
	laccum_dumpList(x);
	laccum_clearList(x);
}

void  laccum_clearList(x)
Laccum *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	
	x->outsize = 1;
	
}

void  laccum_restoreList(x)
Laccum *x;
{
	int i;
	for(i=0;i<x->listsize;++i)
		{
		x->outList[i] = x->theList[i];
		}
	
	x->outsize = x->listsize;
	
}

void  laccum_doInt(x,n)
Laccum *x;
long n;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
			if(x->outList[i].a_type == A_LONG)
			x->outList[i].a_w.w_long += n;
			
			else
			{
			x->outList[i].a_w.w_float += (double)n;
			}
		}
	if(check_inlet(x)==0) laccum_dumpList(x);
	
}

void  laccum_doFloat(x,n)
Laccum *x;
double n;
{
	int i;
	long tempL;

	
	
	for(i=0;i<MAXSIZE;++i)
		{
			if(x->outList[i].a_type == A_FLOAT)
				x->outList[i].a_w.w_float += n;
			else
			{
			tempL = x->outList[i].a_w.w_long;
			x->outList[i].a_type = A_FLOAT;
			x->outList[i].a_w.w_float = n + (double)tempL;
			}

		}
	if(check_inlet(x)==0) laccum_dumpList(x);
	
}

void laccum_dumpList(x)
Laccum *x;
{
	
	if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
				else
				outlet_float(x->out,x->outList[0].a_w.w_float);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}


void laccum_assist(x,b,msg,arg,dst)
Laccum *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Add and output");
			break;
			
			default:
			strcpy(dst,"Add to stored list");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Accumulated totals");//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *laccum_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Laccum  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Laccum *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	
	switch(ac){
		case 0:
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = 0;
		x->outsize = 1;
		x->listsize = 0;
		break;
		
		case 1:					
			switch(theArgs[0].a_type){
				case A_LONG:
				case A_FLOAT:
					x->outList[0] = theArgs[0];
				break;
				
				default:
					x->outList[0].a_type = A_LONG;
					x->outList[0].a_w.w_long = 0;
			
			}
			for(i=1;i<MAXSIZE;++i)	
				x->outList[i] = x->outList[0];
			
			x->outsize = 1;
			x->theList[0] = x->outList[0];
			x->listsize = 1;
			break;
			
		default:
			for(i=0;i<ac;++i)
			{
				switch(theArgs[i].a_type){
					case A_LONG:
					case A_FLOAT:
						x->outList[i] = theArgs[i];
					break;
					
					default:
						x->outList[i].a_type = A_LONG;
						x->outList[i].a_w.w_long = 0;
				}
				x->theList[i] = x->outList[i];
			}
			for(;i<MAXSIZE;++i)
			{
				x->outList[i].a_type = A_LONG;
				x->outList[i].a_w.w_long = 0;
			}
			x->outsize = x->listsize = ac;
	}
	return(x);
}

void laccum_free(x)
Laccum *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Laccum *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}