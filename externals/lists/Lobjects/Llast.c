// llast file for Max object
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

typedef struct llast
{
	struct object l_ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	int putItHere;
	int lastspot;
	void *outL;
} Llast;


void *llast_new();
void llast_free();
void llast_List();
void llast_howMany();
void llast_dumpList();
void llast_clear();
void llast_assist();
void llast_constant();
void llast_Fconstant();
void llast_symbol();
void llast_sym();
void llast_anything();
void lsetlength();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)llast_new,(method)llast_free,(short)sizeof(struct llast),0L,A_DEFLONG,0);
	addint((method)llast_constant);
	addfloat((method)llast_Fconstant);
	addinx((method)lsetlength, 1);
	addbang((method)llast_dumpList);
	addmess((method)llast_symbol,"symbol", A_GIMME,0);
	addmess((method)llast_anything,"anything", A_GIMME,0);
	addmess((method)llast_dumpList,"dump",0);
	addmess((method)llast_List, "list",A_GIMME,0);
	addmess((method)llast_clear, "clear",0);
	addmess((method)llast_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Llast");
	rescopy('STR#', 30023);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void llast_clear(x)
Llast *x;
{
	
	int i;
	
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->putItHere = -1;

	
	
}

void llast_sym(x,s)
Llast *x;
Symbol *s;
{
	int i;
	
	
	
	if(x->putItHere < x->lastspot)
		{
		x->outList[++x->putItHere].a_type = A_SYM;
		x->outList[x->putItHere].a_w.w_sym = s;
		if(x->putItHere == x->lastspot)
			llast_dumpList(x);
		}
	else
		{
		for(i = 0; i < x->lastspot; ++i)
			{
			x->outList[i].a_type = x->outList[i+1].a_type;
			x->outList[i].a_w = x->outList[i+1].a_w;
			}
		x->outList[x->putItHere].a_type = A_SYM;
		x->outList[x->putItHere].a_w.w_sym = s;
		llast_dumpList(x);
		}	
	
}


void llast_constant(x,n)
Llast *x;
long n;
{
	int i;
	

	if(x->putItHere < x->lastspot)
		{
		x->outList[++x->putItHere].a_type = A_LONG;
		x->outList[x->putItHere].a_w.w_long = n;
		if(x->putItHere == x->lastspot)
			llast_dumpList(x);
		}
	else
		{
		for(i = 0; i < x->lastspot; ++i)
			{
			x->outList[i].a_type = x->outList[i+1].a_type;
			x->outList[i].a_w = x->outList[i+1].a_w;
			}
		x->outList[x->putItHere].a_type = A_LONG;
		x->outList[x->putItHere].a_w.w_long = n;
		llast_dumpList(x);
		}
	
	
}

void llast_Fconstant(x,n)
Llast *x;
double n;
{
	int i;
	
	
	if(x->putItHere < x->lastspot)
		{
		x->outList[++x->putItHere].a_type = A_FLOAT;
		x->outList[x->putItHere].a_w.w_float = n;
		if(x->putItHere == x->lastspot)
			llast_dumpList(x);
		}
	else
		{
		for(i = 0; i < x->lastspot; ++i)
			{
			x->outList[i].a_type = x->outList[i+1].a_type;
			x->outList[i].a_w = x->outList[i+1].a_w;
			}
		x->outList[x->putItHere].a_type = A_FLOAT;
		x->outList[x->putItHere].a_w.w_float = n;
		llast_dumpList(x);
		}
		
	
}

void llast_List(x,s,argc,argv)
Llast *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	for(i=0;i < argc;++i)
		{
		if(argv[i].a_type ==A_FLOAT)
			llast_Fconstant(x,argv[i].a_w.w_float);
		if(argv[i].a_type ==A_LONG)
			llast_constant(x,argv[i].a_w.w_long);
		if(argv[i].a_type ==A_SYM)
			llast_sym(x,argv[i].a_w.w_sym);
		}
	
	
}

void llast_symbol(x,s,argc,argv)
Llast *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	if(argc > 1)
		llast_List(x,s,argc,argv);
	else
		llast_sym(x,argv[0].a_w.w_sym);
	
}

void llast_anything(x,s,argc,argv)
Llast *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	if(argc >= MAXSIZE) argc = MAXSIZE-1;
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
		tempList[i +1] = argv[i];
	llast_List(x,s,argc + 1,tempList);
}


void  lsetlength(x,n)
Llast *x;
long n;
{
	int i, difference;
	
	if(n > 0 && n < MAXSIZE)
	{
	if((n < x->outsize) && ( n <= x->putItHere))
		{
			difference = x->outsize -n;
			for(i=0;i < n; ++i)
			{
				x->outList[i].a_type = x->outList[i+difference].a_type;
				x->outList[i].a_w = x->outList[i+difference].a_w;
			}
		x->putItHere = n-1;
		}
	
	x->outsize = n;
	x->lastspot = n-1;
	}
	
}

void llast_dumpList(x)
Llast *x;
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


void llast_assist(x,b,msg,arg,dst)
Llast *x;
void *b;
long  msg,arg;
char *dst;
{
	int j;
	char theString[32];
	j = 0;
	theString[j++] = 'L';
	theString[j++] = 'a';
	theString[j++] = 's';
	theString[j++] = 't';
	theString[j++] = ' ';
	
	
	if(x->outsize / 100) theString[j++] = (x->outsize % 1000) / 100 +48; 
	if(x->outsize / 10) theString[j++] = (x->outsize % 100 ) / 10 +48; 
	theString[j++] = x->outsize % 10 +48; 
	theString[j++] = ' ';
	theString[j++] = 'i';
	theString[j++] = 't';
	theString[j++] = 'e';
	theString[j++] = 'm';
	theString[j++] = 's';
	
	theString[j++] = 0;
	
	
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to gather");
			break;
			
			default:
			strcpy(dst,"Number of items to report");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,theString);
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}

void  *llast_new(arg)
long arg;
{

	Llast  *x;
	int i;
	
	
	x = (Llast *)newobject(class);
	intin(x,1);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	
	x->outsize  =  MAXSIZE;
	x->putItHere = -1;
	
	if(arg == 0)
		x->outsize = 12;
	else
		x->outsize = arg;
		
	x->lastspot = x->outsize - 1;
	
	return(x);
}

void llast_free(x)
Llast *x;
{
	
	

}