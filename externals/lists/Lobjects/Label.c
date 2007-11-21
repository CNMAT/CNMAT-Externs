// label file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1996, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.¬
// modified for hex formating.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define MAXINLETS 64


typedef struct label
{
	struct object ob;
	void *proxy[MAXINLETS];
	long inletNumber;
	Atom theList[MAXSIZE];
	int numberOfInlets;
	void *out;
} Label;


void *label_new();
void label_free();
void  label_List();
void  label_Int();
void  label_Float();
void  label_Symbol();
void label_bang();
void label_set();
void  label_clearList();
void label_assist();
void label_output();
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
	
	setup((t_messlist **)&class,(method)label_new,(method)label_free,(short)sizeof(struct label),0L,A_GIMME,0);
	addint((method)label_Int);
	addfloat((method)label_Float);
	addbang((method)label_bang);
	addmess((method)label_List, "list",A_GIMME,0);
	addmess((method)label_Symbol, "anything",A_GIMME,0);
	addmess((method)label_set, "set",A_GIMME,0);
	addmess((method)label_assist, "assist",A_CANT,0);
	addmess((method)label_clearList, "clear",0);
	finder_addclass("Lists","Label");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void label_output(x,outsize,outList)
Label *x;
long outsize;
Atom * outList;
{
	long oldlock;
	oldlock = lockout_set(1);
	if(outList[0].a_type == A_SYM)
		{
			outlet_anything(x->out,outList[0].a_w.w_sym,outsize -1,&outList[1]);
			
		}
	else
		outlet_list(x->out,0L,outsize,outList);
	lockout_set(oldlock);
}

void label_set(x,s,argc,argv)
Label *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;	
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	for(i=0;i<argc;++i)
		{
		x->theList[i].a_type = argv[i].a_type;
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->theList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_w.w_float = argv[i].a_w.w_float;
			break;
			
			case A_SYM:
			x->theList[i].a_w.w_sym = argv[i].a_w.w_sym;
			break;
			
			default:
			post("Invalid List Member");
			}
		}
	x->inletNumber = 0;
	
}

void label_List(x,s,argc,argv)
Label *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	Atom outList[MAXSIZE];
	long outsize;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	outList[0] = x->theList[check_inlet(x)];
	if(argc > MAXSIZE-1) argc = MAXSIZE -1;
	for(i=0;i<argc;++i)
		{
			outList[i + 1] = argv[i];
			if(argv[i].a_type==A_SYM && isHex(argv[i].a_w.w_sym))
			{
				outList[i+1].a_type = A_LONG;
				outList[i+1].a_w.w_long = convertHex(argv[i].a_w.w_sym);
			}
			//else
				
		}
	outsize = argc +1;
	label_output(x,outsize,outList);
	x->inletNumber = 0;
	
}


void  label_clearList(x)
Label *x;
{
	int i;
	
	
	for(i=0;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = i;
		}
	
	
}


void  label_Int(x,n)
Label *x;
long n;
{
	Atom aList[2];
	
	aList[0] = x->theList[check_inlet(x)];
	
	aList[1].a_type = A_LONG;
	aList[1].a_w.w_long = n;
	label_output(x,2,aList);
	x->inletNumber = 0;
	
	
}

void  label_Float(x,n)
Label *x;
double n;
{
	Atom aList[2];
	
	
	aList[0] = x->theList[check_inlet(x)];
	
	aList[1].a_type = A_FLOAT;
	aList[1].a_w.w_float = n;
	label_output(x,2,aList);;
	x->inletNumber = 0;
	
	
}

void  label_Symbol(x,s,argc,argv)
Label *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	Atom outList[MAXSIZE];
	long outsize;
	
	outList[0] = x->theList[check_inlet(x)];
	if(isHex(s))
	{
		outList[1].a_type = A_LONG;
		outList[1].a_w.w_long = convertHex(s);
	}
	else
	{
		outList[1].a_type = A_SYM;
		outList[1].a_w.w_sym = s;
	}
	if(argc > MAXSIZE - 2) argc = MAXSIZE -2;
	if(argc)
	for(i=0; i<argc;++i)
	{
		if(argv[i].a_type==A_SYM && isHex(argv[i].a_w.w_sym))
		{
			outList[i+2].a_type = A_LONG;
			outList[i+2].a_w.w_long = convertHex(argv[i].a_w.w_sym);
		}
		else
			outList[i + 2] = argv[i];
	}
	outsize = argc + 2;
	label_output(x,outsize,outList);
	x->inletNumber = 0;
	
	
}

