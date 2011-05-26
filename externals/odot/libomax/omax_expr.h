#ifndef __OMAX_EXPR_H__
#define __OMAX_EXPR_H__

#include "ext.h"
#include "omax_util.h"

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

typedef int (*t_omax_funcptr)(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);

/** \struct t_omax_expr_rec
    A record that associates a function name (string) with a function pointer.
*/
typedef struct _omax_expr_rec{
	const char *name; /**< Name of the function as a C string. */
	int (*func)(t_omax_expr*, int*, t_atom64**, int*, t_atom64**); /**< Function pointer */
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
int omax_expr_funcall(t_omax_expr *f, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out);
int omax_expr_getArg(t_omax_expr_arg *arg, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out);
int omax_expr_call(t_omax_expr *f, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out);

int omax_expr_1arg_dbl(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_1arg_dblptr(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);

double omax_expr_add(double f1, double f2);
double omax_expr_subtract(double f1, double f2);
double omax_expr_multiply(double f1, double f2);
double omax_expr_divide(double f1, double f2);
double omax_expr_lt(double f1, double f2);
double omax_expr_lte(double f1, double f2);
double omax_expr_gt(double f1, double f2);
double omax_expr_gte(double f1, double f2);
double omax_expr_eq(double f1, double f2);
double omax_expr_neq(double f1, double f2);
double omax_expr_and(double f1, double f2);
double omax_expr_or(double f1, double f2);
double omax_expr_mod(double f1, double f2);

int omax_expr_sum(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_length(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_get_index(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);
int omax_expr_not(t_omax_expr *f, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out);

t_omax_expr_arg *omax_expr_arg_alloc(void);
void omax_expr_free(t_omax_expr *e);
void omax_expr_arg_free(t_omax_expr_arg *a);

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
	{"+", omax_expr_2arg_dbl_dbl, (void *)omax_expr_add, "Add"},
	{"-", omax_expr_2arg_dbl_dbl, (void *)omax_expr_subtract, "Subtract"},
	{"*", omax_expr_2arg_dbl_dbl, (void *)omax_expr_multiply, "Multiply"},
	{"/", omax_expr_2arg_dbl_dbl, (void *)omax_expr_divide, "Divide"},
	{"<", omax_expr_2arg_dbl_dbl, (void *)omax_expr_lt, "Less than"},
	{"<=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_lte, "Less than or equal to"},
	{">", omax_expr_2arg_dbl_dbl, (void *)omax_expr_gt, "Greater than"},
	{">=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_gte, "Greater than or equal to"},
	{"==", omax_expr_2arg_dbl_dbl, (void *)omax_expr_eq, "Equal"},
	{"!=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_neq, "Not equal"},
	{"&&", omax_expr_2arg_dbl_dbl, (void *)omax_expr_and, "Logical and"},
	{"||", omax_expr_2arg_dbl_dbl, (void *)omax_expr_or, "Logical or"},
	{"%", omax_expr_2arg_dbl_dbl, (void *)omax_expr_mod, "Modulo"},
	// math.h
	{"abs", omax_expr_1arg_dbl, (void *)fabs, "Absolute value"},
	{"acos", omax_expr_1arg_dbl, (void *)acos, "Arc cosine"},
	{"asin", omax_expr_1arg_dbl, (void *)asin, "Arc sine"},
	{"atan", omax_expr_1arg_dbl, (void *)atan, "Arc tangent"},
	{"atan2", omax_expr_2arg_dbl_dbl, (void *)atan2, "Arc tangent of y/x (arg1/arg2)"},
	{"ceil", omax_expr_1arg_dbl, (void *)ceil, "Ceiling--round up to the nearest integer"},
	{"cos", omax_expr_1arg_dbl, (void *)cos, "Cosine"},
	{"cosh", omax_expr_1arg_dbl, (void *)cosh, "Hyperbolic cosine"},
	{"exp", omax_expr_1arg_dbl, (void *)exp, "Exponential function"},
	{"floor", omax_expr_1arg_dbl, (void *)floor, "Round down to the nearest integer"},
	{"fmod", omax_expr_2arg_dbl_dbl, (void *)fmod, "Floating-point remainder"},
	{"log", omax_expr_1arg_dbl, (void *)log, "Natural logarithm"},
	{"log10", omax_expr_1arg_dbl, (void *)log10, "Base 10 logarithm"},
	{"pow", omax_expr_2arg_dbl_dbl, (void *)pow, "Power"},
	{"sin", omax_expr_1arg_dbl, (void *)sin, "Sine"},
	{"sinh", omax_expr_1arg_dbl, (void *)sinh, "Hyperbolic sine"},
	{"sqrt", omax_expr_1arg_dbl, (void *)sqrt, "Square root"},
	{"tan", omax_expr_1arg_dbl, (void *)tan, "Tangent"},
	{"tanh", omax_expr_1arg_dbl, (void *)tanh, "Hyperbolic tangent"},
	{"erf", omax_expr_1arg_dbl, (void *)erf, "Error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"erfc", omax_expr_1arg_dbl, (void *)erfc, "Complementary error function (see http://pubs.opengroup.org/onlinepubs/007908799/xsh/erf.html)"},
	{"gamma", omax_expr_1arg_dbl, (void *)lgamma, "Log gamma function (same as \"lgamma\")"},
	{"hypot", omax_expr_2arg_dbl_dbl, (void *)hypot, "Euclidean distance"},
	{"j0", omax_expr_1arg_dbl, (void *)j0, "0th Bessel function of the first kind"},
	{"j1", omax_expr_1arg_dbl, (void *)j1, "1st Bessel function of the first kind"},
	{"jn", omax_expr_2arg_dbl_dbl, (void *)jn, "nth Bessel function of the first kind"},
	{"lgamma", omax_expr_1arg_dbl, (void *)lgamma, "Log gamma function (same as \"gamma\")"},
	{"y0", omax_expr_1arg_dbl, (void *)y0, "0th Bessel function of the second kind"},
	{"y1", omax_expr_1arg_dbl, (void *)y1, "1st Bessel function of the second kind"},
	{"yn", omax_expr_2arg_dbl_dbl, (void *)yn, "nth Bessel function of the second kind"},
	{"acosh", omax_expr_1arg_dbl, (void *)acosh, "Inverse hyperbolic cosine"},
	{"asinh", omax_expr_1arg_dbl, (void *)asinh, "Inverse hyperbolic sine"},
	{"atanh", omax_expr_1arg_dbl, (void *)atanh, "Inverse hyperbolic tangent"},
	{"cbrt", omax_expr_1arg_dbl, (void *)cbrt, "Cube root"},
	{"expm1", omax_expr_1arg_dbl, (void *)expm1, "Exponential function (e^x - 1)"},
	{"ilogb", omax_expr_1arg_dbl, (void *)ilogb, "Unbiased exponent"},
	{"logb", omax_expr_1arg_dbl, (void *)logb, "Radix-independent exponent"},
	{"nextafter", omax_expr_2arg_dbl_dbl, (void *)nextafter, "Next representable double-precision floating-point number"},
	{"remainder", omax_expr_2arg_dbl_dbl, (void *)remainder, "Remainder function (r = x - ny where y is non-zero and n is the integral value nearest x/y)"},
	//{"rint", omax_expr_1arg_dbl, (void *)rint, "Round to nearest integral value"},
	{"round", omax_expr_1arg_dbl, (void *)round, "Round to nearest integral value"},
	// misc
	{"get_index", omax_expr_get_index, NULL, "Get an element of a list (same as [[ ]])"},
	{"sum", omax_expr_sum, NULL, "Sum all the elements of a list"},
	{"length", omax_expr_length, NULL, "Get the length of a list"},
	{"!", omax_expr_not, NULL, "Logical not"}
};

/*
static t_omax_expr_rec omax_expr_funcsym[] = {
	{"+", omax_expr_add, 2},
	{"-", omax_expr_subtract, 2},
	{"*", omax_expr_multiply, 2},
	{"/", omax_expr_divide, 2},
	{"<", omax_expr_lt, 2},
	{"<=", omax_expr_lte, 2},
	{">", omax_expr_gt, 2},
	{">=", omax_expr_gte, 2},
	{"==", omax_expr_eq, 2},
	{"!=", omax_expr_neq, 2},
	{"get_index", omax_get_index, 2},
	{"pow", pow, 2}
};
*/
#endif // __OMAX_EXPR_H__
