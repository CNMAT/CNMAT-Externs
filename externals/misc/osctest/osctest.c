/*
  Written by John MacCallum, The Center for New Music and Audio Technologies, 
  University of California, Berkeley.  Copyright (c) 2011, The Regents of 
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
  NAME: osctest
  DESCRIPTION: useless object for testing OSC integration
  AUTHORS: John MacCallum 
  COPYRIGHT_YEARS: 2011
  SVN_REVISION: $LastChangedRevision: 587 $ 
  VERSION 0.0: First try 
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
*/

#include "version.h" 
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_critical.h"
#include "ext_hashtab.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "version.c" 
#include "time.h"
#include "omax_util.h"
#include "osc.h"

typedef struct _osctest{
	t_object ob;
	t_omax osc;
	void *outlet;
	double param;
} t_osctest;

void *osctest_class;

void osctest_bang(t_osctest *x);
void *osctest_new(t_symbol *msg, int argc, t_atom *argv);
void osctest_free(t_osctest *x);

void osctest_bang(t_osctest *x){
	outlet_float(x->outlet, x->param);
}

void osctest_free(t_osctest *x){

}

void *osctest_new(t_symbol *msg, int argc, t_atom *argv){
	t_osctest *x = NULL;
	if(x = (t_osctest *)object_alloc(osctest_class)){
		omax_init(&(x->osc), outlet_new(x, "FullPacket"));
		x->outlet = floatout(x);
		x->param = 0;
		attr_args_process(x, argc, argv);
		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = omax_class_new("osctest", (method)osctest_new, (method)osctest_free, sizeof(t_osctest), calcoffset(t_osctest, osc));
	//t_class *c = class_new("osctest", (method)osctest_new, (method)osctest_free, sizeof(t_osctest), 0L, A_GIMME, 0);

	//class_addmethod(c, (method)osctest_bang, "bang", 0);
	OMAX_ADDMETHOD(c, (method)osctest_bang, "bang", 0);
	OMAX_ADDPARAM(c, "param", float64, 0, t_osctest, param, NULL, NULL);

	class_register(CLASS_BOX, c); 
	osctest_class = c;
	
	return 0;
}
