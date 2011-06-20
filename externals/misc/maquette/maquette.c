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
  VERSION 0.1: OSC bundle stuff is better--easier to add contents to a bundle
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
#include "osc.h"
#include "omax_util.h"

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
/*
typedef struct _maq_osc_msg{
	t_symbol *address;
	int argc;
	t_atom *argv;
	int buflen;
	struct _maq_osc_msg *next;
	struct _maq_osc_msg *prev;
} t_maq_osc_msg;
*/
#define MOVE 1
#define RESIZE 2
#define CLONE 4
typedef struct _event{ 
	t_rect rect;
	t_symbol *name;
	long selected;
	long op; // move, resize, clone
	long mouse_over_bottom_right_corner;
	//t_maq_osc_msg *messages_ll;
	char *oscbndl;
	int oscbndl_size;
	int oscbndl_len;
	//t_hashtab *messages_ht;
	char *slot;
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
	t_pt lastmouse;
	clock_t dblclick;
	int takingdump;
	int continuous_output;
	//int eventcounter;
	char *slots;
	int nslots;
	t_rect selbox;
} t_maq; 

void *maq_class; 

void maq_paint(t_maq *x, t_object *patcherview); 
void maq_list(t_maq *x, t_symbol *msg, int argc, t_atom *argv);
void maq_add_to_bundle(t_maq *x, t_symbol *address, t_symbol *FullPacket, long len, long ptr);
void maq_clear_bundle(t_maq *x, t_symbol *address);
void maq_assist(t_maq *x, void *b, long m, long a, char *s); 
void maq_free(t_maq *x); 
//t_maq_osc_msg *maq_osc_msg_alloc(t_symbol *address, long argc, t_atom *argv);
//void maq_osc_msg_free(t_maq_osc_msg *m);
//void maq_osc_msg_freelist(t_maq_osc_msg *m);
void maq_clear_osc_messages(t_event *e);
t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void maq_delta(t_maq *x, t_rect r, t_pt pt, t_pt *delta, t_pt *deltawc);
void maq_addevent(t_maq *x, t_pt wc);
void maq_delete_selected(t_maq *x);
void maq_remove_event(t_event *e);
t_event *maq_event_alloc(t_maq *x);
int maq_get_slot(t_maq *x);
void maq_event_free(t_event *e);
t_event *maq_event_clone(t_maq *x, t_event *e);
void maq_make_bundle_from_event(t_event *e, int *len, char **ret_ptr);
void maq_output_osc(t_maq *x, void *outlet, t_symbol *msg, t_event *e);
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mousemove(t_maq *x, t_object *patcherview, t_pt pt, long modifiers);
void maq_dblclick(t_maq *x);
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

static t_symbol *l_main;
static t_symbol *ps_done, *ps_dump, *ps_clear, *ps_cellblock, *ps_oscname, *ps_FullPacket;
static t_atom pa_dump, pa_clear, pa_all;

