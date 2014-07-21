/*

 
 
Written by Andy Schmeder, The Center for New Music and Audio Technologies,
University of California, Berkeley.  Copyright (c) 2007, The Regents of 
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
NAME: radialbeameq~
DESCRIPTION: IIR filter cascade for radial equalization of a dirac beam
AUTHORS: Franz Zotter, Andy Schmeder
COPYRIGHT_YEARS: 2009
SVN_REVISION: $LastChangedRevision: 550 $
VERSION 0.1: Ported code from PD
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  

*/

#define RADIAL_BEAMFORM_VERSION "0.1"

#include "ext.h"

#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "ext_critical.h"
#include "ext_atomic.h"

#include "z_dsp.h"

#include <math.h>
#include <float.h>

#include "sph_hn_zeros.h"

#include "version.h"
#include "version.c"

/* radial beamforming filter for spherical synthesis of harmonics
* pd obj: [sh_radial_beamform~]
* arguments:
*             N  <int>   ... spherical harmonics degree
*             r0 <float> ... primary synthesis radius
*             r  <float> ... target radius
*             p          ... sound pressure synthesis at primary radius
*             v          ... sound velocity synthesis at primary radius
*
* messages:
*             r0 <float> ... primary synthesis radius
*             r  <float> ... target radius
*
*  Franz Zotter, 2007
*  Institute of Electronic Music and Acoustics
*  University of Music and Dramatic Arts, Graz, Austria. 
*/             

enum BFMode {PRESSURE=0,VELOCITY=1};

static t_class *radialbeameq_class;

typedef struct {
    t_pxobject x_obj;
    
    float r0;
    float r;
    int block_sz;
    int bf_mode;
    t_symbol* bf_mode_sym;
    int sh_degree;
    int num_ar_sec;
    int num_ma_sec;
    int num_ma_sos;
    int num_ar_sos;
    float *b0;
    float *b1;
    float *a1;
    float *a2;
    float *z1;
    float *z2;
    float gain;
    float gain_ar;
    float delta_n;
    float delta_n0;
    
    float max_wng;
    float omega_cutoff;
    float slope_cutoff;
    
    float *omega_breakpoints_den;
    float *omega_breakpoints_num;
    
    float *in;
    float *out;
    float *buf;
    
    void* list_outlet;
    
} RadialBeamform;

static void radialBeamformFreeFilters (RadialBeamform *x) {
    if (x->z1!=0)
        free(x->z1);
    x->z1=0;
    if (x->z2!=0)
        free(x->z2);
    x->z2=0;
    if (x->a1!=0)
        free(x->a1);
    x->a1=0;
    if (x->a2!=0)
        free(x->a2);
    x->a2=0;
    if (x->b0!=0)
        free(x->b0);
    x->b0=0;
    if (x->b1!=0)
        free(x->b1);
    x->b1=0;
    if (x->omega_breakpoints_num!=0)
        free(x->omega_breakpoints_num);
    x->omega_breakpoints_num=0;
    if (x->omega_breakpoints_den!=0)
        free(x->omega_breakpoints_den);
    x->omega_breakpoints_den=0;
    
    x->num_ma_sec=0;
    x->num_ar_sec=0;
}

void radialBeamformFree (RadialBeamform *x) {
    radialBeamformFreeFilters (x);
    if (x->buf!=0)
        free(x->buf);
    x->buf=0;
}

static void radialBeamformReset (RadialBeamform *x) {
    int n, cycles;
    
    post("radialbeameq~: reset");
    
    cycles = (x->num_ar_sos<x->num_ma_sos)?x->num_ma_sos:x->num_ar_sos;
    for (n=0; n<cycles; n++) {
        x->z1[n]=0;
        x->z2[n]=0;
    }
    cycles = (x->num_ar_sec<x->num_ma_sec)?x->num_ma_sec:x->num_ar_sec;
    for (;n<cycles; n++)
        x->z1[n]=0;
}


