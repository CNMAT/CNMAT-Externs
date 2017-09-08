/*
Written by John MacCallum and Ville Pulkki, The Center for New Music and Audio Technologies,
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
   NAME: cnmatrix~
   DESCRIPTION: similar to matrix~, but takes jitter matricies
   AUTHORS: John MacCallum, Ville Pulkki
   COPYRIGHT_YEARS: 2009, 1999
   SVN_REVISION: $LastChangedRevision: 1 $
	VERSION 1.0: First version with jitter matrix support and log xfade
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
#define NAME "cnmatrix~"
#define DESCRIPTION "similar to matrix~, but takes jitter matricies"
#define AUTHORS "John MacCallum, Ville Pulkki"
#define COPYRIGHT_YEARS "1999,2009,12,13"

#include "version.h"
#include "ext.h"
#include "ext_obex.h"


#include "z_dsp.h"
#include <math.h>
#include "jit.common.h"
#include "jit.symbols.h"

#define FRAME   2
#define FAST	1
#define SMOOTH  0

t_class *matrix_class;

typedef struct _matrix {
	t_pxobject m_obj;
    void *obex;
	t_int numInlets;   // how many signals in
	t_int numOutlets;  // how many signals out
	t_int version;
	//t_int **w;
	//t_float **Inlets;
	//t_float **Outlets;
	//t_float *coeffLists;   //  panning factors
    double *coeffLists;
	double *lasty;
	double slide;
    int mode;
} t_matrix;

t_symbol *ps_fast, *ps_smooth, *ps_frame;

/* workaround for jitlib.lib failure on windows */

t_symbol *ps_jit_sym_class_jit_matrix, *ps_jit_sym_lock, *ps_jit_sym_getinfo, *ps_jit_sym_getdata, *ps_jit_sym_err_calculate, *ps_jit_sym_char, *ps_jit_sym_long , *ps_jit_sym_float32, *ps_jit_sym_float64;

