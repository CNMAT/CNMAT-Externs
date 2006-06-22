/*

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2004, The Regents of 
the University of California (Regents).  

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

*/


/*

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: ioi-report~
DESCRIPTION: Inter-onset-interval report for mostly-zero signals, i.e., a signal domain timer.
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2004,05
VERSION 0.1: First version, 5/28/4 
VERSION 0.2: 050714 added way to set min and max ioi time
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/


#define RINGBUF_SIZE 2048
#define MAX_LISTLEN 512

#include "version.h"
#include "ext.h"
#include "z_dsp.h"

void *ioi_report_class;

typedef struct _ioi_report
{
	/* Max stuff */
    t_pxobject x_obj;
    void *outlet;
    
    /* Parameters */
    int min_ioi_time;	/* report only IOIs >= this */
    int max_ioi_time;   /* report only IOIs <= this */
    int max_num_iois;	/* Report at most this many IOIs per event. */
    
    int now;			/* Current time, in samples, since object was created/reset */
        
    /* Ring buffer for recent memory */
    struct {
    	int when;
    	/* float value; */
    } ringBuf[RINGBUF_SIZE];
    
    int ringBufWritePos;
        
    Atom outputList[MAX_LISTLEN];
    
    float T;		/* 1/srate */
} t_ioi_report;


void main(void);
void ioi_report_version (t_ioi_report *x);
void ioi_report_assist(t_ioi_report *x, void *b, long m, long a, char *s);
void *ioi_report_new(Symbol *s, short argc, Atom *argv);
void ioi_report_free(t_ioi_report *x);
void ioi_report_reset(t_ioi_report *x);
void ioi_report_dsp(t_ioi_report *x, t_signal **sp, short *count);
t_int *ioi_report_perform(t_int *w);
void *OutputTimes(t_ioi_report *x, int eventTime, int bufPos);
void ioi_report_min_ioi_time(t_ioi_report *x, long min);
void ioi_report_max_ioi_time(t_ioi_report *x, long max);
void ioi_report_max_iois_per_event(t_ioi_report *x, long max);




void main(void) {

	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");

    setup((t_messlist **)&ioi_report_class, (method) ioi_report_new, (method)ioi_report_free, 
    	  (short)sizeof(t_ioi_report), 0L, A_GIMME, 0);
    addmess((method)ioi_report_dsp, "dsp", A_CANT, 0);
    addmess((method)ioi_report_dsp, "reset", 0);
    addmess((method)ioi_report_version, "version", 0);
    addmess((method)ioi_report_assist,"assist",A_CANT,0);
    addmess((method)ioi_report_min_ioi_time, "min_ioi", A_LONG, 0);
    addmess((method)ioi_report_max_ioi_time, "max_ioi", A_LONG, 0);
    addmess((method)ioi_report_max_iois_per_event, "max_per_event", A_LONG, 0);

    dsp_initclass();
    rescopy('STR#',3241);
}

void ioi_report_version (t_ioi_report *x) {
	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}


/* I don't know why these aren't defined in some Max #include file. */
#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void ioi_report_assist(t_ioi_report *x, void *box, long msg, long arg, char *dstString) {	
	if (msg==ASSIST_INLET) {
		sprintf(dstString, "Mostly-zero signal with some nonzero events");
	} else if (msg==ASSIST_OUTLET) {
		sprintf(dstString, "List of inter-onset-intervals");
	} else {
		post("¥ ioi-report: unrecognized message %ld", msg);
	}
}

void *ioi_report_new(Symbol *s, short argc, Atom *argv) {
	int i;
	
    t_ioi_report *x = (t_ioi_report *)newobject(ioi_report_class);
    x->outlet = listout(x);
    dsp_setup((t_pxobject *)x,1);
    
    x->min_ioi_time = 50;
    x->max_ioi_time = 100000;
    x->max_num_iois = MAX_LISTLEN;
    
    for (i=0; i<MAX_LISTLEN; ++i) {
		x->outputList[i].a_type = A_FLOAT;
    }
    
    ioi_report_reset(x);
    
    return (x);
}

void ioi_report_reset(t_ioi_report *x) {
	int i;
	x->now = 0;
	
	for (i=0; i<RINGBUF_SIZE; ++i) {
		x->ringBuf[i].when = 0;
	}
	
	x->ringBufWritePos = 0;

}

void ioi_report_free(t_ioi_report *x) {
	dsp_free(&(x->x_obj));
}

void ioi_report_dsp(t_ioi_report *x, t_signal **sp, short *count) {
	/* x->T = 1.0f / sys_getsr(); */

	dsp_add(ioi_report_perform, 3, sp[0]->s_vec, sp[0]->s_n, x);
}


t_int *ioi_report_perform(t_int *w) {
    t_float *in = (t_float *)(w[1]);
    int size = w[2];
	t_ioi_report *x = (t_ioi_report *)(w[3]);
	int i, eventTime;

	for (i=0; i<size; ++i) {
		if (in[i] != 0.0) {
			eventTime = x->now+i;
			OutputTimes(x, eventTime, x->ringBufWritePos);
			x->ringBuf[(x->ringBufWritePos)++].when = eventTime;
			if (x->ringBufWritePos >= RINGBUF_SIZE) {
				x->ringBufWritePos = 0;
			}
		}
	}
	
	x->now += size;
		
    return (w+4);
}

void *OutputTimes(t_ioi_report *x, int eventTime, int bufPos) {
	/* Perhaps this should be called by a Qelem. */
	int i, b, difference, oldTime;
	
	b = bufPos-1;		/* Position in ring buffer */
	if (b<0) b=RINGBUF_SIZE-1;
	i = 0;				/* Position in output argument list */
	
	while (i<x->max_num_iois) {
		oldTime = x->ringBuf[b--].when;
		if (b<0) b=RINGBUF_SIZE-1;
				
		if (oldTime == 0) break;
		
		difference = eventTime - oldTime;

		if (difference > x->max_ioi_time) {
			break;
		}

		if (difference >= x->min_ioi_time) {
			x->outputList[i++].a_w.w_float = difference;
		}
	}
	
	outlet_list(x->outlet, 0L, i, x->outputList);
}


void ioi_report_min_ioi_time(t_ioi_report *x, long min) {
	x->min_ioi_time = min;
}

void ioi_report_max_ioi_time(t_ioi_report *x, long max) {
	x->max_ioi_time = max;
}


void ioi_report_max_iois_per_event(t_ioi_report *x, long max) {
	if (max > MAX_LISTLEN) {
		error("ioi-report~: max_per_event: You exceeded MAX_LISTLEN of %ld", MAX_LISTLEN);
	} else {
		x->max_num_iois = max;
	}
}




/*
int secondsToSamples (float secs) {
	return secs * sys_getsr();
}
*/
