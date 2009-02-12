/* matrix~.c

DESCRIPTION: Ville Pulkki's matrix mixer, superseded by MSP's built-in matrix~ object.

written by Ville Pulkki 1999
Helsinki University of Technology 
and 
Unversity of California at Berkeley

See copyright in file with name COPYRIGHT  

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: matrix~
DESCRIPTION: just like matrix~, but takes jitter matricies
AUTHORS: Ville Pulkki, John MacCallum
COPYRIGHT_YEARS: 1999, 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 1.0: Original code with the jitter matrix addition
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/


#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include <math.h>
#include "jit.common.h"

#define FAST	1
#define SMOOTH  0
#define DEF_UPD_INTERVAL 6400

void *matrix_class;

typedef struct _matrix {
	t_pxobject m_obj;
	t_int numInlets;   // how many signals in 
	t_int numOutlets;  // how many signals out
	t_int version;
    t_int **w;			
    t_float **Inlets;			
    t_float **Outlets;	 
    t_float *coeffLists;   //  panning factors		
    t_float *oldcoeffLists; // previous panning factors
    t_float *nextcoeffLists; // next panning factors
    t_int counter; 			  // used in gain factor cross fading
    t_int upd_interval;		   // how often gain factors are changed
} t_matrix;

t_symbol *ps_fast, *ps_smooth;

t_int *matrix_perform_fast(t_int *w);
t_int *matrix_perform_smooth(t_int *w);
void matrix_dsp(t_matrix *x, t_signal **sp, short *connect);
void matrix_fast(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void matrix_smooth(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void matrix_list(t_matrix *x, t_symbol *s, short argc, t_atom *argv);
void jit_matrix(t_matrix *x, t_symbol *sym, long argc, t_atom *argv);
void *matrix_new(t_symbol *s, short argc, t_atom *argv);
void matrix_free(t_matrix *x);

void main(void) {
	
	ps_fast    = gensym("fast");
	ps_smooth  = gensym("smooth");

	setup( &matrix_class, matrix_new, (method)matrix_free, (short)sizeof(t_matrix), 0L, A_GIMME, 0);
		
	addmess((method)matrix_dsp, "dsp", A_CANT, 0);
	addmess((method)matrix_fast, "fast", A_GIMME, 0);
	addmess((method)matrix_smooth, "smooth", A_GIMME, 0);
	addmess((method)matrix_list, "list", A_GIMME, 0);
	addmess((method)jit_matrix, "jit_matrix", A_GIMME, 0);
	dsp_initclass();

	version(0);
}

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
	t_float *coeffptr;
	t_float coeff; 
	
	// Inlets and Outlets
	for (k=0; k<numInlets; k++)
		in[k] = (t_float *)(w[k+3]);

	for (k=0; k<numOutlets; k++)
		out[k] = (t_float *)(w[numInlets+k+3]);	
	
	
	// first channel panning
	coeffptr=x->coeffLists;
	j=0;
	for (k=0; k<numOutlets; k++){
			outptr= out[k];
			coeff = *coeffptr++;
			inptr = in[j];
			if(coeff != 0.0)
				for (i=0; i<n; ++i)
					*outptr++ = *inptr++ * coeff; 
			else
				for (i=0; i<n; ++i)
					*outptr++ = 0.0; 
	}		
	// other channels panning
	for(j=1;j<numInlets; j++){
		for (k=0; k<numOutlets; k++){
			outptr= out[k];
			coeff = *coeffptr++;
			inptr = in[j];
			if(coeff != 0.0)
				for (i=0; i<n; ++i)
					*outptr++ += *inptr++ * coeff; 
		}	
	}		
	return (wAsT_int+numInlets+numOutlets+3);
}

t_int *matrix_perform_smooth(t_int *wAsT_int) {
// smooth == gain factors are cross faded

	t_int k, i,j;
	t_int **w = (t_int **)wAsT_int;
	t_matrix *x = (t_matrix *)(w[1]);
	t_int n = (t_int)(w[2]);	
	// use integers to copy blocs faster: was suggested by David Z.
	t_float **in = x->Inlets;
	t_float **out = x->Outlets;
	t_int numInlets = x->numInlets;
	t_int numOutlets = x->numOutlets;
	t_float ocoeff, ncoeff, coeffsubstr; 
	t_float *coeffptr, *oldcoeffptr, *nextcoeffptr;
	t_float *inptr, *outptr;
	t_float invfloatn;
	
	// Inlets and Outlets
	
	for (k=0; k<numInlets; ++k)
		in[k] = (t_float *)(w[k+3]);

	for (k=0; k<numOutlets; ++k)
		out[k] = (t_float *)(w[numInlets+k+3]);	
	invfloatn =  1.0 / (t_float) x->upd_interval;
	nextcoeffptr=x->nextcoeffLists;
	oldcoeffptr=x->oldcoeffLists;
	j=0;
	//first channel panning
	for (k=0; k<numOutlets; k++){
			outptr= out[k];
			ocoeff = *oldcoeffptr++;
			ncoeff = *nextcoeffptr++;
			coeffsubstr = ncoeff - ocoeff;
			inptr = in[j];
			if(ncoeff != 0.0 || ocoeff != 0.0)
				if(ncoeff != ocoeff)
					for (i = x->counter; i < n + x->counter; i++){
						*outptr++ = *inptr++ * (ocoeff + (t_float) i * invfloatn * coeffsubstr); 	
					}
				else 
					for (i=0; i<n; ++i)
						*outptr++ = *inptr++ * ocoeff;
			else  
				for (i=0; i<n; ++i)
					*outptr++ = 0.0;
	}
	// other channels panning
	for(j=1;j<numInlets; j++){
		for (k=0; k<numOutlets; k++){
			outptr= out[k];
			ocoeff = *oldcoeffptr++;
			ncoeff = *nextcoeffptr++;
			coeffsubstr = ncoeff - ocoeff;
			inptr = in[j];
			if(ncoeff != 0.0 || ocoeff != 0.0)
				if(ncoeff != ocoeff)
					for (i = x->counter; i < n + x->counter; i++){
						*outptr++ += *inptr++ * (ocoeff + (t_float) i * invfloatn  * coeffsubstr); 
					}
				else 
					for (i=0; i<n; ++i)
						*outptr++ += *inptr++ * ocoeff;
		}	
	}	
	x->counter += n;
	if(x->counter >= x->upd_interval){
		coeffptr= x->coeffLists;
		nextcoeffptr= x->nextcoeffLists;
		oldcoeffptr= x->oldcoeffLists;
		for(i=0;i< (numOutlets * numInlets); i++){
			*oldcoeffptr++ = *nextcoeffptr;
			*nextcoeffptr++ = *coeffptr++;
		}
		x->counter = 0;
	}
	return (wAsT_int+numInlets+numOutlets+3);
}

void matrix_dsp(t_matrix *x, t_signal **sp, short *connect) {
	t_int vs = sys_getblksize();
	t_int i, num = x->numInlets + x->numOutlets; // number of Inlets and Outlets 
	t_int **w = x->w;

	w[0] = (t_int *)x;
	w[1] = (t_int *)sp[0]->s_n;

	for (i=0; i<num; ++i)
		w[i+2] = (t_int *)sp[i]->s_vec;
			
	num += 2; // x and n

	if (x->version == FAST) 
		dsp_addv(matrix_perform_fast, num, (void **)w);
	else dsp_addv(matrix_perform_smooth, num, (void **)w);
}

void matrix_fast(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
     x->version = FAST;
}

void matrix_smooth(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
     x->version = SMOOTH;
     if(argc)
     	if (argv[0].a_type == A_LONG) x->upd_interval = argv[0].a_w.w_long;
		else if (argv[0].a_type == A_FLOAT) x->numInlets = (int)argv[0].a_w.w_float;
}

void jit_matrix(t_matrix *x, t_symbol *sym, long argc, t_atom *argv){
	void *matrix;
	long err,dimcount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	long i, j, n;
	long rowstride, colstride;
	long in_savelock;
	t_jit_matrix_info in_minfo;
	char *in_bp;
	char *ip;
	t_atom a_coord[1024];
	t_float *coeffptr,*oldcoeffptr;

	if(x->version == FAST){
		coeffptr= x->coeffLists + x->m_obj.z_in*x->numOutlets;
		oldcoeffptr= x->oldcoeffLists + x->m_obj.z_in*x->numOutlets;
		for(i=0;i<x->numOutlets;i++){
			*oldcoeffptr++ = *coeffptr++;
		}
	}

	coeffptr = x->coeffLists;
	
	if (argc&&argv) {
		//find matrix
		matrix = jit_object_findregistered(jit_atom_getsym(argv));
		if (matrix && jit_object_method(matrix, _jit_sym_class_jit_matrix)) {
			//calculate
			in_savelock = (long) jit_object_method(matrix, _jit_sym_lock, 1);
			jit_object_method(matrix, _jit_sym_getinfo, &in_minfo);
			jit_object_method(matrix, _jit_sym_getdata, &in_bp);
			
			if (!in_bp) { 
				jit_error_sym(x, _jit_sym_err_calculate);
				jit_object_method(matrix, _jit_sym_lock, in_savelock);
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
			if (in_minfo.type==_jit_sym_char) {
				for (j=0;j<dim[1];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];	
					jit_atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[0];i++){
						jit_atom_setlong(&(a_coord[1]),i);
						jit_atom_setlong(&(a_coord[2+i]), *((uchar *)ip));

						post("%d %d %d", i, j, *((uchar *)ip));
						ip += rowstride;
					}
					matrix_list(x, NULL, dim[i], a_coord);
				}
			} else if (in_minfo.type==_jit_sym_long) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];	
					jit_atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++){
						jit_atom_setlong(&(a_coord[1]),i);
						jit_atom_setlong(&(a_coord[2+i]), *((long *)ip));
						post("%d %d %d", i, j, *((long *)ip));
						matrix_list(x, NULL, 3, a_coord);
						ip += rowstride;
					}
					matrix_list(x, NULL, dim[i], a_coord);
				}
			} else if (in_minfo.type==_jit_sym_float32) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];	
					//jit_atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++){
						//jit_atom_setlong(&(a_coord[1]),i);
						jit_atom_setfloat(&(a_coord[i]), *((float *)ip));
						post("i = %d, j = %d, %f", i, j, *((float *)ip));
						ip += rowstride;
					}
					x->m_obj.z_in = j;
					matrix_list(x, NULL, dim[1], a_coord);
				}
			} else if (in_minfo.type==_jit_sym_float64) {
				for (j=0;j<dim[0];j++) {
					ip = in_bp + j * in_minfo.dimstride[0];	
					jit_atom_setlong(&(a_coord[0]),j);	
					for (i=0;i<dim[1];i++){
						jit_atom_setlong(&(a_coord[1]),i);
						jit_atom_setfloat(&(a_coord[2+i]), *((double *)ip));
						post("%d %d %f", i, j, *((double *)ip));
						matrix_list(x, NULL, 3, a_coord);
						ip += rowstride;
					}
					matrix_list(x, NULL, dim[i], a_coord);
				}
			}
			jit_object_method(matrix,_jit_sym_lock,in_savelock);
		} else {

		}
	}
}

void matrix_list(t_matrix *x, t_symbol *s, short argc, t_atom *argv) {
	int i;
	t_float *coeffptr,*oldcoeffptr;

	for(i = 0; i < argc; i++){
		post("%d %f", i, argv[i].a_w.w_float);
	}

	if(argc != x->numOutlets){
		post("matrix~: Wrong number of panning coefficients! Should be %d.",x->numOutlets);
	} else {
		// old values are stored
		if(x->version == FAST){
			coeffptr= x->coeffLists + x->m_obj.z_in*x->numOutlets;
			oldcoeffptr= x->oldcoeffLists + x->m_obj.z_in*x->numOutlets;
			for(i=0;i<x->numOutlets;i++){
				*oldcoeffptr++ = *coeffptr++;
			}
		}
		// new values are picked
		coeffptr= x->coeffLists + x->m_obj.z_in*x->numOutlets;
		for(i=0; i < x->numOutlets; i++){
			if(argv[i].a_type != A_FLOAT ){
				post("matrix~: Panning coefficients must be floats!",0);
				return;
			}
			if(argv[i].a_w.w_float > 10.0 || argv[i].a_w.w_float < -10.0){
				post("matrix~: Too large pan coeff magnitude, not changed!",0);
				coeffptr++;
			} else {
				*coeffptr++ = argv[i].a_w.w_float;
			}
		}
	}
}

void *matrix_new(t_symbol *s, short argc, t_atom *argv) {

	t_int i, k;
	t_int vs = sys_getblksize(); // Size of signal vector selected in MSP
    t_matrix *x = (t_matrix *)newobject(matrix_class);    
	t_float Fs = sys_getsr();
	t_float *fptr,*gptr, *nptr;		
	// Look at 1st argument
	if (argv[0].a_type == A_LONG) x->numInlets = argv[0].a_w.w_long;
	else if (argv[0].a_type == A_FLOAT) x->numInlets = (int)argv[0].a_w.w_float;
	else x->numInlets = 2; // Just a default value

	// Look at 2nd argument
	if (argv[1].a_type == A_LONG) x->numOutlets = argv[1].a_w.w_long;
	else if (argv[1].a_type == A_FLOAT) x->numOutlets = (int)argv[1].a_w.w_float;
	else x->numOutlets = 2; // Just a default value
		
	// Look at 3rd argument
	if (argv[2].a_w.w_sym == ps_fast)
		x->version = FAST;
	else x->version = SMOOTH; // smooth is default

	x->Inlets = t_getbytes(x->numInlets * sizeof(t_float*));
	x->Outlets = t_getbytes(x->numOutlets * sizeof(t_float*));
	x->w = t_getbytes((x->numInlets + x->numOutlets + 2) * sizeof(t_int*));
		
	// Create inlets and outlets
	dsp_setup((t_pxobject *)x, x->numInlets);
	x->m_obj.z_misc = Z_NO_INPLACE; // Necessary when outlets should have different vectors than inlets !!
	for (i=0; i<x->numOutlets; ++i)
		outlet_new((t_object *)x, "signal");
	x->coeffLists = (t_float *) getbytes(x->numInlets* x->numOutlets*sizeof(t_float));
	x->oldcoeffLists = (t_float *) getbytes(x->numInlets* x->numOutlets*sizeof(t_float));
	x->nextcoeffLists = (t_float *) getbytes(x->numInlets* x->numOutlets*sizeof(t_float));
	fptr= x->coeffLists;
	gptr= x->oldcoeffLists;
	nptr=x->nextcoeffLists;
	for(i=0;i< (x->numInlets * x->numOutlets);i++){
		*fptr++ = 0.0;
		*gptr++ = 0.0;
		*nptr++ = 0.0;
	}
	x->upd_interval = DEF_UPD_INTERVAL; 
  
 	if(x->version == SMOOTH && argc>3)
 		if (argv[3].a_type == A_LONG) x->upd_interval = argv[3].a_w.w_long;
		else if (argv[3].a_type == A_FLOAT) x->upd_interval = (int)argv[3].a_w.w_float;
    x->counter=x->upd_interval;
    return (x); 
}

void  matrix_free(t_matrix *x) {

	if (x->Inlets) t_freebytes(x->Inlets, x->numInlets * sizeof(t_float*));
	if (x->Outlets) t_freebytes(x->Outlets, x->numOutlets * sizeof(t_float*));
	if (x->w) t_freebytes(x->w, (x->numInlets + x->numOutlets + 2) * sizeof(t_int*));
	dsp_free((t_pxobject *)x);
}

