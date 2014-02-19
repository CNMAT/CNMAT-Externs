/*
Copyright (c) 1999,2000-05.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Adrian Freed and Matt Wright, The Center for New Music and Audio Technologies,
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
NAME: list-interpolate
DESCRIPTION: Linearly interpolate two lists of numbers element-wise
PUBLICATION: ICMC99 paper about filter objects | http://www.cnmat.berkeley.edu/ICMC99/papers/MSP-filters/filticmc.html
AUTHORS: Adrian Freed and Matt Wright
COPYRIGHT_YEARS: 2000,01,02,03,04,05
DRUPAL_NODE: /patch/4064
SVN_REVISION: $LastChangedRevision$
VERSION 1.1: Bug fixed to really work with floats, Matt Wright 1/4/01
VERSION 1.2: Made compilable in CW 8.3,  Matt Wright 12/5/2
VERSION 1.6: Merged Windows changes into real version
VERSION 1.7: Zero Pad mode
VERSION 1.8: Force Package Info Generation
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


*/
#define NAME "list-interpolate"
#define DESCRIPTION "Linearly interpolate two lists of numbers element-wise"
#define AUTHORS "Adrian Freed and Matt Wright"
#define COPYRIGHT_YEARS "2000-05,12,13"


/* 
   Adapted from "reson", 1999
   Compiled for windows 27 April 2004, Michael Zbyszynski
   
*/


#include "version.h"
#include "ext.h"
#include "ext_obex.h"





t_class *li_class;


//Types
typedef	struct	fobj
{
	Object			object;		/* An embedded MAX object.					*/
	void			*dataoutlet;
	void *bangoutlet;
	
	/* Arrays allocated in the new routine */
	float *rate;
	float *newinputs;
	float *oldinputs;
	t_atom  *out;
	int capacity;	/* Size of these arrays */
	
	int n;
	int countdown;
	int steps;
	int zeroPadMode;
}fobj;


/* prototypes */
void * fnew(t_symbol *s, int argc, t_atom *argv);


static void storelist(fobj *x, struct symbol *s, int argc, struct atom *argv);
static void storelist(fobj *x, struct symbol *s, int argc, struct atom *argv)
{
	float *temp;
	int i;

	/* No worries about mutual exclusion, since this is an event-only object. */

	for(i=0; i<argc; ++i) {
	 	if(argv[i].a_type != A_FLOAT) {	
	 		object_error((t_object *)x, "list did not contain only floats; dropping");
	 		return;
	 	}
	}

	/* Swap x->oldinputs and x->newinputs */
	temp = x->oldinputs;
	x->oldinputs = x->newinputs;
	x->newinputs = temp;
	
	if (argc > x->capacity) {
		post("list-interpolate: %d-element input list trimmed to size %d.",
			 argc, x->capacity);
		argc = x->capacity;
	}
	
	for(i=0;i<argc;++i) {
	 	x->newinputs[i] = argv[i].a_w.w_float;	
	}

	if(x->n == argc) {
		x->countdown = x->steps;
	} else {
		/* Wrong sized list from last time. */
		if (x->zeroPadMode) {
			if (x->n > argc) {
				/* New list has fewer, so just drop end of old list,
				   i.e., pretend new list is same size as old  */
				x->n = argc;
				x->countdown = x->steps;
			} else {
				/* New list has more, so zero-pad old list */
				for (i = x->n; i < argc; ++i) {
					x->oldinputs[i] = 0.0f;
				}
				x->n = argc;
				x->countdown = x->steps;
			}
		} else {
			/* forget the old values. */
			x->n = argc;
			x->countdown = 1;
			for(i=0;i<x->n;++i) {
				x->oldinputs[i] = x->newinputs[i];
			}
		}
	}
	
	for(i=0;i<x->n;++i)	{
		x->rate[i] = (x->newinputs[i] - x->oldinputs[i]) ;
	}
}

static void xbangdump(fobj *x);
static void xbangdump(fobj *x)
{
	float ratio;
	int i;
		
	if (x->countdown<=0) return;
			
	ratio = 1.0f - (((float)x->countdown-1)/(float)x->steps);
	
	for(i=0;i<x->n;++i)
	{
		x->out[i].a_w.w_float  = x->oldinputs[i] + x->rate[i] * ratio;
	}

	--(x->countdown);
	
 	if(x->n > 0) {
 		outlet_list(x->dataoutlet,0L,(short)x->n,x->out);
 	}
 	
	if(x->countdown==0) {
		outlet_bang(x->bangoutlet);
	}
}

static void floatdump(fobj *x, double f);
static void floatdump(fobj *x, double f)
{
		int i;
		for(i=0;i<x->n;++i)
			x->out[i].a_w.w_float  = x->oldinputs[i] + x->rate[i]*f;
	 	if(x->n>0)
 				outlet_list(x->dataoutlet,0L,(short)x->n,x->out);
 }

