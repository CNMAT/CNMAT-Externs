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
  NAME: timewarp.pitchmap
  DESCRIPTION: Pitchmapping object for timewarp
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

#ifdef WIN
#include "../../../SDK/MaxSDK-5/c74support/max-includes/common/commonsyms.c"
#endif

#define MAX_NUM_FUNCTIONS 64

#define POSITION_UPDATE_RATE_MS 100

#define MAX_ARRAY_LEN 4096
#define DEF_NOTE_COUNT 32

typedef struct _twpm_osc_msg{
	t_symbol *address;
	int argc;
	t_atom *argv;
	int buflen;
	struct _twpm_osc_msg *next;
	struct _twpm_osc_msg *prev;
} t_twpm_osc_msg;

#define NOTE 1
#define DURATION 2
#define MARKER 3
typedef struct _event{ 
	t_atom *time;
	int selected;
	t_atom *note_sel, *dur_sel;
	int whatselected;
	t_hashtab *messages_ht;
	struct _twpm_osc_msg *messages_ll;
 	struct _event *next; 
 	struct _event *prev; 
 	struct _event *next_selected; 
 	struct _event *prev_selected; 
} t_event; 

typedef struct _floatbuf{
	long ac;
	float *av;
} t_floatbuf;

typedef struct _twpm{ 
 	t_jbox box; 
 	void *outlet_main, *outlet_info;
	long inlet;
	void *proxy;
	t_event *eventlist;
	t_event *left, *right;
	t_event *selected;
	t_object *pv;
	t_critical lock;
 	t_jrgba bgcolor, selectioncolor, gridcolor; 
	double xmin, xmax, ymin, ymax;
	t_rect sel_box;
	t_pt drag;
	double noteheadsize;
	int takingdump;
	clock_t dblclick;
	float lasttime;
	t_hashtab *subdivs_ht;
	t_floatbuf beatbuf;
	t_symbol **subdivs_to_show;
	int num_subdivs_to_show;
} t_twpm; 

void *twpm_class; 

void twpm_paint(t_twpm *x, t_object *patcherview); 
void twpm_paint_notes(t_twpm *x, t_object *patcherview, t_rect r);
void twpm_reorderEvent(t_twpm *x, t_event *p);
void twpm_swapEvents(t_event *p1, t_event *p2);
double twpm_computeNotePos(double y, t_rect r);
double twpm_uncomputeNotePos(double y, t_rect r);
t_twpm_osc_msg *twpm_lookup_osc_msg(t_event *e, t_symbol *address);
t_twpm_osc_msg *twpm_addMessageToEvent(t_event *e, t_symbol *address, int buflen, int argc, t_atom *argv);
void twpm_show_subdivs(t_twpm *x, t_symbol *msg, int argc, t_atom *argv);
void twpm_set_timepoints(t_twpm *x, t_symbol *msg, int argc, t_atom *argv);
void twpm_fullPacket(t_twpm *x, long len, long ptr);
void twpm_list(t_twpm *x, t_symbol *msg, short argc, t_atom *argv);
void twpm_outputEvent(t_twpm *x, void *outlet, t_symbol *msg, t_event *e);
void twpm_float(t_twpm *x, double f);
void twpm_int(t_twpm *x, long key);
int twpm_find_btn(t_twpm *x, double xx, t_event **left, t_event **right);
t_event *twpm_select(t_twpm *x, float time, float midic);
void twpm_assist(t_twpm *x, void *b, long m, long a, char *s); 
void twpm_free(t_twpm *x); 
t_atom *twpm_find_closest_note(t_twpm_osc_msg *m, float midic);
t_max_err twpm_notify(t_twpm *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void twpm_mousedown(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers); 
void twpm_mousedrag(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers); 
void twpm_mouseup(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers); 
t_event *twpm_insertEvent(t_twpm *x, float time);
void twpm_removeSelected(t_twpm *x);
void twpm_removeEvent(t_twpm *x, t_event *event);
void twpm_clear(t_twpm *x, t_symbol *pat);
void twpm_unselect(t_twpm *x);
t_floatbuf *twpm_floatbuf_alloc(int ac, float *av);
void twpm_floatbuf_store(t_floatbuf *fb, int ac, float *av);
t_event *twpm_event_alloc(void);
t_twpm_osc_msg *twpm_osc_msg_alloc(t_symbol *address, long argc, t_atom *argv);
void twpm_osc_msg_free(t_twpm_osc_msg *m);
void twpm_osc_msg_freelist(t_twpm_osc_msg *m);
void twpm_clear_osc_messages(t_event *e);
void twpm_post_osc_msg(t_twpm_osc_msg *m);
double twpm_scale(double f, double min_in, double max_in, double min_out, double max_out);
double twpm_clip(double f, double min, double max);
void twpm_dump(t_twpm *x);
void twpm_dumpcellblock(t_twpm *x);
void twpm_xminmax(t_twpm *x, double min, double max);
void twpm_yminmax(t_twpm *x, double min, double max);
int main(void); 
void *twpm_new(t_symbol *s, long argc, t_atom *argv); 
t_max_err twpm_events_get(t_twpm *x, t_object *attr, long *argc, t_atom **argv);
t_max_err twpm_events_set(t_twpm *x, t_object *attr, long argc, t_atom *argv);

static t_symbol *l_background, *l_notes;
static t_symbol *ps_done, *ps_dump, *ps_notes, *ps_clear, *ps_cellblock, *ps_FullPacket, *ps_durations, *ps_time, *ps_marker, *ps_beats, *ps_subdivs;
static t_atom pa_dump, pa_clear, pa_all;

//const char *notenames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
int notestep[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6};
int sharps[] = {0, 93, 103, 113, 0, 93, 103, 113, 0, 93, 0, 93, 103, 113, 0, 93, 103, 113, 0, 93, 103, 113, 0, 93};

void twpm_paint(t_twpm *x, t_object *patcherview){ 
	x->pv = patcherview;
 	t_rect r; 
 	t_jrgba c = (t_jrgba){0., 0., 0., 1.};;

 	// get graphics context 
 	//t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 

 	// get our box's rectangle 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &r); 

	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_background, r.width, r.height);
	if(g){
		// draw the outline of the box 
		jgraphics_set_source_jrgba(g, &c); 
		jgraphics_set_line_width(g, 1); 
		jgraphics_rectangle(g, 0., 0., r.width, r.height); 
		jgraphics_stroke(g); 

		jgraphics_set_source_jrgba(g, &(x->bgcolor)); 
		jgraphics_rectangle(g, 0., 0., r.width, r.height); 
		jgraphics_fill(g); 
		jbox_end_layer((t_object *)x, patcherview, l_background);
	}
	jbox_paint_layer((t_object *)x, patcherview, l_background, 0, 0);

	twpm_paint_notes(x, patcherview, r);

	g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);
	jgraphics_rectangle(g, x->sel_box.x, x->sel_box.y, x->sel_box.width, x->sel_box.height);
	jgraphics_stroke(g);
}

