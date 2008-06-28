/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 1996,97,98,99,2000,01,02,03,04,05
The Regents of the University of California (Regents).  

Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: slipOSC
DESCRIPTION: Encode and decode SLIP data to/from the Max serial object and the OSC Fullpacket message format
AUTHORS: Adrian Freed
COPYRIGHT_YEARS: 2007,2008
SVN_REVISION: $LastChangedRevision: 1212 $
VERSION 1.0: Another attempt to fix time tag byte-order bug
VERSION 1.01: Cleanup for release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
     
*/

#include "ext.h"

#include "version.h"
#include "version.c"

#ifdef WIN_VERSION
// To get ntohl() on Windows
// #include <asm/byteorder.h>
// #include </usr/include/asm/byteorder.h>
#include </usr/include/w32api/winsock2.h>
#endif

void *sOSC_class;
Symbol *ps_gimme, *ps_OSCTimeTag, *ps_FullPacket, *ps_OSCBlob;

#define MAXSLIPBUF 2048

typedef struct slipOSC {
	struct object O_ob;
		void *m_proxy;
	long m_inletNumber;

	void *O_outlet1;	// Fullpacket out
	void *O_outlet2;	// int lists representing one byte per int
	void *O_outlet3;	// debugging etc
	short	O_debug;
  
	char slipibuf[MAXSLIPBUF];
	int icount;
	short istate; // initialize to 0
	Atom  *out;
	int errorreporting;	  // Does this object report errors in the Max window?
} sOSC;


void *sOSC_new(long arg);
void sOSC_assist(sOSC *x, void *b, long m, long a, char *s);
void sOSC_debug (sOSC *x);
void sOSC_errorreporting(sOSC *x, int yesno);

void sOSC_send (sOSC *x);

void sOSC_bang (sOSC *x);
void sOSC_readtypestrings(sOSC *x, int yesno);
void sOSC_writetypestrings(sOSC *x, int yesno);
void sOSC_printcontents (sOSC *x);

void sOSC_accumulateMessage(sOSC *x, char *messageName, short argc, Atom *argv);
int sOSC_stringSubstitution(char *target, char *format, short *argcp, Atom **argvp);
void sOSC_sendBuffer(sOSC *x);
void sOSC_sendData(sOSC *x, short size, char *data);

#ifdef __MWERKS__
#define DONT_HAVE_STRING_LIBRARY
/* KLUDGE!!! 
If we're on metrowerks we must be compiling for PPC which is big-endian, so since I don't know
where to link in the real ntohl() I'll define this evil macro: */
#define ntohl(x) (x)
#endif

#ifdef DONT_HAVE_STRING_LIBRARY
void strcpy(char *s1, char *s2);
#endif

// SLIP codes
#define END             0300    // indicates end of packet 
#define ESC             0333    // indicates byte stuffing 
#define ESC_END         0334    // ESC ESC_END means END data byte 
#define ESC_ESC         0335    // ESC ESC_ESC means ESC data byte
void slipencodeFullPacket(sOSC *x, long size, unsigned char *source);
void slipencodeFullPacket(sOSC *x, long size, unsigned char *source)
{
if(((size%4)==0) && size<MAXSLIPBUF &&  size>4 && (source[0]=='/' || source[0]=='#'))
{
	int i=0;
	unsigned char c;
	
	x->out[i++].a_w.w_long  = END;
	for(i=0;i<size && i<MAXSLIPBUF-3;++i)
	{
		switch(c=*source++)
		{
			case END:
				x->out[i++].a_w.w_long = ESC;
			x->out[i].a_w.w_long = ESC_END;
		
			
			
			break;
			
			case ESC:
				x->out[i++].a_w.w_long = ESC;
			x->out[i].a_w.w_long = ESC_ESC;
		
			break;
			default:
			x->out[i].a_w.w_long = c;
		}
	}
	
	x->out[i++].a_w.w_long  = END;
#ifdef DEBUGOUTPUT
	{
					// full packet process		
					 char stringbuf[4096];
					 int j=0;
					 int p;
						for(p=0;p<i && p<4095;++p)
						{
							unsigned char c = x->out[p].a_w.w_long;
							if(c=='/' || c=='#' || c==' ' ||  (c>='a' && c<='z')  || (c>='A' && c<='Z') || (c<='9' && c>='0') )
								stringbuf[j++] = c;
							else
								stringbuf[j++] = '*';
						}
						stringbuf[j++] = '\0';
					    post("packet %d %s", i, stringbuf);
}
#endif
	outlet_list(x->O_outlet2,0L,(short)i,x->out);
 
}
else 
	post("slipOSC: bad fullpacket");

}

