/*
 * Copyright (c) 1996, 1997 Regents of the University of California.
 * All rights reserved.
 * The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

  /* 
        Author: Matt Wright
        
        6/29/99
        From Max OpenSoundControl object

   */

#define RIMASUDPCOUNTER_VERSION "0.0"

#include "ext.h"
#include <SetUpA4.h>
#include <A4Stuff.h>


fptr *FNS;
void *RUC_class;

typedef struct ruc {
	struct object O_ob;
	void *O_outlet;
} RUC;


void *RUC_new(long arg);
void RUC_assist(RUC *x, void *b, long m, long a, char *s);
void RUC_version (RUC *x);
void RUC_ParseFullPacket(RUC *x, long size, long bufptr);

void main (fptr *f) {
	long oldA4;

	oldA4 = SetCurrentA4();
	RememberA4();
	FNS = f;		
	
	post("RimasUDPCounter object version " RIMASUDPCOUNTER_VERSION " by Matt Wright");
	post("Copyright © 1999 Regents of the University of California.  ");
		
	setup(&RUC_class,RUC_new,0L,(short)sizeof(RUC),0L,0);


	addmess((method)RUC_assist, "assist",	A_CANT,0);
	addmess((method)RUC_version, "version", 	0);
	addmess((method)RUC_ParseFullPacket, "FullPacket", A_LONG, A_LONG, 0);

	rescopy('STR#',3009);

quit:
	RestoreA4(oldA4);
}

#define DEFAULT_BUFFER_SIZE 1024

void *RUC_new(long arg) {
	RUC *x;
	char *buf;

	EnterCallback();
	
	x = (RUC *) newobject(RUC_class);
	x->O_outlet = outlet_new(x, 0L);

	ExitCallback();
	return (x);
}

void RUC_assist(RUC *x, void *b, long m, long a, char *s) {
	EnterCallback();
	assist_string(3009,m,a,1,2,s);
	ExitCallback();
}

void RUC_version (RUC *x) {
	EnterCallback();
	post("OpenSoundControl Version " RIMASUDPCOUNTER_VERSION
		  ", by Matt Wright. Compiled " __TIME__ " " __DATE__);	
	ExitCallback();
}


#if 0
#define PRINTABLE(c) ((char) (c>= 0x20 && c <= 0x7e ? c : 'û'))
#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)

void RUC_printcontents (RUC *x) {
	char *m, buf[100], *p;
	int n, i;
	
	EnterCallback();

	m = (x->b).buffer;
	n = RUC_packetSize(&(x->b));
	
	post("RUC_printcontents: buffer %p, size %ld", m, (long) n);
	if (n % 4 != 0) {
		post("Hey, the size isn't a multiple of 4!");
	} else {
	    for (i = 0; i < n; i += 4) {
	    	p = buf;
	    	
	    	p += sprintf(p, "  %x", m[i]);
	    	if (isprint(m[i])) {
	    		p += sprintf(p, "  (%c)", m[i]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+1]);
	    	if (isprint(m[i+1])) {
	    		p += sprintf(p, "  (%c)", m[i+1]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+2]);
	    	if (isprint(m[i+2])) {
	    		p += sprintf(p, "  (%c)", m[i+2]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	p += sprintf(p, "  %x", m[i+3]);
	    	if (isprint(m[i+3])) {
	    		p += sprintf(p, "  (%c)", m[i+3]);
	    	} else {
	    		p += sprintf(p, "  ()");
	    	}
	    	
	    	*p = '\0';
	    	post(buf);

	    	
/*	        post("  %x (%c)  %x (%c)  %x (%c)  %x (%c)  ", 
	    		 m[i], PRINTABLE(m[i]), m[i+1], PRINTABLE(m[i+1]),
	    		 m[i+2], PRINTABLE(m[i+2]), m[i+3], PRINTABLE(m[i+3])); */
	    }
	}
    
	ExitCallback();
}
	
#endif




/*******************************************************************
 Stuff having to do with parsing incoming UDP packets into Max data
 *******************************************************************/



void RUC_ParseFullPacket(RUC *x, long size, long bufptr) {
	Atom args[2];
	char *bytes;
	short *shortPtr;
	long count;
	
	EnterCallback();
	
	bytes = (char *)bufptr;
	
	/* Last two bytes of packet are the count we need to output */
	shortPtr = (short *) (bytes+size-2);
	count = (long) (*shortPtr);
	
	SETLONG(&args[0], count);
	SETLONG(&args[1], size);
	
	outlet_list(x->O_outlet, 0, 2, args);
	
	ExitCallback();
}

