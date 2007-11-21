// bit file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and bitice   
// appear in all subsequent copies and documentation.

//This File is screwed!! do not compile

#include "ext.h"
#include <string.h>
#include <math.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct bit
{
	struct object ob;
	void *proxy;
	long inletNumber;
	long listsize;
	Atom theList[MAXSIZE];
	int numOuts;
	void *out[32];
	int whichBit[32];
} Bit;

void *bit_new();
void bit_free();
void  bit_doInt();
void  bit_doList();
void bit_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)bit_new,(method)bit_free,(short)sizeof(struct bit),0L,A_GIMME,0);
	addint((method)bit_doInt);
	addmess((method)bit_doList, "list",A_GIMME,0);
	addmess((method)bit_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}




void  bit_doInt(x,n)
Bit *x;
long n;
{
	int i;
	for(i = 0; i < x->listsize; ++ i){
		if(n &x->whichBit[i])
			outlet_int(x->out[i], 1) ;
		else
			outlet_int(x->out[i], 0) ;
			}
}

void  bit_doList(x,s,argc,argv)
Bit *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	int i;
	if(argc > x->listsize) argc = x->listsize;
	
		for( i = 0; i < x->listsize; ++i)
		{
			if(argv[i].a_type == A_LONG)
				x->theList[i].a_w.w_long = argv[i].a_w.w_long;
			x->whichBit[x->listsize - i -1] = pow(2,argv[i].a_w.w_long);
			
		}
}



void bit_assist(x,b,msg,arg,dst)
Bit *x;
void *b;
long  msg,arg;
char *dst;
{
	
	char theString[32] = "Bit   \0";
	if(arg > 9){
		theString[4] = (x->theList[arg].a_w.w_long / 10) + 48;
		theString[5] = (x->theList[arg].a_w.w_long % 10) + 48;
	}
	else
		theString[4] = x->theList[arg].a_w.w_long + 48;
		
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Int");
			break;
			
			default:
			strcpy(dst,"set lsb to start with");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			
			
			default:
			strcpy(dst,theString);
			break;
		}
	}
}


void  *bit_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Bit  *x;
	int i, j;
	
	if(ac > 32) ac = 32;
	
	x = (Bit *)newobject(class);
	if(ac){
		j = 0;
		for(i = 0; i < ac; ++i)
		{
			x->theList[i].a_type = A_LONG;
			if(theArgs[i].a_type == A_LONG)
				x->theList[i].a_w.w_long = theArgs[i].a_w.w_long;
			else
				x->theList[i].a_w.w_long = 0;
		}
		
		x->listsize = ac;
	}
	else{
		for(i = 0; i < 8; ++i)
		{
			x->theList[i].a_type = A_LONG;
			x->theList[7-i].a_w.w_long = i;
			
		}
		x->listsize = 8;
	
	}
	for(i = 0 ; i < x->listsize ; ++ i){
		
		x->out[i]= outlet_new(x,0L);  // this will reverse the outlets so bit[0] is the rightmost
		x->whichBit[x->listsize -i -1] = pow(2,x->theList[i].a_w.w_long);
	}
	return(x);
}

void bit_free(x)
Bit *x;
{
}

/*int check_inlet(x)
Bit *x;
{
	return x->inletNumber;  // classic only
//	return proxy_getinlet((t_object *)x);  // OS X only
}*/