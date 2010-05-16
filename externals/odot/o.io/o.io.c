/*
  Written by John MacCallum, The Center for New Music and Audio Technologies, 
  University of California, Berkeley.  Copyright (c) 2009-10, The Regents of 
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
  NAME: o.io
  DESCRIPTION: OSC for all (most) input devices
  AUTHORS: John MacCallum 
  COPYRIGHT_YEARS: 2009-10
  SVN_REVISION: $LastChangedRevision: 587 $ 
  VERSION 0.0: First try 
  VERSION 0.1: Second try 
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
*/
#include <stdio.h>
#include "version.h" 
#include "ext.h" 
#include "ext_obex.h" 
#include "ext_critical.h"
#include "jpatcher_api.h" 
#include "jgraphics.h" 
#include "version.c" 

#include "oio.h"
#include "oio_osc_util.h"
#include "oio_hid_strings.h"
#include "oio_hid_util.h"
#include "osc_match.h"
#include <mach/mach_time.h>

#define CHECKBOX_WIDTH 10
#define CHECKBOX_COLUMN_WIDTH 20
#define ROW_HEIGHT 18
#define PAD 10

typedef struct _device{
	t_symbol *name;
	int vendor_id, product_id;
	t_symbol *protocol;
	int open;
	struct _device *prev;
	struct _device *next;
} t_device;

typedef struct _odotio{ 
 	t_jbox box; 
	t_oio *oio;
 	void *osc_outlet, *info_outlet; 
	t_device *devices;
	int ndevices;
	t_critical lock;
 	t_jrgba bgcolor; 
	int ready;
} t_odotio; 

void *odotio_class; 

void odotio_paint(t_odotio *x, t_object *patcherview); 
void odotio_open(t_odotio *x, t_symbol *sym);
void odotio_close(t_odotio *x, t_symbol *sym);

void odotio_hid_value_callback(t_oio *oio, long n, char *ptr, void *context);
void odotio_hid_connect_callback(t_oio *oio, long n, char *ptr, void *context);
void odotio_hid_disconnect_callback(t_oio *oio, long n, char *ptr, void *context);

void odotio_list(t_odotio *x, t_symbol *msg, short argc, t_atom *argv);
void odotio_assist(t_odotio *x, void *b, long m, long a, char *s); 
void odotio_free(t_odotio *x); 
t_max_err odotio_notify(t_odotio *x, t_symbol *s, t_symbol *msg, void *sender, void *data); 

t_device *odotio_getIthDevice(t_odotio *x, int ii);
void odotio_toggleDevice(t_odotio *x, t_device *d);
void odotio_openDevice(t_odotio *x, t_device *d);
void odotio_closeDevice(t_odotio *x, t_device *d);
void odotio_addDevice(t_odotio *x, char *name);
void odotio_removeDevice(t_odotio *x, char *name);
void odotio_doRemoveDevice(t_odotio *x, t_device *d);

void odotio_mousedown(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers); 
void odotio_mousedrag(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers); 
void odotio_mouseup(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers); 
int main(void); 
void *odotio_new(t_symbol *s, long argc, t_atom *argv); 
void odotio_setup(t_odotio *x, t_symbol *msg, int argc, t_atom *argv);

