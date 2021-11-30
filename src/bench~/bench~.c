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
NAME: bench~
DESCRIPTION: Benchmarking utility
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2008
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
#define NAME "bench~"
#define DESCRIPTION "Benchmarking utility"
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2008,12,13"


#include "version.h"
#include "ext.h"
#include "ext_obex.h"


#include "z_dsp.h"
#include "math.h"
#include "sys/time.h"
#include "cycle.h"

#define BENCH_IN 0
#define BENCH_OUT 1

#define	BENCH_MODE_ZEROS 0
#define BENCH_MODE_ONES 1
#define BENCH_MODE_RAND 2
#define BENCH_MODE_THRU 3

typedef struct _bench{
    t_pxobject t_ob;
	int t_objmode;
	int t_mode;
} t_bench;

t_class *bench_class;

void bench_assist(t_bench *x, void *b, long m, long a, char *s);
void *bench_new(t_symbol *msg, short argc, t_atom *argv);
//t_int *bench_perform_in(t_int *w);
//t_int *bench_perform_in_connected(t_int *w);
//t_int *bench_perform_out(t_int *w);
//void bench_dsp(t_bench *x, t_signal **sp, short *count);
void bench_dsp64(t_bench *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void bench_perform64_in(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void bench_perform64_in_connected(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void bench_perform64_out(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void bench_free(t_bench *x);

int main(void){
	bench_class = class_new("bench~", (method)bench_new, (method)bench_free, (short)sizeof(t_bench), 0L, A_GIMME, 0);

	version_post_copyright();

	class_addmethod(bench_class, (method) version, "version", 0);
	class_addmethod(bench_class, (method)bench_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(bench_class, (method)bench_assist, "assist", A_CANT, 0);

	class_dspinit(bench_class);


	class_register(CLASS_BOX, bench_class);
	return 0;
}

void bench_assist(t_bench *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

void *bench_new(t_symbol *msg, short argc, t_atom *argv){
	t_bench *x;

	x = (t_bench *)object_alloc(bench_class);
	if(!x){
		return NULL;
	}


	if(!argc){
		object_error((t_object *)x, "bench~: one argument is required, either in or out");
		return NULL;
	}else{
		if(!(argv[0].a_type == A_SYM)){
			object_error((t_object *)x, "bench~: the first argument to bench must be a symbol (either \"in\" or \"out\")");
			return NULL;
		}else if(!strcmp(argv[0].a_w.w_sym->s_name, "in")){
			dsp_setup((t_pxobject *)x, 1);
			outlet_new((t_object *)x, "signal");
			outlet_new((t_object *)x, "signal");
			x->t_objmode = BENCH_IN;

			if(argc == 1) x->t_mode = BENCH_MODE_ONES;
			else{
				if(!strcmp(argv[1].a_w.w_sym->s_name, "zeros")){
					x->t_mode = BENCH_MODE_ZEROS;
				}else if(!strcmp(argv[1].a_w.w_sym->s_name, "ones")){
					x->t_mode = BENCH_MODE_ONES;
				}else if(!strcmp(argv[1].a_w.w_sym->s_name, "rand")){
					x->t_mode = BENCH_MODE_RAND;
				}else if(!strcmp(argv[1].a_w.w_sym->s_name, "thru")){
					x->t_mode = BENCH_MODE_THRU;
				}else {
					object_error((t_object *)x, "bench~: unrecognized argument %s.  Setting the mode to \"ones\"", argv[1].a_w.w_sym->s_name);
					x->t_mode = BENCH_MODE_ONES;
				}
			}
		}else if(!strcmp(argv[0].a_w.w_sym->s_name, "out")){
			dsp_setup((t_pxobject *)x, 2);
			outlet_new((t_object *)x, "signal");
			x->t_objmode = BENCH_OUT;
		}else {
			object_error((t_object *)x, "bench~: unrecognized argument %s", argv[0].a_w.w_sym->s_name);
			return NULL;
		}
	}
	//x->t_ob.z_misc |= Z_NO_INPLACE;

	return x;
}

void bench_free(t_bench *x){
	dsp_free((t_pxobject *)x);
}

void bench_dsp64(t_bench *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->t_objmode == BENCH_IN){
        if(!count[0]){
            object_method(dsp64, gensym("dsp_add64"), x, bench_perform64_in, 0, NULL);
        }else{
            object_method(dsp64, gensym("dsp_add64"), x, bench_perform64_in_connected, 0, NULL);
        }
    }else{
        object_method(dsp64, gensym("dsp_add64"), x, bench_perform64_out, 0, NULL);
    }
}
/*
void bench_dsp(t_bench *x, t_signal **sp, short *count){
	if(x->t_objmode == BENCH_IN){
		if(!count[0]) dsp_add(bench_perform_in, 4, x, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
		else dsp_add(bench_perform_in_connected, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
	}
	else dsp_add(bench_perform_out, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}
*/
void bench_perform64_in(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    ticks t = getticks();
    outs[1][0] = (double)t;
}
/*
t_int *bench_perform_in(t_int *w){
	t_bench *x = (t_bench *)w[1];
	t_float *out1 = (t_float *)w[2];
	t_float *out2 = (t_float *)w[3];
	int n = (int)w[4];
	ticks t = getticks();
	unsigned long l1, l2;

	l1 = (unsigned long)((t & 0xffffffff00000000LL) >> 32);
	l2 = (unsigned long)(t & 0xffffffffLL);
	out2[0] = *((float *)(&l1));
	out2[1] = *((float *)(&l2));

	return (w + 5);
}
*/
void bench_perform64_in_connected(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    ticks t = getticks();
    outs[1][0] = (double)t;
    memcpy(outs[0], ins[0], sizeof(double) * sampleframes);
}
/*
t_int *bench_perform_in_connected(t_int *w){
	t_bench *x = (t_bench *)w[1];
	t_float *in = (t_float *)w[2];
	t_float *out1 = (t_float *)w[3];
	t_float *out2 = (t_float *)w[4];
	int n = (int)w[5];
	ticks t = getticks();
	unsigned long l1, l2;

	l1 = (unsigned long)((t & 0xffffffff00000000LL) >> 32);
	l2 = (unsigned long)(t & 0xffffffffLL);
	out2[0] = *((float *)(&l1));
	out2[1] = *((float *)(&l2));

	memcpy(out1, in, sizeof(float) * n);

	return (w + 6);
}
*/
void bench_perform64_out(t_bench *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    ticks t2 = getticks();
    double diff = elapsed(t2, (ticks)ins[1][0]);
    for(int i = 0; i < sampleframes; i++){
        outs[0][i] = diff;
    }
}
/*
t_int *bench_perform_out(t_int *w){
	t_bench *x = (t_bench *)w[1];
	t_float *in1 = (t_float *)w[2];
	t_float *in2 = (t_float *)w[3];
	t_float *out = (t_float *)w[4];

	int n = (int)w[5];
	int i;

	ticks t2 = getticks();
	unsigned long long t1;
	t1 = *((unsigned long *)(in2));
	t1 = t1 << 32;
	t1 = t1 | *((unsigned long *)(in2 + 1));

	double diff = elapsed(t2, (ticks)t1);

	for(i = 0; i < n; i++)
		out[i] = (float)diff;

	return (w + 6);

}
 */
