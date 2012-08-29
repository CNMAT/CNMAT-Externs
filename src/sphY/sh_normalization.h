/*
 * Recursive computation of (arbitrary degree) normalization constants
 * for spherical harmonics, according to Gumerov and Duraiswami, 
 * "The Fast Multipole Methods for the Helmholtz Equation in Three Dimensions", 
 * Elsevier, 2005.
 * and "Fourier Acoustics", E.G. Williams, Academic Press, 1999.
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

#ifndef _sh_normalization_h__
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _sh_normalization_
{
    double **cnorm;
    double **norm;
    int n;
} sh_normalization;

sh_normalization *newShNormalization (int degree);
void deleteShNormalization (sh_normalization *sh_norm);
#define _sh_normalization_h__
#endif
