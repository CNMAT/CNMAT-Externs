/*
 
Copyright (c) 2009
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

 thread.fork.c
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: thread.fork
DESCRIPTION: Pass data from main thread to a secondary thread
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 1634 $
VERSION 0.1: First public release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

// max object header
#include "ext.h"

// version info
#include "version.h"
#include "version.c"

#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#define TYPE_ANYTHING 0
#define TYPE_BANG 1
#define TYPE_INT 2
#define TYPE_FLOAT 3

/* structure definition of your object */
typedef struct _thread_fork
{
    Object o_ob;    // required header
    
    t_object* in_p[1];  // either a normal inlet or a crazy proxy thing
    long in_i_unsafe;   // which inlet message arrived on, but isn't threadsafe
  
    t_object* out_p[1]; // outlet
  
    pthread_t t;
    pthread_mutex_t lock;
    int main_has_lock;

    int type;

    float f;
    int i;
    
    Symbol* s;
    int argc;
    Atom* argv;
    
} thread_fork;

/* global that holds the class definition */
void *thread_fork_class;

// basic prototypes
void* thread_fork_new(Symbol* s, short argc, Atom* argv);
void thread_fork_free(thread_fork *x);
void thread_fork_assist (thread_fork *x, void *box, long msg, long arg, char *dstString);

// methods
void thread_fork_anything(thread_fork *x, Symbol* s, int argc, Atom* argv);
void thread_fork_bang(thread_fork *x);
void thread_fork_float(thread_fork *x, double f);
void thread_fork_int(thread_fork *x, int i);

void* thread_fork_run();

// setup
void main(fptr *f)
{
    // post version
    version(0);
    
    setup((t_messlist **)&thread_fork_class, (method)thread_fork_new, (method)thread_fork_free, (short)sizeof(thread_fork), 0L, A_GIMME, 0);
    
    addmess((method)thread_fork_anything, "anything", A_GIMME, 0);
    addbang((method)thread_fork_bang);
    addfloat((method)thread_fork_float);
    addint((method)thread_fork_int);
    
    // tooltip helper
    addmess((method)thread_fork_assist, "assist", A_CANT, 0);
    
}

void *thread_fork_new(Symbol* s, short argc, Atom *argv)
{
    thread_fork *x;
    
    x = newobject(thread_fork_class);
    x->out_p[0] = outlet_new(x, NULL);

    pthread_create(&(x->t), NULL, thread_fork_run, x);
    pthread_mutex_init(&(x->lock), NULL);
    pthread_mutex_lock(&(x->lock));
    
    return (x);
}

void thread_fork_free(thread_fork* x) {
    
    pthread_cancel(x->t);
    
}

// tooltip assist

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void thread_fork_assist (thread_fork *x, void *box, long msg, long arg, char *dstString) {
    if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "anything, to be passed to other thread");
        }
    } else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "anything, in other thread");
        }
    }
}

void thread_fork_anything(thread_fork *x, Symbol* s, int argc, Atom* argv) {

    x->type = TYPE_ANYTHING;

    x->s = s;
    x->argc = argc;
    x->argv = argv;
    
    pthread_mutex_unlock(&(x->lock));
    sched_yield();
    pthread_mutex_lock(&(x->lock));

}

void thread_fork_bang(thread_fork* x) {
    
    x->type = TYPE_BANG;
    
    pthread_mutex_unlock(&(x->lock));
    sched_yield();
    pthread_mutex_lock(&(x->lock));
    
}

void thread_fork_int(thread_fork* x, int i) {
    
    x->type = TYPE_INT;
    x->i = i;
    
    pthread_mutex_unlock(&(x->lock));
    sched_yield();
    pthread_mutex_lock(&(x->lock));
    
}

void thread_fork_float(thread_fork *x, double f) {
    
    x->type = TYPE_FLOAT;
    x->f = f;

    pthread_mutex_unlock(&(x->lock));
    sched_yield();
    pthread_mutex_lock(&(x->lock));
    
}

void thread_fork_outlet_anything(thread_fork *x) {
    
    Symbol s;
    Atom argv[x->argc];
    int argc;

    argc = x->argc;
    memcpy(&s, x->s, sizeof(Symbol));
    memcpy(argv, x->argv, sizeof(Atom)*x->argc);
    
    pthread_mutex_unlock(&(x->lock));

    outlet_anything(x->out_p[0], &s, argc, argv);
}

void *thread_fork_run(thread_fork *x) {

    int i;
    float f;
    
    while(1) {

        sched_yield();
        pthread_mutex_lock(&(x->lock));

        if(x->type == TYPE_ANYTHING) {
            thread_fork_outlet_anything(x);
        } else if(x->type == TYPE_BANG) {
            pthread_mutex_unlock(&(x->lock));
            outlet_bang(x->out_p[0]);
        } else if(x->type == TYPE_FLOAT) {
            f = x->f;
            pthread_mutex_unlock(&(x->lock));
            outlet_float(x->out_p[0], f);
        } else if(x->type == TYPE_INT) {
            i = x->i;
            pthread_mutex_unlock(&(x->lock));
            outlet_int(x->out_p[0], i);
        }
    }
    
    return NULL;
    
}

