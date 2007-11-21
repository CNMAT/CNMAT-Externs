// lmerge file for Max object
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

typedef struct lmerge
{
	struct object ob;
	void *proxy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	int listsize;
	int outsize;
	int interleaf;
	int insertsize;
	int useall;
	void *outL;
} Lmerge;


void *lmerge_new();
void lmerge_free();
void  lmerge_mergeLists();
void  lmerge_anything();
void lmerge_setlist();
void  lmerge_interleaf();
void  lmerge_useall();
void  lmerge_setConstant();
void  lmerge_floatConstant();
void lmerge_dumpList();
void  lmerge_clearList();
void lmerge_assist();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lmerge_new,(method)lmerge_free,(short)sizeof(struct lmerge),0L,A_GIMME,0);
	addint((method)lmerge_setConstant);
	addfloat((method)lmerge_floatConstant);
	addmess((method)lmerge_mergeLists,"list",A_GIMME,0);
	addmess((method)lmerge_anything,"anything",A_GIMME,0);
	addmess((method)lmerge_interleaf,"interleaf",A_DEFLONG,A_DEFLONG,0);
	addmess((method)lmerge_useall,"useall",A_DEFLONG,0);
	addmess((method)lmerge_assist, "assist",A_CANT,0);
	addmess((method)lmerge_clearList, "clear",0);
	alias("Lmerge3");
	alias("Lmerge4");
	alias("Lmerge5");
	alias("Lmerge6");
	alias("Lmerge7");
	alias("Lmerge8");
	alias("Lmerge9");
	
	finder_addclass("Lists","Lmerge");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lmerge_mergeLists(x,s,argc,argv)
Lmerge *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j,k,m,r;
	if(check_inlet(x))  // list in right inlet
	{
		if(argc > MAXSIZE) argc = MAXSIZE;
		for(i=0;i<argc;++i)
			{
			x->theList[i] = argv[i];
			}
			
		x->listsize = argc;
	}
	else		// list in left inlet
	{
		j = 0;   //position in outlist
		m = 0;	// position in stored list
		for(i=0;i<argc;)  // step through input
		{
			for(k = 0; k < x->interleaf; ++k)  // some from the input
			{
				x->outList[j++] = argv[i++];
				if(j >= MAXSIZE) goto FINISH;
				if (i >= argc)  break;
			}
			if(( x->listsize == 1) && (j < MAXSIZE)) // and some from the list
			{
				for(r=0;r < x->insertsize; ++r){
				x->outList[j++] = x->theList[0];
				if(j >= MAXSIZE) goto FINISH;
				}
			}
			else
			{
				if((m < x->listsize) && (j < MAXSIZE))
				{
					for(r=0;r < x->insertsize; ++r){
						x->outList[j++] = x->theList[m++];
						if(m >= x->listsize) break;
						if(j >= MAXSIZE)  
							goto FINISH;
					}
				}
			}
			if(j >= MAXSIZE)
				goto FINISH;
		}
	if(x->useall) for( ; m < x->listsize; ++m)
		{
			x->outList[j++] = x->theList[m];
		}
FINISH:		x->outsize = j;
			if(x->outsize > MAXSIZE) x->outsize = MAXSIZE;
			lmerge_dumpList(x);
	}
}

void lmerge_anything(x,s,argc,argv)
Lmerge *x;
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
	lmerge_mergeLists(x,s,argc + 1,tempList);
}

void lmerge_dumpList(x)
Lmerge *x;
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

void  lmerge_clearList(x)
Lmerge *x;
{
	int i;
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 0;
}

void  lmerge_interleaf(x,n,m)
Lmerge *x;
long n;
long m;
{		
	x->interleaf = n;
	x->interleaf &= 0xF;
	if(!x->interleaf) x->interleaf = 1;
	x->insertsize = m;
	x->insertsize &= 0xF;
	if(!x->insertsize) x->insertsize = 1;
	
}

void  lmerge_useall(x,n)
Lmerge *x;
long n;
{
	x->useall = n;
}



