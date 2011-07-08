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

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp);
 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_expr **f, char const *e);

 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_expr **f, char const *e){
	error("%s\n", e);
 }

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp){
	 int token = 0;
	 if(*argp < argc){
		 switch(atom_gettype(argv + *argp)){
		 case A_LONG:
		 case A_FLOAT:
			 {
			 	token = ARG;
				t_omax_expr_arg *arg = omax_expr_arg_alloc();
				arg->type = OMAX_ARG_TYPE_ATOM;
				arg->arg.atom = *(argv + (*argp));
				lvalp->arg = arg;
			 }
			 break;
		 case A_SYM:
			 {
				 t_symbol *sym = atom_getsym(argv + *argp);
				 char *ptr = sym->s_name;
				 if(ptr[0] == '/' && strlen(ptr) > 1){
					 token = ARG;
					 t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
					 arg->type = OMAX_ARG_TYPE_OSCADDRESS;
					 arg->arg.osc_address = atom_getsym(argv + (*argp))->s_name;
					 lvalp->arg = arg;
				 }else if(ptr[0] == '('){
					 token = LPAREN;
				 }else if(ptr[0] == ')'){
					 token = RPAREN;
				 }else if(sym == gensym("[[")){
					 token = L_DOUBLE_BRACKET;
				 }else if(sym == gensym("]]")){
					 token = R_DOUBLE_BRACKET;
				 }else if(ptr[0] == ','){
					 token = COMMA;
				 }else{
					 // function or constant name
					 int i;
					 void *func = NULL;
					 for(i = 0; i < sizeof(omax_expr_constsym) / sizeof(t_omax_expr_const_rec); i++){
						 if(!strcmp(ptr, omax_expr_constsym[i].name)){
							 token = ARG;
							 t_omax_expr_arg *arg = omax_expr_arg_alloc();
							 arg->type = OMAX_ARG_TYPE_ATOM;
							 atom_setfloat(&(arg->arg.atom), omax_expr_constsym[i].val);
							 lvalp->arg = arg;
							 goto out;
						 }
					 }
					 for(i = 0; i < sizeof(omax_expr_funcsym) / sizeof(t_omax_expr_rec); i++){
						 if(!strcmp(ptr, omax_expr_funcsym[i].name)){
							 func = omax_expr_funcsym[i].func;
							 int ptrlen = strlen(ptr);
							 switch(*ptr){
							 case '+':
								 token = ADD;
								 break;
							 case '-':
								 token = SUBTRACT;
								 break;
							 case '*':
								 token = MULTIPLY;
								 break;
							 case '/':
								 token = DIVIDE;
								 break;
							 case '<':
								 token = LT;
								 if(ptrlen == 2){
									 token = LTE;
								 }
								 break;
							 case '>':
								 token = GT;
								 if(ptrlen == 2){
									 token = GTE;
								 }
								 break;
							 case '=':
								 if(ptrlen == 1){
									 printf("assign\n");
								 	token = ASSIGN;
								 }else{
								 	token = EQ;
								 }
								 break;
							 case '!':
								 token = NEQ;
								 if(ptrlen == 1){
								 	token = PREFIX_FUNC;
								 }
								 break;
							 case '&':
								 token = AND;
								 break;
							 case '|':
								 token = OR;
								 break;
							 case '%':
								 token = MOD;
								 break;
							 case '^':
								 token = POWER;
								 break;
							 default:
								 token = PREFIX_FUNC;
							 }
							 break;
						 }
					 }
					 if(!func){
						 error("function not found");
						 return 1;
					 }
					 t_omax_expr *expr = (t_omax_expr *)osc_mem_alloc(sizeof(t_omax_expr));
					 memset(expr, '\0', sizeof(expr));
					 expr->rec = omax_expr_funcsym + i;
					 lvalp->expr = expr;
				 }
			 }
		 }
	 out:
		 *argp += 1;
	 }
	 return token;
 }

 int omax_parser_infix(t_omax_expr **f, t_omax_expr_arg **result, t_omax_expr_arg *arg1, t_omax_expr *infixop, t_omax_expr_arg *arg2){
	 printf("%s\n", __func__);
	t_omax_expr *expr = infixop;
	arg1->next = arg2;
	arg2->next = NULL;
	expr->argv = arg1;
	expr->argc = 2;
	t_omax_expr *exprlist = *f;
	*f = expr;
	*result = omax_expr_arg_alloc();
	(*result)->arg.expr = expr;
	(*result)->type = OMAX_ARG_TYPE_EXPR;
	return 0;
 }
 int omax_parser_assignment(t_omax_expr **f, t_omax_expr_arg **result, t_omax_expr_arg *arg1, t_omax_expr *infixop, t_omax_expr_arg *arg2){
printf("%s\n", __func__);
	 omax_parser_infix(f, result, arg1, infixop, arg2);
	 (*f)->assign_result_to_address = 1;
	 return 0;
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
	struct _symbol *sym;
}

