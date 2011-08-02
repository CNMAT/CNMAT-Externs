%{
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef WIN_VERSION
#include <Carbon.h>
#include <CoreServices.h>
#endif
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"
#include "omax_expr.h"
#include "omax_parser.h"

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp);
 void yyerror(struct _omax_expr **function_list, struct _omax_expr **f, int argc, t_atom *argv, int *argp, char const *e);

 void yyerror(struct _omax_expr **function_list, struct _omax_expr **f, int argc, t_atom *argv, int *argp, char const *e){
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
				 int ptrlen = strlen(ptr);
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
						 switch(*ptr){
						 case '+':
							 if(ptrlen == 2){
								 switch(ptr[1]){
								 case '+':
									 token = INC;
									 break;
								 case '=':
									 token = ASSIGN;
									 break;
								 }
							 }else{
								 token = ADD;
							 }
							 break;
						 case '-':
							 if(ptrlen == 2){
								 switch(ptr[1]){
								 case '-':
									 token = DEC;
									 break;
								 case '=':
									 token = ASSIGN;
									 break;
								 }
							 }else{
								 token = SUBTRACT;
							 }
							 break;
						 case '*':
							 if(ptrlen == 2){
								 if(ptr[1] == '='){
									 token = ASSIGN;
								 }
							 }else{
								 token = MULTIPLY;
							 }
							 break;
						 case '/':
							 if(ptrlen == 2){
								 if(ptr[1] == '='){
									 token = ASSIGN;
								 }
							 }else{
								 token = DIVIDE;
							 }
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
							 if(ptrlen == 2){
								 if(ptr[1] == '='){
									 token = ASSIGN;
								 }
							 }else{
							 	token = MOD;
							 }
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
				 if(func){
					 t_omax_expr *expr = (t_omax_expr *)osc_mem_alloc(sizeof(t_omax_expr));
					 memset(expr, '\0', sizeof(t_omax_expr));
					 expr->rec = omax_expr_funcsym + i;
					 lvalp->expr = expr;
				 }else{
					 if(ptr[0] == '/' && strlen(ptr) > 1){
						 token = ARG;
						 //t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
						 t_omax_expr_arg *arg = omax_expr_arg_alloc();
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
					 }else if(ptr[0] == '?'){
						 if(ptrlen == 2){
							 if(ptr[1] == '?'){
								 token = DBLQMARK;
							 }
						 }else{
						 	token = QMARK;
						 }
					 }else if(ptr[0] == ':'){
						 token = COLON;
					 }else{
						 token = ARG;
						 //t_omax_expr_arg *arg = (t_omax_expr_arg *)osc_mem_alloc(sizeof(t_omax_expr_arg));
						 t_omax_expr_arg *arg = omax_expr_arg_alloc();
						 arg->type = OMAX_ARG_TYPE_ATOM;
						 arg->arg.atom = argv[*argp];
						 lvalp->arg = arg;
					 }
				 }
			 }
		 }
	 out:
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
	//t_omax_expr *exprlist = *f;
	*f = expr;
	*result = omax_expr_arg_alloc();
	(*result)->arg.expr = expr;
	(*result)->type = OMAX_ARG_TYPE_EXPR;
	return 0;
 }
 int omax_parser_assignment(t_omax_expr **f, t_omax_expr_arg **result, t_omax_expr_arg *arg1, t_omax_expr *infixop, t_omax_expr_arg *arg2){
	 omax_parser_infix(f, result, arg1, infixop, arg2);
	 (*f)->assign_result_to_address = 1;
	 return 0;
 }

 %}

%expect 1
%define "api.pure"

 %parse-param{t_omax_expr **function_list}
 %parse-param{t_omax_expr **f}
 %parse-param{int argc}
 %parse-param{t_atom *argv}
 %parse-param{int *argp}

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
%token <sym>COMMA L_DOUBLE_BRACKET R_DOUBLE_BRACKET
%left LPAREN 
%right RPAREN
%right DBLCOLON

 // low to high precedence
%right <expr>ASSIGN ADD_ASSIGN SUBTRACT_ASSIGN
%right QMARK COLON DBLQMARK
%left <expr>OR
%left <expr>AND
%left <expr>EQ NEQ
%left <expr>LT LTE GT GTE 
%left <expr>ADD SUBTRACT
%left <expr>MULTIPLY DIVIDE MOD
%left <sym>NOT
%right <expr>POWER 
%right PREFIX_OP
%left <expr>INC DEC 

%type <arg>expn arglist

%%

