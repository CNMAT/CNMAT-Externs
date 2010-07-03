/*
 Written by Rama Gottfried, The Center for New Music and Audio Technologies, 
 University of California, Berkeley.  Copyright (c) 2010, The Regents of 
 the University of California (Regents).  
 Permission to use, copy, modify, distribute, and distribute modified versions 
 of this software and its documentation without fee and without a signed 
 licensing agreement, is hereby granted, provided that the above copyright 
 notice, this paragraph and the following two paragraphs appear in all copies, 
 modifications, and distributions. 
 
 IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, 
 SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING 
 OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS 
 BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED 
 HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE 
 MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS. 
 
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: mton
 DESCRIPTION: MIDI float/list to symbolic note name conversion
 AUTHORS: Rama Gottfried
 COPYRIGHT_YEARS: 2010
 SVN_REVISION: $LastChangedRevision: 587 $ 
 VERSION 0.9: basically ready for release, might need some cleaning up on error handling
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */


#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_critical.h"

// CNMAT version control
#include "version.h"
#include "version.c"

#define MAXLIST 1024

////////////////////////// object struct
typedef struct _mton 
{
	t_object		ob;			// the object itself (must be first)
	void			*outlet;	// the outlet
	long			centMode;	// 0: round +/- < 50¢ -- 1: absolute
	long			accdntls;	// 0: flats -- 1: sharps
	float			a4;
	t_atom			n[2];
	t_critical		lock;
} t_mton;

///////////////////////// function prototypes
void *mton_new(t_symbol *s, long argc, t_atom *argv);
void mton_free(t_mton *x);
void mton_assist(t_mton *x, void *b, long m, long a, char *s);
void mton_setA4(t_mton *x, double a);
int mton_float(t_mton *x, double f);
int mton_int(t_mton *x, long l);
void mton_list(t_mton *x, t_symbol *s, long argc, t_atom *argv);
int mton_mton(t_mton *x, double f);

//////////////////////// global class pointer variable
void *mton_class;


int main(void)
{	
	t_class *c;
	
	c = class_new("mton", (method)mton_new, (method)mton_free, (long)sizeof(t_mton), 
				  0L /* leave NULL!! */, A_GIMME, 0);
	
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;

	class_addmethod(c, (method)mton_int,		"int",		A_LONG, 0);
	class_addmethod(c, (method)mton_float,		"float",		A_FLOAT, 0);
	class_addmethod(c, (method)mton_list,		"list",		A_GIMME, 0);//note: not setup yet!

	/* you CAN'T call this from the patcher */
    class_addmethod(c, (method)mton_assist,			"assist",		A_CANT, 0);  
	
	CLASS_ATTR_FLOAT(c, "refHz", 0, t_mton, a4);
	CLASS_ATTR_DEFAULT_SAVE(c, "refHz", 0, "440.");

	CLASS_ATTR_LONG(c, "roundMode", 0, t_mton, centMode);
	CLASS_ATTR_DEFAULT_SAVE(c, "roundMode", 0, "1");

	CLASS_ATTR_LONG(c, "accidentalsMode", 0, t_mton, accdntls);
	CLASS_ATTR_DEFAULT_SAVE(c, "accidentalsMode", 0, "1");
	
	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	mton_class = c;

	version(0);
	
	return 0;
}

void mton_list(t_mton *x, t_symbol *s, long argc, t_atom *argv)
{
	long i, safe;
	t_atom *ap;
	t_atom notes[(argc*2)-1];
	t_symbol *first;
	
	for (i = 0, ap = argv; i < argc; i++, ap++) {
		if(atom_gettype(ap)==A_LONG) {
			safe = mton_mton(x, (double)atom_getlong(ap));
			if(safe){
				if(x->centMode == 1){
					if(!i){
						first = atom_getsym(x->n);
						atom_setsym(notes, atom_getsym(x->n+1));
					} else {
						atom_setsym(notes+((i*2)-1), atom_getsym(x->n));
						atom_setsym(notes+(i*2), atom_getsym(x->n+1));
					}
				} else if(x->centMode == 0){
					if(!i){
						first = atom_getsym(x->n);
						atom_setfloat(notes, atom_getfloat(x->n+1));
					} else {
						atom_setsym(notes+((i*2)-1), atom_getsym(x->n));
						atom_setfloat(notes+(i*2), atom_getfloat(x->n+1));
					}
				}
			} else {
				object_error((t_object *)x, "something's wrong A_LONG");
				break;
			}
		} else if (atom_gettype(ap)==A_FLOAT){
			safe = mton_mton(x, atom_getfloat(ap));
			if(safe){
				if(x->centMode == 1){
					if(!i){
						first = atom_getsym(x->n);
						if(atom_gettype(x->n+1)==A_SYM)
							atom_setsym(notes, atom_getsym(x->n+1));
						else
							atom_setfloat(notes, atom_getfloat(x->n+1));
					} else {
						atom_setsym(notes+((i*2)-1), atom_getsym(x->n));
						if(atom_gettype(x->n+1)==A_SYM)
							atom_setsym(notes+(i*2), atom_getsym(x->n+1));
						else
							atom_setfloat(notes+(i*2), atom_getfloat(x->n+1));						
					}
				} else if(x->centMode == 0){
					if(!i){
						first = atom_getsym(x->n);
						atom_setfloat(notes, atom_getfloat(x->n+1));
					} else {
						atom_setsym(notes+((i*2)-1), atom_getsym(x->n));
						atom_setfloat(notes+(i*2), atom_getfloat(x->n+1));
					}
				}
			} else {
				object_error((t_object *)x, "something's wrong A_FLOAT");
				break;
			}
		} else {
			object_error((t_object *)x, "list must contain frequencies only");
			safe = 0;
			break;
		}
	}
	
	if(safe && argc){
		outlet_anything(x->outlet, first, (argc*2)-1, notes);
	}
}

