/* vbap.c

written by Ville Pulkki 1999
Helsinki University of Technology 
and 
Unversity of California at Berkeley

Copyright:

This software is being provided to you, the licensee, by Ville Pulkki,
under the following license. By obtaining, using and/or copying this
software, you agree that you have read, understood, and will comply
with these terms and conditions: Permission to use, copy, modify and
distribute, including the right to grant others rights to distribute
at any tier, this software and its documentation for any purpose and
without fee or royalty is hereby granted, provided that you agree to
comply with the following copyright notice and statements, including
the disclaimer, and that the same appear on ALL copies of the software
and documentation, including modifications that you make for internal
use or for distribution:

Copyright 1998 by Ville Pulkki.  All rights reserved.  

Written by Ville Pulkki
Helsinki University of Technology
Laboratory of acoustics and audio signal processing
and
UC Berkeley
Center of new music and audio technologies

The software may be used, distributed, and included to commercial
products without any charges. When included to a commercial product,
the method "Vector Base Amplitude Panning" and its developer Ville
Pulkki must be referred to in documentation.

This software is provided "as is", and Ville Pulkki or Helsinki
University of Technology make no representations or warranties,
expressed or implied. By way of example, but not limitation, Helsinki
University of Technology or Ville Pulkki make no representations or
warranties of merchantability or fitness for any particular purpose or
that the use of the licensed software or documentation will not
infringe any third party patents, copyrights, trademarks or other
rights. The name of Ville Pulkki or Helsinki University of Technology
may not be used in advertising or publicity pertaining to distribution
of the software.



Portions copyright (c) 1999.  The Regents of the University of California
(Regents).

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

 */


#include <math.h>
#include "ext.h"  				/* you must include this - it contains the external object's link to max */

#define RES_ID 8561 			// resource ID for assistance (we'll add that later)
#define MAX_LS_SETS 100			// maximum number of loudspeaker sets (triplets or pairs) allowed
#define MAX_LS_AMOUNT 32        // maximum amount of loudspeakers, can be increased

typedef struct vbap				/* This defines the object as an entity made up of other things */
{
	Object x_ob;				
	long x_azi; 	// panning direction azimuth
	long x_ele;		// panning direction elevation			
	void *x_outlet0;				/* outlet creation - inlets are automatic */
	void *x_outlet1;				
	void *x_outlet2;				
	void *x_outlet3;				
	float x_set_inv_matx[MAX_LS_SETS][9];  // inverse matrice for each loudspeaker set
	float x_set_matx[MAX_LS_SETS][9];      // matrice for each loudspeaker set
	long x_lsset[MAX_LS_SETS][3];          // channel numbers of loudspeakers in each LS set 
	long x_lsset_available;                // have loudspeaker sets been defined with define_loudspeakers
	long x_lsset_amount;				   // amount of loudspeaker sets
	long x_ls_amount;                      // amount of loudspeakers
	long x_dimension;                      // 2 or 3
	long x_spread;                         // speading amount of virtual source (0-100)
	float x_spread_base[3];                // used to create uniform spreading
} t_vbap;

// Globals

void new_spread_dir(t_vbap *x, float spreaddir[3], float vscartdir[3], float spread_base[3]);
void new_spread_base(t_vbap *x, float spreaddir[3], float vscartdir[3]);
void *vbap_class;				
void cross_prod(float v1[3], float v2[3],
                float v3[3]);
additive_vbap(float *final_gs, float cartdir[3], t_vbap *x);
void vbap_bang(t_vbap *x);
void vbap_int(t_vbap *x, long n);
void vbap_matrix(t_vbap *x, Symbol *s, int ac, Atom *av);
void vbap_in1(t_vbap *x, long n);
void vbap_in2(t_vbap *x, long n);
void vbap_in3(t_vbap *x, long n);
void spread_it(t_vbap *x, float *final_gs);
void *vbap_new(Symbol *s, int ac, Atom *av); // using A_GIMME - typed message list
void vbap(float g[3], long ls[3], t_vbap *x);
void angle_to_cart(long azi, long ele, float res[3]);
void cart_to_angle(float cvec[3], float avec[3]);

/* above are the prototypes for the methods/procedures/functions you will use */