static void radialBeamformSetWNG (RadialBeamform *x,  t_object *attr, int argc, t_atom *argv) {
    float max_wng,curr_lvl,next_lvl,curr_omega,next_omega,curr_slope,next_slope;
    float *shn_bpnum=sph_omega_bp[x->sh_degree];
    int n_bpnum=sph_bp_n[x->sh_degree];
    float *shn_bpden=sph_omega_bp[x->sh_degree];;
    int n_bpden=sph_bp_n[x->sh_degree];
    int knum=0;
    int kden=0;
    float one_over_dn=1.0f/x->delta_n;
    float one_over_dn0=1.0f/x->delta_n0;
    
    if ((argc<1)|(argv[0].a_type!=A_FLOAT && (argv[0].a_type!=A_LONG))) {
        post("radial_beamform~: no value given for wng");
        max_wng=20.0f;
    }
    else
        max_wng = atom_getfloat(&argv[0]);
    
    x->max_wng=max_wng;
    
    if (x->bf_mode==VELOCITY) {
        shn_bpden = sph_omega_bpd[x->sh_degree];
        n_bpden = sph_bpd_n[x->sh_degree];
    }
    
    next_slope=0.0f;
    next_omega=1000.0f;
    next_lvl=0.0f;
    
    do {
        curr_omega=next_omega;
        curr_lvl=next_lvl;
        curr_slope=next_slope;
        if ((knum<n_bpnum)&&(kden<n_bpden)) {
            if (shn_bpden[kden]*one_over_dn0>shn_bpnum[knum]*one_over_dn) {
                next_omega=shn_bpden[kden++]*one_over_dn0;
                next_slope+=6.0f;
            }
            else{
                next_omega=shn_bpnum[knum++]*one_over_dn;
                next_slope-=6.0f;
            }
        }
        else if (kden<n_bpden) {
            next_omega=shn_bpden[kden++]*one_over_dn0;
            next_slope+=6.0f;
        }
        else if (knum<n_bpnum) {
            next_omega=shn_bpnum[knum++]*one_over_dn;
            next_slope-=6.0f;
        } 
        else {
            break;
        }
        next_lvl = curr_lvl + curr_slope * (log2f(curr_omega)-log2f(next_omega));
    } while (next_lvl<max_wng);
    
    if (curr_slope>0.0f) {
        next_omega=curr_omega*pow(2.0f,(curr_lvl-max_wng)/curr_slope); 
    }
    else {
        next_omega=0;
    }
    x->omega_cutoff=next_omega;
    x->slope_cutoff=curr_slope;
    //   radialBeamformInfo(x);
}


static void radialBeamformSetDeltan (RadialBeamform *x, t_object *attr, int argc, t_atom *argv) {
    t_atom list[1];
    int k;
    float one_over_dn,one_over_dn_sq, one_over_a0;
    float dn;
    float *shn_b=sph_hn_b[x->sh_degree];
    float *shn_o=sph_hn_omega[x->sh_degree];
    
    if ((argc<1)|(argv[0].a_type!=A_FLOAT)) {
        post("radial_beamform~: no value given for delta_n");
        dn=0;
    }
    else
        dn = atom_getfloat(&argv[0]);
    
    one_over_dn = 1.0f / dn;
    one_over_dn_sq = one_over_dn * one_over_dn;
    x->delta_n = dn;
    x->gain_ar = 1.0f;
    
    // The radius controls the zeroes of the Hankel function in the
    // denominator. Therefore, the AR coefficients are inserted.
    // Basically, all the coefficients correspond to second order AR sections
    // except for every odd degree, where the last coefficient describes 
    // a first order AR section. The V-mode introduces an extra pole thereof.
    
    for (k=0; k < x->num_ar_sos; k++) {
        one_over_a0 = 1.0f / (1.0f-2.0f*one_over_dn*shn_b[k]);
        x->a2[k] = one_over_a0;
        x->a1[k] = (one_over_dn_sq*(shn_b[k]*shn_b[k]+shn_o[k]*shn_o[k])+
                    2.0f*(one_over_dn*shn_b[k]-1.0f)) * one_over_a0;
        x->gain_ar *= one_over_a0;
    }
    for (; k<x->num_ar_sec-x->bf_mode; k++) {
        one_over_a0 = 1.0f / (1.0f-one_over_dn*shn_b[k]);
        x->a1[k] = -one_over_a0;
        x->gain_ar *= one_over_a0;
    }
    if (x->bf_mode==VELOCITY) {
        x->a1[k] = -0.999f;
    }
    x->gain = x->gain_ar * x->delta_n / x->delta_n0;
    SETFLOAT(list,x->max_wng);
    radialBeamformSetWNG(x,gensym("wng"),1,list);
    
}

