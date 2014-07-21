#include "ext.h"
#include "ext_common.h"
#include "z_dsp.h"
#include "sf2.h" // the header for sf_cue.c

#define obj2class(x) ((t_class *)(((t_object *)(x))->o_messlist[-1].m_sym))

enum {
	SF_MAXCHANS = 16,
	SF_MAXVECS = 18,
	//SF_BUFSIZE = 16384,
	SF_BUFSIZE_DEF = 40320, // 8factorial
	SF_BUFSIZE_QUANT = 20160, // 8factorial over two...ensures that we won't have problems wrapping 
	SF_MULT = 8,
	SF_ATOM_MULT = 4,
	SF_PRELOAD = 1,
	SF_MAXPEND = 128,
	SF_EMBED = 1,
	SF_WEIRDCUE = 18811,
	SF_MAX_SWITCH_PER_VECTOR = 16
};

typedef struct _cue_read_req
{
	t_object *c_obj;
	t_sfile *c_s;
	t_cue *c_cue;
	t_cue *c_tmpcue;

} t_cue_read_req;

void sched_cue_read(void *x, t_symbol *s, short argc, t_atom *argv);
long cue_read(t_cue_read_req *crr);
long cue_read_rev(t_cue_read_req *crr);
long cue_read_sus(t_cue_read_req *crr);
void cue_done(t_asyncfile_params *p);
void cue_done_rev(t_asyncfile_params *p);
void cue_done_sus(t_asyncfile_params *p);

void cue_done_epilog(void *x, void *p, long result);
void cue_done_epilog_doit(void *x, t_symbol *s, short argc, t_atom *argv);

t_symbol *ps_getcurrentfile,*ps_unrefer,*ps_getcues,*ps_setcues,*ps_sfilenew,*ps_getbufsize;
t_symbol *ps_cuereport;
t_asyncfile_cb cue_async_cb=NULL,cue_async_cb_rev=NULL,cue_async_cb_sus=NULL;


void cue_init(void)
{
	ps_getcurrentfile = gensym("getcurrentfile");
	ps_getbufsize     = gensym("getbufsize");
	ps_unrefer  = gensym("unrefer");
	ps_getcues  = gensym("getcues");
	ps_setcues  = gensym("setcues");
	ps_sfilenew = gensym("sfilenew");
	ps_cuereport = gensym("cuereport");
	
	cue_async_cb = asyncfile_callback_new((method)cue_done);
	cue_async_cb_rev = asyncfile_callback_new((method)cue_done_rev);
	cue_async_cb_sus = asyncfile_callback_new((method)cue_done_sus);
}

void cue_preload(void *x, t_symbol *s, short argc, t_atom *argv)
{
	defer(x,(method)cue_dopreload,s,argc,argv);
}

void cue_dopreload(void *x, t_symbol *s, short argc, t_atom *argv)
{
	long cn,bidir=0, susloop = 0;
	double start = 0., end = 0.,speed=1., susstart = 0., susend = 0.;
	t_symbol *name;
		
	if (!argc || argv->a_type != A_LONG) {
		error("%s: preload: requires cue number", obj2class(x)->c_sym->s_name);
		return;
	}
	cn = argv->a_w.w_long;
	if (cn <= 1) {
		error("%s: preload: illegal cue number %ld", obj2class(x)->c_sym->s_name, cn);
		return;
	}
	argv++;
	argc--;
	if (argc && argv->a_type == A_SYM) {
		name = argv->a_w.w_sym;
		argv++;
		argc--;
	} else {
		t_sfile *current;
		current = (t_sfile *) mess0(x,ps_getcurrentfile);
		if (!current) {  //jkc, must change to be a message
			error("%s: preload: no current file",obj2class(x)->c_sym->s_name);
			return;
		}
		name = current->s_name; //jkc, must change to be a message
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		start = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		end = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		bidir = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		speed = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		susloop = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long; // 2 means get from file
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		susstart = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
		argc--;
		argv++;
	}
	if (argc && (argv->a_type == A_FLOAT || argv->a_type == A_LONG)) {
		susend = argv->a_type == A_FLOAT? argv->a_w.w_float : argv->a_w.w_long;
	}
	cue_new(x,cn,name,start,end,bidir,speed, susloop, susstart, susend);
}

void cue_freedata(t_cue *c)
{
	c->c_next=NULL;
	if (c->c_data)
		t_freebytes(c->c_data,c->c_datasize);
	if (c->c_revdata)
		t_freebytes(c->c_revdata,c->c_revdatasize);
	if (c->c_susdata)
		t_freebytes(c->c_susdata,c->c_susdatasize);
	t_freebytes(c,sizeof(t_cue));
}

