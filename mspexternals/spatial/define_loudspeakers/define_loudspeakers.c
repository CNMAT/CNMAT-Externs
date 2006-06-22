/* define_loudspeakers.c

DESCRIPTION: Ville Pulkki's final CNMAT version of this VBAP-related object, long superceded by the version he's still maintaining.

written by Ville Pulkki 1999
Helsinki University of Technology 
and 
Unversity of California at Berkeley


Copyright

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
#define MAX_LS_AMOUNT 32		// maximum amount of loudspeakers, can be raised
#define MIN_VOL_P_SIDE_LGTH 0.01  


/* A struct for a loudspeaker instance */
typedef struct {  // distance value is 1.0 == unit vectors
  float x;  // cartesian coordinates
  float y;
  float z;
  float azi;  // polar coordinates
  float ele;
  int channel_nbr;  // which speaker channel number 
} t_ls;

/* A struct for all loudspeaker sets */
typedef struct t_ls_set {
  int ls_nos[3];  // channel numbers
  float inv_mx[9]; // inverse 3x3 or 2x2 matrix
  struct t_ls_set *next;  // next set (triplet or pair)
} t_ls_set;

typedef struct 				/* This defines the object as an entity made up of other things */
{
	Object x_ob;				/* gotta say this... it creates a reference to your object */
	long x_ls_read;	 			// 1 if loudspeaker directions have been read
	long x_triplets_specified;  // 1 if loudspeaker triplets have been chosen
	t_ls x_ls[MAX_LS_AMOUNT];   // loudspeakers
	t_ls_set *x_ls_set;			// loudspeaker sets
	void *x_outlet0;			/* outlet creation - inlets are automatic */
	long x_ls_amount;			// number of loudspeakers
	long x_dimension;		    // 2 (horizontal arrays) or 3 (3d setups)
} t_def_ls;

void *def_ls_class;				/* so max can identify your object */
void def_ls_bang(t_def_ls *x);
void def_ls_int(t_def_ls *x, long n);
void def_ls_read_directions(t_def_ls *x, Symbol *s, int ac, Atom *av);
void def_ls_read_triplets(t_def_ls *x, Symbol *s, int ac, Atom *av);
void *def_ls_new(Symbol *s, int ac, Atom *av); // using A_GIMME - typed message list
void def_ls(float g[3], long ls[3], t_def_ls *x);
void ls_angles_to_cart(t_ls *ls);
void choose_ls_triplets(t_def_ls *x);
int any_ls_inside_triplet(int a, int b, int c,t_ls lss[MAX_LS_AMOUNT],int ls_amount);
void add_ldsp_triplet(int i, int j, int k, t_def_ls *x);
float vec_angle(t_ls v1, t_ls v2);
float vec_length(t_ls v1);
float vec_prod(t_ls v1, t_ls v2);
float vec_prod(t_ls v1, t_ls v2);
float vol_p_side_lgth(int i, int j,int k, t_ls  lss[MAX_LS_AMOUNT] );
void cross_prod(t_ls v1,t_ls v2, t_ls *res);
int lines_intersect(int i,int j,int k,int l,t_ls lss[MAX_LS_AMOUNT]);
void  calculate_3x3_matrixes(t_def_ls *x);
void choose_ls_tuplets(t_def_ls *x);
int calc_2D_inv_tmatrix(float azi1,float azi2, float inv_mat[4]);
void sort_2D_lss(t_ls lss[MAX_LS_AMOUNT], int sorted_lss[MAX_LS_AMOUNT], 
                 int ls_amount);

/* above are the prototypes for the methods/procedures/functions you will use */

void main(void)
{
	setup(&def_ls_class, def_ls_new, 0L, (short)sizeof(t_def_ls), 0L, A_GIMME, 0); 
	/* def_ls_new = creation function, A_DEFLONG = its (optional) arguement is a long (32-bit) int */
	
	addbang((method)def_ls_bang);			/* the procedure it uses when it gets a bang in the left inlet */
	addint((method)def_ls_int);			/* the rocedure for an int in the left inlet (inlet 0) */
	addmess((method)def_ls_read_directions, "ls-directions", A_GIMME, 0);	
	addmess((method)def_ls_read_triplets, "ls-triplets", A_GIMME, 0);
}


