// lperc file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include "ext_wind.h"
#include "edit.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 16
#define UP 0
#define DOWN 1
#define ALTERNATE 2
#define OFF 0
#define ON 1
#define NONE 0
#define INT 1
#define LIST 2
#define ROLL 3
#define TREM 4

typedef struct lperc
{
	struct object ob;
	struct ed *edWin;
	char **textHandle;
	char saveName[32];
	Symbol *fn;
	short vol;
	short bin;
	OSType dstType;
	void *clock;
	void *proxy1;
	void *proxy2;
	void *proxy3;
	Atom theList[MAXSIZE];
	Atom tremList[MAXSIZE];
	long noteQ[128];
	long timeQ[128];
	long link[128];
	long dur[128];
	long vel[128];
	long rate[128];
	long currentnote; 
	int nextoff, nextslot;
	int direction, mode;
	int listsize,nextnote,theListSize;
	long rollLength;
	long rollPitch;
	long rollVel;
	long rollDur;
	long rollRate;
	int rollOn;
	long tremDur;
	int tremOn;
	long defVel,defRate;
	long defDur;
	long inletNumber;
	int lastEvent;
	long lastPitch;
	void *outR;
	void *outL;
	void *rollclock;
	void *tremclock;
	void *rolltimer;
	void *tremtimer;
	short pathID;
} Lperc;


void *lperc_new();
void lperc_free();
void  lperc_List();
void  lperc_Bang();
void  lperc_Int();
void  lperc_Float();
void lperc_setdefRate();
void lperc_setdefVel();
void lperc_setdefDur();
void lperc_setVel();
void lperc_setDur();
void lperc_setRate();
void lperc_alloff();
void  lperc_mute();
void  lperc_stop();
void  lperc_up();
void  lperc_down();
void  lperc_updown();
void lperc_assist();
void lperc_startnote(Lperc *x,long theNote,long Velocity,long Duration); 
void lperc_stopnote();
void lperc_timeout();

void lperc_startRoll(Lperc *x,long Duration,long Pitch,long Velocity,long noteDur,long Rate);
void lperc_rollNote();
void lperc_rollEnd();
void  lperc_startTrem();
void lperc_tremList();
void lperc_tremNote();
void lperc_tremEnd();
void lperc_linkOn();
void lperc_linkOff();
void lperc_clear();
void lperc_edit();
void lperc_edclose();
void bufToPerc();
int purge();
void waitToEdit();
void loadfile();
long convert();
void lperc_insert();
int check_inlet();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lperc_new,(method)lperc_free,(short)sizeof(struct lperc),0L,A_GIMME,0);
	addint((method)lperc_Int);
	addfloat((method)lperc_Float);
	addbang((method)lperc_Bang);
	addmess((method)lperc_List, "list",A_GIMME,0);
	addmess((method)lperc_tremList, "trem",A_GIMME,0);
	addmess((method)lperc_alloff, "flush",0);
	addmess((method)lperc_linkOn, "link",A_DEFLONG,A_DEFLONG,0);
	addmess((method)lperc_setVel, "vel",A_DEFLONG,A_DEFLONG,0);
	addmess((method)lperc_setDur, "dur",A_DEFLONG,A_DEFLONG,0);
	addmess((method)lperc_setRate, "rate",A_DEFLONG,A_DEFLONG,0);
	addmess((method)lperc_startRoll, "roll",A_DEFLONG,A_DEFLONG,A_DEFLONG,A_DEFLONG,A_DEFLONG,0);
	addmess((method)lperc_linkOff, "linkoff",A_DEFLONG,0);
	addmess((method)lperc_stop, "stop",0);
	addmess((method)lperc_mute, "mute",A_GIMME,0);
	addmess((method)lperc_up, "up",A_GIMME,0);
	addmess((method)lperc_down, "down",A_GIMME,0);
	addmess((method)lperc_updown, "updown",A_GIMME,0);
	addmess((method)waitToEdit, "dblclick",0);
	addmess((method)lperc_edclose, "edclose",A_CANT,0);
	addmess((method)loadfile, "read", A_DEFSYM,0);
	addmess((method)lperc_assist, "assist",A_CANT,0);
	addmess((method)lperc_clear, "clear",0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
		post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
		s->s_thing = (struct object *)l;}
	return 0;
}


