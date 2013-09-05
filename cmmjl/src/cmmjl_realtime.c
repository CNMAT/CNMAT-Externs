/*
 *  cmmjl_realtime.c
 *  sig.realtime~
 *
 *  Created by Andrew Schmeder on 4/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>

#include "ext.h"
#include "z_dsp.h"

#include "cmmjl_realtime.h"
#include "cmmjl_osc_timetag.h"
#include "cmmjl_error.h"

#ifndef NAME
#define NAME "cmmjl_realtime_clock"
#endif

t_symbol* _sym_cmmjl_realtime_clock_master;

t_cmmjl_realtime_clock* cmmjl_realtime_clock_get_master() {

    if(! _sym_cmmjl_realtime_clock_master) {
        _sym_cmmjl_realtime_clock_master = gensym("_cmmjl_realtime_clock_master");
    }
    
    if(! _sym_cmmjl_realtime_clock_master->s_thing) {
        _sym_cmmjl_realtime_clock_master->s_thing = (void*)malloc(sizeof(t_cmmjl_realtime_clock));
    }
    
    return (t_cmmjl_realtime_clock*)(_sym_cmmjl_realtime_clock_master->s_thing);
    
}

void cmmjl_realtime_clock_init() {
    
    t_cmmjl_realtime_clock* x = cmmjl_realtime_clock_get_master();
    
    x->dt0 = 0.;
    x->n = 0;
    
    x->running = -1;
    x->trigger = NULL;
    
}

void cmmjl_realtime_clock_register(void* trigger) {

    t_cmmjl_realtime_clock* x = cmmjl_realtime_clock_get_master();

    // only set the first one.
    
    // running or never run
    if(x->running != 0) {
        // not running
        x->running = 0;

        t_dspchain *c = dspchain_get();
        if(c) {
            if(c->c_patcher != 0) {
                PERROR("attempting to use realtime object as master clock in a poly~, bad idea!");
            }
        }
        
        // this pointer will trigger tick
        x->trigger = trigger;

        // dt0 needs to be recalculated, n resets to zero
        x->dt0 = 0.;
        x->n = 0;
    }
    
}

void cmmjl_realtime_clock_set_fc(double fc) {
    
    t_cmmjl_realtime_clock *x = cmmjl_realtime_clock_get_master();
    
    x->fc = fc;
    x->w = 2. * 3.1415926535897931 * x->fc;
    x->b = sqrt(2.) * x->w;
    x->c = x->w * x->w;
    
}

void cmmjl_realtime_clock_tick(void *trigger) {
    
    struct ntptime now;
    //struct ntptime t1_neg;
    struct ntptime t1_next;
    struct ntptime dt1;
    //struct ntptime loop_err_ntp;
    
    double loop_err;

    t_cmmjl_realtime_clock *x = cmmjl_realtime_clock_get_master();

    if(x->running == 0) {
        if(trigger != x->trigger) {
            PERROR("clock is not running but trigger is not first");
        } else {
            x->running = 1;
            x->dt0 = (sys_getblksize() * (1./sys_getsr()));
        }
    }
    
    // tick only applies for the first caller 
    if(trigger != x->trigger) {
        return;
    }

    if(x->running != 1) {
        PERROR("clock should be running now");
    }
        
    // whats the time now
    cmmjl_osc_timetag_now_to_ntp(&now);
    
    // reinitialize if sample rate changes or n == 0.
    if(x->n == 0) {
        
        x->n = 0;
        
        cmmjl_realtime_clock_set_fc(0.1);
        
        // expected time per frame
        x->dt = x->dt0;
        
        // convert to ntp
        cmmjl_osc_timetag_float_to_ntp(x->dt, &dt1);
        
        // now -> t0
        cmmjl_osc_timetag_cpy(&(x->t0), &now); 
        
        // time we expect next frame to occur
        cmmjl_osc_timetag_add(&(x->t0), &dt1, &(x->t1));
        
    }
    
    loop_err = cmmjl_osc_timetag_distance_to_float(&now, &(x->t1));

    /* alternate code to calculate loop_err
    // flip sign for subtraction
    cmmjl_osc_timetag_cpy(&t1_neg, &(x->t1));
    t1_neg.sign = -1;
    
    // calculate loop error
    cmmjl_osc_timetag_add(&now, &t1_neg, &loop_err_ntp);
    loop_err = cmmjl_osc_timetag_ntp_to_float(&loop_err_ntp);
    */
    
    // update loop
    // t1 -> t0
    cmmjl_osc_timetag_cpy(&(x->t0), &(x->t1));
    
    // estimate t1
    cmmjl_osc_timetag_float_to_ntp(loop_err * x->b + x->dt, &dt1);
    cmmjl_osc_timetag_add(&dt1, &(x->t1), &t1_next);
    cmmjl_osc_timetag_cpy(&(x->t1), &t1_next);

    // update estimated frame delta time
    x->dt += x->c * loop_err;
    x->dt_error = loop_err;
    x->n++;
    
    // if the error is this big, then something bad has happened
    // such as a buffer-underrun
    // or the system clock was adjusted by a lot
    if(fabs(loop_err) > (2.*x->dt)) {
        if(fabs(loop_err) > (5.*x->dt)) {
            // hard reset to initial conditions
            x->dt = x->dt0;
            cmmjl_osc_timetag_float_to_ntp(x->dt, &dt1);
            cmmjl_osc_timetag_cpy(&(x->t0), &now); 
            cmmjl_osc_timetag_add(&(x->t0), &dt1, &(x->t1));
            x->n = 0;
            cmmjl_realtime_clock_set_fc(0.01);
        } else {
            // soft reset by backing off the filter for a while
            x->n = 100;
            cmmjl_realtime_clock_set_fc(0.01);
        }
    }
    
    // adjust filter cutoff dynamically
    if(x->n == 100) {
        cmmjl_realtime_clock_set_fc(0.01);
    }
    if(x->n == 1000) {
        cmmjl_realtime_clock_set_fc(0.001);
    }
    if(x->n == 10000) {
        cmmjl_realtime_clock_set_fc(0.0001);
    }
    
}

// when is now
void cmmjl_realtime_clock_now(struct ntptime *t) {
    
    t_cmmjl_realtime_clock *x = cmmjl_realtime_clock_get_master();

    cmmjl_osc_timetag_cpy(t, &(x->t0));
    
}

// when is next
void cmmjl_realtime_clock_next(struct ntptime *t) {

    t_cmmjl_realtime_clock *x = cmmjl_realtime_clock_get_master();
    
    cmmjl_osc_timetag_cpy(t, &(x->t1));
    
}

// how long until next in seconds
double cmmjl_realtime_clock_dt() {

    t_cmmjl_realtime_clock *x = cmmjl_realtime_clock_get_master();
    
    return x->dt;
    
}

