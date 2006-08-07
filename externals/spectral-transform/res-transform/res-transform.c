/*
Copyright (c) 1999.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Adrian Freed, The Center for New Music and Audio Technologies,
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
NAME: res-transform 
AUTHORS: Adrian Freed, Matt Wright, and Michael Zbyszynski
DESCRIPTION: Set of basic transformations for resonance models
COPYRIGHT_YEARS: 1996,97,98,99,2000,2001,2002,2003,2004,2005,2006
SVN_REVISION: $LastChangedRevision: 406$
VERSION 1.2: by Matt Wright: compiles with CW 7 and new Max/MSP SDK
VERSION 1.3: by Matt Wright has "setoneamplitude", "setonefrequency", "setonerate", and "setone"
VERSION 1.4: by Matt Wright allows float midi-pitch
VERSION 1.5: debugs setone and adds the -1 index feature, adds numresonances 
VERSION 1.5alpha: for windows compiled by Michael Zbyszynski
VERSION 1.6: cleaned up so it works for Mac too by Matt Wright
VERSION 1.6.1: Added "more_resonances" method
VERSION 1.6.2: Added amprange and freqrange messages
VERSION 1.7: aliased messages without "-" for compatibility with Javascript, support for sinusoidal models,drop-partials byo from the CAST era , exponential decay maker for percussion effect or clean decay,all calculations in double precision
VERSION 1.72: AF changes setone to setonesinusoid  and adds setoneresonance, removes numresonances changes matt's amprange and freqrange semantics
VERSION 1.73: Updated tellmeeverything to disclose info about new features
VERSION 1.74: fixed amplitude comparison so that non-zero gains were output, changed to larger model size (1024), added alias for sin-transform, didn't test or even compile anything
VERSION 1.75: replaced  getbytes by NewPtr and tested
VERSION 1.76: I can't get the alias feature to work
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


Copyright © 1986,1987 Adrian Freed
Copyright © 1996,1997,1998,1999,2005 Regents of the University of California.


Future:
	"Importance based culling" from resonance.c


	
*/

#include "version.h"


#include <math.h>
#include "ext.h"



fptr *FNS;
void *resonclass;




#define PI (3.14159265358979323)
#define MAXRESON 1024
#define NSPECTRALENVELOPE 32

#define A440 440.0
#define C261 261.6255661
#define SEMITONE 1.0594630943593
#define OCTAVESIZE 12
#define OCTAVEBASE 3
/* ln(SEMITONE) */
#define SEMIFAC 0.057762265046662
#define AASMIDINOTE 69
#define CASMIDINOTE 60

typedef	struct	fobj
{
	Object			object;		/* An embedded MAX object.					*/
	void *m_proxy;
	long m_inletNumber;
	void			*dataoutlet;	/* frequency, gain bandwidth tuplets -first outlet		*/
	int maxresonances;
	Atom 			*model;
//	Atom			model[3*MAXRESON];
/* current model 	*/
	struct			reson { double f,g,b; }	*resonances;	
	int			nreson; /* number of resonances */
	int			nreson_output; /* number of resulting resonances after clussters add them and 
								  frequency-range and gain-range take them away */
	int			fcount; /* count of incoming parameters stored */
	
	// modifiers
	float			freqscale;	/* pitch scale */
	float			freqadd; /* Hz - cheap inharmonicity */ 
	float			stringstretchfactor; /* xxx */
	float			bwscale; /* bandwidth scale */
	float			gainscale; /*  */
//	float			srate; /* sample rate */
	float			freqbase; /* pitch reference for these resonances */
	float			slope;
	float			center;
	float			squelch;
	
	// clusters
	int 	clustersize;
	float	fspread; /* in Hz */
	float	faround; /* in Hz , symmetric around base frequency */
	float	fstretch;/* factor, from base frequency to fstretch*basefreqeuncy*/
	double bwspread; /* in Hz */
	double bwstretch; /* scale factor */
	double attenuationspread; /* in dB */
	


	// drop by frequency
	double fmin; // in Hz
	double fmax; // in Hz

	// Allowable parameter ranges MATT drops partials I set gain to 0 need to resolve this
	float mingain, maxgain;


	
	// drop by partial index numbered from 1
	int partialmin;
	int partialmax;
	
	// variable decay rate (for percussifier clean note off and other effects 
	double k1;
	double k2;
	double fpivot;
	double time;
	
	// odd/even
	double oddgain;
	double evengain;
	
	double oddfscale;
	double evenfscale;
	
	int	sinusoidalmodel; /* no decayrate, i.e. sinusoidal model vs resonance model */
}fobj;


