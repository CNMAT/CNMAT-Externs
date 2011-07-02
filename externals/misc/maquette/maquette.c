/*
  written by John MacCallum, The Center for New Music and Audio Technologies, 
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
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <time.h>
#include "osc.h"
#include "omax_util.h"
#include "avl.h"
#include "rtree.h"
#include "otable_util.h"
#include "maq_hashtab.h"

#define OUTLET_MAIN 0
#define OUTLET_INFO 1

enum{
	MAQ_NOTHING = 0,
	MAQ_MOVE,
	MAQ_RESIZE,
	MAQ_COPY
};

#define RESIZE_BOX_HANDLE_SIZE 7

typedef struct _maq_oscbndl{
	t_otable_oscbndl *bndl; 
	t_rtree_rect rect;
	long xmin_offset, xmax_offset, ymin_offset, ymax_offset;
	struct _maq_oscbndl *next;
} t_maq_oscbndl;

typedef struct _maq_db{
	t_maxdb maxdb;
	t_rtree *rtree;
	RUMATI_AVL_TREE *avltree_visible, *avltree_selected;
	int visible_count, selected_count;
	t_maq_hashtab *obj_tab;
} t_maq_db;

typedef struct _maq{ 
 	t_jbox box; 
 	void *outlets[2];
	t_symbol *name;
	t_maq_db *maq_db;
	t_otable_db *otable_db;
	t_object *pv;
	t_critical lock;
 	t_jrgba bgcolor, 
		bordercolor,
		boxfillcolor, 
		boxbordercolor, 
		resizehandlecolor, 
		selectedboxfillcolor, 
		selectedboxbordercolor, 
		selectedresizehandlecolor; 
	double xmin, xmax, ymin, ymax;
	t_pt lastmouse;
	clock_t dblclick_time;
	int dblclick_count;
	t_rect selbox;
	int op; // move, resize, copy
	int draw_rtree_nodes;
	char *xmin_address, *xmax_address, *ymin_address, *ymax_address; // 4-byte padded
	long xmin_address_len, xmax_address_len, ymin_address_len, ymax_address_len; // 4-byte padded
} t_maq; 

#define RTREE_RECT_TO_RECT(rect) ((t_rect){rect.xmin, rect.ymin, rect.xmax - rect.xmin, rect.ymax - rect.ymin})
#define RECT_TO_RTREE_RECT(rect) ((t_rtree_rect){rect.x, rect.x + rect.width, rect.y, rect.y + rect.height})

#define BOX_RESIZE_HANDLE_SIZE 10

void *maq_class; 

void maq_paint(t_maq *x, t_object *patcherview); 
t_otable_oscbndl *maq_insert_with_coords(t_maq *x, t_rect view_rect, t_pt screen_coords, long len, char *ptr);
t_otable_oscbndl *maq_insert(t_maq *x, long len, char *ptr);
t_otable_oscbndl *maq_insert_with_rtree_rect(t_maq *x, t_rtree_rect r, long len, char *ptr);
t_otable_oscbndl *maq_insert_bndl_with_rtree_rect(t_maq *x, t_rtree_rect r, t_otable_oscbndl *b);
t_rtree_rect maq_make_default_rtree_rect(t_maq *x);
t_rect maq_scale_rect(t_rect rect, t_rect src_rect, t_rect dest_rect);
void maq_assist(t_maq *x, void *b, long m, long a, char *s); 
void maq_free(t_maq *x); 
void maq_output_bundle(t_maq *x, void *outlet, long len, char *ptr);
void maq_rebuild_visible_db(t_maq *x);
t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 
void maq_maxdb_notify(t_maq *x, t_symbol *notification_type, void *obj, t_object *sender);
void maq_rebuil_dvisible_db(t_maq *x);
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers); 
void maq_mousemove(t_maq *x, t_object *patcherview, t_pt pt, long modifiers);
void maq_dblclick(t_maq *x);
void maq_clear(t_maq *x);
void maq_refer(t_maq *x, t_symbol *name);
double maq_scale(double f, double min_in, double max_in, double min_out, double max_out);
double maq_clip(double f, double min, double max);
void maq_xminmax(t_maq *x, double min, double max);
void maq_yminmax(t_maq *x, double min, double max);
void maq_print_rect(char *prefix, t_rect rect);
void maq_print_rtree_rect(char *prefix, t_rtree_rect rect);
void maq_rtree_destructor(t_maq *x, t_otable_oscbndl *bndl);
void maq_make_addresses(t_maq *x);
int main(void); 
void *maq_new(t_symbol *s, long argc, t_atom *argv); 

static t_symbol *ps_FullPacket, *ps_xmin, *ps_xmax, *ps_ymin, *ps_ymax;

void maq_draw_rtree_node(t_maq *x, t_jgraphics *gg, t_rtree_node *n, t_rect view){
	if(n->node_type == RTREE_DATA){
		return;
	}
	t_rect r = (t_rect){
		maq_scale(n->rect.xmin, x->xmin, x->xmax, 0, view.width),
		maq_scale(n->rect.ymax, x->ymin, x->ymax, view.height, 0),
		maq_scale(n->rect.xmax - n->rect.xmin, x->xmin, x->xmax, 0, view.width),
		maq_scale(n->rect.ymax - n->rect.ymin, x->ymin, x->ymax, 0, view.height)
	};
	t_jrgba black = (t_jrgba){0., 0., 0., 1.};
	jgraphics_set_source_jrgba(gg, &black);
	jgraphics_set_line_width(gg, 1.);
	jgraphics_set_dash(gg, (double[]){4., 4.}, 2, 0);
	jgraphics_rectangle(gg, r.x, r.y, r.width, r.height);
	jgraphics_stroke(gg);
	t_rtree_node *e = n->entries;
	while(e){
		maq_draw_rtree_node(x, gg, e, view);
		e = e->next;
	}
}

void maq_paint(t_maq *x, t_object *patcherview){ 
	int visible_count = x->maq_db->visible_count;
	double xmin = x->xmin, xmax = x->xmax, ymin = x->ymin, ymax = x->ymax;
	t_jrgba bgcolor = x->bgcolor, 
		bordercolor = x->bordercolor,
		boxfillcolor = x->boxfillcolor, 
		boxbordercolor = x->boxbordercolor, 
		resizehandlecolor = x->resizehandlecolor, 
		selectedboxfillcolor = x->selectedboxfillcolor, 
		selectedboxbordercolor = x->selectedboxbordercolor, 
		selectedresizehandlecolor = x->selectedresizehandlecolor; 
	x->pv = patcherview;
 	t_rect r; 
 	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &r); 

	// draw the outline of the box 
	jgraphics_set_source_jrgba(g, &bordercolor); 
	jgraphics_set_line_width(g, 1); 
	jgraphics_rectangle(g, 0., 0., r.width, r.height); 
	jgraphics_stroke(g); 

	jgraphics_set_source_jrgba(g, &bgcolor); 
	jgraphics_rectangle(g, 0., 0., r.width, r.height); 
	jgraphics_fill(g); 

	if(!x->maq_db){
		return;
	}

	t_jgraphics *gg = g; //jbox_start_layer((t_object *)x, patcherview, l_main, r.width, r.height);
	if(gg){
		critical_enter(x->lock);
		maxdb_lock((t_maxdb *)x->maq_db);
		t_maq_oscbndl *b = (t_maq_oscbndl *)rumati_avl_get_smallest(x->maq_db->avltree_visible);
		if(!b){
			return;
		}
		t_rect rects[visible_count];
		int selected[visible_count];
		int n = 0, i;
		while(b){
			t_rtree_rect wr = b->rect;
			t_rtree_rect sr = (t_rtree_rect){maq_scale(wr.xmin, xmin, xmax, 0, r.width),
							 maq_scale(wr.xmax, xmin, xmax, 0, r.width),
							 maq_scale(wr.ymax, ymin, ymax, r.height, 0),
							 maq_scale(wr.ymin, ymin, ymax, r.height, 0)};
			rects[n] = (t_rect){sr.xmin,
					    sr.ymin,
					    sr.xmax - sr.xmin,
					    sr.ymax - sr.ymin};
			t_maq_oscbndl *sel = rumati_avl_get(x->maq_db->avltree_selected, (void *)b);
			if(sel){
				selected[n] = 1;
			}else{
				selected[n] = 0;
			}
			b = rumati_avl_get_greater_than(x->maq_db->avltree_visible, (void *)b);
			n++;
		}
		n = visible_count;

		maxdb_unlock((t_maxdb *)x->maq_db);
		critical_exit(x->lock);
		for(i = 0; i < n; i++){
			if(selected[i] == 1){
				jgraphics_set_line_width(gg, 3.);
				// box fill
				jgraphics_set_source_jrgba(gg, &selectedboxfillcolor);
				jgraphics_rectangle(gg, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
				jgraphics_fill(gg);
				// box border
				jgraphics_set_source_jrgba(gg, &selectedboxbordercolor);
				jgraphics_rectangle(gg, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
				jgraphics_stroke(gg);
				// resize box handle
				jgraphics_set_source_jrgba(gg, &selectedresizehandlecolor);
				jgraphics_rectangle(gg, rects[i].x + rects[i].width - RESIZE_BOX_HANDLE_SIZE, rects[i].y + rects[i].height - RESIZE_BOX_HANDLE_SIZE, RESIZE_BOX_HANDLE_SIZE, RESIZE_BOX_HANDLE_SIZE);
				jgraphics_fill(gg);
			}else{
				jgraphics_set_line_width(gg, 1.);
				// box fill
				jgraphics_set_source_jrgba(gg, &boxfillcolor);
				jgraphics_rectangle(gg, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
				jgraphics_fill(gg);
				// box border
				jgraphics_set_source_jrgba(gg, &boxbordercolor);
				jgraphics_rectangle(gg, rects[i].x, rects[i].y, rects[i].width, rects[i].height);
				jgraphics_stroke(gg);
				// resize box handle
				jgraphics_set_source_jrgba(gg, &resizehandlecolor);
				jgraphics_rectangle(gg, rects[i].x + rects[i].width - RESIZE_BOX_HANDLE_SIZE, rects[i].y + rects[i].height - RESIZE_BOX_HANDLE_SIZE, RESIZE_BOX_HANDLE_SIZE, RESIZE_BOX_HANDLE_SIZE);
				jgraphics_fill(gg);
			}
		}
		if(x->selbox.width != 0 && x->selbox.height != 0){
			t_jrgba c = (t_jrgba){0., 0., 0., 1.};
			jgraphics_set_line_width(gg, 1.);
			jgraphics_set_source_jrgba(gg, &c);
			jgraphics_rectangle(gg, x->selbox.x, x->selbox.y, x->selbox.width, x->selbox.height);
			jgraphics_stroke(gg);
		}
		if(x->draw_rtree_nodes){
			t_rtree_node *n = x->maq_db->rtree->root;
			maq_draw_rtree_node(x, gg, n, r);
		}
	}

	//jbox_end_layer((t_object *)x, patcherview, l_main);
	//jbox_paint_layer((t_object *)x, patcherview, l_main, 0, 0);
}

void maq_fullPacket(t_maq *x, long len, long ptr){
	maq_insert(x, len, (char *)ptr);
}

void maq_anything(t_maq *x, t_symbol *msg, int argc, t_atom *argv){
	if(msg == gensym("drawrtreenodes")){
		x->draw_rtree_nodes = atom_getlong(argv);
		jbox_redraw((t_jbox *)&(x->box)); 
	}else if(msg == gensym("printtree")){
		rtree_print(x->maq_db->rtree, (void *)post);
	}else{
		object_error((t_object *)x, "doesn't respond to the message \"%s\"", msg->s_name);
		return;
	}
}

t_rect maq_scale_rect(t_rect rect, t_rect src_rect, t_rect dest_rect){
	double src_rect_xmax = src_rect.x + src_rect.width;
	double src_rect_ymax = src_rect.y + src_rect.height;
	double dest_rect_xmax = dest_rect.x + dest_rect.width;
	double dest_rect_ymax = dest_rect.y + dest_rect.height;
	return (t_rect){maq_scale(rect.x, src_rect.x, src_rect_xmax, dest_rect.x, dest_rect_xmax),
			maq_scale(rect.y, src_rect.y, src_rect_ymax, dest_rect.y, dest_rect_ymax),
			maq_scale(rect.width, src_rect.x, src_rect_xmax, dest_rect.x, dest_rect_xmax),
			maq_scale(rect.height, src_rect.y, src_rect_ymax, dest_rect.y, dest_rect_ymax)};
}

t_max_err maq_notify(t_maq *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
		////jbox_invalidate_layer((t_object *)x, x->pv, l_main);
		if(attrname == ps_xmin || attrname == ps_xmax || attrname == ps_ymin || attrname == ps_ymax){
			maq_rebuild_visible_db(x);
		}
		if(attrname == gensym("name")){
			maq_make_addresses(x);
			maq_refer(x, x->name);
		}
 		jbox_redraw((t_jbox *)&(x->box)); 
	} 
	return 0; 
}

void maq_freebundle(t_maq *x, t_otable_oscbndl *b){
	if(b){
		osc_mem_free(b);
	}
}

void maq_delete(t_maq *x){
	maxdb_lock((t_maxdb *)x->maq_db);
	t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
	while(b){
		t_maq_oscbndl *next = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
		rtree_remove(x->maq_db->rtree, b->rect, b);
		rumati_avl_delete(x->maq_db->avltree_selected, b, NULL);
		otable_util_doRemove((t_object *)x, x->otable_db, b->bndl);
		//maq_freebundle(x, b);
		b = next;
	}
	maxdb_unlock((t_maxdb *)x->maq_db);
	otable_util_renumber((t_object *)x, x->otable_db);
	maq_rebuild_visible_db(x);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_output_selected(t_maq *x){
	maxdb_lock((t_maxdb *)x->maq_db);
	maxdb_lock((t_maxdb *)x->otable_db);
	long len = 0;
	long nbndls = 0;
	t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
	while(b){
		nbndls++;
		len += b->bndl->len;
		b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
	}
	char buf[len];
	memset(buf, '\0', len);
	long lens[nbndls];
	char *ptr = buf;
	int i = 0;
	b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
	while(b){
		lens[i++] = b->bndl->len;
		memcpy(ptr, b->bndl->ptr, b->bndl->len);
		ptr += b->bndl->len;
		b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
	}
	maxdb_unlock((t_maxdb *)x->otable_db);
	maxdb_unlock((t_maxdb *)x->maq_db);
	ptr = buf;
	for(i = 0; i < nbndls; i++){
		maq_output_bundle(x, x->outlets[OUTLET_MAIN], lens[i], ptr);
		ptr += lens[i];
	}
}

void maq_output_bundle(t_maq *x, void *outlet, long len, char *ptr){
	t_atom out[2];
	atom_setlong(out, len);
	atom_setlong(out + 1, (long)ptr);
	outlet_anything(outlet, ps_FullPacket, 2, out);
}

void maq_rebuild_visible_db(t_maq *x){
	if(!x->maq_db){
		return;
	}
	// right now we're just building an avl tree for the visible db, but it should probably
	// also be an rtree
	long argc = 0;
	void **argv = NULL;
	double xmin = x->xmin, xmax = x->xmax, ymin = x->ymin, ymax = x->ymax;
	t_rtree_rect rect = (t_rtree_rect){xmin, xmax, ymin, ymax};
	//critical_enter(x->lock);
	//maxdb_lock((t_maxdb *)x->maq_db);
	rumati_avl_clear(x->maq_db->avltree_visible, NULL);
	rtree_search(x->maq_db->rtree, rect, &argc, &argv);
	if(argc){
		x->maq_db->visible_count = argc;
		int i;
		for(i = 0; i < argc; i++){
			rumati_avl_put(x->maq_db->avltree_visible, (t_maq_oscbndl *)argv[i], NULL);
		}
	}
	//maxdb_unlock((t_maxdb *)x->maq_db);
	//critical_exit(x->lock);
	if(argv){
		free(argv);
	}
}

t_otable_oscbndl *maq_insert_with_coords(t_maq *x, t_rect view_rect, t_pt screen_coords, long len, char *ptr){
	double xmin = x->xmin, xmax = x->xmax, ymin = x->ymin, ymax = x->ymax;
	t_rtree_rect r = (t_rtree_rect){maq_scale(screen_coords.x, 0, view_rect.width, xmin, xmax),
					maq_scale(screen_coords.x + 50, 0, view_rect.width, xmin, xmax),
					maq_scale(screen_coords.y + 30, view_rect.height, 0, ymin, ymax),
					maq_scale(screen_coords.y, view_rect.height, 0, ymin, ymax)};
	return maq_insert_with_rtree_rect(x, r, len, ptr);
}

t_otable_oscbndl *maq_insert(t_maq *x, long len, char *ptr){
	return maq_insert_with_rtree_rect(x, maq_make_default_rtree_rect(x), len, ptr);
}

t_otable_oscbndl *maq_insert_with_rtree_rect(t_maq *x, t_rtree_rect r, long len, char *ptr){
	t_otable_oscbndl *b = otable_util_doStore((t_object *)x, x->otable_db, NULL, x->otable_db->monotonic_counter++, len, ptr);
	return maq_insert_bndl_with_rtree_rect(x, r, b);
}

t_otable_oscbndl *maq_insert_bndl_with_rtree_rect(t_maq *x, t_rtree_rect r, t_otable_oscbndl *b){
	// allocate a t_maq_oscbndl and store a pointer to the t_otable_oscbndl in it.
	// below, we'll stick the t_maq_oscbndl in a hashtable with the address of the t_otable_oscbndl
	// as the key so that we have a way to associate the t_otable_oscbndl with the t_maq_oscbndl
	t_maq_oscbndl *mb = (t_maq_oscbndl *)osc_mem_alloc(sizeof(t_maq_oscbndl));
	mb->bndl = b;
	mb->xmin_offset = mb->xmax_offset = mb->ymin_offset = mb->ymax_offset = 0;

	// make an osc bundle with the box dimensions
	long buflen = 16 // bundle header just in case we don't have one
		+ b->len // length of the bundle if there is one 
		// the following may not be neccessary if these messages are already in the bundle
		// that was passed to us.  but we'll do this anyway and waste a few bytes
		// rather than go through and look everything up
		+ (12 * 4) // size, typetags, and data for the rect
		+ x->xmin_address_len // rect addresses
		+ x->xmax_address_len 
		+ x->ymin_address_len 
		+ x->ymax_address_len;
	char *buf = (char *)osc_mem_alloc(buflen); // there may be 100 bytes or so of wasted memory here
	memcpy(buf, b->ptr, b->len);
	char *p = buf + b->len;
	char *addresses[4] = {x->xmin_address, x->xmax_address, x->ymin_address, x->ymax_address};
	long *offsets[4] = {&(mb->xmin_offset), &(mb->xmax_offset), &(mb->ymin_offset), &(mb->ymax_offset)};
	long address_lens[4] = {x->xmin_address_len, x->xmax_address_len, x->ymin_address_len, x->ymax_address_len};
	float floatrect[4] = {(float)r.xmin, (float)r.xmax, (float)r.ymin, (float)r.ymax};
	double *rptrs[4] = {&(r.xmin), &(r.xmax), &(r.ymin), &(r.ymax)};
	int i;
	if(b->ptr){
		// check for dimensions and add them if necessary
		for(i = 0; i < 4; i++){
			t_osc_msg *msg = NULL;
			osc_bundle_lookupAddress_s(b->len, b->ptr, addresses[i], &msg, 1);
			if(msg){
				*offsets[i] = msg->argv - b->ptr;
				switch(msg->typetags[1]){
				case 'i':
					*rptrs[i] = (double)ntoh32(*((uint32_t *)msg->argv));
					break;
				case 'f':
					{
						uint32_t ll = ntoh32(*((uint32_t *)(msg->argv)));
						float f = *((float *)&ll);
						*rptrs[i] = (double)f;
					}
					break;
				}
			}else{
				*((uint32_t *)p) = hton32(8 + address_lens[i]);
				p += 4;
				memcpy(p, addresses[i], address_lens[i]);
				p += address_lens[i];
				*p++ = ',';
				*p++ = 'f';
				*p++ = '\0';
				*p++ = '\0';
				*((uint32_t *)p) = hton32(*((uint32_t *)(floatrect + i)));
				*offsets[i] = p - buf;
				p += 4;
			}
		}
	}else{
		strncpy(p, "#bundle\0\0\0\0\0\0\0\0\0", 16);
		p += 16;
		for(i = 0; i < 4; i++){
			*((uint32_t *)p) = hton32(8 + address_lens[i]);
			p += 4;
			memcpy(p, addresses[i], address_lens[i]);
			p += address_lens[i];
			*p++ = ',';
			*p++ = 'f';
			*p++ = '\0';
			*p++ = '\0';
			*((uint32_t *)p) = hton32(*((uint32_t *)(floatrect + i)));
			*offsets[i] = p - buf;
			p += 4;
		}
	}
	mb->rect = r;
	b->ptr = buf;
	b->len = p - buf;
	critical_enter(x->lock);
	maq_hashtab_store(x->maq_db->obj_tab, (void *)b, (void *)mb);
	rtree_insert(x->maq_db->rtree, r, (void *)mb);
	critical_exit(x->lock);
	maq_rebuild_visible_db(x);
	jbox_redraw(&(x->box));
	return b;
}

int maq_should_resize(t_maq *x, t_maq_oscbndl *bndl, t_rect view, t_pt wc){
	// if we're in the bottom right corner, prepare to do a resize
	t_rtree_rect resize_handle = bndl->rect;
	resize_handle.xmin = resize_handle.xmax - maq_scale(BOX_RESIZE_HANDLE_SIZE, 0, view.width, x->xmin, x->xmax);
	resize_handle.ymax = resize_handle.ymin + maq_scale(BOX_RESIZE_HANDLE_SIZE, 0, view.height, x->ymin, x->ymax);
	if(wc.x >= resize_handle.xmin && wc.x <= resize_handle.xmax &&
	   wc.y >= resize_handle.ymin && wc.y <= resize_handle.ymax){
		return MAQ_RESIZE;
	}
	return 0;
}



// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
#define DBLCLICK_TIME 0.4
void maq_mousedown(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){ 
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	x->lastmouse = pt;
	x->selbox = (t_rect){pt.x, pt.y, 0, 0};

	double xmin = x->xmin, xmax = x->xmax, ymin = x->ymin, ymax = x->ymax;
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, xmin, xmax), maq_scale(pt.y, r.height, 0, ymin, ymax)};

	switch(modifiers){
	case 0x10:
	case 0x110:
		{
			long argc = 0;
			t_maq_oscbndl **argv = NULL;
			maxdb_lock((t_maxdb *)x->maq_db);
			rtree_search(x->maq_db->rtree, (t_rtree_rect){wc.x, wc.x, wc.y, wc.y}, &argc, (void ***)(&argv));
			if(argc){
				x->op = MAQ_MOVE;
				int i;
				long long highest_idx = -1;
				t_maq_oscbndl *highest = NULL;
				for(i = 0; i < argc; i++){
					if(argv[i]->bndl->id > highest_idx){
						highest_idx = argv[i]->bndl->id;
						highest = argv[i];
					}
				}
				t_maq_oscbndl *bb = rumati_avl_get(x->maq_db->avltree_selected, (void *)highest);
				if(!bb){
					// if the box isn't already in the tree, clear it and stick it in 
					//t_otable_oscbndl *copy = otable_util_makebundle((t_object *)x, NULL, 0, 0, NULL);
					//*copy = *highest;
					rumati_avl_clear(x->maq_db->avltree_selected, NULL);
					rumati_avl_put(x->maq_db->avltree_selected, (void *)highest, NULL);
				}
				// if we're in the bottom right corner, prepare to do a resize
				if(maq_should_resize(x, highest, r, wc)){
					x->op = MAQ_RESIZE;
				}
				free(argv);
			}else{
				rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			}
			maxdb_unlock((t_maxdb *)x->maq_db);
			maq_output_selected(x);

			// double click stuff
			clock_t time = clock();
			if(x->dblclick_count == 0){
				x->dblclick_time = time;
				x->dblclick_count++;
			}else{
				x->dblclick_count++;
			}
		}
		break;
	case 0x12:
	case 0x112:
		// shift click--select
		{
			long argc = 0;
			t_maq_oscbndl **argv = NULL;
			maxdb_lock((t_maxdb *)x->maq_db);
			rtree_search(x->maq_db->rtree, (t_rtree_rect){wc.x, wc.x, wc.y, wc.y}, &argc, (void ***)(&argv));
			if(argc){
				int i;
				long long highest_idx = -1;
				t_maq_oscbndl *highest = NULL;
				for(i = 0; i < argc; i++){
					if(argv[i]->bndl->id > highest_idx){
						highest_idx = argv[i]->bndl->id;
						highest = argv[i];
					}
				}
				t_maq_oscbndl *bb = rumati_avl_get(x->maq_db->avltree_selected, (void *)highest);
				if(!bb){
					//t_maq_oscbndl *copy = otable_util_makebundle((t_object *)x, NULL, 0, 0, NULL);
					//*copy = *highest;
					rumati_avl_put(x->maq_db->avltree_selected, (void *)highest, NULL);
					// if we're in the bottom right corner, prepare to do a resize
					if(maq_should_resize(x, highest, r, wc)){
						x->op = MAQ_RESIZE;
					}
				}else{
					rumati_avl_delete(x->maq_db->avltree_selected, (void *)bb, NULL);
				}
				free(argv);
			}else{
				rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			}
			maxdb_unlock((t_maxdb *)x->maq_db);
		}
		break;

	case 0x18:
	case 0x118:
		// option click--copy
		{
			x->op = MAQ_COPY;
			long argc = 0;
			t_maq_oscbndl **argv = NULL;
			maxdb_lock((t_maxdb *)x->maq_db);
			rtree_search(x->maq_db->rtree, (t_rtree_rect){wc.x, wc.x, wc.y, wc.y}, &argc, (void ***)(&argv));
			if(argc){
				int i;
				long long highest_idx = -1;
				t_maq_oscbndl *highest = NULL;
				for(i = 0; i < argc; i++){
					if(argv[i]->bndl->id > highest_idx){
						highest_idx = argv[i]->bndl->id;
						highest = argv[i];
					}
				}
				t_maq_oscbndl *bb = rumati_avl_get(x->maq_db->avltree_selected, (void *)highest);
				if(!bb){
					rumati_avl_put(x->maq_db->avltree_selected, (void *)highest, NULL);
				}
				free(argv);
			}else{
				rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			}
			maxdb_unlock((t_maxdb *)x->maq_db);
		}
		break;
	case 0x11:
	case 0x111:
		// command click--insert box
		{
			maq_insert_with_coords(x, r, pt, 0, NULL);
		}
		break;
	default:
		{
			maxdb_lock((t_maxdb *)x->maq_db);
			rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			maxdb_unlock((t_maxdb *)x->maq_db);
		}
	}
	maxdb_notify((t_object *)x, (t_maxdb *)x->maq_db, gensym("refresh"), NULL);
	//jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mousedrag(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	double xmin = x->xmin, xmax = x->xmax, ymin = x->ymin, ymax = x->ymax;
	t_pt lastmouse = x->lastmouse;
	t_pt wc = (t_pt){maq_scale(pt.x, 0, r.width, xmin, xmax), maq_scale(pt.y, r.height, 0, ymin, ymax)};
	t_pt lmwc = (t_pt){maq_scale(lastmouse.x, 0, r.width, xmin, xmax), maq_scale(lastmouse.y, r.height, 0, ymin, ymax)};
	t_pt delta = (t_pt){wc.x - lmwc.x, wc.y - lmwc.y};

	switch(modifiers){
	case 0x18:
	case 0x118:
		if(x->op == MAQ_COPY){
			maxdb_lock((t_maxdb *)x->maq_db);
			t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
			t_maq_oscbndl *copylist = NULL;
			while(b){
				t_maq_oscbndl *copy = (t_maq_oscbndl *)osc_mem_alloc(sizeof(t_maq_oscbndl));
				// we don't have to recompute the offsets
				*copy = *b;
				// we really shouldn't access any of this stuff directly...
				long long id = x->otable_db->monotonic_counter++;
				t_symbol *key = NULL;
				if(b->bndl->key){
					char buf[128];
					sprintf(buf, "%s_copy", b->bndl->key->s_name);
					key = gensym("buf");
				}
				copy->bndl = otable_util_doStore((t_object *)x, x->otable_db, key, id, b->bndl->len, b->bndl->ptr);
				copy->next = copylist;
				copylist = copy;
				rtree_insert(x->maq_db->rtree, copy->rect, copy);
				maq_hashtab_store(x->maq_db->obj_tab, copy->bndl, copy);
				b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
			}
			rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			while(copylist){
				//t_maq_oscbndl *copy = otable_util_copybundle((t_object *)x, copylist);
				rumati_avl_put(x->maq_db->avltree_selected, copylist, NULL);
				copylist = copylist->next;
			}
			maxdb_unlock((t_maxdb *)x->maq_db);
			maq_rebuild_visible_db(x);
		}
		x->op = MAQ_MOVE;
		// fall through
	case 0x10:
	case 0x110:
		switch(x->op){
		case MAQ_MOVE:
			{
				maxdb_lock((t_maxdb *)x->maq_db);
				maxdb_lock((t_maxdb *)x->otable_db);
				t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
				while(b){
					rtree_remove(x->maq_db->rtree, b->rect, b);
					b->rect.xmin += delta.x;
					b->rect.xmax += delta.x;
					b->rect.ymin += delta.y;
					b->rect.ymax += delta.y;
					float f = b->rect.xmin;
					*((uint32_t *)(b->bndl->ptr + b->xmin_offset)) = hton32(*((uint32_t *)(&f)));
					f = b->rect.xmax;
					*((uint32_t *)(b->bndl->ptr + b->xmax_offset)) = hton32(*((uint32_t *)(&f)));
					f = b->rect.ymin;
					*((uint32_t *)(b->bndl->ptr + b->ymin_offset)) = hton32(*((uint32_t *)(&f)));
					f = b->rect.ymax;
					*((uint32_t *)(b->bndl->ptr + b->ymax_offset)) = hton32(*((uint32_t *)(&f)));
					rtree_insert(x->maq_db->rtree, b->rect, b);
					b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
				}
				maxdb_unlock((t_maxdb *)x->otable_db);
				maxdb_unlock((t_maxdb *)x->maq_db);
				maq_output_selected(x);
			}
			break;
		case MAQ_RESIZE:
			{
				maxdb_lock((t_maxdb *)x->maq_db);
				maxdb_lock((t_maxdb *)x->otable_db);
				t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
				while(b){
					rtree_remove(x->maq_db->rtree, b->rect, b);

					double xmin = b->rect.xmin, ymax = b->rect.ymax;
					double xmax = b->rect.xmax + delta.x;
					double ymin = b->rect.ymin + delta.y;
					double diff_sc = maq_scale(xmax - xmin, x->xmin, x->xmax, 0, r.width);
					double resize_box_handle_size_wc = maq_scale(RESIZE_BOX_HANDLE_SIZE, 0, r.width, x->xmin, x->xmax);
					if(diff_sc < RESIZE_BOX_HANDLE_SIZE){
						xmax = xmin + resize_box_handle_size_wc;
					}
					diff_sc = maq_scale(ymax - ymin, x->ymin, x->ymax, 0, r.height);
					resize_box_handle_size_wc = maq_scale(RESIZE_BOX_HANDLE_SIZE, 0, r.height, x->ymin, x->ymax);
					if(diff_sc < RESIZE_BOX_HANDLE_SIZE){
						ymin = ymax - resize_box_handle_size_wc;
					}
					b->rect.xmax = xmax;
					b->rect.ymin = ymin;
					float f = (float)xmax;
					*((uint32_t *)(b->bndl->ptr + b->xmax_offset)) = hton32(*((uint32_t *)(&f)));
					f = (float)ymin;
					*((uint32_t *)(b->bndl->ptr + b->ymin_offset)) = hton32(*((uint32_t *)(&f)));
					rtree_insert(x->maq_db->rtree, b->rect, b);
					b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
				}
				maxdb_unlock((t_maxdb *)x->otable_db);
				maxdb_unlock((t_maxdb *)x->maq_db);
				maq_output_selected(x);
			}
			break;
		}
	default:
		if(x->op == MAQ_NOTHING){
			x->selbox.width = pt.x - x->selbox.x;
			x->selbox.height = pt.y - x->selbox.y;
		}
	}
	x->lastmouse = pt;
	maxdb_notify((t_object *)x, (t_maxdb *)x->maq_db, gensym("refresh"), NULL);
	//jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mouseup(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);

	x->op = MAQ_NOTHING;

	switch(modifiers){
	case 0x10:
	case 0x110:
		if(x->selbox.width != 0 && x->selbox.height != 0){
			maxdb_lock((t_maxdb *)x->maq_db);
			rumati_avl_clear(x->maq_db->avltree_selected, NULL);
			maxdb_unlock((t_maxdb *)x->maq_db);
		}
		{
			x->dblclick_count++;
			if(x->dblclick_count == 4){
				clock_t time = clock();
				if((((double)(time - x->dblclick_time)) / CLOCKS_PER_SEC) < DBLCLICK_TIME){
					maq_dblclick(x);
				}
				x->dblclick_count = 0;
			}
		}
	case 0x12:
	case 0x112:
		if(x->selbox.width != 0 && x->selbox.height != 0){
			maxdb_lock((t_maxdb *)x->maq_db);
			t_rtree_rect selbox = (t_rtree_rect){maq_scale(x->selbox.x, 0, r.width, x->xmin, x->xmax),
							     maq_scale(x->selbox.x + x->selbox.width, 0, r.width, x->xmin, x->xmax),
							     maq_scale(x->selbox.y + x->selbox.height, r.height, 0, x->ymin, x->ymax),
							     maq_scale(x->selbox.y, r.height, 0, x->ymin, x->ymax)
			};
			long argc = 0;
			t_maq_oscbndl **argv = NULL;
			rtree_search(x->maq_db->rtree, selbox, &argc, (void ***)(&argv));
			if(argc){
				int i;
				for(i = 0; i < argc; i++){
					t_maq_oscbndl *bb = rumati_avl_get(x->maq_db->avltree_selected, (void *)argv[i]);
					if(!bb){
						//t_maq_oscbndl *copy = otable_util_makebundle((t_object *)x, NULL, 0, 0, NULL);
						//*copy = *(argv[i]);
						rumati_avl_put(x->maq_db->avltree_selected, (void *)argv[i], NULL);
					}
				}
				free(argv);
			}else{
				//rumati_avl_clear(x->maq_db->avltree_selected, (void *)maq_freebundle);
			}
			maxdb_unlock((t_maxdb *)x->maq_db);
		}
		break;
	}
	maxdb_notify((t_object *)x, (t_maxdb *)x->maq_db, gensym("refresh"), NULL);
	//jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	x->selbox = (t_rect){0., 0., 0., 0.};
	jbox_redraw((t_jbox *)&(x->box)); 
}

void maq_mousemove(t_maq *x, t_object *patcherview, t_pt pt, long modifiers){
}

void maq_dblclick(t_maq *x){
	maxdb_lock((t_maxdb *)x->maq_db);
	long indexes[x->otable_db->count];
	t_maq_oscbndl *b = rumati_avl_get_smallest(x->maq_db->avltree_selected);
	int n = 0;
	while(b){
		indexes[n++] = b->bndl->id;
		b = rumati_avl_get_greater_than(x->maq_db->avltree_selected, b);
	}
	maxdb_unlock((t_maxdb *)x->maq_db);
	int i;
	for(i = 0; i < n; i++){
		outlet_int(x->outlets[OUTLET_INFO], indexes[i]);
	}
}

void maq_destroy_db(t_maq *x, t_maq_db *db){
	maxdb_remove_notification((t_maxdb *)db, (t_object *)x);
	maq_hashtab_destroy(db->obj_tab);
	rumati_avl_destroy(db->avltree_visible, NULL);
	rumati_avl_destroy(db->avltree_selected, NULL);
	rtree_destroy(db->rtree, (t_rtree_dtor)maq_freebundle, x);
	x->maq_db = NULL;
}

int maq_comp(void *udata, void *value1, void *value2){
	// i'm not checking for null values here--if one or both are null, that's a bug somewhere
	// else and i want it to crash
	long long id1, id2;
	id1 = ((t_maq_oscbndl *)value1)->bndl->id;
	id2 = ((t_maq_oscbndl *)value2)->bndl->id;
	if(id1 < id2){
		return -1;
	}else if(id1 == id2){
		return 0;
	}else{
		return 1;
	}
}

t_maq_db *maq_make_db(t_maq *x, t_symbol *name){
	t_maq_db *db = (t_maq_db *)maxdb_alloc(maxdb_mangle_name("maquette", name), 
					       sizeof(t_maq_db), 
					       (t_maxdb_dtor)maq_destroy_db,
					       x);
	if(!db){
		return NULL;
	}
	db->obj_tab = maq_hashtab_new();
	db->rtree = rtree_new();
	rumati_avl_new(&(db->avltree_visible), maq_comp, (void *)x);
	rumati_avl_new(&(db->avltree_selected), maq_comp, (void *)x);
	db->visible_count = db->selected_count = 0;
	return db;
}

t_rtree_rect maq_make_default_rtree_rect(t_maq *x){
	t_rect view_rect;
	jbox_get_rect_for_view((t_object *)x, x->pv, &view_rect); 
	double xmin = x->xmin, ymin = x->ymin, xmax = x->xmax, ymax = x->ymax;
	return (t_rtree_rect){xmin,
			xmin + maq_scale(50, 0, view_rect.width, xmin, xmax),
			ymin + maq_scale(30, view_rect.height, 0, ymin, ymax),
			ymax};
}

void maq_maxdb_notify(t_maq *x, t_symbol *notification_type, void *obj, t_object *sender){
	if(notification_type == gensym("maxdb_refresh")){
		goto redraw;
	}
	if(object_classname(x) != object_classname(sender)){
		if(notification_type == gensym("maxdb_remove")){
			t_maq_oscbndl *b = (t_maq_oscbndl *)maq_hashtab_lookup(x->maq_db->obj_tab, obj);
			if(b){
				maq_hashtab_remove(x->maq_db->obj_tab, obj);
				rumati_avl_delete(x->maq_db->avltree_selected, b, NULL);
				rumati_avl_delete(x->maq_db->avltree_visible, b, NULL);
				rtree_remove(x->maq_db->rtree, b->rect, b);
				osc_mem_free(b);
			}
		}else if(notification_type == gensym("maxdb_insert")){
			if(maq_hashtab_lookup(x->maq_db->obj_tab, obj)){
				goto redraw;
			}
			t_rtree_rect r = maq_make_default_rtree_rect(x);
			maq_insert_bndl_with_rtree_rect(x, r, (t_otable_oscbndl *)obj);
		}
	}
 redraw:
	jbox_redraw(&(x->box));
}

void maq_refer(t_maq *x, t_symbol *name){
	critical_enter(x->lock);
	if(x->otable_db){
		maxdb_remove_notification((t_maxdb *)x->otable_db, (t_object *)x);
	}
	x->otable_db = (t_otable_db *)maxdb_refer((t_maxdb *)(x->otable_db), maxdb_mangle_name("otable", name));
	if(!x->otable_db){
		x->otable_db = otable_util_make_db((t_object *)x, name);
	}
	maxdb_add_notification((t_maxdb *)x->otable_db, (t_object *)x, (t_maxdb_notification_cb)maq_maxdb_notify);
	if(x->maq_db){
		maxdb_remove_notification((t_maxdb *)x->maq_db, (t_object *)x);
	}
	x->maq_db = (t_maq_db *)maxdb_refer((t_maxdb *)(x->maq_db), maxdb_mangle_name("maquette", name));
	if(!x->maq_db){
		x->maq_db = maq_make_db(x, name);
		maxdb_lock((t_maxdb *)x->maq_db);
		rtree_clear(x->maq_db->rtree, (t_rtree_dtor)maq_freebundle, x);
		t_otable_oscbndl *b = rumati_avl_get_smallest(x->otable_db->tree);
		t_rtree_rect r = maq_make_default_rtree_rect(x);
		while(b){
			maq_insert_bndl_with_rtree_rect(x, r, b);
			b = rumati_avl_get_greater_than(x->otable_db->tree, b);
		}
		maq_rebuild_visible_db(x);
	}
	maxdb_add_notification((t_maxdb *)x->maq_db, (t_object *)x, (t_maxdb_notification_cb)maq_maxdb_notify);
	x->name = name;
	maxdb_unlock((t_maxdb *)x->maq_db);
	critical_exit(x->lock);
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
        //jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_yminmax(t_maq *x, double min, double max){
	x->ymin = min;
	x->ymax = max;
	//jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_print_rect(char *prefix, t_rect rect){
	printf("%s %f %f %f %f\n", prefix, rect.x, rect.y, rect.width, rect.height);
}

void maq_print_rtree_rect(char *prefix, t_rtree_rect rect){
	printf("%s %f %f %f %f\n", prefix, rect.xmin, rect.xmax, rect.ymin, rect.ymax);
}

void maq_rtree_destructor(t_maq *x, t_otable_oscbndl *bndl){
}

void maq_clear(t_maq *x){
	critical_enter(x->lock);
	// rtree_clear can use a destructor that just frees the t_rtree_rect that's stored
	// in the udata field of the t_otable_oscbndl.  then call otable_util_clear and the 
	// bundles will be freed by otable_util_freebundle
	rumati_avl_clear(x->maq_db->avltree_selected, NULL);
	rumati_avl_clear(x->maq_db->avltree_visible, NULL);
	rtree_clear(x->maq_db->rtree, NULL, NULL);
	otable_util_clear((t_object *)x, x->otable_db);
	critical_enter(x->lock);

        //jbox_invalidate_layer((t_object *)x, x->pv, l_main);
	jbox_redraw((t_jbox *)&(x->box));
}

void maq_assist(t_maq *x, void *b, long io, long num, char *s){ 
 	if(io == ASSIST_OUTLET){ 

 	}else{ 

 	} 
} 

void maq_free(t_maq *x){ 
	jbox_free(&(x->box));
	maxdb_unrefer((t_object *)x, (t_maxdb *)x->maq_db);
} 

void maq_make_addresses(t_maq *x){
	char buf[128];
	char *params[4] = {"/x/left", "/x/right", "/y/bottom", "/y/top"};
	char *addresses[4] = {x->xmin_address, x->xmax_address, x->ymin_address, x->ymax_address};
	long *lens[4] = {&(x->xmin_address_len), &(x->xmax_address_len), &(x->ymin_address_len), &(x->ymax_address_len)};
	int i;
	for(i = 0; i < 4; i++){
		char *ptr = buf;
		if(x->name->s_name[0] == '/'){
			ptr += sprintf(ptr, "%s%s", x->name->s_name, params[i]);
		}else{
			ptr += sprintf(ptr, "/%s%s", x->name->s_name, params[i]);
		}
		ptr++;
		while((ptr - buf) % 4){
			*ptr++ = '\0';
		}
		memcpy(addresses[i], buf, ptr - buf);
		*(lens[i]) = ptr - buf;
	}
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
	//class_addmethod(c, (method)maq_refer, "refer", A_SYM, 0);
	class_addmethod(c, (method)maq_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)maq_delete, "delete", 0);
	class_addmethod(c, (method)maq_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)maq_list, "list", A_GIMME, 0);

	CLASS_ATTR_DOUBLE(c, "xmin", 0, t_maq, xmin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "xmax", 0, t_maq, xmax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "xmax", 0, "1.0");

        CLASS_ATTR_DOUBLE(c, "ymin", 0, t_maq, ymin);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymin", 0, "0.0");

        CLASS_ATTR_DOUBLE(c, "ymax", 0, t_maq, ymax);
        CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "ymax", 0, "1.0");

	CLASS_ATTR_SYM(c, "name", 0, t_maq, name);
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "name", 0, "maquette");

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_maq, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, ".5 .5 .5 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

	CLASS_ATTR_RGBA(c, "bordercolor", 0, t_maq, bordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bordercolor", 0, "0. 0. 0.1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bordercolor", 0, "rgba", "Border Color"); 

 	CLASS_ATTR_RGBA(c, "boxfillcolor", 0, t_maq, boxfillcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "boxfillcolor", 0, ".25 .25 .25 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "boxfillcolor", 0, "rgba", "Box Fill Color"); 

	CLASS_ATTR_RGBA(c, "boxbordercolor", 0, t_maq, boxbordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "boxbordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "boxbordercolor", 0, "rgba", "Box Border Color"); 

 	CLASS_ATTR_RGBA(c, "resizehandlecolor", 0, t_maq, resizehandlecolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "resizehandlecolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "resizehandlecolor", 0, "rgba", "Resize Handle Color"); 

 	CLASS_ATTR_RGBA(c, "selectedboxfillcolor", 0, t_maq, selectedboxfillcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedboxfillcolor", 0, ".25 .25 .25 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedboxfillcolor", 0, "rgba", "Selected box Fill Color"); 

	CLASS_ATTR_RGBA(c, "selectedboxbordercolor", 0, t_maq, selectedboxbordercolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedboxbordercolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedboxbordercolor", 0, "rgba", "Selected box Border Color"); 

 	CLASS_ATTR_RGBA(c, "selectedresizehandlecolor", 0, t_maq, selectedresizehandlecolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "selectedresizehandlecolor", 0, "0. 0. 0. 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "selectedresizehandlecolor", 0, "rgba", "Selected Resize Handle Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

	ps_FullPacket = gensym("FullPacket");
	ps_xmin = gensym("xmin");
	ps_xmax = gensym("xmax");
	ps_ymin = gensym("ymin");
	ps_ymax = gensym("ymax");

 	class_register(CLASS_BOX, c); 
 	maq_class = c; 

	otable_util_init();

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

		x->outlets[1] = outlet_new((t_object *)x, NULL);
		x->outlets[0] = outlet_new((t_object *)x, NULL);

		x->name = NULL;

		x->selbox = (t_rect){0., 0., 0., 0.};
		x->otable_db = NULL;
		x->maq_db = NULL;

		//x->maq_db->obj_tab = maq_hashtab_new();
		//x->maq_db->rtree = rtree_new();
		//rumati_avl_new(&(x->maq_db->avltree_visible), otable_util_comp, (void *)x);
		//rumati_avl_new(&(x->maq_db->avltree_selected), otable_util_comp, (void *)x);
		//x->maq_db->visible_count = 0;
		x->draw_rtree_nodes = 0;
		x->dblclick_count = 0;
		x->dblclick_time = clock();

		critical_new(&(x->lock));

		x->xmin_address = (char *)osc_mem_alloc(128);
		x->xmax_address = (char *)osc_mem_alloc(128);
		x->ymin_address = (char *)osc_mem_alloc(128);
		x->ymax_address = (char *)osc_mem_alloc(128);

		// attr_dictionary_process() will call our notify() method which will call maq_refer(), 
		// so there's no need to set up our databases here since that'll get done in maq_refer().
 		attr_dictionary_process(x, d); 
		// build the addresses after calling attr_dictionary_process() so that we have the right name
		maq_make_addresses(x);
		/*
		if(x->otable_db = otable_util_refer((t_object *)x, NULL, x->name)){}
		else{
			x->otable_db = otable_util_make_db((t_object *)x, NULL, (t_otable_oscbndl_dtor)maq_freebundle);
		}
		*/
		if(!(x->otable_db)){

		}
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
