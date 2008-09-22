/*
 * Recursive Computation of (arbitrary degree) normalized spherical harmonics 
 * according to Gumerov and Duraiswami, 
 * "The Fast Multipole Methods for the Helmholtz Equation in Three Dimensions", 
 * Elsevier, 2005.
 *
 * Implementation by
 *
 * Franz Zotter
 * Institute of Electronic Music and Acoustics
 * University of Music and Dramatic Arts
 * Graz, Austria
 *
 * zotter@iem.at
 * 2006.
 */

#ifndef _legendre_a_h__
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _assoc_legendre_
{
    double ***cospoly;
    double **sinmul;
    int n;
} assoc_legendre;

void deleteAssociatedLegendre (assoc_legendre *a_lgdr);

assoc_legendre *newAssociatedLegendre (int degree);

double evaluateAssociatedLegendre (assoc_legendre *a_lgdr, double z, int n, int m);

#define _legendre_a_h__
#endif
