/*
  Written by John MacCallum, The Center for New Music and Audio Technologies,
  University of California, Berkeley.  Copyright (c) 2011, The Regents of
  the University of California (Regents). 
  Permission to use, copy, modify, distribute, and distribute modified versions
  of this software and its documentation without fee and without a signed
  licensing agreement, is hereby granted, provided that the above copyright
  notice, this paragraph and the following two paragraphs appear in all copies,
  modifications, and distributions.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
  SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
  HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
  MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

/** 	\file osc_expr_parser.y
	\author John MacCallum

*/
%{
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
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

t_osc_err osc_expr_parser_parseString(char *ptr, t_osc_expr **f);

}

%{

int osc_expr_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner);
int osc_expr_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner){
	return osc_expr_scanner_lex(yylval_param, yyscanner);
}

t_osc_err osc_expr_parser_parseString(char *ptr, t_osc_expr **f){
	//printf("parsing %s\n", ptr);
	int len = strlen(ptr);
	int alloc = 0;
	if(ptr[len - 1] != ';'){
		char *tmp = osc_mem_alloc(len + 2);
		memcpy(tmp, ptr, len + 1);
		tmp[len] = ';';
		tmp[len + 1] = '\0';
		ptr = tmp;
		alloc = 1;
	}
	yyscan_t scanner;
	osc_expr_scanner_lex_init(&scanner);
	YY_BUFFER_STATE buf_state = osc_expr_scanner__scan_string(ptr, scanner);
	osc_expr_scanner_set_out(NULL, scanner);
	t_osc_expr *exprstack = NULL;
	t_osc_err ret = osc_expr_parser_parse(&exprstack, scanner);
	osc_expr_scanner__delete_buffer(buf_state, scanner);
	osc_expr_scanner_lex_destroy(scanner);

	*f = exprstack;
	if(alloc){
		osc_mem_free(ptr);
	}
	return ret;
}

void yyerror(t_osc_expr **exprstack, void *scanner, char const *e, ...);
void yyerror(t_osc_expr **exprstack, void *scanner, char const *e, ...){
	va_list ap;
	va_start(ap, e);
	vprintf(e, ap);
	printf("\n");
}

t_osc_expr *osc_expr_parser_infix(char *function_name, t_osc_expr_arg *left, t_osc_expr_arg *right){
	t_osc_expr_rec *r = osc_expr_lookupFunction(function_name);
	if(!r){
		printf("osc_expr_parser: function \"%s\" not found\n", function_name);
		return NULL;
	}
	t_osc_expr *e = osc_expr_alloc();
	osc_expr_setRec(e, r);
	osc_expr_prependArg(e, right);
	osc_expr_prependArg(e, left);
	return e;
}

t_osc_expr *osc_expr_parser_infixAssign(char *function_name, t_osc_expr_arg *left, t_osc_expr_arg *right){
	t_osc_expr *e = osc_expr_parser_infix(function_name, left, right);
	osc_expr_setAssignResultToAddress(e, 1);
	return e;
}

t_osc_expr *osc_expr_parser_prefix(char *function_name, t_osc_expr_arg *arglist){
	t_osc_expr_rec *r = osc_expr_lookupFunction(function_name);
	if(!r){
		printf("osc_expr_parser: function \"%s\" not found\n", function_name);
		return NULL;
	}
	t_osc_expr *e = osc_expr_alloc();
	osc_expr_setRec(e, r);
	osc_expr_setArg(e, arglist);
	return e;
}

t_osc_expr *osc_expr_parser_prefix_incdec(char *oscaddress, char *op){
	char *ptr = oscaddress;
	if(*ptr != '/'){
		printf("osc_expr_parser: expected \"%s\" in \"++%s\" to be an OSC address\n", ptr, ptr);
		return NULL;
	}
	t_osc_expr_arg *arg = osc_expr_arg_alloc();
	osc_expr_arg_setOSCAddress(arg, ptr);
	t_osc_expr *e = osc_expr_parser_prefix(op, arg);
	osc_expr_setAssignResultToAddress(e, 1);
	return e;
}

