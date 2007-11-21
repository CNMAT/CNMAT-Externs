// like file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2004, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct like
{
	struct object ob;
	void *proxy;
	long dummy;
	long inletNumber;
	Atom theList[MAXSIZE];
	Atom outList[MAXSIZE];
	long constant;
	int incount;
	int listsize;
	int outsize;
	int outtype;
	void *out;
	void *outL;
} Like;

void *like_new();
void like_free();
void  like_doLists();
void  like_doConstant();
void  like_doFloatConstant();
void like_dumpList();
void  like_clearList();
void like_assist();
void like_wrapon();
void like_wrapoff();
long convertHex();
int isHex();
int charHex();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)like_new,(method)like_free,(short)sizeof(struct like),0L,A_GIMME,0);
	addint((method)like_doConstant);
	addfloat((method)like_doFloatConstant);
	addbang((method)like_dumpList);
	addmess((method)like_doLists, "list",A_GIMME,0);
	addmess((method)like_assist, "assist",A_CANT,0);
	addmess((method)like_clearList, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void like_doLists(x,s,argc,argv)
Like *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,Same;
	if(argc > MAXSIZE) argc = MAXSIZE;
	if(check_inlet(x))  // list in right inlet replaces stored list.
	{
	for(i=0;i<argc;++i)
	{
		x->theList[i] = argv[i];
		
		if(x->theList[i].a_type == A_SYM)
			if(isHex(x->theList[i].a_w.w_sym))  // convert hex 
			{
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = convertHex(x->theList[i].a_w.w_sym);
			}
		
	}
	x->listsize = argc;
	}
	
	else		// find if input starts with stored list
	{
		Same = 0;
		if(argc >= x->listsize)
		{
			Same = 1;
			for(i=0; i < x->listsize; ++i)
			{
				if(argv[i].a_type == A_SYM)// Symbols are wild cards so I don't need to test them
					continue;
				if(argv[i].a_type == A_FLOAT)
				{
					switch(x->theList[i].a_type)
					{
						case A_LONG:
						if((long)argv[i].a_w.w_float != x->theList[i].a_w.w_long)
							Same = 0;
							break;
					
						case A_FLOAT:
						if(argv[i].a_w.w_float != x->theList[i].a_w.w_float)
							Same = 0;
							break;
					}
				}
				if(argv[i].a_type == A_LONG)
				{
					switch(x->theList[i].a_type)
					{
						case A_LONG:
						if(argv[i].a_w.w_long != x->theList[i].a_w.w_long)
							Same = 0;
							break;
					
						case A_FLOAT:
						if((double)argv[i].a_w.w_long != x->theList[i].a_w.w_float)
							Same = 0;
							break;
							
					}
				}
			}
		}
		if(Same)
		{
			outlet_list(x->outL,0L,argc,argv);
			
		}
		else
		{
			for(i=0; i < argc; ++i)
				x->outList[i] = argv[i];
				
			x->outsize = argc;
			outlet_list(x->out,0L,argc,x->outList);
		}
	}
	x->inletNumber = 0;
	
}

void  like_clearList(x)
Like *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	
	x->listsize = 1;
	x->inletNumber = 0;
	
}


void  like_doConstant(x,n)
Like *x;
long n;
{
	
	if(check_inlet(x))		// constant is in right inlet, set list = constant
	{
		x->theList[0].a_type = A_LONG;
		x->theList[0].a_w.w_long = n;
		x->listsize = 1;
	}
	
	else			// if constant matches stored constant, send it out
	{
		if((x->theList[0].a_type == A_LONG) && (x->listsize == 1))
		{
			if(n == x->theList[0].a_w.w_long)	
			{
				outlet_int(x->outL,n);
				
				return;
			}
		}
		
		x->outList[0].a_type = A_LONG;
			x->outList[0].a_w.w_long = n;
		x->outsize = 1;
		outlet_list(x->out,0L,1,x->outList);
	}
	x->inletNumber = 0;
	
}


void  like_doFloatConstant(x,n)
Like *x;
double n;
{
	
	
	if(check_inlet(x))		// constant is in right inlet, replaces stored list
		{
			x->theList[0].a_type = A_FLOAT;
			x->theList[0].a_w.w_float = n;
			x->listsize = 1;
		}
	else			// if constant matches stored constant, send it out
	{
		if((x->theList[0].a_type == A_FLOAT) && (x->listsize == 1))
		{
			if(n == x->theList[0].a_w.w_float)	
			{
				outlet_float(x->outL,n);
				
				return;
			}
		}
		x->outList[0].a_type = A_FLOAT;
			x->outList[0].a_w.w_float = n;
		x->outsize = 1;
		outlet_list(x->out,0L,1,x->outList);
	}
	x->inletNumber = 0;	
	
}

void like_dumpList(x)
Like *x;
{
	
		outlet_list(x->outL,0L,x->listsize,x->theList);
	x->inletNumber = 0;
	
}


void like_assist(x,b,msg,arg,dst)
Like *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to test");
			break;
			
			default:
			strcpy(dst,"List to Store");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"List if headers match");
			break;
			
			default:
			strcpy(dst,"List if no match");
			break;
		}
	}
}

void  *like_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Like  *x;
	int i;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Like *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->out= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->inletNumber = 0;
	
	for(i=0;i<ac;++i)
	{
		x->theList[i] = theArgs[i];
		
		if(x->theList[i].a_type == A_SYM)
			if(isHex(x->theList[i].a_w.w_sym))  // convert hex 
			{
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = convertHex(x->theList[i].a_w.w_sym);
			}
		
	}
	x->listsize = ac;
	
	return(x);
}
long convertHex(theSymbol)
Symbol *theSymbol;
{
	long result;
	result = charHex(theSymbol->s_name[0]) * 16;
	result += charHex(theSymbol->s_name[1]);
	return(result);

}

int charHex(theChar)
unsigned char theChar;
{
	switch(theChar)
	{
		case '0':
		return(0);
		
		case '1':
		return(1);
		
		case '2':
		return(2);
		
		case '3':
		return(3);
		
		case '4':
		return(4);
		
		case '5':
		return(5);
		
		case '6':
		return(6);
		
		case '7':
		return(7);
		
		case '8':
		return(8);
		
		case '9':
		return(9);
		
		case 'a':
		case 'A':
		return(10);
		
		case 'b':
		case 'B':
		return(11);
		
		case 'c':
		case 'C':
		return(12);
		
		case 'd':
		case 'D':
		return(13);
		
		case 'e':
		case 'E':
		return(14);
		
		case 'f':
		case 'F':
		return(15);
		
		default:
		return(0);
	
	}

}


int isHex(theSymbol)
Symbol *theSymbol;
{
	if(theSymbol->s_name[2] == 'h')
	return(1);
	else
	return(0);
}
void like_free(x)
Like *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Like *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}