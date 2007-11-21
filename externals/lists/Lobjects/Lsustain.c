// lswustain file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1995, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
#include "ext_common.h"
#define MAXSIZE 128
#define OFF 0
#define ON 1
#define HOLDING 2

typedef struct sustain {
struct object ob;
void *proxy;
void *outR;
void *outL;
int sustain[128]; // enable sustain note by note
int playing[128];	// current state of each note
long inletNumber;
long velocity;
} Sustain;


void *sustain_new(), sustain_List(), sustain_Int(), sustain_Float(), sustain_Bang();

void sustain_assist();
void sustain_free();
void noteOn();
void noteOff();
void sustOn();
void sustOff();
void sostOn();
void release();
void mute();
int check_inlet();

void *class;
int main(void)
{
	Symbol * s;
	Symbol * l;
	
	setup ( (t_messlist **)&class, (method)sustain_new, 0L,(short) sizeof(Sustain), 
	0L, A_DEFLONG,0);
	
	addmess ((method)sustain_List, "list",A_GIMME,0);
	addmess ((method)sustOn, "sust",A_GIMME,0);
	addmess ((method)sustOff, "sustoff",A_GIMME,0);
	addmess ((method)mute, "mute",A_GIMME,0);
	addmess ((method)sostOn, "sost",A_GIMME,0);
	addmess ((method)release, "release",A_GIMME,0);
	addint((method)sustain_Int);
	addfloat((method)sustain_Float);
	addbang((method)sustain_Bang);
	addmess((method)sustain_assist, "assist",A_CANT,0);
	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © 2004 Peter Elsea and Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void sustain_List (x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	long pitch;
	long vel;
	
	pitch = 0;
	vel = 0;
	if(argc > 1)
	{
		switch(argv[0].a_type)
		{
			case A_LONG:
			pitch = argv[0].a_w.w_long;
			break;
			
			case A_FLOAT:
			pitch = (long)argv[0].a_w.w_float;
			break;
		}
		switch(argv[1].a_type)
		{
			case A_LONG:
			vel = argv[1].a_w.w_long;
			break;
			
			case A_FLOAT:
			vel = (long)argv[1].a_w.w_float;
			break;
		}
	
		if(vel)
		noteOn(x,pitch, argv[1].a_w.w_long);
		else
		noteOff(x,pitch);
	}
}



void sustain_Int (x,n)
Sustain *x;
long n;
{

	
	switch(check_inlet(x))
	{
		case 0:
		if(x->velocity)
			noteOn(x,n,x->velocity);
		else
			noteOff(x,n);
		break;
			
		case 1:
		x->velocity = n;
		break;
	
	}
}

void sustain_Float (x,n)
Sustain *x;
double n;
{
	sustain_Int(x,(long)n);
}

void sustain_Bang (x)
Sustain *x;
{
	int i;
	
	for(i = 0; i<128;++i)
	{
		if(x->playing[i] == HOLDING)
			{
			outlet_int(x->outR,0L);
			outlet_int(x->outL,(long)i);
			x->playing[i] = OFF;
			}
		x->sustain[i] = OFF;
	}
}



void noteOn(x,theNote,velocity)
Sustain *x;
long theNote;
long velocity;
{
	if(theNote < 0 || theNote > 127)
		return;
	if(x->playing[theNote])	// restart if playing or holding
		{
		outlet_int(x->outR,0L);
		outlet_int(x->outL,theNote);
		}

	x->playing[theNote] = ON; // in any case, mark it on
	outlet_int(x->outR,velocity);
	outlet_int(x->outL,theNote);
	

}

void noteOff(x,theNote)
Sustain *x;
long theNote;
{
	if(theNote < 0 || theNote > 127)
		return;
	
	if(x->playing[theNote])		// either hold it 
	{
		if(x->sustain[theNote])
			x->playing[theNote] = HOLDING;
		else					//or turn it off
		{
			outlet_int(x->outR,0L);
			outlet_int(x->outL,theNote);
			x->playing[theNote] = OFF;
		}
	}
}

void sustOn(x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int theNote;
	
	theNote = 0;
	if(argc)
	{
		for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{
				case A_LONG:
				theNote = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				theNote = (long)argv[i].a_w.w_float;
				break;
			}
		if(theNote < 0 || theNote >127)
			continue;
		x->sustain[theNote] = ON;  // sust with args are additive
		}
	}
	else  // set them all
	{
	for(i=0;i<128;++i)
		x->sustain[i] = ON;
	}
}