t_osc_expr *osc_expr_parser_postfix_incdec(t_osc_expr **exprstack, char *oscaddress, char *op){
	t_osc_expr *incdec = osc_expr_parser_prefix_incdec(oscaddress, op);
	if(!incdec){
		return NULL;
	}
	/*
	if(!(*exprstack)){
		*exprstack = e;
	}else{
		osc_expr_appendExpr(*exprstack, e);
	}
	*/
	/*
	int len = strlen(oscaddress) + 1;
	char *ptr = osc_mem_alloc(len);
	memcpy(ptr, oscaddress, len);
	*/
	t_osc_expr_arg *arg1 = osc_expr_arg_alloc();
	osc_expr_arg_setExpr(arg1, incdec);
	char *oscaddress_copy = NULL;
	osc_util_strdup(&oscaddress_copy, oscaddress);
	t_osc_expr_arg *arg2 = osc_expr_arg_alloc();
	osc_expr_arg_setOSCAddress(arg2, oscaddress_copy);
	osc_expr_arg_setNext(arg2, arg1);
	//t_osc_expr *identity = osc_expr_parser_prefix("identity", arg2);
	t_osc_expr *prog1 = osc_expr_parser_prefix("prog1", arg2);
	//osc_expr_setNext(e2, e);
	return prog1;
}

t_osc_expr *osc_expr_parser_nullCoalesce(t_osc_atom_u *address_to_check, t_osc_expr_arg *arg_if_null){
	char *address = NULL;
	osc_atom_u_getString(address_to_check, &address);
	if(*address != '/'){
		osc_mem_free(address);
		printf("osc_expr_parser: expected \"%s\" in \"%s = ... to be an OSC address\n", address, address);
		return NULL;
	}
	t_osc_expr *expr_def = osc_expr_alloc();
	osc_expr_setRec(expr_def, osc_expr_lookupFunction("defined"));
	t_osc_expr_arg *def_arg = osc_expr_arg_alloc();

	osc_expr_arg_setOSCAddress(def_arg, address);
	osc_expr_setArg(expr_def, def_arg);
	t_osc_expr_arg *arg1 = osc_expr_arg_alloc();
	osc_expr_arg_setExpr(arg1, expr_def);
	t_osc_expr_arg *arg2 = osc_expr_arg_copy(def_arg);
	t_osc_expr_arg *arg3 = arg_if_null;
	osc_expr_arg_setNext(arg1, arg2);
	osc_expr_arg_setNext(arg2, arg3);
	return osc_expr_parser_prefix("if", arg1);
}

%}

%define "api.pure"
%require "2.5"

%parse-param{t_osc_expr **exprstack}
%parse-param{void *scanner}
%lex-param{void *scanner}

%union {
	t_osc_atom_u *atom;
	t_osc_expr *expr;
	t_osc_expr_arg *arg;
}

%type <expr>expr 
%type <arg>arg args
%nonassoc <atom>OSC_EXPR_NUM OSC_EXPR_STRING

// low to high precedence
// adapted from http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B

// level 16
%right '=' OSC_EXPR_PLUSEQ OSC_EXPR_MINUSEQ OSC_EXPR_MULTEQ OSC_EXPR_DIVEQ OSC_EXPR_MODEQ OSC_EXPR_POWEQ

// level 15
%right OSC_EXPR_TERNARY_COND OSC_EXPR_DBLQMARK OSC_EXPR_DBLQMARKEQ '?' ':'

// level 14
%left OSC_EXPR_OR

// level 13
%left OSC_EXPR_AND

// level 9
%left OSC_EXPR_EQ OSC_EXPR_NEQ

// level 8
%left '<' '>' OSC_EXPR_LTE OSC_EXPR_GTE 

// level 6
%left '+' '-'

// level 5
%left '/' '%'
%left '*' 
%left '^' 

// level 3
%right OSC_EXPR_PREFIX_INC OSC_EXPR_PREFIX_DEC OSC_EXPR_UPLUS OSC_EXPR_UMINUS '!'

// level 2
%left OSC_EXPR_INC OSC_EXPR_DEC OSC_EXPR_FUNC_CALL OPEN_DBL_BRKTS CLOSE_DBL_BRKTS

%%

expns: 
	| expns expr ';' {
	 	//osc_expr_setNext($2, *exprstack);
		//*exprstack = $2;
		if(*exprstack){
			/*
			t_osc_expr *tmp = *exprstack;
			while(osc_expr_next(tmp)){
				tmp = osc_expr_next(tmp);
			}
			osc_expr_setNext(tmp, $2);
			*/
			osc_expr_appendExpr(*exprstack, $2);
		}else{
			*exprstack = $2;
		}
 	}
;

args:   arg 
	| args ',' arg {
		osc_expr_arg_append($$, $3);
 	}
;

