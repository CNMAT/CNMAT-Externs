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
				 char *ptr = atom_getsym(argv + *argp)->s_name;
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
				 }else if(ptr[0] == ','){
					 token = COMMA;
				 }else{
					 // function name
					 int i;
					 void *func = NULL;
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
								 token = EQ;
								 break;
							 case '!':
								 token = NEQ;
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
							 default:
								 token = PREFIX_FUNC;
							 }
							 break;
						 }
					 }
					 if(!func){
						 error("function not found");
						 return 1; // function not found
					 }
					 t_omax_expr *expr = (t_omax_expr *)osc_mem_alloc(sizeof(t_omax_expr));
					 memset(expr, '\0', sizeof(expr));
					 //expr->f = omax_expr_prefix_func;
					 expr->rec = omax_expr_funcsym + i;
					 lvalp->expr = expr;
				 }
			 }
		 }
		 *argp += 1;
	 }
	 return token;
 }

 int omax_parser_infix(t_omax_expr **f, t_omax_expr_arg **result, t_omax_expr_arg *arg1, t_omax_expr *infixop, t_omax_expr_arg *arg2){
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

 int omax_parser_prefix_argv(t_omax_expr **f, t_omax_expr_arg **result, t_omax_expr *func, int argc, t_omax_expr_arg **argv){
	 int i;
	 t_omax_expr_arg *arg = *argv;
	 for(i = 1; i < argc; i++){
		 arg->next = *(argv + i);
		 arg = *(argv + i);
	 }
	 arg->next = NULL;
	 func->argv = *argv;
	 func->argc = argc;
	 *f = func;
	 *result = omax_expr_arg_alloc();
	 (*result)->arg.expr = func;
	 (*result)->type = OMAX_ARG_TYPE_EXPR;
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
%token <sym>LPAREN RPAREN COMMA
%token <expr>OR
%token <expr>AND
%token <expr>EQ NEQ
%token <expr>LT LTE GT GTE 
%token <expr>ADD SUBTRACT
%token <expr>MULTIPLY DIVIDE MOD
%token <expr>POWER 
%type <arg>expn

%left ADD SUBTRACT AND OR EQ NEQ LT LTE GT GTE
%left MULTIPLY DIVIDE 
%right POWER    
%%

 /*
   t_omax_expr *expr = $2;
   $1->next = $3;
   expr->argv = $1;
   expr->argc = 2;
   t_omax_expr *exprlist = *f;
   *f = expr;
   $$ = omax_expr_arg_alloc();
   $$->arg.expr = expr;
   $$->type = OMAX_ARG_TYPE_EXPR;
 */

input:
| input expn
;

expn:	ARG {;}
|	expn ADD expn	{
	omax_parser_infix(f, &$$, $1, $2, $3);
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
|	PREFIX_FUNC LPAREN expn RPAREN {
	omax_parser_prefix_argv(f, &$$, $1, 1, &$3);
 }
|	PREFIX_FUNC LPAREN expn COMMA expn RPAREN {
	t_omax_expr_arg *argv[2];
	argv[0] = $3, argv[1] = $5;
	omax_parser_prefix_argv(f, &$$, $1, 2, argv);
 }
| 	LPAREN expn RPAREN	{
	$$ = $2;
 }
;
