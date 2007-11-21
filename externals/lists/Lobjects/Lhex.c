// lhex file for Max object
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

typedef struct lhex
{
	struct object ob;
	
	Symbol *byteSym[256];
	Atom outList[MAXSIZE];
	int listsize;
	int oldListSize;
	long nextValue;
	int dataPending;
	int loHigh;
	int nibbleSize;
	int nibbleMask;
	int maxBytes;
	void *outL;
	Symbol *messageType;
} Lhex;


void *lhex_new();
void lhex_free();
void  lhex_Lists();
void  lhex_setConstant();
void  lhex_floatConstant();
void lhex_anything();
void lhex_bang();
void lhex_dumpList();
void lhex_hiLo();
void lhex_loHi();
void lhex_nibble();
void lhex_clear();
void lhex_assist();
long convertHex();
int isHex();
int charHex();
Symbol *convertToHex();
Symbol *makeHex();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lhex_new,(method)lhex_free,(short)sizeof(struct lhex),0L,A_GIMME,0);
	addint((method)lhex_setConstant);
	addfloat((method)lhex_floatConstant);
	addbang((method)lhex_dumpList);
	addmess((method)lhex_Lists, "list",A_GIMME,0);
	
	//addmess((method)lhex_setList, "set",A_GIMME,0);
	addmess((method)lhex_assist, "assist",A_CANT,0);
	
	addmess((method)lhex_anything, "anything",A_GIMME,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void lhex_Lists(x,s,argc,argv)
Lhex *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i=0;i<argc;++i)
	{
		
		switch(argv[i].a_type)
		{
			case A_LONG:
			x->outList[i].a_w.w_sym = convertToHex(x,argv[i].a_w.w_long);
			break;
		
			case A_FLOAT:
			x->outList[i].a_w.w_sym = convertToHex(x,(long)argv[i].a_w.w_float);
			break;
			
			case A_SYM:
			x->outList[i].a_w.w_sym = argv[i].a_w.w_sym;
			break;
		}
	}
		
	x->listsize = argc;
	lhex_dumpList(x);
	
}


void lhex_anything(x,s,argc,argv)
Lhex *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	x->outList[0].a_type = A_SYM;
	x->outList[0].a_w.w_sym =s;
	if(argc +1 >=MAXSIZE) argc = MAXSIZE-1;
	for(i = 1; i <= argc;++i)
	{
		switch(argv[i].a_type)
		{
			case A_LONG:
			
			x->outList[i].a_w.w_sym = convertToHex(x,argv[i-1].a_w.w_long);
			
			break;
		
			case A_FLOAT:
			
			x->outList[i].a_w.w_sym = convertToHex(x,(long)argv[i-1].a_w.w_float);
			break;
			
			case A_SYM:
			x->outList[i].a_w.w_sym = argv[i-1].a_w.w_sym;
			break;
		}
	}
	x->listsize = argc+1;
	lhex_dumpList(x);
	
}

void lhex_dumpList(x)
Lhex *x;
{	
	outlet_list(x->outL,0L,(short)x->listsize,x->outList);
}

void  lhex_setConstant(x,n)  
Lhex *x;
long n;
{
	
	//x->outList[0].a_w.w_sym = makeHex(n);
	x->outList[0].a_w.w_sym = convertToHex(x,n);
	x->listsize = 1;
	lhex_dumpList(x);
	
	
}

void  lhex_floatConstant(x,n)
Lhex *x;
float n;
{
	
	
	x->outList[0].a_w.w_sym =  convertToHex(x,(long) n);
	x->listsize = 1;
	lhex_dumpList(x);
	
}


void lhex_assist(x,b,msg,arg,dst)
Lhex *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		
		strcpy(dst,"Data");
		
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Data in hex format");
	}
	
	
}

Symbol *convertToHex(x,n)
Lhex *x;
long n;
{
	
	n &= 0xFF;
	return(x->byteSym[n]);
	
}

Symbol *makeHex(n)
long n;
{
	char resultStr[] = "00h\0";
	char digitStr[] = "0123456789ABCDEF\0";
	n &= 0xFF;
	resultStr[0] = digitStr[n/16];
	resultStr[1] = digitStr[n%16];
	return(gensym(resultStr));
	
}

void  *lhex_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lhex  *x;
	long i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lhex *)newobject(class);
	x->outL= outlet_new(x,0L);
	x->listsize=0;
	for(i = 0; i<256;++i)
	{
		x->byteSym[i]= makeHex(i);
		x->outList[i].a_type = A_SYM;
		
	}
	
	return(x);
}


void lhex_free(x)
Lhex *x;
{
}