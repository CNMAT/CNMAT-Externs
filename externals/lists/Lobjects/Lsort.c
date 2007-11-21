// lsort file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1994, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define MAXSIZE 256

typedef struct lsort
{
	struct object ob;
	
	Atom outList[MAXSIZE];
	int outsize;
	void *outL;
	long caseSensitive;
	
} Lsort;


void *lsort_new();
void lsort_free();
void  lsort_List();
void lsort_anything();
long sortStuff();
int compareAtoms(Lsort *x,Atom *AtomA, Atom *AtomB);
int compareSymbols(Lsort *x,Atom *AtomA, Atom *AtomB);
void lsort_dumpList();
void lsort_setCase();
void lsort_assist();
void lsort_constant();
void lsort_Fconstant();
char convertCase(char theChar);
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lsort_new,(method)lsort_free,(short)sizeof(struct lsort),0L,A_DEFLONG,0);
	addint((method)lsort_constant);
	addfloat((method)lsort_Fconstant);
	addbang((method)lsort_dumpList);
	addmess((method)lsort_List, "list",A_GIMME,0);
	addmess((method)lsort_setCase, "casesensitive",A_GIMME,0);
	addmess((method)lsort_anything, "anything",A_GIMME,0);
	addmess((method)lsort_assist, "assist",A_CANT,0);
	//finder_addclass("Lists","Lsort");
	
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void lsort_constant(x,n)
Lsort *x;
long n;
{
	x->outList[0].a_type = A_LONG;
	x->outList[0].a_w.w_long = n;
	x->outsize = 1;
	lsort_dumpList(x);
}

void lsort_Fconstant(x,n)
Lsort *x;
double n;
{
	
	x->outList[0].a_type = A_FLOAT;
	x->outList[0].a_w.w_float = n;
	x->outsize = 1;
	lsort_dumpList(x);
}

void lsort_List(x,s,argc,argv)
Lsort *x;
Symbol *s;
int argc;
Atom *argv;
{

	if(argc > MAXSIZE) argc = MAXSIZE;
	if(argc){
		x->outsize = sortStuff(x,s,argc,argv);
		lsort_dumpList(x);
	}
	
}

void lsort_anything(x,s,argc,argv)
Lsort *x;
Symbol *s;
int argc;
Atom *argv;
{
	Atom tempList[MAXSIZE];
	int i;
	if(argc){
	if(argc >= MAXSIZE) argc = MAXSIZE-1;
	tempList[0].a_type = A_SYM;
	tempList[0].a_w.w_sym = s;
	for(i = 0; i < argc; ++i)
		tempList[i +1] = argv[i];
	lsort_List(x,s,argc + 1,tempList);
	}
	else
	outlet_anything(x->outL,s, 0, 0);
}

long sortStuff(x,s,argc,argv)
Lsort *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i, j;
	long slots[MAXSIZE];
	Atom theItem;
	int temp;
	
	for(i=0;i<MAXSIZE;++i)
		slots[i] = i;
		
		
	for(i=0;i<argc-1;++i){  // dumb ol bubble sort is fast enough for 256 items
		theItem = argv[slots[i]];
		for(j = argc-1; j > i; --j){
			if(compareAtoms(x,&argv[slots[j]], &argv[slots[j-1]])){
				temp = slots[j-1];
				slots[j-1] = slots[j];
				slots[j] = temp;
			}
		}
	}
	
	for(i = 0;i < argc; ++i)
		x->outList[i] = argv[slots[i]];
	return(argc);
}

int compareAtoms(Lsort *x, Atom * AtomA, Atom *AtomB) // returns 1 if AtomA < AtomB
{
	switch(AtomA->a_type){
		case A_LONG :
			switch(AtomB->a_type){
				case A_LONG :
					if(AtomA->a_w.w_long < AtomB->a_w.w_long)
					return(1);
					else
					return(0);
					
				case A_FLOAT :
					if((float)AtomA->a_w.w_long < AtomB->a_w.w_float)
					return(1);
					else
					return(0);
				
				case A_SYM :
					return(1); //symbols move to the end
					break;
			}
		break;
			
			
		case A_FLOAT :
			switch(AtomB->a_type){
				case A_LONG :
					if(AtomA->a_w.w_float < (float)AtomB->a_w.w_long)
					return(1);
					else
					return(0);
					
				case A_FLOAT :
					if(AtomA->a_w.w_float < AtomB->a_w.w_float)
					return(1);
					else
					return(0);
				
				case A_SYM :
					return(1); //symbols move to the end
					break;
			}
		break;
		
		case A_SYM :
			if(AtomB->a_type == A_SYM)
				return(compareSymbols(x,AtomA, AtomB));
			else
				return(0); //symbols move to the end
		break;
			
	}
	return 0;
}

int compareSymbols(Lsort *x, Atom *AtomA, Atom *AtomB) // returns 1 if AtomA < AtomB
{
	int i;
	long A,B;
	
	
	for(i = 0; i < 32; ++i){
		A = AtomA->a_w.w_sym->s_name[i];
		B = AtomB->a_w.w_sym->s_name[i];
		if(!x->caseSensitive){
		A = convertCase(A);
		B = convertCase(B);
		}
		//post("%ld    %ld",A,B);
		if(A == 0) break;
		if(B == 0) return(0); // B is shorter than A
		if(A < B) return(1);
		if(B < A) return(0);
		
	}
	if(AtomB->a_w.w_sym->s_name[i] == 0) 
		return(0); // B is equal to A
	else
		return(1); //A is shorter than B;
}

char convertCase(char theChar)
{
	if((theChar > 96) && (theChar < 123)) 
		return(theChar - 32);
	else
		return(theChar);
}

void lsort_setCase(x,s,argc,argv)
Lsort *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(argc && argv[0].a_type == A_LONG)
		x->caseSensitive = argv[0].a_w.w_long;
	else
		x->caseSensitive = 1;
	
}

void lsort_dumpList(x)
Lsort *x;
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


void lsort_assist(x,b,msg,arg,dst)
Lsort *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to be sorted");
			break;
			
			default:
			strcpy(dst,"right inlet");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Sorted list");
			break;
			
			default:
			strcpy(dst,"OutR");
			break;
		}
	}
}


void  *lsort_new(arg1)
long arg1;
{

	Lsort  *x;
	int i;
	x = (Lsort *)newobject(class);
	x->outL= outlet_new(x,0L);
	for( i = 0; i<MAXSIZE; ++i){
	x->outList[i].a_type = A_LONG;
	x->outList[i].a_w.w_long = 0;
	}
	
	x->caseSensitive = arg1;
	return(x);
}

void lsort_free(x)
Lsort *x;
{
	
	

}