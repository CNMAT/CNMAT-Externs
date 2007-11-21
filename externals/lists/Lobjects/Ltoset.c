// Ltoset file for Max object
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

typedef struct ltoset
{
	struct object l_ob;
	long dummy;
	Atom l_outList[MAXSIZE];
	int l_outsize;
	long value;
	void *l_outL;
	void *l_outR;
} Ltoset;


void *ltoset_new();
void ltoset_free();
void  ltoset_List();
void  ltoset_howMany();
void  ltoset_value();
void ltoset_dumpList();
void ltoset_assist();
void ltoset_constant();
void ltoset_Fconstant();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)ltoset_new,(method)ltoset_free,(short)sizeof(struct ltoset),0L,A_DEFLONG,A_DEFLONG,0);
	addint((method)ltoset_constant);
	addfloat((method)ltoset_Fconstant);
	addinx((method)ltoset_howMany,2);
	addinx((method)ltoset_value,1);
	addbang((method)ltoset_dumpList);
	addmess((method)ltoset_List, "list",A_GIMME,0);
	addmess((method)ltoset_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Ltoset");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void ltoset_constant(x,n)
Ltoset *x;
long n;
{
	int i;
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->l_outList[i].a_type = A_LONG;
		x->l_outList[i].a_w.w_long = 0;
		}
		if(n < 0) n = 0;
		if(n >= x->l_outsize) n = x->l_outsize-1;
		x->l_outList[(int)n].a_w.w_long = x->value;
		outlet_list(x->l_outL,0L,x->l_outsize,x->l_outList);
}

void ltoset_Fconstant(x,n)
Ltoset *x;
double n;
{	
	int i;
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->l_outList[i].a_type = A_LONG;
		x->l_outList[i].a_w.w_long = 0;
		}
		if(n < 0) n = 0;
		if(n >= x->l_outsize) n = x->l_outsize-1;
		x->l_outList[(int)n].a_w.w_long = x->value;
		outlet_list(x->l_outL,0L,x->l_outsize,x->l_outList);
}

void ltoset_List(x,s,argc,argv)
Ltoset *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;	
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i = 0; i < MAXSIZE; ++i)
	{
		x->l_outList[i].a_type = A_LONG;
		x->l_outList[i].a_w.w_long = 0;
	}
	
	for(i=0;i < argc;++i)
	{
		if(argv[i].a_w.w_long < 0) continue;
		if(argv[i].a_w.w_long >= MAXSIZE) continue;
		x->l_outList[argv[i].a_w.w_long].a_w.w_long = x->value;			
	}
	outlet_list(x->l_outL,0L,x->l_outsize,x->l_outList);
}


void  ltoset_howMany(x,n)
Ltoset *x;
long n;
{

	if(n< 1) n =1;
	if(n >MAXSIZE) n = MAXSIZE;
		x->l_outsize = n;
}

void  ltoset_value(x,n)
Ltoset *x;
long n;
{
		x->value = n;
}

void ltoset_dumpList(x)
Ltoset *x;
{
	if(x->l_outsize)
	outlet_list(x->l_outL,0L,x->l_outsize,x->l_outList);
}


void ltoset_assist(x,b,msg,arg,dst)
Ltoset *x;
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
			strcpy(dst,"Members to set");
			break;
			
			case 1:
			strcpy(dst,"Value to place in set");
			break;
			
			default:
			strcpy(dst,"length of Set");
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

void  *ltoset_new(arg1,arg2)
long arg1,arg2;
{
	Ltoset  *x;
	int i;
	if(arg1 == 0) arg1 = 12;
	if(arg2 == 0) arg2 = 1;
	if(arg1 > MAXSIZE) arg1 = MAXSIZE;
	x = (Ltoset *)newobject(class);
	intin(x,2);
	intin(x,1);
	x->l_outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->l_outList[i].a_type = A_LONG;
	x->l_outList[i].a_w.w_long = 0;
	}
	x->l_outsize  = arg1;
	x->value = arg2;
	
	return(x);
}

void ltoset_free(x)
Ltoset *x;
{
}