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
*/

/* reson 

Copyright © 1986,1987 Adrian Freed
Copyright © 1996,1997,1998,1999 Regents of the University of California.
	change to accept a giant list of resonances 

Version 1.2 by Matt Wright: compiles with CW 7 and new Max/MSP SDK
Version 1.3 by Matt Wright has "setoneamplitude", "setonefrequency", "setonerate", and "setone"

*/

#define VERSION "1.3"

/* #include <fp.h>
#include <fenv.h> */
#include <math.h>
#include "ext.h"
/*
#include "typedefs.h"
#include "common.h"
*/

int	errno;

fptr *FNS;
void *resonclass;


//Types

typedef		 void	(*myProcPtr)(struct fobj *x );

#ifdef SHIT
enum {
	uppMainProcInfo = kThinkCStackBased
};
#endif

#define PI (3.14159265358979323f)
#define MAXRESON 256
#define NSPECTRALENVELOPE 32

#define A440 440.0f
#define C261 261.6255661f
#define SEMITONE 1.0594630943593f
#define OCTAVESIZE 12
#define OCTAVEBASE 3
/* ln(SEMITONE) */
#define SEMIFAC 0.057762265046662f
#define AASMIDINOTE 69
#define CASMIDINOTE 60

typedef	struct	fobj
{
	Object			object;		/* An embedded MAX object.					*/
	void *m_proxy;
	long m_inletNumber;
	void			*dataoutlet;	/* frequency, gain bandwidth tuplets -first outlet		*/
	Atom			model[3*MAXRESON];
/* current model 	*/
	struct			reson { float f,g,b; }	resonances[MAXRESON];	
	int			nreson; /* number of resonances */
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
	float bwspread; /* in Hz */
	float bwstretch; /* scale factor */
	float attenuationspread; /* in dB */
	
	
}fobj;


/* prototypes */
static float miditopitchratio(int n);
static float modeltune(int n,float p);
static void faxis(float f[],long n);
void * fnew(Symbol *s, int argc, Atom *argv);
static float senv(fobj *x, float f);
static void computeeverything(fobj *x);
static void dumpresonances(fobj *x);
//static void *setsrate(fobj *it, float ef);
static void *setcenter(fobj *it, float f);
static void *setslope(fobj *it, float f);
static void *setnfilters(fobj *it, long n);
static void *setfreqscale(fobj *it, float f);
static void *setfreqbase(fobj *it, float f);
static void *setfreqadd(fobj *it, float f);
static void *setatten(fobj *it, float f);
static void *setgain(fobj *it, float f);
static void *setrmask(fobj *it, float f);
static void *setmidipitch(fobj *it, int f);
static void *setbwscale(fobj *it, float f);
static void	*specenv(fobj *x, struct symbol *s, int argc, Atom *argv);
static void flist(fobj *x, struct symbol *s, int argc, Atom *argv);
static void fulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
static void resondump(fobj *x, struct symbol *s, int argc, Atom *argv);
static void enddump(fobj *x, struct symbol *s, int argc, struct atom *argv);
static void version(fobj *x);
static void tellmeeverything(fobj *x);

static float miditopitchratio(int n) {
	return  expf( SEMIFAC*(n-AASMIDINOTE));
}

static float modeltune(int n,float p) {
	if(p==0.0f)
		return 1.0f;
		
	return miditopitchratio(n)*(A440/p);
}
static void clearit(fobj *x);
static void clearit(fobj *x)
{
	x->freqscale = 1.0f;
	x->freqadd = 0.0f;
	x->bwscale = 1.0f;
	x->center = 0.0f;
	x->slope = 0.0f;
	x->gainscale = 1.0f;
	x->squelch = -1.0f;
	x->freqbase = 0.0f;


	x->clustersize = 1;
	x->fspread = 0.0f;
	x->fstretch = 1.0f;
	x->bwstretch = 1.0f;
	x->bwspread = 0.0f;
	x->attenuationspread = 0.0f;
	x->faround = 0.0f;
}

