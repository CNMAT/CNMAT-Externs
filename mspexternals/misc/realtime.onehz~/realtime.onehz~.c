/*
Written by Andy Schmeder, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2009, The Regents of
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
NAME: realtime.onehz~
DESCRIPTION: A phasor that runs at exactly one Hz according to physical time
AUTHORS: Andy Schmeder
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First release
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include <math.h>
#include <string.h>

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "z_dsp.h"

#include "version.h"
#include "version.c"

#include "cmmjl.h"
#include "cmmjl_osc_timetag.h"
#include "cmmjl_realtime.h"

typedef struct _realtime_onehz {
    
    // max object stuff
	t_pxobject ob;
    long in;            // when a message is received in an inlet, the inlet number will be stored here.
	void *proxy;        // necessary for objects with multiple inlets
    
    // frame size and sample rate
    int nper;
    float fs;
    
} t_realtime_onehz;

static t_class *realtime_onehz_class;

void realtime_onehz_free(t_realtime_onehz *x);
void realtime_onehz_assist(t_realtime_onehz *x, void *b, long m, long a, char *s);
void *realtime_onehz_new(Symbol *s, short argc, t_atom *argv);

t_int *realtime_onehz_perform(t_int *w) {
    
    t_realtime_onehz *x = (t_realtime_onehz*)w[1];
    float* out = (float*)w[2];

    int i;
    
    double t;
    double dt;
    
    struct ntptime now;
    
    cmmjl_realtime_clock_tick((void*)x);
    cmmjl_realtime_clock_now(&now);

    // ...
    t = cmmjl_osc_timetag_ntp_to_float_frac(&now);   // remainder start time
    
    // rate of time change per sample (not quite correct in poly~)
    dt = (cmmjl_realtime_clock_dt() / (x->nper)) * (x->fs / sys_getsr());   
    
    for(i = 0; i < x->nper; i++) {
        out[i] = t;
        t += dt;
        if(t > 1.0) {
            t -= 1.0;
        }
    }
    
    return w + 2 + 1;
}

void realtime_onehz_dsp(t_realtime_onehz *x, t_signal **sp, short *connect) {
    
    x->nper = sp[0]->s_n;
    x->fs = sp[0]->s_sr;
    
    cmmjl_realtime_clock_register((void*)x);
    
    dsp_add(realtime_onehz_perform, 2, x, sp[0]->s_vec, NULL);
}

void realtime_onehz_free(t_realtime_onehz *x) {
    dsp_free((t_pxobject *)x);
}

void realtime_onehz_assist(t_realtime_onehz *x, void *b, long io, long index, char *s) {
	switch(io) {
	case 1:
		switch(index) {
            case 0:
                sprintf(s, "frequency: signal, float, int");
                break;
            case 1:
                sprintf(s, "phase: signal, float, int");
			break;
        }
	case 2:
        sprintf(s, "onehz signal");
		break;
	}
}

void *realtime_onehz_new(Symbol *s, short argc, Atom *argv) {
    
	t_realtime_onehz *x;

	if(x = (t_realtime_onehz *)object_alloc(realtime_onehz_class)) {
        
        // its a dsp object with zero no signal inlets
        dsp_setup((t_pxobject *)x, 0);

        outlet_new((t_object *)x, "signal");

        // default frequency, phase
        
		return x;
	}
    
	return NULL;
}

int main(void) {
    
	t_class *c = class_new("realtime.onehz~", (method)realtime_onehz_new, (method)realtime_onehz_free, sizeof(t_realtime_onehz), 0L, A_GIMME, 0);

    version(0);
    
    class_dspinit(c);
    
    class_addmethod(c, (method)realtime_onehz_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)realtime_onehz_assist, "assist", A_CANT, 0);
	class_register(CLASS_BOX, c);
	realtime_onehz_class = c;
    
	common_symbols_init();
    cmmjl_common_symbols_init();
    
    cmmjl_realtime_clock_init();

	return 0;
}

