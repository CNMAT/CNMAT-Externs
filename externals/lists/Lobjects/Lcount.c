// lcount file for Max object
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

typedef struct lcount
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	void *proxy3;
	long inletNumber;
	//Atom theList[MAXSIZE];
	//Atom outList[MAXSIZE];
	long startWith;
	long endWith;
	long end;
	long interval;
	long nextOut;
	long lastOut;
	int   floatMode;
	int	remembertobang;
	double fstartWith;
	double fendWith;
	double fend;
	double finterval;
	double fnextOut;
	double flastOut;
	double precision;
	//int outsize;
	void *outL;
	void *outR;
	void *outX;
	Atom theAtom[2];
} Lcount;


void *lcount_new();
void lcount_free();
void  lcount_doLists();
void lcount_doBang();
void doInt();
void doFloat();
void  lcount_setConstant();
void  lcount_floatConstant();
void lcount_dumpList();
void  lcount_clearList();
void lcount_assist();
void  computeCount();
void  lcount_reset();
double roundoff();
void setPrecision();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lcount_new,(method)lcount_free,(short)sizeof(struct lcount),0L,A_GIMME,0);
	addint((method)doInt);
	addfloat((method)doFloat);
	addbang((method)lcount_doBang);
	addmess((method)lcount_doLists, "list",A_GIMME,0);
	addmess((method)lcount_assist, "assist",A_CANT,0);
	addmess((method)setPrecision, "precision",A_DEFLONG,0);
	addmess((method)lcount_reset, "reset",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lcount_doLists(x,s,ac,theArgs)
Lcount *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int oldMode;
	
	if(ac)
	{
		oldMode = x->floatMode;
		x->floatMode = 0;
		switch(ac)  // load in data given
		{
			case 1:
			if(theArgs[0].a_type == A_LONG)
				x->endWith = theArgs[0].a_w.w_long;
			if(theArgs[0].a_type == A_FLOAT)
			{
				x->fendWith = theArgs[0].a_w.w_float;
				x->floatMode = 1;
			}
			if(x->floatMode)
			{
				if(!oldMode) // is this a change?
				{
					x->finterval = (double)x->interval;
					x->fstartWith = (double)x->startWith;
				}
			}
			else
			{
				if(oldMode) // is this a change?
				{
					x->interval = (long)x->finterval;
					x->startWith = (long)x->startWith;
				}
			}

			break;
			
			case 2:
			if(theArgs[1].a_type == A_LONG)
				x->endWith = theArgs[1].a_w.w_long;
			if(theArgs[1].a_type == A_FLOAT)
			{
				x->fendWith = theArgs[1].a_w.w_float;
				x->floatMode = 1;
			}
			if(x->floatMode)
			{
				if(theArgs[0].a_type == A_LONG)
					x->fstartWith = (double)theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->fstartWith = theArgs[0].a_w.w_float;
				if(!oldMode) // is this a change?
				{
					x->finterval = (double)x->interval;
				}
			}
			else
			{
				if(theArgs[0].a_type == A_LONG)
					x->startWith = theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->startWith = (long)theArgs[0].a_w.w_float;
				if(oldMode) // is this a change?
				{
					x->interval = (long)x->finterval;
				}
			}
			
			
			break;
			
			case 3:
			if(theArgs[2].a_type == A_LONG)
				x->endWith = theArgs[2].a_w.w_long;
			if(theArgs[2].a_type == A_FLOAT)
			{
				x->fendWith = theArgs[2].a_w.w_float;
				x->floatMode = 1;
			}
			if(x->floatMode)
			{
				if(theArgs[0].a_type == A_LONG)
					x->finterval = (double)theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->finterval = theArgs[0].a_w.w_float;	
					
				if(theArgs[1].a_type == A_LONG)
					x->fstartWith = (double)theArgs[1].a_w.w_long;
				if(theArgs[1].a_type == A_FLOAT)
					x->fstartWith = theArgs[1].a_w.w_float;
			}
			else
			{
				if(theArgs[0].a_type == A_LONG)
					x->interval = theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->interval = (long)theArgs[0].a_w.w_float;	
					
				if(theArgs[1].a_type == A_LONG)
					x->startWith = theArgs[1].a_w.w_long;
				if(theArgs[1].a_type == A_FLOAT)
					x->startWith = (long)theArgs[1].a_w.w_float;
			}
			break;
			
			default: // set nextOut (argv[0]) later
			if(theArgs[3].a_type == A_LONG)
				x->endWith = theArgs[3].a_w.w_long;
			if(theArgs[3].a_type == A_FLOAT)
			{
				x->fendWith = theArgs[3].a_w.w_float;
				x->floatMode = 1;
			}
			if(x->floatMode)
			{
				if(theArgs[1].a_type == A_LONG)
					x->finterval = (double)theArgs[1].a_w.w_long;
				if(theArgs[1].a_type == A_FLOAT)
					x->finterval = theArgs[1].a_w.w_float;	
					
				if(theArgs[2].a_type == A_LONG)
					x->fstartWith = (double)theArgs[2].a_w.w_long;
				if(theArgs[2].a_type == A_FLOAT)
					x->fstartWith = theArgs[2].a_w.w_float;
			}
			else
			{
				if(theArgs[1].a_type == A_LONG)
					x->interval = theArgs[1].a_w.w_long;
				if(theArgs[1].a_type == A_FLOAT)
					x->interval = (long)theArgs[1].a_w.w_float;	
					
				if(theArgs[2].a_type == A_LONG)
					x->startWith = theArgs[2].a_w.w_long;
				if(theArgs[2].a_type == A_FLOAT)
					x->startWith = (long)theArgs[2].a_w.w_float;
			}
			break;
			
		} // end of big switch
		if(ac > 3) // set next Out
		{
			if(x->floatMode)
			{
				if(theArgs[0].a_type == A_LONG)
					x->fnextOut = (double)theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->fnextOut = theArgs[0].a_w.w_float;	
			}
			else
			{
				if(theArgs[0].a_type == A_LONG)
					x->nextOut = theArgs[0].a_w.w_long;
				if(theArgs[0].a_type == A_FLOAT)
					x->nextOut = (long)theArgs[0].a_w.w_float;
			}
		}
		else
		{
			if(x->floatMode)
			{
				if(x->finterval > 0)
					x->fnextOut = x->fstartWith;
				else
					x->fnextOut = x->fendWith;
			}
			else
			{
				if(x->interval > 0)
					x->nextOut = x->startWith;
				else
					x->nextOut = x->endWith;
			}
		}
	}
	//post("interval %ld start %ld  end %ld",x->interval,x->startWith,x->endWith);
	//post("finterval %f fstart %f  fend %f",x->finterval,x->fstartWith,x->fendWith);
	x->inletNumber = 0;
	
}


