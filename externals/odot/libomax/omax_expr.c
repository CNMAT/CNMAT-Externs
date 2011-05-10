#include <Carbon.h>
#include <CoreServices.h>
#include "omax_expr.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"

int omax_parser_funcall(t_omax_func *f, int *argc_out, t_atom **argv_out){
	t_omax_func_arg *av = f->argv;
	while(av){
		printf("%p: type = %d\n", av, av->type);
		av = av->next;
 	}
	f->f(f, argc_out, argv_out);
}

int omax_parser_getargs(t_omax_func_arg *arglist, int **argc, double **argv){
	t_omax_func_arg *ptr = arglist;
	int count = 0;
	while(ptr){
		switch(ptr->type){

		}
		ptr++;
 	}
}
 
int omax_parser_plus(t_omax_func *f, int *argc_out, t_atom **argv_out){
	int argc = f->argc;
	t_omax_func_arg *argv = f->argv;
	//double arg1 = atom_getfloat(&(argv->arg.atom));
	//argv = argv->next;
	//double arg2 = argc == 1 ? 0. : atom_getfloat(&(argv)->arg.atom);
	//printf("%s: argc = %d, arg1 = %f, arg2 = %f\n", __func__, argc, arg1, arg2);
	//printf("res = %f\n", arg1 + arg2);
}
