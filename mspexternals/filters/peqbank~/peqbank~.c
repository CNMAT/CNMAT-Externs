/*
Copyright (c) 1999,2000,01,02.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Tristan Jehan, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
     
    
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: peqbank~
DESCRIPTION: Bank of biquad filters in series with analog-like control parameters based on shelving or parametric EQ (or low-level control in the biquad coefficient domain)
AUTHORS: Tristan Jehan, Matt Wright
COPYRIGHT_YEARS: 1999,2000,01,02,03,04,05,06
SVN_REVISION: $LastChangedRevision$
PUBLICATION: ICMC99 paper | http://www.cnmat.berkeley.edu/ICMC99/papers/MSP-filters/filticmc.html
VERSION 1.0: Tristan's initial version 
VERSION 1.1: Minor polishing by Matt Wright, 12/10/99 (version, tellmeeverything)
VERSION 1.2: Major fix of smooth mode disaster, Matt Wright 5/4/2000
VERSION 1.3: Fixed fast mode, 7/11/2000
VERSION 1.5 Expires June 2002, 
VERSION 1.6 never expires
VERSION 1.7 expires March 1, 2003
VERSION 1.8 fixes peqbank_free bug
VERSION 1.9 Added "biquads" message; expires 12/1/3
VERSION 2.0 Never expires
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  


TO-DO:  Include b_nbpeq and b_start in the atomic pointer-swapping scheme
        
        Double-precision coefficients, state variables, intra-cascade signal passing
            (in alternate, slower perform routine)

*/



/* How smooth mode works:

	Conceptually, it's simple.  Whenever coefficients change, we linearly interpolate the 
	coefficients from the old values to the new values over one signal vector.
	
	How do we know when they've changed and keep track of the old and new values?
	It's hairy because the audio interrupt might
	come in the middle of computing the new coefficients.  In the worst case, we get new
	parameters and compute new coefficients, then get new new parameters, and, in the
	middle of computing the new new coefficients, get an audio interrupt.  From this case
	we see that we need three buffers:  the old coefficients (that we used on the last
	vector), the new coefficients (that we just computed), and temporary space to compute
	a new set of coefficients.
	
	We have four pointers to these three arrays:
		coeff - the values we'll use next time
		oldcoeff - the values we used last time
		newcoeff - where we compute new values
		freecoeff - If coeff==oldcoeff, freecoeff points to the third buffer.
		
	The default state, when the coefficients haven't changed recently, is that
	coeff==oldcoeff, and the remaining two pointers point to the remaining two buffers.
	
	After new coefficients are computed we make them take effect by setting
	coeff = newcoeff; newcoeff = freecoeff; freecoeff = 0;
	
	But maybe we already computed new coefficients since the last time the perform routine
	happened.  In that case coeff will be the "old new" coefficients, computed recently
	but not yet actually used by a perform routine.  In that case we simply swap coeff and newcoeff.
	
	The perform routine sees that coeff != oldcoeff and interpolates.  Then it sets
	freecoeff = oldcoeff; oldcoeff = coeff;

*/


#include "ext.h"
#include "version.h"
#include "version.c"
#include "z_dsp.h"
#include <Memory.h>
#include <math.h>

#ifdef WIN_VERSION
#define sinhf sinh
#define sqrtf sqrt
#define tanf tan
#define expf exp
#endif


#undef PI  /* so we can use the one below instead of the one from math.h */
#undef TWOPI /* ditto */

#define RES_ID	7007
#define LOG_10	2.30258509299405f	// ln(10)
#define LOG_2	0.69314718055994f	// ln(2)
#define LOG_22	0.34657359027997f	// ln(2)/2
#define PI		3.14159265358979f	// PI
#define PI2		9.86960440108936f	// PI squared
#define TWOPI	6.28318530717959f	// 2 * PI
#define SMALL   0.000001
#define NBCOEFF 5
#define NBPARAM 5
#define FAST	1
#define SMOOTH  0
#define MAXELEM 10

void *peqbank_class;

typedef struct _peqbank {

	t_pxobject b_obj;

	float *param;		// Ptr on stored parameters
	float *oldparam;	// Ptr on stored old parameters
	
	float *coeff;		// See large comment above for explanation of these four pointers
	float *oldcoeff;
	float *newcoeff;
	float *freecoeff;

#ifdef DEBUG	
	float testcoeff3;  // Matt sanity check
#endif	
	float b_Fs;			// Sample rate

	int b_max;			// Number max of peq filters (used to allocate memory)
	int b_nbpeq;		// Current number of peq filters
	int b_start;		// 0=shelf, 5=no shelf
	int b_version;		// SMOOTH (0) or FAST (1) 

	float *b_ym1;		// Ptr on y minus 1 per biquad
	float *b_ym2;		// Ptr on y minus 2 per biquad
	float *b_xm1;		// Ptr on x minus 1 per biquad
	float *b_xm2;		// Pyr on x minus 2 per biquad
	
	Atom *myList;		// Copy of coefficients as Atoms
	void *b_outlet;		// List of biquad coefficients

} t_peqbank;

