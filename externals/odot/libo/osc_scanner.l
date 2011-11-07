%{
#include "osc_parser.h"
#include "osc_mem.h"

int osc_scanner_wrap(yyscan_t scanner){
	return 1;
}

%}

dseq      ([[:digit:]]+)
dseq_opt  ([[:digit:]]*)
frac      (({dseq_opt}"."{dseq})|{dseq}".")
exp       ([eE][+-]?{dseq})
exp_opt   ({exp}?)
fsuff     [flFL]
fsuff_opt ({fsuff}?)
hpref     (0[xX])
hdseq     ([[:xdigit:]]+)
hdseq_opt ([[:xdigit:]]*)
hfrac     (({hdseq_opt}"."{hdseq})|({hdseq}"."))
bexp      ([pP][+-]?{dseq})
dfc       (({frac}{exp_opt}{fsuff_opt})|({dseq}{exp}{fsuff_opt}))
hfc       (({hpref}{hfrac}{bexp}{fsuff_opt})|({hpref}{hdseq}{bexp}{fsuff_opt}))
c99_floating_point_constant  ({dfc}|{hfc})

%option reentrant
%option bison-bridge
%option bison-locations

%%

$[1-9]+	{
		//printf("sub\n");
		char *endp = NULL;
		yylval->i = strtol(yytext + 1, &endp, 0);
		return DOLLARSUB;
	}

-?{c99_floating_point_constant}	{
					//printf("float\n");
					char *endp = NULL;
					double d = strtod(yytext, &endp);
					yylval->f = d;
					return OSCFLOAT;
				}

\[|\]	{
		return *yytext;
	}

\n	{
		return *yytext;
	}


(-?([[:digit:]]{-}[0])[[:digit:]]*|0) 	{
						//printf("int\n");
						char *endp = NULL;
						int32_t i = strtol(yytext, &endp, 0);
						yylval->i = i;
						return OSCINT;
					}

\/[^ \t\n#\"]+	{
			//printf("OSC address: %s\n", yytext);
			int len = strlen(yytext) + 1;
			char *copy = osc_mem_alloc(len);
			memcpy(copy, yytext, len);
			yylval->string = copy;
			return OSCADDRESS;
		}

\"(\\.|[^\"])*\" 	{
				//printf("quoted string: %s\n", yytext);
				int len = strlen(yytext) - 1;
				char *copy = osc_mem_alloc(len);
				memcpy(copy, yytext + 1, len - 1);
				yylval->string = copy;
				return STRING;
			}

			/* [a-zA-Z0-9][^ \t\n]* */
[^ \t\"\n]*	{
				//printf("string: %s\n", yytext);
				int len = strlen(yytext) + 1;
				char *copy = osc_mem_alloc(len);
				memcpy(copy, yytext, len);
				yylval->string = copy;
				return STRING;
			}


[ \t]+



%%
//\xc2\xac\n
/*
int main(int argc, char **argv){
	yyscan_t scanner;
	char buf[128];
	yylex_init(&scanner);
	YY_BUFFER_STATE buf_state = yy_scan_string("/foo bar\n /bar foo\n", scanner);
	yyset_out(NULL, scanner);
	int ret = 1;
	//while(ret){
		//ret = yylex(scanner);
		//printf("foo: %s\n", yyget_text(scanner));
	//}

	//buf_state = yy_scan_string("/foo bar\n /bar foo\n", scanner);
	while(yylex(scanner)){
		//printf("foo: %s\n", yyget_text(scanner));
	}
	yy_delete_buffer(buf_state, scanner);
	yylex_destroy(scanner);
}
*/
