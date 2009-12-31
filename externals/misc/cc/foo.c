#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

typedef struct _myobj{
    double arg;
} t_myobj;

int mult(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){
    t_myobj *xx = (t_myobj *)x;
    switch(argv[0].a_type){
        case A_FLOAT:
            outlet_float(outlets[0], xx->arg / atom_getfloat(argv));
            break;
        case A_LONG:
            outlet_int(outlets[0], xx->arg / atom_getlong(argv));
            break;
    }
    return 0; // non-zero indicates an error
}

int my_bang(t_object *o, char *x, int inlet, int numoutlets, void **outlets){
    post("bang!");
    return 0; // non-zero indicates an error
}

int my_int(t_object *o, char *x, int inlet, long l, int numoutlets, void **outlets){
    if(inlet == 1){
        t_myobj *xx = (t_myobj *)x;
        xx->arg = (double)l;
    }
    return 0; // non-zero indicates an error
}

int my_float(t_object *o, char *x, int inlet, double f, int numoutlets, void **outlets){
    if(inlet == 1){
        t_myobj *xx = (t_myobj *)x;
        xx->arg = f;
    }
    return 0; // non-zero indicates an error
}

int my_list(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){
    post("list:");
    int i;
    for(i = 0; i < argc; i++){
        postatom(argv + i);
    }
    return 0; // non-zero indicates an error
}

int my_anything(t_object *o, char *x, int inlet, t_symbol *msg, int argc, t_atom *argv, int numoutlets, void **outlets){
    post("received message %s with %d arguments", msg->s_name, argc);
    return 0; // non-zero indicates an error
}

void my_free(t_object *o, char *x){
    // free any instance data here
}

void my_new(t_object *o, char *x){
    // allocate and initialize any instance data/variables here
    // this function will be called each time you compile
}