void main(void)
{
	setup(&vbap_class, vbap_new, 0L, (short)sizeof(t_vbap), 0L, A_GIMME, 0); 
	/* vbap_new = creation function, A_DEFLONG = its (optional) arguement is a long (32-bit) int */
	addbang((method)vbap_bang);			/* the procedure it uses when it gets a bang in the left inlet */
	addint((method)vbap_int);			/* the rocedure for an int in the left inlet (inlet 0) */
	addinx((method)vbap_in1, 1);		/* the rocedure for an int in the right inlet (inlet 1) */
	addinx((method)vbap_in2, 2);		/* the rocedure for an int in the right inlet (inlet 2) */
	addinx((method)vbap_in3, 3);
	addmess((method)vbap_matrix, "loudspeaker-matrices", A_GIMME, 0);
}


void angle_to_cart(long azi, long ele, float res[3])
// converts angular coordinates to cartesian
{ 
  float atorad = (2 * 3.1415927 / 360) ;
  res[0] = cos((float) azi * atorad) * cos((float) ele * atorad);
  res[1] = sin((float) azi * atorad) * cos((float) ele * atorad);
  res[2] = sin((float) ele * atorad);
}

void cart_to_angle(float cvec[3], float avec[3])
// converts cartesian coordinates to angular
{
  float tmp, tmp2, tmp3, tmp4;
  float atorad = (2 * 3.1415927 / 360) ;
  float pi =  3.1415927;
  float power;
  float dist, atan_y_per_x, atan_x_pl_y_per_z;
  float azi, ele;
  
  if(cvec[0]==0.0)
  	atan_y_per_x = pi / 2;
  else
    atan_y_per_x = atan(cvec[1] / cvec[0]);
  azi = atan_y_per_x / atorad;
  if(cvec[0]<0.0)
    azi +=180;
  dist = sqrt(cvec[0]*cvec[0] + cvec[1]*cvec[1]);
  if(cvec[2]==0.0)
    atan_x_pl_y_per_z = 0.0;
  else
    atan_x_pl_y_per_z = atan(cvec[2] / dist);
  if(dist == 0.0)
    if(cvec[2]<0.0)
      atan_x_pl_y_per_z = -pi/2.0;
    else
      atan_x_pl_y_per_z = pi/2.0;
  ele = atan_x_pl_y_per_z / atorad;
  dist = sqrtf(cvec[0] * cvec[0] +cvec[1] * cvec[1] +cvec[2]*cvec[2]);
  avec[0]=azi;
  avec[1]=ele;
  avec[2]=dist;
}


void vbap(float g[3], long ls[3], t_vbap *x)
{
  /* calculates gain factors using loudspeaker setup and given direction */
  float power;
  int i,j,k, gains_modified;
  float small_g;
  float big_sm_g, gtmp[3];
  long winner_set;
  float cartdir[3];
  float new_cartdir[3];
  float new_angle_dir[3];
  long dim = x->x_dimension;
  long neg_g_am, best_neg_g_am;
  
  // transfering the azimuth angle to a decent value
  while(x->x_azi > 180)
  	x->x_azi -= 360;
  while(x->x_azi < -179)
  	x->x_azi += 360;
  	
  // transferring the elevation to a decent value
  if(dim == 3){
  	while(x->x_ele > 180)
  		x->x_ele -= 360;
  	while(x->x_ele < -179)
  		x->x_ele += 360;
  } else
  	x->x_ele = 0;
  
  
  // go through all defined loudspeaker sets and find the set which
  // has all positive values. If such is not found, set with largest
  // minimum value is chosen. If at least one of gain factors of one LS set is negative
  // it means that the virtual source does not lie in that LS set. 
  
  angle_to_cart(x->x_azi,x->x_ele,cartdir);
  big_sm_g = -100000.0;   // initial value for largest minimum gain value
  best_neg_g_am=3; 		  // how many negative values in this set
  
  
  for(i=0;i<x->x_lsset_amount;i++){
    small_g = 10000000.0;
    neg_g_am = 3;
    for(j=0;j<dim;j++){
      gtmp[j]=0.0;
      for(k=0;k<dim;k++)
        gtmp[j]+=cartdir[k]* x->x_set_inv_matx[i][k+j*dim];
      if(gtmp[j] < small_g)
        small_g = gtmp[j];
      if(gtmp[j]>= -0.01)
      	neg_g_am--;
    }
    if(small_g > big_sm_g && neg_g_am <= best_neg_g_am){
      big_sm_g = small_g;
      best_neg_g_am = neg_g_am; 
      winner_set=i;
      g[0]=gtmp[0]; g[1]=gtmp[1];
      ls[0]= x->x_lsset[i][0]; ls[1]= x->x_lsset[i][1];
      if(dim==3){
      	g[2]=gtmp[2];
        ls[2]= x->x_lsset[i][2];
      } else {
        g[2]=0.0;
        ls[2]=0;
      }
    }
  }
  
  // If chosen set produced a negative value, make it zero and
  // calculate direction that corresponds  to these new
  // gain values. This happens when the virtual source is outside of
  // all loudspeaker sets. 
  
  if(dim==3){
  	gains_modified=0;
  	for(i=0;i<dim;i++)
  		if(g[i]<-0.01){
  			g[i]=0.0;
  			gains_modified=1;
  		}	
 	if(gains_modified==1){
 	 	new_cartdir[0] =  x->x_set_matx[winner_set][0] * g[0] 
 	 					+ x->x_set_matx[winner_set][1] * g[1]
 	 					+ x->x_set_matx[winner_set][2] * g[2];
 	 	new_cartdir[1] =  x->x_set_matx[winner_set][3] * g[0] 
 	 					+ x->x_set_matx[winner_set][4] * g[1] 
 	 					+ x->x_set_matx[winner_set][5] * g[2];
 	 	new_cartdir[2] =  x->x_set_matx[winner_set][6] * g[0] 
 	 					+ x->x_set_matx[winner_set][7] * g[1]
 	 					+ x->x_set_matx[winner_set][8] * g[2];
 	 	cart_to_angle(new_cartdir,new_angle_dir);
 	 	x->x_azi = (long) (new_angle_dir[0] + 0.5);
 	 	x->x_ele = (long) (new_angle_dir[1] + 0.5);
 	 }
  }
  
  power=sqrt(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);
  g[0] /= power;
  g[1] /= power;
  g[2] /= power;
}


