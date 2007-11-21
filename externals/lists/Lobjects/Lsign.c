// lsign file for Max object
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

typedef struct lsign
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	int testme;
	void *out;
	void *outR;
} Lsign;


void *lsign_new();
void lsign_free();
void  lsign_doLists();
void lsign_setlist();
void  lsign_setConstant();
void  lsign_floatConstant();
void lsign_dumpList();
void  lsign_clearList();
void lsign_assist();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsign_new,(method)lsign_free,(short)sizeof(struct lsign),0L,A_GIMME,0);
	addint((method)lsign_setConstant);
	addfloat((method)lsign_floatConstant);
	addbang((method)lsign_dumpList);
	addmess((method)lsign_doLists, "list",A_GIMME,0);
	addmess((method)lsign_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lsign");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lsign_doLists(x,s,argc,argv)
Lsign *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	
	
	for(i=0;i<argc;++i)
	{
		switch(argv[i].a_type){
		case A_LONG:
		if(argv[i].a_w.w_long > 0) x->outList[i].a_w.w_long = 1;
		if(argv[i].a_w.w_long == 0) x->outList[i].a_w.w_long = 0;
		if(argv[i].a_w.w_long < 0) x->outList[i].a_w.w_long = -1;
		break;
		
		case A_FLOAT:
		if(argv[i].a_w.w_float > 0) x->outList[i].a_w.w_long = 1;
		if(argv[i].a_w.w_float == 0) x->outList[i].a_w.w_long = 0;
		if(argv[i].a_w.w_float < 0) x->outList[i].a_w.w_long = -1;
		break;
		
		case A_SYM:
		x->outList[i].a_w.w_long = 1;
		if(argv[i].a_w.w_sym->s_name[0] == '0') x->outList[i].a_w.w_long = 0;
		if(argv[i].a_w.w_sym->s_name[0] == '-') x->outList[i].a_w.w_long = -1;
		break;
		}
	}
	x->outsize = argc;
	lsign_dumpList(x);
	
}



void  lsign_setConstant(x,n)
Lsign *x;
long n;
{
	
	x->outsize = 1;
	if(n > 0) x->outList[0].a_w.w_long = 1;
	if(n == 0) x->outList[0].a_w.w_long = 0;
	if(n < 0) x->outList[0].a_w.w_long = -1;
	lsign_dumpList(x);
	
	
}

void  lsign_floatConstant(x,n)
Lsign *x;
float n;
{
	
	x->outsize = 1;
	if(n > 0) x->outList[0].a_w.w_long = 1;
	if(n == 0) x->outList[0].a_w.w_long = 0;
	if(n < 0) x->outList[0].a_w.w_long = -1;
	lsign_dumpList(x);
	
}

void lsign_dumpList(x)
Lsign *x;
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


void lsign_assist(x,b,msg,arg,dst)
Lsign *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"list, int or float");
			break;
			
			default:
			strcpy(dst,"1, 0, or -1 indicates sign");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"1, 0, or -1 indicates sign");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
	
	
}


void  *lsign_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lsign  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsign *)newobject(class);
	x->out= outlet_new(x,0L);
	
	x->outsize = 0;
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type =A_LONG;
		}
		
	
	return(x);
}

void lsign_free(x)
Lsign *x;
{
	
	

}

