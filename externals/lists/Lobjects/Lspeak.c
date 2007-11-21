// lspeak.c file for Max object
// by Peter Elsea,  University of California, Santa Cruz
// copyright © 1999, Regents of the University of California
// This code and the object created by it may be modified and/or 
// distributed by any means, provided that this copyright and notice   
// appear in all subsequent copies and documentation.

#include "ext.h"
#include <string.h>
//#include <SpeechSynthesis.h>
//#include <math.h>
#include "ext_common.h"

#define QWRAP 15
#define QSIZE 16
#define MAXSIZE 64

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2


typedef struct lspeak
{
	struct object m_ob;
	void *clock;
	Atom theList[QSIZE][MAXSIZE];
	int listsize[QSIZE];
	Symbol *asciiSym;
	Symbol *voiceList[64];
	Symbol *newVoiceName;
	int numVoices;
	short babble;
	VoiceSpec *voicePtr[64];
	int lastout, nextout, nextslot, clockOff;
	long delay;
	SpeechChannel channel;
	int talking;
	int wordWatch;
	char theText[2048];
	int textLength;
	void *outR;
	void *outL;
	SpeechDoneUPP spDone;
	SpeechWordUPP wordCbk;
	int bangLater;
	void *talkLater;
	void *stopit;
	void *resumeit;
	void *setit;
	void *repeatit;
	void *gottabang;
	int bangPlease;
	
} Lspeak;

void  lspeak_Int();
void  numToString();
void  floatToString();
void  lspeak_Float();
void  lspeak_List();
void  say_List();
void  lspeak_Anything();
void  lspeak_Bang();
void repeat();
void lspeak_stop();
void lspeak_resume();
void lspeak_pitch();
void lspeak_rate();
void lspeak_query();
void lspeak_xstop();
void lspeak_xresume();
void lspeak_xreset();
void lspeak_assist();
void queue_timeout();
void talkLater();
void MySpeechDoneCallback();
void MyWordCallback();
void bang_outL();
void lspeak_xvoice();
void setVoice();
void  lspeak_babble();
void fillVoiceList();
void *lspeak_new();
void lspeak_free();

void *myclass;

int main(void)
{
	
	Symbol * s;
	Symbol * l;
	
	setup((t_messlist **)&myclass,(method)lspeak_new,(method)lspeak_free,(short)sizeof(Lspeak),0L,A_GIMME,0);
	
	addint((method)lspeak_Int);
	addfloat((method)lspeak_Float);
	addbang((method)lspeak_Bang);
	addmess((method)lspeak_List, "list",A_GIMME,0);
	addmess((method)lspeak_Anything, "anything",A_GIMME,0);
	addmess((method)lspeak_assist, "assist",A_CANT,0);
	addmess((method)lspeak_xstop, "xstop",0);
	addmess((method)lspeak_xresume, "xresume",0);
	addmess((method)lspeak_xreset, "xreset",0);
	addmess((method)lspeak_xvoice, "xvoice",A_GIMME,0);
	addmess((method)lspeak_pitch, "ptch",A_GIMME,0);
	addmess((method)lspeak_pitch, "xpitch",A_GIMME,0);
	addmess((method)lspeak_babble, "babble",A_DEFLONG,0);
	addmess((method)lspeak_rate, "rte",A_GIMME,0);
	addmess((method)lspeak_rate, "xrate",A_GIMME,0);
	finder_addclass("Lists","Lspeak");


	s = gensym("goslugsgo");
	l = gensym("lobjectsbypqe");
	if(s->s_thing != (struct object *)l){
	post("Lobjects for Max © Regents of the University of California.");
	s->s_thing = (struct object *)l;}
	return 0;
}



