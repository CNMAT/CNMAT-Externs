#include <Carbon.h>
#include <CoreServices.h>
#include "omax_parser.h"
#include "omax_expr.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"

int omax_expr_funcall(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out){
	int ret = omax_expr_call(f, len, oscbndl, argc_out, argv_out);
	return ret;
}

int omax_expr_getArg(t_omax_expr_arg *arg, int len, char *oscbndl, int *argc_out, double **argv_out){
	switch(arg->type){
	case OMAX_ARG_TYPE_ATOM:
		{
			double *f = (double *)osc_mem_alloc(sizeof(double));
			*f = atom_getfloat(&(arg->arg.atom));
			*argv_out = f;
			*argc_out = 1;
			return 0;
		}
	case OMAX_ARG_TYPE_EXPR:
		{
			return omax_expr_funcall(arg->arg.expr, len, oscbndl, argc_out, argv_out);
		}
	case OMAX_ARG_TYPE_OSCADDRESS:
		{
			t_osc_msg *m = NULL;
			osc_bundle_lookupAddressSerialized(len, oscbndl, arg->arg.osc_address, &m, 1);
			if(m){
				*argc_out = m->argc;
				double *argv = (double *)osc_mem_alloc(m->argc * sizeof(double));
				int i = 0;
				while(osc_message_incrementArg(m)){
					switch(*(m->typetags)){
					case 'i':
						argv[i] = (double)(ntohl(*((int32_t *)m->argv)));
						break;
					case 'f':
						argv[i] = (double)(*((float *)m->argv));
						break;
					case 'd':
						argv[i] = *((double *)m->argv);
						break;
					default:
						argv[i] = 0.0;
					}
					i++;
				}
				osc_message_free(m);
				*argv_out = argv;
				return 0;
			}else{
				*argc_out = 0;
				*argv_out = NULL;
				return 1;
			}
		}
	}
	return 1; // this really shouldn't happen unless there's a bug somewhere
}

int omax_expr_call(t_omax_expr *f, int len, char *oscbndl, int *argc_out, double **argv_out){
	int f_argc = f->argc;
	/*
	if(f->argc != f->rec->num_args){
		error("omax_expr: bad arg count.  Expected %d but got %d", f->rec->num_args, f_argc);
		return 1;
	}
	*/
	t_omax_expr_arg *f_argv = f->argv;
	int argc[f_argc];
	memset(argc, '\0', sizeof(f_argc));
	double *argv[f_argc];
	memset(argv, '\0', sizeof(argv));
	uint32_t min_argc = ~0, max_argc = 0;
	int i = 0;
	while(f_argv){
		int ret = omax_expr_getArg(f_argv, len, oscbndl, argc + i, argv + i);
		if(ret){
			return ret;
		}
		if(argc[i] < min_argc){
			min_argc = argc[i];
		}
		if(argc[i] > max_argc){
			max_argc = argc[i];
		}
		f_argv = f_argv->next;
		i++;
	}
	int ret = f->rec->func(f, argc, argv, argc_out, argv_out);
	for(i = 0; i < f_argc; i++){
		if(argv[i]){
			osc_mem_free(argv[i]);
		}
	}
	return ret;
	/*
	*argc_out = max_argc;
	double *result = (double *)osc_mem_alloc(sizeof(double) * max_argc);
	memset(result, '\0', max_argc * sizeof(double));
	for(i = 0; i < max_argc; i++){
		switch(f->rec->num_args){
		case 0:
			break;
		case 1:

			break;
		case 2:{
			if(i >= argc[0]){
				if(i < argc[1]){
					result[i] = argv[1][i];
				}
				break;
			}else if(i >= argc[1]){
				if(i < argc[0]){
					result[i] = argv[0][i];
				}
				break;
			}
			double (*func)(double,double) = (double (*)(double,double))(f->rec->func);
			result[i] = func(argv[0][i], argv[1][i]);
			break;
		}
		}
	}
	if(argv[0]){
		osc_mem_free(argv[0]);
	}
	if(argv[1]){
		osc_mem_free(argv[1]);
	}
	*argv_out = result;
	return 0;
	*/
}

