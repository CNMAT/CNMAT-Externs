// lexpr file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1997, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

//This object works by first setting an expr, substituting $i1 for in, $i6 for iy-1 and
// so forth.
// During execution, the tables x->inLinks and x->outLinks are used to pass values from argv
// and x->outList to x->types, which is passed to the expr. The result is returned in result
// and appended to x->outList.
// the last four members of argv are saved in x->recentInputs and the last four results
// are saved in x->recentOutputs. These are used and updated by the constant and bang routines.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lexpr
{
	struct object ob;
	long inletNumber;
	Atom outList[MAXSIZE];
	Atom types[9];
	Atom recentOutputs[5];
	Atom recentInputs[5];
	int  inLinks[5];
	int	 outLinks[5];
	int seeds;
	int seedout;
	int outsize;
	void *outL;
	void *theExpr;
	void *proxy;
} Lexpr;


void *lexpr_new();
void lexpr_free();
void  lexpr_doLists();
void lexpr_seed();
void  lexpr_setConstant();
void  lexpr_floatConstant();
void  lexpr_bang();
void  lexpr_clearList();
void lexpr_assist();
void fillin();
void seedout();
void noseedout();
int check_inlet();
void *class;

int main(void)
{
	
	Symbol * s;
	Symbol * l;

	setup((t_messlist **)&class,(method)lexpr_new,(method)lexpr_free,(short)sizeof(struct lexpr),0L,A_GIMME,0);
	addint((method)lexpr_setConstant);
	addfloat((method)lexpr_floatConstant);
	addbang((method)lexpr_bang);
	addmess((method)lexpr_doLists, "list",A_GIMME,0);
	addmess((method)lexpr_seed, "seed",A_GIMME,0);
	addmess((method)lexpr_assist, "assist",A_CANT,0);
	addmess((method)lexpr_clearList, "clear",0);
	addmess((method)seedout, "seedout",0);
	addmess((method)noseedout, "noseedout",0);
	finder_addclass("Lists","Lexpr");
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © Regents of the University of California.");
		s->s_thing = (struct object *)l;}
	return 0;
}

void lexpr_doLists(x,s,argc,argv)
Lexpr *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	Atom result;
	
	if(argc > MAXSIZE) argc = MAXSIZE;
	i=0;
	if(x->seeds)
	{
		for(; i < x->seeds; ++i)
		{
			x->outList[i] = argv[i];
			//post("seed %d",i);
		}
	}
	for(; i < argc; ++i)
	{
		for(j = 0;j < 5 ;j++)
		{
			if(x->inLinks[j] > -1)
			{
				if(i-j > -1)
					x->types[x->inLinks[j]] = argv[i-j];
				else
					x->types[x->inLinks[j]].a_w.w_long = 0;
			}
			if(x->outLinks[j] > -1)
			{
				if(i-j >= 0)
					x->types[x->outLinks[j]] = x->outList[i-j];
				else
					x->types[x->outLinks[j]].a_w.w_long = 0;
			}
		}
		expr_eval(x->theExpr,9,x->types,&result);
		x->outList[i] = result;
		
	}
	for(j= 0; j < 5; j++)
	{
		if((argc - 5 + j) > -1)
		{
			x->recentInputs[j] = argv[argc - 5 +j];
			x->recentOutputs[j] = x->outList[argc - 5 +j];
		}
		else
		{
			x->recentInputs[j].a_w.w_long = 0;
			x->recentOutputs[j].a_w.w_long = 0;
		}
	}
	outlet_list(x->outL,0L,(short)argc,x->outList);
	x->inletNumber = 0;
	
}

void lexpr_seed(x,s,argc,argv)
Lexpr *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;

	for(i = 0; i <argc; ++i)
	{
		x->outList[i] = argv[i];
	}
		for(j= 0; j < 5; j++)
	{
		if((argc - 5 + j) > -1)
		{
			x->recentInputs[j] = argv[argc - 5 +j];
			x->recentOutputs[j] = x->outList[argc - 5 +j];
		}
		else
		{
			x->recentInputs[j].a_w.w_long = 0;
			x->recentOutputs[j].a_w.w_long = 0;
		}
	}
	if(x->seedout)
	outlet_list(x->outL,0L,(short)argc,x->outList);
	x->inletNumber = 0;
	
}

void  lexpr_clearList(x)
Lexpr *x;
{
	int i;
	
	for(i = 0; i < 5;++i)
	{
		x->recentInputs[i].a_w.w_long = 0;
		x->recentOutputs[i].a_w.w_long = 0;
	}
	x->inletNumber = 0;
	
}

