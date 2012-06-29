#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "ext_obex.h"
#include "ext_systhread.h"
#include "ext_critical.h"
#include "ext_atomic.h"

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
	MAXCHAN = 4
};

enum {
	bi_basefreq = 0,
	bi_detune,
	bi_lowfreq,
	bi_hifreq,
	bi_lowvel,
	bi_hivel,
	bi_gain,
	bi_numparams
};


/**	Data structure for the buffer~ object.
	@ingroup buffers
*/
typedef struct _buffer
{
	t_object b_obj;		///< doesn't have any signals so it doesn't need to be pxobject
	long b_valid;		///< flag is off during read replacement or editing operation
	float *b_samples;	///< stored with interleaved channels if multi-channel
	long b_frames;		///< number of sample frames (each one is sizeof(float) * b_nchans bytes)
	long b_nchans;		///< number of channels
	long b_size;		///< size of buffer in floats
	float b_sr;			///< sampling rate of the buffer
	float b_1oversr;	///< 1 / sr
	float b_msr;		///< sr * .001
	// Mac-specific stuff
	float *b_memory;	///< pointer to where memory starts (initial padding for interp)
	t_symbol *b_name;	///< name of the buffer
	short b_vol;
	short b_space;
	// looping info (from AIFF file)
	long b_susloopstart;	///< looping info (from AIFF file) in samples
	long b_susloopend;		///< looping info (from AIFF file) in samples
	long b_relloopstart;	///< looping info (from AIFF file) in samples
	long b_relloopend;		///< looping info (from AIFF file) in samples
	// instrument info (from AIFF file)
	short b_inst[bi_numparams];
	// window stuff
	void *b_wind;
	double b_pixperfr;
	double b_frperpix;
	long b_imagesize;
	Point b_scroll;
	long b_scrollscale;
	long b_selbegin[MAXCHAN];
	long b_selend[MAXCHAN];
	long b_zoom;
	long b_zim[11];
	void *b_mouseout;
	long b_format;			///< 'AIFF' or 'Sd2f'
	t_symbol *b_filename;	///< last file read (not written) for readagain message
	long b_oldnchans;		///< used for resizing window in case of # of channels change
	void *b_doneout;
	long b_outputbytes;		///< number of bytes used for output sample (1-4)
	long b_modtime;			///< last modified time ("dirty" method)
	struct _buffer *b_peer;	///< objects that share this symbol (used as a link in the peers)
	Boolean b_owner;		///< b_memory/b_samples "owned" by this object
	long b_outputfmt;		///< sample type (A_LONG, A_FLOAT, etc.)
	t_int32_atomic b_inuse;	///< objects that use buffer should ATOMIC_INCREMENT / ATOMIC_DECREMENT this in their perform
	void *b_dspchain;		///< dspchain used for this instance
	long b_padding;			///< amount of padding (number of samples) in b_memory before b_samples starts
	long b_paddingchanged;	///< flag indicating that b_padding has changed and needs to be allocated
	t_object *b_jsoundfile;	///< internal instance for reading/writing FLAC format
	t_systhread_mutex b_mutex; ///< mutex to use when locking and performing operations anywhere except perform method
	long b_wasvalid;		///< internal flag used by replacement or editing operation
} t_buffer;

typedef struct _buffer_info
{
	t_symbol *b_name;	///< name of the buffer
	float *b_samples;	///< stored with interleaved channels if multi-channel
	long b_frames;		///< number of sample frames (each one is sizeof(float) * b_nchans bytes)
	long b_nchans;		///< number of channels
	long b_size;		///< size of buffer in floats
	float b_sr;			///< sampling rate of the buffer
	long b_modtime;		///< last modified time ("dirty" method)
	long b_rfu[57];		///< reserved for future use (total struct size is 64x4 = 256 bytes)
} t_buffer_info;

#define BUFWIND(x) ((t_wind *)(x->b_wind))

BEGIN_USING_C_LINKAGE

// buffer_perform functions to replace the direct use of 
// atomics and other buffer state flags from the perform method
t_max_err buffer_perform_begin(t_buffer *b);
t_max_err buffer_perform_end(t_buffer *b);

// utility function for getting buffer info without needing to know entire buffer struct
t_max_err buffer_getinfo(t_buffer *b, t_buffer_info *info);

// the following functions are not to be called in the perform method
// please use the lightweight buffer_perform methods 

// use buffer_edit functions to collapse all operations of 
// locking heavy b_mutex, setting b_valid flag, 
// waiting on lightweight atomic b_inuse, etc. 
t_max_err buffer_edit_begin(t_buffer *b);
t_max_err buffer_edit_end(t_buffer *b, long valid);  // valid 0=FALSE, positive=TRUE, negative=RESTORE_OLD_VALID (not common)

// low level mutex locking used by buffer_edit fucntions. 
// use only if you really know what you're doing.
// otherwise, use the buffer_edit functions 
// if you're touching a t_buffer outside perform 
t_max_err buffer_lock(t_buffer *b);
t_max_err buffer_trylock(t_buffer *b);
t_max_err buffer_unlock(t_buffer *b);

// low level utilities used by buffer_edit functions
// use only if you really know what you're doing.
// otherwise, use the buffer_edit functions 
// if you're touching a t_buffer outside perform 
t_buffer *buffer_findowner(t_buffer *x);
long buffer_spinwait(t_buffer *x);
long buffer_valid(t_buffer *x, long way);


END_USING_C_LINKAGE


#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#endif // #ifndef _BUFFER_H_