/* prototypes */
static double miditopitchratio(double f);
static double modeltune(double f,double p);
static void faxis(double f[],long n);
void * fnew(Symbol *s, int argc, Atom *argv);
static double senv(fobj *x, double f);
static void computeeverything(fobj *x);
static void dumpresonances(fobj *x);
//static void *setsrate(fobj *it, double ef);
static void *setcenter(fobj *it, double f);
static void *setslope(fobj *it, double f);
static void *setnfilters(fobj *it, long n);
static void *setfreqscale(fobj *it, double f);
static void *setfreqbase(fobj *it, double f);
static void *setfreqadd(fobj *it, double f);
static void *setatten(fobj *it, double f);
static void *setgain(fobj *it, double f);
static void *setrmask(fobj *it, double f);
static void *setmidipitch(fobj *it, double f);
static void *setbwscale(fobj *it, double f);
static void *specenv(fobj *x, struct symbol *s, int argc, Atom *argv);
static void setfreqrange(fobj *x, double min, double max);
static void setamprange(fobj *x, double min, double max);
static void flist(fobj *x, struct symbol *s, int argc, Atom *argv);
static void fulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
void more_resonances(fobj *x, struct symbol *s, int argc, struct atom *argv);
static void resondump(fobj *x, struct symbol *s, int argc, Atom *argv);
static void enddump(fobj *x, struct symbol *s, int argc, struct atom *argv);
static void version(fobj *x);
static void tellmeeverything(fobj *x);
void numresonances(fobj *x);


static double miditopitchratio(double f) {
	return  exp( SEMIFAC*(f - AASMIDINOTE));
}

static double modeltune(double f, double p) {
	if(p==0.0)
		return 1.0;
		
	return miditopitchratio(f)*(A440/p);
}

static void clearit(fobj *x);
static void clearit(fobj *x)
{
	x->freqscale = 1.0;
	x->freqadd = 0.0;
	x->bwscale = 1.0;
	x->center = 0.0;
	x->slope = 0.0;
	x->gainscale = 1.0;
	x->squelch = -1.0;
	x->freqbase = 0.0;


	x->clustersize = 1;
	x->fspread = 0.0;
	x->fstretch = 1.0;
	x->bwstretch = 1.0;
	x->bwspread = 0.0;
	x->attenuationspread = 0.0;
	x->faround = 0.0;
	
	x->fmin = 0.0;
	x->fmax = 1000000.0;
	x->partialmax = 1000000;
	x->partialmin = 0;
	
	x->k1 = x->k2 = 0.f;
	x->fpivot = 1000.0;
	
	x->time = 0.0;
	
	x->oddgain = x->evengain = x->evenfscale = x->oddfscale = 1.0;
}

