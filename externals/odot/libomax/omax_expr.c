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
#include "osc_bundle_s.h"
#include "osc_message_s.h"
#include "osc_message_u.h"
#include "osc_message_iterator_s.h"
#include "osc_atom_u.h"

int omax_expr_funcall(t_omax_expr *function, long *len, char **oscbndl, t_osc_atom_ar_u **out){
	t_omax_expr *f = function;
	int jjj = 0;
	while(f){
		int ret = omax_expr_call(f, len, oscbndl, out);
		if(ret){
			return ret;
		}
		if(f->assign_result_to_address){
			t_osc_msg_ar_s *msg_ar = NULL;
			osc_bundle_s_lookupAddress(*len, *oscbndl, f->argv->arg.osc_address, &msg_ar, 1);

			t_osc_msg_u *mm = osc_message_u_alloc();
			osc_message_u_setAddress(mm, f->argv->arg.osc_address);
			int i;
			for(i = 0; i < osc_atom_array_u_getLen(*out); i++){
				t_osc_atom_u *cpy = NULL;
				osc_atom_u_copy(&cpy, osc_atom_array_u_get(*out, i));
				osc_message_u_appendAtom(mm, cpy);
			}
			char *msg_s = NULL;
			long len_s = 0;
			osc_message_u_serialize(mm, &len_s, &msg_s);
			if(msg_ar){
				osc_bundle_s_replaceMessage(len, oscbndl, osc_message_s_getPtr(osc_message_array_s_get(msg_ar, 0)), msg_s);
				osc_message_array_s_free(msg_ar);
			}else{
				osc_bundle_s_appendMessage(len, oscbndl, msg_s);
			}
			osc_message_u_free(mm);
			osc_mem_free(msg_s);
		}
		f = f->next;
	}
	return 0;
}

int omax_expr_getArg(t_omax_expr_arg *arg, long *len, char **oscbndl, t_osc_atom_ar_u **out){
	switch(arg->type){
	case OMAX_ARG_TYPE_ATOM:
		{
			t_osc_atom_ar_u *ar = osc_atom_array_u_alloc(1);
			t_osc_atom_u *a = osc_atom_array_u_get(ar, 0);
			switch(atom_gettype(&(arg->arg.atom))){
			case A_LONG:
				osc_atom_u_setInt32(a, atom_getlong(&(arg->arg.atom)));
				break;
			case A_FLOAT:
				osc_atom_u_setFloat(a, atom_getfloat(&(arg->arg.atom)));
				break;
			case A_SYM:
				{
					char *symptr = atom_getsym(&(arg->arg.atom))->s_name;
					int len = strlen(symptr);
				        switch(len){
					case 3:
						if(!strncmp(symptr, "NIL", 3) ||
						   !strncmp(symptr, "Nil", 3) ||
						   !strncmp(symptr, "nil", 3)){
							osc_atom_u_setNull(a);
							break;
						}
					case 4:
						if(!strncmp(symptr, "true", 4) || 
						   !strncmp(symptr, "True", 4) || 
						   !strncmp(symptr, "TRUE", 4)){
							osc_atom_u_setTrue(a);
							break;
						}else if(!strncmp(symptr, "NULL", 4) ||
							 !strncmp(symptr, "Null", 4) ||
							 !strncmp(symptr, "null", 4)){
							osc_atom_u_setNull(a);
						        break;
						}
					case 5:
						if(!strncmp(symptr, "false", 5) || 
						   !strncmp(symptr, "False", 5) || 
						   !strncmp(symptr, "FALSE", 5)){
							osc_atom_u_setFalse(a);
							break;
						}
						break;
					default:
						osc_atom_u_setString(a, atom_getsym(&(arg->arg.atom))->s_name);
					}
				}
				break;
			}
			*out = ar;
			return 0;
		}
	case OMAX_ARG_TYPE_EXPR:
		{
			return omax_expr_funcall(arg->arg.expr, len, oscbndl, out);
		}
	case OMAX_ARG_TYPE_OSCADDRESS:
		{
			//t_osc_msg *m = NULL;
			t_osc_msg_ar_s *msg_ar = NULL;
			osc_bundle_s_lookupAddress(*len, *oscbndl, arg->arg.osc_address, &msg_ar, 1);
			int i;
			if(msg_ar){
				t_osc_msg_s *m = osc_message_array_s_get(msg_ar, 0);
				long arg_count = osc_message_s_getArgCount(m);
				t_osc_atom_ar_u *atom_ar = osc_atom_array_u_alloc(arg_count);
				//*argc_out = m->argc;
				//t_atom64 *argv = (t_atom64 *)osc_mem_alloc(m->argc * sizeof(t_atom64));
				int i = 0;
				t_osc_msg_it_s *it = osc_msg_it_s_get(m);
				while(osc_msg_it_s_hasNext(it)){
					t_osc_atom_s *as = osc_msg_it_s_next(it);
					t_osc_atom_u *au = osc_atom_array_u_get(atom_ar, i);
					osc_atom_s_deserialize(as, &au);
					i++;
				}
				osc_msg_it_s_destroy(it);
				osc_message_array_s_free(msg_ar);
				*out = atom_ar;
				return 0;
			}else{
				*out = NULL;
				return 0;
			}
		}
	}
	return 1; // this really shouldn't happen unless there's a bug somewhere
}