void  seedout(x)
Lexpr *x;
{
	
	x->seedout = 1;
	x->inletNumber = 0;
	
}

void  noseedout(x)
Lexpr *x;
{
	x->seedout = 0;
	x->inletNumber = 0;
	
}
void  lexpr_bang(x)
Lexpr *x;
{
	long n;
	   double nf;
	
	x->inletNumber = 0;
	if(x->recentInputs[4].a_type == A_LONG)
	{
		n = 2 * x->recentInputs[4].a_w.w_long - x->recentInputs[3].a_w.w_long;
		//post("n = %ld",n);
		lexpr_setConstant(x,n);
	}
	else
	{
		nf =2 * x->recentInputs[4].a_w.w_float - x->recentInputs[3].a_w.w_float;
		lexpr_floatConstant(x,nf);
	}
	
	
}


void  lexpr_setConstant(x,n)
Lexpr *x;
long n;
{
	int i,j;
	Atom result;
	
	if(check_inlet(x))  // it's in the right inlet
	{
		x->types[4].a_w.w_long = n;
	}
	
	else
	{
		for(i=0;i<4;++i)
		{
			x->recentInputs[i] = x->recentInputs[i+1];
			x->recentOutputs[i] = x->recentOutputs[i+1];
		}
		if(x->recentInputs[4].a_type == A_LONG)
			x->recentInputs[4].a_w.w_long = n;
		else
			x->recentInputs[4].a_w.w_float =(   double) n;
		for(j = 0;j < 5 ;j++)
			{
				if(x->inLinks[j] > -1)
				{
					if(i-j > -1)
						x->types[x->inLinks[j]] = x->recentInputs[i-j];
					else
						x->types[x->inLinks[j]].a_w.w_long = 0;
				}
				if(x->outLinks[j] > -1)
				{
					if(i-j >= 0)
						x->types[x->outLinks[j]] = x->recentOutputs[i-j];
					else
						x->types[x->outLinks[j]].a_w.w_long = 0;
				}
			}
			expr_eval(x->theExpr,9,x->types,&result);
			
			x->recentOutputs[4] = result;
			
			if(result.a_type == A_LONG)
				outlet_int(x->outL,result.a_w.w_long);
			if(result.a_type == A_FLOAT)
				outlet_float(x->outL,result.a_w.w_float);
	}
	x->inletNumber = 0;
	
}

void  lexpr_floatConstant(x,n)
Lexpr *x;
   double n;
{	
	int i,j;
	Atom result;
	
	
	if(check_inlet(x))
	{
		x->types[4].a_w.w_float = n;
	}
	
	else
	{
		for(i=0;i<4;++i)
		{
			x->recentInputs[i] = x->recentInputs[i+1];
			x->recentOutputs[i] = x->recentOutputs[i+1];
		}
		if(x->recentInputs[4].a_type == A_FLOAT)
			x->recentInputs[4].a_w.w_float = n;
		else
			x->recentInputs[4].a_w.w_long =(long) n;
		for(j = 0;j < 5 ;j++)
			{
				if(x->inLinks[j] > -1)
				{
					if(i-j > -1)
						x->types[x->inLinks[j]] = x->recentInputs[i-j];
					else
						x->types[x->inLinks[j]].a_w.w_long = 0;
				}
				if(x->outLinks[j] > -1)
				{
					if(i-j >= 0)
						x->types[x->outLinks[j]] = x->recentOutputs[i-j];
					else
						x->types[x->outLinks[j]].a_w.w_long = 0;
				}
			}
			expr_eval(x->theExpr,9,x->types,&result);
			
			x->recentOutputs[4] = result;
			
			if(result.a_type == A_LONG)
				outlet_int(x->outL,result.a_w.w_long);
			if(result.a_type == A_FLOAT)
				outlet_float(x->outL,result.a_w.w_float);
		}
		x->inletNumber = 0;
	
	}




void lexpr_assist(x,b,msg,arg,dst)
Lexpr *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"List to process");
			break;
			
			case 1:
			strcpy(dst,"List of results");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			strcpy(dst,"Merged Lists (A1 B1 A2 etc)");
	}
	
	
}


