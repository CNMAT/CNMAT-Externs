#include <stdio.h>
#include <string.h>
#include "osc.h"
#include "osc_parser.h"
#include "osc_expr.h"
#include "osc_expr_parser.h"

/*
 to run test:
	cd libo
	make -e clean; make -e
 to make test file
	make -e -k test
	test/osc_expr_test
 */

int foo(int argc, char **argv){
	//char *expr = "/foo = 20 + (30 + range(1, 10, 2)[[3, 4]]);";
	//char *expr = "/foo = [10:2:30];";
	//char *expr = "/foo = /bar++;";
	//char *expr = "/foo = /bar == 10 ? 20 : 30;\n/foo++;";
	//char *expr = "/bar = /foo ?? 20;";
	//char *expr = "/bar++;";
	//char *expr = "if(abs(/t) < 10., exec(/tri), /y = 0)";
	char *expr = "/foo = /bar ?? 666."; //test expression (/bar ?? (if it exists otherwise) 666)
	//char *expr = "/foo ??= 666.";

	if(argc == 2){
		expr = argv[1]; //or pass in an expression
	}
	printf("expr = %s\n", expr);
	t_osc_expr *f = NULL; //f = function = lambda
	int ret = osc_expr_parser_parseString(expr, &f); //parse expression string. returns valid or not
	if(ret){
		printf("parsing %s failed\n", expr);
		osc_expr_free(f);
		return 1;
	}
	//text representation of the function tree
	char *functiongraph = NULL;
	long len = 0;
	osc_expr_formatFunctionGraph(f, &len, &functiongraph); //from osc_expr.c
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
		//char *bndl_st = "/t 0.5\n/tri /y = 1. - abs(/t)\n";
		char *bundle_string = "/bar 100\n"; //string representation of osc bundle
		t_osc_bndl_u *bndl = NULL;
		t_osc_parser_subst *subs = NULL;
		long nsubs = 0;
		osc_parser_parseString(strlen(bundle_string) + 2, bundle_string, &bndl, &nsubs, &subs); //unserialized oscizer. throw valid bundles at this and see if it parses it.
		//^multiple bundles need to be /n delimited. 
		while(subs){ //subs is linked list of $n substitutions that need to take place
			t_osc_parser_subst *next = subs->next;
			osc_mem_free(subs);
			subs = next;
		}

		char *sbndl = NULL;
		long sbndl_len = 0;
		osc_bundle_u_serialize(bndl, &sbndl_len, &sbndl); //serialize the bundle

		char *sbuf = NULL;
		long sbuflen = 0;
		osc_bundle_s_format(sbndl_len, sbndl, &sbuflen, &sbuf); //back to string. should look the same as input string. 
		printf("bndl:\n%s", sbuf);
		if(sbuf){
			osc_mem_free(sbuf);
		}

		int j;
		int n = 1;
		for(j = 0; j < n; j++){
			t_osc_atom_ar_u *out = NULL;
			ret = osc_expr_funcall(f, &sbndl_len, &sbndl, &out);//calls the function on the bundle
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

//input an expression and a bundle with data and have it evaluate the expression and outputs the bundle as an answer. 
int test_expression(char *expr, char *bundle_string){
	t_osc_expr *f = NULL; //f = function = lambda
	int ret = osc_expr_parser_parseString(expr, &f); //parse expression string. returns valid or not
	if(ret){
		printf("parsing %s failed\n", expr);
		osc_expr_free(f);
		return 0;
	}	
	//text representation of the function tree
	char *functiongraph = NULL;
	long len = 0;
	osc_expr_formatFunctionGraph(f, &len, &functiongraph); //from osc_expr.c
	
	t_osc_bndl_u *bndl = NULL;
	t_osc_parser_subst *subs = NULL;
	long nsubs = 0;
	osc_parser_parseString(strlen(bundle_string) + 2, bundle_string, &bndl, &nsubs, &subs); //unserialized oscizer. throw valid bundles at this and see if it parses it.
	//^multiple bundles need to be /n delimited. 
	while(subs){ //subs is linked list of $n substitutions that need to take place
		t_osc_parser_subst *next = subs->next;
		osc_mem_free(subs);
		subs = next;
	}
	char *sbndl = NULL;
	long sbndl_len = 0;
	osc_bundle_u_serialize(bndl, &sbndl_len, &sbndl); //serialize the bundle
	
	t_osc_atom_ar_u *out = NULL;
	ret = osc_expr_funcall(f, &sbndl_len, &sbndl, &out);//calls the function on the bundle
	osc_atom_array_u_free(out);
	
	char *buf = NULL;
	long buflen = 0;
	osc_bundle_s_format(sbndl_len, sbndl, &buflen, &buf);
	printf("bndl:\n%s", buf);
	
	//free all the memory
	osc_bundle_u_free(bndl);
	if(sbndl){
		osc_mem_free(sbndl);
	}
	if(buf){
		osc_mem_free(buf);
	}
	osc_expr_free(f);
	if(functiongraph){
		osc_mem_free(functiongraph);
	}
	return 1;
	
}

int main(int argc, char **argv){
	char *bundle_string = "/bar 100\n"; //string representation of osc bundle
	char *expr = "/foo = /bar ?? 666."; //test expression
	printf("returned %d\n",test_expression(expr, bundle_string));
	
}


