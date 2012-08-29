/*

Copyright (c) 2008
The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Andy Schmeder, The Center for New Music and Audio Technologies,
University of California, Berkeley.

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

 OSC-schedule.c
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: OSC-schedule
DESCRIPTION: Performes scheduling for jitter compensation on an OSC stream
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2008
DRUPAL_NODE: /patch/....
SVN_REVISION: $LastChangedRevision: 1634 $
VERSION 0.1: Initial version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

// max object header
#include "ext.h"
#include "ext_critical.h"

// heap-based priority queue
#include "pqops.h"

// timetag operations
#include "OSC-timetag-ops.h"

// version
#include "version.h"
#include "version.c"

#define MAX_PACKET_SIZE 256
#define MAX_PACKETS 64

typedef struct _fullpacket
{
    int length;
    struct ntptime timetag;
    char data[MAX_PACKET_SIZE];
} fullpacket;

/* structure definition of your object */
typedef struct _OSCSchedule
{
	Object o_ob;    // required header
    
    t_object* in_p[1];  // either a normal inlet or a crazy proxy thing
    long  in_i_unsafe;     // which inlet message arrived on, but isn't threadsafe

    t_object* out_p[3]; // outlet
    
    t_object* clock;
    t_critical lock;
    int soft_lock;
    
    int entrance_count;
    
    int ready;
    
    struct ntptime epoch_remote;  // minimum observed time in remote stream
    struct ntptime epoch_local;   // corresponding time on local clock

    double drift;
    double delay;
    
    int next_scheduled_i;
    
    fullpacket  packets[MAX_PACKETS];
    int         packets_n;
    int         packets_i;
    
} OSCSchedule;

/* globalthat holds the class definition */
void *OSCSchedule_class;

Symbol *ps_FullPacket;

// basic prototypes
void* OSCSchedule_new(Symbol* s, short argc, Atom* argv);
void OSCSchedule_free(OSCSchedule *x);
void OSCSchedule_assist (OSCSchedule *x, void *box, long msg, long arg, char *dstString);

// op callback
void OSCSchedule_tick(OSCSchedule *x);

// methods
void OSCSchedule_drift(OSCSchedule* x, double d);
void OSCSchedule_delay(OSCSchedule* x, double d);
void OSCSchedule_reset(OSCSchedule* x);
void OSCSchedule_FullPacket(OSCSchedule *x, Symbol *s, int argc, Atom* argv);

// conversions
void OSCTimeTag_float_to_ntp(double d, struct ntptime* n);
double OSCTimeTag_ntp_to_float(struct ntptime* n);
void OSCTimeTag_now_to_ntp(struct ntptime* n);
int OSCTimeTag_cmp(struct ntptime* a, struct ntptime* b);
double OSCTimeTag_diff(struct ntptime* a, struct ntptime* b);
void OSCTimeTag_add(struct ntptime* a, struct ntptime* b, struct ntptime* c);
void OSCTimeTag_add_f(struct ntptime* a, double d, struct ntptime* c);

// setup
void main(fptr *f)
{

    // fix later
    // version(0);
    
	setup((t_messlist **)&OSCSchedule_class, (method)OSCSchedule_new, (method)OSCSchedule_free, (short)sizeof(OSCSchedule), 0L, A_GIMME, 0);

    // set drift
    addmess((method)OSCSchedule_drift, "drift", A_FLOAT, 0);
    
    // set delay
    addmess((method)OSCSchedule_delay, "delay", A_FLOAT, 0);
    
    // reset
    addmess((method)OSCSchedule_reset, "reset", 0);
    
    // FullPacket
    addmess((method)OSCSchedule_FullPacket, "FullPacket", A_GIMME, 0);
    
    // tooltip helper
	addmess((method)OSCSchedule_assist, "assist", A_CANT, 0);
    
    ps_FullPacket = gensym("FullPacket");

}

