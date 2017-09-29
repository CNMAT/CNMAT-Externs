/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2006, The Regents of
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
NAME: vsnapshot~
DESCRIPTION: Like snapshot~ but outputs the entire current signal vector.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2007
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: First version
VERSION 1.0.1: Fixed a memory bug
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
#define NAME "vsnapshot~"
#define DESCRIPTION "Like snapshot~ but outputs the entire current signal vector."
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2007,12,13-17"


#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_critical.h"

#include "z_dsp.h"
#include <inttypes.h>

#define VSHT_MAX_VSIZE 4096

typedef struct _vsht
{
	t_pxobject ob;
	void *outlet;
	t_atom *buf;
	int n;
	int swap;
	void *clock;
	int interval;
	int running;
	t_critical lock;
	uint64_t dsp_counter, sched_counter;
} t_vsht;

t_class *vsht_class;

void vsht_assist(t_vsht *x, void *b, long m, long a, char *s);
void *vsht_new(long t);
void vsht_start(t_vsht *x);
void vsht_stop(t_vsht *x);
void vsht_int(t_vsht *x, long i);
void vsht_in1(t_vsht *x, long i);
void vsht_bang(t_vsht *x);
t_int *vsht_perform(t_int *w);
void vsht_perform64(t_vsht *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void vsht_dsp(t_vsht *x, t_signal **sp, short *count);
void vsht_dsp64(t_vsht *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void vsht_tick(t_vsht *x, t_symbol *msg, int argc, t_atom *argv);
void vsht_free(t_vsht *x);

t_int *vsht_perform(t_int *w)
{
	t_vsht *x = (t_vsht *)w[1];
	float *in = (t_float *)w[2];
	int n = (int)w[3];

	t_atom a[n + 1];
	atom_setlong(a, x->dsp_counter++);
	for(int i = 0; i < n; i++){
		atom_setfloat(a + i + 1, in[i]);
	}
	schedule_delay(x, (method)vsht_tick, 0, NULL, n + 1, a);

	return w + 4;
}

void vsht_perform64(t_vsht *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	int n = sampleframes;
	t_atom a[n + 1];
	atom_setlong(a, x->dsp_counter++);
	for(int i = 0; i < n; i++){
		atom_setfloat(a + i + 1, ins[0][i]);
	}
	schedule_delay(x, (method)vsht_tick, 0, NULL, n + 1, a);
}

void vsht_dsp(t_vsht *x, t_signal **sp, short *count)
{
	clock_delay(x->clock, x->interval);
	dsp_add(vsht_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);
}

void vsht_dsp64(t_vsht *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	clock_delay(x->clock, x->interval);
	object_method(dsp64, gensym("dsp_add64"), x, vsht_perform64, 0, NULL);
}

void vsht_tick(t_vsht *x, t_symbol *msg, int argc, t_atom *argv)
{
	critical_enter(x->lock);
	if(atom_getlong(argv) != x->sched_counter++){
		// do something productive here or post an error
		//return;
	}
	x->n = argc - 1;
	memcpy(x->buf, argv + 1, (argc - 1) * sizeof(t_atom));
	critical_exit(x->lock);
}

void vsht_start(t_vsht *x)
{
	if(x->interval > 0){
		x->running = 1;
		clock_delay(x->clock, 0);
	}
}

void vsht_stop(t_vsht *x)
{
	x->running = 0;
	clock_unset(x->clock);
}

void vsht_int(t_vsht *x, long i)
{
	x->running = (int)i;
	if(i == 0){
		vsht_stop(x);
	}else{
		vsht_start(x);
	}
}

void vsht_in1(t_vsht *x, long i)
{
	x->interval = (int)i;
	vsht_start(x);
}

void vsht_bang(t_vsht *x)
{
	if(sys_getdspstate()){
		int n = 0;
		critical_enter(x->lock);
		n = x->n;
		t_atom buf[n];
		memcpy(buf, x->buf, n * sizeof(t_atom));
		critical_exit(x->lock);
		outlet_list(x->outlet, NULL, n, buf);
		if(x->running && x->interval > 0){
			clock_delay(x->clock, x->interval);
		}
	}
}

void vsht_assist(t_vsht *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET){
	}
	else {
	}
}

void vsht_free(t_vsht *x)
{
	dsp_free((t_pxobject *)x);
	freeobject(x->clock);
	critical_free(x->lock);
	if(x->buf){
		free(x->buf);
	}
}

void *vsht_new(long t)
{
	t_vsht *x;

	x = (t_vsht *)object_alloc(vsht_class);
	if(!x){
		return NULL;
	}

	intin(x, 1);
	dsp_setup((t_pxobject *)x, 1);
	x->outlet = listout((t_pxobject *)x);

	x->clock = clock_new(x, (method)vsht_bang);
	x->running = 0;
	x->interval = 0;

	x->buf = (t_atom *)calloc(VSHT_MAX_VSIZE, sizeof(t_atom));

	x->dsp_counter = x->sched_counter = 0;

	critical_new(&(x->lock));

	if(t > 0){
		x->interval = t;
		x->running = 1;
		clock_delay(x->clock, x->interval);
	}

	return x;
}

int main(void)
{
	vsht_class = class_new("vsnapshot~", (method)vsht_new, (method)vsht_free, (short)sizeof(t_vsht), 0L, A_DEFLONG, 0);
	
	version_post_copyright();

	class_addmethod(vsht_class, (method) version, "version", 0);
	class_addmethod(vsht_class, (method)vsht_dsp, "dsp", A_CANT, 0);
	class_addmethod(vsht_class, (method)vsht_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(vsht_class, (method)vsht_assist, "assist", A_CANT, 0);
	class_addmethod(vsht_class, (method)vsht_bang, "bang", 0);
	class_addmethod(vsht_class, (method)vsht_in1, "in1", A_LONG, 0);
	class_addmethod(vsht_class, (method)vsht_start, "start", 0);
	class_addmethod(vsht_class, (method)vsht_stop, "stop", 0);
	class_addmethod(vsht_class, (method)vsht_int, "int", A_LONG, 0);
	
	class_dspinit(vsht_class);
	
	
	class_register(CLASS_BOX, vsht_class);
	return 0;
}
