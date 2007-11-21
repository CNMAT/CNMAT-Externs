// llong file for Max object
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

typedef struct llong
{
	struct object ob;
	
	Atom hexList[MAXSIZE];
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
} Llong;


void *llong_new();
void llong_free();
void  llong_Lists();
void  llong_setConstant();
void  llong_floatConstant();
void llong_anything();
void llong_bang();
void llong_dumpList();
void llong_hiLo();
void llong_loHi();
void llong_nibble();
void llong_clear();
void llong_assist();
long convertHex();
int isHex();
int charHex();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)llong_new,(method)llong_free,(short)sizeof(struct llong),0L,A_GIMME,0);
	addint((method)llong_setConstant);
	addfloat((method)llong_floatConstant);
	addbang((method)llong_dumpList);
	addmess((method)llong_Lists, "list",A_GIMME,0);
	
	addmess((method)llong_assist, "assist",A_CANT,0);
	
	addmess((method)llong_anything, "anything",A_GIMME,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void llong_Lists(x,s,argc,argv)
Llong *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	for(i=0;i<argc;++i)
	{
		x->outList[i].a_type = A_LONG;
		switch(argv[i].a_type)
		{
			case A_LONG:
			x->outList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
		
			case A_FLOAT:
			x->outList[i].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(isHex(argv[i].a_w.w_sym))  // convert hex 
			{
				
				x->outList[i].a_w.w_long = convertHex(argv[i].a_w.w_sym);
			}
			else  // pass symbols through
			{
				x->outList[i].a_type = A_SYM;
				x->outList[i].a_w.w_sym = argv[i].a_w.w_sym;
			}
		}
	}
		
	x->listsize = argc;
	llong_dumpList(x);
	
}

void llong_anything(x,s,argc,argv)
Llong *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(isHex(s))  // convert s and put it at the head of the list
	{
		x->outList[0].a_type = A_LONG;
		x->outList[0].a_w.w_long = convertHex(s);
		if(argc >= MAXSIZE) argc = MAXSIZE-1;
		for(i=0;i<argc;++i)
	{
		x->outList[i+1].a_type = A_LONG;
		switch(argv[i].a_type)
		{
			case A_LONG:
			x->outList[i+1].a_w.w_long = argv[i].a_w.w_long;
			break;
		
			case A_FLOAT:
			x->outList[i+1].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			if(isHex(argv[i].a_w.w_sym))  // convert hex 
			{
				x->outList[i+1].a_w.w_long = convertHex(argv[i].a_w.w_sym);
			}
			else  // pass symbols through
			{
				x->outList[i+1].a_type = A_SYM;
				x->outList[i+1].a_w.w_sym = argv[i].a_w.w_sym;
			}
		}
	}
		
	x->listsize = argc+1;
	}
	else   // ignore s and convert the rest
	{
		if(argc >= MAXSIZE) argc = MAXSIZE;
		for(i=0;i<argc;++i)
		{
			x->outList[i].a_type = A_LONG;
			switch(argv[i].a_type)
			{
				case A_LONG:
				x->outList[i].a_w.w_long = argv[i].a_w.w_long;
				break;
			
				case A_FLOAT:
				x->outList[i].a_w.w_long = (long)argv[i].a_w.w_float;
				break;
				
				case A_SYM:
				if(isHex(argv[i].a_w.w_sym))  // convert hex 
				{
					x->outList[i].a_w.w_long = convertHex(argv[i].a_w.w_sym);
				}
				else  // pass symbols through
				{
					x->outList[i].a_type = A_SYM;
					x->outList[i].a_w.w_sym = argv[i].a_w.w_sym;
				}
			}
		}
		
	x->listsize = argc;
	}
	llong_dumpList(x);
	
}

void llong_dumpList(x)
Llong *x;
{
	
	if(x->listsize == 1)
		outlet_int(x->outL,x->outList[0].a_w.w_long);
	else
		outlet_list(x->outL,0L,(short)x->listsize,x->outList);
	
}

void  llong_setConstant(x,n)  // crippled code
Llong *x;
long n;
{
	outlet_int(x->outL,n);
}

void  llong_floatConstant(x,n)
Llong *x;
double n;
{
	outlet_int(x->outL,(long)n);
}




void llong_assist(x,b,msg,arg,dst)
Llong *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		
			strcpy(dst,"Data in");
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Data converted to int");
	}
}


void  *llong_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Llong  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Llong *)newobject(class);
	x->outL = outlet_new(x,0L);
	
	return(x);
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

void llong_free(x)
Llong *x;
{
	
	
	

}