void  *lexpr_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lexpr  *x;
	int i,j;
	Atom argv[MAXSIZE];
	Symbol *inputArgs[36];
	int intsUsed,floatsUsed;
	int theInput;
	int anyInputs;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lexpr *)newobject(class);
	x->outL= outlet_new(x,0L);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->inletNumber = 0;
	x->seedout = 1;
	intsUsed = 0;
	floatsUsed = 0;
	anyInputs = 0;
	fillin(inputArgs);
	
	for(i = 0;i<6;i++)
	{
		x->inLinks[i] = -1;
		x->outLinks[i] = -1;
	}
	for(i = 0;i<ac;i++)
	{
		argv[i] = theArgs[i];
		if(argv[i].a_type == A_SYM)
		{
			theInput = -1;
			for(j = 0;j < 36;j++)		//look for input tokens
			{
				if(argv[i].a_w.w_sym == inputArgs[j])
				{
				theInput = j;
				//post("theInput = %d",j);
				break;
				}
			}
			if((theInput > -1) &&( theInput < 4)) // it's an int for input list
			{
				x->inLinks[theInput] = theInput;
				argv[i].a_w.w_sym = inputArgs[18 + theInput];
				anyInputs = 1;
				continue;
			}
			
			if(theInput == 4) // it's an int Constant
			{
				
				argv[i].a_w.w_sym = inputArgs[22];
				continue;
			}
			
			if((theInput > 4) &&( theInput < 9)) // it's an int for output list
			{
				x->outLinks[theInput-4] = theInput;
				argv[i].a_w.w_sym = inputArgs[18 + theInput];
				if(x->seeds < (theInput - 4)) x->seeds = theInput - 4;
				continue;
			}
			
			if((theInput > 8) &&( theInput < 13)) // it's a    double for input list
			{
				x->inLinks[theInput-9] = theInput - 9;
				argv[i].a_w.w_sym = inputArgs[18 + theInput];
				anyInputs = 1;
				continue;
			}
			
			if(theInput == 13) // it's a    double constant
			{
				
				argv[i].a_w.w_sym = inputArgs[31];
				continue;
			}
			
			if((theInput > 13) &&( theInput < 18)) // it's a    double for output list
			{
				x->outLinks[theInput - 13] = theInput - 9;
				argv[i].a_w.w_sym = inputArgs[18 + theInput];
				if(x->seeds < (theInput - 13)) x->seeds = theInput - 13;
				continue;
			}
			if((theInput > 19) && (theInput < 36)) // it's a mistake
				post("Inappropriate argument to Lexpr: Use in,fn,in-1,fn-1,iy-1,fy-1, etc.");
			
		}
	
	}
	for(i = 0; i< 6; ++ i)
	{
		//post("inlink[%d]=%d",i,x->inLinks[i]);
		//post("outlink[%d]=%d",i,x->outLinks[i]);
		}
	x->theExpr = expr_new((short)ac,argv,x->types);
	if(x->theExpr == 0)
	{
	
	return(0);
	}
	x->types[4].a_w.w_long = 0;
	if(anyInputs)
		x->seeds = 0;

	return(x);
}

void lexpr_free(x)
Lexpr *x;
{
	
	freeobject(x->theExpr);
	freeobject(x->proxy);
	

}


void fillin(inputArgs)
Symbol **inputArgs;
{
	
	
	inputArgs[0] = gensym("in");
	inputArgs[1] = gensym("in-1");
	inputArgs[2] = gensym("in-2");
	inputArgs[3] = gensym("in-3");
	inputArgs[4] = gensym("iC");
	
	inputArgs[5] = gensym("iy-1");
	inputArgs[6] = gensym("iy-2");
	inputArgs[7] = gensym("iy-3");
	inputArgs[8] = gensym("iy-4");
	
	inputArgs[9] = gensym("fn");
	inputArgs[10] = gensym("fn-1");
	inputArgs[11] = gensym("fn-2");
	inputArgs[12] = gensym("fn-3");
	inputArgs[13] = gensym("fC");
	
	inputArgs[14] = gensym("fy-1");
	inputArgs[15] = gensym("fy-2");
	inputArgs[16] = gensym("fy-3");
	inputArgs[17] = gensym("fy-4");
	
	inputArgs[18] = gensym("$i1");
	inputArgs[19] = gensym("$i2");
	inputArgs[20] = gensym("$i3");
	inputArgs[21] = gensym("$i4");
	inputArgs[22] = gensym("$i5");
	inputArgs[23] = gensym("$i6");
	inputArgs[24] = gensym("$i7");
	inputArgs[25] = gensym("$i8");
	inputArgs[26] = gensym("$i9");
	
	inputArgs[27] = gensym("$f1");
	inputArgs[28] = gensym("$f2");
	inputArgs[29] = gensym("$f3");
	inputArgs[30] = gensym("$f4");
	inputArgs[31] = gensym("$f5");
	inputArgs[32] = gensym("$f6");
	inputArgs[33] = gensym("$f7");
	inputArgs[34] = gensym("$f8");
	inputArgs[35] = gensym("$f9");
	

}

int check_inlet(x)
Lexpr *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}