%{
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Carbon.h>
#include <CoreServices.h>
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"
#include "omax_expr.h"
#include "omax_parser.h"

	//#define YYLEX_PARAM llocp
	//#define YYSTYPE t_atom*
	//#define YYLTYPE t_omax_expr *

//void omax_parser_addfunc_2arg(t_omax_func_ptr *f, t_

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp);
 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_expr **f, char const *e);

 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_expr **f, char const *e){
	printf("i'm here, but i really wish i wasn't...\n");
	printf("%s\n", e);
 }

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp){
	 int token = 0;
	 if(*argp < argc){
		 switch(atom_gettype(argv + *argp)){
		 case A_LONG:
			 {
			 	printf("%s: argv[%d] = %ld\n", __func__, *argp, atom_getlong(argv + *argp));
			 	token = ARG;
				t_omax_expr_arg *arg = omax_expr_arg_alloc();
				arg->type = OMAX_ARG_TYPE_ATOM;
				printf("%s: arg = %p\n", __func__, arg);
				arg->arg.atom = *(argv + (*argp));
				lvalp->arg = arg;
			 }
			 break;
		 case A_FLOAT:
			 {
			 	printf("%s: argv[%d] = %f\n", __func__, *argp, atom_getfloat(argv + *argp));
			 	token = ARG;
				t_omax_expr_arg *arg = omax_expr_arg_alloc();
				arg->type = OMAX_ARG_TYPE_ATOM;
				printf("%s: arg = %p\n", __func__, arg);
				arg->arg.atom = *(argv + (*argp));
				lvalp->arg = arg;
			 }
		         break;
		 case A_SYM:
			 printf("%s: argv[%d] = %s\n", __func__, *argp, atom_getsym(argv + *argp)->s_name);
			 {
				 char *ptr = atom_getsym(argv + *argp)->s_name;
				 if(ptr[0] == '/'){
					 token = ARG;
					 t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
					 arg->type = OMAX_ARG_TYPE_OSCADDRESS;
					 arg->arg.osc_address = atom_getsym(argv + (*argp))->s_name;
					 lvalp->arg = arg;
				 }else if(strlen(ptr) == 1){
					 switch(*ptr){
					 case '+':
						 {
						 	token = PLUS;
							t_omax_expr *expr = (t_omax_expr *)osc_mem_alloc(sizeof(t_omax_expr));
							memset(expr, '\0', sizeof(expr));
							printf("%s: expr = %p\n", __func__, expr);
							expr->f = omax_expr_plus;
							lvalp->expr = expr;
						 }
						 
					 }
				 }
			 }
		 }
		 *argp += 1;
	 }
	 return token;
 }

 %}

 %define "api.pure"

 %parse-param{int argc}
 %parse-param{t_atom *argv}
 %parse-param{int *argp}
 %parse-param{t_omax_expr **f}
 %lex-param{int argc}
 %lex-param{t_atom *argv}
 %lex-param{int *argp}

%union {
	struct _omax_expr *expr;
	struct _omax_expr_arg *arg;
	t_symbol sym;
}

%token <arg>ARG
%token <expr>PREFIX_FUNC
%token <sym>LPAREN RPAREN COMMA
%token <expr>PLUS MINUS MULTIPLY DIVIDE POWER AND OR EQ NEQ LT LTE GT GTE
%type <arg>expn

%left PLUS MINUS MULTIPLY DIVIDE AND OR EQ NEQ LT LTE GT GTE
%right POWER    
%% 

input:
| input expn
;

expn:	ARG {;}
|	expn PLUS expn	{
	//t_omax_expr_arg *argv = (t_omax_expr_arg *)osc_mem_alloc(2 * sizeof(t_omax_expr_arg));
	//*argv = *$1;
			//(argv + 1)->arg.atom = *$3;
			//(argv + 1)->type = OMAX_ARG_TYPE_ATOM;
			//*argv = *$3;
			//t_omax_expr *func = (t_omax_expr *)osc_mem_alloc(sizeof(t_omax_expr));
			//func->f = omax_parser_plus;
			//func->argc = 2;
			//func->argv = argv;
			//printf("%s: func = %p\n", __func__, func);
	printf("%p %p %p %p\n", $$, $1, $2, $3);
			t_omax_expr *expr = $2;
			$1->next = $3;
			printf("types = %d %d\n", $1->type, $3->type);
			expr->argv = $1;
			expr->argc = 2;
			t_omax_expr *exprlist = *f;
			//if(exprlist){
				/*
				while(exprlist->next){
					exprlist = exprlist->next;
				}
				exprlist->next = expr;
				expr->next = NULL;
				expr->prev = exprlist;
				*/
			//}else{
				*f = expr;
				//expr->next = NULL;
				//expr->prev = NULL;
				//}
				//$<expr>$ = expr;
			$$ = omax_expr_arg_alloc();
			$$->arg.expr = expr;
			$$->type = OMAX_ARG_TYPE_EXPR;
 		}
;

