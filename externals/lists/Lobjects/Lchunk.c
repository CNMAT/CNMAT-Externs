// lchunk file for Max object
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

typedef struct lchunk
{
	struct object l_ob;
	long dummy;
	Atom outList[MAXSIZE];
	int outsize;
	int putItHere;
	int valuemark;
	double fvaluemark;
	int marktype, initmark;
	int markPoint;
	int wrap;
	void *symbolmark;
	void *outL;
} Lchunk;


void *lchunk_new();
void lchunk_free();
void  lchunk_List();
void  lchunk_howMany();
void  lchunk_markon();
void  lchunk_markoff();
void  lchunk_wrap();
void lchunk_dumpList();
void lchunk_clear();
void lchunk_assist();
void lchunk_constant();
void lchunk_Fconstant();
void lchunk_symbol();
void lchunk_sym();
void  lsetlength();
int check_inlet();
void *class;

int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lchunk_new,(method)lchunk_free,(short)sizeof(struct lchunk),0L,A_GIMME,0);
	addint((method)lchunk_constant);
	addfloat((method)lchunk_Fconstant);
	addinx((method)lsetlength, 1);
	addbang((method)lchunk_dumpList);
	addmess((method)lchunk_symbol,"symbol", A_GIMME,0);
	addmess((method)lchunk_dumpList,"dump",0);
	addmess((method)lchunk_List, "list",A_GIMME,0);
	addmess((method)lchunk_markon, "markon",A_GIMME,0);
	addmess((method)lchunk_markoff, "markoff",0);
	addmess((method)lchunk_wrap, "wrap",A_DEFLONG,0);
	addmess((method)lchunk_clear, "clear",0);
	addmess((method)lchunk_assist, "assist",A_CANT,0);
	finder_addclass("Lists","Lchunk");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;
	}
	return 0;
	
}

void lchunk_clear(x)
Lchunk *x;
{
	
	int i;
	
	
	for(i = 0; i < MAXSIZE; ++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	x->putItHere = 0;

	
	
}

void lchunk_sym(x,s)
Lchunk *x;
Symbol *s;
{
	
	
	
	x->outList[x->putItHere].a_type = A_SYM;
	x->outList[x->putItHere++].a_w.w_sym = s;
	if(x->putItHere == x->outsize)
			lchunk_dumpList(x);
	else
		if((x->marktype == A_SYM) && (s == x->symbolmark))
		{if(x->wrap)
			x->markPoint = x->putItHere;  // note this is one after the mark
		else
			lchunk_dumpList(x);
		}
	
}


void lchunk_constant(x,n)
Lchunk *x;
long n;
{
	

	x->outList[x->putItHere].a_type = A_LONG;
	x->outList[x->putItHere++].a_w.w_long = n;
	if(x->putItHere == x->outsize)
			lchunk_dumpList(x);
	else
		if((x->marktype == A_LONG) && (n == x->valuemark))
		{if(x->wrap)
			x->markPoint = x->putItHere;  // note this is one after the mark
		else
			lchunk_dumpList(x);
		}
	
	
}

void lchunk_Fconstant(x,n)
Lchunk *x;
double n;
{
	

	x->outList[x->putItHere].a_type = A_FLOAT;
	x->outList[x->putItHere++].a_w.w_float = n;
	if(x->putItHere == x->outsize)
			lchunk_dumpList(x);
	else
		if((x->marktype == A_FLOAT) && (n == x->fvaluemark))
		{if(x->wrap)
			x->markPoint = x->putItHere;  // note this is one after the mark
		else
			lchunk_dumpList(x);
		}
		
	
}

void lchunk_List(x,s,argc,argv)
Lchunk *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	for(i=0;i < argc;++i)
		{
		if(argv[i].a_type ==A_FLOAT)
			lchunk_Fconstant(x,argv[i].a_w.w_float);
		if(argv[i].a_type ==A_LONG)
			lchunk_constant(x,argv[i].a_w.w_long);
		if(argv[i].a_type ==A_SYM)
			lchunk_sym(x,argv[i].a_w.w_sym);
		}
	if(!x->wrap)
	if(x->putItHere)  // a short list just falls through, unless wrapping is on
		{
		lchunk_dumpList(x);
		}
	
}

void lchunk_symbol(x,s,argc,argv)
Lchunk *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	
	if(argc > 1)
		lchunk_List(x,s,argc,argv);
	else
		lchunk_sym(x,argv[0].a_w.w_sym);
	
}

void  lchunk_markon(x,s,argc,argv)
Lchunk *x;
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

void  lchunk_markoff(x)
Lchunk *x;
{

	
	
			x->marktype = A_NOTHING;
	
	
}

void  lsetlength(x,n)
Lchunk *x;
long n;
{

	
	if(n < x->putItHere)
		lchunk_dumpList(x);
	if(n < 1) 
		n = MAXSIZE;
	if(n> MAXSIZE) 
		n = MAXSIZE;
	x->outsize = n;
	
	
}

void  lchunk_wrap(x,n)
Lchunk *x;
long n;
{

	
	
	x->wrap = n;
	
	
}

void lchunk_dumpList(x)
Lchunk *x;
{
	int length;
	int i;
	
	if(x->wrap && x->markPoint) {
		length = x->markPoint;
		outlet_list(x->outL,0L,length,x->outList);
		for(i = 0; i < x->putItHere - length; ++i)
			x->outList[i] = x->outList[x->markPoint + i];
		x->putItHere = i;
		x->markPoint =0;
	
	
	}
	else{
	length = x->putItHere;
	x->putItHere = 0;	
		outlet_list(x->outL,0L,length,x->outList);
	}
	
}


void lchunk_assist(x,b,msg,arg,dst)
Lchunk *x;
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
			strcpy(dst,"Length of chunks");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Lists");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
	
}


void  *lchunk_new(s,argc,argv)
Symbol *s;
int argc;
Atom *argv;
{

	Lchunk  *x;
	int i;
	
	
	x = (Lchunk *)newobject(class);
	intin(x,1);
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
	x->markPoint = 0;
	x->wrap = 0;
	
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

void lchunk_free(x)
Lchunk *x;
{
	
	

}