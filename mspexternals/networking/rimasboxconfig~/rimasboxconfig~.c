/*

Copyright (c) 2001.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matthew Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Based on sample code from David Zicarelli.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

/* rimasboxconfig~

	Version 0.0 10/31/01

 */

#define RIMASBOXCONFIG_VERSION "0.2"
#define NUM_REQUESTS 20  // number of linked list nodes allocated per object


#include "ext.h"
#include "z_dsp.h"

void *rbc_class;


/* Values to be sent to the Rimas box are stored in a linked list so we can put them
   in the appropriate place in the outgoing sample.  */

struct requestListStruct {
	float fakeSampleValue;	
	int next;	// Index in object's array of these structs of the next in the list
};

#define BITS_AS_INT(f) *((int *) &(f))  // For printing fakeSampleValue as %x instead of %f


typedef struct _RIMASBOXCONFIG
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    /* The data structures keeping track of outgoing requests */
    int firstPending;	   				// Linked list of pending requests
    int lastPending;					// For FIFO insertion at end of queue
    int firstFree;		   				// Linked list of free requestListStruct objects
    struct requestListStruct *requests; // Array of all  requestListStruct objects
   
    int packetPosition;	   // Counts outgoing samples so we only write into every 64th one
    
    
    int verbose;			// Are we in verbose mode?
    /* The ADC's "Analog Gain Control" register sets two channels at once, so we need to remember
       the current gain for each channel. */
    int currAnalogGain[8];				// Current analog gain of each channel
    
} t_rbc;    


/* Magic numbers in this protocol */

/* The settings for the "A/D" bit, which chooses ADC versus DAC.*/
#define ADBIT_ADC 0
#define ADBIT_DAC 1

/* The registers */
#define REGISTER_IO_AND_POWER 1
#define REGISTER_INTERFACE 2
#define REGISTER_ANALOG_IO 3
#define REGISTER_LEFT_VOLUME 4
#define REGISTER_RIGHT_VOLUME 5
#define REGISTER_ANALOG_GAIN 6
#define REGISTER_CLIP_DETECTION 7


void main(void);
void *rbc_new(void);
void rbc_free(t_rbc *x);
void rbc_dsp(t_rbc *x, t_signal **sp, short *count);
void rbc_register(t_rbc *x, long ad, long whichADC, long reg, long val);
void rbc_adcgain(t_rbc *x, long channelPlusOne, long dbGain);
void rbc_analoggaincontrol(t_rbc *x, long channel, long dbGain);
t_int *rbc_perform(t_int *w);
void rbc_dud(t_rbc *x);
void rbc_verbose(t_rbc *x, long verbose);
void rbc_tellmeeverything(t_rbc *x);
void rbs_InitRequestLists(t_rbc *x);
int rbs_GetFreeRequest(t_rbc *x);
void rbs_FreeRequest(t_rbc *x, int r);
void rbs_ScheduleRequest(t_rbc *x, int r);
int rbs_NextScheduledRequest(t_rbc *x);





void main(void) {
	post("rimasboxconfig~ version " RIMASBOXCONFIG_VERSION " by Matt Wright");
	post("Copyright © 2001 Regents of the University of California.  ");

    setup(&rbc_class, rbc_new, (method) rbc_free, (short)sizeof(t_rbc), 0L, 0);
    addmess((method)rbc_dsp, "dsp", A_CANT, 0);
    addmess((method)rbc_register, "register", A_LONG, A_LONG, A_LONG, A_LONG, 0);
    addmess((method)rbc_adcgain, "adcgain", A_LONG, A_LONG, 0);
    addmess((method)rbc_analoggaincontrol, "analoggaincontrol", A_LONG, A_LONG, 0);
    addmess((method)rbc_verbose, "verbose", A_LONG, 0);
    addmess((method)rbc_tellmeeverything, "tellmeeverything", 0);
    addmess((method)rbc_dud, "dud", 0);
    
    dsp_initclass();
}

void *rbc_new(void) {
	t_rbc *x;
	int i;
		
	x = (t_rbc *)newobject(rbc_class);  // dynamic alloc
	
	x->requests = (struct requestListStruct *) getbytes(NUM_REQUESTS * sizeof(struct requestListStruct));
	rbs_InitRequestLists(x);
	
	x->packetPosition = 0;
	
	x->verbose = 0;
	
	for (i=0; i<8; ++i) {
		x->currAnalogGain[i] = 0;
	}	

  	dsp_setup((t_pxobject *)x,1);  // one inlet
	outlet_new((t_object *)x, "signal");
    return x;
}

void rbc_free(t_rbc *x) {
	freebytes(x->requests, NUM_REQUESTS * sizeof(*(x->requests)));
	dsp_free(&(x->x_obj));
}


void rbc_dsp(t_rbc *x, t_signal **sp, short *count) {
	// x->packetPosition = 0;
	dsp_add(rbc_perform, 4, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n, x);  // in, out, size, x
}

 #define WHICH_SAMPLE_TO_CHANGE 63	// Last sample of every other 32-sample I/O vector