static void radialBeamformSetDeltan0 (RadialBeamform *x,  t_object *attr, int argc, t_atom *argv) {
    t_atom list[1];
    float one_over_dn0,one_over_dn0_sq;
    int k;
    float dn0;
    float *shn_b, *shn_o;
    if ((argc<1)|(argv[0].a_type!=A_FLOAT)) {
        post("radial_beamform~: no value given for delta_n0");
        dn0=0;
    }
    else
        dn0 = atom_getfloat(&argv[0]);
    
    one_over_dn0 = 1.0f / dn0;
    one_over_dn0_sq = one_over_dn0*one_over_dn0;
    x->delta_n0 = dn0;
    x->gain = x->gain_ar * x->delta_n / x->delta_n0;
    
    // The radius controls the zeroes of the differentiated Hankel function
    // in the numerator in case of V-mode, else the Hankel function in the
    // numerator. Therefore, the MA coefficients are inserted.
    // Basically, all the coefficients correspond to second order MA sections
    // except for every odd degree+mode_sw, where the last coefficient describes 
    // a first order AR section.
    
    if (x->bf_mode==VELOCITY) {
        shn_b = sph_hdn_b[x->sh_degree];
        shn_o = sph_hdn_omega[x->sh_degree];
    }
    else {
        shn_b = sph_hn_b[x->sh_degree];
        shn_o = sph_hn_omega[x->sh_degree];
    }
    
    for (k=0; k<x->num_ma_sos; k++) {
        x->b0[k] = 1.0f-2.0f*one_over_dn0*shn_b[k];
        x->b1[k] = one_over_dn0_sq*(shn_b[k]*shn_b[k]+shn_o[k]*shn_o[k])+
        2.0f*(one_over_dn0*shn_b[k]-1.0f);
    }
    for (; k<x->num_ma_sec; k++) {
        x->b0[k] = 1.0f-one_over_dn0*shn_b[k];
    }
    SETFLOAT(list,x->max_wng);
    radialBeamformSetWNG(x,gensym("wng"),1,list);
    
}

static void radialBeamformerUpdateCoeffs (RadialBeamform *x)
{
    t_atom list[1];
    SETFLOAT(list,x->delta_n0);
    radialBeamformSetDeltan0(x,gensym("delta_n0"),1,list);
    SETFLOAT(list,x->delta_n);
    radialBeamformSetDeltan(x,gensym("delta_n"),1,list);
}


t_max_err radialBeamformSetn (RadialBeamform *x, t_object *attr, int argc, t_atom *argv) {
    int sh_degree, num_ar_sec, num_ma_sec, num_ar_sos, num_ma_sos, num_z1, num_z2;
    
    radialBeamformFreeFilters(x);
    
    if ((argc<1)|(argv[0].a_type!=A_FLOAT && (argv[0].a_type!=A_LONG))) {
        post("radial_beamform~: no value given for N");
        sh_degree=0;
    }
    else
        sh_degree = atom_getlong(&argv[0]);
    
    sh_degree=(sh_degree<0)?0:sh_degree;
    sh_degree=(sh_degree>25)?25:sh_degree;
    
    num_ar_sec = x->bf_mode + (int)((sh_degree+1)/2);
    num_ma_sec =   (int)((sh_degree+1+x->bf_mode)/2);
    num_ar_sos = num_ar_sec - sh_degree%2 - x->bf_mode;
    num_ma_sos = num_ma_sec - (sh_degree+x->bf_mode)%2;
    num_z2 = (num_ar_sos > num_ma_sos)? num_ar_sos : num_ma_sos;
    num_z1 = (num_ma_sec<num_ar_sec)?
num_ar_sec:num_ma_sec;
    
    if (num_z1>0)
        x->z1=(float*)calloc(num_z1,sizeof(float));
    
    if (num_ar_sec>0) {
        x->a1=(float*)calloc(num_ar_sec,sizeof(float));
        x->omega_breakpoints_den=(float*)calloc(num_ar_sec+num_ar_sos,sizeof(float));
    }
    
    if (num_ma_sec>0) {
        x->b0=(float*)calloc(num_ma_sec,sizeof(float));
        x->omega_breakpoints_num=(float*)calloc(num_ma_sec+num_ma_sos,sizeof(float));
    }
    
    if (num_z2>0) 
        x->z2=(float*)calloc(num_z2,sizeof(float));
    
    if (num_ar_sos)
        x->a2=(float*)calloc(num_ar_sos,sizeof(float)); 
    
    if (num_ma_sos>0)
        x->b1=(float*)calloc(num_ma_sos,sizeof(float));
    
    x->sh_degree=sh_degree;
    x->num_ar_sec=num_ar_sec;
    x->num_ma_sec=num_ma_sec;
    x->num_ar_sos=num_ar_sos;
    x->num_ma_sos=num_ma_sos;
    
    radialBeamformerUpdateCoeffs(x);

    return 0;
}

static void radialBeamformSetP (RadialBeamform *x) {
    t_atom list[1];
    x->bf_mode = PRESSURE;
    //   post("mode set to %d",x->bf_mode);
    SETFLOAT(list,x->sh_degree);
    radialBeamformSetn(x,NULL,1,list);
}