void def_ls_bang(t_def_ls *x)						/* x = reference to this instance of the object */ 
{   // calculate and print out chosen loudspeaker sets and corresponding  matrices
	Atom at[MAX_LS_AMOUNT*2+1]; 
	float g[3];
	long ls[3];
	long i;
	
	if(x->x_ls_read == 1){
		if(x->x_ls_amount < x->x_dimension){
			post("define-loudspeakers: Too few loudspeakers!",0);
			return;
		} else if(x->x_dimension == 3){
			if(x->x_triplets_specified==0)
				choose_ls_triplets(x);
			calculate_3x3_matrixes(x);
		} else if(x->x_dimension == 2)
		  	choose_ls_tuplets(x);
		  else {
		  	post("define-loudspeakers: Error in loudspeaker direction data");
		  	post("dimension azimuth1 [elevation1] azimuth2 [elevation2]...");
		  	post("dimension == 2 for horizontal ls arrays");
		  	post("dimension == 3 for 3-D ls arrays (speakers also upward and/or downward ",0);
		  }
	} else{
		post("define-loudspeakers: Error in loudspeaker direction data",0);
		post("dimension azimuth1 [elevation1] azimuth2 [elevation2]...",0);
		post("dimension == 2 for horizontal ls arrays",0);
		post("dimension == 3 for 3-D ls arrays (speakers also upward and/or downward ",0);
	}
}

/*--------------------------------------------------------------------------*/

void def_ls_int(t_def_ls *x, long n)		// x = the instance of the object, n = the int received in the right inlet 
{
 // do something if an int comes in the left inlet???	
}

void def_ls_read_triplets(t_def_ls *x, Symbol *s, int ac, Atom *av)
// when loudspeaker triplets come in a message
{
	long l1,l2,l3,i;
	t_ls_set *trip_ptr,  *tmp_ptr, *prev;
	if(x->x_ls_read == 0){
		post("define_loudspeakers: Define loudspeaker directions first!",0);
		return;
	}
	
	if(x->x_dimension == 2){
		post("define_loudspeakers: Can't specify loudspeaker triplets in 2-D setup!",0);
		return;
	}
		
 	trip_ptr = x->x_ls_set;
  	prev = NULL;
  	while (trip_ptr != NULL){
   	     tmp_ptr = trip_ptr;
   	     trip_ptr = trip_ptr->next;
   	     freebytes(tmp_ptr, sizeof (struct t_ls_set));
    }
    x->x_ls_set = NULL;
	
	for(i=0;i<ac;i+=3){
		if(av[i].a_type == A_LONG) 
			l1 = av[i].a_w.w_long;
		else if(av[i].a_type == A_FLOAT) 
			l1 =  (long) av[i].a_w.w_float;
		if(av[i+1].a_type == A_LONG) 
			l2 = av[i+1].a_w.w_long;
		else if(av[i+1].a_type == A_FLOAT) 
			l2 =  (long) av[i+1].a_w.w_float;
		if(av[i+2].a_type == A_LONG) 
			l3 = av[i+2].a_w.w_long;
		else if(av[i+2].a_type == A_FLOAT) 
			l3 =  (long) av[i+2].a_w.w_float;
  		add_ldsp_triplet(l1-1,l2-1,l3-1,x);
  	}
  	x->x_triplets_specified=1;
}