void  lcount_doBang(x)
Lcount *x;
{
	
	
	
	if(x->floatMode)
	{
		outlet_float(x->outR,x->fnextOut);
		x->flastOut = x->fnextOut;
		if(x->remembertobang)
		{
			
			x->remembertobang = 0;
			outlet_bang(x->outL);
		}
		if(x->flastOut == x->fnextOut)
		computeCount(x);
	}
	else
	{
		outlet_int(x->outR,x->nextOut);
		x->lastOut = x->nextOut;
		if(x->remembertobang)
		{
			
			x->remembertobang = 0;
			outlet_bang(x->outL);
		}
		if(x->lastOut == x->nextOut)
		computeCount(x);
	}
	
		
}
		
		
		
void  computeCount(x)
Lcount *x;
{
	short prevlock;
	
	prevlock = lockout_set(1);
	if(x->floatMode)
	{
		if((x->fendWith > x->fstartWith)&&(x->finterval > 0)) // low to high count forward
		{
			x->fnextOut += x->finterval;
			if(x->fnextOut >= x->fendWith)
			{	
				x->fnextOut -= (x->fendWith - x->fstartWith);
				x->remembertobang = 1;
			}
		}
		if((x->fendWith < x->fstartWith)&&(x->finterval > 0)) //high to low, forward  
		{
			x->fnextOut -= x->finterval;
			if(x->fnextOut <= x->fendWith)
			{
				x->fnextOut += (x->fstartWith - x->fendWith);
				x->remembertobang = 1;
			}
		}
		if((x->fendWith > x->fstartWith)&&(x->finterval < 0)) //low to high, backward 
		{
			x->fnextOut += x->finterval;
			if(x->fnextOut < x->fstartWith)
			{
				x->fnextOut += (x->fendWith - x->fstartWith);
				x->remembertobang = 1;
			}
		}
		if((x->fendWith < x->fstartWith)&&(x->finterval < 0)) // high to low, backward
		{
			x->fnextOut -= x->finterval;
			if(x->fnextOut > x->fstartWith)
			{
				x->fnextOut -= (x->fstartWith - x->fendWith);
				x->remembertobang = 1;
			}
		}
		x->fnextOut = roundoff(x,x->fnextOut);
	}
	else
	{		
		if((x->endWith > x->startWith)&&(x->interval > 0)) // low to high count forward
		{
			x->nextOut += x->interval;
			if(x->nextOut >= x->endWith)
			{	x->nextOut = x->startWith;
				x->remembertobang = 1;
			}
		}
		if((x->endWith < x->startWith)&&(x->interval > 0)) //high to low, forward  
		{
			x->nextOut -= x->interval;
			if(x->nextOut <= x->endWith)
			{
				x->nextOut = x->startWith;
				x->remembertobang = 1;
			}
		}
		if((x->endWith > x->startWith)&&(x->interval < 0)) //low to high, backward 
		{
			x->nextOut += x->interval;
			if(x->nextOut < x->startWith)
			{
				x->nextOut = x->endWith + x->interval;
				x->remembertobang = 1;
			}
		}
		if((x->endWith < x->startWith)&&(x->interval < 0)) // high to low, backward
		{
			x->nextOut -= x->interval;
			if(x->nextOut > x->startWith)
			{
				x->nextOut = x->endWith - x->interval;
				x->remembertobang = 1;
			}
		}
	}
	lockout_set(prevlock);
	if(x->remembertobang)
		outlet_bang(x->outX);
		
	
}

