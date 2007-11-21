// lin file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1997, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lin
{
	struct object l_ob;
	long dummy;
	Atom outList[MAXSIZE];
	Atom nada[1];
	int outsize;
	int putItHere;
	int valuemark;
	int holdOff;
	double fvaluemark;
	int marktype, initmark;
	void *symbolmark;
	void *outL;
	void *outR;
} Lin;


void *lin_new();
void lin_free();
void  lin_List();
void  lin_howMany();
void  lin_markon();
void  lin_markoff();
void lin_dumpList();
void lin_clear();
void lin_assist();
void lin_constant();
void lin_Fconstant();
void lin_symbol();
void lin_sym();
void  lin_setlength();
void  lin_backSpace();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lin_new,(method)lin_free,(short)sizeof(struct lin),0L,A_GIMME,0);
	addint((method)lin_constant);
	addfloat((method)lin_Fconstant);
	addinx((method)lin_setlength, 1);
	addbang((method)lin_dumpList);
	addmess((method)lin_symbol,"symbol", A_GIMME,0);
	addmess((method)lin_dumpList,"dump",0);
	addmess((method)lin_List, "list",A_GIMME,0);
	addmess((method)lin_markon, "markon",A_GIMME,0);
	addmess((method)lin_markoff, "markoff",0);
	addmess((method)lin_clear, "clear",0);
	addmess((method)lin_backSpace, "back",0);
	addmess((method)lin_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lin_clear(x)
Lin *x;
{
	
	int i;
	
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->putItHere = 0;
	outlet_list(x->outR,0L,1,x->nada);
	
	
}

void lin_sym(x,s)
Lin *x;
Symbol *s;
{
	
	
	
	x->outList[x->putItHere].a_type = A_SYM;
	x->outList[x->putItHere++].a_w.w_sym = s;
	if(!x->holdOff)
		outlet_list(x->outR,0L,(short)x->putItHere,x->outList);
	if(x->putItHere == x->outsize)
			lin_dumpList(x);
	else
		if((x->marktype == A_SYM) && (s == x->symbolmark))
			lin_dumpList(x);
	
	
}


void lin_constant(x,n)
Lin *x;
long n;
{
	x->outList[x->putItHere].a_type = A_LONG;
	x->outList[x->putItHere++].a_w.w_long = n;
	//if(!x->holdOff)
	if(!((x->marktype == A_LONG) && (n == x->valuemark)))
		outlet_list(x->outR,0L,(short)x->putItHere,x->outList);
	
	if(x->putItHere == x->outsize)
			lin_dumpList(x);
	else
		if((x->marktype == A_LONG) && (n == x->valuemark))
			lin_dumpList(x);
}

void lin_Fconstant(x,n)
Lin *x;
double n;
{
	x->outList[x->putItHere].a_type = A_FLOAT;
	x->outList[x->putItHere++].a_w.w_float = n;
	if(!x->holdOff)
		outlet_list(x->outR,0L,(short)x->putItHere,x->outList);
	if(x->putItHere == x->outsize)
			lin_dumpList(x);
	else
		if((x->marktype == A_FLOAT) && (n == x->fvaluemark))
			lin_dumpList(x);
}

void lin_List(x,s,argc,argv)
Lin *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	x->holdOff = 1;
	for(i=0;i < argc;++i)
		{
		if(argv[i].a_type ==A_FLOAT)
			lin_Fconstant(x,argv[i].a_w.w_float);
		if(argv[i].a_type ==A_LONG)
			lin_constant(x,argv[i].a_w.w_long);
		if(argv[i].a_type ==A_SYM)
			lin_sym(x,argv[i].a_w.w_sym);
		}
	x->holdOff = 0;
	outlet_list(x->outR,0L,(short)x->putItHere,x->outList);
	if(x->putItHere >= x->outsize)
		{
		lin_dumpList(x);
		}
	
}

void lin_symbol(x,s,argc,argv)
Lin *x;
Symbol *s;
int argc;
Atom *argv;
{	
	if(argc > 1)
		lin_List(x,s,argc,argv);
	else
		lin_sym(x,argv[0].a_w.w_sym);
	
}

