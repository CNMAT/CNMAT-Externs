/*
 * File: foflib.h
 * FOF implementation based on IIR representation
 * by Gerhard Eckel and Francisco Iovino, Feb 1994
 * (c) 1994 IRCAM
 */

 // MSP version Dudas Hack Summer 1998
 // changed max number of fofs to 256 just to give us some growing room
 //   (originally 40 - the max that an ispw processor could handle)
 //   this definition is found in foflib.h
 // commented out fof_error() in foflib.c - 
 //   we don't want to do unnecessary printing at audio interrupt!
 
#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DEFAULT_SRATE 44100	/* default sampling rate for initialization */
#define LIST_END ((void*)0)	/* end of list */
//#define MAX_FOFS 40		/* max # of fofs on the SIM at 32 kHz */
#define MAX_FOFS 256

typedef double t_fof_real;	/* fof calculation has to be in double */

typedef struct _fof_inputs {	/* fof inputs */
    t_fof_real f0;		/* center frequency in Hz */
    t_fof_real cf;		/* center frequency in Hz */
    t_fof_real bw;		/* band width in Hz */
    t_fof_real amp;		/* linear amplitude */
    t_fof_real tex;		/* tex (attack time) in s */
    t_fof_real debatt;		/* debatt (attenuation start time) in s */
    t_fof_real atten;		/* atten (attenuation time) in s */
    t_fof_real phase;		/* initial fof phase in radians */
    long changed;		/* flag indicating if inputs changed */
} t_fof_inputs;

typedef struct _ctl
{
	float x_state1;				
	float x_state2;				
	float x_gain;				
	float x_fbcoef1;			
	float x_fbcoef2;			
	float x_ffcoef0;			
	float x_ffcoef1;			
	float x_ffcoef2;			
} t_ctl;

typedef struct _fof_params {	/* fof parameters and pre-calculated values */
    t_fof_real cf2pi;		/* center frequency times two pi */
    t_fof_real bwpi;		/* band width times pi */
    t_fof_real piotex;		/* pi over tex */
    t_fof_real pioatten;	/* pi over atten */
    t_fof_real fpc;		/* center frequency pre-coefficient */
    t_fof_real bpc;		/* bandwidth pre-coefficient */
    t_fof_real tpc;		/* tex pre-coefficient */
    t_fof_real apc;		/* atten pre-coefficient */
    t_fof_real sr;		/* sampling rate in Hz */
    t_fof_real sro2;		/* nyquist frequency in Hz */
    t_fof_real sp;		/* sampling period in s */
    t_fof_real pi;		/* pi (initialized with 4 * atan(1.)) */
    t_fof_real twopi;		/* two pi */
    t_fof_real piosr;		/* pi over sampling rate */
} t_fof_params;

typedef struct _fof_coefs {	/* coefficients for all fof parts */
    t_fof_real ec1;		/* exp coefficient 1 */
    t_fof_real ec2;		/* exp coefficient 2 */
    t_fof_real tc;		/* tex coefficient */
    t_fof_real ac;		/* atten coefficient */
} t_fof_coefs;

typedef struct _fof_state {	/* the state of a fof part */
    long count;			/* samples to go */
    t_fof_real y1;		/* sample t - 1 */
    t_fof_real y2;		/* sample t - 2 */
} t_fof_state;

typedef struct _fof_entry {	/* entry in fof list */
    struct _fof_entry *next;	/* list link (has to be the first component) */
    t_fof_coefs coefs;		/* fof coefficients */
    t_fof_state exp;		/* fof exp state (exp. decaying sine) */
    t_fof_state tex;		/* fof tex state (attack part) */
    t_fof_state atten;		/* fof atten state (attenuation part) */
    long start;			/* start sample in current block */
} t_fof_entry;

typedef struct _fof_loco {	/* locomotive for fof list */
    struct _fof_entry *next;	/* list link (has to be the first component) */
} t_fof_loco;

typedef struct _fof_list {	/* fof list */
    t_fof_loco free;		/* locomotive for free list */
    t_fof_loco used;		/* locomotive for used list */
    long maxfofs;		/* maximum number of entries (<= MAX_FOFS) */
    t_fof_entry fofs[MAX_FOFS];	/* entry space */
} t_fof_list;

typedef struct _fof {		/* fof control structure */
    t_fof_inputs inputs;	/* fof inputs */
    t_fof_params params;	/* fof parameters */
    t_fof_coefs coefs_cache;	/* coefficient cache */
    t_fof_real f0_phase;	/* current f0 phase (0 .. params.sr) */
    t_fof_list list;		/* fof list */
} t_fof;


typedef struct _sigfofs {
    t_pxobject x_obj;
    t_fof *ctlp;
    t_fof ctl;
    t_sample cf, bw, amp, tex, debatt, atten;
    short b_connected[6];
} t_sigfofs;


static void fof_init(t_fof *x);
static void fof_init_list(t_fof_list *x);
static void fof_update(t_fof_entry *entry, t_fof *x);
static void fof_onset(t_fof_entry *e, t_fof *x, long start, t_fof_real d1);
static void fof_trigger(t_fof *x, long start, t_fof_real delta);
static void fof_run_phasor(t_sigfofs *x, t_sample *in, t_sample *out, long n);
static void fof_run_phasor_nosig(t_sigfofs *x, t_sample in, t_sample *out, long n);
static int fof_run_one(t_fof_entry *x, t_sample *out, long n);
static void fof_run_all(t_sigfofs *x, t_sample *out, long n);
static t_int *fof_perform(t_int *w);
static t_int *fof_perform_nosig(t_int *w);
static void fof_error(void); /* to be supplied from user */
