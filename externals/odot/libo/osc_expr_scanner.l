%{
#include "osc_expr.h"
#include "osc_expr_parser.h"

int osc_expr_scanner_wrap(yyscan_t scanner){
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
\+|-|\*|\/|%|^|! {
	return *yytext;
}
\<= {
	return OSC_EXPR_LTE;
}
>= {
	return OSC_EXPR_GTE;
}
!= {
	return OSC_EXPR_NEQ;
}
== {
	return OSC_EXPR_EQ;
}
\&\& {
	return OSC_EXPR_AND;
}
\|\| {
	return OSC_EXPR_OR;
}
\+\+ {
	return OSC_EXPR_INC;
}
\-\- {
	return OSC_EXPR_DEC;
}
\+= {
	return OSC_EXPR_PLUSEQ;
}
\-= {
	return OSC_EXPR_MINUSEQ;
}
\*= {
	return OSC_EXPR_MULTEQ;
}
\/= {
	return OSC_EXPR_DIVEQ;
}
%= {
	return OSC_EXPR_MODEQ;
}
^= {
	return OSC_EXPR_POWEQ;
}

,|;|\(|\)				{
						return *yytext;
					}
\[\[					{
						//printf("open double brackets\n");
						return OPEN_DBL_BRKTS;
					}
\]\]					{
						//printf("close double brackets\n");
						return CLOSE_DBL_BRKTS;
					}
(-?([[:digit:]]{-}[0])[[:digit:]]*|0) 	{
						// int 
						char *endptr = NULL;
						int32_t i = strtol(yytext, &endptr, 0);
						yylval->atom = osc_atom_u_alloc();
						osc_atom_u_setInt32(yylval->atom, i);
						return OSC_EXPR_NUM;
					}
-?{c99_floating_point_constant}		{
						// floating point number
						char *endptr = NULL;
						double f = strtod(yytext, &endptr);
						yylval->atom = osc_atom_u_alloc();
						osc_atom_u_setDouble(yylval->atom, f);
						return OSC_EXPR_NUM;
					}
[^0-9\(\)\[\] \t\n\+\-\*<>=!,;][^\(\)\[\] \t\n\+\-<>=!,;]*		{
						// put in atom
						yylval->atom = osc_atom_u_alloc();
						osc_atom_u_setString(yylval->atom, yytext);
						return OSC_EXPR_STRING;
					}
[ \t\n]+	/* eat up whitespace */

%%
