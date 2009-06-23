/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2009, The Regents of
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
  NAME: te_breakout~
  DESCRIPTION: A breakout object for the bkouto_editor
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2009
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "z_dsp.h"

typedef struct _bkout{
	t_pxobject ob;
	t_symbol *name;
	int function;
} t_bkout;

static t_class *bkout_class;

void bkout_dsp(t_bkout *x, t_signal **sp, short *count);
t_int *bkout_perform(t_int *w);
void bkout_free(t_bkout *x);
void bkout_assist(t_bkout *x, void *b, long m, long a, char *s);
t_max_err bkout_notify(t_bkout *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
t_symbol *bkout_mangleName(t_symbol *name, int fnum, int i);
void *bkout_new(t_symbol *msg, short argc, t_atom *argv);

void bkout_dsp(t_bkout *x, t_signal **sp, short *count){
	dsp_add(bkout_perform, 5, x, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec);
}

t_int *bkout_perform(t_int *w){
	t_bkout *x = (t_bkout *)w[1];
	int n = (int)w[2];
	t_float *out1 = (t_float *)w[3];
	t_float *out2 = (t_float *)w[4];
	t_float *out3 = (t_float *)w[5];

	t_symbol *name1, *name2, *name3;
	t_float *in1 = NULL, *in2 = NULL, *in3 = NULL;

	if(name1 = bkout_mangleName(x->name, 1, x->function)){
		in1 = (t_float *)(name1->s_thing);
		//post("te_breakout~: name1 = %s, %p", name1->s_name, in1);
	}
	if(name2 = bkout_mangleName(x->name, 2, x->function)){
		in2 = (t_float *)(name2->s_thing);
		//post("te_breakout~: name2 = %s, %p", name2->s_name, in2);
        }
	if(name3 = bkout_mangleName(x->name, 3, x->function)){
		in3 = (t_float *)(name3->s_thing);
		//post("te_breakout~: name3 = %s, %p", name3->s_name, in3);
	}

	memset(out1, 0, n * sizeof(t_float));
	memset(out2, 0, n * sizeof(t_float));
	memset(out3, 0, n * sizeof(t_float));

	if(in1){
		memcpy(out1, in1, n * sizeof(t_float));
	}

	if(in2){
		memcpy(out2, in2, n * sizeof(t_float));
	}

	if(in3){
		memcpy(out3, in3, n * sizeof(t_float));
	}
	//int i;
	//for(i = 0; i < n; i++){
	//}
    
	return w + 6;
}

void bkout_free(t_bkout *x){
}

void bkout_assist(t_bkout *x, void *b, long io, long index, char *s){
	switch(io){
	case 1:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	case 2:
		switch(index){
		case 0:
			sprintf(s, "");
			break;
		}
		break;
	}
}

t_max_err bkout_notify(t_bkout *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	//t_symbol *attrname; 
 	if (msg == gensym("attr_modified")){ 
 		//attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		//x->sel_x.click = x->sel_x.drag = x->sel_y.click = x->sel_y.drag = -1; 
		//post("notify");
	} 
	return 0; 
} 


t_symbol *bkout_mangleName(t_symbol *name, int i, int fnum){
	if(!name){
		return NULL;
	}
	char buf[256];
	sprintf(buf, "tempo_editor_%s_%d_%d", name->s_name, i, fnum);
	return gensym(buf);
}

/**************************************************
Object and instance creation functions.
**************************************************/

void *bkout_new(t_symbol *msg, short argc, t_atom *argv){
	t_bkout *x;

	if(x = (t_bkout *)object_alloc(bkout_class)){
		dsp_setup((t_pxobject *)x, 0);
		outlet_new(x, "signal");
		outlet_new(x, "signal");
		outlet_new(x, "signal");
		x->name = NULL;
		x->function = 0;
        
		attr_args_process(x, argc, argv);
		if(x->name == NULL){
			error("te_breakout~: you must supply a name");
			return NULL;
		}

		x->ob.z_misc = Z_PUT_LAST;

		return x;
	}
	return NULL;
}

int main(void){
	t_class *c = class_new("te_breakout~", (method)bkout_new, (method)dsp_free, sizeof(t_bkout), 0L, A_GIMME, 0);
    
	class_dspinit(c);

	class_addmethod(c, (method)bkout_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)bkout_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)bkout_assist, "assist", A_CANT, 0);
    
	CLASS_ATTR_SYM(c, "name", 0, t_bkout, name);
	CLASS_ATTR_LONG(c, "function", 0, t_bkout, function);

	class_register(CLASS_BOX, c);
	bkout_class = c;

	common_symbols_init();

	return 0;
}

