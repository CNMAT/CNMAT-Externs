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
NAME: poly.bus~
DESCRIPTION: Multichannel, dynamically assignable receive for poly~
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 0.0.1: New help file
VERSION 0.1: Fixed a memory leak and made it so that you can safely have 2 of them running at once
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
#define NAME "poly.bus~"
#define DESCRIPTION "Multichannel, dynamically assignable receive for poly~"
#define AUTHORS "John MacCallum"
#define COPYRIGHT_YEARS "2009,2012"


#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "z_dsp.h"


#ifdef WIN32
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

typedef struct _pbus{
	t_pxobject ob;
	t_symbol *name, **mangled_names;
	long num_channels;
	double **sv;
	long blksize;
	long samplerate;
	long my_id;
} t_pbus;

static t_class *pbus_class;

void pbus_dsp(t_pbus *x, t_signal **sp, short *count);
t_int *pbus_perform(t_int *w);
void pbus_mangle(t_pbus *x);
void pbus_free(t_pbus *x);
void pbus_assist(t_pbus *x, void *b, long m, long a, char *s);
void *pbus_new(t_symbol *sym, int argc, t_atom *argv);

void pbus_dsp(t_pbus *x, t_signal **sp, short *count){
	int i;
	for(i = 0; i < x->num_channels; i++){
		x->sv[i] = sp[i + 1]->s_vec;
		if(x->blksize != sp[0]->s_n){
			// block size has changed
			// if there are multiple instances of this object, this will happen more than once, but
			// it's cool because (from the gcc manual):
			// If the new size you specify is the same as the old size, realloc is guaranteed to change
			// nothing and return the same address that you gave.

			void *ptr = realloc(x->mangled_names[i]->s_thing, sp[0]->s_n * sizeof(t_float));
			if(ptr){
				x->mangled_names[i]->s_thing = ptr;
			}else{
				object_error((t_object *)x, "out of memory!");
			}
		}
		memset(x->mangled_names[i]->s_thing, '\0', sizeof(t_float) * sp[0]->s_n);
	}
	x->blksize = sp[0]->s_n;
	long id = (long)(x->name->s_thing);
	x->my_id = ++id;
	x->name->s_thing = (void *)id;
	x->samplerate = sp[0]->s_sr;
	dsp_add(pbus_perform, 1, x);
}

t_int *pbus_perform(t_int *w){
	t_pbus *x = (t_pbus *)w[1];
	int i;
	for(i = 0; i < x->num_channels; i++){
		if(x->mangled_names[i]->s_thing){
			memcpy(x->sv[i], x->mangled_names[i]->s_thing, sizeof(t_float) * x->blksize);
		}
		if(x->my_id == (long)(x->name->s_thing)){
			memset(x->mangled_names[i]->s_thing, '\0', sizeof(t_float) * x->blksize);
		}
	}
	return w + 2;
}

void pbus_mangle(t_pbus *x){
	if(!(x->name) || x->num_channels < 0){
		return;
	}
	char buf[256];
	int i;
	for(i = 0; i < x->num_channels; i++){
		sprintf(buf, "%s_%d", x->name->s_name, i + 1);
		x->mangled_names[i] = gensym(buf);
	}
}

void pbus_free(t_pbus *x){
	dsp_free((t_pxobject *)x);
	int i;
	x->name->s_thing = x->name->s_thing - 1;
	if(x->name->s_thing == 0){
		for(i = 0; i < x->num_channels; i++){
			if(x->mangled_names[i]->s_thing){
				free(x->mangled_names[i]->s_thing);
			}
		}
	}
}

void pbus_assist(t_pbus *x, void *b, long io, long index, char *s){
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

void *pbus_new(t_symbol *sym, int argc, t_atom *argv){
	t_pbus *x;

	if((x = (t_pbus *)object_alloc(pbus_class))){
        if(argc < 2){
            object_error((t_object *)x, "2 arguments required: name and number of channels");
            return NULL;
        }
		dsp_setup((t_pxobject *)x, 1);
        	x->ob.z_misc = Z_NO_INPLACE;
		x->name = atom_getsym(argv);
		x->name->s_thing = x->name->s_thing + 1;
		x->num_channels = atom_getlong(argv + 1);
		if(x->num_channels <= 0){
			object_error((t_object *)x, "poly.bus~: number of channels must be >0");
			return NULL;
		}
		x->mangled_names = (t_symbol **)calloc(x->num_channels, sizeof(t_symbol *));
		x->sv = (double **)calloc(x->num_channels, sizeof(double *));
		int i;
		pbus_mangle(x);
		for(i = 0; i < x->num_channels; i++){
			outlet_new(x, "signal");
		}
		x->blksize = 0;
		x->my_id = 0;

		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("poly.bus~", (method)pbus_new, (method)pbus_free, sizeof(t_pbus), 0L, A_GIMME, 0);
	class_dspinit(c);

	class_addmethod(c, (method)pbus_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)pbus_assist, "assist", A_CANT, 0);

	class_register(CLASS_BOX, c);
	pbus_class = c;

	common_symbols_init();
	version_post_copyright();
	
	class_register(CLASS_BOX, pbus_class);
	return 0;
}

