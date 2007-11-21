// Lbuildset file for Max object
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

typedef struct lbuildset
{
	struct object ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	long value;
	void *outL;
	void *outR;
} Lbuildset;


void *lbuildset_new();
void lbuildset_free();
void  lbuildset_List();
void  lbuildset_howMany();
void  lbuildset_value();
void lbuildset_dumpList();
void lbuildset_clear();
void lbuildset_assist();
void lbuildset_constant();
void lbuildset_Fconstant();
int check_inlet();
void *class;

int main(void)
{

	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lbuildset_new,(method)lbuildset_free,(short)sizeof(struct lbuildset),0L,A_DEFLONG,A_DEFLONG,0);
	addint((method)lbuildset_constant);
	addfloat((method)lbuildset_Fconstant);
	addinx((method)lbuildset_howMany,2);
	addinx((method)lbuildset_value,1);
	addbang((method)lbuildset_dumpList);
	addmess((method)lbuildset_dumpList,"dump",0);
	addmess((method)lbuildset_List, "list",A_GIMME,0);
	addmess((method)lbuildset_clear, "clear",0);
	addmess((method)lbuildset_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lbuildset");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lbuildset_clear(x)
Lbuildset *x;
{
	
	int i;
	
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}

	
	
}

void lbuildset_constant(x,n)
Lbuildset *x;
long n;
{
	
	
	
	
		while(n < 0) n += x->outsize;
		while(n >= x->outsize) n -= x->outsize;
		if(x->value)
			x->outList[n].a_w.w_long = x->value;
		else
			++x->outList[n].a_w.w_long;
	
	
}

void lbuildset_Fconstant(x,n)
Lbuildset *x;
double n;
{
	
	
	
	
		while(n < 0) n += x->outsize;
		while(n >= x->outsize) n -= x->outsize;
		if(x->value)
			x->outList[(int)n].a_w.w_long = x->value;
		else
			++x->outList[(int)n].a_w.w_long;
	
	
}

void lbuildset_List(x,s,argc,argv)
Lbuildset *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc > MAXSIZE) argc = MAXSIZE;

	
	for(i=0;i < argc;++i)
		{
		if(argv[i].a_type ==A_FLOAT)
			{
			argv[i].a_type = A_LONG;
			argv[i].a_w.w_long = (long)argv[i].a_w.w_float;
			}
		while(argv[i].a_w.w_long < 0) argv[i].a_w.w_long += x->outsize;
		while(argv[i].a_w.w_long >= x->outsize) argv[i].a_w.w_long -= x->outsize;
		if(x->value)
			x->outList[argv[i].a_w.w_long].a_w.w_long = x->value;
		else
			++x->outList[argv[i].a_w.w_long].a_w.w_long;
			
		}

		//outlet_list(x->outL,0L,x->outsize,x->outList);
	
}


void  lbuildset_howMany(x,n)
Lbuildset *x;
long n;
{

	
	if(n< 1) n =1;
	if(n >MAXSIZE) n = MAXSIZE;
		x->outsize = n;
	
}

void  lbuildset_value(x,n)
Lbuildset *x;
long n;
{

	
		x->value = n;
	
}

void lbuildset_dumpList(x)
Lbuildset *x;
{
	
		if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->outL,x->outList[0].a_w.w_long);
			if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->outL,x->outList[0].a_w.w_float);
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->outL,x->outList[0].a_w.w_sym, 0, 0);
			}
		else
			if(x->outList[0].a_type == A_SYM)
				outlet_anything(x->outL,x->outList[0].a_w.w_sym, x->outsize-1, &x->outList[1]);
			else
				outlet_list(x->outL,0L,x->outsize,x->outList);
			
	
}


void lbuildset_assist(x,b,msg,arg,dst)
Lbuildset *x;
void *b;
long  msg,arg;
char *dst;
{
	int j;
	char theString[32];
	j = 0;
	theString[j++] = 'L';
	theString[j++] = 'i';
	theString[j++] = 's';
	theString[j++] = 't';
	theString[j++] = ' ';
	theString[j++] = 'o';
	theString[j++] = 'f';
	theString[j++] = ' ';
	if(x->value < 0){ theString[j++] = '-';
	if(x->value / 100) theString[j++] = (-x->value % 1000) / 100 +48; 
	if(x->value / 10) theString[j++] = (-x->value % 100 ) / 10 +48; 
	theString[j++] = -x->value % 10 +48;
	}else{
	if(x->value / 100) theString[j++] = (x->value % 1000) / 100 +48; 
	if(x->value / 10) theString[j++] = (x->value % 100 ) / 10 +48; 
	theString[j++] = x->value % 10 +48; 
	}
	theString[j++] = 's';
	theString[j++] = ' ';
	theString[j++] = 'a';
	theString[j++] = 'n';
	theString[j++] = 'd';
	theString[j++] = ' ';
	theString[j++] = '0';
	theString[j++] = 's';
	theString[j++] = 0;
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Positions to set.");
			break;
			
			case 1:
			strcpy(dst,"Value to set.");
			break;
			
			default:
			strcpy(dst,"Length of output list.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,theString);//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *lbuildset_new(arg1,arg2)
long arg1,arg2;
{

	Lbuildset  *x;
	int i;
	
	if(arg1 == 0) arg1 = 12;
	if(arg1 > MAXSIZE) arg1 = MAXSIZE;
	x = (Lbuildset *)newobject(class);
	intin(x,2);
	intin(x,1);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	x->outsize  = arg1;
	x->value = arg2;
	
	
	return(x);
}

void lbuildset_free(x)
Lbuildset *x;
{
}