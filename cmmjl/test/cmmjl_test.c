#include "ext.h"
#include "cmmjl/cmmjl.h"
#include "cmmjl/cmmjl_commonsymbols.h"
#include "cmmjl/cmmjl_error.h"
#include "cmmjl/cmmjl_osc.h"

typedef struct _test{
	t_object t_ob;
	void *outlet;
} t_test;

void *test_class;

void *test_new();
void test_free(t_test *x);
void test_fullpacket(t_test *x, long len, long ptr);
void test_fullpacket_callback(void *x, t_symbol *sym, int argc, t_atom *argv);

int main(int argc, char **argv){
	setup((t_messlist **)&test_class, (method)test_new, (method)test_free, (short)sizeof(t_test), 0L, 0);
	addmess((method)test_fullpacket, "FullPacket", A_LONG, A_LONG, 0);

	return 0;
}

void *test_new(){
	t_test *x;
	x = (t_test *)newobject(test_class);
	x->outlet = outlet_new(x, 0);

	cmmjl_init();

	return x;
}

void test_free(t_test *x){
}

void test_fullpacket(t_test *x, long len, long ptr){
	t_cmmjl_error e = cmmjl_osc_parseFullPacket(x, (char *)ptr, len, true, test_fullpacket_callback);
}

void test_fullpacket_callback(void *x, t_symbol *sym, int argc, t_atom *argv){
	outlet_anything(((t_test *)x)->outlet, sym, argc, argv);
}
