#ifndef WIN_VERSION
#include <Carbon.h>
#include <CoreServices.h>
#endif

#include <float.h>
#include "omax_parser.h"
#include "omax_util.h"
#include "omax_expr.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"

int omax_expr_funcall(t_omax_expr *function, long *len, char **oscbndl, int *argc_out, t_atom64 **argv_out){
	t_omax_expr *f = function;
	int jjj = 0;
	while(f){
		int ret = omax_expr_call(f, len, oscbndl, argc_out, argv_out);
		if(ret){
			return ret;
		}
		if(f->assign_result_to_address){
			t_osc_msg *m = NULL;
			osc_bundle_lookupAddress_s(*len, *oscbndl, f->argv->arg.osc_address, &m, 1);

			t_osc_msg mm;
			osc_message_clear(&mm);
			osc_message_setAddress(&mm, f->argv->arg.osc_address);
			int i;
			for(i = 0; i < argc_out[0]; i++){
				char tt;
				int buflen = 4;
				if(argv_out[0][i].type == A64_SYM){
					buflen = strlen(atom64_getsym(argv_out[0] + i)->s_name);
					buflen++;
					while(buflen % 4){
						buflen++;
					}
				}
				char buf[buflen];
				memset(buf, '\0', buflen);
				switch(argv_out[0][i].type){
				case A64_LONG:
					{
						tt = 'i';
						int32_t l = (int32_t)atom64_getlong(argv_out[0] + i);
						*((uint32_t *)buf) = l;
					}
					break;
				case A64_FLOAT:
					{
						float f = atom64_getfloat(argv_out[0] + i);
						*((uint32_t *)buf) = *((uint32_t *)&f);
						tt = 'f';
					}
					break;
				case A64_SYM:
					{
						strcpy(buf, atom64_getsym(argv_out[0] + i)->s_name);
						tt = 's';
					}
					break;
				default: 
					continue;
				}
				osc_message_addData(&mm, 1, &tt, buflen, buf);
			}
			long slen = osc_message_getSize(&mm);
			char msg_s[slen + 4];
			osc_message_serialize(&mm, msg_s);
			if(m){
				osc_bundle_replaceMessage_s(len, oscbndl, osc_message_getSize(m), m->address - 4, slen, msg_s);
			}else{
				osc_bundle_addSerializedMessage_s(len, oscbndl, slen, msg_s);
			}
			osc_message_free_internal_buffers(&mm);
		}
		f = f->next;
	}
	return 0;
}

