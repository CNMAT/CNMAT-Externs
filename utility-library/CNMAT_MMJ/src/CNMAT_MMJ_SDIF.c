#include "CNMAT_MMJ_SDIF.h"

static Boolean verbose = FALSE;

SDIFresult CNMAT_MMJ_SDIF_init(void (*ecallback)(SDIFresult r, const char *errorString)){
	error_callback = ecallback;
	SDIFresult r;

	//  initialize SDIF libraries
	if (r = SDIF_Init()) {
		error_callback(r, "Couldn't initialize SDIF library!");
		return r;
	}
	
	if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
		error_callback(r, "Couldn't initialize SDIF memory utilities!");
		return r;
	}
		
	if (r = SDIFbuf_Init()) {
		error_callback(r, "Couldn't initialize SDIF buffer utilities!");
		return r;
	}
	
	if (r = SDIFinterp_Init()) {
		error_callback(r, "Couldn't initialize SDIF interpolation utilities!");
		return r;
	}

	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	ps_emptysymbol  = gensym("");
	ps_concatenate = gensym("concatenate");
	ps_time = gensym("time");
	ps_reltime = gensym("reltime");
	ps_direction = gensym("direction");
	ps_columns = gensym("columns");
	ps_interp = gensym("interp");
	ps_max_rows = gensym("max_rows");

	return ESDIF_SUCCESS;
}

static void *my_getbytes(int numBytes) {
	if (numBytes > SHRT_MAX) {
		return 0;
	}
	return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
	freebytes(bytes, (short) size);
}

void LookupMyBuffer(CNMAT_MMJ_SDIF_buffer *b){
#ifdef THE_S_THING_KLUDGE
	void *thing;
	
	thing = b->t_bufferSym->s_thing;
	
	if (thing) {
		if (ob_sym(thing) = ps_SDIFbuffer) {
			b->t_buffer = thing;
			return;
		}
	}
	b->t_buffer = 0;
#else
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		b->t_buffer = 0;
	} else {
		b->t_buffer = ((*f)(b->t_bufferSym));
	}
#endif

	//  get access to the SDIFbuf_Buffer instance
	if (b->t_buffer)
		b->t_buf = (b->t_buffer->BufferAccessor)(b->t_buffer);
	else
		b->t_buf = NULL;
}

/*
static SDIFmem_Matrix GetMatrix(CNMAT_MMJ_SDIF_buffer *b,
				const char *desiredType,
                                sdif_float64 time,
                                InterpMode mode,
                                int direction,
                                int *columns,
                                int num_columns)
{
	SDIFmem_Matrix matrixOut;
	//char desiredType[4];
	int j;

	LookupMyBuffer(b);
	
	if (b->t_buffer == 0) {
		error_callback(ESDIF_OTHER, "no buffer!");
		return NULL;
	}
	

	//  what matrixType do we want?
	if (x->t_mainMatrix) {
		SDIFmem_Frame f;
		// The "main" matrix is the one whose type is the same as the frame type
		f = SDIFbuf_GetFirstFrame(x->t_buf);
		// post("* main matrix mode - first frame is %p", f);
		if (f == NULL) {
			if (x->t_errorreporting) {
				post("¥ SDIF-tuples: buffer %s is empty!", x->t_bufferSym->s_name);
			}
			return NULL;
		}
  		SDIF_Copy4Bytes(desiredType, f->header.frameType);
	} else {
		//post("** explicit matrix mode: want %c%c%c%c",
		//     x->t_matrixType[0], x->t_matrixType[1], x->t_matrixType[2], x->t_matrixType[3]);
		SDIF_Copy4Bytes(desiredType, x->t_matrixType);
	}	
	// post("* Looking for matrix type %c%c%c%c", desiredType[0], desiredType[1], desiredType[2], desiredType[3]);

	if (mode == INTERP_MODE_NONE)
		{
			//  try to get a matrix without interpolation
			if(!(matrixOut = GetMatrixWithoutInterpolation(b,
								       desiredType,
								       time,
								       direction)))
				return NULL;
		}
	else
		{
			//  try to compute an interpolated matrix
			if(!(matrixOut = GetMatrixWithInterpolation(b,
								    desiredType,
								    time,
								    columns,
								    num_columns,
								    mode)))
				return NULL;
		}

	//  make sure requested columns actually exist in the matrix we found
	if (num_columns != 0) {
		for (j = 0; j < num_columns; ++j) {
			if (columns[j] > matrixOut->header.columnCount || columns[j] <= 0) {
				char buf[256];
				sprintf(buf, "can't output column %d of a %d-column matrix!", 
					columns[j], matrixOut->header.columnCount);
				error_callback(ESDIF_BAD_COLUMN_NUMBER, buf);
				post("¥ SDIF-tuples: can't output column %d of a %d-column matrix!",
				     columns[j], matrixOut->header.columnCount);
				return NULL;
			}
		}
	}
	
	//  return result
	//  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()  
	return matrixOut;
}
*/

