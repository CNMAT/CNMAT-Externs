/** 	@file cmmjl_sdif.h
	Definitions of functions that can be used to access the contents of an SDIF-buffer object in Max/MSP.
	@defgroup SDIF SDIF
 @{
 */

#include "ext.h"
#include "SDIF-buffer.h"  //  includes sdif.h, sdif-mem.h, sdif-buf.h
#include "sdif-util.h"
#include "sdif-interp.h"
#include "sdif-interp-implem.h"


/** Struct containing all of the necessary data structures needed to access and manipulate the contents of an SDIF-buffer in Max/MSP. */
typedef struct _cmmjl_sdif_buffer{
	t_symbol *t_bufferSym;	/**< Buffer name */
	SDIFBuffer *t_buffer;	/**< Instance of the SDIF-buffer */
	SDIFbuf_Buffer t_buf;	/**< Provides API to manipulate buffer contents */
}cmmjl_sdif_buffer;

/** Interpolation modes used when requesting a matrix with interpolation. */
typedef enum {
	INTERP_MODE_NONE, /**< No interpolation (default) */
	INTERP_MODE_LINEAR, /**< Linear interpolation */
	INTERP_MODE_LAGRANGE2, /**< 2nd-order polynomial interpolation */
	INTERP_MODE_LAGRANGE3, /**< 3rd-order polynomial interpolation */
  	INTERP_MODE_END
} InterpMode;

/** 
Callback for reporting error messages and codes to the caller
	@param r SDIF result code
	@param errorString Description of the error.
*/
void (*error_callback)(SDIFresult r, const char *errorString);

/**
Call this to initialize the CNMAT_MMJ_SDIF library.
	@param ecallback Callback function for error reporting back to the caller.  
	@returns An SDIFresult code.
 */
SDIFresult cmmjl_sdif_init(void (*ecallback)(SDIFresult r, const char *errorString));

/** 
Associates b->t_buffer with the name in b->t_bufferSym.
	@param b Struct containing a reference to an SDIF-buffer and the desired buffer's name.
*/
void LookupMyBuffer(cmmjl_sdif_buffer *b);

/**
Get a matrix of a given type from the frame closest to the requested time.
	@param b SDIF-buffer data structures.
	@param desiredType SDIF matrix type to get.
	@param time Get the matrix nearest to this time.
	@param direction Search from the front (1) or the end (-1).
	@returns An SDIF matrix.
*/
SDIFmem_Matrix GetMatrix(cmmjl_sdif_buffer *b,
				const char *desiredType,
				sdif_float64 time,
				int direction);

/**
Perform interpolation on the matrix before it is returned.
	@param b SDIF-buffer data structures.
	@param desiredType SDIF matrix type to get.
	@param time Get the matrix nearest to this time.
	@param columns List of columns to get.
	@param num_columns Number of columns contained in the columns array.
	@param mode Interpolation mode (see #InterpMode).
	@param it Interpolator function declared in sdif-interp.h
	@param itValid Set this to FALSE to forse a new interpolation matrix to be created.
	@returns An SDIF matrix on which interpolation has been performed.
*/
SDIFmem_Matrix GetMatrixWithInterpolation(cmmjl_sdif_buffer *b,
                                                 const char *desiredType,
                                                 sdif_float64 time,
                                                 int *columns,
                                                 int num_columns,
                                                 InterpMode mode,
						 SDIFinterp_Interpolator *it,
						 Boolean *itValid);

/**
Set up the interpolator matrix.
	@param it The interpolation matrix to be initialized.
	@param itValid If FALSE, a new interpolation matrix will be created.
	@param srcColumns Number of source columns.
	@param dstColumns Array of destination columns.
	@param num_dstColumns Number of destination columns.
	@param mode Interpolation mode (see #InterpMode).
 */
void SetupInterpolator(SDIFinterp_Interpolator *it,
			      Boolean *itValid,
                              int srcColumns,
                              int *dstColumns, 
                              int num_dstColumns,
                              InterpMode mode);

/**
Get the type of the 'main' matrix.  The 'main' matrix is the matrix whose type is the same as that of the first frame.
	@param b Buffer structure (see #cmmjl_sdif_buffer).
	@param mt Variable where the matrix type will be stored.
*/
void cmmjl_sdif_getMainMatrixType(cmmjl_sdif_buffer *b, char *mt);

void PrintOneFrame(SDIFmem_Frame f);
void PrintFrameHeader(SDIF_FrameHeader *fh);
void PrintMatrixHeader(SDIF_MatrixHeader *mh);

void SetAtomFromMatrix(Atom *a, SDIFmem_Matrix m, sdif_int32 column, sdif_int32 row);

void verbose_error_reporting(Boolean b);

/*@}*/
