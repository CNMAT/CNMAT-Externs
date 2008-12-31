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

#include "version.h"
#include "ext.h"
#include "ext_obex.h"
#include "version.c"
#include "cmmjl/cmmjl.h" /* Include this for basic functionality */
#include "cmmjl/cmmjl_commonsymbols.h" /* predefined t_symbols */
#include "cmmjl/cmmjl_error.h" /* error reporting */
#include "cmmjl/cmmjl_osc.h" /* OSC support */

typedef struct _test{
	t_object t_ob;
	void *outlet;
} t_test;

void *test_class;

void *test_new();
void test_free(t_test *x);

/* This will be called in response to the FullPacket message */
void test_fullpacket(t_test *x, long len, long ptr);
/* This will be called by cmmjl_osc_parseFullPacket as each message is parsed out of the OSC packet */
void test_fullpacket_callback(void *x, t_symbol *sym, int argc, t_atom *argv);
void test_bar(t_test *x, long l);

int main(int argc, char **argv){
	t_class *c;
	c = class_new("cmmjl_test", (method)test_new, (method)test_free, (long)sizeof(t_test), 0L, 0);
	class_addmethod(c, (method)test_bar, "bar", A_LONG, 0);
	
	/* Initialize error reporter, symbol table, etc. */
	cmmjl_init();
	/* Accept the FullPacket message and set the callback */
	CMMJL_ACCEPT_FULLPACKET(c, test_fullpacket);
	
	int bloo = 21;

	/* report an error like this */
	CMMJL_ERROR(19, "some foo screwed up here making bar = %d", bloo);

	class_register(CLASS_BOX, c);
	test_class = c;

	return 0;
}

void *test_new(){
	t_test *x = (t_test *)object_alloc(test_class);
	x->outlet = outlet_new(x, 0);

	return x;
}

void test_free(t_test *x){
}

void test_fullpacket(t_test *x, long len, long ptr){
	/* Debugging is done with PDEBUG */
	PDEBUG("len = %d, ptr = %d", len, ptr);
	/* When we get a pointer to a packet, call this function with the callback that
	   will handle the messages as they are parsed out of the packet. */
	t_cmmjl_error e = cmmjl_osc_parseFullPacket(x, (char *)ptr, len, true, test_fullpacket_callback);
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
