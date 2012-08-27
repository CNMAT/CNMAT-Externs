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
t_max_err osctest_notify(t_object *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void osctest_FullPacket(t_osctest *x, long len, long ptr){
	// could do whatever we want with the bundle here...
	
	// forward the bundle on to the omax system that will call us back if there are
	// addresses in the bundle that our object has methods to respond to.
	omax_FullPacket((t_object *)x, len, ptr);
}

void osctest_bang(t_osctest *x){
	outlet_float(x->outlet, x->param);
	omax_outputState((t_object *)x);
}

void osctest_free(t_osctest *x){

}

// this will get called in response to the max message ``param <value>'' or
// the OSC message /osctest/param/set
void osctest_setparam(t_osctest *x, t_object *attr, long argc, t_atom *argv){
	if(argc){
		x->param = atom_getfloat(argv);
	}
}

void *osctest_new(t_symbol *msg, int argc, t_atom *argv){
	t_osctest *x = NULL;
	if(x = (t_osctest *)object_alloc(osctest_class)){
		omax_init((t_object *)x);
		x->outlet = floatout(x);
		x->param = 0;
		attr_args_process(x, argc, argv);
		return x;
	}
	return NULL;
}

int main(void){
	// in the call to omax_class_new, we pass osctest_FullPacket as the last argument to override the
	// default omax_FullPacket method.  We could pass NULL instead if we don't want to do anything
	// with an incoming bundle.
	t_class *c = omax_class_new("osctest", (method)osctest_new, (method)osctest_free, sizeof(t_osctest), calcoffset(t_osctest, osc), (method)osctest_FullPacket);
	//t_class *c = class_new("osctest", (method)osctest_new, (method)osctest_free, sizeof(t_osctest), 0L, A_GIMME, 0);

	// OMAX_ADDMETHOD() replaces class_addmethod
	//class_addmethod(c, (method)osctest_bang, "bang", 0);
	OMAX_ADDMETHOD(c, (method)osctest_bang, "bang", 0);


	//CLASS_ATTR_DOUBLE(c, "param", 0, t_osctest, param);
	//CLASS_ATTR_ACCESSORS(c, "param", osctest_setparam, NULL);

	// OMAX_ADDPARAM() replaces the two calls above.
	// here, we are overriding the setter with our own method.
	OMAX_ADDPARAM(c, "param", float64, 0, t_osctest, param, osctest_setparam, NULL);

	// the omax system registers itself with max to receive notifications, so if your object
	// asks to be notified in the usual way by calling class_addmethod(c, (method)func, "notify", ...),
	// the info outlet of the object will not output the state in response to a changed attribute.
	// not the end of the world, but in order for things to work as designed, register with omax
	// to receive a notification.
	omax_addnotification(c, (method)osctest_notify);

	class_register(CLASS_BOX, c); 
	osctest_class = c;
	
	return 0;
}

t_max_err osctest_notify(t_object *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	t_symbol *attrname;

	if (msg == gensym("attr_modified")) {
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		// do something here...
	}
	return MAX_ERR_NONE;
}