void lperc_startnote(Lperc *x,long theNote,long Velocity,long Duration)  //play a noteon and set time for note off
{
	
	if(Velocity == 0)
		Velocity = x->vel[theNote];
	if(Velocity == 0)
		Velocity = x->defVel;
	if(x->link[theNote])
		purge(x,x->link[theNote]);
	purge(x,theNote);
	outlet_int(x->outR,Velocity);			// play the new note
	outlet_int(x->outL,theNote);
	
											// schedule shutoff
	
	x->currentnote = theNote;
	
	if(Duration == 0)
		Duration = x->dur[theNote];
	if(Duration == 0)
		Duration = x->defDur;
	lperc_insert(x, theNote, gettime() + Duration);
	
	//post("nextslot = %d", x->nextslot);
	if(x->timeQ[x->nextoff])
		clock_set(x->clock,x->timeQ[x->nextoff]);

}

void lperc_insert(x, theNote, time)
Lperc *x;
long theNote;
long time;
{
	int i, here;
	short prevlock;
	prevlock = lockout_set(1);
	// make space in Queue
	if(((x->nextslot + 1) & 127) == x->nextoff)  //noteQ full
	{
		lperc_stopnote(x);
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


void lperc_tremNote(x)
Lperc *x;
{
	long velocity;
	
	if(x->listsize)
	{
	if(x->defRate > 0)
		{
		if((x->nextnote < x->listsize) && (x->nextnote >= 0))
			{
			if((velocity = x->vel[x->tremList[x->nextnote].a_w.w_long]) == 0)
				velocity = x->defVel;
			lperc_startnote(x,x->tremList[x->nextnote].a_w.w_long,velocity,0L);
			if(x->direction == DOWN)
				{
				--x->nextnote;
				if(x->nextnote < 1)
					x->nextnote = x->listsize - 1;
				}
			else
				{
				++x->nextnote;
				if(x->nextnote == x->listsize)
					x->nextnote = 1;
				}
			if(x->tremOn)
				clock_delay(x->tremclock,x->defRate);
			}
		}
		else
		{
		while((x->nextnote < x->listsize) && (x->nextnote > 0))
			{
			if((velocity = x->vel[x->tremList[x->nextnote].a_w.w_long]) == 0)
				velocity = x->defVel;
			lperc_startnote(x,x->tremList[x->nextnote].a_w.w_long,velocity,x->tremList[0].a_w.w_long);
			if(x->direction == DOWN)
				--x->nextnote;
			else
				++x->nextnote;
			}
		
		}
	}
	
}

void lperc_startRoll(Lperc *x,long Duration,long Pitch,long Velocity,long noteDur,long Rate)
{
	
	
		if(Duration > 0)
			x->rollLength = Duration;
			
		if(Pitch >= 0 && Pitch < 128)
			{
			x->rollPitch = Pitch;
			
			if(Velocity >= 0 && Velocity < 128)
				x->rollVel = Velocity;
		
			if(noteDur >= 0)
				x->rollDur = noteDur;
	
			if(Rate >= 0)
				x->rollRate = Rate;
				
			x->rollOn = ON;
			clock_delay(x->rolltimer,x->rollLength);
			lperc_rollNote(x);
			x->lastEvent = ROLL;
			}
		
	
}


void lperc_rollNote(x)
Lperc *x;
{
	long Velocity, Duration,Rate;
	
	
	Velocity = x->rollVel;
	if(Velocity == 0)
		Velocity = x->vel[x->rollPitch];
	if(Velocity == 0)
		Velocity = x->defVel;
			
	Duration = x->rollDur;
	if(Duration == 0)
		Duration = x->dur[x->rollPitch];
	if(Duration == 0)
		Duration = x->defDur;
			
	Rate = x->rollRate;
	if(Rate == 0)
		Rate = x->rate[x->rollPitch];
	if(Rate == 0)
		Rate = x->defRate;
		
	if(Duration==Rate)
		--Duration;

	lperc_startnote(x,x->rollPitch,Velocity,Duration);
	
	if(Rate > 0)
		{		
		if(x->rollOn)
			clock_delay(x->rollclock,Rate);
		}

	
	
}


void lperc_rollEnd(x)
Lperc *x;
{
	
	
	x->rollOn = OFF;
	
	
}

void lperc_tremEnd(x)
Lperc *x;
{
	
	
	x->tremOn = OFF;
	
	
}

int purge(x,thenote)
Lperc *x;
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

void lperc_stopnote(x)  //turn off oldest note and reset clock
Lperc *x;
{
	
	
	if(x->nextoff != x ->nextslot)
	{
		x->timeQ[x->nextoff] = 0;
		if(x->noteQ[x->nextoff])   // if note is 0 it was removed from queue
		{
		evnum_incr();
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

void lperc_alloff(x)
Lperc *x;
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



void lperc_List(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	long pitch, velocity, duration;
	Atom tempList[4];
	int i;
	
	
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
	velocity = duration = 0;
	if((argc) && (tempList[0].a_w.w_long > 0))
		{
		pitch = tempList[0].a_w.w_long;
		if((argc > 1) && (tempList[1].a_w.w_long >= 0) && (tempList[1].a_w.w_long < 128))
			velocity = tempList[1].a_w.w_long;
				
			if((argc > 2) && (tempList[2].a_w.w_long > 0))
				duration = tempList[2].a_w.w_long;
				
			lperc_startnote(x,pitch, velocity, duration);
		x->lastEvent = LIST;
		x->theList[0].a_w.w_long = pitch;
		x->theList[1].a_w.w_long = velocity;
		x->theList[2].a_w.w_long = duration;
		x->theListSize = 3;
		}
	x->inletNumber = 0;
	
}

void lperc_tremList(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	

	if(argc)
		{
		for(i=0;i<argc;++i)
			{
			x->tremList[i].a_type = A_FLOAT;
			switch(argv[i].a_type)
				{ 
				case A_LONG:
				x->tremList[i].a_w.w_long = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				x->tremList[i].a_w.w_long = (long)argv[i].a_w.w_float;
				break;
				
				default:
				post("Inappropriate List Member in Lperc");
				x->tremList[i].a_w.w_long = 0;
				}
			}
		
		x->listsize = argc;
		x->lastEvent = TREM;
		}
	if(check_inlet(x) == 0)
		{
		
		lperc_startTrem(x);
		}
	x->inletNumber = 0;
	
}

void  lperc_Bang(x)
Lperc *x;
{

	
	x->inletNumber = 0;
	switch(x->lastEvent)
		{
		case NONE:
		break;
		
		case INT:
		lperc_startnote(x,x->lastPitch, 0L, 0L);
		break;
		
		case LIST:
		lperc_List(x,0L,x->theListSize,x->theList);
		break;
		
		case ROLL:
		x->rollOn = ON;
		clock_delay(x->rolltimer,x->rollLength);
		lperc_rollNote(x);
		break;
		
		case TREM:
		lperc_tremList(x,0L,x->listsize,x->tremList);
		}
	
}




void  lperc_startTrem(x)
Lperc *x;
{
	
		switch(x->mode)
			{
			case UP:
			x->nextnote = 1;			// duration is in tremList[0]
			x->direction = UP;
			break;
			
			case DOWN:
			x->nextnote = x->listsize-1;
			x->direction = DOWN;
			break;
			
			case ALTERNATE:
			if(x->direction)
				x->nextnote = 1;
			else
				x->nextnote = x->listsize-1;
			x->direction = !x->direction;
			break; 
			}
		if(x->rate)
			{
			x->tremOn = ON;
			clock_delay(x->tremtimer,x->tremList[0].a_w.w_long);
			}
		lperc_tremNote(x);

		x->inletNumber = 0;
	
}


void  lperc_mute(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i,j;
	
	if(argc == 0)
		{
		
		x->rollOn = OFF;
		clock_unset(x->rollclock);
		x->tremOn = OFF;
		clock_unset(x->tremclock);
		lperc_alloff(x);
		}
	else
		{
		for(i = 0;i<argc;++i)
			{
			
			if(x->rollPitch ==argv[i].a_w.w_long)
				{
				x->rollOn = OFF;
				clock_unset(x->rollclock);
				}
			for(j = 0;j<x->listsize;++j)
				{
				if(x->tremList[j].a_w.w_long ==argv[i].a_w.w_long)
					{
					x->tremOn = OFF;
					clock_unset(x->tremclock);
					}
				}
			purge(x,argv[i].a_w.w_long);
			}
		}
	x->inletNumber = 0;
	
}

void  lperc_stop(x,s)
Lperc *x;
Symbol *s;
{
	
		
	x->rollOn = OFF;
	clock_unset(x->rollclock);
	x->tremOn = OFF;
	clock_unset(x->tremclock);
	x->inletNumber = 0;
	
}

void  lperc_up(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = UP;
	if(argc)
	lperc_tremList(x,s,argc,argv);
	
}

void  lperc_down(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = DOWN;
	if(argc)
	lperc_tremList(x,s,argc,argv);
	
}

void  lperc_updown(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	
	x->inletNumber = 0;
	x->mode = ALTERNATE;
	if(argc)
	lperc_tremList(x,s,argc,argv);
	
}


void  lperc_Int(x,n)
Lperc *x;
long n;
{
	
	
	switch(check_inlet(x))
	{
	case 0:
	lperc_startnote(x,n, 0L, 0L);
	x->lastEvent = INT;
	x->lastPitch = n;
	break; 
	
	case 1:
	lperc_setdefVel(x,n);
	break;
	
	case 2:
	lperc_setdefDur(x,n);
	break;
	
	case 3:
	lperc_setdefRate(x,n);
	break;
	
	}
	x->inletNumber = 0;
	
}

void  lperc_Float(x,n)
Lperc *x;
double n;
{	
	lperc_Int(x,(long)n);
	
}

void lperc_setdefRate(x,n)
Lperc *x;
long n;
{
	
	if(n >= 0)
		x->defRate = n;
	
}

void lperc_setdefVel(x,n)
Lperc *x;
long n;
{
	
	if(n >= 0 && n < 128)
		x->defVel = n;
	
}

void lperc_setdefDur(x,n)
Lperc *x;
long n;
{
	
	if(n > 0)
		x->defDur = n;
	
	
}

void lperc_setVel(x,n,m)
Lperc *x;
long n,m;
{
	
	if((n >= 0 && n < 128) && (n >= 0 && n < 128))
		x->vel[n] = m;
	
}

void lperc_setDur(x,n,m)
Lperc *x;
long n,m;
{
	
	if((n >= 0 && n < 128) && (n >= 0 && n < 128))
		x->dur[n] = m;
	
}



void lperc_setRate(x,n,m)
Lperc *x;
long n,m;
{
	
	if((n >= 0 && n < 128) && (n >= 0 && n < 128))
		x->rate[n] = m;
	
}

void lperc_linkOn(x,n,m)
Lperc *x;
long n;
long m;
{
	
	if((n > 0) && (n < 128) && (m > 0) && (m < 128))
		x->link[n] = m;
	x->inletNumber = 0;
	
}



void lperc_linkOff(x,n)
Lperc *x;
long n;
{
	
	x->inletNumber = 0;
	if((n > 0) && (n < 128))
		x->link[n] = 0;
	
}

void lperc_clear(x)
Lperc *x;
{
	int i;
	
	for(i = 0; i<128;++i)
		{
		x->link[i] = OFF;
		x->dur[i] = 0;
		x->vel[i] = 0;
		x->rate[i] = 0;
		}
	
}

void waitToEdit(x)
Lperc *x;
{
	
	defer_low(x,(method)lperc_edit,0L,0,0L);
	
}


void lperc_edit(x,s,argc,argv)
Lperc *x;
Symbol *s;
int argc;
Atom *argv;
{
	long i;
	long textSize;
	int here;
	void *buffer;
	
	here = 0;
	x->edWin = ed_new((t_object *)x);
	//x->edWin->e_wind->w_scratch = 1;
	if(x->saveName[0] != '\0')
		wind_settitle(x->edWin->e_wind,x->saveName,0L);
	x->textHandle = newhandle(7200L);
	buffer = binbuf_new();
	binbuf_vinsert(buffer,"sl",gensym("defVel"),x->defVel);
	binbuf_vinsert(buffer,"sl",gensym("defDur"),x->defDur);
	binbuf_vinsert(buffer,"sl",gensym("defRate"),x->defRate);
	for(i = 0; i<128;++i)
		{
		if(x->vel[i])
			binbuf_vinsert(buffer,"sll",gensym("vel"),i,x->vel[i]);
		if(x->dur[i])
			binbuf_vinsert(buffer,"sll",gensym("dur"),i,x->dur[i]);
		if(x->rate[i])
			binbuf_vinsert(buffer,"sll",gensym("rate"),i,x->rate[i]);
		if(x->link[i])
			binbuf_vinsert(buffer,"sll",gensym("link"),i,x->link[i]);
		}
		
	binbuf_totext(buffer,x->textHandle,&textSize);
	ed_settext(x->edWin,x->textHandle, textSize);
	if(x->saveName[0] != '\0')
		wind_settitle(x->edWin->e_wind,x->saveName,0L);
	
	
}



void lperc_edclose(x,textHandle,textSize)
Lperc *x;
char **textHandle;
long textSize;
{
	
	void *buffer;
	
	
	buffer = binbuf_new();
	//post("textH %ld   size  %ld ",textHandle, textSize);
	binbuf_text(buffer,textHandle,textSize);
	bufToPerc(x,buffer);
	
	binbuf_free(buffer);
	
	x->edWin = NULL;
	
	disposhandle(x->textHandle);
	
}

void bufToPerc(x,buffer)
Lperc *x;
void *buffer;
{
	long which = 0;
	long value;
	long typeoffset, stuffoffset;
	Atom *resultptr;
	Atom result;
	int breakflag;
	
	
	resultptr = &result;
	typeoffset = 0;
	stuffoffset = 0;
	breakflag = 0;
	while( breakflag == 0)
	{
	breakflag = binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr);
	if(breakflag)
		break;
		if(result.a_type == A_SYM )
			{
			if(result.a_w.w_sym == gensym("defVel"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					x->defVel = convert(result);
				else
					continue;
				}
			if(result.a_w.w_sym == gensym("defDur"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					x->defDur = convert(result);
				else
					continue;
				}
			if(result.a_w.w_sym == gensym("defRate"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					x->defRate = convert(result);
				else
					continue;
				}
			if(result.a_w.w_sym == gensym("vel"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					which = convert(result);
				else
					continue;
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					value = convert(result);
				else
					continue;
				lperc_setVel(x,which,value);
				}
				
			if(result.a_w.w_sym == gensym("dur"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					which = convert(result);
				else
					continue;
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					value = convert(result);
				else
					continue;
				lperc_setDur(x,which,value);
				}
				
			if(result.a_w.w_sym == gensym("rate"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					which = convert(result);
				else
					continue;
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					value = convert(result);
				else
					continue;
				lperc_setRate(x,which,value);
				}
				
			if(result.a_w.w_sym == gensym("link"))
				{
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					which = convert(result);}
				else
					continue;
				if(binbuf_getatom(buffer,&typeoffset,&stuffoffset,resultptr)==0)
					value = convert(result);
				else
					continue;
				lperc_linkOn(x,which,value);
				}
	
			}
	
}

long convert(Atom argv)
{
	switch(argv.a_type)
	{
	case A_FLOAT:
	//post("convert double");
	return((long)argv.a_w.w_float);
	
	case A_LONG:
	//post("convert long");
	return(argv.a_w.w_long);
	
	case A_SYM:
	//post("convert sym");
	return(0);
	
	default:
	//post("default");
	return(argv.a_type);
	}
}

void loadfile(x,s)
Lperc *x;
Symbol *s;
{	
	int  i;
	long type;
	char name[256];
	//Str255 filename;
	long types[16];
	void *buffer;
	//long textSize;
	long typefound;
	//FILE_REF refNum;
	//t_filehandle theFileHandle;
	
	
	types[0]='TEXT';
	buffer = binbuf_new();
	if (s != gensym("")) 
	{
		x->vol = defvolume();
		if (locatefile_extended (s->s_name, &x->pathID, &typefound, types, 1))
			{
			ouchstring("Couldn't find %s. Please locate the file, or an alternate.",
				s->s_name);
			x->fn=gensym("");
			s = gensym("");
			}
		else
			{
			x->fn = s;
			lperc_clear(x);
			binbuf_read(buffer,s->s_name,x->pathID,0);
			i = 0;
			while(s->s_name[i])
				{
				x->saveName[i] = s->s_name[i];
				++i;
				}
			x->saveName[i] = 0;
			}
	}
	
	if(s == gensym(""))
	{
		if (open_dialog(name, &x->pathID, &type, types, 1)) 
			{
			
			return;
			}
		else
			{
			lperc_clear(x);
			binbuf_read(buffer,name,x->pathID,0);
			i = 0;
			while(name[i])
				{
				x->saveName[i] = name[i];
				++i;
				}
			x->saveName[i] = 0;
			}
	}
	
	
	bufToPerc(x,buffer);
	binbuf_free(buffer);
	
	
}

void lperc_assist(x,b,msg,arg,dst)
Lperc *x;
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
			strcpy(dst,"tremolo rate");
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


void  *lperc_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lperc  *x;
	int i;
	
	
	x = (Lperc *)newobject(class);
	
	x->proxy3 = proxy_new(x,3L,&x->inletNumber);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	x->outR = intout(x);
	x->outL = intout(x);
	x->clock = clock_new(x,(method)lperc_stopnote);
	x->rollclock = clock_new(x,(method)lperc_rollNote);
	x->rolltimer = clock_new(x,(method)lperc_rollEnd);
	x->tremclock = clock_new(x,(method)lperc_tremNote);
	x->tremtimer = clock_new(x,(method)lperc_tremEnd);
	x->saveName[0] ='\0';
	x->nextoff = 0;
	x->nextslot = 0;
	x->listsize = 0;
	x->currentnote = 0;
	for(i = 0; i<128;++i)
		{
		x->timeQ[i]=0;
		x->link[i] = OFF;
		x->dur[i] = 0;
		x->vel[i] = 0;
		x->rate[i] = 0;
		}

	x->defVel = 64;
	x->defDur = 1000;
	x->defRate = 2;
	x->rollPitch = 0;
	x->rollRate = 0;
	x->rollVel = 0;
	x->rollDur = 0;
	x->mode = UP;
	x->direction = UP;
	x->inletNumber = 0;
	
	switch(ac)
		{
		case 0:
		break;
		
		case 1:
		if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0)			
			x->defDur = theArgs[0].a_w.w_long;		
		if(theArgs[0].a_type == A_SYM)
			loadfile(x,theArgs[0].a_w.w_sym);
			
		break;
			
		case 2:
		if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
			x->defVel = theArgs[0].a_w.w_long;
		if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
			x->defDur = theArgs[1].a_w.w_long;
		break;
		
		default:
		if(theArgs[0].a_type == A_LONG && theArgs[0].a_w.w_long > 0 && theArgs[0].a_w.w_long < 128)			
			x->defVel = theArgs[0].a_w.w_long;
		if(theArgs[1].a_type == A_LONG && theArgs[1].a_w.w_long > 0)			
			x->defDur = theArgs[1].a_w.w_long;
		if(theArgs[2].a_type == A_LONG && theArgs[2].a_w.w_long > 0)			
			x->defRate = theArgs[2].a_w.w_long;
		}
		
	for(i=0; i<MAXSIZE; ++i)
	{
		x->tremList[i].a_type = A_LONG;
		x->theList[i].a_type = A_LONG;
	}
	
	
	return(x);
}

void lperc_free(x)
Lperc *x;
{
	
	clock_free(x->clock);
	clock_free(x->rollclock);
	clock_free(x->rolltimer);
	clock_free(x->tremclock);
	clock_free(x->tremtimer);
	

}

int check_inlet(x)
Lperc *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}