int omax_expr_call(t_omax_expr *f, long *len, char **oscbndl, t_osc_atom_ar_u **out){
	int f_argc = f->argc;
	t_omax_expr_arg *f_argv = f->argv;
	int i = 0;
	int ret = 0;

	if(f->rec->func == omax_expr_if){
		if(f_argc < 2){
			error("omax_expr: omax_expr_if expected at least 2 arguments but only got %d\n", f_argc);
			return 1;
		}
		t_osc_atom_ar_u *argv = NULL;
		ret = omax_expr_getArg(f_argv, len, oscbndl, &argv);
		if(ret){
			if(argv){
				osc_atom_array_u_free(argv);
			}
			return ret;
		}
		t_osc_atom_ar_u *argv_out = osc_atom_array_u_alloc(osc_atom_array_u_getLen(argv));
		t_osc_atom_ar_u *arg_true = NULL, *arg_false = NULL;
		omax_expr_getArg(f_argv->next, len, oscbndl, &arg_true);
		omax_expr_getArg(f_argv->next->next, len, oscbndl, &arg_false);
		int j;
		for(j = 0; j < osc_atom_array_u_getLen(argv); j++){
			if(osc_atom_u_getInt32(osc_atom_array_u_get(argv, j)) && (f_argc > 1)){
				t_osc_atom_u *a = osc_atom_array_u_get(argv_out, j);
				osc_atom_u_copy(&a, osc_atom_array_u_get(arg_true, 0));
			}else if(f_argc > 2){
				t_osc_atom_u *a = osc_atom_array_u_get(argv_out, j);
				osc_atom_u_copy(&a, osc_atom_array_u_get(arg_false, 0));
			}
		}
	}else if(f->rec->func == omax_expr_defined){
		*out = osc_atom_array_u_alloc(1);
		t_osc_msg_ar_s *m = NULL;
		osc_bundle_s_lookupAddress(*len, *oscbndl, f_argv->arg.osc_address, &m, 1);
		if(m){
			osc_atom_u_setTrue(osc_atom_array_u_get(*out, 0));
		}else{
			osc_atom_u_setFalse(osc_atom_array_u_get(*out, 0));
		}
	}else{
		t_osc_atom_ar_u *argv[f_argc];
		memset(argv, '\0', sizeof(argv));
		while(f_argv){
			int ret = omax_expr_getArg(f_argv, len, oscbndl, argv + i);
			if(ret){
				int j;
				for(j = 0; j < i; j++){
					if(argv[j]){
						osc_atom_array_u_free(argv[j]);
					}
				}
				return ret;
			}
			f_argv = f_argv->next;
			i++;
		}
		ret = f->rec->func(f, f_argc, argv, out);
		for(i = 0; i < f_argc; i++){
			if(argv[i]){
				osc_atom_array_u_free(argv[i]);
			}
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

int omax_expr_1arg_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	//*argc_out = *argc;
	//t_atom64 *result = (t_atom64 *)osc_mem_alloc(*argc * sizeof(t_atom64));
	long ac = osc_atom_array_u_getLen(*argv);
	if(argc == 0){
		return 0;
	}
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(ac);
	double (*func)(double) = (double (*)(double))(f->rec->extra);
	int i;
	for(i = 0; i < ac; i++){
		//atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i)));
		osc_atom_u_setDouble(osc_atom_array_u_get(result, i), func(osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i))));
	}
	*out = result;
	return 0;
}

