%{
#include <Carbon.h>
#include <CoreServices.h>
#include "ext.h"
#include "ext_obex.h"
#include "ext_obex_util.h"
#include "osc.h"
void omax_scanner_scan_atom_array(int argc, t_atom *argv, int *argc_out, t_atom **argv_out);
%}

%option reentrant
%option extra-type="t_atom **"
%option prefix="omax_scanner_"
%option noyywrap

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

infixop		\+\-\*\/<>=!
infixop_or	  \+|\-|\*|\/|<|>|<=|>=|!=

%x DOUBLE_BRACKETS

%%
\(					{
						/* parens */
						atom_setsym(*yyextra, gensym("("));
						(*yyextra)++;
						//printf("open paren\n");
					}
\)					{
						atom_setsym(*yyextra, gensym(")"));
						(*yyextra)++;
						//printf("open paren\n");
					}
<DOUBLE_BRACKETS>\[\[					{
						//printf("open double brackets\n");
						BEGIN(INITIAL);
						atom_setsym(*yyextra, gensym("[["));
						(*yyextra)++;
					}
<DOUBLE_BRACKETS>\]\]					{
						//printf("close double brackets\n");
						BEGIN(INITIAL);
						atom_setsym(*yyextra, gensym("]]"));
						(*yyextra)++;
					}
(-?([[:digit:]]{-}[0])[[:digit:]]*|0) 	{
						/* int */
						char *endptr = NULL;
						atom_setlong(*yyextra, strtol(yytext, &endptr, 0));
						(*yyextra)++;
					}
-?{c99_floating_point_constant}		{
						/* floating point number */
						char *endptr = NULL;
						atom_setfloat(*yyextra, strtod(yytext, &endptr));
						(*yyextra)++;
						//printf("float: %f\n", f);
					}
[^0-9\(\) \t\n\+\-\*<>=!,][^\(\) \t\n\+\-\*<>=!,]*		{
						int i;
						int b = 0;
						for(i = 0; i < yyleng; i++){
							if(yytext[i] == '[' || yytext[i] == ']'){
								b++;
							}else{
								b = 0;
							}
							if(b == 2){
								break;
							}
						}
						if(b < 2){
							// string 
							atom_setsym(*yyextra, gensym(yytext));
							(*yyextra)++;
						}else{
							i--;
							if(i > 0){
								char buf[i];
								memcpy(buf, yytext, i);
								buf[i] = '\0';
								atom_setsym(*yyextra, gensym(buf));
								(*yyextra)++;
							}
							char brackets[3];
							brackets[0] = yytext[i];
							brackets[1] = yytext[i + 1];
							brackets[2] = '\0';
							atom_setsym(*yyextra, gensym(brackets));
							(*yyextra)++;
							yyless(i + 2);
							//BEGIN(DOUBLE_BRACKETS);
						}
					}
\+|\-|\*|\/|<|>|<=|>=|!=|==|%|\|\||&&|!     			{
						/* infix operator */
						atom_setsym(*yyextra, gensym(yytext));
						(*yyextra)++;
					}
,					{
						/* tiny little comma */
						atom_setsym(*yyextra, gensym(","));
						(*yyextra)++;
					}
[ \t\n]+	/* eat up whitespace */

%%
/*
int main(int argc, char **argv){
	++argv, --argc;
	yyin = stdin;
	yylex();
}
*/

void omax_scanner_scan_atom_array(int argc, t_atom *argv, int *argc_out, t_atom **argv_out){
	yyscan_t scanner;
	t_atom buf[512];
	t_atom *bufptr = buf;
	int i;
	for(i = 0; i < argc; i++){
		if(atom_gettype(argv + i) == A_SYM){
			yylex_init_extra(&bufptr, &scanner);
			YY_BUFFER_STATE myybuf = yy_scan_string(atom_getsym(argv + i)->s_name, scanner);
			yylex(scanner);
			yy_delete_buffer(myybuf, scanner);
			yylex_destroy(scanner);
		}else{
			*bufptr++ = *(argv + i);
		}
	}
	if(*argv_out){
		if(bufptr - buf > *argc_out){
			memcpy(*argv_out, buf, *argc_out * sizeof(t_atom));
		}else{
			memcpy(*argv_out, buf, (bufptr - buf) * sizeof(t_atom));
		}
	}else{
		t_atom *tmp = (t_atom *)osc_mem_alloc((bufptr - buf) * sizeof(t_atom));
		memcpy(tmp, buf, (bufptr - buf) * sizeof(t_atom));
		*argv_out = tmp;
	}
	*argc_out = bufptr - buf;
}

/*
\/[^\/]+.*\[\[([0-9]+|\/.*)\]\]		{
						// double brackets
						atom_setsym(*yyextra, gensym("get_index"));
						(*yyextra)++;
						atom_setsym(*yyextra, gensym("("));
						(*yyextra)++;

						char *endptr = yytext + 1;
						while(*endptr != '[' && endptr[-1] != '['){
							endptr++;
						}
						char buf[(endptr - yytext)];
						strncpy(buf, yytext, (endptr - yytext));
						buf[(endptr - yytext)] = '\0';
						atom_setsym(*yyextra, gensym(buf));
						(*yyextra)++;
						atom_setsym(*yyextra, gensym(","));
						(*yyextra)++;
						int len = (endptr - yytext) + 2;
						if(*(yytext + len) == '/'){
							while(*endptr != ']' && *(endptr + 1) != ']'){
								endptr++;
							}
							int len2 = endptr - (yytext + len) + 1;
							char buf2[len2];
							strncpy(buf2, yytext + len, len2);
							buf2[len2] = '\0';
							atom_setsym(*yyextra, gensym(buf2));
						}else{
							endptr = NULL;
							atom_setlong(*yyextra, strtol(yytext + len, &endptr, 0));
						}
						(*yyextra)++;
						atom_setsym(*yyextra, gensym(")"));
						(*yyextra)++;
					}
*/


/*
[^0-9\(\) \t\n\+\-\*<>=!,][^\(\) \t\n\+\-\*<>=!,]*		{
						// string 
						atom_setsym(*yyextra, gensym(yytext));
						(*yyextra)++;
						printf("string: %s\n", yytext);
					}
*/