int omax_expr_getArg(t_omax_expr_arg *arg, long *len, char **oscbndl, int *argc_out, t_atom64 **argv_out){
	switch(arg->type){
	case OMAX_ARG_TYPE_ATOM:
		{
			t_atom64 *a = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
			switch(atom_gettype(&(arg->arg.atom))){
			case A_LONG:
				atom64_setlong(a, atom_getlong(&(arg->arg.atom)));
				break;
			case A_FLOAT:
				atom64_setfloat(a, atom_getfloat(&(arg->arg.atom)));
				break;
			case A_SYM:
				atom64_setsym(a, atom_getsym(&(arg->arg.atom)));
				break;
			}
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
			osc_bundle_lookupAddress_s(*len, *oscbndl, arg->arg.osc_address, &m, 1);
			if(m){
				*argc_out = m->argc;
				t_atom64 *argv = (t_atom64 *)osc_mem_alloc(m->argc * sizeof(t_atom64));
				int i = 0;
				while(osc_message_incrementArg(m)){
					switch(*(m->typetags)){
					case 'i':
						atom64_setlong(argv + i, (ntoh32(*((int32_t *)m->argv))));
						break;
					case 'f':
						{
							uint32_t ii = ntoh32(*((uint32_t *)m->argv));
							float f = *((float *)&ii);
							atom64_setfloat(argv + i, (double)f);
							//atom64_setfloat(argv + i, (double)(*((float *)m->argv)));
						}
						break;
					case 'd':
						atom64_setfloat(argv + i, *((double *)m->argv));
						break;
					case 's':
						atom64_setsym(argv + i, gensym(m->argv));
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
				return 0;
			}
		}
	}
	return 1; // this really shouldn't happen unless there's a bug somewhere
}

int omax_expr_call(t_omax_expr *f, long *len, char **oscbndl, int *argc_out, t_atom64 **argv_out){
	int f_argc = f->argc;
	t_omax_expr_arg *f_argv = f->argv;
	int argc[f_argc];
	memset(argc, '\0', sizeof(argc));
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

int omax_expr_2arg(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	uint32_t min_argc = argc[0], max_argc = argc[1];
	if(argc[0] > argc[1]){
		min_argc = argc[1], max_argc = argc[0];
	}
	int i;
	t_atom64 (*func)(t_atom64*,t_atom64*) = (t_atom64 (*)(t_atom64*,t_atom64*))(f->rec->extra);
	if(argc[0] == 1){
		*argc_out = max_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		memset(result, '\0', max_argc * sizeof(t_atom64));
		for(i = 0; i < max_argc; i++){
			result[i] = func(*argv, argv[1] + i);
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0]), atom64_getfloat(argv[1] + i)));
		}
		*argv_out = result;
		return 0;
	}else if(argc[1] == 1){
		*argc_out = max_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		memset(result, '\0', max_argc * sizeof(t_atom64));
		for(i = 0; i < max_argc; i++){
			result[i] = func(argv[0] + i, argv[1]);
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1])));
		}
		*argv_out = result;
		return 0;
	}else{
		*argc_out = min_argc;
		t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * min_argc);
		memset(result, '\0', min_argc * sizeof(t_atom64));
		for(i = 0; i < min_argc; i++){
			result[i] = func(argv[0] + i, argv[1] + i);
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1] + i)));
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
t_atom64 omax_expr_add(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1){
		a = *f2;
		return a;
	}
	if(!f2){
		a = *f1;
		return a;
	}
	if(f1->type == A64_SYM && f2->type == A64_SYM){
		t_symbol *sym1, *sym2;
		sym1 = atom64_getsym(f1);
		sym2 = atom64_getsym(f2);
		char buf[strlen(sym1->s_name) + strlen(sym2->s_name) + 1];
		sprintf(buf, "%s%s", sym1->s_name, sym2->s_name);
		atom64_setsym(&a, gensym(buf));
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else if(f1->type == A64_FLOAT || f2->type == A64_FLOAT){
		atom64_setfloat(&a, atom64_getfloat(f1) + atom64_getfloat(f2));
	}else{
		atom64_setlong(&a, atom64_getlong(f1) + atom64_getlong(f2));
	}
	return a;
}

t_atom64 omax_expr_subtract(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1){
		a = *f2;
		return a;
	}
	if(!f2){
		a = *f1;
		return a;
	}
	if(f1->type == A64_FLOAT || f2->type == A64_FLOAT){
		atom64_setfloat(&a, atom64_getfloat(f1) - atom64_getfloat(f2));
	}else{
		atom64_setlong(&a, atom64_getlong(f1) - atom64_getlong(f2));
	}
	return a;
}

t_atom64 omax_expr_multiply(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1){
		a = *f2;
		return a;
	}
	if(!f2){
		a = *f1;
		return a;
	}
	if(f1->type == A64_FLOAT || f2->type == A64_FLOAT){
		atom64_setfloat(&a, atom64_getfloat(f1) * atom64_getfloat(f2));
	}else{
		atom64_setlong(&a, atom64_getlong(f1) * atom64_getlong(f2));
	}
	return a;
}

t_atom64 omax_expr_divide(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1){
		a = *f2;
		return a;
	}
	if(!f2){
		a = *f1;
		return a;
	}
	if(f1->type == A64_FLOAT || f2->type == A64_FLOAT){
		atom64_setfloat(&a, atom64_getfloat(f1) / atom64_getfloat(f2));
	}else{
		atom64_setlong(&a, atom64_getlong(f1) / atom64_getlong(f2));
	}
	return a;
}