int omax_expr_1arg_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
}

int omax_expr_2arg_dbl_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	//uint32_t min_argc = argc[0], max_argc = argc[1];
	uint32_t argc0 = osc_atom_array_u_getLen(argv[0]);
	uint32_t argc1 = osc_atom_array_u_getLen(argv[1]);
	uint32_t min_argc = argc0, max_argc = argc1;
	if(argc0 > argc1){
		min_argc = argc1, max_argc = argc0;
	}
	int i;
	double (*func)(double,double) = (double (*)(double,double))(f->rec->extra);
	if(argc0 == 1){
		//*argc_out = max_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		*out = osc_atom_array_u_alloc(max_argc);
		osc_atom_array_u_clear(*out);
		//memset(result, '\0', max_argc * sizeof(t_atom64));
		for(i = 0; i < max_argc; i++){
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0]), atom64_getfloat(argv[1] + i)));
			osc_atom_u_setDouble(osc_atom_array_u_get(*out, i), 
					     func(osc_atom_u_getDouble(osc_atom_array_u_get(argv[0], 0)), 
						  osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], i))));
		}
		return 0;
	}else if(argc1 == 1){
		//*argc_out = max_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		//memset(result, '\0', max_argc * sizeof(t_atom64));
		*out = osc_atom_array_u_alloc(max_argc);
		osc_atom_array_u_clear(*out);
		for(i = 0; i < max_argc; i++){
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1])));
			osc_atom_u_setDouble(osc_atom_array_u_get(*out, i), 
					     func(osc_atom_u_getDouble(osc_atom_array_u_get(argv[0], i)), 
						  osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0))));
		}
		return 0;
	}else{
		//*argc_out = min_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * min_argc);
		//memset(result, '\0', min_argc * sizeof(t_atom64));
		*out = osc_atom_array_u_alloc(min_argc);
		osc_atom_array_u_clear(*out);
		for(i = 0; i < min_argc; i++){
			//atom64_setfloat(result + i, func(atom64_getfloat(argv[0] + i), atom64_getfloat(argv[1] + i)));
			osc_atom_u_setDouble(osc_atom_array_u_get(*out, i), 
					     func(osc_atom_u_getDouble(osc_atom_array_u_get(argv[0], i)), 
						  osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], i))));
		}
		//*argv_out = result;
		return 0;
	}
}

int omax_expr_2arg(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	uint32_t argc0 = osc_atom_array_u_getLen(argv[0]);
	uint32_t argc1 = osc_atom_array_u_getLen(argv[1]);
	uint32_t min_argc = argc0, max_argc = argc1;
	if(argc0 > argc1){
		min_argc = argc1, max_argc = argc0;
	}
	int i;
	void  (*func)(t_osc_atom_u*,t_osc_atom_u*,t_osc_atom_u**) = (void (*)(t_osc_atom_u*,t_osc_atom_u*,t_osc_atom_u**))(f->rec->extra);
	if(argc0 == 1){
		//*argc_out = max_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		//memset(result, '\0', max_argc * sizeof(t_atom64));
		*out = osc_atom_array_u_alloc(max_argc);
		osc_atom_array_u_clear(*out);
		for(i = 0; i < max_argc; i++){
			t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
			func(osc_atom_array_u_get(argv[0], 0), osc_atom_array_u_get(argv[1], i), &a);
					     //result[i] = func(*argv, argv[1] + i);
		}
		return 0;
	}else if(argc1 == 1){
		//*argc_out = max_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * max_argc);
		//memset(result, '\0', max_argc * sizeof(t_atom64));
		*out = osc_atom_array_u_alloc(max_argc);
		osc_atom_array_u_clear(*out);
		for(i = 0; i < max_argc; i++){
			t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
			func(osc_atom_array_u_get(argv[0], i), osc_atom_array_u_get(argv[1], 0), &a);
			//result[i] = func(argv[0] + i, argv[1]);
		}
		return 0;
	}else{
		//*argc_out = min_argc;
		//t_atom64 *result = (t_atom64 *)osc_mem_alloc(sizeof(t_atom64) * min_argc);
		//memset(result, '\0', min_argc * sizeof(t_atom64));
		*out = osc_atom_array_u_alloc(min_argc);
		osc_atom_array_u_clear(*out);
		for(i = 0; i < min_argc; i++){
			t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
			func(osc_atom_array_u_get(argv[0], i), osc_atom_array_u_get(argv[1], i), &a);
			//result[i] = func(argv[0] + i, argv[1] + i);
		}
		//*argv_out = result;
		return 0;
	}
}

