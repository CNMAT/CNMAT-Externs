#include "ext.h"
#include "ext_path.h"
#include "ext_common.h"
#include "ext_obex.h"
#include "ext_strings.h"
#include "z_dsp.h"
#include "aqueue2.h"
#include "sf2.h"
#include "ext_sndfile.h"
#include "ad.h"
#include "ext_drag.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x)) 
#ifndef TRUE
#define TRUE 	1
#define FALSE 	0
#endif

#define SFPLAY_WAITTIME 2000

typedef t_asyncfile_params IOREQTYPE;


enum {
	SF_MAXCHANS = 32,
	SF_MAXVECS = 32,
	//SF_BUFSIZE = 16384,
	SF_BUFSIZE_DEF = 120960, 
	SF_BUFSIZE_QUANT = 20160, // 8factorial over two...ensures that we won't have problems wrapping 
	SF_MULT = 8,
	SF_ATOM_MULT = 4,
	SF_PRELOAD = 1,
	SF_MAXPEND = 128,
	SF_EMBED = 1,
	SF_WEIRDCUE = 18811,
	SF_MAX_SWITCH_PER_VECTOR = 16
};

typedef struct _sfplay
{
	t_pxobject p_obj;
	void *obex; // so I can instantiate it inside other objects
	float *p_vectors[SF_MAXVECS]; // output signal vectors
	long p_outchans;			// number of output channels
	t_cue *p_cues;				// list of current queues
	t_sfile *p_files;			// list of open files
	t_sfile *p_current;			// current file (assumed for seeking, preload, etc.
	t_cue *p_now;				// now playing this cue
	t_cue *p_next;				// cue to switch to in next vector
	t_cue p_one;				// cue associated with #1: beginning of file p_current
	long p_bufsize;				// preload buffer size
	long p_busy;				// async flag
	long p_where;				// position in current file
	t_aqueue *p_aq;				// data queue
	void *p_done;
	void *p_clock;
	t_symbol *p_bound;			// symbol to use for cue lookup, 0 if none
	long p_flags;				// embed etc.
	long p_pendcount;			// number of pending cues
	long p_ibufsize;			// bufsize arg for saving
	short p_pending[SF_MAXPEND];	// pending cues
	short p_status;				// playing state
	short p_opendeferred;		// currently waiting for deferred open msg
	double p_speed;				// varispeed
	double p_sr;
	long p_lastdir; 			// use for disk read to jump back or fwd in the file if we've switched directions
	long p_loop; 				// global loopflag
	long p_loopone; 			// loopflag for a specific cue (i.e. will only loop the cue specified
	
	short p_sustain;			// sustain loop flag (maybe also add a p_releaseloop?)
	short p_sustaining;			// so we know if we've gotten to the loop part
	double p_sloopstart;		// sustain loop start and end time in ms
	double p_sloopend;
	
	long p_posout;				// for position signal outlets
	long p_switch;				// safety for cue switch while there's an async read 
	long p_connected[2];		// signal input flags for cues, speed 
	long p_sc_last;				// memory for signal cue switching
	t_symbol *p_name;
	long p_async;				// are we realtime?
	long p_modout;				// modulo output(if srcchans<outchans, reduplicate)
	long p_process;				// in process, prevents file-reading
	long p_invalid;				// in reading
	IOREQTYPE *p_iorequest;		// for perform routine only. avoids memory allocation deadlock
	long p_looppalin;
	void *p_loopclock;
	void *p_updateclock;
	void *p_updateqelem;
	char p_linked;
} t_sfplay;

enum {
	SF_Idle = 0,
	SF_Playing,
	SF_Paused
};

t_int *sfplay_perform(t_int *w);
void sfplay_switch(t_sfplay *x, t_aqueue *aq, t_cue *c);
long sfplay_doread(t_sfplay *x, t_aqueue *aq, t_cue *c, long async, long reverse);

void sfplay_done(t_asyncfile_params *p);
void sfplay_done_rev(t_asyncfile_params *p);

