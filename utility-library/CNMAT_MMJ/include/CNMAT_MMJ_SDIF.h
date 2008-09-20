#include "ext.h"
#include "SDIF-buffer.h"  //  includes sdif.h, sdif-mem.h, sdif-buf.h
#include "sdif-util.h"
#include "sdif-interp.h"
#include "sdif-interp-implem.h"

#ifdef DBL_MIN
#define VERY_SMALL DBL_MIN
#else 
#define VERY_SMALL ((sdif_float64) - (DBL_MAX))
#endif

Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup, *ps_emptysymbol, *ps_concatenate,
	*ps_time, *ps_reltime, *ps_direction, *ps_columns, *ps_interp, *ps_max_rows;

typedef struct _CNMAT_MMJ_SDIF_buffer{
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;           //  provides API to manipulate buffer contents
}CNMAT_MMJ_SDIF_buffer;

typedef enum {
  INTERP_MODE_NONE,
  INTERP_MODE_LINEAR,
  INTERP_MODE_LAGRANGE2,
  INTERP_MODE_LAGRANGE3,
  INTERP_MODE_END
} InterpMode;

void (*error_callback)(SDIFresult r, const char *errorString);
SDIFresult CNMAT_MMJ_SDIF_init(void (*ecallback)(SDIFresult r, const char *errorString));
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
void LookupMyBuffer(CNMAT_MMJ_SDIF_buffer *b);
SDIFmem_Matrix GetMatrix(CNMAT_MMJ_SDIF_buffer *b,
				const char *desiredType,
				sdif_float64 time,
				int direction);
SDIFmem_Matrix GetMatrixWithInterpolation(CNMAT_MMJ_SDIF_buffer *b,
                                                 const char *desiredType,
                                                 sdif_float64 time,
                                                 int *columns,
                                                 int num_columns,
                                                 InterpMode mode,
						 SDIFinterp_Interpolator *it,
						 Boolean *itValid);
void SetupInterpolator(SDIFinterp_Interpolator *it,
			      Boolean *itValid,
                              int srcColumns,
                              int *dstColumns, 
                              int num_dstColumns,
                              InterpMode mode);

void PrintOneFrame(SDIFmem_Frame f);
void PrintFrameHeader(SDIF_FrameHeader *fh);
void PrintMatrixHeader(SDIF_MatrixHeader *mh);

void verbose_error_reporting(Boolean b);