static t_symbol *ps_maquette_focus;

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

	t_jgraphics *gg = jbox_start_layer((t_object *)x, patcherview, l_main, r.width, r.height);
	if(gg){
		t_event *e = x->eventlist;
		if(e){
			while(e->next){
				e = e->next;
			}
		}
		while(e){
			double lw = 1.;
			if(e->selected){
				lw = 2.5;
			}
			jgraphics_set_source_jrgba(gg, &x->boxcolor);

			t_rect sr = (t_rect){maq_scale(e->rect.x, x->xmin, x->xmax, 0, r.width),
					     maq_scale(e->rect.y, x->ymin, x->ymax, r.height, 0),
					     maq_scale(e->rect.width, x->xmin, x->xmax, 0, r.width),
					     maq_scale(e->rect.height, x->ymin, x->ymax, 0, r.height)};
			//jgraphics_rectangle_rounded(gg, sr.x, sr.y, sr.width, sr.height, rounded, rounded);
			jgraphics_rectangle(gg, sr.x, sr.y, sr.width, sr.height);
			jgraphics_fill(gg);
			jgraphics_set_line_width(gg, lw);
			jgraphics_set_source_jrgba(gg, &c);
			//jgraphics_rectangle_rounded(gg, sr.x, sr.y, sr.width, sr.height, rounded, rounded);
			jgraphics_rectangle(gg, sr.x, sr.y, sr.width, sr.height);
			jgraphics_stroke(gg);
			jgraphics_set_line_width(gg, .5);
			if(e->mouse_over_bottom_right_corner){
				jgraphics_rectangle(gg, sr.x + sr.width - 5, sr.y + sr.height - 5, 10, 10);
				jgraphics_stroke(gg);
			}
			if(e->name){
				double w, h;
				jgraphics_text_measure(gg, e->name->s_name, &w, &h);
				jgraphics_move_to(gg, sr.x + (sr.width / 2) - (w / 2), sr.y + (sr.height / 2.) + (h / 2));
				jgraphics_show_text(gg, e->name->s_name);
			}
			e = e->prev;
		}

		if(x->selbox.width != 0 && x->selbox.height != 0){
			jgraphics_rectangle(gg, maq_scale(x->selbox.x, x->xmin, x->xmax, 0, r.width),
					    maq_scale(x->selbox.y, x->ymin, x->ymax, r.height, 0),
					    maq_scale(x->selbox.width, x->xmin, x->xmax, 0, r.width),
					    maq_scale(x->selbox.height, x->ymin, x->ymax, 0, r.height));
			jgraphics_stroke(gg);
		}
	}
	jbox_end_layer((t_object *)x, patcherview, l_main);
	jbox_paint_layer((t_object *)x, patcherview, l_main, 0, 0);
}

void maq_outputCellblockCell_1float(void *outlet, long col, long row, char *address, float val){
	t_atom out[5];
	t_atom *outptr = out;
	atom_setsym(outptr++, gensym("set"));
	atom_setlong(outptr++, col);
	atom_setlong(outptr++, row);
	atom_setsym(outptr++, gensym(address));
	atom_setfloat(outptr++, val);
	outlet_anything(outlet, ps_cellblock, outptr - out, out);
}
/*
void maq_dumpcellblock(t_maq *x){
	t_atom out[5];
	t_symbol *ps_cellblock = gensym("cellblock");
	t_symbol *ps_clear = gensym("clear");
	t_symbol *ps_all = gensym("all");
	atom_setsym(out, ps_clear);
	atom_setsym(out + 1, ps_all);
	outlet_anything(x->outlet_info, ps_cellblock, 2, out);

	if(!x->selected){
		return;
	}

	int row = 0;
	t_atom *outptr = out;

	char buf[64];
	sprintf(buf, "%s/x/min", x->selected->name->s_name);
	maq_outputCellblockCell_1float(x->outlet_info, 0, row++, buf, x->selected->rect.x);
	sprintf(buf, "%s/x/width", x->selected->name->s_name);
	maq_outputCellblockCell_1float(x->outlet_info, 0, row++, buf, x->selected->rect.width);
	sprintf(buf, "%s/y/min", x->selected->name->s_name);
	maq_outputCellblockCell_1float(x->outlet_info, 0, row++, buf, x->selected->rect.y - x->selected->rect.height);
	sprintf(buf, "%s/y/height", x->selected->name->s_name);
	maq_outputCellblockCell_1float(x->outlet_info, 0, row++, buf, x->selected->rect.height);

	t_maq_osc_msg *m = x->selected->messages_ll;
	while(m){
		t_atom mout[m->argc + 4];
		outptr = mout;
		atom_setsym(outptr++, gensym("set"));
		atom_setlong(outptr++, 0);
		atom_setlong(outptr++, row++);
		atom_setsym(outptr++, m->address);
		int i;
		for(i = 0; i < m->argc; i++){
			*outptr++ = m->argv[0];
		}
		outlet_anything(x->outlet_info, ps_cellblock, m->argc + 4, mout);
		m = m->next;
	}
}
*/
void maq_int(t_maq *x, long l){
	switch(proxy_getinlet((t_object *)x)){
	case 0:

		break;
	case 1:
		// key
		if(ps_maquette_focus->s_thing == (t_object *)x){
			switch(l){
			case 8: // delete
				maq_delete_selected(x);
			}
		}
		break;
	}
}

