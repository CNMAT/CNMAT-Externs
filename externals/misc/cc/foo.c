#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

int foo(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){
    post("i am bar");
    return 0; // non-zero indicates an error
}

int my_bang(t_object *o, char *x, int inlet, int numoutlets, void **outlets){

    return 0; // non-zero indicates an error
}

int my_int(t_object *o, char *x, int inlet, long l, int numoutlets, void **outlets){

    return 0; // non-zero indicates an error
}

int my_float(t_object *o, char *x, int inlet, double f, int numoutlets, void **outlets){

    return 0; // non-zero indicates an error
}

int my_list(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){

    return 0; // non-zero indicates an error
}

int my_anything(t_object *o, char *x, int inlet, t_symbol *msg, int argc, t_atom *argv, int numoutlets, void **outlets){

    return 0; // non-zero indicates an error
}

void my_free(t_object *o, char *x){
    // free any instance data here
}

void my_new(t_object *o, char *x){
    // allocate and initialize any instance data/variables here
    // this function will be called each time you compile
}
void my_assist(long io, long index, char *s){
    switch(io){
        case 1: // inlet
            switch(index){
            }
            break;
        case 2: // outlet
            switch(index){
            }
            break;
    }
}
