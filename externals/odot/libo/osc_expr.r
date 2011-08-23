#ifndef __OSC_EXPR_R__
#define __OSC_EXPR_R__

#ifdef __cplusplus
extern "C" {
#endif

#include "osc_atom_u.h"

struct _osc_expr{
	struct _osc_expr_rec *rec;
	struct _osc_expr_arg *argv;
	int argc; 
	int assign_result_to_address;
	struct _osc_expr *next; 
};

struct _osc_expr_arg{
	union arg{
		t_osc_atom_u *atom;
		struct _osc_expr *expr;
		char *osc_address;
	} arg;
	int type;
	struct _osc_expr_arg *next;
};

#ifdef __cplusplus
}
#endif

#endif // __OSC_EXPR_R__