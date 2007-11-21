// lnth  an object to spread bangs around   Oct 1994   
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

typedef struct lnth
{
	struct object b_ob;
	void *proxy;
	long inletNumber;
	long pattern[32];
	long patternLength;
	long beat;
	long end;
	long count;
	void *outL;
	void *outR;
} Lnth;

void *lnth_new();
void lnth_free();
void lnth_bang();
void lnth_int();
void lnth_float();
void lnth_List();
void lnth_anything();
void lnth_assist();
void lnth_setLength();
void lnth_reset();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class, (method)lnth_new, (method)lnth_free, (short)sizeof(Lnth), 0L, A_GIMME, 0);
	addbang((method)lnth_bang);
	addint((method)lnth_int);
	addfloat((method)lnth_float);
	addmess((method)lnth_List, "list",A_GIMME,0);
	addmess((method)lnth_anything, "anything",A_GIMME,0);
	addmess((method)lnth_assist, "assist", A_CANT,0);
	addmess((method)lnth_reset, "reset", 0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void lnth_int(x,n)
Lnth *x;
long n;
{	
	if(check_inlet(x))
	{
		x->pattern[0]= n;
		x->patternLength = 1;
		lnth_reset(x);
	}
	else
	{
		if(x->pattern[x->beat]){
			++ x->count;
			if(x->count == x->pattern[x->beat]){
				if(x->beat == x->patternLength -1) 
					outlet_bang(x->outR);
				outlet_int(x->outL,n);
				x->count = 0;
				++ x->beat;
				if(x->beat == x->patternLength)
					x->beat = 0;
			}
		}
	}
}

void lnth_float(x,n)
Lnth *x;
double n;
{
	if(x->pattern[x->beat]){
		++ x->count;
		if(x->count == x->pattern[x->beat]){
			if(x->beat == x->patternLength -1) 
					outlet_bang(x->outR);
			outlet_float(x->outL,n);
			x->count = 0;
			++ x->beat;
			if(x->beat == x->patternLength)
				x->beat = 0;
		}
	}
}

void lnth_List(x,s,argc,argv)
Lnth *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(check_inlet(x))
	{
	
		for(i =0; i < argc; ++i)
		switch(argv[i].a_type){
			case A_LONG:
			x->pattern[i] = argv[i].a_w.w_long;
			break;
			case A_FLOAT:
			x->pattern[i] =(long) argv[i].a_w.w_float;
			break;
			default:
			x->pattern[i] = 1;
			break;
		}
		x->patternLength = argc;
		if(x->pattern[argc-1] == 0){  	// it will stop on 0
			x->pattern[i] = 1;			// and the 1 will be used  by reset
			++ x->patternLength;
			}
		lnth_reset(x);
	}else
	{
		if(x->pattern[x->beat]){
			++ x->count;
			if(x->count == x->pattern[x->beat]){
				if(x->beat == x->patternLength -1) 
						outlet_bang(x->outR);
				outlet_list(x->outL,0L,argc,argv);
				x->count = 0;
				++ x->beat;
				if(x->beat == x->patternLength)
					x->beat = 0;
			}
		}
	}
}

void lnth_anything(x,s,argc,argv)
Lnth *x;
Symbol *s;
int argc;
Atom *argv;
{
	if(x->pattern[x->beat]){
		++ x->count;
		if(x->count == x->pattern[x->beat]){
			if(x->beat == x->patternLength -1) 
				outlet_bang(x->outR);
			outlet_anything(x->outL,s,argc,argv);
			x->count = 0;
			++ x->beat;
			if(x->beat == x->patternLength)
				x->beat = 0;
		}
	}
}

void lnth_bang(x)
Lnth *x;
{
	if(x->pattern[x->beat]){
		++ x->count;
		if(x->count == x->pattern[x->beat]){
				if(x->beat == x->patternLength -1) 
					outlet_bang(x->outR);
				outlet_bang(x->outL);
				x->count = 0;
				++ x->beat;
				if(x->beat == x->patternLength)
					x->beat = 0;
		}
	}
}

void lnth_reset(x)
Lnth *x;
{
	x->beat = x->patternLength-1;
	x->count = x->pattern[x->beat] - 1;
}


void lnth_assist(x,b,msg,arg,dst)
Lnth *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Data to strain");
			break;
			
			default:
			strcpy(dst,"Interval");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Occasional inputs");
			break;
			
			default:
			strcpy(dst,"default");
			break;
		}
	}
}

void *lnth_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lnth *x;
	int i;
	
	x = (Lnth *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR = outlet_new(x,0L);
	x->outL = outlet_new(x,0L);
	
	if(ac >= 32) ac = 32;
	switch(ac){
	
	case 0:
	x->pattern[0] = 2;
	x->patternLength = 1;
	break;
	
	case 1:
		if(theArgs[0].a_type == A_LONG)
			x->pattern[0] = theArgs[0].a_w.w_long;
		else
			x->pattern[0] = 1;
		x->patternLength = 1;
	break;
	
	default :
		for(i = 0; i < ac ; ++i)
		switch(theArgs[i].a_type){
			case A_LONG:
			x->pattern[i] = theArgs[i].a_w.w_long;
			break;
			case A_FLOAT:
			x->pattern[i] =(long) theArgs[i].a_w.w_float;
			break;
			default:
			x->pattern[i] = 1;
			break;
		}
		x->patternLength = ac;
		if(x->pattern[ac-1] == 0){  	// it will stop on 0
			x->pattern[ac] = 1;			// and the 1 will be used  by reset
			++ x->patternLength;
			}
		break;
	}
	
	x->beat = x->patternLength-1;
	x->count = x->pattern[x->beat] - 1;
	return(x);
}

void lnth_free(x)
Lnth *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Lnth *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}