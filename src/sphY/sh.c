#ifndef SHLIB
#include "legendre_a.h"
#include "sh_normalization.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct _sh_
{
    assoc_legendre *algdr;
    sh_normalization *norm;
    int n;
} sh;
#else
#include "sh.h"
#endif
void deleteSH(sh* shp)
{
    if (shp!=0) {
        deleteAssociatedLegendre(shp->algdr);
        deleteShNormalization(shp->norm);
        free(shp);
    }
}

sh *newSH(int degree)
{
    sh *new_sh;
    if ((new_sh=(sh*)malloc(sizeof(sh)))==0) {
        return 0;
    }
    if ((new_sh->algdr=newAssociatedLegendre(degree))==0) {
        deleteSH(new_sh);
        return 0;
    }
    if ((new_sh->norm=newShNormalization(degree))==0) {
        deleteSH(new_sh);
        return 0;
    }
    new_sh->n=degree;
    
    return new_sh;
    
}

double cSHEvaluate_im (sh *shp, int n, int m, double phi, double theta)
{
    double y;
    int absm=(m<0)?-m:m;
    // consistency of integer arguments
    if ((n<0)||(absm>n)) {
        return 0;
    }
    // existence of structure
    if ((shp==0)) {
        return 0;
    }
    // maximum order
    if (n>shp->n) {
        return 0;
    }
    
    y = shp->norm->cnorm[n][absm] * evaluateAssociatedLegendre(shp->algdr,cos(theta),n,absm);
    return sin(m* phi) * y;
}
double cSHEvaluate_re (sh *shp, int n, int m, double phi, double theta)
{
    double y;
    int absm=(m<0)?-m:m;
    // consistency of integer arguments
    if ((n<0)||(absm>n)) {
        return 0;
    }
    // existence of structure
    if ((shp==0)) {
        return 0;
    }
    // maximum order
    if (n>shp->n) {
        return 0;
    }
    
    y = shp->norm->cnorm[n][absm] * evaluateAssociatedLegendre(shp->algdr,cos(theta),n,absm);
    return cos(m* phi) * y;
}

double sHEvaluate (sh *shp, int n, int m, double phi, double theta)
{
    double y;
    int absm=(m<0)?-m:m;
    // consistency of integer arguments
    if ((n<0)||(absm>n)) {
        return 0;
    }
    // existence of structure
    if ((shp==0)) {
        return 0;
    }
    // maximum order
    if (n>shp->n) {
        return 0;
    }
    
    y = shp->norm->norm[n][absm] * evaluateAssociatedLegendre(shp->algdr,cos(theta),n,absm);
    if (m<0)
        return sin(m* phi) * y;
    else
        return cos(m* phi) * y;
}