void slipdecode(sOSC *x, unsigned char c)
{
	
	switch(x->istate)
	{
		case 0: // waiting for packet to start
			x->istate = 1;
			if(c==END)
				break;
			
		case 1: // packet has started
			switch(c )
			{
				case END:
					if((x->icount>0) ) // it was the END byte
					{ 
#ifdef DEBUGOUTPUT					
					// full packet process		
					 char stringbuf[4096];
					 int j=0;
					 int i;
						for(p=0;i<x->icount && i<4095;++i)
						{
							char c = x->slipibuf[i];
							if(c=='/' || c=='#' || (c>='a' && c<='z')  || (c>='A' && c<='Z') || (c<='9' && c>='0') )
								stringbuf[j++] = c;
							else
								stringbuf[j++] = '*';
						}
						stringbuf[j++] = '\0';
					    post("slipOSC: packet %d %s", x->icount, stringbuf);
#endif
//					 ParseOSCPacket(x, x->slipibuf, x->icount, true);
			if((x->icount%4) ==0)
				sOSC_sendData(x, x->icount, x->slipibuf);
			
						x->icount = 0;
					}
					x->istate = 0;
					break;
				case ESC:
					x->istate =2;
					break;

						// otherwise, just stick it in the packet	 buffer	
				default:
					if(x->icount<MAXSLIPBUF)
					{	x->slipibuf[(x->icount)++] = c; }
					else
						x->istate = 3;
					break;
			}
		break;
		case 2: // process escapes
				switch( c )
				{
							case ESC_END:
									if(x->icount<MAXSLIPBUF)
									{	
										x->slipibuf[(x->icount)++] = END;
										x->istate = 1;
									}
									else
										x->istate = 3;
									
								break;
							case ESC_ESC:
									if(x->icount<MAXSLIPBUF)
									{	
										x->slipibuf[(x->icount)++] = ESC;
										x->istate = 1;
									}
									else
										x->istate = 3;
								break;
							default:
								post("slipOSC: ESC not followed by ESC_END or ESC_ESC.");
								x->istate = 3;
					}
					break;
		case 3:   // error state: hunt for END character (this should probably be a hunt for a non escaped END character..
				if( c == END)
				{
					x->icount = 0;
					x->istate = 0;
				}
				break;

	}
}
void slipbyte(sOSC *x, long n)
{
	if((x->m_inletNumber==1) && n<256)
		slipdecode(x, n);
}
void	sliplist(sOSC *x, struct symbol *s, int argc, struct atom *argv)
{
	int i;
	if(x->m_inletNumber!=1)
		return;
	for(i=0; i<argc; ++i) {
	 	if(argv[i].a_type != A_LONG) {	
	 		post("slipOSC: list did not contain only integers; dropping");
	 		return;
	 	}
	}

	for(i=0;i<argc;++i) {
	 	int  e = argv[i].a_w.w_long;	
		if(e<256)
			slipdecode(x,e);
	}
}
void *myobject_free(sOSC *x);
void *myobject_free(sOSC *x)
{
  freeobject(x->m_proxy);
}
void main (fptr *f) {	
 //       version(0);
		
	setup((t_messlist **)&sOSC_class, (method) sOSC_new,(method) myobject_free,(short)sizeof(sOSC),0L,A_DEFLONG,0);

#ifdef SANITY_CHECK
	post("*** sizeof(int4byte) = %ld", (long) sizeof(int4byte));
	post("*** sizeof(long) = %ld", (long) sizeof(long));
#endif

	addmess((method)sOSC_assist, "assist",	A_CANT,0);
//	addmess((method)version, "version", 	0);
	addmess((method)sOSC_debug, "debug", 	0);
	addmess((method)sOSC_errorreporting, "errorreporting", 	A_LONG, 0);


	addint((method)slipbyte);
	addmess((method)sliplist, "list",A_GIMME, 0);


	addmess((method)sOSC_printcontents, "printcontents", 0);
	addmess((method)slipencodeFullPacket, "FullPacket", A_LONG, A_LONG, 0);
	

	finder_addclass("Devices","slipOSC");
	//	rescopy('STR#',3009);
	ps_OSCTimeTag = gensym("OSCTimeTag");
	ps_FullPacket = gensym("FullPacket");
 }



