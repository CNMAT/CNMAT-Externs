// sf2.h - max sound format header file

#ifndef _SF2_H_
#define _SF2_H_

#define USE_ASYNCFILE_CODE // always use async on OSX and Win

#include "sf_byteorder.h"
#include "ext_asyncfile.h"

#ifdef WIN_VERSION
#include "AIFF.h"
#endif // WIN_VERSION

#define SF_FILETYPE_AIFF 		FOUR_CHAR_CODE('AIFF')
#define SF_FILETYPE_SD2 		FOUR_CHAR_CODE('Sd2f')
#define SF_FILETYPE_WAVE 		FOUR_CHAR_CODE('WAVE')
#define SF_FILETYPE_AU 			FOUR_CHAR_CODE('ULAW')
#define SF_FILETYPE_RAW 		FOUR_CHAR_CODE('DATA')

#define SF_SAMPTYPE_INT			FOUR_CHAR_CODE('NONE')
#define SF_SAMPTYPE_UINT		FOUR_CHAR_CODE('UNSN')
#define SF_SAMPTYPE_FL32		FOUR_CHAR_CODE('FL32')
#define SF_SAMPTYPE_FL64		FOUR_CHAR_CODE('FL64')
#define SF_SAMPTYPE_MULAW		FOUR_CHAR_CODE('ULAW')
#define SF_SAMPTYPE_ALAW		FOUR_CHAR_CODE('ALAW')

#define SF_SAMPTYPE_LE			FOUR_CHAR_CODE('sowt')		// little endian twos backwards

//open flags
#define SFILE_DEFAULT			0x0000L
#define SFILE_SYNC				0x0001L //default
#define SFILE_NO_CACHE			0x0002L
#define SFILE_ASYNC				0x0010L

typedef struct _sfile
{
	struct _sfile *s_next;
	t_symbol *s_name;
	long s_srcchans;
	long s_offset;	// offset in bytes into file
	long s_size;	// file size in bytes
	long s_frames;	// file size in frames
	long s_sampsize;	// sample size in bytes
	long s_busy;	// async file read happening
	long s_refcount;	// how many cues are using it?
	double s_srate;
	unsigned long s_comptype;	//compression type
	long s_byteswap;	//0=native, 1=swap
	t_filehandle s_refnum;		// open file refnum
	short s_open;
	// looping info (from AIFF file)
	long s_susloopstart;	// in samples (frames)
	long s_susloopend;		// in samples (frames)
	t_asyncfile *s_asyncfile;
} t_sfile;

typedef struct _cue
{
	struct _cue *c_next;
	long c_no;			// cue number
	t_sfile *c_parent;	// what file it's in
	long c_byteoffset;	// relative (+ s->s_offset) byte start of cue
	long c_end;			// absolute point in file where cue ends (bytes)
	//long c_where;
	short *c_data;		// preloaded data
	long c_pds;			// how much preloaded data
	long c_datasize;	// size of c_data (for freeing only)
	float c_start;		// where it starts in the file (in ms)
	float c_endpoint;	// size (in ms), 0 means play to end
	long c_valid;		
	long c_busy;
	
	//below is the new buffering scheme
	//if c_endpoint is zero bidirectional buffering buffers before the cue startpoint
	//if c_endpoint is nonzero bidirectional buffering buffers at the end of the cue
	long c_bidir;		// flag
	short *c_revdata;	// preloaded data
	long c_revpds;		// how much preloaded data
	long c_revdatasize;	// size of c_revdata (for freeing only)
	double c_speed;		// playback speed
	
	// sustain loop stuff
	long c_susloop;			// flag
	short *c_susdata;		// preloaded data
	long c_suspds;			// how much preloaded data
	long c_susdatasize;		// size of c_data (for freeing only)
	double c_susstart;		// where it starts in the file (in ms)
	double c_susendpoint;	// size (in ms), 0 means play to end
	double c_susend;		// where it ends in the file (in bytes)
	long c_susbyteoffset;	// relative (+ s->s_offset) byte start of cue
	
} t_cue;

typedef struct _sflist
{
	t_object l_obj;
	t_symbol *l_name;
	t_cue *l_cues;
	t_sfile *l_files;
	t_sfile *l_current;
	long l_flags;
	long l_ibufsize;
	long l_bufsize;
	t_cue l_one;				// cue associated with #1: beginning of file l_current
	void *l_report;
} t_sflist;

//sf_cue.c 
//using void * inplace of t_object *
void cue_init(void);
void cue_preload(void *x, t_symbol *s, short argc, t_atom *argv);
void cue_dopreload(void *x, t_symbol *s, short argc, t_atom *argv);
t_cue *cue_new(void *x, long number, t_symbol *file, double start, double endpoint, long bidir, double speed, long susloop, double susstart, double susendpoint);
t_cue *cue_find(t_cue *start, long number);
void cue_preload(void *x, t_symbol *s, short argc, t_atom *argv);
void cue_dopreload(void *x, t_symbol *s, short argc, t_atom *argv);
void cue_freedata(t_cue *c);
void cue_free(void *x, t_cue *me);


// sfile.c
t_sfile *sfile_new(void);
void sfile_free(t_sfile *s);
long sfile_open(t_sfile *s, t_symbol *name, short vol, unsigned long filetype, long flags);  //only pass in SF_FILETYPE_*
long sfile_searchmarkers(MarkerChunk *markers, short id);
void sfile_close(t_sfile *s);
long sfile_aiffopen(t_sfile *s, t_symbol *name, short vol, long flags);
long sfile_sd2open(t_sfile *s, t_symbol *name, short vol, long flags);
long sfile_auopen(t_sfile *s, t_symbol *name, short vol, long flags);
long sfile_waveopen(t_sfile *s, t_symbol *name, short vol, long flags);
void sfile_async_done(t_asyncfile_params *p);

long sfile_rawopen(t_sfile *s, t_symbol *name, short vol, long flags);
// sf_conversion.c
double sf_mulaw_decode(unsigned char ulaw); 
double sf_alaw_decode(unsigned char alaw); 
unsigned char sf_mulaw_encode(double f); 
unsigned char sf_alaw_encode(double f); 
long sf_float2samptype(float *in, void *out, long n, long skip, long bytes, 
		unsigned long samptype, short byteswap, double resamp, double *current);
long sf_float2samptype_interp(float *in, void *out, long n, long offset, long skip, long bytes, 
		unsigned long samptype, short byteswap, double resamp, double *current, double last);
// sf_aiff.c
long sf_aiff_parse(char *header, long *offset, long *size, long *nchans, long *ssize,
	long *srate, unsigned long *samptype, void *chunk, void *markers, long *swap);
long sf_aiff_writeheader(short refnum, long size, long nchans, 
		long ssize, long srate, unsigned long samptype); 
// sf_wave.c
long sf_wave_parse(char *header, long *offset, long *size, long *nchans, long *ssize,
	long *srate, unsigned long *samptype, void *chunk, void *markers);
long sf_wave_writeheader(short refnum, long size, long nchans, 
		long ssize, long srate, unsigned long samptype);
// sf_au.c
long sf_au_parse(char *header, long *offset, long *size, long *nchans, long *ssize,
	long *srate, unsigned long *samptype);
long sf_au_writeheader(short refnum, long size, long nchans, 
		long ssize, long srate, unsigned long samptype); 
// sf_sd2.c
long sf_sd2_setupresource(char *name, short refNum, short vol, long nchans, double sr, long sampbytes);

#endif //_SF2_H_