static void storemodel(fobj *x, struct symbol *s, int argc, struct atom *argv, Boolean old, Boolean formant);
static void storemodel(fobj *x, struct symbol *s, int argc, struct atom *argv, Boolean old, Boolean formant)
{

	if (((argc % 3) == 1) && (argv[0].a_type==A_FLOAT) &&(argv[0].a_w.w_float>0.0f) &&
			(argv[0].a_w.w_float<100000.0f)) {
		x->freqbase = argv[0].a_w.w_float;

		--argc;
		++argv;
	}
	if ((argc % 3) !=0)
	{	
		post("reson: fulllist: bad list");
		return;
	}
	x->fcount = 0;

	while(argc)
	{
			 	if(argv[0].a_type==A_FLOAT && 
			 	argv[1].a_type==A_FLOAT && 
			 	argv[2].a_type==A_FLOAT && x->fcount<MAXRESON)
		 	{
					int n = x->fcount;
					x->resonances[n].f = argv[0].a_w.w_float;
					x->resonances[n].g = old?expf(argv[1].a_w.w_float*0.1151292546497):argv[1].a_w.w_float;
					x->resonances[n].b = old?(argv[2].a_w.w_float*3.1415026535f):argv[2].a_w.w_float;
					if(formant) x->resonances[n].g *= x->resonances[n].f / 250.0;
					++(x->fcount);

//post("%d %d %f",argc, n, x->resonances[n].f);

	 		}
	 		argv+=3 ; argc -= 3;
	}
	x->nreson = x->fcount;
	x->fcount = 0;
}


static void computeeverything(fobj *x)
{
	float fsc = x->freqscale, bwsc=x->bwscale,
	 gainscale = x->gainscale,fadd= x->freqadd,srbar;
	 int i,k;
	int csize = x->clustersize;
	Boolean squelch = ( x->squelch>=0.0f);
	 float r;	
	
	for(i=0;i<x->nreson;++i)
	{
		struct reson transformedresonances;
				
		transformedresonances.b = squelch?x->squelch:(x->resonances[i].b*bwsc);	
		
	//	a = expf(ap*0.1151292546497)*(x->slope*(f-x->center)+1.0f) /* *senv(x,f) */;
	
		transformedresonances.f = (x->resonances[i].f*fsc+fadd);
		transformedresonances.g = x->resonances[i].g*gainscale;
		if(transformedresonances.f>x->center)
			transformedresonances.g *= expf(0.1151292546497f*(x->slope*(x->resonances[i].f-x->center)));
		
		for(k=0;k<csize;++k)
		{
			int halfway = csize/2;
			float f = transformedresonances.f+(k)*(x->fspread/csize)
				+(k-halfway)*(x->faround/csize);
			float g = transformedresonances.g*expf(-0.1151292546497f*k*(x->attenuationspread/csize));
			float b = transformedresonances.b+k*(x->bwspread/csize);
			
			x->model[(csize*i+k)*3+0].a_w.w_float = f;
			x->model[(csize*i+k)*3+1].a_w.w_float = g;
			x->model[(csize*i+k)*3+2].a_w.w_float = b;
		}
	}

}