void  lin_markon(x,s,argc,argv)
Lin *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(argc == 0)
	x->marktype = x->initmark;
	else
	{
		switch(argv[0].a_type)
			{
			case A_LONG:
			x->valuemark = argv[0].a_w.w_long;
			x->marktype = A_LONG;
			break;
			
			case A_FLOAT:
			x->fvaluemark = argv[0].a_w.w_float;
			x->marktype = A_FLOAT;
			break;
			
			case A_SYM:
			x->symbolmark = argv[0].a_w.w_sym;
			x->marktype = A_SYM;
			break;
			}
		x->initmark = x->marktype;
	}
	
}

void  lin_markoff(x)
Lin *x;
{
	x->marktype = A_NOTHING;
}

void  lin_backSpace(x)
Lin *x;
{
	if(x->putItHere)
	{
		-- x->putItHere;
		if(x->putItHere)
			outlet_list(x->outR,0L,(short)x->putItHere,x->outList);
		else
			outlet_list(x->outR,0L,1,x->nada);
	}
	else
		outlet_list(x->outR,0L,1,x->nada);
}

void  lin_setlength(x,n)
Lin *x;
long n;
{
	if(n <= x->putItHere)
		lin_dumpList(x);
	if(n < 1) 
		n = MAXSIZE;
	if(n> MAXSIZE) 
		n = MAXSIZE;
	x->outsize = n;
}

void lin_dumpList(x)
Lin *x;
{
	if(x->putItHere)
	{
		outlet_bang(x->outR);
		if(x->putItHere > 1)
			outlet_list(x->outL,0L,(short)x->putItHere,x->outList);
		else
			switch(x->outList[0].a_type)
			{
			case A_LONG:
			outlet_int(x->outL,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT:
			outlet_float(x->outL,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM:
			outlet_list(x->outL,0L,1,x->outList);
			break;
			}
	}
	x->putItHere = 0;
}


void lin_assist(x,b,msg,arg,dst)
Lin *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to gather and display");
			break;
			
			case 1:
			strcpy(dst,"Length to break list.");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			switch (arg)
		{
			case 0:
			strcpy(dst,"Output when list is full.");
			break;
			
			case 1:
			strcpy(dst,"Incremental output lists.");
			break;
		}
	}
}


void  *lin_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{
	Lin  *x;
	int i;
	x = (Lin *)newobject(class);
	intin(x,1);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	for(i = 0; i< MAXSIZE;++i)
	{
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	x->outsize  =  MAXSIZE;
	x->putItHere = 0;
	x->marktype = x->initmark = A_NOTHING;
	x->valuemark = 0;
	x->fvaluemark = 0;
	x->symbolmark = NULL;
	x->nada[0].a_type = A_SYM;
	x->nada[0].a_w.w_sym = gensym("");
	x->holdOff = 0;
	switch(argc)
	{
		case 0:
		break;
		
		case 1:
			switch(argv[0].a_type)
			{
				case A_LONG:
				if((argv[0].a_w.w_long >MAXSIZE)|| (argv[0].a_w.w_long < 1)) break;
				x->outsize = argv[0].a_w.w_long;
				break;
				
				case A_FLOAT:
				x->initmark = A_FLOAT;
				x->marktype = A_FLOAT;
				x->fvaluemark = argv[0].a_w.w_float;
				break;
				
				case A_SYM:
				x->initmark = A_SYM;
				x->marktype = A_SYM;
				x->symbolmark = argv[0].a_w.w_sym;
				break;
			}
		break;
		
		default:
			switch(argv[0].a_type)
			{
				case A_LONG:
				x->initmark = A_LONG;
				x->marktype = A_LONG;
				x->valuemark = argv[0].a_w.w_long;
				break;
				
				case A_FLOAT:
				x->initmark = A_FLOAT;
				x->marktype = A_FLOAT;
				x->fvaluemark = argv[0].a_w.w_float;
				break;
				
				case A_SYM:
				x->initmark = A_SYM;
				x->marktype = A_SYM;
				x->symbolmark = argv[0].a_w.w_sym;
				break;
			}
			if(argv[1].a_type== A_LONG)
			{
				if((argv[1].a_w.w_long >MAXSIZE)|| (argv[1].a_w.w_long < 1)) break;
				x->outsize = argv[1].a_w.w_long;
			}
	}
	
	return(x);
}

void lin_free(x)
Lin *x;
{
	
}