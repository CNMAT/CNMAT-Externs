/*

Copyright (c) 2001-07.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
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

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: amaranth~ 
DESCRIPTION: Idiosyncratic granular synthesis external
AUTHORS: Matt Wright
COPYRIGHT_YEARS: 2001,02,03,04,05,06,07
SVN_REVISION: $LastChangedRevision$
VERSION 0.0: Initial version. (10/18/01)
VERSION 0.1: (020606) multichannel buffers OK, Clears all grains when DSP turn
VERSION 0.1.1: Recompiled for UB, uses proper version system
VERSION 0.1.2: Help patch no longer requires sound files
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  


*/


#include "version.h"
#include "ext.h"
#include "version.c"
#include "z_dsp.h"
#include <math.h>
#include "buffer.h"


void *amaranth_class;


typedef struct _grainstate {
	int busy;			// 0 if this grain is available, 1 if it's currently playing
	int nextFree;		// index (within object's array of grains) of next free grain.  -1 = null
	int location;		// Start point in buffer, in samples
	int duration;		// Grain duration, in samples
	float transpose;	// Transposition ratio
	
	
	t_buffer *buffer;	// Which buffer to read
	
	unsigned long bufIndex;	// Sample index for buffer, plus fractional part
	unsigned long bufStep;  // How much to advance bufIndex each sample
	
	int samplesToGo;	// How many more samples need to be synthesized for this grain?
	
	unsigned long windowPos;	// Current index into window function table, plus fractional part
	unsigned long windowStep;	// How much to advance windowPos for each output sample
} Grainstate;


typedef struct _amaranth
{
	/* Max stuff */
    t_pxobject x_obj;	//  header
    
    void *outlet;
    int numGrains;
    Grainstate *grains;
    int firstFree;		// Head of "free list" of grains
} t_amaranth;    

t_symbol *ps_buffer;
#define DEFAULT_MAX_NUM_GRAINS 20
#define HANNING_WINDOW_TABLE_SIZE 65536		// arbitrary
static float hanningWindowTable[HANNING_WINDOW_TABLE_SIZE];
#define INTERPOLATION_TABLE_SIZE 256		// 8 bits

struct {
	float left, right;
} interpTable[INTERPOLATION_TABLE_SIZE];



void fillHanningWindowTable(void);
void FillInterpTable(void);
t_buffer *SymbolToBuffer(t_symbol *s);
void main(void);
void *amaranth_new(int maxNumGrains);
void amaranth_free(t_amaranth *x);
int AllocGrain(t_amaranth *x);
void FreeGrain(t_amaranth *x, int which);
int FreeAllGrains(t_amaranth *x);
void amaranth_dsp(t_amaranth *x, t_signal **sp, short *count);
t_int *amaranth_perform(t_int *w);
void amaranth_grain(t_amaranth *x, Symbol *s, short argc, Atom *argv);
// void amaranth_grain(t_amaranth *x, float location, float dur, float trans, t_symbol *buf);
void tellmeeverything(t_amaranth *x);


void main(void) {
	version(0);
	
	fillHanningWindowTable();
	FillInterpTable();
	ps_buffer = gensym("buffer~");
	
    setup((t_messlist **)&amaranth_class, (method)amaranth_new, (method)dsp_free, (short)sizeof(t_amaranth),
          0L, A_DEFLONG, 0);
//    addmess((method)amaranth_grain, "grain", A_FLOAT, A_FLOAT, A_FLOAT, A_SYM, 0);
    addmess((method)amaranth_grain, "grain", A_GIMME, 0);
    addmess((method)amaranth_dsp, "dsp", A_CANT, 0);
	addmess((method)version, "version", 0);
    addmess((method)tellmeeverything, "tellmeeverything", 0);
    dsp_initclass();
}

void *amaranth_new(int maxNumGrains) {
	t_amaranth *x;
	int i;
	
	if (maxNumGrains == 0) maxNumGrains = DEFAULT_MAX_NUM_GRAINS;
	
	if (maxNumGrains < 1 || maxNumGrains > 10000) {
		post("¥ amaranth~: requested number of grains (%ld) is unreasonable.", maxNumGrains);
		return 0;
	}
	
	x = (t_amaranth *)newobject(amaranth_class);
	
	if (x==0) return 0;
	
	x->numGrains = maxNumGrains;
	x->grains = (Grainstate *) NewPtr(maxNumGrains * sizeof(Grainstate));
	if (x->grains == 0) {
		post("¥ amaranth~: out of memory to allocate grain voices.");
		return 0;
	}
	
	for (i = 0; i < maxNumGrains; ++i) {
		x->grains[i].busy = 0;
		x->grains[i].nextFree = i+1;
	}
	
	x->grains[maxNumGrains-1].nextFree = -1;
	x->firstFree = 0;
	
  	dsp_setup((t_pxobject *)x,0);  // no signal inlets
    x->outlet = outlet_new((t_object *)x, 0L);
	outlet_new((t_object *)x, "signal");
    return x;
    
}

