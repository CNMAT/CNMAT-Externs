#ifndef __OMAX_EXPR_H__
#define __OMAX_EXPR_H__

#include "ext.h"

#define OMAX_ARG_TYPE_ATOM 1
#define OMAX_ARG_TYPE_EXPR 2
#define OMAX_ARG_TYPE_OSCADDRESS 3
	
typedef struct _omax_expr{
	//int (*f)(struct _omax_expr *, int, char *, int *, double **);
	struct _omax_expr_rec *rec;
	struct _omax_expr_arg *argv;
	int argc;
	struct _omax_expr *next, *prev;
} t_omax_expr;

typedef struct _omax_expr_arg{
	union arg{
		t_atom atom;
		struct _omax_expr *expr;
		char *osc_address;
	}arg;
	int type;
	struct _omax_expr_arg *next;
} t_omax_expr_arg;

typedef int (*t_omax_funcptr)(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);

typedef struct _omax_expr_rec{
	char *name;
	//t_omax_funcptr *func;
	int (*func)(t_omax_expr*, int*, double**, int*, double**);
	void *extra;
} t_omax_expr_rec;

//typedef int (*t_omax_funcptr)(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);

// these all return non-zero in the case of an error
int omax_expr_funcall(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_getArg(t_omax_expr_arg *arg, int len, char *oscbndl, int *argc_out, double **argv_out);

int omax_expr_1arg_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);
int omax_expr_1arg_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);
int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);
int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);
int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);
int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);

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

int omax_expr_get_index(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out);

/*
int omax_expr_add(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_subtract(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_multiply(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_divide(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_lt(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_lte(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_gt(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_gte(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_eq(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
int omax_expr_neq(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out);
*/
t_omax_expr_arg *omax_expr_arg_alloc(void);

static t_omax_expr_rec omax_expr_funcsym[] = {
	// infix
	{"+", omax_expr_2arg_dbl_dbl, (void *)omax_expr_add},
	{"-", omax_expr_2arg_dbl_dbl, (void *)omax_expr_subtract},
	{"*", omax_expr_2arg_dbl_dbl, (void *)omax_expr_multiply},
	{"/", omax_expr_2arg_dbl_dbl, (void *)omax_expr_divide},
	{"<", omax_expr_2arg_dbl_dbl, (void *)omax_expr_lt},
	{"<=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_lte},
	{">", omax_expr_2arg_dbl_dbl, (void *)omax_expr_gt},
	{">=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_gte},
	{"==", omax_expr_2arg_dbl_dbl, (void *)omax_expr_eq},
	{"!=", omax_expr_2arg_dbl_dbl, (void *)omax_expr_neq},
	{"&&", omax_expr_2arg_dbl_dbl, (void *)omax_expr_and},
	{"||", omax_expr_2arg_dbl_dbl, (void *)omax_expr_or},
	{"%", omax_expr_2arg_dbl_dbl, (void *)omax_expr_mod},
	// math.h
	{"abs", omax_expr_1arg_dbl, (void *)abs},
	{"acos", omax_expr_1arg_dbl, (void *)acos},
	{"asin", omax_expr_1arg_dbl, (void *)asin},
	{"atan", omax_expr_1arg_dbl, (void *)atan},
	{"atan2", omax_expr_2arg_dbl_dbl, (void *)atan2},
	{"ceil", omax_expr_1arg_dbl, (void *)ceil},
	{"cos", omax_expr_1arg_dbl, (void *)cos},
	{"cosh", omax_expr_1arg_dbl, (void *)cosh},
	{"exp", omax_expr_1arg_dbl, (void *)exp},
	{"floor", omax_expr_1arg_dbl, (void *)floor},
	{"fmod", omax_expr_2arg_dbl_dbl, (void *)fmod},
	{"log", omax_expr_1arg_dbl, (void *)log},
	{"log10", omax_expr_1arg_dbl, (void *)log10},
	{"pow", omax_expr_2arg_dbl_dbl, (void *)pow},
	{"sin", omax_expr_1arg_dbl, (void *)sin},
	{"sinh", omax_expr_1arg_dbl, (void *)sinh},
	{"sqrt", omax_expr_1arg_dbl, (void *)sqrt},
	{"tan", omax_expr_1arg_dbl, (void *)tan},
	{"tanh", omax_expr_1arg_dbl, (void *)tanh},
	{"erf", omax_expr_1arg_dbl, (void *)erf},
	{"erfc", omax_expr_1arg_dbl, (void *)erfc},
	{"gamma", omax_expr_1arg_dbl, (void *)gamma},
	{"hypot", omax_expr_2arg_dbl_dbl, (void *)hypot},
	{"j0", omax_expr_1arg_dbl, (void *)j0},
	{"j1", omax_expr_1arg_dbl, (void *)j1},
	{"jn", omax_expr_2arg_dbl_dbl, (void *)jn},
	{"lgamma", omax_expr_1arg_dbl, (void *)lgamma},
	{"y0", omax_expr_1arg_dbl, (void *)y0},
	{"y1", omax_expr_1arg_dbl, (void *)y1},
	{"yn", omax_expr_2arg_dbl_dbl, (void *)yn},
	{"acosh", omax_expr_1arg_dbl, (void *)acosh},
	{"asinh", omax_expr_1arg_dbl, (void *)asinh},
	{"atanh", omax_expr_1arg_dbl, (void *)atanh},
	{"cbrt", omax_expr_1arg_dbl, (void *)cbrt},
	{"expm1", omax_expr_1arg_dbl, (void *)expm1},
	{"ilogb", omax_expr_1arg_dbl, (void *)ilogb},
	{"logb", omax_expr_1arg_dbl, (void *)logb},
	{"nextafter", omax_expr_2arg_dbl_dbl, (void *)nextafter},
	{"remainder", omax_expr_2arg_dbl_dbl, (void *)remainder},
	{"rint", omax_expr_1arg_dbl, (void *)rint},
	{"round", omax_expr_1arg_dbl, (void *)round},
	// misc
	{"get_index", omax_expr_get_index, NULL}
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
