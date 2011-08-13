#ifndef __OMAX_EXPR_H__
#define __OMAX_EXPR_H__

#include "ext.h"
#include "omax_util.h"
#include "osc_atom_u.h"

#define OMAX_ARG_TYPE_ATOM 1
#define OMAX_ARG_TYPE_EXPR 2
#define OMAX_ARG_TYPE_OSCADDRESS 3

/** \struct t_omax_expr
    Data structure representing an expression with a linked-list of arguments.
*/
typedef struct _omax_expr{
	/** The record corresponding to the function in this expression.
	 Contains the function name as a string and a pointer to the function to be called
	*/
	struct _omax_expr_rec *rec;
	struct _omax_expr_arg *argv; /**< Arguments to the function (linked list)*/
	int argc; /**< Number of arguments */
	//struct _omax_expr *next, *prev; /**< Links to the next and previous expressions */
	int assign_result_to_address;
	struct _omax_expr *next; 
	int putlast ; // hack
} t_omax_expr;

/** \struct t_omax_expr_arg
    Data structure representing an argument to a function.  The contents of this argument
    can be a Max atom, an OSC address, or an expression.
 */
typedef struct _omax_expr_arg{
	union arg{
		t_atom atom;
		struct _omax_expr *expr;
		char *osc_address;
	}arg;
	int type;
	struct _omax_expr_arg *next;
} t_omax_expr_arg;

