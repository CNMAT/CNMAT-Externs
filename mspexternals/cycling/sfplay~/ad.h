#ifndef _AD_H_
#define _AD_H_

#ifdef MAC_VERSION

#if TARGET_API_MAC_CARBON
#define AD_DEFAULT_DRIVERNAME "CoreAudio"
#else
#define AD_DEFAULT_DRIVERNAME "Sound Manager"
#endif
#endif

#ifdef WIN_VERSION
#define AD_DEFAULT_DRIVERNAME "ad_mme" // "ad_mme" or "ad_asio" or "ad_directsound"
#endif WIN_VERSION

#define AD_CONFIG_PATCHERNAME "DSP Status"
#define AD_DEFAULT_SR 44100
#define AD_DEFAULT_LOGICALCHANS 512
#define AD_DEFAULT_TAKEOVER 0
#define AD_MEASURE_MIN 1024
#define AD_DEFAULT_IOVS 512
#define AD_DEFAULT_SIGVS 64

#define AD_MAX_SUBNAMES 256		// number of subdrivers that can be listed by an ad object

#define AD_MAX_SRCHOICES	(32)
#define AD_MAX_IOVSCHOICES	(32)
#define AD_MAX_SIGVSCHOICES (32)

#ifdef MAC_VERSION
#define AD_ALIGN_16
#endif /*MAC_VERSION*/

#ifdef WIN_VERSION
#define AD_ALIGN_16 __declspec(align(16))
#endif /*WIN_VERSION*/

enum {
	AD_VS_IO,
	AD_VS_SIG,
	AD_SR
};

#define ADTIMEINFO_SYMBOL		"__adtimeinfo__"

typedef struct _adtimeinfo {
	double t_timenow;
	double t_inputtime;
	double t_outputtime;
} t_adtimeinfo;

// information on channels, used in the channel remapping view of the DSP Status window 

typedef struct _chandefblk 
{ 
	t_symbol *c_name;		// name of this channel 
	long c_input;		// input or output? 
	long c_index;		// number of this channel (i.e., what user sees, usually starting at 1) 
	long c_resolution;	// sample size (i.e., 24 bits) 
	long c_flags;		// additional info (digital/analog etc. could be stored here 
} t_chandefblk; 
 
// header common to all ad objects 
 

 
typedef struct _adobj 
{ 
	t_object a_ob; 
	t_symbol *a_name; 
	long a_numins; 
	long a_numouts; 
	AD_ALIGN_16 float **a_input;			// (remapped) interface to adc~ objects 
	AD_ALIGN_16 float **a_output;			// (remapped) interface to dac~ objects 
	long *a_inenabled;		// input enables 
	long *a_outenabled;		// output enables 
	long a_size;			// replaces SYS_MAXBLKSIZE (I think) 
	long a_iovs;			// current IO vector size 
	long a_sigvs;			// current signal vector size (maintained by host) 
	double a_sr;			// current sampling rate 
	long a_takeon;			// is takeover mode on (maintained by host) 
	void (*a_process)(struct _adobj *x, long n); 	// the process callback (avoids any new DLL problems) 
	long a_unused1;			// for driver use 
	long a_unused2;			// for host use 
	double a_mspersigv;		// ms per signal vector
} t_adobj; 
 typedef void (*t_processmethod)(t_adobj *x, long n);  
// option UI modes 
 
enum { 
	AD_OPTION_MENU = 0, 
	AD_OPTION_SWITCH = 1 
}; 
 
// option flags 
 
enum { 
	AD_RESTART_ON_CHANGE = 1, 
	AD_MAX_OPTION_CHOICES = 32 
}; 
 
// driver property flags 
 
enum { 
	AD_PROP_NONREALTIME = 1 
}; 
 
typedef struct _optiondefblk 
{ 
	t_symbol *o_name;		// name of the option 
	t_symbol *o_message;	// sent to object to change state 
	t_symbol *o_label;	// label 
	long o_uimode;		// not used yet, but would select type of UI used for option 
	long o_flags;		// if option is switched, does audio need to be restarted before/after? 
	t_symbol **o_choices;	// caller allocated space for 32 symbols for choices for this option 
	long o_numchoices;	// you return how many there actually are 
} t_optiondefblk; 

// all drivers

typedef struct _driverinfo
{
	t_symbol *d_name;
	struct _driverinfo *d_child;	// points to subdriver list (uses d_next after that, unless sub-sub)
	struct _driverinfo *d_parent;	// parent driver if applicable
	long d_disabled;				// if error occurred loading it, disable it? (not using this for now)
	long d_index;					// menu insertion index
	struct _driverinfo *d_next;
	t_symbol *d_filename;			// the actual filename for the driver. 
} t_driverinfo;

typedef struct _messlink
{
	t_symbol *s_sym;
	struct _messlink *s_next;
	short s_argc;
	t_atom s_argv[1];	// variable length
} t_messlink;

typedef void (*pollmethod)(void);
typedef void (*setmethod)(long way);

typedef struct _takeover
{
	pollmethod poll_method;
	setmethod set_method;
	double time;
	double incr;
	double inverse;
	double conversion;
	double invconversion;
	long *ct;
	char *pisr;
	char *pep;
} t_takeover;

// object bound to DSP

typedef struct _ad {
	struct object d_ob;
	t_adobj *d_driver;
	t_chandefblk *d_chans;
	t_optiondefblk *d_options;
	short *d_inmap;
	short *d_outmap;
	long d_numlogicalchans;
	long d_numoptions;
	long d_numsrchoices;
	long d_numiovschoices;
	long d_numsigvschoices;
	long d_srchoices[AD_MAX_SRCHOICES];
	long d_iovschoices[AD_MAX_IOVSCHOICES];
	long d_sigvschoices[AD_MAX_SIGVSCHOICES];
	t_driverinfo *d_head;
	t_symbol *d_drivername;
	t_symbol *d_subname;
	long d_prefsr;
	long d_prefiovs;
	long d_prefsigvs;
	long d_preftake;
	t_takeover *d_top;
	void *d_patcher;		// point to the "DSP Status" config patcher
	long d_debug;
	double d_cpu;			// now an attribute
	long d_funcalls;		// global funcall count	-- now an attribute
	long d_sigsused;		// global siguse count -- now an attribute
	double d_msbegin;		
	double d_msinterval;	// iovs microsecond count
	t_messlink *d_overlist;
	Boolean d_override;
	Boolean d_stopping;		// allows immediate turnoff of the dsp
	double d_cpulimit;		// now an attribute
	long d_optimize;		// now an attribute
	double d_1overtotal;	// used in CPU pct calculation
	method d_postprocess;
	long d_sampleinterval;	// used in CPU pct calculation
	long d_intervalcount;
	void *d_exclpatcher;
	void *d_cpu_poller;
	t_object *d_statuscontroller;	// notify this object when options change
	method d_preprocess;
} t_ad;

 
#if defined(WIN_VERSION)

#define C74_CW_DEFAULT		(_CW_DEFAULT | _RC_CHOP)	// let's make chop the default mode. 

typedef struct _controlfp_info {
	long s_control87_val_target;
	long s_fpu_exceptions;
	long s_fpu_underflow;
	long s_fpu_overflow;
	long s_fpu_zerodivide;
	long s_fpu_invalidoperation;
	long s_fpu_denormal;
	void* s_fpu_qelem;  // if this is non-null then the qelem is triggered on fpu exception.
} t_controlfp_info;
#endif

#endif // _AD_H_

