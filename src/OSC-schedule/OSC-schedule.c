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
 DESCRIPTION: Schedules packets using OSC time stamps
 AUTHORS: Andy Schmeder
 COPYRIGHT_YEARS: 2008
 SVN_REVISION: $LastChangedRevision: 1634 $
 VERSION 0.1: First public release
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

// max object header
#include "ext.h"
#include "ext_critical.h"

// heap-based priority queue
#include "pqops.h"

// timetag operations
#include "../OSC-timetag/OSC-timetag-ops.h"

// version
#include "version.h"
#include "version.c"

// default options
#define DEFAULT_PACKET_SIZE 1000
#define DEFAULT_QUEUE_SIZE 1500

/* structure definition of your object */
typedef struct _OSCSchedule
{
    Object o_ob; // required header
    
    // inlet
    t_object* in_p[1]; // either a normal inlet or a crazy proxy thing
    long  in_i_unsafe; // non-threadsafe proxy thing
    
    // outlet
    t_object* out_p[3];
    
    // clock
    t_object* clock;
    
    // lock and re-entrance check
    t_critical lock;
    int soft_lock;
    int entrance_count;
    
    // ready? 
    int ready;
    
    // scheduler precision
    struct ntptime precision;
    
    // maximum delay permitted
    struct ntptime max_delay;
    
    // binary heap
    binary_heap q;
    
    int         packets_max;
    int*        packet_free;
    char*       packet_data;
    int         packet_size;
    
    unsigned int id;
    
} OSCSchedule;

/* global that holds the class definition */
void *OSCSchedule_class;

Symbol *ps_FullPacket;

// basic prototypes
void* OSCSchedule_new(Symbol* s, short argc, Atom* argv);
void OSCSchedule_free(OSCSchedule *x);
void OSCSchedule_assist (OSCSchedule *x, void *box, long msg, long arg, char *dstString);

// op callback
void OSCSchedule_tick(OSCSchedule *x);

// methods
void OSCSchedule_reset(OSCSchedule* x); // clear queue
void OSCSchedule_FullPacket(OSCSchedule *x, Symbol *s, int argc, Atom* argv);

// setup
void main(fptr *f)
{
    
    // announce copyright
    version(0);
    
    // setup
    setup((t_messlist **)&OSCSchedule_class, (method)OSCSchedule_new, (method)OSCSchedule_free, (short)sizeof(OSCSchedule), 0L, A_GIMME, 0);
    
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
    
    x->packets_max = DEFAULT_QUEUE_SIZE;
    x->packet_size = DEFAULT_PACKET_SIZE;
    
    OSCTimeTag_float_to_ntp(0.003, &(x->precision));
    OSCTimeTag_float_to_ntp(1000.0, &(x->max_delay));
    
    x->ready = 0;
    x->entrance_count = 0;
    
    x->id = 0;
    
    x->clock = clock_new(x, OSCSchedule_tick);
    critical_new(&x->lock);
    
    for(i = 0; i < argc; i++) {
        
        if(argv[i].a_type == A_SYM) {
            // @precision
            if(strcmp(argv[i].a_w.w_sym->s_name, "@precision") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_FLOAT) {
                        OSCTimeTag_float_to_ntp(argv[i].a_w.w_float, &(x->precision));
                    } else {
                        post("OSC-schedule: expected float for precision");
                    }
                } else {
                    post("OSC-schedule: missing arg after precision");
                }
            }
            
            // @maxdelay
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@maxdelay") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_FLOAT) {
                        OSCTimeTag_float_to_ntp(argv[i].a_w.w_float, &(x->max_delay));
                    } else {
                        post("OSC-schedule: expected float for maxdelay");
                    }
                } else {
                    post("OSC-schedule: missing arg after maxdelay");
                }
            }
            
            // @queuesize
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@queuesize") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_LONG) {
                        x->packets_max = argv[i].a_w.w_long;
                    } else {
                        post("OSC-schedule: expected int for queuesize");
                    }
                } else {
                    post("OSC-schedule: missing arg after queuesize");
                }
            }
            
            // look for @packetsize
            else if(strcmp(argv[i].a_w.w_sym->s_name, "@packetsize") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_LONG) {
                        x->packet_size = argv[i].a_w.w_long;
                    } else {
                        post("OSC-schedule: expected int for packetsize");
                    }
                } else {
                    post("OSC-schedule: missing arg after packetsize");
                }
            }
        }
    }
    
    x->out_p[2] = outlet_new(x, "FullPacket");
    x->out_p[1] = outlet_new(x, "FullPacket");
    x->out_p[0] = outlet_new(x, "FullPacket");
    
    // allocate packet data buffer
    x->packet_data = (char*)malloc(x->packets_max * x->packet_size);
    
    // allocate nodes
    heap_initialize(&(x->q), x->packets_max);
    
    // allocate free markers
    x->packet_free = (int*)malloc(sizeof(int) * x->packets_max);
    for(i = 0; i < x->packets_max; i++) {
        x->packet_free[i] = 1;
    }
    
    return (x);
    
}

