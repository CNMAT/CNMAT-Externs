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
NAME: o.route
DESCRIPTION: OSC-route for full packets
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 587 $
VERSION 0.0: First try
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_osc.h"
//#include "cmmjl/cmmjl_osc_obj.h"

typedef struct _oroute{
	t_object ob;
	void **outlets;
	t_symbol **args;
	int numArgs;
	t_cmmjl_osc_message *matched, *unmatched;
	int *numCharsMatched;
	int *matched_outlets;
	int numMatched, numUnmatched;
} t_oroute;

void *oroute_class;

void oroute_fullPacket(t_oroute *x, long len, long ptr);
void oroute_foo(t_cmmjl_osc_message msg, void *v);
void oroute_cbk(void *xx, t_symbol *msg, int argc, t_atom *argv);
void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv);
void oroute_free(t_oroute *x);
void oroute_assist(t_oroute *x, void *b, long m, long a, char *s);
void *oroute_new(t_symbol *msg, short argc, t_atom *argv);

void oroute_fullPacket(t_oroute *x, long len, long ptr){
	// make a local copy so the ref doesn't disappear out from underneath us
	char cpy[len];
	memcpy(cpy, (char *)ptr, len);
	long nn = len;

	// if the OSC packet contains a single message, turn it into a bundle
	if(strncmp(cpy, "#bundle\0", 8)){
		nn = cmmjl_osc_bundle_naked_message(len, cpy, cpy);
		if(nn < 0){
			error("problem bundling naked message");
		}
	}

	// flatten any nested bundles
	nn = cmmjl_osc_flatten(nn, cpy, cpy);

	// init the number of matched and unmatched items and extract the messages from the bundle
	x->numMatched = x->numUnmatched = 0;
	cmmjl_osc_extract_messages(nn, cpy, true, oroute_foo, (void *)x);

	int i, j = 0;

	// pack all unmatched messages into a new bundle and output the pointer to it out the right outlet
	{
		char out[len];
		j += cmmjl_osc_init_bundle(len, out, cpy + 8);
		if(x->numUnmatched > 0){
			for(i = 0; i < x->numUnmatched; i++){
				j += cmmjl_osc_add_to_bundle(len - j, out + j, &(x->unmatched[i]));
			}
		}

		t_atom out_atoms[2];
		atom_setlong(&(out_atoms[0]), j);
		atom_setlong(&(out_atoms[1]), (long)out);
		outlet_anything(x->outlets[x->numArgs], gensym("FullPacket"), 2, out_atoms);
	}

	// output all of the matches in right to left outlet order
	for(j = x->numArgs - 1; j >= 0; j--){
		for(i = 0; i < x->numMatched; i++){
			//atom_setsym(&(out_atoms[0]), gensym(x->matched[i].address));
			if(x->matched_outlets[i] == j){
				t_symbol *sym = _sym_list;
				if(x->numCharsMatched[i] > 0){
					sym = gensym(x->matched[i].address + x->numCharsMatched[i]);
				}else{
					//sym = gensym(x->matched[i].address);
				}
				t_atom out_atoms[x->matched[i].argc];
				cmmjl_osc_args2atoms(x->matched[i].typetags, x->matched[i].argv, out_atoms);
				outlet_anything(x->outlets[x->matched_outlets[i]], sym, x->matched[i].argc, out_atoms);
			}
		}
	}
}

void oroute_foo(t_cmmjl_osc_message msg, void *v){
	t_oroute *x = (t_oroute *)v;
	int i;
	int ret;
	int didmatch = 0;
	for(i = 0; i < x->numArgs; i++){
		if((ret = cmmjl_osc_match(x, msg.address, x->args[i]->s_name)) != 0){
			x->matched_outlets[x->numMatched] = i;	
			x->numCharsMatched[x->numMatched] = ret;
			x->matched[x->numMatched++] = msg;
			didmatch += 1;
		}
	}
	if(didmatch == 0){
		x->unmatched[x->numUnmatched++] = msg;
	}
}

