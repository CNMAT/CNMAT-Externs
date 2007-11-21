// link file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1998, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct link
{
	struct object ob;
	void *proxy;
	long inletNumber;
	//Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int initialsize;
	int maximumsize;
	//int cleared;
	int outsize;
	//int interleaf;
	void *outL;
	//int oddtype;
	Symbol *messageType;
} Link;


void *link_new();
void  link_free();
void  link_setList();
void  link_linkLists();
void  link_anything();
void  link_setlist();
void  link_setConstant();
void  link_floatConstant();
void  link_symbol();
void  link_setMaximum();
void  link_bang();
void  link_dumpList();
void  link_clearList();
void  link_assist();
long  convertHex();
int isHex();
int charHex();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)link_new,(method)link_free,(short)sizeof(struct link),0L,A_GIMME,0);
	addint((method)link_setConstant);
	addfloat((method)link_floatConstant);
	addbang((method)link_bang);
	addmess((method)link_linkLists, "list",A_GIMME,0);
	
	addmess((method)link_setList, "set",A_GIMME,0);
	addmess((method)link_setMaximum, "maximum",A_DEFLONG,0);
	addmess((method)link_assist, "assist",A_CANT,0);
	addmess((method)link_clearList, "clear",0);
	addmess((method)link_symbol, "symbol",A_GIMME,0);
	addmess((method)link_anything, "anything",A_GIMME,0);
	finder_addclass("Lists","Link");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	}

void link_setList(x,s,ac,theArgs)
Link *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int i;
	

		for(i=0;i<ac;++i)
		{
			x->outList[i] = theArgs[i];
		}
			
		x->listsize = ac;
		x->initialsize = ac;
		if(x->maximumsize < ac+1) x->maximumsize = ac + 1;
	
	x->inletNumber = 0;
	
}

void link_linkLists(x,s,argc,argv)
Link *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	if(x->listsize < x->maximumsize)
	{
		for(i=0;i<argc;++i)
			{
			x->outList[x->listsize++] = argv[i];
			if(x->listsize == x->maximumsize) break;
			}
		
	}
	if(!check_inlet(x))  // list in left inlet
	{
		link_dumpList(x);
		link_clearList(x);
	}
	
	x->inletNumber = 0;
}

void link_anything(x,s,argc,argv)
Link *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	if(x->listsize < x->maximumsize)
	{
		x->outList[x->listsize].a_type = A_SYM;
		x->outList[x->listsize++].a_w.w_sym = s;
		if(x->listsize < x->maximumsize)
		for(i=0;i<argc;++i)
			{
			x->outList[x->listsize++] = argv[i];
			if(x->listsize == x->maximumsize) break;
			}
	}
	if(!check_inlet(x))  // list in left inlet
	{
		link_dumpList(x);
		link_clearList(x);
	}
	
	x->inletNumber = 0;
}


void link_bang(x)
Link *x;
{
	link_dumpList(x);
	if(!check_inlet(x))  // bang in left inlet
		link_clearList(x);
	x->inletNumber = 0;
}

void link_dumpList(x)
Link *x;
{
	
	outlet_list(x->outL,0L,(short)x->listsize,x->outList);
	
}

void  link_clearList(x)
Link *x;
{
	x->listsize = x->initialsize;
	x->inletNumber = 0;
}


void  link_setConstant(x,n)
Link *x;
long n;
{
	if(x->listsize < x->maximumsize)
	{
		x->outList[x->listsize].a_type = A_LONG;
		x->outList[x->listsize].a_w.w_long = n;	
		++x->listsize;
	}
	if(!check_inlet(x))  //  in left inlet
	{
		link_dumpList(x);
		link_clearList(x);
	}
	
	x->inletNumber = 0;
}

void  link_floatConstant(x,n)
Link *x;
float n;
{
	if(x->listsize < x->maximumsize)
	{
		x->outList[x->listsize].a_type = A_FLOAT;
		x->outList[x->listsize].a_w.w_float = n;	
		++x->listsize;
		//x->cleared = false;
	}
	if(!check_inlet(x))  // in left inlet
	{
		link_dumpList(x);
		link_clearList(x);
	}
	
	x->inletNumber = 0;
}

void  link_symbol(x,s,argc,argv)
Link *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	if(x->listsize < x->maximumsize)
	{
		if(argc + x->listsize >= x->maximumsize) argc = x->maximumsize - argc;
		for(i=0;i<argc;++i)
			{
			x->outList[x->listsize + i] = argv[i];
			}
			
		x->listsize += argc;
		//x->cleared = false;
	}
	if(!check_inlet(x))  // symbols in left inlet
	{
		link_dumpList(x);
		link_clearList(x);
	}
	
	x->inletNumber = 0;
}

void  link_setMaximum(x,n)
Link *x;
long n;
{
	if(n > x->initialsize )
		x->maximumsize = n;
	else
		x->maximumsize = x->initialsize + 1;
	
	x->inletNumber = 0;
}

void link_assist(x,b,msg,arg,dst)
Link *x;
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
			strcpy(dst,"Add to list without Output");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Linked Lists");
	}
	
}


void  *link_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Link  *x;
	int i;
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Link *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	
	for(i=0;i<ac;++i)
		x->outList[i] = theArgs[i];
		
	x->listsize = ac;
	x->initialsize = ac;
	x->maximumsize = MAXSIZE;
	return(x);
}



void link_free(x)
Link *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Link *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}