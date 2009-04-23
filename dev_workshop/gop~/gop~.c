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
NAME: gop~
DESCRIPTION: Grains of Paradise (a granulator)
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

/*
todo:

--attributes for all parameters.
--multiple outlets/channels.
--different spat options (random, permutations, sequential, etc)
--random start pos within a block
--sample-synchronous perform routine
--busy map
--read from buffer
--different (non-random) grain start times from source material
--resize buffer (restart dsp)
--do everything in floating point (get rid of ints)
 */

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "z_dsp.h"

#define GOP_MAX_CHANNELS 8
#define GOP_MAX_GRAINS 256
#define GOP_BUFFER_LENGTH 44100

typedef struct _grain{
	int grain_length_samp;
	int grain_pos;
	int channel;
	int buffer_start_samp;
	int buffer_pos_samp;
	int buffer_end_samp;
} t_grain;

typedef struct _gop{
	t_pxobject ob;
	t_int **w;
	float *buffer;
	int buffer_length;
	int buffer_pos;
	t_grain *grains;
	int *active_grains;
	float grain_length_samp;
	int num_channels;
	t_symbol *spat_mode;
	int channel_order;
} t_gop;

t_symbol *ps_gop_random, *ps_gop_cycle, *ps_gop_permute;

static t_class *gop_class;

void gop_dsp(t_gop *x, t_signal **sp, short *count);
t_int *gop_perform(t_int *w);
t_int *gop_perform_connected(t_int *w);
float gop_triangle_window(float window_length, float pos);
int gop_get_channel(t_gop *x);
void gop_bang(t_gop *x);
void gop_make_new_grain(t_gop *x);
void gop_initgrain(t_gop *x, t_grain *grain);
void gop_grain_length(t_gop *x, int len);
void gop_int(t_gop *x, long n);
void gop_float(t_gop *x, double xx);
void gop_list(t_gop *x, t_symbol *msg, short argc, t_atom *argv);
void gop_anything(t_gop *x, t_symbol *msg, short argc, t_atom *argv);
void gop_free(t_gop *x);
void gop_assist(t_gop *x, void *b, long m, long a, char *s);
void gop_inletinfo(t_gop *x, void *b, long index, char *t);
float gop_rand(float min, float max);
int gop_ms2samp(float ms, float sr);
float gop_samp2ms(float samps, float sr);
void *gop_new(t_symbol *sym, short argc, t_atom *argv);
t_max_err gop_set_grain_length(t_gop *x, t_object *attr, long argc, t_atom *argv);
t_max_err gop_get_grain_length(t_gop *x, t_object *attr, long *argc, t_atom **argv);

void gop_dsp(t_gop *x, t_signal **sp, short *count){
	int i;
	t_int **w = x->w;
	w[0] = (t_int *)x;
	w[1] = (t_int *)sp[0]->s_n;
	w[2] = (t_int *)sp[0]->s_vec;
	if(count[1]){
		w[3] = (t_int *)sp[1]->s_vec;
		for(i = 0; i < x->num_channels; i++){
			w[4 + i] = (t_int *)sp[2 + i]->s_vec;
		}

		dsp_addv(gop_perform_connected, 4 + x->num_channels, (void **)w);
	}else{
		for(i = 0; i < x->num_channels; i++){
			w[3 + i] = (t_int *)sp[2 + i]->s_vec;
		}

		dsp_addv(gop_perform, 3 + x->num_channels, (void **)w);
	}
	
	/*
	if(count[1]){
		dsp_add(gop_perform_connected, 
			5, 
			x, 
			sp[0]->s_vec, 
			sp[1]->s_vec, 
			sp[2]->s_vec, 
			sp[0]->s_n);
	}else{
		dsp_add(gop_perform, 4, x, sp[0]->s_vec, sp[2]->s_vec, sp[0]->s_n);
	}
	*/
}

t_int *gop_perform(t_int *w){
	t_float *in;
	int n, i, j, grain_num;

	t_gop *x = (t_gop *)w[1];
	n = (int)w[2];
	in = (t_float *)w[3];
	//out = (t_float *)w[3];

	for(i = 0; i < n; i++){
		// add the current input vector to our buffer.
		x->buffer[x->buffer_pos] = in[i];
		x->buffer_pos = (x->buffer_pos + 1) % x->buffer_length;
		// zero the output vector as we go
		for(j = 0; j < x->num_channels; j++){
			((t_float *)w[4 + j])[i] = 0.;
		}
		// iterate through the active grains
		for(grain_num = 0; grain_num < GOP_MAX_GRAINS; grain_num++){
			if(x->active_grains[grain_num] == 1){
				t_grain *g = &(x->grains[grain_num]);
				((t_float *)w[4 + g->channel])[i] += gop_triangle_window((float)(g->grain_length_samp), (int)(g->grain_pos)) * x->buffer[g->buffer_pos_samp];
				if(g->buffer_pos_samp == g->buffer_end_samp){
					x->active_grains[grain_num] = 0;
				}else{
					g->buffer_pos_samp++;
					g->buffer_pos_samp %= x->buffer_length;
					g->grain_pos++;
				}
			}
		}
	}

	return w + 3 + x->num_channels + 1;

}