void amaranth_free(t_amaranth *x) {
	DisposePtr((char *) x->grains);

	dsp_free(&(x->x_obj));
}


int AllocGrain(t_amaranth *x) {
#ifdef LINEAR_SEARCH	
	int i;
	for (i = 0; i < x->numGrains; ++i) {
		if (!(x->grains[i].busy)) {
			x->grains[i].busy = 1;
			return i;
		}
	}
	
	return -1;
#else

	int result = x->firstFree;
	
	if (result == -1) return -1;
	x->grains[result].busy = 1;
	x->firstFree = x->grains[result].nextFree;
	return result;
#endif
}

void FreeGrain(t_amaranth *x, int which) {
	x->grains[which].busy = 0;

	x->grains[which].nextFree = x->firstFree;
	x->firstFree = which;
}

int FreeAllGrains(t_amaranth *x) {
	int i;
	int result = 0;
	
	for (i = 0; i < x->numGrains; ++i) {
		if (x->grains[i].busy) {
			FreeGrain(x, i);
			++result;
		}
	}
	return result;
}


 // void amaranth_grain(t_amaranth *x, float location, float dur, float trans, Symbol *bufName) {
// That probably would have worked if I had made them doubles instead of floats...
 
void amaranth_grain(t_amaranth *x, Symbol *s, short argc, Atom *argv) {
	int g, samplesNeeded;
	float location,  dur,  trans;
	Symbol *bufName;

	if (argc != 4) {
		post("¥ amaranth~: grain needs exactly 4 arguments (location, duration, transposition, buffer)");
		return;
	}
	
	if (argv[0].a_type == A_FLOAT) location = argv[0].a_w.w_float;
	else if (argv[0].a_type == A_LONG) location = (float) argv[0].a_w.w_long;
	else {
		post("¥ amaranth~: grain location must be a number");
		return;
	}
	
	if (argv[1].a_type == A_FLOAT) dur = argv[1].a_w.w_float;
	else if (argv[1].a_type == A_LONG) dur = (float) argv[1].a_w.w_long;
	else {
		post("¥ amaranth~: grain duration must be a number");
		return;
	}
	
	if (argv[2].a_type == A_FLOAT) trans = argv[2].a_w.w_float;
	else if (argv[2].a_type == A_LONG) trans = (float) argv[2].a_w.w_long;
	else {
		post("¥ amaranth~: grain transposition ratio must be a number");
		return;
	}
	
	if (argv[3].a_type == A_SYM) bufName = argv[3].a_w.w_sym;
	else {
		post("¥ amaranth~: grain buffer name must be a symbol");
		return;
	}
	
		
	g = AllocGrain(x);
	if (g == -1) {
		outlet_bang(x->outlet);
		return;
	}
	
	// post("amaranth_grain(%p, %f, %f, %f, %s)", x, location, dur, trans, bufName->s_name);

	x->grains[g].buffer = SymbolToBuffer(bufName);
	
	if (x->grains[g].buffer == 0) {
		error("amaranth~: no buffer~ %s", bufName->s_name);
		FreeGrain(x, g);
		return;
	}


	x->grains[g].location = ((int) location * x->grains[g].buffer->b_sr * 0.001);
	x->grains[g].bufIndex = x->grains[g].location << 8;
	
	x->grains[g].bufStep = (int) (256.0f * trans);
	
	x->grains[g].duration = x->grains[g].samplesToGo = dur * sys_getsr() * 0.001;
	x->grains[g].transpose = trans;
	x->grains[g].windowPos = 0;
	x->grains[g].windowStep = (HANNING_WINDOW_TABLE_SIZE << 8) / x->grains[g].duration;
	
	samplesNeeded = x->grains[g].location + x->grains[g].duration / trans;
	
	if (samplesNeeded > x->grains[g].buffer->b_frames) {
		/* Buffer is too small; drop this grain */
		/* xxx make this complaint only happen once...*/
		/* post("¥ amaranth~: %ld-sample buffer %s is too small for loc %f, duration %f, trans %f",
			 x->grains[g].buffer->b_frames, bufName->s_name, x->grains[g].location, 
			 x->grains[g].duration, trans); */
		FreeGrain(x, g);
		return;
	}

	
	/* post("*** grain %ld, loc %ld, dur %ld, trans %f, buf %s", g, x->grains[g].location, x->grains[g].duration,
		 x->grains[g].transpose, x->grains[g].buffer->b_name->s_name);
     */
}

void amaranth_dsp(t_amaranth *x, t_signal **sp, short *count) {
	int numGrainsFreed;
	
	// post("*** 	amaranth_dsp called");
	
	numGrainsFreed = FreeAllGrains(x);
	if (numGrainsFreed > 0) {
		post("amaranth~: DSP just started; cancelling %ld grains");
	}
	
	dsp_add(amaranth_perform, 3, x, sp[0]->s_vec, sp[0]->s_n);  // out, size
}