static void storemodel(fobj *x, struct symbol *s, int argc, struct atom *argv, Boolean old, Boolean formant, Boolean addmore);
static void storemodel(fobj *x, struct symbol *s, int argc, struct atom *argv, Boolean old, Boolean formant, Boolean addmore)
{
if(!x->sinusoidalmodel)
{
	// first argument is frequency base
	if (((argc % 3) == 1) && (argv[0].a_type==A_FLOAT) &&(argv[0].a_w.w_float>0.0f) &&
			(argv[0].a_w.w_float<100000.0f)) {
		x->freqbase = argv[0].a_w.w_float;
		--argc; ++argv;
	}
}
if(x->sinusoidalmodel && ((argc%2)!=0))
	{	
		post("duples required for sinusoidal models %d",argc);
		return;
	}
else if(!x->sinusoidalmodel && ((argc%3)!=0))
	{	
		post("triples required for resonance models");
		return;
	}
#ifdef SILLYATTEMPTTOGUESSKINDOFMODEL
	x->sinusoidalmodel = 0;
	if((argc % 2)==0)
	{
		if((argc % 3)==0)
		{
			int i,n=0;
			double av=0.0;
			for(i=0;i<argc;++i)
			{
		 		if(argv[i].a_type==A_FLOAT && ((i%2)==0) && ((i%3)>0))
		 		{
		 			++n; av += argv[i].a_w.w_float;
		 		}
		 	}
		 	if(n>0 && ((av/n)>30.0))
		 		x->sinusoidalmodel = 1;
		} 	
		else
			x->sinusoidalmodel = 1;
	}
	else
	if ((argc % 3) !=0)
	{	
		post("reson: fulllist: bad list");
		return;
	}
#endif	

	
	if (addmore) {
		x->fcount = x->nreson;
	} else {
		x->fcount = 0;
	}
	
	while(argc)
	{
		int minc = (x->sinusoidalmodel==1)?2:3;
		while(argc)
		{
				if(argv[0].a_type==A_FLOAT && 
				 	argv[1].a_type==A_FLOAT && 
				 	 x->fcount<x->maxresonances)
			 	{
						int n = x->fcount;
						x->resonances[n].f = argv[0].a_w.w_float;
						x->resonances[n].g = old?exp(argv[1].a_w.w_float*0.1151292546497):argv[1].a_w.w_float;
						if(!x->sinusoidalmodel)
						{
							if(argv[2].a_type==A_FLOAT )
								x->resonances[n].b = old?(argv[2].a_w.w_float*3.1415026535):argv[2].a_w.w_float;
							if(formant)
								x->resonances[n].g *= x->resonances[n].f / (1.0f-exp(x->resonances[n].b/44100.0))/ 250.0;
						}
						++(x->fcount);

	//post("%d %d %lf",argc, n, x->resonances[n].f);

		 		}
		 		argv+=minc ; argc -= minc;
		}
	}
	x->nreson = x->fcount;
	x->fcount = 0;
}


static void computeeverything(fobj *x)
{
	double fsc = x->freqscale, bwsc=x->bwscale,
	 gainscale = x->gainscale,fadd= x->freqadd,srbar;
	 int i,j,k;
	int csize = x->clustersize;
	Boolean squelch = ( x->squelch>=0.0);
	 double r;	
		int minc = (x->sinusoidalmodel==1)?2:3;
	
	j = 0; /* Output resonance index */
	for(i=0;i<x->nreson;++i)
	{
		struct reson transformedresonances;
		int odd = ((i%2)==0); // yea yea, I know: the first partial has the index 0
				
		if(!x->sinusoidalmodel)
			transformedresonances.b = squelch?x->squelch:(x->resonances[i].b*bwsc);	
		
	//	a = expf(ap*0.1151292546497)*(x->slope*(f-x->center)+1.0f) /* *senv(x,f) */;
		transformedresonances.f = (x->resonances[i].f*fsc*(odd?x->oddfscale:x->evenfscale)+fadd);
		transformedresonances.g = x->resonances[i].g*gainscale*(odd?x->oddgain:x->evengain);



		if(transformedresonances.f>x->center)
			transformedresonances.g *= exp(0.1151292546497*(x->slope*(x->resonances[i].f-x->center)));

		if(x->time!=0.0)
			transformedresonances.g *= exp(x->time*-x->k1*(1.0+ 20.0*x->k2*(transformedresonances.f-x->fpivot)/50000.0));
		
		if(i>x->partialmax-1 || i<x->partialmin
		 ||transformedresonances.g < x->mingain && transformedresonances.g > x->maxgain
			|| transformedresonances.f<x->fmin || transformedresonances.f>x->fmax)
			transformedresonances.g = 0.0;
		
		for(k=0;k<csize;++k)
		{
			int halfway = csize/2;
			double f = transformedresonances.f+(k)*(x->fspread/csize)
				+(k-halfway)*(x->faround/csize);
			double g = transformedresonances.g*expf(-0.1151292546497*k*(x->attenuationspread/csize));

			x->model[(csize*i+k)*minc+0].a_w.w_float = f;
			x->model[(csize*i+k)*minc+1].a_w.w_float = g;
			if(!x->sinusoidalmodel)
				x->model[(csize*i+k)*minc+2].a_w.w_float = transformedresonances.b+k*(x->bwspread/csize);
		}
	}
	
	x->nreson_output = j;

}

