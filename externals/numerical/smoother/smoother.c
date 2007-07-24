/* Smoother Object: Independent multitrack 1st order low-pass filter and median filter */
/* Written by Alejandro R. Sedeno and Tristan Jehan at the MIT Media Laboratory */
/* Macho port by Michael Zbyszynski */

#include "ext.h"

/* CONSTANTS */
#define MAX_CHANS 50	// Max Channels
#define DEF_CHANS 1		// Default number of channels

#define MED_SIZE 50		// Median Filter Constants
#define DEF_SIZE 1		// Default Median Filter size

#define DEF_COEFF 1.0	// Default Low Pass Filter coefficient

#define COPYRIGHT "copyright © 2001 Massachusetts Institute of Technology"
#define VERSION "1.3"

/* structure definition of the object */
typedef struct medfilt
{
	float m_sorted[MED_SIZE];
	float m_pipe[MED_SIZE];
	long m_cnt;
	long m_max;
	float m_value;
	long m_reset;

} MedFilter;

typedef struct lpfilt
{
	float l_coeff;
	float l_value;

} LPFilter;

typedef struct smoother
{
	Object s_ob;					// required header
	void *s_out[MAX_CHANS];			// outlets
	void *s_proxy[MAX_CHANS - 1];	// inlets
	long s_chans;					// number of chanels, 0 < x <= MAX_CHANS
	long s_inletnumber;				// for proxy
	MedFilter s_mf[MAX_CHANS];		// median filters
	LPFilter s_lpf[MAX_CHANS];		// low pass filters
	long listmode;
	long envmode;
	
} Smoother;

/* globalthat holds the class definition */
void *smoother_class;

/* Prototypes */

// Median Filter Prototypes
void mf_init(MedFilter *f, long size);		// Initialize the Median Filter
float mf_process(MedFilter *f, float data);	// Process the incoming value
void mf_sort(MedFilter *f);					// Sorts the sorted queue
void mf_resize(MedFilter *f, long newsize);	// Change the filter's size

// Low Pass Filter Prototypes
void lpf_init(LPFilter *f, float coeff);
float lpf_process(LPFilter *f, float data);		// Process the incoming value
void lpf_setCoeff(LPFilter *f, float coeff);	// Changes the filter's coefficient

// Smoother Prototypes
Smoother *smoother_new(Symbol *s, short argc, Atom *argv);
void smoother_free(Smoother *s);
void smoother_mfsize(Smoother *s, Symbol *sym, short argc, Atom *argv);
void smoother_lpfcoeff(Smoother *s, Symbol *sym, short argc, Atom *argv);
void smoother_float(Smoother *s, double f);
void smoother_int(Smoother *s, long l);
void smoother_all(Smoother *s, Symbol *sym, short argc, Atom *argv);
void smoother_clear(Smoother *s, Symbol *sym);
void smoother_env(Smoother *s, Symbol *sym, short argc, Atom *argv);
void smoother_clearAll(Smoother *s, Symbol *sym);
void read_sym(Smoother *s, Atom *argv, int i);
void smoother_print(Smoother *s);

/* End Prototypes */


void mf_init(MedFilter *f, long size)
{
	f->m_max = (size >= 1 && size <= MED_SIZE) ? size : DEF_SIZE;
	f->m_cnt = 0;
	f->m_reset = 1;
}

float mf_process(MedFilter *f, float data)
{
	short i;
	float rep;

	/* push up the count if necessary */
	if (f->m_cnt < f->m_max)
	{
		i = f->m_cnt++;
		f->m_sorted[i] = f->m_pipe[i];
	}

	/* Add new element to the sorted queue... */
	i = f->m_cnt - 1;
	rep = f->m_pipe[i];
	for(i = 0; i < f->m_cnt; i++)
	{
		/* replace the oldest */
		if(f->m_sorted[i] == rep)
		{
			f->m_sorted[i] = data;
			break;
		}
	}

	/*...and sort it */
	mf_sort(f);

	/* push the new element into the pipe */
	for(i = f->m_cnt - 1; i > 0; i--)
	{
		f->m_pipe[i] = f->m_pipe[i-1];
	}
	f->m_pipe[0] = data;

	/* set median output */
	i = ((f->m_cnt + 1) / 2) - 1;
	if (f->m_cnt % 2 == 0)
	{
		f->m_value = (f->m_cnt == 0) ? 0 : (f->m_sorted[i] + f->m_sorted[i+1]) / 2;

	}
	else
	{
		f->m_value = f->m_sorted[i];
	}
	return f->m_value;
}