int bogus = 0;

t_int *amaranth_perform(t_int *w) {
	t_float val;
    t_amaranth *x = (t_amaranth *)(w[1]);  // input
    t_float *out = (t_float *)(w[2]); // output
    int size = w[3]; // vector size
	int g, i, j;
	
	val = 0;
	
#if 0
	if (bogus % 1000 == 0) {
		post("*** amaranth_perform %d: x %p, out %p, size %d",
			 bogus, x, out, size);
	}
	++bogus;
#endif
	
	for (j = 0; j < size; ++j) {
		out[j] = 0.0;
	}
	
	for (g = 0; g < x->numGrains; ++g) {
		if (x->grains[g].busy) {
			if (! (x->grains[g].buffer->b_valid))  {
				post("¥ amaranth~: buffer %s invalid", x->grains[g].buffer->b_name->s_name);
				/* Forget this grain; its buffer disappeared */
				FreeGrain(x, g);
			} else {
				register int index, interpIndex;
			    register float interpolatedSample;

				float *samples = x->grains[g].buffer->b_samples;
#ifdef ASSUME_ALL_BUFFERS_ARE_MONO
#define nchans 1
#else
				int nchans = x->grains[g].buffer->b_nchans;
#endif
				int howMany;
				
				if (x->grains[g].samplesToGo > size) {
					howMany = size;
				} else {
					howMany = x->grains[g].samplesToGo;
				}
				
				
				for (j = 0; j < howMany; ++j) {
					index = x->grains[g].bufIndex >> 8;
					interpIndex = x->grains[g].bufIndex & 0xff;
					interpolatedSample = samples[index*nchans] * interpTable[interpIndex].left +
										 samples[(index+1)*nchans] * interpTable[interpIndex].right;
					
					out[j] += interpolatedSample * hanningWindowTable[x->grains[g].windowPos>>8];
					
					x->grains[g].bufIndex += x->grains[g].bufStep;
					x->grains[g].windowPos += x->grains[g].windowStep;
				}

				x->grains[g].samplesToGo -= size;
				if (x->grains[g].samplesToGo <= 0) {
					/* This grain is done. */
					FreeGrain(x, g);
				}
			}
		}
	}
    return (w+4);
}


void fillHanningWindowTable(void) {
	int i;
	int oneQuarter = HANNING_WINDOW_TABLE_SIZE / 4;
	int threeQuarters = 3 * oneQuarter;
	float pi = 3.1415926535897932f;
	float oneOverSize = 1.0f / HANNING_WINDOW_TABLE_SIZE;
	float x;
	
	for (i = 0; i < oneQuarter; ++i) {
		x = ((float) i) * oneOverSize;
		hanningWindowTable[i] = 0.5 * (1 + cos((pi + pi * 4.0f * x)));
	}

	for (i = oneQuarter; i < threeQuarters; ++i) {
		hanningWindowTable[i] = 1.0f;
	}

	for (i = threeQuarters; i < HANNING_WINDOW_TABLE_SIZE; ++i) {
		x = ((float) i) * oneOverSize;	
		hanningWindowTable[i] = 0.5 * (1 + cos((pi + pi * 4.0f * x)));
	}
}


void FillInterpTable(void) {
	int i;
	float oneOverTableSize = 1.0f / (float) INTERPOLATION_TABLE_SIZE;
	
	for (i = 0; i < INTERPOLATION_TABLE_SIZE; ++i) {
		interpTable[i].right = i * oneOverTableSize;
		interpTable[i].left = 1.0f - interpTable[i].right;
	}
}
	


t_buffer *SymbolToBuffer(t_symbol *s) {
	t_buffer *b;
	
	if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer) {
		return b;
	} else {
		return 0;
	}
}

void tellmeeverything(t_amaranth *x) {
	int i, active;
	
	post("Amaranth object: %ld grain capacity", x->numGrains);
	
	active = 0;
	for (i = 0; i < x->numGrains; ++i) {
		if (x->grains[i].busy) {
			post("  grain[%d] loc %d, dur %d, trans %f, buffer %s", i, x->grains[i].location,
				 x->grains[i].duration , x->grains[i].transpose, 
				 x->grains[i].buffer->b_name->s_name);
				 
				 
			post("            bufIndex %d, bufStep %d, toGo %d, winPos %d, winStep %d",
				 x->grains[i].bufIndex, x->grains[i].bufStep, x->grains[i].samplesToGo,
				 x->grains[i].windowPos, x->grains[i].windowStep);
			++active;
		}
	}
	post("  %ld active grains", active);
}				 
				 
				 
				 
				 
				 
				 
				 
				 
