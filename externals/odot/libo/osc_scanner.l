%{
#include "osc_parser.h"

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
					// i wouldn't thin this use of strdup is necessary, but i was getting
					// a strange crash that this fixes, so it's in here for now
					double d = strtod(strdup(yytext), &endp);
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

\/[^ \t\n#]+	{
			//printf("OSC address!\n");
			yylval->string = strdup(yytext);
			return OSCADDRESS;
		}

[a-zA-Z0-9][^ \t\n]*	{
				//printf("string\n");
				yylval->string = strdup(yytext);
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