int omax_expr_2arg_dblptr_dbl(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
}

int omax_expr_2arg_dbl_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
}

int omax_expr_2arg_dblptr_dblptr(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
}

// wrappers for infix ops
void omax_expr_add(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1){
		osc_atom_u_copy(result, f2);
		return;
	}
	if(!f2){
		osc_atom_u_copy(result, f1);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		char *sp1 = osc_atom_u_getStringPtr(f1);
		char *sp2 = osc_atom_u_getStringPtr(f2);
		char buf[strlen(sp1) + strlen(sp2) + 1];
		sprintf(buf, "%s%s", sp1, sp2);
		osc_atom_u_setString(*result, buf);
	}else if(tt1 == 's' || tt2 == 's'){
		osc_atom_u_setFalse(*result);
	}else if(tt1 == 'f' || tt2 == 'f' || tt1 == 'd' || tt2 == 'd'){
		osc_atom_u_setDouble(*result, osc_atom_u_getDouble(f1) + osc_atom_u_getDouble(f2));
	}else{
		osc_atom_u_setInt32(*result, osc_atom_u_getInt32(f1) + osc_atom_u_getInt32(f2));
	}
	return;
}

void omax_expr_subtract(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1){
		osc_atom_u_copy(result, f2);
		return;
	}
	if(!f2){
		osc_atom_u_copy(result, f1);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 'f' || tt1 == 'd' || tt2 == 'f' || tt2 == 'd'){
		osc_atom_u_setDouble(*result, osc_atom_u_getDouble(f1) - osc_atom_u_getDouble(f2));
	}else{
		osc_atom_u_setInt32(*result, osc_atom_u_getInt32(f1) - osc_atom_u_getInt32(f2));
	}
	return;
}

void omax_expr_multiply(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1){
		osc_atom_u_copy(result, f2);
		return;
	}
	if(!f2){
		osc_atom_u_copy(result, f1);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 'f' || tt1 == 'd' || tt2 == 'f' || tt2 == 'd'){
		osc_atom_u_setDouble(*result, osc_atom_u_getDouble(f1) * osc_atom_u_getDouble(f2));
	}else{
		osc_atom_u_setInt32(*result, osc_atom_u_getInt32(f1) * osc_atom_u_getInt32(f2));
	}
	return;
}

void omax_expr_divide(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1){
		osc_atom_u_copy(result, f2);
		return;
	}
	if(!f2){
		osc_atom_u_copy(result, f1);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 'f' || tt1 == 'd' || tt2 == 'f' || tt2 == 'd'){
		osc_atom_u_setDouble(*result, osc_atom_u_getDouble(f1) / osc_atom_u_getDouble(f2));
	}else{
		osc_atom_u_setInt32(*result, osc_atom_u_getInt32(f1) / osc_atom_u_getInt32(f2));
	}
	return;
}

void omax_expr_lt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret < 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) < osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_lte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret <= 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) <= osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_gt(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret > 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) > osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_gte(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret >= 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) >= osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_eq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret == 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) == osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_neq(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 's' && tt2 == 's'){
		int ret = strcmp(osc_atom_u_getStringPtr(f1), osc_atom_u_getStringPtr(f2));
		osc_atom_u_setBool(*result, ret != 0);
	}else if((tt1 == 's') || (tt2 == 's')){
		osc_atom_u_setFalse(*result);
	}else{
		osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) != osc_atom_u_getDouble(f2));
	}
	return;
}

void omax_expr_and(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1 || !f2){
		osc_atom_u_setFalse(*result);
		return;
	}
	osc_atom_u_setBool(*result, osc_atom_u_getDouble(f1) && osc_atom_u_getDouble(f2));
	return;
}

void omax_expr_or(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	double ff1 = 0;
	double ff2 = 0;
	if(f1){
		ff1 = osc_atom_u_getDouble(f1);
	}
	if(f2){
		ff2 = osc_atom_u_getDouble(f2);
	}
	osc_atom_u_setBool(*result, ff1 || ff2);
	return;
}

