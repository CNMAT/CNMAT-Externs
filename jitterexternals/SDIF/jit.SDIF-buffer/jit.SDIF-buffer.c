/* 
	Copyright 2001 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com	
*/

#include "jit.common.h"
#include "CNMAT_MMJ_SDIF.h"

#define jflone 	0x3f800000
#define jflmsk  0x007fffff

typedef struct _jit_SDIF_buffer_vecdata
{
	long			randlast;
} t_jit_SDIF_buffer_vecdata;

typedef struct _jit_SDIF_buffer 
{
	t_object		ob;
} t_jit_SDIF_buffer;

void *_jit_SDIF_buffer_class;

t_jit_err jit_SDIF_buffer_init(void); 
t_jit_SDIF_buffer *jit_SDIF_buffer_new(void);
void jit_SDIF_buffer_free(t_jit_SDIF_buffer *x);
t_jit_err jit_SDIF_buffer_getvecdata(t_jit_SDIF_buffer *x, t_jit_SDIF_buffer_vecdata *vd);
t_jit_err jit_SDIF_buffer_matrix_calc(t_jit_SDIF_buffer *x, void *inputs, void *outputs, SDIFmem_Matrix m);

void jit_SDIF_buffer_calculate_ndim(t_jit_SDIF_buffer_vecdata *vecdata, long dim, long *dimsize, long planecount, t_jit_matrix_info *out_minfo, char *bop);
void jit_SDIF_buffer_vector_char		(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out); 
void jit_SDIF_buffer_vector_long		(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out); 
void jit_SDIF_buffer_vector_float32	(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out);
void jit_SDIF_buffer_vector_float64	(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out); 

t_jit_err jit_SDIF_buffer_init(void) {
	void *mop;
	
	_jit_SDIF_buffer_class = jit_class_new("jit_SDIF_buffer",(method)jit_SDIF_buffer_new,(method)jit_SDIF_buffer_free,
		sizeof(t_jit_SDIF_buffer),0L); 

	//add mop
	mop = jit_object_new(_jit_sym_jit_mop,0,1); 
	jit_class_addadornment(_jit_SDIF_buffer_class,mop);
	//add methods
	jit_class_addmethod(_jit_SDIF_buffer_class, (method)jit_SDIF_buffer_matrix_calc, "matrix_calc", A_CANT, 0L);

	jit_class_register(_jit_SDIF_buffer_class);

	return JIT_ERR_NONE;
}

t_jit_err jit_SDIF_buffer_getvecdata(t_jit_SDIF_buffer *x, t_jit_SDIF_buffer_vecdata *vd){
	if (x&&vd) {
		vd->randlast = jit_rand();
		return JIT_ERR_NONE;
	} else {
		return JIT_ERR_INVALID_PTR;
	}
}