SDIFmem_Matrix GetMatrix(CNMAT_MMJ_SDIF_buffer *b,
				const char *desiredType,
				sdif_float64 time,
				int direction)
{
	LookupMyBuffer(b);
	
	if (b->t_buffer == 0) {
		error_callback(ESDIF_OTHER, "no buffer!");
		return NULL;
	}

	SDIFmem_Frame f;
	SDIFmem_Matrix m, matrixOut;
	SDIFresult r;
  
	//  get the frame
	if(!(f = (*(b->t_buffer->FrameLookup))(b->t_buffer, time, direction))) {
		if ((*(b->t_buffer->FrameLookup))(b->t_buffer, (sdif_float64) VERY_SMALL, 1) == 0) {
			//if (!x->t_complainedAboutEmptyBufferAlready) {
			char buf[256];
			sprintf(buf, "SDIF-buffer %s is empty", b->t_bufferSym->s_name);
			error_callback(ESDIF_OTHER, buf);
			//post("¥ SDIF-tuples: SDIF-buffer %s is empty", x->t_bufferSym->s_name);
			//x->t_complainedAboutEmptyBufferAlready = TRUE;
			//}
		} else {
			if (verbose) {
				char buf[256];
				sprintf(buf, "SDIF-buffer %s has no frame at time %f", 
					b->t_bufferSym->s_name, (float) time);
				error_callback(ESDIF_OTHER, buf);
				//post("¥ SDIF-tuples: SDIF-buffer %s has no frame at time %f", 
				//x->t_bufferSym->s_name, (float) time);
			}
		}
		return NULL;
	}

	//  find the matrix
	for (m = f->matrices; m!= 0; m = m->next) {
		if (SDIF_Char4Eq(m->header.matrixType, desiredType)) {
			break;
		}
	}
	
	//  couldn't find the matrix
	if (m == 0) {
		char buf[256];
		sprintf(buf, "no matrix of type %c%c%c%c in frame at time %f of SDIF-buffer %s",
			desiredType[0], desiredType[1], desiredType[2], desiredType[3],
			(float) f->header.time, b->t_bufferSym->s_name);
		error_callback(ESDIF_BAD_PARAM, buf);
		//post("¥ SDIF-tuples: no matrix of type %c%c%c%c in frame at time %f of SDIF-buffer %s",
		//desiredType[0], desiredType[1], desiredType[2], desiredType[3],
		//(float) f->header.time, x->t_bufferSym->s_name);
		return NULL;
	}

	//  we don't support matrix data types other than float and int32  	
	if (((m->header.matrixDataType >> 8) != SDIF_FLOAT) && (m->header.matrixDataType != SDIF_INT32))  {
		char buf[256];
		sprintf(buf, "Unsupported matrix data type (0x%x) in %c%c%c%c matrix in frame at time %f of SDIF-buffer %s (currently only float and int32 are supported)",
			m->header.matrixDataType, 
			desiredType[0], desiredType[1], desiredType[2], desiredType[3], 
			(float) f->header.time, b->t_bufferSym->s_name);
		error_callback(ESDIF_BAD_PARAM, buf);
		/*
		  post("¥ SDIF-tuples: Unsupported matrix data type (0x%x) in %c%c%c%c matrix ",
		  m->header.matrixDataType, 
		  desiredType[0], desiredType[1], desiredType[2], desiredType[3]);
		  post("  in frame at time %f of SDIF-buffer %s (currently only float and int32 are supported)",
		  (float) f->header.time, x->t_bufferSym->s_name);
		*/
		return NULL;
	}
	
	// post("*** Cloning matrix at time %f, type 0x%x", time, m->header.matrixDataType);
	  
	//  copy result to output matrix
	r = SDIFutil_CloneMatrix(m, &matrixOut);
	
	if (r == ESDIF_OUT_OF_MEMORY) {
		error_callback(r, "out of memory to clone matrix");
		return NULL;
	} else if (r!=ESDIF_SUCCESS) {
		error_callback(r, "couldn't clone matrix");
		return NULL;
  	}
  	
	//  return result
	//  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()
	return matrixOut;
}