static void radialBeamformSetV (RadialBeamform *x) {
    t_atom list[1];
    x->bf_mode = VELOCITY;
    //   post("mode set to %d",x->bf_mode);
    SETFLOAT(list,x->sh_degree);
    radialBeamformSetn(x,NULL,1,list);
}


static t_int *radialBeamformPerform (t_int *arg) {// TODO
    int n,k,num_ar_sos, num_ma_sos, num_ar_fos, num_ma_fos, num_arma_sos;
    float w;
    RadialBeamform *x = (RadialBeamform*)(arg[1]);
    float *a1=x->a1;
    float *a2=x->a2;
    float *b1=x->b1;
    float *b0=x->b0;
    float *z1=x->z1;
    float *z2=x->z2;
    float *buf=x->buf;
    
    num_ar_sos = x->num_ar_sec - x->sh_degree%2 - x->bf_mode;
    num_ma_sos = x->num_ma_sec - (x->sh_degree+x->bf_mode)%2;
    num_arma_sos = (num_ar_sos < num_ma_sos)? num_ar_sos : num_ma_sos;
    
    for (n=0; n<x->block_sz; n++) {
        buf[n]=x->in[n] * x->gain;
        w=x->in[n];
    }
    
    for (k=0; k<num_arma_sos; k++, 
         z1++, z2++, a1++, a2++, b0++, b1++) { // sosARMA
        for (n=0; n<x->block_sz; n++) {
            w = buf[n] - *a1 * *z1 - *a2 * *z2;
            buf[n] = *b0 * w + *b1 * *z1 + *z2;
            *z2 = *z1;
            *z1 = w;
        }
    }
    for (; (k<num_ar_sos)&&(k<x->num_ma_sec); k++, 
         z1++, z2++, a1++, a2++, b0++) { // fosMA sosAR
        for (n=0; n<x->block_sz; n++) {
            w = buf[n] + *a1 * *z1 + *a2 * *z2;
            buf[n] = *b0 * w - *z1;
            *z2 = *z1;
            *z1 = w;
        }
    }
    for (; (k<num_ma_sos)&&(k<x->num_ar_sec); k++, 
         z1++, z2++, a1++, b0++, b1++) { //sosMA fosAR
        for (n=0; n<x->block_sz; n++) {
            w = buf[n] - *a1 * *z1;
            buf[n] = *b0 * w + *b1 * *z1 + *z2;
            *z2 = *z1;
            *z1 = w;
        }
    }
    for (; k<num_ar_sos; k++,
         z1++, z2++, a1++, a2++) { //sosAR
        for (n=0; n<x->block_sz; n++) {
            buf[n] = buf[n] + *a1 * *z1 + *a2 * *z2;
            *z2 = *z1;
            *z1 = buf[n];
        }
    }
    for (; k<num_ma_sos; k++,
         z1++, z2++, b0++, b1++) { //sosMA
        for (n=0; n<x->block_sz; n++) {
            w = *b0 * buf[n] + *b1 * *z1 + *z2;
            *z2 = *z1;
            *z1 = buf[n];
            buf[n] = w;
        }
    }
    for (; (k<x->num_ma_sec)&&(k<x->num_ar_sec); k++,
         z1++, b0++, a1++) { //fosARMA
        for (n=0; n<x->block_sz; n++) {
            w = buf[n] - *a1 * *z1;
            buf[n] = *b0 * w - *z1;
            *z1 = w;
        }
    }
    for (; k<x->num_ar_sec; k++,
         z1++, a1++) { //fosAR 
        for (n=0; n<x->block_sz; n++) {
            *z1 = buf[n] = buf[n] - *a1 * *z1;
        }
    }
    for (; k<x->num_ma_sec; k++,
         z1++, b0++) { //fosMA
        for (n=0; n<x->block_sz; n++) {
            w = *b0 * buf[n] - *z1;
            *z1 = buf[n];
            buf[n] = w;
        }
    }
    
    for (n=0; n<x->block_sz; n++) {
        x->out[n]=buf[n];
    }
    return(arg+2);
}

static void radialBeamformDsp (RadialBeamform *x, t_signal **sp) { 
    x->in=sp[0]->s_vec;
    x->out=sp[1]->s_vec;
    x->block_sz=sp[0]->s_n;
    if(x->buf!=0) {
        free(x->buf);
        x->buf=0;
    }
    x->buf=(t_float *)calloc(sp[0]->s_n,sizeof(t_float));
    
    dsp_add(radialBeamformPerform, 1, x, NULL);
}

