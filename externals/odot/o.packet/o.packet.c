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
NAME: o.packet
DESCRIPTION: Store an OSC packet 
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

typedef struct _opacket{
	t_object ob;
	void *outlet;
	void *proxy;
	long inlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
} t_opacket;

void *opacket_class;

void opacket_fullPacket(t_opacket *x, long len, long ptr);
void opacket_output_bundle(t_opacket *x, long len, char *buf);
void opacket_now(t_opacket *x);
void opacket_immediate(t_opacket *x);
void opacket_future(t_opacket *x, t_symbol *msg, int argc, t_atom *argv);
void opacket_timetag(t_opacket *x, long l1, long l2);
void opacket_anything(t_opacket *x, t_symbol *msg, short argc, t_atom *argv);
void opacket_int(t_opacket *x, long l);
void opacket_float(t_opacket *x, double f);
void opacket_free(t_opacket *x);
void opacket_assist(t_opacket *x, void *b, long m, long a, char *s);
void opacket_bang(t_opacket *x);
void opacket_clear(t_opacket *x);
void *opacket_new(t_symbol *msg, short argc, t_atom *argv);

void opacket_fullPacket(t_opacket *x, long len, long ptr){
	long n = len;
	char buf[n * 2];
	memcpy(buf, (char *)ptr, n);
	if(strncmp((char *)ptr, "#bundle\0", 8)){
		if((n = cmmjl_osc_bundle_naked_message(len, buf, buf)) < 0){
			error("problem bundling naked message");
			return;
		}
	}

	// flatten any nested bundles
	cmmjl_osc_flatten(n, buf, buf);

	if(x->buffer_len > n){
		char *tmp = realloc(x->buffer, sizeof(char) * (n * 2));
		if(tmp){
			x->buffer = tmp;
			x->buffer_len = n * 2;
		}else{
			error("o.packet: out of memory!");
			return;
		}
	}
	memcpy(x->buffer, buf, n * sizeof(char));
	x->buffer_pos = n;

	if(proxy_getinlet((t_object *)x) == 0){
		opacket_output_bundle(x, x->buffer_pos, x->buffer);
	}
}

void opacket_output_bundle(t_opacket *x, long len, char *buf){
	t_atom out[2];
	atom_setlong(&(out[0]), len);
	atom_setlong(&(out[1]), (long)buf);
	outlet_anything(x->outlet, ps_FullPacket, 2, out);
}

void opacket_now(t_opacket *x){
	if(!(x->buffer_pos) || proxy_getinlet((t_object *)x) != 0){
		return;
	}
	long len = x->buffer_pos;
	char buf[len];
	memcpy(buf, x->buffer, len);
	ntptime now;
	cmmjl_osc_timetag_now_to_ntp(&now);
	cmmjl_osc_timetag_set(len, (long)buf, &now);
	opacket_output_bundle(x, len, buf);
}

void opacket_immediate(t_opacket *x){
	if(!(x->buffer_pos) || proxy_getinlet((t_object *)x) != 0){
		return;
	}
	long len = x->buffer_pos;
	char buf[len];
	cmmjl_osc_init_bundle(len, buf, NULL);
	memcpy(buf + 16, x->buffer + 16, len - 16);
	opacket_output_bundle(x, len, buf);
}

void opacket_future(t_opacket *x, t_symbol *msg, int argc, t_atom *argv){
	if(!(x->buffer_pos) || proxy_getinlet((t_object *)x) != 0){
		return;
	}
	if(!argc){
		return;
	}
	double del = atom_getfloat(argv);

	long len = x->buffer_pos;
	char buf[len];
	memcpy(buf, x->buffer, len);

	ntptime now, ndel, nout;
	cmmjl_osc_timetag_now_to_ntp(&now);
	cmmjl_osc_timetag_float_to_ntp(del / 1000., &ndel);
	cmmjl_osc_timetag_add(&now, &ndel, &nout);
	cmmjl_osc_timetag_set(len, (long)buf, &nout);
	opacket_output_bundle(x, len, buf);
}

void opacket_timetag(t_opacket *x, long l1, long l2){
	if(!(x->buffer_pos) || proxy_getinlet((t_object *)x) != 0){
		return;
	}
	char tt[8];
	*(unsigned long *)(tt) = htonl(l1);
	*(unsigned long *)(tt + 4) = htonl(l2);
	long len = x->buffer_pos;
	char buf[len];
	cmmjl_osc_init_bundle(len, buf, tt);
	memcpy(buf + 16, x->buffer + 16, len - 16);
	opacket_output_bundle(x, len, buf);
}

void opacket_anything(t_opacket *x, t_symbol *msg, short argc, t_atom *argv){
	if(!msg){
		error("o.delay: not a properly formatted OSC-style message");
		return;
	}
	long ac = argc + 1;
	t_atom av[ac];
	atom_setsym(av, msg);
	memcpy(av + 1, argv, argc * sizeof(t_atom));
	ntptime now;
	cmmjl_osc_timetag_now_to_ntp(&now);
	char tt[8];
	*(unsigned long *)(tt + 4) = htonl(now.sec);
	*(unsigned long *)(tt) = htonl(now.frac_sec);
	cmmjl_osc_init_bundle(x->buffer_len, x->buffer, tt);
	x->buffer_pos = cmmjl_osc_make_bundle_from_atoms(ac, av, &(x->buffer_len), x->buffer + 16) + 16;
	if(proxy_getinlet((t_object *)x) == 0){
		opacket_output_bundle(x, x->buffer_pos, x->buffer);
	}
}

void opacket_assist(t_opacket *x, void *b, long m, long a, char *s){
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

void opacket_bang(t_opacket *x){
	if(x->buffer_pos){
		opacket_output_bundle(x, x->buffer_pos, x->buffer);
	}
}

void opacket_clear(t_opacket *x){
}

void opacket_free(t_opacket *x){
	object_free(x->proxy);
}

void *opacket_new(t_symbol *msg, short argc, t_atom *argv){
	t_opacket *x;
	int i;
	if(x = (t_opacket *)object_alloc(opacket_class)){
		cmmjl_init(x, NAME, 0);
		x->outlet = outlet_new(x, "FullPacket");
		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));
		x->buffer_len = 2048;
		if(argc){
			x->buffer_len = atom_getlong(argv);
		}
		x->buffer = (char *)calloc(x->buffer_len, sizeof(char));
		x->buffer_pos = 0;
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.packet", (method)opacket_new, (method)opacket_free, sizeof(t_opacket), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)opacket_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)opacket_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)opacket_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)opacket_clear, "clear", 0);
	class_addmethod(c, (method)opacket_bang, "bang", 0);
	class_addmethod(c, (method)opacket_anything, "anything", A_GIMME, 0);

	class_addmethod(c, (method)opacket_now, "now", 0);
	class_addmethod(c, (method)opacket_immediate, "immediate", 0);
	class_addmethod(c, (method)opacket_future, "future", A_GIMME, 0);
	class_addmethod(c, (method)opacket_timetag, "OSCTimeTag", A_LONG, A_LONG, 0);

    
	class_register(CLASS_BOX, c);
	opacket_class = c;

	common_symbols_init();
	return 0;
}
