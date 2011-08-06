/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 2132 of yacc.c  */
#line 26 "osc_parser.y"

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



/* Line 2132 of yacc.c  */
#line 54 "osc_parser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OSCFLOAT = 258,
     OSCINT = 259,
     DOLLARSUB = 260,
     OSCADDRESS_DOLLARSUB = 261,
     STRING = 262,
     OSCADDRESS = 263
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2132 of yacc.c  */
#line 82 "osc_parser.y"

	double f;
	int32_t i;
	char *string;
	struct _osc_msg *msg;



/* Line 2132 of yacc.c  */
#line 88 "osc_parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




