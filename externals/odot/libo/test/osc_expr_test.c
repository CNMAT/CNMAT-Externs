#include <stdio.h>
#include "osc.h"
#include "osc_expr.h"
#include "osc_expr_parser.h"

int foo(int argc, char **argv){
	//char *expr = "/foo = 20 + (30 + range(1, 10, 2)[[3, 4]]);";
	//char *expr = "/foo = [10:2:30];";
	//char *expr = "/foo = /bar++;";
	char *expr = "/foo = /bar == 10 ? 20 : 30;\n/foo++;";
	//char *expr = "/bar = /foo ?? 20;";
	//char *expr = "/bar++;";
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

	t_osc_bndl_u *bndl = osc_bundle_u_alloc();
	t_osc_msg_u *msg = osc_message_u_alloc();
	osc_message_u_setAddress(msg, "/bar");
	osc_message_u_appendInt32(msg, 10);
	osc_bundle_u_addMsg(bndl, msg);

	char *sbndl = NULL;
	long sbndl_len = 0;
	osc_bundle_u_serialize(bndl, &sbndl_len, &sbndl);

	int k;
	for(k = 0; k < sbndl_len; k++){
		printf("%d: %c 0x%02x\n", k, sbndl[k], sbndl[k]);
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
	printf("bndl: %s", buf);

	osc_expr_free(f);
	osc_bundle_u_free(bndl);
	if(sbndl){
		osc_mem_free(sbndl);
	}
	if(buf){
		osc_mem_free(buf);
	}
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
