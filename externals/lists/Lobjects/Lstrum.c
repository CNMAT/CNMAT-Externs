// strum file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 128
#define UP 0
#define DOWN 1
#define ALTERNATE 2
#define OFF 0
#define ON 1

typedef struct strum
{
	struct object ob;
	void *clock;
	void *proxy1;
	void *proxy2;
	void *proxy3;
	Atom theList[MAXSIZE];
	long noteQ[128];
	long timeQ[128];
	long currentnote; 
	int nextoff, nextslot;
	int direction, mode, harp;
	int listsize,nextnote;
	long vel,rate;
	long dur;
	long inletNumber;
	void *outR;
	void *outL;
	void *strumclock;
} Strum;


void *strum_new();
void strum_free();
void  strum_List();
void  strum_Bang();
void  strum_Int();
void  strum_Float();
void strum_setrate();
void strum_setvel();
void strum_alloff();
void  strum_mute();
void  strum_up();
void  strum_down();
void  strum_updown();
void strum_assist();
void strum_startnote();
void strum_stopnote();
void strum_timeout();
void strum_setdur();
void strum_strumnote();
void strum_harpOn();
void strum_harpOff();
void lstrum_insert();
int purge();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)strum_new,(method)strum_free,(short)sizeof(struct strum),0L,A_GIMME,0);
	addint((method)strum_Int);
	addfloat((method)strum_Float);
	addbang((method)strum_Bang);
	addmess((method)strum_List, "list",A_GIMME,0);
	addmess((method)strum_alloff, "flush",0);
	addmess((method)strum_harpOn, "harp",A_DEFLONG,0);
	addmess((method)strum_harpOff, "pick",A_DEFLONG,0);
	addmess((method)strum_mute, "mute",A_GIMME,0);
	addmess((method)strum_up, "up",A_GIMME,0);
	addmess((method)strum_down, "down",A_GIMME,0);
	addmess((method)strum_updown, "updown",A_GIMME,0);
	addmess((method)strum_assist, "assist",A_CANT,0);
	addmess((method)strum_alloff, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void strum_startnote(x,thenote,velocity)  //play a noteon and set time for note off
Strum *x;									//if noteoff is pending, shut off immeadiately
long thenote;
long velocity;
{
	purge(x,thenote);
	
	outlet_int(x->outR,velocity);			// play the new note
	outlet_int(x->outL,thenote);
	
											// schedule shutoff
	if(((x->nextslot + 1) & 127) == x->nextoff)  //noteQ full
		{
			strum_stopnote(x);
		}
	x->currentnote = thenote;
	lstrum_insert(x, thenote, gettime() + x->dur);
	//post("nextslot = %d", x->nextslot);
	if(x->timeQ[x->nextoff])
	clock_set(x->clock,x->timeQ[x->nextoff]);

}

void lstrum_insert(x, theNote, time)
Strum *x;
long theNote;
long time;
{
	int i, here;
	short prevlock;
	prevlock = lockout_set(1);
	// make space in Queue
	if(((x->nextslot + 1) & 127) == x->nextoff)  //noteQ full
	{
		strum_stopnote(x);
	}
	// Find insertion point
	here = x->nextoff;
	++x->nextslot;
	x->nextslot &= 127;
	while(x->timeQ[here] && x->timeQ[here]< time)
	{
		++here;
		here &= 127;
		if (here == x->nextslot)
			break;
	}
	// Move the existing note offs
	i = x->nextslot;
	while(i != here)
	{
		x->noteQ[i] = x->noteQ[(i-1) & 127];
		x->timeQ[i] = x->timeQ[(i-1) & 127];
		--i;
		i &= 127;
	}
	//insert new data
	x->noteQ[here]= theNote;
	x->timeQ[here] = time;
	lockout_set(prevlock);
	
}

void strum_strumnote(x)
Strum *x;
{
	
	if(x->listsize)
	{
	if(x->rate > 0)
		{
		if((x->nextnote < x->listsize) && (x->nextnote >= 0))
			{
			strum_startnote(x,x->theList[x->nextnote].a_w.w_long,x->vel);
			if(x->direction == DOWN)
				--x->nextnote;
			else
				++x->nextnote;
			if((x->nextnote < x->listsize) && (x->nextnote >= 0))
				clock_delay(x->strumclock,x->rate);
			}
		}
		else
		{
		while((x->nextnote < x->listsize) && (x->nextnote >= 0))
			{
			strum_startnote(x,x->theList[x->nextnote].a_w.w_long,x->vel);
			if(x->direction == DOWN)
				--x->nextnote;
			else
				++x->nextnote;
			}
		
		}
	}
	
}

int purge(x,thenote)
Strum *x;
long thenote;
{
	int i;
	int result;
	
	result = 0;
	i = x->nextoff;
	while(i != x->nextslot)
		{
		if(x->noteQ[i] == thenote)				// if scheduled for shutoff
			{
			x->noteQ[i] = 0;  					//  make note off a noop
			outlet_int(x->outR,0L);				// and shut off note now
			outlet_int(x->outL,thenote);
			result = 1;
			break;
			}
		++i;
		i &= 127;
		}
	return(result);
}

void strum_stopnote(x)  //turn off oldest note and reset clock
Strum *x;
{
	
	if(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		if(x->noteQ[x->nextoff])   // if note is 0 it was removed from queue
		{
		outlet_int(x->outR,0L);
		outlet_int(x->outL,x->noteQ[x->nextoff]);
		}
		++x->nextoff;
		x->nextoff &= 127;
		//post("nextoff = %d",x->nextoff);
	}
	if(x->timeQ[x->nextoff])
	{
		clock_set(x->clock,x->timeQ[x->nextoff]);
	}
	

}



void strum_alloff(x)
Strum *x;
{
	
	while(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		outlet_int(x->outR,0L);
		outlet_int(x->outL,x->noteQ[x->nextoff++]);
		x->nextoff &= 127;
	}
	clock_unset(x->clock);
	x->currentnote =0;
	
}



void strum_List(x,s,argc,argv)
Strum *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	
	
	for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{ 
			case A_LONG:
			x->theList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			x->theList[i].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			default:
			post("Inapropriate List Member in Lstrum");
			}
		}
	for(i=argc;i<MAXSIZE;++i)
		{
		x->theList[i].a_type = A_LONG;
		x->theList[i].a_w.w_long = 0;
		}
	if(argc)
		x->listsize = argc;
	
	if(check_inlet(x) == 0)
		{
		strum_Bang(x);
		}
	x->inletNumber = 0;
	
}

