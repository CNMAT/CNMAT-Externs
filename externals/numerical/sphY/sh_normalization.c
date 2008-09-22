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


#ifndef SHLIB
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct _sh_normalization_
{
    double **cnorm;
    double **norm;
    int n;
} sh_normalization;
#else
#include "sh_normalization.h"
#endif

static void initNormalization(sh_normalization *shn) {
    int n,m;
    double factor;
    double sqrt2=pow(2.0f,0.5f);
    for (n=0; n<=shn->n; n++)
        shn->norm[n][0]=shn->cnorm[n][0]=pow( ((double)2*n+1) / (4*M_PI),0.5);
    for (n=0; n<=shn->n; n++)
        for (m=1; m<=n; m++) {
            factor=-pow(((double)(n+m)*(n-m+1)),0.5);
            shn->cnorm[n][m]=shn->cnorm[n][m-1]/factor;
            shn->cnorm[n][-m]=shn->cnorm[n][-m+1]*factor;
            shn->norm[n][m] =sqrt2*shn->cnorm[n][m];
            shn->norm[n][-m]=sqrt2*shn->cnorm[n][-m];
        }
}

static void deleteShNormCoeffTree (double ** tree) {
    if (tree!=0) {
        if (tree[0]!=0)
            free(tree[0]);
        free(tree);
    }
}
void deleteShNormalization (sh_normalization *shn) {
    if (shn!=0) {
        if (shn->norm!=0) {
            deleteShNormCoeffTree(shn->norm);
        }
        if (shn->cnorm!=0) {
            deleteShNormCoeffTree(shn->cnorm);
        }
        free(shn);
    }
}
static double **newShNormCoeffTree (int degree) {
    double **tree;
    int n,num_coeffs;
    
    num_coeffs = (degree+1);
    if ((tree=(double**)calloc(num_coeffs,sizeof(double*)))==0) {
        
        return 0;
        
    }
    num_coeffs = pow((degree+1),2);
    if ((tree[0]=(double*)calloc(num_coeffs,sizeof(double)))==0) {
        deleteShNormCoeffTree(tree);
        
        return 0;
        
    }
    for (n=1; n<=degree; n++) {
        tree[n]=tree[n-1]+2*n;
    }
    return tree;
}


sh_normalization *newShNormalization (int degree) {
    sh_normalization* shn;
    
    
    
    if ((shn=(sh_normalization*)calloc(1,sizeof(sh_normalization)))==0)
    {  return 0;}
    
    if ((shn->cnorm=newShNormCoeffTree (degree))==0) {
        deleteShNormalization(shn);
        
        return 0;
    }
    
    if ((shn->norm=newShNormCoeffTree (degree))==0) {
        deleteShNormalization(shn);
        return 0;
    }
    shn->n=degree;
    initNormalization(shn);
    
    return shn;
    
}

