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
  NAME: o.message
  DESCRIPTION: Message box for OSC bundles
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2009
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  VERSION 1.0: using updated lib
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

#include "jpatcher_api.h" 
//#include "jpatcher_syms.h"
#include "jgraphics.h"

//#include "cmmjl/cmmjl.h"
//#include "cmmjl/cmmjl_osc.h"
#include "omax_util.h"
#include "osc_util.h"
#include "osc_match.h"
#include <mach/mach_time.h>

// i really don't want to reallocate...
#define OMESSAGE_MAX_NUM_MESSAGES 1024
#define OMESSAGE_MAX_MESSAGE_LENGTH 1024
#define BUFLEN 4096

typedef struct _omessage{
	t_jbox ob;
	void *outlet;
	void *proxy;
	long inlet;
	char *buffer;
	int buffer_len;
	int buffer_pos;
	//t_cmmjl_osc_message *messages;
	//int max_num_messages;
	//int num_messages;
	t_atom *atoms;
	int max_num_atoms;
	int num_atoms;
	//char **message_strings;
	int *substitutions;
	int substitutions_len;
	t_jrgba frame_color, background_color, text_color;
} t_omessage;

static t_class *omessage_class;

void omessage_fullPacket(t_omessage *x, long len, long ptr);
void omessage_doFullPacket(t_omessage *x, long len, long ptr);
void omessage_cbk(t_osc_msg msg, void *v);
void omessage_paint(t_omessage *x, t_object *patcherview);
void omessage_atoms2text(t_omessage *x, char *buf);
void omessage_parse_string(t_omessage *x, long len, char *string);
void omessage_set(t_omessage *x, t_symbol *s, long ac, t_atom *av);
void omessage_select(t_omessage *x);
void omessage_doselect(t_omessage *x);
long omessage_key(t_omessage *x, t_object *patcherview, long keycode, long modifiers, long textcharacter);
long omessage_keyfilter(t_omessage *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter);
void omessage_enter(t_omessage *x);
void omessage_gettext(t_omessage *x);
void omessage_make_and_output_bundle(t_omessage *x);
void omessage_mousedown(t_omessage *x, t_object *patcherview, t_pt pt, long modifiers);
void omessage_mouseup(t_omessage *x, t_object *patcherview, t_pt pt, long modifiers);
void omessage_bang(t_omessage *x);
void omessage_int(t_omessage *x, long n);
void omessage_float(t_omessage *x, double xx);
void omessage_list(t_omessage *x, t_symbol *msg, short argc, t_atom *argv);
void omessage_anything(t_omessage *x, t_symbol *msg, short argc, t_atom *argv);
void omessage_settext(t_omessage *x, t_symbol *msg, short argc, t_atom *argv);
void omessage_free(t_omessage *x);
void omessage_assist(t_omessage *x, void *b, long m, long a, char *s);
void omessage_inletinfo(t_omessage *x, void *b, long index, char *t);
void *omessage_new(t_symbol *msg, short argc, t_atom *argv);


void omessage_fullPacket(t_omessage *x, long len, long ptr){
	if(proxy_getinlet((t_object *)x) == 0){
		return;
	}
	omessage_doFullPacket(x, len, ptr);
}

void omessage_doFullPacket(t_omessage *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	memcpy(x->buffer, (char *)ptr, len);
	x->buffer_pos = len;
	long nn = len;
	x->num_atoms = 0;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(x->buffer, "#bundle\0", 8)){
		nn = osc_util_bundle_naked_message(len, x->buffer, x->buffer);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = osc_util_flatten(nn, x->buffer, x->buffer);

	// extract the messages from the bundle
	//cmmjl_osc_extract_messages(nn, x->buffer, true, omessage_cbk, (void *)x);
	osc_util_parseBundleWithCallback(nn, x->buffer, omessage_cbk, (void *)x);

	int i;
	char buf[4096];
	omessage_atoms2text(x, buf);

	object_method(jbox_get_textfield((t_object *)x), gensym("settext"), buf);
	jbox_redraw((t_jbox *)x);

	for(i = 0; i < x->substitutions_len; i++){
		x->substitutions[i] = -1;
	}
}

