// lnote file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1996, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 16
#define QWRAP 511
#define QSIZE 512
typedef struct anote
{
	int pitch;
	int channel;
} Anote;

typedef struct pars
{
	long pitch;
	long duration;
	long velocity;
	long channel;
} Pars;

typedef struct lnote
{
	struct object ob;
	void *clock;
	Anote noteQ[QSIZE];
	long timeQ[QSIZE];
	Anote currentnote; 
	int nextoff, nextslot;
	short sustain;
	Pars def;
	void *outR;
	void *outMid;
	void *outL;
} Lnote;


void *lnote_new();
void lnote_free();
void  lnote_List();
void  lnote_Int();
void  lnote_Float();
void lnote_setchannel();
void lnote_setvel();
void lnote_alloff();
void lnote_assist();
void lnote_startnote();
void lnote_stopnote();
void lnote_timeout();
void lnote_setdur();
void lnote_insert();
void lnote_setsustain();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lnote_new,(method)lnote_free,(short)sizeof(struct lnote),0L,A_GIMME,0);
	addint((method)lnote_Int);
	addinx((method)lnote_setdur,2);
	addinx((method)lnote_setchannel,3);
	addinx((method)lnote_setvel,1);
	addfloat((method)lnote_Float);
	addbang((method)lnote_alloff);
	addmess((method)lnote_List, "list",A_GIMME,0);
	addmess((method)lnote_alloff, "flush",0);
	addmess((method)lnote_assist, "assist",A_CANT,0);
	addmess((method)lnote_alloff, "clear",0);
	addmess((method)lnote_setsustain, "sustain",A_DEFLONG,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lnote_startnote(x,pitch,velocity,duration,channel)  //play a noteon and set time for note off
Lnote *x;									//if noteoff is pending, shut off immeadiately
long pitch;
long velocity;
long duration;
long channel;
{
	int i;
	short prevlock;
	short found = 0;
	prevlock = lockout_set(1);
	i = x->nextoff;				// if nextoff == nextslot, no notes are playing
	while(i != x->nextslot)
		{
		if(x->noteQ[i].pitch == (int)pitch && x->noteQ[i].channel == (int)channel)	// if scheduled for shutoff
			{
			found = 1;
			x->noteQ[i].pitch = -1;  	//  make note off a noop
			if(x->sustain == 0){
			outlet_int(x->outR,channel);	// and shut off note now			
			outlet_int(x->outMid,0L);				
			outlet_int(x->outL,pitch);
			}
			break;
			}
		++i;
		i &= QWRAP;
		}
	if(! (found && x->sustain))	{
		outlet_int(x->outR,(long)channel);
		outlet_int(x->outMid,velocity);			// play the new note
		outlet_int(x->outL,(long)pitch);
	}
		// put note off in Queue
	lnote_insert(x, pitch, channel, gettime() + duration);
	
	lockout_set(prevlock);
	if(x->timeQ[x->nextoff])
		clock_set(x->clock,x->timeQ[x->nextoff]);

}

void lnote_insert(x, theNote,theChannel, time)
Lnote *x;
long theNote;
long theChannel;
long time;
{
	int i, here;
	
	// make space in Queue
	if(((x->nextslot + 1) & QWRAP) == x->nextoff)  //noteQ full, boot one
		{
			lnote_stopnote(x);
		}
	// Find insertion point
	
	
	here = x->nextoff;
	
	
	while(x->timeQ[here] && x->timeQ[here] < time)
	{
		if (here == x->nextslot)
			break;
		++here;
		here &= QWRAP;
		
	}
	// Move the existing note offs
	i = x->nextslot;
	while(i != here)
	{
		x->noteQ[i] = x->noteQ[(i-1) & QWRAP];
		x->timeQ[i] = x->timeQ[(i-1) & QWRAP];
		--i;
		i &= QWRAP;
	}
	++x->nextslot;
	x->nextslot &= QWRAP; //insert new data
	x->noteQ[here].pitch = theNote;			// put new noteoff in Q
	x->noteQ[here].channel = theChannel;
	x->timeQ[here] = time;
	
	//post("insert:nextslot = %d nextoff = %d here = %d", x->nextslot, x->nextoff, here);
}

void lnote_stopnote(x)  //turn off oldest note and reset clock
Lnote *x;
{
	
	if(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		if(x->noteQ[x->nextoff].pitch > -1)   // if pitch is -1 it was removed from queue
		{
		outlet_int(x->outR,(long)x->noteQ[x->nextoff].channel);
		outlet_int(x->outMid,0L);
		outlet_int(x->outL,(long)x->noteQ[x->nextoff].pitch);
		}
		++x->nextoff;
		x->nextoff &= QWRAP;
		//post("stopnote:nextoff = %d",x->nextoff);
	}
	if(x->timeQ[x->nextoff])	// time of 0 indicates end of Q
	{
		clock_set(x->clock,x->timeQ[x->nextoff]);
	}
	

}

void lnote_alloff(x)
Lnote *x;
{
	
	while(x->nextoff != x ->nextslot)
	 {
		x->timeQ[x->nextoff] = 0;
		outlet_int(x->outR,(long)x->noteQ[x->nextoff].channel);
		outlet_int(x->outMid,0L);
		outlet_int(x->outL,(long)x->noteQ[x->nextoff++].pitch);
		x->nextoff &= QWRAP;
	}
	clock_unset(x->clock);
	
}


void lnote_List(x,s,argc,argv)
Lnote *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	Atom theList[4];
	
	
	for(i = 0; i < argc && i < 4; ++i)
	{
		theList[i].a_type = A_LONG;
		switch(argv[i].a_type)
		{ 
			case A_LONG:
			theList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_FLOAT:
			theList[i].a_w.w_long = (long)argv[i].a_w.w_float;
			break;
			
			default:
			switch(i)
			{
			case 0:
			post("Inappropriate Argument to Lnote.");
			break;
			
			case 1:
			theList[i].a_w.w_long = x->def.velocity;
			break;
			
			case 2:
			theList[i].a_w.w_long = x->def.duration;
			break;
			
			case 3:
			theList[i].a_w.w_long = x->def.channel;
			break;
			}
		}
		 
	}
	switch(argc)
		{
			case 0:
			break;
			
			case 1:
			lnote_startnote(x,theList[0].a_w.w_long, x->def.velocity,x->def.duration,x->def.channel);
			break;
			
			case 2:
			lnote_startnote(x,theList[0].a_w.w_long,theList[1].a_w.w_long,x->def.duration,x->def.channel);
			break;
			
			case 3:
lnote_startnote(x,theList[0].a_w.w_long,theList[1].a_w.w_long,theList[2].a_w.w_long,x->def.channel);
			break;
			
			default:
lnote_startnote(x,theList[0].a_w.w_long,theList[1].a_w.w_long,theList[2].a_w.w_long,theList[3].a_w.w_long);

		}
	
}

