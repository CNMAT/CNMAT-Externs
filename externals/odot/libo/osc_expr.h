/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2011, The Regents of
  the University of California (Regents). 
  Permission to use, copy, modify, distribute, and distribute modified versions
  of this software and its documentation without fee and without a signed
  licensing agreement, is hereby granted, provided that the above copyright
  notice, this paragraph and the following two paragraphs appear in all copies,
  modifications, and distributions.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
  HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

/** 	\file osc_expr.h
	\author John MacCallum

*/
#ifndef __OSC_EXPR_H__
#define __OSC_EXPR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "osc_atom_u.h"
#include "osc_atom_array_u.h"

#define OSC_EXPR_ARG_TYPE_ATOM 1
#define OSC_EXPR_ARG_TYPE_EXPR 2
#define OSC_EXPR_ARG_TYPE_OSCADDRESS 3

typedef struct _osc_expr t_osc_expr;
typedef struct _osc_expr_arg t_osc_expr_arg;

typedef int (*t_osc_funcptr)(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

/** \struct t_osc_expr_rec
    A record that associates a function name (string) with a function pointer.
*/
typedef struct _osc_expr_rec{
	const char *name; /**< Name of the function as a C string. */
	int (*func)(t_osc_expr*, int, t_osc_atom_ar_u**, t_osc_atom_ar_u**); /**< Function pointer */
	int numargs;
	void *extra; /**< Extra field that can contain anything. */
	const char *desc;
} t_osc_expr_rec;

typedef struct _osc_expr_const_rec{
	const char *name;
	double val;
	const char *desc;
} t_osc_expr_const_rec;

int osc_expr_funcall(t_osc_expr *function, long *len, char **oscbndl, t_osc_atom_ar_u **out);
int osc_expr_getArg(t_osc_expr_arg *arg, long *len, char **oscbndl, t_osc_atom_ar_u **out);
int osc_expr_call(t_osc_expr *f, long *len, char **oscbndl, t_osc_atom_ar_u **out);
t_osc_expr_rec *osc_expr_lookupFunction(char *name);
t_osc_expr_const_rec *osc_expr_lookupConstant(char *name);

int osc_expr_1arg_dbl(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_2arg_dbl_dbl(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_2arg(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

void osc_expr_add(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_subtract(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_multiply(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_divide(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_lt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_lte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_gt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_gte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_eq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_neq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_and(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_or(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void osc_expr_mod(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);

int osc_expr_assign(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_add1(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_subtract1(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_get_index(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_product(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_sum(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_cumsum(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_length(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_mean(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_median(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_concat(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_reverse(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_make_list(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_range(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_multiplex(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_not(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_dot(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_l2norm(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_min(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_max(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_extrema(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_clip(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_scale(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_mtof(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_ftom(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_rand(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_sgn(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_if(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_defined(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_nothing(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int osc_expr_eval(t_osc_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

t_osc_expr *osc_expr_alloc(void);
t_osc_expr_arg *osc_expr_arg_alloc(void);
void osc_expr_free(t_osc_expr *e);
t_osc_expr *osc_expr_copy(t_osc_expr *e);
t_osc_expr_arg *osc_expr_arg_copy(t_osc_expr_arg *e);
void osc_expr_arg_free(t_osc_expr_arg *a);

void osc_expr_appendExpr(t_osc_expr *e, t_osc_expr *expr_to_append);
void osc_expr_setRec(t_osc_expr *e, t_osc_expr_rec *rec);
t_osc_expr_rec *osc_expr_getRec(t_osc_expr *e);
void osc_expr_setNext(t_osc_expr *e, t_osc_expr *next);
t_osc_expr *osc_expr_next(t_osc_expr *e);
void osc_expr_setArg(t_osc_expr *e, t_osc_expr_arg *a);
void osc_expr_prependArg(t_osc_expr *e, t_osc_expr_arg *a);
void osc_expr_appendArg(t_osc_expr *e, t_osc_expr_arg *a);
t_osc_expr_arg *osc_expr_getArgs(t_osc_expr *e);
long osc_expr_getArgCount(t_osc_expr *e);

void osc_expr_arg_setOSCAtom(t_osc_expr_arg *a, t_osc_atom_u *atom);
void osc_expr_arg_setExpr(t_osc_expr_arg *a, t_osc_expr *e);
void osc_expr_arg_setOSCAddress(t_osc_expr_arg *a, char *osc_address);
int osc_expr_arg_getType(t_osc_expr_arg *a);
t_osc_atom_u *osc_expr_arg_getOSCAtom(t_osc_expr_arg *a);
t_osc_expr *osc_expr_arg_getExpr(t_osc_expr_arg *a);
char *osc_expr_arg_getOSCAddress(t_osc_expr_arg *a);
int osc_expr_arg_append(t_osc_expr_arg *a, t_osc_expr_arg *arg_to_append);
void osc_expr_arg_setNext(t_osc_expr_arg *a, t_osc_expr_arg *next);
t_osc_expr_arg *osc_expr_arg_next(t_osc_expr_arg *a);
void osc_expr_setAssignResultToAddress(t_osc_expr *e, int val);
int osc_expr_getAssignResultToAddress(t_osc_expr *e);

void osc_expr_formatFunctionGraph(t_osc_expr *fg, long *buflen, char **fmt);

static t_osc_expr_const_rec osc_expr_constsym[] = {
	{"E", M_E, "e"},
	{"LOG2E", M_LOG2E, "log 2e"},
	{"LOG10E", M_LOG10E, "log 10e"},
	{"LN2", M_LN2, "log e2"},
	{"LN10", M_LN10, "log e10"},
	{"PI", M_PI, "pi"},
	{"PI_OVER_2", M_PI_2, "pi/2"},
	{"PI_OVER_4", M_PI_4, "pi/4"},
	{"ONE_OVER_PI", M_1_PI, "1/pi"},
	{"TWO_OVER_PI", M_2_PI, "2/pi"},
	{"TWO_OVER_SQRTPI", M_2_SQRTPI, "2/sqrt(pi)"},
	{"SQRT2", M_SQRT2, "sqrt(2)"},
	{"ONE_OVER_SQRT_2", M_SQRT1_2, "1/sqrt(2)"}
};

static t_osc_expr_rec osc_expr_funcsym[] = {
	// infix
	{"+", osc_expr_2arg, 2, (void *)osc_expr_add, "Add"},
	{"-", osc_expr_2arg, 2, (void *)osc_expr_subtract, "Subtract"},
	{"*", osc_expr_2arg, 2, (void *)osc_expr_multiply, "Multiply"},
	{"/", osc_expr_2arg, 2, (void *)osc_expr_divide, "Divide"},
	{"<", osc_expr_2arg, 2, (void *)osc_expr_lt, "Less than"},
	{"<=", osc_expr_2arg, 2, (void *)osc_expr_lte, "Less than or equal to"},
	{">", osc_expr_2arg, 2, (void *)osc_expr_gt, "Greater than"},
	{">=", osc_expr_2arg, 2, (void *)osc_expr_gte, "Greater than or equal to"},
	{"==", osc_expr_2arg, 2, (void *)osc_expr_eq, "Equal"},
	{"!=", osc_expr_2arg, 2, (void *)osc_expr_neq, "Not equal"},
	{"&&", osc_expr_2arg, 2, (void *)osc_expr_and, "Logical and"},
	{"||", osc_expr_2arg, 2, (void *)osc_expr_or, "Logical or"},
	{"%", osc_expr_2arg, 2, (void *)osc_expr_mod, "Modulo"},
	{"=", osc_expr_assign, 2, NULL, "Assignment"},
	{"++", osc_expr_add1, 1, NULL, "Increment"},
	{"--", osc_expr_subtract1, 1, NULL, "Decrement"},
	{"+=", osc_expr_2arg, 2, (void *)osc_expr_add, "Add and assign"},
	{"-=", osc_expr_2arg, 2, (void *)osc_expr_subtract, "Subtract and assign"},
	{"*=", osc_expr_2arg, 2, (void *)osc_expr_multiply, "Multiply and assign"},
	{"/=", osc_expr_2arg, 2, (void *)osc_expr_divide, "Divide and assign"},
	{"%=", osc_expr_2arg, 2, (void *)osc_expr_mod, "Modulo and assign"},
	{"??", NULL, 2, NULL, "Null coalescing operator, returns <arg1> if it exists, otherwise returns <arg2>."},
	// math.h
	{"abs", osc_expr_1arg_dbl, 1, (void *)fabs, "Absolute value"},
	{"acos", osc_expr_1arg_dbl, 1, (void *)acos, "Arc cosine"},
	{"asin", osc_expr_1arg_dbl, 1, (void *)asin, "Arc sine"},
	{"atan", osc_expr_1arg_dbl, 1, (void *)atan, "Arc tangent"},
	{"atan2", osc_expr_2arg_dbl_dbl, 2, (void *)atan2, "Arc tangent of y/x (arg1/arg2)"},
	{"ceil", osc_expr_1arg_dbl, 1, (void *)ceil, "Ceiling--round up to the nearest integer"},
	{"cos", osc_expr_1arg_dbl, 1, (void *)cos, "Cosine"},
	{"cosh", osc_expr_1arg_dbl, 1, (void *)cosh, "Hyperbolic cosine"},
	{"exp", osc_expr_1arg_dbl, 1, (void *)exp, "Exponential function"},
	{"floor", osc_expr_1arg_dbl, 1, (void *)floor, "Round down to the nearest integer"},
	{"fmod", osc_expr_2arg_dbl_dbl, 2, (void *)fmod, "Floating-point remainder"},
	{"log", osc_expr_1arg_dbl, 1, (void *)log, "Natural logarithm"},
	{"log10", osc_expr_1arg_dbl, 1, (void *)log10, "Base 10 logarithm"},
	{"pow", osc_expr_2arg_dbl_dbl, 2, (void *)pow, "Power"},
	{"sin", osc_expr_1arg_dbl, 1, (void *)sin, "Sine"},
	{"sinh", osc_expr_1arg_dbl, 1, (void *)sinh, "Hyperbolic sine"},
	{"sqrt", osc_expr_1arg_dbl, 1, (void *)sqrt, "Square root"},
	{"tan", osc_expr_1arg_dbl, 1, (void *)tan, "Tangent"},
	{"tanh", osc_expr_1arg_dbl, 1, (void *)tanh, "Hyperbolic tangent"},
	{"erf", osc_expr_1arg_dbl, 1, (void *)erf, "Error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"erfc", osc_expr_1arg_dbl, 1, (void *)erfc, "Complementary error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"gamma", osc_expr_1arg_dbl, 1, (void *)lgamma, "Log gamma function (same as \"lgamma\")"},
	{"hypot", osc_expr_2arg_dbl_dbl, 2, (void *)hypot, "Euclidean distance"},
	{"j0", osc_expr_1arg_dbl, 1, (void *)j0, "0th Bessel function of the first kind"},
	{"j1", osc_expr_1arg_dbl, 1, (void *)j1, "1st Bessel function of the first kind"},
	{"jn", osc_expr_2arg_dbl_dbl, 2, (void *)jn, "nth Bessel function of the first kind"},
	{"lgamma", osc_expr_1arg_dbl, 1, (void *)lgamma, "Log gamma function (same as \"gamma\")"},
	{"y0", osc_expr_1arg_dbl, 1, (void *)y0, "0th Bessel function of the second kind"},
	{"y1", osc_expr_1arg_dbl, 1, (void *)y1, "1st Bessel function of the second kind"},
	{"yn", osc_expr_2arg_dbl_dbl, 2, (void *)yn, "nth Bessel function of the second kind"},
	{"acosh", osc_expr_1arg_dbl, 1, (void *)acosh, "Inverse hyperbolic cosine"},
	{"asinh", osc_expr_1arg_dbl, 1, (void *)asinh, "Inverse hyperbolic sine"},
	{"atanh", osc_expr_1arg_dbl, 1, (void *)atanh, "Inverse hyperbolic tangent"},
	{"cbrt", osc_expr_1arg_dbl, 1, (void *)cbrt, "Cube root"},
	{"expm1", osc_expr_1arg_dbl, 1, (void *)expm1, "Exponential function (e^x - 1)"},
	{"ilogb", osc_expr_1arg_dbl, 1, (void *)ilogb, "Unbiased exponent"},
	{"logb", osc_expr_1arg_dbl, 1, (void *)logb, "Radix-independent exponent"},
	{"nextafter", osc_expr_2arg_dbl_dbl, 1, (void *)nextafter, "Next representable double-precision floating-point number"},
	{"remainder", osc_expr_2arg_dbl_dbl, 2, (void *)remainder, "Remainder function (r = x - ny where y is non-zero and n is the integral value nearest x/y)"},
	{"round", osc_expr_1arg_dbl, 1, (void *)round, "Round to nearest integral value"},
	// misc
	{"get_index", osc_expr_get_index, -1, NULL, "Get an element of a list (same as [[ ]])"},
	{"product", osc_expr_product, 1, NULL, "Product of all the elements of a list"},
	{"sum", osc_expr_sum, 1, NULL, "Sum all the elements of a list"},
	{"cumsum", osc_expr_cumsum, 1, NULL, "Cumulative sum"},
	{"length", osc_expr_length, 1, NULL, "Get the length of a list"},
	{"avg", osc_expr_mean, 1, NULL, "The average of a list (same as mean)"},
	{"mean", osc_expr_mean, 1, NULL, "The average of a list (same as avg)"},
	{"median", osc_expr_median, 1, NULL, "Median of a list of values"},
	{"concat", osc_expr_concat, -1, NULL, "Concatenate two lists"},
	{"reverse", osc_expr_reverse, 1, NULL, "Reverse the order of the elements of a list"},
	{"rev", osc_expr_reverse, 1, NULL, "Reverse the order of the elements of a list"},
	{"make_list", osc_expr_make_list, -1, NULL, "Make a list of <arg1> copies of <arg2>.  <arg2 is optional and defaults to 0"},
	{"range", osc_expr_range, -1, NULL, "Make a list from <arg1> to <arg2> in <arg3> steps.  <arg3> is optional and defaults to 1"},
	{"multiplex", osc_expr_multiplex, -1, NULL, "Multiplex two or more lists"},
	{"mux", osc_expr_multiplex, -1, NULL, "Multiplex two or more lists"},
	//make-list, pad, zeros, ones
	{"!", osc_expr_not, -1, NULL, "Logical not"},
	{"dot", osc_expr_dot, 2, NULL, "Dot product of arg1 and arg2"},
	{"l2norm", osc_expr_l2norm, 1, NULL, "Norm of the argument"},
	{"min", osc_expr_min, 1, NULL, "Minimum value of the arguments"},
	{"max", osc_expr_max, 1, NULL, "Maximum value of the arguments"},
	{"extrema", osc_expr_extrema, 1, NULL, "Min and max of the arguments"},
	{"clip", osc_expr_clip, 3, NULL, "Clip the data between arg2 and arg3"},
	{"scale", osc_expr_scale, 5, NULL, "Scale the data from arg1 and arg2 to arg3 and arg4"},
	{"mtof", osc_expr_mtof, -1, NULL, "MIDI note number to frequency.  Optional arg2 sets base."},
	{"ftom", osc_expr_ftom, -1, NULL, "Frequency to MIDI. Optional arg2 sets base."},
	{"rand", osc_expr_rand, 0, NULL, "Crappy UNIX rand() scaled to [0.,1.]"},
	{"sgn", osc_expr_sgn, 1, NULL, "Sign function--returns -1 if <arg1> < 0, 0 if <arg1> == 0, and 1 if <arg1> > 1"},
	{"if", osc_expr_if, -1, NULL, "Conditionally execute <arg2> or optional <arg3> based on the result of <arg1>"},
	{"defined", osc_expr_defined, 1, NULL, "Check for the existance a message with address <arg1>."},
	{"nothing", osc_expr_nothing, -1, NULL, "Just what it says"},
	{"eval", osc_expr_eval, 1, NULL, "Evaluate a function bound to an OSC address"}
};

#ifdef _cplusplus
}
#endif

#endif // __OSC_EXPR_H__
