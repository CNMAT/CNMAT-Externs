#include <stdio.h>
#include <string.h>
#include "osc.h"
#include "osc_parser.h"
#include "osc_expr.h"
#include "osc_expr_parser.h"

int foo(int argc, char **argv){
	//char *expr = "/foo = 20 + (30 + range(1, 10, 2)[[3, 4]]);";
	//char *expr = "/foo = [10:2:30];";
	//char *expr = "/foo = /bar++;";
	//char *expr = "/foo = /bar == 10 ? 20 : 30;\n/foo++;";
	//char *expr = "/bar = /foo ?? 20;";
	//char *expr = "/bar++;";
	char *expr = "if(abs(/t) < 10., exec(/tri), /y = 0)";
	if(argc == 2){
		expr = argv[1];
	}
	printf("expr = %s\n", expr);
	t_osc_expr *f = NULL;
	int ret = osc_expr_parser_parseString(expr, &f);
	if(ret){
		printf("parsing %s failed\n", expr);
		osc_expr_free(f);
		return 1;
	}

	char *functiongraph = NULL;
	long len = 0;
	osc_expr_formatFunctionGraph(f, &len, &functiongraph);
	printf("functiongraph:\n");
	printf("%s", functiongraph);

	int n = 1;
	int k;
	for(k = 0; k < n; k++){
		/*
		t_osc_bndl_u *bndl = osc_bundle_u_alloc();
		t_osc_msg_u *msg = osc_message_u_alloc();
		osc_message_u_setAddress(msg, "/t");
		osc_message_u_appendFloat(msg, ((double)k / 10000.) - .5);
		osc_bundle_u_addMsg(bndl, msg);

		msg = osc_message_u_alloc();
		osc_message_u_setAddress(msg, "/tri");
		osc_message_u_appendString(msg, "/y");
		osc_message_u_appendString(msg, "=");
		osc_message_u_appendFloat(msg, 1.);
		osc_message_u_appendString(msg, "-");
		osc_message_u_appendString(msg, "abs(/t)");
		osc_bundle_u_addMsg(bndl, msg);
		*/
		char *bndl_st = "/t 0.5\n/tri /y = 1. - abs(/t)\n";
		t_osc_bndl_u *bndl = NULL;
		t_osc_parser_subst *subs = NULL;
		long nsubs = 0;
		osc_parser_parseString(strlen(bndl_st) + 2, bndl_st, &bndl, &nsubs, &subs);
		while(subs){
			t_osc_parser_subst *next = subs->next;
			osc_mem_free(subs);
			subs = next;
		}

		char *sbndl = NULL;
		long sbndl_len = 0;
		osc_bundle_u_serialize(bndl, &sbndl_len, &sbndl);

		char *sbuf = NULL;
		long sbuflen = 0;
		osc_bundle_s_format(sbndl_len, sbndl, &sbuflen, &sbuf);
		printf("bndl:\n%s", sbuf);
		if(sbuf){
			osc_mem_free(sbuf);
		}

		int j;
		int n = 1;
		for(j = 0; j < n; j++){
			t_osc_atom_ar_u *out = NULL;
			ret = osc_expr_funcall(f, &sbndl_len, &sbndl, &out);
			osc_atom_array_u_free(out);
		}


		char *buf = NULL;
		long buflen = 0;
		osc_bundle_s_format(sbndl_len, sbndl, &buflen, &buf);
		printf("bndl:\n%s", buf);

		osc_bundle_u_free(bndl);
		if(sbndl){
			osc_mem_free(sbndl);
		}
		if(buf){
			osc_mem_free(buf);
		}

	}
	osc_expr_free(f);
	if(functiongraph){
		osc_mem_free(functiongraph);
	}
}

int main(int argc, char **argv){
	foo(argc, argv);
	while(1){
		sleep(1);
	}
}
