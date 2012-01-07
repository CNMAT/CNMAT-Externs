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
NAME: o.expr
DESCRIPTION: C-like expressions that operate on OSC bundles
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2011
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
VERSION 1.0: Uses flex and bison to do the lexing/parsing
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "../odot_version.h"

#ifdef OIF
#undef NAME
#define NAME "o.if"
#endif

#ifndef WIN_VERSION
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
//#include "jpatcher_api.h" 
//#include "jgraphics.h"
#include "osc.h"
#include "osc_expr.h"
#include "osc_expr_parser.h"
#include "osc_mem.h"
#include "osc_atom_u.h"

//#define __OSC_PROFILE__
#include "osc_profile.h"

double rdtsc_cps;

typedef struct _oexpr{
	t_object ob;
#ifdef OIF
	void *outlets[2];
#else
	void *outlet;
#endif
	t_osc_expr *function_graph;
	//t_jrgba background_color, frame_color, text_color;
} t_oexpr;

void *oexpr_class;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr);
/*
void oexpr_set(t_oexpr *x, t_symbol *s, long ac, t_atom *av);
void oexpr_select(t_oexpr *x);
void oexpr_doselect(t_oexpr *x);
long oexpr_key(t_oexpr *x, t_object *patcherview, long keycode, long modifiers, long textcharacter);
long oexpr_keyfilter(t_oexpr *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter);
void oexpr_enter(t_oexpr *x);
void oexpr_gettext(t_oexpr *x);
*/
void oexpr_output_bundle(t_oexpr *x);
//void oexpr_mousedown(t_oexpr *x, t_object *patcherview, t_pt pt, long modifiers);
//void oexpr_mouseup(t_oexpr *x, t_object *patcherview, t_pt pt, long modifiers);

void oexpr_free(t_oexpr *x);
void oexpr_assist(t_oexpr *x, void *b, long m, long a, char *s);
void *oexpr_new(t_symbol *msg, short argc, t_atom *argv);

t_symbol *ps_FullPacket;

void oexpr_fullPacket(t_oexpr *x, long len, long ptr){
#ifdef OIF
	t_osc_atom_ar_u *argv = NULL;
	t_atom out[2];
	// we don't actually want to do this copy here.  we need to 
	// have another version of omax_expr_funcall that doesn't do 
	// assignment
	char *copy = (char *)osc_mem_alloc(len);
	memcpy(copy, (char *)ptr, len);
	int ret = osc_expr_funcall(x->function_graph, &len, &copy, &argv);
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)copy);
	if(ret){
		outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
	}else{
		int i;
		for(i = 0; i < osc_atom_array_u_getLen(argv); i++){
			if(osc_atom_u_getDouble(osc_atom_array_u_get(argv, i)) == 0){
				outlet_anything(x->outlets[1], ps_FullPacket, 2, out);
				goto out;
			}
		}
		outlet_anything(x->outlets[0], ps_FullPacket, 2, out);
	}
 out:
	if(argv){
		osc_mem_free(argv);
	}
	if(copy){
		free(copy);
	}
#else
	// we need to make a copy incase the expression contains assignment that will
	// alter the bundle.
	// the copy needs to use memory allocated with osc_mem_alloc in case the 
	// bundle has to be resized during assignment
	char *copy = (char *)osc_mem_alloc(len);
	memcpy(copy, (char *)ptr, len);
//int argc = 0;
	t_osc_atom_ar_u *argv = NULL;
	t_atom out[2];
	int ret = osc_expr_funcall(x->function_graph, &len, &copy, &argv);
	if(ret){
		atom_setlong(out, len);
		atom_setlong(out + 1, ptr);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}else{
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)copy);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
	if(argv){
		osc_atom_array_u_free(argv);
	}
	if(copy){
		osc_mem_free(copy);
	}