void maq_list(t_maq *x, t_symbol *msg, int argc, t_atom *argv){
	/*
	switch(proxy_getinlet((t_object *)x)){
	case 0:

		break;
	case 1:
		// cellblock
		if(x->takingdump == 1){
			t_symbol *address = atom_getsym(argv + 2);
			char buf[64];

			sprintf(buf, "%s/x/min", x->selected->name->s_name);
			if(!strcmp(buf, address->s_name)){
				x->selected->rect.x = atom_getfloat(argv + 3);
				break;
			}
			sprintf(buf, "%s/x/width", x->selected->name->s_name);
			if(!strcmp(buf, address->s_name)){
				x->selected->rect.width = atom_getfloat(argv + 3);
				break;
			}
			sprintf(buf, "%s/y/min", x->selected->name->s_name);
			if(!strcmp(buf, address->s_name)){
				x->selected->rect.y = atom_getfloat(argv + 3);
				break;
			}
			sprintf(buf, "%s/y/height", x->selected->name->s_name);
			if(!strcmp(buf, address->s_name)){
				x->selected->rect.height = atom_getfloat(argv + 3);
				break;
			}
			if(strncmp(x->selected->name->s_name, address->s_name, strlen(x->selected->name->s_name))){
				sprintf(buf, "%s%s", x->selected->name->s_name, address->s_name);
				address = gensym(buf);
				break;
			}
			t_maq_osc_msg *m = maq_osc_msg_alloc(address, argc - 3, argv + 3);
			m->next = x->selected->messages_ll;
			if(x->selected->messages_ll){
				x->selected->messages_ll->prev = m;
			}
			x->selected->messages_ll = m;
			hashtab_store(x->selected->messages_ht, address, (t_object *)m);
			m->prev = NULL;
		}else{
			x->takingdump = 1;
			if(x->selected){
				maq_clear_osc_messages(x->selected);
			}
			t_atom out;
			atom_setsym(&out, ps_dump);
			outlet_anything(x->outlet_info, ps_cellblock, 1, &out);
			x->takingdump = 0;
			if(x->selected){
				x->selected->rect.y += x->selected->rect.height;
			}
			maq_dumpcellblock(x);
			jbox_invalidate_layer((t_object *)x, x->pv, l_main);
			jbox_redraw((t_jbox *)&(x->box)); 
		}
		break;
	}
	*/
}
/*
void maq_add_to_bundle(t_maq *x, t_symbol *address, t_symbol *FullPacket, long len, long ptr){
	t_event *e = x->eventlist;
	while(e){
		if(e->name == address){
			break;
		}
		e = e->next;
	}
	if(!e){
		object_error((t_object *)x, "no bundle named %s", address->s_name);
		return;
	}
	char *buf = (char *)ptr;
	int msgcount = 0;
	osc_bundle_getMsgCount(len, buf, &msgcount);
	t_osc_msg *msg = NULL;
	osc_bundle_getMessages(len, buf, &msgcount, &msg);
	while(msg){
		long ac = osc_message_getArgCount(msg);
		t_atom av[ac];		
		omax_util_oscMsg2MaxAtoms(msg, &ac, av);
		t_maq_osc_msg *m = NULL;
		hashtab_lookup(e->messages_ht, atom_getsym(av), (t_object **)(&m));
		if(m){
			if(m->argc < ac - 1){
				m->argv = (t_atom *)sysmem_resizeptr(m->argv, sizeof(t_atom) * ac - 1);
			}
			memcpy(m->argv, av + 1, sizeof(t_atom) * (ac - 1));
			m->argc = ac - 1;
		}else{
			m = maq_osc_msg_alloc(atom_getsym(av), ac - 1, av + 1);
			memcpy(m->argv, av + 1, sizeof(t_atom) * (ac - 1));
			m->argc = ac - 1;
			m->next = e->messages_ll;
			if(e->messages_ll){
				e->messages_ll->prev = m;
			}
			m->prev = NULL;
			e->messages_ll = m;
			hashtab_store(e->messages_ht, atom_getsym(av), (t_object *)m);
		}
		msg = msg->next;
	}
	maq_dumpcellblock(x);
}
*/
void maq_clear_bundle(t_maq *x, t_symbol *address){
	/*
	t_event *e = x->eventlist;
	while(e){
		if(e->name == address){
			break;
		}
		e = e->next;
	}
	if(!e){
		object_error((t_object *)x, "no bundle named %s", address->s_name);
		return;
	}
	maq_osc_msg_freelist(e->messages_ll);
	e->messages_ll = NULL;
	hashtab_clear(e->messages_ht);
	maq_dumpcellblock(x);
	maq_output_osc(x, x->outlet_main, NULL, e);
	*/
}
/*
t_maq_osc_msg *maq_osc_msg_alloc(t_symbol *address, long argc, t_atom *argv){
	t_maq_osc_msg *m = (t_maq_osc_msg *)sysmem_newptr(sizeof(t_maq_osc_msg));
	m->address = address;
	m->argc = argc;
	m->argv = (t_atom *)sysmem_newptr(argc * sizeof(t_atom));
	memcpy(m->argv, argv, argc * sizeof(t_atom));
	m->next = m->prev = NULL;
	return m;
}

void maq_osc_msg_free(t_maq_osc_msg *m){
	if(m){
		if(m->argv){
			sysmem_freeptr(m->argv);
		}
		sysmem_freeptr(m);
	}
}

void maq_osc_msg_freelist(t_maq_osc_msg *m){
	t_maq_osc_msg *next = NULL;
	while(m){
		next = m->next;
		maq_osc_msg_free(m);
		m = next;
	}
}
*/
void maq_clear_osc_messages(t_event *e){
	/*
	maq_osc_msg_freelist(e->messages_ll);
	e->messages_ll = NULL;
	hashtab_clear(e->messages_ht);
	*/
}

