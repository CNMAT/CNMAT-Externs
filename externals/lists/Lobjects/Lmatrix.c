// lmatrix file for Max object
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


typedef struct lmatrix
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	long width;
	long height;
	Atom *arrayBase;
	Atom zero;
	long memoryused;
	void *outR;
	void *out;
} Lmatrix;


void *lmatrix_new();
void lmatrix_free();
void  lmatrix_storeList();
void  lmatrix_storeColumn();
void lmatrix_setlist();
void  lmatrix_getList();
void  lmatrix_doConstant();
void  lmatrix_setMember();
void  lmatrix_getMember();
void  lmatrix_floatConstant();
void lmatrix_dumpList();
void  lmatrix_clearList();
void lmatrix_assist();
int check_inlet();
void *Lmatrix_class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&Lmatrix_class,(method)lmatrix_new,(method)lmatrix_free,(short)sizeof(struct lmatrix),0L,A_GIMME,0);
	addint((method)lmatrix_doConstant);
	addfloat((method)lmatrix_floatConstant);
	addbang((method)lmatrix_dumpList);
	addmess((method)lmatrix_storeList, "list",A_GIMME,0);
	addmess((method)lmatrix_storeColumn, "put",A_GIMME,0);
	addmess((method)lmatrix_getList, "getRow",A_DEFLONG,0);
	addmess((method)lmatrix_doConstant, "getColumn",A_DEFLONG,0);
	addmess((method)lmatrix_setMember, "set",A_GIMME,0);
	addmess((method)lmatrix_getMember, "get",A_GIMME,0);
	addmess((method)lmatrix_assist, "assist",A_CANT,0);
	addmess((method)lmatrix_clearList, "clear",0);
	finder_addclass("Lists","Lmatrix");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	
}

void lmatrix_storeList(x,s,argc,argv) // first arg is row
Lmatrix *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,slot,start;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  slot = argv[0].a_w.w_long;
	if(slot < 0) slot = 0;
	if(slot < x->height) 
	{
		start = slot * x->width;
		for(i=0;i< x->width;++i)		// put list in array - it had better contain longs
			if(i < (argc -1) )
				x->arrayBase[start + i] = argv[i+1];
			else
				x->arrayBase[start + i] = x->zero;
			
	}
	
}

void lmatrix_storeColumn(x,s,argc,argv)  // first arg is column
Lmatrix *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,column;
	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	  column = argv[0].a_w.w_long;
	if(column < 0) column = 0;
	if(column < x->width) 
	{
		
		for(i=0;i< x->height;++i)		// put list in array - it had better contain longs
			{
			if(i < (argc-1) )
				x->arrayBase[column + i*x->width] = argv[i+1];
			else
				x->arrayBase[column + i*x->width] = x->zero;
			}
	}
	
}

void lmatrix_setMember(x,s,argc,argv)  // first is row (X)
Lmatrix *x;
Symbol *s;
int argc;
Atom *argv;
{
	long row,column;
	if(argc > 2 )
	{	
		row = argv[0].a_w.w_long;
		column = argv[1].a_w.w_long;
		if((column >= 0) && (column < x->width))
		if(( row >=0) &&( row < x->height))
		x->arrayBase[row * x->width + column] = argv[2];
		
	
	}

	
}

void lmatrix_getMember(x,s,argc,argv) // first is row (X)
Lmatrix *x;
Symbol *s;
int argc;
Atom *argv;
{
	long row,column;
	if(argc > 1 )
	{
		row = argv[0].a_w.w_long;
		column = argv[1].a_w.w_long;
		if((column >= 0) && (column < x->width))
		if(( row >=0) &&( row < x->height))
		switch(x->arrayBase[row * x->width + column].a_type){
			case A_LONG:
			outlet_int(x->out,x->arrayBase[row * x->width + column].a_w.w_long);
			break;
			
			case A_FLOAT:
			outlet_float(x->out,x->arrayBase[row * x->width + column].a_w.w_float);
			break;
			
			case A_SYM:
			outlet_anything(x->out,x->arrayBase[row * x->width + column].a_w.w_sym,0,NIL);
			break;
			
		}
	}

	
}