void *sOSC_new(long arg) {
	sOSC *x;
	char *buf;
	
	x = (sOSC *) newobject(sOSC_class);
	x->m_proxy = proxy_new(x,1L,&x->m_inletNumber);

	/* Create the outlets in right to left order */
	x->O_outlet3 = outlet_new(x, "OSCTimeTag");
	x->O_outlet2 = outlet_new(x, 0L);
	x->O_outlet1 = outlet_new(x, 0L);


	x->O_debug = false;

	x->icount = 0;
	x->istate = 0;
	x->out = 		(Atom *) getbytes(MAXSLIPBUF * sizeof(Atom));
	
	if (x->out == 0) {
		post("slipOSC: not enough memory for capacity %ld!",MAXSLIPBUF);
		
		return 0;
	}
	
	{
		int i;
		for (i = 0; i<MAXSLIPBUF; ++i) {
			x->out[i].a_type = A_LONG;
		}
	}

	

	
	return (x);
}

void sOSC_assist(sOSC *x, void *b, long m, long a, char *dst) {
	if (m == ASSIST_INLET) {
		if (a == 0) {
			strcpy(dst, "Max msgs; OSC packets");
		} else {
			error("sOSC_assist: unrecognized inlet number %ld", a);
		}
	} else if (m == ASSIST_OUTLET) {
		if (a == 0) {
			strcpy(dst, "OSC packets; bang after msgs in OSC packet");
		} else if (a == 1) {
			strcpy(dst, "OSC msgs decoded from OSC packet");
		} else if (a == 2) {
			strcpy(dst, "Time tag (list of two ints) from OSC packet");
		} else {
			error("sOSC_assist: unrecognized outlet number %ld", a);
		}
	} else {
		error("Unexpected msg %ld in sOSC_assist", m);
	}
}

void sOSC_debug (sOSC *x) {
	x->O_debug = !x->O_debug;
	
	if (x->O_debug)
		post("slipOSC: debug on");
	else
		post("slipOSC: debug off");
}

void sOSC_errorreporting(sOSC *x, int yesno) {
	x->errorreporting = yesno;
	
	if (yesno) {
		post("slipOSC: turning on error reporting.");
	} else {
		post("slipOSC: turning off eror reporting.  What you don't know can't hurt you.");
	}
}


void sOSC_bang (sOSC *x) {
	if (x->O_debug) {
		post("slipOSC: BANG! Sending buffer and resetting");
	}
}

void sOSC_sendData(sOSC *x, short size, char *data) {
	Atom arguments[2];
	
	if (x->O_debug) {
		post("slipOSC: Sending buffer (%ld bytes)", (long) size);
	}
	
	SETLONG(&arguments[0], (long) size);
	SETLONG(&arguments[1], (long) data);
	outlet_anything(x->O_outlet1, ps_FullPacket, 2, arguments);


}

#define MAX_ARGS_TO_sOSC_MSG 1024

#ifdef DONT_HAVE_STRING_LIBRARY
void strcpy(char *s1, char *s2) {
	while (*s1++ = *s2++);
}
#endif

#define PRINTABLE(c) ((char) (c>= 0x20 && c <= 0x7e ? c : 'û'))
#define isprint(c) ((c) >= 0x20 && (c) <= 0x7e)