void sfplay_dsp(t_sfplay *x, t_signal **sp, short *count);
void sfplay_doopen(t_sfplay *x, t_symbol *name);
void sfplay_open(t_sfplay *x, t_symbol *s);
void sfplay_dofclose(t_sfplay *x, t_symbol *s);
void sfplay_fclose(t_sfplay *x, t_symbol *s);
void sfplay_print(t_sfplay *x);
void sfplay_pause(t_sfplay *x);
void sfplay_resume(t_sfplay *x);
void sfplay_seek(t_sfplay *x, double start, double end);
void sfplay_doseek(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void sfplay_dint(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void sfplay_int(t_sfplay *x, long n);
void sfplay_float(t_sfplay *x, double f);
void sfplay_sigint(t_sfplay *x, long n); //for signal input cue switching
void sfplay_tick(t_sfplay *x);
void sfplay_speed(t_sfplay *x, double f);
void sfplay_loop(t_sfplay *x, long c);
void sfplay_loopone(t_sfplay *x, long c);

void sfplay_sustain(t_sfplay *x, long n);
void sfplay_setsustain(t_sfplay *x, double start, double end);

void sfplay_clear(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void sfplay_doclear(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void sfplay_free(t_sfplay *x);
void sfplay_freecues(t_sfplay *x);
void sfplay_freefiles(t_sfplay *x, Boolean freecurrent);
void sfplay_list(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void sfplay_assist(t_sfplay *x, void *b, long m, long a, char *s);
void sfplay_save(t_sfplay *x, void *w);
void sfplay_embed(t_sfplay *x, long n);
void sfplay_set(t_sfplay *x, t_symbol *s);
t_cue *sfplay_who(t_sfplay *x);
t_sflist *sfplay_lookup(t_symbol *s);
void sfplay_anything(t_sfplay *x, t_symbol *s, short argc, t_atom *argv);
void *sfplay_new(t_symbol *s, short argc, t_atom *argv);
short sfplay_spinwait(long *p);

void sfplay_doopenraw(t_sfplay *x, t_symbol *name);
void sfplay_openraw(t_sfplay *x, t_symbol *name);

double sfplay_cue_samps_left(t_sfplay *x, double speed);
void sfplay_calc_pos(t_sfplay *x, t_sfile *s, long n, double speed, t_float *poshi, t_float *poslo);
//force header functions
void sfplay_srate(t_sfplay *x, double f);
void sfplay_srcchans(t_sfplay *x, long n);
void sfplay_offset(t_sfplay *x, long n);
void sfplay_samptype(t_sfplay *x, t_symbol *s, long swap);
//renames sfile stuff that only belongs to sfplay
t_sfile *sfplay_sfile_new(t_sfplay *x, t_symbol *name, short vol, long numtypes, long *typelist);
t_sfile *sfplay_sfile_newraw(t_sfplay *x, t_symbol *name, short vol, long type);
t_sfile *sfplay_sfile_find(t_sfplay *x, t_symbol *name);
long sfplay_sfile_read(t_sfplay *x, t_sfile *s, long start, long bytes, Ptr dst, short async);
void sfplay_sfile_unrefer(t_sfplay *x, t_cue *qc, t_sfile *s);
void sfplay_sfile_free(t_sfplay *x, t_sfile *s, Boolean freecues);
//cue related
t_sfile *sfplay_getcurrentfile(t_sfplay *x);
long sfplay_getbufsize(t_sfplay *x);
t_cue *sfplay_getcues(t_sfplay *x);
void sfplay_setcues(t_sfplay *x, t_cue *p);
//sfinfo related
long sfplay_getsfinfo(t_sfplay *x, t_sndfileinfo *info);
short symbol_bind(t_symbol *s, t_object *x);
void symbol_unbind(t_symbol *s, t_object *x);
void sfplay_name(t_sfplay *x, t_symbol *s);

void sfplay_modout(t_sfplay *x, long c);

// used by jit.qt.movie
long sfplay_getvalid(t_sfplay *x);
void sfplay_speedgimme(t_sfplay *x, t_symbol *s, long ac, t_atom *av);
void sfplay_cuereport(t_sflist *x, long err, t_cue *c, void *rfu);
void sfplay_looppalin(t_sfplay *x, long n);
void sfplay_looptick(t_sfplay *x);

// used by playbar
// controller interface

double sfplay_postoms(t_sfplay *x, double pos);
double sfplay_mstopos(t_sfplay *x, double ms);
double sfplay_curms(t_sfplay *x);
void sfplay_getplaystate(t_sfplay *x, long *play, double *pos);
void sfplay_toggleplay(t_sfplay *x);
void sfplay_pos(t_sfplay *x, double pos);
void sfplay_ff(t_sfplay *x);
void sfplay_rw(t_sfplay *x);
void sfplay_updatetick(t_sfplay *x);
void sfplay_ctrl(t_sfplay *x);
void sfplay_controller(t_sfplay *x);

t_max_err sfplay_audiofile(t_sfplay *x, void *attr, long ac, t_atom *av);
t_max_err sfplay_get_audiofile(t_sfplay *x, void *attr, long *ac, t_atom **av);

long sfplay_acceptsdrag(t_sfplay *x, t_object *drag, t_object *view);

static t_class *sfplay_class;
static t_symbol *ps_X, *ps_nothing, *ps_sflist;
static t_symbol *ps_8bit,*ps_16bit,*ps_24bit,*ps_32bit,*ps_float,*ps_double,*ps_mulaw,*ps_alaw;
static t_symbol *ps_preload, *ps_preload_error, *ps_cue;
static t_symbol *ps_typedmess, *ps_atomarray, *ps_nobox;
static t_symbol *ps_loopreport, *ps_done;
static t_symbol *ps_matchdragrole;
static t_symbol *ps_audiofile, *ps_update, *ps_controller;
static t_symbol *ps_dropaction_addcustom;
static t_symbol *ps_dropaction_addconnectedmessage;

t_asyncfile_cb sfplay_async_cb=NULL,sfplay_async_cb_rev=NULL,sfile_async_cb=NULL;


int main(void)
{
	t_class *c;

	c = class_new("sfplay~", (method)sfplay_new, (method)sfplay_free, (short)sizeof(t_sfplay), 
		0L, A_GIMME, 0);
	class_obexoffset_set(c, calcoffset(t_sfplay, obex));
	
	class_addmethod(c, (method)sfplay_dsp, 		"dsp", 		A_CANT, 0);
	class_addmethod(c, (method)sfplay_open, 	"open", 	A_DEFSYM, 0);
	class_addmethod(c, (method)sfplay_openraw, 	"openraw", 	A_DEFSYM, 0);
	class_addmethod(c, (method)sfplay_fclose, 	"fclose", 	A_DEFSYM, 0);
	class_addmethod(c, (method)sfplay_int, 		"int", A_LONG, 0);
	class_addmethod(c, (method)sfplay_float, 	"float", A_FLOAT, 0);
	class_addmethod(c, (method)sfplay_list, 	"list", 	A_GIMME, 0);
	class_addmethod(c, (method)sfplay_print, 	"print" , 	0);
	class_addmethod(c, (method)sfplay_pause, 	"pause", 	0);
	class_addmethod(c, (method)sfplay_resume, 	"resume", 	0);
	class_addmethod(c, (method)cue_preload, 	"preload", 	A_GIMME, 0);
	class_addmethod(c, (method)sfplay_clear, 	"clear", 	A_GIMME, 0);
	class_addmethod(c, (method)sfplay_seek, 	"seek", 	A_FLOAT, A_DEFFLOAT, 0);
	class_addmethod(c, (method)sfplay_speed, 	"speed", 	A_FLOAT, 0);
	class_addmethod(c, (method)sfplay_loop, 	"loop", 	A_LONG, 0);
	class_addmethod(c, (method)sfplay_loopone, 	"loopone", 	A_LONG, 0);
	class_addmethod(c, (method)sfplay_sustain, 		"sustain", 	A_LONG, 0); // sustain 1/0 controls sustain loop
	CLASS_METHOD_ATTR_PARSE(c, "sustain", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method)sfplay_setsustain,	"setsustain", 	A_FLOAT, A_FLOAT, 0);
	CLASS_METHOD_ATTR_PARSE(c, "setsustain", "undocumented", gensym("long"), 0, "1");
	class_addmethod(c, (method)sfplay_save, 	"save", 	A_CANT, 0);
	class_addmethod(c, (method)sfplay_embed, 	"embed", 	A_LONG, 0);
	class_addmethod(c, (method)sfplay_set, 		"set", 		A_DEFSYM, 0);
	class_addmethod(c, (method)sfplay_anything, "anything", A_GIMME, 0);


	class_addmethod(c, (method)sfplay_srate, 	"srate", 	A_DEFFLOAT, 0);
	class_addmethod(c, (method)sfplay_srcchans, "srcchans", A_DEFLONG, 0);
	class_addmethod(c, (method)sfplay_offset, 	"offset", 	A_DEFLONG, 0);
	class_addmethod(c, (method)sfplay_samptype, "samptype", A_DEFSYM, A_DEFLONG, 0);

	class_addmethod(c, (method)sfplay_getcurrentfile, 	"getcurrentfile", 	A_CANT, 0);
	class_addmethod(c, (method)sfplay_getbufsize, 		"getbufsize", 		A_CANT, 0);
	class_addmethod(c, (method)sfplay_sfile_unrefer, 	"unrefer", 			A_CANT, 0);
	class_addmethod(c, (method)sfplay_getcues, 			"getcues", 	A_CANT, 0);
	class_addmethod(c, (method)sfplay_setcues, 			"setcues", 	A_CANT, 0);
	class_addmethod(c, (method)sfplay_sfile_new, 		"sfilenew", A_CANT, 0);
	//to support sfinfo
	class_addmethod(c, (method)sfplay_getsfinfo, 	"getsfinfo", A_CANT, 0);
	class_addmethod(c, (method)sfplay_name, 		"name", 	A_DEFSYM, 0);
	class_addmethod(c, (method)sfplay_modout, 		"modout", 	A_DEFLONG, 0);
	
	sfplay_async_cb = asyncfile_callback_new((method)sfplay_done);
	sfplay_async_cb_rev = asyncfile_callback_new((method)sfplay_done_rev);
	sfile_async_cb = asyncfile_callback_new((method)sfile_async_done);

	class_addmethod(c, (method)sfplay_assist,"assist",A_CANT,0);
	
//	class_addmethod(c, (method)sfplay_getperform, "getperform", A_CANT, 0);
	class_addmethod(c, (method)sfplay_getvalid, "getvalid", A_CANT, 0);
	class_addmethod(c, (method)sfplay_doseek, "seekgimme", A_CANT, 0);
	class_addmethod(c, (method)cue_dopreload, "dopreload", A_CANT, 0);
	class_addmethod(c, (method)sfplay_speedgimme, "speedgimme", A_CANT, 0);
	// we need to be able to get notifications from this object for cue reporting
	class_addmethod(c, (method)object_register, 	"register",		A_CANT, 0);
	class_addmethod(c, (method)sfplay_cuereport, 	"cuereport", 	A_CANT, 0);
	class_addmethod(c, (method)sfplay_looppalin, 	"looppalin",	A_CANT, 0);
	
	class_addmethod(c, (method)method_true,			"(mouse)",		A_CANT, 0);
	CLASS_METHOD_ATTR_PARSE(c, "(mouse)", "documentable", gensym("long"), 0L, "1");

	
	CLASS_ATTR_SYM(c, "audiofile", 0L, t_sfplay, p_current);
	CLASS_ATTR_STYLE_LABEL(c, "audiofile", 0L, "file", "Audio File"); 
	CLASS_ATTR_ACCESSORS(c, "audiofile", (method)sfplay_get_audiofile, (method)sfplay_audiofile);

	class_addmethod(c, (method)sfplay_acceptsdrag, "acceptsdrag_unlocked", A_CANT, 0);
	class_addmethod(c, (method)sfplay_acceptsdrag, "acceptsdrag_locked", A_CANT, 0);
	
	// playbar controller interface
	class_addmethod(c, (method)sfplay_getplaystate, "getplaystate", A_CANT, 0);
	CLASS_METHOD_ATTR_PARSE(c,"getplaystate","undocumented",gensym("long"),0L,"1");
	class_addmethod(c, (method)sfplay_toggleplay, "toggleplay", 0);
	CLASS_METHOD_ATTR_PARSE(c,"toggleplay","undocumented",gensym("long"),0L,"1");
	class_addmethod(c, (method)sfplay_pos, "pos", A_FLOAT, 0);
	CLASS_METHOD_ATTR_PARSE(c,"pos","undocumented",gensym("long"),0L,"1");
	class_addmethod(c, (method)sfplay_ff, "ff", 0);
	CLASS_METHOD_ATTR_PARSE(c,"ff","undocumented",gensym("long"),0L,"1");
	class_addmethod(c, (method)sfplay_rw, "rw", 0);
	CLASS_METHOD_ATTR_PARSE(c,"rw","undocumented",gensym("long"),0L,"1");
	class_addmethod(c, (method)sfplay_controller,	"__controller__", 0);
	CLASS_METHOD_ATTR_PARSE(c,"__controller__","undocumented",gensym("long"),0L,"1");

	//rescopy(FOUR_CHAR_CODE('STR#'),7519);
	ps_X = gensym("#X");
	ps_nothing 	= gensym("");
	ps_sflist 	= gensym("sflist~");
	ps_8bit 	= gensym("int8");
	ps_16bit 	= gensym("int16");
	ps_24bit 	= gensym("int24");
	ps_32bit 	= gensym("int32");
	ps_float 	= gensym("float32");
	ps_double 	= gensym("float64");
	ps_mulaw 	= gensym("mulaw");
	ps_alaw 	= gensym("alaw");
		
	ps_preload			= gensym("preload");
	ps_preload_error	= gensym("preload_error");
	ps_cue				= gensym("cue");
	ps_typedmess 		= gensym("typedmess");
	ps_nobox			= gensym("nobox");
	ps_atomarray 		= gensym("atomarray");
	ps_loopreport 		= gensym("loopreport");
	ps_done		 		= gensym("done");
	ps_matchdragrole	= gensym("matchdragrole");
	ps_audiofile		= gensym("audiofile");
	ps_dropaction_addcustom = gensym("dropaction_addcustom");
	ps_dropaction_addconnectedmessage = gensym("dropaction_addconnectedmessage");
	ps_update = gensym("update");
	ps_controller = gensym("__controller__");
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	sfplay_class = c;
	
	cue_init(); //same cue code now supports both sfplay + sflist
	
	return 0;
}

t_max_err sfplay_get_audiofile(t_sfplay *x, void *attr, long *ac, t_atom **av)
{
	if (ac && av) {
		if ((*ac) && (*av)) {
			; // use passed-in memory
		} else {
			*ac = 1;
			*av = (t_atom *)sysmem_newptr(sizeof(t_atom));
			if (!(*av))
				return MAX_ERR_OUT_OF_MEM;
		}
		// may need to convert to abs path
		atom_setsym(*av, x->p_current ? x->p_current->s_name : gensym(""));
	}
	return MAX_ERR_NONE;
}

t_max_err sfplay_audiofile(t_sfplay *x, void *attr, long ac, t_atom *av)
{
	if (ac && av) {
		sfplay_open(x, atom_getsym(av));
	}
	return MAX_ERR_NONE;
}

void sfplay_cuereport(t_sflist *x, long err, t_cue *c, void *rfu)
{
	t_atom a[8];
	t_symbol *s=ps_preload;
	t_object *arr;

	if (err)
		s = ps_preload_error;

	if (c) {
		//preload <cue> <filename> <start> <end> <bidirectional buffer flag> <speed>;
		SETSYM(a, ps_cue);
		SETLONG(a+1,c->c_no);
		SETSYM(a+2,c->c_parent?c->c_parent->s_name:ps_nothing);
		SETFLOAT(a+3,c->c_start);
		SETFLOAT(a+4,c->c_endpoint);
		SETLONG(a+5,c->c_bidir);
		SETFLOAT(a+6,c->c_speed);
		
		arr = (t_object *)object_new(ps_nobox, ps_atomarray, 7, a);
		object_notify(x, ps_typedmess, arr);
		freeobject(arr);
	}
}

void sfplay_looppalin(t_sfplay *x, long n)
{
	x->p_looppalin = (n) ? 1 : 0;
}

long sfplay_getvalid(t_sfplay *x)
{
	return x->p_current ? 1 : 0;
}

t_int *sfplay_perform(t_int *w)
{
	t_sfplay *x = (t_sfplay *)(w[1]);
	t_float *sigcue = ((t_float *)w[2]);
	int n = w[4];
	t_aqueue *aq = x->p_aq;
	long i, minchans,srcchans,outchans;
	unsigned long bytesavail;
	long transfersize;
	long copysamps;
	long offset = 0;
	double speed,abs_speed;
	t_cue *c,*pc;
	long sampsize, byteswap;
	unsigned long comptype;
	long switch_count=0,cueleft,cuesampsleft=0;
	t_float *poshi=NULL,*poslo=NULL;
	long sc_numcues=0;
	long sc_n=0;
	long sc_cuelist[SF_MAX_SWITCH_PER_VECTOR]; 	//which cue to switch to
	long sc_spos[SF_MAX_SWITCH_PER_VECTOR];  	//sample position at which point to switch 
	long sc_cur,sc_last;
	long m = n;
	long zero;
	
	if (x->p_invalid)
		goto zero;
		
	x->p_process = true;
	if (x->p_connected[0]) {
		//build a list of points to switch cues at
		sc_last = x->p_sc_last;
		while (m && (sc_numcues < SF_MAX_SWITCH_PER_VECTOR)) {
			sc_cur = (long)(*sigcue++); 
			if ((sc_last != sc_cur) && (sc_cur >= 0)) { //negative signals do nothing
				sc_cuelist[sc_numcues] = sc_cur;
				sc_spos[sc_numcues] = n - m;
				++sc_numcues;
			}
			sc_last = sc_cur;
			m--;
		}
		x->p_sc_last = sc_last;
	}
	
	if (x->p_connected[1]) 
		x->p_speed = *((t_float *)w[3]); //currently only sampling speed input once per vector
		
	if (sc_numcues && ((sc_spos[sc_n])==0)) {
		sfplay_sigint(x, sc_cuelist[sc_n]);
		++sc_n;
		--sc_numcues;
	}
	
	outchans = x->p_outchans;
	if (x->p_posout) {
			poshi = x->p_vectors[outchans];
		if (x->p_posout > 1)
			poslo = x->p_vectors[outchans+1];
	}
	
again:
	if (x->p_next) {
		sfplay_switch(x,aq,x->p_next); // copies from preloaded data and sets p_now
	}
	
	if (sc_numcues) {
		m = sc_spos[sc_n] - offset; //samples til next cue
	} else {
		m = n;
	}
	
	copysamps = m;
	c = x->p_now; 
	if ((!c)||(!c->c_valid)||(x->p_status!=SF_Playing)) {//don't zero the whole vector if we have sigcues pending
		zero = TRUE;
		speed = 0.;
		goto calc;
	} else {
		zero = FALSE;
	}
	
	speed = x->p_speed * c->c_speed * (c->c_parent->s_srate/x->p_sr);
	abs_speed = ABS(speed);
	
	if (speed >= 0.) {
		if (x->p_lastdir==-1) { //disk buffering needs to jump ahead if we were prev. buffering in reverse
			long jump;
			jump = aq->a_tail-aq->a_tailrev;
			if (jump <= 0) jump += aq->a_size;	
			x->p_where += jump;
		}
		x->p_lastdir = 1;
	} else {
		if (x->p_lastdir==1) { //disk buffering needs to jump back if we were prev. buffering forward
			long jump;
			jump = aq->a_tail-aq->a_tailrev;
			if (jump <= 0) jump += aq->a_size;	
			x->p_where -= jump;
		}
		x->p_lastdir = -1;
	}

	byteswap = c->c_parent->s_byteswap;
	comptype = c->c_parent->s_comptype;
	srcchans = c->c_parent->s_srcchans;
	minchans = MIN(srcchans,outchans);
	sampsize = c->c_parent->s_sampsize; 
	cuesampsleft = (long)sfplay_cue_samps_left(x,speed);
	cueleft = cuesampsleft*sampsize*srcchans;
	transfersize = (sampsize * srcchans * ((int)(((double) n) * abs_speed))); 
	
	if (cueleft < 0)
		cueleft = 0x00FFFFFF; // think we need to put in this check to avoid negative numbers down the road
		
	if (speed >= 0) {
		bytesavail = aq_spacetoread(aq);
		if ((long)bytesavail>cueleft) {
			bytesavail=cueleft;
		}
	} else {
		bytesavail = aq_spacetoread_rev(aq);
		if ((long)bytesavail>cueleft) {
			bytesavail=cueleft;
		}
	}	

	if ((long)bytesavail >= transfersize) {
		copysamps = m;
	} else if ((long)bytesavail < transfersize) {
		if (abs_speed) {
			copysamps = ((double)bytesavail)/(((double)(sampsize*srcchans))*abs_speed);
			if (copysamps > m)   //to take into account very small speeds
				copysamps = m;
		} else 
			copysamps = m;
	} 
		
calc:	
	if (zero) {	
		// zero output buffers
		if (c && (x->p_status == SF_Paused)) {
			for (i = 0; i < (outchans); i++)
				set_zero((x->p_vectors[i])+offset,copysamps);
			sfplay_calc_pos(x, c->c_parent, copysamps, speed, poshi?poshi+offset:0, poslo?poslo+offset:0);
		} else {
			for (i = 0; i < (outchans+x->p_posout); i++)
				set_zero((x->p_vectors[i])+offset,copysamps);
		}	
	} else {
		// copy samples from circ buffer
		for (i = 0; i < minchans; i++)
			aq_samptype2float(aq, (x->p_vectors[i])+offset, copysamps, i, srcchans, sampsize, comptype, byteswap, speed); 	
		if ((srcchans < outchans)) {
			if (x->p_modout) {
				for (i = srcchans; i < outchans; i++) {
					t_float *p1=(x->p_vectors[i])+offset-1; 
					t_float *p2=(x->p_vectors[i%srcchans])+offset-1;
					long j=copysamps+1;
				
					while (--j) *++p1 = *++p2;
				} 
			} else {
				for (i = srcchans; i < outchans; i++)
					set_zero((x->p_vectors[i])+offset,copysamps); 
			}
		}
		sfplay_calc_pos(x, c->c_parent, copysamps, speed, poshi?poshi+offset:0, poslo?poslo+offset:0);
	}
	
	if (n > copysamps) {		
		pc = NULL;
		
		n -= copysamps;
		offset += copysamps;
		if (n==0) 
			goto inc;
		else if (n<0) {		
			n = 0; //shouldn't get here. this line is to break on
			goto inc;
		}
		if (sc_numcues && (offset>=sc_spos[sc_n])){ //offset shouldn't ever be > *sc_spos, but watch out for it.
			sfplay_sigint(x, sc_cuelist[sc_n]);
			++sc_n;
			--sc_numcues;
			pc = x->p_next;
		} else {		
			if (x->p_loop || (x->p_loopone == c->c_no) || (x->p_sustain && c->c_susloop)){ // if global loop flag is set or if a specific cue is set to loop
				pc = x->p_now;
			} else if (x->p_pendcount) { //p_pendcount will be 0 if we've switched via signal input
				pc = cue_find(sfplay_who(x),x->p_pending[--x->p_pendcount]);
			}
			if (pc) {
				if (x->p_looppalin) {
					x->p_speed = -x->p_speed;
				}
				if (switch_count++ < SF_MAX_SWITCH_PER_VECTOR) {			
					if (pc->c_valid) {		
						x->p_next = pc;
					} else {
						pc = NULL;
					}
				}
			}
			clock_delay(x->p_loopclock, 0);
		}	
		if (!pc) {
			x->p_status = SF_Idle; 
			clock_delay(x->p_clock,0);  //do we want to output bangs when we switch cues/loop too?
		}
		goto again;
		//jkc -this should check to see if we are really at the end of the cue 
		//or if we are just having trouble reading from disk or some other problem.	
	}
	
inc:
	if (x->p_status == SF_Playing) {
		if (speed > 0.) {
			aq_incrhead_ex(aq,copysamps,srcchans,sampsize,speed);  
			sfplay_doread(x,aq,c,x->p_async,FALSE);
		} else if (speed < 0.) {
			aq_incrhead_ex(aq,copysamps,srcchans,sampsize,speed);  
			sfplay_doread(x,aq,c,x->p_async,TRUE);
		}
	}
//bag:
	x->p_process = false;
	return w + 5;
zero:
	for (i=0; i < (x->p_outchans+x->p_posout); i++)	
		set_zero(x->p_vectors[i],n);
	return w + 5;

}

void sfplay_switch(t_sfplay *x, t_aqueue *aq, t_cue *c)
{
	long qloc=0,qlocrev=0;
	
	// problem: async read may be going, so you need to avoid it
	if (x->p_busy) {
		if (x->p_sustaining) { //(x->p_sustain && c->c_susloop) // (x->p_sustaining && c->c_susloop)
			qloc = aq->a_tail - c->c_suspds;
			if (qloc < 0) 
				qloc = aq->a_size - c->c_suspds;
		}
		else {
			qloc = aq->a_tail - c->c_pds;
			if (qloc < 0) 
				qloc = aq->a_size - c->c_pds;
		}
		// make sure that qloc is a multiple of aq->a_atom
		qloc = (qloc / aq->a_atom) * aq->a_atom;
		x->p_switch = TRUE;
	} else {
		x->p_switch = FALSE;
	}
/*	if ((aq->a_tail>qloc) && ((qloc+c->c_pds)>aq->a_tail)) {
//		//we could overwrite async read
	} else if ((aq->a_tail<qloc) && ((aq->a_tail+x->p_bufsize)>qloc)){
//		//async read could overwrite us
	} else if ((qloc<0) || (qloc>aq->a_size)){
		//out of range
	} 
*/
	if (c->c_bidir && (!c->c_data || !c->c_revdata)) {  // checking for existence of loaded data before we try to copy
		x->p_now = c; // this seems to be what we need to do instead of set it to zero.
	}
	else if (c->c_bidir) { 
		//this assumes that c_revpds is always < aq->a_atom. this should always be the case.
		if (qloc==0) {
			qlocrev = aq->a_size - c->c_revpds;
		} else {
			qlocrev = qloc - c->c_revpds;
		} 
		if ((x->p_speed*c->c_speed) >= 0) {	
			if (!c->c_endpoint) { //this is an in point
				aq_copyfrom_rev(aq,(Ptr)c->c_revdata,c->c_revpds,qlocrev);
				aq_copyfrom(aq,(Ptr)c->c_data,c->c_pds,qloc);
			} else {
				aq_copyfrom(aq,(Ptr)c->c_data,c->c_pds,qloc);
				x->p_aq->a_tailrev = x->p_aq->a_head; //reset tailrev
			}
			x->p_where = c->c_byteoffset + c->c_pds;	
		} else { 
			if (!c->c_endpoint) { //this is an in point
				aq_copyfrom(aq,(Ptr)c->c_data,c->c_pds,qloc);
				aq_copyfrom_rev(aq,(Ptr)c->c_revdata,c->c_revpds,qlocrev);
				x->p_where = c->c_byteoffset - c->c_revpds;
			} else {
				aq_copyfrom_rev(aq,(Ptr)c->c_revdata,c->c_revpds,qlocrev);
				x->p_aq->a_tail = x->p_aq->a_head; //reset tail
				x->p_where = c->c_end - c->c_parent->s_offset - c->c_revpds; 
			}
		}
		x->p_now = c;		
	} else {
		if ((x->p_speed*c->c_speed) >= 0) {
			if (x->p_sustaining) { //(x->p_sustain && c->c_susloop) // (x->p_sustaining && c->c_susloop)
				aq_copyfrom(aq,(Ptr)c->c_susdata,c->c_suspds,qloc);
				x->p_aq->a_tailrev = x->p_aq->a_head; //reset tailrev
				x->p_where = c->c_susbyteoffset + c->c_suspds;
				x->p_now = c;
				
			}
			else {
				aq_copyfrom(aq,(Ptr)c->c_data,c->c_pds,qloc);
				x->p_aq->a_tailrev = x->p_aq->a_head; //reset tailrev
				x->p_where = c->c_byteoffset + c->c_pds;
				x->p_now = c;
			}
		} else {
			//can't jump to a cue at negative speed if there is no bidirectional buffering
			x->p_now = 0;
		}
	}
	x->p_next = 0;
	if (x->p_sustain && c->c_susloop) {
		x->p_sustaining = 1; // turn on sustaining flag so we sustain the next time around
	}
	//post("sustain: %d, susloop: %d, sustaining: %d", x->p_sustain, c->c_susloop, x->p_sustaining);
}

double sfplay_cue_samps_left(t_sfplay *x, double speed)
{
	double pos,oneoverinc,sample,headfrak;
	long diff,framesize; 
	double samps=(double)0x00FFFFFF; //was maximum signed long, but need headroom.
	t_aqueue *a=x->p_aq;
	t_cue *c = x->p_now;
	t_sfile *s = x->p_now->c_parent;
	
	if (c&&s&&((c->c_endpoint>0.) || (x->p_sustain && c->c_susloop))) {
		if (speed != 0.) {
			if (speed>0){
				diff = aq_spacetoread(a);
			} else {
				diff = -aq_spacetoread_rev(a);	
			}
			framesize = s->s_sampsize*s->s_srcchans;
			headfrak = ((long)a->a_head) - a->a_head; 
			sample = (((double)(x->p_where-diff))/(double)framesize) + headfrak;
			pos = sample*(1000./(double)s->s_srate);
			oneoverinc = ((double)x->p_sr)/(speed*1000.);
			if (oneoverinc>(double)0x7FFFFFFF)
				oneoverinc = (double)0x7FFFFFFF; //to avoid wraparound errors. not sure if necessary.
			
			if (x->p_sustain && c->c_susloop) {
				if (speed>0) { 
					samps = (c->c_susendpoint-pos)*oneoverinc; // consider the sustain loop endpoint as the end of the cue
				}	else {
					samps = (c->c_susstart-pos)*oneoverinc; /// not sure what to do in this case
				}
			}
			else {
				if (speed>0) { 
					samps = (c->c_endpoint-pos)*oneoverinc; 
				}	else {
					samps = (c->c_start-pos)*oneoverinc; 
				}
			}
			
			if (samps<0)
				samps = 0;
			else if (samps>(double)0x7FFFFFFF)
				samps = (double)0x7FFFFFFF;
		}
	} 
	
	return samps;
}

void sfplay_calc_pos(t_sfplay *x, t_sfile *s, long n, double speed, t_float *poshi, t_float *poslo)
{
	double pos,inc,sample,headfrak;
	long diff,framesize;
	t_aqueue *a=x->p_aq;
	
	// no need to do this if there's no position outlets anyway -jkc
	if (poshi==NULL&&poslo==NULL)
		return;
		
	// some minimal effort to prevent fclose message crashes
	// should really implement something that waits for this 
	// cue to stop playing before closing the file, but this
	// crash is common enough, that this band-aid fix might
	// be an okay stopgap -jkc 
	if (s&&s->s_open) {
		if (speed>=0){
			diff = aq_spacetoread(a);
		} else {
			diff = -aq_spacetoread_rev(a);	
		}
		framesize = s->s_sampsize*s->s_srcchans;
		headfrak = ((long)a->a_head) - a->a_head; 
		sample = (((double)(x->p_where-diff))/(double)framesize) + headfrak;
		pos = sample*(1000./(double)s->s_srate);
		inc = speed*(1000./(double)x->p_sr);
		
		if (poshi){
			if (poslo)
				while (n--){
					*poshi = pos; 
					*poslo++ = pos - (double)*poshi++;
					pos += inc;
				}
			else 
				while (n--){
					*poshi++ = pos; 
					pos += inc;
				}
		} else {
			if (poslo)
				while (n--){
					*poslo++ = pos - (float)pos;
					pos += inc;
				} 
		}
	} else {
		while (n--) {
			if (poslo) *poslo++ = 0;
			if (poshi) *poshi++ = 0;
		} 
	}
}
	
long sfplay_doread(t_sfplay *x, t_aqueue *aq, t_cue *c, long async, long reverse)			
{
	long bytes, wsize, framesize, offset;
	char *tail,*ptr;
	t_sfile *s = c->c_parent;
	
	if (x->p_busy || !s)
		return TRUE;
	x->p_busy = 1;
	
	if (reverse) {
		bytes = (x->p_where - s->s_offset) - c->c_start;
		bytes = bytes < x->p_bufsize ? bytes : x->p_bufsize;
		wsize = aq_gettail_rev(aq,&tail);
		
	} else { 
		bytes = s->s_size - (x->p_where + s->s_offset); 
		bytes = bytes < x->p_bufsize ? bytes : x->p_bufsize;
		wsize = aq_gettail(aq,&tail);
	}	
	
	if (bytes <= 0 || wsize < bytes) {
		x->p_busy = 0;
		return TRUE;
	}
	
	// now make sure you read only a multiple of the current sample frame size 
	framesize = s->s_srcchans * s->s_sampsize;
	bytes = (bytes/framesize) * framesize;

	if (reverse) {
		ptr = tail - bytes; //should be guaranteed to be within queue
		offset = x->p_where + s->s_offset - bytes; //NOT GUARANTEED? change?
	} else {
		ptr = tail;
		offset = x->p_where + s->s_offset;
	}

	// we could replace the following with something more general interfacing to the
	// sfile API in order to support mp3, and potentially other codecs. - jkc

	{
		t_asyncfile_params *p = x->p_iorequest;
	
		asyncfile_params_default(x->p_iorequest);
		p->file = s->s_asyncfile;
		p->buf = ptr;
		p->requestcount = bytes;
		p->position = offset;
		p->arg = x;
		if (async) {
			p->callback = reverse ? sfplay_async_cb_rev : sfplay_async_cb;
			asyncfile_read(p);
			return 0;
		} else {
			p->flags |= ASYNCFILE_SYNC;
			asyncfile_read(p);
			if (reverse)
				sfplay_done_rev(p);
			else
				sfplay_done(p);
		}
	}

		
	return bytes < x->p_bufsize ? TRUE : FALSE;
}

void sfplay_done(t_asyncfile_params *p)
{
	t_sfplay *x = (t_sfplay *)p->arg;

	if (!x->p_switch) {
		aq_incrtail(x->p_aq,p->actualcount);
		x->p_where += p->actualcount;
	} else {
		x->p_switch = FALSE; //don't update these values, we just switched cues, forget read
	}
	//if (!p->actualcount)
	///	x->p_eof = 1;
	//else 
	if (p->result) {
		object_error((t_object *)x, "disk error %d",p->result);
		x->p_status = SF_Idle;
	}
	x->p_busy = 0;
}

void sfplay_done_rev(t_asyncfile_params *p)
{
	t_sfplay *x = (t_sfplay *)p->arg;

	if (!x->p_switch) {
		aq_incrtail_rev(x->p_aq,p->actualcount);
		x->p_where -= p->actualcount;
	} else {
		x->p_switch = FALSE; //don't update these values, we just switched cues, forget read
	}
	//if (!p->actualcount)
	///	x->p_eof = 1;
	//else 
	if (p->result) {
		object_error((t_object *)x, "disk error %d",p->result);
		x->p_status = SF_Idle;
	}
	x->p_busy = 0;
}

void sfplay_dsp(t_sfplay *x, t_signal **sp, short *count)
{
	short i;
	t_ad *dspobj;
	
	x->p_connected[0] = count[0];
	x->p_connected[1] = count[1];
	x->p_sr = sp[0]->s_sr;
	for (i=0; i < (x->p_outchans+x->p_posout); i++)
		x->p_vectors[i] = sp[i+2]->s_vec;
		
	dspobj = (t_ad *)gensym("dsp")->s_thing;
	if (dspobj)
		x->p_async = dspobj->d_drivername != gensym("NonRealTime");
	else
		x->p_async = false;

	dsp_add(sfplay_perform, 4, x, sp[0]->s_vec,  sp[1]->s_vec, sp[0]->s_n);
}

// theory: open adds to list of currently open files, unless already
// open, then makes cue #1 the beginning

void sfplay_doopen(t_sfplay *x, t_symbol *name)
{
	char filename[128];
	unsigned long type=0, sfType[7];
	long preload,total,framesize; 
	short vol = 0;
	short ntypes = 7;
	t_sfile *s;

	sfType[0] = FOUR_CHAR_CODE('AIFF');
	sfType[1] = FOUR_CHAR_CODE('AIFC');
	sfType[2] = FOUR_CHAR_CODE('NxTS'); //next/sun typed by soundhack
	sfType[3] = FOUR_CHAR_CODE('ULAW'); //next/sun typed by peak and others
	sfType[4] = FOUR_CHAR_CODE('WAVE'); 
	sfType[5] = FOUR_CHAR_CODE('.WAV'); //wave typed by soundhack
#ifdef WIN_VERSION
	ntypes -= 1;
#else
	sfType[6] = FOUR_CHAR_CODE('Sd2f');
#endif // WIN_VERSION
		
	x->p_opendeferred = false;
	if (name == ps_nothing) {
		if (open_dialog(filename, &vol, (long *)&type, (long *)sfType, ntypes))
			return; //canceled
		name = gensym(filename);
	}
	if (sfplay_spinwait(&x->p_process)) {
		object_error((t_object *)x, "%s: can't open file now",filename);
		return;
	}
	x->p_invalid = true;
	if (vol && type) 
		s = sfplay_sfile_new(x,name,vol,1,(long *)&type);
	else
		s = sfplay_sfile_new(x,name,vol,ntypes,(long *)sfType);
	if (!s) 
		goto done;
			
	x->p_current = s;
	if (x->p_one.c_parent)
		sfplay_sfile_unrefer(x,&x->p_one,x->p_one.c_parent);
	x->p_one.c_parent = 0;
	framesize = s->s_sampsize * s->s_srcchans;
	total = s->s_frames * framesize;
	preload = x->p_bufsize * SF_PRELOAD;
	preload = (preload / framesize) * framesize;
	x->p_one.c_pds = MIN(total,preload);
	sfplay_sfile_read(x,s,0,x->p_one.c_pds,(Ptr)x->p_one.c_data,false);	// preload
	x->p_one.c_parent = s;
	x->p_one.c_end = s->s_size;
done:
	x->p_invalid = false;
}

short sfplay_spinwait(long *p)
{
	double start,time;
	
	clock_getftime(&start);
	time = start;
	while (time - start < SFPLAY_WAITTIME) {
		if (!*p)
			return false;
		clock_getftime(&time);
	}
	return true;
}

void sfplay_srate(t_sfplay *x, double f)
{
	if (f<0.) f=0.;
	if (x->p_current) x->p_current->s_srate=f;
}

void sfplay_srcchans(t_sfplay *x, long n)
{
	if (n<1) n=1;
	else if (n>8) n=8;
	if (x->p_current) x->p_current->s_srcchans=n;
}

void sfplay_offset(t_sfplay *x, long n)
{
	if (x->p_current) {
		if (n<0) n=0;
		else if (n>x->p_current->s_size) n = x->p_current->s_size;
		x->p_current->s_offset=n;
		//hmmm...is s_size the file size or just the size to the end after offset???
		//if the former, i fucked up .au files, if the latter, i'm fucking up here
	}
}

void sfplay_samptype(t_sfplay *x, t_symbol *s, long swap)
{
	if (x->p_current) {
		if (s==ps_8bit) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 1;
			x->p_current->s_comptype = SF_SAMPTYPE_INT;
		} else if (s==ps_16bit) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 2;
			x->p_current->s_comptype = SF_SAMPTYPE_INT;
		} else if (s==ps_24bit) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 3;
			x->p_current->s_comptype = SF_SAMPTYPE_INT;
		} else if (s==ps_32bit) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 4;
			x->p_current->s_comptype = SF_SAMPTYPE_INT;
		} else if (s==ps_float) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 4;
			x->p_current->s_comptype = SF_SAMPTYPE_FL32;
		} else if (s==ps_double) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 8;
			x->p_current->s_comptype = SF_SAMPTYPE_FL64;
		} else if (s==ps_mulaw) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 1;
			x->p_current->s_comptype = SF_SAMPTYPE_MULAW;
		} else if (s==ps_alaw) {
			x->p_current->s_byteswap = swap;
			x->p_current->s_sampsize = 1;
			x->p_current->s_comptype = SF_SAMPTYPE_ALAW;
		} else {
			object_post((t_object *)x, "unknown samptype %s",s->s_name);
		}
	}
}
	
