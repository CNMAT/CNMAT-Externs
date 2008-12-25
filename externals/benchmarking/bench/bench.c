/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2008, The Regents of
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


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: bench
DESCRIPTION: Benchmarking utility
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "math.h"
#include "sys/time.h"
#include "cycle.h"

#define BENCH_IN 0
#define BENCH_OUT 1

typedef struct _bench{
       	t_object t_ob;
	void *proxy[1];
	long inletNumber;
	void *out0;
	void *out1;
	int t_objmode;
	unsigned long long time;
} t_bench;

void *bench_class;

void bench_assist(t_bench *x, void *b, long m, long a, char *s);
void *bench_new(t_symbol *mode);
void bench_free(t_bench *x);
void bench_anything(t_bench *x, t_symbol *msg, short argc, t_atom *argv);
void bench_int(t_bench *x, long i);
void bench_float(t_bench *x, double f);
void bench_bang(t_bench *x);

int main(void){
	setup((t_messlist **)&bench_class, (method)bench_new, (method)bench_free, (short)sizeof(t_bench), 0L, A_SYM, 0);
	
	version(0);

	addmess((method) version, "version", 0);
	addmess((method)bench_assist, "assist", A_CANT, 0);
	addmess((method)bench_anything, "anything", A_GIMME, 0);
	addint((method)bench_int);
	addfloat((method)bench_float);
        addbang((method)bench_bang);
	
	return 0;
}

void bench_assist(t_bench *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

void *bench_new(t_symbol *mode){
	t_bench *x;

	x = (t_bench *)newobject(bench_class);

	if(!strcmp(mode->s_name, "in")){
		x->out1 = listout((t_object *)x);
		x->out0 = outlet_new((t_object *)x, 0L);
		x->t_objmode = BENCH_IN;
	}else if(!strcmp(mode->s_name, "out")){
		x->proxy[0] = proxy_new(x, 1, &x->inletNumber);
		x->out0 = floatout((t_object *)x);
		x->t_objmode = BENCH_OUT;
	}else {
		error("bench~: unrecognized argument %s", mode->s_name);
		return NULL;
	}

	return x;
}

void bench_free(t_bench *x){
}

void bench_anything(t_bench *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		ticks t = getticks();
		unsigned long l1, l2;

		l1 = (unsigned long)((t & 0xffffffff00000000LL) >> 32);
		l2 = (unsigned long)(t & 0xffffffffLL);
		SETLONG(&out[0], l1);
		SETLONG(&out[1], l2);
		outlet_anything(x->out1, gensym("list"), 2, out);
		outlet_anything(x->out0, msg, argc, argv);
	}else{
		if(proxy_getinlet((t_object *)x) == 1){
			if(argc != 2){
				error("bench: the right inlet should get 2 numbers, only got %d", argc);
				return;
			}
			long l1 = argv[0].a_w.w_long;
			long l2 = argv[1].a_w.w_long;
			unsigned long long t1;
			t1 = *((unsigned long *)(&l1));
			t1 = t1 << 32;
			t1 = t1 | *((unsigned long *)(&l2));
			x->time = t1;
		}
	}
}

void bench_bang(t_bench *x){
	if(x->t_objmode == BENCH_OUT){
		ticks t2 = getticks();
		double diff = elapsed(t2, (ticks)x->time);
		outlet_float(x->out0, diff);
	}
}

void bench_float(t_bench *x, double f){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		ticks t = getticks();
		unsigned long l1, l2;

		l1 = (unsigned long)((t & 0xffffffff00000000LL) >> 32);
		l2 = (unsigned long)(t & 0xffffffffLL);
		SETLONG(&out[0], l1);
		SETLONG(&out[1], l2);
		outlet_anything(x->out1, gensym("list"), 2, out);
		SETFLOAT(&out[0], f);
		outlet_anything(x->out0, gensym("float"), 1, out);
	}
}

void bench_int(t_bench *x, long i){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		ticks t = getticks();
		unsigned long l1, l2;

		l1 = (unsigned long)((t & 0xffffffff00000000LL) >> 32);
		l2 = (unsigned long)(t & 0xffffffffLL);
		SETLONG(&out[0], l1);
		SETLONG(&out[1], l2);
		outlet_anything(x->out1, gensym("list"), 2, out);
		SETFLOAT(&out[0], i);
		outlet_anything(x->out0, gensym("long"), 1, out);
	}
}
