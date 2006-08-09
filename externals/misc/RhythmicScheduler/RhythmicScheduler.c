/*

Copyright (c) 1998,99,2000,01,02,03,04,05,06.  The Regents of the
University of California (Regents).  All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio
Technologies, University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: RhythmicScheduler
DESCRIPTION: Keep track of events to be scheduled some number of "tatums" (arbitrary units of time) in the future, and output the appropriate events when time advances.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 1998,99,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
VERSION 0.1: Matt's original version
VERSION 0.3: Has "clear" message
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/



#define DEFAULT_BUFFER_SIZE 1024
#define DEFAULT_NUM_EVENTS 128
#define DEFAULT_MAX_LIST_SIZE 16



/* the required include files */
#include "version.h"
#include "ext.h"
#include "version.c"
#include <SetUpA4.h>
#include <A4Stuff.h>

typedef struct StoredListStruct {
	struct StoredListStruct *next;		// for linked list of events to come out at a given tatum
	Symbol *symbol;
	short argc;
	Atom *argv;
} StoredList;

typedef struct RhythmicScheduler
{
	Object r_ob;
	void *r_proxy[2];			// For 2 more inlets
	long r_inletNumber;			// For proxy to put inlet #
	void *r_out1;				// Sends tatum # after all messages for a tatum are sent
	void *r_out2;				// Messages output at the appropriate time
	long r_currentTatum;		// Current tatum number
	long r_referenceTatum;		// "Reference" tatum number for 3rd inlet
	long r_bufPtr;				// Address of ring buffer element scheduled to occur next
	long r_bufSize;				// # tatums in the ring buffer
	StoredList **r_ringBuffer;	// Array of pointers to StoredLists
	long r_maxListSize;			// Most args of any StoredList (max argc).
	long r_maxNumEvents;		// Number of events we have memory to store
	StoredList *r_allEvents;		// Pointer to original block of event memory (so we can free it)
	StoredList *r_freeList;		// Linked list of StoredLists not currently in use
	StoredList *r_endFreeList;	// Ptr to last element of freeList
	Atom *r_allAtomStorage;		// All the Atom memory in all the StoredLists
	short r_verbose;			// Boolean: are we in verbose mode?
} RhythmicScheduler;

/* global necessary for 68K function macros to work */
fptr *FNS;

/* globalthat holds the class definition */
void *RhythmicScheduler_class;

/* prototypes */

void *RhythmicScheduler_new(long bufSize, long numEvents, long maxListSize);
void RhythmicScheduler_free(RhythmicScheduler *x);
void RhythmicScheduler_toggleVerbosity(RhythmicScheduler *x);
void RhythmicScheduler_setRefTatum(RhythmicScheduler *x, long referenceTatum);
void RhythmicScheduler_setCurrentTatum(RhythmicScheduler *x, long newCurrentTatum);

void RhythmicScheduler_anything (RhythmicScheduler *x, Symbol *s, short argc, Atom *argv);
void InsertEvent (RhythmicScheduler *x, long refTatum, Symbol *s, short argc, Atom *argv);
void RhythmicScheduler_bang(RhythmicScheduler *x);
void RhythmicScheduler_clear(RhythmicScheduler *x);
StoredList *AllocEvent(RhythmicScheduler *x);
void FreeEvent(RhythmicScheduler *x, StoredList *l);


/* initialization routine */

void main(fptr *f) {
	long oldA4;
	
	oldA4 = SetCurrentA4();
	RememberA4();
	
	FNS = f;	

	version(0);

	setup(&RhythmicScheduler_class, RhythmicScheduler_new, (method) RhythmicScheduler_free, 
		  (short)sizeof(RhythmicScheduler), 0L, A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);

        addmess((method)version, "version", 0);
	addmess((method)RhythmicScheduler_toggleVerbosity, "verbose", 	0);
	addmess((method)RhythmicScheduler_setRefTatum, "reference_tatum", A_LONG, 0);
	addmess((method)RhythmicScheduler_setCurrentTatum, "current_tatum", A_LONG, 0);
	addmess((method)RhythmicScheduler_anything, "anything",	A_GIMME,0);
	addmess((method)RhythmicScheduler_clear, "clear",	0);

	addbang((method)RhythmicScheduler_bang);

	RestoreA4(oldA4);
}

/* instance creation routine */

