#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"

#include "jpatcher_api.h"
#include "jgraphics.h"

void my_paint(t_object *ob, char *user_ob, t_object *patcherview, int numoutlets, void **outlets){
        t_rect rect;
        t_jgraphics *g = (t_jgraphics *)patcherview_get_jgraphics(patcherview);                                                          
        jbox_get_rect_for_view((t_object *)ob, patcherview, &rect);                                                                       
                                                                                   
        t_jrgba blue = (t_jrgba){1., 0., 0., 1.};                                                      
        jgraphics_set_source_jrgba(g, &blue);                                                                                
        jgraphics_set_line_width(g, 1);                                                                                                  
        jgraphics_rectangle(g, 0., 0., rect.width, rect.height);                                                                         
        jgraphics_stroke(g);                                                                                                             
                                                                                                                                         
        jgraphics_set_source_jrgba(g, &blue);                                                                                    
        jgraphics_rectangle(g, 0., 0., rect.width, rect.height);                                                                         
        jgraphics_fill(g); 
}
int foo(t_object *o, char *x, int inlet, int argc, t_atom *argv, int numoutlets, void **outlets){

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