static void dumpresonances(fobj *x)
{
		int minc = (x->sinusoidalmodel)?2:3;
	computeeverything(x);
 	outlet_list(x->dataoutlet,0L,(short)x->nreson*x->clustersize*minc,x->model);
}
void dumpifnecessary(fobj *x);
void dumpifnecessary(fobj *x)
{
	if(x->m_inletNumber==0)
		dumpresonances(x);
}
static void clear(fobj *it)
{
	clearit(it);
		dumpifnecessary(it);
}

static void *setcenter(fobj *it, double f)
{
	it->center = f;
		dumpifnecessary(it);
}

static void *setslope(fobj *it, double f);
static void *setslope(fobj *it, double f)
{		
	it->slope = f;
		dumpifnecessary(it);
}



static void *setfreqscale(fobj *it, double f);
static void *setfreqscale(fobj *it, double f)
{
	it->freqscale = f;
		dumpifnecessary(it);
}

static void *setoddfreqscale(fobj *it, double f);
static void *setoddfreqscale(fobj *it, double f)
{
	it->oddfscale = f;
		dumpifnecessary(it);
}

static void *setevenfreqscale(fobj *it, double f);
static void *setevenfreqscale(fobj *it, double f)
{
	it->evenfscale = f;
		dumpifnecessary(it);
}

static void *setk1(fobj *it, double f);
static void *setk1(fobj *it, double f)
{
	it->k1 = f;
		dumpifnecessary(it);
}
static void *setk2(fobj *it, double f);
static void *setk2(fobj *it, double f)
{
	it->k2 = f;
		dumpifnecessary(it);
}
static void *setfpivot(fobj *it, double f);
static void *setfpivot(fobj *it, double f)
{
	it->fpivot = f;
		dumpifnecessary(it);
}
static void *setfmax(fobj *it, double f);
static void *setfmax(fobj *it, double f)
{
	it->fmax = f;
		dumpifnecessary(it);
}
static void *setfmin(fobj *it, double f);
static void *setfmin(fobj *it, double f)
{
	it->fmin = f;
		dumpifnecessary(it);
}
static void *settime(fobj *it, double f);
static void *settime(fobj *it, double f)
{
	it->time = f;
		dumpifnecessary(it);
}


static void *setfreqbase(fobj *it, double f);
static void *setfreqbase(fobj *it, double f)
{
	it->freqbase = f;
//	post("freqbase %lf", f);
		dumpifnecessary(it);
}

static void *setfreqadd(fobj *it, double f)
{
	it->freqadd = f;
		dumpifnecessary(it);
}

static void *setatten(fobj *it, double f);
static void *setatten(fobj *it, double f)
{
	it->gainscale = exp(-f*0.1151292546497);
		dumpifnecessary(it);
}
static void *setgain(fobj *it, double f);
static void *setgain(fobj *it, double f)
{
	it->gainscale = f;
		dumpifnecessary(it);
}
static void *setoddgain(fobj *it, double f);
static void *setoddgain(fobj *it, double f)
{
	it->oddgain = f;
		dumpifnecessary(it);
}
static void *setevengain(fobj *it, double f);
static void *setevengain(fobj *it, double f)
{
	it->evengain = f;
		dumpifnecessary(it);
}



static void *setmidipitch(fobj *it, double f);
static void *setmidipitch(fobj *it, double f)
{
	it->freqscale = modeltune(f,it->freqbase);
//	post("%d %lf %lf", n,it->freqbase,it->freqscale);
		dumpifnecessary(it);
}
static void *setpitch(fobj *it, double f);
static void *setpitch(fobj *it, double f)
{
	if(it->freqbase >0.0)
		it->freqscale = f/it->freqbase;
//	post("%lf %lf %lf",f,it->freqbase,it->freqscale);
		dumpifnecessary(it);
}

static void *setbwscale(fobj *it, double f);

static void *setbwscale(fobj *it, double f)
{
	it->bwscale = f;
		dumpifnecessary(it);
}
static void *setfspread(fobj *it, double f);
static void *setfspread(fobj *it, double f)
{
	it->fspread = f;
		dumpifnecessary(it);
}