t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		//t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		jbox_invalidate_layer((t_object *)x, x->pv, l_main);
 		jbox_redraw((t_jbox *)&(x->box)); 
	} 
	return 0; 
}

void maq_delta(t_maq *x, t_rect r, t_pt pt, t_pt *delta, t_pt *deltawc){
	delta->x = x->lastmouse.x - pt.x;
	delta->y = x->lastmouse.y - pt.y;
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax), maq_scale(pt.y, r.height, 0, x->ymin, x->ymax)};
	t_pt lmwc = (t_pt){maq_scale(x->lastmouse.x, 0, r.width, x->xmin, x->xmax), 
			   maq_scale(x->lastmouse.y, r.height, 0, x->ymin, x->ymax)};
	deltawc->x = lmwc.x - wc.x;
	deltawc->y = lmwc.y - wc.y;
}

void maq_unselect(t_maq *x){
	t_event *e = x->eventlist;
	x->selected = NULL;

	while(e){
		if(e->selected){
			e->selected = 0;
		}
		e->op = 0;
		e = e->next;
	}
}

void maq_addevent(t_maq *x, t_pt wc){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, x->pv, &r);
	t_event *e = maq_event_alloc(x);
	e->rect.x = wc.x;
	e->rect.y = wc.y;
	e->rect.width = maq_scale(50, 0, r.width, x->xmin, x->xmax);
	e->rect.height = maq_scale(30, 0, r.height, x->ymin, x->ymax);
	//post("add %f %f %f %f", e->rect.x, e->rect.y, e->rect.width, e->rect.height);
	e->next = x->eventlist;
	if(x->eventlist){
		x->eventlist->prev = e;
	}
	x->eventlist = e;
	x->eventlist->selected = 1;
	x->selected = e;
}