void  lmerge_setConstant(x,n)
Lmerge *x;
long n;
{
	int i,j;
	
	if(check_inlet(x))		// constant is in right inlet
	{
		
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
			
		x->listsize = 1;
	}
	else
	{
		j = 0;
		for(i = 0;i< x->listsize;++i)
		{
			x->outList[j].a_type = A_LONG;
			x->outList[j++].a_w.w_long = n;
			if(j >= MAXSIZE)
				break;
			x->outList[j++] = x->theList[i];
			if(j >= MAXSIZE)
				break;
		}
		x->outsize = j;
		lmerge_dumpList(x);
	}
	x->inletNumber = 0;
	
}

void  lmerge_floatConstant(x,n)
Lmerge *x;
double n;
{int i,j;
	
	if(check_inlet(x))		// constant is in right inlet
	{
		
		x->theList[0].a_type = A_FLOAT;
		x->theList[0].a_w.w_float = n;
			
		x->listsize = 1;
	}
	else
	{
		j = 0;
		for(i = 0;i< x->listsize;++i)
		{
			x->outList[j].a_type = A_FLOAT;
			x->outList[j++].a_w.w_float = n;
			if(j >= MAXSIZE)
				break;
			x->outList[j++] = x->theList[i];
			if(j >= MAXSIZE)
				break;
		}
		x->outsize = j;
		lmerge_dumpList(x);
	}
	x->inletNumber = 0;
	}




void lmerge_assist(x,b,msg,arg,dst)
Lmerge *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to merge (first of pairs)");
			break;
			
			case 1:
			strcpy(dst,"List to merge (second of pairs)");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Merged Lists (A1 B1 A2 etc)");
	}
	
	
}


void  *lmerge_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lmerge  *x;
	int i;
	char firstchar, thirdchar;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lmerge *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outL= outlet_new(x,0L);
	x->interleaf = 1;
	x->insertsize = 1;
	x->outsize = 0;
	x->useall = 0;
	if(ac && (theArgs[0].a_type == A_SYM))
	{
		firstchar = theArgs[0].a_w.w_sym->s_name[0];
		thirdchar = theArgs[0].a_w.w_sym->s_name[2];
		x->interleaf = firstchar - 48;
		x->interleaf &= 0xF;
		x->insertsize = thirdchar - 48;
		x->insertsize &= 0xF;
		//post("Lmerge: interleaf   %ld, insertsize  %ld",x->interleaf, x->insertsize);
		switch(ac)
		{
		case 1:
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;
			}
			x->listsize = 1;
			break;
		
		case 2:					
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = theArgs[1].a_type;
			x->theList[i].a_w.w_long = theArgs[1].a_w.w_long;
			}
			x->listsize = 1;
			break;
			
		default:
			for(i=0;i<ac -1;++i)
			{
			x->theList[i] = theArgs[i+1];
			}
			for(;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;
			}
			x->listsize = ac-1;
		}
		
	}
	else
	{
		if(s == gensym("Lmerge3"))
			x->interleaf = 2;
		if(s == gensym("Lmerge4"))
			x->interleaf = 3;
		if(s == gensym("Lmerge5"))
			x->interleaf =4;
		if(s == gensym("Lmerge6"))
			x->interleaf = 5;
		if(s == gensym("Lmerge7"))
			x->interleaf = 6;
		if(s == gensym("Lmerge8"))
			x->interleaf = 7;
		if(s == gensym("Lmerge9"))
			x->interleaf = 8;
		switch(ac)
		{
		case 0:
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;
			}
			x->listsize = 1;
			break;
		
		case 1:					
		for(i=0;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = theArgs[0].a_type;
			x->theList[i].a_w.w_long = theArgs[0].a_w.w_long;
			}
			x->listsize = 1;
			break;
			
		default:
			for(i=0;i<ac;++i)
			{
			x->theList[i].a_type = theArgs[i].a_type;
			x->theList[i].a_w.w_long = theArgs[i].a_w.w_long;
			}
			for(;i<MAXSIZE;++i)
			{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = 0;
			}
			x->listsize = ac;
		}
		
	}
	
	
	return(x);
}

void lmerge_free(x)
Lmerge *x;
{
	
	freeobject(x->proxy);
	

}
int check_inlet(x)
Lmerge *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}