void sOSC_printcontents (sOSC *x) {
	char *m, buf[100], *p;
	int n, i;
	
	m = x->slipibuf;
	n = x->icount;
	
	post("sOSC_printcontents: buffer %p, size %ld", m, (long) n);

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
	    }
	}
}
	
#ifdef PRECOMPUTE_SIZES
/* In the old days, we used to have to compute the size of our messages
   by hand to see if there was room in the buffer. */
   
int sOSC_messageSize(char *messageName, short argc, Atom *argv) {
	int result;
	int i;
	
	/* First, we need space for the messageName */
	result = sOSC_effectiveStringLength(messageName);	
	slipOSC.c:266: warning: return type of 'main' is not 'int'

	/* Now account for the arguments */
	for (i = 0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				result += sizeof(long);
				break;

			case A_FLOAT:
				result += sizeof(float);
				break;
			
			case A_SYM:
				result += sOSC_effectiveStringLength(argv[i].a_w.w_sym->s_name);
				break;
			
			default:
				error("slipOSC: unrecognized argument type");
				break;
		}
	}

	return result;
}
#endif


/*******************************************************************
 Stuff having to do with parsing incoming OSC packets into Max data
 *******************************************************************/

void ParseOSCPacket(sOSC *x, char *buf, long n, Boolean topLevel);
char *DataAfterAlignedString(char *string, char *boundary); 
Boolean IsNiceString(char *string, char *boundary);
#ifdef DONT_HAVE_STRING_LIBRARY
int strncmp(char *s1, char *s2, int n);
#endif

char *htm_error_string;	// Used for error messages


#define SMALLEST_POSITIVE_FLOAT 0.000001f
#define MAXARGS 500


#define STRING_ALIGN_PAD 4

char *DataAfterAlignedString(char *string, char *boundary) 
{
    /* The argument is a block of data beginning with a string.  The
       string has (presumably) been padded with extra null characters
       so that the overall length is a multiple of STRING_ALIGN_PAD
       bytes.  Return a pointer to the next byte after the null
       byte(s).  The boundary argument points to the character after
       the last valid character in the buffer---if the string hasn't
       ended by there, something's wrong.

       If the data looks wrong, return 0, and set htm_error_string */

    int i;

    if ((boundary - string) %4 != 0) {
		ouchstring("slipOSC: Internal error: DataAfterAlignedString: bad boundary\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (string + i >= boundary) {
		    htm_error_string = "DataAfterAlignedString: Unreasonably long string";
		    return 0;
		}
    }

    /* Now string[i] is the first null character */
    i++;

    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string + i >= boundary) {
		    htm_error_string = "DataAfterAlignedString: Unreasonably long string";
		    return 0;
		}
		if (string[i] != '\0') {
		    htm_error_string = "DataAfterAlignedString: Incorrectly padded string.";
		    return 0;
		}
    }

    return string+i;
}

Boolean IsNiceString(char *string, char *boundary)  {
    /* Arguments same as DataAfterAlignedString().  Is the given "string"
       really a string?  I.e., is it a sequence of isprint() characters
       terminated with 1-4 null characters to align on a 4-byte boundary? */

    int i;

    if ((boundary - string) %4 != 0) {
		ouchstring("slipOSC: Internal error: IsNiceString: bad boundary\n");
		return 0;
    }

    for (i = 0; string[i] != '\0'; i++) {
		if (!isprint(string[i])) return FALSE;
		if (string + i >= boundary) return FALSE;
    }

    /* If we made it this far, it's a null-terminated sequence of printing characters 
       in the given boundary.  Now we just make sure it's null padded... */

    /* Now string[i] is the first null character */
    i++;
    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
		if (string[i] != '\0') return FALSE;
    }

    return TRUE;
}

#ifdef DONT_HAVE_STRING_LIBRARY
int strncmp(char *s1, char *s2, int n) {
	while (n > 0) {
		if (*s1 != *s2) return *s2 - *s1;
		if (*s1 == 0) return 0;
		s1++; s2++; n--;
	}
}
#endif