void* OSCSchedule_new(Symbol* s, short argc, Atom *argv)
{
	OSCSchedule *x;	
    int i;
    
	x = newobject(OSCSchedule_class);
    
    x->drift = 1.0;
    x->delay = 0.01; // in seconds
    x->ready = 0;
    x->entrance_count = 0;
    
    x->clock = clock_new(x, OSCSchedule_tick);
    critical_new(&x->lock);
    
    OSCSchedule_reset(x);
    
    for(i = 0; i < argc; i++) {

        if(argv[i].a_type == A_SYM) {
            // look for @drift
            if(strcmp(argv[i].a_w.w_sym->s_name, "@drift") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_FLOAT) {
                        OSCSchedule_drift(x, argv[i].a_w.w_float);
                    } else {
                        post("OSC-schedule: expected float for drift");
                    }
                } else {
                    post("OSC-schedule: missing arg after drift");
                }
            }
            
            // look for @delay
            if(strcmp(argv[i].a_w.w_sym->s_name, "@delay") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_FLOAT) {
                        OSCSchedule_delay(x, argv[i].a_w.w_float);
                    } else {
                        post("OSC-schedule: expected float for delay");
                    }
                } else {
                    post("OSC-schedule: missing arg after delay");
                }
            }
        }
    }
    
    x->out_p[2] = outlet_new(x, "FullPacket");
    x->out_p[1] = outlet_new(x, "FullPacket");
    x->out_p[0] = outlet_new(x, "FullPacket");
        
	return (x);
}

void OSCSchedule_free(OSCSchedule* x) {

    clock_unset(x->clock);
    freeobject(x->clock);
    
    critical_free(x->lock);
    
}

// tooltip assist

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void OSCSchedule_assist (OSCSchedule *x, void *box, long msg, long arg, char *dstString) {
	if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "FullPacket in, or message 'drift', 'delay' or 'reset'");
        }
	} else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "FullPacket out, rescheduled");
        }
        if(arg == 1) {
            sprintf(dstString, "FullPacket out, which has missed the scheduling deadline");
        }
        if(arg == 2) {
            sprintf(dstString, "FullPacket out, having immediate timetag");
        }
	} else {
		post("OSCTimeTag_assist: unrecognized message %ld", msg);
	}
}

// methods

void OSCSchedule_reset(OSCSchedule *x) {
    
    int i;
    
    critical_enter(x->lock);
    
    x->ready = 0;
    
    clock_unset(x->clock);
    
    // reset epoch
    x->epoch_remote.sec = 0;
    x->epoch_remote.frac_sec = 0;
    
    x->epoch_local.sec = 0;
    x->epoch_local.frac_sec = 0;

    x->next_scheduled_i = -1;
    
    x->packets_n = 0;
    x->packets_i = 0;
    
    for(i = 0; i < MAX_PACKETS; i++) {
        x->packets[i].length = 0;
    }
    
    x->soft_lock = 0;
    
    critical_exit(x->lock);
    
}

void OSCSchedule_drift(OSCSchedule *x, double d) {
    
    x->drift = d;
    
}

void OSCSchedule_delay(OSCSchedule *x, double d) {
    
    x->delay = d;
    
}