#endif
}
/*
void oexpr_paint(t_oexpr *x, t_object *patcherview){
	t_rect rect;
	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);

	jgraphics_set_source_jrgba(g, &(x->background_color));
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_fill(g);

	jgraphics_set_source_jrgba(g, &(x->frame_color));
	jgraphics_set_line_width(g, 2.);
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, 3, 0);
	jgraphics_line_to(g, 3, rect.height);
	jgraphics_stroke(g);
}

void oexpr_select(t_oexpr *x){
	defer(x, (method)oexpr_doselect, 0, 0, 0);
}

void oexpr_doselect(t_oexpr *x){
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


long oexpr_key(t_oexpr *x, t_object *patcherview, long keycode, long modifiers, long textcharacter){
	char buff[256];
	buff[0] = textcharacter;  // we know this is just a simple char
	buff[1] = 0; 
	object_method(patcherview, gensym("insertboxtext"), x, buff);
	jbox_redraw((t_jbox *)x);

	return 1; 
}

long oexpr_keyfilter(t_oexpr *x, t_object *patcherview, long *keycode, long *modifiers, long *textcharacter){
	t_atom arv;
	long rv = 1;
	long k = *keycode;
	
	if (k == JKEY_TAB || k == JKEY_ESC) {
		object_method_typed(patcherview, gensym("endeditbox"), 0, NULL, &arv); 
		rv = 0;		// don't pass those keys to oexpr
	}
	return rv;
}

// enter is triggerd at "endeditbox time"
void oexpr_enter(t_oexpr *x){
	oexpr_gettext(x);
}

void oexpr_gettext(t_oexpr *x){
	long size	= 0;
	char *text	= NULL;
	t_object *textfield = jbox_get_textfield((t_object *)x);
	object_method(textfield, gensym("gettextptr"), &text, &size);
	{
		size = strlen(text); // the value returned in text doesn't make sense
		if(size == 0){
			return;
		}
		t_osc_expr *f = NULL;
		osc_expr_parser_parseString(text, &f);
		if(!f){
			object_error((t_object *)x, "error parsing %s\n", text);
			return;
		}
		if(x->function_graph){
			osc_expr_free(x->function_graph);
		}
		x->function_graph = f;
	}
}

void oexpr_mousedown(t_oexpr *x, t_object *patcherview, t_pt pt, long modifiers){
        //textfield_set_textmargins(jbox_get_textfield((t_object *)x), 4, 4, 2, 2);
	//jbox_redraw((t_jbox *)x);
}

void oexpr_mouseup(t_oexpr *x, t_object *patcherview, t_pt pt, long modifiers){
        //textfield_set_textmargins(jbox_get_textfield((t_object *)x), 3, 3, 3, 3);
	//jbox_redraw((t_jbox *)x);
	//oexpr_output_bundle(x);
}
*/
void oexpr_postFunctionGraph(t_oexpr *fg){
	char *buf = NULL;
	long len = 0;
	t_osc_expr *f = fg->function_graph;
	//while(f){
	osc_expr_formatFunctionGraph(f, &len, &buf);
	// the modulo op '%' gets consumed as a format character with cycling's post() function.
	// so go through and escape each one with another %
	char buf2[len * 2];
	char *r = buf, *w = buf2;
	int i;
	for(i = 0; i < len; i++){
		if(*r == '%'){
			*w++ = '%';
		}
		*w++ = *r++;
	}
	*w = '\0';
	post("%s", buf2);
	//f = osc_expr_next(f);
	//
	if(buf){
		osc_mem_free(buf);
	}
}

void oexpr_postFunctionTable(t_oexpr *fg){
	char *buf = NULL;
	long len = 0;
	osc_expr_formatFunctionTable(&len, &buf);
	char *ptr1 = buf, *ptr2 = buf;
	int i = 0;
	while(*ptr2){
		if(*ptr2 == '\n'){
			*ptr2 = '\0';
			post("%s", ptr1);
			ptr1 = ptr2 + 1;
			ptr2++;
		}
		ptr2++;
	}
	if(buf){
		osc_mem_free(buf);
	}
}

void oexpr_bang(t_oexpr *x){
	char buf[16];
	strncpy(buf, "#bundle\0", 8);
	oexpr_fullPacket(x, 16, (long)buf);
}