void  lcount_reset(x)
Lcount *x;
{	
	if(x->floatMode)
	{
		x->fnextOut = x->fstartWith;
	}
	else
	{
		x->nextOut = x->startWith;
	}
	x->inletNumber = 0;
	
}

void doInt(x,n)
Lcount *x;
long n;

{
	
	short which;
	which = check_inlet(x);
	if(x->floatMode)	
	{
		if(which)
		{
			switch(which)
			{	
				case 1:   // interval
				x->finterval = (double)n;
				break;
				
				case 2:  // start
				x->fstartWith =(double) n;
				break;
				
				case 3:  // end
				x->fendWith = (double)n;
				break;
			}
			//x->fnextOut = x->flastOut;
			//computeCount(x);
		}
		else
		x->fnextOut = n;
	}
	else
	{
		if(which)
		{
			switch(which)
			{	
				case 1:   // interval
				x->interval = n;
				break;
				
				case 2:  // start
				x->startWith = n;
				break;
				
				case 3:  // end
				x->endWith = n;
				//post("endWith = %ld",x->endWith);
				break;
			}
			//x->nextOut = x->lastOut;
			//computeCount(x);
		}
		else
		x->nextOut = n;
	}
	
	
}

void doFloat(x,n)
Lcount *x;
double n;
{
	short which = check_inlet(x);
	if(x->floatMode)	
	{
		if(which)
		{
			switch(which)
			{	
				case 1:   // interval
				x->finterval = n;
				break;
				
				case 2:  // start
				x->fstartWith = n;
				break;
				
				case 3:  // end
				x->fendWith = n;
				break;
			}
			x->fnextOut = x->flastOut;
			computeCount(x);
		}
		else
		x->fnextOut = n;
	}
	else
	{
		if(which)
		{
			switch(which)
			{	
				case 1:   // interval
				x->interval = (long)n;
				break;
				
				case 2:  // start
				x->startWith = (long) n;
				break;
				
				case 3:  // end
				x->endWith =(long) n;
				//post("endWith = %ld",x->endWith);
				break;
			}
			x->nextOut = x->lastOut;
			computeCount(x);
		}
		else
		x->nextOut =(long) n;
	}
	
	
}

double roundoff(x,n)
Lcount *x;
double n;
{
	
	long temp;
	double result;
	long odd;
	//
	if(n >= 0)
	{
	n = n * x->precision;
	temp = (long)n;
	odd = temp % 2;
	n = n - (double)temp;
	if(n > 0.5) ++temp;
	if(n == 0.5) temp += odd;
	result = (double)temp / x->precision;
	}
	else
	{
	n = - n;
	n = n * x->precision;
	temp = (long)n;
	odd = temp % 2;
	n = n - (double)temp;
	if(n > 0.5) ++temp;
	if(n == 0.5) temp += odd;
	result = (double)temp / x->precision;
	result = - result;
	}
	//
	return(result);
}