//t_int *matrix_perform_fast(t_int *w);
//t_int *matrix_perform_smooth(t_int *w);
//t_int *matrix_perform_frame(t_int *w);
//void matrix_dsp(t_matrix *x, t_signal **sp, short *connect);
void matrix_dsp64(t_matrix *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void matrix_fast(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void matrix_smooth(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void matrix_frame(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void matrix_list(t_matrix *x, t_symbol *s, long argc, t_atom *argv);
double mygetfloat(t_atom *a);
double mygetlong(t_atom *a);
void matrix_setgains(t_matrix *x, long in, long out, double gain);
void jit_matrix(t_matrix *x, t_symbol *sym, long argc, t_atom *argv);
void *matrix_new(t_symbol *s, short argc, t_atom *argv);
void matrix_free(t_matrix *x);

void matrix_slide(t_matrix *x, double slide);

int main(void){

	ps_fast    = gensym("fast");
	ps_smooth  = gensym("smooth");
	ps_frame  = gensym("frame");

	ps_jit_sym_class_jit_matrix = gensym("class_jit_matrix");
	ps_jit_sym_lock = gensym("lock");
	ps_jit_sym_getinfo = gensym("getinfo");
	ps_jit_sym_getdata = gensym("getdata");
	ps_jit_sym_err_calculate = gensym("err_calculate");
	ps_jit_sym_char = gensym("char");
	ps_jit_sym_long  = gensym("long");
	ps_jit_sym_float32 = gensym("float32");
	ps_jit_sym_float64 = gensym("float32");

	matrix_class = class_new("cnmatrix~", (method)matrix_new, (method)matrix_free, (short)sizeof(t_matrix), 0L, A_GIMME, 0);

    void *p = max_jit_classex_setup(calcoffset(t_matrix,obex));

    class_addmethod(matrix_class, (method)matrix_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(matrix_class, (method)matrix_fast, "fast", A_GIMME, 0);
	class_addmethod(matrix_class, (method)matrix_smooth, "smooth", A_GIMME, 0);
	class_addmethod(matrix_class, (method)matrix_frame, "frame", A_GIMME, 0);
	class_addmethod(matrix_class, (method)matrix_list, "list", A_GIMME, 0);
	class_addmethod(matrix_class, (method)jit_matrix, "jit_matrix", A_GIMME, 0);
	class_addmethod(matrix_class, (method)matrix_slide, "slide", A_FLOAT, 0);
	class_dspinit(matrix_class);

	version_post_copyright();

    max_jit_classex_standard_wrap(p,NULL,0);

    class_register(CLASS_BOX, matrix_class);
	return 0;
}

void matrix_perform64_fast(t_matrix *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    // fast == no gain factor cross fading -> possible clicks if panning direction
    // changed
    //t_int k, i, j;
    //t_int **w = (t_int **)wAsT_int;
    //t_matrix *x = (t_matrix *)(w[1]);
    //t_int n = (t_int)(w[2]);
    long n = sampleframes;
    // use integers to copy blocs faster: was suggested by David Z.
    //t_float **in = x->Inlets;
    //t_float **out = x->Outlets;
    //t_float *outptr;
    //t_float *inptr;
    double *outptr, *inptr;
    //t_int numInlets = x->numInlets;
    //t_int numOutlets = x->numOutlets;
    double *coeffptr = x->coeffLists;
    double coeff;

    for(int i = 0; i < numouts; i++){
        //memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
        memset(outs[i], 0, n * sizeof(double));
    }

    for(int j = 0; j < (numouts * numins); j++) {
        //outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
        //inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);
        outptr = outs[(j % numouts)];
        inptr = ins[(j / numouts) % numins];
				coeff = *coeffptr++;


        for(int k = 0; k < n; k += 4) {

            *outptr++ += coeff * *inptr++;
            *outptr++ += coeff * *inptr++;
            *outptr++ += coeff * *inptr++;
            *outptr++ += coeff * *inptr++;

        }
    }
}

void matrix_perform64_smooth(t_matrix *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    //int i, j, k;
    //t_matrix *x = (t_matrix *)w[1];
    //int n = (int)w[2];
    long n = sampleframes;
    //float *in, *out;
    //t_int numInlets = x->numInlets;
    //t_int numOutlets = x->numOutlets;
    double coeff;
    double *coeffptr = x->coeffLists;
    double *inptr, *outptr;
    double *lasty = x->lasty;
    double tmp;

    for(int i = 0; i < numouts; i++){
        //memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
        memset(outs[i], 0, n * sizeof(double));
    }

    for(int j = 0; j < (numins * numouts); j++) {
        //outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
        //inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);
        outptr = outs[j % numouts];
        inptr = ins[(j / numouts) % numins];

        coeff = *coeffptr++;

        for(int k = 0; k < n; k += 4) {

            tmp = (*lasty + ((coeff - *lasty) / x->slide));

            *outptr++ += tmp * *inptr++;
            *outptr++ += tmp * *inptr++;
            *outptr++ += tmp * *inptr++;
            *outptr++ += tmp * *inptr++;

            if(fabsf(coeff - *lasty) < 10e-18){
                *lasty = coeff;
            } else{
                *lasty = tmp;
            }

        }
        lasty++;
    }
}

void matrix_perform64_frame(t_matrix *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    //int i, j, k;
    //t_matrix *x = (t_matrix *)w[1];
    //int n = (int)w[2];
    long n = sampleframes;
    //float *in, *out;
    //t_int numInlets = x->numInlets;
    //t_int numOutlets = x->numOutlets;
    double coeff;
    double coeffstep;
    double *coeffptr = x->coeffLists;
    double *inptr, *outptr;
    double *lasty = x->lasty;

    for(int i = 0; i < numouts; i++){
        //memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
        memset(outs[i], 0, n * sizeof(double));
    }

    //x->coeffLists[(in * x->numOutlets) + out] = gain;

    for(int j = 0; j < numouts * numins; j++){
        //outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
        //inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);
        outptr = outs[j % numouts];
        inptr = ins[(j / numouts) % numins];
        coeff = *coeffptr++;
        coeffstep = (coeff - *lasty) / n;
        if(coeffstep != 0.) {
            for(int k = 1; k <= n; k += 1) {
                *outptr++ += (*lasty + k * coeffstep) * *inptr++;
            }
        } else {
            for(int k = 1; k <= n; k += 1) {
                *outptr++ += coeff * *inptr++;
            }
        }
        *lasty = coeff;
        lasty++;
    }
}
/*
t_int *matrix_perform_fast(t_int *wAsT_int) {
	// fast == no gain factor cross fading -> possible clicks if panning direction
	// changed
	t_int k, i, j;
	t_int **w = (t_int **)wAsT_int;
	t_matrix *x = (t_matrix *)(w[1]);
	t_int n = (t_int)(w[2]);
	// use integers to copy blocs faster: was suggested by David Z.
	t_float **in = x->Inlets;
	t_float **out = x->Outlets;
	t_float *outptr;
	t_float *inptr;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	t_float *coeffptr = x->coeffLists;
	t_float coeff;

	for(i = 0; i < numOutlets; i++){
		memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
	}

	for(j = 0; j < (numOutlets * numInlets); j++) {
		outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
		inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);

		coeff = *coeffptr++;

		for(k = 0; k < n; k += 4) {

			*outptr++ += coeff * *inptr++;
			*outptr++ += coeff * *inptr++;
			*outptr++ += coeff * *inptr++;
			*outptr++ += coeff * *inptr++;

		}
	}

	return (wAsT_int+numInlets+numOutlets+3);
}

t_int *matrix_perform_smooth(t_int *w) {
	int i, j, k;
	t_matrix *x = (t_matrix *)w[1];
	int n = (int)w[2];
	float *in, *out;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	float coeff;
	float *coeffptr = x->coeffLists;
	float *inptr, *outptr;
	float *lasty = x->lasty;
	float tmp;

	for(i = 0; i < numOutlets; i++){
		memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
	}

	for(j = 0; j < (numOutlets * numInlets); j++) {
		outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
		inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);

		coeff = *coeffptr++;

		for(k = 0; k < n; k += 4) {

			tmp = (*lasty + ((coeff - *lasty) / x->slide));

			*outptr++ += tmp * *inptr++;
			*outptr++ += tmp * *inptr++;
			*outptr++ += tmp * *inptr++;
			*outptr++ += tmp * *inptr++;

			if(fabsf(coeff - *lasty) < 10e-18){
				*lasty = coeff;
			} else{
				*lasty = tmp;
			}

		}
		lasty++;
	}
    */
	/*
     object_post((t_object *)x, "***************");
     for(i = 0; i < numInlets + numOutlets; i++){
     object_post((t_object *)x, "%d %p", i, w[i + 3]);
     }
     */
	/*
     inptr = (float *)(w[3]);
     for(j = 0; j < numOutlets; j++){
     outptr = (float *)(w[numInlets + j + 3]);
     coeff = *coeffptr++;
     for(k = 0; k < n; k+=4){
     tmp = (*lasty + ((coeff - *lasty) / (x->slide / 4)));
     *outptr++ = tmp * *inptr++;
     *outptr++ = tmp * *inptr++;
     *outptr++ = tmp * *inptr++;
     *outptr++ = tmp * *inptr++;
     if(fabsf(coeff - *lasty) < 10e-18){
     *lasty = coeff;
     }else{
     *lasty = tmp;
     }
     }
     lasty++;
     }

     for(i = 1; i < numInlets; i++){
     inptr = (float *)(w[i + 3]);
     for(j = 0; j < numOutlets; j++){
     outptr = (float *)(w[numInlets + j + 3]);
     coeff = *coeffptr++;
     for(k = 0; k < n; k+=4){
     tmp = (*lasty + ((coeff - *lasty) / (x->slide / 4)));
     *outptr++ += tmp * inptr[i];
     *outptr++ += tmp * inptr[i];
     *outptr++ += tmp * inptr[i];
     *outptr++ += tmp * inptr[i];
     if(fabsf(coeff - *lasty) < 10e-18){
     *lasty = coeff;
     }else{
     *lasty = tmp;
     }
     }
     lasty++;
     }
     }
     */
/*
	return w + numInlets + numOutlets + 3;
}
 */
/*
// this mode does linear interpolation over one signal frame
t_int *matrix_perform_frame(t_int *w) {
	int i, j, k;
	t_matrix *x = (t_matrix *)w[1];
	int n = (int)w[2];
	float *in, *out;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	float coeff;
    float coeffstep;
	float *coeffptr = x->coeffLists;
	float *inptr, *outptr;
	float *lasty = x->lasty;
	float tmp;


	for(i = 0; i < numOutlets; i++){
		memset((float *)(w[numInlets + i + 3]), 0, n * sizeof(float));
	}

    //x->coeffLists[(in * x->numOutlets) + out] = gain;

	for(j = 0; j < numOutlets * numInlets; j++){
		outptr = (float *)(w[(j % numOutlets) + numInlets + 3]);
		inptr = (float *)(w[((j/numOutlets) % numInlets) + 3]);
		coeff = *coeffptr++;
        coeffstep = (coeff - *lasty) / n;
        if(coeffstep != 0.) {
            for(k = 1; k <= n; k += 1) {
                *outptr++ += (*lasty + k * coeffstep) * *inptr++;
            }
        } else {
            for(k = 1; k <= n; k += 1) {
                *outptr++ += coeff * *inptr++;
            }
        }
        *lasty = coeff;
		lasty++;
	}

	return w + numInlets + numOutlets + 3;
}
 */

void matrix_dsp64(t_matrix *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    //t_int vs = maxvectorsize;
    //t_int i, num = x->numInlets + x->numOutlets; // number of Inlets and Outlets
    //t_int **w = x->w;

    //w[0] = (t_int *)x;
    //w[1] = (t_int *)sp[0]->s_n;

    //for (i=0; i<num; ++i)
      //  w[i+2] = (t_int *)sp[i]->s_vec;

    //num += 2; // x and n

    if (x->version == FAST) {
        object_method(dsp64, gensym("dsp_add64"), x, matrix_perform64_fast, 0, NULL);
    } else if (x->version == SMOOTH) {
        object_method(dsp64, gensym("dsp_add64"), x, matrix_perform64_smooth, 0, NULL);
    } else if (x->version == FRAME) {
        object_method(dsp64, gensym("dsp_add64"), x, matrix_perform64_frame, 0, NULL);
    }
}

/*
void matrix_dsp(t_matrix *x, t_signal **sp, short *connect) {
	t_int vs = sys_getblksize();
	t_int i, num = x->numInlets + x->numOutlets; // number of Inlets and Outlets
	t_int **w = x->w;

	w[0] = (t_int *)x;
	w[1] = (t_int *)sp[0]->s_n;

	for (i=0; i<num; ++i)
		w[i+2] = (t_int *)sp[i]->s_vec;

	num += 2; // x and n

	if (x->version == FAST) {
		dsp_addv(matrix_perform_fast, num, (void **)w);
    } else if (x->version == SMOOTH) {
        dsp_addv(matrix_perform_smooth, num, (void **)w);
    } else if (x->version == FRAME) {
        dsp_addv(matrix_perform_frame, num, (void **)w);
    }
}
 */

void matrix_fast(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
	x->version = FAST;
}

void matrix_smooth(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
	x->version = SMOOTH;
}

void matrix_frame(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
	x->version = FRAME;
}

void jit_matrix(t_matrix *x, t_symbol *sym, long argc, t_atom *argv)
{
	void *matrix = NULL;
	long err,dimcount,dim[JIT_MATRIX_MAX_DIMCOUNT]; // YUK
	long i, j, n;
	long rowstride, colstride;
	long in_savelock;
	t_jit_matrix_info in_minfo;
    memset(&in_minfo, '\0', sizeof(t_jit_matrix_info));
	char *in_bp = NULL;
	char *ip = NULL;
	t_atom a_coord[1024];  // YUK
	double *coeffptr,*oldcoeffptr;

	coeffptr = x->coeffLists;
    //post("%s %s", _jit_sym_class_jit_matrix->s_name, ps_jit_sym_class_jit_matrix->s_name);
	if (argc&&argv) // YUK
    {
		//find matrix
		matrix = jit_object_findregistered(jit_atom_getsym(argv));
		if (matrix && jit_object_method(matrix, ps_jit_sym_class_jit_matrix)){
			//calculate
			in_savelock = (long) jit_object_method(matrix, ps_jit_sym_lock, 1);
			jit_object_method(matrix, ps_jit_sym_getinfo, &in_minfo);
			jit_object_method(matrix, ps_jit_sym_getdata, &in_bp);
			if (!in_bp) {
				jit_error_sym(x, ps_jit_sym_err_calculate);
				jit_object_method(matrix, ps_jit_sym_lock, in_savelock);
				return;
			}
			//get dimensions/planecount
			dimcount = in_minfo.dimcount;
			for (i=0;i<dimcount;i++) {
				dim[i] = in_minfo.dim[i];
			}
			//calculate
			n = dim[0];
			rowstride = in_minfo.dimstride[1];
			colstride = in_minfo.dimstride[0];

			if (in_minfo.type==ps_jit_sym_char) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];
					for (i=0;i<dim[1];i++){
						jit_atom_setfloat(&(a_coord[i]), *((uchar *)ip));
						ip += rowstride;
						matrix_setgains(x, j, i, *((float *)ip));
					}
					//matrix_list(x, NULL, dim[1], a_coord);
				}
			} else if (in_minfo.type==ps_jit_sym_long) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];
					for (i=0;i<dim[1];i++){
						jit_atom_setfloat(&(a_coord[i]), *((long *)ip));
						ip += rowstride;
						matrix_setgains(x, j, i, *((float *)ip));
					}
					//matrix_list(x, NULL, dim[1], a_coord);
				}
			} else if (in_minfo.type==ps_jit_sym_float32) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];
					for (i=0;i<dim[1];i++){
						jit_atom_setfloat(&(a_coord[i]), *((float *)ip));
						//post("j = %d, i = %d, f = %f", j, i, *((float *)ip));
						matrix_setgains(x, j, i, *((float *)ip));
						ip += rowstride;
					}
					//matrix_list(x, NULL, dim[1], a_coord);
				}
			} else if (in_minfo.type==ps_jit_sym_float64) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];
					for (i=0;i<dim[1];i++){
						jit_atom_setfloat(&(a_coord[i]), *((double *)ip));
						ip += rowstride;
						matrix_setgains(x, j, i, *((float *)ip));
					}
					//matrix_list(x, NULL, dim[1], a_coord);
				}
			}
			jit_object_method(matrix,ps_jit_sym_lock,in_savelock);
		}
        else {
            post("%p %p %s", matrix, jit_object_method(matrix, ps_jit_sym_class_jit_matrix), jit_atom_getsym(argv)->s_name);
		}
	}
    else
        post("!   (argc&&argv) ");
}

