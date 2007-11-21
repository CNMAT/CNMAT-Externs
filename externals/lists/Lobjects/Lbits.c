// lbits file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and bitice   
// appear in all subsequent copies and documentation.



#include "ext.h"
#include <string.h>
#include <math.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lbits
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outList[MAXSIZE];
	int outsize;
	void *outL;
	int bitMask[32];
} Lbits;

void *lbits_new();
void lbits_free();
void  lbits_doInt();
void dumpList();
void  lbits_doList();
void lbits_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbits_new,(method)lbits_free,(short)sizeof(struct lbits),0L,A_GIMME,0);
	addint((method)lbits_doInt);
	//addmess((method)lbits_doList, "list",A_GIMME,0);
	addmess((method)lbits_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}




void  lbits_doInt(x,n)
Lbits *x;
long n;
{
	int i;
	if(check_inlet(x)){
		x->outsize = n;
		for(i = 0 ; i < x->outsize ; ++ i){
			x->bitMask[x->outsize -i -1] = pow(2,i);
			x->outList[i].a_type = A_LONG;
		}
	}else{
		for(i = 0; i < x->outsize; ++ i){
			if(n & x->bitMask[i])
				x->outList[i].a_w.w_long = 1;
			else
				x->outList[i].a_w.w_long = 0 ;
				}
		dumpList(x);
	}
}

void dumpList(x)
Lbits *x;
{
	
	if(x->outsize == 1)
	{
		switch(x->outList[0].a_type) {
			case A_LONG:
			outlet_int(x->outL,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT :
			outlet_float(x->outL,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM :
			outlet_anything(x->outL,x->outList[0].a_w.w_sym, 0, 0);
			break;
		}
	}else
		if(x->outList[0].a_type == A_SYM)
			outlet_anything(x->outL,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
		else
			outlet_list(x->outL,0L,x->outsize,x->outList);
}



void lbits_assist(x,b,msg,arg,dst)
Lbits *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
		
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Int");
			break;
			
			default:
			strcpy(dst,"number of bits to show");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			default:
			strcpy(dst,"list of bits, msb first");
			break;
		}
	}
}


void  *lbits_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lbits  *x;
	int i;
	
	if(ac > 32) ac = 32;
	
	x = (Lbits *)newobject(class);
	x->outL= outlet_new(x,0L);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	
	if(ac && theArgs[0].a_type == A_LONG){
		x->outsize = theArgs[0].a_w.w_long;
	}
	else{
		x->outsize = 8;
	}
	
	for(i = 0 ; i < x->outsize ; ++ i){
		x->bitMask[x->outsize -i -1] = pow(2,i);
		x->outList[i].a_type = A_LONG;
	}
	return(x);
}

void lbits_free(x)
Lbits *x;
{
	freeobject(x->proxy);
}

int check_inlet(x)
Lbits *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}