void  lspeak_Int(x,n)
Lspeak *x;
long n;
{
	
	int mySlot;
	short lockState;
	
	lockState = lockout_set(1);
	mySlot = x->nextslot++;
	x->nextslot &= QWRAP;
	lockout_set(lockState);
	x->theList[mySlot][0].a_type = A_LONG;
	x->theList[mySlot][0].a_w.w_long = n;
	x->listsize[mySlot] = 1 ;
	
	if(x->nextslot == x->nextout)
		post("Lspeak Overflow!");
	if(x->clockOff)
		{
		clock_delay(x->clock,x->delay);
		x->clockOff = 0;
		}
	
	
}

void numToString(n,theString)
long n;
char *theString;
{
	int i = 30;
	char numString[32];
	char digitString[] = "0123456789";
	int negative = false;
	
	if(n < 0) 
	{
	negative = true;
	n = 0 - n;
	}
	numString[31] = 0;
	numString[30] = '0'; // just a little insurance
	while(n/10)
	{
		numString[i--] = digitString[n % 10];
		n /= 10;
	}
	numString[i--] = digitString[n % 10];
	if(negative)
	{
	 numString[i--] = ' ';
	 numString[i--] = 's';
	 numString[i--] = 'u';
	 numString[i--] = 'n';
	 numString[i--] = 'i';
	 numString[i--] = 'm';
	 }
	strcpy(theString, &numString[i + 1]);
	//post(theString);
}

void floatToString(f,theString)
float f;
char *theString;
{
	long n;
	char point[] = " point ";
	char fraction[] = "0 0";
	int last;
	int lessThanOne = false;
	if(f < 1) lessThanOne = true;
	f *= 100;
	n = (long)f;
	numToString(n,theString);
	last = strlen(theString);
	if(last > 1)
		fraction[0] = theString[last - 2];
	if(theString[last-1] == 48)
		fraction[2] = 0;
	else
		fraction[2] = theString[last - 1];
	last -= 2;
	strcpy(&theString[last], point);
	strcpy(&theString[last + 7],fraction);
	//post(theString);
	

}


void  lspeak_Float(x,f)
Lspeak *x;
float f;
{
	
	int mySlot;
	short lockState;
	
	lockState = lockout_set(1);  // prevent interrupts from creating invalid slotpointer
	mySlot = x->nextslot++;
	x->nextslot &= QWRAP;
	lockout_set(lockState);
	
	x->theList[mySlot][0].a_type = A_FLOAT;
	x->theList[mySlot][0].a_w.w_float = f;
	x->listsize[mySlot] = 1 ;
	
	if(x->nextslot == x->nextout)
		post("Lspeak Overflow!");
	if(x->clockOff)
		{
		clock_delay(x->clock,x->delay);
		x->clockOff = 0;
		}
	
	
	
}


void  lspeak_List(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int i;
	int mySlot;
	short lockState;
	
	lockState = lockout_set(1);
	mySlot = x->nextslot++;
	x->nextslot &= QWRAP;
	lockout_set(lockState);
	for(i=0;i<ac;++i)
	{
		x->theList[mySlot][i]=theArgs[i];
		
	}
	x->listsize[mySlot] = ac ;
	
	if(x->nextslot == x->nextout)
		post("Lspeak Overflow!");
	if(x->clockOff)
		{
		clock_delay(x->clock,x->delay);
		x->clockOff = 0;
		}
	
	
}

