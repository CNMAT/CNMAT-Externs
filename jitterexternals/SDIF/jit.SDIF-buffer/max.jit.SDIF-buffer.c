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
 NAME: jit.SDIF-buffer
 DESCRIPTION: Wrap the contents of an SDIF-buffer in a jitter matrix
 AUTHORS: John MacCallum
 COPYRIGHT_YEARS: 2008
 SVN_REVISION: $LastChangedRevision: 587 $
 VERSION 1.0: First try
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
#include "jit.common.h"
#include "max.jit.mop.h"
//#include "version.h"
//#include "ext.h"
//#include "version.c"
#include "CNMAT_MMJ_SDIF.h"

typedef struct _max_jit_SDIF_buffer {
	t_object ob;
	void *obex;
	CNMAT_MMJ_SDIF_buffer t_b;
	char t_matrixType[4];
	SDIFmem_Matrix t_matrix;
} t_max_jit_SDIF_buffer;

t_jit_err jit_SDIF_buffer_init(void); 

void *max_jit_SDIF_buffer_new(t_symbol *s, long argc, t_atom *argv);
void max_jit_SDIF_buffer_free(t_max_jit_SDIF_buffer *x);
void max_jit_SDIF_buffer_outputmatrix(t_max_jit_SDIF_buffer *x);
void max_jit_SDIF_buffer_error(SDIFresult r, const char *st);
void max_jit_SDIF_buffer_float(t_max_jit_SDIF_buffer *x, double f);
void *max_jit_SDIF_buffer_class;
		 	
int main(void){	
	void *p,*q;
	
	jit_SDIF_buffer_init();	
	setup((t_messlist **)&max_jit_SDIF_buffer_class, (method)max_jit_SDIF_buffer_new, (method)max_jit_SDIF_buffer_free, (short)sizeof(t_max_jit_SDIF_buffer), 
		0L, A_GIMME, 0);

	p = max_jit_classex_setup(calcoffset(t_max_jit_SDIF_buffer,obex));
	q = jit_class_findbyname(gensym("jit_SDIF_buffer"));    
	max_jit_classex_mop_wrap(p,q,MAX_JIT_MOP_FLAGS_OWN_OUTPUTMATRIX|MAX_JIT_MOP_FLAGS_OWN_JIT_MATRIX); 		
	max_jit_classex_standard_wrap(p,q,0); 	
	max_addmethod_usurp_low((method)max_jit_SDIF_buffer_outputmatrix, "outputmatrix");	
	addmess((method)max_jit_mop_assist, "assist", A_CANT,0);
	addfloat((method)max_jit_SDIF_buffer_float);
	return 0;
}

void max_jit_SDIF_buffer_outputmatrix(t_max_jit_SDIF_buffer *x){
	t_atom a;
	long outputmode=max_jit_mop_getoutputmode(x);
	void *mop=max_jit_obex_adornment_get(x,_jit_sym_jit_mop);
	t_jit_err err;	
	
	if (outputmode&&mop) { //always output unless output mode is none
		if (outputmode==1) {
			if (err=(t_jit_err)jit_object_method(
				max_jit_obex_jitob_get(x), 
				_jit_sym_matrix_calc,
				jit_object_method(mop,_jit_sym_getinputlist),
				jit_object_method(mop,_jit_sym_getoutputlist),
				x->t_matrix))
			{
				jit_error_code(x,err); 
			} else {
				post("outputting matrix");
				max_jit_mop_outputmatrix(x);
			}
		}
	}	
}

void max_jit_SDIF_buffer_free(t_max_jit_SDIF_buffer *x){
	max_jit_mop_free(x);
	jit_object_free(max_jit_obex_jitob_get(x));
	max_jit_obex_free(x);
}

void *max_jit_SDIF_buffer_new(t_symbol *s, long argc, t_atom *argv){
	t_max_jit_SDIF_buffer *x;
	void *o;

	CNMAT_MMJ_SDIF_init(max_jit_SDIF_buffer_error);

	if (x=(t_max_jit_SDIF_buffer *)max_jit_obex_new(max_jit_SDIF_buffer_class,gensym("jit_SDIF_buffer"))) {
		if (o=jit_object_new(gensym("jit_SDIF_buffer"))) {
			if(!argc){
				error("jit.SDIF-buffer: you must specify the name of an SDIF-buffer");
				return NULL;
			}
			if(argv[0].a_type != A_SYM){
				error("jit.SDIF-buffer: the first argument to jit.SDIF-buffer must be the name of an SDIF-buffer");
				return NULL;
			}
			x->t_b.t_bufferSym = argv[0].a_w.w_sym;
			max_jit_mop_setup_simple(x,o,argc - 1,argv + 1);
			max_jit_attr_args(x,argc,argv);
		} else {
			error("jit.noise: could not allocate object");
			freeobject(x);
		}
	}
	return x;
}

void max_jit_SDIF_buffer_error(SDIFresult r, const char *st){
	if(r) error("jit.SDIF-buffer: %s: %s", st, SDIF_GetErrorString(r));
	else error("jit.SDIF-buffer: %s", st);
}

void max_jit_SDIF_buffer_float(t_max_jit_SDIF_buffer *x, double f){
	LookupMyBuffer(&(x->t_b));
	if(x->t_b.t_buffer == 0){
		error("jit.SDIF-buffer: warning: there is no SDIF-buffer called %s", x->t_b.t_bufferSym->s_name);
		return JIT_ERR_INVALID_PTR;
	}

	SDIFmem_Matrix m;
	CNMAT_MMJ_SDIF_getMainMatrixType(&(x->t_b), x->t_matrixType);
	post("FRAME TYPE: %s", x->t_matrixType);
	post("time = %f", f);
	if(!(m = GetMatrix(&(x->t_b), "1TRC", f, 1))){
		error("jit.SDIF-buffer: error!!");
		return;
	}
	x->t_matrix = m;
	max_jit_SDIF_buffer_outputmatrix(x);
}