void mf_sort(MedFilter *f)
{
	float tmp = 0;
	short tmp_i = 0;
	short i;
	short j;

	for(i = 0; i < f->m_cnt; i++)
	{
		tmp = f->m_sorted[i];
		tmp_i = i;

		/* find least */
		for(j = i+1; j < f->m_cnt; j++)
		{
			if(f->m_sorted[j] < tmp)
			{
				tmp = f->m_sorted[j];
				tmp_i = j;
			}
		}

		/* sort this element */
		if (tmp_i != i)
		{
			f->m_sorted[tmp_i] = f->m_sorted[i];
			f->m_sorted[i] = tmp;
		}
	}
}

void mf_resize(MedFilter *f, long newsize)
{
	short i;
	/* Adjust size */
	f->m_max = (newsize >= 1 && newsize <= MED_SIZE) ? newsize : DEF_SIZE;

	/* repick elements if needed */	
	if (f->m_cnt > f->m_max)
	{
		for(i = 0; i < f->m_max; i++)
		{
			f->m_sorted[i] = f->m_pipe[i];
		}
		f->m_cnt = f->m_max;
	}
	else if (f->m_cnt < 0)
		f->m_cnt = 0;

	/* and resort */
	mf_sort(f);
}

void lpf_init(LPFilter *f, float coeff)
{
	f->l_value = 0;
	f->l_coeff = (coeff >= 0.0 && coeff <= 1.0) ? coeff : DEF_COEFF;
}

float lpf_process(LPFilter *f, float data)
{
	f->l_value = f->l_coeff * data + (1 - f->l_coeff) * f->l_value;
	return f->l_value;
}	

void lpf_setCoeff(LPFilter *f, float coeff)
{
	f->l_coeff = (coeff >= 0.0 && coeff <= 1.0) ? coeff : DEF_COEFF;
}

void read_sym(Smoother *s, Atom *argv, int i) {
	
	if (argv[i].a_w.w_sym == gensym("env")) {
		s->envmode = 1;
	} else if (argv[i].a_w.w_sym == gensym("noenv")) {
		s->envmode = 0;
	} else if (argv[i].a_w.w_sym == gensym("list")) {
		s->listmode = 1;
	} else if (argv[i].a_w.w_sym == gensym("nolist")) {
		s->listmode = 0;
	} else {
		error("Smoother: can't read text argument. Default selected.");
	}
}