t_int *rbc_perform(t_int *w) {
	t_float val;
    t_float *in = (t_float *)(w[1]);  // input
    t_float *out = (t_float *)(w[2]); // output
    int size = w[3]; // vector size
    t_rbc *x = (t_rbc *)(w[4]);

	// calculation loop
	while (size--) {
		val = *in++;

		if (x->packetPosition == WHICH_SAMPLE_TO_CHANGE) {
			int r = rbs_NextScheduledRequest(x);
			
			if (r == -1) {
				/* Nothing to request right now. */
				*out++ = 0.0;
			} else {
				/* Write into this sample */
					
				*out++ = x->requests[r].fakeSampleValue;
				

#define xxxREPEAT_SAME_REQUEST			
#ifdef REPEAT_SAME_REQUEST
				rbs_ScheduleRequest(x, r);				
#else			
				if (x->verbose) {
					post("rimasboxconfig~: Just wrote 0x%x from request [%ld]", 
					 BITS_AS_INT(x->requests[r].fakeSampleValue), r);
				}

				rbs_FreeRequest(x, r);
#endif	
			}
			x->packetPosition = 0;
		} else {
			/* Just copy input to output unchanged */
			*out++ = val;
			++(x->packetPosition);
		}
	}	

    return (w+5);
}


/*
This text is from Rimas' email explanation of how to format the bits:

the last sample (4 bytes) that goes into the fake dac channel (which can also be
used for midi output) is interpreted by the ASIO driver as a value that gets written
into the registers on the ADCs/DACs on the guitar board.

the driver actually only uses the last 2 bytes (16 bits).  here's is how they are formatted

| 16 | 15 | 14 | 13 | 12 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
 flag  a/d (address)  (--- reg addr ---)   (       value to be written         )

flag bit always is set high when you're writing

the a/d bit decides whether your addressing the DAC or ADC(s)

the address bits decide which of the 4 ADCs you're addressing (its ignored for the DACs
because there's only 1)

the register address decides which of the 8 registers on the ADC or DAC you're writing to

the value to be written is the byte of data you are writing into the register.

*/



void rbc_register(t_rbc *x, long ad, long whichADC, long reg, long val) {
	int r;
	float f;
	float *fp;
	unsigned char *cp;

	if (ad != 0 && ad != 1) {
		post("¥ rimasboxconfig~: A/D number must be 0 (ADC) or 1 (DAC)");
		return;
	}

	if (whichADC < 0 || whichADC > 3) {
		post("¥ rimasboxconfig~: A/D selector must be between 0 and 1");
		return;
	}

	if (reg < 1 || reg > 7) {
		post("¥ rimasboxconfig~: register number must be between 1 and 7");
		return;
	}
	
	if (val < 0 || val > 255) {
		post("¥ rimasboxconfig~: value must be between 0 and 255");
		return;
	}

	r = rbs_GetFreeRequest(x);
	if (r == -1) {
		post("¥ rimasboxconfig~: out of memory to store configuration information %d %d; dropping",
			 reg, val);
		return;
	}
		
	fp = &f;
	cp = (unsigned char *) fp;
	
	cp[0] = cp[1] = 0;
		
	cp[2] = 0x80 |               // Turn on "flag" to show we are writing a new value
			(ad ? 0x40 : 0) |    // A/D bit
			whichADC << 4   |    // "Address bits"
			reg;				 // Which register
				
	cp[3] = val;
		
	x->requests[r].fakeSampleValue = f;
	
	if (x->verbose) {
		post(" rimasboxconfig~: register(%d, %d, %d, %d) = 0x%x", 
			 ad, whichADC, reg, val, BITS_AS_INT(f));
	}

	rbs_ScheduleRequest(x, r);
}	


/* How the strings are hooked up to the ADC input channels:

	String 1: channel 0: ADC 0 right
	String 2: channel 1: ADC 0 left
	String 3: channel 2: ADC 1 right
	String 4: channel 3: ADC 1 left
	String 5: channel 4: ADC 2 right
	String 6: channel 5: ADC 2 left

	Additional channels (eventually) for the magnetic pickups: 
	Channel 6: ADC 3 right
	Channel 7: ADC 3 left
*/


