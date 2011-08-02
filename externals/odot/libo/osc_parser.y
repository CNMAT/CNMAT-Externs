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
#include "osc_parser.h"
#include "osc_scanner.h"

	//#define OSC_PARSER_DEBUG
#ifdef OSC_PARSER_DEBUG
#define PP(fmt, ...)printf(fmt, ##__VA_ARGS__)
#else
#define PP(fmt, ...)
#endif
%}

%code requires{
#ifndef __HAVE_OSC_PARSER_SUBST__
#define __HAVE_OSC_PARSER_SUBST__
#include "osc.h"
typedef struct _osc_parser_subst{
	int listitem;
	t_osc_msg *msg;
	int item_to_replace; // 0 = address
	struct _osc_parser_subst *next;
} t_osc_parser_subst;
t_osc_err osc_parser_parseString(long len, char *ptr, t_osc_bundle **bndl, t_osc_parser_subst **subs);
#endif
}

%{

int osc_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner);
int osc_parser_lex(YYSTYPE *yylval_param, yyscan_t yyscanner){
	return osc_scanner_lex(yylval_param, yyscanner);
}

t_osc_err osc_parser_parseString(long len, char *ptr, t_osc_bundle **bndl, t_osc_parser_subst **subs){
	yyscan_t scanner;
	osc_scanner_lex_init(&scanner);
	YY_BUFFER_STATE buf_state = osc_scanner__scan_string(ptr, scanner);
	osc_scanner_set_out(NULL, scanner);
	t_osc_msg *msg = NULL;
	osc_parser_parse(bndl, &msg, subs, scanner);
	osc_scanner__delete_buffer(buf_state, scanner);
	osc_scanner_lex_destroy(scanner);
}

void yyerror (t_osc_bundle **bndl, t_osc_msg **msg, t_osc_parser_subst **subs, void *scanner, char const *e);
void yyerror (t_osc_bundle **bndl, t_osc_msg **msg, t_osc_parser_subst **subs, void *scanner, char const *e){
	printf("error, bitches: %s\n", e);
}

void osc_parser_substitution(t_osc_parser_subst **subs_list, int listitem, t_osc_msg *msg, int item_to_replace){
	t_osc_parser_subst *s = (t_osc_parser_subst *)osc_mem_alloc(sizeof(t_osc_parser_subst));
	s->listitem = listitem;
	s->msg = msg;
	s->item_to_replace = item_to_replace;
	s->next = *subs_list;
	*subs_list = s;
}

%}

%define "api.pure"

%parse-param{t_osc_bundle **bndl}
%parse-param{t_osc_msg **msg}
%parse-param{t_osc_parser_subst **subs}
%parse-param{void *scanner}
%lex-param{void *scanner}

%union {
	double f;
	int32_t i;
	char *string;
	struct _osc_msg *msg;
}

%token <f>OSCFLOAT 
%token <i>OSCINT DOLLARSUB OSCADDRESS_DOLLARSUB
%token <string>STRING OSCADDRESS 

 //%token ARG

%type <msg>arglist msg

%%

bundle: {
		t_osc_bundle *b = osc_bundle_alloc();
		PP("push BNDL %p->%p\n", b, *bndl);
		b->next = *bndl;
		*bndl = b;
	}
	| bundle msg {
		PP("pop MSG %p<-%p\n", *msg, (*msg)->next);
		PP("add MSG to BNDL %p := %p\n", *bndl, *msg);
		t_osc_msg *m = (*msg)->next;
		(*msg)->next = NULL;
		osc_bundle_addMessage(*bndl, *msg);
		*msg = m;
	}
;

