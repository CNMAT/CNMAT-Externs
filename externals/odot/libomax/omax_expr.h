#ifndef __OMAX_EXPR_H__
#define __OMAX_EXPR_H__

#include "ext.h"

#define OMAX_ARG_TYPE_ATOM 1
#define OMAX_ARG_TYPE_EXPR 2
#define OMAX_ARG_TYPE_OSCADDRESS 3
	
typedef struct _omax_expr{
	int (*f)(struct _omax_expr *, int *, double **);
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

typedef int (*t_omax_funcptr)(t_omax_expr *f, int *argc_out, double **argv_out);

// these all return non-zero in the case of an error
int omax_expr_funcall(t_omax_expr *f, int *argc_out, double **argv_out);
int omax_expr_getArg(t_omax_expr_arg *arg, int *argc_out, double **argv_out);
int omax_expr_plus(t_omax_expr *f, int *argc_out, double **argv_out);
t_omax_expr_arg *omax_expr_arg_alloc(void);

#endif // __OMAX_EXPR_H__
