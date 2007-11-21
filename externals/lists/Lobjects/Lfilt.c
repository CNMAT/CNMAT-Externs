// lfilt file for Max object
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

typedef struct lfilt
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	void *out;
	void *outR;
	void *outL;
} Lfilt;


void *lfilt_new();
void lfilt_free();
void  lfilt_doLists();
void lfilt_doSymbol();
void  lfilt_doConstant();
void  lfilt_floatConstant();
void lfilt_dumpList();
void  lfilt_clearList();
void lfilt_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lfilt_new,(method)lfilt_free,(short)sizeof(struct lfilt),0L,A_GIMME,0);
	addint((method)lfilt_doConstant);
	addfloat((method)lfilt_floatConstant);
	addbang((method)lfilt_dumpList);
	addmess((method)lfilt_doLists, "list",A_GIMME,0);
	addmess((method)lfilt_doSymbol, "anything",A_GIMME,0);
	addmess((method)lfilt_assist, "assist",A_CANT,0);
	addmess((method)lfilt_clearList, "clear",0);
	finder_addclass("Lists","Lfilt");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lfilt_doLists(x,s,argc,argv)
Lfilt *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j, k, found;	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet
	{
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)  // convert comparison value to float
			{ 
			case A_LONG:
			
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = (double)argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_type = A_FLOAT;
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i] = argv[i];
			break;
			
			default:
			
			x->theList[i].a_type = A_SYM;
			x->theList[i].a_w.w_sym = 0;
			}
		}
		
	x->listsize = argc;
	}
	else		// list in left inlet
		{
		k = 0;
		
	for(i=0;i<argc;++i)
		{
		found = 0;
		switch(argv[i].a_type) 
			{
				case A_LONG:
			for(j = 0;j < x->listsize;++j)
				{
				//post("j=%d ",j);
				if(x->theList[j].a_type == A_SYM)
					continue;
				if( (double)argv[i].a_w.w_long == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_FLOAT:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type == A_SYM)
					continue;
				if(argv[i].a_w.w_float == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_SYM:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type == A_SYM)
				
				if(argv[i].a_w.w_sym == x->theList[j].a_w.w_sym)
					{
					found = 1;
					break;
					}
				}
			break;
			
			default:
			//post("Invalid List Member");
			break;
			}
		if(found == 0)
			{
			x->outList[k].a_type = argv[i].a_type;
			x->outList[k].a_w = argv[i].a_w;
			++k;
			//post("k=%d ",k);
			}
		found = 0;
		
		} //end of i loop
		if(k)  // something got through
			{
			x->outsize = k;
			x->out = x->outL;
			}
		else
			{
			for(i=0;i<argc;++i)
				{
				x->outList[i].a_type = argv[i].a_type;
				x->outList[i].a_w = argv[i].a_w;
				}
			x->outsize = argc;
			x->out = x->outR;
			}
		lfilt_dumpList(x);
	}
	
}

void lfilt_doSymbol(x,s,argc,argv)
Lfilt *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j, k, found;
	
	if(check_inlet(x))  // list in right inlet
	{
		x->theList[0].a_type = A_SYM;
			x->theList[0].a_w.w_sym = s;
		for(i=0;i<=argc;++i)
			{
			if(argv[i].a_type==A_LONG){
				
				x->theList[i+1].a_type = A_FLOAT;
				x->theList[i+1].a_w.w_float = (double)argv[i].a_w.w_long;
				
			}else
				x->theList[i+1] = argv[i];
				
			}
		
		x->listsize = argc + 1;
	}
	else		// list in left inlet
	{
		k = 0;
		found = 0;
	for(j = 0;j < x->listsize;++j){
		if(x->theList[j].a_type == A_SYM)
			if(s == x->theList[j].a_w.w_sym)
				{
				found = 1;
				break;
				}
	}
	if(found == 0){
		x->outList[k].a_type = A_SYM;
		x->outList[k].a_w.w_sym = s;
		++k;
		//post("k=%d ",k);
		}
	
	for(i=0;i<argc;++i){
		found = 0;
		switch(argv[i].a_type) 
			{
				case A_LONG:
			for(j = 0;j < x->listsize;++j)
				{
				//post("j=%d ",j);
				if(x->theList[j].a_type == A_SYM)
					continue;
				if( (double)argv[i].a_w.w_long == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_FLOAT:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type == A_SYM)
					continue;
				if(argv[i].a_w.w_float == x->theList[j].a_w.w_float)
					{
					found = 1;
					break;
					}
				}
			break;
			
			case A_SYM:
			for(j = 0;j<x->listsize;++j)
				{
				if(x->theList[j].a_type == A_SYM)
				
				if(argv[i].a_w.w_sym == x->theList[j].a_w.w_sym)
					{
					found = 1;
					break;
					}
				}
			break;
			
			default:
			//post("Invalid List Member");
			break;
			}
		if(found == 0)
			{
			x->outList[k].a_type = argv[i].a_type;
			x->outList[k].a_w = argv[i].a_w;
			++k;
			//post("k=%d ",k);
			}
		found = 0;
		
		} //end of i loop
		if(k)  // something got through
			{
			x->outsize = k;
			x->out = x->outL;
			}
		else
			{
			for(i=0;i<argc;++i)
				{
				x->outList[i].a_type = argv[i].a_type;
				x->outList[i].a_w = argv[i].a_w;
				}
			x->outsize = argc;
			x->out = x->outR;
			}
		lfilt_dumpList(x);
	}
}	



