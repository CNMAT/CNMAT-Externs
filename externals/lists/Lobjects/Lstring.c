// lstring  file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1999, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lstring
{
	struct object ob;
	void *proxy;
	long inletNumber;
	 char theString[MAXSIZE];
	Atom outList[MAXSIZE];
	Atom theAtom;
	Atom nada[1];
	int theStringPointer;
	int breakmode;
	int numMode;
	int outsize;
	void *out;
} Lstring;


void *lstring_new();
void lstring_free();
void lstring_List();
void lstring_dump();
int convert();
void  lstring_setBreak();
void  lstring_setNumeral();
void lstring_clear();
void  lstring_setConstant();
void lstring_Nada();
void lstring_assist();
char *numstr();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lstring_new,(method)lstring_free,(short)sizeof(struct lstring),0L,A_GIMME,0);
	addint((method)lstring_setConstant);
	addmess((method)lstring_List, "list",A_GIMME,0);
	addmess((method)lstring_Nada, "anything",A_GIMME,0);
	addmess((method)lstring_setBreak,"breakatspace",A_DEFLONG,0);
	addmess((method)lstring_setNumeral,"numeralsasints",A_DEFLONG,0);
	addmess((method)lstring_clear, "clear",0);
	addmess((method)lstring_assist, "assist",A_CANT,0);
	//finder_addclass("Lists","Lstring");
	

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
	
}

void lstring_List(x,s,argc,argv)
Lstring *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	
	if((argc==1 && argv[0].a_w.w_sym == x->nada[0].a_w.w_sym)){
		outlet_list(x->out,0L,1,x->nada);
	}
	else{
	j = 0;
	for(i=0;i<argc;++i)		
	{
		if(argv[i].a_w.w_long)
		switch(argv[i].a_type)
		{
			case A_LONG:
			x->theString[j++] = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theString[j++] = (long) argv[i].a_w.w_float;
			break;
			
		}
		
	}
	x->theStringPointer = j;
	lstring_dump(x);
	}
	
}

void lstring_Nada(x,s,argc,argv)
Lstring *x;
Symbol *s;
int argc;
Atom *argv;
{
	outlet_list(x->out,0L,1,x->nada);
}

void  lstring_dump(x)
Lstring *x;
{
	int i,j,k;
	char tempString[MAXSIZE];
	
	j = 0;
	k = 0;
	tempString[0] = 0;
	for(i = 0; i<x->theStringPointer; ++i){
		if((x->theString[i] == 32) && x->breakmode){
			tempString[j++] = 0;
			if(convert(x,tempString))
				x->outList[k++] = x->theAtom;
			j = 0;
		}
		else
		{
			tempString[j++] = x->theString[i];
		}
	}
	tempString[j++] = 0;
	if(x->breakmode == 0)
		outlet_anything(x->out,gensym(tempString), 0, NIL);
	else{
		if(convert(x,tempString))
			x->outList[k++]= x->theAtom;
		
		x->outsize = k;
		
		if(x->outsize == 1)
			{
			if(x->outList[0].a_type == A_LONG)
				outlet_int(x->out,x->outList[0].a_w.w_long);
				else if(x->outList[0].a_type == A_FLOAT)
				outlet_float(x->out,x->outList[0].a_w.w_float);
				else
				outlet_anything(x->out,x->outList[0].a_w.w_sym, 0, NIL);
			}
		else
		outlet_list(x->out,0L,x->outsize,x->outList);
		
	}
	
	
}

int convert(x,theString)  // return 1 if atom is valid
Lstring *x;
char *theString;
{
	short isNumber;
	short isFloat;
	short whereIsDot = 0;
	int i,k;
	float tempf,kf;
	long temp;
	isNumber = 1;
	isFloat = 0;
	if(theString[0] == 0) return 0;
	
	i = 0;
	if(x->numMode){
		while(theString[i]){  // look for non digits
			if(theString[i] == 45) ++i; // minus sign
			else
			if((theString[i] < 48) || (theString[i] > 57)){
			
				if(theString[i] == 46){  // that's a dot
					isFloat = 1;
					whereIsDot = i;
					}
				else{
					isNumber = 0;
					break;
					}
			}
			++i;
		}
		
		if(isFloat && isNumber){
			tempf = 0;
			kf = 1;
			for(i = whereIsDot-1;i>=0;--i){
				if(theString[i] == 45) { // minus sign
						tempf = - tempf;
						break;
				}
				tempf += (float)(theString[i] - 48) * kf;
				kf *= 10;
			}
			kf = 10;
			for(i = whereIsDot +1; theString[i]; ++i){
				if(tempf < 0)
					tempf = tempf - ((float)(theString[i] - 48)) /  kf;
				else
					tempf = tempf + ((float)(theString[i] - 48)) /  kf;
				kf *= 10;
				
			}
			x->theAtom.a_type = A_FLOAT;
			x->theAtom.a_w.w_float = tempf;
			
			return(1);
		}
		if(isNumber){
			temp = 0;
			k = 1;
			for(--i;i>=0;--i){
				if(theString[i] == 45) {
					temp = - temp;
					break;
				}
				temp += (theString[i] - 48) * k;
				k *= 10;
			}
			
			x->theAtom.a_type = A_LONG;
			x->theAtom.a_w.w_long = temp;
			
			return(1);
		}
	}
	x->theAtom.a_type = A_SYM;
	x->theAtom.a_w.w_sym = gensym(theString);
	return(1);

}

void  lstring_setConstant(x,n)
Lstring *x;
long n;
{	
	char tempString[MAXSIZE];
	
	tempString[0] = n;
	tempString[1]= 0;
	if((n > 47) && (n < 58) && x->numMode)
	outlet_int(x->out, n - 48);
	else
	outlet_anything(x->out,gensym(tempString), 0, NIL);
	
	
}

void  lstring_setBreak(x,n)
Lstring *x;
long n;
{
	x->breakmode = n;
}
void  lstring_setNumeral(x,n)
Lstring *x;
long n;
{
	
	x->numMode = n;
}

void lstring_clear(x)
Lstring *x;
{
	x->theString[0] = 0;
	x->theStringPointer = 0;
	x->inletNumber = 0;
}


void lstring_assist(x,b,msg,arg,dst)
Lstring *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Ascii in");
			break;
			
			default:
			strcpy(dst,"list to store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Symbols out");
			break;
			
			default:
			strcpy(dst,"bang when queue empty");
			break;
		}
	}
}


void  *lstring_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lstring  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lstring *)newobject(class);
	x->theAtom.a_type = A_SYM;
	x->theAtom.a_w.w_sym = gensym("buggy");
	x->out= outlet_new(x,0L);
	for(i = 0; i < MAXSIZE;++i) x->outList[i].a_type = A_SYM;
	x->outsize = 0;
	x->theStringPointer = 0;
	x->breakmode = 1;
	x->numMode = 1;
	x->nada[0].a_type = A_SYM;
	x->nada[0].a_w.w_sym = gensym("");
	
	return(x);
}

void lstring_free(x)
Lstring *x;
{
	
	//freeobject(x->proxy);
}