#define DIV 24
#define STEP 4
#define OCTAVE (7 * STEP)
#define SPACE 8

// fonts: http://pagespro-orange.fr/christian.texier/mididesi/free/index.htm
void twpm_paint_notes(t_twpm *x, t_object *patcherview, t_rect r){
	t_jgraphics *g = jbox_start_layer((t_object *)x, patcherview, l_notes, r.width, r.height);
 	if(g){ 
		double xpos = 0, ypos = 8;
		double mid = r.height / 2;
		int i;
		//jgraphics_move_to(g, 0, r.height / 2);
		//jgraphics_line_to(g, r.width, r.height / 2);
		for(i = 0; i < 5; i++){
			jgraphics_move_to(g, 0, ypos + mid);
			jgraphics_line_to(g, r.width, ypos + mid);
			jgraphics_move_to(g, 0, mid - ypos);
			jgraphics_line_to(g, r.width, mid - ypos);
			ypos += SPACE;
		}
		jgraphics_stroke(g);

		t_jrgba black = (t_jrgba){0., 0., 0., 0.5};
		jgraphics_set_source_jrgba(g, &black);
		ypos = SPACE * 8;
		for(i = 0; i < 5; i++){
			jgraphics_move_to(g, 0, ypos + mid);
			jgraphics_line_to(g, r.width, ypos + mid);
			jgraphics_move_to(g, 0, mid - ypos);
			jgraphics_line_to(g, r.width, mid - ypos);
			ypos += SPACE;
		}
		jgraphics_stroke(g);

		// clefs
		black = (t_jrgba){0., 0., 0., 1.};
		jgraphics_set_source_jrgba(g, &black);
		jgraphics_select_font_face(g, "Sonora", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
		jgraphics_set_font_size(g, 54);
		char ch = 38;
		jgraphics_move_to(g, 0, mid - 16);
		jgraphics_show_text(g, &ch);
		ch = 63;
		jgraphics_move_to(g, 0, mid + 16);
		jgraphics_show_text(g, &ch);

		black = (t_jrgba){0., 0., 0., .5};
		jgraphics_set_source_jrgba(g, &black);
		ch = 38;
		jgraphics_move_to(g, 0, mid - (SPACE * 9));
		jgraphics_show_text(g, &ch);
		ch = 63;
		jgraphics_move_to(g, 0, mid + (SPACE * 9));
		jgraphics_show_text(g, &ch);

		// middle c is at r.height / 2
		critical_enter(x->lock);
		t_event *e = x->eventlist;

		black.alpha = 1.;

		jgraphics_set_source_jrgba(g, &x->gridcolor);
		if(x->beatbuf.av){
			if(x->beatbuf.av[0] >= x->xmin){
				int j;
				for(j = 0; j < x->beatbuf.ac; j++){
					float xsc = twpm_scale(x->beatbuf.av[j], x->xmin, x->xmax, 0, r.width);
					jgraphics_move_to(g, xsc, 0);
					jgraphics_line_to(g, xsc, 15);
					jgraphics_stroke(g);
				}
			}
		}

		if(x->num_subdivs_to_show > 0){
			int j;
			for(j = 0; j < x->num_subdivs_to_show; j++){
				t_floatbuf *fb = NULL;
				hashtab_lookup(x->subdivs_ht, x->subdivs_to_show[j], (t_object **)&fb);
				if(fb){
					int k;
					for(k = 0; k < fb->ac; k++){
						float xsc = twpm_scale(fb->av[k], x->xmin, x->xmax, 0, r.width);
						jgraphics_move_to(g, xsc, 0);
						jgraphics_line_to(g, xsc, 10);
						jgraphics_stroke(g);
					}
				}
			}
		}

		while(e){
			t_twpm_osc_msg *notes_msg = twpm_lookup_osc_msg(e, ps_notes);
			t_twpm_osc_msg *durations_msg = twpm_lookup_osc_msg(e, ps_durations);
			double duration = 0;
			double time = atom_getfloat(e->time);
			if(!notes_msg){
				t_twpm_osc_msg *m = twpm_lookup_osc_msg(e, ps_marker);
				double xsc = twpm_scale(time, x->xmin, x->xmax, 0, r.width);
				if(m){
					// marker
					float c[4] = {0., 0., 1., 1.};
					t_twpm_osc_msg *color = twpm_lookup_osc_msg(e, gensym("/color"));
					if(color){
						for(i = 0; i < color->argc; i++){
							c[i] = atom_getfloat(color->argv + i);
						}
					}
					jgraphics_set_source_rgba(g, c[0], c[1], c[2], c[3]);
					jgraphics_move_to(g, xsc, 0);
					jgraphics_line_to(g, xsc, r.height);
					jgraphics_stroke(g);
					jgraphics_move_to(g, xsc, 6);
					jgraphics_line_to(g, xsc - 6, 0);
					jgraphics_line_to(g, xsc + 6, 0);
					jgraphics_close_path(g);
					jgraphics_move_to(g, xsc, r.height - 6);
					jgraphics_line_to(g, xsc - 6, r.height);
					jgraphics_line_to(g, xsc + 6, r.height);
					jgraphics_close_path(g);
					jgraphics_fill(g);
				}else{
					// some other event

				}
			}else{
				t_jrgba selcolor = x->selectioncolor;
				t_jrgba selcolor_light = x->selectioncolor;
				selcolor_light.alpha = .5;
				for(i = 0; i < notes_msg->argc; i++){
					if(durations_msg){
						if(i < durations_msg->argc){
							duration = atom_getfloat(durations_msg->argv + i);
						}
					}
					float midic = atom_getfloat(notes_msg->argv + i);
					int pc = (int)round(midic * ((float)DIV / 12.)) % DIV;
					ypos = twpm_computeNotePos(midic, r);
					double xsc = twpm_scale(time, x->xmin, x->xmax, 0, r.width);
					jgraphics_set_source_jrgba(g, &black);
					// ledger lines
					if(pc > 80){
						double ll = (r.height / 2.) - (SPACE * 6);
						ch = 95;
						while(ll > ypos - SPACE){
							jgraphics_move_to(g, xsc - 7, ll);
							jgraphics_show_text(g, &ch);
							ll -= SPACE;
						}
					}else if(pc < 40){
						double ll = (r.height / 2.) + (SPACE * 6);
						ch = 95;
						while(ll < ypos){
							jgraphics_move_to(g, xsc - 7, ll);
							jgraphics_show_text(g, &ch);
							ll += SPACE;
						}
					}
					if(sharps[pc]){
						jgraphics_select_font_face(g, "Tempera", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
						jgraphics_set_font_size(g, x->noteheadsize - 18);//36
						//ch = 35;
						ch = sharps[pc];
						jgraphics_move_to(g, xsc - 15, ypos - STEP);
						jgraphics_show_text(g, &ch);
					}
					ch = 81;
					if(pc < 50 || (pc > 59 && pc < 71)){
						ch = 113;
					}
					jgraphics_select_font_face(g, "Sonora", JGRAPHICS_FONT_SLANT_NORMAL, JGRAPHICS_FONT_WEIGHT_NORMAL);
					jgraphics_set_font_size(g, x->noteheadsize);//54
					if(e->selected == 1){
						jgraphics_set_source_jrgba(g, &selcolor);
						/*
						  if(e->selected_element){
						  if(atom_getfloat(e->selected_element) == midic){
						  jgraphics_set_source_jrgba(g, &selcolor);
						  }else{
						  jgraphics_set_source_jrgba(g, &selcolor_light);
						  }
						  }else{
						  jgraphics_set_source_jrgba(g, &selcolor);
						  }
						*/
					}
					jgraphics_move_to(g, xsc - 5, ypos);
					jgraphics_show_text(g, &ch);
					if(durations_msg){
						jgraphics_set_line_width(g, 3); 
						jgraphics_move_to(g, xsc, ypos - STEP);
						float dur = twpm_scale(time + duration, x->xmin, x->xmax, 0, r.width);
						jgraphics_line_to(g, dur, ypos - STEP);
						jgraphics_stroke(g);
						jgraphics_move_to(g, dur, ypos - STEP - 6);
						jgraphics_set_line_width(g, 1); 
						jgraphics_line_to(g, dur, ypos - STEP + 6);
						jgraphics_stroke(g);
						//jgraphics_ellipse(g, dur - 3, ypos - STEP - 3, 6, 6);
						//jgraphics_fill(g);
					}
				}
			}
			e = e->next;
		}
		critical_exit(x->lock);
		jbox_end_layer((t_object *)x, patcherview, l_notes);
 	} 
	jbox_paint_layer((t_object *)x, patcherview, l_notes, 0., 0.);
}

void twpm_reorderEvent(t_twpm *x, t_event *e){
	double time = atom_getfloat(e->time);
	if(!e){
		return;
	}
	while(e->prev){
		if(time < atom_getfloat(e->prev->time)){
			twpm_swapEvents(e->prev, e);
		}else{
			break;
		}
	}
	while(e->next){
		if(time > atom_getfloat(e->next->time)){
			twpm_swapEvents(e, e->next);
		}else{
			break;
		}
	}
	while(e->prev){
		e = e->prev;
	}
	if(e->prev == NULL){
		x->eventlist = e;
	}
}

void twpm_swapEvents(t_event *e1, t_event *e2){
	if(!e1 || !e2){
		return;
	}
	if(e1->prev){
		e1->prev->next = e2;
	}
	if(e2->next){
		e2->next->prev = e1;
	}

	e1->next = e2->next;
	e2->prev = e1->prev;
	e1->prev = e2;
	e2->next = e1;

}

double twpm_computeNotePos(double y, t_rect r){
	int pc = (int)round(y * 2) % DIV;
	int step = (notestep[pc] - 1) * STEP;
	int oct = (int)floor(((y * 2) / DIV) - 5);
	
	if(!pc && round(y * 2) > y * 2){
		oct++;
	}
	//post("pc = %d, step = %d, oct = %d, %f", pc, step, oct, ((r.height / 2) - (oct * OCTAVE)) - step);
	return ((r.height / 2) - (oct * OCTAVE)) - step;
}

double twpm_uncomputeNotePos(double y, t_rect r){
	double diff = DBL_MAX;
	double closest = 0;
	double i;
	for(i = 0; i < 128; i += .5){
		double d = fabs(twpm_computeNotePos(i, r) - y);
		//post("y = %f, i = %f, diff = %f, notepos = %f", y, i, diff, twpm_computeNotePos(i, r));
		if(d < diff){
			diff = d;
			closest = i;
		}
	}
	//post("closest = %f", closest);
	double notepos = twpm_computeNotePos(closest, r);
	//post("notepos = %f, middle = %f", notepos, r.height / 2);
	int cl = closest;
	if(y < notepos - (SPACE * (2 / 3))){
		if(sharps[(cl + 1)] % DIV){
			cl += .5;
		}
		//cl += (sharps[(cl + 1) % DIV]);
	}else if(y > notepos + (SPACE * (2 / 3))){
		if((cl % DIV) > 0){
			if(sharps[(cl - 1) % DIV] == 0){
				cl -= .5;
			}
			//cl -= sharps[(cl - 1) % DIV];
		}
	}
	return cl;
	/*
	  double nspaces = r.height / SPACE;
	  double space = (nspaces * (y / (r.height));

	  post("space = %f", space);
	*/
}

t_twpm_osc_msg *twpm_lookup_osc_msg(t_event *e, t_symbol *address){
	t_hashtab *ht = e->messages_ht;
	t_twpm_osc_msg *val = NULL;
	hashtab_lookup(ht, address, (t_object **)&val);
	return val;
}

t_twpm_osc_msg *twpm_addMessageToEvent(t_event *e, t_symbol *address, int buflen, int argc, t_atom *argv){
	t_twpm_osc_msg *m = NULL;
	if(m = twpm_lookup_osc_msg(e, address)){
		if(argc > m->argc){
			m->argv = (t_atom *)sysmem_resizeptr(m->argv, argc * sizeof(t_atom));
			m->buflen = argc;
			m->argc = argc;
			memcpy(m->argv, argv, argc * sizeof(t_atom));
		}
	}else{
		m = (t_twpm_osc_msg *)sysmem_newptr(1 * sizeof(t_twpm_osc_msg));
		m->address = address;
		m->argc = argc;
		if(buflen < argc){
			buflen = argc;
		}
		m->buflen = buflen;
		if(argc == 0){
			m->argv = NULL;
		}else{
			m->argv = (t_atom *)sysmem_newptr(buflen * sizeof(t_atom));
			memcpy(m->argv, argv, argc * sizeof(t_atom));
		}
		if(e->messages_ll){
			e->messages_ll->prev = m;
		}
		m->next = e->messages_ll;
		m->prev = NULL;
		e->messages_ll = m;
		if(!(e->messages_ht)){
			e->messages_ht = hashtab_new(0);
			hashtab_flags(e->messages_ht, OBJ_FLAG_DATA);
		}
		hashtab_store(e->messages_ht, address, (t_object *)m);
	}
	return m;
}

void twpm_show_subdivs(t_twpm *x, t_symbol *msg, int argc, t_atom *argv){
	if(x->subdivs_to_show){
		x->subdivs_to_show = (t_symbol **)sysmem_resizeptr(x->subdivs_to_show, argc * sizeof(t_symbol *));
	}else{
		x->subdivs_to_show = (t_symbol **)sysmem_newptr(argc * sizeof(t_symbol *));
	}
	int i;
	for(i = 0; i < argc; i++){
		char buf[16];
		sprintf(buf, "/subdivs/%ld", atom_getlong(argv + i));
		post("%s: %s", PF, buf);
		x->subdivs_to_show[i] = gensym(buf);
	}
	x->num_subdivs_to_show = argc;
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void twpm_set_timepoints(t_twpm *x, t_symbol *msg, int argc, t_atom *argv){
	if(atom_getsym(argv) == ps_FullPacket){
		char *buf = (char *)atom_getlong(argv + 2);
		long len = atom_getlong(argv + 1);
		t_osc_msg osc_msg[osc_util_getMsgCount(len, buf)];
		osc_util_parseBundle(len, buf, osc_msg);
		int i;
		for(i = 0; i < sizeof(osc_msg) / sizeof(t_osc_msg); i++){
			long ac;
			t_atom av[osc_util_getArgCount(osc_msg + i)];
			t_symbol *address = NULL;
			omax_util_oscMsg2MaxAtoms(osc_msg + i, &ac, av);
			address = atom_getsym(av);
			int po, ao;
			if(address == ps_beats){
				if(x->beatbuf.av){
					x->beatbuf.av = (float *)sysmem_resizeptr(x->beatbuf.av, (ac - 1) * sizeof(float));
				}else{
					x->beatbuf.av = (float *)sysmem_newptr((ac - 1) * sizeof(float));
				}
				x->beatbuf.ac = ac - 1;
				int j;
				for(j = 1; j < ac; j++){
					x->beatbuf.av[j - 1] = atom_getfloat(av + j);
				}
			}else if(osc_match(address->s_name, ps_subdivs->s_name, &po, &ao)){
				t_floatbuf *fb = NULL;
				hashtab_lookup(x->subdivs_ht, address, (t_object **)&fb);
				if(!fb){
					fb = (t_floatbuf *)sysmem_newptr(sizeof(t_floatbuf));
				}
				if(fb->av){
					fb->av = (float *)sysmem_resizeptr(fb->av, (ac - 1) * sizeof(float));
				}
				fb->ac = ac - 1;
				int j;
				for(j = 1; j < ac; j++){
					fb->av[j - 1] = atom_getfloat(av + j);
				}
				hashtab_store(x->subdivs_ht, address, (t_object *)fb);
			}
		}
	}else{

	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void twpm_fullPacket(t_twpm *x, long len, long ptr){
	t_event *e = twpm_event_alloc();
	t_osc_msg *time = NULL, *notes = NULL;
	char *buf = (char *)ptr;
	t_osc_msg msg[osc_util_getMsgCount(len, buf)];
	osc_util_parseBundle(len, buf, msg);
	int i;
	for(i = 0; i < sizeof(msg) / sizeof(t_osc_msg); i++){
		long ac;
		t_atom av[osc_util_getArgCount(msg + i)];
		t_symbol *address = NULL;
		omax_util_oscMsg2MaxAtoms(msg + i, &ac, av);
		address = atom_getsym(av);
		if(address == ps_notes){
			notes = msg + i;
			twpm_addMessageToEvent(e, address, DEF_NOTE_COUNT, ac - 1, av + 1);
		}else if(address == ps_time){
			time = msg + i;
			t_twpm_osc_msg *m = twpm_addMessageToEvent(e, address, 1, 1, av + 1);
			e->time = m->argv;
		}else{
			twpm_addMessageToEvent(e, address, ac - 1, ac - 1, av + 1);
		}
	}
	if(!time){
		object_error((t_object *)x, "OSC bundle without /time message");
		return;
	}
	e->next = x->eventlist;
	if(x->eventlist){
		x->eventlist->prev = e;
	}
	x->eventlist = e;
	twpm_reorderEvent(x, e);
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void twpm_list(t_twpm *x, t_symbol *msg, short argc, t_atom *argv){
	switch(proxy_getinlet((t_object *)x)){
	case 0:

		break;
	case 1:
		// cellblock
		{
			if(x->takingdump == 1){
				if(argc > 2){
					if(x->selected){
						t_symbol *address = atom_getsym(argv + 2);
						if(!address){
							return;
						}
						if(*(address->s_name) != '/'){
							char buf[strlen(address->s_name) + 1];
							sprintf(buf, "/%s", address->s_name);
							address = gensym(buf);
						}
						if(address == ps_notes){
							twpm_addMessageToEvent(x->selected, address, DEF_NOTE_COUNT, argc - 3, argv + 3);
						}else if(address == ps_time){
							t_twpm_osc_msg *m = twpm_addMessageToEvent(x->selected, address, 1, 1, argv + 3);
							x->selected->time = m->argv;
						}else{
							twpm_addMessageToEvent(x->selected, address, argc - 3, argc - 3, argv + 3);
						}
					}
				}
			}else{
				if(x->selected){
					t_event *e = x->selected;
					x->takingdump = 1;
					twpm_clear_osc_messages(e);
					t_atom dump;
					atom_setsym(&dump, ps_dump);
					outlet_anything(x->outlet_info, ps_cellblock, 1, &pa_dump);
					x->takingdump = 0;
					t_twpm_osc_msg *m = e->messages_ll;
				}
			}
		}
		jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
		jbox_redraw((t_jbox *)&(x->box)); 
		break;
	}
}

void twpm_outputEvent(t_twpm *x, void *outlet, t_symbol *msg, t_event *e){
	if(!e){
		return;
	}
	t_twpm_osc_msg *m = e->messages_ll;
	if(!m){
		return;
	}

	long size = 16;
	while(m){
		size += 4; // size
		size += strlen(m->address->s_name);
		size++;
		while(size % 4){
			size++;
		}
		// typetags
		size += m->argc + 1;
		size++;
		while(size % 4){
			size++;
		}
		int i;
		for(i = 0; i < m->argc; i++){
			long type = atom_gettype(m->argv + i);
			if(type == A_LONG || type == A_FLOAT){
				size += 4;
			}else{
				t_symbol *s = atom_getsym(m->argv + i);
				size += strlen(s->s_name);
				size++;
				while(size % 4){
					size++;
				}
			}
		}
		m = m->next;
	}
	char bundle[size];
	char *ptr = bundle;
	memset(ptr, '\0', size);
	strncpy(ptr, "#bundle\0", 8);
	// timetag...
	ptr += 16;

	m = e->messages_ll;
	while(m){
		ptr += omax_util_encode_atoms(ptr, size - (ptr - bundle), m->address, m->argc, m->argv);
		m = m->next;
	}

	if(msg){
		t_atom out[3];
		atom_setsym(out, ps_FullPacket);
		atom_setlong(out + 1, size);
		atom_setlong(out + 2, (long)bundle);
		outlet_anything(outlet, msg, 3, out);
	}else{
		post("here");
		t_atom out[2];
		atom_setlong(out, size);
		atom_setlong(out + 1, (long)bundle);
		outlet_anything(outlet, ps_FullPacket, 2, out);
	}
}

void twpm_float(t_twpm *x, double f){
	if(!x->eventlist){
		return;
	}
	t_event *e = x->right;
	if(!e){
		e = x->eventlist;
	}
	if(f < atom_getfloat(e->time)){
		while(e->prev){
			if(f < atom_getfloat(e->prev->time)){
				e = e->prev;
			}else{
				break;
			}
		}
	}
	double ff = floor(10000. * f) / 10000.;
	double lt = floor(10000. * x->lasttime) / 10000.;
	double tt = floor(10000. * atom_getfloat(e->time)) / 10000.;
	while(ff >= tt){
		if((lt < tt && ff >= tt && fabs(lt - ff) < .25) || (ff > tt && fabs(ff - tt) < .0002 && fabs(lt - tt) > .1)){
			twpm_outputEvent(x, x->outlet_main, NULL, e);
		}
		e = e->next;
		if(!e){
			break;
		}
		tt = floor(10000. * atom_getfloat(e->time)) / 10000.;
	}
	x->lasttime = ff;
	x->right = e;
}

// key commands
void twpm_int(t_twpm *x, long key){
}

// this function returns the number of the point to the left of xx
int twpm_find_btn(t_twpm *x, double xx, t_event **left, t_event **right){
	//post("function = %p", function);
	int i = 0;
	t_event *ptr = x->eventlist;

	if(xx < atom_getfloat(ptr->time)){
		*left = NULL;
		*right = ptr;
		return -1;
	}
	while(ptr->next){
		float time = atom_getfloat(ptr->time), nexttime = atom_getfloat(ptr->next->time);
		if(xx >= time && xx <= nexttime){
			*left = ptr;
			*right = ptr->next;
			if(*right){
				if(xx == nexttime){
					return i + 1;
				}
			}
			return i;
		}
		ptr = ptr->next;
		i++;
	}

	*left = ptr;
	*right = NULL;
	return i;
}

t_event *twpm_select(t_twpm *x, float time, float midic){
	double min = 1000000000.;
	t_event *min_ptr = NULL;
	t_event *ptr = x->eventlist;
	double timedif, midicdif;
	t_rect r;
	jbox_get_patching_rect((t_object *)&(x->box), &r);

	while(ptr){
		timedif = fabs(time - atom_getfloat(ptr->time));
		if(timedif < min){
			min = timedif;
			min_ptr = ptr;
		}
		ptr = ptr->next;
	}
	if(min < .02){
		return min_ptr;
	}else{
		return NULL;
	}
}

t_atom *twpm_find_closest_note(t_twpm_osc_msg *m, float midic){
	float min = FLT_MAX;
	t_atom *minptr = NULL;
	int i;
	for(i = 0; i < m->argc; i++){
		float v = atom_getfloat(m->argv + i);
		float dif = fabs(midic - v);
		if(dif < min){
			minptr = m->argv + i;
			min = dif;
		}
	}
	return minptr;
}

float twpm_find_closest_gridtime(t_twpm *x, float time){
	int i, j;
	float xx = time;
	float diff = FLT_MAX;
	if(x->num_subdivs_to_show > 0){
		// don't need to go through beats cause they'll be in here
		for(i = 0; i < x->num_subdivs_to_show; i++){
			t_floatbuf *fb = NULL;
			hashtab_lookup(x->subdivs_ht, x->subdivs_to_show[i], (t_object **)&fb);
			if(fb){
				for(j = 0; j < fb->ac; j++){
					float d = fabs(fb->av[j] - time);
					if(d < diff){
						diff = d;
						xx = fb->av[j];
					}
				}
			}
		}
	}else if(x->beatbuf.ac > 0){
		for(j = 0; j < x->beatbuf.ac; j++){
			float d = fabs(x->beatbuf.av[j] - time);
			if(d < diff){
				diff = d;
				xx = x->beatbuf.av[j];
			}
		}
	}
	return xx;
}

t_max_err twpm_notify(t_twpm *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		//t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		jbox_invalidate_layer((t_object *)x, x->pv, l_background);
		jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
 		jbox_redraw((t_jbox *)&(x->box)); 
	} 
	return 0; 
} 

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void twpm_mousedown(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers){ 
	int dblclick = 0;
	if(((double)clock() - x->dblclick) / CLOCKS_PER_SEC < .2){
		dblclick = 1;
	}else{
		x->dblclick = clock();
	}
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	float time = twpm_scale(pt.x, 0., r.width, x->xmin, x->xmax);
	float midic = twpm_uncomputeNotePos(pt.y, r);

	switch(modifiers){
	case 0x10:
	case 0x110:
		{
			t_event *e = x->eventlist;
			float mintime = FLT_MAX;
			t_event *minptr = NULL;
			while(e){
				float diff = fabs(time - atom_getfloat(e->time));
				if(diff < mintime){
					mintime = diff;
					minptr = e;
				}
				e = e->next;
			}
			twpm_unselect(x);
			if(minptr){
				if(twpm_scale(mintime, x->xmin, x->xmax, 0, r.width) < 5){
					x->selected = minptr;
					minptr->selected = 1;
					minptr->note_sel = minptr->dur_sel = NULL;
					minptr->whatselected = 0;
				}
			}
		}
		break;
	case 0x11:
	case 0x111:{
		t_event *e = x->eventlist;
		{
			while(e){
				t_twpm_osc_msg *notes = twpm_lookup_osc_msg(e, ps_notes);
				t_twpm_osc_msg *durations = twpm_lookup_osc_msg(e, ps_durations);
				t_atom *durptr = NULL;
				float duration = 0;
				float etime = atom_getfloat(e->time);
				int timematch = 0;
				if(fabs(pt.x - twpm_scale(etime, x->xmin, x->xmax, 0, r.width)) < 5){
					timematch = 1;
				}
				if(!notes){
					t_twpm_osc_msg *m = twpm_lookup_osc_msg(e, gensym("/marker"));
					if(m){
						if(timematch && (pt.y < 8 || pt.y > (r.height - 8))){
							twpm_unselect(x);
							e->selected = 1;
							e->note_sel = e->dur_sel = NULL;
							e->whatselected = NOTE;
							x->selected = e;
							goto out;
						}
					}
				}else{
					int i;
					for(i = 0; i < notes->argc; i++){
						float py = twpm_computeNotePos(atom_getfloat(notes->argv + i), r) - 3;
						if(durations){
							if(i < durations->argc){
								durptr = durations->argv + i;
								duration = atom_getfloat(durations->argv + i);
							}
						}

						if(fabs(py - pt.y) < 5){
							if(timematch){
								// selected a note
								twpm_unselect(x);
								e->selected = 1;
								e->whatselected = NOTE;
								e->note_sel = notes->argv + i;
								e->dur_sel = NULL;
								x->selected = e;
								goto out;
							}else if(fabs(pt.x - twpm_scale(etime + duration, x->xmin, x->xmax, 0, r.width)) < 5){
								// selected a duration
								twpm_unselect(x);
								e->selected = 1;
								e->whatselected = DURATION;
								e->note_sel = notes->argv + i;
								e->dur_sel = durptr;
								x->selected = e;
								goto out;
							}
						}else if(timematch == 1){
							// we're adding a note to a chord
							notes->argc++;
							if(notes->argc > notes->buflen){
								notes->argv = (t_atom *)sysmem_resizeptr(notes->argv, notes->buflen * 2 * sizeof(t_atom));
								notes->buflen *= 2;
							}
							atom_setfloat(notes->argv + notes->argc - 1, midic);
							twpm_unselect(x);
							x->selected = e;
							e->selected = 1;
							e->note_sel = notes->argv + notes->argc - 1;
							e->dur_sel = NULL;
							e->whatselected = NOTE;
							goto out;
						}
					}
				}
				e = e->next;
			}
		}
		twpm_unselect(x);
		e = twpm_insertEvent(x, time);
		e->selected = 1;
		x->selected = e;
		t_atom a;
		atom_setfloat(&a, midic);
		t_twpm_osc_msg *m = twpm_addMessageToEvent(e, ps_notes, 1, 1, &a);
		e->note_sel = m->argv;
		e->dur_sel = NULL;
		e->whatselected = NOTE;
	}
	}
 out:
	twpm_dumpcellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void twpm_mousedrag(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	switch(modifiers){
	default:
		if(x->selected){
			t_event *e = x->selected;
			switch(e->whatselected){
			case NOTE:
				{	
					float tt = twpm_scale(pt.x, 0, r.width, x->xmin, x->xmax);
					tt = twpm_find_closest_gridtime(x, tt);
					atom_setfloat(e->time, tt);
					twpm_reorderEvent(x, e);
					atom_setfloat(e->note_sel, twpm_uncomputeNotePos(pt.y, r));
				}
				break;
			case DURATION:
				{
					float dur = twpm_scale(pt.x, 0, r.width, x->xmin, x->xmax) - atom_getfloat(e->time);
					if(dur < 0){
						dur = 0;
					}
					atom_setfloat(e->dur_sel, dur);
				}
				break;
			case MARKER:
				atom_setfloat(e->time, twpm_scale(pt.x, 0, r.width, x->xmin, x->xmax));
				twpm_reorderEvent(x, e);
				break;
			}
		}
	}
	twpm_dumpcellblock(x);
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void twpm_mouseup(t_twpm *x, t_object *patcherview, t_pt pt, long modifiers){
}

t_event *twpm_insertEvent(t_twpm *x, float time){
	critical_enter(x->lock);
	t_event *elist = x->eventlist;
	t_event *e = twpm_event_alloc();

	t_twpm_osc_msg *m = twpm_addMessageToEvent(e, ps_notes, 0, 0, NULL);
	m->argv = (t_atom *)sysmem_newptr(DEF_NOTE_COUNT * sizeof(t_atom));
	m->argc = 0;
	m->buflen = DEF_NOTE_COUNT;

	t_atom t;
	atom_setfloat(&t, time);
	m = twpm_addMessageToEvent(e, ps_time, 1, 1, &t);
	e->time = m->argv;
	if(elist == NULL){
		e->prev = NULL;
		e->next = NULL;
		x->eventlist = e;
	}else if(time < atom_getfloat(x->eventlist->time)){
		e->prev = NULL;
		e->next = x->eventlist;
		x->eventlist->prev = e;
		x->eventlist = e;
	}else{
		int i = 1;
		t_event *current, *next;
		current = x->eventlist;
		next = current->next;
		while(next){
			if(time >= atom_getfloat(current->time) && time <= atom_getfloat(next->time)){
				current->next = e;
				next->prev = e;
				e->next = next;
				e->prev = current;
				goto out;
			}
			current = next;
			next = next->next;
			i++;
		}
		e->prev = current;
		e->next = NULL;
		current->next = e;
	}
 out:
	critical_exit(x->lock);
	return e;
}

void twpm_removeSelected(t_twpm *x){
	t_event *e = x->selected, *next;
	while(e){
		next = e->next_selected;
		twpm_removeEvent(x, e);
		e = next;
	}
	x->selected = NULL;
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box));
}

void twpm_removeEvent(t_twpm *x, t_event *event){
	if(!event){
		return;
	}
	critical_enter(x->lock);
	t_event *e = x->eventlist;
	int i = 0;
	while(e){
		if(e == event){
			if(e->next){
				e->next->prev = e->prev;
			}
			if(e->prev){
				e->prev->next = e->next;
			}
			if(e->next_selected){
				e->next_selected->prev_selected = e->prev_selected;
			}
			if(e->prev_selected){
				e->prev_selected->next_selected = e->next_selected;
			}
			if(i == 0){
				x->eventlist = e->next;
			}
			if(x->selected == e){
				x->selected = e->next_selected;
			}

			if(e){
				free(e);
			}
			goto out;
		}
		i++;
		e = e->next;
	}
 out:
	critical_exit(x->lock);
}


double twpm_scale(double f, double min_in, double max_in, double min_out, double max_out){
	float m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	return m * f + b;
}

double twpm_clip(double f, double min, double max){
	if(f < min){
		f = min;
	}
	if(f > max){
		f = max;
	}
	return f;
}

void twpm_dump(t_twpm *x){
	t_event *e = x->eventlist;
	while(e){
		twpm_outputEvent(x, x->outlet_info, ps_dump, e);
		e = e->next;
	}
}

void twpm_dumpcellblock(t_twpm *x){
	t_atom clearall[2] = {pa_clear, pa_all};
	outlet_anything(x->outlet_info, ps_cellblock, 2, clearall);
	t_event *e = x->selected;
	if(e){
		t_twpm_osc_msg *msg = e->messages_ll;
		long c = hashtab_getsize(e->messages_ht) - 1;
		while(msg){
			t_atom out[msg->argc + 4];
			t_atom *outptr = out;
			atom_setsym(outptr++, gensym("set"));
			atom_setlong(outptr++, 0);
			atom_setlong(outptr++, c--);
			atom_setsym(outptr++, msg->address);
			int i;
			for(i = 0; i < msg->argc; i++){
				*outptr++= msg->argv[i];
			}
			outlet_anything(x->outlet_info, ps_cellblock, outptr - out, out);
			msg = msg->next;
		}
	}
}

void twpm_xminmax(t_twpm *x, double min, double max){
	x->xmin = min;
	x->xmax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box));
}

void twpm_yminmax(t_twpm *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box));
}

void twpm_clear(t_twpm *x, t_symbol *pat){
	if(pat){
		if(pat == ps_notes || pat == ps_marker){
			twpm_unselect(x);
			t_event *e = x->eventlist, *next = NULL;
			int po, ao;
			while(e){
				next = e->next;
				t_twpm_osc_msg *m = e->messages_ll;
				while(m){
					if(osc_match(pat->s_name, m->address->s_name, &po, &ao)){
						if(e->prev){
							e->prev->next = e->next;
						}else{
							x->eventlist = e->next;
						}
						if(e->next){
							e->next->prev = e->prev;
						}
						twpm_clear_osc_messages(e);
						sysmem_freeptr(e);
					}
					m = m->next;
				}
				e = next;
			}
		}
	}else{
		x->selected = NULL;
		t_event *e = x->eventlist, *next = NULL;
		while(e){
			next = e->next;
			twpm_clear_osc_messages(e);
			sysmem_freeptr(e);
			e = next;
		}
		x->eventlist = NULL;
		x->left = NULL;
		x->right = NULL;
	}
	jbox_invalidate_layer((t_object *)x, x->pv, l_notes);
	jbox_redraw((t_jbox *)&(x->box));
}

void twpm_unselect(t_twpm *x){
	t_event *e = x->eventlist;
	while(e){
		e->selected = 0;
		e = e->next;
	}
	x->selected = NULL;
}

t_floatbuf *twpm_floatbuf_alloc(int ac, float *av){
	t_floatbuf *fb = (t_floatbuf *)sysmem_newptr(sizeof(t_floatbuf));
	fb->ac = 0;
	fb->av = NULL;
	twpm_floatbuf_store(fb, ac, av);
	return fb;
}

void twpm_floatbuf_store(t_floatbuf *fb, int ac, float *av){
	if(fb->av){
		fb->av = (float *)sysmem_resizeptr(fb->av, ac * sizeof(float));
	}else{
		fb->av = (float *)sysmem_newptr(ac * sizeof(float));
	}
	memcpy(fb->av, av, ac * sizeof(float));
	fb->ac = ac;
}

t_event *twpm_event_alloc(void){
	t_event *e = (t_event *)sysmem_newptr(1 * sizeof(t_event));
	e->time = NULL;
	e->selected = 0;
	e->note_sel = e->dur_sel = NULL;
	e->whatselected = 0;
	e->messages_ll = NULL;
	e->messages_ht = NULL;
	e->next = NULL;
	e->prev = NULL;
	e->next_selected = NULL;
	e->prev_selected = NULL;
	return e;
}

t_twpm_osc_msg *twpm_osc_msg_alloc(t_symbol *address, long argc, t_atom *argv){
	t_twpm_osc_msg *m = (t_twpm_osc_msg *)sysmem_newptr(sizeof(t_twpm_osc_msg));
	m->address = address;
	m->argc = argc;
	m->argv = (t_atom *)sysmem_newptr(argc * sizeof(t_atom));
	memcpy(m->argv, argv, argc * sizeof(t_atom));
	return m;
}

void twpm_osc_msg_free(t_twpm_osc_msg *m){
	if(m){
		if(m->argv){
			sysmem_freeptr(m->argv);
		}
		sysmem_freeptr(m);
	}
}

void twpm_osc_msg_freelist(t_twpm_osc_msg *m){
	t_twpm_osc_msg *next = NULL;
	while(m){
		next = m->next;
		twpm_osc_msg_free(m);
		m = next;
	}
}

void twpm_clear_osc_messages(t_event *e){
	twpm_osc_msg_freelist(e->messages_ll);
	e->messages_ll = NULL;
	hashtab_clear(e->messages_ht);
}

void twpm_post_osc_msg(t_twpm_osc_msg *m){
	char buffer[1024];
	char *ptr = buffer;
	int i;
	for(i = 0; i < m->argc; i++){
		switch(atom_gettype(m->argv + i)){
		case A_FLOAT:
			ptr += sprintf(ptr, "%f ", atom_getfloat(m->argv + i));
			break;
		case A_LONG:
			ptr += sprintf(ptr, "%ld ", atom_getlong(m->argv + i));
			break;
		case A_SYM:
			ptr += sprintf(ptr, "%s ", atom_getsym(m->argv + i)->s_name);
			break;
		}
	}
	post("%s %s", m->address->s_name, buffer);
}

void twpm_assist(t_twpm *x, void *b, long io, long num, char *s){ 
 	if(io == ASSIST_OUTLET){ 

 	}else{ 

 	} 
} 

void twpm_free(t_twpm *x){ 
	//dsp_freejbox((t_pxjbox *)x);
 	jbox_free((t_jbox *)x); 
	if(x->eventlist){
		t_event *e, *next;
		e = x->eventlist;
		while(e){
			next = e->next;
			sysmem_freeptr(e);
			e = next;
		}
	}
} 

int main(void){ 
 	t_class *c = class_new("timewarp.pitchmap", (method)twpm_new, (method)twpm_free, sizeof(t_twpm), 0L, A_GIMME, 0); 
	//class_dspinitjbox(c);

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

 	class_addmethod(c, (method)twpm_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)twpm_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)twpm_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)twpm_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)twpm_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)twpm_mouseup, "mouseup", A_CANT, 0); 
	class_addmethod(c, (method)twpm_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)twpm_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	class_addmethod(c, (method)twpm_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)twpm_int, "int", A_LONG, 0);
	class_addmethod(c, (method)twpm_clear, "clear", A_DEFSYM, 0);
	class_addmethod(c, (method)twpm_dump, "dump", 0);
	class_addmethod(c, (method)twpm_xminmax, "xminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)twpm_yminmax, "yminmax", A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)twpm_set_timepoints, "settimepoints", A_GIMME, 0);
	class_addmethod(c, (method)twpm_show_subdivs, "showsubdivs", A_GIMME, 0);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_twpm, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_twpm, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_twpm, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_twpm, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_DOUBLE(c, "noteheadsize", 0, t_twpm, noteheadsize);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "noteheadsize", 0, "54");

	// noteheadsize is a dummy--we're overriding the accessors.
	CLASS_ATTR_ATOM_VARSIZE(c, "points", 0, t_twpm, eventlist, noteheadsize, 4096);
	CLASS_ATTR_ACCESSORS(c, "points", twpm_events_get, twpm_events_set);
	CLASS_ATTR_SAVE(c, "points", 0);

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_twpm, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, "1. 1. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_ATTR_RGBA(c, "selectioncolor", 0, t_twpm, selectioncolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectioncolor", 0, "1. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectioncolor", 0, "rgba", "Selection Color"); 

 	CLASS_ATTR_RGBA(c, "gridcolor", 0, t_twpm, gridcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "gridcolor", 0, "0. 0. 1. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "gridcolor", 0, "rgba", "Grid Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

	l_background = gensym("l_background");
	l_notes = gensym("l_notes");
	ps_done = gensym("done");
	ps_dump = gensym("dump");
	ps_notes = gensym("/notes");
	ps_cellblock = gensym("cellblock");
	ps_clear = gensym("clear");
	ps_FullPacket = gensym("FullPacket");
	ps_durations = gensym("/durations");
	ps_time = gensym("/time");
	ps_marker = gensym("/marker");
	ps_beats = gensym("/beats");
	ps_subdivs = gensym("/subdivs");
	atom_setsym(&pa_clear, ps_clear);
	atom_setsym(&pa_dump, ps_dump);
	atom_setsym(&pa_all, gensym("all"));
 	class_register(CLASS_BOX, c); 
 	twpm_class = c; 

 	version(0); 
	
 	return 0; 
} 