void  lfilt_clearList(x)
Lfilt *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_SYM;
		x->theList[i].a_w.w_sym = 0;
		}
	
	x->listsize = 0;

	
}


void  lfilt_doConstant(x,n)
Lfilt *x;
long n;
{
	int i;
	int found;
	
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = (double)n;
	x->listsize = 1;
	}
	else
		{
	for(i=0;i<x->listsize;++i)
		{	
		if(x->theList[i].a_type == A_SYM)
					continue;
		if( n == (long)x->theList[i].a_w.w_float)
			{
			found = 1;
			break;
			}
		}
		x->outsize = 1;
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = n;
		if(found == 0)
			x->out = x->outL;
		else
			x->out = x->outR;
		lfilt_dumpList(x);
		}
	
	
}

void  lfilt_floatConstant(x,n)
Lfilt *x;
double n;
{
	int i,found;
	
	found = 0;
	if(check_inlet(x))		// constant is in right inlet
	{
	x->theList[0].a_type = A_FLOAT;
	x->theList[0].a_w.w_float = n;
	x->listsize = 1;
	}
	else
		{
	for(i=0;i<x->listsize;++i)
		{
		if(x->theList[i].a_type == A_SYM)
					continue;
		if( n == x->theList[i].a_w.w_float)
			{
			found = 1;
			break;
			}
		}
		x->outsize = 1;
		x->outList[0].a_type = A_FLOAT;
		x->outList[0].a_w.w_float = n;
		if(found == 0)
			x->out = x->outL;
		else
			x->out = x->outR;
		lfilt_dumpList(x);
		}
	
	
}

void lfilt_dumpList(x)
Lfilt *x;
{
	
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


void lfilt_assist(x,b,msg,arg,dst)
Lfilt *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to process");
			break;
			
			default:
			strcpy(dst,"items to reject");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List without rejections");//forgot something
			break;
			
			default:
			strcpy(dst,"List if all rejects");
			break;
		}
	}
	
}



void  *lfilt_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lfilt  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lfilt *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	
	switch(ac)
	{
	case 0:
		theArgs[0].a_type = A_SYM;  // sym matches anything
		theArgs[0].a_w.w_sym = 0;
		x->listsize = 0;
		break;
	
	case 1:					
		x->theList[0].a_type = A_FLOAT;
		if(theArgs[0].a_type == A_LONG)
			x->theList[0].a_w.w_float = (double)theArgs[0].a_w.w_long;
		else
			x->theList[0] = theArgs[0];
	
		x->listsize = 1;
		break;
		
	default:
		for(i=0;i<ac;++i)
		{
		x->theList[i].a_type = A_FLOAT;
		if(theArgs[i].a_type == A_LONG)
			x->theList[i].a_w.w_float = (double)theArgs[i].a_w.w_long;
		else
			x->theList[i] = theArgs[i];
		}
		
	x->listsize = ac;
	}
	
	
	return(x);
}

void lfilt_free(x)
Lfilt *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lfilt *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}