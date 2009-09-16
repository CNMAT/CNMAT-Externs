#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

int foo(char *obj, int inlet, int argc, t_atom *argv, t_atombuf **out){
    out[0] = atombuf_new(argc, argv);
    return 0;
}