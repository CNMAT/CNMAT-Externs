// lscale object after Karlheinz Essl
// by Peter Elsea, University of Californina, Santa Cruz
// Nov 10, 1995


#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct {
struct object ob;

double inLow, inHigh, outLow,outHigh;
double denominator;
double result;
void *proxy4;
void *proxy3;
void *proxy2;
void *proxy1;
long inletNumber;
Atom theList[MAXSIZE];
int listsize;
Atom inList[MAXSIZE]; 	// copy of input
int insize;	
int mode;
int limit;
void *outL;
} Lscale;

void *lscaleNew();
void lscaleFree();
void setArgsf();
void setArgsi();

void setLimit();
void DoLscaleFloat();
void DoLscaleLong();
void DoLscaleList();
void doBang();
void   DoLscaleSet();
void   dumpList();
void   lscaleAssist();
char *strcpy();
int check_inlet();

void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;

	setup((t_messlist **)&class, (method)lscaleNew, (method)lscaleFree,(short)sizeof(Lscale),0L, A_GIMME, 0);
	addfloat((method)setArgsf);
	addint((method)setArgsi);
	addbang((method)doBang);
	addmess((method)DoLscaleList, "list", A_GIMME,0);
	addmess((method)DoLscaleSet, "set", A_GIMME,0);
	addmess((method)setLimit, "limit",A_DEFLONG,0);
	addmess((method)lscaleAssist, "assist", A_CANT,0);
	finder_addclass("math","lscale");

	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;

}


void *lscaleNew(s, ac,av)
struct symbol *s;
int ac;
struct atom *av;
{
	
	Lscale *x;
	int i;
	   
	
	x = (Lscale *)newobject(class);
	if(ac)
	x->mode = A_LONG;
	else
	x-> mode = A_FLOAT;
	x->limit = 0;
	x->insize = 0;
	x->inLow = 0;
	x->inHigh = 127;
	x->outLow = 0;
	x->outHigh = 1;
	if(ac> 0){
			if(av[0].a_type == A_LONG)
				x->inLow = (double)av[0].a_w.w_long;
			if(av[0].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->inLow = av[0].a_w.w_float;
			}
		}
		if(ac> 1){
			if(av[1].a_type == A_LONG)
				x->inHigh = (double)av[1].a_w.w_long;
			if(av[1].a_type == A_FLOAT){
				x->mode = A_FLOAT;	
				x->inHigh = av[1].a_w.w_float;
			}
		}
		if(ac> 2){
			if(av[2].a_type == A_LONG)
				x->outLow = (double)(av[2].a_w.w_long);
			if(av[2].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->outLow = (double)(av[2].a_w.w_float);
			}
		}
		if(ac> 3){
			if(av[3].a_type == A_LONG)
				x->outHigh = (double)(av[3].a_w.w_long);
			if(av[3].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->outHigh = (double)(av[3].a_w.w_float);
			}
		}
	if(x->inHigh == x->inLow)
		{
		post("Warning:Input Range of 0 in lscale. Range of 127 will be used");
		x->denominator = 127;
		}
	else
		x->denominator = (x->inHigh - x->inLow);
	
	x->proxy4 = proxy_new(x,4L,&x->inletNumber);
	x->proxy3 = proxy_new(x,3L,&x->inletNumber);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outL = outlet_new(x,0L);
	if(x->mode == A_LONG)
		for(i =0; i < MAXSIZE;++i)
			x->theList[i].a_type = A_LONG;
	else
		for(i =0; i < MAXSIZE;++i)
			x->theList[i].a_type = A_FLOAT;
	   
	return(x);
}

void setArgsf(x,n)
Lscale *x;
double n;
{
	
	   
	switch(check_inlet(x))
	{
	case 0:
	DoLscaleFloat(x,n);
	break;
	
	case 1:
	x->inLow = n;
	break;
	
	case 2:
	x->inHigh= n;
	break;
	
	case 3:
	x->outLow= n;
	break;
	
	case 4:
	x->outHigh= n;
	break;
	}
	
	
	
	if(x->inHigh == x->inLow){
		x->denominator = 127;
		post("Warning:Input Range of 0 in lscale. 127 used");
		}
	else
		x->denominator = (x->inHigh - x->inLow);
	   


}
void setArgsi(x,n)
Lscale *x;
long n;
{
	setArgsf(x, (double)n);
	   
}	

void setLimit(x,n)
Lscale *x;
long n;
{
	x->limit = n;
}



void DoLscaleFloat(x, n)
Lscale *x;
double n;
{
	double result;
	   
		if(x->denominator == 0) return;
		
		result = (n - x->inLow)*(x->outHigh - x->outLow) /x->denominator + x->outLow ;
		if(x->limit){
			if(x->outHigh > x->outLow){
				if(result > x->outHigh) result = x->outHigh;
				if(result < x->outLow) result = x->outLow;
			} else{
				if(result < x->outHigh) result = x->outHigh;
				if(result > x->outLow) result = x->outLow;
			}
		}
		
		if(x->mode == A_LONG)
		x->theList[0].a_w.w_long = (double)result;
		else
		x->theList[0].a_w.w_float = result;
		x->insize = 1;
		x->inList[0].a_w.w_float = n;
		x->inList[0].a_type = A_FLOAT;
		x->listsize = 1;
		dumpList(x);
		
	   
}