void sfplay_open(t_sfplay *x, t_symbol *s)
{
	x->p_opendeferred = true;
	defer(x,(method)sfplay_doopen,s,0,0L);
}

void sfplay_doopenraw(t_sfplay *x, t_symbol *name)
{
	char filename[128];
	unsigned long type = 0;  
	long preload,total,framesize; 
	short vol = 0;
	t_sfile *s;
	
	x->p_opendeferred = false;
	if (name == ps_nothing) {
		if (open_dialog(filename, &vol, (long *)&type, 0L, 0))
			return; //canceled
		name = gensym(filename);
	} 
	if (sfplay_spinwait(&x->p_process)) {
		object_error((t_object *)x, "%s: can't open file now",filename);
		return;
	}
	x->p_invalid = true;
	s = sfplay_sfile_newraw(x,name,vol,type);
	if (!s)
		goto done;
	x->p_current = s;
	if (x->p_one.c_parent)
		sfplay_sfile_unrefer(x,&x->p_one,x->p_one.c_parent);
	x->p_one.c_parent = 0;	
	framesize = s->s_sampsize * s->s_srcchans;
	total = s->s_frames * framesize;
	preload = x->p_bufsize * SF_PRELOAD;
	preload = (preload / framesize) * framesize;
	x->p_one.c_pds = MIN(total,preload);	
	sfplay_sfile_read(x,s,0,x->p_one.c_pds,(Ptr)x->p_one.c_data,false);	// preload
	x->p_one.c_parent = s;
	x->p_one.c_end = s->s_size;
	x->p_one.c_endpoint = 0;
done:
	x->p_invalid = false;
}
	