static void *setfstretch(fobj *it, double f);

static void *setfstretch(fobj *it, double f)
{
	it->fstretch = f;
		dumpifnecessary(it);
}
static void *setfaround(fobj *it, double f)
{
	it->faround = f;
		dumpifnecessary(it);
}
static void *setbwstretch(fobj *it, double f);
static void *setbwstretch(fobj *it, double f)
{
	it->bwstretch = f;
		dumpifnecessary(it);
}
static void *setbwspread(fobj *it, double f);

static void *setbwspread(fobj *it, double f)
{
	it->bwspread = f;
		dumpifnecessary(it);
}
static void *setattenuationspread(fobj *it, double f);
static void *setattenuationspread(fobj *it, double f)
{
	it->attenuationspread = f;
		dumpifnecessary(it);
}

static void *setpartialmin(fobj *it, int i);
static void *setpartialmin(fobj *it, int i)
{
	if(i<0)
		post("partial number must be positive");
	else
		it->partialmin = i;
	dumpifnecessary(it);
}
static void *setpartialmax(fobj *it, int i);
static void *setpartialmax(fobj *it, int i)
{
	if(i<0)
		post("partial number must be positive");
	else
		it->partialmax = i;
	dumpifnecessary(it);
}

static void *setclustersize(fobj *it, int i);
static void *setclustersize(fobj *it, int i)
{
	if(i<=0)
		post("cluster size must be larger than 0");
	else
		it->clustersize = i;
	dumpifnecessary(it);
}

void fulllist(fobj *x, struct symbol *s, int argc, struct atom *argv) ;
void fulllist(fobj *x, struct symbol *s, int argc, struct atom *argv) 
{
	x->sinusoidalmodel=0;
	storemodel(x,s,argc,argv,false, false, false);
		dumpifnecessary(x);

}

void sinusoidal(fobj *x, struct symbol *s, int argc, struct atom *argv) ;
void sinusoidal(fobj *x, struct symbol *s, int argc, struct atom *argv) 
{
	x->sinusoidalmodel=1;
	storemodel(x,s,argc,argv,false, false,false);
		dumpifnecessary(x);

}


void more_resonances(fobj *x, struct symbol *s, int argc, struct atom *argv) {
	storemodel(x,s,argc,argv,false, false, true);
	dumpifnecessary(x);
}

void oldfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
void oldfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv)
 {
	x->sinusoidalmodel=0;
	storemodel(x,s,argc,argv, true,false,false);
		dumpifnecessary(x);

}


void formantfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
void formantfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv)
{
	x->sinusoidalmodel=0;

	storemodel(x,s,argc,argv, true,true,false);
		dumpifnecessary(x);
}

static void squelch(fobj *x, double f);
static void squelch(fobj *x, double f)
{
	x->squelch = f;
	dumpresonances(x);
	x->squelch = -1.0;
}
void resondump(fobj *x, struct symbol *s, int argc, Atom *argv)
{
	dumpresonances(x);
}



static void setoneamplitude(fobj *x, int i, double newamplitude);

static void setoneamplitude(fobj *x, int i, double newamplitude) {
	if (i < 0) {
		post("¥ res-transform: setoneamplitude: index must be >= 0");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setoneamplitude: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		x->resonances[i].g = newamplitude;
		dumpifnecessary(x);

	}
}

static void setonefrequency(fobj *x, int i, double newfreq);

static void setonefrequency(fobj *x, int i, double newfreq) {
	if (i < 0) {
		post("¥ res-transform: setonefrequency: index must be >= 0");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setonefrequency: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		x->resonances[i].f = newfreq;
		dumpifnecessary(x);

	}
}

static void setonerate(fobj *x, int i, double newrate) {
	if (i < 0) {
		post("¥ res-transform: setonerate: index must be >= 0");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setonerate: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		x->resonances[i].b = newrate;
		dumpifnecessary(x);
	}
}