// People forget that you can return structures in C.
// This code is roughlyis the same length as the
// buggy version that follows.
typedef struct
{
    float f;
    bool valid;
} validatedfloat;

validatedfloat mymorebettergetfloat(t_atom *a){
    validatedfloat r; r.valid = true;
	if(a->a_type == A_FLOAT){
		r.f = a->a_w.w_float;
	}else if(a->a_type == A_LONG){
		r.f = (a->a_w.w_long);
	}else{
		error("cnmatrix~: I don't understand %s", a->a_w.w_sym->s_name);
		r.valid = false;
	}
    return r;
}
//YUK: these need a boolean valid flag and pass by reference (See above example)

double mygetfloat(t_atom *a){
	if(a->a_type == A_FLOAT){
		return a->a_w.w_float;
	}else if(a->a_type == A_LONG){
		return (float)(a->a_w.w_long);
	}else{
		error("cnmatrix~: I don't understand %s", a->a_w.w_sym->s_name);
		return 0;
	}
}

double mygetlong(t_atom *a){
	if(a->a_type == A_FLOAT){
		return (long)(a->a_w.w_float);
	}else if(a->a_type == A_LONG){
		return a->a_w.w_long;
	}else{
		error("cnmatrix~: I don't understand %s", a->a_w.w_sym->s_name);
		return 0;
	}
}

