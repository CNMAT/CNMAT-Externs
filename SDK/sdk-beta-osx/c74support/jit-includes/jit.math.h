
/* 
	jit.math.h
	Copyright 2001-2002 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com

	- platform independent mathlib calls
*/

#ifndef _JIT_MATH_H_
#define _JIT_MATH_H_

double jit_math_cos		(double x);
double jit_math_sin		(double x);
double jit_math_tan		(double x);
double jit_math_acos	(double x);
double jit_math_asin	(double x);
double jit_math_atan	(double x);
double jit_math_atan2	(double y, double x);
double jit_math_cosh	(double x);
double jit_math_sinh	(double x);
double jit_math_tanh	(double x);
double jit_math_acosh	(double x);
double jit_math_asinh	(double x);
double jit_math_atanh	(double x);
double jit_math_exp		(double x);
double jit_math_expm1	(double x);
double jit_math_exp2	(double x);
double jit_math_log		(double x);
double jit_math_log2	(double x);
double jit_math_log10	(double x);
double jit_math_hypot	(double x, double y);
double jit_math_pow		(double x, double y);
double jit_math_sqrt	(double x);
double jit_math_ceil	(double x);
double jit_math_floor	(double x);
double jit_math_round	(double x);
double jit_math_trunc	(double x);
double jit_math_fmod	(double x, double y);

#endif /* _JIT_MATH_H_ */