void def_ls_read_directions(t_def_ls *x, Symbol *s, int ac, Atom *av)
// when loudspeaker directions come in a message
{
	t_ls_set *trip_ptr, *prev, *tmp_ptr;
	long i,pointer,newlyread;	
	newlyread = 0;
	if(av[0].a_type == A_LONG){
		 x->x_dimension= av[0].a_w.w_long;
		 newlyread = 1;
	} else if(av[0].a_type == A_FLOAT) {
			x->x_dimension= (int) av[0].a_w.w_float;
			newlyread = 1;
		} else x->x_dimension= 0;
		
 	if(x->x_dimension <2 || x->x_dimension >3){
 		post("define-loudspeakers: Dimension has to be 2 or 3!",0);
		return;
 	}
 		
	pointer = 1;
	x->x_ls_amount= (ac-1) / (x->x_dimension - 1);
	for(i=0; i < x->x_ls_amount;i++){
		if(av[0].a_type == A_LONG) 
			x->x_ls[i].azi = (float) av[pointer].a_w.w_long;
		else if(av[0].a_type == A_FLOAT) 
				x->x_ls[i].azi =  av[pointer].a_w.w_float;
		else {
		 	post("define-loudspeakers: Error in loudspeaker data!",0);
		 	 newlyread =0;
		 	 return;
		}
		pointer++;
		if(x->x_dimension == 3){
			if(av[0].a_type == A_LONG) 
				x->x_ls[i].ele = (float) av[pointer].a_w.w_long;
			else if(av[0].a_type == A_FLOAT) 
				x->x_ls[i].ele =  av[pointer].a_w.w_float;
			 else {
			 	post("define-loudspeakers: Error in loudspeaker data!",0);
			 	newlyread =0;
			 	return;
			 }
			pointer++;
		} else
			x->x_ls[i].ele = 0.0; 	// 2-D case
	}
	
	if(newlyread == 1){ 
		x->x_ls_read = 1;  // preprocess data
		for(i=0;i<x->x_ls_amount;i++)
			ls_angles_to_cart(&x->x_ls[i]);	
 		trip_ptr = x->x_ls_set;
  		prev = NULL;
  		while (trip_ptr != NULL){ // remove old matrices
   		     tmp_ptr = trip_ptr;
   		     trip_ptr = trip_ptr->next;
   		     freebytes(tmp_ptr, sizeof (struct t_ls_set));
    	}
    	x->x_ls_set = NULL;
    }
    x->x_triplets_specified=0;
}

/*--------------------------------------------------------------------------*/

void ls_angles_to_cart(t_ls *ls)
// convert angular direction to cartesian
{
  float atorad = (2 * 3.1415927 / 360) ;
  float azi = ls->azi;
  float ele = ls->ele;
  ls->x = cos((float) azi * atorad) * cos((float) ele * atorad);
  ls->y = sin((float) azi * atorad) * cos((float) ele * atorad);
  ls->z = sin((float) ele * atorad);
}

void *def_ls_new(Symbol *s, int ac, Atom *av)	/* create new instance of object... MUST 
													send it an int even if you do nothing with this int!! */

{
	long i,pointer;
	t_def_ls *x;
	x = (t_def_ls *)newobject(def_ls_class);
	
	x->x_ls_read = 0;
	if(av[0].a_type == A_LONG){
		 x->x_dimension= av[0].a_w.w_long;
		 x->x_ls_read = 1;
	} else if(av[0].a_type == A_FLOAT) {
			x->x_dimension= (int) av[0].a_w.w_float;
			x->x_ls_read = 1;
		} else x->x_dimension= 0;
		
	if(x->x_dimension <2 || x->x_dimension >3){
 		post("define-loudspeakers: Dimension has to be 2 or 3!",0);
		return(0);
 	}
 	
 	
	pointer = 1;
	x->x_ls_amount= (ac-1) / (x->x_dimension - 1);

	// read loudspeaker direction angles  
	for(i=0; i < x->x_ls_amount;i++){
		if(av[0].a_type == A_LONG) 
			x->x_ls[i].azi = (float) av[pointer].a_w.w_long;
		else if(av[0].a_type == A_FLOAT) 
				x->x_ls[i].azi =  av[pointer].a_w.w_float;
		else {
		 	post("define-loudspeakers: Error in loudspeaker data!",0);
		 	 x->x_ls_read =0;
		 	 return;
		}
		pointer++;
		if(x->x_dimension == 3){  // 3-D 
			if(av[0].a_type == A_LONG) 
				x->x_ls[i].ele = (float) av[pointer].a_w.w_long;
			else if(av[0].a_type == A_FLOAT) 
				x->x_ls[i].ele =  av[pointer].a_w.w_float;
			 else {
			 	post("define-loudspeakers: Error in loudspeaker data!",0);
			 	x->x_ls_read =0;
			 	return;
			 }
			pointer++;
		} else
			x->x_ls[i].ele = 0.0; 	// in 2d elevation is zero
	}
	
	if(x->x_ls_read == 1)
		for(i=0;i<x->x_ls_amount;i++)
			ls_angles_to_cart(&x->x_ls[i]);	
	x->x_triplets_specified=0;
	x->x_outlet0 =  outlet_new(x, 0L);	/* create a (list) outlet */
	x->x_ls_set = NULL;
	return(x);					/* return a reference to the object instance */
}