void odotio_paint(t_odotio *x, t_object *patcherview){ 
	if(x->ready == 0){
		return;
	}
 	t_rect rect; 
 	t_jrgba c = (t_jrgba){0,0,0,1.}; 

 	// get graphics context 
 	t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview); 

 	// get our box's rectangle 
 	jbox_get_rect_for_view((t_object *)x, patcherview, &rect); 

 	// draw the outline of the box 
 	jgraphics_set_source_jrgba(g, &c); 
 	jgraphics_set_line_width(g, 1); 
 	jgraphics_rectangle(g, 0., 0., rect.width, rect.height); 
 	jgraphics_stroke(g); 

 	jgraphics_set_source_jrgba(g, &(x->bgcolor)); 
 	jgraphics_rectangle(g, 0., 0., rect.width, rect.height); 
 	jgraphics_fill(g); 

	t_jrgba black = (t_jrgba){0,0,0,1.};

	// vertical line after the checkboxes
	jgraphics_set_source_jrgba(g, &black);
	jgraphics_move_to(g, CHECKBOX_COLUMN_WIDTH, 0);
	jgraphics_line_to(g, CHECKBOX_COLUMN_WIDTH, rect.height);
	jgraphics_stroke(g);

	critical_enter(x->lock);
	char vid[x->ndevices][128], pid[x->ndevices][128];
	int maxvidw = 0, maxpidw = 0;
	t_device *d = x->devices;
	jgraphics_set_font_size(g, 12);
	int i = 0;
	while(d){
		jgraphics_rectangle(g, ((CHECKBOX_COLUMN_WIDTH - CHECKBOX_WIDTH) / 2), ((CHECKBOX_COLUMN_WIDTH - CHECKBOX_WIDTH) / 2) + (ROW_HEIGHT * (i + 1) + 2), CHECKBOX_WIDTH, CHECKBOX_WIDTH);

		if(d->open){
			jgraphics_fill(g);
		}else{
			jgraphics_stroke(g);
		}
		jgraphics_move_to(g, CHECKBOX_COLUMN_WIDTH + PAD, ROW_HEIGHT * (i + 2));
		jgraphics_show_text(g, d->name->s_name);
		double w,h;
		sprintf(vid[i], "%-5d", d->vendor_id);
		jgraphics_text_measure(g, vid[i], &w, &h);
		if(w > maxvidw){
			maxvidw = w;
		}
		sprintf(pid[i], "%-5d", d->product_id);
		jgraphics_text_measure(g, pid[i], &w, &h);
		if(w > maxpidw){
			maxpidw = w;
		}
		i++;
		d = d->next;
	}
	jgraphics_set_source_jrgba(g, &(x->bgcolor));
	jgraphics_rectangle(g, rect.width - (maxvidw + maxpidw + (PAD * 4)), 0, rect.width, rect.height);
	jgraphics_fill(g);
	jgraphics_set_source_jrgba(g, &black);
	for(i = 0; i < x->ndevices; i++){
		jgraphics_move_to(g, rect.width - (maxpidw + PAD), ROW_HEIGHT * (i + 2));
		jgraphics_show_text(g, vid[i]);
		jgraphics_move_to(g, rect.width - ((maxvidw + PAD) + (maxpidw + (PAD * 2))), ROW_HEIGHT * (i + 2));
		jgraphics_show_text(g, pid[i]);
	}
	jgraphics_move_to(g, rect.width - (maxpidw + PAD * 2), 0);
	jgraphics_line_to(g, rect.width - (maxpidw + PAD * 2), rect.height);
	jgraphics_stroke(g);

	jgraphics_move_to(g, rect.width - ((maxvidw + PAD * 2) + (maxpidw + PAD * 2)), 0);
	jgraphics_line_to(g, rect.width - ((maxvidw + PAD * 2) + (maxpidw + PAD * 2)), rect.height);
	jgraphics_stroke(g);

	for(i = 0; i < x->ndevices + 1; i++){
		jgraphics_move_to(g, 0, ROW_HEIGHT * (i + 1) + 3);
		jgraphics_line_to(g, rect.width, ROW_HEIGHT * (i + 1) + 3);
	}
	critical_exit(x->lock);
	jgraphics_stroke(g);

	/*
	jgraphics_set_source_jrgba(g, &(x->textColor));
	jgraphics_set_font_size(g, 10);
	char buf[128];
	if(x->selected){	
		sprintf(buf, "%s:(%f, %f)", x->funcattr[x->currentFunction]->name, x->selected->coords.x, x->selected->coords.y);
	}else{
		sprintf(buf, "%s", x->funcattr[x->currentFunction]->name);
	}
	jgraphics_move_to(g, 1, 10);
	jgraphics_show_text(g, buf);
	*/
} 

void odotio_open(t_odotio *x, t_symbol *sym){
	oio_hid_registerValueCallback(x->oio, sym->s_name, odotio_hid_value_callback, (void *)x);
	t_device *d = x->devices;
	while(d){
		int i1, i2;
		if(osc_match(sym->s_name, d->name->s_name, &i1, &i2) == 3){
			d->open = 1;
		}
		d = d->next;
	}
	jbox_redraw(&(x->box));
}

void odotio_close(t_odotio *x, t_symbol *sym){
	oio_hid_unregisterValueCallback(x->oio, sym->s_name, odotio_hid_value_callback);
	t_device *d = x->devices;
	while(d){
		int i1, i2;
		if(osc_match(sym->s_name, d->name->s_name, &i1, &i2) == 3){
			d->open = 0;
		}
		d = d->next;
	}
	jbox_redraw(&(x->box));
}