t_atom64 omax_expr_lt(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret < 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) < atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_lte(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret <= 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) <= atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_gt(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret > 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) > atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_gte(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret >= 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) >= atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_eq(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret == 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) == atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_neq(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 1);
		return a;
	}
	if((f1->type == A64_SYM) && (f2->type == A64_SYM)){
		int ret = strcmp(atom64_getsym(f1)->s_name, atom64_getsym(f2)->s_name);
		atom64_setlong(&a, ret != 0);
	}else if((f1->type == A64_SYM) || (f2->type == A64_SYM)){
		atom64_setlong(&a, 0);
	}else{
		atom64_setlong(&a, atom64_getfloat(f1) != atom64_getfloat(f2));
	}
	return a;
}

t_atom64 omax_expr_and(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1 || !f2){
		atom64_setlong(&a, 0);
		return a;
	}
	atom64_setlong(&a, atom64_getfloat(f1) && atom64_getfloat(f2));
	return a;
}

t_atom64 omax_expr_or(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	double ff1 = 0;
	double ff2 = 0;
	if(f1){
		ff1 = atom64_getfloat(f1);
	}
	if(f2){
		ff2 = atom64_getfloat(f2);
	}
	atom64_setlong(&a, ff1 || ff2);
	return a;
}

t_atom64 omax_expr_mod(t_atom64 *f1, t_atom64 *f2){
	t_atom64 a;
	if(!f1){
		atom64_setfloat(&a, 0);
		return a;
	}
	if(!f2){
		a = *f1;
		return a;
	}
	long ff1 = atom64_getlong(f1), ff2 = atom64_getlong(f2);
	if(ff2 == 0){
		a = *f1;
		return a;
	}
	atom64_setlong(&a, ff1 % ff2);
	return a;
}

