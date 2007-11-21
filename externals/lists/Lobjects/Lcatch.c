// lcatch file for Max object
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
typedef struct lcatch
{
	struct object l_ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	int putItHere;
	long pause;
	void *outL;
	void *m_clock;
} Lcatch;


void *lcatch_new();
void lcatch_free();
void  lcatch_List();
void  lcatch_howMany();
void lcatch_onclock();
void lcatch_dumpList();
void lcatch_clear();
void lcatch_bang();
void lcatch_assist();
void lcatch_constant();
void lcatch_Fconstant();
void lcatch_symbol();
void lcatch_sym();
void setTimeout();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lcatch_new,(method)lcatch_free,(short)sizeof(struct lcatch),0L,A_GIMME,0);
	addint((method)lcatch_constant);
	addfloat((method)lcatch_Fconstant);
	addinx((method)setTimeout, 1);
	addbang((method)lcatch_bang);
	addmess((method)lcatch_symbol,"symbol", A_GIMME,0);
	addmess((method)lcatch_dumpList,"dump",0);
	addmess((method)lcatch_List, "list",A_GIMME,0);
	addmess((method)lcatch_clear, "clear",0);
	addmess((method)lcatch_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lcatch");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lcatch_clear(x)
Lcatch *x;
{
	
	int i;
	
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->putItHere = 0;

	
	
}

void lcatch_sym(x,s)
Lcatch *x;
Symbol *s;
{
	
	
	clock_unset(x->m_clock);	
	x->outList[x->putItHere].a_type = A_SYM;
	x->outList[x->putItHere++].a_w.w_sym = s;
	if(x->putItHere == x->outsize)
			lcatch_dumpList(x);
	clock_delay(x->m_clock,x->pause);
	
}


void lcatch_constant(x,n)
Lcatch *x;
long n;
{
	
	clock_unset(x->m_clock);
	x->outList[x->putItHere].a_type = A_LONG;
	x->outList[x->putItHere++].a_w.w_long = n;
	if(x->putItHere == x->outsize)
			lcatch_dumpList(x);
	clock_delay(x->m_clock,x->pause);
	
}

void lcatch_Fconstant(x,n)
Lcatch *x;
double n;
{
	
	clock_unset(x->m_clock);
	x->outList[x->putItHere].a_type = A_FLOAT;
	x->outList[x->putItHere++].a_w.w_float = n;
	if(x->putItHere == x->outsize)
			lcatch_dumpList(x);
	clock_delay(x->m_clock,x->pause);
	
}

void lcatch_List(x,s,argc,argv)
Lcatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	for(i=0;i < argc;++i)
		{
		if(argv[i].a_type ==A_FLOAT)
			lcatch_Fconstant(x,argv[i].a_w.w_float);
		if(argv[i].a_type ==A_LONG)
			lcatch_constant(x,argv[i].a_w.w_long);
		if(argv[i].a_type ==A_SYM)
			lcatch_sym(x,argv[i].a_w.w_sym);
		}

	
}

void lcatch_symbol(x,s,argc,argv)
Lcatch *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	if(argc > 1)
		lcatch_List(x,s,argc,argv);
	else
		lcatch_sym(x,argv[0].a_w.w_sym);
	
}


void  setTimeout(x,n)
Lcatch *x;
long n;
{

	
	if(n < 1) 
		n = 10;
	x->pause = n;
	
	
}

void lcatch_bang(x)
Lcatch *x;
{
	
		clock_unset(x->m_clock);
		outlet_list(x->outL,0L,x->putItHere,x->outList);
		lcatch_clear(x);
	
}

void lcatch_dumpList(x)
Lcatch *x;
{
	
		outlet_list(x->outL,0L,x->putItHere,x->outList);
		lcatch_clear(x);
	
}

void lcatch_onclock(x)
Lcatch *x;
{

		outlet_list(x->outL,0L,x->putItHere,x->outList);
		x->putItHere = 0;
}

void lcatch_assist(x,b,msg,arg,dst)
Lcatch *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to gather");
			break;
			
			default:
			strcpy(dst,"Timeout time");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Lists demarcated by timeout");//forgot something
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *lcatch_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lcatch  *x;
	int i;
	
	
	x = (Lcatch *)newobject(class);
	x->m_clock = clock_new(x,(method)lcatch_onclock);
	intin(x,1);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	
	x->outsize  =  MAXSIZE;
	x->putItHere = 0;
	x->pause = 10;
	
	switch(argc)
	{
		case 0:
		break;
		
		case 1:
		
			if(argv[0].a_type == A_LONG)
				{
				if(argv[0].a_w.w_long < 1) break;
				x->pause = argv[0].a_w.w_long;
				}
		break;
		
		default:
			if(argv[0].a_type == A_LONG)
				{
				if(argv[0].a_w.w_long < 1) break;
				x->pause = argv[0].a_w.w_long;
				}
			if(argv[1].a_type== A_LONG)
				{
				if((argv[1].a_w.w_long >MAXSIZE)|| (argv[1].a_w.w_long < 1)) break;
				x->outsize = argv[1].a_w.w_long;
				}

	}
	
	return(x);
}

void lcatch_free(x)
Lcatch *x;
{
	
	

}