void *twpm_new(t_symbol *s, long argc, t_atom *argv){ 
 	t_twpm *x = NULL; 
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

 	if(x = (t_twpm *)object_alloc(twpm_class)){ 
 		jbox_new((t_jbox *)x, boxflags, argc, argv); 

 		x->box.b_firstin = (void *)x; 
		//dsp_setupjbox((t_pxjbox *)x, 1);

		x->proxy = proxy_new((t_object *)x, 1, &(x->inlet));

		x->outlet_info = outlet_new((t_object *)x, NULL);
		x->outlet_main = outlet_new((t_object *)x, NULL);
		x->takingdump = 0;

		x->selected = NULL;
		x->eventlist = NULL;
		x->left = x->right = NULL;
		x->lasttime = 0.;

		x->subdivs_ht = hashtab_new(0);
		hashtab_flags(x->subdivs_ht, OBJ_FLAG_DATA);
		x->beatbuf.av = NULL;
		x->beatbuf.ac = 0;
		x->subdivs_to_show = NULL;
		x->num_subdivs_to_show = 0;

		critical_new(&(x->lock));
 		attr_dictionary_process(x, d); 
 		jbox_ready((t_jbox *)x); 
		//x->box.z_misc = Z_PUT_FIRST;

 		return x; 
 	} 
 	return NULL; 
} 


t_max_err twpm_events_get(t_twpm *x, t_object *attr, long *argc, t_atom **argv){
	return MAX_ERR_NONE;
}

t_max_err twpm_events_set(t_twpm *x, t_object *attr, long argc, t_atom *argv){
	return MAX_ERR_NONE;
}