arglist: '\n' {;}
	| STRING {
		t_osc_msg *m = osc_message_alloc();
		PP("push MSG %p->%p\n", m, *msg);
		PP("add STRING to MSG %p := %s\n", m, $1);
		m->next = *msg;
		*msg = m;
		osc_message_addData(*msg, 1, "s", strlen($1), $1);
  	}
	| OSCADDRESS {
		t_osc_msg *m = osc_message_alloc();
		PP("push MSG %p->%p\n", m, *msg);
		PP("add OSCADDRESS to MSG %p := %s\n", m, $1);
		m->next = *msg;
		*msg = m;
		osc_message_addData(*msg, 1, "s", strlen($1), $1);
	}
	| OSCFLOAT {
		t_osc_msg *m = osc_message_alloc();
		PP("push MSG %p->%p\n", m, *msg);
		PP("add OSCFLOAT to MSG %p := %f\n", m, $1);
		m->next = *msg;
		*msg = m;
		float f = $1;
		osc_message_addData(*msg, 1, "f", 4, (char *)&f);
	}
	| OSCINT {
		t_osc_msg *m = osc_message_alloc();
		PP("push MSG %p->%p\n", m, *msg);
		PP("add OSCINT to MSG %p := %d\n", m, $1);
		m->next = *msg;
		*msg = m;
		osc_message_addData(*msg, 1, "i", 4, (char *)&$1);
	}
	| DOLLARSUB {
		t_osc_msg *m = osc_message_alloc();
		PP("push MSG %p->%p\n", m, *msg);
		PP("add DOLLARSUB to MSG %p := %d\n", m, $1);
		m->next = *msg;
		*msg = m;
		char buf[8];
		sprintf(buf, "$%d", $1);
		osc_message_addData(*msg, 1, "s", strlen(buf), buf);
		//osc_parser_substitution(subs, $1, *msg, (*msg)->argc);
	}
	| arglist STRING {
		PP("add STRING to MSG %p := %s\n", *msg, $2);
		osc_message_addData(*msg, 1, "s", strlen($2), $2);
 	}
	| arglist OSCADDRESS {
		PP("add OSCADDRESS to MSG %p := %s\n", *msg, $2);
		osc_message_addData(*msg, 1, "s", strlen($2), $2);
 	}
	| arglist OSCFLOAT {
		PP("add OSCFLOAT to MSG %p := %f\n", *msg, $2);
		float f = $2;
		osc_message_addData(*msg, 1, "f", 4, (char *)&f);
 	}
	| arglist OSCINT {
		PP("add OSCINT to MSG %p := %d\n", *msg, $2);
		osc_message_addData(*msg, 1, "i", 4, (char *)&$2);
 	}
	| arglist DOLLARSUB {
		PP("add DOLLARSUB to MSG %p := %d\n", *msg, $2);
		char buf[8];
		sprintf(buf, "$%d", $2);
		osc_message_addData(*msg, 1, "s", 4, buf);
		//osc_parser_substitution(subs, $2, *msg, (*msg)->argc);
 	}
	| '[' '\n' bundle ']' {
		long slen = 0;
		osc_bundle_getLen_s(*bndl, &slen);
		char sbuf[slen + 4];
		*((uint32_t *)sbuf) = hton32(slen);
		osc_bundle_serializeWithBuffer(*bndl, sbuf + 4);
		osc_message_addData(*msg, 1, "#", slen + 4, sbuf);
		t_osc_bundle *b = (*bndl)->next;
		osc_bundle_freeBundle(*bndl);
		*bndl = b;
	}
	| arglist '[' '\n' bundle ']' {
		long slen = 0;
		osc_bundle_getLen_s(*bndl, &slen);
		char sbuf[slen + 4];
		*((uint32_t *)sbuf) = hton32(slen);
		osc_bundle_serializeWithBuffer(*bndl, sbuf + 4);
		osc_message_addData(*msg, 1, "#", slen + 4, sbuf);
		t_osc_bundle *b = (*bndl)->next;
		osc_bundle_freeBundle(*bndl);
		*bndl = b;
	}
;

msg: 
	OSCADDRESS arglist '\n' {
		PP("set ADDRESS %p := %s\n", *msg, $1);
		osc_message_setAddress(*msg, $1);
 	}
	| OSCADDRESS_DOLLARSUB arglist '\n' {
		char buf[8];
		sprintf(buf, "/$%d", $1);
		PP("set ADDRESS %p := %s\n", *msg, buf);
		osc_message_setAddress(*msg, buf);
		//osc_parser_substitution(subs, $1, *msg, 0);
 	}
;