void omessage_cbk(t_osc_msg msg, void *v){
	printf("%s\n", __PRETTY_FUNCTION__);
	t_omessage *x = (t_omessage *)v;
	int i;
	//x->messages[x->num_messages] = msg;
	//t_cmmjl_osc_atom a[msg.argc];
	//cmmjl_osc_get_data(&msg, a);

	//atom_setsym(x->atoms + x->num_atoms++, gensym(msg.address));

	char buf[1024], data_buf[256];
	char *bufptr = buf;
	long len;
	t_atom a[1024];
	//for(i = 0; i < msg.argc; i++){
		//cmmjl_osc_atom2maxatom(a + i, x->atoms + x->num_atoms++);
	omax_util_oscMsg2MaxAtoms(&msg, &len, a);
		//}
	for(i = 0; i < len; i++){
		x->atoms[x->num_atoms++] = a[i];
	}
}

void omessage_paint(t_omessage *x, t_object *patcherview){
	t_rect rect;
	t_jrgba white = (t_jrgba){1., 1., 1., 1.};    
	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	jgraphics_set_source_jrgba(g, &(x->background_color));
	//jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_move_to(g, 0, 0);
	jgraphics_line_to(g, 0, rect.height - 8);
	jgraphics_line_to(g, 8, rect.height);
	jgraphics_line_to(g, rect.width, rect.height);
	jgraphics_line_to(g, rect.width, 8);
	jgraphics_line_to(g, rect.width - 8, 0);
	jgraphics_line_to(g, 0, 0);
	jgraphics_fill(g);

	jgraphics_ellipse(g, rect.width - 16., 0., 16, 16);
	jgraphics_ellipse(g, 0., rect.height - 16., 16., 16.);
	jgraphics_fill(g);

	jgraphics_set_source_jrgba(g, &(x->frame_color));
	jgraphics_set_line_width(g, 2.);
	jgraphics_move_to(g, rect.width * .75, 0.);
	jgraphics_line_to(g, 0., 0.);
	jgraphics_line_to(g, 0., rect.height * .25);
	jgraphics_move_to(g, rect.width - (rect.width * .75), rect.height);
	jgraphics_line_to(g, rect.width, rect.height);
	jgraphics_line_to(g, rect.width, rect.height - (rect.height * .25));
	jgraphics_stroke(g);
}

void omessage_atoms2text(t_omessage *x, char *buf){
	char *bufptr = buf;
	int i;
	for(i = 0; i < x->num_atoms; i++){
		switch(atom_gettype(x->atoms + i)){
		case A_LONG:
			bufptr += sprintf(bufptr, "%ld ", atom_getlong(x->atoms + i));
			break;
		case A_FLOAT:
			bufptr += sprintf(bufptr, "%f ", atom_getfloat(x->atoms + i));
			break;
		case A_SYM:
			{
				t_symbol *sym = atom_getsym(x->atoms + i);
				if(*(sym->s_name) == '/' && i > 0){
					bufptr += sprintf(bufptr, "\n%s ", sym->s_name);
				}else{
					bufptr += sprintf(bufptr, "%s ", sym->s_name);
				}
			}
			break;
		}
	}
}

void omessage_parse_string(t_omessage *x, long len, char *string){
	long argc = 0;
	t_atom *argv = NULL;
	t_max_err err = atom_setparse(&argc, &argv, string);
	if(err != MAX_ERR_NONE){
		return;
	}

	//x->buffer_pos += cmmjl_osc_init_bundle(x->buffer_len, x->buffer, NULL);
	strncpy(x->buffer, "#bundle\0", 8);
	x->buffer_pos = 8;

	x->buffer_pos += osc_util_make_bundle_from_atoms(argc, argv, &(x->buffer_len), x->buffer + x->buffer_pos);

	/*
	  int i;
	  post("buffer_pos = %d", x->buffer_pos);
	  for(i = 0; i < x->buffer_pos; i++){
	  post("%d %c 0x%x", i, x->buffer[i], x->buffer[i]);
	  }
	*/
}

void omessage_set(t_omessage *x, t_symbol *s, long ac, t_atom *av){
	if(proxy_getinlet((t_object *)x)){
		return;
	}
	if(ac){
		if(atom_gettype(av) == A_SYM){
			t_symbol *sym = atom_getsym(av);
			if(strlen(sym->s_name) == 10){
				if(!(strncmp(sym->s_name, "FullPacket", 10) && ac == 3)){
					omessage_doFullPacket(x, atom_getlong(av + 1), atom_getlong(av + 2));
					return;
				}
			}
			omessage_settext(x, NULL, ac, av);
		}
	}
}

void omessage_select(t_omessage *x){
	defer(x, (method)omessage_doselect, 0, 0, 0);
}

