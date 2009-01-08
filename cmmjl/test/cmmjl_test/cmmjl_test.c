/*
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: cmmjl_test
 DESCRIPTION: Testing object for the cmmjl
 AUTHORS: John MacCallum
 COPYRIGHT_YEARS: 2008
 SVN_REVISION: $LastChangedRevision: 587 $
 VERSION 1.0: First version
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
//#define DEBUG
#define CMMJL_PROFILE

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "version.c"
#include "cmmjl/cmmjl.h" /* Include this for basic functionality */
#include "cmmjl/cmmjl_osc.h" /* OSC support */

typedef struct _test{
	t_object t_ob;
	void *outlet;
} t_test;

void *test_class;

void *test_new();
void test_free(t_test *x);
void test_assist(t_test *x, void *b, long m, long a, char *s);

void test_seterrorcb(t_test *x);
void test_error(t_test *x);
void test_errorcb(const char *obj, const char *file, const char *func, int line, t_cmmjl_error code, char *reason);

/* This will be called in response to the FullPacket message */
void test_fullpacket(t_test *x, long len, long ptr);
/* This will be called by cmmjl_osc_parseFullPacket as each message is parsed out of the OSC packet */
void test_fullpacket_callback(void *x, t_symbol *sym, int argc, t_atom *argv);
void test_bar(t_test *x, long l);

int main(int argc, char **argv){
	t_class *c;
	c = class_new("cmmjl_test", (method)test_new, (method)test_free, (long)sizeof(t_test), 0L, 0);
	class_addmethod(c, (method)test_assist, "assist", A_CANT, 0);
	CMMJL_CLASS_ADDMETHOD(c, (method)test_bar, "bar", A_LONG, 0);
	CMMJL_CLASS_ADDMETHOD(c, (method)test_seterrorcb, "seterrorcb", 0);
	CMMJL_CLASS_ADDMETHOD(c, (method)test_error, "error", 0);
	
	/* Accept the FullPacket message and set the callback */
	//CMMJL_ACCEPT_FULLPACKET(c, test_fullpacket);
	
	/* Or pass NULL in as the callback to use the default which will attempt to 
	 send the OSC message received to this object as a message (function call). 
	 */
	CMMJL_ACCEPT_FULLPACKET(c, test_fullpacket);

	class_register(CLASS_BOX, c);
	test_class = c;

	return 0;
}

void *test_new(){
	t_test *x = (t_test *)object_alloc(test_class);
	x->outlet = outlet_new(x, 0);
	
	/* Initialize error reporter, symbol table, etc. */
	cmmjl_init(x, CMMJL_CREATE_INFO_OUTLET);
	
	int bloo = 21;
	
	post("%llx", CMMJL_EMASK_OSC_ALL);
	post("%llx", CMMJL_EMASK_ALL);
	/* report an error like this */
	CMMJL_ERROR(x, 19, "some foo screwed up here making bar = %d", bloo);
	
	post("typtag: %s", cmmjl_strerror(CMMJL_OSC_ETYPTAG));

	post("%p", x);

	return x;
}

void test_free(t_test *x){
}

void test_assist(t_test *x, void *b, long m, long a, char *s){
	
}

void test_errorcb(const char *obj, const char *file, const char *func, int line, t_cmmjl_error code, char *reason){
	error("FOO: %s:%s:%s:%d:%s (%d)", obj, file, func, line, reason, code);
}

void test_error(t_test *x){
	CMMJL_ERROR(x, 666, "some foo");
}

void test_seterrorcb(t_test *x){
	cmmjl_set_error_handler(x, test_errorcb);
}

void test_fullpacket(t_test *x, long len, long ptr){
	CMMJL_ENTER(x);
	CMMJL_TIMER();
	CMMJL_TIMER_START();
	/* Debugging is done with PDEBUG */
	PDEBUG("len = %d, ptr = %d", len, ptr);
	/* When we get a pointer to a packet, call this function with the callback that
	   will handle the messages as they are parsed out of the packet. */
	t_cmmjl_error e = cmmjl_osc_parseFullPacket(x, (char *)ptr, len, true, test_fullpacket_callback);
	CMMJL_TIMER_END();
	CMMJL_EXIT(x);
}

void test_fullpacket_callback(void *x, t_symbol *sym, int argc, t_atom *argv){
	t_atom *out = NULL;
	t_max_err r = object_method_typed(x, gensym(basename(sym->s_name)), argc, argv, out);
	post("r = %d", r);
	outlet_anything(((t_test *)x)->outlet, sym, argc, argv);
}

void test_bar(t_test *x, long l){
	post("l = %d");	
}