static void dumpresonances(fobj *x)
{
	computeeverything(x);
 	outlet_list(x->dataoutlet,0L,(short)x->nreson*x->clustersize*3,x->model);
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

static void *setcenter(fobj *it, float f)
{
	it->center = f;
		dumpifnecessary(it);
}

static void *setslope(fobj *it, float f)
{		
	it->slope = f;
		dumpifnecessary(it);
}



static void *setfreqscale(fobj *it, float f)
{
	it->freqscale = f;
		dumpifnecessary(it);
}

static void *setfreqbase(fobj *it, float f)
{
	it->freqbase = f;
//	post("freqbase %f", f);
		dumpifnecessary(it);
}

static void *setfreqadd(fobj *it, float f)
{
	it->freqadd = f;
		dumpifnecessary(it);
}

static void *setatten(fobj *it, float f)
{
	it->gainscale = expf(-f*0.1151292546497f);
		dumpifnecessary(it);
}
static void *setgain(fobj *it, float f)
{
	it->gainscale = f;
		dumpifnecessary(it);
}




static void *setmidipitch(fobj *it, int n)
{
	it->freqscale = modeltune(n,it->freqbase);
//	post("%d %f %f", n,it->freqbase,it->freqscale);
		dumpifnecessary(it);
}
static void *setpitch(fobj *it, float f);
static void *setpitch(fobj *it, float f)
{
	if(it->freqbase >0.0f)
		it->freqscale = f/it->freqbase;
//	post("%f %f %f",f,it->freqbase,it->freqscale);
		dumpifnecessary(it);
}

static void *setbwscale(fobj *it, float f);

static void *setbwscale(fobj *it, float f)
{
	it->bwscale = f;
		dumpifnecessary(it);
}
static void *setfspread(fobj *it, float f);
static void *setfspread(fobj *it, float f)
{
	it->fspread = f;
		dumpifnecessary(it);
}

static void *setfstretch(fobj *it, float f);

static void *setfstretch(fobj *it, float f)
{
	it->fstretch = f;
		dumpifnecessary(it);
}
static void *setfaround(fobj *it, float f)
{
	it->faround = f;
		dumpifnecessary(it);
}
static void *setbwstretch(fobj *it, float f);
static void *setbwstretch(fobj *it, float f)
{
	it->bwstretch = f;
		dumpifnecessary(it);
}
static void *setbwspread(fobj *it, float f);

static void *setbwspread(fobj *it, float f)
{
	it->bwspread = f;
		dumpifnecessary(it);
}
static void *setattenuationspread(fobj *it, float f);
static void *setattenuationspread(fobj *it, float f)
{
	it->attenuationspread = f;
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

void fulllist(fobj *x, struct symbol *s, int argc, struct atom *argv) {
	storemodel(x,s,argc,argv,false, false);
		dumpifnecessary(x);

}
void oldfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
void oldfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv)
 {
	storemodel(x,s,argc,argv, true,false);
		dumpifnecessary(x);

}


void formantfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv);
void formantfulllist(fobj *x, struct symbol *s, int argc, struct atom *argv)
{

	storemodel(x,s,argc,argv, true,true);
		dumpifnecessary(x);
}

static void squelch(fobj *x, float f);
static void squelch(fobj *x, float f)
{
	x->squelch = f;
	dumpresonances(x);
	x->squelch = -1.0f;
}

void resondump(fobj *x, struct symbol *s, int argc, Atom *argv)
{
	dumpresonances(x);
}



/* Matt Wright, Aug 2002... */

static void setoneamplitude(fobj *x, int i, float newamplitude);

