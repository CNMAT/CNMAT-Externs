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
  NAME: tco
  DESCRIPTION: Tco loop for max
  AUTHORS: John MacCallum
  COPYRIGHT_YEARS: 2011
  SVN_REVISION: $LastChangedRevision: 587 $
  VERSION 0.0: First try
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#include "version.h"
#include "ext.h"
#include "version.c"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_systhread.h"
#include <pthread.h>
#include <execinfo.h>

typedef struct _tco{
	t_object ob;
	void *outlet;
	struct stack_frame *ebp;
	char *stp;
} t_tco;


void *tco_class;

#define SYSTHREAD_STACKMARGIN 65536

char *systhread_getstackbase(void)
{
	#ifdef MAC_VERSION
	char c;
	char *addr;
	char *base;

	addr = &c;
		#if TARGET_RT_MAC_MACHO
	base = addr - pthread_get_stacksize_np((pthread_t)systhread_self());
		#else
	base = addr - pt_get_stacksize_np(systhread_self());
		#endif

	base += SYSTHREAD_STACKMARGIN;
	return base;
#endif // #ifdef MAC_VERSION
	#ifdef WIN_VERSION
	char c;
	char *addr;
	char* base;
	MEMORY_BASIC_INFORMATION mbi;

	addr = &c;
	// __asm mov addr, esp   // rbs -- experimental: seems to make no difference
	if (VirtualQuery(addr, &mbi, sizeof(mbi)) != 0)
		{
			base = (char*) mbi.AllocationBase;
			base += SYSTHREAD_STACKMARGIN;
		}
	else
		{
			error("Error querying stack base.");
			base =  addr - 250000;  // This would probably keep things running if VirtualQuery failed for some reason.
								}
	return base;
#endif // #ifdef WIN_VERSION
}

t_symbol *ps_FullPacket;


void tco_int(t_tco *x, long l);
void tco_float(t_tco *x, double f);
void tco_list(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_anything(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_bang(t_tco *x);
void tco_fullPacket(t_tco *x, long len, long ptr);
void tco_outlet_int(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_outlet_float(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_outlet_bang(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_outlet_anything(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_outlet_list(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_outlet_fullPacket(t_tco *x, t_symbol *msg, long argc, t_atom *argv);
void tco_free(t_tco *x);
void tco_assist(t_tco *x, void *b, long m, long a, char *s);
void *tco_new(t_symbol *msg, short argc, t_atom *argv);

/*
	char c;
	int clear_stptr = 0;
	if(x->st == NULL){
		//x->st = systhread_getstackbase();
		//clear_stptr = 1;
	}
	printf("%p %p %d\n", &c, st, &c - st);
	if(&c < st){
		printf("STACK OVERFLOW!!!\n");
	}
	if(clear_stptr){
		x->st = NULL;
	}
 */
void tco_int(t_tco *x, long l){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		t_atom argv;
		atom_setlong(&argv, l);
		schedule_delay(x, (method)tco_outlet_int, 0, NULL, 1, &argv);
	}else{
		outlet_int(x->outlet, l);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_float(t_tco *x, double f){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		t_atom argv;
		atom_setfloat(&argv, f);
		schedule_delay(x, (method)tco_outlet_float, 0, NULL, 1, &argv);
	}else{
		outlet_float(x->outlet, f);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_list(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		schedule_delay(x, (method)tco_outlet_list, 0, NULL, argc, argv);
	}else{
		outlet_list(x->outlet, NULL, argc, argv);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_anything(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		schedule_delay(x, (method)tco_outlet_anything, 0, msg, argc, argv);
	}else{
		outlet_anything(x->outlet, msg, argc, argv);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_bang(t_tco *x){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		schedule_delay(x, (method)tco_outlet_bang, 0, NULL, 0, NULL);
	}else{
		outlet_bang(x->outlet);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_fullPacket(t_tco *x, long len, long ptr){
	char c;
	int clear_stptr = 0;
	char *stp = x->stp;
	if(stp == NULL){
		stp = x->stp = systhread_getstackbase();
		clear_stptr = 1;
	}
	if(&c < stp){
		char *buf = (char *)sysmem_newptr(len);
		memcpy(buf, (char *)ptr, len);
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, (long)buf);
		schedule_delay(x, (method)tco_outlet_fullPacket, 0, NULL, 2, out);
	}else{
		t_atom out[2];
		atom_setlong(out, len);
		atom_setlong(out + 1, ptr);
		outlet_anything(x->outlet, ps_FullPacket, 2, out);
	}
	if(clear_stptr){
		x->stp = NULL;
	}
}

void tco_outlet_int(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_atoms(x->outlet, argc, argv);
}

void tco_outlet_float(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_atoms(x->outlet, argc, argv);
}

void tco_outlet_bang(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_bang(x->outlet);
}

void tco_outlet_anything(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_anything(x->outlet, msg, argc, argv);
}

void tco_outlet_list(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_list(x->outlet, NULL, argc, argv);
}

void tco_outlet_fullPacket(t_tco *x, t_symbol *msg, long argc, t_atom *argv){
	outlet_anything(x->outlet, ps_FullPacket, argc, argv);
	char *ptr = (char *)atom_getlong(argv + 1);
	if(ptr){
		sysmem_freeptr(ptr);
	}
}

void tco_assist(t_tco *x, void *b, long m, long a, char *s){
	if (m == ASSIST_OUTLET){
		//sprintf(s,"OSC bundle");
	}else {
		//sprintf(s, "Arguments for address %s", x->addresses[a]->s_name);
	}
}

void tco_free(t_tco *x){
}

void *tco_new(t_symbol *msg, short argc, t_atom *argv){	
	t_tco *x;
	if(x = (t_tco *)object_alloc(tco_class)){
		x->outlet = outlet_new(x, NULL);
		x->stp = NULL;
	}
		   	
	return(x);
}

int main(void){
	t_class *c = class_new("tco", (method)tco_new, (method)tco_free, sizeof(t_tco), 0L, A_GIMME, 0);
	class_addmethod(c, (method)tco_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)tco_bang, "bang", 0);
	class_addmethod(c, (method)tco_int, "int", A_LONG, 0);
	class_addmethod(c, (method)tco_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)tco_anything, "anything", A_GIMME, 0);
	class_addmethod(c, (method)tco_list, "list", A_GIMME, 0);
	class_addmethod(c, (method)tco_fullPacket, "FullPacket", A_LONG, A_LONG, 0);

	class_register(CLASS_BOX, c);
	tco_class = c;

	ps_FullPacket = gensym("FullPacket");

	common_symbols_init();
	return 0;
}
