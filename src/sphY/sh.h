#ifndef _sh_h__

/**
 * Spherical harmonics library by
 *
 * Franz Zotter
 * Institute of Electronic Music and Acoustics
 * University of Music and Dramatic Arts
 * Graz, Austria
 *
 * zotter@iem.at
 * 2006.
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "legendre_a.h"
#include "sh_normalization.h"

typedef struct _sh_
{
    assoc_legendre *algdr;
    sh_normalization *norm;
    int n;
} sh;

void deleteSH(sh* shp);

sh *newSH(int degree);

double cSHEvaluate_im (sh *shp, int n, int m, double phi, double theta);

double cSHEvaluate_re (sh *shp, int n, int m, double phi, double theta);

double sHEvaluate (sh *shp, int n, int m, double phi, double theta);

#define _sh_h__
#endif