void oexpr_anything(t_oexpr *x, t_symbol *msg, int argc, t_atom *argv){
	object_error((t_object *)x, "nope");
}

void oexpr_postConstants(t_oexpr *x){
	int i;
	for(i = 0; i < sizeof(osc_expr_constsym) / sizeof(t_osc_expr_const_rec); i++){
		post("%s: %s (%f)", osc_expr_constsym[i].name, osc_expr_constsym[i].docstring, osc_expr_constsym[i].val);
	}
}

void oexpr_postFunctions(t_oexpr *x){
	int i;
	for(i = 0; i < sizeof(osc_expr_funcsym) / sizeof(t_osc_expr_rec); i++){
		if(osc_expr_funcsym[i].arity < 0){
			post("%s(): %s", osc_expr_funcsym[i].name, osc_expr_funcsym[i].docstring);
		}else if(osc_expr_funcsym[i].arity == 0){
			post("%s(...): %s", osc_expr_funcsym[i].name, osc_expr_funcsym[i].docstring);
		}else{
			char buf[256];
			char *ptr = buf;
			ptr += sprintf(ptr, "%s(", osc_expr_funcsym[i].name);
			int j;
			for(j = 0; j < osc_expr_funcsym[i].arity; j++){
				ptr += sprintf(ptr, "arg%d ", j + 1);
			}
			*(--ptr) = '\0';
			post("%s): %s", buf, osc_expr_funcsym[i].docstring);
		}
	}
}

void oexpr_documentation(t_oexpr *x, t_symbol *func)
{
	t_osc_expr_rec *rec = osc_expr_lookupFunction(func->s_name);
	if(rec){
		post("%s(): %s", rec->name, rec->docstring);
	}
}

void oexpr_assist(t_oexpr *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
	}else{
		switch (a) {	
		case 0:
			break;
		}
	}
}

void oexpr_free(t_oexpr *x){
	//jbox_free((t_jbox *)x);
	osc_expr_free(x->function_graph);
}

t_max_err oexpr_notify(t_oexpr *x, t_symbol *s, t_symbol *msg, void *sender, void *data){
	return MAX_ERR_NONE;
	t_symbol *attrname;

        if(msg == gensym("attr_modified")){
		attrname = (t_symbol *)object_method((t_object *)data, gensym("getname"));
	}
	return MAX_ERR_NONE;
}

void *oexpr_new(t_symbol *msg, short argc, t_atom *argv){
	t_oexpr *x;
	/*
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
	*/
	if(x = (t_oexpr *)object_alloc(oexpr_class)){
		//jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		//x->ob.b_firstin = (void *)x; 
#ifdef OIF
		x->outlets[1] = outlet_new((t_object *)x, "FullPacket");
		x->outlets[0] = outlet_new((t_object *)x, "FullPacket");
#else
		x->outlet = outlet_new((t_object *)x, "FullPacket");
#endif
		if(argc){
			char buf[65536];
			char *ptr = buf;
			int i;
			for(i = 0; i < argc; i++){
				switch(atom_gettype(argv + i)){
				case A_LONG:
					ptr += sprintf(ptr, "%ld ", atom_getlong(argv + i));
					break;
				case A_FLOAT:
					ptr += sprintf(ptr, "%f ", atom_getfloat(argv + i));
					break;
				case A_SYM:
					ptr += sprintf(ptr, "%s ", atom_getsym(argv + i)->s_name);
					break;
				}
			}
			t_osc_expr *f = NULL;
			TIMER_START(foo, rdtsc_cps);
			osc_expr_parser_parseString(buf, &f);
			TIMER_STOP(foo, rdtsc_cps);
			TIMER_PRINTF(foo);
			TIMER_SNPRINTF(foo, buff);
#ifdef __OSC_PROFILE__
			post("%s\n", buff);
#endif
			if(!f){
				object_error((t_object *)x, "error parsing %s\n", buf);
				return NULL;
			}
			if(x->function_graph){
				osc_expr_free(x->function_graph);
			}
			x->function_graph = f;
		}
		/*
		if(argc){
			omax_scanner_scan_atom_array(attr_args_offset(argc, argv), argv, &(x->argclex), &(x->argvlex));
			x->function_graph = NULL;
			omax_expr_parse(&(x->function_graph), x->argclex, x->argvlex);
		}
		*/

		//attr_dictionary_process(x, d); 
		/*
		t_object *textfield = jbox_get_textfield((t_object *)x);
		if(textfield){
			object_attr_setchar(textfield, gensym("editwhenunlocked"), 1);
			textfield_set_editonclick(textfield, 0);
			textfield_set_textmargins(textfield, 7, 3, 3, 3);
			textfield_set_textcolor(textfield, &(x->text_color));
		}

 		jbox_ready((t_jbox *)x);
		oexpr_gettext(x);
		*/
	}
		   	
	return x;
}

