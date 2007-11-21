// bbyte file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 2000, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define MAXINLETS 32


typedef struct bbyte
{
	struct object ob;
	void *proxy[MAXINLETS];
	long inletNumber;
	long baseValue;
	long inValues[MAXINLETS];
	long outValue;
	int outsize;
	long immediate;
	int numberOfInlets;
	void *out;
} BByte;


void *bbyte_new();
void bbyte_free();
void  bbyte_List();
void  bbyte_Int();
void  bbyte_Float();
void  bbyte_Symbol();
void bbyte_bang();
void bbyte_set();
void bbyte_immediate();
void  bbyte_clear();
void bbyte_assist();
void bbyte_output();
void convert();
long convertHex();
int isHex();
int charHex();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)bbyte_new,(method)bbyte_free,(short)sizeof(struct bbyte),0L,A_GIMME,0);
	addint((method)bbyte_Int);
	addfloat((method)bbyte_Float);
	addbang((method)bbyte_bang);
	addmess((method)bbyte_List, "list",A_GIMME,0);
	addmess((method)bbyte_set, "set",A_GIMME,0);
	addmess((method)bbyte_immediate, "immediate",A_DEFLONG,0);
	addmess((method)bbyte_assist, "assist",A_CANT,0);
	addmess((method)bbyte_clear, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void bbyte_output(x)
BByte *x;
{
	int i;
	x->outValue = x->baseValue;
	for(i = 0  ; i< x->numberOfInlets; ++i)
		x->outValue |= x->inValues[i] << i ;
	outlet_int(x->out,x->outValue);
}

void bbyte_immediate(x,n)
BByte *x;
long n;
{
	x->immediate = n;
}
void bbyte_set(x,s,argc,argv)
BByte *x;
Symbol *s;
int argc;
Atom *argv;
{		
	if(argc == 0)
		x->baseValue = 0;
	else
	{
		if(argv[0].a_type == A_LONG)
		{
			x->baseValue = argv[0].a_w.w_long;
		}
		if(argv[0].a_type == A_SYM)
		{
			x->baseValue = convertHex(argv[0].a_w.w_sym);
		}
	}
	bbyte_clear(x);
	x->inletNumber = 0;
	
}

void bbyte_List(x,s,argc,argv)
BByte *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	Atom temp[MAXINLETS];
	
	if(argc > x->numberOfInlets) argc = x->numberOfInlets;
	for(i = 0;i < MAXINLETS; ++i)
	{
		temp[i].a_w.w_long = 0;
	}
	for(i = 0;i < argc; ++i)
	{
		temp[i] = argv[argc - i -1];
	}
	bbyte_clear(x);
	for(i = 0;i < argc; ++i)
	{
		if(temp[i].a_type == A_LONG)
			x->inValues[i] = temp[i].a_w.w_long ;
	}
	
	if((x->immediate) ||(check_inlet(x) == 0))
		bbyte_output(x);
	x->inletNumber = 0;
	
}


void  bbyte_clear(x)
BByte *x;
{
	int i;
	
	
	for(i = 0;i < x->numberOfInlets; ++i)
		{
			x->inValues[i] = 0 ;
			
		}
	x->inletNumber = 0;
	
}


void  bbyte_Int(x,n)
BByte *x;
long n;
{
	
	int which = check_inlet(x);
	//post("inlet %ld",x->inletNumber);
	x->inValues[x->numberOfInlets -1 - which] = n ;
	if((x->immediate) ||(which == 0))
		bbyte_output(x);
	x->inletNumber = 0;
	
	
}

void  bbyte_Float(x,n)
BByte *x;
double n;
{
	
	int which = check_inlet(x);
	x->inValues[ x->numberOfInlets-1 - which] = (long)n ;
	if((x->immediate) ||(which == 0))
		bbyte_output(x);
	x->inletNumber = 0;
	
	
}



void bbyte_bang(x)
BByte *x;
{
	
	bbyte_output(x);
	x->inletNumber = 0;
	
}


void bbyte_assist(x,b,msg,arg,dstString)
BByte *x;
void *b;
long  msg,arg;
char *dstString;
{
	char tempString[32];
	char numString[8];
	
	
	if(msg == 1)
	{
		
		convert(numString,x->numberOfInlets - arg -1);
		strcpy(tempString,"sets bit ");
		strcat(tempString,numString);	
		strcpy(dstString,tempString);
	}
	if(msg == 2)
		strcpy(dstString,"Value with bits set. ");
	
	
}

void convert(destString,value)
char *destString;
long value;
{
	char *digits = "0123456789";
	int i,mag;
	
	mag = 1;
	while(value/mag)
		mag *= 10;
	mag /= 10;
	for(i = 0; i < 32; )
		{
			destString[i++] = digits[value/mag];
			value %= mag;
			mag /= 10;
			if(!mag)
				break;
		}
	destString[i] = '\0';

}


void  *bbyte_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	BByte  *x;
	int i;
	
	if(ac > MAXINLETS) ac = MAXINLETS;
	x = (BByte *)newobject(class);
	x->inletNumber = 0;
	x->numberOfInlets = 8;
	x->outValue = 0;
	x->immediate = 0;
	switch(ac)
	{
		case 0:
		x->baseValue = 0;
		
		break;
		
		case 1:
		if(theArgs[0].a_type == A_LONG)
		{
			x->baseValue = theArgs[0].a_w.w_long;
		}
		if(theArgs[0].a_type == A_SYM)
		{
			x->baseValue = convertHex(theArgs[0].a_w.w_sym);
		}
		
		break;
		
		
		default:
		if(theArgs[0].a_type == A_LONG)
		{
			x->numberOfInlets = theArgs[0].a_w.w_long;
		}
		if(theArgs[1].a_type == A_LONG)
		{
			x->baseValue = theArgs[1].a_w.w_long;
		}
		else
		if(theArgs[1].a_type == A_SYM)
		{
			x->baseValue = convertHex(theArgs[1].a_w.w_sym);
		}
	}

	if(x->numberOfInlets)
		{
		for(i = x->numberOfInlets-1; i > 0; --i)
		x->proxy[i-1] = proxy_new(x,(long)i,&x->inletNumber);
		}
	x->out= outlet_new(x,0L);
	bbyte_clear(x);
	
	return(x);
}

void bbyte_free(x)
BByte *x;
{
	int i;
	
	for(i=0; i < x->numberOfInlets-1; ++i)
		freeobject(x->proxy[i]);
	

}



long convertHex(theSymbol)
Symbol *theSymbol;
{
	long result = 0;
	int i,length;
	length = strlen(theSymbol->s_name);
	for(i = 0;i<length - 1; ++i)
	{
	result = result << 4;
	result += charHex(theSymbol->s_name[i]);
	
	}
	return(result);

}

int isHex(theSymbol)
Symbol *theSymbol;
{
	int i,length;
	unsigned char theChar;
	length = strlen(theSymbol->s_name);
	if(theSymbol->s_name[length-1] != 'h' && theSymbol->s_name[length-1] != 'H')
		return(0);
	for(i= 0; i < length-1; ++i)
	{
		theChar = theSymbol->s_name[i];
		if(theChar > 0x2F && theChar < 0x3A)
			continue;
		if(theChar > 0x40 && theChar < 0x47)
			continue;
		if(theChar > 0x60 && theChar < 0x67)
			continue;
		return(0);
	}
	return(1);
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
int check_inlet(x)
BByte *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}