SDIFmem_Matrix GetMatrixWithInterpolation(CNMAT_MMJ_SDIF_buffer *b,
                                                 const char *desiredType,
                                                 sdif_float64 time,
                                                 int *columns,
                                                 int num_columns,
                                                 InterpMode mode,
						 SDIFinterp_Interpolator *it,
						 Boolean *itValid)
{
	LookupMyBuffer(b);
	
	if (b->t_buffer == 0) {
		error_callback(ESDIF_OTHER, "no buffer!");
		return NULL;
	}

	SDIFmem_Frame f;
	SDIFmem_Matrix m, matrixOut;
	sdif_float64 t1;
	sdif_int32 interpParam;
  
	//  find nearby frame (used as starting point for neighbor value searches)
	if(!(f = SDIFbuf_GetFrame(b->t_buf, time, ESDIF_SEARCH_BACKWARDS)))
		return NULL;
  
	//  try to find a matrix of desired type in this frame or earlier
	if(!(m = SDIFbuf_GetMatrixNearby(f, desiredType, time, ESDIF_SEARCH_BACKWARDS, &t1)))
		return NULL;

	//  we don't support matrix data types other than float	
	if ((m->header.matrixDataType >> 8) != SDIF_FLOAT){
		char buf[256];
		sprintf(buf, "Unsupported matrix data type (Ox%x) in %c%c%c%c matrix in frame at time %f of SDIF-buffer %s (currently only float data is supported with interpolation",
			m->header.matrixDataType, 
			desiredType[0], desiredType[1], desiredType[2], desiredType[3],
			(float) f->header.time, b->t_bufferSym->s_name);
		/*
		  post("¥ SDIF-tuples: Unsupported matrix data type (Ox%x) in %c%c%c%c matrix ",
		  m->header.matrixDataType, 
		  desiredType[0], desiredType[1], desiredType[2], desiredType[3]);
		  post("  in frame at time %f of SDIF-buffer %s (currently only float data is supported with interpolation)",
		  (float) f->header.time, x->t_bufferSym->s_name);
		*/
		return NULL;
	}		
	  
	//  build the interpolator matrix, if anything has changed since last time
	if(!(*itValid)){
		SetupInterpolator(it, itValid, m->header.columnCount, columns, num_columns, mode);
		if(!(it))
			return NULL;
	}
  
	//  create the output matrix (find dimensions by looking at nearby matrix instance)
	if(!(matrixOut = SDIFutil_CreateMatrix(m->header.columnCount,
					       m->header.rowCount,
					       SDIF_FLOAT64,
					       desiredType)))
		return NULL;
  
	//  compute the interpolated matrix
	switch(mode)
		{
		case INTERP_MODE_LINEAR:
			interpParam = 0;   //  ignored by interpolator anyway
			break;
		case INTERP_MODE_LAGRANGE2:
			interpParam = 2;   //  Lagrange/Waring degree == 2
			break;
		case INTERP_MODE_LAGRANGE3:
			interpParam = 3;   //  Lagrange/Waring degree == 3
			break;
		default:
			interpParam = 0;
		}
	if(ESDIF_SUCCESS != SDIFinterp_GetMatrixNearby(*it,
						       f,
						       desiredType,
						       time,
						       ESDIF_NAN_ACTION_KEEP_LOOKING,
						       matrixOut,
						       interpParam))
		{
			SDIFmem_FreeMatrix(matrixOut);
			return NULL;
		}

	//  make sure requested columns actually exist in the matrix we found
	int j;
	for (j = 0; j < num_columns; ++j) {
		if (columns[j] > matrixOut->header.columnCount || columns[j] <= 0) {
			char buf[256];
			sprintf(buf, "can't output column %d of a %d-column matrix!", 
				columns[j], matrixOut->header.columnCount);
			error_callback(ESDIF_BAD_COLUMN_NUMBER, buf);
			/*
			  post("¥ SDIF-tuples: can't output column %d of a %d-column matrix!",
			  columns[j], matrixOut->header.columnCount);
			*/
			return NULL;
		}
	}
  
	//  return result
	//  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()
	return matrixOut;
}


