/*
Copyright (c) 2004.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Ben "Jacobs", The Center for New Music and Audio Technologies,
University of California, Berkeley.  Based on sample code from David Zicarelli.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

/* --04/05/2004 SDIF-importSEP.c -- the SDIF-importSEP object -- 

  PURPOSE
  
  Read an ascii file containing Helios SEP (Solar Energetic Particle) data,
  import into an existing SDIF-buffer object.

  VERSION HISTORY

  04/05/2004 - 0.0.1 (bj): basic functionality ("import" method)
  06/22/2004 - 0.0.2 (bj): cleanup
  
  NOTES
  
*/


#define FINDER_NAME "SDIF-importSEP"
#define OBJECT_VERSION "0.0.1"


/* the required include files */
#include "ext.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>  // for SHRT_MAX
#ifdef WIN_VERSION
double NAN;
#include <z_dsp.h>
extern int isnan(double d) {
	return IS_NAN_DOUBLE(d);
}
#else
#include <FSp_fopen.h>
#endif

/* #include <assert.h> */
#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}

/* CNMAT include files */
#include "SDIF-buffer.h"  //  includes "sdif.h", "sdif-mem.h", "sdif-buf.h"
#include "sdif-util.h"


/* class type */
typedef struct _SDIFimport 
{
 	t_object s_obj;
	Symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;           //  provides API to manipulate buffer contents
 	void *t_outlet;
 	int t_debug;
} SDIFImport;


/* SDIF matrix data type (careful: more dependencies in scanLine(), createFrame()) */
#define SEP_DATA_ROWS 6
#define SEP_DATA_COLUMNS 2
#define SEP_DATA_TYPE SDIF_FLOAT32

typedef struct _MatrixRowData
{
  sdif_float32 intensity;
  sdif_float32 intensityErr;
} 
  MatrixRowData;

typedef struct _MatrixData
{
  MatrixRowData row[SEP_DATA_ROWS];
}
  MatrixData;


/* prototypes for my functions */
static void *my_getbytes(int numBytes);
static void my_freebytes(void *bytes, int size);
void *SDIFimport_new(Symbol *, short argc, Atom *argv);
void SDIFimport_free(SDIFImport *x);
static void LookupMyBuffer(SDIFImport *x);
static void doclear(SDIFImport *x);
void SDIFimport_set(SDIFImport *x, Symbol *bufName);
void SDIFimport_import(SDIFImport *x, Symbol *fileNameIn);
static time_t scanLine(const char *line, MatrixData *row);
static time_t convertTime(int year, int day, int hhmm);
static SDIFmem_Frame createFrame(sdif_float64 time, const MatrixData *data);
void SDIFimport_debug(SDIFImport *x, long debugMode);
void SDIFimport_test(SDIFImport *x);


/* SDIF frame/matrix type */
#define SEP_FRAME_TYPE "xSEP"
#define SEP_MATRIX_TYPE "xSEP"


/* globals */
void *SDIFimport_class;
static Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup;


void main(fptr *fp) 
{
	SDIFresult r;
	
	post(FINDER_NAME " version " OBJECT_VERSION " by Ben \"Jacobs\"");
	post("Copyright © 2004 Regents of the University of California.");
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&SDIFimport_class, (method)SDIFimport_new, (method)SDIFimport_free,
			(short)sizeof(SDIFImport), 0L, A_GIMME, A_NOTHING);
	
	/* bind my methods to symbols */
	addmess((method)SDIFimport_import, "import", A_SYM, A_NOTHING);
	addmess((method)SDIFimport_debug, "debug", A_LONG, A_NOTHING);
	addmess((method)SDIFimport_test, "test", A_NOTHING);

  //  initialize SDIF libraries
	if (r = SDIF_Init()) {
		ouchstring("%s: Couldn't initialize SDIF library! %s", 
		           FINDER_NAME,
		           SDIF_GetErrorString(r));
    return;
	}
	
	if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
		post("¥ %s: Couldn't initialize SDIF memory utilities! %s", 
		     FINDER_NAME,
		     SDIF_GetErrorString(r));
    return;
	}
		
	if (r = SDIFbuf_Init()) {
		post("¥ %s: Couldn't initialize SDIF buffer utilities! %s", 
		     FINDER_NAME,
		     SDIF_GetErrorString(r));
		return;
	}
	
	/* list object in the new object list */
	finder_addclass("Data", FINDER_NAME);

	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");

