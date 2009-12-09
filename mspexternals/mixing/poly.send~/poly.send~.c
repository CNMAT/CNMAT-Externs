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
NAME: poly.send~
DESCRIPTION: Multichannel, dynamically assignable output for poly~
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "z_dsp.h"
#include "version.c"

typedef struct _psend{
	t_pxobject ob;
	t_symbol *name, *mangled_name;
	long channel;
	t_float *sv;
	long blksize;
	long samplerate;
} t_psend;

static t_class *psend_class;

void psend_dsp(t_psend *x, t_signal **sp, short *count);
t_int *psend_perform(t_int *w);
void psend_mangle(t_psend *x);
void psend_free(t_psend *x);
void psend_assist(t_psend *x, void *b, long m, long a, char *s);
t_max_err psend_setname(t_psend *x, t_object *attr, long argc, t_atom *argv);
t_max_err psend_getname(t_psend *x, t_object *attr, long *argc, t_atom **argv);
t_max_err psend_setchannel(t_psend *x, t_object *attr, long argc, t_atom *argv);
t_max_err psend_getchannel(t_psend *x, t_object *attr, long *argc, t_atom **argv);
void *psend_new(t_symbol *sym, int argc, t_atom *argv);

void psend_dsp(t_psend *x, t_signal **sp, short *count){
	x->sv = sp[0]->s_vec;
	x->blksize = sp[0]->s_n;
	x->samplerate = sp[0]->s_sr;
	dsp_add(psend_perform, 1, x);
}

t_int *psend_perform(t_int *w){
	t_psend *x = (t_psend *)w[1];
	if(!(x->mangled_name->s_thing)){
		return w + 2;
	}
	int i;
	t_float *ptr = (t_float *)(x->mangled_name->s_thing);
	for(i = 0; i < x->blksize; i++){
		ptr[i] += x->sv[i];
	}
	return w + 2;
}

void psend_mangle(t_psend *x){
	if(x->name){
		char buf[256];
		sprintf(buf, "%s_%ld", x->name->s_name, x->channel);
		x->mangled_name = gensym(buf);
	}
}

void psend_free(t_psend *x){
	dsp_free((t_pxobject *)x);
}

void psend_assist(t_psend *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	}
}

t_max_err psend_setname(t_psend *x, t_object *attr, long argc, t_atom *argv){
	if(argc == 0){
		return 0;
	}

	x->name = atom_getsym(argv);
	psend_mangle(x);
	return 0;
}

t_max_err psend_getname(t_psend *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setsym(*argv, x->name);
	return 0;
}

t_max_err psend_setchannel(t_psend *x, t_object *attr, long argc, t_atom *argv){
	if(argc == 0){
		return 0;
	}

	x->channel = atom_getlong(argv);
	psend_mangle(x);
	return 0;
}

t_max_err psend_getchannel(t_psend *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;
	atom_alloc(argc, argv, &alloc);
	atom_setlong(*argv, x->channel);
	return 0;
}

void *psend_new(t_symbol *sym, int argc, t_atom *argv){
	t_psend *x;

	if(x = (t_psend *)object_alloc(psend_class)){
		dsp_setup((t_pxobject *)x, 1);
        	x->ob.z_misc = Z_NO_INPLACE;
		x->name = NULL;
		attr_args_process(x, argc, argv);
		psend_mangle(x);
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("poly.send~", (method)psend_new, (method)psend_free, sizeof(t_psend), 0L, A_GIMME, 0);
	class_dspinit(c);

	class_addmethod(c, (method)psend_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)psend_assist, "assist", A_CANT, 0);

	CLASS_ATTR_SYM(c, "name", 0, t_psend, name);
	CLASS_ATTR_ACCESSORS(c, "name", psend_getname, psend_setname);

	CLASS_ATTR_LONG(c, "channel", 0, t_psend, channel);
	CLASS_ATTR_ACCESSORS(c, "channel", psend_getchannel, psend_setchannel);

	class_register(CLASS_BOX, c);
	psend_class = c;

	common_symbols_init();
	version(0);
	return 0;
}