void *RhythmicScheduler_new(long bufSize, long numEvents, long maxListSize) {
	RhythmicScheduler *x;
	int i;
	StoredList *list;
	
	EnterCallback();
	x = newobject(RhythmicScheduler_class);
	
	if (bufSize <= 0) bufSize = DEFAULT_BUFFER_SIZE;
	if (numEvents <= 0) numEvents = DEFAULT_NUM_EVENTS;
	if (maxListSize <= 0) maxListSize = DEFAULT_MAX_LIST_SIZE;
	
	post("RhythmicScheduler: buffer size %ld, %ld events, max list size %ld", bufSize, numEvents, maxListSize);
	
	// Allocate memory
	
	x->r_ringBuffer = (StoredList **) getbytes((short) (bufSize * sizeof(StoredList *)));
	if (x->r_ringBuffer == 0) goto noMemory;
	x->r_bufSize = bufSize;
		
	x->r_allEvents = (StoredList *) getbytes((short) (numEvents * sizeof(StoredList)));
	if (x->r_allEvents == 0) {
		freebytes(x->r_ringBuffer, (short) (bufSize * sizeof(StoredList)));
		goto noMemory;
	}
/*	
	post("maxListSize %ld, numEvents %ld, sizeof(atom) %ld", maxListSize, numEvents, (long) sizeof(Atom));
	post("Asking for %ld bytes for atoms", (long) (maxListSize * numEvents * sizeof(Atom)));
*/	
	x->r_allAtomStorage = (Atom *) getbytes ((short) (maxListSize * numEvents * sizeof(Atom)));
	if (x->r_allAtomStorage == 0) {
		freebytes(x->r_ringBuffer, (short) (bufSize * sizeof(StoredList)));
		freebytes(x->r_allEvents, (short) (numEvents * sizeof(StoredList)));
		goto noMemory;
	}
	x->r_maxListSize = maxListSize;
	x->r_maxNumEvents = numEvents;
	
	
	// Initialize ring buffer (all empty)
	for (i = 0; i < bufSize; ++i) {
		x->r_ringBuffer[i] = 0;
	}
	
	// Set up free list and make each stored list point to some atoms
	list = x->r_freeList = x->r_allEvents;
	for (i = 0; i<numEvents; ++i) {
		list[i].next = &(list[i+1]);
		list[i].symbol = 0;
		list[i].argc = 0;
		list[i].argv = x->r_allAtomStorage + (i * maxListSize);
	}
	list[numEvents-1].next = 0;			// terminates free list
	x->r_endFreeList = &(list[numEvents-1]);
	
	
	// Make proxies
	x->r_proxy[1] = proxy_new(x,2L,&x->r_inletNumber);
	x->r_proxy[0] = proxy_new(x,1L,&x->r_inletNumber);
	
	// Make outlets (in right to left order)
	x->r_out2 = outlet_new(x, 0L);
	x->r_out1 = intout(x);

	// Initialize other fields
	x->r_currentTatum = 0;
	x->r_referenceTatum = -1;
	x->r_bufPtr = 0;
	x->r_verbose = true;
		
	ExitCallback();
	return (x);							// return newly created object to caller
	
	noMemory:
		ouchstring("RhythmicScheduler: not enough memory to allocate object.");
		return 0;
}

void RhythmicScheduler_free(RhythmicScheduler *x) {
	EnterCallback();
	freebytes(x->r_ringBuffer, (short) (x->r_bufSize * sizeof(StoredList *)));
	freebytes(x->r_allEvents, (short) (x->r_maxNumEvents * sizeof(StoredList)));
	freebytes(x->r_allAtomStorage, (short) x->r_maxListSize * x->r_maxNumEvents * sizeof(Atom));
	
	freeobject(x->r_proxy[0]);
	freeobject(x->r_proxy[1]);
	ExitCallback();
}

void RhythmicScheduler_toggleVerbosity(RhythmicScheduler *x) {
	EnterCallback();
	x->r_verbose = !x->r_verbose;
	
	if (x->r_verbose)
		post("RhythmicScheduler: verbose on");
	else
		post("RhythmicScheduler: verbose off");
	
	ExitCallback();
}

void RhythmicScheduler_setRefTatum(RhythmicScheduler *x, long referenceTatum) {
	EnterCallback();
	x->r_referenceTatum = referenceTatum;
	ExitCallback();
}
	
void RhythmicScheduler_setCurrentTatum(RhythmicScheduler *x, long newCurrentTatum) {
	EnterCallback();
	x->r_currentTatum = newCurrentTatum;
	if (x->r_verbose)
		post("RhythmicScheduler: current tatum now %ld", x->r_currentTatum);
	else
	ExitCallback();
}