void OSCSchedule_free(OSCSchedule* x) {
    
    clock_unset(x->clock);
    
    freeobject(x->clock);
    
    critical_free(x->lock);
    
    free(x->packet_data);
    
    free(x->packet_free);
    
    heap_finalize(&(x->q));
    
}

// tooltip assist

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void OSCSchedule_assist(OSCSchedule *x, void *box, long msg, long arg, char *dstString) {
    
    if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "FullPacket in to schedule, or message 'reset'");
        }
    } else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "FullPacket out, rescheduled");
        }
        if(arg == 1) {
            sprintf(dstString, "FullPacket out, which has missed the scheduling deadline, or if queue is full, or packet exceeds maximum size");
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
    
    // wait for soft lock to open
    critical_enter(x->lock);
    while(! x->soft_lock) {
        critical_exit(x->lock);
        critical_enter(x->lock);
    }
    
    x->soft_lock = 1;
    
    clock_unset(x->clock);
    
    // clear queue
    while(heap_max(&(x->q)) != NULL) {
        heap_extract_max(&(x->q));
    }
    
    // clear soft lock
    x->soft_lock = 0;
    
    critical_exit(x->lock);
    
}

void OSCSchedule_FullPacket(OSCSchedule *x, Symbol *s, int argc, Atom* argv) {
    
    struct ntptime now;
    struct ntptime nowp1;
    
    char* p_data;
    int i;
    node n;
    float dt;
    
    node* p_n;
    
    //x->entrance_count++;
    
    //if(x->entrance_count > 1) {
    //  post("re-entrance count: %d", x->entrance_count);
    //}
    
    if(argc == 2 && argv[0].a_type == A_LONG && argv[0].a_w.w_long >= 16 && argv[1].a_type == A_LONG && argv[1].a_w.w_long != 0) {
        
        n.length = (char*)(argv[0].a_w.w_long);
        p_data = (char*)(argv[1].a_w.w_long);
        
        // check for queue full condition
        if(x->q.heap_size == x->packets_max) {
            post("OSC-schedule: queue overflow");
            outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
            return;
        }
        
        // check for length condition
        if(n.length >= x->packet_size) {
            post("OSC-schedule: packet length %d exceeds maximum", n.length);
            outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
            return;
        }
        
        // make sure its a bundle
        if(strcmp(p_data, "#bundle") != 0) {
            // not a bundle, send it out the 2nd outlet
            post("OSC-schedule: input is not a bundle");
            outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
            return;
        }
        
        n.timestamp.sec = ntohl(*((unsigned long *)(p_data+8)));
        n.timestamp.frac_sec = ntohl(*((unsigned long *)(p_data+12)));
        n.timestamp.sign = 1;
        
        // immediate goes out the third outlet 
        if(n.timestamp.sec == 0 && n.timestamp.frac_sec == 1) {
            //post("OSC-schedule: immediate recieved");
            outlet_anything(x->out_p[2], ps_FullPacket, 2, argv);
            return;
        } else {
            n.timestamp.type = TIME_STAMP;
        }
        
        // get now
        OSCTimeTag_now_to_ntp(&now);
        
        // now plus scheduler precision interval -> x_nowp1
        OSCTimeTag_add(&now, &(x->precision), &nowp1);
        
        // compare
        switch(OSCTimeTag_cmp(&nowp1, &(n.timestamp))) {
          case 0: // output is on time
              //post("OSC-schedule: output on time exactly");
              outlet_anything(x->out_p[0], ps_FullPacket, 2, argv);
              return;
              
          case 1: // deadline miss or on-time
              switch(OSCTimeTag_cmp(&now, &(n.timestamp))) {
                case -1: // within scheduler boundary, output on time
                case 0:
                    //post("OSC-schedule: output on time without rescheduling");
                    outlet_anything(x->out_p[0], ps_FullPacket, 2, argv);
                    return;
                    
                case 1: // deadline missed
                    //post("OSC-schedule: input preceeds now");
                    outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
                    return;
              }
        }
        
        // message is candidate for future scheduling...
        
        // check that delay is less than max_delay...
        
        OSCTimeTag_add(&now, &(x->max_delay), &nowp1);
        
        // delay exceeds maximum
        if(OSCTimeTag_cmp(&nowp1, &(n.timestamp)) < 0) {
            post("OSC-schedule: delay exceeds maximum");
            outlet_anything(x->out_p[1], ps_FullPacket, 2, argv);
            return;
        }
        
        // lock
        critical_enter(x->lock);
        
        while(x->soft_lock == 1) {  // spin until soft_lock is released
            critical_exit(x->lock);
            critical_enter(x->lock);
        }
        
        // claim soft lock
        x->soft_lock = 1;
        
        // reset clock timeout
        clock_unset(x->clock);
        
        // transform timestamp to priority (reverse sorted!)
        // n.p = 0xffffffffffffffffULL - (((unsigned long long int)(n.timestamp.sec)) << 32 + ((unsigned long long int)(n.timestamp.frac_sec)));
        
        while(x->packet_free[x->id] != 1) {
            x->id = ((x->id + 1) % x->packets_max);
        }
        n.id = x->id;
        x->id++;
        
        // mark slot as busy
        x->packet_free[n.id] = 0;
        
        // add to queue
        i = heap_insert(&(x->q), n);
        
        // copy data...
        // x->q.elements[i].data = x->packet_data + (x->packet_size * n.id);
        memcpy(x->packet_data + (x->packet_size * n.id), p_data, n.length);
        
        // check for new scheduling target delay
        
        p_n = heap_max(&(x->q));
        
        OSCTimeTag_now_to_ntp(&now);
        now.sign = -1;
        
        OSCTimeTag_add(&(p_n->timestamp), &now, &nowp1);
        dt = OSCTimeTag_ntp_to_float(&nowp1);
        
        critical_exit(x->lock);
        
        //post("OSC-schedule: target delay %f msec", dt * 1000.0);
        
        if(dt < 0.001) {
            x->soft_lock = 0;
            OSCSchedule_tick(x);
        } else {
            clock_fdelay(x->clock, dt * 1000.);
            x->soft_lock = 0;
        }
        
    }
    
    //x->entrance_count--;
    
}