static void *radialBeamformNew(t_symbol *s, int argc, t_atom *argv) {
    
    RadialBeamform *x = (RadialBeamform *)object_alloc(radialbeameq_class);
    
    int k;
    
    x->z1=0;
    x->z2=0;
    x->a1=0;
    x->a2=0;
    x->b0=0;
    x->b1=0;
    x->num_ma_sec=0;
    x->num_ar_sec=0;
    x->delta_n0=0.3f/343.0f*44100.0f/8.0f;
    x->delta_n=1.5f/343.0f*44100.0f/8.0f;
    x->max_wng=20;
    x->sh_degree=0;
    radialBeamformSetV(x);
    
    /*
    for (k=0; k<argc; k++) {
        if (atom_getsym(&argv[k])==gensym("delta_n0")) {
            k++;
            radialBeamformSetDeltan0 (x,gensym("delta_n0"),argc-k,argv+k);
        }
        else if (atom_getsym(&argv[k])==gensym("delta_n")) {
            k++;
            radialBeamformSetDeltan (x,gensym("delta_n"),argc-k,argv+k);
        }
        else if (atom_getsym(&argv[k])==gensym("wng")) {
            k++;
            radialBeamformSetWNG (x,gensym("wng"),argc-k,argv+k);
        }
        else if (atom_getsym(&argv[k])==gensym("n")) {
            k++;
            radialBeamformSetn (x,gensym("n"),argc-k,argv+k);
        }
        else if (atom_getsym(&argv[k])==gensym("v")) {
            radialBeamformSetV(x);
        }
        else if (atom_getsym(&argv[k])==gensym("p")) {
            radialBeamformSetP(x);
        }
    }
     */
    
    attr_args_process(x, argc, argv);
    
    // one signal in
    dsp_setup((t_pxobject*)x, 1);

    // no inplace
    x->x_obj.z_misc = Z_NO_INPLACE;
    
    // some list out
    x->list_outlet = outlet_new(&x->x_obj, "list");
    
    // one signal out
    outlet_new(&x->x_obj, "signal");

    return x;
}

t_max_err radialBeamformSetMode(RadialBeamform *x, t_object *attr, long argc, t_atom *argv);

t_max_err radialBeamformSetMode(RadialBeamform *x, t_object *attr, long argc, t_atom *argv) {
    
    Symbol* s;
    
    s = atom_getsym(argv);
    
    if(s == gensym("velocity")) {
        x->bf_mode_sym = gensym("velocity");
        radialBeamformSetV(x);
    }
    if(s == gensym("pressure")) {
        x->bf_mode_sym = gensym("pressure");
        radialBeamformSetP(x);
    }
    
    return 0;
}

int main(void) {
    
    t_class *c = class_new("radialbeameq~", (method)radialBeamformNew, (method)radialBeamformFree, sizeof(RadialBeamform), 0L, A_GIMME, 0);
    
    class_dspinit(c);
    
    version(0);
    
    CLASS_ATTR_LONG(c, "n", 0, RadialBeamform, sh_degree);
    CLASS_ATTR_FILTER_MIN(c, "n", 0);
    CLASS_ATTR_ACCESSORS(c, "n", 0, radialBeamformSetn);
    
    // needs special set get
    CLASS_ATTR_SYM(c, "mode", 0, RadialBeamform, bf_mode_sym);
    CLASS_ATTR_ACCESSORS(c, "mode", 0, radialBeamformSetMode);

    CLASS_ATTR_FLOAT(c, "max_wng", 0, RadialBeamform, max_wng);
    CLASS_ATTR_FILTER_MIN(c, "max_wng", 0);
    CLASS_ATTR_FILTER_MAX(c, "max_wng", 96);
    CLASS_ATTR_ACCESSORS(c, "max_wng", 0, radialBeamformSetWNG);
    
    CLASS_ATTR_FLOAT(c, "omega_cutoff", 0, RadialBeamform, omega_cutoff);
    CLASS_ATTR_FLOAT(c, "slope_cutoff", 0, RadialBeamform, slope_cutoff);

    CLASS_ATTR_FLOAT(c, "delta_n", 0, RadialBeamform, delta_n);
    CLASS_ATTR_ACCESSORS(c, "delta_n", 0, radialBeamformSetDeltan);
    CLASS_ATTR_FLOAT(c, "delta_n0", 0, RadialBeamform, delta_n0);
    CLASS_ATTR_ACCESSORS(c, "delta_n0", 0, radialBeamformSetDeltan);
    
    class_addmethod(c, (method)radialBeamformDsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)radialBeamformReset, "reset", 0);
    
	class_register(CLASS_BOX, c);
	radialbeameq_class = c;
    
	//common_symbols_init();
    
	return 0;
    
}


