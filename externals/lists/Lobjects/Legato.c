// legato file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 256

typedef struct legato
{
	struct object ob;
	void *clock;
	int noteQ[128];
	long timeQ[128];
	long currentnote; 
	int nextoff, nextslot;
	long vel,overlap;
	long dur;
	void *outR;
	void *outL;
	void *durclock;
} Legato;

void *legato_new();
void legato_free();
void  legato_List();
void  legato_Int();
void  legato_Float();
void legato_setoverlap();
void legato_setvel();
void legato_alloff();
void  legato_cutoff();
void legato_assist();
void legato_startnote();
void legato_stopnote();
void legato_timeout();
void legato_setdur();
int check_inlet();
void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)legato_new,(method)legato_free,(short)sizeof(struct legato),0L,A_GIMME,0);
	addint((method)legato_Int);
	addinx((method)legato_setdur,2);
	addinx((method)legato_setoverlap,3);
	addinx((method)legato_setvel,1);
	addfloat((method)legato_Float);
	addmess((method)legato_List, "list",A_GIMME,0);
	addmess((method)legato_alloff, "flush",0);
	addmess((method)legato_cutoff, "off",0);
	addmess((method)legato_assist, "assist",A_CANT,0);
	addmess((method)legato_alloff, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void legato_startnote(x,thenote,velocity)  //play a noteon and set time for note off
Legato *x;									//if noteoff is pending, shut off immeadiately
long thenote;
long velocity;
{
	int i;
	if(velocity)
		{
		if(x->currentnote)   // if  a note is playing turn it off
			{
				if(x->overlap) //or schedule it for turnoff
				{
					
					if(((x->nextslot + 1) & 127) == x->nextoff)  //noteQ full
						{
							legato_stopnote(x);
						}
					
					x->noteQ[x->nextslot]= (int)x->currentnote;
					x->timeQ[x->nextslot++] = gettime() + x->overlap;
					x->nextslot &= 127;
					//post("nextslot = %d", x->nextslot);
					if(x->timeQ[x->nextoff])
						clock_set(x->clock,x->timeQ[x->nextoff]);
				
				}
				else
				{
					outlet_int(x->outR,0L);
					outlet_int(x->outL,x->currentnote);
					
				}
			}
		i = x->nextoff;
		while(i != x->nextslot)
			{
			if(x->noteQ[i] == thenote)				// if scheduled for shutoff
				{
				x->noteQ[i] = 0;  					//  make note off a noop
				outlet_int(x->outR,0L);				// and shut off note now
				outlet_int(x->outL,thenote);
				break;
				}
			++i;
			i &= 127;
			}
		outlet_int(x->outR,velocity);			// play the new note
		outlet_int(x->outL,thenote);
		x->currentnote = thenote;
		clock_delay(x->durclock,x->dur);
	}
	else
	{// incoming note offs mean legato is used as a filter
		if(thenote == x->currentnote)		// if it is playing		
		{							
			i = x->nextoff;					// clear it from the queue
			while(i != x->nextslot)
			{
			if(x->noteQ[i] == thenote)				// if scheduled for shutoff
				{
				x->noteQ[i] = 0;  					//  make note off a noop
				break;
				}
			++i;
			i &= 127;
			}
			legato_cutoff(x);				// and shut off note now
		}										// otherwise ignore it
	}
}


void legato_stopnote(x)  //turn off oldest note and reset clock
Legato *x;
{
	
	if(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		if(x->noteQ[x->nextoff])   // if note is 0 it was removed from queue
		{
		outlet_int(x->outR,0L);
		outlet_int(x->outL,(long)x->noteQ[x->nextoff]);
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

void legato_alloff(x)
Legato *x;
{
	
	while(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		outlet_int(x->outR,0L);
		outlet_int(x->outL,(long)x->noteQ[x->nextoff++]);
		x->nextoff &= 127;
	}
	clock_unset(x->clock);
	outlet_int(x->outR,0L);
	outlet_int(x->outL,x->currentnote);
	x->currentnote =0;
	
}

void  legato_cutoff(x)
Legato *x;
{
	
	if(x->currentnote)
	{
	outlet_int(x->outR,0L);
	outlet_int(x->outL,(long)x->currentnote);
	x->currentnote = 0;
	clock_unset(x->durclock);
	}
	
}

void legato_List(x,s,argc,argv)
Legato *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	Atom tempList[4];
	
	
	if(argc > 4) argc = 4;
	for(i=0; i<argc; ++i)
		{
		tempList[i].a_type = A_LONG;
		switch(argv[i].a_type)
			{
			case A_LONG:
			tempList[i].a_w.w_long = argv[i].a_w.w_long;
			break;
			
			case A_SYM:
			tempList[i].a_w.w_long = 0;
			break;
			
			case A_FLOAT:
			tempList[i].a_w.w_long = (long)argv[i].a_w.w_float;
			}
		}
	switch(argc)
		{
			case 0:
			break;
			
			case 1:
			legato_startnote(x,tempList[0].a_w.w_long, x->vel);
			break;
			
			case 2:
			legato_startnote(x,tempList[0].a_w.w_long, tempList[1].a_w.w_long);
			break;
			
			default:
			if((argc > 2) && (tempList[2].a_w.w_long > 0))
				x->dur = tempList[2].a_w.w_long;
			if((argc > 3) && (tempList[3].a_w.w_long > 0))
				x->overlap = tempList[3].a_w.w_long;
			
			if(tempList[1].a_w.w_long == 0)
				{
				outlet_int(x->outR,0L);
				outlet_int(x->outL,tempList[0].a_w.w_long);
				if(tempList[0].a_w.w_long == x->currentnote)
					x->currentnote = 0;
				else						// it may be in overlap phase
					{
					i = x->nextoff;
					while(i != x->nextslot)
						{
						if(x->noteQ[i] == (int)tempList[0].a_w.w_long)
							{
							x->noteQ[i] = 0;  //  make note off a noop
							break;
							}
						++i;
						i &= 127;
						}
					}
				}
			else
				legato_startnote(x,tempList[0].a_w.w_long, tempList[1].a_w.w_long);
			break;
		}
	
}

void  legato_Int(x,n)
Legato *x;
long n;
{
	
	legato_startnote(x,n, x->vel);
	
}

void  legato_Float(x,n)
Legato *x;
double n;
{	
	legato_startnote(x,(long)n, x->vel);
	
}

void legato_setoverlap(x,n)
Legato *x;
long n;
{
	
	if(n >= 0)
		x->overlap = n;
	
}

void legato_setvel(x,n)
Legato *x;
long n;
{
	
	if(n >= 0)
		x->vel = n;
	
}

void legato_setdur(x,n)
Legato *x;
long n;
{
	
	if(n >= 0)
		x->dur = n;
	
}

void legato_assist(x,b,msg,arg,dst)
Legato *x;
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
			strcpy(dst,"Overlap");
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


void  *legato_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Legato  *x;
	int i;
	
	
	x = (Legato *)newobject(class);
	intin(x,3);
	intin(x,2);
	intin(x,1);
	x->outR = intout(x);
	x->outL = intout(x);
	x->clock = clock_new(x,(method)legato_stopnote);
	x->durclock = clock_new(x,(method)legato_cutoff);
	x->nextoff = 0;
	x->nextslot = 0;
	x->currentnote = 0;
	for(i = 0; i<128;++i)
		x->timeQ[i]=0;
	x->vel = 64;
	x->dur = 1000;
	x->overlap = 0;
	switch(ac)
	{
	case 0:
	break;
	
	case 1:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0)		
		x->dur = theArgs[0].a_w.w_long;		
	
	break;
		
	case 2:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
		x->vel = theArgs[0].a_w.w_long;
	if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
		x->dur = theArgs[1].a_w.w_long;
	break;
	
	default:
	if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
		x->vel = theArgs[0].a_w.w_long;
	if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
		x->dur = theArgs[1].a_w.w_long;
	if(theArgs[2].a_type == A_LONG && theArgs[2].a_w.w_long > 0)			
		x->overlap = theArgs[2].a_w.w_long;
	}
	
	
	return(x);
}

void legato_free(x)
Legato *x;
{	
	clock_free(x->clock);
}

