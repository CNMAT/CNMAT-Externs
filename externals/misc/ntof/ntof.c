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
 NAME: ntof
 DESCRIPTION: Symbolic note name to frequency conversion
 AUTHORS: Rama Gottfried
 COPYRIGHT_YEARS: 2010
 SVN_REVISION: $LastChangedRevision: 587 $ 
 VERSION 0.9: basically ready for release, might need some cleaning up on error handling
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_critical.h"

// CNMAT version control
#include "version.h"
#include "version.c"


#define MAXLIST 4096

////////////////////////// object struct
typedef struct _ntof 
{
	t_object		ob;			// the object itself (must be first)
	void			*outlet;	// the outlet
	float			a4ref;
	char			pitchName[7];
	int				pitchValue[7];
	int				noteCount;
	bool			followsNote;
	t_atom			*freqs;
	int				*mNote;
	float			*mCents;
	t_critical		lock;
} t_ntof;

///////////////////////// function prototypes
void *ntof_new(t_symbol *s, long argc, t_atom *argv);
void ntof_free(t_ntof *x);
void ntof_assist(t_ntof *x, void *b, long m, long a, char *s);
void ntof_anything(t_ntof *x, t_symbol *s, long argc, t_atom *argv);
int ntof_returnMidi(t_ntof *x, t_symbol *s);
void ntof_seta4ref(t_ntof *x, double a);
int ntof_parseSYM(t_ntof *x, t_symbol *s, int noteCount);
void ntof_calcOut(t_ntof *x, int noteCount);
void ntof_bang(t_ntof *x);

//////////////////////// global class pointer variable
void *ntof_class;


int main(void)
{	
	t_class *c;
	
	c = class_new("ntof", (method)ntof_new, (method)ntof_free, (long)sizeof(t_ntof), 
				  0L /* leave NULL!! */, A_GIMME, 0);
	
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;

    class_addmethod(c, (method)ntof_anything,	"anything",		A_GIMME,	0);
	class_addmethod(c, (method)ntof_bang,		"bang",						0);
	/* you CAN'T call this from the patcher */
    class_addmethod(c, (method)ntof_assist,		"assist",		A_CANT,		0);  
	
	CLASS_ATTR_FLOAT(c, "refHz", 0, t_ntof, a4ref);
	CLASS_ATTR_DEFAULT_SAVE(c, "refHz", 0, "440.");

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	ntof_class = c;

	version(0);
	
	return 0;
}


void ntof_anything(t_ntof *x, t_symbol *s, long argc, t_atom *argv)
{
	int q;
	t_atom *ap;
	
	//post("anything: %s and %d arguments", s->s_name, argc);
	
	critical_enter(x->lock);
	for(q=0; q<x->noteCount; q++){
		x->mNote[q] = 0;
		x->mCents[q] = 0;
	}
	x->noteCount = 0;
	x->followsNote = FALSE;
	critical_exit(x->lock);
	
	int t = ntof_parseSYM(x, s, x->noteCount); // why does this not work when i send a 1 but 0 works?
	
	if(t && argc){
		int i;
		for (i = 1, ap = argv; i < argc+1; i++, ap++) { 
			switch (atom_gettype(ap)) {
				case A_LONG:
					critical_enter(x->lock);
					x->mCents[x->noteCount-1] = (float)atom_getlong(ap); // all mCents should be index-1 because of noteCount++
					x->followsNote = FALSE;
					critical_exit(x->lock);
					//post("%d long %f", x->noteCount, x->mCents[x->noteCount-1]);
					break;
				case A_FLOAT:
					//post("%f received", atom_getfloat(ap));
					critical_enter(x->lock);
					x->mCents[x->noteCount-1] = atom_getfloat(ap);
					x->followsNote = FALSE;
					critical_exit(x->lock);
					//post("float %f", x->mCents[x->noteCount-1]);
					break;
				case A_SYM:
					t = ntof_parseSYM(x, atom_getsym(ap), x->noteCount);
					break;
				default:
					break;
			}
			
			if(!t)
				break;
		}	
	}
	if(t)
		ntof_calcOut(x, x->noteCount);

}



int ntof_parseSYM(t_ntof *x, t_symbol *s, int noteCount)
{

	char ch = s->s_name[0];
	int mNote[noteCount];
	float mCents[noteCount];

	critical_enter(x->lock);
	memcpy(mNote, x->mNote, noteCount * sizeof(int));
	memcpy(mCents, x->mCents, noteCount * sizeof(float));
	critical_exit(x->lock);
		
	if( (( ch >= 'a') && (ch <= 'g')) || ((ch >= 'A') && (ch <= 'G')) ){
		if(x->followsNote == TRUE){
			mCents[noteCount-1] = 0;
			//post("follows note! previous cents = %f", mCents[noteCount-1]);
		}
		
		mNote[noteCount] = ntof_returnMidi(x, s);
		
		if(!mNote[noteCount]){
//			post("!mNote[noteCount]");
			return 0;
		}
		
		if(noteCount >= MAXLIST - 1){
			object_error((t_object *)x, "max list size is %d", MAXLIST);
			return 0;
		}
		critical_enter(x->lock);
		x->noteCount++;
		memcpy(x->mNote, mNote, x->noteCount * sizeof(int));
		x->followsNote = TRUE;
		critical_exit(x->lock);
		
		return 1;
		
	} else if((x->followsNote == TRUE) && ((ch == '+') || (ch == '-'))){
		if(ch =='+'){
			mCents[noteCount-1] = atof(s->s_name);
			//post("+%f cents", mCents[noteCount-1]);
		} else if(ch =='-'){
			mCents[noteCount-1] = -1 * atof(s->s_name);
			//post("-%f cents", mCents[noteCount-1]);
		}
		critical_enter(x->lock);
		x->followsNote = FALSE;
		memcpy(x->mCents, mCents, noteCount * sizeof(float));
		critical_exit(x->lock);

		return 1;
	} else if(s == gensym("zero")){
		mNote[noteCount] = 0;
		critical_enter(x->lock);
		x->noteCount++;
		memcpy(x->mNote, mNote, x->noteCount * sizeof(int));
		x->followsNote = FALSE;
		critical_exit(x->lock);
		return 1;
	} else {
		object_error((t_object *)x, "syntax error");
		return 0;
	}
	
	//post("finish %d", x->noteCount);
}


