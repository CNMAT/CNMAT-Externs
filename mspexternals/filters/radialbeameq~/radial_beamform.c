#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sph_hn_zeros.h"
#include "m_pd.h"


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

static t_class *radial_beamform_class;

typedef struct {
   t_object x_obj;
   t_float r0;
   t_float r;
   int block_sz;
   int bf_mode;
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

   t_float *in;
   t_float *out;
   t_outlet *list_outlet;
   float *buf;
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
   cycles = (x->num_ar_sos<x->num_ma_sos)?x->num_ma_sos:x->num_ar_sos;
   for (n=0; n<cycles; n++) {
      x->z1[n]=0;
      x->z2[n]=0;
   }
   cycles = (x->num_ar_sec<x->num_ma_sec)?x->num_ma_sec:x->num_ar_sec;
   for (;n<cycles; n++)
      x->z1[n]=0;
}

// not yet...
static void radialBeamformInfo (RadialBeamform *x) {
   t_atom list[3];
   
   SETSYMBOL(&list[0],gensym("n"));
   SETFLOAT(&list[1],x->sh_degree);
   outlet_list(x->list_outlet, &s_list,2,list);
   
   if (x->bf_mode==VELOCITY)
      SETSYMBOL(&list[0],gensym("v"));
   else
      SETSYMBOL(&list[0],gensym("p"));
   outlet_list(x->list_outlet, &s_list,1,list);
   
   SETSYMBOL(&list[0],gensym("max_wng"));
   SETFLOAT(&list[1],x->max_wng);
   outlet_list(x->list_outlet, &s_list,2,list);
   
   SETSYMBOL(&list[0],gensym("omega_cutoff"));
   SETFLOAT(&list[1],x->omega_cutoff);
   outlet_list(x->list_outlet, &s_list,2,list);

   SETSYMBOL(&list[0],gensym("cutoff_slope"));
   SETFLOAT(&list[1],x->slope_cutoff);
   outlet_list(x->list_outlet, &s_list,2,list);

   SETSYMBOL(&list[0],gensym("delta_n"));
   SETFLOAT(&list[1],x->delta_n);
   outlet_list(x->list_outlet, &s_list,2,list);

   SETSYMBOL(&list[0],gensym("delta_n0"));
   SETFLOAT(&list[1],x->delta_n0);
   outlet_list(x->list_outlet, &s_list,2,list);

}

static void radialBeamformSetWNG (RadialBeamform *x,  t_symbol *s, int argc, t_atom *argv) {
   float max_wng,curr_lvl,next_lvl,curr_omega,next_omega,curr_slope,next_slope;
   float *shn_bpnum=sph_omega_bp[x->sh_degree];
   int n_bpnum=sph_bp_n[x->sh_degree];
   float *shn_bpden=sph_omega_bp[x->sh_degree];;
   int n_bpden=sph_bp_n[x->sh_degree];
   int knum=0;
   int kden=0;
   float one_over_dn=1.0f/x->delta_n;
   float one_over_dn0=1.0f/x->delta_n0;

   if ((argc<1)|(argv[0].a_type!=A_FLOAT)) {
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


static void radialBeamformSetDeltan (RadialBeamform *x, t_symbol *s, int argc, t_atom *argv) {
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

static void radialBeamformSetDeltan0 (RadialBeamform *x,  t_symbol *s, int argc, t_atom *argv) {
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
   
static void radialBeamformSetn (RadialBeamform *x, t_symbol *s, int argc, t_atom *argv) {
   int sh_degree, num_ar_sec, num_ma_sec, num_ar_sos, num_ma_sos, num_z1, num_z2;

   radialBeamformFreeFilters(x);
   
   if ((argc<1)|(argv[0].a_type!=A_FLOAT)) {
      post("radial_beamform~: no value given for N");
      sh_degree=0;
   }
   else
      sh_degree = atom_getint(&argv[0]);

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

}

static void radialBeamformSetP (RadialBeamform *x) {
   t_atom list[1];
   x->bf_mode = PRESSURE;
//   post("mode set to %d",x->bf_mode);
   SETFLOAT(list,x->sh_degree);
   radialBeamformSetn(x,gensym("n"),1,list);
}

static void radialBeamformSetV (RadialBeamform *x) {
   t_atom list[1];
   x->bf_mode = VELOCITY;
//   post("mode set to %d",x->bf_mode);
   SETFLOAT(list,x->sh_degree);
   radialBeamformSetn(x,gensym("n"),1,list);
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

   dsp_add(radialBeamformPerform, 1, x);
}


static void *radialBeamformNew(t_symbol *s, int argc, t_atom *argv) {
   RadialBeamform *x = (RadialBeamform *)pd_new(radial_beamform_class);
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

   for (k=0; k<argc; k++) {
      if (atom_getsymbol(&argv[k])==gensym("delta_n0")) {
    k++;
         radialBeamformSetDeltan0 (x,gensym("delta_n0"),argc-k,argv+k);
      }
      else if (atom_getsymbol(&argv[k])==gensym("delta_n")) {
    k++;
         radialBeamformSetDeltan (x,gensym("delta_n"),argc-k,argv+k);
      }
      else if (atom_getsymbol(&argv[k])==gensym("wng")) {
    k++;
         radialBeamformSetWNG (x,gensym("wng"),argc-k,argv+k);
      }
      else if (atom_getsymbol(&argv[k])==gensym("n")) {
    k++;
         radialBeamformSetn (x,gensym("n"),argc-k,argv+k);
      }
      else if (atom_getsymbol(&argv[k])==gensym("v")) {
    radialBeamformSetV(x);
      }
      else if (atom_getsymbol(&argv[k])==gensym("p")) {
    radialBeamformSetP(x);
      }
   }
   outlet_new (&x->x_obj, &s_signal);
   x->list_outlet=outlet_new (&x->x_obj, &s_list);
   return x;
}

void radial_beamform_setup(void)
{
   radial_beamform_class = class_new(gensym("sh_radial_beamform~"), (t_newmethod)radialBeamformNew, 
         (t_method) radialBeamformFree, sizeof(RadialBeamform), CLASS_DEFAULT, A_GIMME, 0);
   class_addmethod(radial_beamform_class, nullfn, gensym("signal"), 0);
   class_addmethod(radial_beamform_class, (t_method) radialBeamformDsp, gensym ("dsp"), 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetDeltan, gensym("delta_n"), A_GIMME, 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetDeltan0, gensym("delta_n0"), A_GIMME, 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetWNG, gensym("wng"), A_GIMME, 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetV, gensym("v"), 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetP, gensym("p"), 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformSetn, gensym("n"), A_GIMME, 0);
   class_addmethod(radial_beamform_class, (t_method)radialBeamformReset, gensym("reset"), 0);
   class_addbang(radial_beamform_class, (t_method)radialBeamformInfo);
}
