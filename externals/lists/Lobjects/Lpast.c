// lpast object 
// by Peter Elsea, University of Californina, Santa Cruz
// Jan 27,2002


#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2
#define UP 1
#define DOWN 0
#define BOTH 2

typedef struct {
struct object ob;

double inLow, inHigh, outLow,outHigh;
double denominator;
double result;

void *proxy2;
void *proxy1;
long inletNumber;
//Atom theList[MAXSIZE];
int listsize;
int mode;
int direction;
int target;
double ftarget;
int resetPoint;
double fresetPoint;
int tripped;
long last;
double lastf;
void *outL;
} Lpast;

void *lpastNew();
void lpastFree();
void doFloatInput();
void doIntInput();
void testList();
void DoTestForFloat();
void DoTestForInt();
void lpastReset();
void lpastAssist();
int setDirection();
int check_inlet();

void *class;


int main(void)
{
	Symbol * s;
	Symbol * l;
	setup((t_messlist **)&class, (method)lpastNew, (method)lpastFree,(short)sizeof(Lpast),0L, A_GIMME, 0);
	addfloat((method)doFloatInput);
	addint((method)doIntInput);
	addmess((method)testList, "list", A_GIMME,0);
	addmess((method)lpastReset, "reset", 0);
	addmess((method)lpastAssist, "assist", A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;

}


void *lpastNew(s, ac,av)
struct symbol *s;
int ac;
struct atom *av;
{
	
	Lpast *x;
	x = (Lpast *)newobject(class);
	
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outL = outlet_new(x,0L);
	
	x->mode = A_LONG;
	x->direction = BOTH;
	x->target = 0;
	x->ftarget = 0;
	x->resetPoint = 0;
	x->fresetPoint = 0;
	x->last = 0;
	x->lastf = 0;
	x->tripped = 0;
	if(ac && (av[0].a_type != A_SYM)){
	
		x->mode = av[0].a_type;
		if(x->mode == A_LONG){
			x->target = av[0].a_w.w_long;
			x->ftarget = (double)x->target;
		}
		else{
			x->ftarget = av[0].a_w.w_float;
			x->target = (long)x->ftarget;
		}
		
		if(ac >1 && av[1].a_type != A_SYM){
			if(av[1].a_type == A_LONG){
				x->resetPoint = av[1].a_w.w_long;
				x->fresetPoint = (double)x->target;
			}
			else{
				x->fresetPoint = av[1].a_w.w_float;
				x->resetPoint = (long)x->ftarget;
			}
		}
		else{
			x->resetPoint = x->target;
			x->fresetPoint = x->ftarget;
		}
	
	setDirection(x);
	
	}
	
	return(x);
}

void doFloatInput(x,n)
Lpast *x;
double n;
{
	
	
	switch(check_inlet(x))
	{
	case 0:
	DoTestForFloat(x,n);
	x->lastf = n;
	break;
	
	case 1:
	x->ftarget = n;
	x->target = (long)n;
	setDirection(x);
	break;
	
	case 2:
	x->fresetPoint = n;
	x->resetPoint = (long)n;
	setDirection(x);
	break;
	
	}
	x->inletNumber = 0;
	


}
void doIntInput(x,n)
Lpast *x;
long n;
{
	
	switch(check_inlet(x))
	{
		case 0:
		DoTestForInt(x,n);
		x->last = n;
		break;
		
		case 1:
		x->target = n;
		x->ftarget = (double)n;
		setDirection(x);
		break;
		
		case 2:
		x->resetPoint = n;
		x->fresetPoint = (double)n;
		setDirection(x);
		break;
	
	}
	x->inletNumber = 0;
	
}	

void testList(x,s,argc,argv)
Lpast *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	for(i = 0; i < argc; ++i)
	{
		if(argv[i].a_type == A_LONG){
			doIntInput(x,argv[i].a_w.w_long);
			
			}
		if(argv[i].a_type == A_FLOAT){
			doFloatInput(x,argv[i].a_w.w_float);
			
			}
	}
	x->inletNumber = 0;
	
}


int setDirection(x)
Lpast *x;
{
	if(x->mode == A_LONG){
		if(x->resetPoint < x->target) x->direction = UP;
		else
			if(x->resetPoint > x->target) x->direction = DOWN;
			else
				if(x->resetPoint == x->target) x->direction = BOTH;
	}
	else{
		if(x->fresetPoint < x->ftarget) x->direction = UP;
		else
			if(x->fresetPoint > x->ftarget) x->direction = DOWN;
			else
				if(x->fresetPoint == x->ftarget) x->direction = BOTH;
	}

	return(x->direction);

}


void DoTestForFloat(x, n)
Lpast *x;
double n;
{
	
	
	switch(x->direction)
	{	case UP:
		if(n <= x->fresetPoint) x->tripped = 0;
		if((n >= x->ftarget) && (x->tripped == 0)){
			x->tripped = 1;
			outlet_bang(x->outL);
		}
		break;
	
		case DOWN:
		if(n >= x->fresetPoint) x->tripped = 0;
		if((n <= x->ftarget) && (x->tripped == 0)){
			x->tripped = 1;
			outlet_bang(x->outL);
		}
		break;
		
		case BOTH:
		if((x->lastf <= x->ftarget) && n > x->ftarget)
			outlet_bang(x->outL);
		else
		if((x->lastf >= x->ftarget) && n < x->ftarget)
			outlet_bang(x->outL);
	}
		
	
}

void DoTestForInt(x, n)
Lpast *x;
long n;
{
	
	
	switch(x->direction)
	{	case UP:
		if(n <= x->resetPoint) x->tripped = 0;
		if((n >= x->target) && (x->tripped == 0)){
			x->tripped = 1;
			outlet_bang(x->outL);
		}
		break;
	
		case DOWN:
		if(n >= x->resetPoint) x->tripped = 0;
		if((n <= x->target) && (x->tripped == 0)){
			x->tripped = 1;
			outlet_bang(x->outL);
		}
		break;
		
		case BOTH:
		if((x->last <= x->target) && n > x->target)
			outlet_bang(x->outL);
		else
		if((x->last >= x->target) && n < x->target)
			outlet_bang(x->outL);
	}
		
	
}

void lpastReset(x)
Lpast *x;
{
	
	x->tripped = 0;
	//post("reset");
	

}


void lpastAssist(x,b,msg,arg,dst)
Lpast *x;
void *b;
long  msg,arg;
char *dst;
{
	
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Value to test");
			break;
			
			case 1:
			strcpy(dst,"Target point");
			break;
			
			case 2:
			strcpy(dst,"Reset point");
			break;
			
			
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			if(x->direction)
			strcpy(dst,"bang if above target and has been below reset");
			else
			strcpy(dst,"bang if below target and has been above reset");
			break;
			
			default:
			strcpy(dst,"OutR");
			break;
		}
	}
	
	
}

void lpastFree(x)
Lpast *x;
{
	freeobject(x->proxy2);
	freeobject(x->proxy1);
	
}


int check_inlet(x)
Lpast *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}