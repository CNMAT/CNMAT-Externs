#include <Carbon.h>
#include <CoreServices.h>
#include "omax_expr.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"

int omax_expr_funcall(t_omax_expr *f, int *argc_out, double **argv_out){
	printf("%s: f = %p\n", __func__, f);
	t_omax_expr_arg *av = f->argv;
	/*
	while(av){
		printf("%p: type = %d\n", av, av->type);
		av = av->next;
 	}
	*/
	f->f(f, argc_out, argv_out);
	printf("DONE: argc_out = %d, argv_out[0] = %f\n", *argc_out, *argv_out[0]);
}

int omax_expr_getArg(t_omax_expr_arg *arg, int *argc_out, double **argv_out){
	printf("%s: type = %d\n", __func__, arg->type);
	switch(arg->type){
	case OMAX_ARG_TYPE_ATOM:
		{
			printf("ATOM\n");
			double *f = (double *)osc_mem_alloc(sizeof(double));
			*f = atom_getfloat(&(arg->arg.atom));
			*argv_out = f;
			*argc_out = 1;
		}
		break;
	case OMAX_ARG_TYPE_EXPR:
		{
			printf("EXPR\n");
			int argc = 0;
			double *argv = NULL;
			printf("arg->arg.expr: %p\n", arg->arg.expr);
			omax_expr_funcall(arg->arg.expr, argc_out, argv_out);
			printf("EXPR: argc = %d\n", *argc_out);
			//*argv_out = argv;
			//*argc_out = argc;
		}
		break;
	case OMAX_ARG_TYPE_OSCADDRESS:
		{

		}
		break;
	}
}
 
int omax_expr_plus(t_omax_expr *f, int *argc_out, double **argv_out){
	printf("%s\n", __func__);
	int f_argc = f->argc;
	t_omax_expr_arg *f_argv = f->argv;
	int argc[] = {0, 0};
	double *argv[] = {NULL, NULL};
	int i = 0;
	while(f_argv){
		printf("%s: i = %d\n", __func__, i);
		omax_expr_getArg(f_argv, argc + i, argv + i);
		f_argv = f_argv->next;
		i++;
	}
	*argc_out = 1;
	double *d = (double *)osc_mem_alloc(sizeof(double));
	*d = argv[0][0] + argv[1][0];
	*argv_out = d;
	for(i = 0; i < 2; i++){
		printf("argc[%d] = %d\n", i, argc[i]);
		int j;
		for(j = 0; j < argc[i]; j++){
			printf("argv[%d] = %f\n", j, argv[i][j]);
		}
	}
	
	//double arg1 = atom_getfloat(&(argv->arg.atom));
	//argv = argv->next;
	//double arg2 = argc == 1 ? 0. : atom_getfloat(&(argv)->arg.atom);
	//printf("%s: argc = %d, arg1 = %f, arg2 = %f\n", __func__, argc, arg1, arg2);
	//printf("res = %f\n", arg1 + arg2);
}

t_omax_expr_arg *omax_expr_arg_alloc(void){
	t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
	memset(arg, '\0', sizeof(t_omax_expr_arg));
	return arg;
}