void  lmatrix_getList(x,n)  // get row  n = Y
Lmatrix *x;
long n;
{
	int start,i;
	
	
	if(n >= 0 && n < x->height)
	{
		start = n * x->width;
		if(check_inlet(x) == 0)		// constant is in left inlet
			for(i = 0; i < x->width; ++i)
			{
				x->outList[i] = x->arrayBase[start + i];
				
			}
		x->outsize = x->width;
		outlet_int(x->outR,n);
		lmatrix_dumpList(x);
	}
	x->inletNumber = 0;
	
}

void  lmatrix_doConstant(x,n)  // get column n = X
Lmatrix *x;
long n;
{
	int start,i;
	
	
	if(n >= 0 && n < x->width)
	{
		start = 0;
		if(check_inlet(x) == 0)		// constant is in left inlet
			for(i = 0; i < x->height; ++i)
			{
				x->outList[i] = x->arrayBase[start + n];
				start += x->width;
			}
		x->outsize = x->height;
		outlet_int(x->outR,n);
		lmatrix_dumpList(x);
	}
	x->inletNumber = 0;
	
}

void  lmatrix_floatConstant(x,n)
Lmatrix *x;
float n;
{
	
	
	lmatrix_doConstant(x,(long)n);
	
	
	
}

void lmatrix_dumpList(x)
Lmatrix *x;
{
	
	if(x->outsize == 1)
			{
			switch(x->outList[0].a_type){
			case A_LONG:
				outlet_int(x->out,x->outList[0].a_w.w_long);
			break;
			
			case A_FLOAT:
				outlet_float(x->out,x->outList[0].a_w.w_float);
			break;
			
			case A_SYM:
				outlet_anything(x->out,x->outList[0].a_w.w_sym,0,NIL);
			break;
			}
		}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
	
}

void  lmatrix_clearList(x)
Lmatrix *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->outList[i].a_type = A_LONG;
		x->outList[i].a_w.w_long = 0;
		}
	for(i = 0; i < x->width * x->height; ++ i)
		x->arrayBase[i] = x->zero;
	
	x->outsize = x->width;
	
}

void lmatrix_assist(x,b,msg,arg,dst)
Lmatrix *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List is stored, int gets list of nth members");
			break;
			
			default:
			strcpy(dst,"Nada");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List");
			break;
			
			default:
			strcpy(dst,"row or column number");
			break;
		}
	}
	
	
}


void  *lmatrix_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lmatrix  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lmatrix *)newobject(Lmatrix_class);
	//x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	x->height =  16;
	x->width = 128;
	x->zero.a_type = A_LONG;
	x->zero.a_w.w_long = 0;
		
	if(ac > 1 )
	{
		if(theArgs[0].a_type == A_LONG)
			x->height = theArgs[0].a_w.w_long;
		if(theArgs[1].a_type == A_LONG)
			x->width = theArgs[1].a_w.w_long;
	}
	if(x->height > MAXSIZE) x->height = MAXSIZE;
	if(x->width > MAXSIZE) x->width = MAXSIZE;
	x->memoryused = x->width * x->height * sizeof(Atom);
	if(x->memoryused > 32767){
		x->height = 4095 / x->width;
		 post("Lmatrix: array too large -- reduced to %ld by %ld",x->height,x->width);
		x->memoryused = 32767;
	}
	
	x->arrayBase = (Atom *)getbytes(x->memoryused);
	lmatrix_clearList(x);
	x->outsize = 0;
	
	return(x);
}

void lmatrix_free(x)
Lmatrix *x;
{
	
	//freeobject(x->proxy);
	if(x->arrayBase)
		freebytes((char *)x->arrayBase,x->memoryused);
	

}

int check_inlet(x)
Lmatrix *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}