void odotio_hid_value_callback(t_oio *oio, long n, char *ptr, void *context){
	//PP("%ld %p", n, ptr);
	t_odotio *x = (t_odotio *)context;
	t_atom a[2];
	atom_setlong(a, n);
	atom_setlong(a + 1, (long)ptr);
	outlet_anything(x->osc_outlet, gensym("FullPacket"), 2, a);
	/*
	t_oio_osc_msg *msg = NULL;
	t_oio_err err = oio_osc_util_parseOSCBundle(n, ptr, &msg);
	if(err){
		OIO_ERROR(err);
	}
	while(msg){
		int i;
		post("%s ", msg->address);
		for(i = 0; i < msg->natoms; i++){
			uint64_t l = *((uint64_t *)(msg->atoms + i));
			post("%llu (%c) ", l, msg->atoms[i].typetag);
		}
		printf("\n");
		msg = msg->next;
	}
	oio_osc_util_printBundle(n, ptr, printf);
	oio_obj_sendOSC(oio, n, ptr);
	*/
}

void odotio_hid_connect_callback(t_oio *oio, long n, char *ptr, void *context){
	t_odotio *x = (t_odotio *)context;
	odotio_addDevice(x, ptr);
	jbox_redraw(&(x->box));
}

void odotio_hid_disconnect_callback(t_oio *oio, long n, char *ptr, void *context){
	t_odotio *x = (t_odotio *)context;
	odotio_removeDevice(x, ptr);
	jbox_redraw(&(x->box));
}

void odotio_list(t_odotio *x, t_symbol *msg, short argc, t_atom *argv){
}

t_max_err odotio_notify(t_odotio *x, t_symbol *s, t_symbol *msg, void *sender, void *data){ 
 	if (msg == gensym("attr_modified")){ 
 		//t_symbol *attrname = (t_symbol *)object_method((t_object *)data, gensym("getname")); 
 		jbox_redraw(&(x->box)); 
	} 
	return 0; 
} 

t_device *odotio_getIthDevice(t_odotio *x, int ii){
	int i = 0;
	t_device *d = x->devices;
	while(d){
		if(i++ == ii){
			break;
		}
		d = d->next;
	}
	if(d){
		return d;
	}
	return NULL;
}

void odotio_toggleDevice(t_odotio *x, t_device *d){
	if(d){
		if(d->open){
			odotio_closeDevice(x, d);
		}else{
			odotio_openDevice(x, d);
		}
	}
}

void odotio_openDevice(t_odotio *x, t_device *d){
	if(!oio_hid_registerValueCallback(x->oio, d->name->s_name, odotio_hid_value_callback, (void *)x)){
		d->open = 1;
	}
}

void odotio_closeDevice(t_odotio *x, t_device *d){
	oio_hid_unregisterValueCallback(x->oio, d->name->s_name, odotio_hid_value_callback);
	d->open = 0;
}

void odotio_addDevice(t_odotio *x, char *name){
	t_device *d = (t_device *)sysmem_newptr(sizeof(t_device));
	uint32_t pid = -1, vid = -1;
	oio_hid_util_getDeviceProductIDFromDeviceName(x->oio, name, &pid);
	oio_hid_util_getDeviceVendorIDFromDeviceName(x->oio, name, &vid);
	d->name = gensym(name);
	d->vendor_id = vid;
	d->product_id = pid;
	d->protocol = gensym("HID");
	d->open = 0;
	critical_enter(x->lock);
	if(x->devices){
		x->devices->prev = d;
	}
	d->next = x->devices;
	d->prev = NULL;
	x->devices = d;
	x->ndevices++;
	critical_exit(x->lock);
}

void odotio_removeDevice(t_odotio *x, char *name){
	int i;
	critical_enter(x->lock);
	t_device *d = x->devices;
	while(d){
		int i1, i2;
		if(osc_match(name, d->name->s_name, &i1, &i2) == 3){
			odotio_doRemoveDevice(x, d);
			x->ndevices--;
		}
		d = d->next;
	}
	critical_exit(x->lock);
}

void odotio_doRemoveDevice(t_odotio *x, t_device *d){
	if(d->prev){
		d->prev->next = d->next;
	}else{
		x->devices = d->next;
	}
	if(d->next){
		d->next->prev = d->prev;
	}
	sysmem_freeptr(d);
}

