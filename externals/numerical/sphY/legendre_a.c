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

#ifndef SHLIB
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct _assoc_legendre_
{
    double ***cospoly;
    double **sinmul;
    int n;
} assoc_legendre;
#else
#include "legendre_a.h"
#endif

static void deleteALgdrPolyTree (double ***tree)
{
    if (tree!=0) {
        if (tree[0]!=0) {
            if (tree[0][0]!=0) {
                free(tree[0][0]);
            }
            free(tree[0]);
        }
        free(tree);
    }
}

static void deleteALgdrCoeffTree (double **tree)
{
    if (tree!=0) {
        if (tree[0]!=0) {
            free(tree[0]);
        }
        free(tree);
    }
}

static double ***newALgdrPolyTree (int degree)
{  // THIS SPHERICAL HARMONICS TREE DATA TYPE HAS DEPTH 3
    // FIRST INDEX n=0...degree DETERMINES THE ALgdr DEGREE (in ambisonics mistaken as "order")
    // SECOND INDEX m=-n...n DETERMINES THE ALgdr ORDER
    // THIRD INDEX k ACCESSES AN ARRAY of doubles k=0...(n+1)
    
    double ***tree;
    int n,m,num_coeffs;
    
    num_coeffs = (degree+1);
    // CREATING DEGREE (n) (1)
    if ((tree = (double ***)calloc(num_coeffs,sizeof(double**)))==0) {
        return 0;
    }
    // CREATING ORDER (m) (2)
    num_coeffs = pow(degree+1,2);
    if ((tree[0] = (double **)calloc(num_coeffs,sizeof(double*)))==0) {
        deleteALgdrPolyTree(tree);
        return 0;
    }
    // INDEXING DEGREES
    for (n=1; n<=degree; n++) {
        tree[n]=tree[n-1]+2*n;
    }
    
    // CREATING COEFFICIENTS (pow(x,k)) (3)
    for (n=0,num_coeffs=0;n<=degree;n++) {
        num_coeffs += (2*n+1)*(n+1)-2*((int)((n+1)/2));
    }
    if ((tree[0][0] = (double *)calloc(num_coeffs,sizeof(double)))==0) {
        deleteALgdrPolyTree(tree);
        return 0;
    }
    
    // INDEXING ORDER VECTORS
    for (n=1; n<=degree; n++) {
        tree[n][-n]=tree[n-1][n-1]+n-((n-1)&1);
        for (m=-n+1; m<=n; m++) {
            tree[n][m]=tree[n][m-1]+n+1-((m-1)&1);
        }
    }
    
    return tree;
}

static double **newALgdrCoeffTree (int degree)
{  // THIS SPHERICAL HARMONICS TREE DATA TYPE HAS DEPTH 2
    // FIRST INDEX n=0...degree DETERMINES THE ALgdr DEGREE (in ambisonics mistaken as "order")
    // SECOND INDEX m=-n...n DETERMINES THE ALgdr ORDER
    // AND ACCESSES AN ARRAY OF doubles
    
    double **tree;
    int n,num_coeffs;
    
    // CREATING DEGREE (n)
    num_coeffs = degree+1;
    if ((tree =
         (double **)calloc(num_coeffs,sizeof(double*)))==0) {
        return 0;
    }
    // CREATING ORDER (m)
    num_coeffs = pow((degree+1),2);
    if ((tree[0] =
         (double *)calloc(num_coeffs,sizeof(double)))==0) {
        deleteALgdrCoeffTree(tree);
        return 0;
    }
    // INDEXING DEGREE
    for (n=1; n<=degree; n++) {
        tree[n] = tree[n-1]+2*n;
    }
    return tree;
}

void deleteAssociatedLegendre (assoc_legendre *a_lgdr)
{
    if (a_lgdr!=0) {
        if (a_lgdr->cospoly!=0) {
            deleteALgdrPolyTree (a_lgdr->cospoly);
            a_lgdr->cospoly=0;
        }
        if (a_lgdr->sinmul!=0){
            deleteALgdrCoeffTree (a_lgdr->sinmul);
            a_lgdr->sinmul=0;
        }
        free (a_lgdr);
    }
}

static void addScalePolyCoeffs(double *source, int source_n, double *dest, int dest_n, double f)
{
    int count = (source_n<dest_n)?source_n:dest_n;
    source += source_n;
    dest += dest_n;
    while (count--)
        *--dest += f* *--source;
}
static void addPolyCoeffs(double *source, int source_n, double *dest, int dest_n)
{
    int count = (source_n<dest_n)?source_n:dest_n;
    source += source_n;
    dest += dest_n;
    while (count--)
        *--dest += *--source;
}
static void copyPolyCoeffs(double *source, int source_n, double *dest, int dest_n)
{
    int count = (source_n<dest_n)?source_n:dest_n;
    source += source_n;
    dest += dest_n;
    while (count--)
        *--dest = *--source;
}