#ifdef WIN_VERSION
	{
		unsigned long nan_bits[2]={0xffffffff, 0x7fffffff};
		NAN = *(double *)nan_bits;
	}
#endif
}


void *SDIFimport_new(Symbol *dummy, short argc, Atom *argv)
{
	SDIFImport *x;

	x = newobject(SDIFimport_class);
	
	x->t_outlet = bangout(x);
	
	doclear(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			post("¥ %s: argument must be name of an SDIF-buffer", FINDER_NAME);
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}
	
	return (x);
}


void SDIFimport_free(SDIFImport *x) 
{
}


void *my_getbytes(int numBytes) 
{
    if (numBytes > SHRT_MAX) 
    {
    	return 0;
    }
    return (void *) getbytes((short) numBytes);
}


static void LookupMyBuffer(SDIFImport *x) {
#ifdef THE_S_THING_KLUDGE
	void *thing;
	
	thing = x->t_bufferSym->s_thing;
	
	if (thing) {
		if (ob_sym(thing) = ps_SDIFbuffer) {
			x->t_buffer = thing;
			return;
		}
	}
	x->t_buffer = 0;
#else
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		x->t_buffer = 0;
	} else {
		x->t_buffer = (*f)(x->t_bufferSym);
	}
#endif

  //  get access to the SDIFbuf_Buffer instance
  if (x->t_buffer)
    x->t_buf = (x->t_buffer->BufferAccessor)(x->t_buffer);
  else
    x->t_buf = NULL;
}


void my_freebytes(void *bytes, int size) 
{
	freebytes(bytes, (short) size);
}


void doclear(SDIFImport *x) 
{
	x->t_debug = 0;
}