void omessage_doselect(t_omessage *x){
	t_object *p = NULL; 
	object_obex_lookup(x,gensym("#P"), &p);
	if (p) {
		t_atom rv; 
		long ac = 1; 
		t_atom av[1]; 
		atom_setobj(av, x); 
		object_method_typed(p, gensym("selectbox"), ac, av, &rv); 
	}
}


long omessage_key(t_omessage *x, t_object *patcherview, long keycode, long modifiers, long textcharacter){
	char buff[256];
	buff[0] = textcharacter;  // we know this is just a simple char
	buff[1] = 0; 
	object_method(patcherview, gensym("insertboxtext"), x, buff);
	jbox_redraw((t_jbox *)x);

	return 1; 
}

long omessage_keyfilter(t_omessage *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter){
	t_atom arv;
	long rv = 1;
	long k = *keycode;
	
	if (k == JKEY_TAB || k == JKEY_ENTER || k == JKEY_RETURN || k == JKEY_ESC) {
		object_method_typed(patcherview, gensym("endeditbox"), 0, NULL, &arv); 
		rv = 0;		// don't pass those keys to omessage
	}
	return rv;
}

// enter is triggerd at "endeditbox time"
void omessage_enter(t_omessage *x){
	omessage_gettext(x);
}

void omessage_gettext(t_omessage *x){
	x->buffer_pos = 0;
	long size	= 0;
	char *text	= NULL;
	t_object *textfield = jbox_get_textfield((t_object *)x);
	object_method(textfield, gensym("gettextptr"), &text, &size);
	if(size){
		long argc = 0;
		t_atom *argv = NULL;
		t_max_err err = atom_setparse(&argc, &argv, text);
		if(err != MAX_ERR_NONE){
			return;
		}

		int i;
		for(i = 0; i < x->substitutions_len; i++){
			x->substitutions[i] = -1;
		}
		for(i = 0; i < argc; i++){
			long type = atom_gettype(argv + i);
			long sub = 0;
			if(type == A_DOLLSYM){
				t_symbol *sym = argv[i].a_w.w_sym;
				sub = strtol(sym->s_name, NULL, 0);
				if(sub > x->substitutions_len){
					int *tmp = realloc(x->substitutions, sizeof(int) * (sub * 2));
					if(tmp){
						x->substitutions = tmp;
					}else{
						error("o.message: couldn't allocate memory for %d substitutions", sub);
						return;
					}
					int j;
					for(j = x->substitutions_len; j < sub * 2; j++){
						x->substitutions[j] = -1;
					}
					x->substitutions_len = sub * 2;
				}
				x->substitutions[sub - 1] = i;
				atom_setlong(argv + i, 0);
			}else if(type == A_DOLLAR){
				sub = argv[i].a_w.w_long; // atom_getlong() apparently doesn't worke with A_DOLLAR
				if(sub > x->substitutions_len){
					int *tmp = realloc(x->substitutions, sizeof(int) * (sub * 2));
					if(tmp){
						x->substitutions = tmp;
					}else{
						error("o.message: couldn't allocate memory for %d substitutions", sub);
						return;
					}
					int j;
					for(j = x->substitutions_len; j < sub * 2; j++){
						x->substitutions[j] = -1;
					}
					x->substitutions_len = sub * 2;
				}
				x->substitutions[sub - 1] = i;
				atom_setlong(argv + i, 0);
			}else if(type == A_SYM){
				t_symbol *sym = argv[i].a_w.w_sym;
				if(*(sym->s_name) == '$'){
					sub = strtol(sym->s_name + 1, NULL, 0);
					if(sub > x->substitutions_len){
						int *tmp = realloc(x->substitutions, sizeof(int) * (sub * 2));
						if(tmp){
							x->substitutions = tmp;
						}else{
							error("o.message: couldn't allocate memory for %d substitutions", sub);
							return;
						}
						int j;
						for(j = x->substitutions_len; j < sub * 2; j++){
							x->substitutions[j] = -1;
						}
						x->substitutions_len = sub * 2;
					}
					x->substitutions[sub - 1] = i;
					atom_setlong(argv + i, 0);
				}
			}else{

			}

		}

		memcpy(x->atoms, argv, argc * sizeof(t_atom));
		x->num_atoms = argc;

		sysmem_freeptr(argv);

	}else{
		x->buffer_pos = 0;
	}
}