static void setoneresonance(fobj *x, struct symbol *s, int argc, struct atom *argv) {
	int i;
	double f, g, b;
	
	if (argc != 4) {
		error("res-transform: setoneresonance needs exactly 4 arguments");
		return;
	} 
	
	if (argv[0].a_type!= A_LONG) {
		error("res-transform: index (first arg) to setone must be an integer");
		return;
	}
	
	if (argv[1].a_type != A_FLOAT || argv[2].a_type != A_FLOAT || argv[3].a_type != A_FLOAT) {
		error("res-transform: freq, gain, and bw must all be floats");
		return;
	}
	
	i = argv[0].a_w.w_long;
	f = argv[1].a_w.w_float;
	g = argv[2].a_w.w_float;
	b = argv[3].a_w.w_float;
	
		
	if (i < -1) {
		post("¥ res-transform: setoneresonance: index must be >= 0 (or -1 to add a new resonance)");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setone: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		if (i == -1) {
			/* Add a new resonance */
			i = (x->nreson)++;
		}
		
		x->resonances[i].f = f;
		x->resonances[i].g = g;
		x->resonances[i].b = b;
		dumpifnecessary(x);
	}
}
static void setonesinusoid(fobj *x, struct symbol *s, int argc, struct atom *argv) {
	int i;
	double f, g;
	
	if (argc != 3) {
		error("res-transform: setonesinusoid needs exactly 3 arguments");
		return;
	} 
	
	if (argv[0].a_type!= A_LONG) {
		error("res-transform: index (first arg) to setone must be an integer");
		return;
	}
	
	if (argv[1].a_type != A_FLOAT || argv[2].a_type != A_FLOAT || argv[3].a_type != A_FLOAT) {
		error("res-transform: freq, gain, and bw must all be floats");
		return;
	}
	
	i = argv[0].a_w.w_long;
	f = argv[1].a_w.w_float;
	g = argv[2].a_w.w_float;
	
		
	if (i < -1) {
		post("¥ res-transform: setone: index must be >= 0 (or -1 to add a new resonance)");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setone: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		if (i == -1) {
			/* Add a new resonance */
			i = (x->nreson)++;
		}
		
		x->resonances[i].f = f;
		x->resonances[i].g = g;
		dumpifnecessary(x);
	}
}
/* this should output to second outlet in next release or be removed forever
void numresonances(fobj *x) {
	outlet_int(x, x->nreson);
}
	
*/

static void setfreqrange(fobj *x, double min, double max) {
	x->fmin = min;
	x->fmax = max;
	dumpresonances(x);
}



static void setamprange(fobj *x, double min, double max) {
	x->mingain = min;
	x->maxgain = max;
	dumpresonances(x);
}


void *myobject_free(fobj *x);
void *myobject_free(fobj *x)
{
freeobject(x->m_proxy);
}

void * fnew(Symbol *s, int argc, Atom *argv) {
	fobj *x;
	int i;

	x = (fobj *)newobject(resonclass);				/* initialize an instance of this class */	
	x->m_proxy = proxy_new(x,1L,&x->m_inletNumber);
	x->dataoutlet = listout(x);
	clearit(x);
	
	x->maxresonances = MAXRESON; // get this from the command line eventually
	x->model = (Atom *) NewPtr(3 * x->maxresonances * sizeof(Atom));
	x->resonances = (struct reson *) NewPtr(sizeof(struct reson) * x->maxresonances);
	
	if(!x->model || ! x->resonances)
	{
		error("cannot allocate space for model");
		return 0;
	}

	x->sinusoidalmodel = (s==gensym("sin-transform")); 
	
	

	x->mingain = 0.0;
	x->maxgain = 9999999.0;
	
	storemodel(x,s,argc, argv, false,false,false);
	computeeverything(x);		// So that tellmeeverything will tell the truth right after the object is instantiated

	for(i=0;i<x->maxresonances;++i)
	{
		x->model[i*3+0].a_type = A_FLOAT;
		x->model[i*3+1].a_type = A_FLOAT;
		x->model[i*3+2].a_type = A_FLOAT;
 	}
	return  x;
}

