// lpoly file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1996, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"

#define MAXSIZE 256
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

typedef struct lpoly
{
	struct object ob;
	void *proxy1;
	void *proxy2;
	long inletNumber;
	long voice[MAXSIZE];
	long voiceTime[MAXSIZE];
	long velocityPending;
	short numVoices;
	short voicebase;
	short useright;
	short numberPlaying;
	Atom outList[4];
	int outsize;
	void *out;
	void *outR;
} Lpoly;


void *lpoly_new();
void lpoly_free();
void  lpoly_List();
void  lpoly_Int();
void  ripOneOff();
void lpoly_bang();
void lpoly_assist();
void lpoly_output();
int check_inlet();

void *class;

int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&class,(method)lpoly_new,(method)lpoly_free,(short)sizeof(struct lpoly),0L,A_GIMME,0);
	addint((method)lpoly_Int);
	addbang((method)lpoly_bang);
	addmess((method)lpoly_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}

void lpoly_output(x)
Lpoly *x;
{
	
	if(x->useright){
		outlet_int(x->outR,x->outList[0].a_w.w_long + x->voicebase);
		outlet_list(x->out,0L,2L,&x->outList[1]);
	}else
	outlet_list(x->out,0L,3L,x->outList);
}


void  ripOneOff(x)
Lpoly *x;
{
	int i;
	long oldest, secondOldest;
	short oldestNote, secondOldestNote;
	
	oldest = 0x7FFFFFFF;
	secondOldest =0x7FFFFFFF;
	oldestNote = -1;
	secondOldestNote = -1;
	for(i=0;i<x->numVoices;++i)  // find the oldest
	{
		if(x->voiceTime[i] < oldest)
		{
			secondOldest = oldest;
			oldest = x->voiceTime[i];
			secondOldestNote = oldestNote;
			oldestNote = i;
		}
		else
		if(x->voiceTime[i] < secondOldest)
		{
			secondOldest = x->voiceTime[i];
			secondOldestNote = i;
		}
	}
	if(secondOldestNote > -1){   // but not lowest
		if(x->voice[oldestNote] < x->voice[secondOldestNote])
		{
			oldestNote = secondOldestNote;
		}}
	x->outList[0].a_w.w_long = oldestNote;
	x->outList[1].a_w.w_long = x->voice[oldestNote];
	x->outList[2].a_w.w_long = 0;
	lpoly_output(x);
	x->voice[oldestNote] = 0;  // mark the slot as empty
	-- x->numberPlaying;
	
	
}


void  lpoly_Int(x,n)
Lpoly *x;
long n;
{
	int i;
	
	i = 0;
	switch(check_inlet(x))
	{
		case 0:
		if(x->numVoices)
		{
			for(i = 0; i < x->numVoices; ++i)
			{
				if(x->voice[i] == n)  // note is playing, turn it off
				{
					x->outList[0].a_w.w_long = i;
					x->outList[1].a_w.w_long = n;
					x->outList[2].a_w.w_long = 0;
					lpoly_output(x);
					x->voice[i] = 0;  // mark the slot as empty
					-- x->numberPlaying;
					break;
				}
			}
			if(x->velocityPending)	//play the note
			{
				if(x->numberPlaying >= x->numVoices)
					ripOneOff(x);
				i = 0;
				while(x->voice[i])  // find an empty slot
				{
					++i;
					if(i == x->numVoices)
						break;
				}
				x->outList[0].a_w.w_long = i;
				x->outList[1].a_w.w_long = n;
				x->outList[2].a_w.w_long = x->velocityPending;
				lpoly_output(x);
				x->voice[i] = n;
				x->voiceTime[i] = gettime();
				++x->numberPlaying;
			}
		}
		break;
			
		case 1:
		x->velocityPending = n;
		break;
		
		case 2:
		lpoly_bang(x);
		x->numVoices = n;
		if(x->numVoices < 0) x->numVoices = 0;
		if(x->numVoices > MAXSIZE) x->numVoices = MAXSIZE;
		break;
	}
	x->inletNumber = 0;
	
	
}




void lpoly_bang(x)  // all notes off
Lpoly *x;
{
	int i;
	
	for(i = 0; i < x->numVoices; ++i)
	{
		if(x->voice[i])  // note is playing, turn it off
		{
			x->outList[0].a_w.w_long = i;
			x->outList[1].a_w.w_long = x->voice[i];
			x->outList[2].a_w.w_long = 0;
			lpoly_output(x);
			x->voice[i] = 0;  // mark the slot as empty
			
			
		}
	}
	x->numberPlaying = 0;
	x->inletNumber = 0;
	
}


void lpoly_assist(x,b,msg,arg,dstString)
Lpoly *x;
void *b;
long  msg,arg;
char *dstString;
{
	
	if(msg == 1)
	{
		switch (arg)
		{
			case 0:
			strcpy(dstString,"Pitch; bang for alloff");
			break;
			
			case 1:
			strcpy(dstString,"Velocity");
			break;
			
			case 2:
			strcpy(dstString,"Number of voices");
			break;
		}
	}
	if(msg == 2)
		strcpy(dstString,"List: voice#, pitch, velocity ");
	
	
}



void  *lpoly_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lpoly  *x;
	int i;
	
	x = (Lpoly *)newobject(class);
	x->proxy2 = proxy_new(x,2L,&x->inletNumber);
	x->proxy1 = proxy_new(x,1L,&x->inletNumber);
	if(ac > 1)
		x->outR = outlet_new(x,0L);
	x->out= outlet_new(x,0L);
	x->inletNumber = 0;
	if(ac && theArgs[0].a_type == A_LONG)
		x->numVoices = theArgs[0].a_w.w_long;
	else
		x->numVoices = 2;
	if(ac > 1 && theArgs[1].a_type == A_LONG){
		x->voicebase = theArgs[1].a_w.w_long;
		x->useright = 1;
	}else{
		x->voicebase = 0;
		x->useright = 0;
	}
	if(x->numVoices < 0) x->numVoices = 2;
	if(x->numVoices > MAXSIZE) x->numVoices = MAXSIZE;
	x->numberPlaying = 0;
	for(i=0;i<MAXSIZE;++i)
		x->voice[i] = 0;
	x->outList[0].a_type = A_LONG;
	x->outList[1].a_type = A_LONG;
	x->outList[2].a_type = A_LONG;
	
	return(x);
}

void lpoly_free(x)
Lpoly *x;
{	
		freeobject(x->proxy1);
		freeobject(x->proxy2);
}

int check_inlet(x)
Lpoly *x;
{
//	return x->inletNumber;  // classic only
	return proxy_getinlet((t_object *)x);  // OS X only
}