void sfplay_openraw(t_sfplay *x, t_symbol *s)
{
	x->p_opendeferred = true;
	defer(x,(method)sfplay_doopenraw,s,0,0L);
}

void sfplay_dofclose(t_sfplay *x, t_symbol *s)
{
	t_sfile *sf;
	
	if (s == ps_nothing) {	// close what's associated with cue 1
		if (x->p_current)
			s = x->p_current->s_name;
	}
	for (sf = x->p_files; sf; sf = sf->s_next) {
		if (sf->s_name == s) {
			sfplay_sfile_free(x,sf,true);
			return;
		}
	}
	object_error((t_object *)x, "no open file %s",s->s_name);
}

void sfplay_fclose(t_sfplay *x, t_symbol *s)
{
	defer(x,(method)sfplay_dofclose,s,0,0L);
}

void sfplay_print(t_sfplay *x)
{
	t_cue *c;
	
	object_post((t_object *)x, "sfplay~ status:");
	if (x->p_status == SF_Playing)
		object_post((t_object *)x, " playing");
	else if (x->p_status == SF_Idle)
		object_post((t_object *)x, " idle");
	else if (x->p_status == SF_Paused)
		object_post((t_object *)x, " paused");
	object_post((t_object *)x, " queue head %lx tail %lx tailrev %lx, %ld bytes read", (long)(x->p_aq->a_head), x->p_aq->a_tail, x->p_aq->a_tailrev,
		x->p_where);
	object_post((t_object *)x, " cues:");
	for (c = x->p_cues; c; c = c->c_next) {
		object_post((t_object *)x, "  %ld %s start %.2f end %.2f bidir=%d speed=%.4f",
			c->c_no,c->c_parent->s_name->s_name,c->c_start,c->c_endpoint,c->c_bidir,c->c_speed);
	}
	if (x->p_now) {
		object_post((t_object *)x, "current file: %s",x->p_now->c_parent->s_name->s_name);
		object_post((t_object *)x, "samplerate: %f",x->p_now->c_parent->s_srate);
		object_post((t_object *)x, "numchans:   %d",x->p_now->c_parent->s_srcchans);
		object_post((t_object *)x, "sampsize:   %d",x->p_now->c_parent->s_sampsize);
	}
}