/*
  void oroute_fullPacket(t_oroute *x, long len, long ptr){
  t_cmmjl_osc_packet *p;
  char lptr[len];
  memcpy(lptr, (char *)ptr, len);
  p = cmmjl_osc_make_packet(len, lptr);
  t_cmmjl_osc_obj *obj = p->contents;
  t_cmmjl_osc_obj *element;
  while(obj){
  if(cmmjl_osc_is_bundle(obj)){
  post("it's a bundle");
  element = ((t_cmmjl_osc_bundle *)obj)->element;
  while(element){
  t_cmmjl_osc_message *msg = ((t_cmmjl_osc_message *)element);
  post("foo: %s %s", msg->address, ((t_cmmjl_osc_message *)element)->typetags);
  int numArgs = msg->argc;
  t_cmmjl_osc_atom argv[numArgs];
  cmmjl_osc_message_get_arguments(msg, obj->size, argv);
  int i;
  for(i = 0; i < numArgs; i++){
  switch(argv[i].a_type){
  case 'i':
  post("%d", argv[i].a_w.w_int);
  break;
  case 'f':
  post("%f", argv[i].a_w.w_float);
  break;
  case 's':
  post("%s", argv[i].a_w.w_string);
  break;
  }
  }
  element = element->next;
  }
  }else{
  post("not a bundle");
  post("%s", ((t_cmmjl_osc_message *)obj)->address);
  }
  obj = obj->next;
  }

  cmmjl_osc_free_packet(p);
  }
*/

/*
  int oroute_make_osc_packet(long len, char *ptr, t_osc_packet *p){
  char *lptr = ptr;
  int i = 0;
  if((len % 4) != 0 || len <= 0 || p == NULL){
  return 1;
  }

  if(strncmp(ptr, "#bundle", 8) != 0){
  // this is not a bundle, so just treat the entire packet as a message and return
  t_osc_message *msg = (t_osc_message *)malloc(sizeof(t_osc_message));
  oroute_make_osc_message(len, ptr, msg);
  p->contents = (t_bundle *)msg;
  return 0;
  }

  // we're in the main bundle

  p->contents = (t_osc_bundle *)malloc(sizeof(t_osc_bundle));
  t_osc_bundle *bundle = p->contents;
  t_osc_bundle_element *be = bundle->bundle_element;
  //bundle->bundle_element = (t_osc_bundle_element *)malloc(sizeof(t_osc_bundle_element));
  //bundle->bundle_element->msg = (t_osc_message *)malloc(sizeof(t_osc_message));

  lptr += 8;
  bundle->timetag = (*(long long *)(lptr + i));
  lptr += 8; // timetag

  int l = 0;

  while(lptr - ptr < len){
  l = *((long *)lptr);
  lptr += 4; // size
  if(strncmp(lptr, "#bundle", 8) == 0){
  // this is a bundle
  lptr += 8;  // #bundle\0
  be = (t_osc_bundle_element *)malloc(sizeof(t_osc_bundle));
  oroute_set_bundle_id((t_osc_bundle *)be);
  ((t_osc_bundle *)be)->timetag = ntohl(*((long *)lptr));
  lptr += 8; // timetag
  lptr += oroute_make_osc_bundle(l, lptr, (t_osc_bundle *)be);
  }else{
  be = (t_osc_bundle_element *)malloc(sizeof(t_osc_bundle_element));
  lptr += oroute_make_osc_message(l, lptr, msg);
  }
  }
  return 0;
  }

  int oroute_make_osc_bundle(long len, char *ptr, t_osc_bundle *bndl){
  char *lptr = ptr;
  int i = 0;
  t_osc_bundle_element *be = &(bndl->bundle_element);
  be->size = ntohl(*((long *)lptr));
  i += 4;
  i += oroute_make_osc_message(be->size, lptr + i, &(be->msg));
  post("%s len = %d, %d", __FUNCTION__, len, i);
  while(i < len){
  post("i = %d", i);
  post("%c", *(lptr + i));
  if(*(lptr + i) == '#'){
  post("got nested bundle");
  i += 8;
  be->next = (t_osc_bundle_element *)malloc(sizeof(t_osc_bundle));
  be = be->next;
  oroute_set_bundle_id((t_osc_bundle *)be);
  be->next = NULL;
  be->size = ntohl(*((long *)(lptr + i)));
  i += 8;
  i += oroute_make_osc_bundle(be->size, lptr + i, (t_osc_bundle *)be);
  }else{
  be->next = (t_osc_bundle_element *)malloc(sizeof(t_osc_bundle_element));
  be = be->next;
  be->next = NULL;
  be->size = ntohl(*((long *)(lptr + i)));
  i += 4;
  i += oroute_make_osc_message(be->size, lptr + i, &(be->msg));
  }
  }
  return i;
  }

  int oroute_make_osc_message(long len, char *ptr, t_osc_message *msg){
  char *ptr1, *ptr2;
  long llen = len;
  ptr1 = ptr;
  if(!(ptr2 = oroute_incpointer_pad(llen, ptr1))){
  return -1;
  }
  msg->msg = ptr1;
  llen -= (ptr2 - ptr1);
  ptr1 = ptr2;
  if(!(ptr2 = oroute_incpointer(llen, ptr1))){
  return -1;
  }
  msg->argc = (ptr2 - ptr1) - 1;
  msg->argv = (t_osc_atom *)malloc(msg->argc * sizeof(t_osc_atom));

  char *tt = ptr1 + 1;
  int i;
  while(*ptr2 == '\0' && ((ptr2 - ptr)) % 4){
  ptr2++;
  }
  llen -= (ptr2 - ptr1);

  ptr1 = ptr2;
  for(i = 0; i < msg->argc; i++){
  if(*(tt + i) == '\0'){
  break;
  }

  msg->argv[i].a_type = *(tt + i);
  msg->argv[i].a_ptr = ptr1;

  if(*(tt + i) == 't'){
  ptr1 += 8;
  }else if(*(tt + i) == 's'){
  while(*ptr1 != '\0'){
  ptr1++;
  }
  ptr1++;
  while(*ptr1 == '\0' && ((ptr1 - ptr)) % 4){
  ptr1++;
  }
  }else{
  ptr1 += 4;
  }
  }
  return ptr1 - ptr;
  }

  char *oroute_incpointer_pad(long len, char *ptr){
  char *c = ptr;
  int i = 0;
  while((i < len)){
  if((*c == '\0') && (((i + 1) % 4) == 0)){
  return c + 1;
  }
  i++;
  c++;
  }
  return NULL;
  }

  char *oroute_incpointer(long len, char *ptr){
  char *c = ptr;
  int i = 0;
  while((i < len)){
  if(ptr[i] == '\0'){
  return c;
  }
  i++;
  c++;
  }
  return NULL;
  }

  int oroute_is_bundle(t_osc_bundle_element *be){
  if(strncmp(be + sizeof(t_osc_bundle_element *), "#bundle", 8)){
  return 0;
  }else{
  return 1;
  }
  }

  void oroute_set_bundle_id(t_osc_bundle *b){
  char *bb = ((char *)b) + sizeof(t_osc_bundle_element *);
  char *bundle = "#bundle";
  int i;
  for(i = 0; i < 8; i++){
  bb[i] = bundle[i];
  }
  }
*/
void oroute_cbk(void *xx, t_symbol *msg, int argc, t_atom *argv){

}

