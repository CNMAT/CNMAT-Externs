// lbag file for Max object
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


typedef struct lbag
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	int listsize;
	int storeNext;
	void *outL;
	void *outR;
} Lbag;


void *lbag_new();
void lbag_free();
void  lbag_storeList();
void lbag_setlist();
void  lbag_setConstant();
void  lbag_setMember();
void  lbag_floatConstant();
void lbag_dumpList();
void  lbag_clearList();
void lbag_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbag_new,(method)lbag_free,(short)sizeof(struct lbag),0L,A_GIMME,0);
	addint((method)lbag_setConstant);
	addfloat((method)lbag_floatConstant);
	addbang((method)lbag_dumpList);
	addmess((method)lbag_storeList, "list",A_GIMME,0);
	addmess((method)lbag_assist, "assist",A_CANT,0);
	addmess((method)lbag_clearList, "clear",0);
	finder_addclass("Lists","Lbag");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbag_storeList(x,s,argc,argv)
Lbag *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  
	
	for(i=0;i<argc;++i)		// list in either inlet
		{
			if(argv[i].a_type ==A_LONG)
			lbag_setConstant(x,argv[i].a_w.w_long);
		}
		
	
	
}



void  lbag_clearList(x)
Lbag *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 0;
	
}


void  lbag_setConstant(x,n)
Lbag *x;
long n;
{
	int i;
	int found;
	
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
		x->storeNext = n;
	else
	{
		
		if(x->storeNext)  // add number to list if it isn't already there
		{
			if(x->listsize == MAXSIZE)
				goto finish;
			if(x->listsize <= 0)
			{
				x->theList[0].a_w.w_long = n;
				x->listsize = 1;
				goto finish;
			}
			for(i=0;i<x->listsize; ++i)
			{
				if(x->theList[i].a_w.w_long >= n) // list is sorted so stop
				{
					found = i;
					break;
				}
			}
			if((x->theList[i].a_w.w_long == n) && (i < x->listsize)) // there it is
				goto finish;
			if(!found && i)
			{
				x->theList[x->listsize++].a_w.w_long = n;
			}
			else  // insert it
			{
				for(i = x->listsize -1; i >= found; --i)
				{
					x->theList[i + 1].a_w.w_long = x->theList[i].a_w.w_long;
				}
				++x->listsize;
				x->theList[found].a_w.w_long = n;
			}
		}
		else  // find and remove it
		{
			for(i=0;i<x->listsize; ++i)
			{
				if(x->theList[i].a_w.w_long == n) // there it is
				{
					found = 1;
					break;
				}
			}
			if(found)
			{
				for(; i < x->listsize -1 ; ++i)
				{
					x->theList[i].a_w.w_long = x->theList[i+1].a_w.w_long;
				}
				--x->listsize;
			}
		}
	}
	finish: 	x->inletNumber = 0;
	
}

void  lbag_floatConstant(x,n)
Lbag *x;
double n;
{
	
	
	lbag_setConstant(x,(long)n);
	
}
void lbag_dumpList(x)
Lbag *x;
{
	
	if(x->listsize)
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	else
		outlet_bang(x->outR);
	
}


void lbag_assist(x,b,msg,arg,dst)
Lbag *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to store or delete");
			break;
			
			default:
			strcpy(dst,"1 to store 0 to delete");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List");
			break;
			
			default:
			strcpy(dst,"bang when Lbag empty");
			break;
		}
	}
	
	
}


void  *lbag_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lbag  *x;
	int i;
	
	
	x = (Lbag *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->storeNext = 0;
	x->listsize = 0;
	
	for(i = 0; i < MAXSIZE; ++i)
	{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
	}
	
	x->storeNext = 1;
	for(i = 0; i < argc; ++i)
	{
		
		if(argv[i].a_type == A_LONG);
		lbag_setConstant(x,argv[i].a_w.w_long);
	}
	x->storeNext = 0;
	return(x);
}

void lbag_free(x)
Lbag *x;
{
	
	freeobject(x->proxy);
	

}

int check_inlet(x)
Lbag *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}