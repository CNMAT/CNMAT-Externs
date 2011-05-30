#include <Carbon.h>
#include <CoreServices.h>
#include "omax_parser.h"
#include "omax_util.h"
#include "omax_expr.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"

int omax_expr_funcall(t_omax_expr *f, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out){
	int ret = omax_expr_call(f, len, oscbndl, argc_out, argv_out);
	return ret;
}

int omax_expr_getArg(t_omax_expr_arg *arg, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out){
	switch(arg->type){
	case OMAX_ARG_TYPE_ATOM:
		{
			t_atom64 *a = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
			atom64_setfloat(a, atom_getfloat(&(arg->arg.atom)));
			*argv_out = a;
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
				t_atom64 *argv = (t_atom64 *)osc_mem_alloc(m->argc * sizeof(t_atom64));
				int i = 0;
				while(osc_message_incrementArg(m)){
					switch(*(m->typetags)){
					case 'i':
						atom64_setlong(argv + i, (ntohl(*((int32_t *)m->argv))));
						break;
					case 'f':
						atom64_setfloat(argv + i, (double)(*((float *)m->argv)));
						break;
					case 'd':
						atom64_setfloat(argv + i, *((double *)m->argv));
						break;
					default:
						atom64_setfloat(argv + i, 0.0);
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

int omax_expr_call(t_omax_expr *f, int len, char *oscbndl, int *argc_out, t_atom64 **argv_out){
	int f_argc = f->argc;
	t_omax_expr_arg *f_argv = f->argv;
	int argc[f_argc];
	memset(argc, '\0', sizeof(f_argc));
	t_atom64 *argv[f_argc];
	memset(argv, '\0', sizeof(argv));
	uint32_t min_argc = ~0, max_argc = 0;
	int i = 0;
	while(f_argv){
		int ret = omax_expr_getArg(f_argv, len, oscbndl, argc + i, argv + i);
		if(ret){
			int j;
			for(j = 0; j < i; j++){
				if(argv[j]){
					osc_mem_free(argv[j]);
				}
			}
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
	int ret = f->rec->func(f, f_argc, argc, argv, argc_out, argv_out);
	for(i = 0; i < f_argc; i++){
		if(argv[i]){
			osc_mem_free(argv[i]);
		}
	}
	return ret;
}

t_omax_expr_rec *omax_expr_lookupFunction(char *name){
	t_omax_expr_rec *rec = NULL;
	int i;
	for(i = 0; i < sizeof(omax_expr_funcsym) / sizeof(t_omax_expr_rec); i++){
		if(!strcmp(name, omax_expr_funcsym[i].name)){
			rec = omax_expr_funcsym + i;
			break;
		}
	}
	return rec;
}

int omax_expr_1arg_dbl(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = *argc;
	t_atom64 *result = (t_atom64 *)osc_mem_alloc(*argc * sizeof(t_atom64));
	double (*func)(double) = (double (*)(double))(f->rec->extra);
	int i;
	for(i = 0; i < *argc; i++){
		atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i)));
	}
	*argv_out = result;
	return 0;
}

int omax_expr_1arg_dblptr(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
}

int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	uint32_t min_argc = argc[0], max_argc = argc[1];
	if(argc[0] > argc[1]){
		min_argc = argc[1], max_argc = argc[0];
	}
	int i;
	double (*func)(double,double) = (double (*)(double,double))(f->rec->extra);
	if(argc[0] == 1){
		*argc_out = max_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		memset(result, '\0', max_argc * sizeof(t_atom64));
		for(i = 0; i < max_argc; i++){
			atom64_setfloat(result + i, func(atom64_getfloat(argv[0]), atom64_getfloat(argv[1] + i)));
		}
		*argv_out = result;
		return 0;
	}else if(argc[1] == 1){
		*argc_out = max_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		memset(result, '\0', max_argc * sizeof(t_atom64));
		for(i = 0; i < max_argc; i++){
			atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1])));
		}
		*argv_out = result;
		return 0;
	}else{
		*argc_out = min_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * min_argc);
		memset(result, '\0', min_argc * sizeof(t_atom64));
		for(i = 0; i < min_argc; i++){
			atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1] + i)));
		}
		*argv_out = result;
		return 0;
	}
}