void setPrecision(x,n)
Lcount *x;
long n;
{
	int i;
	
	if(n > 8) n = 8;
	if(n < -8) n = -8;	
	x->precision = 1;
	if(n > 0)
	{
	for(i=0;i < n;++i)
		{
		x->precision = 10 * x->precision;
		}
	}
	if(n < 0)
	for(i=n;i < 0;++i)
	{
		{
		x->precision =  x->precision / 10;
		}
	}
}

void lcount_assist(x,b,msg,arg,dst)
Lcount *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Count bangs.");
			break;
			
			case 1:
			strcpy(dst,"Increment");
			break;
			
			case 2:
			strcpy(dst,"Start value");
			break;
			
			case 3:
			strcpy(dst,"End value");
			break;
			
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
			switch (arg)
		{
			case 0:
			strcpy(dst,"Bang on start over.");
			break;
			
			case 1:
			strcpy(dst,"Count output");
			break;
			
			case 2:
			strcpy(dst,"Bang at end");
			break;
		}
	}
	
	
}


void  *lcount_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{
	Lcount  *x;
	
	if(ac > MAXSIZE) ac = MAXSIZE;
	x = (Lcount *)newobject(class);
	x->proxy3 = proxy_new(x,3L,&x->inletNumber);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outX= outlet_new(x,0L);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	x->startWith = 0;
	x->endWith = 0x7FFFFFFF;
	
	x->interval = 0;
	x->nextOut = 0;
	x->fstartWith = 0;
	x->fendWith = 0;
	
	x->finterval = 1;
	x->fnextOut = 0;
	x->floatMode = 0;
	x->remembertobang = 0;
	x->precision = 10000;
	switch(ac)
	{
		case 0:
		break;
		
		case 1:
		if(theArgs[0].a_type == A_LONG)
			x->endWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
		{
			x->fendWith = theArgs[0].a_w.w_float;
			x->floatMode = 1;
		}

		break;
		
		case 2:
		if(theArgs[0].a_type == A_LONG)
			x->startWith = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->fstartWith = theArgs[0].a_w.w_float;
			
		if(theArgs[1].a_type == A_LONG)
			x->endWith = theArgs[1].a_w.w_long;
		if(theArgs[1].a_type == A_FLOAT)
		{
			x->fendWith = theArgs[1].a_w.w_float;
			x->floatMode = 1;
		}
		
		break;
		
		default:
		if(theArgs[0].a_type == A_LONG)
			x->interval = theArgs[0].a_w.w_long;
		if(theArgs[0].a_type == A_FLOAT)
			x->finterval = theArgs[0].a_w.w_float;	
			
		if(theArgs[1].a_type == A_LONG)
			x->startWith = theArgs[1].a_w.w_long;
		if(theArgs[1].a_type == A_FLOAT)
			x->fstartWith = theArgs[1].a_w.w_float;
			
		if(theArgs[2].a_type == A_LONG)
			x->endWith = theArgs[2].a_w.w_long;
		if(theArgs[2].a_type == A_FLOAT)
		{
			x->fendWith = theArgs[2].a_w.w_float;
			x->floatMode = 1;
		}
		break;
		
	}
	if(x->floatMode)
	{
	
		if(x->interval)
			x->finterval = (double)x->interval;
		if(x->startWith)
			x->fstartWith = (double)x->startWith;
		if(x->finterval > 0)
			x->fnextOut = x->fstartWith;
		else
			x->fnextOut = x->fendWith;
	}
	else
	{
		if(x->interval == 0)
			x->interval = 1;
		if(x->interval > 0)
			x->nextOut = x->startWith;
		else
			x->nextOut = x->endWith;
	}
	x->lastOut = x->startWith;
	x->flastOut = x->fstartWith;
	x->end = x->endWith;
	x->fend = x->fendWith;
	
	return(x);
}

void lcount_free(x)
Lcount *x;
{
	
	freeobject(x->proxy1);
	freeobject(x->proxy2);
	freeobject(x->proxy3);
	

}

int check_inlet(x)
Lcount *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}