void  strum_Bang(x)
Strum *x;
{
	
		switch(x->mode)
			{
			case UP:
			x->nextnote = 0;
			x->direction = UP;
			break;
			
			case DOWN:
			x->nextnote = x->listsize-1;
			x->direction = DOWN;
			break;
			
			case ALTERNATE:
			if(x->direction)
				x->nextnote = 0;
			else
				x->nextnote = x->listsize-1;
			x->direction = !x->direction;
			break; 
			}
		strum_strumnote(x);

		x->inletNumber = 0;
	
}


void  strum_mute(x,s,argc,argv)
Strum *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	
	if(argc == 0)
		strum_alloff(x);
	else
		{
		for(i = 0;i<argc;++i)
			purge(x,argv[i].a_w.w_long);
		}
	x->inletNumber = 0;
	
}

void  strum_up(x,s,argc,argv)
Strum *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = UP;
	if(argc)
	strum_List(x,s,argc,argv);
	
}

void  strum_down(x,s,argc,argv)
Strum *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = DOWN;
	if(argc)
	strum_List(x,s,argc,argv);
	
}

void  strum_updown(x,s,argc,argv)
Strum *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = ALTERNATE;
	if(argc)
	strum_List(x,s,argc,argv);
	
}


void  strum_Int(x,n)
Strum *x;
long n;
{
	
	switch(check_inlet(x))
	{
	case 0:
	if(x->harp == OFF)
		purge(x,x->currentnote);
	strum_startnote(x,n, x->vel);
	break; 
	
	case 1:
	strum_setvel(x,n);
	break;
	
	case 2:
	strum_setdur(x,n);
	break;
	
	case 3:
	strum_setrate(x,n);
	break;
	
	}
	x->inletNumber = 0;
	
}

void  strum_Float(x,n)
Strum *x;
double n;
{
	strum_Int(x,(long)n);
	
}

void strum_setrate(x,n)
Strum *x;
long n;
{
	
	if(n >= 0)
		x->rate = n;
	
}

void strum_setvel(x,n)
Strum *x;
long n;
{
	
	if(n >= 0 && n < 128)
		x->vel = n;
	
}

void strum_setdur(x,n)
Strum *x;
long n;
{
	
	if(n > 0)
		x->dur = n;
	
	
}

void strum_harpOn(x,n)
Strum *x;
long n;
{
	
	x->inletNumber = 0;
	if(n)
		strum_Int(x,n);
	x->harp = ON;
	
}



void strum_harpOff(x,n)
Strum *x;
long n;
{
	
	x->inletNumber = 0;
	if(n)
		strum_Int(x,n);
	x->harp = OFF;
	
}

void strum_assist(x,b,msg,arg,dst)
Strum *x;
void *b;
long  msg,arg;
char *dst;
{
	
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Pitch");
			break;
			
			case 1:
			strcpy(dst,"velocity");
			break;
			
			case 2:
			strcpy(dst,"Duration");
			break;
			
			default:
			strcpy(dst,"Rate");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Pitch");
			break;
			
			default:
			strcpy(dst,"velocity");
			break;
		}
	}
}


void  *strum_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Strum  *x;
	int i;
	
	
	x = (Strum *)newobject(class);
	
	x->proxy3 = proxy_new(x,3L,&x->inletNumber);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outR = intout(x);
	x->outL = intout(x);
	x->clock = clock_new(x,(method)strum_stopnote);
	x->strumclock = clock_new(x,(method)strum_strumnote);
	x->nextoff = 0;
	x->nextslot = 0;
	
	x->currentnote = 0;
	for(i = 0; i<128;++i)
		x->timeQ[i]=0;
	x->vel = 64;
	x->dur = 1000;
	x->rate = 2;
	x->mode = UP;
	x->direction = UP;
	x->harp = OFF;
	x->inletNumber = 0;
	switch(ac)
	{
	
	default:
	if(theArgs[2].a_type == A_LONG && theArgs[2].a_w.w_long > 0)			
		x->rate = theArgs[2].a_w.w_long;
	case 2:
	if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
		x->dur = theArgs[1].a_w.w_long;
	case 1:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0)			
		x->vel = theArgs[0].a_w.w_long;		

	case 0:
	break;	
	
	}
	for(i = 0; i<MAXSIZE; ++i)
		x->theList[i].a_type = A_LONG;
	x->listsize = 0;
	
	
	return(x);
}

void strum_free(x)
Strum *x;
{
	
	clock_free(x->clock);
	clock_free(x->strumclock);
}

int check_inlet(x)
Strum *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}