int mton_mton(t_mton *x, double mCents)
{
	char *notes[12];
	if(x->accdntls == 1){
		char *sharps[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
		memcpy(notes, sharps, 12 * sizeof(char *));
	} else if (x->accdntls == 0) {
		char *flats[] = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
		memcpy(notes, flats, 12 * sizeof(char *));
	} else {
		object_error((t_object *)x, "accidental mode can be 0: flats or 1: sharps only");
		return 0;
	}
	
	double cents;
	int mNote, oct;
	char *roundCents, *noteSym;
	t_atom outList[2];
	
	if(mCents){
		switch (x->centMode) {
			case 1:
				if(mCents > 0){
					asprintf(&roundCents, "%0.5f", mCents);
					mNote = (int)trunc(atof(roundCents) + 0.5);
				} else {
					asprintf(&roundCents, "%0.5f", mCents);
					mNote = (int)trunc(atof(roundCents) - 0.5);
				}
				//post("%d", mNote);
				if(mNote >= 0){
					oct = (mNote / 12) - 1;
					asprintf(&noteSym, "%s%d", notes[mNote%12], oct);
				} else {
					oct = ((mNote+1) / 12) - 2;
					asprintf(&noteSym, "%s%d", notes[(288+mNote)%12], oct);	
				}
//				post("%s", noteSym);
				atom_setsym(outList, gensym(noteSym));

				cents = 100 * (mCents - mNote);
				if (fabs(cents) < 0.001)//round off error tweak
					cents = 0;
				
				char *centSym;
				if(cents >= 0){
					asprintf(&centSym, "%+.2lf", (double)(lround(cents * 1000))/1000.);
				//	post("%s %f", centSym, cents);
					atom_setsym(outList+1, gensym(centSym));
				} else {
					asprintf(&centSym, "%+.2lf", (double)(lround(cents * 1000))/1000.);
				//	post("%s %f", centSym, cents);
					atom_setfloat(outList+1, (float)atof(centSym));
				}
				
				free(centSym);
				break;
			case 0:
				asprintf(&roundCents, "%0.5f", mCents);
				mNote = (int)trunc(atof(roundCents));
				
				if(mNote >= 0){
					oct = (mNote / 12) - 1;
					asprintf(&noteSym, "%s%d", notes[mNote%12], oct);
				} else {
					oct = ((mNote+1) / 12) - 2;
					asprintf(&noteSym, "%s%d", notes[(288+mNote)%12], oct);	
				}
				atom_setsym(outList, gensym(noteSym));
				
				cents = 100 * (mCents - mNote);
				if (fabs(cents) < 0.001)//round off error tweak
					cents = 0;
				atom_setfloat(outList+1, cents);
				break;				
			default:
				break;
		}
		critical_enter(x->lock);
		memcpy(x->n, outList, 2 * sizeof(t_atom));
		critical_exit(x->lock);
		free(noteSym);
		free(roundCents);
		return 1;
	} else {
		outlet_anything(x->outlet, gensym("zero"), 0, NULL);
		free(noteSym);
		free(roundCents);
		return 0;
	}
}


int mton_float(t_mton *x, double f)
{
	int t = mton_mton(x, f);
	if(t)
		outlet_anything(x->outlet, atom_getsym(x->n), 1, x->n+1);
	return t;

}

int mton_int(t_mton *x, long l)
{
	int t = mton_mton(x, (double)l);
	if(t)
		outlet_anything(x->outlet, atom_getsym(x->n), 1, x->n+1);
	return t;
	
}

void mton_setA4(t_mton *x, double a){
	if(a>0){
		x->a4 = a;
	} else {
		object_error((t_object *)x, "A4 must be > 0 Hz");
	}

}

void mton_assist(t_mton *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { // inlet
		sprintf(s, "(float/int/list) pitch MIDI number");
	} 
	else {	// outlet
		sprintf(s, "note name(s) with cent offset"); 			
	}
}

void mton_free(t_mton *x)
{
	critical_free(x->lock);
}

void *mton_new(t_symbol *s, long argc, t_atom *argv)
{
	t_mton *x = NULL;
    	
	t_dictionary *d = NULL;
	
	if(!(d = object_dictionaryarg(argc, argv))){
		 return NULL;
	 }
	
	if (x = (t_mton *)object_alloc(mton_class)) {
		x->outlet = outlet_new((t_object *)x,NULL);
	}

	critical_new(&x->lock);

	attr_dictionary_process(x, d);
	return (x);
}