t_int *gop_perform_connected(t_int *w){
	t_float *in1, *in2;
	int n, i, j, grain_num;

	t_gop *x = (t_gop *)w[1];
	n = (int)w[2];
	in1 = (t_float *)w[3];
	in2 = (t_float *)w[4];

	for(i = 0; i < n; i++){
		// add the current input vector to our buffer.
		x->buffer[x->buffer_pos] = in1[i];
		x->buffer_pos = (x->buffer_pos + 1) % x->buffer_length;
		// zero the output vector as we go
		for(j = 0; j < x->num_channels; j++){
			((t_float *)w[j + 5])[i] = 0.;
		}
		// iterate through the active grains

		if(in2[i] > 0){
			gop_make_new_grain(x);
		}
		for(grain_num = 0; grain_num < GOP_MAX_GRAINS; grain_num++){
			if(x->active_grains[grain_num] == 1){
				t_grain *g = &(x->grains[grain_num]);
				/*
				if(i == 0){
					post("%f %f %f, %f", gop_triangle_window((float)(g->grain_length_samp),fabsf((float)(g->buffer_pos_samp - g->buffer_start_samp))) * x->buffer[g->buffer_pos_samp], 
					     (float)(g->grain_length_samp), 
					     (float)(g->buffer_pos_samp - g->buffer_start_samp), 
					     x->buffer[g->buffer_pos_samp]);
				}
				*/
				((t_float *)w[5 + g->channel])[i] += gop_triangle_window((float)(g->grain_length_samp), (int)(g->grain_pos)) * x->buffer[g->buffer_pos_samp];
				if(g->buffer_pos_samp == g->buffer_end_samp){
					x->active_grains[grain_num] = 0;
				}else{
					g->buffer_pos_samp++;
					g->buffer_pos_samp %= x->buffer_length;
					g->grain_pos++;
				}
			}
		}
	}

	return w + 4 + x->num_channels + 1;

}

float gop_triangle_window(float window_length, float pos){
	float y = pos / (window_length / 2);
	if(pos < (window_length / 2)){
		return y;
	}else{
		return 2. - y;
	}
}

int gop_get_channel(t_gop *x){
	if(x->spat_mode == ps_gop_random){
		return (int)gop_rand(0, x->num_channels);
	}else if(x->spat_mode == ps_gop_cycle){
		return 0;
	}else if(x->spat_mode == ps_gop_permute){
		return 0;
	}else{
		return 0;
	}
}

void gop_bang(t_gop *x){
	gop_make_new_grain(x);
}

void gop_make_new_grain(t_gop *x){
	int i = 0;
	while(x->active_grains[i] == 1 && i < GOP_MAX_GRAINS){
		i++;
	}
	if(i == GOP_MAX_GRAINS - 1 && x->active_grains[i] == 1){
		return;
	}
	// important to init the grain before announcing that it is ready
	gop_initgrain(x, &(x->grains[i]));
	x->active_grains[i] = 1;
	//post("grain = %d", i);
}

void gop_initgrain(t_gop *x, t_grain *grain){
	grain->grain_length_samp = x->grain_length_samp;
	grain->grain_pos = 0;
	grain->channel = gop_get_channel(x);
	grain->buffer_start_samp = (int)gop_rand(0, x->buffer_length - sys_getblksize());
	//post("%d", grain->buffer_start_samp);
	grain->buffer_start_samp += x->buffer_pos + sys_getblksize();
	//post("%d", grain->buffer_start_samp);
	grain->buffer_start_samp %= x->buffer_length;
	//post("%d", grain->buffer_start_samp);
	grain->buffer_pos_samp = grain->buffer_start_samp;
	grain->buffer_end_samp = (grain->buffer_start_samp + grain->grain_length_samp) % x->buffer_length;
	/*
	post("grain length = %d", grain->grain_length_samp);
	post("channel = %d", grain->channel);
	post("buffer start = %d", grain->buffer_start_samp);
	post("buffer pos = %d", grain->buffer_pos_samp);
	post("buffer end = %d", grain->buffer_end_samp);
	*/
}