arg:    OSC_EXPR_NUM {
		$$ = osc_expr_arg_alloc();
		osc_expr_arg_setOSCAtom($$, $1);
 	}
	| OSC_EXPR_STRING {
		$$ = osc_expr_arg_alloc();
		int oscaddress = 0;
		if(osc_atom_u_getTypetag($1) == 's'){

			char *st = osc_atom_u_getStringPtr($1);
			if(st){
				if(*st == '/' && st[1] != '\0'){
					// this is an OSC address
					oscaddress = 1;
				}
			}
		}
		if(oscaddress){
			char *st = osc_atom_u_getStringPtr($1);
			int len = strlen(st) + 1;
			char *buf = osc_mem_alloc(len);
			memcpy(buf, st, len);
			osc_expr_arg_setOSCAddress($$, buf);
			osc_atom_u_free($1);
		}else{
			osc_expr_arg_setOSCAtom($$, $1);
		}
 	}
	| expr {
		$$ = osc_expr_arg_alloc();
		osc_expr_arg_setExpr($$, $1);
  	}
;

expr:   
	'(' expr ')' {
		$$ = $2;
  	}
// prefix function call
	| OSC_EXPR_STRING '(' args ')' %prec OSC_EXPR_FUNC_CALL {
		t_osc_expr *e = osc_expr_parser_prefix(osc_atom_u_getStringPtr($1), $3);
		if(!e){
			return 1;
		}
		$$ = e;
		osc_atom_u_free($1);
  	}
// infix operators
	| arg '+' arg {
		$$ = osc_expr_parser_infix("+", $1, $3);
 	}
	| arg '-' arg {
		$$ = osc_expr_parser_infix("-", $1, $3);
 	}
	| arg '*' arg {
		$$ = osc_expr_parser_infix("*", $1, $3);
 	}
	| arg '/' arg {
		$$ = osc_expr_parser_infix("/", $1, $3);
 	}
	| arg '%' arg {
		$$ = osc_expr_parser_infix("%", $1, $3);
 	}
	| arg '^' arg {
		$$ = osc_expr_parser_infix("^", $1, $3);
 	}
	| arg OSC_EXPR_EQ arg {
		$$ = osc_expr_parser_infix("==", $1, $3);
 	}
	| arg OSC_EXPR_NEQ arg {
		$$ = osc_expr_parser_infix("!=", $1, $3);
 	}
	| arg '<' arg {
		$$ = osc_expr_parser_infix("<", $1, $3);
 	}
	| arg OSC_EXPR_LTE arg {
		$$ = osc_expr_parser_infix("<=", $1, $3);
 	}
	| arg '>' arg {
		$$ = osc_expr_parser_infix(">", $1, $3);
 	}
	| arg OSC_EXPR_GTE arg {
		$$ = osc_expr_parser_infix(">=", $1, $3);
 	}
	| arg OSC_EXPR_AND arg {
		$$ = osc_expr_parser_infix("&&", $1, $3);
 	}
	| arg OSC_EXPR_OR arg {
		$$ = osc_expr_parser_infix("||", $1, $3);
 	}
	| arg OSC_EXPR_PLUSEQ arg {
		$$ = osc_expr_parser_infixAssign("+=", $1, $3);
 	}
	| arg OSC_EXPR_MINUSEQ arg {
		$$ = osc_expr_parser_infixAssign("-=", $1, $3);
 	}
	| arg OSC_EXPR_MULTEQ arg {
		$$ = osc_expr_parser_infixAssign("*=", $1, $3);
 	}
	| arg OSC_EXPR_DIVEQ arg {
		$$ = osc_expr_parser_infixAssign("/=", $1, $3);
 	}
	| arg OSC_EXPR_MODEQ arg {
		$$ = osc_expr_parser_infixAssign("%=", $1, $3);
 	}
	| arg OSC_EXPR_POWEQ arg {
		$$ = osc_expr_parser_infixAssign("^=", $1, $3);
 	}

// prefix not
	| '!' arg {
		$$ = osc_expr_alloc();
		osc_expr_setRec($$, osc_expr_lookupFunction("!"));
		osc_expr_setArg($$, $2);
	}
// prefix inc/dec
	| OSC_EXPR_INC OSC_EXPR_STRING %prec OSC_EXPR_PREFIX_INC {
		char *copy = NULL;
		osc_atom_u_getString($2, &copy);
		t_osc_expr *e = osc_expr_parser_prefix_incdec(copy, "++");
		if(!e){
			osc_mem_free(copy);
			osc_atom_u_free($2);
			return 1;
		}
		osc_atom_u_free($2);
		$$ = e;
	}
	| OSC_EXPR_DEC OSC_EXPR_STRING %prec OSC_EXPR_PREFIX_DEC {
		char *copy = NULL;
		osc_atom_u_getString($2, &copy);
		t_osc_expr *e = osc_expr_parser_prefix_incdec(copy, "--");
		if(!e){
			osc_mem_free(copy);
			osc_atom_u_free($2);
			return 1;
		}
		osc_atom_u_free($2);
		$$ = e;
	}