void cross_prod(float v1[3], float v2[3],
                float v3[3]) 
// vector cross product            
{
  float length;
  v3[0] = (v1[1] * v2[2] ) - (v1[2] * v2[1]);
  v3[1] = (v1[2] * v2[0] ) - (v1[0] * v2[2]);
  v3[2] = (v1[0] * v2[1] ) - (v1[1] * v2[0]);

  length= sqrt(v3[0]*v3[0] + v3[1]*v3[1] + v3[2]*v3[2]);
  v3[0] /= length;
  v3[1] /= length;
  v3[2] /= length;
}

additive_vbap(float *final_gs, float cartdir[3], t_vbap *x)
// calculates gains to be added to previous gains, used in
// multiple direction panning (source spreading)
{
	float power;
    int i,j,k, gains_modified;
  	float small_g;
  	float big_sm_g, gtmp[3];
  	long winner_set;
  	float new_cartdir[3];
  	float new_angle_dir[3];
  	long dim = x->x_dimension;
  	long neg_g_am, best_neg_g_am;
	float g[3];
	long ls[3];
	
  	big_sm_g = -100000.0;
  	best_neg_g_am=3;
  
  	for(i=0;i<x->x_lsset_amount;i++){
  	  small_g = 10000000.0;
  	  neg_g_am = 3;
  	  for(j=0;j<dim;j++){
  	    gtmp[j]=0.0;
  	    for(k=0;k<dim;k++)
  	      gtmp[j]+=cartdir[k]* x->x_set_inv_matx[i][k+j*dim];
  	    if(gtmp[j] < small_g)
  	      small_g = gtmp[j];
  	    if(gtmp[j]>= -0.01)
  	    	neg_g_am--;
    	}
    	if(small_g > big_sm_g && neg_g_am <= best_neg_g_am){
      	big_sm_g = small_g;
      	best_neg_g_am = neg_g_am; 
      	winner_set=i;
      	g[0]=gtmp[0]; g[1]=gtmp[1];
      	ls[0]= x->x_lsset[i][0]; ls[1]= x->x_lsset[i][1];
      	if(dim==3){
      		g[2]=gtmp[2];
      	  	ls[2]= x->x_lsset[i][2];
      	} else {
      	  	g[2]=0.0;
      	  	ls[2]=0;
      	}
    	}
  	}

  	gains_modified=0;
  	for(i=0;i<dim;i++)
  		if(g[i]<-0.01){
  			gains_modified=1;
  		}
  
  	if(gains_modified != 1){
  		power=sqrt(g[0]*g[0] + g[1]*g[1] + g[2]*g[2]);
  		g[0] /= power;
  		g[1] /= power;
  		g[2] /= power;
  		
  		final_gs[ls[0]-1] += g[0];
  		final_gs[ls[1]-1] += g[1];
  		final_gs[ls[2]-1] += g[2];
  	}
}


