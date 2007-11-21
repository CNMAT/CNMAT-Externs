// lreplace file for Max object
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

typedef struct lreplace
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	void *out;
} Lreplace;


void *lreplace_new();
void lreplace_free();
void  lreplace_doLists();
void lreplace_delete();
void  lreplace_setConstant();
void  lreplace_floatConstant();
void lreplace_dumpList();
void  lreplace_clearList();
void lreplace_assist();
void lreplace_restore();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lreplace_new,(method)lreplace_free,(short)sizeof(struct lreplace),0L,A_GIMME,0);
	addint((method)lreplace_setConstant);
	addfloat((method)lreplace_floatConstant);
	addbang((method)lreplace_dumpList);
	addmess((method)lreplace_doLists, "list",A_GIMME,0);
	addmess((method)lreplace_delete, "delete",A_GIMME,0);
	addmess((method)lreplace_assist, "assist",A_CANT,0);
	addmess((method)lreplace_clearList, "clear",0);
	addmess((method)lreplace_restore, "restore",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lreplace_doLists(x,s,argc,argv)
Lreplace *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	int thisone[MAXSIZE];
	int insertThisMany;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(x->inletNumber == 0)  // list in left inlet
	{
	insertThisMany = argc -2;
		for(i = 0; i < x->outsize;++i)
		{
			
			if((argv[0].a_type == x->outList[i].a_type) && 
										(argv[0].a_w.w_long == x->outList[i].a_w.w_long))
				thisone[i] = 1;
			else
				thisone[i] = 0;
		}
		for(i = x->outsize-1; i >= 0; --i)
		{
			if(thisone[i])
			{
				x->outsize+=insertThisMany;
				if(x->outsize >= MAXSIZE)
					x->outsize = MAXSIZE;
				for(j= x->outsize-1;j > i + insertThisMany; --j)
						x->outList[j] = x->outList[j-insertThisMany];
				for(j = 0; j<= insertThisMany; ++ j)
					x->outList[i + j] = argv[j+1];
			}
		}
	}
	else
	{
		for(i=0;i<argc;++i)
		{
			x->outList[i] = argv[i];
		}
		x->outsize = argc;
	}
	x->inletNumber = 0;
	
}

void lreplace_delete(x,s,argc,argv)
Lreplace *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	int thisone[MAXSIZE];
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	
		for(i = 0; i < x->outsize;++i)
		{
		thisone[i] = 0;
			for(j = 0;j < argc;++ j)
			{
			if((argv[j].a_type == x->outList[i].a_type) && 
										(argv[j].a_w.w_long == x->outList[i].a_w.w_long))
				thisone[i] = 1;
			}
		}
		for(i = x->outsize-1; i >= 0; --i)
		{
			if(thisone[i])
			{
				
				for(j= i;j< x->outsize-1; ++j)
						x->outList[j] = x->outList[j+1];
				--x->outsize;
				
			}
		}
	
	

	x->inletNumber = 0;
	
}

void  lreplace_clearList(x)
Lreplace *x;
{		
	
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = 0;
	
	x->outsize = 1;
	x->inletNumber = 0;
	
}


void lreplace_restore(x)
Lreplace *x;
{
	int i;
	
	
	for(i=0;i<x->listsize;++i)
		{
		x->outList[i].a_type = x->theList[i].a_type;
		x->outList[i].a_w = x->theList[i].a_w;
		}
	
	x->outsize = x->listsize;
	x->inletNumber = 0;
	
}


void  lreplace_setConstant(x,n)
Lreplace *x;
long n;
{	
	if(x->inletNumber)		// constant is in right inlet
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
	x->inletNumber = 0;
	
}

void  lreplace_floatConstant(x,n)
Lreplace *x;
double n;
{
	
	lreplace_setConstant(x,(long)n);
	
}

void lreplace_dumpList(x)
Lreplace *x;
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
	x->inletNumber = 0;
	
}


void lreplace_assist(x,b,msg,arg,dst)
Lreplace *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List, first is replaced by rest.");
			break;
			
			case 1:
			strcpy(dst,"Starting List.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"List with items replaced.");
	}
	
	
}



void  *lreplace_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lreplace  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lreplace *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->listsize = ac;
	x->outsize = ac;
	
	
	switch(ac)
	{
	case 0:
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
		x->listsize = 1;
		break;
	
	case 1:					
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = theArgs[0].a_type;
		x->outList[i].a_w.w_long = theArgs[0].a_w.w_long;
		}
		
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->outList[i].a_type = theArgs[i].a_type;
		x->outList[i].a_w.w_long = theArgs[i].a_w.w_long;
		}
		for(;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	
	}
	for(i = 0;i<MAXSIZE;++i)
		{
		x->theList[i] = x->outList[i];
		}
		
	
	return(x);
}

void lreplace_free(x)
Lreplace *x;
{
	
	freeobject(x->proxy);
	

}

int check_inlet(x)
Lreplace *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}