static void setoneamplitude(fobj *x, int i, float newamplitude) {
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

static void setonefrequency(fobj *x, int i, float newfreq);

static void setonefrequency(fobj *x, int i, float newfreq) {
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

static void setonerate(fobj *x, int i, float newrate) {
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

static void setone(fobj *x, int i, float f, float g, float b) {
	if (i < 0) {
		post("¥ res-transform: setone: index must be >= 0");
	} else if (i >= x->nreson) {
		post("¥ res-transform: setone: model has only %ld resonances; can't set number %ld",
			  x->nreson, i);
	} else {
		x->resonances[i].f = f;
		x->resonances[i].g = g;
		x->resonances[i].b = b;
		dumpifnecessary(x);
	}
}

/* ... end of Matt Wright's additions */

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
	storemodel(x,s,argc, argv, false,false);
	for(i=0;i<MAXRESON;++i)
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
		
	setup((struct messlist **) &resonclass, (method) fnew, (method) myobject_free, (int) sizeof(fobj), 0L, A_GIMME, 0 );

	addfloat( (method) setfreqbase );
	addmess((method)clear, "clear", 0);
	addmess((method)setpitch, "pitch", A_DEFFLOAT,0);
	addmess((method)setmidipitch, "midi-pitch", A_DEFLONG,0);
	addmess((method)setfreqbase, "frequency-base", A_DEFFLOAT,0);
	addmess((method)setfreqscale, "frequency-scale", A_DEFFLOAT,0);
	addmess((method)setfreqadd, "frequency-add", A_DEFFLOAT,0);
	addmess((method)setbwscale, "rate-scale", A_DEFFLOAT,0);
	addmess((method)setatten, "attenuate", A_DEFFLOAT,0);
	addmess((method)setgain, "gain-scale", A_DEFFLOAT,0);
	
	addmess((method)setoneamplitude, "setoneamplitude", A_LONG, A_FLOAT, 0);
	addmess((method)setonefrequency, "setonefrequency", A_LONG, A_FLOAT, 0);
	addmess((method)setonerate, "setonerate", A_LONG, A_FLOAT, 0);
	addmess((method)setone, "setone", A_LONG, A_FLOAT, A_FLOAT, A_FLOAT, 0);

	addmess((method)setslope, "spectral-slope", A_DEFFLOAT,0);
	addmess((method)setcenter, "spectral-corner", A_DEFFLOAT,0);

	addmess((method)setclustersize, "cluster-size", A_DEFLONG,0);
	addmess((method)setfspread, "frequency-spread", A_DEFFLOAT,0);
	addmess((method)setfaround, "frequency-around", A_DEFFLOAT,0);
	addmess((method)setfstretch, "frequency-stretch", A_DEFFLOAT,0);
	addmess((method)setbwspread, "rate-spread", A_DEFFLOAT,0);
	addmess((method)setbwstretch, "rate-stretch", A_DEFFLOAT,0);
	addmess((method)setattenuationspread, "attenuation-spread", A_DEFFLOAT,0);	
//addmess((method)squelch, "squelch", A_DEFFLOAT,0);
	addmess((method)fulllist,"list",A_GIMME,0);
	addmess((method)oldfulllist,"filter-form",A_GIMME,0);
	addmess((method)formantfulllist,"formant-form",A_GIMME,0);
	addmess((method)tellmeeverything, "tellmeeverything", 0);
	addmess((method)version, "version", 0);

	addbang( (method) resondump  );
	version(0);

}

static void version(fobj *x) {
	post("res-transform " VERSION " - Adrian Freed");
	post("  Copyright © 1986,1987 Adrian Freed");
	post("  Copyright © 1996,1997,1998,1999,2000,2001,2002 Regents of the University of California.");
	post("  Compiled " __DATE__ " " __TIME__);
	post("  Never expires.");
}

static void tellmeeverything(fobj *x) {
	int i;
	version(x);
	post("  Model has %ld resonances, each acually a cluster of size %ld",
		 x->nreson, x->clustersize);
	post("  I think the base pitch of this model is %f Hz.", x->freqbase);
	post("  frequency-scale %f frequency-add %f", x->freqscale, x->freqadd);
	post("  gain-scale %f, spectral-slope %f, spectral-corner %f", 
		 x->gainscale, x->slope, x->center);
    post("  rate-scale", x->bwscale);
    post("  Clusters: cluster-size %d", x->clustersize);
    post("    frequency-spread %f, frequency-around %f, frequency-stretch %f",
    	 x->fspread, x->faround, x->fstretch);
    post("    rate-spread %f, rate-stretch %f, attenuation-spread %f",
    	 x->bwspread, x->bwstretch, x->attenuationspread);
    post("  Resulting model (f, g, bw triplets):");
    for (i = 0; i < x->nreson*x->clustersize; ++i) {
    	post("    f %f g %f r %f", x->model[3*i].a_w.w_float, 
    		 x->model[3*i + 1].a_w.w_float, x->model[3*i + 2].a_w.w_float);
    }
}