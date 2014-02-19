/** @file cmmjl_realtime.h
 Real-time support for signal processing
 @author Andy Schmeder
 */
/*
 Copyright (c) 2009.  The Regents of the University of California (Regents).
 All Rights Reserved.
 
 Permission to use, copy, modify, and distribute this software and its
 documentation for educational, research, and not-for-profit purposes, without
 fee and without a signed licensing agreement, is hereby granted, provided that
 the above copyright notice, this paragraph and the following two paragraphs
 appear in all copies, modifications, and distributions.  Contact The Office of
 Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
 CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.
 
 Written by Andy Schmeder, The Center for New Music and
 Audio Technologies, University of California, Berkeley.
 
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
 */

#ifndef __CMMJL_REALTIME_H__
#define __CMMJL_REALTIME_H__

#include "cmmjl_osc_timetag.h"

typedef struct _cmmjl_realtime_clock {
    
    double dt0;         // a priori expected time between ticks
    double dt;          // expected time between ticks
    
    int n;              // number of frame ticks
    double fc;          // filter cutoff (0.0-1.0), controls how aggressively clock filters jitter
    double w;           // w = 2 PI lp
    double b;           // sqrt(2) * w
    double c;           // w^2
    
    double dt_error;    // jitter of clock on last tick
    
    ntptime t0;
    ntptime t1;
    
    int running;
    void* trigger;
    
} t_cmmjl_realtime_clock;

t_cmmjl_realtime_clock* cmmjl_realtime_clock_get_master();

// initialize clock with a dt0
void cmmjl_realtime_clock_init();

// set the filter cutoff, not user-adjustable
void cmmjl_realtime_clock_set_fc(double fc0);

// update the clock
void cmmjl_realtime_clock_tick(void *x);

// when is now
void cmmjl_realtime_clock_now(ntptime *t);

// when is next
void cmmjl_realtime_clock_next(ntptime *t);

// how long until next in seconds
double cmmjl_realtime_clock_dt();

// register interest in the clock
void cmmjl_realtime_clock_register();

#endif