void RhythmicScheduler_anything (RhythmicScheduler *x, Symbol *s, short argc, Atom *argv) {
	EnterCallback();
	
	// Figure out which inlet the message came to and dispatch it

	if (x->r_inletNumber == 0) {
		post("RhythmicScheduler: doesn't understand %s in left inlet", s->s_name);
	} else if (x->r_inletNumber == 1) {
		// Inlet #1 (second from left) schedules events with respect to the current tatum.
		InsertEvent(x, x->r_currentTatum, s, argc, argv);
	} else if (x->r_inletNumber == 2) {
		// Inlet #2 (rightmost) schedules events with respect to the reference tatum.
		if (x->r_referenceTatum < 0) {
			post("RhythmicScheduler: no reference tatum; can't schedule with third inlet yet.");
		} else {
			InsertEvent(x, x->r_referenceTatum, s, argc, argv);
		}
	} else {
		post("еее RhythmicScheduler: this can't happen!");
	}

	ExitCallback();
}

void InsertEvent (RhythmicScheduler *x, long refTatum, Symbol *s, short argc, Atom *argv) {
	// Ignore what the symbol is.  The first element of the list is the tatum number.
	
	long targetTatum, bufferIndex, i;
	StoredList *l;
	
	if (argc == 0) return;
	if (argv[0].a_type != A_LONG) {
		post("RhythmicScheduler: first element of list to be ");
		post("  scheduled must be int relative tatum number");
		return;
	}

	targetTatum = argv[0].a_w.w_long + refTatum;
	if (targetTatum < x->r_currentTatum) {
		if (x->r_verbose) {
			post("RhythmicScheduler: you want to schedule something at ");
			post("  tatum %ld, but it's already tatum %ld!", targetTatum, x->r_currentTatum);
		}		
		return;
	}
	
	if (targetTatum >= x->r_currentTatum + x->r_bufSize) {
		if (x->r_verbose) {
				post("RhythmicScheduler: You want to schedule something at ");
				post("  tatum %ld, but it's only tatum %ld and the buffer", targetTatum, x->r_currentTatum);
				post("  holds only %ld tatums, so I can't remember it.", x->r_bufSize);
		}		
		return;
	}
	
	if (argc > x->r_maxListSize) {
		if (x->r_verbose) {
			post("RhythmicScheduler: can't store a list of size %ld because max size is %ld",
				 argc, x->r_maxListSize);
		}
		return;
	}

	/* Allocate an event object and fill it with the data. */
	l = AllocEvent(x);
	if (l == 0) {
		if (x->r_verbose) {
			post("RhythmicScheduler: No memory to store another event!");
		}
		return;
	}
	
	/* Don't store argv[0]! */
	l->symbol = s;
	l->argc=argc-1;
	for (i = 1; i < argc; ++i) {
		l->argv[i-1] = argv[i];
	}
	
	/* Insert the event object into the ring buffer */
	bufferIndex = (x->r_bufPtr + (targetTatum - x->r_currentTatum)) % x->r_bufSize;

	l->next = x->r_ringBuffer[bufferIndex];
	x->r_ringBuffer[bufferIndex] = l;
	
	// By golly; I think we're done.
}

void RhythmicScheduler_bang(RhythmicScheduler *x) {
	StoredList *l, *next;
	EnterCallback();
	
	for (l = x->r_ringBuffer[x->r_bufPtr]; l != 0; ) {
		outlet_anything(x->r_out2, l->symbol, l->argc, l->argv);
		next = l->next;
		FreeEvent(x, l);
		l = next;
	}
	x->r_ringBuffer[x->r_bufPtr] = 0;
	
	outlet_int(x->r_out1, x->r_currentTatum);
	
	++(x->r_currentTatum);
	x->r_bufPtr = ((x->r_bufPtr + 1) % x->r_bufSize);
	
	ExitCallback();
}


void RhythmicScheduler_clear(RhythmicScheduler *x) {
	StoredList *l, *next;
	int i;
	EnterCallback();
	
	
	for (i = 0; i < x->r_bufSize; ++i) {
		for (l = x->r_ringBuffer[i]; l != 0; ) {
			next = l->next;
			FreeEvent(x, l);
			l = next;
		}
		x->r_ringBuffer[i] = 0;
	}
	
	ExitCallback();
}


// Trivial memory manager for event structures (using free list)

StoredList *AllocEvent(RhythmicScheduler *x) {
	StoredList *result;
	if (x->r_freeList == x->r_endFreeList) {
		if (x->r_verbose) {
			post("RhythmicScheduler: no more free events!");
		}
		return 0;
	}
	
	result = x->r_freeList;
	x->r_freeList = x->r_freeList->next;
	return result;
}

void FreeEvent(RhythmicScheduler *x, StoredList *l) {
	l->next = 0;
	x->r_endFreeList->next = l;
	x->r_endFreeList = l;
}
