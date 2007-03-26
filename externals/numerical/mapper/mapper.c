/* 	Mapper Object: Independent multitrack scaling object
	Written by Tristan Jehan at the MIT Media Laboratory
	Copyright (c) 2003 Massachusetts Institute of Technology.
	All rights reserved.
*/

#include <math.h>
#include "ext.h"

/* CONSTANTS */
#define MAX_CHANS 50	// Max Channels
#define DEF_CHANS 1		// Default number of channels
#define DEF_COEFF 1.0	// Default exponential coefficient
#define DEF_CLIP 0
#define DEF_LIST 1
#define DEF_MIN 0
#define DEF_MAX 127

#define COPYRIGHT "copyright © 2003 Massachusetts Institute of Technology"
#define VERSION "1.01 "

/* structure definition of the object */
typedef struct map
{
	float m_coeff;
	float m_input1;
	float m_input2;
	float m_output1;
	float m_output2;
	int m_clip;
} Map;

typedef struct mapper
{
	Object s_ob;					// required header
	void *s_out[MAX_CHANS];			// outlets
	void *s_proxy[MAX_CHANS - 1];	// inlets
	long s_chans;					// number of chanels, 0 < x <= MAX_CHANS
	long s_inletnumber;				// for proxy
	Map s_map[MAX_CHANS];			// scaling
	float s_input1;					// default input1 for reset
	float s_input2;					// default input2 for reset
	float s_output1;				// default output1 for reset
	float s_output2;				// default output2 for reset
	float s_coeff;					// default coefficient for reset
	long clipmode;
	long listmode;		
} Mapper;

/* globalthat holds the class definition */
void *mapper_class;

// Mapper prototypes
float map_process(Map *m, float data);		// Process the incoming value
void map_setCoeff(Map *m, float exp_coeff);	// Changes the map's exponential coefficient
void map_setInput1(Map *f, float val);
void map_setInput2(Map *f, float val);
void map_setOutput1(Map *f, float val);
void map_setOutput2(Map *f, float val);
void map_setClip(Map *f, int val);
void map_init(Mapper *s, Map *f);