void omax_expr_mod(t_osc_atom_u *f1, t_osc_atom_u *f2, t_osc_atom_u **result){
	if(!f1){
		osc_atom_u_setInt32(*result, 0);
		return;
	}
	if(!f2){
		osc_atom_u_copy(result, f1);
		return;
	}
	char tt1 = osc_atom_u_getTypetag(f1), tt2 = osc_atom_u_getTypetag(f2);
	if(tt1 == 'f' || tt1 == 'd' || tt2 == 'f' || tt2 == 'd'){
		double ff1 = osc_atom_u_getDouble(f1), ff2 = osc_atom_u_getDouble(f2);
		if(ff2 == 0){
			osc_atom_u_copy(result, f1);
			return;
		}
		osc_atom_u_setDouble(*result, fmod(ff1, ff2));
	}else{
		int32_t ff1 = osc_atom_u_getInt32(f1), ff2 = osc_atom_u_getInt32(f2);
		if(ff2 == 0){
			osc_atom_u_copy(result, f1);
			return;
		}
		osc_atom_u_setInt32(*result, ff1 % ff2);
	}
	return;
}

int omax_expr_assign(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	//*argc_out = argc[1];
	//*argv_out = (t_atom64 *)osc_mem_alloc(argc[1] * sizeof(t_atom64));
	//memcpy(*argv_out, argv[1], argc[1] * sizeof(t_atom64));
	*out = osc_array_copy(argv[1]);
	return 0;
}

int omax_expr_get_index(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	int i, j, k = 0;
	int argc_out = 0;
	for(i = 1; i < argc; i++){
		argc_out += osc_atom_array_u_getLen(argv[i]);
	}
	//t_atom64 *result = (t_atom64 *)osc_mem_alloc(*argc_out * sizeof(t_atom64));
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(argc_out);
	long argv0len = osc_atom_array_u_getLen(argv[0]);
	for(j = 1; j < argc; j++){
		for(i = 0; i < osc_atom_array_u_getLen(argv[j]); i++){
				//atom64_setfloat(result + k, 0.);
			osc_atom_u_setDouble(osc_atom_array_u_get(result, k), 0.);
			//if(atom64_getlong(argv[j] + i) > argc[0] - 1){
			int32_t l = osc_atom_u_getInt32(osc_atom_array_u_get(argv[j], i));
			if(l > argv0len - 1){
				error("index %d exceeds array length (%d)", l, argv0len);
				return 1;
			}
			//atom64_setfloat(result + k, atom64_getfloat(argv[0] + (atom64_getlong(argv[j] + i))));
			t_osc_atom_u *r = osc_atom_array_u_get(result, k);
			osc_atom_u_copy(&r, osc_atom_array_u_get(argv[0], l));
			k++;
		}
	}
	*out = result;
	return 0;
}

int omax_expr_product(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	long len = osc_atom_array_u_getLen(*argv);
	if(len == 0){
		osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), 0.);
		return 0;
	}
	int i;
	double val = 1;
	for(i = 0; i < len; i++){
		val *= osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
	}
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), val);
	return 0;
}

int omax_expr_sum(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	long len = osc_atom_array_u_getLen(*argv);
	if(len == 0){
		osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), 0.);
		return 0;
	}
	int i;
	double val = 1;
	for(i = 0; i < len; i++){
		val += osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
	}
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), val);
	return 0;
}

int omax_expr_cumsum(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(osc_atom_array_u_getLen(*argv));
	int i;
	double val = 0;
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		val += osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		osc_atom_u_setDouble(osc_atom_array_u_get(*out, i), val);
	}
	return 0;
}

int omax_expr_length(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	osc_atom_u_setInt32(osc_atom_array_u_get(*out, 0), osc_atom_array_u_getLen(*argv));
	return 0;
}

int omax_expr_mean(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	double sum = 0;
	int i;
	long len = osc_atom_array_u_getLen(*argv);
	for(i = 0; i < len; i++){
		//sum += atom64_getfloat(argv[0] + i);
		sum += osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
	}
	//atom64_setfloat(*argv_out, sum / *argc);
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), sum / len);
	return 0;
}

// this implementation of the median is not great--it just sorts 
// the list and takes the median.  this could be optimized by 
// implementing a median of medians algorithm or something
int comp(const void *val1, const void *val2){
	double v1 = *((double *)val1);
	double v2 = *((double *)val2);
	if(v1 < v2){
		return -1;
	}
	if(v1 == v2){
		return 0;
	}
	return 1;
}