int omax_expr_assign(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = argc[1];
	*argv_out = (t_atom64 *)osc_mem_alloc(argc[1] * sizeof(t_atom64));
	memcpy(*argv_out, argv[1], argc[1] * sizeof(t_atom64));
	return 0;
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

int omax_expr_cumsum(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = *argc;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * *argc);
	int i;
	double val = 0;
	for(i = 0; i < *argc; i++){
		val += atom64_getfloat(argv[0] + i);
		atom64_setfloat(argv_out[0] + i, val);
	}
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

// this implementation of the median is not great--it just sorts 
// the list and takes the median.  this could be optimized by 
// implementing a median of medians algorithm or something
int comp(const void *val1, const void *val2){
	double v1 = atom64_getfloat((t_atom64 *)val1);
	double v2 = atom64_getfloat((t_atom64 *)val2);
	if(v1 < v2){
		return -1;
	}
	if(v1 == v2){
		return 0;
	}
	return 1;
}

int omax_expr_median(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = 1;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	t_atom64 tmp[*argc];
	memcpy(tmp, *argv, *argc * sizeof(t_atom64));
	qsort((void *)(*argv), *argc, sizeof(t_atom64), comp);
	if((*argc % 2) == 0){
		atom64_setfloat(*argv_out, (atom64_getfloat(*argv + (int)((*argc - 1) / 2.)) + atom64_getfloat(*argv + ((int)((*argc - 1) / 2.) + 1))) / 2.);
	}else{
		atom64_setfloat(*argv_out, atom64_getfloat(*argv + ((*argc - 1) / 2)));
	}
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
	int i;
	for(i = 0; i < n; i++){
		atom64_setfloat(result + i, val);
 	}
	*argc_out = n;
	*argv_out = result;
	return 0;
}

int omax_expr_range(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	double start, end, step = 1;
	if(argcc < 2){
		error("omax_expr: range requires at least 2 arguments:  start and end.");
		return 1;
	}
	start = atom64_getfloat(*argv);
	end = atom64_getfloat(argv[1]);
	if(argcc > 2){
		step = atom64_getfloat(argv[2]);
 	}
	int n = (end - start) / step;
	int i;
        t_atom64 *result = (t_atom64 *)osc_mem_alloc(n * sizeof(t_atom64));
	*argc_out = n;
	for(i = 0; i < n; i++){
		atom64_setfloat(result + i, start);
		start += step;
	}
	*argv_out = result;
	return 0;
}

int omax_expr_multiplex(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	int min = *argc;
	int i, j, k = 0;
	for(i = 1; i < argcc; i++){
		int val = argc[i];
		if(val < min){
			min = val;
		}
	}
	t_atom64 *result = (t_atom64 *)osc_mem_alloc(min * argcc * sizeof(t_atom64));
	for(i = 0; i < argcc; i++){
		for(j = 0; j < min; j++){
			result[i + (j * argcc)] = argv[i][j];
		}
	}
	*argc_out = min * argcc;
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

int omax_expr_first(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	*argc_out = 1;
	**argv_out = **argv;
	return 0;
}

int omax_expr_dot(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc < 2){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	*argc_out = 1;
	double s = 0;
	int i;
	for(i = 0; i < (argc[0] < argc[1] ? argc[0] : argc[1]); i++){
		double f1 = 0, f2 = 0;
		if(i < argc[0]){
			f1 = atom64_getfloat(argv[0] + i);
		}
		if(i < argc[1]){
			f2 = atom64_getfloat(argv[1] + i);
		}
		s += f1 * f2;
	}
	atom64_setfloat(*argv_out, s);
	return 0;
}

int omax_expr_l2norm(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc < 0){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	*argc_out = 1;
	double s = 0;
	int i;
	for(i = 0; i < argc[0]; i++){
		s += pow(atom64_getfloat(argv[0] + i), 2.);
	}
	atom64_setfloat(*argv_out, sqrt(s));
	return 0;
}

int omax_expr_min(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	*argc_out = 1;
	int i;
	double min = DBL_MAX;
	int type = A64_FLOAT;
	for(i = 0; i < argc[0]; i++){
		double f = atom64_getfloat(argv[0] + i);
		if(f < min){
			min = f;
			type = argv[0][i].type;
		}
	}
	switch(type){
	case A64_FLOAT:
		atom64_setfloat(*argv_out, min);
		break;
	case A64_LONG:
		atom64_setlong(*argv_out, (int64_t)min);
		break;
	}
	return 0;
}

int omax_expr_max(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64));
	*argc_out = 1;
	int i;
	double max = -DBL_MAX;
	int type = A64_FLOAT;
	for(i = 0; i < argc[0]; i++){
		double f = atom64_getfloat(argv[0] + i);
		if(f > max){
			max = f;
			type = argv[0][i].type;
		}
	}
	switch(type){
	case A64_FLOAT:
		atom64_setfloat(*argv_out, max);
		break;
	case A64_LONG:
		atom64_setlong(*argv_out, (int64_t)max);
		break;
	}
	return 0;
}

int omax_expr_extrema(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 0;
	}
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * 2);
	*argc_out = 2;
	int i;
	double min = DBL_MAX, max = -DBL_MAX;
	int min_type = A64_FLOAT, max_type = A64_FLOAT;
	for(i = 0; i < argc[0]; i++){
		double f = atom64_getfloat(argv[0] + i);
		if(f > max){
			max = f;
			max_type = argv[0][i].type;
		}
		if(f < min){
			min = f;
			min_type = argv[0][i].type;
		}
	}

	switch(min_type){
	case A64_FLOAT:
		atom64_setfloat(*argv_out, min);
		break;
	case A64_LONG:
		atom64_setlong(*argv_out, (int64_t)min);
		break;
	}

	switch(max_type){
	case A64_FLOAT:
		atom64_setfloat((*argv_out) + 1, max);
		break;
	case A64_LONG:
		atom64_setlong((*argv_out) + 1, (int64_t)max);
		break;
	}
	return 0;
}