void cue_free(void *x, t_cue *me)
{
	t_cue *c,*prev;
	/* should be wrapped by caller
	if (me == &x->p_one)
		return;
	if (x->p_next == me)
		x->p_next = 0;
	if (x->p_now == me)
		x->p_now = 0;
	*/
	for (prev = 0, c = (t_cue *)mess0(x,ps_getcues); c; prev = c, c = c->c_next) {
		if (c == me) {
			if (prev)
				prev->c_next = c->c_next;
			else {
				mess1(x,ps_setcues,c->c_next);
			}
			mess2(x,ps_unrefer,c,c->c_parent);  //tmp. apb preload bug -jkc
			cue_freedata(me);
			break;
		}
	}
}

// make a new private cue
t_cue *cue_new(void *x, long number, t_symbol *file, double start, double endpoint, long bidir, double speed, long susloop, double susstart, double susendpoint)
{
	t_cue *c;			// cue pointer
	t_sfile *s;			// soundfile pointer
	Boolean cn = false; // free cue flag
	
	long framesize;	// frame size in bytes (nchans * sampsize)
	long offset;	// cue start in samples
	long ending;	// cue end in samples
	long len;		// cue length in samples (ending-offset)
	long end;		// cue end location in bytes
	long bufsize;	// buffer size from sfplay
	long preload;	// raw preload size
	long buflen;	// adjusted preload size in bytes
	long revbuflen; // adjusted reverse buffer size in bytes
	
	long susoffset = 0;	// sustain loop start in samples
	long susending = 0; // sustain loop end in samples
	long suslen = 0;	// sustain loop length in samples
	long susend = 0;	// sustain loop end in bytes
	long susbuflen = 0; // adjusted sustain loop buf size in bytes
			
	if (!(c = cue_find(mess0(x,ps_getcues),number))) {  //jkc, change to method
		c = t_getbytes(sizeof(t_cue));
		c->c_no = number;
		c->c_data = 0;
		c->c_datasize = 0;
		c->c_revdata = 0;
		c->c_revdatasize = 0;
		c->c_end = 0;
		c->c_byteoffset = 0;
		c->c_parent = 0;
		c->c_pds = 0;
		c->c_next = mess0(x,ps_getcues);
		mess1(x,ps_setcues,c);
		cn = true;
		c->c_valid = FALSE;
		c->c_busy = 0;
		c->c_bidir = bidir;
		c->c_speed = speed;
		
		c->c_susdata = 0;
		c->c_suspds = 0;
		c->c_susdatasize = 0;
	} 
	
//	while (c->c_busy) //wait to load cue. a better way to do this might be to use a queue.
//		;
	if (!c->c_busy)
	{
		if ((!c->c_parent) || (c->c_parent->s_name != file)) {
			long sfType[7];
			
			sfType[0] = FOUR_CHAR_CODE('AIFF');
			sfType[1] = FOUR_CHAR_CODE('Sd2f');
			sfType[2] = FOUR_CHAR_CODE('AIFC');
			sfType[3] = FOUR_CHAR_CODE('NxTS'); //next/sun typed by soundhack
			sfType[4] = FOUR_CHAR_CODE('ULAW'); //next/sun typed by peak and others
			sfType[5] = FOUR_CHAR_CODE('WAVE'); 
			sfType[6] = FOUR_CHAR_CODE('.WAV'); //wave typed by soundhack
	
			s = mess4(x,ps_sfilenew,file,0,7,sfType);
			if (!s) {
				//already reported by sfilenew
				//error("%s: can't find %s",obj2class(x)->c_sym->s_name,file->s_name);
				if (cn)
					cue_free(x,c);
				return 0;
			}
		} else {
			s = c->c_parent;
		}
		
		framesize = s->s_srcchans * s->s_sampsize; 
		offset = start * s->s_srate * 0.001;
		if (offset > s->s_frames) {
			error("%s: %s: cue past end of file",obj2class(x)->c_sym->s_name,file->s_name);
			offset = s->s_frames - 1;
		}
		ending = endpoint * s->s_srate * 0.001; 
		if (ending > s->s_frames) {
			post("%s: truncating cue %ld",obj2class(x)->c_sym->s_name,number);
			ending = s->s_frames;
		}
		if (!ending || ending < offset) {
			end = s->s_size;
			len = s->s_frames - offset;
		} else {
			end = (ending * framesize) + s->s_offset;
			len = ending - offset;
		}
		
		bufsize = (long)(mess0(x,ps_getbufsize)) * SF_PRELOAD;
		//preload = bufsize;
		//preload = (preload / framesize) * framesize; /// huh??? why the acrobatics? -Rd
		preload = (bufsize / framesize) * framesize;
		buflen = MIN((len*framesize), preload);
		//calculate these values for the reverse buffer too!
		if (bidir) {
			// if no endpoint then just an inpoint, else bounded begin/end
			len = (endpoint==0) ? ((offset * framesize) - s->s_offset) : (end - (offset * framesize));
			revbuflen = MIN(len, preload);
		} else {
			revbuflen=0;
		}
		
		// calculate sustain loop buffer // VERIFY THIS IS RIGHT -Rd
		if (susloop) {
			susoffset = (susloop == 2) ? s->s_susloopstart : susstart * s->s_srate * 0.001; 
			if (susoffset > s->s_frames) {
				error("%s: %s: sustain loop past end of file",obj2class(x)->c_sym->s_name,file->s_name);
				susoffset = s->s_frames - 1;
			}
			susending = (susloop == 2) ? s->s_susloopend : susendpoint * s->s_srate * 0.001; 
			if (susending > s->s_frames) {
				post("%s: truncating sustain loop %ld",obj2class(x)->c_sym->s_name,number);
				susending = s->s_frames;
			}
			
			if (!susending || susending < susoffset) {
				susend = s->s_size;
				suslen = s->s_frames - susoffset;
			} else {
				susend = (susending * framesize) + s->s_offset;
				suslen = susending - susoffset;
			}
			susbuflen = MIN(suslen*framesize, preload);
		} else {
			susbuflen=0;
		}
		
		{
			short *tmp,*tmp2=0, *tmp3 = 0;
			t_cue *c2;
			t_cue_read_req *crr;
			
			tmp = (short *)t_getbytes(bufsize);
			if (bidir) tmp2 = (short *)t_getbytes(bufsize);
			if (susloop) tmp3 = (short *)t_getbytes(bufsize);
			c2 = (t_cue *)t_getbytes(sizeof(t_cue));
			crr = (t_cue_read_req *)t_getbytes(sizeof(t_cue_read_req));
			crr->c_obj = x;
			
			if ((!tmp)||(!c2)||(!crr)||(bidir&&(!tmp2))||(susloop&&(!tmp3))) {
				error("%s: out of memory loading cue %ld",obj2class(x)->c_sym->s_name,number);
				
				if (cn) cue_free(x,c);
				if (tmp) t_freebytes(tmp,bufsize);
				if (tmp2) t_freebytes(tmp2,bufsize);
				if (tmp3) t_freebytes(tmp3,bufsize);
				if (c2) t_freebytes(c2,sizeof(t_cue));
				if (crr) t_freebytes(crr,sizeof(t_cue_read_req));
				
				return 0;
			}
			c2->c_parent 	= s;
			c2->c_data 		= tmp;
			c2->c_pds 		= buflen;
			c2->c_datasize 	= bufsize;
			c2->c_start 	= start;
			c2->c_endpoint 	= endpoint;
			c2->c_end 		= end;
			c2->c_byteoffset = offset * framesize;
			
			// bidirectional loop stuff
			c2->c_bidir 	= bidir;
			c2->c_revdata 	= tmp2;
			c2->c_revpds 	= revbuflen;
			c2->c_revdatasize = (bidir) ? bufsize : 0;
			c2->c_speed 	= speed;
			
			// sustain loop stuff
			c2->c_susloop 	= susloop;
//			post("susloop %d", c2->c_susloop); // TEMP
			c2->c_susdata 	= tmp3;
			c2->c_suspds 	= susbuflen;
			c2->c_susdatasize = (susloop) ? bufsize : 0;
			c2->c_susstart 	= (susloop == 2) ?  s->s_susloopstart / s->s_srate * 1000. : susstart;
			c2->c_susendpoint = (susloop == 2) ?  s->s_susloopend / s->s_srate * 1000. : susendpoint;
			c2->c_susend 	= susend; // calcualted from susending
			c2->c_susbyteoffset = susoffset * framesize;
			
			crr->c_s = s;
			crr->c_cue = c;
			crr->c_tmpcue = c2;	
			
			//we're using schedule below since it seems like there might be 
			//reentrancy problems with PBAsyncRead(). always do at interrupt.
			{
				t_atom a;
				
				a.a_type = A_OBJ;
				a.a_w.w_obj = (void *)crr;  //passing a pointer as an atom.
				
				schedule_delay(x, (method)sched_cue_read, 0L, 0L, 1, &a); //must fake as atom to use shedule		
			}
		} 
	} 

	return c;
}