void choose_ls_triplets(t_def_ls *x) 
     /* Selects the loudspeaker triplets, and
      calculates the inversion matrices for each selected triplet.
     A line (connection) is drawn between each loudspeaker. The lines
     denote the sides of the triangles. The triangles should not be 
     intersecting. All crossing connections are searched and the 
     longer connection is erased. This yields non-intesecting triangles,
     which can be used in panning. 
     See theory in paper Pulkki, V. Lokki, T. "Creating Auditory Displays
     with Multiple Loudspeakers Using VBAP: A Case Study with
     DIVA Project" in International Conference on 
     Auditory Displays -98.*/
{
  int i,j,k,l,m,li, table_size;
  int *i_ptr;
  t_ls vb1,vb2,tmp_vec;
  int connections[MAX_LS_AMOUNT][MAX_LS_AMOUNT];
  float angles[MAX_LS_AMOUNT];
  int sorted_angles[MAX_LS_AMOUNT];
  float distance_table[((MAX_LS_AMOUNT * (MAX_LS_AMOUNT - 1)) / 2)];
  int distance_table_i[((MAX_LS_AMOUNT * (MAX_LS_AMOUNT - 1)) / 2)];
  int distance_table_j[((MAX_LS_AMOUNT * (MAX_LS_AMOUNT - 1)) / 2)];
  float distance;
  t_ls_set *trip_ptr, *prev, *tmp_ptr;
  int ls_amount = x->x_ls_amount;
  t_ls *lss = x->x_ls;
  if (ls_amount == 0) {
    post("define-loudspeakers: Number of loudspeakers is zero",0);
    return;
  }
  
 
  for(i=0;i<ls_amount;i++)
    for(j=i+1;j<ls_amount;j++)
      for(k=j+1;k<ls_amount;k++){
        if(vol_p_side_lgth(i,j,k, x->x_ls) > MIN_VOL_P_SIDE_LGTH){
          connections[i][j]=1;
          connections[j][i]=1;
          connections[i][k]=1;
          connections[k][i]=1;
          connections[j][k]=1;
          connections[k][j]=1;
          add_ldsp_triplet(i,j,k,x);
        }
      }
   

  /*calculate distancies between all lss and sorting them*/
  table_size =(((ls_amount - 1) * (ls_amount)) / 2); 
  for(i=0;i<table_size; i++)
    distance_table[i] = 100000.0;
  for(i=0;i<ls_amount;i++){ 
    for(j=(i+1);j<ls_amount; j++){ 
     if(connections[i][j] == 1) {
        distance = fabs(vec_angle(lss[i],lss[j]));
        k=0;
        while(distance_table[k] < distance)
          k++;
        for(l=(table_size - 1);l > k ;l--){
          distance_table[l] = distance_table[l-1];
          distance_table_i[l] = distance_table_i[l-1];
          distance_table_j[l] = distance_table_j[l-1];
        }
        distance_table[k] = distance;
        distance_table_i[k] = i;
        distance_table_j[k] = j;
      } else
        table_size--;
    }
  }

  /* disconnecting connections which are crossing shorter ones,
     starting from shortest one and removing all that cross it,
     and proceeding to next shortest */
  for(i=0; i<(table_size); i++){
    int fst_ls = distance_table_i[i];
  int sec_ls = distance_table_j[i];
    if(connections[fst_ls][sec_ls] == 1)
      for(j=0; j<ls_amount ; j++)
        for(k=j+1; k<ls_amount; k++)
          if( (j!=fst_ls) && (k != sec_ls) && (k!=fst_ls) && (j != sec_ls)){
            if(lines_intersect(fst_ls, sec_ls, j,k,x->x_ls) == 1){
              connections[j][k] = 0;
              connections[k][j] = 0;
            }
          }
  }

  /* remove triangles which had crossing sides
     with smaller triangles or include loudspeakers*/
  trip_ptr = x->x_ls_set;
  prev = NULL;
  while (trip_ptr != NULL){
    i = trip_ptr->ls_nos[0];
    j = trip_ptr->ls_nos[1];
    k = trip_ptr->ls_nos[2];
    if(connections[i][j] == 0 || 
       connections[i][k] == 0 || 
       connections[j][k] == 0 ||
     any_ls_inside_triplet(i,j,k,x->x_ls,ls_amount) == 1 ){
      if(prev != NULL) {
        prev->next = trip_ptr->next;
        tmp_ptr = trip_ptr;
        trip_ptr = trip_ptr->next;
        freebytes(tmp_ptr, sizeof (struct t_ls_set));
      } else {
        x->x_ls_set = trip_ptr->next;
        tmp_ptr = trip_ptr;
        trip_ptr = trip_ptr->next;
        freebytes(tmp_ptr, sizeof (struct t_ls_set));
      }
    } else {
      prev = trip_ptr;
      trip_ptr = trip_ptr->next;

    }
  }
  x->x_triplets_specified=1;
}