int omax_expr_median(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	long len = osc_atom_array_u_getLen(*argv);
	double *tmp = NULL;
	osc_atom_array_u_getDoubleArray(*argv, &tmp);
	qsort((void *)(tmp), len, sizeof(double), comp);
	if((len % 2) == 0){
		double left = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, (int)((len - 1) / 2.)));
		double right = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, ((int)((len - 1) / 2.) + 1)));
		osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), (left + right) / 2.);
	}else{
		osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), osc_atom_u_getDouble(osc_atom_array_u_get(*argv, ((len - 1) / 2))));
	}
	if(tmp){
		osc_mem_free(tmp);
	}
	return 0;
}

int omax_expr_concat(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){/*
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
											      */
	return 0;
}

int omax_expr_reverse(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	long len = osc_atom_array_u_getLen(*argv);
	*out = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
		osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, len - i - 1));
	}
	return 0;
}

int omax_expr_make_list(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	int n = osc_atom_u_getInt(osc_atom_array_u_get(*argv, 0));
	printf("n = %d\n", n);
	t_osc_atom_u *val = NULL;
	//t_atom64 *result = (t_atom64 *)osc_mem_alloc(n * sizeof(t_atom64));
	*out = osc_atom_array_u_alloc(n);
	int alloc = 0;
	if(argc == 2){
		val = osc_atom_array_u_get(argv[1], 0);
	}
	if(!val){
		alloc = 1;
		val = osc_atom_u_alloc();
		osc_atom_u_setDouble(val, 0.);
	}
	int i;
	for(i = 0; i < n; i++){
		//atom64_setfloat(result + i, val);
		t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
		osc_atom_u_copy(&a, val);
 	}
	if(alloc){
		osc_atom_u_free(val);
	}
	return 0;
}

int omax_expr_range(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc < 2){
		error("omax_expr: range requires at least 2 arguments:  start and end.");
		return 1;
	}
	int dblup = 0;
	if(osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, 0)) == 'f' || 
	   osc_atom_u_getTypetag(osc_atom_array_u_get(argv[1], 0)) == 'f' || 
	   osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, 0)) == 'd' || 
	   osc_atom_u_getTypetag(osc_atom_array_u_get(argv[1], 0)) == 'd'){
		dblup = 1;
	}
	if(dblup == 0 && argc > 2){
		if(osc_atom_u_getTypetag(osc_atom_array_u_get(argv[2], 0)) == 'f' || 
		   osc_atom_u_getTypetag(osc_atom_array_u_get(argv[2], 0)) == 'd'){
			dblup = 1;
		}
	}
	if(dblup){
		double start, end, step = 1.;
		start = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, 0));
		end = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0));
		if(argc > 2){
			step = osc_atom_u_getDouble(osc_atom_array_u_get(argv[2], 0));
		}
		int n = (end - start) / step;
		int i;
		*out = osc_atom_array_u_alloc(n);
		for(i = 0; i < n; i++){
			osc_atom_u_setDouble(osc_atom_array_u_get(*out, i), start);
			start += step;
		}
	}else{
		int32_t start, end, step = 1;
		start = osc_atom_u_getInt32(osc_atom_array_u_get(*argv, 0));
		end = osc_atom_u_getInt32(osc_atom_array_u_get(argv[1], 0));
		if(argc > 2){
			step = osc_atom_u_getInt32(osc_atom_array_u_get(argv[2], 0));
		}
		int n = (end - start) / step;
		int i;
		*out = osc_atom_array_u_alloc(n);
		for(i = 0; i < n; i++){
			osc_atom_u_setInt32(osc_atom_array_u_get(*out, i), start);
			start += step;
		}
	}
	return 0;
}

int omax_expr_multiplex(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	int min = osc_atom_array_u_getLen(*argv);
	int i, j, k = 0;
	for(i = 1; i < argc; i++){
		int val = osc_atom_array_u_getLen(argv[i]);
		if(val < min){
			min = val;
		}
	}
	*out = osc_atom_array_u_alloc(argc * min);
	for(i = 0; i < argc; i++){
		for(j = 0; j < min; j++){
			t_osc_atom_u *a = osc_atom_array_u_get(*out, i + (j * argc));
			osc_atom_u_copy(&a, osc_atom_array_u_get(argv[i], j));
		}
	}
	return 0;
}