void omessage_make_and_output_bundle(t_omessage *x){
	int i;
	x->buffer_pos = 0;
	//x->buffer_pos += cmmjl_osc_init_bundle(x->buffer_len, x->buffer, NULL);
	strcpy(x->buffer, "#bundle\0");
	x->buffer_pos = 8;
	//ntptime now;
	//cmmjl_osc_timetag_now_to_ntp(&now);
	//cmmjl_osc_timetag_set(x->buffer_len, (long)x->buffer, &now);
	*((unsigned long long *)(x->buffer + x->buffer_pos)) = hton64(mach_absolute_time());
	x->buffer_pos += 8;

	x->buffer_pos += osc_util_make_bundle_from_atoms(x->num_atoms, x->atoms, &(x->buffer_len), x->buffer + x->buffer_pos);
	//for(i = 0; i < x->buffer_pos; i++){
	//post("%d %c 0x%x", i, x->buffer[i], x->buffer[i]);
	//}
	t_atom a[2];
	atom_setlong(a, x->buffer_pos);
	atom_setlong(a + 1, (long)(x->buffer));
	outlet_anything(x->outlet, gensym("FullPacket"), 2, a);
}

void omessage_mousedown(t_omessage *x, t_object *patcherview, t_pt pt, long modifiers){
        textfield_set_textmargins(jbox_get_textfield((t_object *)x), 4, 4, 2, 2);
	jbox_redraw((t_jbox *)x);
}

void omessage_mouseup(t_omessage *x, t_object *patcherview, t_pt pt, long modifiers){
        textfield_set_textmargins(jbox_get_textfield((t_object *)x), 3, 3, 3, 3);
	jbox_redraw((t_jbox *)x);
	omessage_make_and_output_bundle(x);
}

void omessage_bang(t_omessage *x){
	omessage_make_and_output_bundle(x);
}

void omessage_int(t_omessage *x, long n){
	t_atom a;
	atom_setlong(&a, n);
	omessage_list(x, NULL, 1, &a);
}

void omessage_float(t_omessage *x, double f){
	t_atom a;
	atom_setfloat(&a, f);
	omessage_list(x, NULL, 1, &a);
}

void omessage_list(t_omessage *x, t_symbol *msg, short argc, t_atom *argv){
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		{
			int i;
			int n = argc <= x->substitutions_len ? argc : x->substitutions_len;
			for(i = 0; i < n; i++){
				if(x->substitutions[i] >= 0){
					x->atoms[x->substitutions[i]] = argv[i];
					//post("substituting the %dth element of argv into the %dth position in x->atoms", i, x->substitutions[i]);
				}
			}
			omessage_make_and_output_bundle(x);
		}
		break;
	case 1:

		break;
	}
}

void omessage_anything(t_omessage *x, t_symbol *msg, short argc, t_atom *argv){
	t_atom av[argc + 1];
	int ac = argc;

	if(msg){
		ac = argc + 1;
		atom_setsym(av, msg);
		memcpy(av + 1, argv, argc * sizeof(t_atom));
	}else{
		memcpy(av, argv, argc * sizeof(t_atom));
	}
	switch(proxy_getinlet((t_object *)x)){
	case 0:
		omessage_list(x, NULL, ac, av);
		break;
	case 1:
		omessage_settext(x, NULL, ac, av);
		break;
	}
}

void omessage_settext(t_omessage *x, t_symbol *msg, short argc, t_atom *argv){
	if(atom_gettype(argv) != A_SYM){
		error("o.message: not a proper OSC message");
		return;
	}
	t_symbol *address = atom_getsym(argv);
	if(*(address->s_name) != '/'){
		error("o.message: %s is not a valid OSC address", address->s_name);
		return;
	}
	memcpy(x->atoms, argv, argc * sizeof(t_atom));
	x->num_atoms = argc;
	char buf[2048];
	omessage_atoms2text(x, buf);
	object_method(jbox_get_textfield((t_object *)x), gensym("settext"), buf);
	jbox_redraw((t_jbox *)x);
	int i;
	for(i = 0; i < x->substitutions_len; i++){
		x->substitutions[i] = -1;
	}
}

void omessage_free(t_omessage *x){
	jbox_free((t_jbox *)x);
}

