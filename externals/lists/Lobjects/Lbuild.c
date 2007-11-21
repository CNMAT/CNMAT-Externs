// lbuild file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2001, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lbuild
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	Atom dropList[MAXSIZE];
	int listsize;
	int outsize;
	int dropsize;
	int start;
	int nextIn;
	int nextOut;
	void *outL;
	void *outR;
	void *out3;
} Lbuild;

void *lbuild_new();
void lbuild_free();
void  lbuild_storeList();
void lbuild_setlist();
void  lbuild_setConstant();
void  lbuild_setMember();
void  lbuild_setOne();
void  lbuild_get();
void  lbuild_next();
void  lbuild_drop();
void  lbuild_delete();
void  lbuild_floatConstant();
void lbuild_dumpList();
void lbuild_dropList();
void  lbuild_clearList();
void lbuild_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbuild_new,(method)lbuild_free,(short)sizeof(struct lbuild),0L,A_GIMME,0);
	addint((method)lbuild_setConstant);
	addfloat((method)lbuild_floatConstant);
	addbang((method)lbuild_dumpList);
	addmess((method)lbuild_storeList, "list",A_GIMME,0);
	addmess((method)lbuild_get, "get",A_DEFLONG, A_DEFLONG,0);
	addmess((method)lbuild_next, "next",A_DEFLONG, 0);
	addmess((method)lbuild_delete, "delete",A_DEFLONG, A_DEFLONG,0);
	addmess((method)lbuild_drop, "drop",A_DEFLONG, 0);
	addmess((method)lbuild_assist, "assist",A_CANT,0);
	addmess((method)lbuild_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbuild_storeList(x,s,argc,argv)
Lbuild *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i=0;
	if(argc > MAXSIZE - x->nextIn) 
		post("too much data for Lbuild");
	 else	
		for(i = 0;i< argc;++i)		// list in either inlet
			{
				x->theList[i + x->nextIn] = argv[i];
			}
		x->nextIn += i;
		x->listsize = x->nextIn;
		if(check_inlet(x) == 0)		// constant is in left inlet
		lbuild_dumpList(x);
	x->inletNumber = 0;
}



void  lbuild_clearList(x)
Lbuild *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	x->nextIn = 0;
	x->nextOut = 0;
	x->listsize = 0;
	
}


void  lbuild_setConstant(x,n)
Lbuild *x;
long n;
{
	x->theList[x->nextIn].a_type = A_LONG;
	x->theList[x->nextIn ++ ].a_w.w_long = n;
	++ x->listsize;
	if(check_inlet(x) == 0)		// constant is in left inlet
		lbuild_dumpList(x);
	x->inletNumber = 0;
	
}

void  lbuild_get(x,index,count)
Lbuild *x;
long index, count;
{
	int i;
	int here;
	
	if( (index > -1) && (index < x->listsize))
	{
		if(count > MAXSIZE) count = MAXSIZE;
		if(count < 1) count = 1;
		here = index;
		x->outsize = 0;
		for(i=0; i < count; ++i) {
			x->outList[i] = x->theList[here];
			++ x->outsize;
			++here;
			if(here == x->nextIn) here = 0;  // wrap around
		}
		x->nextOut = here;
		if(x->outsize == 1){
			switch(x->outList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->outList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->outList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->outL,x->outList[0].a_w.w_sym,0,NIL);
		}}
		else
		outlet_list(x->outL,0L,x->outsize,x->outList);	
	}
	else
	outlet_bang(x->outR);
	x->inletNumber = 0;
	
}
  
void  lbuild_next(x,count)
Lbuild *x;
long count;
{
	
	lbuild_get(x,x->nextOut,count);
	
}
  
void  lbuild_delete(x,index,count)
Lbuild *x;
long index, count;
{
	int i,j;
	
	if(x->listsize)
	{
		if((index > -1) && (index < x->nextIn)) // valid index
		{
			if( count == 0) count = 1;
			if((count + index) > x->nextIn ) count = x->nextIn - index; // valid coount
			
			j = 0;
			for(i=index; i < index + count ; ++i) {
				x->dropList[j++] = x->theList[i];  //we who are about to die
				}
			
			for(i=index; i < x->listsize - count ; ++i) {
				x->theList[i] = x->theList[i + count];
			}
			
			x->listsize -= count;
			x->nextIn = x->listsize;
			x->dropsize = j;
			lbuild_dropList(x);
		}
	
	}
	
	x->inletNumber = 0;
	
}
  
void  lbuild_drop(x,count)
Lbuild *x;
long count;
{
	long index;
	
	index = x->nextIn - count;
	if(index < 0) 
		index = 0;  // delete will fix count
	
	lbuild_delete(x,index,count);
	
}
  
void  lbuild_floatConstant(x,n)
Lbuild *x;
double n;
{
	
	x->theList[x->nextIn].a_type = A_FLOAT;
	x->theList[x->nextIn ++ ].a_w.w_float = n;
	++ x->listsize;
	if(check_inlet(x) == 0)		// constant is in left inlet
		lbuild_dumpList(x);
	x->inletNumber = 0;
	
}
void lbuild_dumpList(x)
Lbuild *x;
{
	
	if(x->listsize){
		if(x->listsize == 1){
			switch(x->theList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->theList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->theList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->outL,x->theList[0].a_w.w_sym,0,NIL);
			}
		}
		else
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	}
	else
		outlet_bang(x->outR);
	x->nextOut = 0;
	
}

void lbuild_dropList(x)
Lbuild *x;
{
	
	if(x->dropsize){
		if(x->dropsize == 1){
			switch(x->dropList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->out3,x->dropList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->out3,x->dropList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->out3,x->dropList[0].a_w.w_sym,0,NIL);
			}
		}
		else
		outlet_list(x->out3,0L,x->dropsize,x->dropList);	
	}
	else
		outlet_bang(x->outR);
	x->nextOut = 0;
	
}


void lbuild_assist(x,b,msg,arg,dst)
Lbuild *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Add to list and output");
			break;
			
			default:
			strcpy(dst,"Add to list");
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
			
			case 1:
			strcpy(dst,"bang when Lbuild empty");
			break;
			
			default:
			strcpy(dst,"Items deleted or dropped");
			break;
		}
	}
}

void  *lbuild_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{
	Lbuild  *x;
	int i;
	
	
	x = (Lbuild *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out3= outlet_new(x,0L);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->nextIn = argc;
	x->nextOut = 0;
	x->listsize = argc;
	for(i = 0; i < argc; ++i)
	{
		x->theList[i] = argv[i];
	}
	
	for(; i < MAXSIZE; ++i)
	{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
	} 	
	
	
	return(x);
}

void lbuild_free(x)
Lbuild *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lbuild *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}