%token <arg>ARG
%token <expr>PREFIX_FUNC
%token <sym>LPAREN RPAREN COMMA L_DOUBLE_BRACKET R_DOUBLE_BRACKET

%left <expr>OR
%left <expr>AND
%left <expr>EQ NEQ
%left <expr>LT LTE GT GTE 
%left <expr>ADD SUBTRACT
%left <expr>MULTIPLY DIVIDE MOD
%left <sym>NOT
%right <expr>POWER 
%right <expr>ASSIGN
%type <arg>expn arglist

%%

input:
| input expn
;

arglist: expn {
	//printf("ARG: $$ = %p $1 = %p\n", $$, $1);
	$$ = $1;
	$$->next = NULL;
 }
| arglist COMMA expn{
	//printf("$$ = %p arglist = %p ARG = %p\n", $$, $1, $3);
	$3->next = NULL;
	t_omax_expr_arg *arg = $1;
	t_omax_expr_arg *last = arg;
	while(arg){
		last = arg;
		arg = arg->next;
	}
	//printf("%p %p\n", $1, last);
	last->next = $3;
	$$ = $1;
 }
;

expn:	ARG {;}
|	expn ADD expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	PREFIX_FUNC LPAREN arglist RPAREN {
	//printf("$$ = %p $3 = %p\n", $$, $3);
	t_omax_expr *func = $1;
	int i = 0;
	t_omax_expr_arg *ptr = $3;
	while(ptr){
		ptr = ptr->next;
		i++;
	}
	if(func->rec->numargs != i && func->rec->numargs != -1){
		error("omax_parser: %s expects %d arguments, but %d were found", func->rec->name, func->rec->numargs, i);
	}
	func->argv = $3;
	func->argc = i;
	*f = func;
	$$ = omax_expr_arg_alloc();
	$$->arg.expr = func;
	$$->type = OMAX_ARG_TYPE_EXPR;
 }
|	expn SUBTRACT expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn MULTIPLY expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn DIVIDE expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn LT expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn LTE expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn GT expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn GTE expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn EQ expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn NEQ expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn AND expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn OR expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn MOD expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
 }
|	expn ASSIGN expn{
	printf("%s\n", __func__);
	omax_parser_assignment(f, &$$, $1, $2, $3);
}
/*
|	PREFIX_FUNC LPAREN expn RPAREN {
	omax_parser_prefix_argv(f, &$$, $1, 1, &$3);
 }
|	PREFIX_FUNC LPAREN expn COMMA expn RPAREN {
	t_omax_expr_arg *argv[2];
	argv[0] = $3, argv[1] = $5;
	omax_parser_prefix_argv(f, &$$, $1, 2, argv);
 }
*/
|	ARG L_DOUBLE_BRACKET arglist R_DOUBLE_BRACKET {
	t_omax_expr *func = osc_mem_alloc(sizeof(t_omax_expr));
	func->rec = omax_expr_lookupFunction("get_index");
	$1->next = $3;
	func->argv = $1;
	t_omax_expr_arg *ptr = $3;
	int i = 1;
	while(ptr){
		ptr = ptr->next;
		i++;
	}
	func->argc = i;
	*f = func;
	$$ = omax_expr_arg_alloc();
	$$->arg.expr = func;
	$$->type = OMAX_ARG_TYPE_EXPR;
 }
| 	LPAREN expn RPAREN	{
	$$ = $2;
 }
;