void main(fptr *f)		/* called once at launch to define this class */
{
	FNS = f;		
	alias("sin-transform");	
	
	setup((struct messlist **) &resonclass, (method) fnew, (method) myobject_free, (int) sizeof(fobj), 0L, A_GIMME, 0 );
	alias("sin-transform");	
	
	addfloat( (method) setfreqbase );
	addmess((method)clear, "clear", 0);
	addmess((method)setpitch, "pitch", A_FLOAT,0);
	addmess((method)setmidipitch, "midi-pitch", A_FLOAT,0);
	addmess((method)setfreqbase, "frequency-base", A_FLOAT,0);
	addmess((method)setfreqscale, "frequency-scale", A_FLOAT,0);
	addmess((method)setfreqadd, "frequency-add", A_FLOAT,0);
	addmess((method)setbwscale, "rate-scale", A_FLOAT,0);
	addmess((method)setatten, "attenuate", A_FLOAT,0);
	addmess((method)setgain, "gain-scale", A_FLOAT,0);
	
	addmess((method)setoneamplitude, "setoneamplitude", A_LONG, A_FLOAT, 0);
	addmess((method)setonefrequency, "setonefrequency", A_LONG, A_FLOAT, 0);
	addmess((method)setonerate, "setonerate", A_LONG, A_FLOAT, 0);

	addmess((method)setoneresonance, "setoneresonance", A_GIMME,0);
	addmess((method)setonesinusoid, "setonesinusoid", A_GIMME,0);
//	addmess((method)numresonances, "numresonances", 0);
	
	addmess((method)setslope, "spectral-slope", A_FLOAT,0);
	addmess((method)setcenter, "spectral-corner", A_FLOAT,0);

	addmess((method)setclustersize, "cluster-size", A_LONG,0);
	addmess((method)setfspread, "frequency-spread", A_FLOAT,0);
	addmess((method)setfaround, "frequency-around", A_FLOAT,0);
	addmess((method)setfstretch, "frequency-stretch", A_FLOAT,0);
	addmess((method)setbwspread, "rate-spread", A_FLOAT,0);
	addmess((method)setbwstretch, "rate-stretch", A_FLOAT,0);
	addmess((method)setattenuationspread, "attenuation-spread", A_FLOAT,0);	
	
		addmess((method)setmidipitch, "midipitch", A_DEFFLOAT,0);
	addmess((method)setfreqbase, "frequencybase", A_DEFFLOAT,0);
	addmess((method)setfreqscale, "frequencyscale", A_DEFFLOAT,0);
	addmess((method)setfreqadd, "frequencyadd", A_DEFFLOAT,0);
	addmess((method)setbwscale, "ratescale", A_DEFFLOAT,0);
	addmess((method)setatten, "attenuate", A_DEFFLOAT,0);
	addmess((method)setgain, "gainscale", A_DEFFLOAT,0);
	
	
	addmess((method)setslope, "spectralslope", A_DEFFLOAT,0);
	addmess((method)setcenter, "spectralcorner", A_DEFFLOAT,0);

	addmess((method)setclustersize, "clustersize", A_DEFLONG,0);
	addmess((method)setfspread, "frequencyspread", A_DEFFLOAT,0);
	addmess((method)setfaround, "frequencyaround", A_DEFFLOAT,0);
	addmess((method)setfstretch, "frequencystretch", A_DEFFLOAT,0);
	addmess((method)setbwspread, "ratespread", A_DEFFLOAT,0);
	addmess((method)setbwstretch, "ratestretch", A_DEFFLOAT,0);
	addmess((method)setattenuationspread, "attenuationspread", A_DEFFLOAT,0);	
	
	
		addmess((method)setfmin, "frequencyminimum", A_DEFFLOAT,0);	
	addmess((method)setfmax, "frequencymaximum", A_DEFFLOAT,0);	
	addmess((method)setpartialmin, "partialminimum", A_DEFLONG,0);	
	addmess((method)setpartialmax, "partialmaximum", A_DEFLONG,0);	

	
	
	addmess((method)setfpivot, "frequencypivot", A_DEFFLOAT,0);	
	addmess((method)setk1, "decayrate", A_DEFFLOAT,0);	
	addmess((method)setk2, "skew", A_DEFFLOAT,0);	
	addmess((method)settime, "time", A_DEFFLOAT,0);	
	
	
	addmess((method)setoddgain, "oddgainscale", A_DEFFLOAT,0);	
	addmess((method)setevengain, "evengainscale", A_DEFFLOAT,0);	
	addmess((method)setevenfreqscale, "evenfrequencyscale", A_DEFFLOAT,0);	
	addmess((method)setoddfreqscale, "oddfrequencyscale", A_DEFFLOAT,0);	
	
	
	
	
//addmess((method)squelch, "squelch", A_DEFFLOAT,0);

//	addmess((method)setfreqrange, "frequency-range", A_FLOAT, A_FLOAT, 0);
	addmess((method)setamprange, "gain-range", A_FLOAT, A_FLOAT, 0);

	addmess((method)fulllist,"list",A_GIMME,0);
	addmess((method)fulllist,"resonance",A_GIMME,0);
	addmess((method)fulllist,"resonances",A_GIMME,0);
	addmess((method)more_resonances,"more",A_GIMME,0);
	addmess((method)oldfulllist,"filter-form",A_GIMME,0);
	addmess((method)formantfulllist,"formant-form",A_GIMME,0);
	addmess((method)sinusoidal,"sinusoids",A_GIMME,0);
	addmess((method)tellmeeverything, "tellmeeverything", 0);
	addmess((method)version, "version", 0);

	addbang( (method) resondump  );
	post(NAME " object version " VERSION " by " AUTHORS ".");
	post("  Copyright © 1986,1987 Adrian Freed");
	post("Copyright © " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");

}

static void version(fobj *x) {

	post(NAME " Version " VERSION
		  ", by " AUTHORS ". Compiled " __TIME__ " " __DATE__);	
}

 static void reson_assist(fobj *x, void *b, long m, long a, char *s) 
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"(list)to resonators~");
	else {
		switch(a) {	
		case 0:
			sprintf(s,"(list/symbol) tranformation parameters");
			break;
		case 1:
			sprintf(s,"(list/symbol) set tranformation parameters (no output)");
			break;
		}
	}
}
static void tellmeeverything(fobj *x) {
	int i;
	version(x);
	post("  Model has %ld resonances, each actually a cluster of size %ld:",
		 x->nreson, x->clustersize);

	post("  I think the base pitch of this model is %lf Hz.", x->freqbase);
	post("  frequency-scale %lf frequency-add %lf", x->freqscale, x->freqadd);
	post("  gain-scale %lf, spectral-slope %lf, spectral-corner %lf", 

		 x->gainscale, x->slope, x->center);
    post("  rate-scale", x->bwscale);
    post("  Clusters: cluster-size %d", x->clustersize);
    post("    frequency-spread %lf, frequency-around %lf, frequency-stretch %lf",
    	 x->fspread, x->faround, x->fstretch);
    post("    rate-spread %lf, rate-stretch %lf, attenuation-spread %lf",
    	 x->bwspread, x->bwstretch, x->attenuationspread);

    	 
     post("    oddgainscale %lf, evengainscale %lf,oddfrequencyscale %lf evenfrequencyscale %lf",
    	 x->oddgain, x->evengain, x->oddfscale,x->evenfscale);
     
     post("    frequencyminimum %lf, frequencymaximum %lf (outside this range gain is set to zero)",
	  x->fmin, x->fmax);
     post("    partialminimum %ld, partialmaximum %ld (outside this range gain is set to zero)",
	  x->partialmin, x->partialmax);


     post("    Percussifier: decayrate %lf, skew %lf, frequencypivot %lf, time %lf",
	  x->k1, x->k2, x->fpivot, x->time);

   if(x->sinusoidalmodel)
 	{
 	    post("  Resulting model (frequency, amplitude duples):");

 	   for (i = 0; i < x->nreson*x->clustersize; ++i) {
    	post("    f %f g %f", x->model[2*i].a_w.w_float, 
    		 x->model[2*i + 1].a_w.w_float);
    		 }
    }
    else
    {
    	    post("  Resulting model (frequency, amplitude, decayrate triplets):");
		for (i = 0; i < x->nreson*x->clustersize; ++i) {
    		post("    f %f g %f r %f", x->model[3*i].a_w.w_float, 
    		 x->model[3*i + 1].a_w.w_float, x->model[3*i + 2].a_w.w_float);
    	}	 
    }
}