void OSCSchedule_FullPacket(OSCSchedule *x, Symbol *s, int argc, Atom* argv) {

    struct ntptime x_star;
    struct ntptime x_now;
    
    unsigned long sec;
    unsigned long frac_sec;
    
    double d_sec;
    double d_frac_sec;
    double dt;
    
    unsigned long length;
    
    char* p_data;
    char data[256];
    
    int i;
    
    x->entrance_count++;
    if(x->entrance_count > 1) {
        post("re-entrance count: %d", x->entrance_count);
    }
    
    if(argc == 2 && argv[0].a_type == A_LONG && argv[0].a_w.w_long >= 16 && argv[1].a_type == A_LONG && argv[1].a_w.w_long != 0) {
        
        critical_enter(0);
        
        length = (char*)argv[0].a_w.w_long;
        p_data = (char*)argv[1].a_w.w_long;

        // i don't trust this thing... lets make a local copy of it now!
        memcpy(data, p_data, length);
        
        critical_exit(0);
        
        if(strcmp(data, "#bundle") == 0) {
            
            sec =      ntohl(*((unsigned long *)(data+8)));
            frac_sec = ntohl(*((unsigned long *)(data+12)));
            
        }

        // immediate goes out the third outlet 
        if(sec == 0 && frac_sec == 1) {
            outlet_anything(x->out_p[2], ps_FullPacket, 2, argv);
        }
        
        critical_enter(x->lock);
        
        // we don't have an epoch recorded yet... assume this is it.
        if(! x->ready) {
            
            x->ready = 1;
            
            // record local epoch
            OSCTimeTag_now_to_ntp(&(x->epoch_local));
            
            // record remote epoch
            x->epoch_remote.sec = sec;
            x->epoch_remote.frac_sec = frac_sec;
            
        }
        
        critical_exit(x->lock);
        
        // convert to float, relative to remote epoch
        
        d_sec = (double)sec - (double)x->epoch_remote.sec;
        d_frac_sec = (((double)frac_sec) / 4294967295.0) - (((double)x->epoch_remote.frac_sec) / 4294967295.0);

        // apply drift correction
        dt = (d_sec + d_frac_sec) / x->drift;
        
        // now add the local epoch
        // result is the ntp time on the local system when event happened...
        x_star.sec = (unsigned long int)(dt - fmod(dt, 1.0));
        x_star.frac_sec = (unsigned long int)(fmod(dt, 1.0) * 4294967295.0);
        OSCTimeTag_add(&x_star, &(x->epoch_local), &x_star);
        
        // distance between now and the event time
        OSCTimeTag_now_to_ntp(&x_now);
        dt = OSCTimeTag_diff(&x_now, &x_star);
        
        //post("corrected to now dt: %f", dt);
        
        // write the new timetag back into the bundle timetag...
        *((unsigned long *)(data+8)) = htonl(x_star.sec);
        *((unsigned long *)(data+12)) = htonl(x_star.frac_sec);

        if((dt + x->delay) < 0.0) {
            // deadline missed
            post("deadline missed");
            outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
        } else if(x->packets_n == MAX_PACKETS) {
            post("OSC-schedule: packet queue full");
        } else if(length > MAX_PACKET_SIZE) {
            post("OSC-schedule: packet exceeds maximum size");
        } else {
            // deadline OK, packet ready to queue

            critical_enter(x->lock);
            
            while(x->soft_lock == 1) {  // spin until soft_lock is released
                critical_exit(x->lock);
                critical_enter(x->lock);
            }
            
            // reset clock timeout
            clock_unset(x->clock);
            
            // copy data in next available slot
            x->packets_n++;
            
            // this slot must be always free
            x->packets[x->packets_i].length = length;
            memcpy(x->packets[x->packets_i].data, data, length);

            // when to schedule this one
            OSCTimeTag_add_f(&x_star, (x->delay), &(x->packets[x->packets_i].timetag));
            
            // compare with next_scheduled to determine who is earlier
            if(x->next_scheduled_i == -1 || OSCTimeTag_cmp(&(x->packets[x->next_scheduled_i].timetag), &(x->packets[x->packets_i].timetag)) == 1) {
                x->next_scheduled_i = x->packets_i;
            }
            
            // search for next available slot
            if(x->packets_n < MAX_PACKETS) {
                i = 0;
                while(i < MAX_PACKETS) {
                    x->packets_i = (x->packets_i + 1) % MAX_PACKETS;
                    i++;
                    
                    if(x->packets[x->packets_i].length == 0) {
                        break; // free packet found
                    }
                }
            } else {
                x->packets_i = -1;
            }
            
            // set clock fdelay for now - next scheduled in msec
            OSCTimeTag_now_to_ntp(&x_now);
            dt = OSCTimeTag_diff(&x_now, &(x->packets[x->next_scheduled_i].timetag)) * -1000.0;

            post("delay in msec: %f", dt);
            
            critical_exit(x->lock);
            
            if(dt > 0.1) {
                clock_fdelay(x->clock, dt);
            } else {
                // so close, just dump it out now...
                OSCSchedule_tick(x);
            }
            
        }
        
    }

    x->entrance_count--;
    
}


void OSCSchedule_tick(OSCSchedule *x) {

    Atom fp[2];
    struct ntptime x_now;
    int i, j;
    double dt;

    post("tick");
    
    critical_enter(x->lock);
    
    if(x->next_scheduled_i != -1) {
        
        clock_unset(x->clock);
        
        i = x->next_scheduled_i;
        x->next_scheduled_i = -1;
        
        OSCTimeTag_now_to_ntp(&x_now);

        // verify that this packet is actually ready to go...
        if(OSCTimeTag_cmp(&x_now, &(x->packets[i].timetag)) >= 0) {
            
            // dequeue and pass on the next scheduled packet
            SETLONG(&(fp[0]), x->packets[i].length);
            SETLONG(&(fp[1]), (unsigned long int)((x->packets[i].data)));

            x->soft_lock = 1;
            critical_exit(x->lock);
            
            outlet_anything(x->out_p[0], ps_FullPacket, 2, fp); // outlet can't be in critical section
            
            critical_enter(x->lock);
            x->soft_lock = 0;
            
            // free up this slot
            x->packets[i].length = 0;
            // x->packets_i = i;
            x->packets_n--;

            // find the next scheduled
            for(i = 0; i < MAX_PACKETS; i++) {
                if(x->packets[i].length != 0) {
                    if(x->next_scheduled_i == -1 || OSCTimeTag_cmp(&(x->packets[x->next_scheduled_i].timetag), &(x->packets[i].timetag)) == 1) {
                        x->next_scheduled_i = i;
                    }
                }
            }
        } else {
            post("tick called but next scheduled is not ready yet");
            x->next_scheduled_i = i;
        }
            
        if(x->next_scheduled_i != -1) {
            OSCTimeTag_now_to_ntp(&x_now);
            dt = OSCTimeTag_diff(&x_now, &(x->packets[x->next_scheduled_i].timetag)) * -1000.0;
            
            critical_exit(x->lock);
            
            post("tick delay: %f msec", dt);
            
            if(dt > 0.1) {
                clock_fdelay(x->clock, dt);
            } else {
                // just dequeue the next one right now...
                OSCSchedule_tick(x);
            }
        } else {
            // queue is empty... nothing to schedule
            post("packet queue is empty");
            critical_exit(x->lock);
        }
    } else {
        post("tick called but no packets is scheduled");
        critical_exit(x->lock);
    }
    
}