void DoLscaleList(x, s, argc,argv)
Lscale *x;
Symbol *s;
long argc;
Atom *argv;
{
	double result,outRange, scaler,inValue;
	int i;
	   
	outRange = (x->outHigh - x->outLow);
	if(x->denominator == 0) return;
	scaler = 1/ x->denominator ;
	
	for(i = 0 ; i < argc; ++i){
		x->inList[i] = argv[i];
		if(argv[i].a_type == A_SYM){
			x->theList[i] = argv[i];
				continue;
				}
		if(argv[i].a_type == A_LONG)
			inValue = (double)argv[i].a_w.w_long;
		else
			inValue = argv[i].a_w.w_float;
			
		result = (inValue - x->inLow)* outRange * scaler + x->outLow ;
		if(x->limit){
			if(x->outHigh > x->outLow){
				if(result > x->outHigh) result = x->outHigh;
				if(result < x->outLow) result = x->outLow;
			} else{
				if(result < x->outHigh) result = x->outHigh;
				if(result > x->outLow) result = x->outLow;
			}
		}
		
		if(x->mode == A_LONG)
			x->theList[i].a_w.w_long = (long)result;
		else
			x->theList[i].a_w.w_float = result;
	
	}
	x->listsize = argc;
	x->insize = argc;
	dumpList(x);
}

void doBang(x)
Lscale *x;
{
	double result,outRange, scaler,inValue;
	int i;
	   
	outRange = (x->outHigh - x->outLow);
	if(x->denominator == 0) return;
	scaler = 1/ x->denominator ;
	
	for(i = 0 ; i < x->insize; ++i){
	if(x->inList[i].a_type == A_LONG)
		inValue = (double)x->inList[i].a_w.w_long;
	else
		inValue = x->inList[i].a_w.w_float;
		
	result = (inValue - x->inLow)* outRange * scaler + x->outLow ;
	if(x->limit){
		if(x->outHigh > x->outLow){
			if(result > x->outHigh) result = x->outHigh;
			if(result < x->outLow) result = x->outLow;
		} else{
			if(result < x->outHigh) result = x->outHigh;
			if(result > x->outLow) result = x->outLow;
		}
	}
	
	if(x->mode == A_LONG)
		x->theList[i].a_w.w_long = (long)result;
	else
		x->theList[i].a_w.w_float = result;
	
	}
	x->listsize = x->insize;
	dumpList(x);
}

void DoLscaleSet(x, s, ac,av)
Lscale *x;
Symbol *s;
long ac;
Atom *av;
{
	int i;
	   
	x-> mode = A_LONG;
	if(ac> 0){
			if(av[0].a_type == A_LONG)
				x->inLow = (double)av[0].a_w.w_long;
			if(av[0].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->inLow = av[0].a_w.w_float;
			}
		}
		if(ac> 1){
			if(av[1].a_type == A_LONG)
				x->inHigh = (double)av[1].a_w.w_long;
			if(av[1].a_type == A_FLOAT){
				x->mode = A_FLOAT;	
				x->inHigh = av[1].a_w.w_float;
			}
		}
		if(ac> 2){
			if(av[2].a_type == A_LONG)
				x->outLow = (double)(av[2].a_w.w_long);
			if(av[2].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->outLow = (double)(av[2].a_w.w_float);
			}
		}
		if(ac> 3){
			if(av[3].a_type == A_LONG)
				x->outHigh = (double)(av[3].a_w.w_long);
			if(av[3].a_type == A_FLOAT){
				x->mode = A_FLOAT;
				x->outHigh = (double)(av[3].a_w.w_float);
			}
		}
	if(x->inHigh == x->inLow)
		{
		post("Warning:Input Range of 0 in lscale. Range of 127 will be used");
		x->denominator = 127;
		
		}
	else
		x->denominator = (x->inHigh - x->inLow);
		if(x->mode == A_LONG)
		for(i =0; i < MAXSIZE;++i)
			x->theList[i].a_type = A_LONG;
	else
		for(i =0; i < MAXSIZE;++i)
			x->theList[i].a_type = A_FLOAT;
	   
}

void dumpList(x)
Lscale *x;
{
	   
	if(x->listsize){
		if(x->listsize == 1){
			switch(x->theList[0].a_type)
			{
			 case A_LONG:
				outlet_int(x->outL,x->theList[0].a_w.w_long);
				break;
				
			case A_FLOAT:
				outlet_float(x->outL,x->theList[0].a_w.w_float);
				break;
				
			case A_SYM:
				outlet_anything(x->outL,x->theList[0].a_w.w_sym,0,NIL);
			}
		}
		else
		outlet_list(x->outL,0L,x->listsize,x->theList);	
	}
	//else
	//	outlet_bang(x->outR);
	
	   
}

void lscaleAssist(x,b,msg,arg,dst)
Lscale *x;
void *b;
long  msg,arg;
char *dst;
{
	
	   
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Value or list to rescale");
			break;
			
			case 1:
			strcpy(dst,"Input Range Low");
			break;
			
			case 2:
			strcpy(dst,"Input Range High");
			break;
			
			case 3:
			strcpy(dst,"Output Range Low");
			break;
			
			
			default:
			strcpy(dst,"Output Range High");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			if(x->limit)
			strcpy(dst,"Scaled and limited output");
			else
			strcpy(dst,"Scaled Output");
			break;
			
			default:
			strcpy(dst,"OutR");
			break;
		}
	}
}

void lscaleFree(x)
Lscale *x;
{
	freeobject(x->proxy3);
	freeobject(x->proxy2);
	freeobject(x->proxy1);
	   
}

int check_inlet(x)
Lscale *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}