void  say_List(x,ac,theArgs) // this is the one that actually talks
Lspeak  *x;
int ac;
Atom *theArgs;
{
	int i,j;
	int asciiMode;
	char tempString[64];
	
	x->textLength = 0;
	asciiMode = 0;
	for(i=0;i<ac;++i)
	{
		switch(theArgs[i].a_type)
		{
			case A_SYM:
			if(theArgs[i].a_w.w_sym  == x->asciiSym)
				asciiMode = 1;
			else
			{
				for(j = 0;j < strlen(theArgs[i].a_w.w_sym->s_name); ++ j)
				{
					x->theText[x->textLength ++] = theArgs[i].a_w.w_sym->s_name[j];
				}
				x->theText[x->textLength ++] = 32;
			}
			break;
			
			case A_LONG:
			if(asciiMode)
			{
				x->theText[x->textLength ++] = theArgs[i].a_w.w_long;
			}
			else
			{
				numToString(theArgs[i].a_w.w_long,tempString);
				for(j = 0;j < strlen(tempString); ++ j)
				{
					x->theText[x->textLength ++] = tempString[j];
				}
				x->theText[x->textLength ++] = 32;
			}
			break;
			
			case A_FLOAT:
			floatToString(theArgs[i].a_w.w_float,tempString);
			for(j = 0;j < strlen(tempString); ++ j)
			{
				x->theText[x->textLength ++] = tempString[j];
			}
			x->theText[x->textLength ++] = 32;
			break;
			
		}
		
	}
	SpeakText(x->channel,x->theText,x->textLength);
	x->textLength = 0;
	
}