t_jit_err jit_SDIF_buffer_matrix_calc(t_jit_SDIF_buffer *x, void *inputs, void *outputs, SDIFmem_Matrix m){
	t_jit_err err=JIT_ERR_NONE;
	long out_savelock;
	t_jit_matrix_info out_minfo;
	char *out_bp;
	long i,dimcount,planecount,dim[JIT_MATRIX_MAX_DIMCOUNT];
	t_jit_SDIF_buffer_vecdata	vecdata;
	void *out_matrix;
	
	out_matrix 	= jit_object_method(outputs,_jit_sym_getindex,0);

	if (x&&out_matrix) {
		out_savelock = (long) jit_object_method(out_matrix,_jit_sym_lock,1);
		jit_object_method(out_matrix,_jit_sym_getinfo,&out_minfo);		

		/**************************************************
		// get the info struct and fill it up the way we want
		**************************************************/
		out_minfo.size = m->header.rowCount * m->header.columnCount * (m->header.matrixDataType & 0xF);
		switch(m->header.matrixDataType){
		case SDIF_FLOAT64:
			out_minfo.type = _jit_sym_float64;
			break;
		case SDIF_FLOAT32:
			out_minfo.type = _jit_sym_float32;
			break;
		}

		out_minfo.dimcount = 2;
		out_minfo.dim[0] = m->header.columnCount;
		out_minfo.dim[1] = m->header.rowCount;
		out_minfo.dimstride[0] = (m->header.matrixDataType & 0xF);
		out_minfo.dimstride[1] = out_minfo.dim[0] * out_minfo.dimstride[0];
		out_minfo.planecount = 1;

		/**************************************************
		// Set the info struct and then get it again so we have the dimstrides as they 
		// may have changed to be 16-byte aligned.
		**************************************************/
		jit_object_method(out_matrix, gensym("setinfo"), &out_minfo);
		jit_object_method(out_matrix, gensym("getinfo"), &out_minfo);

		/**************************************************
		// now get the data array which should be the correct size.
		**************************************************/
		jit_object_method(out_matrix,_jit_sym_getdata,&out_bp);
		if (!out_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		char *sdifData = (char *)(m->data);
		int colWidth = m->header.matrixDataType & 0xF;
		int i = 0;
		while(i < out_minfo.size){
			memcpy(out_bp + i, sdifData, m->header.columnCount * colWidth);
			//out_bp += out_minfo.dimstride[1];
			sdifData += (colWidth * m->header.columnCount);
			i += out_minfo.dimstride[1];
		}

		jit_object_method(out_matrix, gensym("data"), out_bp);
		
		//get dimensions/planecount
		//dimcount   = out_minfo.dimcount;
		//planecount = out_minfo.planecount;			
		
		//for (i=0;i<dimcount;i++) {
		//dim[i] = out_minfo.dim[i];
		//}
				
		//jit_SDIF_buffer_getvecdata(x,&vecdata);
		//jit_parallel_ndim_simplecalc1((method)jit_SDIF_buffer_calculate_ndim,
		//&vecdata, dimcount, dim, planecount, &out_minfo, out_bp,
		//0 /* flags1 */);

		//post("rowCount = %d, colCount = %d", m->header.rowCount, m->header.columnCount);
		//post("type = %x", m->header.matrixDataType);

	} else {
		return JIT_ERR_INVALID_PTR;
	}
	
out:
	jit_object_method(out_matrix,_jit_sym_lock,out_savelock);
	return err;
}


void jit_SDIF_buffer_calculate_ndim(t_jit_SDIF_buffer_vecdata *vecdata, long dimcount, long *dim, long planecount, t_jit_matrix_info *out_minfo, char *bop)
{
	long i,n;
	char *op;
	t_jit_op_info out_opinfo;
		
	if (dimcount<1) return; //safety
	
	switch(dimcount) {
	case 1:
		dim[1]=1;
	case 2:
		//flatten planes - treat as single plane data for speed...ALWAYS
		n = dim[0];
		out_opinfo.stride = 1;
		n *= planecount;
		planecount = 1;
		if (out_minfo->type==_jit_sym_char) {
			for (i=0;i<dim[1];i++){
				vecdata->randlast 	= jit_rand(); 
				out_opinfo.p = bop + i*out_minfo->dimstride[1];
				jit_SDIF_buffer_vector_char(n,vecdata,&out_opinfo);
			}
		} else if (out_minfo->type==_jit_sym_long) {
			for (i=0;i<dim[1];i++){
				vecdata->randlast 	= jit_rand(); 
				out_opinfo.p = bop + i*out_minfo->dimstride[1];
				jit_SDIF_buffer_vector_long(n,vecdata,&out_opinfo);
			}
		} else if (out_minfo->type==_jit_sym_float32) {
			for (i=0;i<dim[1];i++){
				vecdata->randlast 	= jit_rand(); 
				out_opinfo.p = bop + i*out_minfo->dimstride[1];
				jit_SDIF_buffer_vector_float32(n,vecdata,&out_opinfo);
			}
		} else if (out_minfo->type==_jit_sym_float64) {
			for (i=0;i<dim[1];i++){
				vecdata->randlast 	= jit_rand(); 
				out_opinfo.p = bop + i*out_minfo->dimstride[1];
				jit_SDIF_buffer_vector_float64(n,vecdata,&out_opinfo);
			}
		}
		break;
	default:
		for	(i=0;i<dim[dimcount-1];i++) {
			op  = bop  + i*out_minfo->dimstride[dimcount-1];
			jit_SDIF_buffer_calculate_ndim(vecdata,dimcount-1,dim,planecount,out_minfo,op);
		}
	}
}

void jit_SDIF_buffer_vector_char(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out) 
{
	uchar *op;
	long os;
	unsigned long  idum=vecdata->randlast;
	
	op  = ((uchar *)out->p);
	os  = out->stride; 
	
	if (os==1) {
		++n;--op;
		while (--n) {
			idum  = 1664525L * idum + 1013904223L;
			*++op = (idum>>16L); //shift right to remove low order correlation
		}		
		vecdata->randlast = idum;
	} else {
		while (n--) {
			idum = 1664525L * idum + 1013904223L;
			*op = (idum>>16L);
			op+=os;
		}
		vecdata->randlast = idum;
	}
}

void jit_SDIF_buffer_vector_long(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out) 
{
	long *op;
	long os;
	unsigned long  idum=vecdata->randlast,idum2;
	
	op  = ((long *)out->p);
	os  = out->stride; 
	
	if (os==1) {
		++n;--op;
		idum2 = idum;
		while (--n) {
			idum  = 1664525L * idum2 + 1013904223L;
			idum2 = 1664525L * idum + 1013904223L;
			*++op = (idum2&0xFFFF0000)|(idum>>16L); //shift right to remove low order correlation
		}		
		vecdata->randlast = idum2;
	} else {
		idum2 = idum;
		while (n--) {
			idum = 1664525L * idum2 + 1013904223L;
			idum2 = 1664525L * idum + 1013904223L;
			*op = (idum2&0xFFFF0000)|(idum>>16L);
			op+=os;
		}
		vecdata->randlast = idum2;
	}
}

void jit_SDIF_buffer_vector_float32(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out) 
{
	float *op;
	long os;
	unsigned long  idum=vecdata->randlast,itemp;
	
	op  = ((float *)out->p);
	os  = out->stride; 
	
	if (os==1) {
		++n;--op;
		while (--n) {
			idum = 1664525L * idum + 1013904223L;
			itemp = jflone | (jflmsk & idum);
			*++op = ((*(float *)&itemp) - 1.0); 
		}		
		vecdata->randlast = idum;
	} else {
		while (n--) {
			idum = 1664525L * idum + 1013904223L;
			itemp = jflone | (jflmsk & idum);
			*op = ((*(float *)&itemp) - 1.0); 	
			op+=os;
		}
		vecdata->randlast = idum;
	}
}

void jit_SDIF_buffer_vector_float64(long n, t_jit_SDIF_buffer_vecdata *vecdata, t_jit_op_info *out) 
{
	double *op;
	long os;
	unsigned long  idum=vecdata->randlast,itemp;
	
	op  = ((float *)out->p);
	os  = out->stride; 
	
	if (os==1) {
		++n;--op;
		while (--n) {
			idum = 1664525L * idum + 1013904223L;
			itemp = jflone | (jflmsk & idum);
			*++op = ((*(float *)&itemp) - 1.0); 
		}		
		vecdata->randlast = idum;
	} else {
		while (n--) {
			idum = 1664525L * idum + 1013904223L;
			itemp = jflone | (jflmsk & idum);
			*op = ((*(float *)&itemp) - 1.0); 	
			op+=os;
		}
		vecdata->randlast = idum;
	}
}

t_jit_SDIF_buffer *jit_SDIF_buffer_new(void)
{
	t_jit_SDIF_buffer *x;
		
	if (x=(t_jit_SDIF_buffer *)jit_object_alloc(_jit_SDIF_buffer_class)) {
		//nada
	} else {
		x = NULL;
	}	
	return x;
}

void jit_SDIF_buffer_free(t_jit_SDIF_buffer *x)
{
	//nada
}
