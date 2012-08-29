#include <stdlib.h>
#include <math.h>

// MaxMSP
#include "ext.h"

// Franz Zotter's SH library
#include "sh.h"

// CNMAT version control
#include "version.h"
#include "version.c"

/**
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: sphY
 DESCRIPTION: Evaluates the real-valued spherical harmonics up to order N at a point (theta, phi)
 AUTHORS: Franz Zotter, Andy Schmeder
 COPYRIGHT_YEARS: 2006-2008
 VERSION 0.1: Ported to MaxMSP
 VERSION 0.2: Improve help patch
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */

Symbol* ps_index;
Symbol* ps_r;

typedef struct _sphY
{
    
    t_object o_ob;
    
    t_object* in_p[1];
    long in_i_unsafe;
    
    t_object* out_p[1];
    
    sh* dummy;
    sh* shp;
    
    t_atom *list_buf;
    t_atom *list_i_buf;
    int order;
    int len;
    
} sphY;

static t_class *sphY_class;

void* sphY_new(Symbol* s, short argc, Atom* argv);
void sphY_free(sphY* x);
void sphY_assist(sphY* x, void *box, long msg, long arg, char *dstString);
void sphY_list(sphY* x, Symbol* mess, short argc, Atom* argv);
void sphY_index(sphY* x, Symbol* mess, short argc, Atom* argv);

// setup
void main(fptr *f)
{
    
    // announce copyright
    version(0);
    
    // setup
    setup((t_messlist **)&sphY_class, (method)sphY_new, (method)sphY_free, (short)sizeof(sphY), 0L, A_GIMME, 0);
    
    // reset
    addmess((method)sphY_list, "eval", A_GIMME, 0);
    
    // reset
    addmess((method)sphY_index, "index", A_GIMME, 0);
    
    // tooltip helper
    addmess((method)sphY_assist, "assist", A_CANT, 0);
    
    ps_index = gensym("index");
    ps_r = gensym("r");
    
}

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void sphY_assist(sphY *x, void *box, long msg, long arg, char *dstString) {
    
    if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "message 'eval theta phi': theta in [0, Pi] (declination), phi in [0, 2Pi] (azimuth), or message 'index'");
        }
    } else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "list of real-valued spherical harmonics, with length (order+1)^2, or list of order, degree pairs");
        }
    } else {
        post("sphY_assist: unrecognized message %ld", msg);
    }
    
}

void *sphY_new(Symbol* s, short argc, Atom *argv)
{
    
    sphY *x;
    int i;
    
    x = (sphY*) newobject(sphY_class);
    x->order = 0;
    
    for(i = 0; i < argc; i++) {
        
        if(argv[i].a_type == A_SYM) {
            
            // @order
            if(strcmp(argv[i].a_w.w_sym->s_name, "@order") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_LONG) {
                        x->order = argv[i].a_w.w_long;
                    } else {
                        post("sphY: expected int for order");
                    }
                } else {
                    post("sphY: missing arg after order");
                }
            }
            
        }
        
    }
    
    // ensure positive (else crashy)
    x->order = (x->order < 0) ? 0 : x->order;
    
    // try to allocate
    if ((x->shp = newSH(x->order))==0) {
        return 0;
    }
    
    x->len = (x->order+1)*(x->order+1);
    x->list_buf = (Atom*)calloc(x->len, sizeof(Atom));
    x->list_i_buf = (Atom*)calloc(2*x->len, sizeof(Atom));
    
    x->out_p[0] = outlet_new(x, "list");
    
    x->dummy = x->shp;
    
    return (void *)x;
}

void sphY_free(sphY *x)
{
    if (x->shp != NULL) {
        deleteSH(x->shp);
    }
    if (x->list_buf != NULL) {
        free(x->list_buf);
    }
    if (x->list_i_buf != NULL) {
        free(x->list_buf);
    }
}

void sphY_list(sphY* x, Symbol* mess, short argc, Atom* argv)
{
    
    int n,m,mn;
    double phi,theta,value;
    if(argc == 2) {
        
        theta=(double)(argv[0].a_w.w_float);
        phi=(double)(argv[1].a_w.w_float);
        
        for(n=0, mn=0; n <= x->order; n++) {
            for (m = -n; m <= n; m++, mn++) {
                value=sHEvaluate(x->shp, n, m, phi, theta);
                SETFLOAT(&x->list_buf[mn], (float)value);
            }
        }
        outlet_anything(x->out_p[0], ps_r, x->len, x->list_buf);
    }
    else {
        post("sphY: invalid number of input arguments, expected two, got %d", argc);
    }
}

void sphY_index(sphY* x, Symbol* mess, short argc, Atom* argv)
{
    
    int n, m, mn;
    
    for(n=0, mn=0; n <= x->order; n++) {
        for (m = -n; m <= n; m++, mn++) {
            SETLONG(&x->list_i_buf[2*mn], n);
            SETLONG(&x->list_i_buf[2*mn+1], m);
        }
    }
    
    outlet_anything(x->out_p[0], ps_index, 2*x->len, x->list_i_buf);
    
}