void new_spread_dir(t_vbap *x, float spreaddir[3], float vscartdir[3], float spread_base[3])
// subroutine for spreading
{
	float beta,gamma;
	float a,b;
	float pi = 3.1415927;
	float power;
	
	gamma = acos(vscartdir[0] * spread_base[0] +
					vscartdir[1] * spread_base[1] +
					vscartdir[2] * spread_base[2])/pi*180;
	if(fabs(gamma) < 1){
		angle_to_cart(x->x_azi+90, 0, spread_base);
		gamma = acos(vscartdir[0] * spread_base[0] +
					vscartdir[1] * spread_base[1] +
					vscartdir[2] * spread_base[2])/pi*180;
	}
	beta = 180 - gamma;
	b=sin(x->x_spread * pi / 180) / sin(beta * pi / 180);
	a=sin((180- x->x_spread - beta) * pi / 180) / sin (beta * pi / 180);
	spreaddir[0] = a * vscartdir[0] + b * spread_base[0];
	spreaddir[1] = a * vscartdir[1] + b * spread_base[1];
	spreaddir[2] = a * vscartdir[2] + b * spread_base[2];
	
	power=sqrt(spreaddir[0]*spreaddir[0] + spreaddir[1]*spreaddir[1] 
				+ spreaddir[2]*spreaddir[2]);
  	spreaddir[0] /= power;
  	spreaddir[1] /= power;
  	spreaddir[2] /= power;
}

void new_spread_base(t_vbap *x, float spreaddir[3], float vscartdir[3])
// subroutine for spreading
{
	float d;
	float pi = 3.1415927;
	float power;
	
	d = cos(x->x_spread/180*pi);
	x->x_spread_base[0] = spreaddir[0] - d * vscartdir[0];
	x->x_spread_base[1] = spreaddir[1] - d * vscartdir[1];
	x->x_spread_base[2] = spreaddir[2] - d * vscartdir[2];
	power=sqrt(x->x_spread_base[0]*x->x_spread_base[0] + x->x_spread_base[1]*x->x_spread_base[1] 
				+ x->x_spread_base[2]*x->x_spread_base[2]);
  	x->x_spread_base[0] /= power;
  	x->x_spread_base[1] /= power;
  	x->x_spread_base[2] /= power;
}

void spread_it(t_vbap *x, float *final_gs)
// apply the sound signal to multiple panning directions
// that causes some spreading.
// See theory in paper V. Pulkki "Uniform spreading of amplitude panned
// virtual sources" in WASPAA 99