// Mapper Prototypes
Mapper *mapper_new(Symbol *s, short argc, Atom *argv);
void mapper_free(Mapper *s);
void mapper_mfsize(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_param_in1(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_param_in2(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_param_out1(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_param_out2(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_param_clip(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_mapcoeff(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_float(Mapper *s, double f);
void mapper_int(Mapper *s, long l);
void mapper_all(Mapper *s, Symbol *sym, short argc, Atom *argv);
void mapper_reset(Mapper *s, Symbol *sym);
void mapper_resetAll(Mapper *s, Symbol *sym);
void read_sym(Mapper *s, Atom *argv, int i);
void mapper_print(Mapper *s);

float map_process(Map *f, float data)
{
	float result;
	
	if (f->m_coeff != DEF_COEFF) {
		result = (pow((data - f->m_input1) / (f->m_input2 - f->m_input1), f->m_coeff) * (f->m_output2 - f->m_output1) + f->m_output1);
	} else {
		result = (((data - f->m_input1) / (f->m_input2 - f->m_input1)) * (f->m_output2 - f->m_output1) + f->m_output1);
	}
	
	if (!f->m_clip) {
		return result;
	} else if (result >= f->m_output1 && result <= f->m_output2) {
		return result;
	} else if (result > f->m_output2) {
		return f->m_output2;
	} else if (result < f->m_output1) {
		return f->m_output1;
	}
}	

void map_init(Mapper *s, Map *f)
{
	f->m_coeff = s->s_coeff;
	f->m_input1 = s->s_input1;
	f->m_input2 = s->s_input2;
	f->m_output1 = s->s_output1;
	f->m_output2 = s->s_output2;
	f->m_clip = s->clipmode;
}
void map_setCoeff(Map *f, float coeff)
{
	f->m_coeff = coeff;	
}
void map_setInput1(Map *f, float val)
{
	f->m_input1 = val;	
}
void map_setInput2(Map *f, float val)
{
	f->m_input2 = val;	
}
void map_setOutput1(Map *f, float val)
{
	f->m_output1 = val;	
}
void map_setOutput2(Map *f, float val)
{
	f->m_output2 = val;	
}
void map_setClip(Map *f, int val)
{
	if (val == 0) {
		f->m_clip = 0;
	} else {
		f->m_clip = 1;
	} 	
}

void read_sym(Mapper *s, Atom *argv, int i) {
	
	if (argv[i].a_w.w_sym == gensym("clip")) {
		s->clipmode = 1;
	} else if (argv[i].a_w.w_sym == gensym("noclip")) {
		s->clipmode = 0;
	} else if (argv[i].a_w.w_sym == gensym("list")) {
		s->listmode = 1;
	} else if (argv[i].a_w.w_sym == gensym("nolist")) {
		s->listmode = 0;
	} else {
		error("Mapper: can't read text argument. Default selected.");
	}
}

Mapper *mapper_new(Symbol *sym, short argc, Atom *argv)
{
	Mapper *s;
	long chcnt = DEF_CHANS;
	long clip = DEF_CLIP;
	long list = DEF_LIST;
	float mapcoeff = DEF_COEFF;
	float mapmin = DEF_MIN;
	float mapmax = DEF_MAX;
	float inputmin = DEF_MIN;
	float inputmax = DEF_MAX;
	short i, n;

	s = newobject(mapper_class);
	s->clipmode = clip;
	s->listmode = list;

	switch (argc) { // Read arguments
		case 0:
			break;
		case 1: 
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			} else if (argv[0].a_type == A_SYM) {
				read_sym(s, argv, 0);
			}
			break;
		case 2:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			} else if (argv[0].a_type == A_SYM) {
				read_sym(s, argv, 0);
				break;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			} else if (argv[1].a_type == A_SYM) {
				read_sym(s, argv, 1);
			}
			break;
		case 3:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_LONG) {
				chcnt = argv[2].a_w.w_long;
				chcnt = (chcnt >= 1 && chcnt <= MAX_CHANS) 
					? chcnt 
					: DEF_CHANS;
			} else if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_SYM) {
				read_sym(s, argv, 2);
			}
			break;
		case 4:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			} else if (argv[2].a_type == A_SYM) {
				read_sym(s, argv, 2);
				if (argv[3].a_type == A_SYM) {
					read_sym(s, argv, 3);
				}
				break;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			} else if (argv[3].a_type == A_SYM) {
				read_sym(s, argv, 3);
			}
			break;
		case 5:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			} else if (argv[3].a_type == A_SYM) {
				read_sym(s, argv, 3);
				if (argv[4].a_type == A_SYM) {
					read_sym(s, argv, 4);
				}
				break;
			}
			if (argv[4].a_type == A_FLOAT) {
				mapcoeff = argv[4].a_w.w_float;
			} else if (argv[4].a_type == A_LONG) {
				chcnt = argv[4].a_w.w_long;
			} else if (argv[4].a_type == A_SYM) {
				read_sym(s, argv, 4);
			}
			break;
		case 6:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			}
			if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_LONG) {
				mapcoeff = argv[4].a_w.w_float;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_SYM) {
				mapcoeff = argv[4].a_w.w_float;
				read_sym(s, argv, 5);
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_LONG) {
				mapcoeff = (float)argv[4].a_w.w_long;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_FLOAT) {
				chcnt = argv[4].a_w.w_long;
				mapcoeff = argv[5].a_w.w_float;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_SYM) {
				chcnt = argv[4].a_w.w_long;
				read_sym(s, argv, 5);
			} else if (argv[4].a_type == A_SYM && argv[5].a_type == A_SYM) {
				read_sym(s, argv, 4);
				read_sym(s, argv, 5);
			}
			break;
		case 7:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			}			
			if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_LONG) {
				mapcoeff = argv[4].a_w.w_float;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_LONG) {
				mapcoeff = (float)argv[4].a_w.w_long;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_FLOAT) {
				chcnt = argv[4].a_w.w_long;
				mapcoeff = argv[5].a_w.w_float;
			} else if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_SYM) {
				mapcoeff = argv[4].a_w.w_float;
				read_sym(s, argv, 5);
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_SYM) {
				chcnt = argv[4].a_w.w_long;
				read_sym(s, argv, 5);
			} else {
				error("Mapper: argument error.");
			}
			if (argv[6].a_type == A_SYM) {
				read_sym(s, argv, 6);
			} else {
				error("Mapper: argument error.");
			}
			break;
		case 8:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			}			
			if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_LONG) {
				mapcoeff = argv[4].a_w.w_float;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_LONG) {
				mapcoeff = (float)argv[4].a_w.w_long;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_FLOAT) {
				chcnt = argv[4].a_w.w_long;
				mapcoeff = argv[5].a_w.w_float;
			} else {
				error("Mapper: argument error.");
			}
			if (argv[6].a_type == A_SYM) {
				read_sym(s, argv, 6);
			} else {
				error("Mapper: argument error.");
			}
			if (argv[7].a_type == A_SYM) {
				read_sym(s, argv, 7);
			} else {
				error("Mapper: argument error.");
			}
			break;
		default:
			if (argv[0].a_type == A_FLOAT) {
				inputmin = argv[0].a_w.w_float;
			} else if (argv[0].a_type == A_LONG) {
				inputmin = (float) argv[0].a_w.w_long;
			}
			if (argv[1].a_type == A_FLOAT) {
				inputmax = argv[1].a_w.w_float;
			} else if (argv[1].a_type == A_LONG) {
				inputmax = (float) argv[1].a_w.w_long;
			}
			if (argv[2].a_type == A_FLOAT) {
				mapmin = argv[2].a_w.w_float;
			} else if (argv[2].a_type == A_LONG) {
				mapmin = (float) argv[2].a_w.w_long;
			}
			if (argv[3].a_type == A_FLOAT) {
				mapmax = argv[3].a_w.w_float;
			} else if (argv[3].a_type == A_LONG) {
				mapmax = (float) argv[3].a_w.w_long;
			}			
			if (argv[4].a_type == A_FLOAT && argv[5].a_type == A_LONG) {
				mapcoeff = argv[4].a_w.w_float;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_LONG) {
				mapcoeff = (float)argv[4].a_w.w_long;
				chcnt = argv[5].a_w.w_long;
			} else if (argv[4].a_type == A_LONG && argv[5].a_type == A_FLOAT) {
				chcnt = argv[4].a_w.w_long;
				mapcoeff = argv[5].a_w.w_float;
			} else {
				error("Mapper: argument error.");
			}
			if (argv[6].a_type == A_SYM) {
				read_sym(s, argv, 6);
			} else {
				error("Mapper: argument error.");
			}
			if (argv[7].a_type == A_SYM) {
				read_sym(s, argv, 7);
			} else {
				error("Mapper: argument error.");
			}
			error("Mapper: too many arguments.");			
	}

	s->s_chans = chcnt;
	s->s_inletnumber = 0;
	s->s_input1 = inputmin;
	s->s_input2 = inputmax;
	s->s_output1 = mapmin;
	s->s_output2 = mapmax;
	s->s_coeff = mapcoeff;
		
	for(i=chcnt-1; i>=0; i--) {
		if (i>0 && !s->listmode)
			s->s_proxy[i-1] = proxy_new(s,(long)i,&s->s_inletnumber);
		if (i==0 || !s->listmode)
			s->s_out[i] = outlet_new(s,0L);
		map_init(s, &s->s_map[i]);
	}

