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
	//#define YYLTYPE t_omax_func_arg*

//void omax_parser_addfunc_2arg(t_omax_func_ptr *f, t_

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp);
 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_func **f, char const *e);

 void yyerror(int argc, t_atom *argv, int *argp, struct _omax_func **f, char const *e){
	printf("i'm here, but i really wish i wasn't...\n");
	printf("%s\n", e);
 }

 int yylex(YYSTYPE *lvalp, int argc, t_atom *argv, int *argp){
	printf("%s\n", __func__);
	 //printf("%s: lvalp = %p, argc = %p\n", __func__, lvalp, *lvalp, argv);
	 int token = 0;
	 if(*argp < argc){
		 switch(atom_gettype(argv + *argp)){
		 case A_LONG:
			 {
			 	printf("%s: argv[%d] = %ld\n", __func__, *argp, atom_getlong(argv + *argp));
			 	token = ARG;
				t_omax_func_arg *arg = (t_omax_func_arg *)osc_mem_alloc(sizeof(t_omax_func_arg));
				arg->type = OMAX_ARG_TYPE_ATOM;
				printf("arg = %p\n", arg);
				arg->arg.atom = *(argv + (*argp));
				lvalp->arg = arg;
			 }
			 break;
		 case A_FLOAT:
			 {
			 	printf("%s: argv[%d] = %f\n", __func__, *argp, atom_getfloat(argv + *argp));
			 	token = ARG;
				t_omax_func_arg *arg = (t_omax_func_arg *)osc_mem_alloc(sizeof(t_omax_func_arg));
				arg->type = OMAX_ARG_TYPE_ATOM;
				printf("arg = %p\n", arg);
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
					 t_omax_func_arg *arg = (t_omax_func_arg *)osc_mem_alloc(sizeof(t_omax_func_arg));
					 arg->type = OMAX_ARG_TYPE_OSCADDRESS;
					 arg->arg.osc_address = atom_getsym(argv + (*argp))->s_name;
					 lvalp->arg = arg;
				 }else if(strlen(ptr) == 1){
					 switch(*ptr){
					 case '+':
						 {
						 	token = PLUS;
							t_omax_func *func = (t_omax_func *)osc_mem_alloc(sizeof(t_omax_func));
							memset(func, '\0', sizeof(func));
							printf("func = %p\n", func);
							func->f = omax_parser_plus;
							lvalp->func = func;
						 }
						 
					 }
				 }
			 }
		 }
	 }
	 *argp += 1;
	 printf("%s: returning token %d\n", __func__, token);
	 return token;
 }

 %}

 %define "api.pure"

 %parse-param{int argc}
 %parse-param{t_atom *argv}
 %parse-param{int *argp}
 %parse-param{t_omax_func **f}
 %lex-param{int argc}
 %lex-param{t_atom *argv}
 %lex-param{int *argp}

%union {
	struct _omax_func *func;
	struct _omax_func_arg *arg;
	t_symbol sym;
}

%token <arg>ARG
%token <func>PREFIX_FUNC
%token <sym>LPAREN RPAREN COMMA
%token <func>PLUS MINUS MULTIPLY DIVIDE POWER AND OR EQ NEQ LT LTE GT GTE
%type <arg>expn

%left PLUS MINUS MULTIPLY DIVIDE AND OR EQ NEQ LT LTE GT GTE
%right POWER    
%% 

input:
| input expn
;

expn:	ARG {printf("first rule arg = %p %p\n", $$, $1);}
|	expn PLUS expn	{
	//t_omax_func_arg *argv = (t_omax_func_arg *)osc_mem_alloc(2 * sizeof(t_omax_func_arg));
	//*argv = *$1;
			//(argv + 1)->arg.atom = *$3;
			//(argv + 1)->type = OMAX_ARG_TYPE_ATOM;
			//*argv = *$3;
			//t_omax_func *func = (t_omax_func *)osc_mem_alloc(sizeof(t_omax_func));
			//func->f = omax_parser_plus;
			//func->argc = 2;
			//func->argv = argv;
			//printf("%s: func = %p\n", __func__, func);
	printf("%p %p %p\n", $1, $2, $3);
			t_omax_func *func = $2;
			$1->next = $3;
			func->argv = $1;
			func->argc = 2;
			t_omax_func *funclist = *f;
			if(funclist){
				while(funclist->next){
					funclist = funclist->next;
				}
				funclist->next = func;
				func->next = NULL;
				func->prev = funclist;
			}else{
				*f = func;
				func->next = NULL;
				func->prev = NULL;
			}
			$$->arg.func = func;
			$$->type = OMAX_ARG_TYPE_FUNC;
 		}
;