void OSCSchedule_tick(OSCSchedule *x) {
    
    Atom fp[2];
    struct ntptime now;
    struct ntptime nowp1;
    
    double dt;
    
    node n;
    node_ptr p_n;
    
    int t;
    
    critical_enter(x->lock);
    
    while(x->soft_lock == 1) {
        critical_exit(x->lock);
        critical_enter(x->lock);
    }
    
    clock_unset(x->clock);
    
    x->soft_lock = 1;
    
    p_n = heap_max(&(x->q));
    
    OSCTimeTag_now_to_ntp(&now);
    OSCTimeTag_add(&now, &(x->precision), &nowp1);
    
    while(p_n != NULL) {
        
        t = OSCTimeTag_cmp(&nowp1, &((*p_n).timestamp));
        
        if(t == 1 || t == 0) {
            // now actually dequeue it...
            n = heap_extract_max(&(x->q));
            
            // dequeue and pass on the next scheduled packet
            SETLONG(&(fp[0]), n.length);
            SETLONG(&(fp[1]), (unsigned long int)((x->packet_data + (x->packet_size * n.id))));
            
            x->packet_free[n.id] = 1;
            x->id = n.id; // this isn't necessary but should keep the cache footprint smaller
            
            // soft lock is NOT released...
            critical_exit(x->lock);
            outlet_anything(x->out_p[0], ps_FullPacket, 2, fp); // outlet can't be in critical section
            critical_enter(x->lock);
            
        } else {
            break;
        }
        
        // get next node
        p_n = heap_max(&(x->q));
        
    }
    
    critical_exit(x->lock);
    
    if(p_n != NULL) {
        
        OSCTimeTag_now_to_ntp(&now);
        now.sign = -1;
        
        OSCTimeTag_add(&(p_n->timestamp), &now, &nowp1);
        dt = OSCTimeTag_ntp_to_float(&nowp1);
        
        //post("tick delay: %f msec", dt * 1000.);
        
        if(dt < 0.001) {
            x->soft_lock = 0;
            OSCSchedule_tick(x); // re-enter on self... shouldn't happen too often, hopefully.
        } else {
            x->soft_lock = 0;
            clock_fdelay(x->clock, dt * 1000.);
        }
        
    } else {
        x->soft_lock = 0;
    }
    
    
}