t_symbol *ps_maxelem, *ps_shelf, *ps_peq, *ps_fast, *ps_smooth;

t_int *peqbank_perform(t_int *w);
t_int *peqbank_perform_fast(t_int *w);
t_int *do_peqbank_perform_fast(t_int *w, float *mycoeffs);
void peqbank_clear(t_peqbank *x);
void peqbank_dsp(t_peqbank *x, t_signal **sp, short *connect);
void peqbank_reset(t_peqbank *x);
int maxelem(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest);
int shelf(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest);
int peq(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest);
void peqbank_maxelem(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_shelf(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_peq(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_fast(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_smooth(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_list(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_biquads(t_peqbank *x, t_symbol *s, short argc, t_atom *argv);
void peqbank_init(t_peqbank *x);
void peqbank_assist(t_peqbank *x, void *b, long m, long a, char *s);
void *peqbank_new(t_symbol *s, short argc, t_atom *argv);
void peqbank_free(t_peqbank *x);
void peqbank_compute(t_peqbank *x);
void swap_in_new_coeffs(t_peqbank *x);
void compute_parameq(t_peqbank *x, int index); 
void compute_shelf(t_peqbank *x); 
float pow10(float x);
float pow2(float x);
void peqbank_tellmeeverything(t_peqbank *x);

int test_normal_state(t_peqbank *x);
int test_newcoeffs_state(t_peqbank *x);

void main(void) {
        version(0);

	ps_maxelem = gensym("maxelem");
	ps_shelf   = gensym("shelf");
	ps_peq     = gensym("peq");
	ps_fast    = gensym("fast");
	ps_smooth  = gensym("smooth");
	
	setup( (t_messlist **)&peqbank_class, (method)peqbank_new, (method)peqbank_free,
			(short)sizeof(t_peqbank), 0L, A_GIMME, 0);

	{
#ifdef EXPIRE
#define EXPIRATION_STRING "Expires December 1, 2003"
		DateTimeRec date;
		GetTime(&date);
		post(EXPIRATION_STRING);
		if(!((date.year==2002) || (date.year==2003 && date.month < 12)))
		{
			post("Expired");
		}
		else
#else
		post ("Never expires.");
#endif
		addmess((method)peqbank_dsp, "dsp", A_CANT, 0);
	}
	addmess((method)peqbank_reset, "reset", A_GIMME, 0);
	addmess((method)peqbank_list, "list", A_GIMME, 0);
	addmess((method)peqbank_maxelem, "maxelem", A_GIMME, 0);
	addmess((method)peqbank_shelf, "shelf", A_GIMME, 0);
	addmess((method)peqbank_peq, "peq", A_GIMME, 0);
	addmess((method)peqbank_fast, "fast", A_GIMME, 0);
	addmess((method)peqbank_smooth, "smooth", A_GIMME, 0);
	addmess((method)peqbank_clear, "clear", 0);
	addmess((method)peqbank_assist, "assist", A_CANT, 0);
	addmess((method)version, "version", 0);
	addmess((method)peqbank_tellmeeverything, "tellmeeverything", 0);
	addmess((method)peqbank_biquads, "biquads", A_GIMME);
	
	dsp_initclass();

	//rescopy('STR#', RES_ID);
}

void peqbank_tellmeeverything(t_peqbank *x) {
	int i;
	
	version(x);

    if (x->b_version == SMOOTH) {
    	post("  Smooth mode: coefficients linearly interpolated over one MSP vector");
    } else if (x->b_version == FAST) {
    	post("  Fast mode: no interpolation when filter parameters change");
    } else {
    	post("  ERROR: object is in neither FAST mode nor SMOOTH mode!");
    }
    
    post("  coeff = %p, oldcoeff = %p, newcoeff = %p, freecoeff = %p",
    	x->coeff, x->oldcoeff, x->newcoeff, x->freecoeff);
    
    post("  Allocated enough memory for %ld filters", x->b_max);
    if (x->b_start == 0) {
    	post("  Shelving EQ: %.2f dB, %.2f dB, %.2f dB, %.2f Hz, %.2f Hz",
    		 x->param[0], x->param[1], x->param[2], x->param[3], x->param[4]);
    	post("  (%.1f dB below %.0f Hz, %.1f dB between, %.1f dB above %.0f Hz)",
    		 x->param[0], x->param[3], x->param[1], x->param[2], x->param[4]);
    	post("  (biquad: %f %f %f %f %f)", x->coeff[0], x->coeff[1], x->coeff[2], x->coeff[3], x->coeff[4]);
    } else {
    	post("  No shelving EQ.");
    }
    post("  Computing %ld PEQ filters:", x->b_nbpeq);
    for (i = 1; i <= x->b_nbpeq; ++i) {
    	post("   %ld: Ctr %.2fHz, BW %.2f oct, %.1fdB at DC, %.1fdB at ctr, %.1fdB at BW edges (biquad: %f %f %f %f %f)", 
    		 i, x->param[i*5], x->param[i*5+1], x->param[i*5+2], x->param[i*5+3], x->param[i*5+4],
    		 x->coeff[i*5], x->coeff[i*5+1], x->coeff[i*5+2], x->coeff[i*5+3], x->coeff[i*5+4]);
    }
    
   
    if (x->coeff == x->oldcoeff) {
    	if (!test_normal_state(x)) post("Assertions failed in tme, x->coeff == x->oldcoeff");
    } else {
    	if (!test_newcoeffs_state(x)) post("Assertions failed in tme, x->coeff != x->oldcoeff");
    }
}


int posted = 0;

t_int *peqbank_perform(t_int *w) {
	t_float *in  = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_peqbank *x = (t_peqbank *)(w[3]);
	int n        = (int)(w[4]);	
	float *mycoeff;
	
	// In overdrive, it's possible that this perform routine could be interrupted (e.g., 
	// by a MIDI event) by new coefficients being calculated.  So we snapshot a local
	// copy of x->coeff
	// Nowhere else is x->oldcoeff set, so we don't have to worry about that.
	
	mycoeff = x->coeff;

#ifdef DEBUG	
	if (x->oldcoeff[3] != x->testcoeff3) {
		if (posted == 1) {
			post("testcoeff3 is %f, but x->oldcoeff[3] is %f!", x->testcoeff3, x->oldcoeff[3]);
		}
		++ posted;
		posted = posted %100;
	}
#endif

	if (mycoeff == x->oldcoeff) {
		// Coefficients haven't changed, so no need to interpolate
		
#ifdef DEBUG		
		if (!test_normal_state(x)) post("peqbank_perform: mycoeff==x->oldcoeff");
#endif
		
		return do_peqbank_perform_fast(w, mycoeff);
	} else {

		/* Biquad with linear interpolation: smooth-biquad~ */ 		
		int i, j, k=0;
		float rate = 1.0f/n;
		float i0, i1, i2, i3;
		float a0, a1, a2, b1, b2;
		float a0inc, a1inc, a2inc, b1inc, b2inc;
		float y0, y1;
		
		float *tmpparam;	// So we can swap old and new
		float *tmpcoeff;

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_perform: mycoeff!=x->oldcoeff");
#endif

		/* Cascade of Biquads */
		for (j=x->b_start; j<(x->b_nbpeq+1)*NBCOEFF; j+=NBCOEFF) {
			
			i2 = x->b_xm2[k]; 
			i3 = x->b_xm1[k];
			y0 = x->b_ym2[k]; 
			y1 = x->b_ym1[k];	
			
			/* Interpolated values */
			a0 = x->oldcoeff[j  ];
			a1 = x->oldcoeff[j+1];
			a2 = x->oldcoeff[j+2];
			b1 = x->oldcoeff[j+3];	
			b2 = x->oldcoeff[j+4];
					
			/* Incrementation values */
			a0inc = (mycoeff[j  ] - a0) * rate;
			a1inc = (mycoeff[j+1] - a1) * rate;
			a2inc = (mycoeff[j+2] - a2) * rate;
			b1inc = (mycoeff[j+3] - b1) * rate;
			b2inc = (mycoeff[j+4] - b2) * rate;
			
			for (i=0; i<n; i+=4) {

				in[i  ] = y0 = (a0 * (i0 = in[i  ])) + (a1 * i3) + (a2 * i2) - (b1 * y1) - (b2 * y0);
				a1 += a1inc; a2 += a2inc; a0 += a0inc; b1 += b1inc; b2 += b2inc;
		
				in[i+1] = y1 = (a0 * (i1 = in[i+1])) + (a1 * i0) + (a2 * i3) - (b1 * y0) - (b2 * y1);
				a1 += a1inc; a2 += a2inc; a0 += a0inc; b1 += b1inc; b2 += b2inc;

				in[i+2] = y0 = (a0 * (i2 = in[i+2])) + (a1 * i1) + (a2 * i0) - (b1 * y1) - (b2 * y0);
				a1 += a1inc; a2 += a2inc; a0 += a0inc; b1 += b1inc; b2 += b2inc;

				in[i+3] = y1 = (a0 * (i3 = in[i+3])) + (a1 * i2) + (a2 * i1) - (b1 * y0) - (b2 * y1);
				a1 += a1inc; a2 += a2inc; a0 += a0inc; b1 += b1inc; b2 += b2inc;
		
			} // Interpolation loop						

			x->b_xm2[k] = i2;
			x->b_xm1[k] = i3;
			x->b_ym2[k] = y0;
			x->b_ym1[k] = y1;
			k ++;
			
		} // cascade loop
			
		for (i=0; i<n; ++i) out[i] = in[i];
		
		
		// Now that we've made it to the end of the signal vector, the "old" coefficients are
		// the ones we just used.  The previous "old" coeffients, which we've now finished
		// interpolating away from, are not needed any more.
		
		
		if (x->freecoeff != 0) {
			post("е peqbank~ disaster!  freecoeff should be zero now!");
		}
		
#ifdef DEBUG	
		x->testcoeff3 = x->oldcoeff[3];
#endif
		
		x->freecoeff = x->oldcoeff;
		x->oldcoeff = mycoeff;		
	}
	return (w+5);
}

t_int *peqbank_perform_fast(t_int *w) {
	t_peqbank *x = (t_peqbank *)(w[3]);
	t_int *result;

	result = do_peqbank_perform_fast(w, x->coeff);
	
	
	/* We still have to shuffle the coeff pointers around. */
	
	if (x->coeff != x->oldcoeff) {
		if (x->freecoeff != 0) {
			post("е peqbank~ disaster!  freecoeff should be zero now!");
		}
		x->freecoeff = x->oldcoeff;
		x->oldcoeff = x->coeff;		
	}
	
	return result;	
}

t_int *do_peqbank_perform_fast(t_int *w, float *mycoeff) {
	t_float *in  = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_peqbank *x = (t_peqbank *)(w[3]);
	int n        = (int)(w[4]);	

	/* Biquad with linear interpolation: smooth-biquad~ */ 		
	int i, j, k=0;
	float a0, a1, a2, b1, b2;
	float xn, yn, xm2, xm1, ym2, ym1; 

	/* Cascade of Biquads */
	for (j=x->b_start; j<(x->b_nbpeq+1)*NBCOEFF; j+=NBCOEFF) {
		
		xm2 = x->b_xm2[k]; 
		xm1 = x->b_xm1[k];
		ym2 = x->b_ym2[k]; 
		ym1 = x->b_ym1[k];	
		
		a0 = mycoeff[j  ];
		a1 = mycoeff[j+1];
		a2 = mycoeff[j+2];
		b1 = mycoeff[j+3];	
		b2 = mycoeff[j+4];
				
		for (i=0; i<n; i++) {
			xn = in[i];
		    in[i] = yn = (a0 * xn) + (a1 * xm1) + (a2 * xm2) - (b1 * ym1) - (b2 * ym2);

			xm2 = xm1;
			xm1 = xn;
			ym2 = ym1;
			ym1 = yn;
		}
		
		x->b_xm2[k] = xm2;
		x->b_xm1[k] = xm1;
		x->b_ym2[k] = ym2;
		x->b_ym1[k] = ym1;		
		k ++;
		
	} // cascade loop
		
	for (i=0; i<n; ++i) out[i] = in[i];
	
	return (w+5);
}

void peqbank_clear(t_peqbank *x) {

	int i;
	
	for(i=0; i<x->b_max; ++i) {
		x->b_ym1[i] = 0.0f;
		x->b_ym2[i] = 0.0f;
		x->b_xm1[i] = 0.0f;
		x->b_xm2[i] = 0.0f;
	}
}

void peqbank_dsp(t_peqbank *x, t_signal **sp, short *connect) {

	x->b_Fs = sys_getsr();
	peqbank_clear(x);

	if (x->b_version == FAST) 
	   dsp_add(peqbank_perform_fast, 4, sp[0]->s_vec, sp[1]->s_vec, x, sp[0]->s_n);
	else dsp_add(peqbank_perform, 4, sp[0]->s_vec, sp[1]->s_vec, x, sp[0]->s_n);
}

void peqbank_reset(t_peqbank *x) {

	x->b_nbpeq = 0;
	x->b_start = NBCOEFF;
	x->b_max = MAXELEM;
	
	peqbank_free(x);
	peqbank_init(x);
	peqbank_compute(x);	
}

int maxelem(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest) {

	x->b_nbpeq = 0;
	x->b_start = NBCOEFF;
	 		
	if ((rest < argc) && (argv[rest].a_type == A_LONG)) { 
		x->b_max = argv[rest].a_w.w_long;
		if (x->b_max < 2) {
			x->b_max = 2;
		}
		rest ++;
	}
	
	peqbank_free(x);
	peqbank_init(x);
		
	return(rest);		
}

int shelf(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest) {
		
	float Fl, Fh;
	
	if ((rest+4 < argc) && (argv[rest  ].a_type == A_FLOAT) && (argv[rest+1].a_type == A_FLOAT) 
						&& (argv[rest+2].a_type == A_FLOAT) && (argv[rest+3].a_type == A_FLOAT)
						&& (argv[rest+4].a_type == A_FLOAT)) {
						
		x->param[0] = argv[rest  ].a_w.w_float;
		x->param[1] = argv[rest+1].a_w.w_float;
		x->param[2] = argv[rest+2].a_w.w_float;
		x->param[3] = argv[rest+3].a_w.w_float;
		x->param[4] = argv[rest+4].a_w.w_float;
		
		if (x->param[3] == 0) x->param[3] = SMALL;
		if (x->param[4] == 0) x->param[4] = SMALL;

		rest += 5;
		x->b_start = 0;
	} 
	return(rest);
}

int peq(t_peqbank *x, t_symbol *s, short argc, t_atom *argv, int rest) {

	int index=NBPARAM;
	float G0, G, GB;
		
	x->b_nbpeq = 0;
					
	while ((rest < argc) && (x->b_nbpeq+1 < x->b_max)) {
		if ((rest+4 < argc) && (argv[rest  ].a_type == A_FLOAT) && (argv[rest+1].a_type == A_FLOAT) 
							&& (argv[rest+2].a_type == A_FLOAT) && (argv[rest+3].a_type == A_FLOAT) 
							&& (argv[rest+4].a_type == A_FLOAT)) {
			
			x->param[index  ] = argv[rest  ].a_w.w_float;
			x->param[index+1] = argv[rest+1].a_w.w_float;
			G0 = argv[rest+2].a_w.w_float;
			G  = argv[rest+3].a_w.w_float;
			GB = argv[rest+4].a_w.w_float;

			if (x->param[index] == 0) x->param[index] = SMALL;
							
			if (G0 == G) {
				GB = G0;
				G  = G0 + SMALL;
				G0 = G0 - SMALL;
			} else if (!((G0 < GB) && (GB < G)) && !((G0 > GB) && (GB > G))) GB = (G0 + G) * 0.5f; 
						
			x->param[index+2] = G0;
			x->param[index+3] = G;
			x->param[index+4] = GB;
			
			index += NBPARAM;
			rest  += NBPARAM;
			x->b_nbpeq ++;
			
			if (argv[rest].a_type != A_FLOAT) {
				if (argv[rest].a_w.w_sym == ps_peq) rest ++;
				else return(rest);
			} // End of if
		} // End of if
	} // End of while
	return(rest);
}

void peqbank_maxelem(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
	maxelem(x,s,argc,argv,0);
	peqbank_compute(x);
}

void peqbank_shelf(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
	shelf(x,s,argc,argv,0);
	peqbank_compute(x);
}

void peqbank_peq(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
	peq(x,s,argc,argv,0);
	peqbank_compute(x);
}

void peqbank_fast(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
     x->b_version = FAST;
}

void peqbank_smooth(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
     x->b_version = SMOOTH;
}

void peqbank_list(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {

	int i, rest=0;

	x->b_Fs = sys_getsr();
	x->b_nbpeq = 0;
	x->b_start = NBCOEFF;

    /* process list of atoms */
	while ((rest < argc) && (x->b_nbpeq+1 < x->b_max)) {
		if (argv[rest].a_type != A_SYM) return;
		if (argv[rest].a_w.w_sym == ps_maxelem) {
			if (argv[rest+1].a_w.w_long != x->b_max) 
				rest = maxelem(x, s, argc, argv, rest+1);
			else rest += 2;		
		}									
		if (argv[rest].a_w.w_sym == ps_shelf) rest = shelf(x, s, argc, argv, rest+1);
		if (argv[rest].a_w.w_sym == ps_peq) rest = peq(x, s, argc, argv, rest+1);
		if (argv[rest].a_w.w_sym == ps_fast) { 
			peqbank_fast(x, s, argc, argv); 
			rest ++;
		}
		if (argv[rest].a_w.w_sym == ps_smooth) rest ++;
	}	
	peqbank_compute(x);
}

#define ASFLOAT(x) (((x).a_type == A_FLOAT) ? ((x).a_w.w_float) : ((float) (x).a_w.w_long))

void peqbank_biquads(t_peqbank *x, t_symbol *s, short argc, t_atom *argv) {
	int i;
	
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type == A_SYM) {
			error("peqbank~: all arguments to biquads message must be numbers");
			return;
		}
	}
	
	if ((argc % 5) != 0) {
		error("peqbank~: biquads message must have a multiple of 5 arguments");
		return;
	}
	
	if ((argc / 5) > x->b_max) {
		error("peqbank~: Too many biquad coefficients (only memory for %d filters)", x->b_max);
		post("   (ignoring entire biquads list)");
		return;
	}
	
	
	for (i = 0; i < argc; ++i) {
		x->newcoeff[i] = ASFLOAT(argv[i]);
	}


	/* These should happen atomically... */
	x->b_start = 0;
	x->b_nbpeq = (argc/5)-1;    /* The first biquad is the "shelf"; the other n-1 are the "peq"s */
	swap_in_new_coeffs(x);	
}		
		
		

void peqbank_init(t_peqbank *x) {

	int i;

	/* alocate and initialize memory */
	x->param    = (float*) sysmem_newptr( x->b_max * NBPARAM * sizeof(*x->param) );
	x->oldparam = (float*) sysmem_newptr( x->b_max * NBPARAM * sizeof(*x->oldparam) );
	x->coeff    = (float*) sysmem_newptr( x->b_max * NBCOEFF * sizeof(*x->coeff) );
	x->oldcoeff = x->coeff;
    x->newcoeff = (float*) sysmem_newptr( x->b_max * NBCOEFF * sizeof(*x->newcoeff) );
    x->freecoeff = (float*) sysmem_newptr( x->b_max * NBCOEFF * sizeof(*x->freecoeff) );
    x->b_ym1    = (float*) sysmem_newptr( x->b_max * sizeof(*x->b_ym1) );
    x->b_ym2    = (float*) sysmem_newptr( x->b_max * sizeof(*x->b_ym2) );     
    x->b_xm1    = (float*) sysmem_newptr( x->b_max * sizeof(*x->b_xm1) );
    x->b_xm2    = (float*) sysmem_newptr( x->b_max * sizeof(*x->b_xm2) );     
    x->myList   = (Atom*)  sysmem_newptr( x->b_max * NBCOEFF * sizeof(*x->myList) );     


	if (x->param == NIL || x->oldparam == NIL || x->coeff == NIL || x->newcoeff == NIL ||
	    x->freecoeff == NIL || x->b_ym1 == NIL || x->b_ym2 == NIL || x->b_xm1 == NIL || 
	    x->b_xm2 == NIL || x->myList == NIL) {
		post("е peqbank~: warning: not enough memory.  Expect to crash soon.");
	}

	for (i=0; i<x->b_max * NBPARAM; ++i) {
		x->param[i]    = 0.0f;
		x->oldparam[i] = 0.0f;
		x->coeff[i]    = 0.0f;
	}
	
	for (i=0; i<x->b_max; ++i) {
		x->b_ym1[i] = 0.0f;
		x->b_ym2[i] = 0.0f;
		x->b_xm1[i] = 0.0f;
		x->b_xm2[i] = 0.0f;
	}
	
#ifdef DEBUG
	x->testcoeff3 = 0.0;
#endif
}

void peqbank_assist(t_peqbank *x, void *b, long m, long a, char *s) 

/*{

	assist_string(RES_ID, m, a, 1, 2, s);
}*/

{
	if (m == ASSIST_INLET)
		sprintf(s,"(signal)input");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"(signal) output");
			break;
		case 1:
			sprintf(s,"(list)shelf + parametric EQ biquad coefficients [a0 a1 a2 b1 b2]*");
			break;
		}
	}
}

void *peqbank_new(t_symbol *s, short argc, t_atom *argv) {

	int i, rest=0;
	 	
	t_peqbank *x = (t_peqbank *)newobject(peqbank_class);	
	x->b_max = MAXELEM;

	/* get the maximum of filters */
	if (argv[0].a_type == A_LONG) x->b_max = argv[0].a_w.w_long;
	else for (i=0; i<argc; ++i) {	
			if (argv[rest].a_w.w_sym == ps_maxelem) {
				rest ++;
				if ((rest < argc) && (argv[rest].a_type == A_LONG)) { 
					x->b_max = argv[rest].a_w.w_long;
					if (x->b_max < 2) {
						x->b_max = 2;
					}
					break;
				}
			}	
		 }
	peqbank_init(x);
		
    dsp_setup((t_pxobject *)x, 1);			// number of inlets
    x->b_outlet = listout((t_object *)x);	// Create an outlet
	outlet_new((t_object *)x, "signal");	// type of outlet: "signal"
	x->b_Fs = sys_getsr();
	x->b_version = SMOOTH;
	peqbank_clear(x);
    peqbank_list(x, s, argc, argv);
	
    return (x);
}

void  peqbank_free(t_peqbank *x) {
	sysmem_freeptr((char *) x->param);
	sysmem_freeptr((char *) x->oldparam);

	if (x->coeff != x->oldcoeff) sysmem_freeptr((char *) x->oldcoeff);
	sysmem_freeptr((char *) x->coeff);
	sysmem_freeptr((char *) x->newcoeff);
	if (x->freecoeff) sysmem_freeptr((char *) x->freecoeff);

    sysmem_freeptr((char *) x->b_ym1);
    sysmem_freeptr((char *) x->b_ym2);
    sysmem_freeptr((char *) x->b_xm1);
    sysmem_freeptr((char *) x->b_xm2);
    sysmem_freeptr((char *) x->myList);
}

#define WORRIED_ABOUT_PEQBANK_REENTRANCY
#ifdef WORRIED_ABOUT_PEQBANK_REENTRANCY
int already_peqbank_compute = 0;
#endif

void peqbank_compute(t_peqbank *x) {		
	int i;
	
#ifdef WORRIED_ABOUT_PEQBANK_REENTRANCY
	if (already_peqbank_compute) {
		post("е Congratulations; you have discovered a bug in peqbank~!");
		post("е Please email matt@cnmat.berkeley.edu and tell him that");
		post("е peqbank_compute must be reentrant.  Sorry about the click.");
	}
	++already_peqbank_compute;
#endif

	// Do the actual computation of coefficients, into x->newcoeff
	compute_shelf(x);
	for (i=NBPARAM; i<(x->b_nbpeq+1)*NBPARAM; i+=NBPARAM) compute_parameq(x,i);

	swap_in_new_coeffs(x);

#ifdef WORRIED_ABOUT_PEQBANK_REENTRANCY
	--already_peqbank_compute;
#endif
}


void swap_in_new_coeffs(t_peqbank *x) {
	float *prevcoeffs, *prevnew, *prevfree;
	int i;

	// To make the new coefficients take effect we swap around the pointers to the
	// coefficient buffers.  See the large comment at the top of this file.
	// The audio processing interrupt might come at any time.
	
	if (x->coeff == x->oldcoeff) {
		// Normal case: these are the first new coeffients since the last perform routine		

#ifdef DEBUG		
		if (!test_normal_state(x)) post("peqbank_compute: x->coeff == x->oldcoeff");
#endif

		if (x->freecoeff == 0) {
			post("е peqbank: disaster!  freecoeff shouldn't be zero here!");
			   post("  coeff = %p, oldcoeff = %p, newcoeff = %p, freecoeff = %p",
   					x->coeff, x->oldcoeff, x->newcoeff, x->freecoeff);

		}
		prevcoeffs = x->coeff;
		prevnew = x->newcoeff;
		prevfree = x->freecoeff;
		
		x->freecoeff = 0;
		x->coeff = x->newcoeff;	// Now if we're interrupted the new values will be used.
		x->newcoeff = prevfree;

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: just set first new coeffs");
#endif

	} else {
		// We already computed new coeffients since the last perform routine, and now we
		// have even newer ones.  The perform routine may have interrupted this procedure
		// already, but it only would have changed oldcoeffs and freecoeffs.

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: x->coeff != x->oldcoeff");
#endif
		
		prevcoeffs = x->coeff;
		prevnew = x->newcoeff;
		
		x->coeff = x->newcoeff;
		x->newcoeff = prevcoeffs;

#ifdef DEBUG		
		if (!test_newcoeffs_state(x)) post("peqbank_compute: just set new new coeffs");
#endif
	}

	// Output the new coefficients out the outlet
	for (i=0; i<(x->b_nbpeq+1)*NBPARAM; i++) SETFLOAT(x->myList+i, x->coeff[i]);		
	outlet_list(x->b_outlet, 0L, (x->b_nbpeq+1)*NBPARAM, x->myList);
}



void compute_parameq(t_peqbank *x, int index) {
	
	/* Biquad coefficient estimation */
	float G0 = pow10(x->param[index+2] * 0.05f);
	float G  = pow10(x->param[index+3] * 0.05f);
	float GB = pow10(x->param[index+4] * 0.05f);
	
	float w0  = TWOPI * x->param[index] / x->b_Fs;
	float G02 = G0 * G0;
	float GB2 = GB * GB;
	float G2  = G * G;	
	float w02 = w0 * w0;
	
	float val1 = 1.0f / fabs(G2 - GB2);
	float val2 = fabs(G2 - G02);
	float val3 = fabs(GB2 - G02);
	float val4 = (w02 - PI2) * (w02 - PI2);
	
	float mul1 = LOG_22 * x->param[index+1];
	float Dw   = 2.0f * w0 * sinhf(mul1);
	float mul2 = val3 * PI2 * Dw * Dw;
	float num  = G02 * val4 + G2 * mul2 * val1;
	float den  = val4 + mul2 * val1;
	
	float G1  = sqrtf(num / den);
	float G12 = G1 * G1;
	
	float mul3 = G0 * G1;
	float val5 = fabs(G2 - mul3);
	float val6 = fabs(G2 - G12);
	float val7 = fabs(GB2 - mul3);
	float val8 = fabs(GB2 - G12);	
	float val9 = sqrtf((val3 * val6) / (val8 * val2));
	
	float tan0 = tanf(w0 * 0.5f);
	float w1   = w0 * pow2(x->param[index+1] * -0.5f);
	float tan1 = tanf(w1 * 0.5f);
	float tan2 = val9 * tan0 * tan0 / tan1;
		
	float W2 = sqrtf(val6 / val2) * tan0 * tan0;
	float DW = tan2 - tan1;
	
	float C = val8 * DW * DW - 2.0f * W2 * (val7 - sqrtf(val3 * val8));
	float D = 2.0f * W2 * (val5 - sqrtf(val2 * val6));
	float A = sqrtf((C + D) * val1);
	float B = sqrtf((G2 * C + GB2 * D) * val1);
	
	float val10 = 1.0f / (1.0f + W2 + A);
    
   	/* New values */
 	x->newcoeff[index  ] = (G1 + G0 * W2 + B)     * val10; 
	x->newcoeff[index+1] = -2.0f * (G1 - G0 * W2) * val10;
	x->newcoeff[index+2] = (G1 - B + G0 * W2)     * val10;
	x->newcoeff[index+3] = -2.0f * (1.0f - W2)    * val10;
	x->newcoeff[index+4] = (1.0f + W2 - A)        * val10;
}

void compute_shelf(t_peqbank *x) {
	
	/* Biquad coefficient estimation */	
	float G1 = pow10((x->param[0] - x->param[1]) * 0.05f);
	float G2 = pow10((x->param[1] - x->param[2]) * 0.05f);
	float Gh = pow10(x->param[2] * 0.05f);
	
	/* Low shelf */
	float X  = tanf(x->param[3] * PI / x->b_Fs) / sqrtf(G1);
	float L1 = (X - 1.0f) / (X + 1.0f);
	float L2 = (G1 * X - 1.0f) / (G1 * X + 1.0f);
	float L3 = (G1 * X + 1.0f) / (X + 1.0f);
	
	/* High shelf */
	float Y  = tanf(x->param[4] * PI / x->b_Fs) / sqrtf(G2);
	float H1 = (Y - 1.0f) / (Y + 1.0f);
	float H2 = (G2 * Y - 1.0f) / (G2 * Y + 1.0f);
	float H3 = (G2 * Y + 1.0f) / (Y + 1.0f);
	
	float C0 = L3 * H3 * Gh;
  
    /* New values */
 	x->newcoeff[0] = C0; 
	x->newcoeff[1] = C0 * (L2 + H2);
	x->newcoeff[2] = C0 * L2 * H2;
	x->newcoeff[3] = L1 + H1;
	x->newcoeff[4] = L1 * H1;
}

float pow10(float x) {
	return expf(LOG_10 * x);
}

float pow2(float x) {
	return expf(LOG_2 * x);
}


#define YUCKKYIF(test) if (test) {post("ее normal:  " #test); ok = 0;}
int test_normal_state(t_peqbank *x) {
	int ok = 1;
	YUCKKYIF (x->coeff == 0)
	YUCKKYIF (x->oldcoeff == 0)
	YUCKKYIF (x->coeff != x->oldcoeff) 
	YUCKKYIF (x->newcoeff == 0) 
	YUCKKYIF (x->freecoeff == 0) 
	YUCKKYIF (x->coeff == x->newcoeff)
	YUCKKYIF (x->coeff == x->freecoeff)
	YUCKKYIF (x->newcoeff == x->freecoeff)

	return ok;
}

int test_newcoeffs_state(t_peqbank *x) {
	int ok = 1;
	YUCKKYIF (x->oldcoeff == 0)
	YUCKKYIF (x->coeff == 0)
	YUCKKYIF (x->newcoeff == 0) 
	YUCKKYIF (x->freecoeff != 0)
	
	YUCKKYIF (x->coeff == x->oldcoeff)
	YUCKKYIF (x->coeff == x->newcoeff)
	YUCKKYIF (x->newcoeff == x->oldcoeff)

	return ok;
}