void gop_grain_length(t_gop *x, int len){
	if(len >= x->buffer_length - sys_getblksize()){
		error("gop~: grain_length must be less than %d.", x->buffer_length - sys_getblksize());
	}
	x->grain_length_samp = (float)gop_ms2samp((float)len, (float)sys_getsr());
}

void gop_int(t_gop *x, long n){

}

void gop_float(t_gop *x, double f){

}

void gop_list(t_gop *x, t_symbol *msg, short argc, t_atom *argv){

}

void gop_anything(t_gop *x, t_symbol *msg, short argc, t_atom *argv){

}

void gop_free(t_gop *x){
	// dsp_free() should be first.
	dsp_free((t_pxobject *)x);
	// free any memory that we've allocated with [cm]alloc
	free(x->buffer);
	free(x->grains);
	free(x->active_grains);
}

void gop_assist(t_gop *x, void *b, long io, long index, char *s){
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

float gop_rand(float min, float max){
	float mmin, mmax;
	if(min < max){
		mmin = min;
		mmax = max;
	}else{
		mmin = max;
		mmax = min;
	}
	float r = ((float)rand() / RAND_MAX);
	r = (r * (mmax - mmin)) + mmin;
	return r;
}

int gop_ms2samp(float ms, float sr){
	return (int)(floorf((ms / 1000.) * sr));
}

float gop_samp2ms(float samps, float sr){
	return roundf((samps / sr) * 1000.);
}

/**************************************************
Object and instance creation functions.
 **************************************************/

void *gop_new(t_symbol *sym, short argc, t_atom *argv){
	t_gop *x;

	if(x = (t_gop *)object_alloc(gop_class)){
		dsp_setup((t_pxobject *)x, 2);
		x->ob.z_misc = Z_NO_INPLACE;
		//x->outlet = outlet_new(x, "signal");


		x->buffer_length = GOP_BUFFER_LENGTH;
		x->buffer_pos = 0;
		x->grains = (t_grain *)malloc(GOP_MAX_GRAINS * sizeof(t_grain));
		x->active_grains = (int *)calloc(GOP_MAX_GRAINS, sizeof(int));
		x->grain_length_samp = gop_ms2samp(100., 44100.);
		x->num_channels = 1;

		attr_args_process(x, argc, argv);
		x->buffer = (float *)calloc(GOP_BUFFER_LENGTH, sizeof(float));
		int i;
		for(i = 0; i < x->num_channels; i++){
			outlet_new(x, "signal");
		}
		// 2 inlets, num_channels, our object, and the vector size
		x->w = malloc((2 + x->num_channels + 2) * sizeof(t_int*));

		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("gop~", (method)gop_new, (method)gop_free, sizeof(t_gop), 0L, A_GIMME, 0);
	class_dspinit(c);
	common_symbols_init();

	CLASS_ATTR_LONG(c, "num_channels", 0, t_gop, num_channels);
	CLASS_ATTR_INVISIBLE(c, "num_channels", 0);

	CLASS_ATTR_FLOAT(c, "grain_length", 0, t_gop, grain_length_samp);
	CLASS_ATTR_ACCESSORS(c, "grain_length", (method)gop_get_grain_length, (method)gop_set_grain_length);

	CLASS_ATTR_SYM(c, "spat_mode", 0, t_gop, spat_mode);

	class_addmethod(c, (method)gop_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)gop_bang, "bang", 0);

	// if you have an attribute declared, you must not also create a message
	// handler that responds to the same name or it will override the attributes
	// setter method.
	//class_addmethod(c, (method)gop_grain_length, "grain_length", A_DEFLONG, 0);

	class_addmethod(c, (method)gop_int, "int", A_LONG, 0);
	class_addmethod(c, (method)gop_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)gop_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)gop_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)gop_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);

	ps_gop_cycle = gensym("cycle");
	ps_gop_random = gensym("random");
	ps_gop_permute = gensym("permute");

	class_register(CLASS_BOX, c);
	gop_class = c;

	return 0;
}

t_max_err gop_set_grain_length(t_gop *x, t_object *attr, long argc, t_atom *argv){
	float len = atom_getfloat(argv);
	if(len < 0){
		error("gop: grain length must be greater than 0");
		return 0;
	}
	x->grain_length_samp = gop_ms2samp(len, (float)(sys_getsr()));
	return 0;
}

t_max_err gop_get_grain_length(t_gop *x, t_object *attr, long *argc, t_atom **argv){
	char alloc;

	atom_alloc(argc, argv, &alloc);
	atom_setfloat(*argv, gop_samp2ms(x->grain_length_samp, (float)(sys_getsr())));
	return 0;
}