void sfplay_pause(t_sfplay *x)
{
	if (x->p_status == SF_Playing)
		x->p_status = SF_Paused;
}

void sfplay_resume(t_sfplay *x)
{
	if (x->p_status == SF_Paused)
		x->p_status = SF_Playing;
}

void sfplay_speed(t_sfplay *x, double f)
{
	x->p_speed = f;	
}

void sfplay_speedgimme(t_sfplay *x, t_symbol *s, long ac, t_atom *av)
{
	if (ac && av) {
		sfplay_speed(x, atom_getfloat(av));
	}
}

void sfplay_loop(t_sfplay *x, long c)
{
	if (c < 0) 
		c *= -1;
	x->p_loop = c;	
}

void sfplay_loopone(t_sfplay *x, long c)
{
	if (c < 0) 
		c *= -1;
	x->p_loopone = c;	
}

void sfplay_sustain(t_sfplay *x, long n)
{
	x->p_sustain = n ? 1 : 0;
	
	if (n == 0) x->p_sustaining = 0;
}

void sfplay_setsustain(t_sfplay *x, double start, double end)
{
	x->p_sloopstart = (start<0.) ? -start : start;	
	x->p_sloopend = (end<0.) ? -end : end;	
}

// you can't seek outside of the current cue, so reset to cue #1
void sfplay_seek(t_sfplay *x, double start, double end)
{
	t_atom a[2];
	
	a[0].a_type = A_FLOAT;
	a[0].a_w.w_float = start;
	a[1].a_type = A_FLOAT;
	a[1].a_w.w_float = end;
	defer(x,(method)sfplay_doseek,0L,2,a);
}
/*
void sfplay_seekgimme(t_sfplay *x, t_symbol *s, long ac, t_atom *av)
{
	if (ac && av) {
		if (ac > 1)
			defer(x,(method)sfplay_doseek,0L,2,av);
		else sfplay_seek(x, atom_getfloat(av), 0);
	}
}
*/
void sfplay_doseek(t_sfplay *x, t_symbol *sym, short argc, t_atom *argv)
{
	long byteoffset,endpoint,framesize;
	t_sfile *s;
	Ptr p;
	long status,count;
	double start = argv->a_w.w_float, end = (argv+1)->a_w.w_float;
	
	if (!(s = x->p_current)) {
		object_error((t_object *)x, "seek: no open file");
		return;
	}
	status = x->p_status;
	x->p_status = SF_Idle;
	
	//wait for perform loop to finish
	if (sfplay_spinwait(&x->p_process)) {
		object_error((t_object *)x, "can't seek now");
		return;
	}
	x->p_invalid = true;

	//wait for pending async reads
	if (sfplay_spinwait(&x->p_busy)) {
		object_error((t_object *)x, "can't seek now");
		return;
	}
		
	if (x->p_now) {
		x->p_now = 0;
	}
	if (x->p_next) {
		x->p_next = 0;
	}
	framesize = s->s_srcchans * s->s_sampsize;  
	byteoffset = ((long)(start * s->s_srate * 0.001)) * framesize;
	if (byteoffset < 0)
		byteoffset = 0;
	
	if (byteoffset > s->s_size - s->s_offset)	// seek to end
		byteoffset = s->s_size - s->s_offset;
	if (end < start)
		end = 0.;
	if (end)
		endpoint = ((long)(end * s->s_srate * 0.001)) * framesize;
	else
		endpoint = s->s_size - s->s_offset;
	if (endpoint < 0)
		endpoint = 0;
	if (endpoint > s->s_size - s->s_offset)	// over the end?
		endpoint = s->s_size - s->s_offset;
	aq_init(x->p_aq);
	aq_gettail(x->p_aq, &p);
	count = MIN(endpoint - byteoffset,x->p_bufsize * SF_PRELOAD);
	if (!count)
		return;
	sfplay_sfile_read(x,s,byteoffset,count,p,false); // preload some shit
	aq_incrtail(x->p_aq,count);
	x->p_where = byteoffset + count;
	x->p_one.c_end = endpoint + s->s_offset;
	x->p_one.c_endpoint = end;
	//post("where %ld end %ld offset %ld gh %ld",x->p_where,x->p_one.c_end,s->s_offset,
	//	aq_gethead(x->p_aq,&p));
	x->p_now = &x->p_one;
	x->p_pendcount = 0;
	
	x->p_invalid = false;
	
	x->p_status = SF_Playing;
	clock_delay(x->p_updateclock,100);
}

