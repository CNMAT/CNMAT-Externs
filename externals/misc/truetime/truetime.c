/* truetime code from Mike Berry via Chris Brown */
 


#include "ext.h"
#include <SetUpA4.h>
#include <A4Stuff.h>

#define MAXSIZE 32

#include <MacTypes.h>
#include <timer.h>

typedef struct Element
{
	long size;
	long time;
	Atom data[1];
} Element;

typedef struct TrueTime
{
	struct object m_ob;
	void *m_out;
	Ptr dataPtr;
	long dataSize;
	long curSize;
	UnsignedWide startTime;
} TrueTime;

fptr *FNS;
void *class;

void trueTime_elapsed(TrueTime *x);
void trueTime_start(TrueTime *x);
void *trueTime_new(Symbol *s, short ac, Atom *av);

void InsertElement(TrueTime *x, short ac, Atom *av, long time, long
inSize);


void main(fptr *f)
{
	
	EnterCodeResource();
	PrepareCallback();
	FNS = f;	
		
	setup(&class, trueTime_new,0L, (short)sizeof(TrueTime), 0L,
A_GIMME, 0);
	addmess((method)trueTime_elapsed,"elapsed",A_NOTHING,0);
	addmess((method)trueTime_start,"start",A_NOTHING,0);
//	finder_addclass("Arith/Logic/Bitwise","minimum");
	rescopy('STR#',3008);
	ExitCodeResource();
}

void trueTime_elapsed(TrueTime *x)
{
	double temp;
	long output;
	UnsignedWide now;

	EnterCallback();
	
	Microseconds(&now);
	
	
	temp = (now.hi - x->startTime.hi) * 4294967295.0;
	temp += (double)(now.lo) - (double)(x->startTime.lo);
	output = temp * 0.001;

	outlet_int(x->m_out, output);
	ExitCallback();
}

void trueTime_start(TrueTime *x)
{
	EnterCallback();
	Microseconds(&(x->startTime));
	ExitCallback();
}

void *trueTime_new(Symbol *s, short ac, Atom *av)
{
	TrueTime *x;
	
	EnterCallback();
	x = (TrueTime *)newobject(class);
	
	if (ac && av->a_type == A_LONG) {
		x->dataSize = av->a_w.w_long;
		if (x->dataSize <= 0) x->dataSize = 512;
	} else {
		x->dataSize = 512;
	}
	
	x->dataPtr = getbytes(x->dataSize);
	x->curSize = 0;
	x->m_out = listout(x);
	
	ExitCallback();
	return (x);
}