// note that start could be 0

t_cue *cue_find(t_cue *start, long number)
{
	t_cue *c;
	
	for (c = start; c; c = c->c_next)
		if (c->c_no == number)
			return c;
	return 0;
}


void sched_cue_read(void *x, t_symbol *s, short argc, t_atom *argv)
{
	if (argc && argv && (argv->a_type==A_OBJ)) {  //passing a pointer as an object pointer in an atom
		cue_read((t_cue_read_req *)(argv->a_w.w_obj));
	}
}

// could merge read + read rev into one function
long cue_read(t_cue_read_req *crr)
{
	t_asyncfile_params *p=asyncfile_params_new();

	p->file = crr->c_s->s_asyncfile;
	p->buf = (Ptr)crr->c_tmpcue->c_data;
	p->requestcount = crr->c_tmpcue->c_pds;
	p->position = crr->c_s->s_offset + crr->c_tmpcue->c_byteoffset;
	p->arg = crr;

	crr->c_s->s_busy = 1;
	crr->c_cue->c_busy = 1;
	p->callback = cue_async_cb;
	asyncfile_read(p);
	return 0;

}

long cue_read_rev(t_cue_read_req *crr)
{
	t_asyncfile_params *p=asyncfile_params_new();

	p->file = crr->c_s->s_asyncfile;
	p->buf = (Ptr)crr->c_tmpcue->c_revdata;
	p->requestcount = crr->c_tmpcue->c_revpds;
	if (crr->c_tmpcue->c_endpoint) 
		p->position = crr->c_tmpcue->c_end - crr->c_tmpcue->c_revpds;
	else
		p->position = crr->c_s->s_offset + crr->c_tmpcue->c_byteoffset - crr->c_tmpcue->c_revpds;
	p->arg = crr;

	crr->c_s->s_busy = 1;
	crr->c_cue->c_busy = 1;
	p->callback = cue_async_cb_rev;
	asyncfile_read(p);
	return 0;
}