static void setsteps(fobj *it, long l);
static void setsteps(fobj *it, long l)
{
	it->countdown = it->steps = l;
}

void newlist(fobj *x, struct symbol *s, int argc, struct atom *argv) ;
void newlist(fobj *x, struct symbol *s, int argc, struct atom *argv) {
	
	storelist(x,s,argc,argv);
	
}
void bangdump(fobj *x);
void bangdump(fobj *x)
{
	
	xbangdump(x);
	
}

static void setzeropad(fobj *it, long l);
static void setzeropad(fobj *it, long l)
{
	it->zeroPadMode = l;
}


static void tellmeeverything(fobj *x) {
	int i;
	t_atom a;
	
	
	
	a.a_type = A_FLOAT;
		
	object_post((t_object *)x, "list-interpolate version by " AUTHORS ", compiled " __DATE__ " " __TIME__);
	object_post((t_object *)x, "  list length %ld, list capacity %ld", x->n, x->capacity);
	object_post((t_object *)x, "  %ld steps, current count %ld", x->steps, x->countdown);
	object_post((t_object *)x, "  %susing zero-pad mode", x->zeroPadMode ? "" : "NOT ");
		
	object_post((t_object *)x, "  oldinputs:");
	for (i = 0; i<x->n; ++i) {
		atom_setfloat(&a, x->oldinputs[i]);
		postatom(&a);
	}
	object_post((t_object *)x, "  newinputs:");
	for (i = 0; i<x->n; ++i) {
		atom_setfloat(&a, x->newinputs[i]);
		postatom(&a);
	}
	object_post((t_object *)x, "  rate:");
	for (i = 0; i<x->n; ++i) {
		atom_setfloat(&a, x->rate[i]);
		postatom(&a);
	}
			
	
}

void * fnew(t_symbol *s, int argc, t_atom *argv) {
	fobj *x;
	int i;

	

	x = (fobj *)object_alloc(li_class);				/* initialize an instance of this class */
    if(!x){
        return NULL;
    }
	x->bangoutlet = bangout(x);   // Was listout(x)
	x->dataoutlet = listout(x);

	x->steps = 1;	/* default */
	
	if (argc >= 1) {
		if (argv[0].a_type != A_LONG) {
			object_error((t_object *)x, "first arg must be a long (# steps)");
		} else {
			x->steps = argv[0].a_w.w_long;
		}
	}	
	
	x->capacity = 1024;
	if (argc >= 2) {
		if (argv[1].a_type != A_LONG) {
			object_error((t_object *)x, "second arg must be a long (capacity)");
		} else {
			x->capacity = argv[1].a_w.w_long;
		}
	}
	
	x->rate = 		(float *) getbytes(x->capacity * sizeof(float));
	x->newinputs = 	(float *) getbytes(x->capacity * sizeof(float));
	x->oldinputs = 	(float *) getbytes(x->capacity * sizeof(float));
	x->out = 		(t_atom *) getbytes(x->capacity * sizeof(t_atom));
	
	if (x->out == 0) {
		object_error((t_object *)x, "not enough memory for capacity %ld!", x->capacity);
		
		return 0;
	}
	
	for (i = 0; i<x->capacity; ++i) {
		x->out[i].a_type = A_FLOAT;
	}
	
	x->n = 0;
	x->countdown = 1;
	x->zeroPadMode = 0;
	
	
	return  x;
}

void ffree(fobj *x);
void ffree(fobj *x) {
	freebytes((char *) x->rate, sizeof(float));
	freebytes((char *) x->newinputs, sizeof(float));
	freebytes((char *) x->oldinputs, sizeof(float));
	freebytes((char *) x->out, x->capacity * sizeof(t_atom));
}	

int main(void)		/* called once at launch to define this class */
{		
	li_class = class_new("list-interpolate", (method) fnew, (method) ffree, (int) sizeof(fobj), 0L, A_GIMME, 0 );


	class_addmethod(li_class, (method)setsteps, "steps", A_DEFLONG,0);
	class_addmethod(li_class, (method)setzeropad, "zeropad", A_LONG,0);
	class_addmethod(li_class, (method)newlist,"list",A_GIMME,0);
	class_addmethod(li_class, (method)tellmeeverything, "tellmeeverything", 0);
	class_addmethod(li_class,  (method) bangdump  , "bang", 0);
	class_addmethod(li_class,  (method) floatdump  , "float", A_FLOAT, 0);
	
	//post(NAME " object version by " AUTHORS ".");
	//post("Copyright � " COPYRIGHT_YEARS " Regents of the University of California. All Rights Reserved.");
	version_post_copyright();
    class_register(CLASS_BOX, li_class);
    return 0;
}