void rbc_adcgain(t_rbc *x, long channelPlusOne, long dbGain) {
	long whichADC, channel, whichRegister;

	signed char charGain;
	long longGain;
	long *lp;
	char *cp;

	
	if (channelPlusOne < 1 || channelPlusOne > 8) {
		post("¥ rimasboxconfig~ adcgain: channel must be between 1 and 8");
		return;
	}
	
	if (dbGain < -96 || dbGain > 12) {
		post("¥ rimasboxconfig~ adcgain: gain must be between -96 and 12");
		return;
	}
	
	channel = channelPlusOne - 1;
	
	// See comment above about how channels relate to the four stereo ADCs
	whichADC = 	channel >> 1;  
	
	if (channel & 1) {
		// Odd-numbered channel 
		whichRegister = REGISTER_LEFT_VOLUME;
	} else {
		whichRegister = REGISTER_RIGHT_VOLUME;
	}

	// Convert 32-bit long (in the range -12 to 96) to an 8-bit 2's complement signed int

	charGain = dbGain;
	longGain = 0;
	lp = &longGain;
	cp = (char *) lp;
	cp[3] = charGain;

	// post("** db Gain %ld -> char 0x%x -> long 0x%x (%ld)", dbGain, charGain, longGain, longGain);


	// First set the "AINMUX" bits so that the output of the analog gain control
	// is what the A/D converter sees
	rbc_register(x, ADBIT_ADC, whichADC, REGISTER_IO_AND_POWER, 32);
	
	// Then set the gain
	rbc_register(x, ADBIT_ADC, whichADC, whichRegister, longGain);
}



void rbc_analoggaincontrol(t_rbc *x, long channelPlusOne, long dbGain) {
	long whichADC, val, channel;
	
	if (channelPlusOne < 1 || channelPlusOne > 8) {
		post("¥ rimasboxconfig~ analoggaincontrol: channel must be between 1 and 8");
		return;
	}
	
	
	if (dbGain < 0 || dbGain > 12) {
		post("¥ rimasboxconfig~ analoggaincontrol: gain must be between 0 and 12 dB");
		return;
	}
	
	channel = channelPlusOne - 1;
	x->currAnalogGain[channel] = dbGain;
		
	// See comment above about how channels relate to the four stereo ADCs
	whichADC = 	channel >> 1;
	
	// Need to put both the left and right channel gains in the same message, even
	// though the user's only setting one right now.
	val = x->currAnalogGain[whichADC * 2] << 4 | x->currAnalogGain[(whichADC * 2) + 1];

	if (x->verbose) {
		post("** You set gain for channel %d, so that's ADC %d and channels %d and %d: %x",
			 channel, whichADC, whichADC * 2, (whichADC * 2) + 1, val);
	}

	rbc_register(x, ADBIT_ADC, whichADC, REGISTER_ANALOG_GAIN, val);
}

void rbc_dud(t_rbc *x) {
	// Print out, instead of sending, the next request
	int r;
	
	r = rbs_NextScheduledRequest(x);
	
	if (r == -1) {
		post("none");
	} else {
		post("request [%ld] 0x%x", r, BITS_AS_INT(x->requests[r].fakeSampleValue));
		rbs_FreeRequest(x, r);
	}
}

void rbc_verbose(t_rbc *x, long verbose) {
	x->verbose = verbose;
		
	post("rimasboxconfig~: verbose mode %s", verbose ? "on" : "off");
}


void rbc_tellmeeverything(t_rbc *x) {
	int i;
	
	post("rimasboxconfig~ object currently at position %ld in output buffer", x->packetPosition);
	if (x->verbose) post("verbose mode");
	
	post("  pending requests:");
	for (i = x->firstPending; i != -1; i = x->requests[i].next) {
			post("    [%ld] value 0x%x (%f)", i, BITS_AS_INT(x->requests[i].fakeSampleValue), 
				 x->requests[i].fakeSampleValue);
	}
	
	post("  last pending request is %ld", x->lastPending);
	
	post("  free request structs (and their meaningless data):");
	for (i = x->firstFree; i != -1; i = x->requests[i].next) {
			post("    [%ld] value 0x%x (%f)", i, BITS_AS_INT(x->requests[i].fakeSampleValue), 
				 x->requests[i].fakeSampleValue);
	}
}				 

void rbs_InitRequestLists(t_rbc *x) {
	int i;
	
	// Construct linked lists
	x->firstPending = x->lastPending = -1;
	x->firstFree = 0;
	for (i = 0; i < NUM_REQUESTS; ++i) {
		x->requests[i].next = i+1;
	}
	x->requests[NUM_REQUESTS - 1].next = -1;
}

int rbs_GetFreeRequest(t_rbc *x) {
	int result = x->firstFree;
	
	if (result == -1) return -1;
	
	x->firstFree = x->requests[result].next;
	return result;
}

void rbs_FreeRequest(t_rbc *x, int r) {
	x->requests[r].next = x->firstFree;
	x->firstFree = r;
}


void rbs_ScheduleRequest(t_rbc *x, int r) {
	if (x->lastPending == -1) {
		/* There were no requests. */
		x->firstPending = r; 
	} else {
		x->requests[x->lastPending].next = r;		
	}

	x->lastPending = r;
	x->requests[r].next = -1;
}

int rbs_NextScheduledRequest(t_rbc *x) {
	int result = x->firstPending;
	
	if (result == -1) return -1;
	
	x->firstPending  = x->requests[result].next;
	if (x->firstPending == -1) x->lastPending = -1;
	
	return result;
}