void ntof_calcOut(t_ntof *x, int noteCount)
{
	int mNote[noteCount];
	float mCents[noteCount];
	//post("calcOut noteCount = %d", noteCount);
	
	critical_enter(x->lock);
	memcpy(mNote, x->mNote, noteCount * sizeof(int));
	memcpy(mCents, x->mCents, noteCount * sizeof(float));
	critical_exit(x->lock);

	int i;
	t_atom out[noteCount];
	for(i=0; i<noteCount; i++){
		if(mNote[i]){
			float midiCents = mNote[i] + (mCents[i] * 0.01);
			atom_setfloat(out+i, x->a4ref * pow(2, (midiCents-69)/12));
		} else {
			atom_setfloat(out+i, 0);
		}
		//post("note loop %d note %d cents %f =  %fhz", i, mNote[i], mCents[i], atom_getfloat(out+i));
	}
	if(noteCount > 1)
		outlet_list(x->outlet, NULL, noteCount, out);
	else
		outlet_float(x->outlet, atom_getfloat(out));

	critical_enter(x->lock);
	memcpy(x->freqs, out, noteCount * sizeof(t_atom));
	critical_exit(x->lock);
 

}


int ntof_returnMidi(t_ntof *x, t_symbol *s)
{
	
	long i;
	int pInt = 0;
	int strSize = strlen(s->s_name);
	char letter = s->s_name[0];
	if((letter >= 'a') && (letter <= 'g'))
		letter = toupper(letter);
	
	for(i=0;i<7;i++){
		if(letter == x->pitchName[i])
			pInt = x->pitchValue[i];
	}
	
	char ch;
	int digitCount = 0;
	int octave = 0;
	//int inOctStr = 0; //follow string to determine number group as octave or cents
	for(i=strSize; i>0; i--){
		ch = s->s_name[i];
		if((ch >= '0') && (ch <= '9')){
			
			octave += atoi(&ch) * pow(10, digitCount++);
			//post("octave %d digitcount %d",octave, digitCount);
		}
		switch(ch){
			case'#':
				pInt++;
				break;
			case 'b':
				pInt--;
				break;
			case 'B':
				pInt--;
				break;
			case '-':
				octave *= -1;
			default:
				break;
		}
	}
	/*if( (octave = atoi(&s->s_name[strSize-1])) ){
		octave += 1;
		//object_post((t_object *)x, "this part works:  %d", pInt+octave*12);
	 */
	
	return pInt + (1 + octave)*12;

}

void ntof_bang(t_ntof *x) //after all that, this is really not necessary probably...
{
	outlet_list(x->outlet, NULL, x->noteCount, x->freqs);
	//post("ok ok");
}


void ntof_seta4ref(t_ntof *x, double a){
	x->a4ref = a;
/*	if(x->hz){
		x->hz = x->a4ref * pow(2, (x->midiCents - 69) /12);
		outlet_float(x->outlet, x->hz);
	}
 */
}

void ntof_assist(t_ntof *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { // inlet
		sprintf(s, "(message/list) note name(s) in C4 format, with cent offset (e.g. A4 C#5 -14 ...)");
	} 
	else {	// outlet
		sprintf(s, "(float/list) frequency in Hz"); 			
	}
}

void ntof_free(t_ntof *x)
{
	sysmem_freeptr(x->mNote);
	sysmem_freeptr(x->mCents);
	sysmem_freeptr(x->freqs);
	critical_free(x->lock);
}

void *ntof_new(t_symbol *s, long argc, t_atom *argv)
{
	t_ntof *x = NULL;
    	
	t_dictionary *d = NULL;
	
	if(!(d = object_dictionaryarg(argc, argv))){
		 return NULL;
	 }
	
	if (x = (t_ntof *)object_alloc(ntof_class)) {
		x->outlet = outlet_new((t_object *)x,NULL);
	}
	
	long i;
	char pitchName[] = {	'C',	'D',	'E',	'F',	'G',	'A',	'B' };
	int pitchValue[] = {	0,		2,		4,		5,		7,		9,		11	};
	for(i=0;i<7;i++){
		x->pitchName[i] = pitchName[i];
		x->pitchValue[i] = pitchValue[i];
	}
	
	x->mNote = (int *)sysmem_newptr(MAXLIST * sizeof(int));
	x->mCents = (float *)sysmem_newptr(MAXLIST * sizeof(float));
	x->freqs = (t_atom *)sysmem_newptr(MAXLIST * sizeof(t_atom));
	
	critical_new(&x->lock);
	
	attr_dictionary_process(x, d);
	return (x);
}