typedef int (*t_omax_funcptr)(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

/** \struct t_omax_expr_rec
    A record that associates a function name (string) with a function pointer.
*/
typedef struct _omax_expr_rec{
	const char *name; /**< Name of the function as a C string. */
	int (*func)(t_omax_expr*, int, t_osc_atom_ar_u**, t_osc_atom_ar_u**); /**< Function pointer */
	int numargs;
	void *extra; /**< Extra field that can contain anything. */
	const char *desc;
} t_omax_expr_rec;

typedef struct _omax_expr_const_rec{
	const char *name;
	double val;
	const char *desc;
} t_omax_expr_const_rec;

/**
   Call a function.
   @param f The struct containing the function to be called.
   @param len The length in bytes of the OSC bundle.
   @param oscbndl Pointer to the OSC bundle.
   @param argc_out Length of the output array.
   @param argv_out Output array--the result of calling the function.

   @returns 0 if no error, 1 if there was an error.
 */
int omax_expr_funcall(t_omax_expr *function, long *len, char **oscbndl, t_osc_atom_ar_u **out);
int omax_expr_getArg(t_omax_expr_arg *arg, long *len, char **oscbndl, t_osc_atom_ar_u **out);
int omax_expr_call(t_omax_expr *f, long *len, char **oscbndl, t_osc_atom_ar_u **out);
t_omax_expr_rec *omax_expr_lookupFunction(char *name);

int omax_expr_1arg_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_1arg_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_2arg(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

void omax_expr_add(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_subtract(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_multiply(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_divide(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_lt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_lte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_gt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_gte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_eq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_neq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_and(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_or(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);
void omax_expr_mod(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result);

int omax_expr_assign(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);

int omax_expr_get_index(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_product(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_sum(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_cumsum(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_length(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_mean(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_median(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_concat(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_reverse(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_make_list(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_range(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_multiplex(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_not(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_dot(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_l2norm(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_min(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_max(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_extrema(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_clip(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_scale(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_mtof(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_ftom(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_rand(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_sgn(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_if(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);
int omax_expr_defined(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out);


t_omax_expr_arg *omax_expr_arg_alloc(void);
void omax_expr_free(t_omax_expr *e);
void omax_expr_arg_free(t_omax_expr_arg *a);
void omax_expr_parse(t_omax_expr **function_graph, long argclex, t_atom *argvlex);

static t_omax_expr_const_rec omax_expr_constsym[] = {
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

static t_omax_expr_rec omax_expr_funcsym[] = {
	// infix
	{"+", omax_expr_2arg, 2, (void *)omax_expr_add, "Add"},
	{"-", omax_expr_2arg, 2, (void *)omax_expr_subtract, "Subtract"},
	{"*", omax_expr_2arg, 2, (void *)omax_expr_multiply, "Multiply"},
	{"/", omax_expr_2arg, 2, (void *)omax_expr_divide, "Divide"},
	{"<", omax_expr_2arg, 2, (void *)omax_expr_lt, "Less than"},
	{"<=", omax_expr_2arg, 2, (void *)omax_expr_lte, "Less than or equal to"},
	{">", omax_expr_2arg, 2, (void *)omax_expr_gt, "Greater than"},
	{">=", omax_expr_2arg, 2, (void *)omax_expr_gte, "Greater than or equal to"},
	{"==", omax_expr_2arg, 2, (void *)omax_expr_eq, "Equal"},
	{"!=", omax_expr_2arg, 2, (void *)omax_expr_neq, "Not equal"},
	{"&&", omax_expr_2arg, 2, (void *)omax_expr_and, "Logical and"},
	{"||", omax_expr_2arg, 2, (void *)omax_expr_or, "Logical or"},
	{"%", omax_expr_2arg, 2, (void *)omax_expr_mod, "Modulo"},
	{"=", omax_expr_assign, 2, NULL, "Assignment"},
	{"++", omax_expr_2arg, 1, (void *)omax_expr_add, "Increment"},
	{"--", omax_expr_2arg, 1, (void *)omax_expr_subtract, "Decrement"},
	{"+=", omax_expr_2arg, 2, (void *)omax_expr_add, "Add and assign"},
	{"-=", omax_expr_2arg, 2, (void *)omax_expr_subtract, "Subtract and assign"},
	{"*=", omax_expr_2arg, 2, (void *)omax_expr_multiply, "Multiply and assign"},
	{"/=", omax_expr_2arg, 2, (void *)omax_expr_divide, "Divide and assign"},
	{"%=", omax_expr_2arg, 2, (void *)omax_expr_mod, "Modulo and assign"},
	{"??", NULL, 2, NULL, "Null coalescing operator, returns <arg1> if it exists, otherwise returns <arg2>."},
	// math.h
	{"abs", omax_expr_1arg_dbl, 1, (void *)fabs, "Absolute value"},
	{"acos", omax_expr_1arg_dbl, 1, (void *)acos, "Arc cosine"},
	{"asin", omax_expr_1arg_dbl, 1, (void *)asin, "Arc sine"},
	{"atan", omax_expr_1arg_dbl, 1, (void *)atan, "Arc tangent"},
	{"atan2", omax_expr_2arg_dbl_dbl, 2, (void *)atan2, "Arc tangent of y/x (arg1/arg2)"},
	{"ceil", omax_expr_1arg_dbl, 1, (void *)ceil, "Ceiling--round up to the nearest integer"},
	{"cos", omax_expr_1arg_dbl, 1, (void *)cos, "Cosine"},
	{"cosh", omax_expr_1arg_dbl, 1, (void *)cosh, "Hyperbolic cosine"},
	{"exp", omax_expr_1arg_dbl, 1, (void *)exp, "Exponential function"},
	{"floor", omax_expr_1arg_dbl, 1, (void *)floor, "Round down to the nearest integer"},
	{"fmod", omax_expr_2arg_dbl_dbl, 2, (void *)fmod, "Floating-point remainder"},
	{"log", omax_expr_1arg_dbl, 1, (void *)log, "Natural logarithm"},
	{"log10", omax_expr_1arg_dbl, 1, (void *)log10, "Base 10 logarithm"},
	{"pow", omax_expr_2arg_dbl_dbl, 2, (void *)pow, "Power"},
	{"sin", omax_expr_1arg_dbl, 1, (void *)sin, "Sine"},
	{"sinh", omax_expr_1arg_dbl, 1, (void *)sinh, "Hyperbolic sine"},
	{"sqrt", omax_expr_1arg_dbl, 1, (void *)sqrt, "Square root"},
	{"tan", omax_expr_1arg_dbl, 1, (void *)tan, "Tangent"},
	{"tanh", omax_expr_1arg_dbl, 1, (void *)tanh, "Hyperbolic tangent"},
	{"erf", omax_expr_1arg_dbl, 1, (void *)erf, "Error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"erfc", omax_expr_1arg_dbl, 1, (void *)erfc, "Complementary error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"gamma", omax_expr_1arg_dbl, 1, (void *)lgamma, "Log gamma function (same as \"lgamma\")"},
	{"hypot", omax_expr_2arg_dbl_dbl, 2, (void *)hypot, "Euclidean distance"},
	{"j0", omax_expr_1arg_dbl, 1, (void *)j0, "0th Bessel function of the first kind"},
	{"j1", omax_expr_1arg_dbl, 1, (void *)j1, "1st Bessel function of the first kind"},
	{"jn", omax_expr_2arg_dbl_dbl, 2, (void *)jn, "nth Bessel function of the first kind"},
	{"lgamma", omax_expr_1arg_dbl, 1, (void *)lgamma, "Log gamma function (same as \"gamma\")"},
	{"y0", omax_expr_1arg_dbl, 1, (void *)y0, "0th Bessel function of the second kind"},
	{"y1", omax_expr_1arg_dbl, 1, (void *)y1, "1st Bessel function of the second kind"},
	{"yn", omax_expr_2arg_dbl_dbl, 2, (void *)yn, "nth Bessel function of the second kind"},
	{"acosh", omax_expr_1arg_dbl, 1, (void *)acosh, "Inverse hyperbolic cosine"},
	{"asinh", omax_expr_1arg_dbl, 1, (void *)asinh, "Inverse hyperbolic sine"},
	{"atanh", omax_expr_1arg_dbl, 1, (void *)atanh, "Inverse hyperbolic tangent"},
	{"cbrt", omax_expr_1arg_dbl, 1, (void *)cbrt, "Cube root"},
	{"expm1", omax_expr_1arg_dbl, 1, (void *)expm1, "Exponential function (e^x - 1)"},
	{"ilogb", omax_expr_1arg_dbl, 1, (void *)ilogb, "Unbiased exponent"},
	{"logb", omax_expr_1arg_dbl, 1, (void *)logb, "Radix-independent exponent"},
	{"nextafter", omax_expr_2arg_dbl_dbl, 1, (void *)nextafter, "Next representable double-precision floating-point number"},
	{"remainder", omax_expr_2arg_dbl_dbl, 2, (void *)remainder, "Remainder function (r = x - ny where y is non-zero and n is the integral value nearest x/y)"},
	{"round", omax_expr_1arg_dbl, 1, (void *)round, "Round to nearest integral value"},
	// misc
	{"get_index", omax_expr_get_index, -1, NULL, "Get an element of a list (same as [[ ]])"},
	{"product", omax_expr_product, 1, NULL, "Product of all the elements of a list"},
	{"sum", omax_expr_sum, 1, NULL, "Sum all the elements of a list"},
	{"cumsum", omax_expr_cumsum, 1, NULL, "Cumulative sum"},
	{"length", omax_expr_length, 1, NULL, "Get the length of a list"},
	{"avg", omax_expr_mean, 1, NULL, "The average of a list (same as mean)"},
	{"mean", omax_expr_mean, 1, NULL, "The average of a list (same as avg)"},
	{"median", omax_expr_median, 1, NULL, "Median of a list of values"},
	{"concat", omax_expr_concat, -1, NULL, "Concatenate two lists"},
	{"reverse", omax_expr_reverse, 1, NULL, "Reverse the order of the elements of a list"},
	{"rev", omax_expr_reverse, 1, NULL, "Reverse the order of the elements of a list"},
	{"make_list", omax_expr_make_list, -1, NULL, "Make a list of <arg1> copies of <arg2>.  <arg2 is optional and defaults to 0"},
	{"range", omax_expr_range, -1, NULL, "Make a list from <arg1> to <arg2> in <arg3> steps.  <arg3> is optional and defaults to 1"},
	{"multiplex", omax_expr_multiplex, -1, NULL, "Multiplex two or more lists"},
	{"mux", omax_expr_multiplex, -1, NULL, "Multiplex two or more lists"},
	//make-list, pad, zeros, ones
	{"!", omax_expr_not, -1, NULL, "Logical not"},
	{"dot", omax_expr_dot, 2, NULL, "Dot product of arg1 and arg2"},
	{"l2norm", omax_expr_l2norm, 1, NULL, "Norm of the argument"},
	{"min", omax_expr_min, 1, NULL, "Minimum value of the arguments"},
	{"max", omax_expr_max, 1, NULL, "Maximum value of the arguments"},
	{"extrema", omax_expr_extrema, 1, NULL, "Min and max of the arguments"},
	{"clip", omax_expr_clip, 3, NULL, "Clip the data between arg2 and arg3"},
	{"scale", omax_expr_scale, 5, NULL, "Scale the data from arg1 and arg2 to arg3 and arg4"},
	{"mtof", omax_expr_mtof, -1, NULL, "MIDI note number to frequency.  Optional arg2 sets base."},
	{"ftom", omax_expr_ftom, -1, NULL, "Frequency to MIDI. Optional arg2 sets base."},
	{"rand", omax_expr_rand, 0, NULL, "Crappy UNIX rand() scaled to [0.,1.]"},
	{"sgn", omax_expr_sgn, 1, NULL, "Sign function--returns -1 if <arg1> < 0, 0 if <arg1> == 0, and 1 if <arg1> > 1"},
	{"if", omax_expr_if, -1, NULL, "Conditionally execute <arg2> or optional <arg3> based on the result of <arg1>"},
	{"defined", omax_expr_defined, 1, NULL, "Check for the existance a message with address <arg1>."}
};

#endif // __OMAX_EXPR_H__