void matrix_list(t_matrix *x, t_symbol *s, long argc, t_atom *argv) {
	//int i;
	//t_float *coeffptr;
	//int in, out;
	//float val;
    //YUK:  these values need to be sanitized before they are used
	if(argc == 2){
		matrix_setgains(x, x->m_obj.z_in, mygetlong(argv), mygetfloat(argv + 1));
	}else if(argc == 3){
		matrix_setgains(x, mygetlong(argv), mygetlong(argv + 1), mygetfloat(argv + 2));
	}

	/*
	if(argc != x->numOutlets){
		object_post((t_object *)x, "matrix~: Wrong number of panning coefficients! Should be %d.",x->numOutlets);
	} else {
		// new values are picked
		coeffptr= x->coeffLists + x->m_obj.z_in*x->numOutlets;
		for(i=0; i < x->numOutlets; i++){
			//post("list %d", x->m_obj.z_in * x->numOutlets + i);

			if(argv[i].a_type != A_FLOAT ){
				object_post((t_object *)x, "matrix~: Panning coefficients must be floats!",0);
				return;
			}
			if(argv[i].a_w.w_float > 10.0 || argv[i].a_w.w_float < -10.0){
				object_post((t_object *)x, "matrix~: Too large pan coeff magnitude, not changed!",0);
				coeffptr++;
			} else {
				*coeffptr++ = argv[i].a_w.w_float;
			}
		}
	}
	*/
}