{
	float vscartdir[3];
	float spreaddir[16][3];
	float spreadbase[16][3];
	long i, spreaddirnum;
	float power;
	if(x->x_dimension == 3){
		spreaddirnum=16;
		angle_to_cart(x->x_azi,x->x_ele,vscartdir);
		new_spread_dir(x, spreaddir[0], vscartdir, x->x_spread_base);
		new_spread_base(x, spreaddir[0], vscartdir);
		cross_prod(x->x_spread_base, vscartdir, spreadbase[1]); // four orthogonal dirs
		cross_prod(spreadbase[1], vscartdir, spreadbase[2]);
		cross_prod(spreadbase[2], vscartdir, spreadbase[3]);
	
		// four between them
		for(i=0;i<3;i++) spreadbase[4][i] =  (x->x_spread_base[i] + spreadbase[1][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[5][i] =  (spreadbase[1][i] + spreadbase[2][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[6][i] =  (spreadbase[2][i] + spreadbase[3][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[7][i] =  (spreadbase[3][i] + x->x_spread_base[i]) / 2.0;
		
		// four at half spreadangle
		for(i=0;i<3;i++) spreadbase[8][i] =  (vscartdir[i] + x->x_spread_base[i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[9][i] =  (vscartdir[i] + spreadbase[1][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[10][i] =  (vscartdir[i] + spreadbase[2][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[11][i] =  (vscartdir[i] + spreadbase[3][i]) / 2.0;
		
		// four at quarter spreadangle
		for(i=0;i<3;i++) spreadbase[12][i] =  (vscartdir[i] + spreadbase[8][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[13][i] =  (vscartdir[i] + spreadbase[9][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[14][i] =  (vscartdir[i] + spreadbase[10][i]) / 2.0;
		for(i=0;i<3;i++) spreadbase[15][i] =  (vscartdir[i] + spreadbase[11][i]) / 2.0;
		
		additive_vbap(final_gs,spreaddir[0],x); 
		for(i=1;i<spreaddirnum;i++){
			new_spread_dir(x, spreaddir[i], vscartdir, spreadbase[i]);
			additive_vbap(final_gs,spreaddir[i],x); 
		}
	} else if (x->x_dimension == 2) {
		spreaddirnum=6;		
		
		angle_to_cart(x->x_azi - x->x_spread, 0, spreaddir[0]);
		angle_to_cart(x->x_azi - x->x_spread/2, 0, spreaddir[1]);
		angle_to_cart(x->x_azi - x->x_spread/4, 0, spreaddir[2]);
		angle_to_cart(x->x_azi + x->x_spread/4, 0, spreaddir[3]);
		angle_to_cart(x->x_azi + x->x_spread/2, 0, spreaddir[4]);
		angle_to_cart(x->x_azi + x->x_spread, 0, spreaddir[5]);
		
		for(i=0;i<spreaddirnum;i++)
			additive_vbap(final_gs,spreaddir[i],x); 
	} else
		return;
		
	if(x->x_spread > 70)
		for(i=0;i<x->x_ls_amount;i++){
			final_gs[i] += (x->x_spread - 70) / 30.0 * (x->x_spread - 70) / 30.0 * 10.0;
		}
	
	for(i=0,power=0.0;i<x->x_ls_amount;i++){
		power += final_gs[i] * final_gs[i];
	}
		
	power = sqrt(power);
	for(i=0;i<x->x_ls_amount;i++){
		final_gs[i] /= power;
	}
}	
	

void vbap_bang(t_vbap *x)			
// top level, vbap gains are calculated and outputted	
{
	Atom at[MAX_LS_AMOUNT]; 
	float g[3];
	long ls[3];
	long i;
	float *final_gs;
	
	final_gs = (float *) getbytes(x->x_ls_amount * sizeof(float));
	if(x->x_lsset_available ==1){
		vbap(g,ls, x);
		for(i=0;i<x->x_ls_amount;i++)
			final_gs[i]=0.0; 			
		for(i=0;i<x->x_dimension;i++)
			final_gs[ls[i]-1]=g[i];  
		if(x->x_spread != 0)
			spread_it(x,final_gs);
		for(i=0;i<x->x_ls_amount;i++)
			SETFLOAT(&at[i], final_gs[i]);
		outlet_list(x->x_outlet0, 0L, x->x_ls_amount, at);
		outlet_int(x->x_outlet1, x->x_azi); 
		outlet_int(x->x_outlet2, x->x_ele); 
		outlet_int(x->x_outlet3, x->x_spread); 
	}
	else
		post("vbap: Configure loudspeakers first!",0);
	freebytes(final_gs, x->x_ls_amount * sizeof(float)); // bug fix added 9/00
}

/*--------------------------------------------------------------------------*/

void vbap_int(t_vbap *x, long n)		// x = the instance of the object, n = the int received in the right inlet 
{
 // do something if an int comes in the left inlet???	
}

void vbap_matrix(t_vbap *x, Symbol *s, int ac, Atom *av)
// read in loudspeaker matrices
{
	long counter;
	long datapointer=0;
	long setpointer=0;
	long i;
	
 
 	if(ac>0) 
 		if(av[datapointer].a_type == A_LONG){
 			x->x_dimension = av[datapointer++].a_w.w_long;
 			x->x_lsset_available=1;
 		} else if(av[datapointer].a_type == A_FLOAT){
 			x->x_dimension = (long) av[datapointer++].a_w.w_float;
 			x->x_lsset_available=1;
 		} else {
 			post("Error in loudspeaker data!",0);
 			x->x_lsset_available=0;
 			return;
 		}
 	if(ac>1) 
 		if(av[datapointer].a_type == A_LONG)
 			x->x_ls_amount = av[datapointer++].a_w.w_long;
 		else if(av[datapointer].a_type == A_FLOAT)
 			x->x_ls_amount = (long) av[datapointer++].a_w.w_float;
 		else {
 			post("vbap: Error in loudspeaker data!",0);
 			x->x_lsset_available=0;
 			return;
 		}
 	else
 		x->x_lsset_available=0;
 	
 	if(x->x_dimension == 3)
 		counter = (ac - 2) / ((x->x_dimension * x->x_dimension*2) + x->x_dimension);
 	if(x->x_dimension == 2)
 		counter = (ac - 2) / ((x->x_dimension * x->x_dimension) + x->x_dimension);
 	x->x_lsset_amount=counter;

 	if(counter<=0){
 		post("vbap: Error in loudspeaker data!",0);
 		x->x_lsset_available=0;
 		return;
 	}
 	
 	while(counter-- > 0){
 		for(i=0; i < x->x_dimension; i++){
 			if(av[datapointer].a_type == A_LONG)
 				x->x_lsset[setpointer][i]=av[datapointer++].a_w.w_long;
 			else{
 				post("vbap: Error in loudspeaker data!",0);
 				x->x_lsset_available=0;
 				return;
 			}
 		}	
 		for(i=0; i < x->x_dimension*x->x_dimension; i++){
 			if(av[datapointer].a_type == A_FLOAT)
 				x->x_set_inv_matx[setpointer][i]=av[datapointer++].a_w.w_float;
 			else {
 				post("vbap: Error in loudspeaker data!",0);
 				x->x_lsset_available=0;
 				return;
 			}
 		}
 		if(x->x_dimension == 3){ 
 			for(i=0; i < x->x_dimension*x->x_dimension; i++){
 				if(av[datapointer].a_type == A_FLOAT)
 					x->x_set_matx[setpointer][i]=av[datapointer++].a_w.w_float;
 				else {
 					post("vbap: Error in loudspeaker data!",0);
 					x->x_lsset_available=0;
 					return;
 				}
 			}
 		}
 		setpointer++;
	}
	post("vbap: Loudspeaker setup configured!",0);
}

void vbap_in1(t_vbap *x, long n)				/* x = the instance of the object, n = the int received in the right inlet */
// panning angle azimuth
{
	x->x_azi = n;							/* store n in a global variable */
	
}

void vbap_in2(t_vbap *x, long n)				/* x = the instance of the object, n = the int received in the right inlet */
// panning angle elevation
{
	x->x_ele = n;							/* store n in a global variable */

}
/*--------------------------------------------------------------------------*/

void vbap_in3(t_vbap *x, long n)				/* x = the instance of the object, n = the int received in the right inlet */
// spread amount
{
	if (n<0) n = 0;
	if (n>100) n = 100;
	x->x_spread = n;							/* store n in a global variable */
	
}


void *vbap_new(Symbol *s, int ac, Atom *av)	
/* create new instance of object... MUST send it an int even if you do nothing with this int!! */
{
	t_vbap *x;
	x = (t_vbap *)newobject(vbap_class);
	
	intin(x,3);	
	intin(x,2);					/* create a second (int) inlet... remember right-to-left ordering in Max */
	intin(x,1);					/* create a second (int) inlet... remember right-to-left ordering in Max */
	x->x_outlet3 = intout(x);
	x->x_outlet2 = intout(x);	/* create an (int) outlet  - rightmost outlet first... */
	x->x_outlet1 = intout(x);	/* create an (int) outlet */
	x->x_outlet0 = listout(x);	/* create a (list) outlet */
	
	x->x_azi = 0;
	x->x_ele = 0;
	x->x_spread_base[0] = 0.0;
	x->x_spread_base[1] = 1.0;
	x->x_spread_base[2] = 0.0;
	x->x_spread = 0;
	x->x_lsset_available =0;
	if (ac>0) {
		if (av[0].a_type == A_LONG)
			x->x_azi = av[0].a_w.w_long;
		else if (av[0].a_type == A_FLOAT)
			x->x_azi = (long)av[0].a_w.w_float;		
	}
	if (ac>1) {
		if (av[1].a_type == A_LONG)
			x->x_ele = av[1].a_w.w_long;
		else if (av[1].a_type == A_FLOAT)
			x->x_ele = (long)av[1].a_w.w_float;	
	}
	return(x);					/* return a reference to the object instance */
}