void sustOff(x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int theNote;
	
	theNote = 0;
	if(argc)
	{
		for(i=0;i<argc;++i)
		{
		switch(argv[i].a_type)
			{
				case A_LONG:
				theNote = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				theNote = (long)argv[i].a_w.w_float;
				break;
			}
		if(theNote < 0 || theNote >127)
			continue;
		x->sustain[theNote] = OFF;  // sustoff with args is additive
		}
	}
	else  // clear them all
	{
	for(i=0;i<128;++i)
		x->sustain[i] = OFF;
	}
}

void sostOn(x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	int theNote;
	
	theNote = 0;
	if(argc)
	{
		
		for(i=0;i<argc;++i)
		{
			switch(argv[i].a_type)
			{
				case A_LONG:
				theNote = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				theNote = (long)argv[i].a_w.w_float;
				break;
			}
			if(theNote < 0 || theNote >127)
				continue;
			x->sustain[theNote] = ON;
		}
	}
	for(i=0;i<128;++i)
	{
		if(x->playing[i])
			x->sustain[i] = ON;
	}
}

void release(x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long theNote;
	
	theNote = 0;
	if(argc)
	{
		for(i=0;i<argc;++i)
		{
			switch(argv[i].a_type)
			{
				case A_LONG:
				theNote = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				theNote = (long)argv[i].a_w.w_float;
				break;
				
				default:
				continue;
			}
			if(theNote < 0 || theNote > 127)
					continue;
			if(x->playing[theNote]== HOLDING)
			{
				
				outlet_int(x->outR,0L);
				outlet_int(x->outL,theNote);
				x->playing[theNote] = OFF;
			}
			x->sustain[theNote] = OFF;
		}
	}
	else
	{
		for(i=0;i<128;++i)
		{
			if(x->playing[i]== HOLDING)
			{
				outlet_int(x->outR,0L);
				outlet_int(x->outL,(long)i);
				x->playing[i] = OFF;
			}
			x->sustain[i] = OFF;
		}
	}
	
}

void mute(x,s,argc,argv)
Sustain *x;
Symbol *s;
int argc;
Atom *argv;
{
	int i;
	long theNote;
	
	theNote = 0;
	if(argc)
	{
		for(i=0;i<argc;++i)
		{
			switch(argv[i].a_type)
			{
				case A_LONG:
				theNote = argv[i].a_w.w_long;
				break;
				
				case A_FLOAT:
				theNote = (long)argv[i].a_w.w_float;
				break;
				
				default:
				continue;
			}
			if(theNote < 0 || theNote > 127)
					continue;
			if(x->playing[theNote])
			{
				
				outlet_int(x->outR,0L);
				outlet_int(x->outL,theNote);
				x->playing[theNote] = OFF;
			}
			
		}
	}
	else
	{
		for(i=0;i<128;++i)
		{
			if(x->playing[i])
			{
				outlet_int(x->outR,0L);
				outlet_int(x->outL,(long)i);
				x->playing[i] = OFF;
			}
			
		}
	}
}


void sustain_assist(x,b,msg,arg,dst)
Sustain *x;
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

void *sustain_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Sustain *x;
	register int i;
	
	
	x = (Sustain *)newobject(class);
	x->proxy = proxy_new(x,1L,&x->inletNumber);
	x->outR= outlet_new(x,0L);
	x->outL= outlet_new(x,0L);
	for(i=0;i<128;++i)
		{
		x->sustain[i]=0;
		x->playing[i]=0;
		}
	x->inletNumber=0;
	x->velocity = 0;
	return (x);
}

void sustain_free(x)
Sustain *x;
{
	
	freeobject(x->proxy);
}

int check_inlet(x)
Sustain *x;
{
	return x->inletNumber;  // classic only
//	return proxy_getinlet((t_object *)x);  // OS X only
}