int omax_expr_clip(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc != 3){
		return 1;
	}
	double min = atom64_getfloat(argv[1]);
	double max = atom64_getfloat(argv[2]);
	t_atom64 *tmp = (t_atom64 *)osc_mem_alloc(argc[0]);
	int i;
	for(i = 0; i < argc[0]; i++){
		if(argv[0][i].type == A64_SYM){
			tmp[i] = argv[0][i];
			continue;
		}
		double val = atom64_getfloat(argv[0] + i);
		if(val < min){
			val = min;
		}
		if(val > max){
			val = max;
		}
		if(argv[0][i].type == A64_FLOAT){
			atom64_setfloat(tmp + i, val);
		}else{
			atom64_setlong(tmp + i, (int64_t)val);
		}
	}
	*argc_out = *argc;
	*argv_out = tmp;
	return 0;
}

int omax_expr_scale(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc != 5){
		return 1;
	}
	double min_in = atom64_getfloat(argv[1]);
	double max_in = atom64_getfloat(argv[2]);
	double min_out = atom64_getfloat(argv[3]);
	double max_out = atom64_getfloat(argv[4]);
	double m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	t_atom64 *tmp = (t_atom64 *)osc_mem_alloc(argc[0]);
	int i;
	for(i = 0; i < argc[0]; i++){
		if(argv[0][i].type == A64_SYM){
			tmp[i] = argv[0][i];
			continue;
		}
		double val = atom64_getfloat(argv[0] + i);
		val = m * val + b;
		if(argv[0][i].type == A64_FLOAT){
			atom64_setfloat(tmp + i, val);
		}else{
			atom64_setlong(tmp + i, (int64_t)val);
		}
	}
	*argc_out = *argc;
	*argv_out = tmp;
	return 0;
}

int omax_expr_mtof(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 1;
	}
	double base = 440;
	if(argcc > 1){
		if(argc[1] > 0){
			base = atom64_getfloat(argv[1]);
		}
	}
	t_atom64 *tmp = (t_atom64 *)osc_mem_alloc(argc[0]);
	int i;
	for(i = 0; i < argc[0]; i++){
		if(argv[0][i].type == A64_SYM){
			tmp[i] = argv[0][i];
			continue;
		}
		double val = atom64_getfloat(argv[0] + i);
		// l(2) * (1. / 12.)
		val = base * exp(.05776226504666210911810267678817 * (val - 69));
		atom64_setfloat(tmp + i, val);
	}
	*argc_out = *argc;
	*argv_out = tmp;
	return 0;
}

int omax_expr_ftom(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	if(argcc == 0){
		return 1;
	}
	double base = 440;
	if(argcc > 1){
		if(argc[1] > 0){
			base = atom64_getfloat(argv[1]);
		}
	}
	t_atom64 *tmp = (t_atom64 *)osc_mem_alloc(argc[0]);
	int i;
	for(i = 0; i < argc[0]; i++){
		if(argv[0][i].type == A64_SYM){
			tmp[i] = argv[0][i];
			continue;
		}
		double val = atom64_getfloat(argv[0] + i);
		// 1. / (l(2) * (1. / 12))
		val = (69 + (17.31234049066756088831909617202611 * log(val / base)));
		atom64_setfloat(tmp + i, val);
	}
	*argc_out = *argc;
	*argv_out = tmp;
	return 0;
}

int omax_expr_rand(t_omax_expr *f, int argcc, int *argc, t_atom64 **argv, int *argc_out, t_atom64 **argv_out){
	*argc_out = 1;
	*argv_out = (t_atom64 *)osc_mem_alloc(sizeof(t_atom));
	atom64_setfloat(*argv_out, (double)rand() / (double)RAND_MAX);
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

void omax_expr_parse(t_omax_expr **function_graph, long argclex, t_atom *argvlex){
	int counter = 0;
	t_omax_expr *f = NULL;
	yyparse(function_graph, &f, argclex, argvlex, &counter);
}