int omax_expr_not(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	int i;
	*out = osc_atom_array_u_alloc(1);
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		switch(osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, i))){
			// lazy...
		case 'i':
		case 'I':
		case 'h':
		case 'H':
			osc_atom_u_setInt32(osc_atom_array_u_get(*out, i), !(osc_atom_u_getInt32(osc_atom_array_u_get(*argv, i))));
			break;
		case 'f':
		case 'd':
			osc_atom_u_setInt32(osc_atom_array_u_get(*out, i), !(osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i))));
			break;
		case 's':
		case 'b':
		case '#':
			{
				t_osc_atom_u *a = osc_atom_array_u_get(*out, i);
				osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, i));
			}
			break;
		case 'T':
			osc_atom_u_setFalse(osc_atom_array_u_get(*out, i));
			break;
		case 'F':
		case 'N':
			osc_atom_u_setTrue(osc_atom_array_u_get(*out, i));
			break;
		default:
			osc_atom_u_setFalse(osc_atom_array_u_get(*out, i));
		}
	}
	return 0;
}

int omax_expr_dot(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc < 2){
		return 0;
	}
	*out = osc_atom_array_u_alloc(1);
	double s = 0;
	int i;
	long len0 = osc_atom_array_u_getLen(*argv);
	long len1 = osc_atom_array_u_getLen(argv[1]);
	// this should work on ints too
	for(i = 0; i < (len0 < len1 ? len0 : len1); i++){
		double f1 = 0, f2 = 0;
		if(i < len0){
			f1 = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		}
		if(i < len1){
			f2 = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], i));
		}
		s += f1 * f2;
	}
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), s);
	return 0;
}

int omax_expr_l2norm(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc < 0){
		return 0;
	}
	*out = osc_atom_array_u_alloc(1);
	double s = 0;
	int i;
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		s += pow(osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i)), 2.);
	}
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), sqrt(s));
	return 0;
}

int omax_expr_min(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc == 0){
		return 0;
	}
	*out = osc_atom_array_u_alloc(1);
	int i;
	double min = DBL_MAX;
	t_osc_atom_u *a = NULL;
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		double f = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		if(f <= min){
			min = f;
			a = osc_atom_array_u_get(*argv, i);
		}
	}
	t_osc_atom_u *o = osc_atom_array_u_get(*out, 0);
	osc_atom_u_copy(&o, a);
	return 0;
}

int omax_expr_max(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc == 0){
		return 0;
	}
	*out = osc_atom_array_u_alloc(1);
	int i;
	double max = -DBL_MAX;
	t_osc_atom_u *a = NULL;
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		double f = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		if(f >= max){
			max = f;
			a = osc_atom_array_u_get(*argv, i);
		}
	}
	t_osc_atom_u *o = osc_atom_array_u_get(*out, 0);
	osc_atom_u_copy(&o, a);
	return 0;
}

int omax_expr_extrema(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc == 0){
		return 0;
	}
	*out = osc_atom_array_u_alloc(2);
	int i;
	double min = DBL_MAX, max = -DBL_MAX;
	t_osc_atom_u *amin = NULL, *amax = NULL;
	for(i = 0; i < osc_atom_array_u_getLen(*argv); i++){
		double f = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		if(f <= min){
			min = f;
			amin = osc_atom_array_u_get(*argv, i);
		}
		if(f >= max){
			max = f;
			amax = osc_atom_array_u_get(*argv, i);
		}
	}
	t_osc_atom_u *omin = osc_atom_array_u_get(*out, 0);
	t_osc_atom_u *omax = osc_atom_array_u_get(*out, 1);
	osc_atom_u_copy(&omin, amin);
	osc_atom_u_copy(&omax, amax);
	return 0;
}

int omax_expr_clip(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc != 3){
		return 1;
	}
	double min = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0));
	double max = osc_atom_u_getDouble(osc_atom_array_u_get(argv[2], 0));
	long len = osc_atom_array_u_getLen(*argv);
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		char type = osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, i));
		if(type == 's' || type == 'T' || type == 'F' || type == 'N' || type == 'b' || type == '#'){
			t_osc_atom_u *a = osc_atom_array_u_get(result, i);
			osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, i));
			continue;
		}
		double val = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		if(val < min){
			val = min;
		}
		if(val > max){
			val = max;
		}
		if(type == 'f' || type == 'd'){
			osc_atom_u_setDouble(osc_atom_array_u_get(result, i), val);
		}else{
			osc_atom_u_setInt32(osc_atom_array_u_get(result, i), (int32_t)val);
		}
	}
	*out = result;
	return 0;
}