static void scalePolyCoeffs(double *x, int x_n, double f)
{
    while(x_n--)
        *x++*=f;
}
static void convPolyCoeffs(double *x1, int x1_n, double *x2, int x2_n, double *y, int y_n)
{
    int k;
    int n;
    x1 += x1_n-1;
    x2 += x2_n-1;
    y += y_n-1;
    
    if ((x1_n>0)&&(x2_n>0)) {
        for (n=0; n<y_n; n++)
            y[-n]=0;
        for (n=0; n<x1_n; n++) {
            for (k=n; (k-n<x2_n)&&(k<y_n); k++) {
                y[-k] += x1[-n]*x2[-k+n];
            }
        }
    }
}

static double evaluatePolynomial (double *coeffs, int size, double x)
{
    double y = 0.0;
    double x_power = 1.0;
    int k;
    
    for (k=size-1; k>=0; k--) {
        y += coeffs[k] * x_power;
        x_power *= x;
    }
    return y;
}

static void legendrePolynomialStartValues (assoc_legendre *a_lgdr)
{
    if (a_lgdr->n >= 0) {
        a_lgdr->cospoly[0][0][0]=1.0;
        a_lgdr->sinmul[0][0]=0.0;
    }
}

static void legendrePolynomialIteration1 (assoc_legendre *a_lgdr)
{
    int n;
    int polyconv_size = 3;
    double polyconv[] = {-1.0, 0.0, 1.0};
    
    for (n=1; n<=a_lgdr->n; n++) {
        switch (n&1) {
          case 1: //odd
              addScalePolyCoeffs(a_lgdr->cospoly[n-1][n-1],n, a_lgdr->cospoly[n][n],n,-(double)(2*n-1));
              a_lgdr->sinmul[n][n]=1.0;
              break;
          case 0: //even
              convPolyCoeffs(a_lgdr->cospoly[n-1][n-1],n-1,polyconv,polyconv_size,a_lgdr->cospoly[n][n],n+1);
              scalePolyCoeffs(a_lgdr->cospoly[n][n],n+1,-(double)(2*n-1));
              a_lgdr->sinmul[n][n]=0.0;
              break;
        }
    }
}
static void legendrePolynomialIteration2 (assoc_legendre *a_lgdr)
{
    int m,n,mmod2;
    int polyconv_size = 2;
    double polyconv[] = {1.0, 0.0};
    
    for (n=1; n<=a_lgdr->n; n++) {
        for (m=0; m<n; m++) {
            mmod2=(m&1);
            a_lgdr->sinmul[n][m]=mmod2;
            convPolyCoeffs(a_lgdr->cospoly[n-1][m],n-mmod2,polyconv,polyconv_size,a_lgdr->cospoly[n][m],n+1-mmod2);
            scalePolyCoeffs(a_lgdr->cospoly[n][m],n+1-mmod2,((double)(2*n-1)));
            if (m<=n-2) {
                addScalePolyCoeffs(a_lgdr->cospoly[n-2][m],n-1-mmod2,
                                   a_lgdr->cospoly[n][m],n+1-mmod2,-((double)(n+m-1)));
            }
            scalePolyCoeffs(a_lgdr->cospoly[n][m],n+1-mmod2,(1.0/(n-m)));
        }
    }
}

static void legendrePolynomialIteration3 (assoc_legendre *a_lgdr) {
    int n,m;
    double gnm;
    for (n=1;n<=a_lgdr->n;n++) {
        gnm=1.0;
        for (m=1; m<=n; m++) {
            gnm/=(double)((n+m)*(n-m+1));
            addScalePolyCoeffs(a_lgdr->cospoly[n][m],n+1-(m&1),a_lgdr->cospoly[n][-m],n+1-(m&1),gnm);
            a_lgdr->sinmul[n][-m]=a_lgdr->sinmul[n][m];
        }
    }
}

assoc_legendre *newAssociatedLegendre (int degree)
{
    
    assoc_legendre *a_lgdr;
    if ((a_lgdr=(assoc_legendre *)calloc(1,sizeof(assoc_legendre)))==0)
        return 0;
    
    a_lgdr->n = degree;
    
    if ((a_lgdr->cospoly = newALgdrPolyTree (degree))==0) {
        deleteAssociatedLegendre(a_lgdr);
        return 0;
    }
    else {
        if ((a_lgdr->sinmul = newALgdrCoeffTree (degree))==0) {
            deleteAssociatedLegendre(a_lgdr);
            return 0;
        }
    }
    legendrePolynomialStartValues (a_lgdr);
    legendrePolynomialIteration1 (a_lgdr);
    legendrePolynomialIteration2 (a_lgdr);
    legendrePolynomialIteration3 (a_lgdr);
    return a_lgdr;
}

double evaluateAssociatedLegendre (assoc_legendre *a_lgdr, double z, int n, int m)
{
    double z_sin=pow(1.0-pow(z,2),0.5);
    double y=1;
    
    if ((n>a_lgdr->n)||(n<0)||(m<-n)||(m>n)) {
        return 0;
    }
    
    if (a_lgdr->sinmul[n][m]==1) {
        y *= z_sin;
    }
    y *= evaluatePolynomial (a_lgdr->cospoly[n][m], n+1-(m&1), z);
    return y;
}