void sfplay_float(t_sfplay *x, double f)
{
	if (proxy_getinlet((t_object *)x) == 0) 	
		sfplay_int(x, (long) f);
	else 
		x->p_speed = f;
	return;	
}

void sfplay_int(t_sfplay *x, long n)
{
	t_cue *c;
	
	if (proxy_getinlet((t_object *)x) == 1) {
		x->p_speed = n;
		return;
	}	
	
	if (x->p_opendeferred) {	// bug fix for sfctrl~ control of object
		t_atom a;
		
		a.a_type = A_LONG;
		a.a_w.w_long = n;
		defer_low(x,(method)sfplay_dint,0L,1,&a);
		return;
	}
	if (n == 1) {
		if (x->p_current) {
			x->p_pendcount = 0;
			x->p_next = &x->p_one;
			x->p_one.c_end = x->p_current->s_size;	// reset in case seek messed with it
			x->p_one.c_endpoint = 0;	// reset in case seek messed with it
			x->p_status = SF_Playing;
			x->p_sustaining = 0; // reset looping flag if we get a new int
		} else
			object_error((t_object *)x, "no open file");
	} else if (n > 1) {
		if (c = cue_find(sfplay_who(x),n)) {
			x->p_pendcount = 0;
			x->p_status = SF_Playing;
			x->p_next = c;
			x->p_sustaining = 0; // reset looping flag if we get a new int
		} else {
			if (x->p_bound)
				object_post((t_object *)x, "no cue %s",x->p_bound->s_name);
		}
	} else {		// 0 or negative, stop
		if (x->p_status == SF_Playing)
			// ddz changed this from a direct call to sfplay_tick to avoid stack overflow
			clock_delay(x->p_clock, 0);
		x->p_status = SF_Idle;
		x->p_pendcount = 0;
		if (x->p_next) {
			x->p_next = 0;
		}
		if (x->p_now) {
			x->p_now = 0;
		}
	}

	if (x->p_status == SF_Playing)
		clock_delay(x->p_updateclock,100);
	else
		clock_unset(x->p_updateclock);	
}

void sfplay_dint(t_sfplay *x, t_symbol *s, short argc, t_atom *argv)
{
	sfplay_int(x,argv->a_w.w_long);
}

void sfplay_sigint(t_sfplay *x, long n)
{
	t_cue *c;
	
	if (x->p_opendeferred) {	// bug fix for sfctrl~ control of object
		// do nothing special for this case.
		// should we instead not cue switch???
		//return;
	}
	if (n == 1) {
		if (x->p_current) {
			x->p_pendcount = 0;
			x->p_next = &x->p_one;
			x->p_one.c_end = x->p_current->s_size;	// reset in case seek messed with it
			x->p_one.c_endpoint = 0;	// reset in case seek messed with it
			x->p_status = SF_Playing;
		} else {
			// don't show messages in signal vector 
			// object_error((t_object *)x, "no open file");
		}
	} else if (n > 1) {
		if (c = cue_find(sfplay_who(x),n)) {
			x->p_pendcount = 0;
			x->p_status = SF_Playing;
			x->p_next = c;
		} else {
			if (x->p_bound) {
				// don't show messages in signal vector
				//post("no cue %s",x->p_bound->s_name);
			}	
		}
	} else {		// 0 or negative, stop
		if (x->p_status == SF_Playing)
			clock_delay(x->p_clock,0); //in signal vector.
		x->p_status = SF_Idle;
		x->p_pendcount = 0;
		if (x->p_next) {
			x->p_next = 0;
		}
		if (x->p_now) {
			x->p_now = 0;
		}
	}
	if (x->p_status == SF_Playing)
		clock_delay(x->p_updateclock,100);
	else
		clock_unset(x->p_updateclock);
}


void sfplay_tick(t_sfplay *x)
{
	outlet_bang(x->p_done);
	object_notify(x, ps_done, 0L);	
	qelem_set(x->p_updateqelem);
}

void sfplay_looptick(t_sfplay *x)
{
	object_notify(x, ps_loopreport, 0L);
}

void sfplay_updatetick(t_sfplay *x)
{
	if (x->p_status == SF_Playing && x->p_linked)
		clock_delay(x->p_updateclock,100);
	
	qelem_set(x->p_updateqelem);
}

/*
preload N M  preload and define (or redefine) cue N at M milliseconds
into the file. Cue 1 is always the beginning. If N already exists and
M is not present, preload buffer is just reloaded. If N and M are not
present, all existing cues are preloaded (could be used from a
loadbang with embed below).

int, if > 1, start playing cue N if defined, 1 plays from the beginning.
0 stops. New implementation always preloads the beginning, so there
is no need for -1 or reopening to get decent performance.

unload N     remove data for cue N, if int is sent for this cue, data
will be loaded before it plays
remove N     remove data and the existence of cue N
next	     move to next cue (does not need to be the next number, just
	     higher)

embed N	     if N = 1, save all cues with the object in the patcher,
             if N = 0, don't. N = 0 by default

pause	     pauses playback
resume	     starts where paused
*/

// remove cue. This only removes private cues

void sfplay_clear(t_sfplay *x, t_symbol *s, short argc, t_atom *argv)
{
	defer(x,(method)sfplay_doclear,s,argc,argv);
}

void sfplay_doclear(t_sfplay *x, t_symbol *s, short argc, t_atom *argv)
{
	long n,i;
	t_cue *c;
	
	if (sys_getdspstate()) {
		object_error((t_object *)x, "no open file");
		return;
	}
	if (!argc) {
		sfplay_freecues(x);
		sfplay_freefiles(x,false);
	} else {
		for (i = 0; i < argc; i++,argv++) {
			if (argv->a_type != A_LONG)
				continue;
			n = argv->a_w.w_long;
	
			c = cue_find(x->p_cues,n);
			if (c)
				cue_free(x,c);
		}
	}
}

t_sfile *sfplay_sfile_new(t_sfplay *x, t_symbol *name, short vol, long numtypes, long *typelist)
{
	t_sfile *s;
	long theType=0,flags=0;
	char filename[MAX_PATH_CHARS];
		
	if (!(s = sfplay_sfile_find(x,name))) {	// already exists?
		// try to open the file
		if (!vol) {
		 	if ((numtypes > 0) && typelist) {
				strcpy(filename,name->s_name);
				if (!locatefile_extended(filename, &vol, &theType, typelist, numtypes)) {
					name = gensym(filename); //in case filename changes(e.g. aliases)
					if (s = sfplay_sfile_find(x,name)) goto out;
					else goto found;
				}
				object_error((t_object *)x, "can't find file %s",name->s_name);
				return 0;
			} else {
				return 0;  //should I instead allow for any type we support? 
			}
		} else {
			if ((numtypes==1) && typelist)
				theType = *typelist;
		}
found:
		flags |= SFILE_ASYNC;

		s = sfile_new();
		if ((theType == FOUR_CHAR_CODE('AIFF'))||(theType == FOUR_CHAR_CODE('AIFC'))) {
			if (sfile_aiffopen(s,name,vol,flags)) {
				object_error((t_object *)x, "can't open aiff file %s",name->s_name);
				sfile_free(s);
				return NULL;
			}
		} else if (theType == FOUR_CHAR_CODE('Sd2f')) {
			if (sfile_sd2open(s,name,vol,flags)) {
				object_error((t_object *)x, "can't open sd2 file %s",name->s_name);
				sfile_free(s);
				return NULL;
			}
		} else if ((theType == FOUR_CHAR_CODE('NxTS'))||(theType == FOUR_CHAR_CODE('ULAW'))) {
			if (sfile_auopen(s,name,vol,flags)) {
				object_error((t_object *)x, "can't open NeXT/Sun file %s",name->s_name);
				sfile_free(s);
				return NULL;
			}
		} else if ((theType == FOUR_CHAR_CODE('WAVE'))||(theType == FOUR_CHAR_CODE('.WAV'))) {
			if (sfile_waveopen(s,name,vol,flags)) {
				object_error((t_object *)x, "can't open wave file %s",name->s_name);
				sfile_free(s);
				return NULL;
			}
		} else {
			object_error((t_object *)x, "unrecognized file type %s",name->s_name);
			return NULL;
		}
			
		s->s_next = x->p_files;
		x->p_files = s;
	}
out:
	return s;
}

