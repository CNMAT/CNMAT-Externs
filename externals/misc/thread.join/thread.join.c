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

 thread.join.c
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: thread.join
DESCRIPTION: Pass data from a secondary thread back to the main thread
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 1634 $
VERSION 0.1: First public release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

// max object header
#include "ext.h"

// version info
#include "version.h"
#include "version.c"

/* structure definition of your object */
typedef struct _thread_join
{
    Object o_ob;    // required header
    
    t_object* in_p[1];  // either a normal inlet or a crazy proxy thing
    long in_i_unsafe;   // which inlet message arrived on, but isn't threadsafe
  
    t_object* out_p[1]; // outlet
  
} thread_join;

/* global that holds the class definition */
void *thread_join_class;

// basic prototypes
void* thread_join_new(Symbol* s, short argc, Atom* argv);
void thread_join_free(thread_join *x);
void thread_join_assist (thread_join *x, void *box, long msg, long arg, char *dstString);

// methods
void thread_join_anything(thread_join *x, Symbol* s, int argc, Atom* argv);
void thread_join_outlet_anything(thread_join *x, Symbol* s, int argc, Atom* argv);

void thread_join_bang(thread_join *x);
void thread_join_outlet_bang(thread_join *x, Symbol* s, int argc, Atom* argv);

void thread_join_float(thread_join *x, double f);
void thread_join_outlet_float(thread_join *x, Symbol* s, int argc, Atom* argv);

void thread_join_int(thread_join *x, int i);
void thread_join_outlet_int(thread_join *x, Symbol* s, int argc, Atom* argv);

Symbol* ps_bang;
Symbol* ps_float;
Symbol* ps_int;

// setup
void main(fptr *f)
{
    // post version
    version(0);
    
    setup((t_messlist **)&thread_join_class, (method)thread_join_new, (method)thread_join_free, (short)sizeof(thread_join), 0L, A_GIMME, 0);
    
    addmess((method)thread_join_anything, "anything", A_GIMME, 0);
    addbang((method)thread_join_bang);
    addfloat((method)thread_join_float);
    addint((method)thread_join_int);
    
    // tooltip helper
    addmess((method)thread_join_assist, "assist", A_CANT, 0);
    
    ps_bang = gensym("bang");
    ps_int = gensym("int");
    ps_float = gensym("float");
    
}

void *thread_join_new(Symbol* s, short argc, Atom *argv)
{
    thread_join *x;
    
    x = newobject(thread_join_class);
    x->out_p[0] = outlet_new(x, NULL);

    return (x);
}

void thread_join_free(thread_join* x) {
    
}

// tooltip assist

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void thread_join_assist (thread_join *x, void *box, long msg, long arg, char *dstString) {
    if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "anything, to be passed to main thread");
        }
    } else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "anything, in main thread");
        }
    }
}

void thread_join_anything(thread_join *x, Symbol* s, int argc, Atom* argv) {
    schedule_defer(x, thread_join_outlet_anything, 0, s, argc, argv);
}

void thread_join_outlet_anything(thread_join *x, Symbol* s, int argc, Atom* argv) {
    outlet_anything(x->out_p[0], s, argc, argv);
}

void thread_join_bang(thread_join* x) {
    schedule_defer(x, thread_join_outlet_bang, 0, ps_bang, 0, NULL);
}

void thread_join_outlet_bang(thread_join *x, Symbol* s, int argc, Atom* argv) {
    outlet_bang(x->out_p[0]);
}

void thread_join_float(thread_join* x, double f) {
    Atom a;
    atom_setfloat(&a, f);
    schedule_defer(x, thread_join_outlet_float, 0, ps_float, 1, &a);
}

void thread_join_outlet_float(thread_join *x, Symbol* s, int argc, Atom* argv) {
    outlet_float(x->out_p[0], atom_getfloat(argv));
}

void thread_join_int(thread_join* x, int i) {
    Atom a;
    atom_setlong(&a, i);
    schedule_defer(x, thread_join_outlet_int, 0, ps_int, 1, &a);
}

void thread_join_outlet_int(thread_join *x, Symbol* s, int argc, Atom* argv) {
    outlet_int(x->out_p[0], atom_getlong(argv));
}