// 0x10 = no modifiers 
// 0x11 = command 
// 0x12 = shift 
// 0x94 = control 
// 0x18 = option 
void odotio_mousedown(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers){ 
 	//post("0x%X", modifiers); 
 	t_rect r; 
	jbox_get_rect_for_view((t_object *)x, patcherview, &r);
	if(pt.x < CHECKBOX_COLUMN_WIDTH){
		int row = (int)pt.y / ROW_HEIGHT;
		t_device *d = odotio_getIthDevice(x, row - 1);
		if(d){
			odotio_toggleDevice(x, d); // we have a header row
		}
	}

	jbox_redraw(&(x->box));
}

void odotio_mousedrag(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers){
	//t_rect r;
	//jbox_get_rect_for_view((t_object *)x, patcherview, &r);
}

void odotio_mouseup(t_odotio *x, t_object *patcherview, t_pt pt, long modifiers){
}

void odotio_assist(t_odotio *x, void *b, long io, long num, char *s){ 
 	if(io == ASSIST_OUTLET){ 
		switch(num){
		case 0:
			break;
		}
 	}else{ 
 		switch (num) {	 
 		case 0: 
 			break; 
 		} 
 	} 
} 

void odotio_free(t_odotio *x){ 
 	jbox_free((t_jbox *)x); 
} 

int main(void){ 
 	t_class *c = class_new("o.io", (method)odotio_new, (method)odotio_free, sizeof(t_odotio), 0L, A_GIMME, 0); 

 	c->c_flags |= CLASS_FLAG_NEWDICTIONARY; 
 	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR | JBOX_FONTATTR); 

 	class_addmethod(c, (method)odotio_paint, "paint", A_CANT, 0); 
 	class_addmethod(c, (method)version, "version", 0); 
 	class_addmethod(c, (method)odotio_assist, "assist", A_CANT, 0); 
 	class_addmethod(c, (method)odotio_notify, "notify", A_CANT, 0); 
 	class_addmethod(c, (method)odotio_mousedown, "mousedown", A_CANT, 0); 
 	class_addmethod(c, (method)odotio_mousedrag, "mousedrag", A_CANT, 0); 
 	class_addmethod(c, (method)odotio_mouseup, "mouseup", A_CANT, 0); 
	class_addmethod(c, (method)odotio_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)odotio_open, "open", A_SYM, 0);
	class_addmethod(c, (method)odotio_close, "close", A_SYM, 0);

 	CLASS_STICKY_ATTR(c, "category", 0, "Color");

 	CLASS_ATTR_RGBA(c, "bgcolor", 0, t_odotio, bgcolor); 
 	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT(c, "bgcolor", 0, ".75 .75 .75 1."); 
 	CLASS_ATTR_STYLE_LABEL(c, "bgcolor", 0, "rgba", "Background Color"); 

 	CLASS_STICKY_ATTR_CLEAR(c, "category"); 

 	CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 300. 100."); 

 	class_register(CLASS_BOX, c); 
 	odotio_class = c; 

 	version(0); 
	
 	return 0; 
} 

void *odotio_new(t_symbol *s, long argc, t_atom *argv){ 
 	t_odotio *x = NULL; 
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

 	if(x = (t_odotio *)object_alloc(odotio_class)){ 
		post("%s: %p", __PRETTY_FUNCTION__, x);
 		jbox_new((t_jbox *)x, boxflags, argc, argv); 
 		x->box.b_firstin = (void *)x; 
		x->info_outlet = outlet_new((t_object *)x, NULL);
 		x->osc_outlet = outlet_new((t_object *)x, "FullPacket");
		critical_new(&(x->lock));
 		attr_dictionary_process(x, d); 
		x->devices = NULL;

		x->ready = 0;
 		jbox_ready((t_jbox *)x); 
		schedule_delay(x, (method)odotio_setup, 100, NULL, 0, NULL);
 		return x; 
 	} 
 	return NULL; 
} 

void odotio_setup(t_odotio *x, t_symbol *msg, int argc, t_atom *argv){
	const char *hid_usageplist = "/Users/john/Development/cnmat/trunk/max/externals/odot/liboio/hid_usage_strings.plist";
	const char *hid_cookieplist = "/Users/john/Development/cnmat/trunk/max/externals/odot/liboio/hid_cookie_strings.plist";
	x->oio = oio_obj_alloc(odotio_hid_connect_callback, x, odotio_hid_disconnect_callback, x, hid_usageplist, hid_cookieplist);
	x->ready = 1;
}