// YUK, no defensiveness here at all.
void matrix_setgains(t_matrix *x, long in, long out, double gain){
	//post("%d %d %d %f", in, out, (in * x->numOutlets) + out, gain);
	//if(
	x->coeffLists[(in * x->numOutlets) + out] = gain;
}

void *matrix_new(t_symbol *s, short argc, t_atom *argv) {
	//t_int i, k;
	//t_int vs = sys_getblksize(); // Size of signal vector selected in MSP
	t_matrix *x = (t_matrix *)object_alloc(matrix_class);
	if(!x){
		post("bailing");
		return NULL;
	}

	//t_float Fs = sys_getsr();
	//t_float *fptr,*gptr, *nptr;
	// Look at 1st argument
	if (argv[0].a_type == A_LONG) x->numInlets = argv[0].a_w.w_long;
	else if (argv[0].a_type == A_FLOAT) x->numInlets = (int)argv[0].a_w.w_float;
	else x->numInlets = 2; // Just a default value

	// Look at 2nd argument
	if (argv[1].a_type == A_LONG) x->numOutlets = argv[1].a_w.w_long;
	else if (argv[1].a_type == A_FLOAT) x->numOutlets = (int)argv[1].a_w.w_float;
	else x->numOutlets = 2; // Just a default value

	// Look at 3rd argument
	if (argv[2].a_w.w_sym == ps_fast) {
		x->version = FAST;
    } else if(argv[2].a_w.w_sym == ps_frame) {
        x->version = FRAME;
    } else {
        x->version = SMOOTH; // smooth is default
    }

	//x->Inlets = malloc(x->numInlets * sizeof(t_float*));
	//x->Outlets = malloc(x->numOutlets * sizeof(t_float*));
	//x->w = malloc((x->numInlets + x->numOutlets + 2) * sizeof(t_int*));

	// Create inlets and outlets
	dsp_setup((t_pxobject *)x, x->numInlets);
	x->m_obj.z_misc = Z_NO_INPLACE; // Necessary when outlets should have different vectors than inlets !!
	for (int i=0; i<x->numOutlets; ++i)
		outlet_new((t_object *)x, "signal");
	x->coeffLists = (double *) calloc(x->numInlets * x->numOutlets, sizeof(double));

	x->lasty = (double *)calloc(x->numInlets * x->numOutlets, sizeof(double));

	x->slide = 1000;
	return x;
}

void  matrix_free(t_matrix *x) {

    dsp_free((t_pxobject *)x);
    /*
	if (x->Inlets){
		free(x->Inlets);
	}
	if (x->Outlets){
		free(x->Outlets);
	}
	if (x->w){
		free(x->w);
	}
     */
	if(x->coeffLists){
		free(x->coeffLists);
	}

    if(x->lasty) {
        free(x->lasty);
    }

}

void matrix_slide(t_matrix *x, double slide){
	x->slide = slide;
}