int any_ls_inside_triplet(int a, int b, int c,t_ls lss[MAX_LS_AMOUNT],int ls_amount)
   /* returns 1 if there is loudspeaker(s) inside given ls triplet */
{
  float invdet;
  t_ls *lp1, *lp2, *lp3;
  float invmx[9];
  int i,j,k;
  float tmp;
  int any_ls_inside, this_inside;

  lp1 =  &(lss[a]);
  lp2 =  &(lss[b]);
  lp3 =  &(lss[c]);

  /* matrix inversion */
  invdet = 1.0 / (  lp1->x * ((lp2->y * lp3->z) - (lp2->z * lp3->y))
                    - lp1->y * ((lp2->x * lp3->z) - (lp2->z * lp3->x))
                    + lp1->z * ((lp2->x * lp3->y) - (lp2->y * lp3->x)));
  
  invmx[0] = ((lp2->y * lp3->z) - (lp2->z * lp3->y)) * invdet;
  invmx[3] = ((lp1->y * lp3->z) - (lp1->z * lp3->y)) * -invdet;
  invmx[6] = ((lp1->y * lp2->z) - (lp1->z * lp2->y)) * invdet;
  invmx[1] = ((lp2->x * lp3->z) - (lp2->z * lp3->x)) * -invdet;
  invmx[4] = ((lp1->x * lp3->z) - (lp1->z * lp3->x)) * invdet;
  invmx[7] = ((lp1->x * lp2->z) - (lp1->z * lp2->x)) * -invdet;
  invmx[2] = ((lp2->x * lp3->y) - (lp2->y * lp3->x)) * invdet;
  invmx[5] = ((lp1->x * lp3->y) - (lp1->y * lp3->x)) * -invdet;
  invmx[8] = ((lp1->x * lp2->y) - (lp1->y * lp2->x)) * invdet;

  any_ls_inside = 0;
  for(i=0; i< ls_amount; i++) {
    if (i != a && i!=b && i != c){
      this_inside = 1;
      for(j=0; j< 3; j++){
        tmp = lss[i].x * invmx[0 + j*3];
        tmp += lss[i].y * invmx[1 + j*3];
        tmp += lss[i].z * invmx[2 + j*3];
        if(tmp < -0.001)
          this_inside = 0;
      }
      if(this_inside == 1)
        any_ls_inside=1;
    }
  }
  return any_ls_inside;
}

void add_ldsp_triplet(int i, int j, int k, t_def_ls *x)
     /* adds i,j,k triplet to structure*/
{
  struct t_ls_set *trip_ptr, *prev;
  trip_ptr = x->x_ls_set;
  prev = NULL;
  while (trip_ptr != NULL){
    prev = trip_ptr;
    trip_ptr = trip_ptr->next;
  }
  trip_ptr = (struct t_ls_set*) 
    getbytes (sizeof (struct t_ls_set));
  if(prev == NULL)
    x->x_ls_set = trip_ptr;
  else 
    prev->next = trip_ptr;
  trip_ptr->next = NULL;
  trip_ptr->ls_nos[0] = i;
  trip_ptr->ls_nos[1] = j;
 trip_ptr->ls_nos[2] = k;
}




float vec_angle(t_ls v1, t_ls v2)
// angle between two loudspeakers
{
  float inner= ((v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)/
              (vec_length(v1) * vec_length(v2)));
  if(inner > 1.0)
    inner= 1.0;
  if (inner < -1.0)
    inner = -1.0;
  return fabs( acos( inner));
}

float vec_length(t_ls v1)
// length of a vector
{
  return (sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z));
}

float vec_prod(t_ls v1, t_ls v2)
// vector dot product
{
  return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}


