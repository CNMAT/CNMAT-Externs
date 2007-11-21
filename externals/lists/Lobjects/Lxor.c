// lxor file for Max object
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

typedef struct lxor
{
	struct object ob;
	void *proxy;
	long dummy;
	void *outL;
} Lxor;


void *lxor_new();
void lxor_free();
void  lxor_doLists();
void lxor_anything();
void lxor_assist();
long convertHex();
int isHex();
int charHex();
void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	setup((t_messlist **)&class,(method)lxor_new,(method)lxor_free,(short)sizeof(struct lxor),0L,A_GIMME,0);
	
	addmess((method)lxor_doLists, "list",A_GIMME,0);
	addmess((method)lxor_assist, "assist",A_CANT,0);
	addmess((method)lxor_anything, "anything",A_GIMME,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lxor_doLists(x,s,argc,argv)
Lxor *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long total;
	long temp;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	total = 0;
	temp = 0;
	for(i=0; i < argc; ++i){
			switch(argv[i].a_type)
		{
			case A_LONG:
			temp = argv[i].a_w.w_long;
			break;
		
			case A_FLOAT:
			temp = (long)argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(isHex(argv[i].a_w.w_sym))  // convert hex 
				temp = convertHex(argv[i].a_w.w_sym);
			else
				temp = 0;
		}
		total ^= temp;
	}
	outlet_int(x->outL, (long)total);
	
}


void lxor_anything(x,s,argc,argv)
Lxor *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int temp;
	long total;
	
	total = 0;
	temp = 0;
	if(isHex(s))  // convert s and put it at the head of the list
	{
		total = convertHex(s);
		if(argc >= MAXSIZE) argc = MAXSIZE-1;
		for(i=0;i<argc;++i)
	{
		
			switch(argv[i].a_type)
		{
			case A_LONG:
			temp = argv[i].a_w.w_long;
			break;
		
			case A_FLOAT:
			temp = (long)argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(isHex(argv[i].a_w.w_sym))  // convert hex 
				temp = convertHex(argv[i].a_w.w_sym);
		}
		total ^= temp;
	}
		
	outlet_int(x->outL, (long)total);
	}

	
}

void lxor_assist(x,b,msg,arg,dst)
Lxor *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List");
			break;
			
			default:
			strcpy(dst,"");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Xor of items in List");
			break;
			
			default:
			strcpy(dst,"");
			break;
		}
	}
}

void  *lxor_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lxor  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lxor *)newobject(class);
	x->outL= outlet_new(x,0L);
	
	return(x);
}

void lxor_free(x)
Lxor *x;
{
}

long convertHex(theSymbol)
Symbol *theSymbol;
{
	long result = 0;
	int i,length;
	length = strlen(theSymbol->s_name);
	for(i = 0;i<length - 1; ++i)
	{
	result = result << 4;
	result += charHex(theSymbol->s_name[i]);
	
	}
	return(result);

}

int charHex(theChar)
unsigned char theChar;
{
	switch(theChar)
	{
		case '0':
		return(0);
		
		case '1':
		return(1);
		
		case '2':
		return(2);
		
		case '3':
		return(3);
		
		case '4':
		return(4);
		
		case '5':
		return(5);
		
		case '6':
		return(6);
		
		case '7':
		return(7);
		
		case '8':
		return(8);
		
		case '9':
		return(9);
		
		case 'a':
		case 'A':
		return(10);
		
		case 'b':
		case 'B':
		return(11);
		
		case 'c':
		case 'C':
		return(12);
		
		case 'd':
		case 'D':
		return(13);
		
		case 'e':
		case 'E':
		return(14);
		
		case 'f':
		case 'F':
		return(15);
		
		default:
		return(0);
	
	}

}


int isHex(theSymbol)
Symbol *theSymbol;
{
	int i,length;
	unsigned char theChar;
	length = strlen(theSymbol->s_name);
	if(theSymbol->s_name[length-1] != 'h' && theSymbol->s_name[length-1] != 'H')
		return(0);
	for(i= 0; i < length-1; ++i)
	{
		theChar = theSymbol->s_name[i];
		if(theChar > 0x2F && theChar < 0x3A)
			continue;
		if(theChar > 0x40 && theChar < 0x47)
			continue;
		if(theChar > 0x60 && theChar < 0x67)
			continue;
		return(0);
	}
	return(1);
}