int omax_expr_1arg_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
	*argc_out = *argc;
	double *result = (double *)osc_mem_alloc(*argc * sizeof(double));
	double (*func)(double) = (double (*)(double))(f->rec->extra);
	int i;
	for(i = 0; i < *argc; i++){
		result[i] = func(argv[0][i]);
	}
	*argv_out = result;
	return 0;
}

int omax_expr_1arg_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
}

int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
	uint32_t min_argc = argc[0], max_argc = argc[1];
	if(argc[0] > argc[1]){
		min_argc = argc[1], max_argc = argc[0];
	}
	int i;
	double (*func)(double,double) = (double (*)(double,double))(f->rec->extra);
	if(argc[0] == 1){
		*argc_out = max_argc;
		double *result = (double *)osc_mem_alloc(sizeof(double) * max_argc);
		memset(result, '\0', max_argc * sizeof(double));
		for(i = 0; i < max_argc; i++){
			result[i] = func(argv[0][0], argv[1][i]);
		}
		*argv_out = result;
		return 0;
	}else if(argc[1] == 1){
		*argc_out = max_argc;
		double *result = (double *)osc_mem_alloc(sizeof(double) * max_argc);
		memset(result, '\0', max_argc * sizeof(double));
		for(i = 0; i < max_argc; i++){
			result[i] = func(argv[0][i], argv[1][0]);
		}
		*argv_out = result;
		return 0;
	}else{
		*argc_out = min_argc;
		double *result = (double *)osc_mem_alloc(sizeof(double) * min_argc);
		memset(result, '\0', min_argc * sizeof(double));
		for(i = 0; i < min_argc; i++){
			/*
			if(i >= argc[0]){
				if(i < argc[1]){
					result[i] = argv[1][i];
					break;
				}
			}else if(i >= argc[1]){
				if(i < argc[0]){
					result[i] = argv[0][i];
					break;
				}
			}
			*/
			result[i] = func(argv[0][i], argv[1][i]);
		}
		*argv_out = result;
		return 0;
	}
}

int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
}

int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
}

int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
}

// wrappers for infix ops
double omax_expr_add(double f1, double f2){
	return f1 + f2;
}

double omax_expr_subtract(double f1, double f2){
	return f1 - f2;
}

double omax_expr_multiply(double f1, double f2){
	return f1 * f2;
}

double omax_expr_divide(double f1, double f2){
	return f1 / f2;
}

double omax_expr_lt(double f1, double f2){
	return f1 < f2;
}

double omax_expr_lte(double f1, double f2){
	return f1 <= f2;
}

double omax_expr_gt(double f1, double f2){
	return f1 > f2;
}

double omax_expr_gte(double f1, double f2){
	return f1 >= f2;
}

double omax_expr_eq(double f1, double f2){
	return f1 == f2;
}

double omax_expr_neq(double f1, double f2){
	return f1 != f2;
}

double omax_expr_and(double f1, double f2){
	return f1 && f2;
}

double omax_expr_or(double f1, double f2){
	return f1 || f2;
}

double omax_expr_mod(double f1, double f2){
	return (int)f1 % (int)f2;
}

int omax_expr_get_index(t_omax_expr *f, int *argc, double **argv, int *argc_out, double **argv_out){
	*argc_out = 1;
	*argv_out = (double *)malloc(sizeof(double));
	**argv_out = 0;
	if(argv[1][0] > argc[0] - 1){
		error("index %d exceeds array length (%d)", (int)argv[1][0], (int)argc[0]);
		return 1;
	}
	**argv_out = argv[0][(int)argv[1][0]];
	return 0;
}

t_omax_expr_arg *omax_expr_arg_alloc(void){
	t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
	memset(arg, '\0', sizeof(t_omax_expr_arg));
	return arg;
}