void  lnote_Int(x,n)
Lnote *x;
long n;
{
	
if(n >= 0 && n < 128)
	lnote_startnote(x,n, x->def.velocity,x->def.duration,x->def.channel);
	
}

void  lnote_Float(x,n)
Lnote *x;
double n;
{	
if(n >= 0 && n < 128)
	lnote_startnote(x,(long)n,x->def.velocity, x->def.duration,x->def.channel);
	
}

void lnote_setchannel(x,n)
Lnote *x;
long n;
{
	
	if(n >= 0)
		x->def.channel = n;
	
}

void lnote_setvel(x,n)
Lnote *x;
long n;
{
	
	if(n >= 0)
		x->def.velocity = n;
	
}

void lnote_setsustain(x,n)
Lnote *x;
long n;
{
	
	if(n >= 0)
		x->sustain = n;
	
}

void lnote_setdur(x,n)
Lnote *x;
long n;
{
	
	if(n >= 0)
		x->def.duration = n;
	
}

void lnote_assist(x,b,msg,arg,dst)
Lnote *x;
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
			strcpy(dst,"Velocity");
			break;
			
			case 2:
			strcpy(dst,"Duration");
			break;
			
			default:
			strcpy(dst,"Channel");
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
			strcpy(dst,"Velocity");
			break;
		}
	}
}


void  *lnote_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lnote  *x;
	int i;
	
	
	x = (Lnote *)newobject(class);
	intin(x,3);
	intin(x,2);
	intin(x,1);
	x->outR = intout(x);
	x->outMid = intout(x);
	x->outL = intout(x);
	x->clock = clock_new(x,(method)lnote_stopnote);
	x->nextoff = 0;
	x->nextslot = 0;
	x->sustain = 0;
	for(i = 0; i < QSIZE ;++i)
		x->timeQ[i]=0;
	x->def.velocity = 64;
	x->def.duration = 1000;
	x->def.channel = 1;
	switch(ac)
	{
	case 0:
	break;
	
	case 1:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0)		
		x->def.duration = theArgs[0].a_w.w_long;		
	
	break;
		
	case 2:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
		x->def.velocity = theArgs[0].a_w.w_long;
	if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
		x->def.duration = theArgs[1].a_w.w_long;
	break;
	
	default:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
		x->def.velocity = theArgs[0].a_w.w_long;
	if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
		x->def.duration = theArgs[1].a_w.w_long;
	if(theArgs[2].a_type == A_LONG && theArgs[2].a_w.w_long > 0)			
		x->def.channel = theArgs[2].a_w.w_long;
	}
	
	
	return(x);
}

void lnote_free(x)
Lnote *x;
{
	clock_free(x->clock);
}