t_sfile *sfplay_sfile_newraw(t_sfplay *x, t_symbol *name, short vol, long type)
{
	t_sfile *s;
	long theType=type,flags=0;
	char filename[MAX_PATH_CHARS];
	
	if (!(s = sfplay_sfile_find(x,name))) {	// already exists?
		strcpy(filename,name->s_name);
		if (!vol && locatefile_extended(filename, &vol, &theType, 0L, 0)) {
			object_error((t_object *)x, "can't find file %s",name->s_name);
			return NULL;
		}
		name = gensym(filename); //in case filename changes(e.g. aliases)
//found:
		flags |= SFILE_ASYNC;

		s = sfile_new();
		if (sfile_rawopen(s,name,vol,flags)){
			object_error((t_object *)x, "can't open raw file %s",name->s_name);
			sfile_free(s);
			return NULL;
		}
		s->s_next = x->p_files;
		x->p_files = s;
	}
	return s;
}

t_sfile *sfplay_sfile_find(t_sfplay *x, t_symbol *name)
{
	t_sfile *s;
	
	for (s = x->p_files; s; s = s->s_next)
		if (s->s_name == name)
			return s;
	return 0;
}

void sfplay_sfile_unrefer(t_sfplay *x, t_cue *qc, t_sfile *s)
{
	t_cue *c;
	long count;
	
	// decrement refcount & possibly close/remove
	// rather than maintain a refcount, we'll just count each time
	if (!qc)	// mode where s wouldn't be closed anyway
		return;
	
	for (count = 0, c = x->p_cues; c; c = c->c_next) {
		if (c != qc && c->c_parent == s)
			count++;
	}
	if (count)	// there are still other cues using this sfile
		return;
	sfplay_sfile_free(x,s,false);
}

void sfplay_freefiles(t_sfplay *x, Boolean freecurrent)
{
	t_sfile *s, *temp;
	//long i = 0;

	// now freeing the files one at a time
	for (s = x->p_files; s; ) {
		// not strictly necessary - as sfplay_sfile_free won't free the current file if we say nofreecues which we do
		//i++;
		//post("freeing soundfile %ld %s", i, s->s_name->s_name);
		temp = s->s_next;
		sfplay_sfile_free(x,s,0);  //  all cues have been freed already, sfplay_sfile_free won't free the current file if we say nofreecues which we do
		s = temp;
	}
	
	// this only happens when we free the object
	if (freecurrent) {
		s = x->p_current;
		sfplay_sfile_free(x,s,1); // the cues have been freed
		x->p_current = 0;
		x->p_one.c_parent = 0;
	}
}

void sfplay_sfile_free(t_sfplay *x, t_sfile *s, Boolean freecues)
{
	t_sfile *prev,*sp;
	t_cue *cp,*c,*temp;
		
	if (!s) return;
	
	// check for closing the current file on a clear cue message
	if (!freecues && (s == x->p_current))
		return;
		
	// do everything to protect against file access
	if (sys_getdspstate())
		sfplay_int(x,0);
	if (x->p_pendcount)
		x->p_pendcount = 0;
	if (x->p_next)
		x->p_next = 0;
	if (x->p_now)
		x->p_now = 0;
	if (x->p_current == s) {
		x->p_current = 0;
		x->p_one.c_parent = 0;
	}
	if (s->s_open) {
		if (sfplay_spinwait(&s->s_busy)) 
			object_error((t_object *)x, "free file timeout");
		sfile_close(s);
	}
	// remove the file from the list
	for (prev = 0, sp = x->p_files; sp; prev = sp, sp = sp->s_next) {
		if (sp == s) {
			if (prev)
				prev->s_next = sp->s_next;
			else
				x->p_files = sp->s_next;
			sfile_free(s);
			break;
		}
	}
	if (freecues) {
		// now free any cues that use this file 
		for (cp = 0, c = x->p_cues; c; ) {
			if (c->c_parent == s) {
				temp = c->c_next;
				if (cp)
					cp->c_next = temp;
				else
					x->p_cues = temp;
				cue_freedata(c);
				c = temp;
			} else {
				cp = c;
				c = c->c_next;
			}
		}
	}
}

long sfplay_sfile_read(t_sfplay *x, t_sfile *s, long start, long bytes, Ptr dst, short async)
{
	// we could replace the following with something more general interfacing to the
	// sfile API in order to support mp3, and potentially other codecs. - jkc

	t_asyncfile_params *p = asyncfile_params_new();
	
	p->file = s->s_asyncfile;
	p->buf = dst;
	p->requestcount = bytes;
	p->position = s->s_offset + start;
	p->arg = s;
	if (async) {
		s->s_busy = 1; // this might be no longer necessary
		p->callback = sfile_async_cb;
		asyncfile_read(p);
		return 0;
	} else {
		p->flags |= ASYNCFILE_SYNC;
		asyncfile_read(p);
		asyncfile_params_free(p);
		return p->actualcount;
	}
}

void sfplay_free(t_sfplay *x)
{
	dsp_free((t_pxobject *)x);
	symbol_unbind(x->p_name,(t_object *)x);
	sfplay_freecues(x);
	sfplay_freefiles(x,true);
	freeobject(x->p_clock);
	freeobject(x->p_loopclock);
	aq_free(x->p_aq);
	if (x->p_one.c_data)
		t_freebytes(x->p_one.c_data,x->p_one.c_datasize);
	if (x->p_iorequest)
		asyncfile_params_free(x->p_iorequest);	
	freeobject(x->p_updateclock);
	qelem_free(x->p_updateqelem);
}

void sfplay_freecues(t_sfplay *x)
{
	t_cue *c,*temp;
	
	x->p_next = 0;
	x->p_now = 0;
	x->p_pendcount = 0;
	for (c = x->p_cues; c; ) {
		temp = c->c_next;
		sfplay_sfile_unrefer(x,0,c->c_parent);
		x->p_cues = temp; //to prevent possible problems in unrefer
		cue_freedata(c);
		c = temp;
	}
	x->p_cues = 0;
}


void sfplay_list(t_sfplay *x, t_symbol *s, short argc, t_atom *argv)
{
	short i,j;
	// copy cues to pending list. not good to copy pointers, since
	// someone could remove a cue while you are playing
	for (j = 0, i = argc - 1; i >= 0; i--,j++) {
		if ((argv+j)->a_type != A_SYM)
			x->p_pending[i] = (argv+j)->a_type == A_LONG? (argv+j)->a_w.w_long : (argv+j)->a_w.w_float;
		else
			x->p_pending[i] = SF_WEIRDCUE;
	}
	sfplay_int(x,x->p_pending[argc-1]);
	x->p_pendcount = argc-1;
}

void sfplay_assist(t_sfplay *x, void *b, long m, long a, char *s)
{		
	if (m==1) { // input
		switch (a) {
			case 0: sprintf(s,"(signal/msg)open Opens File, int Plays Cue, 0 Stops"); break;
			case 1: sprintf(s,"(signal/float) Speed Input"); break;
		}
	} 
	else if (m == 2) {
		if (a < x->p_outchans) {
			sprintf(s,"(signal) File Ch %ld Output",a+1); //audio outlets			
		} else if ((a -= x->p_outchans) < x->p_posout) {
			switch (a) { //position oulets
				case 0: sprintf(s,"(signal) File Position(ms)"); break;
				case 1: sprintf(s,"(signal) Position Roundoff Error(ms)"); break;
			}
		} else {
			sprintf(s,"bang When Done Playing"); //bangout
		}
	}
}

void sfplay_save(t_sfplay *x, void *w)
{
	t_cue *c;
	t_atom msg[8];
	
	binbuf_vinsert(w, "sssllls", gensym("#N"), gensym("sfplay~"), x->p_bound? x->p_bound : gensym(""),
		x->p_outchans, x->p_ibufsize, x->p_posout, x->p_name);
	if (x->p_flags & SF_EMBED) {
		SETSYM(msg,ps_X);
		SETSYM(msg+1,gensym("preload"));
		for (c = x->p_cues; c; c = c->c_next) {
			SETLONG(msg+2,c->c_no);
			SETSYM(msg+3,c->c_parent->s_name);
			SETFLOAT(msg+4,c->c_start);
			SETFLOAT(msg+5,c->c_endpoint);
			SETLONG(msg+6,c->c_bidir);	
			SETFLOAT(msg+7,c->c_speed);
			binbuf_insert(w,0L,8,msg);
		}
		if (x->p_current)
			binbuf_vinsert(w,"sss",ps_X,gensym("open"),x->p_current->s_name);
		binbuf_vinsert(w,"ssl",ps_X,gensym("embed"),1L);
	}
}

void sfplay_embed(t_sfplay *x, long n)
{
	if (n)
		x->p_flags |= SF_EMBED;
	else
		x->p_flags &= ~SF_EMBED;
}

void sfplay_set(t_sfplay *x, t_symbol *s)
{
	if (s != ps_nothing)
		x->p_bound = s;
	else
		x->p_bound = 0;
}

t_cue *sfplay_who(t_sfplay *x)
{
	if (x->p_bound) {
		t_sflist *l;
	
		if (l = sfplay_lookup(x->p_bound))
			return l->l_cues;
		else
			return 0;
	} else
		return x->p_cues;
}

t_sflist *sfplay_lookup(t_symbol *s)
{
	t_sflist *l = (t_sflist *)(s->s_thing);
	if (l && ob_sym(l) == ps_sflist)
		return l;
	else 
		return 0;
}

// messages must take the form symbol cue-number
void sfplay_anything(t_sfplay *x, t_symbol *s, short argc, t_atom *argv)
{
	t_sflist *l;
	t_cue *c;
	long q;
	
	if (argc && argv->a_type == A_LONG) {
		q = argv->a_w.w_long;
		if (l = sfplay_lookup(s)) {
			if (c = cue_find(l->l_cues,q)) {
				x->p_pendcount = 0;
				x->p_status = SF_Playing;
				x->p_next = c;
			}
		}
	}	
}

void sfplay_name(t_sfplay *x, t_symbol *s)
{
	symbol_unbind(s,(t_object *)x);
	x->p_name=s;
	if (s!=ps_nothing) {
		symbol_bind(s,(t_object *)x);	
	}
}

void sfplay_modout(t_sfplay *x, long c)
{
	x->p_modout=c;
}

//access functions to be called from cue code which supports both sfplay and sflist
t_sfile *sfplay_getcurrentfile(t_sfplay *x)
{
	return (x)?x->p_current:0;
}

