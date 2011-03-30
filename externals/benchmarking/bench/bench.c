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
#include "ext_obex.h"
#include "version.c"
#include "math.h"
//#include "sys/time.h"
//#include "cycle.h"
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#define BENCH_IN 0
#define BENCH_OUT 1

typedef struct _bench{
       	t_object t_ob;
	void *proxy[1];
	long inletNumber;
	void *out0;
	void *out1;
	int t_objmode;
	uint64_t time;
} t_bench;

t_class *bench_class;

void bench_assist(t_bench *x, void *b, long m, long a, char *s);
void *bench_new(t_symbol *mode);
void bench_free(t_bench *x);
void bench_anything(t_bench *x, t_symbol *msg, short argc, t_atom *argv);
void bench_list(t_bench *x, t_symbol *msg, short argc, t_atom *argv);
void bench_int(t_bench *x, long i);
void bench_float(t_bench *x, double f);
void bench_bang(t_bench *x);

int main(void){
	t_class *c = class_new("bench", (method)bench_new, (method)bench_free, (short)sizeof(t_bench), 0L, A_SYM, 0);
	
	version(0);

	class_addmethod(c, (method) version, "version", 0);
	class_addmethod(c, (method)bench_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)bench_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)bench_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)bench_int, "int", 0);
	class_addmethod(c, (method)bench_float, "float", 0);
	class_addmethod(c, (method)bench_bang, "bang", 0);

	bench_class = c;
	class_register(CLASS_BOX, c);
	
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

	x = (t_bench *)object_alloc(bench_class);
	if(!x){
		return NULL;
	}

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
		uint64_t t = mach_absolute_time();
		uint32_t l1, l2;

		l1 = (uint32_t)((t & 0xffffffff00000000LL) >> 32);
		l2 = (uint32_t)(t & 0xffffffffLL);
		SETLONG(&out[0], l1);
		SETLONG(&out[1], l2);
		outlet_list(x->out1, NULL, 2, out);
		outlet_anything(x->out0, msg, argc, argv);
	}else{
		if(proxy_getinlet((t_object *)x) == 1){
			if(argc != 2){
				error("bench: the right inlet should get 2 numbers, only got %d", argc);
				return;
			}
			int32_t l1 = argv[0].a_w.w_long;
			int32_t l2 = argv[1].a_w.w_long;
			uint64_t t1;
			t1 = *((uint32_t *)(&l1));
			t1 = t1 << 32;
			t1 = t1 | *((uint32_t *)(&l2));
			x->time = t1;
		}
	}
}

void bench_list(t_bench *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		uint64_t t = mach_absolute_time();
		uint32_t l1, l2;

		l1 = (uint32_t)((t & 0xffffffff00000000LL) >> 32);
		l2 = (uint32_t)(t & 0xffffffffLL);
		SETLONG(&out[0], l1);
		SETLONG(&out[1], l2);
		outlet_list(x->out1, NULL, 2, out);
		outlet_list(x->out0, msg, argc, argv);
	}else{
		if(proxy_getinlet((t_object *)x) == 1){
			if(argc != 2){
				error("bench: the right inlet should get 2 numbers, only got %d", argc);
				return;
			}
			int32_t l1 = argv[0].a_w.w_long;
			int32_t l2 = argv[1].a_w.w_long;
			uint64_t t1;
			t1 = *((uint32_t *)(&l1));
			t1 = t1 << 32;
			t1 = t1 | *((uint32_t *)(&l2));
			x->time = t1;
		}
	}
}

void bench_bang(t_bench *x){
	if(x->t_objmode == BENCH_OUT){
		uint64_t t2 = mach_absolute_time();
		uint64_t diff = t2 - x->time;
		UnsignedWide elapsedNano = AbsoluteToNanoseconds(*(AbsoluteTime *)&diff);
		uint64_t en = UnsignedWideToUInt64(elapsedNano);
		//post("%f, %f", (double)en, (double)en * 0.000001);
		outlet_float(x->out0, (double)en * 0.000001);
	}else{
		t_atom out[2];
		uint64_t t = mach_absolute_time();
		uint32_t l1, l2;

		l1 = (uint32_t)((t & 0xffffffff00000000LL) >> 32);
		l2 = (uint32_t)(t & 0xffffffffLL);
		atom_setlong(out, l1);
		atom_setlong(out + 1, l2);
		outlet_list(x->out1, NULL, 2, out);
		outlet_bang(x->out0);
	}
}

void bench_float(t_bench *x, double f){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		uint64_t t = mach_absolute_time();
		uint32_t l1, l2;

		l1 = (uint32_t)((t & 0xffffffff00000000LL) >> 32);
		l2 = (uint32_t)(t & 0xffffffffLL);
		atom_setlong(out, l1);
		atom_setlong(out + 1, l2);
		outlet_list(x->out1, NULL, 2, out);
		outlet_float(x->out0, f);
	}
}

void bench_int(t_bench *x, long i){
	t_atom out[2];
	if(x->t_objmode == BENCH_IN){
		uint64_t t = mach_absolute_time();
		uint32_t l1, l2;

		l1 = (uint32_t)((t & 0xffffffff00000000LL) >> 32);
		l2 = (uint32_t)(t & 0xffffffffLL);
		atom_setlong(out, l1);
		atom_setlong(out + 1, l2);
		outlet_list(x->out1, NULL, 2, out);
		outlet_int(x->out0, i);
	}
}