float vol_p_side_lgth(int i, int j,int k, t_ls  lss[MAX_LS_AMOUNT] ){
  /* calculate volume of the parallelepiped defined by the loudspeaker
     direction vectors and divide it with total length of the triangle sides. 
     This is used when removing too narrow triangles. */

  float volper, lgth;
  t_ls xprod;
  cross_prod(lss[i], lss[j], &xprod);
  volper = fabsf(vec_prod(xprod, lss[k]));
  lgth = (fabsf(vec_angle(lss[i],lss[j])) 
          + fabsf(vec_angle(lss[i],lss[k])) 
          + fabsf(vec_angle(lss[j],lss[k])));
  if(lgth>0.00001)
    return volper / lgth;
  else
    return 0.0;
}

void cross_prod(t_ls v1,t_ls v2, 
                t_ls *res) 
// vector cross product
{
  float length;
  res->x = (v1.y * v2.z ) - (v1.z * v2.y);
  res->y = (v1.z * v2.x ) - (v1.x * v2.z);
  res->z = (v1.x * v2.y ) - (v1.y * v2.x);

  length= vec_length(*res);
  res->x /= length;
  res->y /= length;
  res->z /= length;
}


int lines_intersect(int i,int j,int k,int l,t_ls  lss[MAX_LS_AMOUNT])
     /* checks if two lines intersect on 3D sphere 
       */
{
 t_ls v1;
  t_ls v2;
  t_ls v3, neg_v3;
  float angle;
  float dist_ij,dist_kl,dist_iv3,dist_jv3,dist_inv3,dist_jnv3;
  float dist_kv3,dist_lv3,dist_knv3,dist_lnv3;

  cross_prod(lss[i],lss[j],&v1);
  cross_prod(lss[k],lss[l],&v2);
  cross_prod(v1,v2,&v3);

  neg_v3.x= 0.0 - v3.x;
  neg_v3.y= 0.0 - v3.y;
  neg_v3.z= 0.0 - v3.z;

  dist_ij = (vec_angle(lss[i],lss[j]));
  dist_kl = (vec_angle(lss[k],lss[l]));
  dist_iv3 = (vec_angle(lss[i],v3));
  dist_jv3 = (vec_angle(v3,lss[j]));
  dist_inv3 = (vec_angle(lss[i],neg_v3));
  dist_jnv3 = (vec_angle(neg_v3,lss[j]));
  dist_kv3 = (vec_angle(lss[k],v3));
  dist_lv3 = (vec_angle(v3,lss[l]));
  dist_knv3 = (vec_angle(lss[k],neg_v3));
  dist_lnv3 = (vec_angle(neg_v3,lss[l]));

  /* if one of loudspeakers is close to crossing point, don't do anything*/
  if(fabsf(dist_iv3) <= 0.01 || fabsf(dist_jv3) <= 0.01 || 
  fabsf(dist_kv3) <= 0.01 || fabsf(dist_lv3) <= 0.01 ||
     fabsf(dist_inv3) <= 0.01 || fabsf(dist_jnv3) <= 0.01 || 
     fabsf(dist_knv3) <= 0.01 || fabsf(dist_lnv3) <= 0.01 )
    return(0);

  // if crossing point is on line between both loudspeakers return 1
  if (((fabsf(dist_ij - (dist_iv3 + dist_jv3)) <= 0.01 ) &&
       (fabsf(dist_kl - (dist_kv3 + dist_lv3))  <= 0.01)) ||
      ((fabsf(dist_ij - (dist_inv3 + dist_jnv3)) <= 0.01)  &&
       (fabsf(dist_kl - (dist_knv3 + dist_lnv3)) <= 0.01 ))) {
    return (1);
  } else {
    return (0);
  }
}