void SetupInterpolator(SDIFinterp_Interpolator *it,
			      Boolean *itValid,
                              int srcColumns,
                              int *dstColumns, 
                              int num_dstColumns,
                              InterpMode mode)
{
	//  release old interpolator, if any
	if(*it)
		SDIFinterp_Free(*it);
  
	//  create new interpolator
	if(!(*it = SDIFinterp_Create(srcColumns)))
		return;

	/////
	/////  to avoid interpolating unused columns, should fill in unused columns with NULL
	/////
  
	switch(mode)
		{
		case INTERP_MODE_LINEAR:
			SDIFinterp_SetAllInterpolatorFn(*it, LinearInterpolator);
			break;
		case INTERP_MODE_LAGRANGE2:
		case INTERP_MODE_LAGRANGE3:
			SDIFinterp_SetAllInterpolatorFn(*it, LagrangeInterpolator);
			break;
		default:
			SDIFinterp_SetAllInterpolatorFn(*it, NULL);
			break;
		}
  
	//  set this flag if we want to avoid creating a new interpolator on every matrix request:
	//  t_itValid = TRUE;
}

void PrintOneFrame(SDIFmem_Frame f) {
	SDIFmem_Matrix m;
	if (f == 0) {
		post("PrintOneFrame: null SDIFmem_Frame pointer");
		return;
	}
	// post("SDIF frame at %p, prev is %p, next is %p", f, f->prev, f->next);
	PrintFrameHeader(&(f->header));
	
	for (m = f->matrices; m != 0; m=m->next) {
		PrintMatrixHeader(&(m->header));
	}
}

void PrintFrameHeader(SDIF_FrameHeader *fh) {
	post(" Frame header: type %c%c%c%c, size %ld, time %g, stream ID %ld, %ld matrices",
	     fh->frameType[0], fh->frameType[1], fh->frameType[2], fh->frameType[3], 
	     fh->size, fh->time, fh->streamID, fh->matrixCount);
}
	
void PrintMatrixHeader(SDIF_MatrixHeader *mh) {
	post("  Matrix header: type %c%c%c%c, data type 0x%x, %ld rows, %ld cols",
	     mh->matrixType[0], mh->matrixType[1], mh->matrixType[2], mh->matrixType[3],
	     mh->matrixDataType, mh->rowCount, mh->columnCount);
}

void verbose_error_reporting(Boolean b){verbose = b;}