// conversion functions

void OSCTimeTag_float_to_ntp(double d, struct ntptime* n) {

    double sec;
    double frac_sec;
    
    frac_sec = fmod(d, 1.0);
    sec = d - frac_sec;
    
    n->sec = (unsigned long int)(sec);
    n->frac_sec= (unsigned long int)(frac_sec * 4294967295.0);
}

double OSCTimeTag_ntp_to_float(struct ntptime* n) {
    return (double)n->sec + ((double)((unsigned long int)n->frac_sec)) / 4294967295.0;
}

void OSCTimeTag_now_to_ntp(struct ntptime* n) {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    
    n->sec = (unsigned long)2208988800 + 
        (unsigned long) tv.tv_sec - 
        (unsigned long)(60 * tz.tz_minuteswest) +
        (unsigned long)(tz.tz_dsttime == 1 ? 3600 : 0);
    
    n->frac_sec = (unsigned long)(tv.tv_usec * 4295); // 2^32-1 / 1.0e6
}

void OSCTimeTag_add(struct ntptime* a, struct ntptime* b, struct ntptime* c) {
    
    double dt_sec;
    double dt_frac_sec;
    
    dt_sec = (double)a->sec + (double)b->sec;
    dt_frac_sec = ((double)a->frac_sec) / 4294967295.0 + ((double)b->frac_sec) / 4294967295.0;
    
    c->sec = (unsigned long int)dt_sec + (dt_frac_sec - fmod(dt_frac_sec, 1.0));
    c->frac_sec = (unsigned long int)(fmod(dt_frac_sec, 1.0) * 4294967295.0);
    
}

void OSCTimeTag_add_f(struct ntptime* a, double d, struct ntptime* c) {
    
    double dt_sec;
    double dt_frac_sec;
    
    dt_sec = (double)a->sec + (double)(d - fmod(d, 1.0));  // whole part only ...
    dt_frac_sec = (((double)a->frac_sec) / 4294967295.0) + fmod(d, 1.0); // add fractions part
    
    c->sec = (unsigned long int)(dt_sec + (dt_frac_sec - fmod(dt_frac_sec, 1.0))); // add whole overflow of fraction if relevant
    c->frac_sec = (unsigned long int)(fmod(dt_frac_sec, 1.0) * 4294967295.0);
    
}

// distance between 
double OSCTimeTag_diff(struct ntptime* a, struct ntptime* b) {
    
    double dt_sec;
    double dt_frac_sec;
    
    dt_sec = (double)a->sec - (double)b->sec;
    dt_frac_sec = ((double)a->frac_sec) / 4294967295.0 - ((double)b->frac_sec) / 4294967295.0;
    
    return dt_sec + dt_frac_sec;
    
    //return ((double)a->sec + (((double)a->frac_sec) / 4294967295.0)) - ((double)b->sec + (((double)b->frac_sec) / 4294967295.0));
}

// comparison
int OSCTimeTag_cmp(struct ntptime *a, struct ntptime* b) {
    if(a->sec == b->sec) {
        if(a->frac_sec == b->frac_sec) {
            return 0;
        }
        else if(a->frac_sec < b->frac_sec) {
            return -1;
        }
        else {
            return 1;
        }
    }
    else if(a->sec < b->sec) {
        return -1;
    }
    else {
        return 1;
    }
    
}