void  calculate_3x3_matrixes(t_def_ls *x)
     /* Calculates the inverse matrices for 3D */
{  
  float invdet;
  t_ls *lp1, *lp2, *lp3;
  float *invmx;
  float *ptr;
  struct t_ls_set *tr_ptr = x->x_ls_set;
  int triplet_amount = 0, ftable_size,i,j,k, pointer,list_length=0;
  Atom *at;
  t_ls *lss = x->x_ls;
  
  if (tr_ptr == NULL){
    post("define-loudspeakers: Not valid 3-D configuration\n",1);
    return;
  }
	
  /* counting triplet amount */
  while(tr_ptr != NULL){
    triplet_amount++;
    tr_ptr = tr_ptr->next;
  }
  tr_ptr = x->x_ls_set;
  list_length= triplet_amount * 21 + 3;
  at= (Atom *) getbytes(list_length*sizeof(Atom));
  
  SETLONG(&at[0], x->x_dimension);
  SETLONG(&at[1], x->x_ls_amount);
  pointer=2;
  
  while(tr_ptr != NULL){
    lp1 =  &(lss[tr_ptr->ls_nos[0]]);
    lp2 =  &(lss[tr_ptr->ls_nos[1]]);
    lp3 =  &(lss[tr_ptr->ls_nos[2]]);

    /* matrix inversion */
    invmx = tr_ptr->inv_mx;
    invdet = 1.0 / (  lp1->x * ((lp2->y * lp3->z) - (lp2->z * lp3->y))
                    - lp1->y * ((lp2->x * lp3->z) - (lp2->z * lp3->x))
                    + lp1->z * ((lp2->x * lp3->y) - (lp2->y * lp3->x)));

    invmx[0] = ((lp2->y * lp3->z) - (lp2->z * lp3->y)) * invdet;
    invmx[3] = ((lp1->y * lp3->z) - (lp1->z * lp3->y)) * -invdet;
  	invmx[6] = ((lp1->y * lp2->z) - (lp1->z * lp2->y)) * invdet;
    invmx[1] = ((lp2->x * lp3->z) - (lp2->z * lp3->x)) * -invdet;
    invmx[4] = ((lp1->x * lp3->z) - (lp1->z * lp3->x)) * invdet;
    invmx[7] = ((lp1->x * lp2->z) - (lp1->z * lp2->x)) * -invdet;
    invmx[2] = ((lp2->x * lp3->y) - (lp2->y * lp3->x)) * invdet;
    invmx[5] = ((lp1->x * lp3->y) - (lp1->y * lp3->x)) * -invdet;
    invmx[8] = ((lp1->x * lp2->y) - (lp1->y * lp2->x)) * invdet;
    for(i=0;i<3;i++){
    	SETLONG(&at[pointer], tr_ptr->ls_nos[i]+1);
    	pointer++;
    }
    for(i=0;i<9;i++){
    	SETFLOAT(&at[pointer], invmx[i]);
    	pointer++;
    }
    SETFLOAT(&at[pointer], lp1->x); pointer++;
    SETFLOAT(&at[pointer], lp2->x); pointer++;
    SETFLOAT(&at[pointer], lp3->x); pointer++;
    SETFLOAT(&at[pointer], lp1->y); pointer++;
    SETFLOAT(&at[pointer], lp2->y); pointer++;
    SETFLOAT(&at[pointer], lp3->y); pointer++;
    SETFLOAT(&at[pointer], lp1->z); pointer++;
    SETFLOAT(&at[pointer], lp2->z); pointer++;
    SETFLOAT(&at[pointer], lp3->z); pointer++;
 
    tr_ptr = tr_ptr->next;
  }
  outlet_anything(x->x_outlet0, gensym("loudspeaker-matrices"), list_length, at);
  freebytes(at, list_length*sizeof(Atom));
}