void  lspeak_Anything(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;  // the first word of the phrase will be in s
int ac;
Atom *theArgs;
{
	int i;
	int mySlot;
	short lockState;
	
	if(ac > MAXSIZE - 1) ac = MAXSIZE - 1;
	lockState = lockout_set(1);
	mySlot = x->nextslot++;
	x->nextslot &= QWRAP;
	lockout_set(lockState);
	
	x->theList[mySlot][0].a_type = A_SYM;
	x->theList[mySlot][0].a_w.w_sym = s;
	for(i=0;i<ac ;++i)
	{
		x->theList[mySlot][i+1]=theArgs[i];
		
	}
	x->listsize[mySlot] = ac + 1;
	
	if(x->nextslot == x->nextout)
		post("Lspeak Overflow!");
	if(x->clockOff)
		{
		clock_delay(x->clock,x->delay);
		x->clockOff = 0;
		}
	
}

void  lspeak_Bang(x)
Lspeak  *x;
{	
	qelem_set(x->repeatit);
	
}

void repeat(x)
Lspeak  *x;
{
	
	say_List(x,x->listsize[x->lastout],x->theList[x->lastout]);
	
}

void  lspeak_stop(x)
Lspeak  *x;
{
	
	StopSpeechAt(x->channel,kEndOfWord);
	x->nextout = x->nextslot;
	clock_unset(x->clock);
	x->clockOff = 1;
	x->talking = 0;
	
	
}

void  lspeak_resume(x)
Lspeak  *x;
{
	
	
	ContinueSpeech(x->channel);
	x->talking = 1;
	clock_delay(x->clock,x->delay);
	x->clockOff = 0;
	
	
}

void  lspeak_pitch(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	Fixed n = 0;
	float fraction;
	long lfract;
	
	if(ac)
	{
		 if(theArgs[0].a_type == A_LONG)
		{
			n = theArgs[0].a_w.w_long;
			n <<= 16;
		}
		if(theArgs[0].a_type == A_FLOAT)
		{
			n = (long)theArgs[0].a_w.w_float;
			fraction = theArgs[0].a_w.w_float - n;
			n <<= 16;
			lfract = fraction * 0xFFFF;
			lfract &= 0xFFFF;
			n += lfract;
			
		}
		SetSpeechPitch(x->channel,n);
	}
}

void  lspeak_rate(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	Fixed n =0;
	float fraction;
	long lfract;
	
	if(ac)
	{
		 if(theArgs[0].a_type == A_LONG)
		{
			n = theArgs[0].a_w.w_long;
			n <<= 16;
		}
		if(theArgs[0].a_type == A_FLOAT)
		{
			n = (long)theArgs[0].a_w.w_float;
			fraction = theArgs[0].a_w.w_float - n;
			n <<= 16;
			lfract = fraction * 0xFFFF;
			lfract &= 0xFFFF;
			n += lfract;
			
		}
		SetSpeechRate(x->channel,n);
	}
	
}

void  lspeak_query(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	int n;
	
	if(ac)
	{
		n = theArgs[0].a_w.w_long;
		n &= 0x0f;
		say_List(x,x->listsize[n],x->theList[n]);
	}
	
}

void  lspeak_xstop(x)
Lspeak  *x;
{
	qelem_set(x->stopit);
	
}

void  lspeak_xresume(x)
Lspeak  *x;
{
	qelem_set(x->resumeit);
}

void  lspeak_xreset(x)
Lspeak  *x;
{
	qelem_set(x->setit);
	SetSpeechInfo(x->channel,'rset',0);
}

void lspeak_assist(x,b,msg,arg,dst)
Lspeak *x;
void *b;
long  msg,arg;
char *dst;
{
	if(msg == ASSIST_INLET)
	{
		switch (arg)
		{
			case 0:
			strcpy(dst,"Lists and numbers to speak");
			break;
			
			default:
			strcpy(dst,"Delay between outputs");
			break;
		}
	}
	else
	{
		if(msg == ASSIST_OUTLET)
		switch (arg)
		{
			case 0:
			strcpy(dst,"Bang with words");
			break;
			
			default:
			strcpy(dst,"Bang when queue empty");
			break;
		}
	}
	
	
}
void queue_timeout(x)
Lspeak *x;
{
	if(SpeechBusy() && (x->babble == 0))// if busy try again later
	{
		x->clockOff = 0;
		clock_delay(x->clock,x->delay);
		if(x->wordWatch)
		{
		outlet_bang(x->outL);
		x->wordWatch = 0;
		}
	}
	else
	{
		if(x->nextslot == x->nextout) // this was the last one
		{
			x->clockOff = 1;
			outlet_bang(x->outR);
		}
		else
		{
			qelem_set(x->talkLater);
		}
	}
	
}

void talkLater(x)
Lspeak *x;
{
	short lockState;
	int theNextOne;
	
	theNextOne = x->nextout;
	say_List(x,x->listsize[theNextOne],x->theList[theNextOne]);// start speaking the next list
	lockState = lockout_set(1);
	x->lastout = x->nextout++;
	x->nextout &= QWRAP;
	lockout_set(lockState);
	//x->talking = 1;
	x->clockOff = 0;
	clock_delay(x->clock,x->delay);
	
}

void MySpeechDoneCallback(channel,refCon)
SpeechChannel channel;
long refCon;
{
Lspeak *x;

x = (void *)refCon;
 x->talking = 0;
}

void MyWordCallback(channel,refCon,wordPos,wordLen)
SpeechChannel channel;
long refCon;
long wordPos;
short wordLen;
{
Lspeak *x;

x = (void *)refCon;
x->talking= 1;
x->wordWatch = 1;
qelem_front(x->gottabang);

}



void bang_outL(x)
Lspeak *x;
{
	short prevLock;
	
	prevLock = lockout_set(1);
	outlet_bang(x->outL);
	lockout_set(prevLock);
	
}

void  lspeak_xvoice(x,s,ac,theArgs)
Lspeak  *x;
Symbol *s;
int ac;
Atom *theArgs;
{
	
	if(ac)
	{
		switch(theArgs[0].a_type)
		{
			case A_LONG:
			if(theArgs[0].a_w.w_long < x->numVoices)
				x->newVoiceName = x->voiceList[theArgs[0].a_w.w_long];
			else
				x->newVoiceName = x->voiceList[0];
			break;
			
			case A_FLOAT:
			if(theArgs[0].a_w.w_float < x->numVoices)
				x->newVoiceName = x->voiceList[(long)theArgs[0].a_w.w_float];
			else
				x->newVoiceName = x->voiceList[0];
			break;
			
			case A_SYM:
			x->newVoiceName = theArgs[0].a_w.w_sym;
			break;
		}
	qelem_set(x->setit);
	}
	
}

void setVoice(x)
Lspeak *x;
{
	
	int found = 0;
	VoiceSpec voice;
	
	int i;
	
	
	
	for( i = 0; (i < x->numVoices) && (i < 64);++i)
	{
		if(x->newVoiceName == x->voiceList[i])
		{
			found = 1;
			break;
		}
	}
	if(found)
	{
		DisposeSpeechChannel(x->channel);
		GetIndVoice(i,&voice);
		NewSpeechChannel(&voice,&x->channel);
		SetSpeechInfo(x->channel,soRefCon,x);
	//SetSpeechInfo(x->channel,soCurrentA5,(void *)MaxA5);
	SetSpeechInfo(x->channel,soSpeechDoneCallBack,x->spDone);
	SetSpeechInfo(x->channel,soWordCallBack,x->wordCbk);
		x->talking = 0;
	}
	
}

void  lspeak_babble(x,n)
Lspeak  *x;
long n;
{
	x->babble = n;
	if(x->babble < 0) x->babble = 0;
	
}

void fillVoiceList(x)
Lspeak *x;
{
	VoiceDescription theVoice;
	VoiceSpec voice;
	short numVoices;
	char theName[64];
	int i,j;
	
	CountVoices(&numVoices);
	x->numVoices = numVoices;
	
	for( i = 0; i < numVoices;++i)
	{
		GetIndVoice(i,&voice);
		GetVoiceDescription(&voice,&theVoice,sizeof(VoiceDescription));
		for(j = 0; j < theVoice.name[0];++ j)
			theName[j] = theVoice.name[j+1];
		theName[j] = 0;
		x->voiceList[i] = gensym(theName);
		
	}
	
}

void  *lspeak_new(s,ac,theArgs)
Symbol *s;
int ac;
Atom *theArgs;
{

	Lspeak  *x;
	int i;
	x = (Lspeak *)newobject(myclass);
	
	x->talkLater = qelem_new(x,(method)talkLater);
	x->resumeit = qelem_new(x,(method)lspeak_resume);
	x->stopit = qelem_new(x,(method)lspeak_stop);
	x->setit = qelem_new(x,(method)setVoice);
	x->repeatit = qelem_new(x,(method)repeat);
	x->gottabang = qelem_new(x,(method)bang_outL);
	x->outR = bangout(x);
	x->outL = bangout(x);
	x->clock = clock_new(x,(method)queue_timeout);
	x->nextout = 0;
	x->nextslot = 0;
	x->lastout = 0;
	x->delay = 150;
	x->clockOff = 1;
	x->talking = 0;
	x->wordWatch = 0;
	x->babble = 0;
	x->bangPlease = 0;
	x->asciiSym = gensym("ascii");
	NewSpeechChannel(NIL,&x->channel);
	x->spDone = NewSpeechDoneUPP((SpeechDoneProcPtr)MySpeechDoneCallback);
	x->wordCbk = NewSpeechWordUPP((SpeechWordProcPtr)MyWordCallback);
	SetSpeechInfo(x->channel,soRefCon,x);
	//SetSpeechInfo(x->channel,soCurrentA5,(void *)MaxA5);
	SetSpeechInfo(x->channel,soSpeechDoneCallBack,x->spDone);
	SetSpeechInfo(x->channel,soWordCallBack,x->wordCbk);
	
	fillVoiceList(x);
	if(ac)
	{
		for(i = 0 ; i < ac ; ++i)
		{
			x->theList[x->nextslot][i] = theArgs[i];		
		}
		x->listsize[x->nextslot] = ac;
	}

	
	return(x);
}



void lspeak_free(x)
Lspeak *x;
{
	
	DisposeSpeechChannel(x->channel);
	qelem_free(x->talkLater);
	qelem_free(x->resumeit);
	qelem_free(x->stopit);
	qelem_free(x->setit );
	qelem_free(x->repeatit );
	qelem_free(x->gottabang );
	

}