void omessage_assist(t_omessage *x, void *b, long io, long index, char *s){
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


/**************************************************
Object and instance creation functions.
**************************************************/

void *omessage_new(t_symbol *msg, short argc, t_atom *argv){
	t_omessage *x;
    
	t_dictionary *d = NULL; 
 	long boxflags; 
    
	// box setup 
	if(!(d = object_dictionaryarg(argc, argv))){ 
		return NULL; 
	} 
    
	boxflags = 0 
		| JBOX_DRAWFIRSTIN 
		| JBOX_NODRAWBOX
		| JBOX_DRAWINLAST
		| JBOX_TRANSPARENT  
		//      | JBOX_NOGROW
		//| JBOX_GROWY
		//| JBOX_GROWBOTH
		//      | JBOX_HILITE
		//| JBOX_BACKGROUND
		//| JBOX_DRAWBACKGROUND
		//      | JBOX_NOFLOATINSPECTOR
		//      | JBOX_MOUSEDRAGDELTA
		| JBOX_TEXTFIELD
		;

	if(x = (t_omessage *)object_alloc(omessage_class)){
		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->ob.b_firstin = (void *)x; 
		x->outlet = outlet_new(x, NULL);
		x->proxy = proxy_new(x, 1, &(x->inlet));
		x->atoms = (t_atom *)malloc(OMESSAGE_MAX_NUM_MESSAGES * sizeof(t_atom));
		x->max_num_atoms = OMESSAGE_MAX_NUM_MESSAGES;
		x->num_atoms = 0;
		x->buffer = (char *)calloc(BUFLEN, sizeof(char));
		x->buffer_len = BUFLEN;
		x->buffer_pos = 0;
		x->substitutions = (int *)calloc(1024, sizeof(int));
		x->substitutions_len = 1024;

		int i;
		for(i = 0; i < x->substitutions_len; i++){
			x->substitutions[i] = -1;
		}

		attr_dictionary_process(x, d); 

		t_object *textfield = jbox_get_textfield((t_object *)x);
		if(textfield){
			object_attr_setchar(textfield, gensym("editwhenunlocked"), 1);
			textfield_set_editonclick(textfield, 0);
			textfield_set_textmargins(textfield, 3, 3, 3, 3);
			textfield_set_textcolor(textfield, &(x->text_color));
		}


 		jbox_ready((t_jbox *)x);

		omessage_gettext(x);
        
		return x;
	}
	return NULL;
}

int main(void){
	common_symbols_init();
	//jpatcher_syms_init();
	t_class *c = class_new("o.message", (method)omessage_new, (method)omessage_free, sizeof(t_omessage), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_TEXTFIELD | JBOX_FIXWIDTH | JBOX_FONTATTR); 
    
	class_addmethod(c, (method)omessage_paint, "paint", A_CANT, 0); 

	class_addmethod(c, (method)omessage_bang, "bang", 0);
	class_addmethod(c, (method)omessage_int, "int", A_LONG, 0);
	class_addmethod(c, (method)omessage_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)omessage_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)omessage_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)omessage_set, "set", A_GIMME, 0);
	class_addmethod(c, (method)omessage_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);
	class_addmethod(c, (method)omessage_fullPacket, "FullPacket", A_LONG, A_LONG, 0);

	class_addmethod(c, (method)omessage_key, "key", A_CANT, 0);
	class_addmethod(c, (method)omessage_keyfilter, "keyfilter", A_CANT, 0);
	class_addmethod(c, (method)omessage_enter, "enter", A_CANT, 0);
	class_addmethod(c, (method)omessage_select, "select", 0);

	class_addmethod(c, (method)omessage_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)omessage_mouseup, "mouseup", A_CANT, 0);


 	CLASS_ATTR_RGBA(c, "background_color", 0, t_omessage, background_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "background_color", 0, ".87 .87 .87 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "background_color", 0, "rgba", "Background Color"); 
	CLASS_ATTR_CATEGORY(c, "background_color", 0, "Color");

 	CLASS_ATTR_RGBA(c, "frame_color", 0, t_omessage, frame_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "frame_color", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "frame_color", 0, "rgba", "Frame Color"); 
	CLASS_ATTR_CATEGORY(c, "frame_color", 0, "Color");

 	CLASS_ATTR_RGBA(c, "text_color", 0, t_omessage, text_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "text_color", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "text_color", 0, "rgba", "Text Color"); 
	CLASS_ATTR_CATEGORY(c, "text_color", 0, "Color");

	CLASS_ATTR_DEFAULT(c, "rect", 0, "0. 0. 50., 18.");

	class_register(CLASS_BOX, c);
	omessage_class = c;

	return 0;
}

