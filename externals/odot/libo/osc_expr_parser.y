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
#include "osc_expr.h"
#include "osc_error.h"
#include "osc_expr_parser.h"
#include "osc_expr_scanner.h"

#define OSC_EXPR_PARSER_DEBUG
#ifdef OSC_EXPR_PARSER_DEBUG
#define PP(fmt, ...)printf(fmt, ##__VA_ARGS__)
#else
#define PP(fmt, ...)
#endif

%}
%code requires{
#include "osc.h"
#include "osc_mem.h"
#include "osc_atom_u.h"
#include "osc_expr.h"
#include "osc_expr.r"

t_osc_err osc_expr_parser_parseString(char *ptr, t_osc_expr **f);

}

%{

int osc_expr_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner);
int osc_expr_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner){
	return osc_expr_scanner_lex(yylval_param, yyscanner);
}

t_osc_err osc_expr_parser_parseString(char *ptr, t_osc_expr **f){
	yyscan_t scanner;
	osc_expr_scanner_lex_init(&scanner);
	YY_BUFFER_STATE buf_state = osc_expr_scanner__scan_string(ptr, scanner);
	osc_expr_scanner_set_out(NULL, scanner);
	t_osc_expr *exprstack = NULL;
	t_osc_err ret = osc_expr_parser_parse(&exprstack, scanner);
	osc_expr_scanner__delete_buffer(buf_state, scanner);
	osc_expr_scanner_lex_destroy(scanner);

	*f = exprstack;
	return ret;
}

void yyerror(t_osc_expr **exprstack, void *scanner, char const *e);
void yyerror(t_osc_expr **exprstack, void *scanner, char const *e){
	printf("%s\n", e);
}

t_osc_expr *osc_expr_parser_infix(char *function_name, t_osc_expr_arg *left, t_osc_expr_arg *right){
	t_osc_expr_rec *r = osc_expr_lookupFunction(function_name);
	if(!r){
		printf("function \"%s\" not found\n", function_name);
		return NULL;
	}
	t_osc_expr *e = osc_expr_alloc();
	e->rec = r;
	left->next = e->argv;
	e->argv = left;
	right->next = e->argv;
	e->argv = right;
	return e;
}

t_osc_expr *osc_expr_parser_prefix(char *function_name, t_osc_expr_arg *arglist){
	t_osc_expr_rec *r = osc_expr_lookupFunction(function_name);
	if(!r){
		printf("function \"%s\" not found\n", function_name);
		return NULL;
	}
	t_osc_expr *e = osc_expr_alloc();
	e->rec = r;
	int argc = 0;
	t_osc_expr_arg *a = arglist;
	while(a){
		argc++;
		a = a->next;
	}
	e->argv = arglist;
	e->argc = argc;
	return e;
}

%}

%define "api.pure"

%parse-param{t_osc_expr **exprstack}
%parse-param{void *scanner}
%lex-param{void *scanner}

%union {
	t_osc_atom_u *atom;
	t_osc_expr *expr;
	t_osc_expr_arg *arg;
}


%left OPEN_DBL_BRKTS
%right CLOSE_DBL_BRKTS
%right DBLCOLON

 // low to high precedence
%right ASSIGN ADD_ASSIGN SUBTRACT_ASSIGN
%right QMARK COLON DBLQMARK
%left OSC_EXPR_OR
%left OSC_EXPR_AND
%left OSC_EXPR_EQ OSC_EXPR_NEQ
%left OSC_EXPR_LT OSC_EXPR_LTE OSC_EXPR_GT OSC_EXPR_GTE 
%left '+' '-'
%left '*' '/' '%'
%left '!'
%right '^'
%left OSC_EXPR_INC OSC_EXPR_DEC 

%left OSC_EXPR_PLUSEQ OSC_EXPR_MINUSEQ
%left OSC_EXPR_MULTEQ OSC_EXPR_DIVEQ OSC_EXPR_MODEQ
%left OSC_EXPR_POWEQ

%type <expr>expr 
%type <arg>arg args
%token <atom>OSC_EXPR_NUM OSC_EXPR_STRING

%%

expns: 
	| expns expr ';' {
		*exprstack = $2;
 	}
;

args:   arg 
	| args ',' arg {
		$3->next = $$;
		$$ = $3;
 	}
;

arg:    OSC_EXPR_NUM {
		$$ = osc_expr_arg_alloc();
		$$->arg.atom = $1;
		$$->type = OSC_EXPR_ARG_TYPE_ATOM;
 	}
	| OSC_EXPR_STRING {
		$$ = osc_expr_arg_alloc();
		$$->arg.atom = $1;
		$$->type = OSC_EXPR_ARG_TYPE_ATOM;
 	}
	| expr {
		$$ = osc_expr_arg_alloc();
		$$->arg.expr = $1;
		$$->type = OSC_EXPR_ARG_TYPE_EXPR;
  	}
;

expr:   
	arg '+' arg {
		$$ = osc_expr_parser_infix("+", $1, $3);
 	}
	| arg OPEN_DBL_BRKTS args CLOSE_DBL_BRKTS {
		$1->next = $3;
		$$ = osc_expr_parser_prefix("get_index", $1);
	}
	| '(' expr ')' {
		$$ = $2;
  	}
	| OSC_EXPR_STRING '(' args ')' {
		t_osc_expr *e = osc_expr_parser_prefix(osc_atom_u_getStringPtr($1), $3);
		if(!e){
			return 1;
		}
		$$ = e;
  	}
	| '!' arg {
		$$ = osc_expr_alloc();
		$$->rec = osc_expr_lookupFunction("!");
		$$->argv = $2;
		$$->argc = 1;
	}
;