/* For debugging */	
	post("--- Mapper ---");	
	post("	input 1  = %.2f", inputmin);
	post("	input 2  = %.2f", inputmax);
	post("	output 1 = %.2f", mapmin);
	post("	output 2 = %.2f", mapmax);
	post("	exponential coefficient = %.2f", mapcoeff);
	post("	number of channels = %d", chcnt);
	if (s->listmode) {
		post("	list mode = list");
	} else {
		post("	list mode = nolist");
	}	
	if (s->clipmode) {
		post("	clip mode = clip");
	} else {
		post("	clip mode = noclip");
	}	
	post("");
	
	return s;

}

void mapper_free(Mapper *s)
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

void mapper_param_in1(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("in1all"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				f = argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setInput1(&s->s_map[i], f);
		}
		return;
	} else if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setInput1(&s->s_map[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setInput1(&s->s_map[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setInput1(&s->s_map[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setInput1(&s->s_map[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}
}

void mapper_param_in2(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("in2all"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				f = argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setInput2(&s->s_map[i], f);
		}
		return;
	} else if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setInput2(&s->s_map[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setInput2(&s->s_map[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setInput2(&s->s_map[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setInput2(&s->s_map[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}
}

void mapper_param_out1(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("out1all"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				f = argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setOutput1(&s->s_map[i], f);
		}
		return;
	} else if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setOutput1(&s->s_map[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setOutput1(&s->s_map[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setOutput1(&s->s_map[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setOutput1(&s->s_map[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}
}

void mapper_param_out2(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("out2all"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				f = argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setOutput2(&s->s_map[i], f);
		}
		return;
	} else if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setOutput2(&s->s_map[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setOutput2(&s->s_map[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setOutput2(&s->s_map[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setOutput2(&s->s_map[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}	
}

void mapper_param_clip(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i, j;

	if (sym == gensym("clipall"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				j = argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				j = (long)argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setClip(&s->s_map[i], j);
		}
		return;
	} else if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setClip(&s->s_map[i], argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setClip(&s->s_map[i], (int)argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setClip(&s->s_map[s->s_inletnumber], argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setClip(&s->s_map[s->s_inletnumber], (int)argv[0].a_w.w_float);
		}
	}
	return;
}

void mapper_mapcoeff(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;

	if (sym == gensym("coeffall"))
	{
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG) 
				f = (float)argv[0].a_w.w_long;
			else if (argv[0].a_type == A_FLOAT) 
				f = argv[0].a_w.w_float;

			for (i=0; i<s->s_chans; i++)
				map_setCoeff(&s->s_map[i], f);
		}
		return;
	}
	
	if (s->listmode != 0)
	{
		for(i=0; i<argc && i<s->s_chans; i++)
		{
			if (argv[i].a_type == A_LONG)
				map_setCoeff(&s->s_map[i], (float)argv[i].a_w.w_long);
			else if (argv[i].a_type == A_FLOAT)
				map_setCoeff(&s->s_map[i], argv[i].a_w.w_float);
		}
	}
	else
	{	
		if (argc >= 1)
		{
			if (argv[0].a_type == A_LONG)
				map_setCoeff(&s->s_map[s->s_inletnumber], (float)argv[0].a_w.w_long);
			else if (argv[0].a_type == A_FLOAT)
				map_setCoeff(&s->s_map[s->s_inletnumber], argv[0].a_w.w_float);
		}
	}
    
}

void mapper_float(Mapper *s, double f)
{
	short i;
	i = s->s_inletnumber;
		
	outlet_float(s->s_out[i], map_process(&s->s_map[i], f));
}

void mapper_int(Mapper *s, long l)
{
	mapper_float(s, (float)l);
}


void mapper_all(Mapper *s, Symbol *sym, short argc, Atom *argv)
{
	short i;
	float f;
	Atom ret[MAX_CHANS];

	for(i=0; i<argc && i<s->s_chans; i++)
	{	
		f = 0.0;
		if(argv[i].a_type == A_LONG)
			f = (float)argv[i].a_w.w_long;
		else if (argv[i].a_type == A_FLOAT)
			f = argv[i].a_w.w_float;

		SETFLOAT(ret+i, map_process(&s->s_map[i], f));
	}

	outlet_list(s->s_out[0], 0L, i, ret);
}

void mapper_reset(Mapper *s, Symbol *sym)
{
	short i;
	i = s->s_inletnumber;

	s->s_map[i].m_coeff = s->s_coeff;
	s->s_map[i].m_input1 = s->s_input1;
	s->s_map[i].m_input2 = s->s_input2;
	s->s_map[i].m_output1 = s->s_output1;
	s->s_map[i].m_output2 = s->s_output2;
	s->s_map[i].m_clip = s->clipmode;
}

void mapper_resetAll(Mapper *s, Symbol *sym)
{
	short i;
	
	for(i=0; i<s->s_chans; i++) {
		s->s_map[i].m_coeff = s->s_coeff;
		s->s_map[i].m_input1 = s->s_input1;
		s->s_map[i].m_input2 = s->s_input2;
		s->s_map[i].m_output1 = s->s_output1;
		s->s_map[i].m_output2 = s->s_output2;
		s->s_map[i].m_clip = s->clipmode;
	}
}

void mapper_print(Mapper *s) 
{
	int i;
	
	post("--- Mapper ---");
	post("	Number of channels = %d", s->s_chans);
	if (s->listmode) {
		post("	list mode = list");
	} else {
		post("	list mode = nolist");
	}	
	post("  #: in_1 in_2 out_1 out_2 coeff clip");
	for (i=0; i<s->s_chans; i++) {
		if (s->s_map[i].m_clip == 1)
			post("  %d: %.2f %.2f %.2f %.2f %.2f clip", i+1, s->s_map[i].m_input1, s->s_map[i].m_input2, s->s_map[i].m_output1, s->s_map[i].m_output2, s->s_map[i].m_coeff);
		else
			post("  %d: %.2f %.2f %.2f %.2f %.2f noclip", i+1, s->s_map[i].m_input1, s->s_map[i].m_input2, s->s_map[i].m_output1, s->s_map[i].m_output2, s->s_map[i].m_coeff);
	}
	post("");

}
int main(void)
//void main(fptr *f)
{
	post("Mapper object version " VERSION " by Tristan Jehan");
	post(COPYRIGHT);
	post(" ");
	
	setup(							// Setup Object
		(Messlist**) &mapper_class,	// Pointer to class
		(method) mapper_new,		// Constructor
		(method) mapper_free,		// Destructor
		(short) sizeof(Mapper),		// Class Size
		0L,							// Menu function (not used)
		A_GIMME, 0);

	/* bind your methods to symbols */
	addfloat((method)mapper_float);
	addint((method)mapper_int);

	addmess((method)mapper_reset, "reset", 0);
	addmess((method)mapper_resetAll, "resetall", 0);
	addmess((method)mapper_param_in1, "in1", A_GIMME, 0);
	addmess((method)mapper_param_in1, "in1all", A_GIMME, 0);
	addmess((method)mapper_param_in2, "in2", A_GIMME, 0);
	addmess((method)mapper_param_in2, "in2all", A_GIMME, 0);
	addmess((method)mapper_param_out1, "out1", A_GIMME, 0);
	addmess((method)mapper_param_out1, "out1all", A_GIMME, 0);
	addmess((method)mapper_param_out2, "out2", A_GIMME, 0);
	addmess((method)mapper_param_out2, "out2all", A_GIMME, 0);
	addmess((method)mapper_param_clip, "clip", A_GIMME, 0);
	addmess((method)mapper_param_clip, "clipall", A_GIMME, 0);
	addmess((method)mapper_mapcoeff, "coeff", A_GIMME, 0);
	addmess((method)mapper_mapcoeff, "coeffall", A_GIMME, 0);
	addmess((method)mapper_print, "print", 0);
	addmess((method)mapper_all, "list", A_GIMME, 0);

	/* list object in the new object list */
//	finder_addclass("Analysis","mapper");
return 0;
}