void choose_ls_tuplets(t_def_ls *x)
     /* selects the loudspeaker pairs, calculates the inversion
        matrices and stores the data to a global array*/
{
  float atorad = (2 * 3.1415927 / 360) ;
  int i,j,k;
  float w1,w2;
  float p1,p2;
  int sorted_lss[MAX_LS_AMOUNT];
  int exist[MAX_LS_AMOUNT];   
  int amount=0;
  float inv_mat[MAX_LS_AMOUNT][4];  // In 2-D ls amount == max amount of LS pairs
  float *ptr;   
  float *ls_table;
  t_ls *lss = x->x_ls;
  long ls_amount=x->x_ls_amount;
  long list_length;
  Atom *at;
  long pointer;
  
  for(i=0;i<MAX_LS_AMOUNT;i++){
    exist[i]=0;
  }

  /* sort loudspeakers according their aximuth angle */
  sort_2D_lss(x->x_ls,sorted_lss,ls_amount);

  /* adjacent loudspeakers are the loudspeaker pairs to be used.*/
  for(i=0;i<(ls_amount-1);i++){
    if((lss[sorted_lss[i+1]].azi - 
        lss[sorted_lss[i]].azi) <= (180 - 10)){
      if (calc_2D_inv_tmatrix( lss[sorted_lss[i]].azi, 
                               lss[sorted_lss[i+1]].azi, 
                               inv_mat[i]) != 0){
        exist[i]=1;
        amount++;
      }
    }
  }

  if(((6.283 - lss[sorted_lss[ls_amount-1]].azi) 
      +lss[sorted_lss[0]].azi) <= (180 -  10)) {
    if(calc_2D_inv_tmatrix(lss[sorted_lss[ls_amount-1]].azi, 
                           lss[sorted_lss[0]].azi, 
                           inv_mat[ls_amount-1]) != 0) { 
        exist[ls_amount-1]=1;
        amount++;
    } 
  }
  
  
  // Output
  list_length= amount * 6 + 2;
  at= (Atom *) getbytes(list_length*sizeof(Atom));
  
  SETLONG(&at[0], x->x_dimension);
  SETLONG(&at[1], x->x_ls_amount);
  pointer=2;
  
  for (i=0;i<ls_amount - 1;i++){
    if(exist[i] == 1) {
    	SETLONG(&at[pointer], sorted_lss[i]+1);
    	pointer++;
    	SETLONG(&at[pointer], sorted_lss[i+1]+1);
    	pointer++;
       	for(j=0;j<4;j++) {
       		SETFLOAT(&at[pointer], inv_mat[i][j]);
    		pointer++;
       	}
    }
  }
  if(exist[ls_amount-1] == 1) {
    SETLONG(&at[pointer], sorted_lss[ls_amount-1]+1);
    pointer++;
    SETLONG(&at[pointer], sorted_lss[0]+1);
    pointer++;
    for(j=0;j<4;j++) {
    	SETFLOAT(&at[pointer], inv_mat[ls_amount-1][j]);
    	pointer++;
    }
  }
  outlet_anything(x->x_outlet0, gensym("loudspeaker-matrices"), list_length, at);
  freebytes(at, list_length*sizeof(Atom));
}

void sort_2D_lss(t_ls lss[MAX_LS_AMOUNT], int sorted_lss[MAX_LS_AMOUNT], 
                 int ls_amount)
// sort loudspeakers according to azimuth angle
{
  int i,j,index;
  float tmp, tmp_azi;
  float rad2ang = 360.0 / ( 2 * 3.141592 );

  float x,y;
  /* Transforming angles between -180 and 180 */
  for (i=0;i<ls_amount;i++) {
    ls_angles_to_cart(&lss[i]);
    lss[i].azi = acos( lss[i].x) * rad2ang;
    if (fabs(lss[i].y) <= 0.001)
        tmp = 1.0;
    else
        tmp = lss[i].y / fabs(lss[i].y);
    lss[i].azi *= tmp;
  }
  for (i=0;i<ls_amount;i++){
    tmp = 2000;
    for (j=0 ; j<ls_amount;j++){
      if (lss[j].azi <= tmp){
        tmp=lss[j].azi;
        index = j;
      }
    }
    sorted_lss[i]=index;
    tmp_azi = (lss[index].azi);
    lss[index].azi = (tmp_azi + (float) 4000.0);
  }
  for (i=0;i<ls_amount;i++) {
    tmp_azi = (lss[i].azi);
    lss[i].azi = (tmp_azi - (float) 4000.0);
  }
}
  

int calc_2D_inv_tmatrix(float azi1,float azi2, float inv_mat[4])
// calculate inverse 2x2 matrix
{
  float x1,x2,x3,x4; /* x1 x3 */
  float y1,y2,y3,y4; /* x2 x4 */
  float det;
  float rad2ang = 360.0 / ( 2 * 3.141592 );
  
  x1 = cos(azi1 / rad2ang);
  x2 = sin(azi1 / rad2ang);
  x3 = cos(azi2 / rad2ang);
  x4 = sin(azi2 / rad2ang);
  det = (x1 * x4) - ( x3 * x2 );
   if(fabsf(det) <= 0.001) {

    inv_mat[0] = 0.0;
    inv_mat[1] = 0.0;
    inv_mat[2] = 0.0;
    inv_mat[3] = 0.0;
    return 0;
  } else {
    inv_mat[0] =   (x4 / det);
    inv_mat[1] =  (-x3 / det);
    inv_mat[2] =   (-x2 / det);
    inv_mat[3] =    (x1 / det);
    return 1;
  }
}