void SDIFimport_set(SDIFImport *x, Symbol *bufName) {
	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("¥ SDIF-tuples: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
}


/*
On Fri, 27 Feb 2004 16:19:01 -0800
"Laura Peticolas" <laura@ssl.berkeley.edu> wrote:

> I have placed data from the Helios Mission in
> ftp://cse.ssl.berkeley.edu/sounds/data/.  These data files are ascii
> files.  The name of the files represents the type of data (SEP, see next
> paragraph), the satellite number (there were two satellites), and the
> Carrington Rotation.  For example sep1_1622 and sep2_1622 are SEP data
> from the two satellites during the Carrington Rotation 1622.  A
> Carrington Rotation is a period of 27.3 days, representing one full
> rotation of the Sun as seen from the Earth. Carrington Rotation numbers
> are arbitrarily referenced to the time the term came into use, on
> November 9, 1853. The Helios mission sent two spacecraft out into space
> to study the Sun and the solar wind from two perspectives at the same
> time.  You can find out much more about the Helios mission here:
> http://www.linmpi.mpg.de/english/projekte/helios/
>  
>  The Solar Energetic Particle (SEP) data are very high energy protons
> that are flowing from the sun faster than the average solar wind
> particles are traveling.  The detector on the Helios satellites measure
> the proton "Intensity", otherwise known as flux (units = # of
> protons/(cm2 sr s MeV)).  This intensity is a type of density flux and
> it is the number of protons that hit the detector per square cm per
> second per solid angle per energy.    The detectors measure protons over
> 6 different energy ranges (see above) from 3-56 MeV energies.  The data
> is given in 30 minute time intervals and the columns are (in this
> order): 
>   
> Column 1:	 verse number; ignore	
> Columns 2-4: 	 Start time year, day-of-year, time (HHMM)	
> Columns 5-7: 	 Stop time year, day-of-year, time (HHMM)	
> Column 8:	 Intensity of 3-6 MeV protons	 
> Column 9:	 Error for Column 8	
> Column 10:	 Intensity of 6-11 MeV protons	 
> Column 11:	 Error for Column 10	
> Column 12:	 Intensity of 11-21 MeV protons	 
> Column 13:	 Error for Column 12	
> Column 14:	 Intensity of 20-30 MeV protons	 
> Column 15:	 Error for Column 14	
> Column 16:	 Intensity of 32-45 MeV protons	 
> Column 17:	 Error for Column 16	
> Column 18:	 Intensity of 45-56 MeV protons	
> Column 19:	 Error for Column 18	
>  
> There are many problems with the data with -1s and 0s.  It is probably
> smart to interpolate over these for now.  When we get data from the
> STEREO mission (launch in 2005), the data will be much more continuous.
*/

void SDIFimport_import(SDIFImport *x, Symbol *fileNameIn) 
{
  char fni[1024];
  short fiPath;
  long fiType;
  PATH_SPEC fiSpec;
  FILE *fi;
	int i;

	post("** SDIFimport_import (%p, %s)", x, fileNameIn->s_name);

  LookupMyBuffer(x);
  if(!(x->t_buf))
  {
    error("%s: can't find buffer", FINDER_NAME);
    return;
  }
  
  //  open input file (mac-only stuff to handle MSL path problems)
  strncpy(fni, fileNameIn->s_name, sizeof(fni));
  if(0 != locatefile_extended(fni, &fiPath, &fiType, 0L, 0))
  {
    error("%s: problem opening file \"%s\" for reading", FINDER_NAME, fileNameIn->s_name);
    goto finish;
  }
  
#ifdef WIN_VERSION
    {
		short maxErr;
		char fullpath[256];
		char conformed[256];
		maxErr = path_topathname(fiPath, fni, fullpath);
		if (maxErr) {
			error("%s: path_topathname returned error code %d - can't open %s", 
				  FINDER_NAME, maxErr, fileNameIn->s_name);
			goto finish;
		}
		maxErr = path_nameconform(fullpath, conformed, PATH_STYLE_NATIVE, PATH_TYPE_ABSOLUTE);
		if (maxErr) {
			error("%s: path_nameconform returned error code %d - can't open %s", 
				  FINDER_NAME, maxErr, fileNameIn->s_name);
			goto finish;
		}
		fi = fopen(conformed, "rb");
		if (fi == NULL) {
			error("%s: fopen returned NULL; can't open %s", FINDER_NAME, fileNameIn->s_name);
			goto finish;
		} 
	}
#else
/* Macintosh version */

#define PATH_SPEC_MEANS_FSSPEC
#ifdef PATH_SPEC_MEANS_FSSPEC
  if (0 != path_tospec(fiPath, fni, &fiSpec))
  {
    error("%s: problem opening file \"%s\" for reading (path_tospec)",
		  FINDER_NAME, fileNameIn->s_name);
    goto finish;
  }
  
  if(!(fi = FSp_fopen(&fiSpec, "r")))
  {
    error("%s: problem opening file \"%s\" for reading (FSp_fopen)", 
		  FINDER_NAME, fileNameIn->s_name);
    goto finish;
  }
 
#else 	
#error What do I do with a PATH_SPEC?	
#endif /* PATH_SPEC_MEANS_FSSPEC */
#endif /* WIN_VERSION */


 
	//  loop to read lines / write frames
  for(i = 0; TRUE; ++i)
  {
  	char line[1024];
  	sdif_float64 t;
  	SDIFmem_Frame f;
  	MatrixData row;
  	SDIFresult r;

    //  read line from input file
    if(!fgets(line, sizeof(line), fi))
      break;
    if(feof(fi))
      break;
    
    //  scan input line, get number of seconds since 1/1/1970
    if(-1 == (t = scanLine(line, &row)))
    {
      //  bail if bad input line
      error("%s: illegal time value on line %d of file \"%s\"", FINDER_NAME, i + 1, fileNameIn->s_name);
      break;
    }
    
    //  create a new frame
    if(!(f = createFrame(t, &row)))
    {
      error("%s: out of memory while reading file \"%s\"", FINDER_NAME, fileNameIn->s_name);
      break;
    }
    
    //  add frame to buffer (if frame already exists at same time, replace it)
    r = SDIFbuf_InsertFrame(x->t_buf, f, ESDIF_INSERT_REPLACE);
    if(r != ESDIF_SUCCESS)
    {
      error("%s: problem adding frame at time %f in file \"%s\"", FINDER_NAME, t, fileNameIn->s_name);
      break;
    }
  }
  
  //  close input file
  fclose(fi);
  
finish:  
  //  finish up -- send bang, whether successful or not
  outlet_bang(x->t_outlet);
}


//  scan one input line, fill in row data struct
//
//  return time as seconds since 1/1/1970
//  return -1 if year out of range

time_t scanLine(const char *line, MatrixData *row)
{
  int dummy;
  int start_year;
  int start_day;
  int start_hhmm;

  //  extract data from input line
  sscanf(
    line,
    "%d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f",
    &dummy,
    &start_year,
    &start_day,
    &start_hhmm,
    &dummy,
    &dummy,
    &dummy,
    &row->row[0].intensity,
    &row->row[0].intensityErr,
    &row->row[1].intensity,
    &row->row[1].intensityErr,
    &row->row[2].intensity,
    &row->row[2].intensityErr,
    &row->row[3].intensity,
    &row->row[3].intensityErr,
    &row->row[4].intensity,
    &row->row[4].intensityErr,
    &row->row[5].intensity,
    &row->row[5].intensityErr
    );
  
  //  convert cells where intensity == -1 to NaN
  if(row->row[0].intensity == -1)
    row->row[0].intensity = NAN;
  if(row->row[1].intensity == -1)
    row->row[1].intensity = NAN;
  if(row->row[2].intensity == -1)
    row->row[2].intensity = NAN;
  if(row->row[3].intensity == -1)
    row->row[3].intensity = NAN;
  if(row->row[4].intensity == -1)
    row->row[4].intensity = NAN;
  if(row->row[5].intensity == -1)
    row->row[5].intensity = NAN;

  //  return number of seconds since 1/1/1970
  return convertTime(start_year, start_day, start_hhmm);
}


//  find seconds elapsed from 1/1/1970 til specified time
//
//  (year must be on valid unix time_t range: 1900-2038)
//  (day counted since 1/1 of current year, 0-based)
//  (time specified in HHMM format)
// 
//  return -1 if year out of range

time_t convertTime(int year, int day, int hhmm)
{
  struct tm tm;
  time_t year_t;

  //  find seconds elapsed from 1/1/1970 til 1/1 of current year 
  tm.tm_sec = 0;
  tm.tm_min = 0;
  tm.tm_hour = 0;
  tm.tm_mday = 1;
  tm.tm_mon = 0;
  tm.tm_year = year - 1900;
  tm.tm_wday = 0;
  tm.tm_yday = 0;
  tm.tm_isdst = 0;
  year_t = mktime(&tm);
  
  //  return error if year out of range
  if(year_t == -1)
    return -1;

  //  add seconds elapsed since 1/1 of current year
  return year_t + (86400 * day) + (3600 * (hhmm / 100)) + (60 * (hhmm % 100));
}


static SDIFmem_Frame createFrame(sdif_float64 time, const MatrixData *data)
{
  SDIFmem_Frame f;
  SDIFmem_Matrix m;
  int i, j;
  SDIFresult r;
  sdif_float32 *dp = (sdif_float32 *)data;
  
  //  create frame
  f = SDIFmem_CreateEmptyFrame();
  SDIF_Copy4Bytes(f->header.frameType, SEP_FRAME_TYPE);
  f->header.time = time;
  f->header.streamID = 1;

  //  create matrix
  if(!(m = SDIFutil_CreateMatrix(SEP_DATA_COLUMNS, SEP_DATA_ROWS, SEP_DATA_TYPE, SEP_MATRIX_TYPE)))
    ;
  
  //  copy data
  for(i = 0; i < SEP_DATA_ROWS; i++)
    for(j = 0; j < SEP_DATA_COLUMNS; j++)
      SDIFutil_SetMatrixCell(m, j, i, dp[(i * SEP_DATA_COLUMNS) + j]);
  
  //  attach matrix to frame
  if(ESDIF_SUCCESS != SDIFmem_AddMatrix(f, m))
  {
    SDIFmem_FreeFrame(f);
    SDIFmem_FreeMatrix(m);
    return NULL;
  }
  
  return f;
}


void SDIFimport_debug(SDIFImport *x, long debugMode)
{
	x->t_debug = debugMode;	
}


void SDIFimport_test(SDIFImport *x)
{
  char fn[1024];
  char p[1024];
  short path;
  OSType type;
  short result;
  
  result = open_dialog(fn, &path, (long *)&type, 0L, 0);
  
  result = path_topathname(path, fn, p);
  post("%s: open \"%s\"", FINDER_NAME, p);
}