long cue_read_sus(t_cue_read_req *crr)
{
	t_asyncfile_params *p=asyncfile_params_new();

	p->file = crr->c_s->s_asyncfile;
	p->buf = (Ptr)crr->c_tmpcue->c_susdata;
	p->requestcount = crr->c_tmpcue->c_suspds;
	if (crr->c_tmpcue->c_susendpoint) 
		p->position = crr->c_tmpcue->c_susend - crr->c_tmpcue->c_suspds;
	else
		p->position = crr->c_s->s_offset + crr->c_tmpcue->c_susbyteoffset - crr->c_tmpcue->c_suspds;
	p->arg = crr;

	crr->c_s->s_busy = 1;
	crr->c_cue->c_busy = 1;
	p->callback = cue_async_cb_sus;
	asyncfile_read(p);
	return 0;
}

void cue_done(t_asyncfile_params *p)
{
	t_cue_read_req *crr = (t_cue_read_req *)p->arg;
	
	if (p->result||(!crr->c_tmpcue->c_bidir)) {
		cue_done_epilog(crr->c_obj, crr, p->result);
	} else if (crr->c_tmpcue->c_bidir) {
		//now do the read for the reverse buffer.
		cue_read_rev(crr); 
	} else if (crr->c_tmpcue->c_susloop) {
		//now do the read for the sustain loop buffer.
		cue_read_sus(crr); 
	} 
	asyncfile_params_free(p);
}


void cue_done_epilog(void *x, void *p, long result)
{
	t_atom a[2];
	
	SETOBJ(a,p);
	SETLONG(a+1,result);		
	schedule_defer(x,(method)cue_done_epilog_doit,0,NULL,2,a); 
}

