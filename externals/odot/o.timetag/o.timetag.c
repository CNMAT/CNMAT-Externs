/*
Written by John MacCallum, The Center for New Music and Audio Technologies,
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
NAME: o.timetag
DESCRIPTION: Manipulate the timetag of an OSC bundle
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_osc.h"
#include "cmmjl/cmmjl_osc_timetag.h"

typedef struct _otimetag{
	t_object ob;
	void *outlet;
	void *proxy;
	char *buffer;
	int buffer_len;
	int buffer_pos;
	long inlet;
	t_symbol *op_sym;
	void (*op)(struct _otimetag *, long, char *);
	t_symbol *time; // now, immediate, future, or past
	double arg;
	ntptime prev_timetag;
	int have_prev_timetag;
	ntptime right_timetag;
	int have_right_timetag;
} t_otimetag;


void *otimetag_class;

void otimetag_fullPacket(t_otimetag *x, long len, long ptr);
void otimetag_op_set(t_otimetag *x, long len, char *ptr);
void otimetag_op_add(t_otimetag *, long len, char *ptr);
void otimetag_op_subtract(t_otimetag *, long len, char *ptr);
void otimetag_op_multiply(t_otimetag *, long len, char *ptr);
void otimetag_op_divide(t_otimetag *, long len, char *ptr);
void otimetag_op_derivative(t_otimetag *, long len, char *ptr);
void otimetag_op_synchronize(t_otimetag *, long len, char *ptr);
void otimetag_output_bundle(t_otimetag *x, long len, char *buf);
void otimetag_cbk(t_cmmjl_osc_message msg, void *v);
void otimetag_anything(t_otimetag *x, t_symbol *msg, short argc, t_atom *argv);
void otimetag_int(t_otimetag *x, long l);
void otimetag_float(t_otimetag *x, double f);
void otimetag_free(t_otimetag *x);
void otimetag_assist(t_otimetag *x, void *b, long m, long a, char *s);
void otimetag_clear(t_otimetag *x);
void *otimetag_new(t_symbol *msg, short argc, t_atom *argv);
t_max_err otimetag_notify(t_otimetag *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void otimetag_fullPacket(t_otimetag *x, long len, long ptr){
	// check for buffer overflow here!

	long n = len;
	memcpy(x->buffer, (char *)ptr, n);
	x->buffer_pos = len;
	if(strncmp(x->buffer, "#bundle\0", 8)){
		if((n = cmmjl_osc_bundle_naked_message(len, x->buffer, x->buffer)) < 0){
			error("problem bundling naked message");
			return;
		}
	}

	// flatten any nested bundles
	x->buffer_pos = cmmjl_osc_flatten(n, x->buffer, x->buffer);

	if(proxy_getinlet((t_object *)x) == 0){
		x->op(x, x->buffer_pos, x->buffer);
		cmmjl_osc_timetag_get(n, ptr, &(x->prev_timetag));
		x->have_prev_timetag = 1;
	}else{
		cmmjl_osc_timetag_get(n, ptr, &(x->right_timetag));
		x->have_right_timetag = 1;
	}
}

void otimetag_op_set(t_otimetag *x, long len, char *ptr){
	if(x->time == gensym("now")){
		ntptime now;
		cmmjl_osc_timetag_now_to_ntp(&now);
		cmmjl_osc_timetag_set(x->buffer_len, (long)x->buffer, &now);
	}else if(x->time == gensym("immediate")){
		cmmjl_osc_init_bundle(x->buffer_len, x->buffer, NULL);
	}else if(x->time == gensym("future")){
		post("future");
		ntptime now, offset, now_plus_offset;
		cmmjl_osc_timetag_now_to_ntp(&now);
		cmmjl_osc_timetag_float_to_ntp(x->arg / 1000., &offset);
		cmmjl_osc_timetag_add(&now, &offset, &now_plus_offset);
		cmmjl_osc_timetag_set(x->buffer_len, (long)x->buffer, &now_plus_offset);
	}else if(x->time == gensym("past")){
		ntptime now, offset, now_plus_offset;
		cmmjl_osc_timetag_now_to_ntp(&now);
		cmmjl_osc_timetag_float_to_ntp((x->arg / 1000.) * -1., &offset);
		cmmjl_osc_timetag_add(&now, &offset, &now_plus_offset);
		cmmjl_osc_timetag_set(x->buffer_len, (long)x->buffer, &now_plus_offset);
	}

	otimetag_output_bundle(x, x->buffer_pos, x->buffer);
}

void otimetag_op_add(t_otimetag *x, long len, char *ptr){
	ntptime tt, offset, sum;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	cmmjl_osc_timetag_float_to_ntp(x->arg / 1000., &offset);
	cmmjl_osc_timetag_add(&tt, &offset, &sum);
	cmmjl_osc_timetag_set(len, (long)ptr, &sum);
	otimetag_output_bundle(x, len, ptr);
}

void otimetag_op_subtract(t_otimetag *x, long len, char *ptr){
	ntptime tt, offset, sum;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	cmmjl_osc_timetag_float_to_ntp(x->arg / 1000., &offset);
	offset.sign *= -1;
	cmmjl_osc_timetag_add(&tt, &offset, &sum);
	cmmjl_osc_timetag_set(len, (long)ptr, &sum);
	otimetag_output_bundle(x, len, ptr);
}

void otimetag_op_multiply(t_otimetag *x, long len, char *ptr){
	ntptime tt, result;
	double f;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	f = cmmjl_osc_timetag_ntp_to_float(&tt);
	f = (x->arg / 1000.) * f;
	cmmjl_osc_timetag_float_to_ntp(f, &result);
	cmmjl_osc_timetag_set(len, (long)ptr, &result);
	otimetag_output_bundle(x, len, ptr);
}

void otimetag_op_divide(t_otimetag *x, long len, char *ptr){
	ntptime tt, result;
	double f;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	f = cmmjl_osc_timetag_ntp_to_float(&tt);
	f = (x->arg / 1000.) / f;
	cmmjl_osc_timetag_float_to_ntp(f, &result);
	cmmjl_osc_timetag_set(len, (long)ptr, &result);
	otimetag_output_bundle(x, len, ptr);
}

void otimetag_op_derivative(t_otimetag *x, long len, char *ptr){
	if(!(x->have_prev_timetag)){
		return;
	}
	ntptime tt, diff;
	if(cmmjl_osc_timetag_is_immediate(ptr + 8) || cmmjl_osc_timetag_is_immediate((char *)(&(x->prev_timetag)))){
			return;
	}
	x->prev_timetag.sign = -1;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	cmmjl_osc_timetag_add(&tt, &(x->prev_timetag), &diff);
	double f = cmmjl_osc_timetag_ntp_to_float(&diff);
	outlet_float(x->outlet, f);
}

void otimetag_op_synchronize(t_otimetag *x, long len, char *ptr){
	if(x->have_right_timetag){
		cmmjl_osc_timetag_set(len, (long)ptr, &(x->right_timetag));
		otimetag_output_bundle(x, len, ptr);
	}
}

void otimetag_op_compare(t_otimetag *x, long len, char *ptr){
	if(!(x->have_right_timetag)){
		return;
	}
	ntptime tt;
	cmmjl_osc_timetag_get(len, (long)ptr, &tt);
	outlet_int(x->outlet, cmmjl_osc_timetag_cmp(&(x->right_timetag), &tt));
}

void otimetag_output_bundle(t_otimetag *x, long len, char *buf){
	t_atom out[2];
	atom_setlong(&(out[0]), len);
	atom_setlong(&(out[1]), (long)buf);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void otimetag_anything(t_otimetag *x, t_symbol *msg, short argc, t_atom *argv){
	if(!msg){
		error("o.delay: not a properly formatted OSC-style message");
		return;
	}
	long ac = argc + 1;
	t_atom av[ac];
	atom_setsym(av, msg);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	char buf[4096];
	int len = 4096;
	cmmjl_osc_init_bundle(len, buf, NULL);
	len = cmmjl_osc_make_bundle_from_atoms(ac, av, &len, buf + 16) + 16;
	otimetag_output_bundle(x, len, buf);
}

void otimetag_assist(t_otimetag *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void otimetag_clear(t_otimetag *x){
}

void otimetag_free(t_otimetag *x){
	object_free(x->proxy);
}

void *otimetag_new(t_symbol *msg, short argc, t_atom *argv){
	t_otimetag *x;
	int i;
	if(x = (t_otimetag *)object_alloc(otimetag_class)){
		cmmjl_init(x, NAME, 0);
		object_attach_byptr_register(x, x, CLASS_BOX);
		x->outlet = outlet_new(x, NULL);
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		x->buffer = (char *)calloc(4096, sizeof(char));
		x->buffer_len = 4096;
		x->buffer_pos = 0;
		x->op_sym = gensym("set");
		x->op = otimetag_op_set;
		x->time = gensym("now");
		x->arg = 1.;
		x->have_prev_timetag = 0;
		x->have_right_timetag = 0;

		attr_args_process(x, argc, argv);
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.timetag", (method)otimetag_new, (method)otimetag_free, sizeof(t_otimetag), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)otimetag_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)otimetag_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)otimetag_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)otimetag_clear, "clear", 0);
	class_addmethod(c, (method)otimetag_anything, "anything", A_GIMME, 0);

	CLASS_ATTR_SYM(c, "op", 0, t_otimetag, op_sym);
	CLASS_ATTR_SYM(c, "time", 0, t_otimetag, time);
	CLASS_ATTR_DOUBLE(c, "arg", 0, t_otimetag, arg);
    
	class_register(CLASS_BOX, c);
	otimetag_class = c;

	common_symbols_init();
	return 0;
}


t_max_err otimetag_notify(t_otimetag *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		if(attrname == gensym("op")){
			if(x->op_sym == gensym("set")){
				x->op = otimetag_op_set;
			}else if(x->op_sym == gensym("+")){
				x->op = otimetag_op_add;
			}else if(x->op_sym == gensym("-")){
				x->op = otimetag_op_subtract;
			}else if(x->op_sym == gensym("*")){
				x->op = otimetag_op_multiply;
			}else if(x->op_sym == gensym("/")){
				x->op = otimetag_op_divide;			
			}else if(x->op_sym == gensym("derivative")){
				x->op = otimetag_op_derivative;
			}else if(x->op_sym == gensym("synchronize")){
				x->op = otimetag_op_synchronize;
			}else if(x->op_sym == gensym("compare")){
				x->op = otimetag_op_compare;
			}
		}else if(attrname == gensym("time")){ 
		}
	}
	return MAX_ERR_NONE;
}