// postfix inc/dec
	| OSC_EXPR_STRING OSC_EXPR_INC {
		char *copy = NULL;
		osc_atom_u_getString($1, &copy);
		t_osc_expr *e = osc_expr_parser_postfix_incdec(exprstack, copy, "++");
		if(!e){
			osc_mem_free(copy);
			osc_atom_u_free($1);
			return 1;
		}
		osc_atom_u_free($1);
		$$ = e;
	}
	| OSC_EXPR_STRING OSC_EXPR_DEC {
		char *copy = NULL;
		osc_atom_u_getString($1, &copy);
		t_osc_expr *e = osc_expr_parser_postfix_incdec(exprstack, copy, "--");
		if(!e){
			osc_mem_free(copy);
			osc_atom_u_free($1);
			return 1;
		}
		osc_atom_u_free($1);
		$$ = e;
	}
// assignment
	| OSC_EXPR_STRING '=' arg {
		// basic assignment 
		char *ptr = NULL;
		osc_atom_u_getString($1, &ptr);
		if(*ptr != '/'){
			yyerror(exprstack, scanner, "osc_expr_parser: expected \"%s\" in \"%s = ... to be an OSC address\n", ptr, ptr);
			return 1;
		}
		t_osc_expr_arg *arg = osc_expr_arg_alloc();
		osc_expr_arg_setOSCAddress(arg, ptr);
		$$ = osc_expr_parser_infix("=", arg, $3);
		osc_expr_setAssignResultToAddress($$, 1);
		osc_atom_u_free($1);
 	}
	| OSC_EXPR_STRING '=' '[' args ']' {
		// assign a list of stuff
		char *ptr = osc_atom_u_getStringPtr($1);
		if(*ptr != '/'){
			yyerror(exprstack, scanner, "osc_expr_parser: expected \"%s\" in \"%s = ... to be an OSC address\n", ptr, ptr);
			return 1;
		}
		t_osc_expr_arg *arg = osc_expr_arg_alloc();
		osc_expr_arg_setOSCAddress(arg, ptr);
		$$ = osc_expr_parser_infix("=", arg, $4);
		osc_expr_setAssignResultToAddress($$, 1);
 	}
// shorthand constructions
	| '[' arg ':' arg ']' %prec OSC_EXPR_FUNC_CALL {
		// matlab-style range
		osc_expr_arg_append($2, $4);
		$$ = osc_expr_parser_prefix("range", $2);
 	}
	| '[' arg ':' arg ':' arg ']' %prec OSC_EXPR_FUNC_CALL {
		// matlab-style range
		osc_expr_arg_append($2, $6);
		osc_expr_arg_append($2, $4);
		$$ = osc_expr_parser_prefix("range", $2);
 	}
	| arg '?' arg ':' arg %prec OSC_EXPR_TERNARY_COND {
		// ternary conditional
		osc_expr_arg_append($1, $3);
		osc_expr_arg_append($1, $5);
		$$ = osc_expr_parser_prefix("if", $1);
  	}
	| OSC_EXPR_STRING OSC_EXPR_DBLQMARK arg {
		// null coalescing operator from C#.  
		// /foo ?? 10 means /foo if /foo is in the bundle, otherwise 10
		$$ = osc_expr_parser_nullCoalesce($1, $3);
		osc_atom_u_free($1); // the above function will copy that
	}
	| OSC_EXPR_STRING OSC_EXPR_DBLQMARKEQ arg {
		// null coalescing operator from C#.  
		// /foo ?? 10 means /foo if /foo is in the bundle, otherwise 10
		t_osc_expr *if_expr = osc_expr_parser_nullCoalesce($1, $3);
		if(!if_expr){
			return 1;
		}
		char *ptr = NULL;
		osc_atom_u_getString($1, &ptr);
		t_osc_expr_arg *arg = osc_expr_arg_alloc();
		osc_expr_arg_setOSCAddress(arg, ptr);
		t_osc_expr_arg *arg2 = osc_expr_arg_alloc();
		osc_expr_arg_setExpr(arg2, if_expr);
		$$ = osc_expr_parser_infix("=", arg, arg2);
		osc_expr_setAssignResultToAddress($$, 1);
		osc_atom_u_free($1);
	}
// array lookup
	| arg OPEN_DBL_BRKTS args CLOSE_DBL_BRKTS {
		osc_expr_arg_setNext($1, $3);
		$$ = osc_expr_parser_prefix("get_index", $1);
	}
;