void label_bang(x)
Label *x;
{
	Atom outList[2];
	long outsize;
	
	outList[0] = x->theList[check_inlet(x)];
	outsize = 1;
	label_output(x,outsize,outList);
	x->inletNumber = 0;
	
}


void label_assist(x,b,msg,arg,dstString)
Label *x;
void *b;
long  msg,arg;
char *dstString;
{
	int j, value;
	char theString[32];
	j = 0;
	theString[j++] = 'P';
	theString[j++] = 'r';
	theString[j++] = 'e';
	theString[j++] = 'p';
	theString[j++] = 'e';
	theString[j++] = 'n';
	theString[j++] = 'd';
	theString[j++] = ' ';
	if(x->theList[arg].a_type == A_LONG){
		value = x->theList[arg].a_w.w_long;
		if(value < 0){ 
			theString[j++] = '-';
			if(value / 100) theString[j++] = (-value % 1000) / 100 +48; 
			if(value / 10) theString[j++] = (-value % 100 ) / 10 +48; 
			theString[j++] = -value % 10 +48;
		}else{
			if(value / 100) theString[j++] = (value % 1000) / 100 +48; 
			if(value / 10) theString[j++] = (value % 100 ) / 10 +48; 
			theString[j++] = value % 10 +48; 
		}
	}
	theString[j++] = '\0';
	if(x->theList[arg].a_type == A_SYM)
		strcat(theString,x->theList[arg].a_w.w_sym->s_name);
	if(msg == 1)
	{
		strcpy(dstString,theString);
	}
	if(msg == 2)
		strcpy(dstString,"Label followed by input. ");
	
	
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
	for(i = 0; i < 32; ++i)
		{
			destString[i] = digits[value/mag];
			value %= mag;
			mag /= 10;
			if(!mag)
				break;
		}
	destString[i] = '\0';

}


void  *label_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Label  *x;
	int i;
	
	if(ac > MAXINLETS) ac = MAXINLETS;
	x = (Label *)newobject(class);
	x->theList[0].a_type = A_SYM;
	x->theList[0].a_w.w_sym = gensym("none");
	x->inletNumber = 0;
	x->numberOfInlets = 0;
	
	switch(ac)
	{
	case 0:
	post("Label object should have at least one argument!");
	x->theList[0].a_type = A_SYM;
	x->theList[0].a_w.w_sym = gensym("label");
	x->numberOfInlets = 1;
	break;
	
	case 1:
	if(theArgs[0].a_type == A_LONG)
	{
		if(theArgs[0].a_w.w_long > MAXINLETS)
			x->numberOfInlets = MAXINLETS;
		else
			x->numberOfInlets = theArgs[0].a_w.w_long;
		for(i=0; i < x->numberOfInlets; ++i)
		{
			x->theList[i].a_type = A_LONG;
			x->theList[i].a_w.w_long = i;
		}
	}
	else
	{
		x->numberOfInlets = 1;
		x->theList[0] = theArgs[0];
	}
	
	break;
	
	case 2:
	if(theArgs[0].a_type == A_LONG && (theArgs[1].a_type == A_LONG))
	{
		if(theArgs[1].a_w.w_long > theArgs[0].a_w.w_long)
		{
			x->numberOfInlets = theArgs[1].a_w.w_long - theArgs[0].a_w.w_long + 1;
			for(i=0; i < x->numberOfInlets; ++i)
			{
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = i + theArgs[0].a_w.w_long;
			}
		}
		else
		{
			x->numberOfInlets = theArgs[0].a_w.w_long - theArgs[1].a_w.w_long + 1;
			for(i=0; i < x->numberOfInlets; ++i)
			{
				x->theList[i].a_type = A_LONG;
				x->theList[i].a_w.w_long = theArgs[0].a_w.w_long - i;
			}
		}
	}
	else
	{
		x->numberOfInlets = ac;
		for(i=0;i<ac;++i)
			{
			x->theList[i] = theArgs[i];
			}
	}
	break;
	
	default:
	x->numberOfInlets = ac;
	for(i=0;i<ac;++i)
		{
		x->theList[i] = theArgs[i];
		}
	}

	if(x->numberOfInlets)
		{
		for(i = x->numberOfInlets-1; i > 0; --i)
		x->proxy[i-1] = proxy_new(x,(long)i,&x->inletNumber);
		}
	x->out= outlet_new(x,0L);

	
	return(x);
}

void label_free(x)
Label *x;
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
Label *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}