long sfplay_getbufsize(t_sfplay *x)
{
	return (x)?x->p_bufsize:0;
}

t_cue *sfplay_getcues(t_sfplay *x)
{
	return (x)?x->p_cues:0;
}

void sfplay_setcues(t_sfplay *x, t_cue *p)
{
	if (x) x->p_cues=p;
}

long sfplay_getsfinfo(t_sfplay *x, t_sndfileinfo *info)
{
	t_sfile *p;
	
	if (x->p_now) p = x->p_now->c_parent;
	else p = x->p_current;
	if ((!p)||(!info))
		return -1;
	
	info->sr		= p->s_srate;
	info->length	= p->s_size;
	info->sampsize	= p->s_sampsize; 
	info->filetype	= 0;
	info->comptype	= p->s_comptype;
	info->format	= 0;
	sprintf(info->name,p->s_name->s_name);
	info->byteswap	= p->s_byteswap;
	info->nchans	= p->s_srcchans;
	info->frames	= p->s_frames;
	info->inst		= 0;
	info->markers	= 0;
	info->framesize = p->s_sampsize*p->s_srcchans;
	info->offset	= p->s_offset;
	
	return 0;
}


void *sfplay_new(t_symbol *s, short argc, t_atom *argv)
{
	t_sfplay *x;
	t_aqueue *a;
	short i,intarg = 0;
	long nchans = 1, bufsize,posout=0;
	long ibufsize = SF_BUFSIZE_DEF;
	t_symbol *listname = ps_nothing;
	t_symbol *myname = ps_nothing;
	
	if (argc) {
		if (argv->a_type == A_SYM)
			listname = argv->a_w.w_sym;
		else {
			nchans = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
			intarg = 1;
		}
		argc--;
		argv++;
		if (argc && argv->a_type != A_SYM) {
			if (intarg)
				ibufsize = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
			else
				nchans = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
			argc--;
			argv++;
			if (argc && argv->a_type != A_SYM) {
				if (intarg)
					posout = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
				else
					ibufsize = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
				argc--;
				argv++;
				if (argc) {
					if (intarg) {
						if (argv->a_type == A_SYM) 
							myname = argv->a_w.w_sym;
					} else {
						if (argv->a_type != A_SYM) 
							posout = argv->a_type == A_LONG? argv->a_w.w_long : argv->a_w.w_float;
					}
					argc--;
					argv++;
					if (argc && (!intarg) && (argv->a_type == A_SYM))
						myname = argv->a_w.w_sym;
				}
			}
		}
	}
	
	if (nchans < 1)
		nchans = 1;
	else if (nchans > SF_MAXCHANS)
		nchans = SF_MAXCHANS;

	posout = MIN((SF_MAXVECS-nchans),posout);
	if (!ibufsize)
		ibufsize = SF_BUFSIZE_DEF; // if a zero is provided then use the default value
	bufsize = ibufsize;
	bufsize = ((long)((float)ibufsize/(float)SF_BUFSIZE_QUANT + 0.5)) * SF_BUFSIZE_QUANT; //changed for sf_bufsize of 8factorial to avoid wrapping problems
	if (bufsize < SF_BUFSIZE_QUANT)
		bufsize = SF_BUFSIZE_QUANT;
	//bufsize *= nchans; //changed-jkc
	a = aq_new(bufsize * SF_MULT, bufsize*SF_ATOM_MULT);
	if (!a) {
		error("out of memory");
		return 0;
	}
	if (posout>2) posout=2;
	else if (posout<0) posout=0;
	
	if (x = (t_sfplay *)object_alloc(sfplay_class)) {
		x->p_invalid = false;
		x->p_process = false;
		x->p_ibufsize = ibufsize;
		dsp_setup((t_pxobject *)x,2);
		x->p_done = bangout((t_object *)x);
		for (i=0; i < (nchans+posout); i++)
	    	outlet_new((t_object *)x, "signal");
		x->p_bound = (listname == ps_nothing)? 0 : listname;
	    x->p_clock = clock_new(x,(method)sfplay_tick);
	    x->p_loopclock = clock_new(x,(method)sfplay_looptick);
		x->p_updateclock = clock_new(x,(method)sfplay_updatetick);
		x->p_updateqelem = qelem_new(x,(method)sfplay_ctrl);
		x->p_linked = false;
		x->p_outchans = nchans;
		x->p_aq = a;
		x->p_status = SF_Idle;
		x->p_bufsize = bufsize;
		x->p_pendcount = 0;
		x->p_current = 0;
		x->p_files = 0;
		x->p_cues = 0;
		x->p_now = 0;
		x->p_next = 0;
		x->p_one.c_data = t_getbytes(bufsize * SF_PRELOAD);
		if (!x->p_one.c_data) {
			object_error((t_object *)x, "out of memory for preload");
			return 0;
		}
		x->p_one.c_datasize = bufsize * SF_PRELOAD;
		x->p_one.c_pds = 0;
		x->p_one.c_no = 1;
		x->p_one.c_parent = 0;
		x->p_one.c_start = 0.;
		x->p_one.c_endpoint = 0.;
		x->p_one.c_byteoffset = 0;
		x->p_one.c_end = 0;
		x->p_one.c_bidir = 0; //maybe this could be user settable later
		x->p_one.c_revdata = 0;
		x->p_one.c_revpds = 0;
		x->p_one.c_revdatasize = 0;
		x->p_one.c_speed = 1.;
		x->p_one.c_valid = 1; 
		x->p_one.c_busy = 0;  
		
		// sustain loop stuff - maybe user-settable one day, too: for now cue 1 has no loop
		x->p_one.c_susloop 	= 0;
		x->p_one.c_susdata 	= 0;
		x->p_one.c_suspds 	= 0;
		x->p_one.c_susdatasize = 0;
		x->p_one.c_susstart = 0;
		x->p_one.c_susendpoint = 0;
		x->p_one.c_susend 	= 0;
		x->p_one.c_susbyteoffset = 0;
		
		x->p_busy = 0;
		x->p_flags = 0;
		x->p_opendeferred = 0;
		x->p_speed = 1.;
		x->p_lastdir = 1;
		x->p_loop = 0;
		
		x->p_sustain = 0;
		x->p_sustaining = 0;
		x->p_sloopstart = 0.;
		x->p_sloopend = 0.;
		
		x->p_loopone = 0;
		x->p_looppalin = 0;
		
		x->p_posout = posout;
		x->p_switch = FALSE;
		x->p_sr	= sys_getsr(); //placeholder
		x->p_name = myname;
		x->p_modout = 0;
		x->p_iorequest = asyncfile_params_new();	
	
		symbol_bind(myname,(t_object *)x);
		object_register(CLASS_BOX, symbol_unique(), x);
		ps_X->s_thing = (t_object *)x;
	}
	return (x);
}

short symbol_bind(t_symbol *s, t_object *x)
{
	if (s==ps_nothing) return -1;
	if (s->s_thing) {
		object_error((t_object *)x, "%s: already in use",s->s_name);
		return -1;
	}
	s->s_thing = x;
	return 0;
}

void symbol_unbind(t_symbol *s, t_object *x)
{
	if (s==ps_nothing) return;
	if (s->s_thing == x) s->s_thing = 0;
}

long sfplay_acceptsdrag(t_sfplay *x, t_object *drag, t_object *view)
{
	if (jdrag_matchdragrole(drag, ps_audiofile, 0)) {
		jdrag_object_add(drag, (t_object *)x, gensym("open"));
		return true;
	}
	return false;
}

// controller interface

double sfplay_postoms(t_sfplay *x, double pos)
{
	t_sndfileinfo info;
	double totalms;
	
	sfplay_getsfinfo(x,&info);
	if (!info.sr)
		return 0;
	totalms = info.frames / (info.sr * 0.001);
	return totalms * pos;
}

double sfplay_mstopos(t_sfplay *x, double ms)
{
	t_sndfileinfo info;
	double totalms;
	
	sfplay_getsfinfo(x,&info);
	if (!info.sr)
		return 0;
	totalms = info.frames / (info.sr * 0.001);
	if (totalms)
		return ms / totalms;
	else
		return 0.;
}

double sfplay_curms(t_sfplay *x)
{
	t_aqueue *a = x->p_aq;
	long diff,framesize;
	t_sfile *s = x->p_current;
	double headfrak, sample;
	double pos;
	
	if (!s || !a) {
		cpost("bad s or a\n");
		return 0;
	}
		
	if (x->p_speed >= 0){
		diff = aq_spacetoread(a);
	} else {
		diff = -aq_spacetoread_rev(a);	
	}
	framesize = s->s_sampsize*s->s_srcchans;
	headfrak = ((long)a->a_head) - a->a_head; 
	sample = (((double)(x->p_where-diff))/(double)framesize) + headfrak;
	pos = sample*(1000./(double)s->s_srate);
	return pos;
}

void sfplay_getplaystate(t_sfplay *x, long *play, double *pos)
{
	*pos = sfplay_mstopos(x,sfplay_curms(x));
	*play = x->p_status == SF_Playing;
}

void sfplay_toggleplay(t_sfplay *x)
{
	sfplay_controller(x);
	if (x->p_status == SF_Playing)
		sfplay_int(x,0);
	else
		sfplay_int(x,1);
	qelem_set(x->p_updateqelem);
}

void sfplay_pos(t_sfplay *x, double pos)
{
	double ms = sfplay_postoms(x,pos);
	
	sfplay_controller(x);
	sfplay_seek(x,ms,0.);
	qelem_set(x->p_updateqelem);
}

void sfplay_ff(t_sfplay *x)
{
	cpost("ff!\n");
	// implement some other time in terms of speed I guess?
}

void sfplay_rw(t_sfplay *x)
{
	cpost("rw!\n");
	// implement some other time in terms of speed I guess?
}

void sfplay_ctrl(t_sfplay *x)
{
	object_notify(x, ps_update, NULL);
}

void sfplay_controller(t_sfplay *x)	
{
	t_object *ctrl;
	
	// the first time, send a "link" message back to the controller object
	ctrl = ps_controller->s_thing;
	if (ctrl) {
		object_method(ctrl,gensym("link"),x);
		object_notify(x, ps_update,NULL);
		x->p_linked = true;
	}
}