void maq_delete_selected(t_maq *x){
	t_event *e = x->selected, *next = NULL;
	while(e){
		next = e->next_selected;
		if(e == x->eventlist){
			x->eventlist = e->next;
		}
		maq_remove_event(e);
		*(e->slot) = 0;
		maq_event_free(e);
		e = next;
	}
	x->selected = NULL;
	maq_dumpcellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_remove_event(t_event *e){
	if(!e){
		return;
	}
	if(e->next){
		e->next->prev = e->prev;
	}
	if(e->prev){
		e->prev->next = e->next;
	}
}

t_event *maq_event_alloc(t_maq *x){
	t_event *e = (t_event *)sysmem_newptr(sizeof(t_event));
	e->selected = 0;
	e->op = 0;
	e->mouse_over_bottom_right_corner = 0;
	e->next = e->prev = e->next_selected = e->prev_selected = NULL;
	//e->messages_ll = NULL;
	//e->messages_ht = hashtab_new(0);
	hashtab_flags(e->messages_ht, OBJ_FLAG_DATA);
	char buf[32];
	int slot = maq_get_slot(x);
	e->slot = x->slots + slot;
	sprintf(buf, "/maquette/%d", slot);
	e->name = gensym(buf);
	return e;
}

int maq_get_slot(t_maq *x){
	int i;
	for(i = 0; i < x->nslots; i++){
		if(x->slots[i] == 0){
			x->slots[i] = 1;
			return i;
		}
	}
	x->slots = (char *)sysmem_resizeptr(x->slots, x->nslots * 2);
	x->nslots *= 2;
	return (x->nslots / 2);
}

void maq_event_free(t_event *e){
	maq_clear_osc_messages(e);
	sysmem_freeptr(e);
}

t_event *maq_event_clone(t_maq *x, t_event *e){
	t_event *c = maq_event_alloc(x);
	c->rect = e->rect;
	t_maq_osc_msg *m = e->messages_ll;
	if(m){
		while(m->next){
			m = m->next;
		}
		while(m){
			t_maq_osc_msg *mm = maq_osc_msg_alloc(m->address, m->argc, m->argv);
			mm->next = c->messages_ll;
			//mm->prev = NULL;
			if(c->messages_ll){
				c->messages_ll->prev = mm;
			}
			c->messages_ll = mm;
			hashtab_store(c->messages_ht, mm->address, (t_object *)mm);
			m = m->prev;
		}
	}
	return c;
}

inline long maq_addMessageToBundle_1float(char *bndl, char *address, float f){
	char *ptr = bndl;
	char *sizeptr = ptr;
	ptr += 4;
	strcpy(ptr, address);
	ptr += strlen(address);
	ptr++;
	while((ptr - sizeptr) % 4){
		ptr++;
	}
	strncpy(ptr, ",f", 2);
	ptr += 4;
	*((uint32_t *)ptr) = *((uint32_t *)(&f));
	ptr += 4;
	*((uint32 *)sizeptr) = hton32(ptr - sizeptr - 4);
	return ptr - sizeptr;
}

inline long maq_calcMessageLen_1float(char *address){
	long size = 0;
	size += 4; // size
	size += strlen(address);
	size++;
	while(size % 4){
		size++;
	}
	size += 4; // typetags
	size += 4; // float
	return size;
}

void maq_make_bundle_from_event(t_event *e, int *len, char **ret_ptr){
	if(!e){
		return;
	}
	long size = 20;
	long namelen = strlen(e->name->s_name);
	long namelen_padded = namelen;
	namelen_padded++;
	while(namelen_padded % 4){
		namelen_padded++;
	}
	/*
	if(e->name){
		size += 8; // /name
		size += 4; // ,s\0\0
		size += namelen_padded;
	}
	*/

	char xmin[64];
	sprintf(xmin, "%s/x/min", e->name->s_name);
	long xminlen = maq_calcMessageLen_1float(xmin);
	char xwidth[64];
	sprintf(xwidth, "%s/x/width", e->name->s_name);
	long xwidthlen = maq_calcMessageLen_1float(xwidth);
	char ymin[64];
	sprintf(ymin, "%s/y/min", e->name->s_name);
	long yminlen = maq_calcMessageLen_1float(ymin);
	char yheight[64];
	sprintf(yheight, "%s/y/height", e->name->s_name);
	long yheightlen = maq_calcMessageLen_1float(yheight);

	size += xminlen + xwidthlen + yminlen + yheightlen;

	t_maq_osc_msg *m = e->messages_ll;
	while(m){
		size += 4;
		size += strlen(m->address->s_name);
		size++;
		while(size % 4){
			size++;
		}
		size += (m->argc + 2);
		while(size % 4){
			size++;
		}
		int i;
		for(i = 0; i < m->argc; i++){
			switch(atom_gettype(m->argv + i)){
			case A_FLOAT:
			case A_LONG:
				size += 4;
				break;
			case A_SYM:
				size += strlen(atom_getsym(m->argv + i)->s_name);
				size++;
				while(size % 4){
					size++;
				}
				break;
			}
		}
		m = m->next;
	}


	//char buf[size];
	char *buf = (char *)sysmem_newptr(size);
	memset(buf, '\0', size);
	char *ptr = buf;
	strncpy(ptr, "#bundle\0", 8);
	ptr += 16;

	char *sizeptr = ptr;
	/*
	ptr += 4;
	strncpy(ptr, "/name", 5);
	ptr += 8;
	strncpy(ptr, ",s", 2);
	ptr += 4;
	strcpy(ptr, e->name->s_name);
	ptr += strlen(e->name->s_name);
	ptr++;
	while((ptr - buf) % 4){
		ptr++;
	}
	*((uint32 *)sizeptr) = hton32(ptr - sizeptr - 4);
	*/

	ptr += maq_addMessageToBundle_1float(ptr, xmin, e->rect.x);
	ptr += maq_addMessageToBundle_1float(ptr, xwidth, e->rect.width);
	ptr += maq_addMessageToBundle_1float(ptr, ymin, e->rect.y);
	ptr += maq_addMessageToBundle_1float(ptr, yheight, e->rect.height);

	m = e->messages_ll;
	while(m){
		sizeptr = ptr;
		ptr += 4;
		strcpy(ptr, m->address->s_name);
		ptr += strlen(m->address->s_name) + 1;
		while((ptr - sizeptr) %4){
			ptr++;
		}
		*ptr++ = ',';
		int i;
		for(i = 0; i < m->argc; i++){
			switch(atom_gettype(m->argv + i)){
			case A_FLOAT:
				*ptr++ = 'f';
				break;
			case A_LONG:
				*ptr++ = 'i';
				break;
			case A_SYM:
				*ptr++ = 's';
				break;
			}
		}
		ptr++;
		while((ptr - sizeptr) % 4){
			ptr++;
		}
		for(i = 0; i < m->argc; i++){
			switch(atom_gettype(m->argv + i)){
			case A_FLOAT:
				{
					float f = atom_getfloat(m->argv + i);
					*((uint32_t *)ptr) = *((uint32_t *)(&f));
					ptr += 4;
				}
				break;
			case A_LONG:
				{
					uint32 l = atom_getlong(m->argv + i);
					*((uint32 *)ptr) = hton32(l);
					ptr += 4;
				}
				break;
			case A_SYM:
				{
					t_symbol *s = atom_getsym(m->argv + i);
					strcpy(ptr, s->s_name);
					ptr += strlen(s->s_name);
					ptr++;
					while((ptr - sizeptr) % 4){
						ptr++;
					}
				}
				break;
			}
		}
		*((uint32 *)sizeptr) = hton32(ptr - sizeptr - 4);
		m = m->next;
	}
	*len = ptr - buf;
	*ret_ptr = buf;
	/*
	int i;
	for(i = 0; i < ptr - buf; i++){
		post("%d: %c %x", i, buf[i], buf[i]);
	}
	*/
}

void maq_output_osc(t_maq *x, void *outlet, t_symbol *msg, t_event *e){
	if(!e){
		return;
	}
	char *buf = NULL;
	int len;
	maq_make_bundle_from_event(e, &len, &buf);
	if(!msg){
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)buf);
		outlet_anything(outlet, ps_FullPacket, 2, out);
	}else{
		t_atom out[3];
		atom_setsym(out, ps_FullPacket);
		atom_setlong(out + 1, len);
		atom_setlong(out + 2, (long)buf);
		outlet_anything(outlet, msg, 3, out);
	}
	if(buf){
		sysmem_freeptr(buf);
	}
}

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){ 
	ps_maquette_focus->s_thing = (t_object *)x;
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	int dblclick = 0;
	if((((double)clock() - x->dblclick) / CLOCKS_PER_SEC) < .2){
		dblclick = 1;
	}else{
		x->dblclick = clock();
	}

	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax), maq_scale(pt.y, r.height, 0, x->ymin, x->ymax)};
	x->lastmouse = pt;

	switch(modifiers){
	case 0x10:
	case 0x110:
	case 0x18:
	case 0x118:
		{
			t_event *sel = x->selected;
			while(sel){
				if((wc.x < (sel->rect.x + sel->rect.width)) && (wc.x > sel->rect.x) && (wc.y < sel->rect.y) && (wc.y > (sel->rect.y - sel->rect.height))){
					sel = x->selected;
					while(sel){
						if(modifiers == 0x18){
							sel->op = CLONE;
						}else{
							sel->op = MOVE;
						}
						sel = sel->next_selected;
					}
					goto out;
				}else if(fabs(wc.x - (sel->rect.x + sel->rect.width)) < maq_scale(10., 0, r.width, x->xmin, x->xmax) && 
				   fabs(wc.y - (sel->rect.y - sel->rect.height)) < maq_scale(10., 0, r.height, x->ymin, x->ymax)){
					sel = x->selected;
					while(sel){
						sel->op = RESIZE;
						sel = sel->next_selected;
					}
					goto out;
				}
				sel = sel->next_selected;
			}
			maq_unselect(x);
			t_event *e = x->eventlist;
			while(e){
				//post("%f %f %f %f %f", wc.y, e->rect.y, e->rect.height, (e->rect.y - e->rect.height), e->rect.height + e->rect.y);
				if(fabs(wc.x - (e->rect.x + e->rect.width)) < maq_scale(10., 0, r.width, x->xmin, x->xmax) && 
				   fabs(wc.y - (e->rect.y - e->rect.height)) < maq_scale(10., 0, r.height, x->ymin, x->ymax)){
					maq_unselect(x);
					x->selected = e;
					e->selected = 1;
					e->op = RESIZE;
					goto out;
				}else if((wc.x < (e->rect.x + e->rect.width)) && (wc.x > e->rect.x) && (wc.y < e->rect.y) && (wc.y > (e->rect.y - e->rect.height))){
					maq_unselect(x);
					x->selected = e;
					e->selected = 1;
					if(modifiers == 0x18){
						e->op = CLONE;
					}else{
						e->op = MOVE;
					}
					goto out;
				}
				e = e->next;
			}
			x->selbox.x = wc.x;
			x->selbox.y = wc.y;
		}
		break;
	case 0x11:
	case 0x111:
		break;
	}
 out:
	if(dblclick == 1){
		maq_dblclick(x);
	}
	maq_dumpcellblock(x);
	if(x->continuous_output){
		maq_output_osc(x, x->outlet_main, NULL, x->selected);
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax), maq_scale(pt.y, r.height, 0, x->ymin, x->ymax)};
	t_pt delta, deltawc;
	maq_delta(x, r, pt, &delta, &deltawc);
	x->lastmouse = pt;

	switch(modifiers){
	case 0x10:
	case 0x110:
	case 0x18:
	case 0x118:
		{
			t_event *sel = x->selected;
			if(sel){
				while(sel){
					switch(sel->op){
					case CLONE:
						{
							t_event *ee = maq_event_clone(x, sel);
							t_event *copied = sel;
							//ee->rect = sel->rect;
							//maq_unselect(x);
							ee->next_selected = sel->next_selected;
							if(sel->prev_selected){
								sel->prev_selected->next_selected = ee;
							}
							if(sel->next_selected){
								sel->next_selected->prev_selected = ee;
							}
							ee->prev_selected = sel->prev_selected;
							if(x->selected == sel){
								x->selected = ee;
							}
							sel->selected = 0;
							sel->next_selected = sel->prev_selected = NULL;
							ee->next = x->eventlist;
							if(x->eventlist){
								x->eventlist->prev = ee;
							}
							x->eventlist = ee;
							ee->selected = 1;
							ee->op = MOVE;
							sel = ee;
							sel->rect.x -= deltawc.x;
							sel->rect.y -= deltawc.y;

							t_atom out[6];
							char *buf1 = NULL, *buf2 = NULL;
							int len1, len2;
							maq_make_bundle_from_event(copied, &len1, &buf1);
							maq_make_bundle_from_event(sel, &len2, &buf2);
							atom_setsym(out, gensym("FullPacket"));
							atom_setlong(out + 1, len1);
							atom_setlong(out + 2, (long)buf1);
							atom_setsym(out + 3, gensym("FullPacket"));
							atom_setlong(out + 4, len2);
							atom_setlong(out + 5, (long)buf2);
							outlet_anything(x->outlet_info, gensym("copy"), 6, out);
							break;
						}
					case MOVE:
						sel->rect.x -= deltawc.x;
						sel->rect.y -= deltawc.y;
						break;
					case RESIZE:
						sel->rect.width -= deltawc.x;
						sel->rect.height += deltawc.y;
						break;
					}
					sel = sel->next_selected;
				}
			}else{
				x->selbox.width = wc.x - x->selbox.x;
				x->selbox.height = x->selbox.y - wc.y;
			}
		}
	}
 out:
	maq_dumpcellblock(x);
	if(x->continuous_output){
		maq_output_osc(x, x->outlet_main, NULL, x->selected);
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax), maq_scale(pt.y, r.height, 0, x->ymin, x->ymax)};
	if(x->selbox.width == 0 && x->selbox.height == 0 && (modifiers == 0x11 || modifiers == 0x111)){
		maq_unselect(x);
		maq_addevent(x, wc);
	}
	if(x->selbox.width != 0 && x->selbox.height != 0){
		t_event *e = x->eventlist;
		while(e){
			if((e->rect.x < (x->selbox.x + x->selbox.width)) && ((e->rect.x + e->rect.width) > x->selbox.x) &&
			   ((e->rect.y - e->rect.height) < (x->selbox.y)) && (e->rect.y > (x->selbox.y - x->selbox.height))){
				e->next_selected = x->selected;
				if(x->selected){
					x->selected->prev_selected = e;
				}
				e->selected = 1;
				x->selected = e;
			}
			e = e->next;
		}
	}
	x->selbox = (t_rect){0., 0., 0., 0.};
	if(!(x->continuous_output)){
		maq_output_osc(x, x->outlet_main, NULL, x->selected);
	}
	x->lastmouse = (t_pt){0., 0.};
	maq_dumpcellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mousemove(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, x->xmin, x->xmax), maq_scale(pt.y, r.height, 0, x->ymin, x->ymax)};
	t_event *e = x->eventlist;
	while(e){
		if(fabs(wc.x - (e->rect.x + e->rect.width)) < maq_scale(10., 0, r.width, x->xmin, x->xmax) && 
		   fabs(wc.y - (e->rect.y - e->rect.height)) < maq_scale(10., 0, r.height, x->ymin, x->ymax)){
			e->mouse_over_bottom_right_corner = 1;
		}else{
			e->mouse_over_bottom_right_corner = 0;
		}
		e = e->next;
	}
	maq_dumpcellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_dblclick(t_maq *x){
	if(x->selected){
		maq_output_osc(x, x->outlet_info, gensym("doubleclick"), x->selected);
	}
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

void maq_xminmax(t_maq *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
        jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_yminmax(t_maq *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_clear(t_maq *x){
        jbox_invalidate_layer((t_object *)x, x->pv, l_main);
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
 	class_addmethod(c, (method)maq_mousemove, "mousemove", A_CANT, 0); 
	class_addmethod(c, (method)maq_clear, "clear", 0);
	class_addmethod(c, (method)maq_dump, "dump", 0);
	class_addmethod(c, (method)maq_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)maq_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)maq_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)maq_add_to_bundle, "addtobundle", A_SYM, A_SYM, A_LONG, A_LONG, 0);
	class_addmethod(c, (method)maq_clear_bundle, "clearbundle", A_SYM, 0);
	class_addmethod(c, (method)maq_int, "int", A_LONG, 0);
	class_addmethod(c, (method)maq_delete_selected, "deleteselected", 0);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_maq, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_maq, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_maq, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_maq, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

        CLASS_ATTR_LONG(c, "continuous_output", 0, t_maq, continuous_output);
        CLASS_ATTR_DEFAULTNAME_SAVE(c, "continuous_output", 0, "1");

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_maq, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, ".5 .5 .5 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "boxcolor", 0, t_maq, boxcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "boxcolor", 0, ".25 .25 .25 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "boxcolor", 0, "rgba", "Box Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

	l_main = gensym("l_main");
	ps_done = gensym("done");
	ps_dump = gensym("dump");
	ps_cellblock = gensym("cellblock");
	ps_clear = gensym("clear");
	ps_oscname = gensym("/name");
	ps_FullPacket = gensym("FullPacket");
	ps_maquette_focus = gensym("maquette_focus");
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
		x->takingdump = 0;
		x->selbox = (t_rect){0., 0., 0., 0.};
		x->slots = (char *)sysmem_newptr(512);
		memset(x->slots, '\0', 512);
		x->nslots = 512;

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