Smoother *smoother_new(Symbol *sym, short argc, Atom *argv)
{
	Smoother *s;
	long chcnt = DEF_CHANS;
	long mfsize = 1;
	float lpfcoeff = 1;
	short i;

	s = newobject(smoother_class);
	
	s->envmode = 0;
	s->listmode = 1;
	s->s_chans = chcnt;

	switch (argc) { // Read arguments
		case 0:
			break;
		case 1: 
			if (argv[0].a_type == A_FLOAT) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0) ? lpfcoeff : DEF_COEFF;
			} else if (argv[0].a_type == A_LONG) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
			} else if (argv[0].a_type == A_SYM) {
				read_sym(s,argv,0);
			}
			break;
		case 2:
			if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_LONG) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
			} else if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				read_sym(s,argv,1);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				read_sym(s,argv,1);
			} else {
				error("Smoother: can't read arguments. Default selected.");
			}
			break;
		case 3:
			if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG && argv[2].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,2);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_LONG && argv[2].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,2);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT && argv[2].a_type == A_LONG) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
			} else if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG && argv[2].a_type == A_LONG) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				mfsize = argv[1].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT && argv[2].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				read_sym(s,argv,2);
			} else if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_SYM && argv[2].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				read_sym(s,argv,1);
				read_sym(s,argv,2);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_SYM && argv[2].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				read_sym(s,argv,1);
				read_sym(s,argv,2);
			} else {
				error("Smoother: can't read arguments. Default selected.");
			}
			break;
		case 4:
			if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG && argv[2].a_type == A_SYM && argv[3].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,2);
				read_sym(s,argv,3);
			} else if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG && argv[2].a_type == A_LONG && argv[3].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				mfsize = argv[1].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,3);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_LONG && argv[2].a_type == A_SYM && argv[3].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[1].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,2);
				read_sym(s,argv,3);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT && argv[2].a_type == A_LONG && argv[3].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,3);
			} else if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT && argv[2].a_type == A_SYM && argv[3].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				read_sym(s,argv,2);
				read_sym(s,argv,3);
			} else {
				error("Smoother: can't read arguments. Default selected.");
			}
			break;
		case 5:
			if (argv[0].a_type == A_LONG && argv[1].a_type == A_FLOAT && argv[2].a_type == A_LONG && argv[3].a_type == A_SYM && argv[4].a_type == A_SYM) {
				mfsize = argv[0].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				lpfcoeff = argv[1].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,3);
				read_sym(s,argv,4);
			} else if (argv[0].a_type == A_FLOAT && argv[1].a_type == A_LONG && argv[2].a_type == A_LONG && argv[3].a_type == A_SYM && argv[4].a_type == A_SYM) {
				lpfcoeff = argv[0].a_w.w_float;
				lpfcoeff = (lpfcoeff >= 0.0 && lpfcoeff <= 1.0)
					? lpfcoeff
					: DEF_COEFF;
				mfsize = argv[1].a_w.w_long;
				mfsize = (mfsize > 0 && mfsize <= MED_SIZE)
					? mfsize
					: DEF_SIZE;
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
				read_sym(s,argv,3);
				read_sym(s,argv,4);
			} else {
				error("Smoother: can't read arguments. Default selected.");
			}
			break;
		default:
				error("Smoother: too many arguments. Default selected.");
			
	 }

	s->s_chans = chcnt;
	s->s_inletnumber = 0;
	
	for(i = chcnt - 1; i >= 0; i--)
	{
		if (i > 0 && !s->listmode)
			s->s_proxy[i-1] = proxy_new(s,(long)i,&s->s_inletnumber);
		if (i == 0 || !s->listmode)
			s->s_out[i] = outlet_new(s,0L);
		mf_init(&s->s_mf[i], mfsize);
		lpf_init(&s->s_lpf[i], lpfcoeff);
	}
	
	post("--- Smoother ---");	
	post("	LPF coefficients = %.2f", lpfcoeff);
	post("	Median filter length = %d", mfsize);
	post("	Number of channels = %d", chcnt);
	if (s->listmode) {
		post("	list mode = list");
	} else {
		post("	list mode = nolist");
	}	
	if (s->envmode) {
		post("	list mode = env");
	} else {
		post("	list mode = noenv");
	}	
	post("");
	
	return s;

}

void smoother_free(Smoother *s)
{
	short i;

	if(!s->listmode)
	{
		for(i = 0; i < s->s_chans - 1 ; i++)
		{
			freeobject(s->s_proxy[i]);
		}
	}
}

void smoother_mfsize(Smoother *s, Symbol *sym, short argc, Atom *argv)
{
	short i;

	if (sym == gensym("medall"))
	{
		if (argc >= 1 && argv[0].a_type == A_LONG)
		{
			for(i = 0; i < s->s_chans; i++)
			{
				mf_resize(&s->s_mf[i], argv[0].a_w.w_long);
			}
		}
		return;
	}
	
	if (s->listmode != 0)
	{
		for(i = 0; i < argc && i < s->s_chans; i++)
			if (argv[i].a_type == A_LONG)
				mf_resize(&s->s_mf[i], argv[i].a_w.w_long);
	}
	else
	{
		if (argc >= 1 && argv[0].a_type == A_LONG)
			mf_resize(&s->s_mf[s->s_inletnumber], argv[0].a_w.w_long);
	}
}

