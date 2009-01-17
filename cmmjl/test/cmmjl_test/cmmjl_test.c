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
#include "ext_obex_util.h"
#include "jpatcher_api.h"
#include "version.c"
#include "cmmjl/cmmjl.h" /* Include this for basic functionality */
#include "cmmjl/cmmjl_osc.h" /* OSC support */
#include "cmmjl/cmmjl_osc_pattern.h"
#include "cmmjl/cmmjl_osc_timetag.h"
#include "cmmjl/cmmjl_osc_schedule.h"
#include "regex.h"

typedef struct _test{
	t_object ob;
	void *outlet;
} t_test;

void *test_class;

void *test_new();
void test_free(t_test *x);
void test_assist(t_test *x, void *b, long m, long a, char *s);

void test_seterrorcb(t_test *x);
void test_error(t_test *x);
void test_errorcb(const char *obj, const char *file, const char *func, int line, t_cmmjl_error code, char *reason);
void test_verbose(t_test *x, long b);

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
	CMMJL_CLASS_ADDMETHOD(c, (method)test_verbose, "verbose", A_LONG, 0);
	
	/* Accept the FullPacket message and set the callback */
	//CMMJL_ACCEPT_FULLPACKET(c, test_fullpacket);
	
	/* Or pass NULL in as the callback to use the default which will attempt to 
	 send the OSC message received to this object as a message (function call). 
	 */
	CMMJL_ACCEPT_FULLPACKET(c, t_test);

	class_register(CLASS_BOX, c);
	test_class = c;

	return 0;
}

void *test_new(){
	t_test *x = (t_test *)object_alloc(test_class);
	x->outlet = outlet_new(x, 0);
	
	/* Initialize error reporter, symbol table, etc. */
	cmmjl_init(x, NAME, CMMJL_CREATE_INFO_OUTLET | CMMJL_OSC_SCHEDULER_ON);
	
	int bloo = 21;
       	/* report an error like this */
	CMMJL_ERROR(x, 19, "some foo screwed up here making bar = %d", bloo);

	char *st1 = "/app*e/pi?/{1,3,5}/foo";
	char *st2 = "/appqcue/pie/3/foo";
	if(cmmjl_osc_match(x, st1, st2)){
		error("No match!!");
	}

	/*
	t_patcher *patcher, *ppatcher;
	t_box *box;
	object_obex_lookup(x, gensym("#P"), (t_object **)&patcher);
	post("patcher address: %p", patcher);
	post("patcher name: %s", jpatcher_get_name(patcher)->s_name);
	post("patcher title: %s", jpatcher_get_title(patcher)->s_name);
	box = jpatcher_get_box(patcher);
	post("box = %p", box);
	ppatcher = jpatcher_get_parentpatcher(patcher);
	post("parent address: %p", ppatcher);
	*/

	/*
	t_object *parent, *patcher;
        t_symbol *name;

        object_obex_lookup(x, gensym("#P"), &patcher);
        parent = patcher;
        do {
		parent = jpatcher_get_parentpatcher(parent);
		if (parent) {
			name = object_attr_getsym(parent, gensym("name"));
			if (name)
				post("%s",name->s_name);
		}
        } while (parent != NULL);
	*/

	/*
	object_obex_lookup(x, gensym("#B"), (t_object **)&box);
	post("box is called %s", jbox_get_maxclass(box)->s_name);
	post("id = %d", jbox_get_id(box)->s_name);
	while(patcher){
		patcher = jpatcher_get_parentpatcher(patcher);
		if(patcher){
			post("patcher: %s", jpatcher_get_name(patcher)->s_name);
		}
	}
	*/
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

void test_verbose(t_test *x, long b){
	if(b == 0){
		cmmjl_set_default_error_handler(x);
	}else{
		cmmjl_set_verbose_error_handler(x);
	}
}

void test_fullpacket(t_test *x, long len, long ptr){
	CMMJL_ENTER(x);
	CMMJL_TIMER();
	CMMJL_TIMER_START();
	/* Debugging is done with PDEBUG */
	PDEBUG("len = %d, ptr = %d", len, ptr);
	/* When we get a pointer to a packet, call this function with the callback that
	   will handle the messages as they are parsed out of the packet. */
	t_cmmjl_error e = cmmjl_osc_parseFullPacket(x, len, ptr, test_fullpacket_callback);
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