void cue_done_epilog_doit(void *x, t_symbol *s, short argc, t_atom *argv)
{
	t_cue_read_req *crr;
	short *tmp,*tmprev, *tmpsus;
	long size,sizerev, sizesus;
	t_sfile *sf;
	method m=NULL;
	long err;
	
	if (argv) {
		crr=(t_cue_read_req *)argv[0].a_w.w_obj;
		if (argv[1].a_w.w_long) {
			error("%s: disk error %d on cue preload",obj2class(crr->c_obj)->c_sym->s_name,argv[1].a_w.w_long);
			if (crr->c_tmpcue->c_data)
				t_freebytes(crr->c_tmpcue->c_data,crr->c_tmpcue->c_datasize);
			if (crr->c_tmpcue->c_revdata)
				t_freebytes(crr->c_tmpcue->c_revdata,crr->c_tmpcue->c_revdatasize);
			if (crr->c_tmpcue->c_susdata)
				t_freebytes(crr->c_tmpcue->c_susdata,crr->c_tmpcue->c_susdatasize);
			err = -1;
		} else {
			//no reverse buffer, so we're done
			tmp = crr->c_cue->c_data;
			size = crr->c_cue->c_datasize;
			tmprev = crr->c_cue->c_revdata;
			sizerev = crr->c_cue->c_revdatasize;
			tmpsus = crr->c_cue->c_susdata;
			sizesus = crr->c_cue->c_susdatasize;
			sf = crr->c_cue->c_parent;
			
			//must happen before the switch
			if (sf && (sf != crr->c_tmpcue->c_parent))
				mess2(crr->c_obj,ps_unrefer,crr->c_cue,sf); //tmp. apb preload bug -jkc			

			crr->c_cue->c_valid = FALSE;
			crr->c_cue->c_data = crr->c_tmpcue->c_data;
			crr->c_cue->c_parent = crr->c_tmpcue->c_parent;
			crr->c_cue->c_pds = crr->c_tmpcue->c_pds;
			crr->c_cue->c_datasize = crr->c_tmpcue->c_datasize;
			crr->c_cue->c_start = crr->c_tmpcue->c_start;
			crr->c_cue->c_endpoint = crr->c_tmpcue->c_endpoint;
			crr->c_cue->c_end = crr->c_tmpcue->c_end;
			crr->c_cue->c_byteoffset = crr->c_tmpcue->c_byteoffset;
			crr->c_cue->c_bidir = crr->c_tmpcue->c_bidir;
			crr->c_cue->c_revdata = crr->c_tmpcue->c_revdata;
			crr->c_cue->c_revpds = crr->c_tmpcue->c_revpds;
			crr->c_cue->c_revdatasize = crr->c_tmpcue->c_revdatasize;
			crr->c_cue->c_speed = crr->c_tmpcue->c_speed;
			crr->c_cue->c_valid = TRUE;
			//sustain stuff
			crr->c_cue->c_susloop = crr->c_tmpcue->c_susloop;
			crr->c_cue->c_susdata = crr->c_tmpcue->c_susdata;
			crr->c_cue->c_suspds = crr->c_tmpcue->c_suspds;
			crr->c_cue->c_susdatasize = crr->c_tmpcue->c_susdatasize;
			crr->c_cue->c_susstart = crr->c_tmpcue->c_susstart;
			crr->c_cue->c_susendpoint = crr->c_tmpcue->c_susendpoint;
			crr->c_cue->c_susend = crr->c_tmpcue->c_susend;
			crr->c_cue->c_susbyteoffset = crr->c_tmpcue->c_susbyteoffset;
			
			if (tmp)
				t_freebytes(tmp,size);
			if (tmprev)
				t_freebytes(tmprev,sizerev);
			if (tmpsus)
				t_freebytes(tmpsus,sizesus);
			err = 0;		
		}
		crr->c_s->s_busy = 0;
		crr->c_cue->c_busy = 0;
		if (crr->c_obj&&(m=zgetfn(crr->c_obj,ps_cuereport)))
			(*m)(crr->c_obj,err,crr->c_cue,NULL);			
		freebytes(crr->c_tmpcue,(short)sizeof(t_cue));
		freebytes(crr,(short)sizeof(t_cue_read_req ));				
	}
}

void cue_done_rev(t_asyncfile_params *p)
{
	t_cue_read_req *crr = (t_cue_read_req *)p->arg;
	
	//cue_done_epilog(crr->c_obj, crr, p->result);
	if (!crr->c_tmpcue->c_susloop) {
		cue_done_epilog(crr->c_obj, crr, p->result);
	} else if (crr->c_tmpcue->c_susloop) {
		//now do the read for the sustain loop buffer.
		cue_read_sus(crr); 
	} 
	asyncfile_params_free(p);
}

void cue_done_sus(t_asyncfile_params *p)
{
	t_cue_read_req *crr = (t_cue_read_req *)p->arg;
	
	cue_done_epilog(crr->c_obj, crr, p->result);
	asyncfile_params_free(p);
}