void smoother_lpfcoeff(Smoother *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("lpfall"))
	{
		f = -1.0;
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT)
				f = argv[0].a_w.w_float;
			if (f != -1.0)
			{
				for(i = 0; i < s->s_chans; i++)
					lpf_setCoeff(&s->s_lpf[i], f);
			}
		}
		return;
	}
	if (s->listmode != 0)
	{
		for(i = 0; i < argc && i < s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				lpf_setCoeff(&s->s_lpf[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				lpf_setCoeff(&s->s_lpf[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				lpf_setCoeff(&s->s_lpf[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				lpf_setCoeff(&s->s_lpf[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}
    
}

void smoother_float(Smoother *s, double f)
{
	short i;
	i = s->s_inletnumber;
		
	if (s->envmode && f>s->s_lpf[i].l_value) { // mode envelope follower
		s->s_lpf[i].l_value = mf_process(&s->s_mf[i], f);
		outlet_float(s->s_out[i], s->s_lpf[i].l_value);
	} else {
		outlet_float(s->s_out[i], lpf_process(&s->s_lpf[i], mf_process(&s->s_mf[i], f)));
	}		
}

void smoother_int(Smoother *s, long l)
{
	smoother_float(s, (float)l);
}


void smoother_all(Smoother *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;
	Atom ret[MAX_CHANS];

	for(i = 0; i < argc && i < s->s_chans; i++)
	{	
		f = 0.0;
		if(argv[i].a_type == A_LONG)
			f = (float)argv[i].a_w.w_long;
		else if (argv[i].a_type == A_FLOAT)
			f = argv[i].a_w.w_float;

			if (s->envmode && f>s->s_lpf[i].l_value) { // mode envelope follower
				s->s_lpf[i].l_value = mf_process(&s->s_mf[i], f);
				SETFLOAT(ret+i, s->s_lpf[i].l_value);
			} else {
				SETFLOAT(ret+i,lpf_process(&s->s_lpf[i], mf_process(&s->s_mf[i], f)));
			}
	}

	outlet_list(s->s_out[0], 0L, i, ret);
}

void smoother_clear(Smoother *s, Symbol *sym)
{
	short i;
	i = s->s_inletnumber;
	s->s_mf[i].m_cnt = 0;
	s->s_lpf[i].l_value = 0;
}

void smoother_clearAll(Smoother *s, Symbol *sym)
{
	short i;
	for(i = 0; i < s->s_chans; i++)
	{	
		s->s_mf[i].m_cnt = 0;
		s->s_lpf[i].l_value = 0;
	}
}

void smoother_env(Smoother *s, Symbol *sym, short argc, Atom *argv)
{
	if (sym == gensym("env")) {
		if (argv[0].a_type == A_LONG) {
			if (argv[0].a_w.w_long == 0) {
				s->envmode = 0;
			} else {
				s->envmode = 1;
			}
		} else if (argv[0].a_type == A_FLOAT) {
			if (argv[0].a_w.w_float == 0.0f) {
				s->envmode = 0;
			} else {
				s->envmode = 1;
			}
		}
	}
}

void smoother_print(Smoother *s) 
{
	int i;
	
	post("--- Smoother ---");
	post("	Number of channels = %d", s->s_chans);
	if (s->listmode) {
		post("	list mode = list");
	} else {
		post("	list mode = nolist");
	}	
	if (s->envmode) {
		post("	list mode = env");
	} else {
		post("	list mode = noenv");
	}	
	post("  #: MF, LPF");
	for (i=0; i<s->s_chans; i++) {
		post("  %d: %d, %.2f",i+1,s->s_mf[i].m_max, s->s_lpf[i].l_coeff);
	}
	post("");

}

int main(void)
//void main(fptr *f)
{
	post("Smoother object version " VERSION " by Tristan Jehan and Alejandro R. Sedeno");
	post(COPYRIGHT);
	post(" ");
	
	setup(								// Setup Object
		(Messlist**) &smoother_class,	// Pointer to class
		(method) smoother_new,			// Constructor
		(method) smoother_free,			// Destructor
		(short) sizeof(Smoother),		// Class Size
		0L,								// Menu function (not used)
		A_GIMME, 0);

	/* bind your methods to symbols */
	addfloat((method)smoother_float);
	addint((method)smoother_int);

	addmess((method)smoother_clear, "clear", 0);
	addmess((method)smoother_clearAll, "clearall", 0);
	addmess((method)smoother_env, "env", A_GIMME, 0);
	addmess((method)smoother_mfsize, "med", A_GIMME, 0);
	addmess((method)smoother_mfsize, "medall", A_GIMME, 0);
	addmess((method)smoother_lpfcoeff, "lpf", A_GIMME, 0);
	addmess((method)smoother_lpfcoeff, "lpfall", A_GIMME, 0);
	addmess((method)smoother_print, "print", 0);
	addmess((method)smoother_all, "list", A_GIMME, 0);

	/* list object in the new object list */
//	finder_addclass("Analysis","smoother");
return 0;
}