int omax_expr_scale(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc != 5){
		return 1;
	}
	double min_in = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0));
	double max_in = osc_atom_u_getDouble(osc_atom_array_u_get(argv[2], 0));
	double min_out = osc_atom_u_getDouble(osc_atom_array_u_get(argv[3], 0));
	double max_out = osc_atom_u_getDouble(osc_atom_array_u_get(argv[4], 0));
	double m = (max_out - min_out) / (max_in - min_in);
	float b = (min_out - (m * min_in));
	long len = osc_atom_array_u_getLen(*argv);
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		char type = osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, i));
		if(type == 's' || type == 'T' || type == 'F' || type == 'N' || type == 'b' || type == '#'){
			t_osc_atom_u *a = osc_atom_array_u_get(result, i);
			osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, i));
			continue;
		}
		double val = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		val = m * val + b;
		if(type == 'f' || type == 'd'){
			osc_atom_u_setDouble(osc_atom_array_u_get(result, i), val);
		}else{
			osc_atom_u_setInt32(osc_atom_array_u_get(result, i), (int32_t)val);
		}
	}
	*out = result;
	return 0;
}

int omax_expr_mtof(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc == 0){
		return 1;
	}
	double base = 440;
	if(argc > 1){
		if(osc_atom_array_u_getLen(argv[1]) > 0){
			base = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0));
		}
	}
	long len = osc_atom_array_u_getLen(*argv);
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		char type = osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, i));
		if(type == 's' || type == 'T' || type == 'F' || type == 'N' || type == 'b' || type == '#'){
			t_osc_atom_u *a = osc_atom_array_u_get(result, i);
			osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, i));
			continue;
		}
		double val = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		// l(2) * (1. / 12.)
		val = base * exp(.05776226504666210911810267678817 * (val - 69));
		osc_atom_u_setDouble(osc_atom_array_u_get(result, i), val);
	}
	*out = result;
	return 0;
}

int omax_expr_ftom(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	if(argc == 0){
		return 1;
	}
	double base = 440;
	if(argc > 1){
		if(osc_atom_array_u_getLen(argv[1]) > 0){
			base = osc_atom_u_getDouble(osc_atom_array_u_get(argv[1], 0));
		}
	}
	long len = osc_atom_array_u_getLen(*argv);
	t_osc_atom_ar_u *result = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		char type = osc_atom_u_getTypetag(osc_atom_array_u_get(*argv, i));
		if(type == 's' || type == 'T' || type == 'F' || type == 'N' || type == 'b' || type == '#'){
			t_osc_atom_u *a = osc_atom_array_u_get(result, i);
			osc_atom_u_copy(&a, osc_atom_array_u_get(*argv, i));
			continue;
		}
		double val = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		// 1. / (l(2) * (1. / 12))
		val = (69 + (17.31234049066756088831909617202611 * log(val / base)));
		osc_atom_u_setDouble(osc_atom_array_u_get(result, i), val);
	}
	*out = result;
	return 0;
}

int omax_expr_rand(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	*out = osc_atom_array_u_alloc(1);
	osc_atom_u_setDouble(osc_atom_array_u_get(*out, 0), (double)rand() / (double)RAND_MAX);
	return 0;
}

int omax_expr_sgn(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	long len = osc_atom_array_u_getLen(*argv);
	*out = osc_atom_array_u_alloc(len);
	int i;
	for(i = 0; i < len; i++){
		double f = osc_atom_u_getDouble(osc_atom_array_u_get(*argv, i));
		if(f == 0){
			osc_atom_u_setInt32(osc_atom_array_u_get(*argv, i), 0);
		}else if(f < 0){
			osc_atom_u_setInt32(osc_atom_array_u_get(*argv, i), -1);
		}else{
			osc_atom_u_setInt32(osc_atom_array_u_get(*argv, i), 1);
		}
	}
	return 0;
}

int omax_expr_if(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	// this is a dummy function.  we'll use this to do a pointer comparison.
}

int omax_expr_defined(t_omax_expr *f, int argc, t_osc_atom_ar_u **argv, t_osc_atom_ar_u **out){
	// this is a dummy function.  we'll use this to do a pointer comparison.
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
