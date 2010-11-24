/*
  Written by John MacCallum, The Center for New Music and Audio Technologies, 
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
  NAME: Maquette
  DESCRIPTION: Like the maquette object from OpenMusic (only lame)
  AUTHORS: John MacCallum 
  COPYRIGHT_YEARS: 2010
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
#include "osc_util.h"
#include "osc_match.h"

#define PF __PRETTY_FUNCTION__

#ifndef FLT_MAX
#define FLT_MAX 999999999.f
#endif

// to select the selection box
#define LEFT_EDGE 1
#define RIGHT_EDGE 2
#define TOP_EDGE 4
#define BOTTOM_EDGE 8
#define TOP_LEFT_CORNER LEFT_EDGE | TOP_EDGE
#define TOP_RIGHT_CORNER RIGHT_EDGE | TOP_EDGE
#define BOTTOM_LEFT_CORNER LEFT_EDGE | BOTTOM_EDGE
#define BOTTOM_RIGHT_CORNER RIGHT_EDGE | BOTTOM_EDGE
#define BOX_SELECTED 16

#ifdef WIN
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

typedef struct _event{ 
	t_rect rect;
	int selected;
 	struct _event *next; 
 	struct _event *prev; 
 	struct _event *next_selected; 
 	struct _event *prev_selected; 
} t_event; 

typedef struct _maq{ 
 	t_jbox box; 
 	void *outlet_main, *outlet_info;
	long inlet;
	void *proxy;
	t_event *eventlist;
	t_event *selected;
	t_object *pv;
	t_critical lock;
 	t_jrgba bgcolor, boxcolor; 
	double xmin, xmax, ymin, ymax;
	t_rect sel_box;
	t_pt drag;
	clock_t dblclick;
} t_maq; 

void *maq_class; 

void maq_paint(t_maq *x, t_object *patcherview); 
t_event *maq_select(t_maq *x, float time, float midic);
void maq_assist(t_maq *x, void *b, long m, long a, char *s); 
void maq_free(t_maq *x); 
t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_clear(t_maq *x);
double maq_scale(double f, double min_in, double max_in, double min_out, double max_out);
double maq_clip(double f, double min, double max);
void maq_dump(t_maq *x);
void maq_dumpcellblock(t_maq *x);
void maq_xminmax(t_maq *x, double min, double max);
void maq_yminmax(t_maq *x, double min, double max);
int main(void); 
void *maq_new(t_symbol *s, long argc, t_atom *argv); 
t_max_err maq_events_get(t_maq *x, t_object *attr, long *argc, t_atom **argv);
t_max_err maq_events_set(t_maq *x, t_object *attr, long argc, t_atom *argv);

static t_symbol *ps_done, *ps_dump, *ps_clear, *ps_cellblock;
static t_atom pa_dump, pa_clear, pa_all;

void maq_paint(t_maq *x, t_object *patcherview){ 
	x->pv = patcherview;
 	t_rect r; 
 	t_jrgba c = (t_jrgba){0., 0., 0., 1.};

 	// get graphics context 
 	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 

 	// get our box's rectangle 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &r); 

	// draw the outline of the box 
	jgraphics_set_source_jrgba(g, &c); 
	jgraphics_set_line_width(g, 1); 
	jgraphics_rectangle(g, 0., 0., r.width, r.height); 
	jgraphics_stroke(g); 

	jgraphics_set_source_jrgba(g, &(x->bgcolor)); 
	jgraphics_rectangle(g, 0., 0., r.width, r.height); 
	jgraphics_fill(g); 


	jgraphics_set_source_jrgba(g, &x->boxcolor);
	t_event *e = x->eventlist;
	while(e){
		jgraphics_rectangle(g, maq_scale(e->rect.x, x->xmin, x->xmax, 0, r.width),
				    maq_scale(e->rect.y, x->ymin, x->ymax, r.height, 0),
				    maq_scale(e->rect.width, x->xmin, x->xmax, 0, r.width),
				    maq_scale(e->rect.height, x->ymin, x->ymax, r.height, 0));
		jgraphics_fill(g);
		e = e->next;
	}
}

t_event *maq_select(t_maq *x, float time, float midic){
	return NULL;
}

t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		//t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		jbox_redraw((t_jbox *)&(x->box)); 
	} 
	return 0; 
} 

unsigned long maq_selectBox(t_maq *x, t_pt pt){
	t_rect sel_box = x->sel_box;
	unsigned long sel_box_selected = 0;
	// the width could be 0--we don't want to select both the right and left edges, so do the
	// right edge first
	if(fabs(pt.x - (sel_box.x + sel_box.width)) < 4. && pt.y >= sel_box.y && pt.y <= (sel_box.y + sel_box.height)){
		sel_box_selected |= RIGHT_EDGE;
	}else if(fabs(pt.x - sel_box.x) < 4. && pt.y >= sel_box.y && pt.y <= (sel_box.y + sel_box.height)){
		sel_box_selected |= LEFT_EDGE;
	}
	if(fabs(pt.y - (sel_box.y + sel_box.height)) < 4. && pt.x >= sel_box.x && pt.x <= (sel_box.x + sel_box.width)){
		sel_box_selected |= BOTTOM_EDGE;
	}else if(fabs(pt.y - sel_box.y) < 4. && pt.x >= sel_box.x && pt.x <= (sel_box.x + sel_box.width)){
		sel_box_selected |= TOP_EDGE;
	}
	if(sel_box_selected == 0){
		if(pt.x < (sel_box.x + sel_box.width) && pt.x > sel_box.x && pt.y > sel_box.y && pt.y < (sel_box.y + sel_box.height)){
			sel_box_selected = BOX_SELECTED;
		}
	}
	return sel_box_selected;
}

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){ 
	int dblclick = 0;
	if(((double)clock() - x->dblclick) / CLOCKS_PER_SEC < .2){
		dblclick = 1;
	}else{
		x->dblclick = clock();
	}
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	switch(modifiers){
	case 0x10:
	case 0x110:
		{
			t_event *e = (t_event *)sysmem_newptr(sizeof(t_event));
			e->rect = (t_rect){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax),
					   maq_scale(pt.y, r.height, 0, x->ymin, x->ymax),
					   maq_scale(10, 0, r.width, x->xmin, x->xmax),
					   maq_scale(10, r.height, 0, x->ymin, x->ymax)};
			e->next = x->eventlist;
			if(x->eventlist){
				x->eventlist->prev = e;
			}
			e->prev = NULL;
			x->eventlist = e;
		}
		break;
	case 0x11:
	case 0x111:
		break;
	}
 out:
	maq_dumpcellblock(x);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	switch(modifiers){
	}
	maq_dumpcellblock(x);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
}

double maq_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double maq_clip(double f, double min, double max){
	if(f < min){
		f = min;
	}
	if(f > max){
		f = max;
	}
	return f;
}

void maq_dump(t_maq *x){
}

void maq_dumpcellblock(t_maq *x){
}

void maq_xminmax(t_maq *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_yminmax(t_maq *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_clear(t_maq *x){
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_assist(t_maq *x, void *b, long io, long num, char *s){ 
 	if(io == ASSIST_OUTLET){ 

 	}else{ 

 	} 
} 

void maq_free(t_maq *x){ 
} 

int main(void){ 
 	t_class *c = class_new("maquette", (method)maq_new, (method)maq_free, sizeof(t_maq), 0L, A_GIMME, 0); 
	//class_dspinitjbox(c);

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

 	class_addmethod(c, (method)maq_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)maq_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)maq_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)maq_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)maq_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)maq_mouseup, "mouseup", A_CANT, 0); 
	class_addmethod(c, (method)maq_clear, "clear", 0);
	class_addmethod(c, (method)maq_dump, "dump", 0);
	class_addmethod(c, (method)maq_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)maq_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_maq, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_maq, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_maq, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_maq, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_maq, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "boxcolor", 0, t_maq, boxcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "boxcolor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "boxcolor", 0, "rgba", "Box Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

	ps_done = gensym("done");
	ps_dump = gensym("dump");
	ps_cellblock = gensym("cellblock");
	ps_clear = gensym("clear");
	atom_setsym(&pa_clear, ps_clear);
	atom_setsym(&pa_dump, ps_dump);
	atom_setsym(&pa_all, gensym("all"));
 	class_register(CLASS_BOX, c); 
 	maq_class = c; 

 	version(0); 
	
 	return 0; 
} 

void *maq_new(t_symbol *s, long argc, t_atom *argv){ 
 	t_maq *x = NULL; 
 	t_dictionary *d = NULL; 
 	long boxflags; 

	// box setup 
	if(!(d = object_dictionaryarg(argc, argv))){ 
		return NULL; 
	} 

	boxflags = 0 
		| JBOX_DRAWFIRSTIN  
		//| JBOX_NODRAWBOX 
		| JBOX_DRAWINLAST 
		//| JBOX_TRANSPARENT   
		//      | JBOX_NOGROW 
		//| JBOX_GROWY 
		| JBOX_GROWBOTH 
		//      | JBOX_HILITE 
		| JBOX_BACKGROUND 
		| JBOX_DRAWBACKGROUND 
		//      | JBOX_NOFLOATINSPECTOR 
		//      | JBOX_MOUSEDRAGDELTA 
		//      | JBOX_TEXTFIELD 
		; 

 	if(x = (t_maq *)object_alloc(maq_class)){ 
 		jbox_new((t_jbox *)x, boxflags, argc, argv); 

 		x->box.b_firstin = (void *)x; 
		//dsp_setupjbox((t_pxjbox *)x, 1);

		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));

		x->outlet_info = outlet_new((t_object *)x, NULL);
		x->outlet_main = outlet_new((t_object *)x, NULL);

		x->selected = NULL;
		x->eventlist = NULL;

		critical_new(&(x->lock));
 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
		//x->box.z_misc = Z_PUT_FIRST;

 		return x; 
 	} 
 	return NULL; 
} 


t_max_err maq_events_get(t_maq *x, t_object *attr, long *argc, t_atom **argv){
	return MAX_ERR_NONE;
}

t_max_err maq_events_set(t_maq *x, t_object *attr, long argc, t_atom *argv){
	return MAX_ERR_NONE;
}
