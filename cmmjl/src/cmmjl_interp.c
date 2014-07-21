/*
Copyright (c) 2008.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by John MacCallum, The Center for New Music and
Audio Technologies, University of California, Berkeley.

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

#include "cmmjl_interp.h"

double cmmjl_interp_simple(double *x_vec, 
			  double *y_vec, 
			  double x, 
			  long len, 
			  double minx, 
			  double maxx, 
			  double alpha, 
			  double w0)
{
	double avec[len];
	int i;
	for(i = 0; i < len; i++){
		avec[i] = alpha;
	}
	return cmmjl_interp_simple_avec(x_vec, y_vec, x, len, minx, maxx, avec, w0);
}

double cmmjl_interp_simple_avec(double *x_vec, 
			       double *y_vec, 
			       double x, 
			       long len, 
			       double minx, 
			       double maxx, 
			       double *alpha, 
			       double w0)
{
	int i;
	double num = 0.;
	double den = 0.;
	double d = 0;
	double x_w[len + 2], y_w[len + 2], w[len + 2];
	double sum = 0;
	x_w[0] = minx;
	y_w[0] = 0.;

	for(i = 0; i < len; i++){
		x_w[i + 1] = x_vec[i];
		y_w[i + 1] = y_vec[i];
		w[i + 1] = 1;
		sum += y_vec[i];
	}
	sum /= w0;

	//x_w[len + 1] = x_vec[len - 1] * 1.1 < 22050. ? x_vec[len - 1] * 1.1 : 22050;
	x_w[len + 1] = maxx;
	y_w[len + 1] = 0.;

	w[0] = sum / 2;
	w[len + 1] = sum / 2;

	for(i = 0; i < len + 2; i++){
		d = pow(abs(x - x_w[i]), alpha[i]);
		if(d < 0.0001){
			num = y_w[i];
			den = 1.;
			break;
		}else{
			num += (w[i] * y_w[i]) / d;
			den += w[i] / d;
		}
	}
	return num / den;
}
