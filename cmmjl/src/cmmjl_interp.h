/** @file cmmjl_interp.h
	Interpolation functions
	@author John MacCallum
*/
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

#include <math.h>
#include "cmmjl_profile.h"

#ifndef __CMMJL_INTERP_H__
#define __CMMJL_INTERP_H__

/**	Simple interpolation function.

	@param	x_vec	The x vector to interpolate over.
	@param 	y_vec	The y vector to interpolate over.
	@param	x	The x-value to evaluate the interpolation function at.
	@param 	len	The length of the vectors.
	@param 	minx	The interpolation function is bounded on both sides.  
			This value should be some value that is smaller than the
			minimum of x_vec.
	@param 	maxx	The interpolation function is bounded on both sides.  
			This value should be some value that is greater than the
			maximum of x_vec.
	@param 	alpha	This value sets the width of the peaks.  Small values of 
			alpha (<= 1) will result in narrow peaks, while larger 
			values will have wider peaks.
	@param  w0	The average of the interpolation function.  The interpolator 
			will tend towards this value when it is between peaks.
	
	@returns	The interpolated y-value

	@see		http://arXiv:astro-ph/0208533

	@note		The interpolator function is
			\f[
				S\left(x;\left\{\left(x_i,y_i\right)\right\}\right)=
					\frac{\sum_{i=1}^N w_iy_i/|x-x_i|^\alpha}
						{\sum_{i=1}^N w_i/|x-x_i|^\alpha}
			\f]
			where \f$w_i\f$ is a vector of weights
			\f[
  				w_i = \left\{
  					\begin{array}{ll}
    						\frac{\sum_0^{N-1}a_i}{w0} & 
							\quad \mbox{for $i = 0, N+1$} \\
    						1 & \quad \mbox{for $i = 1\dots N$}
  					\end{array}
				\right.
			\f]
			and \f$w0\f$ is the average of that the interpolation function 
			will tend towards when it is between peaks.  The function is
			 bounded on both sides at xmin and xmax.
*/
double mcmc_interp_simple(double *x_vec, 
			  double *y_vec, 
			  double x, 
			  long len, 
			  double minx, 
			  double maxx, 
			  double alpha, 
			  double w0);

/**	The same interpolation function as mcmc_interp() but with alpha specified
	as a vector to have local control over the width of the peaks in different 
	regions.

	@param	x_vec	The x vector to interpolate over.
	@param 	y_vec	The y vector to interpolate over.
	@param	x	The x-value to evaluate the interpolation function at.
	@param 	len	The length of the vectors.
	@param 	minx	The interpolation function is bounded on both sides.  
			This value should be some value that is smaller than the
			minimum of x_vec.
	@param 	maxx	The interpolation function is bounded on both sides.  
			This value should be some value that is greater than the
			maximum of x_vec.
	@param 	alpha	A vector of alpha values which set the width of the peaks.  
			Small values of alpha (<= 1) will result in narrow peaks, 
			while larger values will have wider peaks.
	@param  w0	The average of the interpolation function.  The interpolator 
			will tend towards this value when it is between peaks.
	
	@returns	The interpolated y-value

	@see		http://arXiv:astro-ph/0208533
	@see		mcmc_interp() for an explanation of the function
*/
double mcmc_interp_simple_avec(double *x_vec, 
			       double *y_vec, 
			       double x, 
			       long len, 
			       double minx, 
			       double maxx, 
			       double *alpha, 
			       double w0);

#endif // __CMMJL_INTERP_H__