int main(void){
#ifdef OIF
	char *buf = "o.if";
#else
	char *buf = "o.expr";
#endif
	t_class *c = class_new(buf, (method)oexpr_new, (method)oexpr_free, sizeof(t_oexpr), 0L, A_GIMME, 0);

	//c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	//jbox_initclass(c, JBOX_TEXTFIELD | JBOX_FIXWIDTH | JBOX_FONTATTR); 
    
	//class_addmethod(c, (method)oexpr_paint, "paint", A_CANT, 0); 

	//osc_set_mem((void *)sysmem_newptr, sysmem_freeptr, (void *)sysmem_resizeptr);
	class_addmethod(c, (method)oexpr_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)oexpr_assist, "assist", A_CANT, 0);
	//class_addmethod(c, (method)oexpr_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oexpr_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)oexpr_bang, "bang", 0);

	class_addmethod(c, (method)oexpr_postFunctions, "post-functions", 0);
	class_addmethod(c, (method)oexpr_postConstants, "post-constants", 0);
	class_addmethod(c, (method)oexpr_postFunctionGraph, "post-function-graph", 0);
	class_addmethod(c, (method)oexpr_postFunctionTable, "post-function-table", 0);
	class_addmethod(c, (method)oexpr_documentation, "documentation", A_SYM, 0);
	/*
	class_addmethod(c, (method)oexpr_key, "key", A_CANT, 0);
	class_addmethod(c, (method)oexpr_keyfilter, "keyfilter", A_CANT, 0);
	class_addmethod(c, (method)oexpr_enter, "enter", A_CANT, 0);
	class_addmethod(c, (method)oexpr_select, "select", 0);

	class_addmethod(c, (method)oexpr_mousedown, "mousedown", A_CANT, 0);
	class_addmethod(c, (method)oexpr_mouseup, "mouseup", A_CANT, 0);


 	CLASS_ATTR_RGBA(c, "background_color", 0, t_oexpr, background_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "background_color", 0, ".87 .87 .87 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "background_color", 0, "rgba", "Background Color"); 
	CLASS_ATTR_CATEGORY(c, "background_color", 0, "Color");

 	CLASS_ATTR_RGBA(c, "frame_color", 0, t_oexpr, frame_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "frame_color", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "frame_color", 0, "rgba", "Frame Color"); 
	CLASS_ATTR_CATEGORY(c, "frame_color", 0, "Color");

 	CLASS_ATTR_RGBA(c, "text_color", 0, t_oexpr, text_color); 
 	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "text_color", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "text_color", 0, "rgba", "Text Color"); 
	CLASS_ATTR_CATEGORY(c, "text_color", 0, "Color");

	CLASS_ATTR_DEFAULT(c, "rect", 0, "0. 0. 150., 18.");
	*/
	class_register(CLASS_BOX, c);
	oexpr_class = c;

	common_symbols_init();
	ps_FullPacket = gensym("FullPacket");

	rdtsc_cps = RDTSC_CYCLES_PER_SECOND;

	ODOT_PRINT_VERSION;

	return 0;
}