input:
| input expn {
	if(*f){
		t_omax_expr *ff = *function_list;
		t_omax_expr *prev = NULL;
		while(ff){
			if(ff->putlast){
				break;
			}
			prev = ff;
			ff = ff->next;
		}
		(*f)->next = ff;
		if(prev){
			prev->next = *f;
		}else{
			*function_list = *f;
		}
	}
}
;

arglist: expn {
	$$ = $1;
	$$->next = NULL;
 }
| arglist COMMA expn{
	$3->next = NULL;
	t_omax_expr_arg *arg = $1;
	t_omax_expr_arg *last = arg;
	while(arg){
		last = arg;
		arg = arg->next;
	}
	last->next = $3;
	$$ = $1;
 }
;

expn:	ARG {;}
|	PREFIX_FUNC LPAREN RPAREN {
	t_omax_expr *func = $1;
	func->argv = NULL;
	func->argc = 0;
	*f = func;
	$$ = omax_expr_arg_alloc();
	$$->arg.expr = func;
	$$->type = OMAX_ARG_TYPE_EXPR;
 }
|	PREFIX_FUNC LPAREN arglist RPAREN {
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
|	expn ASSIGN expn{
	omax_parser_assignment(f, &$$, $1, $2, $3);
 } 
|	ARG INC{
	t_omax_expr_arg *arg = omax_expr_arg_alloc();
	t_atom a;
	atom_setlong(&a, 1);
	arg->arg.atom = a;
	arg->type = OMAX_ARG_TYPE_ATOM;
	//omax_parser_assignment(f, &$$, $1, $2, arg);
	t_omax_expr *expr = $2;
	t_omax_expr_arg *arg1 = omax_expr_arg_alloc();
	memcpy(arg1, $1, sizeof(t_omax_expr_arg));
	arg1->next = arg;
	expr->argv = arg1;
	expr->argc = 2;
	expr->putlast = 1;
	expr->assign_result_to_address = 1;
	t_omax_expr *flist = *function_list;
	t_omax_expr *prev = NULL;
	while(flist){
		if(flist->putlast){
			break;
		}
		prev = flist;
		flist = flist->next;
	}
	expr->next = flist;
	if(prev){
		prev->next = expr;
	}else{
		*function_list = expr;
	}
	$$ = $1;
 }
|	INC ARG %prec PREFIX_OP{
	t_omax_expr_arg *arg = omax_expr_arg_alloc();
	t_atom a;
	atom_setlong(&a, 1);
	arg->arg.atom = a;
	arg->type = OMAX_ARG_TYPE_ATOM;
	omax_parser_assignment(f, &$$, $2, $1, arg);
 }
|	ARG L_DOUBLE_BRACKET arglist R_DOUBLE_BRACKET {
	t_omax_expr *func = osc_mem_alloc(sizeof(t_omax_expr));
	memset(func, '\0', sizeof(t_omax_expr));
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
|	expn QMARK expn COLON expn {
	t_omax_expr *func = osc_mem_alloc(sizeof(t_omax_expr));
	memset(func, '\0', sizeof(t_omax_expr));
	func->rec = omax_expr_lookupFunction("if");
	$1->next = $3;
	$3->next = $5;
	func->argc = 3;
	func->argv = $1;
	*f = func;
	$$ = omax_expr_arg_alloc();
	$$->arg.expr = func;
	$$->type = OMAX_ARG_TYPE_EXPR;
}
|	expn DBLQMARK expn {
	t_omax_expr *ifunc = osc_mem_alloc(sizeof(t_omax_expr));
	memset(ifunc, '\0', sizeof(t_omax_expr));
	ifunc->rec = omax_expr_lookupFunction("if");
	t_omax_expr *defunc = osc_mem_alloc(sizeof(t_omax_expr));
	memset(defunc, '\0', sizeof(t_omax_expr));
	defunc->rec = omax_expr_lookupFunction("defined");
	defunc->argc = 1;
	$1->next = NULL;
	defunc->argv = $1;
	t_omax_expr_arg *ifarg1 = omax_expr_arg_alloc();
	t_omax_expr_arg *ifarg2 = omax_expr_arg_alloc();
	memcpy(ifarg2, $1, sizeof(t_omax_expr_arg));
	ifarg1->arg.expr = defunc;
	ifarg1->type = OMAX_ARG_TYPE_EXPR;
	ifarg1->next = ifarg2;
	ifunc->argc = 3;
	ifunc->argv = ifarg1;
	ifarg2->next = $3;
	$3->next = NULL;
	$$ = omax_expr_arg_alloc();
	$$->arg.expr = ifunc;
	$$->type = OMAX_ARG_TYPE_EXPR;
 }
| 	LPAREN expn RPAREN	{
	$$ = $2;
 }
;
