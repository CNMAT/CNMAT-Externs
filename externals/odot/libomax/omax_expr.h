#ifndef __OMAX_EXPR_H__
#define __OMAX_EXPR_H__

#include "ext.h"

#define OMAX_ARG_TYPE_ATOM 1
#define OMAX_ARG_TYPE_FUNC 2
#define OMAX_ARG_TYPE_OSCADDRESS 3
	
typedef struct _omax_func{
	int (*f)(struct _omax_func *, int *, t_atom **);
	struct _omax_func_arg *argv;
	int argc;
	struct _omax_func *next, *prev;
} t_omax_func;

typedef struct _omax_func_arg{
	union arg{
		t_atom atom;
		struct _omax_func *func;
		char *osc_address;
	}arg;
	int type;
	struct _omax_func_arg *next;
} t_omax_func_arg;

typedef int (*t_omax_func_ptr)(t_omax_func *f, int *argc_out, t_atom **argv_out);

// these all return non-zero in the case of an error
int omax_parser_funcall(t_omax_func *f, int *argc_out, t_atom **argv_out);
int omax_parser_getargs(t_omax_func_arg *arglist, int **argc, double **argv);
int omax_parser_plus(struct _omax_func *f, int *argc_out, t_atom **argv_out);

#endif // __OMAX_EXPR_H__