void oroute_anything(t_oroute *x, t_symbol *msg, short argc, t_atom *argv){
}

void oroute_assist(t_oroute *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET)
		sprintf(s,"Probability distribution and arguments");
	else {
		switch (a) {	
		case 0:
			sprintf(s,"Random variate");
			break;
		}
	}
}

void oroute_free(t_oroute *x){
	if(x->args){
		free(x->args);
	}
}

void *oroute_new(t_symbol *msg, short argc, t_atom *argv){
	t_oroute *x;
	int i;
	if(x = (t_oroute *)object_alloc(oroute_class)){
		cmmjl_init(x, NAME, 0);
		x->outlets = (void **)malloc((argc + 1) * sizeof(void *));
		x->args = (t_symbol **)malloc(argc * sizeof(t_symbol *));
		x->numArgs = argc;
		x->outlets[argc] = outlet_new(x, "FullPacket"); // unmatched outlet
		for(i = 0; i < argc; i++){
			x->outlets[argc - 1 - i] = outlet_new(x, NULL);
			x->args[i] = atom_getsym(argv + i);
			x->matched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			x->numCharsMatched = (int *)malloc(1024 * sizeof(int));
			x->unmatched = (t_cmmjl_osc_message *)malloc(1024 * sizeof(t_cmmjl_osc_message));
			x->matched_outlets = (int *)malloc(1024 * sizeof(int));
			x->numMatched = 0;
			x->numUnmatched = 0;
		}
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("o.route", (method)oroute_new, (method)oroute_free, sizeof(t_oroute), 0L, A_GIMME, 0);
    
	class_addmethod(c, (method)oroute_fullPacket, "FullPacket", A_LONG, A_LONG, 0);
	//class_addmethod(c, (method)oroute_notify, "notify", A_CANT, 0);
	class_addmethod(c, (method)oroute_assist, "assist", A_CANT, 0);
    
	class_register(CLASS_BOX, c);
	oroute_class = c;

	common_symbols_init();
	return 0;
}
