// lsx file for Max object
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

typedef struct lsx
{
	struct object ob;
	
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	Symbol *names[MAXSIZE];
	int listsize;
	int oldListSize;
	int numNames;
	long nextValue;
	int dataPending;
	int loHigh;
	int nibbleSize;
	int nibbleMask;
	int maxBytes;
	void *outL;
	Symbol *messageType;
} Lsx;


void *lsx_new();
void lsx_free();
void  lsx_Lists();
void  lsx_setConstant();
void  lsx_floatConstant();
void lsx_anything();
void lsx_stuffvalue();
void lsx_bang();
void lsx_dumpList();
void lsx_hiLo();
void lsx_loHi();
void lsx_nibble();
void lsx_clear();
void lsx_assist();
long isInNames();
long convertHex();
int isHex();
int charHex();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsx_new,(method)lsx_free,(short)sizeof(struct lsx),0L,A_GIMME,0);
	addint((method)lsx_setConstant);
	addfloat((method)lsx_floatConstant);
	addbang((method)lsx_dumpList);
	addmess((method)lsx_Lists, "list",A_GIMME,0);
	
	//addmess((method)lsx_setList, "set",A_GIMME,0);
	addmess((method)lsx_assist, "assist",A_CANT,0);
	addmess((method)lsx_clear, "clear",0);
	addmess((method)lsx_loHi, "loHi",0);
	addmess((method)lsx_hiLo, "hiLo",0);
	addmess((method)lsx_nibble, "nibble",A_DEFLONG,0);
	addmess((method)lsx_anything, "anything",A_GIMME,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}


void lsx_Lists(x,s,argc,argv)
Lsx *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	for(i = 0; i < argc; ++i){
		if(i == x->numNames) return;
		lsx_stuffvalue(x,x->names[i],1,&argv[i]);
	}
}

void lsx_anything(x,s,argc,argv)
Lsx *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom temp;
	int i;
	if(argc){
		temp = argv[0];
		lsx_stuffvalue(x,s,1,&temp);
		
		for(i= 1; i< argc -1;++i)
			if(argv[i].a_type == A_SYM){
			temp = argv[i + 1];
			lsx_stuffvalue(x,argv[i].a_w.w_sym,1,&temp);
			}
	}
}

void lsx_stuffvalue(x,s,argc,argv)
Lsx *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,n,dataleft;
	n = 0;
	switch(argv[0].a_type){
		case A_LONG :
		n = argv[0].a_w.w_long;
		break;
		
		case  A_FLOAT :
			n =(long) argv[0].a_w.w_float;
			break;
		case A_SYM :
		if(isHex(argv[0].a_w.w_sym))  // convert hex in both lists to speed up search
			n = convertHex(argv[0].a_w.w_sym);
		else  
			n  = 0;
		break;
	}
	dataleft = x->maxBytes;
	if(x->loHigh)
	{
		for(i = 0; i < x->listsize;++i)
		{
			if(x->theList[i].a_type == A_SYM && x->theList[i].a_w.w_sym == s && dataleft)
			{
				x->outList[i].a_w.w_long = n & x->nibbleMask;
				n >>= x->nibbleSize;
				-- dataleft;
			}
		}
	}
	else
	{
		for(i = x->listsize-1; i >=0 ;--i)
		{
			if(x->theList[i].a_type == A_SYM && x->theList[i].a_w.w_sym == s && dataleft)
			{
				x->outList[i].a_w.w_long = n & x->nibbleMask;
				n >>= x->nibbleSize;
				-- dataleft;
			}
		}
	}
}

void lsx_dumpList(x)
Lsx *x;
{
	int i;
	
	//outlet_list(x->outL,0L,(short)x->listsize,x->outList);
	for(i = 0;i< x->listsize; ++i)
		outlet_int(x->outL,x->outList[i].a_w.w_long);
	
}

void  lsx_setConstant(x,n)  // crippled code
Lsx *x;
long n;
{
	Atom temp;
	temp.a_type = A_LONG;
	temp.a_w.w_long = n;
	lsx_stuffvalue(x,x->names[0],1,&temp);
	
}

void  lsx_floatConstant(x,n)
Lsx *x;
double n;
{
	Atom temp;
	temp.a_type = A_FLOAT;
	temp.a_w.w_float = n;
	lsx_stuffvalue(x,x->names[0],1,&temp);
}

void lsx_clear(x)
Lsx *x;
{
	int i;
	
for(i=0;i<x->listsize;++i)
	{
		if(x->theList[i].a_type ==A_SYM)
			x->outList[i].a_w.w_long = 0;
	}
}

void lsx_loHi(x)
Lsx *x;
{
	x->loHigh = true;
}

void lsx_hiLo(x)
Lsx *x;
{
	
	x->loHigh = false;
	
	
}

void lsx_nibble(x,n)
Lsx *x;
long n;
{
	
	if(n > 0 && n < 8);
	{
		x->nibbleSize = n;
		x->nibbleMask = (0x01 << n) - 1;
		//x->maxBytes = sizeof(long) / n;
	}
	
	
}


void lsx_assist(x,b,msg,arg,dst)
Lsx *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Member to set, or bang to output");
			break;
			
			default:
			strcpy(dst,"Data");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Data with changed members");
	}
	
	
}


void  *lsx_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lsx  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lsx *)newobject(class);
	x->outL= outlet_new(x,0L);
	x->nextValue = 0;
	x->dataPending = false;
	x->nibbleSize = 7;
	x->nibbleMask = 0x7f;
	x->maxBytes = 4;
	x->loHigh = true;
	x->numNames = 0;
	for(i=0;i<ac;++i)
	{
		x->theList[i] = theArgs[i];
		x->outList[i].a_type = A_LONG;
		switch(x->theList[i].a_type){
			case A_LONG:
			x->outList[i].a_w.w_long = x->theList[i].a_w.w_long;
			break;
			case A_FLOAT:
			x->outList[i].a_w.w_long = (long)x->theList[i].a_w.w_float;
			break;
			case A_SYM :
			if(isHex(x->theList[i].a_w.w_sym))  // convert hex in both lists to speed up search
			{
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = convertHex(x->theList[i].a_w.w_sym);
				x->outList[i].a_w.w_long = x->theList[i].a_w.w_long;
			
			}
			else  
			{
				if(!isInNames(x,x->theList[i].a_w.w_sym)){
					x->names[x->numNames] = x->theList[i].a_w.w_sym;
					++x->numNames;
				}
				x->outList[i].a_w.w_long = 0;// initialize tokens to 0
			}
			break;
		}
	}
		
	x->listsize = ac;

	
	return(x);
}

long isInNames(x,s)
Lsx *x;
Symbol *s;
{
	int i;
	for(i = 0; i < x->numNames; ++i)
		if(s == x->names[i]) return(1);
	return(0);
}

long convertHex(theSymbol)
Symbol *theSymbol;
{
	long result;
	result = charHex(theSymbol->s_name[0]) * 16;
	result += charHex(theSymbol->s_name[1]);
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
	if(theSymbol->s_name[2] == 'h')
	return(1);
	else
	return(0);
}

void lsx_free(x)
Lsx *x;
{
	
}