int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
}

int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
}

int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
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

int omax_expr_get_index(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	int i, j, k = 0;
	*argc_out = 0;
	for(i = 1; i < argcc; i++){
		(*argc_out) += argc[i];
	}
	t_atom64 *result = (t_atom64 *)osc_mem_alloc(*argc_out * sizeof(t_atom64));
	for(j = 1; j < argcc; j++){
		for(i = 0; i < argc[j]; i++){
			atom64_setfloat(result + k, 0.);
			if(atom64_getlong(argv[j] + i) > argc[0] - 1){
				error("index %d exceeds array length (%d)", atom64_getlong(argv[j] + i), (int)argc[0]);
				return 1;
			}
			atom64_setfloat(result + k++, atom64_getfloat(argv[0] + (atom64_getlong(argv[j] + i))));
		}
	}
	*argv_out = result;
	return 0;
}

int omax_expr_sum(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = 1;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	atom64_setfloat(*argv_out, 0.);
	int i;
	double val = 0;
	for(i = 0; i < *argc; i++){
		val += atom64_getfloat(argv[0] + i);
	}
	atom64_setfloat(*argv_out, val);
	return 0;
}

int omax_expr_length(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = 1;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	atom64_setlong(*argv_out, (int64_t)(*argc));
	return 0;
}

int omax_expr_mean(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = 1;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	double sum = 0;
	int i;
	for(i = 0; i < *argc; i++){
		sum += atom64_getfloat(argv[0] + i);
	}
	atom64_setfloat(*argv_out, sum / *argc);
	return 0;
}

int omax_expr_concat(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	int i, j = 0, k;
	for(i = 0; i < argcc; i++){
		*argc_out += argc[i];
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(*argc_out * sizeof(t_atom64));
	for(i = 0; i < argcc; i++){
		for(k = 0; k < argc[i]; k++){
			atom64_setfloat((*argv_out) + j++, atom64_getfloat(argv[i] + k));
		}
	}
	return 0;
}

int omax_expr_reverse(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = *argc;
	t_atom64 *result = (t_atom64 *)osc_mem_alloc(*argc * sizeof(t_atom64));
	int i;
	for(i = 0; i < *argc; i++){
		result[i] = argv[0][argc[0] - i - 1];
	}
	*argv_out = result;
	return 0;
}

int omax_expr_make_list(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	int n = atom64_getlong(*argv);
	double val = 0.;
	t_atom64 *result = (t_atom64 *)osc_mem_alloc(n * sizeof(t_atom64));
	if(argcc == 2){
		val = atom64_getfloat(argv[1]);
	}
	printf("val = %f\n", val);
	int i;
	for(i = 0; i < n; i++){
		atom64_setfloat(result + i, val);
 	}
	*argc_out = n;
	*argv_out = result;
	return 0;
}

int omax_expr_not(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	int i;
	*argc_out = *argc;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	for(i = 0; i < *argc; i++){
		switch(argv[0][i].type){
		case A64_LONG:
			atom64_setlong(argv_out[0] + i, !(atom64_getlong(argv[0] + i)));
			break;
		case A64_FLOAT:
			atom64_setlong(argv_out[0] + i, !((int)(atom64_getfloat(argv[0] + i))));
			break;
		case A64_SYM:
			argv_out[0][i] = argv[0][i];
			break;
		
		}
	}
	return 0;
}

t_omax_expr_arg *omax_expr_arg_alloc(void){
	t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
	memset(arg, '\0', sizeof(t_omax_expr_arg));
	return arg;
}

void omax_expr_free(t_omax_expr *e){
	if(!e){
		return;
	}
	omax_expr_arg_free(e->argv);
	osc_mem_free(e);
}

void omax_expr_arg_free(t_omax_expr_arg *a){
	if(!a){
		return;
	}
	omax_expr_arg_free(a->next);
	if(a->type == OMAX_ARG_TYPE_EXPR){
		omax_expr_free(a->arg.expr);
	}
	osc_mem_free(a);
}
